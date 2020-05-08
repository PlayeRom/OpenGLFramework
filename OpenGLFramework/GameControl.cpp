// GameControl.cpp: implementation of the CGameControl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "Console.h"
#include "../Fonts/BitmapFont.h"
#include "../Fonts/OutlineFont.h"
#include "../Fonts/TextureFont.h"
#include "../Fonts/SDLFont.h"
#include "../Fps/Fps.h"
#include "../Sound/MasterVolume.h"
#include "WindowData.h"
#include "Cursor.h"
#include "../Extensions/ARBMultiTexturing.h"
#include "../Extensions/WGLEXTSwapControl.h"
#include "../Extensions/ARBVertexBufferObject.h"
#include "../Draw/VertexArrays.h"
#include "../Draw/3DObjManager.h"
#include "../Draw/EmbossBump.h"
#include "../Network/NetworkManager.h"
#include "GameControl.h"


#if _USE_SOUND_OPENAL_
	#pragma comment( lib, "alut.lib" )
	#pragma comment( lib, "OpenAL32.lib" )
	#if _USE_OGG_
		#pragma comment( lib, "ogg.lib" )
		#pragma comment( lib, "vorbisfile.lib" )
	#endif // _USE_OGG_
#else // use FMOD
	#pragma comment( lib, "fmodvc.lib" )
#endif // !_USE_SOUND_OPENAL_

GLvoid MsgExit( GLvoid );

CGameControl* CGameControl::Construct()
{
	return CSingletonBase::Construct( new CGameControl );
}

CGameControl::CGameControl()
{
	m_pWin = CWindowData::GetInstance();

	::SecureZeroMemory( m_bKeys, 256 );
	
	//przy restarcie ponizszych zmieninnych nie mozna zmieniac
	m_bIs2D = GL_FALSE;
	m_pQuadric = NULL;
	
	m_aIndicesQuadAllScreen = new GLint[ 8 ];

	m_fRotY = 0.0f;
	m_bUseBump = GL_TRUE;
}

CGameControl::~CGameControl()
{
	DeleteObjects();
}

GLboolean CGameControl::CreateObjects()
{
	SetIndicesQuadAllScreen();

	m_pMatrixOp = new CMatrixOperations();
	if( !m_pMatrixOp ) return GL_FALSE;

	m_pConsole = new CConsole();
	if( !m_pConsole ) return GL_FALSE;

	m_pFps = new CFps();
	if( !m_pFps ) return GL_FALSE;

	m_pSpeedControl = new CSpeedControl();
	if( !m_pSpeedControl ) return GL_FALSE;

	m_pBitmapFont = new CBitmapFont( GetWinData()->GetHDC(), _T("Arial") );
	if( !m_pBitmapFont ) return GL_FALSE;
	
	m_pOutlineFont = new COutlineFont( GetWinData()->GetHDC(), _T("Arial") );
	if( !m_pOutlineFont ) return GL_FALSE;

	m_pSDLFont = new CSDLFont();
	if( !m_pSDLFont ) return GL_FALSE;

	m_pARBMultiTexturing = CARBMultiTexturing::Construct();
	if( !m_pARBMultiTexturing ) return GL_FALSE;

	m_pWglExtSwapCtrl = CWGLEXTSwapControl::Construct();
	if( !m_pWglExtSwapCtrl ) return GL_FALSE;

	m_pTextureLoader = new CTextureLoader();
	if( !m_pTextureLoader ) return GL_FALSE;

	m_pTextureFont = new CTextureFont();
	if( !m_pTextureFont ) return GL_FALSE;

#if _USE_SOUND_OPENAL_
	m_pOpenALManager = COpenALManager::Construct();
	if( !m_pOpenALManager ) return GL_FALSE;
#else
	m_pSoundFMOD = new CSoundFMOD();
	if( !m_pSoundFMOD ) return GL_FALSE;
#endif

	m_pMasterVolume = CMasterVolume::Construct();
	if( !m_pMasterVolume ) return GL_FALSE;

	m_pLighting = new CLighting();
	if( !m_pLighting ) return GL_FALSE;

	m_pCursor = new CCursor();
	if( !m_pCursor ) return GL_FALSE;

	m_pEmbossBump = new CEmbossBump();
	if( !m_pEmbossBump ) return GL_FALSE;

	m_pVertexArrays = CVertexArrays::Construct();
	if( !m_pVertexArrays ) return GL_FALSE;

	m_pStencilShadow = new CStencilShadow();
	if( !m_pStencilShadow ) return GL_FALSE;

	m_p3DObjManager = new C3DObjManager();
	if( !m_p3DObjManager ) return GL_FALSE;

	m_pParticles = new CParticles();
	if( !m_pParticles ) return GL_FALSE;

	m_pBillboard = new CBillboard();
	if( !m_pBillboard ) return GL_FALSE;

	m_pNetworkManager = new CNetworkManager();
	if( !m_pNetworkManager ) return GL_FALSE;
	
	m_pRMessageBox = new CRMessageBox();
	if( !m_pRMessageBox ) return GL_FALSE;

	return GL_TRUE;
}

GLvoid CGameControl::DeleteObjects()
{
	if( m_pQuadric ) {
		gluDeleteQuadric( m_pQuadric );
		m_pQuadric = NULL;
	}

	delete [] m_aIndicesQuadAllScreen;
	delete m_pSpeedControl;
	delete m_pTextureLoader;
	delete m_pBitmapFont;
	delete m_pOutlineFont;
	delete m_pConsole;
	delete m_pFps;
#ifndef _USE_SOUND_OPENAL_
	delete m_pSoundFMOD;
#endif
	delete m_pMasterVolume;
	delete m_pSDLFont;
	delete m_pLighting;
	delete m_pTextureFont;
	delete m_pCursor;
	delete m_pVertexArrays;
	delete m_pMatrixOp;
	delete m_p3DObjManager;
	delete m_pEmbossBump;
	delete m_pParticles;
	delete m_pBillboard;
	delete m_pStencilShadow;
	delete m_pNetworkManager;
	delete m_pRMessageBox;
}

GLboolean CGameControl::Initialization()
{
	if( !CreateObjects() ) {
		__LOG( _T("ERROR: CGameControl::CreateObjects - create objects failed") );
		return GL_FALSE;
	}

	SetLights();

	CreateAllAnimCtrl();
	
	if( !LoadAllTextures() ) {
		__LOG( _T("ERROR: CGameControl::Initialization() - LoadAllTextures() failed") );
		return GL_FALSE;
	}

	if( !CreateAllParticles() ) {
		__LOG( _T("ERROR: CGameControl::Initialization() - CreateAllParticles() failed") );
		return GL_FALSE;
	}

	if( !LoadAll3DObjFiles() ) {
		__LOG( _T("ERROR: CGameControl::Initialization() - LoadAll3DObjFiles() failed") );
		return GL_FALSE;
	}

	if( !LoadAllSounds() ) {
		__LOG( _T("ERROR: CGameControl::Initialization() - LoadAllSounds() failed") );
		return GL_FALSE;
	}

	if( !CMultiLanguage::GetInstance()->LoadLanguage( GetWinData()->SettingFile().iIndexLanguage ) ) {
		__LOG( _T("ERROR: CGameControl::Initialization() - LoadLanguage() failed") );
		return GL_FALSE;
	}
	
	SetVSync();
	CreateVertices();

	CreateQuadric();

	return GL_TRUE;
}

GLvoid CGameControl::CreateVertices()
{
	//Przyklad wykorzystania VertexArray

	//sami podajemy normalna
//	GetVertexArrays()->AddFullVertex( -1.0f,-1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f );
//	GetVertexArrays()->AddFullVertex(  1.0f,-1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f );
//	GetVertexArrays()->AddFullVertex(  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f );
//	GetVertexArrays()->AddFullVertex( -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f );
//	GetVertexArrays()->InitialArrays();

	//normalna zostanie wyliczona
	GetVertexArrays()->AddFullVertexCalcNormal( -1.0f,-1.0f, 0.0f, 0.0f, 0.0f );
	GetVertexArrays()->AddFullVertexCalcNormal(  1.0f,-1.0f, 0.0f, 1.0f, 0.0f );
	GetVertexArrays()->AddFullVertexCalcNormal(  1.0f, 1.0f, 0.0f, 1.0f, 1.0f );
	GetVertexArrays()->AddFullVertexCalcNormal( -1.0f, 1.0f, 0.0f, 0.0f, 1.0f );
	GetVertexArrays()->CalcAndAddNormalsFlat( GL_QUADS, GetVertexArrays()->GetVerticesSize() - 4, 4 ); //wyliczamy normalna

/*	GetVertexArrays()->AddFullVertexCalcNormal( -1.0f,-1.0f, 0.0f, 0.0f, 0.0f );
	GetVertexArrays()->AddFullVertexCalcNormal(  1.0f,-1.0f, 0.0f, 1.0f, 0.0f );
	GetVertexArrays()->AddFullVertexCalcNormal(  1.0f, 1.0f, 0.0f, 1.0f, 1.0f );

	GetVertexArrays()->AddFullVertexCalcNormal( -1.0f,-1.0f, 0.0f, 0.0f, 0.0f );
	GetVertexArrays()->AddFullVertexCalcNormal(  1.0f, 1.0f, 0.0f, 1.0f, 1.0f );
	GetVertexArrays()->AddFullVertexCalcNormal( -1.0f, 1.0f, 0.0f, 0.0f, 1.0f );
	*/
/*
//******************** dla bumpmappingu ***********************
	GetVertexArrays()->AddFullVertex( -1.0f,-1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f );
	GetVertexArrays()->AddFullVertex(  1.0f,-1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f );
	GetVertexArrays()->AddFullVertex(  1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f );

	GetVertexArrays()->AddFullVertex( -1.0f,-1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f );
	GetVertexArrays()->AddFullVertex(  1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f );
	GetVertexArrays()->AddFullVertex( -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f );
//	GetVertexArrays()->CalcAndAddNormalsFlat( GL_TRIANGLES, 0, 6 ); //wyliczamy normalna
	GetEmbossBump()->CalcTangents( 0, 6 );
//***************************************************************
*/
	GetVertexArrays()->InitialArrays();
}

GLvoid CGameControl::SetVSync()
{
	if( GetWinData()->SettingFile().bVSync )
		GetWGLEXTSwapControl()->EnableVSync();
	else
		GetWGLEXTSwapControl()->DisableVSync();
}

GLvoid CGameControl::CreateQuadric()
{
	if( m_pQuadric ) {
		gluDeleteQuadric( m_pQuadric );
		m_pQuadric = NULL;
	}
	//*************** dla kwadrykow *********************
    m_pQuadric = gluNewQuadric(); 
	//
	//gluQuadricNormals( m_pQuadric, GLU_NONE ); //bez normalnych
	//gluQuadricNormals( m_pQuadric, GLU_FLAT ); //naormalne na p³aszczyznach
    gluQuadricNormals( m_pQuadric, GLU_SMOOTH ); //normalne na wierzcholkach
	//
	//gluQuadricDrawStyle( m_pQuadric, GLU_LINE );
	//gluQuadricDrawStyle( m_pQuadric, GLU_POINT );
	//gluQuadricDrawStyle( m_pQuadric, GLU_SILHOUETTE );
	gluQuadricDrawStyle( m_pQuadric, GLU_FILL ); //domyslnie
	//
	//gluQuadricOrientation( m_pQuadric, GLU_INSIDE ); //normalne do wewnatrz kwadryki, np. gdy kamera wernatrz kwadryki
	gluQuadricOrientation( m_pQuadric, GLU_OUTSIDE ); //domyslnie - normalna na zewnatrz kwadryki
    //
	gluQuadricTexture( m_pQuadric, GL_TRUE );
	//***************************************************
}

GLvoid CGameControl::SetLights()
{
	//******************** oswietlenie **************************
	GetLighting()->SetAmbient( GL_LIGHT0 );						//ustaw oswietlenie otoczenia
	GetLighting()->SetDiffuse( GL_LIGHT0 );						//ustaw oswietlenie rozpraszajace
	GetLighting()->SetSpecular( GL_LIGHT0 );					//Ustaw swiatlo odb³ysków
	GetLighting()->SetPosition( GL_LIGHT0, 1.0f, 0.0f, -3.0f );	//ustaw pozycje swiatla
	GetLighting()->SetMatSpecular();
	GetLighting()->SetMatShinness();
	glEnable( GL_LIGHT0 );
	glEnable( GL_LIGHTING );
	//***********************************************************
}

GLvoid CGameControl::CreateAllAnimCtrl()
{
	//TODO: Tutaj wywo³uj metody GetSpeedCtrl()->CreateAnimationControl();
}

GLboolean CGameControl::LoadAllTextures()
{
	if( !GetTexLoader() )
		return GL_FALSE;

	//Tutaj nalezy wpisywac kolejne tekstury jake chcemy wykorzystac, wedle ponizszego wzoru
	//TODO: tutaj wczytuj kolejne tekstury

	GetTexLoader()->LoadTexMipmaps( _T("textures/test.bmp") );			//index 0
	GetTexLoader()->LoadTexLowQuality( _T("textures/console.bmp") );	//index 1
	GetTexLoader()->LoadTexMipmaps( _T("textures/test.bmp") );			//index 2

	//"multitexturing" z maska - tworzy jeden index
	GetTexLoader()->LoadMultiTexMaskMipmaps( _T("textures/star_mask.bmp"), _T("textures/star_rgb.bmp") ); //index 3

	GetTexLoader()->LoadTexMipmaps( _T("textures/multitex1.bmp") );	//index 4
	GetTexLoader()->LoadTexMipmaps( _T("textures/multitex2.bmp") );	//index 5

	//
	GetTexLoader()->LoadTexMipmaps( _T("textures/player03.bmp") );	//index 6
	GetTexLoader()->LoadEmbossBump( _T("textures/player03bump.bmp") );		//index 7, 8

	GetTexLoader()->LoadTexMipmaps( _T("textures/particle.bmp") );		//index 9
	
	return GL_TRUE;
}

GLboolean CGameControl::CreateAllParticles()
{
	if( !GetParticles() )
		return GL_FALSE;
		
	GetParticles()->CreateEmitter( CVector3( 0.0f, 0.0f, -20.0f ),
								   9,
								   CVector3( 0.0f, 0.0f, 0.0f ),
								   CVector3( 0.0f, 0.0f, 0.0f ),
								   1.0f, //scale
								   1.0f, //life
								   GL_TRUE,
								   1.0f, //r
								   1.0f, //g
								   0.2f, //b
								   0.0f,
								   0.0f,
								   0.0f,
								   100 );
	return GL_TRUE;
}

GLboolean CGameControl::LoadAll3DObjFiles()
{
	if( !Get3DObjManager() )
		return GL_FALSE;

	//TODO: tutaj wczytuj pliki .3DObj
	Get3DObjManager()->Create3DObject( _T("objects/player03.3DObj"), 6, -1, 7 ); //index 0

	// Musi byæ wywo³ana na samym koñcu
	Get3DObjManager()->Initialization();
	return GL_TRUE;
}

GLboolean CGameControl::LoadAllSounds()
{
#if _USE_SOUND_OPENAL_

	if( !GetOpenALManager() )
		return GL_FALSE;

	if( !GetOpenALManager()->IsInitOK() )
		return GL_FALSE;

	//TODO: Tutaj wczytuj wszelkie dŸwiêki dla biblioteki OpenAL
	GetOpenALManager()->LoadSound( "sounds/test.wav" );					// index 0

#ifdef _USE_OGG_
	GetOpenALManager()->LoadSound( "sounds/test.ogg", AL_TRUE, AL_TRUE );	// index 1
	GetOpenALManager()->DoUpdateAllStream();
#endif

	return AL_TRUE;

#else // FMOD

	if(!GetSoundFMOD())
		return GL_FALSE;

	//TODO: Tutaj wczytuj wszelkie dŸwiêki dla biblioteki FMOD

	GetSoundFMOD()->LoadSample( "sounds/test.wav" );

	return AL_TRUE;
#endif
}

GLvoid CGameControl::Exit()
{
	GetRMessageBox()->DoMessageBox( LNG_LINE( 1 ), CRMessageBox::EMsgYesNo, MsgExit, NULL );
}

GLvoid MsgExit( GLvoid ) //odpowiedz na klikniecie na tak w msgboxie przy wychodzeniu z gry
{
	::PostQuitMessage( 0 );
}

GLvoid CGameControl::RestartObjects()
{
	//przy restartowaniu nalezy dla kazdego obiektu klasy CSDLFont wywolac funkcje ClearStorageBuffer
	for( GLint i = static_cast< GLint >( g_aPointerSDLFont.size() - 1 ); i >= 0; --i ) {
		if( g_aPointerSDLFont[ i ] )
			g_aPointerSDLFont[ i ]->ClearStorageBuffer();
	}

	delete m_p3DObjManager;
	delete m_pTextureLoader;
	delete m_pTextureFont;
	delete m_pVertexArrays;
	delete m_pBitmapFont;
	delete m_pOutlineFont;

	m_pTextureLoader = new CTextureLoader();
	m_pTextureFont = new CTextureFont();
	GetEmbossBump()->ClearAllArrays();
	m_pVertexArrays = CVertexArrays::Construct();
	GetStencilShadow()->RestartObjects();
	m_p3DObjManager = new C3DObjManager();
	
	SetLights();

	LoadAllTextures();
	LoadAll3DObjFiles();
	
	SetVSync();
	CreateVertices();

	m_pBitmapFont = new CBitmapFont( GetWinData()->GetHDC(), _T("Arial") );
	m_pOutlineFont = new COutlineFont( GetWinData()->GetHDC(), _T("Arial") );
	
	CreateQuadric();

	GetConsole()->RestartObjects();
	GetRMessageBox()->RestartObjects();
}

GLboolean CGameControl::KeyDown( GLuint uiKey )
{
	if( GetConsole()->KeyDown( uiKey ) )
		return GL_TRUE;
	
	if( GetRMessageBox()->KeyDown( uiKey ) )
		return GL_TRUE;

	//aby klawisz przeszedl do funkcji CGameControl::Keyboard()
	//gdy nie zostal skonsumowany:
	m_bKeys[ uiKey ] = GL_TRUE;

	return GL_TRUE;
}

GLboolean CGameControl::KeyUp( GLuint uiKey )
{
	if( GetConsole()->KeyUp( uiKey ) )
		return GL_TRUE;
	
	//tutaj mo¿emy przekazac klawisz dalej do naszej klasy
	//lub obs³u¿yc go w CGameControl:
	m_bKeys[ uiKey ] = GL_FALSE;
	
	return GL_TRUE;
}

GLboolean CGameControl::MouseLButtonDown( GLint iX, GLint iY )
{
	if( GetRMessageBox()->MouseLButtonDown( iX, iY ) )
		return GL_TRUE;
	
	/*
	tutaj przetwarzamy klikniecie myszy lub
	przekazujemy je dalej do innej klasy
	np:

	switch( GameMode ) {
		case EMainMenu:
			obiektKalsy1->MouseLButtonDown( iX, iY );
			break;
		case ESingleGame:
			obiektKalsy2->MouseLButtonDown( iX, iY );
			break;
		case EOptions:
			obiektKalsy3->MouseLButtonDown( iX, iY );
			break;
	}
	*/
	return GL_FALSE;
}

GLboolean CGameControl::MouseLButtonUp( GLint /*iX*/, GLint /*iY*/ )
{
	return GL_FALSE;
}

GLboolean CGameControl::MouseRButtonDown( GLint /*iX*/, GLint /*iY*/ )
{
	//tutaj przetwarzamy klikniecie prawego przycisku
	//myszy lub przekazujemy je dalej do innej klasy

	return GL_FALSE;
}

GLboolean CGameControl::MouseRButtonUp( GLint /*iX*/, GLint /*iY*/ )
{
	return GL_FALSE;
}

GLboolean CGameControl::MouseWheel( GLint iScrollLines )
{
	if( GetConsole()->MouseWheel( iScrollLines ) )
		return GL_TRUE;
	
	//tutaj implementujemy obsluge rolki myszy
	//parametr iScrollLines mowi nam o ile linii i w ktora strone 
	//wykonano obrot, wartosc ujemna - w dol, wartosc dodatnia - w gore

	return GL_FALSE;
}

GLvoid CGameControl::Enable2D()
{
	if( m_bIs2D )
		return;
	m_bIs2D = GL_TRUE;

	GLdouble vPort[ 4 ];
	glGetDoublev( GL_VIEWPORT, vPort );
  
	glMatrixMode( GL_PROJECTION );
	glPushMatrix();
	glLoadIdentity();
  
	glOrtho( 0.0, vPort[ 2 ], vPort[ 3 ], 0.0, -1.0, 1.0 );

	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();
	glLoadIdentity();
}

GLvoid CGameControl::Disable2D()
{
	if( !m_bIs2D )
		return;
	m_bIs2D = GL_FALSE;

	glMatrixMode( GL_PROJECTION );
	glPopMatrix();   
	glMatrixMode( GL_MODELVIEW );
	glPopMatrix();	
}

GLvoid CGameControl::Draw()
{
	PROFILER_BEGIN( _T("CGameControl::Draw()") );
	
	GetSpeedCtrl()->RefreshTime();
	GetFps()->IncreaseFps();

//******************** tutaj rysujemy ****************************************
	//wyszsto co jest zawarte miedzy /*****/ jest tylko przyk³adem i oczywiscie
	//nale¿y w to miejsce rysowaæ w³asne obiekty, wedle w³asnego uznania
	
	GetCursorPosition();
	
#if _USE_2D_ONLY_
	glColor3f( 1.0f, 1.0f, 1.0f );

	glDisable( GL_LIGHTING );	
	
	GetTexLoader()->SetTexture( 0 );

	glBegin( GL_QUADS );
		glTexCoord2f( 1.0f, 1.0f ); glVertex2f( 100.0f,   0.0f );
		glTexCoord2f( 0.0f, 1.0f ); glVertex2f(   0.0f,   0.0f );
		glTexCoord2f( 0.0f, 0.0f ); glVertex2f(   0.0f, 100.0f );
		glTexCoord2f( 1.0f, 0.0f ); glVertex2f( 100.0f, 100.0f );
	glEnd();
	
	glLoadIdentity();

	//rysowanie tekstu za pomoca CTextureFont:
	GetTextureFont()->DrawTextFormat( 10.0f, 200.0f, 0, 1.0f, 1.0f, 1.0f, 1.0f, _T("Texture font %.1f format!"), 0.1f );
	
	//rysowanie tesktu za pomoc¹ CSDLFont z wykorzystwaniem CMultiLanguage:
	GetSDLFont()->DrawTextFormat( 10, 35, GL_TRUE, RGB( 255, 255, 255 ), LNG_LINE( 0 ), 10 );
	
	//rysowanie tesktu za pomoc¹ CBitmapFont
	GetBitmapFont()->DrawText( 10.0f, 300.0f, _T("Bitmap font") );

#else //3D:
	GetLighting()->DrawPositionLight( GL_LIGHT0, 1.0f, 0.0f, 0.0f );

	//Przyklad rysowania "multitekstury" z mask¹:
	glLoadIdentity();
	Disable2D();

	glColor3f( 1.0f, 1.0f, 1.0f );
	glEnable( GL_LIGHTING );
	glDisable( GL_BLEND );
	glEnable( GL_DEPTH_TEST );

	m_fRotY += ( 10.0f * GetSpeedCtrl()->GetMultiplier() );
	if( m_fRotY > 360.0f )
		m_fRotY -= 360.0f;

	glLoadIdentity();
	glTranslatef( 0.0f, 0.0f, -100.0f );
	glRotatef( 30.0f, 1.0f, 0.0f, 0.0f );
	glRotatef( m_fRotY, 0.0f, 1.0f, 0.0f );

	//rysujemy statki kosmiczne
	if( m_bUseBump ) {
		//rysujemy z wykorzystaniem list oraz bump mappingu
		Get3DObjManager()->Draw3DObject_Lists( 0, GL_TRUE, GL_LIGHT0 );
		glTranslatef( 50.0f, 0.0f, -50.0f );
		Get3DObjManager()->Draw3DObject_Lists( 0, GL_TRUE, GL_LIGHT0 );
		glTranslatef( -100.0f, 0.0f, 0.0f );
		Get3DObjManager()->Draw3DObject_Lists( 0, GL_TRUE, GL_LIGHT0 );
	}
	else {
		//rysujemy bez wykorzystania list oraz bez bump mappingu
		Get3DObjManager()->Draw3DObject( 0, GL_FALSE );
		glTranslatef( 50.0f, 0.0f, -50.0f );
		Get3DObjManager()->Draw3DObject( 0, GL_FALSE );
		glTranslatef( -100.0f, 0.0f, 0.0f );
		Get3DObjManager()->Draw3DObject( 0, GL_FALSE );
		glTranslatef( 50.0f, 0.0f, -50.0f );
		Get3DObjManager()->Draw3DObject( 0, GL_FALSE );
	}

	glLoadIdentity();
	glTranslatef( 0.0f, 0.0f, -100.0f );
	glRotatef( 30.0f, 1.0f, 0.0f, 0.0f );
	glRotatef( m_fRotY, 0.0f, 1.0f, 0.0f );

	GetParticles()->DrawEmitter( 0 );

	//rysujemy gwiazde
	glLoadIdentity();
	glTranslatef( -2.0f, 1.0f, -10.0f );
	GetVertexArrays()->InitialArrays();
	GetTexLoader()->SetMultiTextures( 3 ); //gwiazda
	glDrawArrays( GL_QUADS, GetVertexArrays()->GetVerticesSize() - 4, 4 );
	GetVertexArrays()->DisableClientState();

	//Przyk³ad rysowania z multiteksturami i VerexArray:
	glLoadIdentity();
	glTranslatef( 3.0f, 1.5f, -15.0f );
//	GetVertexArrays()->EnableTextureCoordsMultiTex( 2 ); //2 = ilosc jednostek teksturuj¹cych, czyli ilosc tekstur
	GetVertexArrays()->InitialArraysWithMultiTex( 2 );
	GetARBMultiTexturing()->BindMultiTextures( 4, 2 ); //4 = indeks pierwszej tekstury, 2 = ilosc tekstur
	glDrawArrays( GL_QUADS, GetVertexArrays()->GetVerticesSize() - 4, 4 );
	GetARBMultiTexturing()->DisableMultiTeksturing(); //uaktywniamy tylko pierwsza jednostke teksturujaca
	GetVertexArrays()->DisableClientState();

	glLoadIdentity();
	//rysowanie tekstu za pomoca CTextureFont:
	GetTextureFont()->DrawTextFormat( 200.0f, 200.0f, 0, 1.0f, 1.0f, 1.0f, 1.0f, _T("Texture font %.1f format!"), 0.1f );

	//rysowanie tesktu za pomoc¹ CSDLFont z wykorzystwaniem CMultiLanguage:
	GetSDLFont()->DrawTextFormat( 0, 35, GL_TRUE, RGB( 255, 255, 255 ), LNG_LINE( 0 ), 10 );

#endif //_USE_2D_ONLY_
	
	GetRMessageBox()->Draw();
	
//*************************** koniec rysowania ************************************
	glLoadIdentity();

	GetFps()->DrawFps();

#if _USE_2D_ONLY_
	//jezeli mamy sta³y tryb 2D to mo¿emy wyœwietliæ konsolê tylko za pomoc¹ CSDLFont
	GetConsole()->DrawConsoleWithSDLFont();
#else
	#if _USE_CONSOLE_WITH_MY_FONT_
		GetConsole()->DrawConsoleMyFont();
	#elif _USE_CONSOLE_WITH_BITMAP_FONT_
		GetConsole()->DrawConsoleWithBitmapFont();
	#else
		GetConsole()->DrawConsoleWithSDLFont();
	#endif //_USE_CONSOLE_WITH_MY_FONT_ _USE_CONSOLE_WITH_BITMAP_FONT_
#endif //_USE_2D_ONLY_

	//rysowanie jasnosci, poni¿ej tej instrukcji nie mo¿na ju¿ niczego rysowaæ
	CBrightness::GetInstance()->Draw();

	PROFILER_END();
}

GLvoid CGameControl::GetCursorPosition()
{
	::GetCursorPos( &m_ptCursor );
	if( !GetWinData()->SettingFile().bFullScreen ){
		RECT rcWin, rcClient;
		::GetWindowRect( GetWinData()->GetHWND(), &rcWin );
		::GetClientRect( GetWinData()->GetHWND(), &rcClient );

		//jako, ¿e otrzymana pozycja kursora
		//jest wzgledem ekranu, to dla programu okienkowego trzeba pozycjê
		//kursora przekszta³ciæ tak aby by³a wzglêdem okna klienta:

		//iWidthFrame to szerokosc ramki okna, ramki tej po bokach i od do³u.
		//Obliczam d³ugoœæ okna (zawsze jest wiêksza od ustawionej rozdzielczoœci
		//bo dochodzi w³aœnie ramka) i odejmuje od niej d³ugoœæ okna klienat (czyli X
		//rozdzielczoœci)
		//teraz mam gruboœæ dwóch ramek z lewej i prawej strony okna, aby obliczyæ
		//gruboœæ pojedynczej ramki, dzielimy przez 2
		GLint iWidthFrame = ( ( rcWin.right - rcWin.left - rcClient.right ) / 2 );
		//pozycja kursora wzglêdem okna klienta:
		m_ptCursor.x -= ( rcWin.left + iWidthFrame );
		m_ptCursor.y -= ( rcWin.top + ( ( rcWin.bottom - rcWin.top - rcClient.bottom ) - iWidthFrame ) );
	}
}

GLvoid CGameControl::Timer()
{
	GetFps()->SetTextFps();
	GetFps()->CleanFps();

	for( GLint i = static_cast< GLint> ( g_aPointerSDLFont.size() - 1 ); i >= 0; --i ) {
		if( g_aPointerSDLFont[ i ] )
			g_aPointerSDLFont[ i ]->Timer();
	}
#if _USE_SOUND_OPENAL_
	COpenALManager::GetInstance()->Timer();
#endif
}

GLvoid CGameControl::Keyboard()
{
	if( m_bKeys[ VK_RETURN ] ) { //nacisnieto ENTER
		m_bKeys[ VK_RETURN ] = GL_FALSE;

		if( HIBYTE( ::GetKeyState( VK_MENU ) ) ) //kombinacja Alt+Enter
			::ShowWindow( GetWinData()->GetHWND(), SW_MINIMIZE ); //minimalizacja
		else{
			//TODO: Tutaj obsluga samego ENTERa
		}
	}

	if( m_bKeys[ VK_ESCAPE ] ) { //wyjœcie z programu
		m_bKeys[ VK_ESCAPE ] = GL_FALSE;
		Exit();
	}

	if( m_bKeys[ 0xC0 ] ) { //~
		m_bKeys[ 0xC0 ] = GL_FALSE;
		if( GetConsole()->IsConsole() )
			GetConsole()->HideConsole();
		else
			GetConsole()->ShowConsole();
	}

	if( m_bKeys[ 'B' ] ) {
		m_bUseBump = !m_bUseBump;
		m_bKeys[ 'B' ] = GL_FALSE;
	}

#if _USE_SOUND_OPENAL_
	if( m_bKeys[ 'P' ] ) {
		m_bKeys[ 'P' ] = GL_FALSE;
		GetOpenALManager()->Play( 1 );
	}
	if( m_bKeys[ 'S' ] ) {
		m_bKeys[ 'S' ] = GL_FALSE;
		GetOpenALManager()->Stop( 1 );
	}
#endif

/*
	//przyklad wykorzystania funkcji CWindowData::FullscreenSwitch()
	//gdzie klawiszem F2 mamy mozliwosc przelaczania sie pomiedzy trybami
	//pelnoekranowym a okienkowym
	if( m_bKeys[ VK_F2 ] ) {
		m_bKeys[ VK_F2 ] = GL_FALSE;
		GetWinData()->FullscreenSwitch();
	}
*/
}

GLboolean CGameControl::WriteOnKeyboard( GLuint uiKey, LPTSTR lpBuffer, GLint iBufferLenMax )
{
	GLint iLength = static_cast< GLint >( _tcslen( lpBuffer ) );
	
	switch( uiKey ) {
		case VK_BACK:
			if( iLength > 0 ) --iLength;
			lpBuffer[ iLength ] = 0;
			return GL_TRUE;
		default:
			if( iLength == 0 && uiKey == VK_SPACE )
				return GL_TRUE; //pierwszym znakiem nie moze byc spacja
			if( iLength >= iBufferLenMax - 1 ) {
				//if( CWindowData::GetInstance()->SettingFile().bSound )
				//	COpenALManager::GetInstance()->PlayNo3D( id_sound_max_string );
				return GL_TRUE; //ograniczenie dlugosci
			}
			if(/*number*/( uiKey >= 0x30 && uiKey <= 0x39 )
				/*alphabet*/|| ( uiKey >= 0x41 && uiKey <= 0x5A ) || uiKey == 0x20 
				/*NUMPAD*/|| ( uiKey >= 0x60 && uiKey <= 0x6F ) 
				/*pozostale ,.;'[]*/|| ( uiKey >= 0xBA && uiKey <= 0xBF ) || ( uiKey >= 0xDB && uiKey <= 0xDE )
			) {
				lpBuffer[ iLength ] = static_cast< TCHAR >( GetKeyChar( uiKey ) );
				lpBuffer[ iLength + 1 ] = 0;
				return GL_TRUE;
			}
			break;
	}

	return GL_FALSE;
}

GLuint CGameControl::GetKeyChar( GLuint uiKeyCode )
{
	//numpad
	if( uiKeyCode == 0x60 ) return 0x30;
	if( uiKeyCode == 0x61 ) return 0x31;
	if( uiKeyCode == 0x62 ) return 0x32;
	if( uiKeyCode == 0x63 ) return 0x33;
	if( uiKeyCode == 0x64 ) return 0x34;
	if( uiKeyCode == 0x65 ) return 0x35;
	if( uiKeyCode == 0x66 ) return 0x36;
	if( uiKeyCode == 0x67 ) return 0x37;
	if( uiKeyCode == 0x68 ) return 0x38;
	if( uiKeyCode == 0x69 ) return 0x39;
	if( uiKeyCode == 0x6A ) return 0x2A; //*
	if( uiKeyCode == 0x6B ) return 0x2B; //+
	if( uiKeyCode == 0x6C ) return 0x2C; //,
	if( uiKeyCode == 0x6D ) return 0x2D; //-
	if( uiKeyCode == 0x6E ) return 0x2E; //.
	if( uiKeyCode == 0x6F ) return 0x2F; ///

	if( HIBYTE( ::GetKeyState( VK_SHIFT ) ) ) { //kombinacja Shift+...
		//Shift+numer
		if( uiKeyCode == 0x30 ) uiKeyCode = 0x29; // )
		if( uiKeyCode == 0x31 ) uiKeyCode = 0x21; //!
		if( uiKeyCode == 0x32 ) uiKeyCode = 0x40; //@
		if( uiKeyCode == 0x33 ) uiKeyCode = 0x23; //#
		if( uiKeyCode == 0x34 ) uiKeyCode = 0x24; //$
		if( uiKeyCode == 0x35 ) uiKeyCode = 0x25; //%
		if( uiKeyCode == 0x36 ) uiKeyCode = 0x5E; //^
		if( uiKeyCode == 0x37 ) uiKeyCode = 0x26; //&
		if( uiKeyCode == 0x38 ) uiKeyCode = 0x2A; //*
		if( uiKeyCode == 0x39 ) uiKeyCode = 0x28; //(
		//if( uiKeyCode >= 0x41 && uiKeyCode <= 0x5A ) uiKeyCode += 32; //alfabet jet ok bo od razu dostajemy wielkimi
		if( uiKeyCode == 0xBA ) uiKeyCode = 0x3A; // znak :
		if( uiKeyCode == 0xBB ) uiKeyCode = 0x2B; // znak +
		if( uiKeyCode == 0xBC ) uiKeyCode = 0x3C; // znak <
		if( uiKeyCode == 0xBD ) uiKeyCode = 0x5F; // znak _
		if( uiKeyCode == 0xBE ) uiKeyCode = 0x3E; // znak >
		if( uiKeyCode == 0xBF ) uiKeyCode = 0x3F; // znak ?
		if( uiKeyCode == 0xDB ) uiKeyCode = 0x7B; // znak {
		if( uiKeyCode == 0xDC ) uiKeyCode = 0x7C; // znak |
		if( uiKeyCode == 0xDD ) uiKeyCode = 0x7D; // znak }
		if( uiKeyCode == 0xDE ) uiKeyCode = 0x22; // znak "
	}
	else { //bez shifta
		if( uiKeyCode >= 0x41 && uiKeyCode <= 0x5A ) uiKeyCode += 32; //alfabet malymi literami
		if( uiKeyCode == 0xBA ) uiKeyCode = 0x3B; // znak ;
		if( uiKeyCode == 0xBB ) uiKeyCode = 0x3D; // znak =
		if( uiKeyCode == 0xBC ) uiKeyCode = 0x2C; // znak ,
		if( uiKeyCode == 0xBD ) uiKeyCode = 0x2D; // znak -
		if( uiKeyCode == 0xBE ) uiKeyCode = 0x2E; // znak .
		if( uiKeyCode == 0xBF ) uiKeyCode = 0x2F; // znak /
		if( uiKeyCode == 0xDB ) uiKeyCode = 0x5B; // znak [
		if( uiKeyCode == 0xDC ) uiKeyCode = 0x5C; // znak '\'
		if( uiKeyCode == 0xDD ) uiKeyCode = 0x5D; // znak ]
		if( uiKeyCode == 0xDE ) uiKeyCode = 0x27; // znak '
	}

	return uiKeyCode;
}

GLvoid CGameControl::SetIndicesQuadAllScreen()
{
	m_aIndicesQuadAllScreen[ 0 ] = CWindowData::GetInstance()->SettingFile().iWidth;
	m_aIndicesQuadAllScreen[ 1 ] = CWindowData::GetInstance()->SettingFile().iHeight;
	m_aIndicesQuadAllScreen[ 2 ] = CWindowData::GetInstance()->SettingFile().iWidth;
	m_aIndicesQuadAllScreen[ 3 ] = 0;
	m_aIndicesQuadAllScreen[ 4 ] = 0;
	m_aIndicesQuadAllScreen[ 5 ] = CWindowData::GetInstance()->SettingFile().iHeight;
	m_aIndicesQuadAllScreen[ 6 ] = 0;
	m_aIndicesQuadAllScreen[ 7 ] = 0;
}

GLvoid CGameControl::DrawQuadOnAllScreen()
{
	glEnableClientState( GL_VERTEX_ARRAY );
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );
	glDisableClientState( GL_NORMAL_ARRAY );
	glDisableClientState( GL_COLOR_ARRAY );

	glVertexPointer( 2, GL_INT, 0, m_aIndicesQuadAllScreen );
	glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
}