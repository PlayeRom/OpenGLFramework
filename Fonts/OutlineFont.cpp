// OutlineFont.cpp: implementation of the OutlineFont class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "OutlineFont.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COutlineFont::COutlineFont( HDC hDC, LPCTSTR lpFontName )
{
	m_hDC = hDC;
	BuildFont( lpFontName );
}

COutlineFont::~COutlineFont()
{
	KillFont();
}

GLvoid COutlineFont::BuildFont( LPCTSTR lpFontName )
{
	HFONT hFont;

	m_uiBase = glGenLists( 256 );

	hFont = ::CreateFont( -12,							// Height Of Font
						  0,							// Width Of Font
						  0,							// Angle Of Escapement
						  0,							// Orientation Angle
						  FW_BOLD,						// Font Weight
						  FALSE,						// Italic
						  FALSE,						// Underline
						  FALSE,						// Strikeout
						  DEFAULT_CHARSET,				// Character Set Identifier
						  OUT_TT_PRECIS,				// Output Precision
						  CLIP_DEFAULT_PRECIS,			// Clipping Precision
						  ANTIALIASED_QUALITY,			// Output Quality
						  FF_DONTCARE|DEFAULT_PITCH,	// Family And Pitch
						  lpFontName );					// Font Name

	::SelectObject( m_hDC, hFont );

	wglUseFontOutlines(	m_hDC,							// Select The Current DC
						0,								// Starting Character
						255,							// Number Of Display Lists To Build
						m_uiBase,						// Starting Display Lists
						0.0f,							// Deviation From The True Outlines
						0.2f,							// Font Thickness In The Z Direction
						WGL_FONT_POLYGONS,				// Use Polygons, Not Lines
						m_sGlyphMetric );				// Address Of Buffer To Recieve Data
}

GLvoid COutlineFont::KillFont()
{
	glDeleteLists( m_uiBase, 256 );
}

GLvoid COutlineFont::DrawTextFormat( LPCTSTR fmt, ... )
{
	if( !fmt )
		return;	

	glDisable( GL_TEXTURE_2D );
	glPushMatrix();

	GLfloat fLength = 0;

	TCHAR cText[ 256 ];
	va_list	ap;

	va_start( ap, fmt );
	_vstprintf_s( cText, 256, fmt, ap );
	va_end( ap );

	for( GLuint i = 0; i < _tcslen( cText ); ++i )
		fLength += m_sGlyphMetric[ cText[ i ] ].gmfCellIncX;		// Increase Length By Each Characters Width

	glTranslatef( -fLength / 2.0f , 0.0f, 0.0f );		// Center Our Text On The Screen

	glPushAttrib( GL_LIST_BIT );						// Pushes The Display List Bits
	glListBase( m_uiBase );								// Sets The Base Character to 0
	glCallLists( static_cast< GLsizei >( _tcslen( cText ) ), GL_UNSIGNED_SHORT, cText );
	glPopAttrib();										// Pops The Display List Bits

	glPopMatrix();
	glEnable( GL_TEXTURE_2D );
}

GLvoid COutlineFont::DrawText( LPCTSTR fmt )
{
	if( !fmt )
		return;	

	glDisable( GL_TEXTURE_2D );
	glPushMatrix();

	GLfloat fLength = 0;

	TCHAR cText[ 256 ];
	va_list	ap;

	va_start( ap, fmt );
	_vstprintf_s( cText, 256, fmt, ap );
	va_end( ap );

	for( GLuint i = 0; i < _tcslen( cText ); ++i )
		fLength += m_sGlyphMetric[ cText[ i ] ].gmfCellIncX;	// Increase Length By Each Characters Width

	glTranslatef( -fLength / 2.0f, 0.0f, 0.0f );			// Center Our Text On The Screen

	glPushAttrib( GL_LIST_BIT );						// Pushes The Display List Bits
	glListBase( m_uiBase );								// Sets The Base Character to 0
	glCallLists( static_cast< GLsizei >( _tcslen( cText ) ), GL_UNSIGNED_SHORT, cText );
	glPopAttrib();										// Pops The Display List Bits

	glPopMatrix();
	glEnable( GL_TEXTURE_2D );
}