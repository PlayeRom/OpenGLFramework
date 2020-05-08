
/** \file
	\brief Modu³ szkieletu (framework).
*/

#include "StdAfx.h"
#include "WindowData.h"
#include "GameControl.h"
#include "Cursor.h"
#include "resource.h"
#include "Console.h"
#include "Framework.h"
#include <time.h>

CWindowData*	CFramework::g_pWinData = NULL;			///< WskaŸnik na obiekt klasy singletonu CWindowData.
CGameControl*	CFramework::g_pGameCtrl = NULL;			///< WskaŸnik na obiekt klasy singletonu CGameControl.
GLboolean		CFramework::g_bActive = GL_TRUE;		///< Znacznik aktywnoœci okna, domyœlnie ustawiony na \p GL_TRUE np. okno nie zminimalizowane.
GLint			CFramework::g_iCompleteWheelRotate = 0;	///< Licznik kompletnego obrotu rolki myszy.

LRESULT	CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM );

CFramework::CFramework()
{
	::timeBeginPeriod( 1 );
}

CFramework::~CFramework()
{
	::timeEndPeriod( 1 );
}

GLint CFramework::Run()
{
	CBrightness::Construct();
	g_pWinData = CWindowData::Construct();
	if( !g_pWinData->Initialization() ) {
		::MessageBox( NULL, _T("Window initialization error"), GAME_NAME, MB_OK | MB_ICONEXCLAMATION );
		Destroy( GL_FALSE );
		return 0;
	}

#if _QUESTION_FULLSCREEN_
	if( ::MessageBox( NULL, _T("Fullscreen mode?"), GAME_NAME, MB_YESNO | MB_ICONQUESTION ) == IDNO )
		g_pWinData->SettingFile().bFullScreen = GL_FALSE;
	else
		g_pWinData->SettingFile().bFullScreen = GL_TRUE;
#endif

	if( !g_pWinData->CreateGLWindow( ( WNDPROC* )WndProc, IDI_OPENGLFRAMEWORK, IDC_CURSOR1 ) ) {
		Destroy( GL_FALSE );
		return 0;
	}

	srand( ( unsigned )time( NULL ) );

	g_pWinData->ReSizeGLScene();	// Ustawia perspektywê

	if( !InitalizationGL() ) {					// Inicjalizuje nasze nowo utworzone okno
		::MessageBox( NULL, _T("OpenGL initialization error"), GAME_NAME, MB_OK | MB_ICONEXCLAMATION );
		Destroy( GL_TRUE );
		return 0;
	}

	MSG sMsg = { 0 };
	GLboolean bDone = GL_FALSE;
	
	while( !bDone ) {
		PROFILER_BEGIN( _T("MAIN LOOP") );

		if( g_pWinData->IsRestart() ) {
			g_pWinData->ReSizeGLScene();	// Ustawia perspektywê

			if( !InitalizationGL() ) {					// Inicjalizuje nasze nowo utworzone okno
				::MessageBox( NULL, _T("OpenGL initialization error"), GAME_NAME, MB_OK | MB_ICONEXCLAMATION );
				Destroy( GL_TRUE );
				PROFILER_END();
				return 0;
			}
			g_pWinData->SetRestart( GL_FALSE );
		}
		if( g_pWinData->IsDoubleRestart() ) {
			g_pWinData->SettingFile().iWidth = g_pWinData->GetResDoubleRestartX();
			g_pWinData->SettingFile().iHeight = g_pWinData->GetResDoubleRestartY();
			g_pWinData->SettingFile().bFullScreen = GL_FALSE;
			g_pWinData->SaveSettingFile();
			g_pWinData->SetDoubleRestart( GL_FALSE );
			g_pWinData->RestartWindow();
		}

		if( ::PeekMessage( &sMsg, NULL, 0, 0, PM_REMOVE ) ) {
			if( sMsg.message == WM_QUIT )
				bDone = GL_TRUE;
			else {
				::TranslateMessage( &sMsg );
				::DispatchMessage( &sMsg );
			}
		}
		else {
			if( g_bActive ) {				// Czy program jest aktywny?
				DrawGLScene();
				::SwapBuffers( g_pWinData->GetHDC() );	// Zamienia bufory ( podwójne buforowanie )
				
				g_pGameCtrl->Keyboard();
			}
		}

		PROFILER_END();
		PROFILER_FLUSH();
	}

	Destroy( GL_TRUE );

	return static_cast< GLint >( sMsg.wParam );
}

GLboolean CFramework::InitalizationGL()
{
	glShadeModel( GL_SMOOTH );
	glClearColor( g_pWinData->GetBkColorR(), g_pWinData->GetBkColorG(), g_pWinData->GetBkColorB(), g_pWinData->GetBkColorA() );
	glClearStencil( 0 );
	glClearDepth( 1.0f );
	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LEQUAL );
	glDisable( GL_DITHER ); //szybciej dziala, gorzej wyglada dla siawtla i cieniowania
	
	glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );

	if( !CreateGameControl() )
		return GL_FALSE;

#if _USE_2D_ONLY_
	g_pGameCtrl->Enable2D();
#endif

	__LOG( _T("Initialization OpenGL -> OK") );

	return GL_TRUE;
}

GLboolean CFramework::DrawGLScene()
{
	if( g_pWinData->IsUseAntialiasing() )
		glEnable( GL_MULTISAMPLE_ARB );

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
	glLoadIdentity();

	g_pGameCtrl->Draw(); //glowna funkcja rysujaca

	glFlush();
	if( g_pWinData->IsUseAntialiasing() )
		glDisable( GL_MULTISAMPLE_ARB );

	return GL_TRUE;
}

GLboolean CFramework::CreateGameControl()
{
	if( g_pWinData->IsRestart() ) {
		//jezeli wywolywane podczas restartu okna
		g_pGameCtrl->RestartObjects();
		return GL_TRUE;
	}
	
	g_pGameCtrl = CGameControl::Construct();
	if( !g_pGameCtrl ) {
		__LOG( _T("ERROR: CreateGameControl() - g_pGameCtrl = CGameControl::Construct() failed") );
		return GL_FALSE;
	}
	if( !g_pGameCtrl->Initialization() ) {
		__LOG( _T("ERROR: CreateGameControl() - g_pGameCtrl->Initialization() failed") );
		return GL_FALSE;
	}

	return GL_TRUE;
}

/// Funkcja przetwarzaj¹ca komunikaty systemowe przekazywane do okna aplikacji.
/*! Funkcja ta przetwarza niezbêdne komunikaty przekazywane przez system i przekazuje je dalej do klasy CGameControl
gdzie zostan¹ odpowiednio obs³u¿one reakcje na dane komunikaty.
\param[in] hWnd Uchyt okna aplikacji
\param[in] uMsg Identyfikator komunikatu jaki bêdzie przetwarzany.
\param[in] wParam Parametr komunikatu.
\param[in] lParam Parametr komunikatu.
\return \p 0 je¿eli komunikat zosta³ przetworzony. */
LRESULT CALLBACK WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( uMsg )
	{
		case WM_ACTIVATE:
		{
			if( !HIWORD( wParam ) )		// Sprawdza, czy okno nie jest zminimalizowane
				CFramework::g_bActive = GL_TRUE;	// Program jest aktywny
			else
				CFramework::g_bActive = GL_FALSE;	// Program nie jest aktywny
			return 0;
		}
		case WM_SYSCOMMAND:				// Przechwytuje komendy systemowe
		{
			switch( wParam ) {			// Sprawdza wywo³ania systemowe
				case SC_SCREENSAVE:		// Czy wygaszacz ekranu próbuje siê uruchomiæ?
				case SC_MONITORPOWER:	// Czy monitor stara siê wejœæ w tryb oszczêdzania energii?
					return 0;			// Zapobiega obu tym czynnoœciom
			}
			break;
		}
		case WM_CLOSE:
		{
			::PostQuitMessage( 0 );		// Wysy³a wiadomoœæ o wyjœciu
			return 0;
		}
		case WM_KEYDOWN:
		{
			if( CFramework::g_pWinData->m_bCharModeKeyboard || !CFramework::g_pGameCtrl )
				break;

			CFramework::g_pGameCtrl->KeyDown( static_cast< GLuint >( wParam ) );
			return 0;
		}
		case WM_KEYUP:
		{
			if( CFramework::g_pWinData->m_bCharModeKeyboard || !CFramework::g_pGameCtrl )
				break;

			CFramework::g_pGameCtrl->KeyUp( static_cast< GLuint >( wParam ) );
			return 0;
		}
		case WM_CHAR:
		{
			//TODO nigdy nie testowane i nie u¿ywane
			if( !CFramework::g_pWinData->m_bCharModeKeyboard || !CFramework::g_pGameCtrl )
				break;

			CFramework::g_pGameCtrl->m_cCharKey = ( TCHAR )wParam;
			return 0;
		}
		case WM_SIZE:
		{
			switch( wParam )
			{
				case SIZE_MINIMIZED:
					CFramework::g_bActive = GL_FALSE;
					return 0;
				case SIZE_MAXIMIZED:
					CFramework::g_bActive = GL_TRUE;
					CFramework::g_pWinData->ReSizeGLScene( LOWORD( lParam ), HIWORD( lParam ) );
					return 0;
				case SIZE_RESTORED:
					CFramework::g_bActive = GL_TRUE;
					CFramework::g_pWinData->ReSizeGLScene( LOWORD( lParam ), HIWORD( lParam ) );
					return 0;
			}
		}
		case WM_SETCURSOR:
		{
			if( CFramework::g_pGameCtrl && CFramework::g_pGameCtrl->GetCursor()->GetMyCursor() > 0 ) {
				CFramework::g_pGameCtrl->GetCursor()->SetMyCursor( CFramework::g_pGameCtrl->GetCursor()->GetMyCursor() );
				return 0;
			}
			break;
		}
		case WM_LBUTTONDOWN:
		{
			if( !CFramework::g_pGameCtrl )
				break;

			CFramework::g_pGameCtrl->MouseLButtonDown( LOWORD( lParam ), HIWORD( lParam ) );
			return 0;
		}
		case WM_LBUTTONUP:
		{
			if( !CFramework::g_pGameCtrl )
				break;

			CFramework::g_pGameCtrl->MouseLButtonUp( LOWORD( lParam ), HIWORD( lParam ) );
			return 0;
		}
		case WM_RBUTTONDOWN:
		{
			if( !CFramework::g_pGameCtrl )
				break;

			CFramework::g_pGameCtrl->MouseRButtonDown( LOWORD( lParam ), HIWORD( lParam ) );
			return 0;
		}
		case WM_RBUTTONUP:
		{
			if( !CFramework::g_pGameCtrl )
				break;

			CFramework::g_pGameCtrl->MouseRButtonUp( LOWORD( lParam ), HIWORD( lParam ) );
			return 0;
		}
		case WM_MOUSEWHEEL:
		{
			if( !CFramework::g_pGameCtrl )
				break;

			CFramework::g_iCompleteWheelRotate += GET_WHEEL_DELTA_WPARAM( wParam );
			if( abs( CFramework::g_iCompleteWheelRotate ) >= WHEEL_DELTA ) {
				CFramework::g_pGameCtrl->MouseWheel( CFramework::g_iCompleteWheelRotate / WHEEL_DELTA );
				CFramework::g_iCompleteWheelRotate %= WHEEL_DELTA;
			}
			return 0;
		}
		case WM_TIMER:
		{
			if( !CFramework::g_pGameCtrl )
				break;

			CFramework::g_pGameCtrl->Timer();
			return 0;
		}
	}

	return ::DefWindowProc( hWnd, uMsg, wParam, lParam );
}

GLvoid CFramework::Destroy( GLboolean bKillGLWindow )
{
	if( bKillGLWindow && g_pWinData )
		g_pWinData->KillGLWindow();

	if( g_pGameCtrl )
	{
		delete g_pGameCtrl;
		g_pGameCtrl = NULL;
	}
}