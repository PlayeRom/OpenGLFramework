
#include "StdAfx.h"
#include "../OpenGLFramework/GameControl.h"
#include "Brightness.h"

CBrightness* CBrightness::Construct()
{
	return CSingletonBase::Construct( new CBrightness );
}

CBrightness::CBrightness()
{
	m_iBrightness = 0;
	m_ubBrightness = 0;
}

CBrightness::~CBrightness()
{
}

GLvoid CBrightness::SetBrightness( GLint iBrightness )
{
	if( iBrightness < -255 )
		iBrightness = -255;
	if( iBrightness > 255 )
		iBrightness = 255;

	m_iBrightness = iBrightness;
	if( m_iBrightness >= 0 )
		m_ubBrightness = static_cast<GLubyte>( m_iBrightness );
	else
		m_ubBrightness = static_cast<GLubyte>( -m_iBrightness );
}

GLvoid CBrightness::Draw()
{
	if( m_iBrightness == 0 )
		return; //nie rysuj bo po co

	glLoadIdentity();
	CGameControl::GetInstance()->Enable2D();
	glDisable( GL_LIGHTING );
	glDisable( GL_DEPTH_TEST );
	glDisable( GL_TEXTURE_2D );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glEnable( GL_BLEND );

	if( m_iBrightness > 0 )
		glColor4ub( 255, 255, 255, m_ubBrightness );
	else
		glColor4ub( 0, 0, 0, m_ubBrightness );

	CGameControl::GetInstance()->DrawQuadOnAllScreen();

	glDisable( GL_BLEND );

	//wroc do narmalnego koloru
	glColor4ub( 255, 255, 255, 255 );
}