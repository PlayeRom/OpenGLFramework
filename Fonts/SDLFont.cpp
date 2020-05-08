// SDLFont.cpp: implementation of the CSDLFont class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../Extensions/ARBMultiTexturing.h"
#include "../OpenGLFramework/WindowData.h"
#include "../OpenGLFramework/GameControl.h"
#include "../Logger/Logger.h"
#include "SDLFont.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

GLint CSDLFont::ms_iCountRef = 0;
std::vector<CSDLFont*> g_aPointerSDLFont;

CSDLFont::CSDLFont( const char *lpFontFile, GLint iSizeFont, GLboolean bUseStorageBuffer )
{
	m_pWinData = CWindowData::GetInstance();
	m_bUseStorageBuffer = bUseStorageBuffer;
	
	m_iID = ms_iCountRef;
	++ms_iCountRef;

	m_iHeightFont = -1;
	m_bDrawBkgText = GL_FALSE;
	m_uiCountTimer = 0;
	m_uiInterval = 30;		//sekundy co ile czyscic stare bufory
	m_iBufferLimit = 200;	//limit buforowania tekstów
	m_uiTimeOut = 30;		//sekundy przedawnienia bufora

	m_pTTFFont = NULL;
	if( !TTF_WasInit() ) {
		if( TTF_Init() == -1 )
			__LOG_FORMATTED( "ERROR: CSDLFont::CSDLFont - Initialization SDLFont failed: %s.", TTF_GetError() );
	}

	m_pTTFFont = TTF_OpenFont( lpFontFile, iSizeFont );
	if( !m_pTTFFont ) {
		__LOG_FORMATTED( "ERROR: CSDLFont::CSDLFont - TTF_OpenFont failed: %s.", TTF_GetError() );

		//nie udalo sie wczytac czcionki wiec ladujemy standardowa, zawsze dostepna wraz z frameworkiem
		m_pTTFFont = TTF_OpenFont( "fonts/FreeSansBold.ttf", iSizeFont );
		if( !m_pTTFFont )
			__LOG_FORMATTED( "ERROR: CSDLFont::CSDLFont - TTF_OpenFont failed again: %s.", TTF_GetError() );
	}

	if( m_pTTFFont )
		m_iHeightFont = TTF_FontHeight( m_pTTFFont );

	g_aPointerSDLFont.push_back( this );

	m_aTexCoord = new CVector2[ 4 ];
	m_aTexCoord[ 0 ] = CVector2( 1.0f, 1.0f );
	m_aTexCoord[ 1 ] = CVector2( 1.0f, 0.0f );
	m_aTexCoord[ 2 ] = CVector2( 0.0f, 1.0f );
	m_aTexCoord[ 3 ] = CVector2( 0.0f, 0.0f );

	m_aIndices = new CVector2[ 4 ];
}

CSDLFont::~CSDLFont()
{
	delete [] m_aTexCoord;
	delete [] m_aIndices;

	ClearStorageBuffer();

	--ms_iCountRef;
	g_aPointerSDLFont[ m_iID ] = NULL;

	if( m_pTTFFont )
		TTF_CloseFont( m_pTTFFont );
	m_pTTFFont = NULL;

	if( ms_iCountRef == 0 ) {
		//usuwany jest ostatni obiekt CSDLFont
		if( TTF_WasInit() ) 
			TTF_Quit();
		g_aPointerSDLFont.clear();
	}
}

//******************************* interface *************************************
GLvoid CSDLFont::DrawText( GLint iPosX, GLint iPosY, COLORREF crColor, LPCTSTR lpText, GLboolean bMidScreen )
{
	if( !m_pTTFFont || lpText[ 0 ] == NULL )
		return;

#if !_USE_2D_ONLY_
	CGameControl::GetInstance()->Enable2D();
#endif

	glDisable( GL_DEPTH_TEST );

	SDL_Color sdlColor;
	SDL_Rect sdlRect;

	sdlColor.r = GetBValue( crColor );
	sdlColor.g = GetGValue( crColor );
	sdlColor.b = GetRValue( crColor );

	sdlRect.x = static_cast<Sint16>( iPosX );
	// Gdy odejmiemy wysokosc czcionki to baza pozycji tektu bedzie dolna pozycja
	// czyli jak podamy iPosY = 0 to tek nie bedzie widoczny, bo bootom bedzie = 0 a nie top.
	// Zato tekst bedzie widoczny gdy iPosY = 768 - wtedy tekst bedzie dopasowany do dolnej krawedzi ekranu.
	sdlRect.y = static_cast<Sint16>( iPosY - m_iHeightFont );

	RenderText( lpText, m_pTTFFont, sdlColor, sdlRect, bMidScreen );

#if !_USE_2D_ONLY_
	CGameControl::GetInstance()->Disable2D();
#endif

	glEnable( GL_DEPTH_TEST );
}

GLvoid CSDLFont::DrawText( const RECT &rcRectangle, COLORREF crColor, LPCTSTR lpText )
{
	if( !m_pTTFFont || lpText[ 0 ] == NULL )
		return;

#if !_USE_2D_ONLY_
	CGameControl::GetInstance()->Enable2D();
#endif

	glDisable( GL_DEPTH_TEST );

	SDL_Color sdlColor;
	SDL_Rect sdlRect;

	sdlColor.r = GetBValue( crColor );
	sdlColor.g = GetGValue( crColor );
	sdlColor.b = GetRValue( crColor );

	//wazna kolejnosc
	sdlRect.h = static_cast<Sint16>( rcRectangle.bottom );
	sdlRect.w = static_cast<Sint16>( rcRectangle.left );
	sdlRect.x = static_cast<Sint16>( rcRectangle.right );
	sdlRect.y = static_cast<Sint16>( rcRectangle.top );

	RenderText( lpText, m_pTTFFont, sdlColor, sdlRect, GL_FALSE, GL_TRUE );

#if !_USE_2D_ONLY_
	CGameControl::GetInstance()->Disable2D();
#endif

	glEnable( GL_DEPTH_TEST );
}

GLvoid CSDLFont::DrawTextFormat( GLint iPosX, GLint iPosY, GLboolean bMidScreen, COLORREF crColor, LPCTSTR fmt, ... )
{
	if( !m_pTTFFont || fmt[ 0 ] == NULL )
		return;

	TCHAR cText[ 256 ];
	va_list	ap;

	va_start( ap, fmt );
	_vstprintf_s( cText, 256, fmt, ap );
	va_end( ap );

#if !_USE_2D_ONLY_
	CGameControl::GetInstance()->Enable2D();
#endif

	glDisable( GL_DEPTH_TEST );

	SDL_Color sdlColor;
	SDL_Rect sdlRect;

	sdlColor.r = GetBValue( crColor );
	sdlColor.g = GetGValue( crColor );
	sdlColor.b = GetRValue( crColor );

	sdlRect.x = static_cast<Sint16>( iPosX );
	// Gdy odejmiemy wysokosc czcionki to baza pozycji tektu bedzie dolna pozycja
	// czyli jak podamy iPosY = 0 to tek nie bedzie widoczny, bo bootom bedzie = 0 a nie top.
	// Zato tekst bedzie widoczny gdy iPosY = 768 - wtedy tekst bedzie dopasowany do dolnej krawedzi ekranu.
	sdlRect.y = static_cast<Sint16>( iPosY - m_iHeightFont );

	RenderText( cText, m_pTTFFont, sdlColor, sdlRect, bMidScreen );

#if !_USE_2D_ONLY_
	CGameControl::GetInstance()->Disable2D();
#endif

	glEnable( GL_DEPTH_TEST );
}

GLvoid CSDLFont::DrawTextFormat( const RECT &rcRectangle, COLORREF crColor, LPCTSTR fmt, ... )
{
	if( !m_pTTFFont || fmt[ 0 ] == NULL )
		return;

	TCHAR cText[ 256 ];
	va_list	ap;

	va_start( ap, fmt );
	_vstprintf_s( cText, 256, fmt, ap );
	va_end( ap );

#if !_USE_2D_ONLY_
	CGameControl::GetInstance()->Enable2D();
#endif

	glDisable( GL_DEPTH_TEST );

	SDL_Color sdlColor;
	SDL_Rect sdlRect;

	sdlColor.r = GetBValue( crColor );
	sdlColor.g = GetGValue( crColor );
	sdlColor.b = GetRValue( crColor );

	//wazna kolejnosc
	sdlRect.h = static_cast<Sint16>( rcRectangle.bottom );
	sdlRect.w = static_cast<Sint16>( rcRectangle.left );
	sdlRect.x = static_cast<Sint16>( rcRectangle.right );
	sdlRect.y = static_cast<Sint16>( rcRectangle.top );

	RenderText( cText, m_pTTFFont, sdlColor, sdlRect, GL_FALSE, GL_TRUE );

#if !_USE_2D_ONLY_
	CGameControl::GetInstance()->Disable2D();
#endif

	glEnable( GL_DEPTH_TEST );
}

GLint CSDLFont::GetHeightFont()
{
	if( m_iHeightFont > -1 ) 
		return m_iHeightFont;

	if( !m_pTTFFont )
		return -1;

	m_iHeightFont = TTF_FontHeight( m_pTTFFont );
	return m_iHeightFont;
}

GLvoid CSDLFont::SetBkgText( GLboolean bDrawBkgText, COLORREF crColor )
{
	m_bDrawBkgText = bDrawBkgText;
	if( m_bDrawBkgText ) {
		m_ubBkgR = GetRValue( crColor );
		m_ubBkgG = GetGValue( crColor );
		m_ubBkgB = GetBValue( crColor );
	}
}

//*******************************************************************************

GLvoid CSDLFont::CalculatePosTextFromRect( SDL_Rect &in_out_sdlLocation, GLint iIndex )
{
	//wyliczamy pozycje tekstu z rcRectangle, gdzie tekst musi byc dokladnie
	//na srodku rcRectangle. Zalozeniem jest, ze tekst miesci sie w rcRectangle
	//np. rcRectangle jest przyciskiem a tekst napisem na przycisku
	//Kolejne zalozenie, ze zmienne rcRectangle zawieraja pozycje w pikselach
	//dla rozdzielczosci ekranu 1024x768 wyliczone z procentow,
	//tj. funkcjami CGameControl::SetRegionX itp. z parametrem bFont = GL_TRUE
	//Kolejne zalozenie, ze top jest wieksza wartoscia - dla Y, zero jest na dole ekranu.
	//wazna kolejnosc:
	//in_out_sdlLocation.h = static_cast<Sint16>( rcRectangle.bottom );
	//in_out_sdlLocation.w = static_cast<Sint16>( rcRectangle.left );
	//in_out_sdlLocation.x = static_cast<Sint16>( rcRectangle.right );
	//in_out_sdlLocation.y = static_cast<Sint16>( rcRectangle.top );
	Sint16 width = in_out_sdlLocation.x - in_out_sdlLocation.w; //right - left
	Sint16 height = in_out_sdlLocation.y - in_out_sdlLocation.h; //top - bottom

	//nowa pozycja tekstu
	in_out_sdlLocation.x = ( in_out_sdlLocation.w + ( width / 2 ) ) - static_cast<Sint16>( m_aStorageBuffer[ iIndex ].iOriginalWidth / 2 );
	in_out_sdlLocation.y = static_cast<Sint16>( m_pWinData->GetDesktopHeight() )
						- ( static_cast<Sint16>( m_pWinData->GetDesktopHeight() )
						- ( ( in_out_sdlLocation.h + ( height / 2 ) ) - static_cast<Sint16>( m_aStorageBuffer[ iIndex ].iOriginalHeight / 2 ) ) );
}

inline GLint CSDLFont::NextPowerOfTwo( GLint iNumber )
{
	GLint iPowerOfTwo = 2;
	while( iPowerOfTwo < iNumber )
		iPowerOfTwo <<= 1;

	return iPowerOfTwo;
}

GLvoid CSDLFont::ScaleFont()
{
	//skalowanie wzgledem rozdzielczosci, baza 1024x768

	GLfloat fScaleX = 0.0f;
	GLfloat fScaleY = 0.0f;

	if( m_pWinData->SettingFile().iWidth < 1024 ) // zmniejszamy fonta
		fScaleX = 1.0f / ( 1024.0f / static_cast< GLfloat >( m_pWinData->SettingFile().iWidth ) );
	else // zwieksz fonta
		fScaleX = static_cast< GLfloat >( m_pWinData->SettingFile().iWidth ) / 1024.0f;

	if( m_pWinData->SettingFile().iHeight < 768 ) // zmniejszamy fonta
		fScaleY = 1.0f / ( 768.0f  / static_cast< GLfloat >( m_pWinData->SettingFile().iHeight ) );
	else // zwieksz fonta
		fScaleY = static_cast< GLfloat >( m_pWinData->SettingFile().iHeight ) / 768.0f;

	glScalef( fScaleX, fScaleY, 1.0f );
}

GLvoid CSDLFont::MidScreenFont( SDL_Rect &in_out_sdlLocation, GLint iIndex )
{
	if( !m_aStorageBuffer[ iIndex ].bMidScreen )
		return;

	//tekst ma byc justowany na srodku ekran
	//wiec modyfikujemy ustawienie tekstu po osi x
	//512 = 1024 / 2 - czyli srodek ekranu
	in_out_sdlLocation.x = static_cast<Sint16>( 512 - ( m_aStorageBuffer[ iIndex ].iOriginalWidth / 2 ) );
}

GLvoid CSDLFont::DrawBkgText( const SDL_Rect &in_sdlLocation, GLint iIndex )
{
	//rysujemy t³o pod tekstem
	if( !m_bDrawBkgText )
		return;

	glDisable( GL_TEXTURE_2D );
	glColor3ub( m_ubBkgR, m_ubBkgG, m_ubBkgB );
	GLint iOW = m_aStorageBuffer[ iIndex ].iOriginalWidth;
	GLint iOH = m_aStorageBuffer[ iIndex ].iOriginalHeight;

	m_aIndices[ 0 ] = CVector2( GLfloat( in_sdlLocation.x + iOW ), GLfloat( in_sdlLocation.y + iOH ) );
	m_aIndices[ 1 ] = CVector2( GLfloat( in_sdlLocation.x + iOW ), GLfloat( in_sdlLocation.y ) );
	m_aIndices[ 2 ] = CVector2( GLfloat( in_sdlLocation.x ),	   GLfloat( in_sdlLocation.y + iOH ) );
	m_aIndices[ 3 ] = CVector2( GLfloat( in_sdlLocation.x ),	   GLfloat( in_sdlLocation.y ) );

	glEnableClientState( GL_VERTEX_ARRAY );
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );
	glDisableClientState( GL_NORMAL_ARRAY );
	glDisableClientState( GL_COLOR_ARRAY );

	glVertexPointer( 2, GL_FLOAT, 0, m_aIndices );

	glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
}

GLvoid CSDLFont::DrawQuadText( const SDL_Rect &in_sdlLocation, GLint iIndex )
{
	GLint iW = m_aStorageBuffer[ iIndex ].iWidthPowOfTwo;
	GLint iH = m_aStorageBuffer[ iIndex ].iHeightPowOfTwo;

	m_aIndices[ 0 ] = CVector2( GLfloat( in_sdlLocation.x + iW ), GLfloat( in_sdlLocation.y + iH ) );
	m_aIndices[ 1 ] = CVector2( GLfloat( in_sdlLocation.x + iW ), GLfloat( in_sdlLocation.y ) );
	m_aIndices[ 2 ] = CVector2( GLfloat( in_sdlLocation.x ),	  GLfloat( in_sdlLocation.y + iH ) );
	m_aIndices[ 3 ] = CVector2( GLfloat( in_sdlLocation.x ),	  GLfloat( in_sdlLocation.y ) );

	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );
	glDisableClientState( GL_NORMAL_ARRAY );
	glDisableClientState( GL_COLOR_ARRAY );

	glTexCoordPointer( 2, GL_FLOAT, 0, m_aTexCoord );
	glVertexPointer( 2, GL_FLOAT, 0, m_aIndices );

	glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
}

GLvoid CSDLFont::RenderText( LPCTSTR lpText, TTF_Font *pFont, const SDL_Color &in_sdlColor, SDL_Rect &in_out_sdlLocation, GLboolean bMidScreen, GLboolean bIsRect /*= GL_FALSE*/ )
{
	if( !pFont || lpText == NULL )
		return;

	glDisable( GL_LIGHTING );

	GLint iIndex = 0; //index sorage buffera

	if( IsTextInStorageBuffer( lpText, in_sdlColor, bMidScreen, iIndex ) ) {
		//tekst juz istnieje
		RenderAlreadyExistText( in_out_sdlLocation, iIndex, bIsRect );
		return;
	}

	//tworzymy nowy tekst
	RenderNewText( lpText, pFont, in_sdlColor, in_out_sdlLocation, bMidScreen, bIsRect );
}

GLboolean CSDLFont::IsTextInStorageBuffer( LPCTSTR lpText, const SDL_Color &in_sdlColor, GLboolean bMidScreen, GLint &out_iIndex )
{
	//sprawdzenie czy tekst juz istnieje
	//sprawdzane jest wszystko, tj. kolor, tekst i bMidScreen
	out_iIndex = 0;

	if( m_bUseStorageBuffer ) {
		for( out_iIndex = GetStorageBufferSize() - 1; out_iIndex >= 0; --out_iIndex ) {
			if( m_aStorageBuffer[ out_iIndex ].lpText == NULL ) continue;
			if( in_sdlColor.r != m_aStorageBuffer[ out_iIndex ].sdlColor.r ) continue;
			if( in_sdlColor.g != m_aStorageBuffer[ out_iIndex ].sdlColor.g ) continue;
			if( in_sdlColor.b != m_aStorageBuffer[ out_iIndex ].sdlColor.b ) continue;
			if( bMidScreen != m_aStorageBuffer[ out_iIndex ].bMidScreen ) continue;

			if( _tcscmp( lpText, m_aStorageBuffer[ out_iIndex ].lpText ) == 0 )
				return GL_TRUE;
		}
	}

	return GL_FALSE;
}

GLvoid CSDLFont::RenderAlreadyExistText( SDL_Rect &in_out_sdlLocation, GLint iIndex, GLboolean bIsRect )
{
	//aktualizujemy czas ostatniego odwolania sie
	m_aStorageBuffer[ iIndex ].uiRefTime = ( ::timeGetTime() / 1000 );

	ScaleFont();
	if( bIsRect ) {
		//in_out_sdlLocation zawiera wspolrzedne prostokata,
		//w ktorego centrum ma sie narysowac tekst
		CalculatePosTextFromRect( in_out_sdlLocation, iIndex );
	}
	else
		MidScreenFont( in_out_sdlLocation, iIndex );

	DrawBkgText( in_out_sdlLocation, iIndex );

	glBlendFunc( GL_ONE, GL_ONE );
	glEnable( GL_BLEND );

	glBindTexture( GL_TEXTURE_2D, m_aStorageBuffer[ iIndex ].uiTexture );
	glEnable( GL_TEXTURE_2D );

	glColor3f( 1.0f, 1.0f, 1.0f );

	DrawQuadText( in_out_sdlLocation, iIndex );

	glFinish();

	glDisable( GL_BLEND );
	glDisable( GL_TEXTURE_2D );
}

GLvoid CSDLFont::RenderNewText( LPCTSTR lpText, TTF_Font *pFont, const SDL_Color &in_sdlColor, SDL_Rect &in_out_sdlLocation, GLboolean bMidScreen, GLboolean bIsRect )
{
	//tworzymy nowy tekst
	SStorageBuffer storageBuffer;
	m_aStorageBuffer.push_back( storageBuffer );
	GLint iIndex = GetStorageBufferSize() - 1;

	SDL_Surface *pTextSufrace;
	SDL_Surface *pScreenSufrace;

	m_aStorageBuffer[ iIndex ].sdlColor = in_sdlColor;
	m_aStorageBuffer[ iIndex ].bMidScreen = bMidScreen;
	m_aStorageBuffer[ iIndex ].uiRefTime = 0;

	if( m_bUseStorageBuffer ) {
		size_t uiLength = _tcslen( lpText ) + 1;
		m_aStorageBuffer[ iIndex ].lpText = new TCHAR[ uiLength ];
		_tcscpy_s( m_aStorageBuffer[ iIndex ].lpText, uiLength, lpText );
	}
	else m_aStorageBuffer[ iIndex ].lpText = NULL;

#ifdef UNICODE
	pTextSufrace = TTF_RenderUNICODE_Blended( pFont, ( Uint16* )lpText, in_sdlColor );
#else
	pTextSufrace = TTF_RenderText_Blended( pFont, lpText, in_sdlColor );
#endif
	if( pTextSufrace == NULL ) {
		__LOG_FORMATTED( "ERROR: CSDLFont::RenderText - TTF_RenderText_Blended failed: %s.", TTF_GetError() );
		return;
	}
	
	m_aStorageBuffer[ iIndex ].iWidthPowOfTwo = NextPowerOfTwo( pTextSufrace->w );
	m_aStorageBuffer[ iIndex ].iHeightPowOfTwo = NextPowerOfTwo( pTextSufrace->h );
	
	m_aStorageBuffer[ iIndex ].iOriginalWidth = pTextSufrace->w;
	m_aStorageBuffer[ iIndex ].iOriginalHeight = pTextSufrace->h;

	if( bIsRect ) {
		//in_out_sdlLocation zawiera wspolrzedne prostokata,
		//w ktorego centrum ma sie narysowac tekst
		CalculatePosTextFromRect( in_out_sdlLocation, iIndex );
	}
	else
		MidScreenFont( in_out_sdlLocation, iIndex );

	pScreenSufrace = SDL_CreateRGBSurface( SDL_SWSURFACE,
										   m_aStorageBuffer[ iIndex ].iWidthPowOfTwo,
										   m_aStorageBuffer[ iIndex ].iHeightPowOfTwo,
										   32,
										   0x00ff0000,
										   0x0000ff00,
										   0x000000ff,
										   0xff000000 );

	if( pScreenSufrace == NULL ) {
		__LOG_FORMATTED( "ERROR: CSDLFont::RenderText - SDL_CreateRGBSurface failed: %s.", SDL_GetError() );
		SDL_FreeSurface( pTextSufrace );
		return;
	}

	if( SDL_BlitSurface( pTextSufrace, 0, pScreenSufrace, 0 ) ) {
		__LOG_FORMATTED( "ERROR: CSDLFont::RenderText - SDL_BlitSurface failed: %s.", SDL_GetError() );
		SDL_FreeSurface( pTextSufrace );
		SDL_FreeSurface( pScreenSufrace );
		return;
	}

	ScaleFont();
	DrawBkgText( in_out_sdlLocation, iIndex );

	glBlendFunc( GL_ONE, GL_ONE );
	glEnable( GL_BLEND );

	// tworzymy nowa teksture
	glGenTextures( 1, &m_aStorageBuffer[ iIndex ].uiTexture );
	glBindTexture( GL_TEXTURE_2D, m_aStorageBuffer[ iIndex ].uiTexture );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	glTexImage2D( GL_TEXTURE_2D,
				  0, 4,
				  m_aStorageBuffer[ iIndex ].iWidthPowOfTwo,
				  m_aStorageBuffer[ iIndex ].iHeightPowOfTwo,
				  0, GL_RGBA, GL_UNSIGNED_BYTE, pScreenSufrace->pixels );
	
	glEnable( GL_TEXTURE_2D );

	glColor3f( 1.0f, 1.0f, 1.0f );
	
	DrawQuadText( in_out_sdlLocation, iIndex );
	
	glFinish();
	
	glDisable( GL_BLEND );
	glDisable( GL_TEXTURE_2D );
	
	if( !m_bUseStorageBuffer )
		ClearStorageBuffer( GL_FALSE );
	else {
		//limit buforowania przez jeden obiekt
		if( GetStorageBufferSize() > m_iBufferLimit )
			DeleteOneStorageBuffer( iIndex );
	}

	SDL_FreeSurface( pTextSufrace );
	SDL_FreeSurface( pScreenSufrace );
}

//czyszczenie calego bufora
GLvoid CSDLFont::ClearStorageBuffer( GLboolean bLog /*= GL_TRUE*/ )
{
	if( bLog )
		__LOG_FORMATTED( _T("CSDLFont::ClearStorageBuffer: delete items: %d"), GetStorageBufferSize() );

	for( GLint i = GetStorageBufferSize() - 1; i >= 0; --i )
		DeleteOneStorageBuffer( i );
	
	m_aStorageBuffer.clear();
}

//czyszczenie tylko starych i nieuzywanych
GLvoid CSDLFont::DeleteOldStorageBuffer()
{
	GLuint uiCurrentSeconds = ::timeGetTime() / 1000;
	
	for( GLint i = GetStorageBufferSize() - 1; i >= 0; --i ) {
		if( uiCurrentSeconds - m_uiTimeOut <= m_aStorageBuffer[ i ].uiRefTime )
			continue;

		DeleteOneStorageBuffer( i );
	}
}

GLvoid CSDLFont::DeleteOneStorageBuffer( GLint iIndex )
{
	if( m_aStorageBuffer[ iIndex ].lpText ) {
		delete [] m_aStorageBuffer[ iIndex ].lpText;
		m_aStorageBuffer[ iIndex ].lpText = NULL;
	}

	glDeleteTextures( 1, &m_aStorageBuffer[ iIndex ].uiTexture );

	m_aStorageBuffer.erase( m_aStorageBuffer.begin() + iIndex );
}

GLvoid CSDLFont::Timer()
{
	if( ++m_uiCountTimer == m_uiInterval ) { //usuwamy co m_uiInterval sekund
		DeleteOldStorageBuffer();
		m_uiCountTimer = 0;
	}
}

LPTSTR CSDLFont::GetCompileVersionSLD_ttf()
{
	SDL_version version, sdl_version;
	TTF_VERSION( &version );
	SDL_VERSION( &sdl_version );

	::wsprintf( m_tcVersion, _T("SDL compile version: %d.%d.%d, SDL_ttf compile version: %d.%d.%d"),
				sdl_version.major,
				sdl_version.minor,
				sdl_version.patch,
				version.major,
				version.minor,
				version.patch );
	
	return m_tcVersion;
}

LPTSTR CSDLFont::GetLinkedVersionSLD_ttf()
{
	SDL_version *version = const_cast<SDL_version*>( TTF_Linked_Version() );
	SDL_version *sdl_version = const_cast<SDL_version*>( SDL_Linked_Version() );

	::wsprintf( m_tcVersion, _T("SDL linked version:  %d.%d.%d, SDL_ttf linked version:  %d.%d.%d"),
				sdl_version->major,
				sdl_version->minor,
				sdl_version->patch,
				version->major,
				version->minor,
				version->patch );

	return m_tcVersion;
}