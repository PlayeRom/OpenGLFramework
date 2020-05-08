// GLArrays.h: interface for the CVertexArrays class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "../OpenGLFramework/SingletonBase.h"

class CGameControl;
class CARBVertexBufferObject;
class CVector2;
class CVector3;
class CVector4;

class CVertexArrays : public CSingletonBase <CVertexArrays>
{
public:
	static CVertexArrays* Construct();
	virtual ~CVertexArrays();

	GLvoid AddVertex( GLfloat x, GLfloat y, GLfloat z );
	GLvoid AddVertex( const CVector3 &in_cVec );
	GLvoid AddTexCoord( GLfloat s, GLfloat t );
	GLvoid AddTexCoord( const CVector2 &in_cVecTCoord );
	GLvoid AddNormalSingle( GLfloat x, GLfloat y, GLfloat z ); //dla jednego wierzcho³ka
	GLvoid AddNormalSingle( const CVector3 &in_cVec ); //dla jednego wierzcho³ka
	GLvoid AddNormals( GLfloat x, GLfloat y, GLfloat z, GLint iNumberVertex );		//dla ca³ego poligonu
	GLvoid AddNormals( const CVector3 &in_cVec, GLint iNumberVertex );		//dla ca³ego poligonu
	GLvoid AddColorSingle( GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.0f ); //dla jednego wierzcholka
	GLvoid AddColors( GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLint iNumberVertex ); //dla calego poligonu

	GLvoid AddFullVertex( GLfloat vx, GLfloat vy, GLfloat vz,
						  GLfloat s, GLfloat t,
						  GLfloat nx, GLfloat ny, GLfloat nz,
						  GLfloat r = 1.0f, GLfloat g = 1.0f, GLfloat b = 1.0f, GLfloat a = 1.0f );
	GLvoid AddFullVertex( const CVector3 &in_cVecV, const CVector2 &in_cVecTCoord, const CVector3 &in_cVecN,
						  GLfloat r = 1.0f, GLfloat g = 1.0f, GLfloat b = 1.0f, GLfloat a = 1.0f );

	GLvoid AddFullVertexCalcNormal( GLfloat vx, GLfloat vy, GLfloat vz, GLfloat s, GLfloat t,
									GLfloat r = 1.0f, GLfloat g = 1.0f, GLfloat b = 1.0f, GLfloat a = 1.0f );
	GLvoid CalcAndAddNormalsFlat( GLenum eMode, GLint iFirst, GLsizei iCount, GLboolean bTurnSign = GL_FALSE );
	GLvoid CalcAndAddNormalsSmooth( GLenum eMode, GLint iFirst, GLsizei iCount, GLboolean bTurnSign = GL_FALSE );

	GLboolean InitialArrays( GLboolean bEmbossBump = GL_FALSE );
	GLboolean InitialArraysWithMultiTex( GLsizei iCount = 2, GLboolean bEmbossBump = GL_FALSE );
	GLboolean InitialArraysWithEmbossBump();

	GLboolean EnableArrays();
	GLboolean EnableArraysWithMultiTex( GLsizei iCount = 2 );
	GLboolean EnableArraysWithEmbossBump();

	inline GLint GetVerticesSize()	{ return static_cast<GLint>( m_aVertices.size() );	}
	inline GLint GetTexCoordsSize()	{ return static_cast<GLint>( m_aTexCoords.size() );	}
	inline GLint GetNormalsSize()	{ return static_cast<GLint>( m_aNormals.size() );	}
	inline GLint GetColorsSize()	{ return static_cast<GLint>( m_aColors.size() );	}

	GLvoid EnableTextureCoords();
	GLvoid EnableTextureCoordsMultiTex( GLsizei iCount = 2, GLboolean bEmbossBump = GL_FALSE );

	CVector3 GetVertex( GLint iIndex );
	CVector3 GetNormal( GLint iIndex );
	CVector2 GetTexCoord( GLint iIndex );
	CVector4 GetColor( GLint iIndex );

	GLvoid DrawNormals( GLint iFirst, GLsizei iCount, GLfloat fMultiplier = 1.0f );
	GLvoid DrawNormalsElem( GLint iFirst, GLsizei iCount, GLfloat fMultiplier = 1.0f );
	CVector3 CreateFlatNormal( const CVector3 &in_cVec1, const CVector3 &in_cVec2, const CVector3 &in_cVec3,
							   GLboolean bNormalize = GL_TRUE );

	GLvoid DisableClientState();
	GLvoid ClearAllVertexArrays( GLboolean bDisableClientState = GL_TRUE );

	inline GLvoid* GetVerticesArray()	{	return &m_aVertices[ 0 ]; }
	inline GLvoid* GetNormalsArray()	{	return &m_aNormals[ 0 ]; }
	inline GLvoid* GetTexCoordsArray()	{	return &m_aTexCoords[ 0 ]; }
	inline GLvoid* GetColorsArray()		{	return &m_aColors[ 0 ]; }

	inline CARBVertexBufferObject* GetARBVBO()	{ return m_pARBVBO; }
	inline GLboolean IsInitialized() { return m_bInitalizedAlready; }
	inline GLboolean IsInitializedElem() { return m_bInitalizedAlreadyElem; }

	////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////
	void AddVertexElem( const CVector3 &in_cVec );
	void AddTexCoordElem( const CVector2 &in_cTexCoord );
	void AddNormalElem( const CVector3 &in_cNormal );
	void AddSubIndices( const std::vector<GLushort> &in_aSubIndices );
#ifdef _3DSLOADER_
	void CreateIndices( GLint iFirst, GLsizei iCount );
#endif

	GLboolean InitialArraysElem();
	GLboolean InitialArraysWithMultiTexElem( GLsizei iCount = 2 );
	GLboolean InitialArraysWithEmbossBumpElem();

	GLboolean EnableArraysElem();
	GLboolean EnableArraysWithMultiTexElem( GLsizei iCount = 2 );
	GLboolean EnableArraysWithEmbossBumpElem();

	GLvoid EnableClientState();
	GLvoid EnableClientStateElem();

	GLvoid EnableTextureCoordsElem();
	GLvoid EnableTextureCoordsMultiTexElem( GLsizei iCount = 2, GLboolean bEmbossBump = GL_FALSE );

	inline GLint GetIndicesSize(){ return static_cast<GLint>( m_aShortIndices.size() ); }
	GLint GetSubIndicesSize( GLint iIndex );
	GLint GetIndexFromIndices( GLint iIndex, GLint iSubIndex );

	inline CVector3 GetVertexElem( GLint iIndex ) {
		if( iIndex < 0 || iIndex >= GetVerticesElemSize() )
			return CVector3();
		return m_aVerticesElem[ iIndex ];
	}
	inline CVector3& GetVertexElemRef( GLint iIndex ) { return m_aVerticesElem[ iIndex ]; }
	CVector2 GetTexCoordElem( GLint iIndex );
	CVector3 GetNormalElem( GLint iIndex );
	CVector4 GetColorElem( GLint iIndex );

	GLvoid SetVertexElem( GLint iIndex, const CVector3 &in_cVertex );
	GLvoid SetTexCoordElem( GLint iIndex, const CVector2 &in_cTexCoord );
	GLvoid SetNormalElem( GLint iIndex, const CVector3 &in_cNormal );

	GLvoid SetVertex( GLint iIndex, const CVector3 &in_cVertex );
	GLvoid SetTexCoord( GLint iIndex, const CVector2 &in_cTexCoord );
	GLvoid SetNormal( GLint iIndex, const CVector3 &in_cNormal );

	inline GLint GetVerticesElemSize() { return static_cast<GLint>( m_aVerticesElem.size() ); }
	inline GLint GetTexCoordsElemSize() { return static_cast<GLint>( m_aTexCoordsElem.size() ); }
	inline GLint GetNormalsElemSize() { return static_cast<GLint>( m_aNormalsElem.size() ); }
	inline GLint GetColorsElemSize() { return static_cast<GLint>( m_aColorsElem.size() ); }
	
	GLvoid* GetSubIndices( GLint iIndex );

	GLvoid ClearAllVertexArraysElem( GLboolean bDisableClientState = GL_TRUE );

	GLvoid LockArrays( GLint iFirst, GLsizei iCount );
	GLvoid UnlockArrays();

private:
	CVertexArrays();

	GLboolean m_bInitalizedAlready;
	GLboolean m_bInitalizedAlreadyElem;

	CARBVertexBufferObject *m_pARBVBO;

	//tablice do rysowania metoda glDrawArrays
	std::vector<CVector3> m_aVertices;
	std::vector<CVector2> m_aTexCoords;
	std::vector<CVector3> m_aNormals;
	std::vector<CVector3> m_aNormalsVertex; //pomocnicza przy rysowaniu normalnych
	std::vector<CVector4> m_aColors;

	//tablice do rysowania metoda glDrawElements
	std::vector<CVector3> m_aVerticesElem;
	std::vector<CVector2> m_aTexCoordsElem;
	std::vector<CVector3> m_aNormalsElem;
	std::vector<CVector4> m_aColorsElem;
	//tablica zawierajaca podtablice indeksow
	std::vector<std::vector<GLushort>> m_aShortIndices;

	CGameControl *m_pGameCtrl;
};