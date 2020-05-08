/*! \file
	\brief Plik nag³ówkowy dla klasy CWindowData.
*/

#pragma once

#include "../Extensions/ARBMultisample.h"
#include "SingletonBase.h"

#define ID_TIMER 1 ///< Identyfikator timer-a okna aplikacji.

class CLogger;
class CMultiLanguage;

/// Klasa okna aplikacji.
/*! Klata ta odpowiada za tworzenie i usuwanie okna oraz co wa¿ne przechowuje ró¿ne informacje takie jak:
     - rozdzielczoœæ ekranu,
     - g³êbia kolorów,
     - kolor t³a,
     - nazwê okna i klasy okna,
     - aktywnoœæ antyaliasingu, trybu pe³noekranowego, wyœwietlanie kursora
 */
class CWindowData : public CSingletonBase <CWindowData>
{
public:
	/// Struktura przechowuj¹ca ustawienia aplikacji.
	/*! S¹ tu przechowywane ró¿ne opcje, które mog¹ byæ modyfikowane w trakcie dzia³ania apliakcji.
	Je¿eli planujemy dodaæ wiêcej opcji konfiguracyjnych (np. w³¹czenie/wy³¹czenie dŸwiêku, wybór jêzyka itp.),
	to nale¿y dodaæ je do struktury SSettingFile. Zapewni to automatyczny zapis i odczyt tych¿e ustawieñ. \n
	Dodaj¹c z czasem nowe pola nale¿y tak¿e pamiêtaæ o zmianie numerowania wersji pliku. Dziêki temu unikniemy
	sytuacji wczytania przez u¿ytkownika starszego pliku konfiguracyjnego, który bêdzie zawiera³ starsz¹ wersjê
	struktury. Dla zmiany wersji pliku konfiguracyjnego, patrz plik define.h. */
	struct SSettingFile
	{
		GLubyte ubMajorVersion;		///< G³ówna wersja pliku konfiguracyjnego.
		GLubyte ubMinorVersion;		///< Pomniejsza wersja pliku konfiguracyjnego.
		GLubyte ubReleaseVersion;	///< Trzecia sk³adowa wersji pliku konfiguracyjnego.
		//
		GLint iWidth;				///< Szerokoœæ okna aplikacji - rozdzielczoœæ X.
		GLint iHeight;				///< Wysokoœæ okna aplikacji - rozdzielczoœæ Y.
		GLubyte ubBitsPerPixel;		///< G³êbia kolorów.
		GLint iAntialiasing;		///< Iloœæ sampli dla antyaliasingu.
		GLboolean bFullScreen;		///< Czy aplikacja ma pracowaæ w trybie pe³noekranowym.
		GLboolean bVSync;			///< Czy w³¹czona jest synchronizacja pionowa.
		GLboolean bClearOldLog;		///< Czy stary plik log-a ma byæ usuwany przy kolejnym uruchomieniem aplikacji.
		GLint iIndexLanguage;		///< Indeks pliku jêzykowego aplikacji.
		GLboolean bSound;			///< Czy dŸwiêk ma byæ w³¹czony.
		GLboolean bMusic;			///< Czy muzyka ma byæ w³¹czona.
		GLint iVolumeSound;			///< G³oœnoœæ dŸwiêków okreœlona w procentach.
		GLint iVolumeMusic;			///< G³oœnoœæ muzyki okreœlona w procentach.
		GLint iBrightness;			///< Jasnoœæ, od -255 (czerñ) do 255 (biel), 0 - normalnie.
	};

	/// Statyczna metoda tworz¹ca obiekt klasy CWindowData. 
	/*! CWindowData jest singletonem, wiêc je¿eli wywo³amy tê metodê po raz drugi to zwróci ona wskaŸnik na obiekt
	ju¿ istniej¹cy, utworzony wczeœniej.
	\return WskaŸnik na obiekt CWindowData. */
	static CWindowData* Construct();

	/// Inicjalizacja obiektu CWindowData.
	/*! Tutaj tworzone s¹ wszelkie obiekty innych klas oraz ustawiane pocz¹tkowe wartoœci zmiennych.
	\return \p GL_TRUE w przypadku powodzenia inicjalizacji wszelkich obiektów. \p GL_FALSE w
	przypadku wyst¹pienia jakiegoœ b³êdu. */
	GLboolean Initialization();

	/// Destruktor.
	/*! Destruktor. */
	virtual ~CWindowData();

	/// G³ówna metoda tworz¹ca okno aplikacji.
	/*! \param[in] pWndProc WskaŸnik na funkcje \ref WndProc.
	\param[in] iIcon ID zasobu ikonki jaka zostanie przypisana aplikacji.
	\param[in] iCursor ID zasobu kursora jaki zostanie wykorzystany w aplikacji.
	\return \p GL_TRUE w przypadku powodzenia utworzenia okna. \p GL_FALSE w przypadku wyst¹pienia jakiegoœ b³êdu. */
	GLboolean CreateGLWindow( WNDPROC *pWndProc, const GLint iIcon, const GLint iCursor );

	/// Metoda niszcz¹ca okno aplikacji.
	GLvoid KillGLWindow();

	/// Metoda restaru okna.
	/*! Metodê tê nale¿y wywo³aæ je¿eli wymagany jest restart okna, np. pod wp³ywem zmiany rozdzielczoœci, g³êbi
	kolorów, próbkowania anyatliasingu, prze³¹czaj¹c siê z trybu okienkowego na pe³no-ekranowy i na odwrót.
	\sa m_bRestart, SetRestart, IsRestart. */
	GLvoid RestartWindow();

	/// Metoda prze³¹czaj¹ca pomiêdzy aplikacj¹ okienkow¹ a pe³no-ekranow¹.
	GLvoid FullscreenSwitch();

	/// Metoda ustawiaj¹ca zmienn¹ nakazuj¹c¹ restartowaæ okno.
	/*! \param[in] bRestart \p GL_TRUE aby restartowaæ okno, \p GL_FALSE aby nie restartowaæ.
	\sa m_bRestart, IsRestart, RestartWindow. */
	inline GLvoid SetRestart( GLboolean bRestart )	{ m_bRestart = bRestart; }

	/// Metoda sprawdzaj¹ca czy nale¿y zrestartowaæ okno aplikacji.
	/*! \return \p GL_TRUE je¿eli nale¿y restartowaæ okno, \p GL_FALSE je¿eli nie.
	\sa m_bRestart, SetRestart, RestartWindow. */
	inline GLboolean IsRestart()					{ return m_bRestart; }

	/// Metoda zwracaj¹ca strukturê SSettingFile przechowuj¹c¹ ustawienia programu.
	/** \return Referencja na strukturê SSettingFile. */
	inline SSettingFile& SettingFile()				{ return m_sSettingFile; }

	/// Metoda wczytuj¹ca plik konfiguracyjny.
	/*! Zwszelkia konfiguracja wczytana z pliku zostanie zapisana do struktury CWindowData::m_sSettingFile.
	\return \p GL_TRUE w przypadku powodzenia wczytania pliku konfiguracyjnego. \p GL_FALSE je¿eli wyst¹pi³ jakiœ b³¹d.
	\sa SaveSettingFile, SETTING_FILE_NAME, m_sSettingFile. */
	GLboolean LoadSettingFile();

	/// Metoda zapisuj¹ca plik konfiguracyjny.
	/*! Do pliku konfiguracyjnego zostanie zapisana struktura CWindowData::m_sSettingFile.
	\param[in] bDefaultSave Je¿eli podamy \p GL_TRUE to zostan¹ zapisane ustawienia domyœlne
	a nie rzeczywiste jakie s¹ aktualnie przechowywane w CWindowData::m_sSettingFile. Ustawienia
	domyœlne zapisywane s¹ tylko przy pierwszym uruchomieniu aplikacji. Dalej nale¿y
	zapisywaæ ustawienia rzeczywiste czyli skorzystaæ w domyœlnej wartoœci parametru: \p GL_FALSE.
	\return \p GL_TRUE w przypadku powodzenia zapisu pliku konfiguracyjnego. \p GL_FALSE je¿eli wyst¹pi³ jakiœ b³¹d.
	\sa LoadSettingFile, SETTING_FILE_NAME, m_sSettingFile. */
	GLboolean SaveSettingFile( GLboolean bDefaultSave = GL_FALSE );

	GLboolean m_bCharModeKeyboard; ///< Je¿eli \p GL_TRUE to komunikaty WM_KEYDOWN maj¹ byæ ignorowane, bo wtedy dzia³a WM_CHAR.

	/// Metoda sprawdzaj¹ca czy program zosta³ uruchomiony z danym parametrem.
	/*! \param[in] lpParam Tekst jaki chcemy sprawdziæ, czy zosta³ zawarty w parametrze programu.
	\return \p GL_TRUE je¿eli lpParam zostanie znaleziony w parametrze uruchomionym wraz z programem. W przeciwnym
	wypadku \p GL_FALSE. */
	GLboolean IsCommandLine( LPCTSTR lpParam );

	/// Pobranie aktualnej iloœci sampli dla antyaliasingu.
	/*! \return Aktualna iloœæ sampli dla anyaliasingu.
	\sa CARBMultisample, SetAntialiasingSamples, m_pARBMultisample. */
	inline GLint GetAntialiasingSamples() { return m_pARBMultisample->GetSamples(); }

	/// Ustawienie aktualnej iloœci sampli dla antyaliasingu.
	/*! \param[in] iSamples Iloœæ sampli jak¹ chcemy ustawiæ.
	\param[in] bRestart Czy od razu restartowaæ okno? Je¿eli podamy \p GL_TRUE to okno aplikacji zostanie zrestartowane.
	\sa CARBMultisample, GetAntialiasingSamples, m_pARBMultisample. */
	GLvoid SetAntialiasingSamples( GLint iSamples, GLboolean bRestart = GL_FALSE );

	//! Przeskalowanie okna OpenGL.
	/*! \param[in] fViewAngle K¹t pola widzenia przekazywany do funkcji \a gluPerspective. */
	GLvoid ReSizeGLScene( GLfloat fViewAngle = 45.0f );

	/// Przeskalowanie okna OpenGL.
	/*! \param[in] iWidth Szerokoœæ okna - rozdzielczoœæ X.
	\param[in] iHeight Wysokoœæ okna - rozdzielczoœæ Y.
	\param[in] fViewAngle K¹t pola widzenia przekazywany do funkcji \a gluPerspective. */
	GLvoid ReSizeGLScene( GLsizei iWidth, GLsizei iHeight, GLfloat fViewAngle = 45.0f );

	/// \name Gettery dla danych obiektów
	//@{
	inline CLogger*				GetLogger()		{ return m_pLogger; }
	inline CMultiLanguage*		GetMultiLang()	{ return m_pMultiLang; }
	inline HDC					GetHDC()		{ return m_hDC; }
	inline HINSTANCE			GetHINSTANCE()	{ return m_hInstance; }
	inline HWND					GetHWND()		{ return m_hWnd; }
	inline HGLRC				GetHRC()		{ return m_hRC; }
	//@}

	/// \name Settery i gettery dla podwójnego restartu
	//@{ 
	inline GLvoid		SetDoubleRestart( GLboolean bParam )	{ m_bDoubleRestart = bParam; }
	inline GLboolean	IsDoubleRestart()						{ return m_bDoubleRestart; }
	inline GLvoid		SetResDoubleRestartX( GLint iResX )		{ m_iResDoubleRestartX = iResX; }
	inline GLint		GetResDoubleRestartX()					{ return m_iResDoubleRestartX; }
	inline GLvoid		SetResDoubleRestartY( GLint iResY )		{ m_iResDoubleRestartY = iResY; }
	inline GLint		GetResDoubleRestartY()					{ return m_iResDoubleRestartY; }
	//@}

	/// \name Settery i gettery dla ró¿nych pól
	//@{ 
	inline GLboolean IsUseAntialiasing()	{ return m_bUseAntialiasing; }
	inline GLvoid SetUseAntialiasing( GLboolean bAntialiasing ) { m_bUseAntialiasing = bAntialiasing; }
	inline GLfloat GetBkColorR()			{ return m_fBkColor[ 0 ]; }
	inline GLfloat GetBkColorG()			{ return m_fBkColor[ 1 ]; }
	inline GLfloat GetBkColorB()			{ return m_fBkColor[ 2 ]; }
	inline GLfloat GetBkColorA()			{ return m_fBkColor[ 3 ]; }
	inline GLubyte GetZBufferBits()			{ return m_ubZBufferBits; }
	inline GLint GetDesktopWidth()			{ return m_iDesktopWidth; }
	inline GLint GetDesktopHeight()			{ return m_iDesktopHeight; }
	//@}

protected:
	/// Konstruktor.
	/*! Konstruktor. */
	CWindowData();

	HGLRC			m_hRC;				///< Sta³y kontekst renderowania.
	HDC				m_hDC;				///< Prywatny kontekst urz¹dzenia GDI.
	HWND			m_hWnd;				///< Przechowuje uchwyt okna.
	HINSTANCE		m_hInstance;		///< Przechowuje instancjê aplikacji.

	SSettingFile	m_sSettingFile;		///< Struktura przechowuj¹ca wczelk¹ konfiguracjê programu.
	
	/// \name WskaŸniki na wszelkie obiekty jakie s¹ tworzone w CWindowData.
	//@{
	CLogger			*m_pLogger;			///< WskaŸnik na obiekt klasy CLogger.
	CARBMultisample	*m_pARBMultisample;	///< WskaŸnik na obiekt klasy CARBMultisample.
	CMultiLanguage	*m_pMultiLang;		///< WskaŸnik na obiekt klasy CMultiLanguage.
	//@}
	WNDPROC			*m_pWndProc;			///< WskaŸnik na funkcjê \ref WndProc.

	GLboolean m_bRestart;	///< Czy okno bêdzie restartowane. Zmienna wykorzystywana w g³ównej pêtli programu.

	TCHAR *m_lpTitle;		///< Tytu³ okna aplikacji.
	TCHAR *m_lpClassName;	///< Nazwa klasy aplikacji.

	GLint m_iIcon;			///< ID ikony aplikacji.
	GLint m_iCursor;		///< ID kursora aplikacji.

	//dla podwójnego restartu - naprawa bug 12
	GLboolean m_bDoubleRestart; ///< Czy wymagany podwójny restart okna. 
	GLint m_iResDoubleRestartX;	///< Oczekiwana szerokoœæ (rozdzielczoœæ X) okna aplikacji po podwójnym restarcie.
	GLint m_iResDoubleRestartY; ///< Oczekiwana wysokoœæ (rozdzielczoœæ Y) okna aplikacji po podwójnym restarcie.

	GLfloat m_fBkColor[ 4 ];			///< Kolor t³a RGB + alfa.
	GLuint m_uiTimer;					///< Czêstotliwoœæ tikania timera w milisekundach. Domyœlnie 1000, je¿eli ustawimy na 0 to timer nieaktywny.
	GLboolean m_bUseAntialiasing;		///< Czy aktywny pe³noekranowy antyaliasing.
	GLboolean m_bShowMouseCursor;		///< Czy kursor myszy ma byæ pokazywany - tylko gdy fullscreen.
	GLubyte m_ubBitsPerPixel;			///< G³êbia kolorów pulpitu.
	GLubyte m_ubZBufferBits;			///< Bufor g³êbi.
	GLint m_iDesktopWidth;				///< Szerokoœæ pulpitu - rozdzielczoœæ X.
	GLint m_iDesktopHeight;				///< Wysokoœæ pulpitu - rozdzielczoœæ Y.

	/// \name Metody wywo³ywane przy tworzeniu okna.
	//@{ 

	/// Metoda pobieraj¹ca instancjê naszego programu.
	/*! Metoda ustawia uchwyt CWindowData::m_hInstance instancj¹ naszej aplikacji.
	\return \p GL_TRUE w przypadku powodzenia pobrania instancji. \p GL_FALSE w przypadku wyst¹pienia jakiegoœ b³êdu. */
	GLboolean CreateInstance();

	/// \brief Metoda tworz¹ca oraz rejestruj¹ca klasê okna naszej aplikacji.
	/*! \return \p GL_TRUE w przypadku powodzenia. \p GL_FALSE w przypadku wyst¹pienia jakiegoœ b³êdu. */
	GLboolean RegisterMyClass();

	/// Metoda ustawiaj¹ca tryb urz¹dzenia graficznego na tryb pe³noekranowy.
	/*! Próba ustawienia trybu pe³noekranowego bêdzie realizowana tylko wtedy je¿eli flaga SSettingFile::bFullScreen
	ustawiona bêdzie na \p GL_TRUE. \n
	Je¿eli próba ustawienia urz¹dzenia graficznego na tryb pe³noekranowy zakoñczy siê niepowodzeniem, aplikacja
	zostanie uruchomiona w trybie okna oraz flaga SSettingFile::bFullScreen zostanie ustawiona na \p GL_FALSE. */
	GLvoid SetDeviceOnFullscreen();

	/// Metoda tworz¹ca okno naszej aplikacji.
	/*! Metoda ustawia uchwyt CWindowData::m_hWnd.
	\return \p GL_TRUE w przypadku powodzenia utworzenia okna. \p GL_FALSE w przypadku wyst¹pienia jakiegoœ b³êdu. */
	GLboolean CreateMyWindow();

	/// Metoda pobieraj¹ca kontekst urz¹dzenia naszej aplikacji.
	/*! Metoda ustawia zmienn¹ CWindowData::m_hDC.
	\return \p GL_TRUE w przypadku powodzenia pobrania kontekstu urz¹dzenia. \p GL_FALSE w przypadku wyst¹pienia
	jakiegoœ b³êdu. */
	GLboolean GetDeviceContent();

	/// Metoda ustawiaj¹ca format piksela.
	/*! Je¿eli aplikacja wykorzystuje pe³noekranowy antyaliasing to format piksela zostanie wybrany z klasy
	CARBMultisample. Je¿eli nie to format piksela zostanie wybrany z kontekstu urz¹dzenia.
	\return \p GL_TRUE w przypadku znalezienia pasuj¹cego formatu piksela. W przeciwnym wypadku \p GL_FALSE. */
	GLboolean SetPixelFormat();

	/// Metoda tworz¹ca kontekst renderowania.
	/*! Metoda ustawia zmienn¹ CWindowData::m_hRC.
	\return \p GL_TRUE w przypadku powodzenia. \p GL_FALSE w przypadku wyst¹pienia jakiegoœ b³êdu. */
	GLboolean CreateRenderingContext();
	//@}
};