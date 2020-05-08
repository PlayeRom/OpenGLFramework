/*! \file
	\brief Modu³ makr.
*/

#ifndef _DEFINE_H_
#define _DEFINE_H_

/// Nazwa frameworka
#define FRAME_NAME _T(" PlayeRom OpenGLFramework v.1.05 ")

/// Nazwa gry/programu.
#define GAME_NAME _T("OpenGLFramework")
/// Adres strony.
#define HOMEPAGE _T("www.your.homepage.com")

/// \name Wersja gry - ka¿dy cz³on mo¿e byæ w zakresie od 0 do 255.
//@{
#define MAJOR_VERSION 			0
#define MINOR_VERSION 			1
#define RELEASE_VERSION 		1
//@}

/// \name Wersja pliku konfiguracyjnego - ka¿dy cz³on mo¿e byæ w zakresie od 0 do 255.
//@{
#define MAJOR_VERSION_FILE 		1
#define MINOR_VERSION_FILE 		0
#define RELEASE_VERSION_FILE 	0
//@}

/// Nazwa pliku konfiguracyjnego.
#define SETTING_FILE_NAME "setting.cfg"

/// Maksymalna d³ugoœæ tekstu w jednej linii dla konsoli.
#define CONSOLE_MSG_LENGTH 128

#define MAX_3DS_NAME 256

//#define _3DSLOADER_

/// Sta³a PI.
#define PI 3.141592f//6535897932384626433832795f
/// Makro przekszta³cenia stopni na radiany.
#define DEG2RAD(x) ((x*PI)/180.0f)

// UWAGA! U¿ycie, b¹dŸ nie UNICODE ustawiamy we w³aœciwoœciach projektu:
// Alt+F7 -> Configuration Properties -> General -> Character set

//################################################################
/// Jakiej biblioteki dŸwiêkowej u¿yæ?
/*! Je¿eli makro to zostanie ustawione na 1 to do odgrywania dŸwiêków i muzyki u¿yta zostanie biblioteka OpenAL.
Je¿eli ustawimy to makro na 0 to u¿yta zostanie biblioteka FMOD. Domyœlnie makro _USE_SOUND_OPENAL_ jest zdefiniowane. */
#define _USE_SOUND_OPENAL_	1

/// Czy w OpenAL u¿ywaæ formatu pliku muzycznego OGG?
/** Ustawiaj¹c to makro na 1 udostêpnimy mo¿liwoœæ odgrywania plików OGG poprzez bibliotekê OpenAL. Oznacza to,
¿e makro \ref _USE_SOUND_OPENAL_ tak¿e musi byæ ustawione na 1. */
#define _USE_OGG_		1
//################################################################

/// Zapytanie o tryb pe³noekranowy.
/*! Zdefiniowanie tego makra spowoduje, i¿ w trakcie uruchamiania naszej aplikacji, zostaniemy zapytani czy program
ma siê uruchomiæ w trybie pe³noekranowym czy te¿ nie. Je¿eli makro jest niezdefiniowane (i tak jest domyœlnie) – to
program zawsze uruchomi siê na pe³nym ekranie. */
#define _QUESTION_FULLSCREEN_	1

/// Czy pobraæ systemow¹ rozdzielczoœæ i kolory?
/*! Je¿eli makro to jest zdefiniowane to w trakcie inicjalizacji okna, rozdzielczoœæ i glêbia kolorów zostan¹ pobrane
z ustawien systemowych pulpitu. Je¿eli chcemy aby u¿yte by³y wartoœci zdefiniowane przez nas (patrz konstruktor klasy
CWindowData), makro nale¿y dezaktywowaæ. */
#define _USE_SYSTEM_RES_AND_BITSPERPIXEL_	1

#if !_USE_SYSTEM_RES_AND_BITSPERPIXEL_
	// Tutaj ustaw w³asn¹, sta³¹ rozdzielczoœæ ekranu oraz g³êbiê kolorów
	#define _SCREEN_WIDTH_	800
	#define _SCREEN_HEIGHT_	600
	#define _SCREEN_BITS_	32
#endif

/// Czy u¿yæ trybu tylko 2D?
/*! Zdefiniuj poni¿sze makro je¿eli twoja gra u¿ywaæ ma jedynie trybu 2D.
Je¿eli gra ma wykorzystywaæ grafikê 3D, makro nale¿y dezaktywowaæ. */
#define _USE_2D_ONLY_	0

// Wy³¹czenie warningów o niezgodnoœci z ISO
//#define _CRT_SECURE_NO_DEPRECATE
//#define _CRT_NON_CONFORMING_SWPRINTFS

//################################################################
/// Jakiej czcionki u¿yæ w konsoli? 
/*! Jakiej czcionki (sposobu rysowania) u¿yæ w konsoli?
Je¿eli oba makra dezaktywowane to u¿yta zostajnie CSDLFont. Zalecane u¿ycie CSDLFont. */
#define _USE_CONSOLE_WITH_MY_FONT_		0
#define _USE_CONSOLE_WITH_BITMAP_FONT_	0

// Je¿eli zdefiniowane oba _USE_CONSOLE_WITH_MY_FONT_ i _USE_CONSOLE_WITH_BITMAP_FONT_ to u¿ywamy CSDLFont
#if _USE_CONSOLE_WITH_MY_FONT_
	#if _USE_CONSOLE_WITH_BITMAP_FONT_
		#undef _USE_CONSOLE_WITH_MY_FONT_
		#undef _USE_CONSOLE_WITH_BITMAP_FONT_
	#endif
#endif
//################################################################

/// Czy u¿yæ profilera?
/** Wszelkie wywo³ania funkcji CProfiler::Begin oraz CProfiler::End dobrze jest wywo³ywaæ za pomoc¹ makr
PROFILER_BEGIN oraz PROFILER_END. Wówczas przypisuj¹c _USE_PROFILER_ 1 lub 0 w ³atwy sposób bêdziemy mogli aktywowaæ
i dezaktywowaæ profiler w aplikacji. */
#define _USE_PROFILER_	0

#if _USE_PROFILER_
	#define PROFILER_BEGIN( strDesc )	CProfiler::GetInstance()->Begin( strDesc );
	#define PROFILER_END()				CProfiler::GetInstance()->End();
	#define PROFILER_FLUSH()			CProfiler::GetInstance()->Flush();
#else
	#define PROFILER_BEGIN( strDesc )
	#define PROFILER_END()
	#define PROFILER_FLUSH()
#endif

/// Maksymalna odleg³oœæ tylnej p³aszczyzny obcinania.
#define MAX_DISTANCE_VIEW 5000.0f

#define DEFAULT_SOUND_VOLUME			90
#define DEFAULT_MUSIC_VOLUME			60

#endif //_DEFINE_H_
