// GLArrays.cpp: implementation of the CVertexArrays class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../OpenGLFramework/GameControl.h"
#include "../OpenGLFramework/Console.h"
#include "../Extensions/ARBMultiTexturing.h"
#include "../Extensions/ARBVertexBufferObject.h"
#include "EmbossBump.h"
#include "VertexArrays.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

PFNGLLOCKARRAYSEXTPROC		glLockArraysEXT = NULL;
PFNGLUNLOCKARRAYSEXTPROC	glUnlockArraysEXT = NULL;

CVertexArrays* CVertexArrays::Construct()
{
	return CSingletonBase::Construct( new CVertexArrays );
}

CVertexArrays::CVertexArrays()
{
	m_bInitalizedAlready = GL_FALSE;
	m_bInitalizedAlreadyElem = GL_FALSE;
	m_pGameCtrl = CGameControl::GetInstance();

	m_pARBVBO = new CARBVertexBufferObject();
	GetARBVBO()->SetUse( GL_FALSE ); //jezeli uzywamy bump wylaczamy VBO
	
	glLockArraysEXT = ( PFNGLLOCKARRAYSEXTPROC )wglGetProcAddress( "glLockArraysEXT" );
	glUnlockArraysEXT = ( PFNGLUNLOCKARRAYSEXTPROC )wglGetProcAddress( "glUnlockArraysEXT" );
}

CVertexArrays::~CVertexArrays()
{
	ClearAllVertexArrays();
	ClearAllVertexArraysElem();
	delete m_pARBVBO;
}

GLvoid CVertexArrays::LockArrays( GLint iFirst, GLsizei iCount )
{
	glLockArraysEXT( iFirst, iCount );
}

GLvoid CVertexArrays::UnlockArrays()
{
	glUnlockArraysEXT();
}

GLvoid CVertexArrays::ClearAllVertexArrays( GLboolean bDisableClientState )
{
	if( bDisableClientState )
		DisableClientState(); // <- wewnatrz m_bInitalizedAlready = GL_FALSE;

	m_aVertices.clear();
	m_aTexCoords.clear();
	m_aNormals.clear();
	m_aNormalsVertex.clear();
	m_aColors.clear();

	if( bDisableClientState ) {
		GetARBVBO()->DeleteVerticesBuffers();
		GetARBVBO()->DeleteNormalsBuffers();
		GetARBVBO()->DeleteTexCoordsBuffers();
		GetARBVBO()->DeleteTexCoordsEmbossBumpBuffers();
		GetARBVBO()->DeleteColorsBuffers();
	}
}

GLvoid CVertexArrays::DisableClientState()
{
	glDisableClientState( GL_VERTEX_ARRAY );
	glDisableClientState( GL_NORMAL_ARRAY );
	glDisableClientState( GL_COLOR_ARRAY );
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );
	m_bInitalizedAlready = GL_FALSE;
	m_bInitalizedAlreadyElem = GL_FALSE;
}

GLvoid CVertexArrays::AddVertex( GLfloat x, GLfloat y, GLfloat z )
{
	m_aVertices.push_back( CVector3( x, y, z ) );
}

GLvoid CVertexArrays::AddVertex( const CVector3 &in_cVec )
{
	m_aVertices.push_back( in_cVec );
}

GLvoid CVertexArrays::AddTexCoord( GLfloat s, GLfloat t )
{
	m_aTexCoords.push_back( CVector2( s, t ) );
}

GLvoid CVertexArrays::AddTexCoord( const CVector2 &in_cVecTCoord )
{
	m_aTexCoords.push_back( in_cVecTCoord );
}

GLvoid CVertexArrays::AddNormalSingle( GLfloat x, GLfloat y, GLfloat z ) //dla jednego wierzcho³ka
{
	m_aNormals.push_back( CVector3( x, y, z ) );
}

GLvoid CVertexArrays::AddNormalSingle( const CVector3 &in_cVec ) //dla jednego wierzcho³ka
{
	m_aNormals.push_back( in_cVec );
}

GLvoid CVertexArrays::AddNormals( GLfloat x, GLfloat y, GLfloat z, GLint iNumberVertex )	//dla ca³ego poligonu
{
	CVector3 cNormal( x, y, z );
	for( GLint i = 0; i < iNumberVertex; ++i )
		m_aNormals.push_back( cNormal );
}

GLvoid CVertexArrays::AddNormals( const CVector3 &in_cVec, GLint iNumberVertex )	//dla ca³ego poligonu
{
	for( GLint i = 0; i < iNumberVertex; ++i )
		m_aNormals.push_back( in_cVec );
}

GLvoid CVertexArrays::AddColorSingle( GLfloat r, GLfloat g, GLfloat b, GLfloat a ) //dla jednego wierzcholka
{
	m_aColors.push_back( CVector4( r, g, b, a ) );
}

GLvoid CVertexArrays::AddColors( GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLint iNumberVertex ) //dla calego poligonu
{
	CVector4 cColor( r, g, b, a );
	for( GLint i = 0; i < iNumberVertex; ++i )
		m_aColors.push_back( cColor );
}

GLvoid CVertexArrays::AddFullVertex( GLfloat vx, GLfloat vy, GLfloat vz,
									 GLfloat s, GLfloat t,
									 GLfloat nx, GLfloat ny, GLfloat nz,
									 GLfloat r, GLfloat g, GLfloat b, GLfloat a )
{
	AddVertex( vx, vy, vz );
	AddTexCoord( s, t );
	AddNormalSingle( nx, ny, nz );
	AddColorSingle( r, g, b, a );
}

GLvoid CVertexArrays::AddFullVertex( const CVector3 &in_cVecV, const CVector2 &in_cVecTCoord, const CVector3 &in_cVecN,
									 GLfloat r, GLfloat g, GLfloat b, GLfloat a )
{
	AddVertex( in_cVecV );
	AddTexCoord( in_cVecTCoord );
	AddNormalSingle( in_cVecN );
	AddColorSingle( r, g, b, a );
}

GLvoid CVertexArrays::AddFullVertexCalcNormal( GLfloat vx, GLfloat vy, GLfloat vz,
											   GLfloat s, GLfloat t,
											   GLfloat r, GLfloat g, GLfloat b, GLfloat a )
{
	//normalna jest poznej wyliczana za pomoca CalcAndAddNormals...()
	AddVertex( vx, vy, vz );
	AddTexCoord( s, t );
	AddColorSingle( r, g, b, a );
}

GLvoid CVertexArrays::CalcAndAddNormalsFlat( GLenum eMode, GLint iFirst, GLsizei iCount,
											 GLboolean bTurnSign /*= GL_FALSE*/ )
{
	//obsluguje tylko GL_QUADS i GL_TRIANGLES
	
	GLint iCountVertex = 0;
	switch( eMode ) {
		case GL_QUADS:
			iCountVertex = 4;
			break;
		case GL_TRIANGLES:
			iCountVertex = 3;
			break;
		default:
			CONSOLE_ADDTEXT( CConsole::EError, _T("ERROR - CalcAndAddNormalsFlat invalid parameters") );
			return;
	}

	//sprawdzamy poprawnosc parametrow
	if( iCount % iCountVertex != 0 ) {
		CONSOLE_ADDTEXT( CConsole::EError, _T("ERROR - CalcAndAddNormalsFlat invalid parameters") );
		return;
	}

	for( GLint i = iFirst; i < iFirst + iCount; i += iCountVertex ) {
		CVector3 cNormal = CreateFlatNormal( m_aVertices[ i ], m_aVertices[ i + 1 ], m_aVertices[ i + 2 ] );

		if( bTurnSign )
			-cNormal;

		AddNormals( cNormal, iCountVertex );
		for( GLint j = 0; j < iCountVertex; j += iCountVertex ) {
			m_aNormalsVertex.push_back( m_aVertices[ i ] );
			m_aNormalsVertex.push_back( m_aVertices[ i + 1 ] );
			m_aNormalsVertex.push_back( m_aVertices[ i + 2 ] );
		}
	}
}

GLvoid CVertexArrays::CalcAndAddNormalsSmooth( GLenum eMode, GLint iFirst, GLsizei iCount, GLboolean bTurnSign )
{
	//obsluguje tylko GL_QUADS i GL_TRIANGLES

	GLint iCountVertex = 0;
	switch( eMode ) {
		case GL_QUADS:
			iCountVertex = 4;
			break;
		case GL_TRIANGLES:
			iCountVertex = 3;
			break;
		default:
			CONSOLE_ADDTEXT( CConsole::EError, _T("ERROR - CalcAndAddNormalsSmooth invalid parameters") );
			return;
	}

	CVector3 cVecNormal;
	GLuint uiCountFace = 0, uiFace = 0;

	CVector3 cVec[ 3 ]; //3 pierwsze wierzcholki poligonu
	CVector3 *pVertexTemp = new CVector3[ iCountVertex ];
	for( GLuint i = iFirst; i < static_cast< GLuint >( iFirst + iCount ); ++i )
	{
		cVecNormal.x = cVecNormal.y = cVecNormal.z = 0.0f;
		uiCountFace = 0;
		uiFace = 0;

		//nastepny poligon
		for( GLuint j = iFirst; j < static_cast< GLuint >( iFirst + iCount ); ++j )
		{
			if( m_aVertices[ i ].x == m_aVertices[ j ].x &&
				m_aVertices[ i ].y == m_aVertices[ j ].y &&
				m_aVertices[ i ].z == m_aVertices[ j ].z )
			{ 
				GLint iCounter = 0;
				for( GLuint k = uiFace * iCountVertex;  k < ( uiFace + 1 ) * iCountVertex; ++k ) {
					pVertexTemp[ iCounter ] = m_aVertices[ k + iFirst ];
					++iCounter;
				}
				memcpy_s( cVec, sizeof( cVec ), pVertexTemp, sizeof( CVector3 ) * iCountVertex );
				cVecNormal += CreateFlatNormal( cVec[ 0 ], cVec[ 1 ], cVec[ 2 ], GL_FALSE );
			}

			++uiCountFace;
			if( uiCountFace % iCountVertex == 0 )
				++uiFace;
		}

		cVecNormal.Normalize();
		if( bTurnSign )
			-cVecNormal;

		AddNormalSingle( cVecNormal );
		m_aNormalsVertex.push_back( m_aVertices[ i ] );
	}

	delete [] pVertexTemp;
}

GLvoid CVertexArrays::EnableClientState()
{
	if( GetVerticesSize() == 0 )
		return;

	if( GetARBVBO()->BindVerticesBuffer() )
		glVertexPointer( 3, GL_FLOAT, 0, ( GLvoid* )( ( char* )NULL ) );
	else
		glVertexPointer( 3, GL_FLOAT, 0, &m_aVertices[ 0 ] );
	glEnableClientState( GL_VERTEX_ARRAY );

	if( GetARBVBO()->BindNormalsBuffer() ) {
		glNormalPointer( GL_FLOAT, 0, ( GLvoid* )( ( char* )NULL ) );
		glEnableClientState( GL_NORMAL_ARRAY );
	}
	else if( GetNormalsSize() > 2 ) {
		glNormalPointer( GL_FLOAT, 0, &m_aNormals[ 0 ] );
		glEnableClientState( GL_NORMAL_ARRAY );
	}
	else glDisableClientState( GL_NORMAL_ARRAY );

	if( GetARBVBO()->BindColorsBuffer() ) {
		glColorPointer( 4, GL_FLOAT, 0, ( GLvoid* )( ( char* )NULL ) );
		glEnableClientState( GL_COLOR_ARRAY );
	}
	else if( GetColorsSize() > 2 ) {
		glColorPointer( 4, GL_FLOAT, 0, &m_aColors[ 0 ] );
		glEnableClientState( GL_COLOR_ARRAY );
	}
	else glDisableClientState( GL_COLOR_ARRAY );
}

GLboolean CVertexArrays::InitialArrays( GLboolean
#ifndef _3DSLOADER_
									    bEmbossBump /*= GL_FALSE*/
#endif
									   )
{
	if( IsInitialized() )
		return GL_FALSE;

	if( GetVerticesSize() < 3 )
		return GL_FALSE;

//	GetARBVBO()->CreateVerticesBuffer( GetVerticesArray(), GetVerticesSize() * 3 * sizeof( GLfloat ) );
//	GetARBVBO()->CreateNormalsBuffer( GetNormalsArray(), GetNormalsSize() * 3 * sizeof( GLfloat ) );
//	GetARBVBO()->CreateColorsBuffer( GetColorsArray(), GetColorsSize() * 3 * sizeof( GLfloat ) );
//	GetARBVBO()->CreateTexCoordsBuffer( GetTexCoordsArray(), GetTexCoordsSize() * 2 * sizeof( GLfloat ) );

	EnableClientState();
	EnableTextureCoords();

#ifndef _3DSLOADER_
	if( !bEmbossBump && GetARBVBO()->IsUse() && GetARBVBO()->IsSupported() )
		ClearAllVertexArrays( GL_FALSE );
#endif

	m_bInitalizedAlready = GL_TRUE;
	m_bInitalizedAlreadyElem = GL_FALSE;

	return GL_TRUE;
}

GLboolean CVertexArrays::InitialArraysWithMultiTex( GLsizei iCount, GLboolean
#ifndef _3DSLOADER_
												    bEmbossBump
#endif
												   )
{
	if( IsInitialized() )
		return GL_FALSE;

	if( GetVerticesSize() < 3 && ( !GetARBVBO()->IsUse() || !GetARBVBO()->IsSupported() ) )
		return GL_FALSE;

//	GetARBVBO()->CreateVerticesBuffer( GetVerticesArray(), GetVerticesSize() * 3 * sizeof( GLfloat ) );
//	GetARBVBO()->CreateNormalsBuffer( GetNormalsArray(), GetNormalsSize() * 3 * sizeof( GLfloat ) );
//	GetARBVBO()->CreateColorsBuffer( GetColorsArray(), GetColorsSize() * 3 * sizeof( GLfloat ) );
//	GetARBVBO()->CreateTexCoordsBuffer( GetTexCoordsArray(), GetTexCoordsSize() * 2 * sizeof( GLfloat ) );

	EnableClientState();
	EnableTextureCoordsMultiTex( iCount );

#ifndef _3DSLOADER_
	if( !bEmbossBump && GetARBVBO()->IsUse() && GetARBVBO()->IsSupported() )
		ClearAllVertexArrays( GL_FALSE );
#endif

	m_bInitalizedAlready = GL_TRUE;
	m_bInitalizedAlreadyElem = GL_FALSE;

	return GL_TRUE;
}

GLboolean CVertexArrays::InitialArraysWithEmbossBump()
{
	if( IsInitialized() )
		return GL_FALSE;

	if( GetVerticesSize() < 3 )
		return GL_FALSE;

//	GetARBVBO()->CreateVerticesBuffer( GetVerticesArray(), GetVerticesSize() * 3 * sizeof( GLfloat ) );
//	GetARBVBO()->CreateNormalsBuffer( GetNormalsArray(), GetNormalsSize() * 3 * sizeof( GLfloat ) );
//	GetARBVBO()->CreateColorsBuffer( GetColorsArray(), GetColorsSize() * 3 * sizeof( GLfloat ) );
//	GetARBVBO()->CreateTexCoordsBuffer( GetTexCoordsArray(), GetTexCoordsSize() * 2 * sizeof( GLfloat ) );
//
//	GetARBVBO()->CreateTexCoordsEmbossBumpBuffer( m_pGameCtrl->GetEmbossBump()->GetTexCoordsArray(),
//												  GetTexCoordsSize() * 2 * sizeof( GLfloat ) );

	EnableClientState();
	EnableTextureCoordsMultiTex( 2, GL_TRUE );

	m_bInitalizedAlready = GL_TRUE;
	m_bInitalizedAlreadyElem = GL_FALSE;

	return GL_TRUE;
}

GLboolean CVertexArrays::EnableArrays()
{
	if( IsInitialized() )
		return GL_FALSE;

	EnableClientState();
	EnableTextureCoords();

	m_bInitalizedAlready = GL_TRUE;
	m_bInitalizedAlreadyElem = GL_FALSE;

	return GL_TRUE;
}

GLboolean CVertexArrays::EnableArraysWithMultiTex( GLsizei iCount )
{
	if( IsInitialized() )
		return GL_FALSE;

	EnableClientState();
	EnableTextureCoordsMultiTex( iCount );

	m_bInitalizedAlready = GL_TRUE;
	m_bInitalizedAlreadyElem = GL_FALSE;

	return GL_TRUE;
}

GLboolean CVertexArrays::EnableArraysWithEmbossBump()
{
	if( IsInitialized() )
		return GL_FALSE;

	EnableClientState();
	EnableTextureCoordsMultiTex( 2, GL_TRUE );

	m_bInitalizedAlready = GL_TRUE;
	m_bInitalizedAlreadyElem = GL_FALSE;

	return GL_TRUE;
}

GLvoid CVertexArrays::EnableTextureCoords()
{
	if( GetARBVBO()->BindTexCoordsBuffer() ) {
		glTexCoordPointer( 2, GL_FLOAT, 0, ( GLvoid* )( ( char* )NULL ) );
		glEnableClientState( GL_TEXTURE_COORD_ARRAY );
	}
	else if( GetTexCoordsSize() > 1 ) {
		glTexCoordPointer( 2, GL_FLOAT, 0, &m_aTexCoords[ 0 ] );
		glEnableClientState( GL_TEXTURE_COORD_ARRAY );
	}
	else glDisableClientState( GL_TEXTURE_COORD_ARRAY );
}

GLvoid CVertexArrays::EnableTextureCoordsMultiTex( GLsizei iCount /*= 2*/, GLboolean bEmbossBump /*= GL_FALSE*/ )
{
	if( bEmbossBump ) {
		if( iCount == 2 ) {
			m_pGameCtrl->GetARBMultiTexturing()->ClientActiveTextureARB( 0 );
			if( GetARBVBO()->BindTexCoordsBuffer() ) {
				glTexCoordPointer( 2, GL_FLOAT, 0, ( GLvoid* )( ( char* )NULL ) );
				glEnableClientState( GL_TEXTURE_COORD_ARRAY );
			}
			else if( GetTexCoordsSize() > 1 ) {
				glTexCoordPointer( 2, GL_FLOAT, 0, &m_aTexCoords[ 0 ] );
				glEnableClientState( GL_TEXTURE_COORD_ARRAY );
			}

			m_pGameCtrl->GetARBMultiTexturing()->ClientActiveTextureARB( 1 );
			if( GetARBVBO()->BindTexCoordsEmbossBumpBuffer() ) {
				glTexCoordPointer( 2, GL_FLOAT, 0, ( GLvoid* )( ( char* )NULL ) );
				glEnableClientState( GL_TEXTURE_COORD_ARRAY );
			}
			else if( m_pGameCtrl->GetEmbossBump()->GetTexCoordsSize() > 1 ) {
				glTexCoordPointer( 2, GL_FLOAT, 0, m_pGameCtrl->GetEmbossBump()->GetTexCoordsArray() );
				glEnableClientState( GL_TEXTURE_COORD_ARRAY );
			}
		}
		else glDisableClientState( GL_TEXTURE_COORD_ARRAY );
		return;
	}

	if( iCount > 1 ) {
		for( GLint i = 0; i < iCount; ++i ) {
			m_pGameCtrl->GetARBMultiTexturing()->ClientActiveTextureARB( i );
			if( GetARBVBO()->BindTexCoordsBuffer() ) {
				glTexCoordPointer( 2, GL_FLOAT, 0, ( GLvoid* )( ( char* )NULL ) );
				glEnableClientState( GL_TEXTURE_COORD_ARRAY );
			}
			else if( GetTexCoordsSize() > 1 ) {
				glTexCoordPointer( 2, GL_FLOAT, 0, &m_aTexCoords[ 0 ] );
				glEnableClientState( GL_TEXTURE_COORD_ARRAY );
			}
			else glDisableClientState( GL_TEXTURE_COORD_ARRAY );
		}
	}
	else {
		CONSOLE_ADDTEXT( CConsole::EWarning, _T("WARNING: CVertexArrays::InitialArraysWithMultiTex - invalid iCount: %d"), iCount );
		if( GetARBVBO()->BindTexCoordsBuffer() ) {
			glTexCoordPointer( 2, GL_FLOAT, 0, ( GLvoid* )( ( char* )NULL ) );
			glEnableClientState( GL_TEXTURE_COORD_ARRAY );
		}
		else if( GetTexCoordsSize() > 1 ) {
			glTexCoordPointer( 2, GL_FLOAT, 0, &m_aTexCoords[ 0 ] );
			glEnableClientState( GL_TEXTURE_COORD_ARRAY );
		}
		else glDisableClientState( GL_TEXTURE_COORD_ARRAY );
	}
}

CVector3 CVertexArrays::GetVertex( GLint iIndex )
{
	if( iIndex < 0 || iIndex >= GetVerticesSize() )
		return CVector3();

	return m_aVertices[ iIndex ];
}

CVector3 CVertexArrays::GetNormal( GLint iIndex )
{
	if( iIndex < 0 || iIndex >= GetNormalsSize() )
		return CVector3();

	return m_aNormals[ iIndex ];
}

CVector2 CVertexArrays::GetTexCoord( GLint iIndex )
{
	if( iIndex < 0 || iIndex >= GetTexCoordsSize() )
		return CVector2();

	return m_aTexCoords[ iIndex ];
}

CVector4 CVertexArrays::GetColor( GLint iIndex )
{
	if( iIndex < 0 || iIndex >= GetColorsSize() )
		return CVector4();

	return m_aColors[ iIndex ];
}

GLvoid CVertexArrays::DrawNormals( GLint iFirst, GLsizei iCount, GLfloat fMultiplier )
{
	GLint iSize = GetVerticesSize();

	if( iFirst < 0 || iFirst + iCount > iSize )
		return;

	GLboolean bLighting = GL_FALSE;
	if( glIsEnabled( GL_LIGHTING ) ) {
		bLighting = GL_TRUE;
		glDisable( GL_LIGHTING );
	}

	GLboolean bTexture2D = GL_FALSE;
	if( glIsEnabled( GL_TEXTURE_2D ) ) {
		bTexture2D = GL_TRUE;
		glDisable( GL_TEXTURE_2D );
	}

	glColor3f( 0.0f, 1.0f, 0.0f );

	CVector3* pNormalVer = new CVector3[ iCount * 2 ];
	if( pNormalVer )
	{
		for( GLint i = iFirst, j = 0; i < iFirst + iCount; ++i, j += 2 )
		{
			pNormalVer[ j ] = CVector3( m_aVertices[ i ].x, m_aVertices[ i ].y, m_aVertices[ i ].z );
			pNormalVer[ j + 1 ] = CVector3( m_aVertices[ i ].x + m_aNormals[ i ].x * fMultiplier,
											m_aVertices[ i ].y + m_aNormals[ i ].y * fMultiplier,
											m_aVertices[ i ].z + m_aNormals[ i ].z * fMultiplier );
		}

		glEnableClientState( GL_VERTEX_ARRAY );
		glDisableClientState( GL_TEXTURE_COORD_ARRAY );
		glDisableClientState( GL_NORMAL_ARRAY );
		glDisableClientState( GL_COLOR_ARRAY );

		glVertexPointer( 3, GL_FLOAT, 0, pNormalVer );
		glDrawArrays( GL_LINES, 0, iCount * 2 );

		delete [] pNormalVer;
	}

	if( bLighting )
		glEnable( GL_LIGHTING );

	if( bTexture2D )
		glEnable( GL_TEXTURE_2D );
}

GLvoid CVertexArrays::DrawNormalsElem( GLint iFirst, GLsizei iCount, GLfloat fMultiplier )
{
	GLint iSize = GetVerticesElemSize();

	if( iFirst < 0 || iFirst + iCount > iSize )
		return;

	GLboolean bLighting = GL_FALSE;
	if( glIsEnabled( GL_LIGHTING ) ) {
		bLighting = GL_TRUE;
		glDisable( GL_LIGHTING );
	}

	GLboolean bTexture2D = GL_FALSE;
	if( glIsEnabled( GL_TEXTURE_2D ) ) {
		bTexture2D = GL_TRUE;
		glDisable( GL_TEXTURE_2D );
	}

	glColor3f( 0.0f, 1.0f, 0.0f );

	CVector3* pNormalVer = new CVector3[ iCount * 2 ];
	if( pNormalVer )
	{
		for( GLint i = iFirst, j = 0; i < iFirst + iCount; ++i, j += 2 )
		{
			pNormalVer[ j ] = CVector3( m_aVerticesElem[ i ].x, m_aVerticesElem[ i ].y, m_aVerticesElem[ i ].z );
			pNormalVer[ j + 1 ] = CVector3( m_aVerticesElem[ i ].x + m_aNormalsElem[ i ].x * fMultiplier,
											m_aVerticesElem[ i ].y + m_aNormalsElem[ i ].y * fMultiplier,
											m_aVerticesElem[ i ].z + m_aNormalsElem[ i ].z * fMultiplier );
		}

		glEnableClientState( GL_VERTEX_ARRAY );
		glDisableClientState( GL_TEXTURE_COORD_ARRAY );
		glDisableClientState( GL_NORMAL_ARRAY );
		glDisableClientState( GL_COLOR_ARRAY );

		glVertexPointer( 3, GL_FLOAT, 0, pNormalVer );
		glDrawArrays( GL_LINES, 0, iCount * 2 );

		delete [] pNormalVer;
	}

	if( bLighting )
		glEnable( GL_LIGHTING );

	if( bTexture2D )
		glEnable( GL_TEXTURE_2D );
}

//tworzy normalna z cieniowaniem plaskim dla poligonu
CVector3 CVertexArrays::CreateFlatNormal( const CVector3 &in_cVec1, const CVector3 &in_cVec2,
										  const CVector3 &in_cVec3, GLboolean bNormalize )
{
	CVector3 cNormal = CVector3( in_cVec2 - in_cVec1 ).CrossProduct( in_cVec3 - in_cVec2 );
	if( bNormalize )
		cNormal.Normalize();

	return cNormal;
}

///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////

void CVertexArrays::AddVertexElem( const CVector3 &in_cVec )
{
	m_aVerticesElem.push_back( in_cVec );
}

void CVertexArrays::AddTexCoordElem( const CVector2 &in_cTexCoord )
{
	m_aTexCoordsElem.push_back( in_cTexCoord );
}

void CVertexArrays::AddNormalElem( const CVector3 &in_cNormal )
{
	m_aNormalsElem.push_back( in_cNormal );
}

void CVertexArrays::AddSubIndices( const std::vector<GLushort> &in_aSubIndices )
{
	m_aShortIndices.push_back( in_aSubIndices );
}

#ifdef _3DSLOADER_
void CVertexArrays::CreateIndices( GLint iFirst, GLsizei iCount )
{
	/*
	Funkcja ta tworzy tablice indeksow oraz tablice niepowtarzalnych wierzcholkow z
	tablic "nie Elem"
	*/
	if( iFirst < 0 || iFirst + iCount > GetVerticesSize() ) {
		CONSOLE_ADDTEXT( CConsole::EError, _T("ERROR - CVertexArrays::CreateIndices - incorrect parameters") );
		return;
	}

	std::vector< GLushort > aSingleShortIndices;
	GLboolean bFind = 0;
	for( GLint i = iFirst; i < iFirst + iCount; ++i ) {
		bFind = GL_FALSE;
		GLint j;
		for( j = 0; j < GetVerticesElemSize(); ++j ) {
			if( m_aVertices[ i ] == m_aVerticesElem[ j ] &&
				m_aTexCoords[ i ] == m_aTexCoordsElem[ j ] &&
				m_aNormals[ i ] == m_aNormalsElem[ j ] &&
				j >= iFirst //&&
				//m_aColors[ i ] == m_aColorsElem[ j ]
				)
			{
				bFind = GL_TRUE; // znaleziono ten sam wierzcholek
				break;
			}
		}

		if( bFind )
			aSingleShortIndices.push_back( static_cast< GLushort >( j ) );
		else {
			m_aVerticesElem.push_back( m_aVertices[ i ] );
			m_aTexCoordsElem.push_back( m_aTexCoords[ i ] );
			m_aNormalsElem.push_back( m_aNormals[ i ] );
			m_aColorsElem.push_back( m_aColors[ i ] );
			aSingleShortIndices.push_back( static_cast< GLushort >( m_aVerticesElem.size() - 1 ) );
		}
	}

	m_aShortIndices.push_back( aSingleShortIndices );
}
#endif

GLboolean CVertexArrays::InitialArraysElem()
{
	if( IsInitializedElem() )
		return GL_FALSE;

	if( GetVerticesElemSize() < 3 )
		return GL_FALSE;

	EnableClientStateElem();
	EnableTextureCoordsElem();

#ifdef _3DSLOADER_
	ClearAllVertexArrays( GL_FALSE );
#endif

	m_bInitalizedAlreadyElem = GL_TRUE;
	m_bInitalizedAlready = GL_FALSE;

	return GL_TRUE;
}

GLboolean CVertexArrays::InitialArraysWithMultiTexElem( GLsizei iCount )
{
	if( IsInitializedElem() )
		return GL_FALSE;

	if( GetVerticesElemSize() < 3 && ( !GetARBVBO()->IsUse() || !GetARBVBO()->IsSupported() ) )
		return GL_FALSE;

	EnableClientStateElem();
	EnableTextureCoordsMultiTexElem( iCount );

#ifdef _3DSLOADER_
	ClearAllVertexArrays( GL_FALSE );
#endif

	m_bInitalizedAlreadyElem = GL_TRUE;
	m_bInitalizedAlready = GL_FALSE;

	return GL_TRUE;
}

GLboolean CVertexArrays::InitialArraysWithEmbossBumpElem()
{
	if( IsInitializedElem() )
		return GL_FALSE;

	if( GetVerticesElemSize() < 3 )
		return GL_FALSE;

	EnableClientStateElem();
	EnableTextureCoordsMultiTexElem( 2, GL_TRUE );

#ifdef _3DSLOADER_
	ClearAllVertexArrays( GL_FALSE );
#endif

	m_bInitalizedAlreadyElem = GL_TRUE;
	m_bInitalizedAlready = GL_FALSE;

	return GL_TRUE;
}

GLboolean CVertexArrays::EnableArraysElem()
{
	if( IsInitializedElem() )
		return GL_FALSE;

	EnableClientStateElem();
	EnableTextureCoordsElem();

	m_bInitalizedAlreadyElem = GL_TRUE;
	m_bInitalizedAlready = GL_FALSE;

	return GL_TRUE;
}

GLboolean CVertexArrays::EnableArraysWithMultiTexElem( GLsizei iCount )
{
	if( IsInitializedElem() )
		return GL_FALSE;

	EnableClientStateElem();
	EnableTextureCoordsMultiTexElem( iCount );

	m_bInitalizedAlreadyElem = GL_TRUE;
	m_bInitalizedAlready = GL_FALSE;

	return GL_TRUE;
}

GLboolean CVertexArrays::EnableArraysWithEmbossBumpElem()
{
	if( IsInitializedElem() )
		return GL_FALSE;

	EnableClientStateElem();
	EnableTextureCoordsMultiTexElem( 2, GL_TRUE );

	m_bInitalizedAlreadyElem = GL_TRUE;
	m_bInitalizedAlready = GL_FALSE;

	return GL_TRUE;
}

GLvoid CVertexArrays::EnableClientStateElem()
{
	glVertexPointer( 3, GL_FLOAT, 0, &m_aVerticesElem[ 0 ] );
	glEnableClientState( GL_VERTEX_ARRAY );

	if( GetNormalsElemSize() > 2 ) {
		glNormalPointer( GL_FLOAT, 0, &m_aNormalsElem[ 0 ] );
		glEnableClientState( GL_NORMAL_ARRAY );
	}
	else glDisableClientState( GL_NORMAL_ARRAY );

//	if( m_aColorsElem.size() > 2 ) {
//		glColorPointer( 4, GL_FLOAT, 0, &m_aColorsElem[ 0 ] );
//		glEnableClientState( GL_COLOR_ARRAY );
//	}
//	else
	glDisableClientState( GL_COLOR_ARRAY );
}

GLvoid CVertexArrays::EnableTextureCoordsElem()
{
	if( GetTexCoordsElemSize() > 1 ) {
		glTexCoordPointer( 2, GL_FLOAT, 0, &m_aTexCoordsElem[ 0 ] );
		glEnableClientState( GL_TEXTURE_COORD_ARRAY );
	}
	else glDisableClientState( GL_TEXTURE_COORD_ARRAY );
}

GLvoid CVertexArrays::EnableTextureCoordsMultiTexElem( GLsizei iCount /*= 2*/, GLboolean bEmbossBump /*= GL_FALSE*/ )
{
	if( bEmbossBump ) {
		if( iCount == 2 ) {
			m_pGameCtrl->GetARBMultiTexturing()->ClientActiveTextureARB( 0 );
//			if( GetARBVBO()->BindTexCoordsBuffer() ) {
//				glTexCoordPointer( 2, GL_FLOAT, 0, ( GLvoid* )( ( char* )NULL ) );
//				glEnableClientState( GL_TEXTURE_COORD_ARRAY );
//			}
//			else
			if( GetTexCoordsElemSize() > 1 ) {
				glTexCoordPointer( 2, GL_FLOAT, 0, &m_aTexCoordsElem[ 0 ] );
				glEnableClientState( GL_TEXTURE_COORD_ARRAY );
			}
			else glDisableClientState( GL_TEXTURE_COORD_ARRAY );

			m_pGameCtrl->GetARBMultiTexturing()->ClientActiveTextureARB( 1 );
//			if( GetARBVBO()->BindTexCoordsEmbossBumpBuffer() ) {
//				glTexCoordPointer( 2, GL_FLOAT, 0, ( GLvoid* )( ( char* )NULL ) );
//				glEnableClientState( GL_TEXTURE_COORD_ARRAY );
//			}
//			else 
			if( m_pGameCtrl->GetEmbossBump()->GetTexCoordsElemSize() > 1 ) {
				glTexCoordPointer( 2, GL_FLOAT, 0, m_pGameCtrl->GetEmbossBump()->GetTexCoordsArrayElem() );
				glEnableClientState( GL_TEXTURE_COORD_ARRAY );
			}
			else glDisableClientState( GL_TEXTURE_COORD_ARRAY );
		}
		else
			glDisableClientState( GL_TEXTURE_COORD_ARRAY );
		return;
	}

	if( iCount > 1 ) {
		for( GLint i = 0; i < iCount; ++i ) {
			m_pGameCtrl->GetARBMultiTexturing()->ClientActiveTextureARB( i );
//			if( GetARBVBO()->BindTexCoordsBuffer() ) {
//				glTexCoordPointer( 2, GL_FLOAT, 0, ( GLvoid* )( ( char* )NULL ) );
//				glEnableClientState( GL_TEXTURE_COORD_ARRAY );
//			}
//			else
			if( GetTexCoordsElemSize() > 1 ) {
				glTexCoordPointer( 2, GL_FLOAT, 0, &m_aTexCoordsElem[ 0 ] );
				glEnableClientState( GL_TEXTURE_COORD_ARRAY );
			}
			else glDisableClientState( GL_TEXTURE_COORD_ARRAY );
		}
	}
	else {
		CONSOLE_ADDTEXT( CConsole::EWarning, _T("WARNING: CVertexArrays::InitialArraysWithMultiTex - invalid iCount: %d"), iCount );
//		if( GetARBVBO()->BindTexCoordsBuffer() ) {
//			glTexCoordPointer( 2, GL_FLOAT, 0, ( GLvoid* )( ( char* )NULL ) );
//			glEnableClientState( GL_TEXTURE_COORD_ARRAY );
//		}
//		else
		if( GetTexCoordsElemSize() > 1 ) {
			glTexCoordPointer( 2, GL_FLOAT, 0, &m_aTexCoordsElem[ 0 ] );
			glEnableClientState( GL_TEXTURE_COORD_ARRAY );
		}
		else glDisableClientState( GL_TEXTURE_COORD_ARRAY );
	}
}

GLvoid CVertexArrays::ClearAllVertexArraysElem( GLboolean bDisableClientState )
{
	if( bDisableClientState )
		DisableClientState(); // <- wewnatrz m_bInitalizedAlready = GL_FALSE;
	
	m_aVerticesElem.clear();
	m_aTexCoordsElem.clear();
	m_aNormalsElem.clear();
	m_aColorsElem.clear();

	for( GLint i = GetIndicesSize() - 1; i >= 0; --i )
		m_aShortIndices[ i ].clear();

	m_aShortIndices.clear();
}

GLint CVertexArrays::GetIndexFromIndices( GLint iIndex, GLint iSubIndex )
{
	if( iIndex < 0 || iIndex >= GetIndicesSize() )
		return -1;

	if( iSubIndex < 0 || iSubIndex >= GetSubIndicesSize( iIndex ) )
		return -1;

	return static_cast<GLint>( m_aShortIndices[ iIndex ][ iSubIndex ] );
}

CVector2 CVertexArrays::GetTexCoordElem( GLint iIndex )
{
	if( iIndex < 0 || iIndex >= GetTexCoordsElemSize() )
		return CVector2();

	return m_aTexCoordsElem[ iIndex ];
}

CVector3 CVertexArrays::GetNormalElem( GLint iIndex )
{
	if( iIndex < 0 || iIndex >= GetNormalsElemSize() )
		return CVector3();

	return m_aNormalsElem[ iIndex ];
}

CVector4 CVertexArrays::GetColorElem( GLint iIndex )
{
	if( iIndex < 0 || iIndex >= GetColorsElemSize() )
		return CVector4();

	return m_aColorsElem[ iIndex ];
}

GLvoid CVertexArrays::SetVertexElem( GLint iIndex, const CVector3 &in_cVertex )
{
	if( iIndex < 0 || iIndex >= GetVerticesElemSize() )
		return;

	m_aVerticesElem[ iIndex ] = in_cVertex;
}

GLvoid CVertexArrays::SetTexCoordElem( GLint iIndex, const CVector2 &in_cTexCoord )
{
	if( iIndex < 0 || iIndex >= GetTexCoordsElemSize() )
		return;

	m_aTexCoordsElem[ iIndex ] = in_cTexCoord;
}

GLvoid CVertexArrays::SetNormalElem( GLint iIndex, const CVector3 &in_cNormal )
{
	if( iIndex < 0 || iIndex >= GetNormalsElemSize() )
		return;

	m_aNormalsElem[ iIndex ] = in_cNormal;
}

GLvoid CVertexArrays::SetVertex( GLint iIndex, const CVector3 &in_cVertex )
{
	if( iIndex < 0 || iIndex >= GetVerticesSize() )
		return;

	m_aVertices[ iIndex ] = in_cVertex;
}

GLvoid CVertexArrays::SetTexCoord( GLint iIndex, const CVector2 &in_cTexCoord )
{
	if( iIndex < 0 || iIndex >= GetTexCoordsSize() )
		return;

	m_aTexCoords[ iIndex ] = in_cTexCoord;
}

GLvoid CVertexArrays::SetNormal( GLint iIndex, const CVector3 &in_cNormal )
{
	if( iIndex < 0 || iIndex >= GetNormalsSize() )
		return;

	m_aNormals[ iIndex ] = in_cNormal;
}

GLvoid* CVertexArrays::GetSubIndices( GLint iIndex )
{
	if( iIndex < 0 || iIndex >= GetIndicesSize() )
		return NULL;

	return &m_aShortIndices[ iIndex ][ 0 ];
}

GLint CVertexArrays::GetSubIndicesSize( GLint iIndex )
{
	if( iIndex < 0 || iIndex >= GetIndicesSize() )
		return 0;

	return static_cast< GLint >( m_aShortIndices[ iIndex ].size() );
}