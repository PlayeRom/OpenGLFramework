// ARBMultisample.cpp: implementation of the CARBMultisample class.
//
//////////////////////////////////////////////////////////////////////
/*========================================================================================

  Name:   ARB_multisample.cpp
	Author: Colt "MainRoach" McAnlis
	Modification: PlayeRom
	Date:   4/29/04
	Desc:   This file contains the context to load a WGL extension from a string
		    As well as collect the sample format available based upon the graphics card.

========================================================================================*/

#include "stdafx.h"
#include "ARBMultisample.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CARBMultisample::CARBMultisample()
{
	m_bMultisampleSupported = GL_FALSE;
	m_iMultisampleFormat	= 0;
	m_iAntialiasingSamples	= 0; // 0, 2, 4, 6
}

CARBMultisample::~CARBMultisample()
{
}

// WGLisExtensionSupported: This Is A Form Of The Extension For WGL
GLboolean CARBMultisample::WGLisExtensionSupported( const char *lpExtension )
{
	const size_t uiExtLen = strlen( lpExtension );
	const GLchar *lpSupported = NULL;

	// Try To Use wglGetExtensionStringARB On Current DC, If Possible
	PROC wglGetExtString = wglGetProcAddress( "wglGetExtensionsStringARB" );

	if( wglGetExtString )
		lpSupported = ( ( GLchar*( __stdcall* )( HDC ) )wglGetExtString )( wglGetCurrentDC() );

	// If That Failed, Try Standard Opengl Extensions String
	if( !lpSupported )
		lpSupported = ( GLchar* )glGetString( GL_EXTENSIONS );

	// If That Failed Too, Must Be No Extensions Supported
	if( !lpSupported )
		return GL_FALSE;

	// Begin Examination At Start Of String, Increment By 1 On False Match
	for( const GLchar* ptr = lpSupported; ; ++ptr )
	{
		// Advance p Up To The Next Possible Match
		ptr = strstr( ptr, lpExtension );

		if( ptr == NULL )
			return GL_FALSE;		// No Match

		// Make Sure That Match Is At The Start Of The String Or That
		// The Previous Char Is A Space, Or Else We Could Accidentally
		// Match "wglFunkywglExtension" With "wglExtension"

		// Also, Make Sure That The Following Character Is Space Or NULL
		// Or Else "wglExtensionTwo" Might Match "wglExtension"
		if( ( ptr == lpSupported || ptr[ -1 ] == ' ' ) && ( ptr[ uiExtLen ] == '\0' || ptr[ uiExtLen ] == ' ' ) )
			return GL_TRUE;			// Match
	}
}

// InitMultisample: Used To Query The Multisample Frequencies
GLboolean CARBMultisample::InitMultisample( HWND hWnd )
{
	 // See If The String Exists In WGL!
	if( !WGLisExtensionSupported( "WGL_ARB_multisample" ) ) {
		m_bMultisampleSupported = GL_FALSE;
		m_iAntialiasingSamples = -1;
		//::MessageBox( hWnd, _T("Multisample not supported!"), GAME_NAME, MB_OK | MB_ICONEXCLAMATION );
		__LOG( _T("CARBMultisample::InitMultisample - Multisample not supported!") );
		return GL_FALSE;
	}

	// Get Our Pixel Format
	PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = ( PFNWGLCHOOSEPIXELFORMATARBPROC )wglGetProcAddress( "wglChoosePixelFormatARB" );	
	if( !wglChoosePixelFormatARB ) {
		m_bMultisampleSupported = GL_FALSE;
		return GL_FALSE;
	}

	// Get Our Current Device Context
	HDC hDC = ::GetDC( hWnd );

	GLint	iPixelFormat;
	GLint	iValid;
	GLuint	uiNumFormats;
	GLfloat	fAttributes[] = { 0, 0 };

	// These Attributes Are The Bits We Want To Test For In Our Sample
	// Everything Is Pretty Standard, The Only One We Want To 
	// Really Focus On Is The SAMPLE BUFFERS ARB And WGL SAMPLES
	// These Two Are Going To Do The Main Testing For Whether Or Not
	// We Support Multisampling On This Hardware.
	GLint iAttributes[] =
	{
		WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
		WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
		WGL_ACCELERATION_ARB,	WGL_FULL_ACCELERATION_ARB,
		WGL_COLOR_BITS_ARB,		CWindowData::GetInstance()->SettingFile().ubBitsPerPixel,
		WGL_ALPHA_BITS_ARB,		8,
		WGL_DEPTH_BITS_ARB,		CWindowData::GetInstance()->GetZBufferBits(),
		WGL_STENCIL_BITS_ARB,	1,
		WGL_DOUBLE_BUFFER_ARB,	GL_TRUE,
		WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,
		WGL_SAMPLES_ARB,		m_iAntialiasingSamples,
		0,						0
	};

	//sprawdzamy czy wspierane sa koleje probki, od najwiekszej ilosc po najmniejsza
	for( GLint i = m_iAntialiasingSamples; i >= 2; i -= 2 ) {
		iAttributes[ 19 ] = i;
		iValid = wglChoosePixelFormatARB( hDC, iAttributes, fAttributes, 1, &iPixelFormat, &uiNumFormats );
		
		if( iValid && uiNumFormats >= 1 ) {
			m_bMultisampleSupported = GL_TRUE;
			m_iMultisampleFormat = iPixelFormat;
			m_iAntialiasingSamples = i;
			
			__LOG_FORMATTED( _T("CARBMultisample::InitMultisample - antialiasing supported, samples x%d"), m_iAntialiasingSamples );

			return m_bMultisampleSupported;
		}
	}

	// Return The Valid Format
	return m_bMultisampleSupported;
}
