
#include "StdAfx.h"
#include "VertexArrays.h"
#include "../OpenGLFramework/Console.h"
#include "../OpenGLFramework/GameControl.h"
#include "../MatrixOperations/MatrixOperations.h"
#include "3DObjFile.h"

C3DObjFile::C3DObjFile( LPCTSTR lpFileName /*= NULL*/ )
{
	m_pGameCtrl = CGameControl::GetInstance();

#ifndef _3DSLOADER_
	if( lpFileName != NULL ) {
		GLboolean bIsElem;
		LoadObject( lpFileName, bIsElem );
	}
#endif
}

C3DObjFile::~C3DObjFile()
{
}

#ifdef _3DSLOADER_
GLboolean C3DObjFile::GetFileNameSaveObject( LPCTSTR lpFileName, TCHAR* out_cFileName, GLboolean bFixed )
{
	if( !lpFileName )
		return GL_FALSE;

	TCHAR cFileNameTemp[ 256 ] = { 0 };
	::wsprintf( cFileNameTemp, _T("%s"), lpFileName );
	for( GLint i = static_cast< GLint >( _tcslen( cFileNameTemp ) ) - 1; i >= 0; --i ) {
		if( cFileNameTemp[ i ] == '.' ) {
			cFileNameTemp[ i ] = 0;
			break;
		}
	}

	::wsprintf( out_cFileName, bFixed ? _T("%s_fixed.3DObj")
									  : _T("%s.3DObj"), cFileNameTemp );

	return GL_TRUE;
}

GLboolean C3DObjFile::SaveObjectNoElem( LPCTSTR lpFileName, const S3DSObject &in_s3DSObject,
										const CVector3 &in_cScale, const CVector3 &in_cOffsetVector, const CVector3 &in_cRot90,
										GLboolean bSmoothNormal, GLboolean bFixed )
{
	if( !lpFileName )
		return GL_FALSE;

	TCHAR cFileNameFinally[ 256 ] = { 0 };
	GetFileNameSaveObject( lpFileName, cFileNameFinally, bFixed );

	FILE *hFile = NULL;
	if( _tfopen_s( &hFile, cFileNameFinally, _T("wb") ) != 0 )
		return GL_FALSE;

	SaveHeader( hFile, GL_FALSE );

	fwrite( &in_s3DSObject.usVerticesNum, sizeof( GLushort ), 1, hFile );
	fwrite( &in_s3DSObject.usNormalsNum, sizeof( GLushort ), 1, hFile );
	fwrite( &in_s3DSObject.usFacesNum, sizeof( GLushort ), 1, hFile );
	fwrite( &in_s3DSObject.usTexCoordsNum, sizeof( GLushort ), 1, hFile );

	//transformacje wierzcholkow
	for( GLint i = 0; i < in_s3DSObject.usVerticesNum; ++i ) {
		in_s3DSObject.pVertices[ i ] = m_pGameCtrl->GetMatrixOp()->ScaleXYZ3( in_s3DSObject.pVertices[ i ], in_cScale );
		in_s3DSObject.pVertices[ i ] = m_pGameCtrl->GetMatrixOp()->RotateXYZ3( in_s3DSObject.pVertices[ i ], in_cRot90 );
		in_s3DSObject.pVertices[ i ] = m_pGameCtrl->GetMatrixOp()->TranslateXYZ3( in_s3DSObject.pVertices[ i ], in_cOffsetVector );
	}

	for( GLint i = 0; i < in_s3DSObject.usNormalsNum; ++i )
		in_s3DSObject.pNormals[ i ] = m_pGameCtrl->GetMatrixOp()->RotateXYZ3( in_s3DSObject.pNormals[ i ], in_cRot90 );

	CVector2 cTexCoords[ 3 ];

	GLint iNormalsCount = -1;
	for( GLint j = 0; j < in_s3DSObject.usFacesNum; ++j ) {
		GLint v1 = in_s3DSObject.pFaces[ j ].v1;
		GLint v2 = in_s3DSObject.pFaces[ j ].v2;
		GLint v3 = in_s3DSObject.pFaces[ j ].v3;

		if( in_s3DSObject.pTexCoords ) {
			cTexCoords[ 0 ] = in_s3DSObject.pTexCoords[ v1 ];
			cTexCoords[ 1 ] = in_s3DSObject.pTexCoords[ v2 ];
			cTexCoords[ 2 ] = in_s3DSObject.pTexCoords[ v3 ];
		}
		else ::SecureZeroMemory( &cTexCoords, sizeof( CVector2 ) * 3 );

		if( bFixed )
		{
			SaveVertex( hFile, in_s3DSObject.pVertices[ v1 ], cTexCoords[ 0 ], bSmoothNormal ? in_s3DSObject.pNormals[ ++iNormalsCount ]
																							 : in_s3DSObject.pNormals[ v1 ], GL_TRUE );
			SaveVertex( hFile, in_s3DSObject.pVertices[ v2 ], cTexCoords[ 1 ], bSmoothNormal ? in_s3DSObject.pNormals[ ++iNormalsCount ]
																							 : in_s3DSObject.pNormals[ v2 ], GL_TRUE );
			SaveVertex( hFile, in_s3DSObject.pVertices[ v3 ], cTexCoords[ 2 ], bSmoothNormal ? in_s3DSObject.pNormals[ ++iNormalsCount ]
																							 : in_s3DSObject.pNormals[ v3 ], GL_TRUE );
		}
		else
		{
			SaveVertex( hFile, in_s3DSObject.pVertices[ v1 ], cTexCoords[ 0 ], bSmoothNormal ? in_s3DSObject.pNormals[ ++iNormalsCount ]
																							 : in_s3DSObject.pNormals[ v1 ] );
			SaveVertex( hFile, in_s3DSObject.pVertices[ v2 ], cTexCoords[ 1 ], bSmoothNormal ? in_s3DSObject.pNormals[ ++iNormalsCount ]
																							 : in_s3DSObject.pNormals[ v2 ] );
			SaveVertex( hFile, in_s3DSObject.pVertices[ v3 ], cTexCoords[ 2 ], bSmoothNormal ? in_s3DSObject.pNormals[ ++iNormalsCount ]
																							 : in_s3DSObject.pNormals[ v3 ] );
		}
	}

	fclose( hFile );

	return GL_TRUE;
}

GLboolean C3DObjFile::SaveObjectElem( GLint iIndexObject, LPCTSTR lpFileName, GLboolean bFixed )
{
	// Nie trzeba informacji o SmoothNormals (jak dla metody SaveObject), normalne ju¿ s¹ przekonwertowane odpowiednio z uwzglêdnieniem tej opcji.

	if( !lpFileName )
		return GL_FALSE;

	TCHAR cFileNameFinally[ 256 ] = { 0 };
	GetFileNameSaveObject( lpFileName, cFileNameFinally, bFixed );

	FILE *hFile = NULL;
	if( _tfopen_s( &hFile, cFileNameFinally, _T("wb") ) != 0 )
		return GL_FALSE;

	SaveHeader( hFile, GL_TRUE );

	//znajdz ilosc wierzcholkow niepowtarzalnych z danego SubIndices
	GLint iIndex = 0;
	GLint iCountIndices = 0;
	GLushort usVerticesNum = 0;
	for( GLint i = 0; i < CVertexArrays::GetInstance()->GetSubIndicesSize( iIndexObject ); ++i ) {
		iIndex = CVertexArrays::GetInstance()->GetIndexFromIndices( iIndexObject, i );
		if( iIndex == iCountIndices + 1 || iCountIndices == 0 ) {}
		else 
			continue;
		iCountIndices = iIndex;

		++usVerticesNum;
	}

	fwrite( &usVerticesNum, sizeof( GLushort ), 1, hFile ); //zapis ilosci wierzcholkow

	//CVector4 cColor( 1.0f, 1.0f, 1.0f, 1.0f );
	iIndex = 0;
	iCountIndices = 0;
	for( GLint i = 0; i < CVertexArrays::GetInstance()->GetSubIndicesSize( iIndexObject ); ++i ) {
		iIndex = CVertexArrays::GetInstance()->GetIndexFromIndices( iIndexObject, i );
		if( iIndex == iCountIndices + 1 || iCountIndices == 0 ) {}
		else
			continue;

		iCountIndices = iIndex;

		CVector3 cVertex( CVertexArrays::GetInstance()->GetVertexElem( iIndex ) );
		CVector2 cTexCoord( CVertexArrays::GetInstance()->GetTexCoordElem( iIndex ) );
		CVector3 cNormal( CVertexArrays::GetInstance()->GetNormalElem( iIndex ) );

		//zapis wierzcholkow - niepowtarzalnych
		if( bFixed ) {
			SaveFixedVector( hFile, cVertex );
			SaveFixedVector( hFile, cTexCoord );
			SaveFixedVector( hFile, cNormal );
		}
		else {
			fwrite( &cVertex, sizeof( CVector3 ), 1, hFile );
			fwrite( &cTexCoord, sizeof( CVector2 ), 1, hFile );
			fwrite( &cNormal, sizeof( CVector3 ), 1, hFile );
			//fwrite( &cColor, sizeof( CVector4 ), 1, hFile );
		}
	}

	GLushort iTemp = static_cast<GLushort>( CVertexArrays::GetInstance()->GetSubIndicesSize( iIndexObject ) );
	fwrite( &iTemp, sizeof( GLushort ), 1, hFile ); //ilosc indeksow

	for( GLint i = 0; i < CVertexArrays::GetInstance()->GetSubIndicesSize( iIndexObject ); ++i ) {
		iTemp = static_cast<GLushort>( CVertexArrays::GetInstance()->GetIndexFromIndices( iIndexObject, i ) );
		fwrite( &iTemp, sizeof( GLushort ), 1, hFile ); //zapis indeksu
	}

	fclose( hFile );

	return GL_TRUE;
}

GLboolean C3DObjFile::SaveHeader( FILE *hFile, GLboolean bIsElem )
{
	if( !hFile )
		return GL_FALSE;

	GLushort usVersionMajor = _3DOBJFILEVERSION_MAJOR;
	GLushort usVersionMinor = _3DOBJFILEVERSION_MINOR;

	char cHeader[ 14 ];
	strcpy_s( cHeader, 14, _3DOBJFILE_HEADER );

	fwrite( cHeader, sizeof( cHeader ), 1, hFile );
	fwrite( &usVersionMajor, sizeof( GLushort ), 1, hFile );
	fwrite( &usVersionMinor, sizeof( GLushort ), 1, hFile );
	fwrite( &bIsElem, sizeof( GLboolean ), 1, hFile );

	return GL_TRUE;
}

GLvoid C3DObjFile::SaveVertex( FILE* hFile, const CVector3& in_cVertex, const CVector2& in_cTexCoord, const CVector3& in_cNormal, GLboolean bFixed )
{
	if( !hFile )
		return;

	if( bFixed )
	{
		SaveFixedVector( hFile, in_cVertex );
		SaveFixedVector( hFile, in_cTexCoord );
		SaveFixedVector( hFile, in_cNormal );
	}
	else
	{
		fwrite( &in_cVertex, sizeof( CVector3 ), 1, hFile );
		fwrite( &in_cTexCoord, sizeof( CVector2 ), 1, hFile );
		fwrite( &in_cNormal, sizeof( CVector3 ), 1, hFile );
	}
}

GLvoid C3DObjFile::SaveFixedVector( FILE* hFile, const CVector3& in_cVec )
{
	GLint iX( FLOAT_TO_FIXED( in_cVec.x ) );
	GLint iY( FLOAT_TO_FIXED( in_cVec.y ) );
	GLint iZ( FLOAT_TO_FIXED( in_cVec.z ) );

	fwrite( &iX, sizeof( GLint ), 1, hFile );
	fwrite( &iY, sizeof( GLint ), 1, hFile );
	fwrite( &iZ, sizeof( GLint ), 1, hFile );
}

GLvoid C3DObjFile::SaveFixedVector( FILE* hFile, const CVector2& in_cVec )
{
	GLint iX( FLOAT_TO_FIXED( in_cVec.x ) );
	GLint iY( FLOAT_TO_FIXED( in_cVec.y ) );

	fwrite( &iX, sizeof( GLint ), 1, hFile );
	fwrite( &iY, sizeof( GLint ), 1, hFile );
}

#endif // _3DSLOADER_

GLboolean C3DObjFile::LoadHeader( FILE *hFile, LPCTSTR lpFileName, GLboolean &out_bIsElem )
{
	if( !hFile )
		return GL_FALSE;

	char cHeader[ 14 ];
	fread( cHeader, sizeof( cHeader ), 1, hFile );
	if( strcmp( cHeader, _3DOBJFILE_HEADER ) != 0 ) {
		CONSOLE_ADDTEXT( CConsole::EError, _T("ERROR: Load %s file failed - incorrect header"), lpFileName );
		return GL_FALSE;
	}

	GLushort usVersionMajor;
	GLushort usVersionMinor;
	fread( &usVersionMajor, sizeof( GLushort ), 1, hFile );
	fread( &usVersionMinor, sizeof( GLushort ), 1, hFile );
	fread( &out_bIsElem, sizeof( GLboolean ), 1, hFile );
	if( usVersionMajor != _3DOBJFILEVERSION_MAJOR || usVersionMinor != _3DOBJFILEVERSION_MINOR ) {
		CONSOLE_ADDTEXT( CConsole::EError, _T("ERROR: Load %s file failed - incorrect file version"), lpFileName );
		return GL_FALSE;
	}

	return GL_TRUE;
}

GLboolean C3DObjFile::LoadObject( LPCTSTR lpFileName, GLboolean &out_bIsElem )
{
	if( !lpFileName )
		return GL_FALSE;

	FILE *hFile = NULL;
	if( _tfopen_s( &hFile, lpFileName, _T("rb") ) != 0 )
		return GL_FALSE;

	if( !LoadHeader( hFile, lpFileName, out_bIsElem ) ) {
		if( hFile )
			fclose( hFile );

		return GL_FALSE;
	}

	if( out_bIsElem )
		LoadDataElem( hFile );
	else
		LoadDataNoElem( hFile );

	fclose( hFile );

	return GL_TRUE;
}

GLboolean C3DObjFile::LoadDataNoElem( FILE *hFile )
{
	m_iVerticesNum = 0;

	GLushort usVerticesNum;
	GLushort usNormalsNum;
	GLushort usFacesNum;
	GLushort usTexCoordsNum;

	CVector3 cVertex;
	CVector2 cTexCoord;
	CVector3 cNormal;

	fread( &usVerticesNum, sizeof( GLushort ), 1, hFile );
	fread( &usNormalsNum, sizeof( GLushort ), 1, hFile );
	fread( &usFacesNum, sizeof( GLushort ), 1, hFile );
	fread( &usTexCoordsNum, sizeof( GLushort ), 1, hFile );

	for( GLushort j = 0; j < usFacesNum; ++j ) {

		fread( &cVertex, sizeof( cVertex ), 1, hFile );
		fread( &cTexCoord, sizeof( cTexCoord ), 1, hFile );
		fread( &cNormal, sizeof( cNormal ), 1, hFile );

		CVertexArrays::GetInstance()->AddFullVertex( cVertex, cTexCoord, cNormal );

		fread( &cVertex, sizeof( cVertex ), 1, hFile );
		fread( &cTexCoord, sizeof( cTexCoord ), 1, hFile );
		fread( &cNormal, sizeof( cNormal ), 1, hFile );

		CVertexArrays::GetInstance()->AddFullVertex( cVertex, cTexCoord, cNormal );

		fread( &cVertex, sizeof( cVertex ), 1, hFile );
		fread( &cTexCoord, sizeof( cTexCoord ), 1, hFile );
		fread( &cNormal, sizeof( cNormal ), 1, hFile );

		CVertexArrays::GetInstance()->AddFullVertex( cVertex, cTexCoord, cNormal );

		m_iVerticesNum += 3;
	}

	return GL_TRUE;
}

GLboolean C3DObjFile::LoadDataElem( FILE *hFile )
{
	m_iVerticesNum = 0;

	GLushort usVerticesNum;
	fread( &usVerticesNum, sizeof( GLushort ), 1, hFile ); //odczyt ilosci wierzcholkow

	CVector3 cVertex;
	CVector2 cTexCoord;
	CVector3 cNormal;

	//pobranie aktualnej ilosc ElemVertices
	GLint iElemVertSize = CVertexArrays::GetInstance()->GetVerticesElemSize();

	for( GLushort i = 0; i < usVerticesNum; ++i ) {
		//odczyt wierzcholkow - niepowtarzalnych
		fread( &cVertex, sizeof( CVector3 ), 1, hFile );
		fread( &cTexCoord, sizeof( CVector2 ), 1, hFile );
		fread( &cNormal, sizeof( CVector3 ), 1, hFile );

		CVertexArrays::GetInstance()->AddVertexElem( cVertex );
		CVertexArrays::GetInstance()->AddTexCoordElem( cTexCoord );
		CVertexArrays::GetInstance()->AddNormalElem( cNormal );
	}

	m_iVerticesNum = static_cast<GLint>( usVerticesNum );

	GLushort usIndicesNum;
	fread( &usIndicesNum, sizeof( GLushort ), 1, hFile ); //ilosc indeksow

	std::vector< GLushort > aSingleShortIndices;
	GLushort usIndex;
	for( GLushort i = 0; i < usIndicesNum; ++i ) {
		fread( &usIndex, sizeof( GLushort ), 1, hFile ); //odczyt indeksu
		//poniewaz moga juz byc wczytane inne obiekty musimy zwiekszyc indeks o istniejace juz indeksy
		usIndex = usIndex + static_cast<GLushort>( iElemVertSize );
		aSingleShortIndices.push_back( usIndex );
	}
	CVertexArrays::GetInstance()->AddSubIndices( aSingleShortIndices );

	return GL_TRUE;
}