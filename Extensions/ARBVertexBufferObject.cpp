
#include "StdAfx.h"
#include "../OpenGLFramework/GameControl.h"
#include "../OpenGLFramework/Console.h"
#include "ARBVertexBufferObject.h"

PFNGLBINDBUFFERARBPROC		glBindBufferARB = NULL;
PFNGLGENBUFFERSARBPROC		glGenBuffersARB = NULL;
PFNGLISBUFFERARBPROC		glIsBufferARB = NULL;
PFNGLBUFFERDATAARBPROC		glBufferDataARB = NULL;
PFNGLDELETEBUFFERSARBPROC	glDeleteBuffersARB = NULL;

CARBVertexBufferObject::CARBVertexBufferObject()
{
	m_pGameCtrl = CGameControl::GetInstance();

	m_bUse = GL_TRUE;
	m_uiVerticesBuffer = 0;
	m_uiNormalsBuffer = 0;
	m_uiColorsBuffer = 0;
	m_uiTexCoordsBuffer = 0;
	m_bIsSupported = InitExtension();
}

CARBVertexBufferObject::~CARBVertexBufferObject()
{
}

GLboolean CARBVertexBufferObject::InitExtension()
{
	GLchar *cExtStr = ( GLchar* )glGetString( GL_EXTENSIONS );
	
	if( !strstr( cExtStr, "GL_ARB_vertex_buffer_object") ) {
		CONSOLE_ADDTEXT( CConsole::EWarning, _T("WARNING: GL_ARB_vertex_buffer_object not supported") );
		return GL_FALSE;
	}
	
	glBindBufferARB		= ( PFNGLBINDBUFFERARBPROC )wglGetProcAddress( "glBindBufferARB" );
	glGenBuffersARB		= ( PFNGLGENBUFFERSARBPROC )wglGetProcAddress( "glGenBuffersARB" );
	glIsBufferARB		= ( PFNGLISBUFFERARBPROC )wglGetProcAddress( "glIsBufferARB" );
	glBufferDataARB		= ( PFNGLBUFFERDATAARBPROC )wglGetProcAddress( "glBufferDataARB" );
	glDeleteBuffersARB	= ( PFNGLDELETEBUFFERSARBPROC )wglGetProcAddress( "glDeleteBuffersARB" );

	return GL_TRUE;
}

GLvoid CARBVertexBufferObject::SetUse( GLboolean bUse )
{
	m_bUse = bUse;
}

GLboolean CARBVertexBufferObject::IsUse()
{
	return m_bUse;
}

GLboolean CARBVertexBufferObject::IsSupported()
{
	return m_bIsSupported;
}

GLvoid CARBVertexBufferObject::CreateVerticesBuffer( const GLvoid *lpVoid, GLsizei iSize )
{
	if( !m_bUse || !m_bIsSupported )
		return;

	if( m_uiVerticesBuffer )
		DeleteVerticesBuffers();

	glGenBuffersARB( 1, &m_uiVerticesBuffer );
	if( glIsBufferARB( m_uiVerticesBuffer ) )
		CONSOLE_ADDTEXT( CConsole::EWarning, _T("WARNING: CreateVerticesBuffer failed") );

	glBindBufferARB( GL_ARRAY_BUFFER_ARB, m_uiVerticesBuffer );
	glBufferDataARB( GL_ARRAY_BUFFER_ARB, iSize, lpVoid, GL_STATIC_DRAW_ARB );
}

GLboolean CARBVertexBufferObject::BindVerticesBuffer()
{
	if( !m_bUse || !m_bIsSupported )
		return GL_FALSE;

	glBindBufferARB( GL_ARRAY_BUFFER_ARB, m_uiVerticesBuffer );

	return GL_TRUE;
}

GLvoid CARBVertexBufferObject::DeleteVerticesBuffers()
{
	if( !m_bUse || !m_bIsSupported )
		return;

	glDeleteBuffersARB( 1, &m_uiVerticesBuffer );
	m_uiVerticesBuffer = 0;
}

GLvoid CARBVertexBufferObject::CreateNormalsBuffer( const GLvoid *lpVoid, GLsizei iSize )
{
	if( !m_bUse || !m_bIsSupported )
		return;

	if( m_uiNormalsBuffer )
		DeleteNormalsBuffers();

	glGenBuffersARB( 1, &m_uiNormalsBuffer );
	if( glIsBufferARB( m_uiNormalsBuffer ) )
		CONSOLE_ADDTEXT( CConsole::EWarning, _T("WARNING: CreateNormalsBuffer failed") );

	glBindBufferARB( GL_ARRAY_BUFFER_ARB, m_uiNormalsBuffer );
	glBufferDataARB( GL_ARRAY_BUFFER_ARB, iSize, lpVoid, GL_STATIC_DRAW_ARB );
}

GLboolean CARBVertexBufferObject::BindNormalsBuffer()
{
	if( !m_bUse || !m_bIsSupported )
		return GL_FALSE;

	glBindBufferARB( GL_ARRAY_BUFFER_ARB, m_uiNormalsBuffer );
	
	return GL_TRUE;
}

GLvoid CARBVertexBufferObject::DeleteNormalsBuffers()
{
	if( !m_bUse || !m_bIsSupported )
		return;

	glDeleteBuffersARB( 1, &m_uiNormalsBuffer );
	m_uiNormalsBuffer = 0;
}

GLvoid CARBVertexBufferObject::CreateColorsBuffer( const GLvoid *lpVoid, GLsizei iSize )
{
	if( !m_bUse || !m_bIsSupported )
		return;

	if( m_uiColorsBuffer )
		DeleteColorsBuffers();

	glGenBuffersARB( 1, &m_uiColorsBuffer );
	if( glIsBufferARB( m_uiColorsBuffer ) )
		CONSOLE_ADDTEXT( CConsole::EWarning, _T("WARNING: CreateColorsBuffer failed") );
	
	glBindBufferARB( GL_ARRAY_BUFFER_ARB, m_uiColorsBuffer );
	glBufferDataARB( GL_ARRAY_BUFFER_ARB, iSize, lpVoid, GL_STATIC_DRAW_ARB );
}

GLboolean CARBVertexBufferObject::BindColorsBuffer()
{
	if( !m_bUse || !m_bIsSupported )
		return GL_FALSE;

	glBindBufferARB( GL_ARRAY_BUFFER_ARB, m_uiColorsBuffer );
	return GL_TRUE;
}

GLvoid CARBVertexBufferObject::DeleteColorsBuffers()
{
	if( !m_bUse || !m_bIsSupported )
		return;

	glDeleteBuffersARB( 1, &m_uiColorsBuffer );
	m_uiColorsBuffer = 0;
}

GLvoid CARBVertexBufferObject::CreateTexCoordsBuffer( const GLvoid *lpVoid, GLsizei iSize )
{
	if( !m_bUse || !m_bIsSupported )
		return;

	if( m_uiTexCoordsBuffer )
		DeleteTexCoordsBuffers();

	glGenBuffersARB( 1, &m_uiTexCoordsBuffer );
	if( glIsBufferARB( m_uiTexCoordsBuffer ) )
		CONSOLE_ADDTEXT( CConsole::EWarning, _T("WARNING: CreateTexCoordsBuffer failed") );

	glBindBufferARB( GL_ARRAY_BUFFER_ARB, m_uiTexCoordsBuffer );
	glBufferDataARB( GL_ARRAY_BUFFER_ARB, iSize, lpVoid, GL_STATIC_DRAW_ARB );
}

GLboolean CARBVertexBufferObject::BindTexCoordsBuffer()
{
	if( !m_bUse || !m_bIsSupported )
		return GL_FALSE;

	glBindBufferARB( GL_ARRAY_BUFFER_ARB, m_uiTexCoordsBuffer );
	return GL_TRUE;
}

GLvoid CARBVertexBufferObject::DeleteTexCoordsBuffers()
{
	if( !m_bUse || !m_bIsSupported )
		return;

	glDeleteBuffersARB( 1, &m_uiTexCoordsBuffer );
	m_uiTexCoordsBuffer = 0;
}

GLvoid CARBVertexBufferObject::CreateTexCoordsEmbossBumpBuffer( const GLvoid *lpVoid, GLsizei iSize )
{
	if( !m_bUse || !m_bIsSupported )
		return;

	if( m_uiTexCoordsEmbossBumpBuffer )
		DeleteTexCoordsEmbossBumpBuffers();

	glGenBuffersARB( 1, &m_uiTexCoordsEmbossBumpBuffer );
	if( glIsBufferARB( m_uiTexCoordsEmbossBumpBuffer ) )
		CONSOLE_ADDTEXT( CConsole::EWarning, _T("WARNING: CreateTexCoordsEmbossBumpBuffer failed") );
	
	glBindBufferARB( GL_ARRAY_BUFFER_ARB, m_uiTexCoordsEmbossBumpBuffer );
	glBufferDataARB( GL_ARRAY_BUFFER_ARB, iSize, lpVoid, GL_STATIC_DRAW_ARB );
}

GLboolean CARBVertexBufferObject::BindTexCoordsEmbossBumpBuffer()
{
	if( !m_bUse || !m_bIsSupported )
		return GL_FALSE;

	glBindBufferARB( GL_ARRAY_BUFFER_ARB, m_uiTexCoordsEmbossBumpBuffer );
	return GL_TRUE;
}

GLvoid CARBVertexBufferObject::DeleteTexCoordsEmbossBumpBuffers()
{
	if( !m_bUse || !m_bIsSupported )
		return;

	glDeleteBuffersARB( 1, &m_uiTexCoordsEmbossBumpBuffer );
	m_uiTexCoordsEmbossBumpBuffer = 0;
}