// ARBMultiTexuring.cpp: implementation of the CARBMultiTexuring class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../OpenGLFramework/GameControl.h"
#include "../OpenGLFramework/Console.h"
#include "../Draw/TextureLoader.h"
#include "ARBMultiTexturing.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

PFNGLMULTITEXCOORD1FARBPROC		glMultiTexCoord1fARB	= NULL;
PFNGLMULTITEXCOORD2FARBPROC		glMultiTexCoord2fARB	= NULL;
PFNGLMULTITEXCOORD3FARBPROC		glMultiTexCoord3fARB	= NULL;
PFNGLMULTITEXCOORD4FARBPROC		glMultiTexCoord4fARB	= NULL;

PFNGLACTIVETEXTUREARBPROC       glActiveTextureARB       = NULL;
PFNGLCLIENTACTIVETEXTUREARBPROC glClientActiveTextureARB = NULL;

CARBMultiTexturing* CARBMultiTexturing::Construct()
{
	return CSingletonBase::Construct( new CARBMultiTexturing );
}

CARBMultiTexturing::CARBMultiTexturing()
{
	m_iMaxTexUnits = 1;
	m_bIsSupported = InitMultiTexturing();
}

CARBMultiTexturing::~CARBMultiTexturing()
{
}

GLboolean CARBMultiTexturing::InitMultiTexturing()
{
	GLchar* cExtStr = ( GLchar* )glGetString( GL_EXTENSIONS );
	
	if( cExtStr && !strstr( cExtStr, "GL_ARB_multitexture" ) && !strstr( cExtStr, "GL_ARB_texture_env_combine" ) ) {
		CONSOLE_ADDTEXT( CConsole::EWarning, _T("WARNING: Multitexturing not supported") );
		return GL_FALSE;
	}
	
	glGetIntegerv( GL_MAX_TEXTURE_UNITS_ARB, &m_iMaxTexUnits );
	CONSOLE_ADDTEXT( _T("Max texture units: %d"), m_iMaxTexUnits );

	glMultiTexCoord1fARB = ( PFNGLMULTITEXCOORD1FARBPROC )wglGetProcAddress( "glMultiTexCoord1fARB" );
	glMultiTexCoord2fARB = ( PFNGLMULTITEXCOORD2FARBPROC )wglGetProcAddress( "glMultiTexCoord2fARB" );
	glMultiTexCoord3fARB = ( PFNGLMULTITEXCOORD3FARBPROC )wglGetProcAddress( "glMultiTexCoord3fARB" );
	glMultiTexCoord4fARB = ( PFNGLMULTITEXCOORD4FARBPROC )wglGetProcAddress( "glMultiTexCoord4fARB" );

	glActiveTextureARB       = ( PFNGLACTIVETEXTUREARBPROC )wglGetProcAddress( "glActiveTextureARB" );
	glClientActiveTextureARB = ( PFNGLCLIENTACTIVETEXTUREARBPROC )wglGetProcAddress( "glClientActiveTextureARB" );
	
	return GL_TRUE;
}

GLboolean CARBMultiTexturing::BindMultiTextures( GLint iFirstTex, GLsizei iCount /*=2*/, GLboolean bEmbossBump /*= GL_FALSE*/ )
{
	if( !m_bIsSupported )
		return GL_FALSE;

	if( iFirstTex < 0 || iFirstTex >= CGameControl::GetInstance()->GetTexLoader()->GetTextureSize() 
		|| iCount < 2 || iFirstTex + iCount > CGameControl::GetInstance()->GetTexLoader()->GetTextureSize() )
		return GL_FALSE;

	if( bEmbossBump && iCount != 2 )
		return GL_FALSE;

	//zapamietujemy dla DisableMultiTeksturing
	m_iCountUnit = iCount;

	for( GLint i = 0; i < iCount; ++i ) {
		ActiveTextureARB( i );
		CGameControl::GetInstance()->GetTexLoader()->SetTexture( iFirstTex + i );
		if( bEmbossBump ) {
			if( i == 0 ) {
				glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT );
				glTexEnvf( GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_REPLACE );
			}
			else { //if( i == 1 )
				glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT );
				glTexEnvf( GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_ADD );
			}
		}
		else
			glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
	}
	return GL_TRUE;
}

GLboolean CARBMultiTexturing::ClientActiveTextureARB( GLint iIndex )
{
	if( !m_bIsSupported || iIndex < 0 || iIndex >= m_iMaxTexUnits )
		return GL_FALSE;

	glClientActiveTextureARB( GL_TEXTURE0_ARB + iIndex );

	return GL_TRUE;
}

GLboolean CARBMultiTexturing::ActiveTextureARB( GLint iIndex /*= 0*/ )
{
	if( !m_bIsSupported || iIndex < 0 || iIndex >= m_iMaxTexUnits )
		return GL_FALSE;

	glActiveTextureARB( GL_TEXTURE0_ARB + iIndex );
	glEnable( GL_TEXTURE_2D );

	return GL_TRUE;
}

GLboolean CARBMultiTexturing::InactiveTextureARB( GLint iIndex /*= 0*/ )
{
	if( !m_bIsSupported || iIndex < 0 || iIndex >= m_iMaxTexUnits )
		return GL_FALSE;

	glActiveTextureARB( GL_TEXTURE0_ARB + iIndex );
	glDisable( GL_TEXTURE_2D );

	return GL_TRUE;
}

GLvoid CARBMultiTexturing::DisableMultiTeksturing()
{
	GLint iStart = m_iCountUnit;
	if( iStart < 1 || iStart >= m_iMaxTexUnits )
		iStart = m_iMaxTexUnits - 1;

	for( GLint i = iStart; i > 0; --i )
		InactiveTextureARB( i );

	ActiveTextureARB( 0 );
	ClientActiveTextureARB( 0 );
}

GLboolean CARBMultiTexturing::MultiTexCoord2fARB( GLint iIndex, GLfloat s, GLfloat t )
{
	if( !m_bIsSupported || iIndex < 0 || iIndex >= m_iMaxTexUnits )
		return GL_FALSE;

	glMultiTexCoord2fARB( GL_TEXTURE0_ARB + iIndex, s, t );

	return GL_TRUE;
}

GLboolean CARBMultiTexturing::ActiveOnlyOneUnit( GLint iIndex )
{
	if( !m_bIsSupported )
		return GL_FALSE;

	//dezaktywuj wszystkie
	for( GLint i = m_iMaxTexUnits - 1; i >= 0; --i ) {
		if( i == iIndex ) {
			//aktywuj tylko nasza zadana
			ActiveTextureARB( iIndex );
			ClientActiveTextureARB( iIndex );
		}
		else
			InactiveTextureARB( i );
	}

	return GL_TRUE;
}