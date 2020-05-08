
#include "StdAfx.h"
#include "VertexArrays.h"
#include "EmbossBump.h"

CEmbossBump::CEmbossBump()
{
	m_bIsSetLight = GL_FALSE;
}

CEmbossBump::~CEmbossBump()
{
	ClearAllArrays();
}

GLvoid CEmbossBump::ClearAllArrays()
{
	for( GLint i = 0; i < GetEmbossBumpDataSize(); ++i ) {
		m_aEmbossBumpData[ i ].aTangent.clear();
		m_aEmbossBumpData[ i ].aBinormal.clear();
	}
	m_aEmbossBumpData.clear();

	m_aTexCoords.clear();
	m_aTexCoordsElem.clear();
}

GLvoid CEmbossBump::VMatMult( const CVector4 &in_cLightPos )
{
	GLfloat fMinv[ 16 ];
	glGetFloatv( GL_MODELVIEW_MATRIX, fMinv );

	GLfloat fRes[ 3 ];
	fRes[ 0 ] = fMinv[  0 ] * in_cLightPos.x 
			  + fMinv[  1 ] * in_cLightPos.y
			  + fMinv[  2 ] * in_cLightPos.z
			  + fMinv[  3 ] * in_cLightPos.w;
	fRes[ 1 ] = fMinv[  4 ] * in_cLightPos.x 
			  + fMinv[  5 ] * in_cLightPos.y
			  + fMinv[  6 ] * in_cLightPos.z
			  + fMinv[  7 ] * in_cLightPos.w;
	fRes[ 2 ] = fMinv[  8 ] * in_cLightPos.x
			  + fMinv[  9 ] * in_cLightPos.y
			  + fMinv[ 10 ] * in_cLightPos.z
			  + fMinv[ 11 ] * in_cLightPos.w;	
	
	m_cLight.x = fRes[ 0 ];
	m_cLight.y = fRes[ 1 ];
	m_cLight.z = fRes[ 2 ];
	m_cLight.w = fMinv[ 15 ];

	m_bIsSetLight = GL_TRUE;
}

GLint CEmbossBump::CalcTangents( GLint iIndexObj, GLint iFirstVertex, GLsizei iCountVertices )
{
	GLint iIndex = -1; //indeks m_aEmbossBumpData dla ktorego bedziemy tworzyc tangent i binormal
	//sprawdz czy istanieje indeks obiektu
	GLboolean bIndexExist = GL_FALSE;
	for( GLint i = 0; i < GetEmbossBumpDataSize(); ++i ) {
		if( m_aEmbossBumpData[ i ].iIndexObj == iIndexObj ) {
			m_aEmbossBumpData[ i ].aBinormal.clear();
			m_aEmbossBumpData[ i ].aTangent.clear();
			iIndex = i;
			bIndexExist = GL_TRUE;
			break;
		}
	}
	if( !bIndexExist ) {
		//tworz nowy m_aEmbossBumpData
		SEmbossBumpData sEmbossBumpData;
		sEmbossBumpData.iIndexObj = iIndexObj;
		m_aEmbossBumpData.push_back( sEmbossBumpData );

		iIndex = GetEmbossBumpDataSize() - 1;
	}

	CVector3 P, Q;
	CVector2 st1, st2;
	CVector3 binormal, tangent;
	GLfloat r;

	for( GLint i = iFirstVertex; i < ( iFirstVertex + iCountVertices ); i += 3 )
	{
		P = CVertexArrays::GetInstance()->GetVertex( i + 1 ) - CVertexArrays::GetInstance()->GetVertex( i );
		Q = CVertexArrays::GetInstance()->GetVertex( i + 2 ) - CVertexArrays::GetInstance()->GetVertex( i );

		st1 = CVertexArrays::GetInstance()->GetTexCoord( i + 1 ) - CVertexArrays::GetInstance()->GetTexCoord( i );
		st2 = CVertexArrays::GetInstance()->GetTexCoord( i + 2 ) - CVertexArrays::GetInstance()->GetTexCoord( i );

		if( fabsf( st1.x * st2.y - st2.x * st1.y ) <= 0.0001f )
			r = 1.0f;
		else
			r = 1.0f / ( st1.x * st2.y - st2.x * st1.y );

		tangent.x = r * ( st2.y * P.x - st1.y * Q.x );
		tangent.y = r * ( st2.y * P.y - st1.y * Q.y );
		tangent.z = r * ( st2.y * P.z - st1.y * Q.z );
		
		binormal.x = r * ( st1.x * Q.x - st2.x * P.x );
		binormal.y = r * ( st1.x * Q.y - st2.x * P.y );
		binormal.z = r * ( st1.x * Q.z - st2.x * P.z );

		tangent.Normalize();
		binormal.Normalize();

		for( GLint x = 0; x < 3; ++x ) {
			m_aEmbossBumpData[ iIndex ].aTangent.push_back( tangent );
			m_aEmbossBumpData[ iIndex ].aBinormal.push_back( binormal );
		}
	}

	return iIndex;
}

GLint CEmbossBump::CalcTangentsElem( GLint iIndexObj, GLsizei iCountVertices )
{
	GLint iIndex = -1; //indeks m_aEmbossBumpData dla ktorego bedziemy tworzyc tangent i binormal
	//sprawdz czy istanieje indeks obiektu
	GLboolean bIndexExist = GL_FALSE;
	for( GLint i = 0; i < GetEmbossBumpDataSize(); ++i ) {
		if( m_aEmbossBumpData[ i ].iIndexObj == iIndexObj ) {
			m_aEmbossBumpData[ i ].aBinormal.clear();
			m_aEmbossBumpData[ i ].aTangent.clear();
			iIndex = i;
			bIndexExist = GL_TRUE;
			break;
		}
	}
	if( !bIndexExist ) {
		//tworz nowy m_aEmbossBumpData
		SEmbossBumpData sEmbossBumpData;
		sEmbossBumpData.iIndexObj = iIndexObj;
		m_aEmbossBumpData.push_back( sEmbossBumpData );

		iIndex = GetEmbossBumpDataSize() - 1;
	}

	CVector3 v21, v31;
	CVector2 st1, st2;
	GLfloat r;

	for( GLint i = 0; i < iCountVertices; i += 3 )
	{
		GLint iIndex1 = CVertexArrays::GetInstance()->GetIndexFromIndices( iIndex, i );
		GLint iIndex2 = CVertexArrays::GetInstance()->GetIndexFromIndices( iIndex, i + 1 );
		GLint iIndex3 = CVertexArrays::GetInstance()->GetIndexFromIndices( iIndex, i + 2 );

		if( iIndex1 == -1 || iIndex2 == -1 || iIndex3 == -1 ) {
			__LOG( _T("ERROR: CEmbossBump::CalcTangentsElem - invalid GetIndexFromIndices") );
			return -1;
		}

		v21 = CVertexArrays::GetInstance()->GetVertexElem( iIndex2 ) - CVertexArrays::GetInstance()->GetVertexElem( iIndex1 );
		v31 = CVertexArrays::GetInstance()->GetVertexElem( iIndex3 ) - CVertexArrays::GetInstance()->GetVertexElem( iIndex1 );

		st1 = CVertexArrays::GetInstance()->GetTexCoordElem( iIndex2 ) - CVertexArrays::GetInstance()->GetTexCoordElem( iIndex1 );
		st2 = CVertexArrays::GetInstance()->GetTexCoordElem( iIndex3 ) - CVertexArrays::GetInstance()->GetTexCoordElem( iIndex1 );

		r = 1.0f;
		GLfloat fDenominator = st1.x * st2.y - st2.x * st1.y;
		if( fabsf( fDenominator ) > 0.0001f )
			r = 1.0f / fDenominator;

		CVector3 tangent( ( ( v21 * st2.y ) - ( v31 * st1.y ) ) * r );
		CVector3 binormal( ( ( v31 * st1.x ) - ( v21 * st2.x ) ) * r );

		tangent.Normalize();
		binormal.Normalize();

		for( GLint j = 0; j < 3; ++j ) {
			m_aEmbossBumpData[ iIndex ].aTangent.push_back( tangent );
			m_aEmbossBumpData[ iIndex ].aBinormal.push_back( binormal );
		}
	}

	return iIndex;
}

GLvoid CEmbossBump::SetUpBumps( GLint iIndexObj, GLint iFirstVertex, GLsizei iCountVertices )
{
	if( !m_bIsSetLight ) {
		CVector4 sTemp;
		VMatMult( sTemp );
	}
	m_bIsSetLight = GL_FALSE;

	GLint iIndex = -1;
	GLboolean bIndexExist = GL_FALSE;
	for( GLint i = 0; i < GetEmbossBumpDataSize(); ++i ) {
		if( m_aEmbossBumpData[ i ].iIndexObj == iIndexObj ) {
			iIndex = i;
			bIndexExist = GL_TRUE;
			break;
		}
	}

	if( !bIndexExist )
		iIndex = CalcTangents( iIndexObj, iFirstVertex, iCountVertices );

	CVector3 cPosToLigth;	// wierzcholek dla aktualnej pozycji do swiatla
	CVector2 cOffset;
	GLint j = 0;
		
	//doprodukowanie m_aTexCoords jezeli za malo
	while( CVertexArrays::GetInstance()->GetTexCoordsSize() > GetTexCoordsSize() )
		m_aTexCoords.push_back( CVector2() );

	for( GLint i = iFirstVertex; i < ( iFirstVertex + iCountVertices ); ++i )
	{
		cPosToLigth.x = m_cLight.x - CVertexArrays::GetInstance()->GetVertex( i ).x;
		cPosToLigth.y = m_cLight.y - CVertexArrays::GetInstance()->GetVertex( i ).y;
		cPosToLigth.z = m_cLight.z - CVertexArrays::GetInstance()->GetVertex( i ).z;

		cPosToLigth.Normalize();

		j = i - iFirstVertex;
		cOffset.x = ( m_aEmbossBumpData[ iIndex ].aTangent[ j ].x * cPosToLigth.x
					+ m_aEmbossBumpData[ iIndex ].aTangent[ j ].y * cPosToLigth.y
					+ m_aEmbossBumpData[ iIndex ].aTangent[ j ].z * cPosToLigth.z ) * MAX_EMBOSS;
		cOffset.y = ( m_aEmbossBumpData[ iIndex ].aBinormal[ j ].x * cPosToLigth.x
					+ m_aEmbossBumpData[ iIndex ].aBinormal[ j ].y * cPosToLigth.y
					+ m_aEmbossBumpData[ iIndex ].aBinormal[ j ].z * cPosToLigth.z ) * MAX_EMBOSS;

		CVector2 cOffsetTexCoord( CVertexArrays::GetInstance()->GetTexCoord( i ) + cOffset );

		m_aTexCoords[ i ] = cOffsetTexCoord;
	}
}

GLvoid CEmbossBump::SetUpBumpsElem( GLint iIndexObj, GLsizei iCountVertices )
{
	if( !m_bIsSetLight ) {
		CVector4 sTemp;
		VMatMult( sTemp );
	}
	m_bIsSetLight = GL_FALSE;

	GLint iIndex = -1;
	GLboolean bIndexExist = GL_FALSE;
	for( GLint i = 0; i < GetEmbossBumpDataSize(); ++i ) {
		if( m_aEmbossBumpData[ i ].iIndexObj == iIndexObj ) {
			iIndex = i;
			bIndexExist = GL_TRUE;
			break;
		}
	}

	if( !bIndexExist ) {
		iIndex = CalcTangentsElem( iIndexObj, iCountVertices );
		if( iIndex == -1 ) {
			__LOG( _T("ERROR: CEmbossBump::SetUpBumpsElem - invalid result CalcTangentsElem") );
			return;
		}
	}

	CVector3 cPosToLigth;				// Vertex From Current Position To Light
	CVector2 cOffset;
		
	//doprodukowanie m_aTexCoords jezeli za malo
	while( CVertexArrays::GetInstance()->GetTexCoordsElemSize() > GetTexCoordsElemSize() )
		m_aTexCoordsElem.push_back( CVector2() );

	GLint iCountIndices = 0;
	GLint iIndexVer = 0;

	for( GLint i = 0; i < iCountVertices; ++i )
	{
		iIndexVer = CVertexArrays::GetInstance()->GetIndexFromIndices( iIndex, i );

		if( iIndexVer == -1 ) {
			__LOG( _T("ERROR: CEmbossBump::SetUpBumpsElem - invalid GetIndexFromIndices") );
			return;
		}

		if( iIndexVer == iCountIndices + 1 || iCountIndices == 0 ) {}
		else continue;

		iCountIndices = iIndexVer;

		cPosToLigth.x = m_cLight.x - CVertexArrays::GetInstance()->GetVertexElem( iIndexVer ).x;
		cPosToLigth.y = m_cLight.y - CVertexArrays::GetInstance()->GetVertexElem( iIndexVer ).y;
		cPosToLigth.z = m_cLight.z - CVertexArrays::GetInstance()->GetVertexElem( iIndexVer ).z;

		cPosToLigth.Normalize();

		cOffset.x = ( m_aEmbossBumpData[ iIndex ].aTangent[ i ].x * cPosToLigth.x
					+ m_aEmbossBumpData[ iIndex ].aTangent[ i ].y * cPosToLigth.y
					+ m_aEmbossBumpData[ iIndex ].aTangent[ i ].z * cPosToLigth.z ) * MAX_EMBOSS;
		cOffset.y = ( m_aEmbossBumpData[ iIndex ].aBinormal[ i ].x * cPosToLigth.x
					+ m_aEmbossBumpData[ iIndex ].aBinormal[ i ].y * cPosToLigth.y
					+ m_aEmbossBumpData[ iIndex ].aBinormal[ i ].z * cPosToLigth.z ) * MAX_EMBOSS;

		CVector2 cOffsetTexCoord( CVertexArrays::GetInstance()->GetTexCoordElem( iIndexVer ) + cOffset );

		m_aTexCoordsElem[ iIndexVer ] = cOffsetTexCoord;
	}
}

CVector2 CEmbossBump::GetTexCoord( GLint iIndex )
{
	if( iIndex < 0 || iIndex >= GetTexCoordsSize() )
		return CVector2();

	return m_aTexCoords[ iIndex ];
}

CVector2 CEmbossBump::GetTexCoordElem( GLint iIndex )
{
	if( iIndex < 0 || iIndex >= GetTexCoordsElemSize() )
		return CVector2();

	return m_aTexCoordsElem[ iIndex ];
}