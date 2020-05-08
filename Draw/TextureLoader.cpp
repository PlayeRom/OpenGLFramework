// TextureLoader.cpp: implementation of the CTextureLoader class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "../OpenGLFramework/Console.h"
#include "../OpenGLFramework/GameControl.h"
#include "../Extensions/ARBMultiTexturing.h"
#include "TextureLoader.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTextureLoader::CTextureLoader()
{
	m_iCounterTexture = 0;
}

CTextureLoader::~CTextureLoader()
{
	for( GLint i = GetTextureSize() - 1; i >= 0; --i )
		DeleteTexture( i );

	m_aTexture.clear();
}

GLvoid CTextureLoader::DeleteTexture( GLint iIndex )
{
	if( iIndex < 0 || iIndex >= GetTextureSize() )
		return;

	delete [] m_aTexture[ iIndex ].lpFileName;
	glDeleteTextures( 1, &m_aTexture[ iIndex ].uiTexture );
	m_aTexture.erase( m_aTexture.begin() + iIndex );
	--m_iCounterTexture;
}

GLboolean CTextureLoader::SetTexture( GLint iIndex )
{
	//
	//za pomoca tej funkcji ustwiamy wybrana teksture na aktywna
	//

	glEnable( GL_TEXTURE_2D );

	if( iIndex < 0 || iIndex >= GetTextureSize() ) {
		if( GetTextureSize() > 0 )
			glBindTexture( GL_TEXTURE_2D, m_aTexture[ 0 ].uiTexture );
		return GL_FALSE;
	}
	
	glBindTexture( GL_TEXTURE_2D, m_aTexture[ iIndex ].uiTexture );

	return GL_TRUE;
}

GLboolean CTextureLoader::SetMultiTextures( GLint iIndex )
{
//	glDepthFunc( GL_ALWAYS );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glEnable( GL_BLEND );

	return SetTexture( iIndex );
}

GLint CTextureLoader::CheckExtension( LPCTSTR lpFileName )
{
	GLint iLength = static_cast< GLint >( _tcslen( lpFileName ) );
	if( iLength > 3 ) {
		if( lpFileName[ iLength - 3 ] == 'b' && lpFileName[ iLength - 2 ] == 'm' && lpFileName[ iLength - 1 ] == 'p' )
			return 0;
		if( lpFileName[ iLength - 3 ] == 't' && lpFileName[ iLength - 2 ] == 'g' && lpFileName[ iLength - 1 ] == 'a' )
			return 1;
		if( lpFileName[ iLength - 3 ] == 'j' && lpFileName[ iLength - 2 ] == 'e' && lpFileName[ iLength - 1 ] == 'h' )
			return 2;
	}
	return -1;
}

GLint CTextureLoader::LoadTexMipmaps( LPCTSTR lpFileName )
{
	GLint iResult = LoadGLTex( lpFileName, 0, CheckExtension( lpFileName ) );
	if( CGameControl::GetInstance() ) {
		if( iResult > -1 )
			CONSOLE_ADDTEXT( CConsole::ESuccess, _T("Load texture: \"%s\" -> OK"), lpFileName );
		else
			CONSOLE_ADDTEXT( CConsole::EError, _T("Load texture: \"%s\" -> failed"), lpFileName );
	}

	return iResult;
}

GLint CTextureLoader::LoadTex( LPCTSTR lpFileName )
{
	GLint iResult = LoadGLTex( lpFileName, 1, CheckExtension( lpFileName ) );
	if( CGameControl::GetInstance() ) {
		if( iResult > -1 )
			CONSOLE_ADDTEXT( CConsole::ESuccess, _T("Load texture: \"%s\" -> OK"), lpFileName );
		else
			CONSOLE_ADDTEXT( CConsole::EError, _T("Load texture: \"%s\" -> failed"), lpFileName );
	}

	return iResult;
}

GLint CTextureLoader::LoadTexLowQuality( LPCTSTR lpFileName )
{
	GLint iResult = LoadGLTex( lpFileName, 2, CheckExtension( lpFileName ) );
	if( CGameControl::GetInstance() ) {
		if( iResult > -1 )
			CONSOLE_ADDTEXT( CConsole::ESuccess, _T("Load texture: \"%s\" -> OK"), lpFileName );
		else
			CONSOLE_ADDTEXT( CConsole::EError, _T("Load texture: \"%s\" -> failed"), lpFileName );
	}

	return iResult;
}

GLint CTextureLoader::LoadMultiTexMaskMipmaps( LPCTSTR lpFileNameMask, LPCTSTR lpFileNameRGB )
{
	GLint iResult = LoadGLMultiTexMask( lpFileNameMask, lpFileNameRGB, 0,
										CheckExtension( lpFileNameMask ), CheckExtension( lpFileNameRGB ) );
	if( CGameControl::GetInstance() ) {
		if( iResult > -1 )
			CONSOLE_ADDTEXT( CConsole::ESuccess, _T("Load multitextures: \"%s\", \"%s\" -> OK"), lpFileNameMask, lpFileNameRGB );
		else
			CONSOLE_ADDTEXT( CConsole::EError, _T("Load multitextures: \"%s\", \"%s\" -> failed"), lpFileNameMask, lpFileNameRGB );
	}

	return iResult;
}

GLint CTextureLoader::LoadMultiTexMaskMipmaps( LPCTSTR lpFileNameMask, COLORREF crColor )
{
	GLint iResult = LoadGLMultiTexMask( lpFileNameMask, crColor, 0, CheckExtension( lpFileNameMask ) );
	if( CGameControl::GetInstance() ) {
		if( iResult > -1 )
			CONSOLE_ADDTEXT( CConsole::ESuccess, _T("Load multitextures: \"%s\", RGB(%d, %d, %d) -> OK"),
							 lpFileNameMask, GetRValue( crColor ), GetGValue( crColor ), GetBValue( crColor ) );
		else
			CONSOLE_ADDTEXT( CConsole::EError, _T("Load multitextures: \"%s\", RGB(%d, %d, %d) -> failed"),
							 lpFileNameMask, GetRValue( crColor ), GetGValue( crColor ), GetBValue( crColor ) );
	}

	return iResult;
}

GLint CTextureLoader::LoadMultiTexMask( LPCTSTR lpFileNameMask, LPCTSTR lpFileNameRGB )
{
	GLint iResult = LoadGLMultiTexMask( lpFileNameMask, lpFileNameRGB, 1,
										CheckExtension( lpFileNameMask ), CheckExtension( lpFileNameRGB ) );
	if( CGameControl::GetInstance() ) {
		if( iResult > -1 )
			CONSOLE_ADDTEXT( CConsole::ESuccess, _T("Load multitextures: \"%s\", \"%s\" -> OK"), lpFileNameMask, lpFileNameRGB );
		else
			CONSOLE_ADDTEXT( CConsole::EError, _T("Load multitextures: \"%s\", \"%s\" -> failed"), lpFileNameMask, lpFileNameRGB );
	}

	return iResult;
}

GLint CTextureLoader::LoadMultiTexMask( LPCTSTR lpFileNameMask, COLORREF crColor )
{
	GLint iResult = LoadGLMultiTexMask( lpFileNameMask, crColor, 1, CheckExtension( lpFileNameMask ) );
	if( CGameControl::GetInstance() ) {
		if( iResult > -1 )
			CONSOLE_ADDTEXT( CConsole::ESuccess, _T("Load multitextures: \"%s\", RGB(%d, %d, %d) -> OK"),
							 lpFileNameMask, GetRValue( crColor ), GetGValue( crColor ), GetBValue( crColor ) );
		else
			CONSOLE_ADDTEXT( CConsole::EError, _T("Load multitextures: \"%s\", RGB(%d, %d, %d) -> failed"),
							 lpFileNameMask, GetRValue( crColor ), GetGValue( crColor ), GetBValue( crColor ) );
	}

	return iResult;
}

GLint CTextureLoader::LoadMultiTexMaskLowQuality( LPCTSTR lpFileNameMask, LPCTSTR lpFileNameRGB )
{
	GLint iResult = LoadGLMultiTexMask( lpFileNameMask, lpFileNameRGB, 2,
										CheckExtension( lpFileNameMask ), CheckExtension( lpFileNameRGB ) );
	if( CGameControl::GetInstance() ) {
		if( iResult > -1 )
			CONSOLE_ADDTEXT( CConsole::ESuccess, _T("Load multitextures: \"%s\", \"%s\" -> OK"), lpFileNameMask, lpFileNameRGB );
		else
			CONSOLE_ADDTEXT( CConsole::EError, _T("Load multitextures: \"%s\", \"%s\" -> failed"), lpFileNameMask, lpFileNameRGB );
	}

	return iResult;
}

GLint CTextureLoader::LoadMultiTexMaskLowQuality( LPCTSTR lpFileNameMask, COLORREF crColor )
{
	GLint iResult = LoadGLMultiTexMask( lpFileNameMask, crColor, 2, CheckExtension( lpFileNameMask ) );
	if( CGameControl::GetInstance() ) { 
		if( iResult > -1 )
			CONSOLE_ADDTEXT( CConsole::ESuccess, _T("Load multitextures: \"%s\", RGB(%d, %d, %d) -> OK"),
							 lpFileNameMask, GetRValue( crColor ), GetGValue( crColor ), GetBValue( crColor ) );
		else
			CONSOLE_ADDTEXT( CConsole::EError, _T("Load multitextures: \"%s\", RGB(%d, %d, %d) -> failed"),
							 lpFileNameMask, GetRValue( crColor ), GetGValue( crColor ), GetBValue( crColor ) );
	}

	return iResult;
}

GLint CTextureLoader::LoadEmbossBump( LPCTSTR lpFileName ) //tworzy dwie tekstury, czyli 2 indexy
{
	GLint iResult = LoadGLTex( lpFileName, 0, CheckExtension( lpFileName ), GL_TRUE );
	if( CGameControl::GetInstance() ) {
		if( iResult > -1 )
			CONSOLE_ADDTEXT( CConsole::ESuccess, _T("Load emboss bump: \"%s\" -> OK"), lpFileName );
		else
			CONSOLE_ADDTEXT( CConsole::EError, _T("Load emboss bump: \"%s\" -> failed"), lpFileName );
	}

	return iResult;
}

GLboolean CTextureLoader::LoadBMP( STexture &out_sTexture, LPCTSTR lpFileName )
{
	if( !lpFileName )
		return GL_FALSE;

	BITMAPFILEHEADER sBmpFileHeader;
	BITMAPINFOHEADER sBmpInfoHeader;
	FILE *hFile = NULL;
	if( _tfopen_s( &hFile, lpFileName, _T("rb") ) != 0 )
		return GL_FALSE;

	//wczytaj naglowek pliku
	fread( &sBmpFileHeader, sizeof( BITMAPFILEHEADER ), 1, hFile );
	if( sBmpFileHeader.bfType != 0x4D42 ) { //0x4D42 - identyfikator bitmapy
		fclose( hFile );
		return GL_FALSE;
	}
	//wczytaj informacje o naglowku
	fread( &sBmpInfoHeader, sizeof( BITMAPINFOHEADER ), 1, hFile );

	if( sBmpInfoHeader.biBitCount < 24 ) {
		fclose( hFile );
		return GL_FALSE;
	}

	fseek( hFile, sBmpFileHeader.bfOffBits, SEEK_SET );
	//tworzymy bufor na przechowanie obrazu
	if( sBmpInfoHeader.biSizeImage == 0 ) {
		//sBmpInfoHeader.biSizeImage moze byc 0, wiec musimy obliczyc
		sBmpInfoHeader.biSizeImage = ( sBmpInfoHeader.biBitCount / 8 ) * sBmpInfoHeader.biHeight * sBmpInfoHeader.biWidth;
	}
	if( out_sTexture.ubImageData )
		delete [] out_sTexture.ubImageData;
	out_sTexture.ubImageData = new GLubyte[ sBmpInfoHeader.biSizeImage ];
	if( out_sTexture.ubImageData == NULL ) {
		fclose( hFile );
		return GL_FALSE;
	}

	//wczytaj obraz
	if( fread( out_sTexture.ubImageData, 1, sBmpInfoHeader.biSizeImage, hFile ) != sBmpInfoHeader.biSizeImage ) {
		fclose( hFile );
		delete [] out_sTexture.ubImageData;
		out_sTexture.ubImageData = NULL;
		return GL_FALSE;
	}
	fclose( hFile );

	//jako, ¿e obraz zapisany jest w BGR, zamieniamy bajty aby uzyskaæ RGB
	GLubyte ubTemp = 0;
	for( GLuint i = 0; i < sBmpInfoHeader.biSizeImage; i += ( sBmpInfoHeader.biBitCount / 8 ) ) {
		ubTemp = out_sTexture.ubImageData[ i ];
		out_sTexture.ubImageData[ i ] = out_sTexture.ubImageData[ i + 2 ];
		out_sTexture.ubImageData[ i + 2 ] = ubTemp;
	}

	out_sTexture.uiBitsPerPixel = sBmpInfoHeader.biBitCount;
	out_sTexture.uiWidth = sBmpInfoHeader.biWidth;
	out_sTexture.uiHeight = sBmpInfoHeader.biHeight;
	out_sTexture.uiType = GL_RGB;

	return GL_TRUE;
}

GLboolean CTextureLoader::LoadTGA( STexture &out_sTexture, LPCTSTR lpFileName )
{
	STGAFile tgaImage = { 0 };
	FILE *hFile = NULL;
	if( _tfopen_s( &hFile, lpFileName, _T("rb") ) != 0 )
		return GL_FALSE;

	//wczytujemy nag³ówek TGA:
	if( fread( tgaImage.ubHeader, sizeof( tgaImage.ubHeader ), 1, hFile ) == 0 ) { 
		fclose( hFile );
		return GL_FALSE;
	}
	//sprawdzamy czy format RGB, 1 - ?, 3 - w kolorach szaroœci
	if( tgaImage.ubHeader[ 2 ] != 2 ) {
		fclose( hFile );
		return GL_FALSE;
	}

	out_sTexture.uiWidth = tgaImage.ubHeader[ 13 ] * 256 + tgaImage.ubHeader[ 12 ];
	out_sTexture.uiHeight = tgaImage.ubHeader[ 15 ] * 256 + tgaImage.ubHeader[ 14 ];
	out_sTexture.uiBitsPerPixel = tgaImage.ubHeader[ 16 ];

	tgaImage.uiWidth = out_sTexture.uiWidth;
	tgaImage.uiHeight = out_sTexture.uiHeight;
	tgaImage.uiBitsPerPixel = out_sTexture.uiBitsPerPixel;

	//sprawdzamy poprawnosc rozmiarow i g³êbi kolorów
	if( out_sTexture.uiWidth <= 0 || out_sTexture.uiHeight <= 0
		|| ( out_sTexture.uiBitsPerPixel != 24 && out_sTexture.uiBitsPerPixel != 32 ) ) 
	{
		fclose( hFile );
		return GL_FALSE;
	}
	
	if( out_sTexture.uiBitsPerPixel == 24 )
		out_sTexture.uiType = GL_RGB;
	else
		out_sTexture.uiType = GL_RGBA;
 
	//obliczamy wielkoœæ obrazu i alokujemy dla niego pamiêæ
	tgaImage.uiBytesPerPixel = ( tgaImage.uiBitsPerPixel / 8 );
	tgaImage.uiImageSize = ( tgaImage.uiBytesPerPixel * tgaImage.uiWidth * tgaImage.uiHeight );
	
	if( out_sTexture.ubImageData )
		delete [] out_sTexture.ubImageData;
	out_sTexture.ubImageData = new GLubyte[ tgaImage.uiImageSize ];
	if( out_sTexture.ubImageData == NULL ) { //czy alokacja udana
		fclose( hFile );
		return GL_FALSE;
	}
	//oczytujemy dane obrazu
	if( fread( out_sTexture.ubImageData, 1, tgaImage.uiImageSize, hFile ) != tgaImage.uiImageSize ) {
		fclose( hFile );
		free( out_sTexture.ubImageData );
		return GL_FALSE;
	}
	
	//jako, ¿e obraz zapisany jest w BGR, zamieniamy bajty aby uzyskaæ RGB
	GLubyte ubTemp = 0;
	for( GLuint i = 0; i < tgaImage.uiImageSize; i += tgaImage.uiBytesPerPixel ) {
		ubTemp = out_sTexture.ubImageData[ i ];
		out_sTexture.ubImageData[ i ] = out_sTexture.ubImageData[ i + 2 ];
		out_sTexture.ubImageData[ i + 2 ] = ubTemp;
	}

	fclose( hFile );
	return GL_TRUE;
}

GLboolean CTextureLoader::LoadJEH( STexture &out_sTexture, LPCTSTR lpFileName )
{
	SHeaderJEH sHeaderJEH = { 0 };
	FILE *hFile = NULL;
	if( _tfopen_s( &hFile, lpFileName, _T("rb") ) != 0 )
		return GL_FALSE;

	if( fread( &sHeaderJEH, sizeof( SHeaderJEH ), 1, hFile ) == 0 ) { 
		fclose( hFile );
		return GL_FALSE;
	}

	if( strcmp( sHeaderJEH.cHeader, JEH_HEADER ) != 0 )
		return GL_FALSE;

	out_sTexture.uiWidth = sHeaderJEH.uiWidth;
	out_sTexture.uiHeight = sHeaderJEH.uiHeight;
	out_sTexture.uiType = GL_RGB;
	out_sTexture.uiBitsPerPixel = sHeaderJEH.uiBitCount;

	if( out_sTexture.ubImageData ) {
		delete [] out_sTexture.ubImageData;
		out_sTexture.ubImageData = NULL;
	}

	if( out_sTexture.uiBitsPerPixel == 24 ) {
		out_sTexture.ubImageData = new GLubyte[ sHeaderJEH.uiSizeImage ];
		if( fread( out_sTexture.ubImageData, 1, sHeaderJEH.uiSizeImage, hFile ) == 0 ) { 
			fclose( hFile );
			delete [] out_sTexture.ubImageData;
			return GL_FALSE;
		}
	}
	else if( out_sTexture.uiBitsPerPixel == 16 ) {
		GLubyte *tempBuff = new GLubyte[ sHeaderJEH.uiSizeImage ];
		if( fread( tempBuff, 1, sHeaderJEH.uiSizeImage, hFile ) == 0 ) { 
			fclose( hFile );
			delete [] tempBuff;
			return GL_FALSE;
		}

		//konwertujemy na 24 bity
		std::vector<GLubyte> aImgTemp;
		for( unsigned int i = 0, iModulo = 1; i < sHeaderJEH.uiSizeImage; ++i, ++iModulo ) {
			if( i > 0 && tempBuff[ i ] == 0 ) {
				aImgTemp.push_back( tempBuff[ i - 1 ] );
				aImgTemp.push_back( tempBuff[ i - 1 ] );
				aImgTemp.push_back( tempBuff[ i - 1 ] );
				iModulo = 0;
			}
			else if( iModulo % 3 == 0 ) {
				aImgTemp.push_back( tempBuff[ i - 2 ] );
				aImgTemp.push_back( tempBuff[ i - 1 ] );
				aImgTemp.push_back( tempBuff[ i ] );
			}
		}
		delete [] tempBuff;
		sHeaderJEH.uiSizeImage = static_cast<unsigned int>( aImgTemp.size() );
		out_sTexture.ubImageData = new GLubyte[ sHeaderJEH.uiSizeImage ];
		for( unsigned int i = 0; i < sHeaderJEH.uiSizeImage; ++i )
			out_sTexture.ubImageData[ i ] = aImgTemp[ i ];
		
		aImgTemp.clear();
	}
	else if( out_sTexture.uiBitsPerPixel == 8 ) {
		GLubyte *tempBuff = new GLubyte[ sHeaderJEH.uiSizeImage ];
		if( fread( tempBuff, 1, sHeaderJEH.uiSizeImage, hFile ) == 0 ) { 
			fclose( hFile );
			delete [] tempBuff;
			return GL_FALSE;
		}

		//zamiana 8 bitowej na 24 bitowa
		out_sTexture.ubImageData = new GLubyte[ sHeaderJEH.uiSizeImage * 3 ];
		for( GLuint i = 0, j = 0; i < sHeaderJEH.uiSizeImage; ++i, j += 3 ) {
			out_sTexture.ubImageData[ j ] = tempBuff[ i ];
			out_sTexture.ubImageData[ j + 1 ] = tempBuff[ i ];
			out_sTexture.ubImageData[ j + 2 ] = tempBuff[ i ];
		}

		delete [] tempBuff;
	}

	fclose( hFile );
	return GL_TRUE;
}

GLint CTextureLoader::LoadGLTex( LPCTSTR lpFileName, GLint iMode /* = 0 */, GLint iFormat /* = 0 */,
								 GLboolean bEmbossBump /*= GL_FALSE*/ )
{
	// iMode:
	// = 0 - textura mipmapowa
	// = 1 - textura linear
	// = 2 - textura nearest - najbrzydsza i najszybsza

	// iFormat:
	// = 0 - BMP
	// = 1 - TGA
	// = 2 - JEH

	STexture texTexture = { 0 };
	STextureData texData = { 0 };
	m_aTexture.push_back( texData );
	GLint iIndex = GetTextureSize() - 1;

	m_aTexture[ iIndex ].lpFileName = NULL;

	++m_iCounterTexture;
	if( m_iCounterTexture != GetTextureSize() )
		return -1;

	if( iFormat < 0 || iFormat > 2 )
		return -1;

	if( iFormat == 0 ) { //dla BMP
		if( !LoadBMP( texTexture, lpFileName ) )
			return -1;
	}
	else if( iFormat == 1 ) { //Dla TGA
		if( !LoadTGA( texTexture, lpFileName ) )
			return -1;
	}
	else if( iFormat == 2 ) { //Dla JEH
		if( !LoadJEH( texTexture, lpFileName ) )
			return -1;
	}

	if( bEmbossBump )
		iIndex = CreateEmbossBumpTexture( iIndex, texTexture, iMode );
	else {
		glGenTextures( 1, &m_aTexture[ iIndex ].uiTexture );
		glBindTexture( GL_TEXTURE_2D, m_aTexture[ iIndex ].uiTexture );

		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
		glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

		switch( iMode ) {
			case 1:
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
				glTexImage2D( GL_TEXTURE_2D, 0, 3, texTexture.uiWidth, texTexture.uiHeight, 0, texTexture.uiType,
							  GL_UNSIGNED_BYTE, texTexture.ubImageData );
				break;
			case 2:
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST ); 
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST ); 
				glTexImage2D( GL_TEXTURE_2D, 0, 3, texTexture.uiWidth, texTexture.uiHeight, 0, texTexture.uiType,
							  GL_UNSIGNED_BYTE, texTexture.ubImageData );
				break;
			default:
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST ); 
				gluBuild2DMipmaps( GL_TEXTURE_2D, 3, texTexture.uiWidth, texTexture.uiHeight, texTexture.uiType,
								   GL_UNSIGNED_BYTE, texTexture.ubImageData );
				break;
		}
	}

	if( texTexture.ubImageData )
		delete [] texTexture.ubImageData;

	size_t uiSize = _tcslen( lpFileName ) + 1;
	m_aTexture[ iIndex ].lpFileName = new TCHAR[ uiSize ];
	_tcscpy_s( m_aTexture[ iIndex ].lpFileName, uiSize, lpFileName );

	return iIndex;
}

GLint CTextureLoader::CreateEmbossBumpTexture( GLint iIndex, STexture &texTexture, GLint iMode )
{
	GLfloat aGray[] = { 0.5f, 0.5f, 0.5f, 1.0f };

	glPixelTransferf( GL_RED_SCALE, 0.5f );			// Scale RGB By 50%, So That We Have Only			
	glPixelTransferf( GL_GREEN_SCALE, 0.5f );			// Half Intenstity
	glPixelTransferf( GL_BLUE_SCALE, 0.5f );

	glGenTextures( 1, &m_aTexture[ iIndex ].uiTexture );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );	// No Wrapping, Please!
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
	glTexParameterfv( GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, aGray );

	glBindTexture( GL_TEXTURE_2D, m_aTexture[ iIndex ].uiTexture );

	switch( iMode ) {
		case 1:
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
			glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB8, texTexture.uiWidth, texTexture.uiHeight, 0, texTexture.uiType,
						  GL_UNSIGNED_BYTE, texTexture.ubImageData );
			break;
		case 2:
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST ); 
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST ); 
			glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB8, texTexture.uiWidth, texTexture.uiHeight, 0, texTexture.uiType,
						  GL_UNSIGNED_BYTE, texTexture.ubImageData );
			break;
		default:
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );//NEAREST 
			gluBuild2DMipmaps( GL_TEXTURE_2D, GL_RGB8, texTexture.uiWidth, texTexture.uiHeight, texTexture.uiType,
							   GL_UNSIGNED_BYTE, texTexture.ubImageData );
			break;
	}

	//tworzymy druga teksture - negatyw tekstury bump
	for( GLuint i = 0; i < 3 * texTexture.uiWidth * texTexture.uiHeight; ++i )
		texTexture.ubImageData[ i ] = 255 - texTexture.ubImageData[ i ]; //negatyw

	STextureData texDataInvert = { 0 };
	m_aTexture.push_back( texDataInvert );
	GLint iIndexInvert = GetTextureSize() - 1;

	++m_iCounterTexture;
	if( m_iCounterTexture != GetTextureSize() )
		return -1;

	glGenTextures( 1, &m_aTexture[ iIndexInvert ].uiTexture );
	glBindTexture( GL_TEXTURE_2D, m_aTexture[ iIndexInvert ].uiTexture );

	switch( iMode ) {
		case 1:
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
			glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB8, texTexture.uiWidth, texTexture.uiHeight, 0,
						  texTexture.uiType, GL_UNSIGNED_BYTE, texTexture.ubImageData );
			break;
		case 2:
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST ); 
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST ); 
			glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB8, texTexture.uiWidth, texTexture.uiHeight, 0,
						  texTexture.uiType, GL_UNSIGNED_BYTE, texTexture.ubImageData );
			break;
		default:
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );//NEAREST 
			gluBuild2DMipmaps( GL_TEXTURE_2D, GL_RGB8, texTexture.uiWidth, texTexture.uiHeight,
							   texTexture.uiType, GL_UNSIGNED_BYTE, texTexture.ubImageData );
			break;
	}

	glPixelTransferf( GL_RED_SCALE, 1.0f );	// Scale RGB Back To 100% Again		
	glPixelTransferf( GL_GREEN_SCALE, 1.0f );			
	glPixelTransferf( GL_BLUE_SCALE, 1.0f );

	TCHAR *lpFileName = _T("Invert emboss bump");

	size_t uiSize = _tcslen( lpFileName ) + 1;
	m_aTexture[ iIndexInvert ].lpFileName = new TCHAR[ uiSize ];
	_tcscpy_s( m_aTexture[ iIndexInvert ].lpFileName, uiSize, lpFileName );

	return iIndex;
}

GLint CTextureLoader::LoadGLMultiTexMask( LPCTSTR lpFileName1, LPCTSTR lpFileName2, GLint iMode /* = 0 */,
										  GLint iFormat1 /* = 0 */, GLint iFormat2 /* = 0 */ )
{
	// iMode:
	// = 0 - textura mipmapowa
	// = 1 - textura linear
	// = 2 - textura nearest - najbrzydsza i najszybsza

	// iFormat:
	// = 0 - BMP
	// = 1 - TGA
	// = 2 - JEH

	STexture texTexture = { 0 };
	GLubyte *lpAlpha = NULL;

	STextureData texData  = { 0 };
	m_aTexture.push_back( texData );
	GLint iIndex = GetTextureSize() - 1;

	m_aTexture[ iIndex ].lpFileName = NULL;

	++m_iCounterTexture;
	if( m_iCounterTexture != GetTextureSize() )
		return -1;

	if( iFormat1 < 0 || iFormat1 > 2 || iFormat2 < 0 || iFormat2 > 2 ) return -1;
	if( iFormat1 != iFormat2 ) return -1;

	if( iFormat1 == 0 ) { //dla BMP
		if( !LoadBMP( texTexture, lpFileName1 ) )
			return -1;

		lpAlpha = new GLubyte[ 4 * texTexture.uiWidth * texTexture.uiHeight ];
		for( GLuint i = 0; i < texTexture.uiWidth * texTexture.uiHeight; ++i )
			lpAlpha[ 4 * i + 3 ] = texTexture.ubImageData[ i * 3 ];	// zapisujemy skladowa R (nieistotne ktora bo i tak odcienie szarosci ) do alpha

		if( !LoadBMP( texTexture, lpFileName2 ) ) {
			delete [] lpAlpha;
			return -1;
		}
	}
	else if( iFormat1 == 1 ) { //Dla TGA
		if( !LoadTGA( texTexture, lpFileName1 ) )
			return -1;

		lpAlpha = new GLubyte[ 4 * texTexture.uiWidth * texTexture.uiHeight ];
		for( GLuint i = 0; i < texTexture.uiWidth * texTexture.uiHeight; ++i )
			lpAlpha[ 4 * i + 3 ] = texTexture.ubImageData[ i * 3 ];	// zapisujemy skladowa R (nieistotne ktora bo i tak odcienie szarosci ) do alpha

		if( !LoadTGA( texTexture, lpFileName2 ) ) {
			delete [] lpAlpha;
			return -1;
		}
	}
	else if( iFormat1 == 2 ) { //dla JEH
		if( !LoadJEH( texTexture, lpFileName1 ) )
			return -1;

		lpAlpha = new GLubyte[ 4 * texTexture.uiWidth * texTexture.uiHeight ];
		for( GLuint i = 0; i < texTexture.uiWidth * texTexture.uiHeight; ++i )
			lpAlpha[ 4 * i + 3 ] = texTexture.ubImageData[ i * 3 ];	// zapisujemy skladowa R (nieistotne ktora bo i tak odcienie szarosci ) do alpha

		if( !LoadJEH( texTexture, lpFileName2 ) ) {
			delete [] lpAlpha;
			return -1;
		}
	}

	for( GLuint i = 0; i < texTexture.uiWidth * texTexture.uiHeight; ++i ) {
		//zapisujemy pozostale kolory do tablicy lpAlpha
		lpAlpha[ 4 * i ] = texTexture.ubImageData[ i * 3 ];					// R
		lpAlpha[ 4 * i + 1 ] = texTexture.ubImageData[ i * 3 + 1 ];			// G
		lpAlpha[ 4 * i + 2 ] = texTexture.ubImageData[ i * 3 + 2 ];			// B
	}

	glGenTextures( 1, &m_aTexture[ iIndex ].uiTexture );

	glBindTexture( GL_TEXTURE_2D, m_aTexture[ iIndex ].uiTexture );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

	switch( iMode ) {
		case 1:
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
			glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA8, texTexture.uiWidth, texTexture.uiHeight, 0, GL_RGBA,
						  GL_UNSIGNED_BYTE, lpAlpha );
			break;
		case 2:
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST ); 
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST ); 
			glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA8, texTexture.uiWidth, texTexture.uiHeight, 0, GL_RGBA,
						  GL_UNSIGNED_BYTE, lpAlpha );
			break;
		default:
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );
			gluBuild2DMipmaps( GL_TEXTURE_2D, GL_RGBA8, texTexture.uiWidth, texTexture.uiHeight, GL_RGBA,
							   GL_UNSIGNED_BYTE, lpAlpha );
			break;
	}

	if( texTexture.ubImageData )
		delete [] texTexture.ubImageData;

	delete [] lpAlpha;

	m_aTexture[ iIndex ].lpFileName = new TCHAR[ _tcslen( lpFileName1 ) + _tcslen( lpFileName2 ) + 2 ];
	::wsprintf( m_aTexture[ iIndex ].lpFileName, _T("%s %s"), lpFileName1, lpFileName2 );

	return iIndex;
}

GLint CTextureLoader::LoadGLMultiTexMask( LPCTSTR lpFileNameMask, COLORREF crColor,
										  GLint iMode /* = 0 */, GLint iFormat /* = 0 */ )
{
	// iMode:
	// = 0 - textura mipmapowa
	// = 1 - textura linear
	// = 2 - textura nearest - najbrzydsza i najszybsza

	// iFormat:
	// = 0 - BMP
	// = 1 - TGA
	// = 2 - JEH

	STexture texTexture = { 0 };
	STextureData texData = { 0 };
	m_aTexture.push_back( texData );
	GLint iIndex = GetTextureSize() - 1;

	m_aTexture[ iIndex ].lpFileName = NULL;

	++m_iCounterTexture;
	if( m_iCounterTexture != GetTextureSize() )
		return -1;

	if( iFormat < 0 || iFormat > 2 ) return -1;

	if( iFormat == 0 ) { //dla BMP
		if( !LoadBMP( texTexture, lpFileNameMask ) )
			return -1;
	}
	else if( iFormat == 1 ) { //Dla TGA
		if( !LoadTGA( texTexture, lpFileNameMask ) )
			return -1;
	}
	else if( iFormat == 2 ) { //Dla JAH
		if( !LoadJEH( texTexture, lpFileNameMask ) )
			return -1;
	}

	GLubyte *lpAlpha = new GLubyte[ 4 * texTexture.uiWidth * texTexture.uiHeight ];
	for( GLuint i = 0; i < texTexture.uiWidth * texTexture.uiHeight; ++i ) {
		//zapisujemy pozostale kolory do tablicy lpAlpha
		lpAlpha[ 4 * i ] = GetRValue( crColor );		// R
		lpAlpha[ 4 * i + 1 ] = GetGValue( crColor );	// G
		lpAlpha[ 4 * i + 2 ] = GetBValue( crColor );	// B
		lpAlpha[ 4 * i + 3 ] = texTexture.ubImageData[ i * 3 ];	// zapisujemy skladowa R (nieistotne ktora bo i tak odcienie szarosci ) do alpha
	}

	glGenTextures( 1, &m_aTexture[ iIndex ].uiTexture );

	glBindTexture( GL_TEXTURE_2D, m_aTexture[ iIndex ].uiTexture );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

	switch( iMode ) {
		case 1:
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
			glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA8, texTexture.uiWidth, texTexture.uiHeight, 0, GL_RGBA,
						  GL_UNSIGNED_BYTE, lpAlpha );
			break;
		case 2:
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
			glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA8, texTexture.uiWidth, texTexture.uiHeight, 0, GL_RGBA,
						  GL_UNSIGNED_BYTE, lpAlpha );
			break;
		default:
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );
			gluBuild2DMipmaps( GL_TEXTURE_2D, GL_RGBA8, texTexture.uiWidth, texTexture.uiHeight, GL_RGBA,
							   GL_UNSIGNED_BYTE, lpAlpha );
			break;
	}

	if( texTexture.ubImageData )
		delete [] texTexture.ubImageData;

	delete [] lpAlpha;

	m_aTexture[ iIndex ].lpFileName = new TCHAR[ _tcslen( lpFileNameMask ) + 20 ];
	::wsprintf( m_aTexture[ iIndex ].lpFileName,
				_T("%s RGB(%d, %d, %d)"),
				lpFileNameMask, GetRValue( crColor ), GetGValue( crColor ), GetBValue( crColor ) );

	return iIndex;
}

GLvoid CTextureLoader::DisplayIDTextureName()
{
	if( !CGameControl::GetInstance()->GetConsole() )
		return;

	for( GLint i = 0; i < GetTextureSize(); ++i )
		CONSOLE_ADDTEXT( _T("ID: %3d -> \"%s\""), i, m_aTexture[ i ].lpFileName );
}