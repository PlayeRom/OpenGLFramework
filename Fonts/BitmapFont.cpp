// BitmapFont.cpp: implementation of the CBitmapFont class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "BitmapFont.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBitmapFont::CBitmapFont( HDC hDC, LPCTSTR lpFontName, GLint iHeight )
{
	m_hDC = hDC;
	BuildFont( lpFontName, iHeight );
}

CBitmapFont::~CBitmapFont()
{
	KillFont();
}

GLvoid CBitmapFont::BuildFont( LPCTSTR lpFontName, GLint iHeight )
{
	HFONT	hFont;
	HFONT	hOldfont;

	m_uiBase = glGenLists( 256 );

	hFont = ::CreateFont( iHeight,						// Height Of Font
						  0,							// Width Of Font
						  0,							// Angle Of Escapement
						  0,							// Orientation Angle
						  FW_NORMAL,					// Font Weight
						  FALSE,						// Italic
						  FALSE,						// Underline
						  FALSE,						// Strikeout
						  DEFAULT_CHARSET,				// Character Set Identifier
						  OUT_TT_PRECIS,				// Output Precision
						  CLIP_DEFAULT_PRECIS,			// Clipping Precision
						  NONANTIALIASED_QUALITY,
						  FF_DONTCARE|DEFAULT_PITCH,	// Family And Pitch
						  lpFontName );					// Font Name

	hOldfont = ( HFONT )::SelectObject( m_hDC, hFont );
	wglUseFontBitmaps( m_hDC, 0, 256, m_uiBase );
	::SelectObject( m_hDC, hOldfont );
	::DeleteObject( hFont );
}

GLvoid CBitmapFont::KillFont()
{
	glDeleteLists( m_uiBase, 256 );
}

GLvoid CBitmapFont::DrawTextFormat( GLfloat fX, GLfloat fY, LPCTSTR fmt, ... )
{
	if( !fmt )
		return;

	glDisable( GL_TEXTURE_2D );
	glPushMatrix();
	glTranslatef( 0.0f, 0.0f, -1.0f );

	glRasterPos2f( fX, fY );

	TCHAR cText[ 256 ];
	va_list	ap;

	va_start( ap, fmt );
	_vstprintf_s( cText, 256, fmt, ap );
	va_end( ap );

	glPushAttrib( GL_LIST_BIT );
	glListBase( m_uiBase );
	glCallLists( static_cast< GLsizei >( _tcslen( cText ) ), GL_UNSIGNED_SHORT, cText );
	glPopAttrib();
	
	glPopMatrix();
	glEnable( GL_TEXTURE_2D );
}

GLvoid CBitmapFont::DrawText( GLfloat fX, GLfloat fY, LPCTSTR fmt )
{
	if( !fmt )
		return;

	glDisable( GL_TEXTURE_2D );
	glPushMatrix();
	glTranslatef( 0.0f, 0.0f, -1.0f );

	glRasterPos2f( fX, fY );

	glPushAttrib( GL_LIST_BIT );
	glListBase( m_uiBase );
	glCallLists( static_cast<GLsizei>( _tcslen( fmt ) ), GL_UNSIGNED_SHORT, fmt );
	glPopAttrib();
	
	glPopMatrix();
	glEnable( GL_TEXTURE_2D );
}