// WindowsData.cpp: implementation of the CWindowData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WindowData.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWindowData* CWindowData::Construct()
{
	return CSingletonBase::Construct( new CWindowData );
}

CWindowData::CWindowData()
{
//******************** nie zmieniaj: *******************************************
	m_hRC = NULL;		// Sta³y kontekst renderowania
	m_hDC = NULL;		// Prywatny kontekst urz¹dzenia GDI
	m_hWnd = NULL;		// Przechowuje uchwyt okna

	m_pWndProc = NULL;
	m_iIcon = 0;
	m_iCursor = 0;
	m_bRestart = GL_FALSE;
	m_bCharModeKeyboard = GL_FALSE;
	SetDoubleRestart( GL_FALSE );
//*****************************************************************************

//*********************** zmienne do modyfikowania ****************************
	//domyslny kolor tla rgb+alfa
	m_fBkColor[ 0 ] = 0.0f;
	m_fBkColor[ 1 ] = 0.0f;
	m_fBkColor[ 2 ] = 0.0f;
	m_fBkColor[ 3 ] = 1.0f;

	m_uiTimer = 1000; //iloœæ milisekund, =0-timer nieaktywny

	m_bUseAntialiasing = GL_TRUE; //pe³noekranowy antyaliasing

	m_lpTitle = GAME_NAME;		//tytul okna
	m_lpClassName = GAME_NAME;	//nazwa klasy

	//pobranie rozdzielczosci i glebie kolorow z ustawien ekranu
	m_iDesktopWidth = ::GetSystemMetrics( SM_CXSCREEN );
	m_iDesktopHeight = ::GetSystemMetrics( SM_CYSCREEN );

	HDC hDCScreen = ::GetDC( NULL );
	m_ubBitsPerPixel = static_cast< GLubyte >( ::GetDeviceCaps( hDCScreen, BITSPIXEL ) );
	::ReleaseDC( NULL, hDCScreen );

	m_ubZBufferBits = 24; //bufor g³êbi 16/24/32
	m_bShowMouseCursor = GL_TRUE;	//czy kursor myszy ma byc pokazywany - tylko dla fullscreen

//******************************************************************************
}

GLboolean CWindowData::Initialization()
{
//******************** nie zmieniaj: *******************************************
	GLboolean bResult = GL_TRUE;
	
	m_pMultiLang = CMultiLanguage::Construct();
	if( !m_pMultiLang ) bResult = GL_FALSE; 
	
	m_pLogger = new CLogger( LOG_FILE_NAME );
	if( !m_pLogger ) bResult = GL_FALSE; 
	
	m_pARBMultisample = new CARBMultisample();
	if( !m_pARBMultisample ) bResult = GL_FALSE; 

	GetMultiLang()->InitializeLanguage();

	if( !LoadSettingFile()  ) //blad odczytu pliku setting, wiec zapisujemy z ustawieniami domyœlnymi
		SaveSettingFile( GL_TRUE );

	if( m_sSettingFile.bClearOldLog )
		GetLogger()->ClearOldLog();

	if( IsCommandLine( _T("/logger") ) )
		GetLogger()->SetActive( GL_TRUE ); //otwieramy loga

//******************************************************************************

	return bResult;
}

CWindowData::~CWindowData()
{
	delete m_pARBMultisample;
	delete m_pLogger;
}

GLboolean CWindowData::LoadSettingFile()
{
	FILE *hFile = NULL;
	if( fopen_s( &hFile, SETTING_FILE_NAME, "rb" ) != 0 )
		return GL_FALSE;

	fread( &m_sSettingFile, sizeof( SSettingFile ), 1, hFile );
	fclose( hFile );

	if( m_sSettingFile.ubMajorVersion == MAJOR_VERSION_FILE &&
		m_sSettingFile.ubMinorVersion == MINOR_VERSION_FILE &&
		m_sSettingFile.ubReleaseVersion == RELEASE_VERSION_FILE )
	{
		m_pARBMultisample->SetSamples( m_sSettingFile.iAntialiasing );
		if( !GetAntialiasingSamples() )
			m_bUseAntialiasing = GL_FALSE;

		CBrightness::GetInstance()->SetBrightness( m_sSettingFile.iBrightness );
	}
	else {
		::MessageBox( NULL, _T("Incorrect setting file version. Suggestion: delete \"setting.cfg\" file."), GAME_NAME, MB_OK | MB_ICONERROR );
		return GL_FALSE;
	}

	//co do bug-a 51, rozdzielczoœæ w setting nie moze byæ wiêksza od pulpitowej
	if( m_sSettingFile.iWidth > m_iDesktopWidth && m_sSettingFile.iHeight > m_iDesktopHeight ) {
		m_sSettingFile.iWidth = m_iDesktopWidth;
		m_sSettingFile.iHeight = m_iDesktopHeight;
		SaveSettingFile();
	}

	return GL_TRUE;
}

GLboolean CWindowData::SaveSettingFile( GLboolean bDefaultSave /*= GL_FALSE*/ )
{
	FILE *hFile = NULL;
	if( fopen_s( &hFile, SETTING_FILE_NAME, "wb" ) != 0 )
		return GL_FALSE;

	if( bDefaultSave ) {
		m_sSettingFile.iAntialiasing = m_pARBMultisample->GetSamples();

		m_sSettingFile.ubMajorVersion = ( GLubyte )MAJOR_VERSION_FILE;
		m_sSettingFile.ubMinorVersion = ( GLubyte )MINOR_VERSION_FILE;
		m_sSettingFile.ubReleaseVersion = ( GLubyte )RELEASE_VERSION_FILE;

#if _USE_SYSTEM_RES_AND_BITSPERPIXEL_
		m_sSettingFile.iWidth = m_iDesktopWidth;
		m_sSettingFile.iHeight = m_iDesktopHeight;
		m_sSettingFile.ubBitsPerPixel = m_ubBitsPerPixel;
#else
		//tutaj mozemy podac recznie wlasne ustawienia
		m_sSettingFile.iWidth = _SCREEN_WIDTH_;
		m_sSettingFile.iHeight = _SCREEN_HEIGHT_;
		m_sSettingFile.ubBitsPerPixel = _SCREEN_BITS_;
#endif
		m_sSettingFile.bFullScreen = GL_TRUE;
		m_sSettingFile.bVSync = GL_TRUE;
		m_sSettingFile.bClearOldLog = GetLogger()->IsClearOldLog();
		m_sSettingFile.iIndexLanguage = 0;
		m_sSettingFile.bSound = GL_TRUE;
		m_sSettingFile.bMusic = GL_TRUE;
		m_sSettingFile.iVolumeSound = DEFAULT_SOUND_VOLUME;
		m_sSettingFile.iVolumeMusic = DEFAULT_MUSIC_VOLUME;
		m_sSettingFile.iBrightness = 0;
	}

	fwrite( &m_sSettingFile, sizeof( SSettingFile ), 1, hFile );
	fclose( hFile );

	return GL_TRUE;
}

GLboolean CWindowData::CreateGLWindow( WNDPROC* pWndProc, const GLint iIcon, const GLint iCursor )
{
	m_pWndProc = pWndProc;
	m_iIcon = iIcon;
	m_iCursor = iCursor;

	if( !CreateInstance() ) return GL_FALSE;
	if( !RegisterMyClass() ) return GL_FALSE;
	SetDeviceOnFullscreen();
	if( !CreateMyWindow() ) return GL_FALSE;
	if( !GetDeviceContent() ) return GL_FALSE;
	if( !SetPixelFormat() ) {
		KillGLWindow();
		return GL_FALSE;
	}
	if( !CreateRenderingContext() ) {
		KillGLWindow();
		return GL_FALSE;
	}

	if( !m_pARBMultisample->IsSupported() && m_bUseAntialiasing ) {
		if( m_pARBMultisample->InitMultisample( m_hWnd ) ) {
			KillGLWindow();
			return CreateGLWindow( pWndProc, iIcon, iCursor );
		}
		else
			m_bUseAntialiasing = GL_FALSE;
	}

	::ShowWindow( m_hWnd, SW_SHOW );	// Pokazuje okno
	::SetForegroundWindow( m_hWnd );	// Wy¿szy priorytet
	::SetFocus( m_hWnd );				// Uaktywnia okno

	if( m_uiTimer > 0 )
		::SetTimer( m_hWnd, ID_TIMER, m_uiTimer, NULL );

	return GL_TRUE;
}

GLvoid CWindowData::KillGLWindow()
{
	if( m_uiTimer > 0 )
		::KillTimer( m_hWnd, ID_TIMER );

	if( m_sSettingFile.bFullScreen ) {
		LONG lResult = ::ChangeDisplaySettings( NULL, 0 );// prze³¹cza z powrotem do pulpitu
		switch( lResult ) {
			case DISP_CHANGE_SUCCESSFUL:
				__LOG( _T("KillGLWindow - ChangeDisplaySettings - The settings change was successful.") );
				break;
			case DISP_CHANGE_RESTART:
				__LOG( _T("KillGLWindow - ChangeDisplaySettings - The computer must be restarted in order for the graphics mode to work.") );
				break;
			case DISP_CHANGE_BADFLAGS:
				__LOG( _T("KillGLWindow - ChangeDisplaySettings - Bad flags.") );
				break;
			case DISP_CHANGE_BADPARAM:
				__LOG( _T("KillGLWindow - ChangeDisplaySettings - Bad param.") );
				break;
			case DISP_CHANGE_FAILED:
				__LOG( _T("KillGLWindow - ChangeDisplaySettings - The display driver failed the specified graphics mode.") );
				break;
			case DISP_CHANGE_BADMODE:
				__LOG( _T("KillGLWindow - ChangeDisplaySettings - The graphics mode is not supported.") );
				break;
			case DISP_CHANGE_NOTUPDATED:
				__LOG( _T("KillGLWindow - ChangeDisplaySettings - NOUPDATE, Unable to write settings to the registry.") );
				break;
		}
		::ShowCursor( TRUE );
	}

	if( m_hRC ) { // Czy mamy uchwyt do kontekstu renderowania?
		wglMakeCurrent( NULL, NULL );
		__LOG_LAST_ERROR( _T("KillGLWindow - wglMakeCurrent") );
		
		wglDeleteContext( m_hRC );
		__LOG_LAST_ERROR( _T("KillGLWindow - wglDeleteContext") );
		
		m_hRC = NULL;
	}

	if( m_hDC ) {
		if( !::ReleaseDC( m_hWnd, m_hDC ) ) // Czy jesteœmy w stanie usun¹æ DC?
			m_hDC = NULL;
		__LOG_LAST_ERROR( _T("KillGLWindow - ReleaseDC") );
	}

	if( m_hWnd ) {
		if( !::DestroyWindow( m_hWnd ) )
			m_hWnd = NULL;
		__LOG_LAST_ERROR( _T("KillGLWindow - DestroyWindow") );
	}

	if( !::UnregisterClass( m_lpClassName, m_hInstance ) )	//Czy jesteœmy w stanie wyrejestrowaæ klasê?
		m_hInstance = NULL;
	__LOG_LAST_ERROR( _T("KillGLWindow - UnregisterClass") );
}

GLvoid CWindowData::RestartWindow()
{
	KillGLWindow();
	m_pARBMultisample->SetSupported( GL_FALSE );
	m_bRestart = GL_TRUE;
	CreateGLWindow( m_pWndProc, m_iIcon, m_iCursor );
}

GLvoid CWindowData::FullscreenSwitch()
{
	m_sSettingFile.bFullScreen = !m_sSettingFile.bFullScreen;
	RestartWindow();
}

GLboolean CWindowData::IsCommandLine( LPCTSTR lpParam )
{
	return ( _tcsstr( GetCommandLine(), lpParam ) == NULL ) ? GL_FALSE : GL_TRUE;
}

GLvoid CWindowData::SetAntialiasingSamples( GLint iSamples, GLboolean bRestart /*= GL_FALSE*/ )
{
	m_bUseAntialiasing = GL_TRUE;
	m_pARBMultisample->SetSamples( iSamples );
	if( bRestart ) {
		//gdy np. zmieniamy antialiasing z konsoli
		//zapisujemy takze w ustawieniach
		m_sSettingFile.iAntialiasing = iSamples;
		SaveSettingFile();
		RestartWindow();
	}
}

GLvoid CWindowData::ReSizeGLScene( GLfloat fViewAngle /*= 45.0f*/ )
{
	ReSizeGLScene( m_sSettingFile.iWidth, m_sSettingFile.iHeight, fViewAngle );
}

GLvoid CWindowData::ReSizeGLScene( GLsizei iWidth, GLsizei iHeight, GLfloat fViewAngle /*= 45.0f*/ )
{
	if( iHeight == 0 )
		iHeight = 1;
	
	glViewport( 0, 0, iWidth, iHeight );

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	// Oblicza stosunek wysokoœci do szerokoœci okna
	gluPerspective( fViewAngle, ( GLfloat )iWidth / ( GLfloat )iHeight, 1.0f, MAX_DISTANCE_VIEW );

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
}

GLboolean CWindowData::CreateInstance()
{
	// Pobiera instancjê naszego programu 
	m_hInstance = ::GetModuleHandle( NULL );
	__LOG_LAST_ERROR( _T("GetModuleHandle") );

	return ( m_hInstance != NULL );
}

GLboolean CWindowData::RegisterMyClass()
{
	if( !m_pWndProc || !m_lpClassName || !m_hInstance ) {
		__LOG( _T("ERROR: RegisterMyClass - incorrect need parameters") );
		return GL_FALSE;
	}

	WNDCLASS wc;												// Struktura klasy okna
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Ustawia odœwie¿anie okna przy zmianie wymiarów b¹dŸ pozycji oraz prywatny DC
	wc.lpfnWndProc		= ( WNDPROC )m_pWndProc;				// WndProc obs³uguje wiadomoœci
	wc.cbClsExtra		= 0;									// Brak dodatkowych danych okna
	wc.cbWndExtra		= 0;									// Brak dodatkowych danych okna
	wc.hInstance		= m_hInstance;							// Ustawia instancjê
	wc.hIcon			= ::LoadIcon( m_hInstance, MAKEINTRESOURCE( m_iIcon ) );
	wc.hCursor			= ::LoadCursor( m_hInstance, MAKEINTRESOURCE( m_iCursor ) );
	wc.hbrBackground	= NULL;									// GL nie wymaga t³a
	wc.lpszMenuName		= NULL;									// Nie potrzebujemy menu
	wc.lpszClassName	= m_lpClassName;						// Ustawia nazwê klasy

	ATOM aResult = ::RegisterClass( &wc );
	__LOG_LAST_ERROR( _T("RegisterClass") );
	
	return ( aResult != 0 );
}

GLvoid CWindowData::SetDeviceOnFullscreen()
{
	if( !m_sSettingFile.bFullScreen )
		return;

	DEVMODE dmScreenSettings;										// Tryb urz¹dzenia
	::SecureZeroMemory( &dmScreenSettings, sizeof( dmScreenSettings ) );	// Upewnij siê, ¿e pamiêæ jest wyczyszczona
	dmScreenSettings.dmSize = sizeof( dmScreenSettings );			// Rozmiar struktury Devmode
	dmScreenSettings.dmPelsWidth = m_sSettingFile.iWidth;			// Wybrana szerokoœæ ekranu
	dmScreenSettings.dmPelsHeight = m_sSettingFile.iHeight;			// Wybrana wysokoœæ ekranu
	dmScreenSettings.dmBitsPerPel = m_sSettingFile.ubBitsPerPixel;	// Wybrana g³êbia kolorów ( w bitach na piksel )
	dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
	
	// Próbuje ustawiæ wybrany tryb. Zauwa¿:CDS_FULLSCREEN pozbywa siê paska start
	if( ::ChangeDisplaySettings( &dmScreenSettings, CDS_FULLSCREEN ) != DISP_CHANGE_SUCCESSFUL ) {
		__LOG( _T("Fullscreen mode is not supported.") );
		::MessageBox( NULL,_T("Fullscreen mode is not supported."), GAME_NAME, MB_OK | MB_ICONEXCLAMATION );
		m_sSettingFile.bFullScreen = GL_FALSE;
	}
}

GLboolean CWindowData::CreateMyWindow()
{
	if( !m_lpClassName || !m_lpTitle || !m_hInstance ) {
		__LOG( _T("ERROR: CreateMyWindow - incorrect need parameters") );
		return GL_FALSE;
	}

	DWORD dwExStyle = 0;									// Rozszerzony styl okna
	DWORD dwStyle = 0;										// Normalny styl okna

	RECT rcWindowRect;										//Przechowuje wspó³rzêdne górnego lewego i dolnego prawego rogu okna
	rcWindowRect.left	= ( LONG )0;						// Ustawia wspó³rzêdn¹ lewej krawêdzi na 0
	rcWindowRect.right	= ( LONG )m_sSettingFile.iWidth;	// Ustawia wspó³rzêdn¹ prawej krawêdzi na ¿¹dan¹ szerokoœæ
	rcWindowRect.top	= ( LONG )0;						// Ustawia wspó³rzêdn¹ górnej krawêdzi na 0
	rcWindowRect.bottom = ( LONG )m_sSettingFile.iHeight;	// Ustawia wspó³rzêdn¹ dolnej krawêdzi na ¿¹dan¹ wysokoœæ

	if( m_sSettingFile.bFullScreen ) {		// Czy nadal jesteœmy w trybie pe³noekranowym?
		dwExStyle = WS_EX_APPWINDOW;		// Rozszerzony styl okna
		dwStyle = WS_POPUP;					// Normalny styl okna
		::ShowCursor( m_bShowMouseCursor );	// Ukrywa kursor myszy - jezeli program ma nie byc interaktywny - zalezy od programisty
	}
	else {
		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;	// Rozszerzony styl okna
		//dwStyle = WS_OVERLAPPEDWINDOW;					// Normalny styl okna - z minimalizacja itd, paskiem itp
		dwStyle = WS_OVERLAPPED | WS_CAPTION;
		dwStyle |= WS_SYSMENU;			//ikona oraz przyciski [_][#][X] na belce okna
		//dwStyle |= WS_THICKFRAME;		//mozliwosc zmiany rozmiaru okna
		//dwStyle |= WS_MAXIMIZEBOX;	//przyciski [#] dostepny
		dwStyle |= WS_MINIMIZEBOX;		//przyciski [_] dostepny
	}

	// Poprawia wymiary okna, aby naprawdê odzwierciedla³y ¿¹dane rozmiary
	if( !::AdjustWindowRectEx( &rcWindowRect, dwStyle, FALSE, dwExStyle ) ) {
		__LOG_LAST_ERROR( _T("AdjustWindowRectEx") );
		return GL_FALSE;
	}

	m_hWnd = ::CreateWindowEx( dwExStyle,									// Rozszerzony styl okna
							   m_lpClassName,								// Nazwa klasy okna
							   m_lpTitle,									// Nag³ówek okna
							   WS_CLIPSIBLINGS | WS_CLIPCHILDREN | dwStyle,	// Wybrany styl okna
							   0, 0,										// Pozycja okna
							   rcWindowRect.right - rcWindowRect.left,		// Oblicza poprawion¹ szerokoœæ okna
							   rcWindowRect.bottom - rcWindowRect.top,		// Oblicza poprawion¹ wysokoœæ okna
							   NULL,										// Bez okna nadrzêdnego
							   NULL,										// Bez menu
							   m_hInstance,									// Instancja
							   NULL );										// Nie przekazuje niczego do WM_CREATE
	
	__LOG_LAST_ERROR( _T("CreateWindowEx") );
	
	if( !m_hWnd ) {
		KillGLWindow();
		return GL_FALSE;
	}
	
	return GL_TRUE;
}

GLboolean CWindowData::GetDeviceContent()
{
	if( !m_hWnd ) {
		__LOG( _T("ERROR: GetDeviceContent - incorrect need parameters") );
		return GL_FALSE;
	}

	m_hDC = ::GetDC( m_hWnd );
	__LOG_LAST_ERROR( _T("GetDC") );
	if( !m_hDC ) {
		KillGLWindow();
		return GL_FALSE;
	}
	
	return GL_TRUE;
}

GLboolean CWindowData::SetPixelFormat()
{
	if( !m_hDC ) {
		__LOG( _T("ERROR: SetPixelFormat - incorrect need parameters") );
		return GL_FALSE;
	}

	GLint iPixelFormat;						// Przechowuje rezultat bêd¹cy wynikiem szukania pasuj¹cego formatu piksela
	
	static PIXELFORMATDESCRIPTOR pfd =		// W pfd precyzujemy ¿¹dane ustawienia
	{
		sizeof( PIXELFORMATDESCRIPTOR ),	// Rozmiar deskryptora formatu piksela
		1,									// Numer wersji
		PFD_DRAW_TO_WINDOW |				// Format musi obs³ugiwaæ okno
		PFD_SUPPORT_OPENGL |				// Format musi obs³ugiwaæ OpenGL
		PFD_DOUBLEBUFFER,					// Musi obs³ugiwaæ podwójne buforowanie
		PFD_TYPE_RGBA,						// ¯¹da formatu RGBA
		m_sSettingFile.ubBitsPerPixel,		// Wybiera nasz¹ g³êbiê kolorów
		0, 0, 0, 0, 0, 0,					// Ignoruje bity koloru
		0,									// Brak bufora Alfa
		0,									// Ignoruje bit przesuniêcia
		0,								    // Brak bufora akumulacyjnego
		0, 0, 0, 0,						    // Ignoruje bity akumulacyjne
		m_ubZBufferBits,					// Z-Bufor ( bufor g³êbi )
		1,									// Uzyj bufora szablonów
		0,									// Brak bufora pomocniczego
		PFD_MAIN_PLANE,						// G³ówna warstwa rysowania
		0,									// Zarezerwowane
		0, 0, 0								// Ignoruje maski warstwy
	};

	if( m_pARBMultisample->IsSupported() )
		iPixelFormat = m_pARBMultisample->GetFormat();
	else {
		iPixelFormat = ::ChoosePixelFormat( m_hDC, &pfd );
		__LOG_LAST_ERROR( _T("ChoosePixelFormat") );
		if( !iPixelFormat ) //Czy system Windows znalaz³ pasuj¹cy format piksela?
			return GL_FALSE;
	}

	BOOL bResult = ::SetPixelFormat( m_hDC, iPixelFormat, &pfd );
	__LOG_LAST_ERROR( _T("SetPixelFormat") );
	if( !bResult )
		return GL_FALSE;

	return GL_TRUE;
}

GLboolean CWindowData::CreateRenderingContext()
{
	if( !m_hDC ) {
		__LOG( _T("ERROR: CreateRenderingContext - incorrect need parameters") );
		return GL_FALSE;
	}

	//Pobranie kontekstu renderowania
	m_hRC = wglCreateContext( m_hDC );
	__LOG_LAST_ERROR( _T("wglCreateContext") );
	if( !m_hRC )
		return GL_FALSE;
	
	//Próbuje uaktywniæ kontekst renderowania
	BOOL bResult = wglMakeCurrent( m_hDC, m_hRC ); 
	__LOG_LAST_ERROR( _T("wglMakeCurrent") );
	if( !bResult )
		return GL_FALSE;
	
	return GL_TRUE;
}