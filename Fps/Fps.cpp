// Fps.cpp: implementation of the CFps class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "../OpenGLFramework/WindowData.h"
#include "Fps.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFps::CFps()
{
	m_pSDLFont = new CSDLFont( "fonts/FreeSans.ttf", 14 );

	m_bShowFps = GL_FALSE;
	if( CWindowData::GetInstance()->IsCommandLine( _T("/fps") ) )
		m_bShowFps = GL_TRUE;

	m_uiFps = 0;
	SetTextFps();
}

CFps::~CFps()
{
	if( m_pSDLFont ) {
		delete m_pSDLFont;
		m_pSDLFont = NULL;
	}
}
