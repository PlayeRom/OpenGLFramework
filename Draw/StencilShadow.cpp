
#include "StdAfx.h"
#include "VertexArrays.h"
#include "3DObjManager.h"
#include "../OpenGLFramework/GameControl.h"
#include "../OpenGLFramework/Console.h"
#include "StencilShadow.h"

#define MAX_VERTICES_ARRAY	4096

CStencilShadow::CStencilShadow()
{
	m_bIsGL_EXT_stencil_wrap = GL_FALSE;
	if( strstr( ( char* )glGetString( GL_EXTENSIONS ), "GL_EXT_stencil_wrap" ) ) {
		m_bIsGL_EXT_stencil_wrap = GL_TRUE;
		CONSOLE_ADDTEXT( CConsole::ESuccess, _T("GL_EXT_stencil_wrap supported") );
	}
	else
		CONSOLE_ADDTEXT( CConsole::EError, _T("GL_EXT_stencil_wrap not supported") );

	for( GLint i = 0; i < MAX_VERTICES_ARRAY; ++i )
		m_aVertices.push_back( CVector3() );
}

CStencilShadow::~CStencilShadow()
{
	m_aVertices.clear();

	ClearObjFaces();
	ClearObjTransformInfo();
}

GLvoid CStencilShadow::ClearObjFaces()
{
	for( GLint i = GetObjFacesSize() - 1; i >= 0; --i )
		m_aObjFaces[ i ].aFaceArray.clear();
		
	m_aObjFaces.clear();
}

GLvoid CStencilShadow::ClearObjTransformInfo()
{
	for( GLint i = GetObjTransformInfoSize() - 1; i >= 0; --i )
		m_aObjTransformInfo[ i ].aTransform.clear();
		
	m_aObjTransformInfo.clear();
}

GLvoid CStencilShadow::RestartObjects()
{
	ClearObjFaces();
	ClearObjTransformInfo();
}

GLvoid CStencilShadow::CreateFaceEquationElem( GLint iIndexObj, GLsizei iCountVertices )
{
	GLint iIndex = -1; //indeks m_aEmbossBumpData dla ktorego bedziemy tworzyc tangent i binormal
	//sprawdz czy istanieje indeks obiektu
	GLboolean bIndexExist = GL_FALSE;
	for( GLint i = 0; i < GetObjFacesSize(); ++i ) {
		if( m_aObjFaces[ i ].iIndexObj == iIndexObj ) {
			m_aObjFaces[ i ].aFaceArray.clear();
			iIndex = i;
			bIndexExist = GL_TRUE;
			break;
		}
	}
	if( !bIndexExist ) {
		//tworz nowy m_aObjFaces
		SObjFaces sObjFaces;
		sObjFaces.iIndexObj = iIndexObj;
		sObjFaces.bIsElem = GL_TRUE;
		m_aObjFaces.push_back( sObjFaces );

		iIndex = GetObjFacesSize() - 1;
	}

	CVector3 v[ 3 ];

	for( GLint i = 0; i < iCountVertices; i += 3 ) {
		SFaceEquation sFaceEq;
		sFaceEq.bVisible = GL_FALSE;
		sFaceEq.aIndexFace[ 0 ] = CVertexArrays::GetInstance()->GetIndexFromIndices( iIndex, i );
		sFaceEq.aIndexFace[ 1 ] = CVertexArrays::GetInstance()->GetIndexFromIndices( iIndex, i + 1 );
		sFaceEq.aIndexFace[ 2 ] = CVertexArrays::GetInstance()->GetIndexFromIndices( iIndex, i + 2 );
		sFaceEq.iNeighbour[ 0 ] = -1;
		sFaceEq.iNeighbour[ 1 ] = -1;
		sFaceEq.iNeighbour[ 2 ] = -1;

		if( sFaceEq.aIndexFace[ 0 ] == -1 || sFaceEq.aIndexFace[ 1 ] == -1 || sFaceEq.aIndexFace[ 2 ] == -1 ) {
			__LOG( _T("ERROR: CStencilShadow::CreateFaceEquationElem - invalid GetIndexFromIndices") );
			return;
		}

		for( GLint i = 0; i < 3; ++i )	//pobieramy wspolrzedne wierzcholkow dla danego trojkata
			v[ i ] = CVertexArrays::GetInstance()->GetVertexElem( sFaceEq.aIndexFace[ i ] );

		//obliczamy [a, b, c, d] rownania plaszczyzny
		sFaceEq.cFaceEq.x = v[ 0 ].y * ( v[ 1 ].z - v[ 2 ].z ) + v[ 1 ].y * ( v[ 2 ].z - v[ 0 ].z ) + v[ 2 ].y * ( v[ 0 ].z - v[ 1 ].z );
		sFaceEq.cFaceEq.y = v[ 0 ].z * ( v[ 1 ].x - v[ 2 ].x ) + v[ 1 ].z * ( v[ 2 ].x - v[ 0 ].x ) + v[ 2 ].z * ( v[ 0 ].x - v[ 1 ].x );
		sFaceEq.cFaceEq.z = v[ 0 ].x * ( v[ 1 ].y - v[ 2 ].y ) + v[ 1 ].x * ( v[ 2 ].y - v[ 0 ].y ) + v[ 2 ].x * ( v[ 0 ].y - v[ 1 ].y );
		sFaceEq.cFaceEq.w = -( v[ 0 ].x * ( v[ 1 ].y * v[ 2 ].z - v[ 2 ].y * v[ 1 ].z )
							 + v[ 1 ].x * ( v[ 2 ].y * v[ 0 ].z - v[ 0 ].y * v[ 2 ].z )
							 + v[ 2 ].x * ( v[ 0 ].y * v[ 1 ].z - v[ 1 ].y * v[ 0 ].z ) );

		m_aObjFaces[ iIndex ].aFaceArray.push_back( sFaceEq );
	}

	CalculateNeighbours( iIndex );
}

GLvoid CStencilShadow::CreateFaceEquation( GLint iIndexObj, GLint iFirstVertex, GLsizei iCountVertices )
{
	/*
	UWAGA - NIGDY NIE TESTOWANE
	*/

	GLint iIndex = -1; //indeks m_aEmbossBumpData dla ktorego bedziemy tworzyc tangent i binormal
	//sprawdz czy istanieje indeks obiektu
	GLboolean bIndexExist = GL_FALSE;
	for( GLint i = 0; i < GetObjFacesSize(); ++i ) {
		if( m_aObjFaces[ i ].iIndexObj == iIndexObj ) {
			m_aObjFaces[ i ].aFaceArray.clear();
			iIndex = i;
			bIndexExist = GL_TRUE;
			break;
		}
	}
	if( !bIndexExist ) {
		//tworz nowy m_aEmbossBumpData
		SObjFaces sObjFaces;
		sObjFaces.iIndexObj = iIndexObj;
		sObjFaces.bIsElem = GL_FALSE;
		m_aObjFaces.push_back( sObjFaces );

		iIndex = GetObjFacesSize() - 1;
	}

	CVector3 v[ 3 ];

	for( GLint i = iFirstVertex; i < ( iFirstVertex + iCountVertices ); i += 3 ) {
		SFaceEquation sFaceEq;
		sFaceEq.bVisible = GL_FALSE;
		sFaceEq.aIndexFace[ 0 ] = i;
		sFaceEq.aIndexFace[ 1 ] = i + 1;
		sFaceEq.aIndexFace[ 2 ] = i + 2;
		sFaceEq.iNeighbour[ 0 ] = -1;
		sFaceEq.iNeighbour[ 1 ] = -1;
		sFaceEq.iNeighbour[ 2 ] = -1;

		v[ 0 ] = CVertexArrays::GetInstance()->GetVertex( i );
		v[ 1 ] = CVertexArrays::GetInstance()->GetVertex( i + 1 );
		v[ 2 ] = CVertexArrays::GetInstance()->GetVertex( i + 2 );

		//obliczamy [a, b, c, d] rownania plaszczyzny
		sFaceEq.cFaceEq.x = v[ 0 ].y * ( v[ 1 ].z - v[ 2 ].z ) + v[ 1 ].y * ( v[ 2 ].z - v[ 0 ].z ) + v[ 2 ].y * ( v[ 0 ].z - v[ 1 ].z );
		sFaceEq.cFaceEq.y = v[ 0 ].z * ( v[ 1 ].x - v[ 2 ].x ) + v[ 1 ].z * ( v[ 2 ].x - v[ 0 ].x ) + v[ 2 ].z * ( v[ 0 ].x - v[ 1 ].x );
		sFaceEq.cFaceEq.z = v[ 0 ].x * ( v[ 1 ].y - v[ 2 ].y ) + v[ 1 ].x * ( v[ 2 ].y - v[ 0 ].y ) + v[ 2 ].x * ( v[ 0 ].y - v[ 1 ].y );
		sFaceEq.cFaceEq.w = -( v[ 0 ].x * ( v[ 1 ].y * v[ 2 ].z - v[ 2 ].y * v[ 1 ].z )
							 + v[ 1 ].x * ( v[ 2 ].y * v[ 0 ].z - v[ 0 ].y * v[ 2 ].z )
							 + v[ 2 ].x * ( v[ 0 ].y * v[ 1 ].z - v[ 1 ].y * v[ 0 ].z ) );

		m_aObjFaces[ iIndex ].aFaceArray.push_back( sFaceEq );
	}

	CalculateNeighbours( iIndex );
}

GLvoid CStencilShadow::CalculateNeighbours( GLint iIndex )
{
	//wyczukiwanie sasiadujacych
	GLint p1i, p2i, p1j, p2j;
	GLboolean bEdgeFound = GL_FALSE;
	GLint iFacesNumber = GetFaceArraySize( iIndex );
	for( GLint i = 0; i < iFacesNumber - 1; ++i ) {
		for( GLint ki = 0; ki < 3; ++ki ) { //petla po iNeighbour z trojkata po petli i
			bEdgeFound = GL_FALSE;
			if( m_aObjFaces[ iIndex ].aFaceArray[ i ].iNeighbour[ ki ] == -1 ) { //na poczatku zawsze == 0
				for( GLint j = i + 1; j < iFacesNumber; ++j ) { //porownanie trojkatow, kazdy z kazdym
					for( GLint kj = 0; kj < 3; ++kj ) { //petla po iNeighbour z trojkata po petli j

						//obliczenia w poszukiwaniu wspolnej krawedzi
						//tj. krawedzi takich dwoch face'ow, ktore te krawedzie maja
						//pare identycznych indeksow wierzcholkow

						p1i = m_aObjFaces[ iIndex ].aFaceArray[ i ].aIndexFace[ ki ];
						p2i = m_aObjFaces[ iIndex ].aFaceArray[ i ].aIndexFace[ ( ki + 1 ) % 3 ];
						p1j = m_aObjFaces[ iIndex ].aFaceArray[ j ].aIndexFace[ kj ];
						p2j = m_aObjFaces[ iIndex ].aFaceArray[ j ].aIndexFace[ ( kj + 1 ) % 3 ];

						if( ( p1i == p1j && p2i == p2j ) || ( p1i == p2j && p2i == p1j ) ) {
							//mamy wspolna krawedz dla dwoch face'ow

							//dla face'a pierwszego zapisujemy pod
							//odpowiednim iNeighbour dla korego wystapilo sasiedztwo
							//zapisujemy index sasiedniego face'a
							m_aObjFaces[ iIndex ].aFaceArray[ i ].iNeighbour[ ki ] = j;

							//dla face'a drugiego zapisujemy pod
							//odpowiednim iNeighbour dla ktorego wystapilo sasiedztwo
							//zapisujemy index sasiedniego face'a
							m_aObjFaces[ iIndex ].aFaceArray[ j ].iNeighbour[ kj ] = i;
							bEdgeFound = GL_TRUE;
							break;
						}
					}
					if( bEdgeFound )
						break;
				}
			}
		}
	}
}

GLvoid CStencilShadow::SetVisibleFaces( GLint iIndexObj, const CVector4 &in_cLightPos )
{
	GLint iFaceNumber = GetFaceArraySize( iIndexObj );
	for( GLint i = 0; i < iFaceNumber; ++i ) {
		//obliczamy ( dot product ) czy dany trojtak obiektu znajduje sie przodem czy tylem do
		//swiatla, innaczej czy jest widoczny przez swiatlo - czy jest oswietlony
		GLfloat fSide =	m_aObjFaces[ iIndexObj ].aFaceArray[ i ].cFaceEq.DotProduct( in_cLightPos );
		
		m_aObjFaces[ iIndexObj ].aFaceArray[ i ].bVisible = fSide > 0.0f ? GL_TRUE : GL_FALSE;
	}
}

GLvoid CStencilShadow::StartStencil()
{
	glDisable( GL_LIGHTING );
	glDepthMask( GL_FALSE );
	glDepthFunc( GL_LEQUAL );

	glEnable( GL_STENCIL_TEST );
	glColorMask( GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE );
	glStencilFunc( GL_ALWAYS, 0x00000001, 0xffffffff );
}

GLvoid CStencilShadow::FinishStencil()
{
	//draw a shadowing rectangle covering the entire screen
	glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE );
	glColor4f( 0.0f, 0.0f, 0.0f, 0.6f ); //tutaj mozemy zmienic kolor cienia, wartosc alpha zwieksza intensywnosc ciania, np. 1.0 - cien bardzo mocny
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glStencilFunc( GL_NOTEQUAL, 0x00000000, 0xffffffff );
	glStencilOp( GL_ZERO, GL_ZERO, GL_ZERO );

	CGameControl::GetInstance()->Enable2D();
	CGameControl::GetInstance()->DrawQuadOnAllScreen();
	CGameControl::GetInstance()->Disable2D();

	glDisable( GL_BLEND );
	glDepthFunc( GL_LEQUAL );
	glDepthMask( GL_TRUE );
	glEnable( GL_LIGHTING );
	glDisable( GL_STENCIL_TEST );
}

GLvoid CStencilShadow::DrawShadowForObj( GLint iIndexObj, const CVector4 &in_cLightPos )
{
	if( iIndexObj < 0 || iIndexObj >= GetObjFacesSize() )
		return;

	PROFILER_BEGIN( _T("CStencilShadow::DrawShadowForObj") );

	SetVisibleFaces( iIndexObj, in_cLightPos );
	StartStencil();

	// first pass, stencil operation decreases stencil value
	glFrontFace( GL_CCW );
	if( m_bIsGL_EXT_stencil_wrap )
		glStencilOp( GL_KEEP, GL_KEEP, GL_INCR_WRAP );
	else
		glStencilOp( GL_KEEP, GL_KEEP, GL_INCR );

	if( m_aObjFaces[ iIndexObj ].bIsElem )
		DoDrawShadowForObjElem( iIndexObj, in_cLightPos ); //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	else
		DoDrawShadowForObjNoElem( iIndexObj, in_cLightPos );

	// second pass, stencil operation increases stencil value
	glFrontFace( GL_CW );
	if( m_bIsGL_EXT_stencil_wrap )
		glStencilOp( GL_KEEP, GL_KEEP, GL_DECR_WRAP );
	else
		glStencilOp( GL_KEEP, GL_KEEP, GL_DECR );

	if( m_aObjFaces[ iIndexObj ].bIsElem )
		DoDrawShadowForObjElem( iIndexObj, in_cLightPos ); //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	else
		DoDrawShadowForObjNoElem( iIndexObj, in_cLightPos );

	glFrontFace( GL_CCW );

	FinishStencil();

	PROFILER_END();
}

GLvoid CStencilShadow::Begin_ObjTransfInfo()
{
	ClearObjTransformInfo();
	::SecureZeroMemory( &m_sObjTransfInfo, sizeof( m_sObjTransfInfo ) );
}

GLvoid CStencilShadow::Set_ObjTransfInfo( GLint iIndexObj, const CVector4 &in_cLightPos )
{
	m_sObjTransfInfo.iIndexObj = iIndexObj;
	m_sObjTransfInfo.cLightPos = in_cLightPos;
	m_sObjTransfInfo.aTransform.clear();
}

GLvoid CStencilShadow::Translate_ObjTransfInfo( const CVector3 &in_cTranslate )
{
	STransform sTranslatef;
	sTranslatef.eTransfMode = ETransfTranslate;
	sTranslatef.cTransform = in_cTranslate;
	m_sObjTransfInfo.aTransform.push_back( sTranslatef );
}

GLvoid CStencilShadow::Rotate_ObjTransfInfo( GLfloat fAngle, const CVector3 &in_cAxesRot )
{
	STransform sRot;
	sRot.eTransfMode = ETransfRotate;
	sRot.cTransform = in_cAxesRot;
	sRot.fAngleRot = fAngle;
	m_sObjTransfInfo.aTransform.push_back( sRot );
}

GLvoid CStencilShadow::Scale_ObjTransfInfo( const CVector3 &in_cScale )
{
	STransform sRot;
	sRot.eTransfMode = ETransfScale;
	sRot.cTransform = in_cScale;
	m_sObjTransfInfo.aTransform.push_back( sRot );
}

GLvoid CStencilShadow::End_ObjTransfInfo()
{
	m_aObjTransformInfo.push_back( m_sObjTransfInfo );
}

/*
Rysuje cien dla kilku obiektow na raz
*/
GLvoid CStencilShadow::DrawShadowForObjects()
{
	if( GetObjTransformInfoSize() == 0 )
		return; //pusta tablica, nic nie rob

	PROFILER_BEGIN( _T("CStencilShadow::DrawShadowForObjects") );

	for( GLint j = 0; j < GetObjTransformInfoSize(); ++j )
		SetVisibleFaces( m_aObjTransformInfo[ j ].iIndexObj, m_aObjTransformInfo[ j ].cLightPos );

	StartStencil();

	// first pass, stencil operation decreases stencil value
	glFrontFace( GL_CCW );
	if( m_bIsGL_EXT_stencil_wrap )
		glStencilOp( GL_KEEP, GL_KEEP, GL_INCR_WRAP );
	else
		glStencilOp( GL_KEEP, GL_KEEP, GL_INCR );
	glPushMatrix();

	PROFILER_BEGIN( _T("CStencilShadow::DrawShadowForObjects - GL_INCR") );

	for( GLint i = 0; i < GetObjTransformInfoSize(); ++i ) {
		glLoadIdentity();
		for( GLint j = 0; j < static_cast<GLint>( m_aObjTransformInfo[ i ].aTransform.size() ); ++j ) {
			switch( m_aObjTransformInfo[ i ].aTransform[ j ].eTransfMode ) {
				case ETransfTranslate:
					glTranslatef( m_aObjTransformInfo[ i ].aTransform[ j ].cTransform.x,
								  m_aObjTransformInfo[ i ].aTransform[ j ].cTransform.y,
								  m_aObjTransformInfo[ i ].aTransform[ j ].cTransform.z );
					break;
				case ETransfRotate:
					glRotatef( m_aObjTransformInfo[ i ].aTransform[ j ].fAngleRot,
							   m_aObjTransformInfo[ i ].aTransform[ j ].cTransform.x,
							   m_aObjTransformInfo[ i ].aTransform[ j ].cTransform.y,
							   m_aObjTransformInfo[ i ].aTransform[ j ].cTransform.z );
					break;
				case ETransfScale:
					glScalef( m_aObjTransformInfo[ i ].aTransform[ j ].cTransform.x,
							  m_aObjTransformInfo[ i ].aTransform[ j ].cTransform.y,
							  m_aObjTransformInfo[ i ].aTransform[ j ].cTransform.z );
					break;
			}
		}
		if( m_aObjFaces[ m_aObjTransformInfo[ i ].iIndexObj ].bIsElem )
			DoDrawShadowForObjElem( m_aObjTransformInfo[ i ].iIndexObj, m_aObjTransformInfo[ i ].cLightPos ); //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		else
			DoDrawShadowForObjNoElem( m_aObjTransformInfo[ i ].iIndexObj, m_aObjTransformInfo[ i ].cLightPos );
	}

	PROFILER_END();

	glPopMatrix();
	// second pass, stencil operation increases stencil value
	glFrontFace( GL_CW );
	if( m_bIsGL_EXT_stencil_wrap )
		glStencilOp( GL_KEEP, GL_KEEP, GL_DECR_WRAP );
	else
		glStencilOp( GL_KEEP, GL_KEEP, GL_DECR );
	glPushMatrix();

	PROFILER_BEGIN( _T("CStencilShadow::DrawShadowForObjects - GL_DECR") );

	for( GLint i = 0; i < GetObjTransformInfoSize(); ++i ) {
		glLoadIdentity();
		for( GLint j = 0; j < static_cast<GLint>( m_aObjTransformInfo[ i ].aTransform.size() ); ++j ) {
			switch( m_aObjTransformInfo[ i ].aTransform[ j ].eTransfMode ) {
				case ETransfTranslate:
					glTranslatef( m_aObjTransformInfo[ i ].aTransform[ j ].cTransform.x,
								  m_aObjTransformInfo[ i ].aTransform[ j ].cTransform.y,
								  m_aObjTransformInfo[ i ].aTransform[ j ].cTransform.z );
					break;
				case ETransfRotate:
					glRotatef( m_aObjTransformInfo[ i ].aTransform[ j ].fAngleRot,
							   m_aObjTransformInfo[ i ].aTransform[ j ].cTransform.x,
							   m_aObjTransformInfo[ i ].aTransform[ j ].cTransform.y,
							   m_aObjTransformInfo[ i ].aTransform[ j ].cTransform.z );
					break;
				case ETransfScale:
					glScalef( m_aObjTransformInfo[ i ].aTransform[ j ].cTransform.x,
							  m_aObjTransformInfo[ i ].aTransform[ j ].cTransform.y,
							  m_aObjTransformInfo[ i ].aTransform[ j ].cTransform.z );
					break;
			}
		}
		if( m_aObjFaces[ m_aObjTransformInfo[ i ].iIndexObj ].bIsElem )
			DoDrawShadowForObjElem( m_aObjTransformInfo[ i ].iIndexObj, m_aObjTransformInfo[ i ].cLightPos ); //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		else
			DoDrawShadowForObjNoElem( m_aObjTransformInfo[ i ].iIndexObj, m_aObjTransformInfo[ i ].cLightPos );
	}
	
	PROFILER_END();
	
	glPopMatrix();

	glFrontFace( GL_CCW );
	
	FinishStencil();

	PROFILER_END();
}

GLvoid CStencilShadow::DoDrawShadowForObjElem( GLint iIndexObj, const CVector4 &in_cLightPos )
{
	PROFILER_BEGIN( _T("CStencilShadow::DoDrawShadowForObjElem") );

	GLint p1, p2;
	CVector3 v1, v2;
	GLint iNeighValue;

	GLint iFaceNumber = GetFaceArraySize( iIndexObj );
	GLint iVerCounter = -1;

	for( GLint i = 0; i < iFaceNumber; ++i ) //petla po trojkatach
	{
		if( m_aObjFaces[ iIndexObj ].aFaceArray[ i ].bVisible ) //czy trojkat oswietlony
		{
			for( GLint j = 0; j < 3; ++j ) //petla po krawedziach
			{
				iNeighValue = m_aObjFaces[ iIndexObj ].aFaceArray[ i ].iNeighbour[ j ];
				if( iNeighValue == -1 || !m_aObjFaces[ iIndexObj ].aFaceArray[ iNeighValue ].bVisible )
				{
					//tutaj mamy krawedŸ, wiec musimy narysowac poligon
					p1 = m_aObjFaces[ iIndexObj ].aFaceArray[ i ].aIndexFace[ j ];
					p2 = m_aObjFaces[ iIndexObj ].aFaceArray[ i ].aIndexFace[ ( j + 1 ) % 3 ];

					//obliczenie dlugosci wektowa
					v1 = ( CVertexArrays::GetInstance()->GetVertexElem( p1 ) - in_cLightPos ) * MAX_DISTANCE_VIEW;
					v2 = ( CVertexArrays::GetInstance()->GetVertexElem( p2 ) - in_cLightPos ) * MAX_DISTANCE_VIEW;

					//dodawanie indeksów do tablicy dla rysowania GL_QUADS:
					m_aVertices[ ++iVerCounter ] = CVertexArrays::GetInstance()->GetVertexElemRef( p2 );
					m_aVertices[ ++iVerCounter ] = CVertexArrays::GetInstance()->GetVertexElemRef( p1 );
					m_aVertices[ ++iVerCounter ] = CVertexArrays::GetInstance()->GetVertexElem( p1 ) + v1;
					m_aVertices[ ++iVerCounter ] = CVertexArrays::GetInstance()->GetVertexElem( p2 ) + v2;
				}
			}
		}
	}

	DrawArrays( EFloat, &m_aVertices[ 0 ], GL_QUADS, iVerCounter + 1 );

	PROFILER_END();
}

GLvoid CStencilShadow::DoDrawShadowForObjNoElem( GLint iIndexObj, const CVector4 &in_cLightPos )
{
	GLint p1, p2;
	CVector3 v1, v2;
	GLint iNeighValue;

	GLint iFaceNumber = GetFaceArraySize( iIndexObj );
	GLint iVerCounter = -1;

	for( GLint i = 0; i < iFaceNumber; ++i ) {						//petla po trojkatach
		if( m_aObjFaces[ iIndexObj ].aFaceArray[ i ].bVisible ) {	//czy trojkat oswietlony
			for( GLint j = 0; j < 3; ++j ) 							//petla po krawedziach
			{
				iNeighValue = m_aObjFaces[ iIndexObj ].aFaceArray[ i ].iNeighbour[ j ];
				if( iNeighValue == -1 || !m_aObjFaces[ iIndexObj ].aFaceArray[ iNeighValue ].bVisible ) {
					//tutaj mamy krawedŸ, wiec musimy narysowac poligon
					p1 = m_aObjFaces[ iIndexObj ].aFaceArray[ i ].aIndexFace[ j ];
					p2 = m_aObjFaces[ iIndexObj ].aFaceArray[ i ].aIndexFace[ ( j + 1 ) % 3 ];

					//calculate the length of the vector
					v1 = ( CVertexArrays::GetInstance()->GetVertexElem( p1 ) - in_cLightPos ) * MAX_DISTANCE_VIEW;
					v2 = ( CVertexArrays::GetInstance()->GetVertexElem( p2 ) - in_cLightPos ) * MAX_DISTANCE_VIEW;
					
					m_aVertices[ ++iVerCounter ] = CVertexArrays::GetInstance()->GetVertex( p1 );
					m_aVertices[ ++iVerCounter ] = CVertexArrays::GetInstance()->GetVertex( p1 ) + v1;
					m_aVertices[ ++iVerCounter ] = CVertexArrays::GetInstance()->GetVertex( p2 );
					m_aVertices[ ++iVerCounter ] = CVertexArrays::GetInstance()->GetVertex( p2 ) + v2;
				}
			}
		}
	}

	DrawArrays( EFloat, &m_aVertices[ 0 ], GL_TRIANGLE_STRIP, iVerCounter + 1 );
}

GLvoid CStencilShadow::DrawArrays( EDrawArrayMode eMode, GLvoid* pArray, GLenum uiDAMode, GLsizei iCount )
{
	glEnableClientState( GL_VERTEX_ARRAY );
	glDisableClientState( GL_NORMAL_ARRAY );
	glDisableClientState( GL_COLOR_ARRAY );
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );

	if( eMode == EFloat )
		glVertexPointer( 3, GL_FLOAT, 0, pArray );
	else if( eMode == EInt )
		glVertexPointer( 2, GL_INT, 0, pArray );

	glDrawArrays( uiDAMode, 0, iCount );
}

// Rezultat tej funkcji trzeba wrzucic do DrawShadowForObj() jako in_cLightPos
CVector4 CStencilShadow::GetLightPos( const CVector4 &in_cLightPos )
{
	GLfloat fMinv[ 16 ];
	CVector4 cLightPosResult;

	glGetFloatv( GL_MODELVIEW_MATRIX, fMinv );	// pobierz macierz modelowania do fMinv
    cLightPosResult.x = in_cLightPos.z;         // sk³adowa X pozycji œwiat³a
    cLightPosResult.y = in_cLightPos.y;         // sk³adowa Y pozycji œwiat³a
    cLightPosResult.z = in_cLightPos.z;         // sk³adowa Z pozycji œwiat³a
	cLightPosResult.w = in_cLightPos.w;         // kierunek œwiat³a
    
	VMatMult( fMinv, cLightPosResult );

	return cLightPosResult;
}

GLvoid CStencilShadow::VMatMult( GLfloat M[ 16 ], CVector4 &in_out_cLightPos )
{
	CVector4 cTemp;
	cTemp.x = M[ 0 ] * in_out_cLightPos.x + M[ 4 ] * in_out_cLightPos.y + M[ 8 ] * in_out_cLightPos.z + M[ 12 ] * in_out_cLightPos.w;
	cTemp.y = M[ 1 ] * in_out_cLightPos.x + M[ 5 ] * in_out_cLightPos.y + M[ 9 ] * in_out_cLightPos.z + M[ 13 ] * in_out_cLightPos.w;
	cTemp.z = M[ 2 ] * in_out_cLightPos.x + M[ 6 ] * in_out_cLightPos.y + M[ 10 ] * in_out_cLightPos.z + M[ 14 ] * in_out_cLightPos.w;
	cTemp.w = M[ 3 ] * in_out_cLightPos.x + M[ 7 ] * in_out_cLightPos.y + M[ 11 ] * in_out_cLightPos.z + M[ 15 ] * in_out_cLightPos.w;

	in_out_cLightPos = cTemp;
}