// Console.cpp: implementation of the CConsole class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "WindowData.h"
#include "../Fps/Fps.h"
#include "../Fonts/BitmapFont.h"
#include "../Fonts/SDLFont.h"
#include "../Fonts/MyFont.h"
#include "GameControl.h"
#include "../Extensions/ARBMultisample.h"
#include "../Draw/3DObjManager.h"
#include "Console.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// Je¿eli 1 to loguj do pliku wszystko to co wypisuje konsola
#define _LOG_CONSOLE_	1

CConsole::CConsole()
{
	m_pSDLFont = NULL;
	m_pBitmapFont = NULL;
	m_pMyFont = NULL;

	m_aTextCoord = new CVector2[ 4 ];
	m_aIndices = new CVector2[ 4 ];

#if _USE_2D_ONLY_
	m_pSDLFont = new CSDLFont( "fonts/FreeMonoBold.ttf", 16 );
#else
	#if _USE_CONSOLE_WITH_BITMAP_FONT_
		m_pBitmapFont = new CBitmapFont( CWindowData::GetInstance()->GetHDC(), _T("Fixedsys"), -12 );
	#elif _USE_CONSOLE_WITH_MY_FONT_
		m_pMyFont = new CMyFont();
	#else
		m_pSDLFont = new CSDLFont( "fonts/FreeMonoBold.ttf", 16 );
	#endif
#endif //_USE_2D_ONLY_
	
	m_pSpeedControl = NULL;

	m_bDrawCursor = GL_FALSE;
	m_bShowConsole = GL_FALSE;

	::wsprintf( m_cLastLineConst, _T("%s"), _T("Game:\\>") );
	::wsprintf( m_cLastLine, _T("%s"), m_cLastLineConst );
	m_iStartLine = 0;
	m_iEndLine = 0;

	m_iCurrentLastCommand = 0;

	SetBlockConsole( GL_TRUE );

	m_eFilter = ENormal;

	SetVariables();

	//sprawdzamy czy wystepuje parametr "/console"
	if( CWindowData::GetInstance()->IsCommandLine( _T("/console") ) )
		SetBlockConsole( GL_FALSE ); //anuluj blokade

	AddText( _T("**********************************") );
	AddText( FRAME_NAME );
	AddText( _T("**********************************") );

	AddText( _T("") );
	AddText( "Compile date: %s, %s", __DATE__, __TIME__ );
	AddText( _T("") );

	ExecuteCommand( _T("time") );
	AddText( _T("") );
	ExecuteCommand( _T("info") );
	AddText( _T("") );
	ExecuteCommand( _T("antialiasing info") );
}

CConsole::~CConsole()
{
	DeleteLastCommands();
	DeleteConsoleLines();
	delete m_pMyFont;
	delete m_pBitmapFont;
	delete m_pSDLFont;
	delete m_pSpeedControl;
	delete [] m_aTextCoord;
	delete [] m_aIndices;
}

GLvoid CConsole::SetVariables()
{
#if _USE_2D_ONLY_
	//dla czcionki z SDL_ttf
	m_fPosX = 0.0f; //pozycje wyjœciowe tekstu - dla czcionki bitmapowej
	m_fPosY = 0.0f;
	m_iPageCountLine = 22;	//wyswietla 22 linii + znak zachety

	SetRegionX( m_rcRegionBoard.left, 1 );
	SetRegionX( m_rcRegionBoard.right, 99 );
	SetRegionY( m_rcRegionBoard.top, 0 );
	SetRegionY( m_rcRegionBoard.bottom, 63 );

	m_aTextCoord[ 0 ] = CVector2( 1.0f, 0.0f );
	m_aTextCoord[ 1 ] = CVector2( 1.0f, 1.0f );
	m_aTextCoord[ 2 ] = CVector2( 0.0f, 0.0f );
	m_aTextCoord[ 3 ] = CVector2( 0.0f, 1.0f );
#else
	#if _USE_CONSOLE_WITH_BITMAP_FONT_
		//dla czcionki bitmapowej
		m_fPosX = -0.52f;
		m_fPosY = 0.385f;
		m_iPageCountLine = 22;
		//wyswietla 22 linii + znak zachety

		m_aTextCoord[ 0 ] = CVector2( 1.0f, 0.0f );
		m_aTextCoord[ 1 ] = CVector2( 1.0f, 1.0f );
		m_aTextCoord[ 2 ] = CVector2( 0.0f, 0.0f );
		m_aTextCoord[ 3 ] = CVector2( 0.0f, 1.0f );

		m_aIndices[ 0 ] = CVector2( +0.54f, -0.08f );
		m_aIndices[ 1 ] = CVector2( +0.54f,  +0.4f );
		m_aIndices[ 2 ] = CVector2( -0.54f, -0.08f );
		m_aIndices[ 3 ] = CVector2( -0.54f,  +0.4f );

	#elif _USE_CONSOLE_WITH_MY_FONT_
		//dla mojej czcionki
		m_fPosX = -66.0f; //pozycje wyjœciowe tekstu
		m_fPosY = 0.37f;
		m_iPageCountLine = 25; //wyswietla 25 linii + znak zachety

		m_aIndices[ 0 ] = CVector2( +0.54f, -0.25f );
		m_aIndices[ 1 ] = CVector2( +0.54f,  +0.4f );
		m_aIndices[ 2 ] = CVector2( -0.54f, -0.25f );
		m_aIndices[ 3 ] = CVector2( -0.54f,  +0.4f );

	#else
		//dla czcionki z SDL_ttf
		m_fPosX = 0.0f; //pozycje wyjœciowe tekstu - dla czcionki bitmapowej
		m_fPosY = 0.0f;
		m_iPageCountLine = 22;	//wyswietla 22 linii + znak zachety

		SetRegionX( m_rcRegionBoard.left, 1 );
		SetRegionX( m_rcRegionBoard.right, 99 );
		SetRegionY( m_rcRegionBoard.top, 0 );
		SetRegionY( m_rcRegionBoard.bottom, 63 );

		m_aTextCoord[ 0 ] = CVector2( 1.0f, 0.0f );
		m_aTextCoord[ 1 ] = CVector2( 1.0f, 1.0f );
		m_aTextCoord[ 2 ] = CVector2( 0.0f, 0.0f );
		m_aTextCoord[ 3 ] = CVector2( 0.0f, 1.0f );
	#endif
#endif //_USE_2D_ONLY_
}

GLvoid CConsole::CreateSpeedControl()
{
	if( !m_pSpeedControl ) {
		m_pSpeedControl = new CSpeedControl();
		m_iAnimCtrl1 = m_pSpeedControl->CreateAnimationControl();
		m_iAnimCtrl2 = m_pSpeedControl->CreateAnimationControl();
	}
}

GLvoid CConsole::SetStartEndLineOnEnd() //ustawia wyswietlanie ostatnich linii
{
	m_iStartLine = 0;
	m_iEndLine = GetConsoleLineSize();
	if( GetConsoleLineSize() > m_iPageCountLine )
		m_iStartLine = GetConsoleLineSize() - m_iPageCountLine;
}

GLvoid CConsole::ShowConsole()
{
	if( m_bBlockConsole )
		return;

#if _USE_SOUND_OPENAL_
	COpenALManager::GetInstance()->PlayNo3D( 0 );
#else
	CGameControl::GetInstance()->GetSoundFMOD()->PlaySample( 0 );
#endif

	SetStartEndLineOnEnd();

#if _USE_2D_ONLY_
	//jak dla SDLFont
	m_fPosYConsole = 0.005f;
	m_iPosYConsole = m_rcRegionBoard.bottom * 2;
	m_iRateYConsole = m_rcRegionBoard.bottom / 4;
#else
	#if _USE_CONSOLE_WITH_MY_FONT_
		m_fPosYConsole = 0.007f;
	#elif _USE_CONSOLE_WITH_BITMAP_FONT_
		m_fPosYConsole = 0.005f;
	#else
		//dla CSDLFont
		m_fPosYConsole = 0.005f;
		m_iPosYConsole = m_rcRegionBoard.bottom * 2;
		m_iRateYConsole = m_rcRegionBoard.bottom / 4;
	#endif
#endif //_USE_2D_ONLY_
	m_bShowConsole = GL_TRUE;
}

GLvoid CConsole::AddText( const char *fmt, ... )
{
	char cText[ MAX_ADDTEXT_BUFFER ];
	va_list	ap;

	va_start( ap, fmt );
	vsprintf_s( cText, MAX_ADDTEXT_BUFFER, fmt, ap );
	va_end( ap );

	PrepareText( ENormal, cText );
}

GLvoid CConsole::AddText( EConsoleMsgType eType, const char *fmt, ... )
{
	if( m_bBlockConsole )
		return;

	char cText[ MAX_ADDTEXT_BUFFER ];
	va_list	ap;

	va_start( ap, fmt );
	vsprintf_s( cText, MAX_ADDTEXT_BUFFER, fmt, ap );
	va_end( ap );

	PrepareText( eType, cText );
}

#ifdef UNICODE
GLvoid CConsole::AddText( const wchar_t *fmt, ... )
{
	wchar_t cText[ MAX_ADDTEXT_BUFFER ];
	va_list	ap;

	va_start( ap, fmt );
	vswprintf_s( cText, MAX_ADDTEXT_BUFFER, fmt, ap );
	va_end( ap );

#if _LOG_CONSOLE_
	__LOG( cText );
#endif

	if( m_bBlockConsole )
		return;

	AddTextFinally( ENormal, cText, wcslen( cText ) );
}

GLvoid CConsole::AddText( EConsoleMsgType eType, const wchar_t *fmt, ... )
{
	wchar_t cText[ MAX_ADDTEXT_BUFFER ];
	va_list	ap;

	va_start( ap, fmt );
	vswprintf_s( cText, MAX_ADDTEXT_BUFFER, fmt, ap );
	va_end( ap );

#if _LOG_CONSOLE_
	__LOG( cText );
#endif

	if( m_bBlockConsole )
		return;

	AddTextFinally( eType, cText, wcslen( cText ) );
}
#endif //UNICODE

GLvoid CConsole::PrepareText( EConsoleMsgType eType, const char *lpText )
{
	TCHAR cTextUni[ MAX_ADDTEXT_BUFFER ];
#ifdef UNICODE
	CUHF::ConvertCharToWchar( cTextUni, MAX_ADDTEXT_BUFFER - 1, lpText );
#else
	_tcscpy_s( cTextUni, MAX_ADDTEXT_BUFFER - 1, lpText );
#endif

#if _LOG_CONSOLE_
	__LOG( cTextUni );
#endif

	if( m_bBlockConsole )
		return;

	AddTextFinally( eType, cTextUni, _tcslen( cTextUni ) );
}

GLvoid CConsole::AddTextFinally( EConsoleMsgType eType, LPCTSTR lpText, size_t uiLength )
{
	if( GetConsoleLineSize() > 998 )
		DeleteConsoleLines();

	SConsoleLine consoleLine;
	m_aConsoleLine.push_back( consoleLine );
	GLint iIndex = GetConsoleLineSize() - 1;

#if !_USE_CONSOLE_WITH_MY_FONT_ && !_USE_CONSOLE_WITH_BITMAP_FONT_
	const GLuint uiMaxLengthOfLine = 91;

	if( uiLength <= uiMaxLengthOfLine ) {
#endif

		m_aConsoleLine[ iIndex ].lpMsg = new TCHAR[ uiLength + 6 ];
		::wsprintf( m_aConsoleLine[ iIndex ].lpMsg, _T("%003d. %s"), GetConsoleLineSize(), lpText );
		m_aConsoleLine[ iIndex ].eType = eType;

		++m_iEndLine;
		if( GetConsoleLineSize() > m_iPageCountLine )
			++m_iStartLine;

#if !_USE_CONSOLE_WITH_MY_FONT_ && !_USE_CONSOLE_WITH_BITMAP_FONT_
		return;
	}

	TCHAR *tcLine1 = new TCHAR[ uiMaxLengthOfLine + 1 ];
	TCHAR *tcLine2 = new TCHAR[ uiLength - uiMaxLengthOfLine + 1 ];
	GLuint i = 0;
	for( ; i < uiMaxLengthOfLine; ++i )
		tcLine1[ i ] = lpText[ i ];
	tcLine1[ i ] = 0;

	GLuint j = 0;
	for( ; i < uiLength; ++i, ++j )
		tcLine2[ j ] = lpText[ i ];
	tcLine2[ j ] = 0;

	m_aConsoleLine[ iIndex ].lpMsg = new TCHAR[ _tcslen( tcLine1 ) + 6 ];
	::wsprintf( m_aConsoleLine[ iIndex ].lpMsg, _T("%003d. %s"), GetConsoleLineSize(), tcLine1 );
	m_aConsoleLine[ iIndex ].eType = eType;

	++m_iEndLine;
	if( GetConsoleLineSize() > m_iPageCountLine )
		++m_iStartLine;

	// dodaj drugi, zlamany tekst
	if( GetConsoleLineSize() > 998 )
		DeleteConsoleLines();

	SConsoleLine consoleLine2;
	m_aConsoleLine.push_back( consoleLine2 );
	GLint iIndex2 = GetConsoleLineSize() - 1;

	m_aConsoleLine[ iIndex2 ].lpMsg = new TCHAR[ _tcslen( tcLine2 ) + 6 ];
	::wsprintf( m_aConsoleLine[ iIndex2 ].lpMsg, _T("%003d. %s"), GetConsoleLineSize(), tcLine2 );
	m_aConsoleLine[ iIndex2 ].eType = eType;

	++m_iEndLine;
	if( GetConsoleLineSize() > m_iPageCountLine )
		++m_iStartLine;

	delete [] tcLine1;
	delete [] tcLine2;
#endif
}

GLvoid CConsole::RestartObjects()
{
	delete m_pBitmapFont;
	m_pBitmapFont = NULL;
#if _USE_CONSOLE_WITH_BITMAP_FONT_
	m_pBitmapFont = new CBitmapFont( CWindowData::GetInstance()->GetHDC(), _T("Fixedsys"), -12 );
#endif
	SetVariables();
}

GLvoid CConsole::DrawRect( GLboolean bTexCoord /*= GL_TRUE*/ )
{
	glEnableClientState( GL_VERTEX_ARRAY );
	( bTexCoord ) ? glEnableClientState( GL_TEXTURE_COORD_ARRAY ) : glDisableClientState( GL_TEXTURE_COORD_ARRAY );
	glDisableClientState( GL_NORMAL_ARRAY );
	glDisableClientState( GL_COLOR_ARRAY );

	if( bTexCoord )
		glTexCoordPointer( 2, GL_FLOAT, 0, m_aTextCoord );
	glVertexPointer( 2, GL_FLOAT, 0, m_aIndices );

	glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
}

GLvoid CConsole::DrawConsoleWithSDLFont()
{
	//dobre i szybkie rysowanie ale polskie fonty tylko w UNICODE

	if( !m_bShowConsole || m_bBlockConsole )
		return;

	CreateSpeedControl();

#if !_USE_2D_ONLY_
	CGameControl::GetInstance()->Enable2D();
#endif

	glDisable( GL_LIGHTING );

	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glDisable( GL_DEPTH_TEST );
	
	if( m_pSpeedControl->CheckAnimationTime( 25, m_iAnimCtrl1 ) )
		if( m_iPosYConsole > 0 )
			m_iPosYConsole -= ( m_iRateYConsole );
	if( m_iPosYConsole < 0 )
		m_iPosYConsole = 0;

	CGameControl::GetInstance()->GetTexLoader()->SetTexture( 1 );

	glColor4ub( 255, 255, 255, 200 );
	glEnable( GL_BLEND );

	m_aIndices[ 0 ] = CVector2( GLfloat( m_rcRegionBoard.right ), GLfloat( m_rcRegionBoard.bottom - m_iPosYConsole ) );
	m_aIndices[ 1 ] = CVector2( GLfloat( m_rcRegionBoard.right ), GLfloat( m_rcRegionBoard.top - m_iPosYConsole ) );
	m_aIndices[ 2 ] = CVector2( GLfloat( m_rcRegionBoard.left ),  GLfloat( m_rcRegionBoard.bottom - m_iPosYConsole ) );
	m_aIndices[ 3 ] = CVector2( GLfloat( m_rcRegionBoard.left ),  GLfloat( m_rcRegionBoard.top - m_iPosYConsole ) );

	DrawRect();

	glDisable( GL_BLEND );
	glEnable( GL_DEPTH_TEST );

	COLORREF crColor;
	if( m_iPosYConsole == 0 ) {
		GLint iY = 30;
		for( GLint i = m_iStartLine; i < m_iEndLine; ++i ) {
			if( m_eFilter != ENormal && m_eFilter != m_aConsoleLine[ i ].eType )
				continue;

			switch( m_aConsoleLine[ i ].eType ) {
				case EWarning:
					crColor = RGB( 255, 255, 0 );
					break;
				case EError:
					crColor = RGB( 255, 0, 0 );
					break;
				case ESuccess:
					crColor = RGB( 0, 255, 0 );
					break;
				default:
					crColor = RGB( 255, 255, 255 );
					break;
			}
			m_pSDLFont->DrawText( 30, iY - m_iPosYConsole, crColor, m_aConsoleLine[ i ].lpMsg );
			iY += 20; //skok linijki
		}
	
		//znak zachêty
		if( m_pSpeedControl->CheckAnimationTime( 500, m_iAnimCtrl2 ) )
			m_bDrawCursor = !m_bDrawCursor;

		if( m_bDrawCursor )
			m_pSDLFont->DrawTextFormat( 30, iY - m_iPosYConsole, GL_FALSE, RGB( 255, 255, 255 ), _T("%s_"), m_cLastLine );
		else
			m_pSDLFont->DrawText( 30, iY - m_iPosYConsole, RGB( 255, 255, 255 ), m_cLastLine );
	}

#if !_USE_2D_ONLY_
	CGameControl::GetInstance()->Disable2D();
#endif //_USE_2D_ONLY_
}

#if _USE_CONSOLE_WITH_BITMAP_FONT_
GLvoid CConsole::DrawConsoleWithBitmapFont()
{
	//dobre rysowanie ale bardzo obciaza procesor
	//nie wspó³pracuje z _USE_2D_ONLY_

	if( !m_bShowConsole || m_bBlockConsole )
		return;

	CreateSpeedControl();

	glDisable( GL_DEPTH_TEST );
	glDisable( GL_LIGHTING );
	
	glPushMatrix();
	if( m_fPosYConsole > 0.0f )
		m_fPosYConsole -= ( 0.01f * m_pSpeedControl->GetMultiplier() );
	if( m_fPosYConsole < 0.014f )
		m_fPosYConsole = 0.014f;

	CGameControl::GetInstance()->GetTexLoader()->SetTexture( 1 );
	glTranslatef( 0.0f, m_fPosYConsole, -1.0f );

	DrawRect();

	glPopMatrix();

	glPushMatrix();
	glTranslatef( 0.0f, 0.0f, -0.005f ); //wysuwam tekst ciutke do przodu aby by³ widoczny
	
	if( m_fPosYConsole == 0.014f ) { //pokaz tekst jezeli konsla sie wysunela
		GLfloat fY = m_fPosY;
		for( GLint i = m_iStartLine; i < m_iEndLine; ++i ) {
			if( m_eFilter != ENormal && m_eFilter != m_aConsoleLine[ i ].eType )
				continue;

			switch( m_aConsoleLine[ i ].eType ) {
				case EWarning:
					glColor3f( 1.0f, 1.0f, 0.0f );
					break;
				case EError:
					glColor3f( 1.0f, 0.0f, 0.0f );
					break;
				case ESuccess:
					glColor3f( 0.0f, 1.0f, 0.0f );
					break;
				default:
					glColor3f( 1.0f, 1.0f, 1.0f );
					break;
			}
			m_pBitmapFont->DrawText( m_fPosX, m_fPosYConsole + fY, m_aConsoleLine[ i ].lpMsg );
			fY -= 0.02f; //skok linijki
		}
		
		//znak zachêty
		if( m_pSpeedControl->CheckAnimationTime( 500, m_iAnimCtrl2 ) )
			m_bDrawCursor = !m_bDrawCursor;

		glColor3f( 1.0f, 1.0f, 1.0f );
		if( m_bDrawCursor )
			m_pBitmapFont->DrawTextFormat( m_fPosX, m_fPosYConsole + fY, _T("%s\r"), m_cLastLine );
		else
			m_pBitmapFont->DrawText( m_fPosX, m_fPosYConsole + fY, m_cLastLine );
	}
	glPopMatrix();

	glEnable( GL_DEPTH_TEST );
}
#endif //_USE_CONSOLE_WITH_BITMAP_FONT_

#if _USE_CONSOLE_WITH_MY_FONT_
GLvoid CConsole::DrawConsoleMyFont()
{
	//nie wspó³pracuje z _USE_2D_ONLY_

	if( !m_bShowConsole || m_bBlockConsole )
		return;

	CreateSpeedControl();

	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glDisable( GL_DEPTH_TEST );
	glDisable( GL_LIGHTING );
	GLfloat fY = m_fPosY;
	
	glPushMatrix();
	if( m_fPosYConsole > 0.0f )
		m_fPosYConsole -= ( 0.01f * m_pSpeedControl->GetMultiplier() );
	if( m_fPosYConsole < 0.0f )
		m_fPosYConsole = 0.0f;

	glTranslatef( 0.0f, m_fPosYConsole, -1.0f );
	glColor4ub( 12, 12, 12, 200 );
	glEnable( GL_BLEND );

	DrawRect( GL_FALSE );

	glDisable( GL_BLEND );
	glColor3f( 1.0f, 1.0f, 1.0f );
	glPopMatrix();

	glPushMatrix();
	glColor3f( 1.0f, 1.0f, 1.0f );
	glTranslatef( 0.0f, 0.0f, -0.005f ); //wysuwam tekst ciutke do przodu aby by³ widoczny
	
	glPopMatrix();
	
	COLORREF crColor;
	for( GLint i = m_iStartLine; i < m_iEndLine; ++i ) {
		if( m_eFilter != ENormal && m_eFilter != m_aConsoleLine[ i ].eType )
			continue;

		switch( m_aConsoleLine[ i ].eType ) {
			case EWarning:
				crColor = RGB( 255, 255, 0 );
				break;
			case EError:
				crColor = RGB( 255, 0, 0 );
				break;
			case ESuccess:
				crColor = RGB( 0, 255, 0 );
				break;
			default:
				crColor = RGB( 255, 255, 255 );
				break;
		}
		m_pMyFont->DrawText( m_aConsoleLine[ i ].lpMsg, m_fPosX, m_fPosYConsole + fY, -1.0f, crColor, 1.3f, 0.008f );
		fY -= 0.024f; //skok linijki
	}

	//znak zachêty
	if( m_pSpeedControl->CheckAnimationTime( 500, m_iAnimCtrl2 ) )
		m_bDrawCursor = !m_bDrawCursor;
		
	if( m_bDrawCursor )
		m_pMyFont->DrawTextFormat( m_fPosX, m_fPosYConsole + fY, -1.0f, RGB( 255, 255, 255 ), 1.3f, 0.008f, _T("%s_"), m_cLastLine );
	else
		m_pMyFont->DrawText( m_cLastLine, m_fPosX, m_fPosYConsole + fY, -1.0f, RGB( 255, 255, 255 ), 1.3f, 0.008f );

	glPopMatrix();

	glEnable( GL_DEPTH_TEST );
}
#endif //_USE_CONSOLE_WITH_MY_FONT_

GLvoid CConsole::LineScrollUp()
{
	--m_iStartLine;
	--m_iEndLine;

	if( m_iStartLine < 0 ) {
		m_iStartLine = 0;

		if( GetConsoleLineSize() > m_iPageCountLine )
			m_iEndLine = m_iPageCountLine;
		else
			m_iEndLine = GetConsoleLineSize();
	}

	if( m_iEndLine < 0 )
		m_iEndLine = 0;
}

GLvoid CConsole::LineScrollDown()
{
	++m_iStartLine;
	++m_iEndLine;

	if( m_iStartLine > GetConsoleLineSize() - m_iPageCountLine )
		m_iStartLine = GetConsoleLineSize() - m_iPageCountLine;

	if( m_iStartLine < 0 )
		m_iStartLine = 0;

	if( m_iEndLine > GetConsoleLineSize() )
		m_iEndLine = GetConsoleLineSize();
}

GLboolean CConsole::KeyDown( GLuint uiKeyCode )
{
	if( m_bBlockConsole || !IsConsole() )
		return GL_FALSE;

	GLint iLengthLastLineConst = static_cast< GLint >( _tcslen( m_cLastLineConst ) );
	GLint iLengthLastLine = static_cast< GLint >( _tcslen( m_cLastLine ) );

	switch( uiKeyCode ) {
		case VK_RETURN:
		{
			TCHAR cCommand[ CONSOLE_MSG_LENGTH ];
			GLint iCount = 0;
			for( GLint i = iLengthLastLineConst; i < CONSOLE_MSG_LENGTH; ++i ) {
				cCommand[ iCount ] = m_cLastLine[ i ];
				if( cCommand[ iCount ] == 0 )
					break;
				++iCount;
			}
			AddText( _T("> %s"), cCommand );

			AddLastCommand( cCommand );
			
			ExecuteCommand( cCommand );
			::wsprintf( m_cLastLine, _T("%s"), m_cLastLineConst );
			SetStartEndLineOnEnd();

			return GL_TRUE;
		}
		case VK_BACK:
			if( iLengthLastLine > iLengthLastLineConst )
				m_cLastLine[ iLengthLastLine - 1 ] = 0;

			return GL_TRUE;

		case VK_ESCAPE:
		case 0xC0: //~
			HideConsole();
			return GL_TRUE;

		case VK_PRIOR: //page up
			m_iStartLine -= m_iPageCountLine;
			m_iEndLine -= m_iPageCountLine;
			if( m_iStartLine < 0 ) {
				m_iStartLine = 0;
				if( GetConsoleLineSize() > m_iPageCountLine )
					m_iEndLine = m_iPageCountLine;
				else
					m_iEndLine = GetConsoleLineSize();
			}

			if( m_iEndLine < 0 )
				m_iEndLine = 0;

			return GL_TRUE;

		case VK_NEXT: //page down
			m_iStartLine += m_iPageCountLine;
			m_iEndLine += m_iPageCountLine;

			if( m_iStartLine > GetConsoleLineSize() - m_iPageCountLine )
				m_iStartLine = GetConsoleLineSize() - m_iPageCountLine;

			if( m_iStartLine < 0 )
				m_iStartLine = 0;

			if( m_iEndLine > GetConsoleLineSize() )
				m_iEndLine = GetConsoleLineSize();

			return GL_TRUE;

		case VK_HOME:
			m_iStartLine = 0;
			m_iEndLine = GetConsoleLineSize();

			if( GetConsoleLineSize() > m_iPageCountLine )
				m_iEndLine = m_iPageCountLine;

			return GL_TRUE;

		case VK_END:
			SetStartEndLineOnEnd();
			return GL_TRUE;

		case VK_UP:
			LineScrollUp();
			return GL_TRUE;

		case VK_DOWN:
			LineScrollDown();
			return GL_TRUE;

		case VK_TAB:
		{
			GLint iSize = GetLastCommandSize();
			if( iSize <= 0 )
				return GL_TRUE;

			size_t uiLength = _tcslen( m_cLastLineConst );
			size_t uiLengthLastCommand = _tcslen( m_aLastCommand[ m_iCurrentLastCommand ] );

			GLuint i;
			for( i = 0; i < uiLengthLastCommand; ++i )
				m_cLastLine[ uiLength + i ] = m_aLastCommand[ m_iCurrentLastCommand ][ i ];

			m_cLastLine[ uiLength + i ] = 0;

			--m_iCurrentLastCommand;
			if( m_iCurrentLastCommand < 0 )
				m_iCurrentLastCommand = iSize - 1;

			return GL_TRUE;
		}
		default:
			//piszemy w wierszu polecen
			return CGameControl::GetInstance()->WriteOnKeyboard( uiKeyCode, m_cLastLine, 100 );
	}

	//return GL_FALSE; //<-nigdy tu nie wejdzie
}

GLboolean CConsole::KeyUp( GLuint /*uiKeyCode*/ )
{
	if( m_bBlockConsole || !IsConsole() )
		return GL_FALSE;

	return GL_FALSE;
}

GLboolean CConsole::MouseWheel( GLint iScrollLines )
{
	if( m_bBlockConsole || !IsConsole() ) 
		return GL_FALSE;

	if( iScrollLines > 0 )
		LineScrollUp();
	else if( iScrollLines < 0 )
		LineScrollDown();

	return GL_TRUE;
}

GLboolean CConsole::GetParameterCommand( LPCTSTR lpCommand, LPTSTR lpParameter )
{
	size_t uiLengthCommand = _tcslen( lpCommand );

	if( uiLengthCommand > 255 - 1 )
		return GL_FALSE;

	//szukamy spacji i zapisujemy to co jest po spacji
	GLboolean bSpace = GL_FALSE;
	GLuint j = 0;
	for( GLuint i = 1; i < uiLengthCommand; ++i ) {
		if( bSpace ) {
			lpParameter[ j ] = lpCommand[ i ];
			++j;
		}

		if( lpCommand[ i ] == 32 )
			bSpace = GL_TRUE;
	}
	lpParameter[ j ] = 0;
	lpParameter[ j + 1 ] = 0;

	return GL_TRUE;
}

GLvoid CConsole::AddLastCommand( LPCTSTR lpCommand )
{
	m_aLastCommand.push_back( NULL );
	GLint iIndex = GetLastCommandSize() - 1;
	m_iCurrentLastCommand = iIndex;

	size_t uiSize = _tcslen( lpCommand ) + 1;
	m_aLastCommand[ iIndex ] = new TCHAR[ uiSize ];
	_tcscpy_s( m_aLastCommand[ iIndex ], uiSize, lpCommand );
}

GLvoid CConsole::DeleteLastCommands()
{
	for( GLint i = GetLastCommandSize() - 1; i >= 0; --i )
		delete [] m_aLastCommand[ i ];

	m_aLastCommand.clear();
}

//
//ExecuteCommand - wykonanie wprowadzanych polecen przez usera
//
GLvoid CConsole::ExecuteCommand( LPCTSTR lpCommand )
{
	if( lpCommand == NULL )
		return;

	if( m_pSDLFont )
		m_pSDLFont->ClearStorageBuffer();

	if( _tcscmp( lpCommand, _T("antialiasing info") ) == 0 ) {
		if( CWindowData::GetInstance()->IsUseAntialiasing() )
			AddText( _T("Antialiasing x%d"), CWindowData::GetInstance()->GetAntialiasingSamples() );
		else {
			if( CWindowData::GetInstance()->GetAntialiasingSamples() == -1 )
				AddText( EWarning, _T("Antialiasing not supported!") );
			else
				AddText( _T("Antialiasing -> disable") );
		}
	}
	else if( _tcsstr( lpCommand, _T("antialiasing ") ) ) {
		TCHAR cParam[ MAX_ADDTEXT_BUFFER ];
		if( !GetParameterCommand( lpCommand, cParam ) )
			return;
		GLint iSamples = _tstoi( cParam );
		if( iSamples == 0 ) {
			CWindowData::GetInstance()->SetUseAntialiasing( GL_FALSE );
			CWindowData::GetInstance()->RestartWindow();
		}
		else if( iSamples > 0 && iSamples%2 == 0 )
			CWindowData::GetInstance()->SetAntialiasingSamples( iSamples, GL_TRUE );
		else
			AddText( EWarning, _T("Antialiasing -> wrong parameter") );
	}
	else if( _tcscmp( lpCommand, _T("clear") ) == 0 ) {
		DeleteConsoleLines();
		::wsprintf( m_cLastLine, _T("%s"), m_cLastLineConst );
	}
	else if( _tcscmp( lpCommand, _T("exit") ) == 0 ) {
		HideConsole();
	}
	else if( _tcscmp( lpCommand, _T("filter all") ) == 0 ) {
		m_eFilter = ENormal;
	}
	else if( _tcscmp( lpCommand, _T("filter error") ) == 0 ) {
		m_eFilter = EError;
	}
	else if( _tcscmp( lpCommand, _T("filter success") ) == 0 ) {
		m_eFilter = ESuccess;
	}
	else if( _tcscmp( lpCommand, _T("filter warning") ) == 0 ) {
		m_eFilter = EWarning;
	}
	else if( _tcscmp( lpCommand, _T("fps") ) == 0 ) {
		CGameControl::GetInstance()->GetFps()->ShowFps( !CGameControl::GetInstance()->GetFps()->IsFps() );

		if( CGameControl::GetInstance()->GetFps()->IsFps() )
			AddText( _T("FPS show") );
		else
			AddText( _T("FPS hide") );
	}
	else if( _tcscmp( lpCommand, _T("log") ) == 0 ) {
		CWindowData::GetInstance()->GetLogger()->SetActive( !CWindowData::GetInstance()->GetLogger()->IsActive() );
		if( CWindowData::GetInstance()->GetLogger()->IsActive() )
			AddText( _T("Log on") );
		else
			AddText( _T("Log off") );
	}
	else if( _tcscmp( lpCommand, _T("log clear") ) == 0 ) {
		CWindowData::GetInstance()->GetLogger()->ClearOldLog();
		AddText( _T("Log clear -> ok") );
	}
	else if( _tcscmp( lpCommand, _T("show lng files") ) == 0 ) {
		//pokazanie listy plikow lng
		for( GLint i = 0; i < CMultiLanguage::GetInstance()->GetLangFileNumber(); ++i )
			AddText( _T("%d. %s"), i, CMultiLanguage::GetInstance()->GetLangFileName( i ) );
	}
	else if( _tcsstr( lpCommand, _T("lng ") ) ) {
		TCHAR cParam[ MAX_ADDTEXT_BUFFER ];
		if( !GetParameterCommand( lpCommand, cParam ) )
			return;
		GLint iIndex = _tstoi( cParam );
		if( iIndex < 0 || iIndex >= CMultiLanguage::GetInstance()->GetLangFileNumber() )
			AddText( EWarning, _T("Change language -> wrong parameter") );
		else {
			if( CMultiLanguage::GetInstance()->LoadLanguage( iIndex ) ) {
				CWindowData::GetInstance()->SettingFile().iIndexLanguage = iIndex;
				CWindowData::GetInstance()->SaveSettingFile();
			}
		}
	}
	else if( _tcscmp( lpCommand, _T("sdl") ) == 0 ) {
		AddText( CGameControl::GetInstance()->GetSDLFont()->GetCompileVersionSLD_ttf() );
		AddText( CGameControl::GetInstance()->GetSDLFont()->GetLinkedVersionSLD_ttf() );
	}
	else if( _tcscmp( lpCommand, _T("sdl_ttf" ) ) == 0 ) {
		AddText( CGameControl::GetInstance()->GetSDLFont()->GetCompileVersionSLD_ttf() );
		AddText( CGameControl::GetInstance()->GetSDLFont()->GetLinkedVersionSLD_ttf() );
	}
	else if( _tcscmp( lpCommand, _T("textures") ) == 0 ) {
		CGameControl::GetInstance()->GetTexLoader()->DisplayIDTextureName();
	}
	else if( _tcscmp( lpCommand, _T("time") ) == 0 ) {
		SYSTEMTIME st;
		::GetLocalTime( &st );
		AddText( _T("%02d.%02d.%d - %02d:%02d:%02d"), st.wDay, st.wMonth, st.wYear, st.wHour, st.wMinute, st.wSecond );
	}
	else if( _tcscmp( lpCommand, _T("info") ) == 0 ) {
		if( glGetString( GL_VENDOR ) )
			AddText( "Vendor: %s", ( char* )glGetString( GL_VENDOR ) );
		if( glGetString( GL_RENDERER ) )
			AddText( "Render: %s", ( char* )glGetString( GL_RENDERER ) );
		if( glGetString( GL_VERSION ) )
			AddText( "Version: %s", ( char* )glGetString( GL_VERSION ) );
	}
	else if( _tcsstr( lpCommand, _T("brightness ") ) ) {
		TCHAR cBrightness[ MAX_ADDTEXT_BUFFER ];
		if( !GetParameterCommand( lpCommand, cBrightness ) )
			return;
		GLint iBrightness = _tstoi( cBrightness );
		if( CBrightness::GetInstance() ) {
			CBrightness::GetInstance()->SetBrightness( iBrightness );
			CWindowData::GetInstance()->SettingFile().iBrightness = iBrightness;
			CWindowData::GetInstance()->SaveSettingFile();
			AddText( ESuccess, _T("Set brightness -> OK") );
		}
		else
			AddText( EError, _T("Set brightness -> failed") );
	}
	else if( _tcscmp( lpCommand, _T("help") ) == 0 ) {
		AddText( _T("") );
		AddText( _T("COMMANDS:") );
		AddText( _T("antialiasing info - get info about antialiasing") );
		AddText( _T("antialiasing X    - set antialiasing, where X = samples, if X = 0 then antialiasing disable") );
		AddText( _T("brightness X      - set brightness, where X is number between -255 and 255") );
		AddText( _T("clear             - clear console") );
		AddText( _T("exit              - close console") );
		AddText( _T("filter all        - show all messages") );
		AddText( _T("filter error      - show only error messages") );
		AddText( _T("filter success    - show only success messages") );
		AddText( _T("filter warning    - show only warning messages") );
		AddText( _T("fps               - show/hide frames per second") );
		AddText( _T("help              - show available commands and keys") );
		AddText( _T("info              - description current OpenGL connection") );
		AddText( _T("log               - on/off logger") );
		AddText( _T("log clear         - on/off clear old log files") );
		AddText( _T("show lng files    - show all language files and index") );
		AddText( _T("lng X             - change language, where X = index") );
		AddText( _T("sdl, sdl_ttf      - show SDL and SDL_ttf version") );
		AddText( _T("textures          - show textures ID with files name") );
		AddText( _T("time              - show local date and time") );
		AddText( _T("") );
		AddText( _T("KEYS:") );
		AddText( _T("[PgUp]   - scroll page up") );
		AddText( _T("[PgDown] - scroll page down") );
		AddText( _T("[UP]     - scroll up") );
		AddText( _T("[DOWN]   - scroll down") );
		AddText( _T("[Home]   - set page on first line") );
		AddText( _T("[End]    - set page on last line") );
		AddText( _T("[Tab]    - last command") );
		AddText( _T("[~]      - open/close console") );
		AddText( _T("[Esc]    - close console") );
	}
	else {
		AddText( EWarning, _T("Unknown command") );
	}
}

GLvoid CConsole::DeleteConsoleLines()
{
	m_iStartLine = 0;
	m_iEndLine = 0;

	for( GLint i = GetConsoleLineSize() - 1; i >= 0; --i )
		delete [] m_aConsoleLine[ i ].lpMsg;

	m_aConsoleLine.clear();
}