
#include "StdAfx.h"
#include "../OpenGLFramework/GameControl.h"
#include "../OpenGLFramework/Console.h"
#include "WGLEXTSwapControl.h"

typedef GLvoid ( APIENTRY *WGLEXTSWAPCONTROLPROC )( GLint );
typedef GLint ( *WGLEXTGETSWAPINTERVALPROC )( GLvoid );

WGLEXTSWAPCONTROLPROC		wglSwapIntervalEXT = NULL;
WGLEXTGETSWAPINTERVALPROC	wglGetSwapIntervalEXT = NULL;

CWGLEXTSwapControl* CWGLEXTSwapControl::Construct()
{
	return CSingletonBase::Construct( new CWGLEXTSwapControl );
}

CWGLEXTSwapControl::CWGLEXTSwapControl()
{
	m_iDefaultSwapInterval = 0;
	m_bIsSupported = InitSwapControl();
}

CWGLEXTSwapControl::~CWGLEXTSwapControl()
{
}

GLboolean CWGLEXTSwapControl::InitSwapControl()
{
	GLchar* cExtensions = ( GLchar* )glGetString( GL_EXTENSIONS );
	if( !strstr( cExtensions, "WGL_EXT_swap_control") ) {
		CONSOLE_ADDTEXT( CConsole::EWarning, _T("WARNING: WGL_EXT_swap_control not supported") );
		return GL_FALSE;
	}

	wglSwapIntervalEXT		= ( WGLEXTSWAPCONTROLPROC )wglGetProcAddress( "wglSwapIntervalEXT" );
	wglGetSwapIntervalEXT	= ( WGLEXTGETSWAPINTERVALPROC )wglGetProcAddress( "wglGetSwapIntervalEXT" );

	//0 - bez VSync, 1 - z VSync
	m_iDefaultSwapInterval = wglGetSwapIntervalEXT();

	CONSOLE_ADDTEXT( CConsole::ESuccess, _T("WGL_EXT_swap_control supported, SwapInterval = %d"), m_iDefaultSwapInterval );

	return GL_TRUE;
	
}

GLvoid CWGLEXTSwapControl::SetSwapInterval( GLint iInterval )
{
	if( m_bIsSupported )
		wglSwapIntervalEXT( iInterval );
}

GLvoid CWGLEXTSwapControl::DefaultVSync()
{
	if( m_bIsSupported )
		wglSwapIntervalEXT( m_iDefaultSwapInterval );
}

GLvoid CWGLEXTSwapControl::EnableVSync()
{
	if( m_bIsSupported )
		wglSwapIntervalEXT( 1 );
}

GLvoid CWGLEXTSwapControl::DisableVSync()
{
	if( m_bIsSupported )
		wglSwapIntervalEXT( 0 );
}