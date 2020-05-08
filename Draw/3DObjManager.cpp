
#include "StdAfx.h"
#include "3DObjFile.h"
#include "../OpenGLFramework/Console.h"
#include "../OpenGLFramework/GameControl.h"
#include "VertexArrays.h"
#include "TextureLoader.h"
#include "EmbossBump.h"
#include "../Lighting/Lighting.h"
#include "../Extensions/ARBMultiTexturing.h"

#ifdef _3DSLOADER_
#include "3DSLoader.h"
#endif

#include "3DObjManager.h"

C3DObjManager::C3DObjManager()
{
	m_pGameCtrl = CGameControl::GetInstance();
	m_p3DObjFile = new C3DObjFile();

	m_iMaxLightsForLists = -1;
}

GLvoid C3DObjManager::Initialization()
{
	CVertexArrays::GetInstance()->InitialArraysElem();
	//wrzucamy rysowanie obiektow do listy, kazdy obiekt bez bump i z bump
	CreateLists();
	CreateListsBump();
}

C3DObjManager::~C3DObjManager()
{
	DeleteLists();
	m_a3DObject.clear();
	delete m_p3DObjFile;
}

GLvoid C3DObjManager::CreateLists() //wszystkie bez bumpa
{
	m_ui3DObjLists = glGenLists( Get3DObjectSize() );
	for( GLint i = 0; i < Get3DObjectSize(); ++i ) {
		if( m_a3DObject[ i ].bIsElem ) {
			glNewList( m_ui3DObjLists + i, GL_COMPILE );
				Draw3DObjectElem( i, GL_FALSE );
			glEndList();
		}
		else {
			glNewList( m_ui3DObjLists + i, GL_COMPILE );
				Draw3DObjectNoElem( i, GL_FALSE );
			glEndList();
		}
	}
}

GLvoid C3DObjManager::CreateListsBump( GLint iMaxLights /*= 8*/ ) //wszytskie z bump dla kazdego swiatla GL_LIGHTx
{
	if( iMaxLights < 1 || iMaxLights > 8 )
		iMaxLights = 8;

	if( m_iMaxLightsForLists != -1 )
		return; //ta funkcja juz byla wywolana wczesniej i nie moze byc po raz drugi

	m_iMaxLightsForLists = iMaxLights;
	for( GLint iLight = 0; iLight < iMaxLights; ++iLight ) {
		m_ui3DObjListsBump[ iLight ] = glGenLists( Get3DObjectSize() );
		for( GLint i = 0; i < Get3DObjectSize(); ++i ) {
			if( m_a3DObject[ i ].bIsElem ) {
				glNewList( m_ui3DObjListsBump[ iLight ] + i, GL_COMPILE );
					Draw3DObjectElem( i, GL_TRUE, GL_LIGHT0 + iLight );
				glEndList();
			}
			else {
				glNewList( m_ui3DObjListsBump[ iLight ] + i, GL_COMPILE );
					Draw3DObjectNoElem( i, GL_TRUE, GL_LIGHT0 + iLight );
				glEndList();
			}
		}
	}
}

GLvoid C3DObjManager::DeleteLists()
{
	glDeleteLists( m_ui3DObjLists, Get3DObjectSize() );
	for( GLint i = 0; i < m_iMaxLightsForLists; ++i )
		glDeleteLists( m_ui3DObjListsBump[ i ], Get3DObjectSize() );
}

GLint C3DObjManager::Create3DObject( LPCTSTR lpFileName, GLint iTextureFirst, GLint iTextureCount, GLint iTextureEmbossBump )
{
	if( iTextureEmbossBump > -1 && iTextureCount != -1 ) {
		CONSOLE_ADDTEXT( CConsole::EError, _T("ERROR: Load 3DObj file: \"%s\" -> failed parameters"), lpFileName );
		return -1;
	}

	// pobierz ilosc wierzcholkow przed wczytaniem obiektu (gdy obj. bez indeksu, ale teraz tego nie wiemy)
	GLint iActualIndex = CVertexArrays::GetInstance()->GetVerticesSize();

	//pobierz aktualna liczbe wierzcholkow ( przed wczytaniem obiektu )
	S3DObject s3DObj;
	if( !m_p3DObjFile->LoadObject( lpFileName, s3DObj.bIsElem ) ) {
		CONSOLE_ADDTEXT( CConsole::EError, _T("ERROR - C3DObjManager::Create3DObject - not found file: %s"), lpFileName );
		return -1;
	}

	if( s3DObj.bIsElem ) {
		s3DObj.iIndexStart = 0;
		s3DObj.iIndexEnd = 0;
	}
	else {
		s3DObj.iIndexStart = iActualIndex;
		//pobierz ilosc wierzcholkow dla tej bryly
		s3DObj.iIndexEnd = m_p3DObjFile->GetVerticesNum();
	}
	s3DObj.iTextureFirst = iTextureFirst;
	s3DObj.iTextureCount = iTextureCount;
	s3DObj.iTextureEmbossBump = iTextureEmbossBump;

	m_a3DObject.push_back( s3DObj );
	GLint iIndex = Get3DObjectSize() - 1;

	if( s3DObj.bIsElem ) {
		if( s3DObj.iTextureEmbossBump > -1 )
			m_pGameCtrl->GetEmbossBump()->CalcTangentsElem( iIndex,
															CVertexArrays::GetInstance()->GetSubIndicesSize( iIndex ) );
		m_pGameCtrl->GetStencilShadow()->CreateFaceEquationElem( iIndex,
																 CVertexArrays::GetInstance()->GetSubIndicesSize( iIndex ) );
	}
	else {
		if( s3DObj.iTextureEmbossBump > -1 )
			m_pGameCtrl->GetEmbossBump()->CalcTangents( iIndex, s3DObj.iIndexStart, s3DObj.iIndexEnd );
		m_pGameCtrl->GetStencilShadow()->CreateFaceEquation( iIndex, s3DObj.iIndexStart, s3DObj.iIndexEnd );
	}

	CONSOLE_ADDTEXT( CConsole::ESuccess, _T("Load 3DObj file: \"%s\" -> OK"), lpFileName );
	return iIndex;
}

GLvoid C3DObjManager::Draw3DObject_Lists( GLint iIndex, GLboolean bUseEmbossBump /*= GL_FALSE*/, GLuint uiLight /*= GL_LIGHT0*/ )
{
	if( bUseEmbossBump && ( uiLight >= GL_LIGHT0 && uiLight <= GL_LIGHT7 ) ) {
		GLint iIndexLight = m_pGameCtrl->GetLighting()->GetIndex( uiLight );
		if( m_iMaxLightsForLists > iIndexLight ) {
			glCallList( m_ui3DObjListsBump[ iIndexLight ] + iIndex );
			return;
		}
	}

	glCallList( m_ui3DObjLists + iIndex );
}

GLvoid C3DObjManager::Draw3DObject( GLint iIndex, GLboolean bUseEmbossBump /*= GL_FALSE*/, GLuint uiLight /*= GL_LIGHT0*/ )
{
	if( iIndex < 0 || iIndex >= Get3DObjectSize() )
		return;

	if( m_a3DObject[ iIndex ].bIsElem )
		Draw3DObjectElem( iIndex, bUseEmbossBump, uiLight );
	else
		Draw3DObjectNoElem( iIndex, bUseEmbossBump, uiLight );
}

GLvoid C3DObjManager::Draw3DObjectNoElem( GLint iIndex, GLboolean bUseEmbossBump /*= GL_FALSE*/, GLuint uiLight /*= GL_LIGHT0*/ )
{
	if( iIndex < 0 || iIndex >= Get3DObjectSize() )
		return;

	GLint iFirst = m_a3DObject[ iIndex ].iTextureFirst;
	GLint iCount = m_a3DObject[ iIndex ].iTextureCount;
	GLint iEmbossBump = m_a3DObject[ iIndex ].iTextureEmbossBump;

	if( iEmbossBump > -1 && iCount == -1 && bUseEmbossBump && iFirst > -1 ) { //bump mapping
		CVertexArrays::GetInstance()->DisableClientState();

		m_pGameCtrl->GetEmbossBump()->VMatMult( m_pGameCtrl->GetLighting()->GetPosition( uiLight ) );
		m_pGameCtrl->GetEmbossBump()->SetUpBumps( iIndex,
												  m_a3DObject[ iIndex ].iIndexStart,
												  m_a3DObject[ iIndex ].iIndexEnd );
		CVertexArrays::GetInstance()->InitialArraysWithEmbossBump();
		m_pGameCtrl->GetARBMultiTexturing()->BindMultiTextures( iEmbossBump, 2, GL_TRUE );
		glDisable( GL_BLEND );

		CVertexArrays::GetInstance()->LockArrays( m_a3DObject[ iIndex ].iIndexStart,
												  m_a3DObject[ iIndex ].iIndexEnd );
		glDrawArrays( GL_TRIANGLES, m_a3DObject[ iIndex ].iIndexStart, m_a3DObject[ iIndex ].iIndexEnd );
		m_pGameCtrl->GetARBMultiTexturing()->DisableMultiTeksturing(); //uaktywniamy tylko pierwsza jednostke teksturujaca

		glBlendFunc( GL_DST_COLOR, GL_SRC_COLOR );
		glEnable( GL_BLEND );
		CVertexArrays::GetInstance()->DisableClientState();
		CVertexArrays::GetInstance()->InitialArrays( GL_TRUE );
		glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
		m_pGameCtrl->GetTexLoader()->SetTexture( iFirst );
		glDrawArrays( GL_TRIANGLES, m_a3DObject[ iIndex ].iIndexStart, m_a3DObject[ iIndex ].iIndexEnd );
		CVertexArrays::GetInstance()->UnlockArrays();
		CVertexArrays::GetInstance()->DisableClientState();
		glDisable( GL_BLEND );

		return;
	}

	CVertexArrays::GetInstance()->EnableArrays(); //nie jest wymagane jezeli na stale nie jest uzywany EmbossBumpMapping

	if( iFirst > -1 ) {
		if( iCount < 2 )	//bez multiteksturingu
			m_pGameCtrl->GetTexLoader()->SetTexture( iFirst );
		else { //multiteksturing
			CVertexArrays::GetInstance()->EnableTextureCoordsMultiTex( iCount ); //2 = ilosc jednostek teksturuj¹cych, czyli ilosc tekstur
			m_pGameCtrl->GetARBMultiTexturing()->BindMultiTextures( iFirst, iCount );
		}
	}
	else if( iFirst == -1 ) //bez tekstur
		glDisable( GL_TEXTURE_2D );

	glDrawArrays( GL_TRIANGLES, m_a3DObject[ iIndex ].iIndexStart, m_a3DObject[ iIndex ].iIndexEnd ); 

	if( iFirst > -1 && iCount > 1 )
		m_pGameCtrl->GetARBMultiTexturing()->DisableMultiTeksturing(); //uaktywniamy tylko pierwsza jednostke teksturujaca

	CVertexArrays::GetInstance()->DisableClientState();//nie jest wymagane jezeli na stale nie jest uzywany EmbossBumpMapping
}

GLvoid C3DObjManager::Draw3DObjectElem( GLint iIndex, GLboolean bUseEmbossBump /*= GL_FALSE*/, GLuint uiLight /*= GL_LIGHT0*/ )
{
	if( iIndex < 0 || iIndex >= Get3DObjectSize() )
		return;

	GLint iFirst = m_a3DObject[ iIndex ].iTextureFirst;
	GLint iCount = m_a3DObject[ iIndex ].iTextureCount;
	GLint iEmbossBump = m_a3DObject[ iIndex ].iTextureEmbossBump;

	if( iEmbossBump > -1 && iCount == -1 && bUseEmbossBump && iFirst > -1 ) { //bump mapping
		CVertexArrays::GetInstance()->DisableClientState();

		m_pGameCtrl->GetEmbossBump()->VMatMult( m_pGameCtrl->GetLighting()->GetPosition( uiLight ) );
		m_pGameCtrl->GetEmbossBump()->SetUpBumpsElem( iIndex, CVertexArrays::GetInstance()->GetSubIndicesSize( iIndex ) );
		CVertexArrays::GetInstance()->InitialArraysWithEmbossBumpElem();
		m_pGameCtrl->GetARBMultiTexturing()->BindMultiTextures( iEmbossBump, 2, GL_TRUE );
		glDisable( GL_BLEND );
		
		CVertexArrays::GetInstance()->LockArrays( 0, CVertexArrays::GetInstance()->GetVerticesElemSize() );
		glDrawElements( GL_TRIANGLES, CVertexArrays::GetInstance()->GetSubIndicesSize( iIndex ), GL_UNSIGNED_SHORT,
						CVertexArrays::GetInstance()->GetSubIndices( iIndex ) );
		
		m_pGameCtrl->GetARBMultiTexturing()->DisableMultiTeksturing(); //uaktywniamy tylko pierwsza jednostke teksturujaca

		glBlendFunc( GL_DST_COLOR, GL_SRC_COLOR );
		glEnable( GL_BLEND );
		CVertexArrays::GetInstance()->DisableClientState();
		CVertexArrays::GetInstance()->InitialArraysElem();
		glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
		m_pGameCtrl->GetTexLoader()->SetTexture( iFirst );

		glDrawElements( GL_TRIANGLES, CVertexArrays::GetInstance()->GetSubIndicesSize( iIndex ), GL_UNSIGNED_SHORT,
						CVertexArrays::GetInstance()->GetSubIndices( iIndex ) );
		CVertexArrays::GetInstance()->UnlockArrays();

		CVertexArrays::GetInstance()->DisableClientState();
		glDisable( GL_BLEND );
		return;
	}

	CVertexArrays::GetInstance()->EnableArraysElem(); //nie jest wymagane jezeli na stale nie jest uzywany EmbossBumpMapping

	if( iFirst > -1 ) {
		if( iCount < 2 )	//bez multiteksturingu
			m_pGameCtrl->GetTexLoader()->SetTexture( iFirst );
		else { //multiteksturing
			CVertexArrays::GetInstance()->EnableTextureCoordsMultiTexElem( iCount ); //iCount = ilosc jednostek teksturuj¹cych, czyli ilosc tekstur
			m_pGameCtrl->GetARBMultiTexturing()->BindMultiTextures( iFirst, iCount );
		}
	}
	else if( iFirst == -1 ) //bez tekstur
		glDisable( GL_TEXTURE_2D );

	glDrawElements( GL_TRIANGLES, CVertexArrays::GetInstance()->GetSubIndicesSize( iIndex ), GL_UNSIGNED_SHORT,
					CVertexArrays::GetInstance()->GetSubIndices( iIndex ) );

	if( iFirst > -1 && iCount > 1 ) 
		m_pGameCtrl->GetARBMultiTexturing()->DisableMultiTeksturing(); //uaktywniamy tylko pierwsza jednostke teksturujaca

	CVertexArrays::GetInstance()->DisableClientState();//nie jest wymagane jezeli na stale nie jest uzywany EmbossBumpMapping
}

#ifdef _3DSLOADER_

GLboolean C3DObjManager::SaveObject( LPCTSTR lpPath,
									 GLboolean bIndexing,
									 GLboolean bFixed,
									 const CVector3& in_cScale /*= CVector3( 1.0f, 1.0f, 1.0f )*/,
									 const CVector3& in_cVector /*= CVector3( 1.0f, 1.0f, 1.0f )*/,
									 const CVector3& in_cRot90 /*= CVector3( 1.0f, 1.0f, 1.0f )*/,
									 GLboolean bSmoothNormal /*= GL_TRUE*/ )
{
	if( bIndexing )
		return SaveObjectElem( lpPath, bFixed );

	return SaveObjectNoElem( lpPath, in_cScale, in_cVector, in_cRot90, bSmoothNormal, bFixed );
}

GLboolean C3DObjManager::SaveObjectNoElem( LPCTSTR lpPath,
										   const CVector3 &in_cScale,
										   const CVector3 &in_cVector,
										   const CVector3 &in_cRot90,
										   GLboolean bSmoothNormal,
										   GLboolean bFixed )
{
	GLboolean bResult = GL_TRUE;
	TCHAR cFileName[ 256 ];

	//zapisujemy pierwszy obiekt o indeksie 0, nazwa pliku taka sama jak pliku 3ds
	::wsprintf( cFileName, _T("%s%s"), lpPath, m_pGameCtrl->Get3DSLoader()->GetActualFileName() );
	bResult = m_p3DObjFile->SaveObjectNoElem( cFileName,
											  m_pGameCtrl->Get3DSLoader()->Get3DSObject( 0 ),
											  in_cScale, in_cVector, in_cRot90, bSmoothNormal, bFixed );
	if( !bResult )
		return bResult;

	//zapisuje kazdy nastepny obiekt w osobnym pliku, dopisujac na konic kolejny numer
	for( GLint i = 1; i < m_pGameCtrl->Get3DSLoader()->Get3DSObjectsSize(); ++i ) {
		::wsprintf( cFileName, _T("%s%s_%02d.3ds"), lpPath, m_pGameCtrl->Get3DSLoader()->GetActualFileNameWithoutExt(), i );
		bResult = m_p3DObjFile->SaveObjectNoElem( cFileName,
												  m_pGameCtrl->Get3DSLoader()->Get3DSObject( i ),
												  in_cScale, in_cVector, in_cRot90, bSmoothNormal, bFixed );
		if( !bResult )
			return bResult;
	}

	return GL_TRUE;
}

GLboolean C3DObjManager::SaveObjectElem( LPCTSTR lpPath, GLboolean bFixed )
{
	GLboolean bResult = GL_TRUE;
	TCHAR cFileName[ 256 ];

	//zapisujemy pierwszy obiekt o indeksie 0, nazwa pliku taka sama jak pliku 3ds
	::wsprintf( cFileName, _T("%s%s"), lpPath, m_pGameCtrl->Get3DSLoader()->GetActualFileName() );
	bResult = m_p3DObjFile->SaveObjectElem( 0, cFileName, bFixed );
	if( !bResult )
		return bResult;

	//zapisuje kazdy nastepny obiekt w osobnym pliku, dopisujac na konic kolejny numer
	for( GLint i = 1; i < m_pGameCtrl->Get3DSLoader()->Get3DSObjectsSize(); ++i ) {
		::wsprintf( cFileName, _T("%s%s_%02d.3ds"), lpPath, m_pGameCtrl->Get3DSLoader()->GetActualFileNameWithoutExt(), i );
		bResult = m_p3DObjFile->SaveObjectElem( i, cFileName, bFixed );
		if( !bResult )
			return bResult;
	}

	return GL_TRUE;
}

#endif // _3DSLOADER_

GLint C3DObjManager::GetTextureFirstIndex( GLint iIndex )
{
	if( iIndex < 0 || iIndex >= Get3DObjectSize() )
		return -1;

	return m_a3DObject[ iIndex ].iTextureFirst;
}