// TextureFont.cpp: implementation of the CTextureFont class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "../Draw/TextureLoader.h"
#include "../OpenGLFramework/GameControl.h"
#include "../OpenGLFramework/WindowData.h"
#include "TextureFont.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTextureFont::CTextureFont()
{
	m_pTextureLoader = new CTextureLoader();
	m_pTextureLoader->LoadMultiTexMaskMipmaps( _T("textures/font.bmp"), RGB( 255, 255, 255 ) );

	m_aTextCoord = new CVector2[ 4 ];
	m_aIndices = new CVector2[ 4 ];
	m_aIndices[ 2 ] = CVector2(  0.0f, 16.0f );
	m_aIndices[ 3 ] = CVector2( 16.0f, 16.0f );
	m_aIndices[ 1 ] = CVector2( 16.0f,  0.0f );
	m_aIndices[ 0 ] = CVector2(  0.0f,  0.0f );

	BuildFont();
}

CTextureFont::~CTextureFont()
{
	delete [] m_aTextCoord;
	delete [] m_aIndices;
	delete m_pTextureLoader;

	glDeleteLists( m_uiBase, 256 );
}

GLvoid CTextureFont::BuildFont()
{
	glDeleteLists( m_uiBase, 256 );

	m_uiBase = glGenLists( 256 );							// Creating 256 Display Lists

	for( GLint i = 0; i < 256; ++i )						// Loop Through All 256 Lists
	{
		GLfloat cx = static_cast< GLfloat >( i % 16 ) / 16.0f;		// X Position Of Current Character
		GLfloat cy = static_cast< GLfloat >( i / 16 ) / 16.0f;		// Y Position Of Current Character

		m_aTextCoord[ 2 ] = CVector2( cx,			1.0f - cy - 0.0625f );
		m_aTextCoord[ 3 ] = CVector2( cx + 0.0625f, 1.0f - cy - 0.0625f );
		m_aTextCoord[ 1 ] = CVector2( cx + 0.0625f, 1.0f - cy );
		m_aTextCoord[ 0 ] = CVector2( cx,			1.0f - cy );

		glNewList( m_uiBase + i, GL_COMPILE );				// Start Building A List
			glEnableClientState( GL_VERTEX_ARRAY );
			glEnableClientState( GL_TEXTURE_COORD_ARRAY );
			glDisableClientState( GL_NORMAL_ARRAY );
			glDisableClientState( GL_COLOR_ARRAY );

			glTexCoordPointer( 2, GL_FLOAT, 0, m_aTextCoord );
			glVertexPointer( 2, GL_FLOAT, 0, m_aIndices );
			glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );

			glTranslatef( 9.0f, 0.0f, 0.0f );				// Move To The Right Of The Character
		glEndList();										// Done Building The Display List
	}														// Loop Until All 256 Are Built
}

GLvoid CTextureFont::DrawTextFormat( GLfloat fPosX, GLfloat fPosY,
									 GLint iFont,
									 GLfloat fR, GLfloat fG, GLfloat fB,
									 GLfloat fScale, LPCTSTR lpFormat, ... )
{
	if( !lpFormat )
		return;

	TCHAR cText[ 256 ];
	va_list	ap;

	va_start( ap, lpFormat );
	_vstprintf_s( cText, 256, lpFormat, ap );
	va_end( ap );

	DrawText( fPosX, fPosY, cText, iFont, fR, fG, fB, fScale );
}

GLvoid CTextureFont::DrawText( GLfloat fPosX, GLfloat fPosY,
							   LPCTSTR lpText, GLint iFont,
							   GLfloat fR, GLfloat fG, GLfloat fB,
							   GLfloat fScale )
{
	if( !lpText )
		return;

	if( iFont < 0 || iFont > 1 )
		iFont = 0;

	CGameControl::GetInstance()->Enable2D();

	glDisable( GL_LIGHTING );
	m_pTextureLoader->SetMultiTextures( 0 );
	glDisable( GL_DEPTH_TEST );
	glColor3f( fR, fG, fB );
	glPushMatrix();
	
	glTranslatef( fPosX, fPosY, 0.0f );

	ScaleFont( fScale );

	glListBase( m_uiBase - 32 + ( 128 * iFont ) );
	glCallLists( static_cast< GLsizei >( _tcslen( lpText ) ), GL_UNSIGNED_SHORT, lpText );

	glPopMatrix();

	CGameControl::GetInstance()->Disable2D();
}

GLvoid CTextureFont::ScaleFont( GLfloat fScale )
{
	CWindowData* m_pWinData = CWindowData::GetInstance();

	//skalowanie wzgledem rozdzielczosci, baza 1024x768
	if( m_pWinData->SettingFile().iWidth < 1024	&& m_pWinData->SettingFile().iHeight < 768 ) {
		//jezeli rozdzielczosc mniejsza od 1024x768 to zmniejszamy fonta
		glScalef( fScale * ( 1.0f / ( 1024.0f / static_cast< GLfloat >( m_pWinData->SettingFile().iWidth ) ) ),
				  fScale * ( 1.0f / ( 768.0f  / static_cast< GLfloat >( m_pWinData->SettingFile().iHeight ) ) ),
				  0.0f );
	}
	else if( m_pWinData->SettingFile().iWidth > 1024 && m_pWinData->SettingFile().iHeight > 768 ) {
		//jezeli rozdzielczosc wieksza od 1024x768 to zwieksz fonta
		glScalef( fScale * ( static_cast< GLfloat >( m_pWinData->SettingFile().iWidth )  / 1024.0f ),
				  fScale * ( static_cast< GLfloat >( m_pWinData->SettingFile().iHeight ) / 768.0f ),
				  0.0f );
	}
}