
#include "StdAfx.h"
#include "Billboard.h"

CBillboard::CBillboard()
{
	m_pTexCoord = new CVector2[ 4 ];
	m_pVertices = new CVector3[ 4 ];
}

CBillboard::~CBillboard()
{
	delete [] m_pTexCoord;
	delete [] m_pVertices;
}

GLvoid CBillboard::DrawQuadBillboard( const CVector3 &in_cPos,
									  GLfloat fScale /*= 1.0f*/,
									  GLenum eMode /*= GL_TRIANGLE_STRIP*/,
									  GLfloat fTexCoordX1 /*= 0.0f*/,
									  GLfloat fTexCoordX2 /*= 1.0f*/,
									  GLfloat fTexCoordY1 /*= 0.0f*/,
									  GLfloat fTexCoordY2 /*= 1.0f*/ )
{
	if( eMode != GL_TRIANGLE_STRIP && eMode != GL_LINE_LOOP )
		eMode = GL_TRIANGLE_STRIP;

	CVector3 cPoint0( in_cPos.x + 1.0f, in_cPos.y + 1.0f, in_cPos.z );
	CVector3 cPoint1( in_cPos.x - 1.0f, in_cPos.y + 1.0f, in_cPos.z );
	CVector3 cPoint2( in_cPos.x + 1.0f, in_cPos.y - 1.0f, in_cPos.z );
	CVector3 cPoint3( in_cPos.x - 1.0f, in_cPos.y - 1.0f, in_cPos.z );

	switch( eMode ) {
		case GL_LINE_LOOP:
			cPoint0 = CVector3( in_cPos.x - 1.0f, in_cPos.y + 1.0f, in_cPos.z );
			cPoint1 = CVector3( in_cPos.x + 1.0f, in_cPos.y + 1.0f, in_cPos.z );
			break;
	}

	GLfloat mat[ 16 ];
	glGetFloatv( GL_MODELVIEW_MATRIX, mat );

	CVector3 cRight( mat[ 0 ], mat[ 4 ], mat[ 8 ] );
	CVector3 cUp( mat[ 1 ], mat[ 5 ], mat[ 9 ] );
	CVector3 cCenter( in_cPos.x, in_cPos.y, in_cPos.z );

	switch( eMode ) {
		case GL_LINE_LOOP:
			cPoint1 = cCenter + ( ( -cRight - cUp ) * fScale );
			cPoint0 = cCenter + ( (  cRight - cUp ) * fScale );
			break;
		default:
			cPoint0 = cCenter + ( ( -cRight - cUp ) * fScale );
			cPoint1 = cCenter + ( (  cRight - cUp ) * fScale );
			break;
	}
	cPoint2 = cCenter + ( (  cRight + cUp ) * fScale );
	cPoint3 = cCenter + ( ( -cRight + cUp ) * fScale );

	//---------------------------------------------------------------------
	//
	// cPoint3                cPoint2
	//         +------------+
	//         |            |
	//         |            |
	//         |     +      |
	//         |  cCenter   |
	//         |            |
	//         |            |
	//         +------------+
	// cPoint0                cPoint1
	//
	//---------------------------------------------------------------------
	//

	m_pTexCoord[ 0 ] = CVector2( fTexCoordX2, fTexCoordY2 );
	m_pTexCoord[ 1 ] = CVector2( fTexCoordX1, fTexCoordY2 );
	m_pTexCoord[ 2 ] = CVector2( fTexCoordX2, fTexCoordY1 );
	m_pTexCoord[ 3 ] = CVector2( fTexCoordX1, fTexCoordY1 );

	m_pVertices[ 0 ] = cPoint2;
	m_pVertices[ 1 ] = cPoint3;
	m_pVertices[ 2 ] = cPoint1;
	m_pVertices[ 3 ] = cPoint0;

	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );
	glDisableClientState( GL_NORMAL_ARRAY );
	glDisableClientState( GL_COLOR_ARRAY );

	glVertexPointer( 3, GL_FLOAT, 0, m_pVertices );
	glTexCoordPointer( 2, GL_FLOAT, 0, m_pTexCoord );

	glDrawArrays( eMode, 0, 4 );
}