/** \file
	\brief Plik nag³ówkowy dla klasy CLogger.
*/
#pragma once

/// Domyœlny, pierwszy cz³on nazwy pliku logger'a - bez rozszerzenia.
#define LOG_FILE_NAME _T("Log")

/// \name Makra u³atwiaj¹ce u¿ywanie metod zapisu do pliku.
//@{

/// Makro u³atwiaj¹ce dostêp do metody CLogger::LOG.
/*! Makro to mo¿emy wywo³aæ w dowolnym miejscu kodu.
\param[in] msg Dowolny tekst jaki chcemy zapisaæ do pliku. */
#define __LOG( msg ) CWindowData::GetInstance()->GetLogger()->LOG( msg )

/// Makro u³atwiaj¹ce dostêp do metody CLogger::LOG, z parametrami dla formatowania.
/*! Makro to mo¿emy wywo³aæ w dowolnym miejscu kodu.
\param[in] fmt Dowolny tekst jaki chcemy zapisaæ do pliku.
\param[in] ... Dodatkowe parametry dla formatowania. */
#define __LOG_FORMATTED( fmt, ... ) CWindowData::GetInstance()->GetLogger()->LOG( fmt, __VA_ARGS__ )

/// Makro u³atwiaj¹ce dostêp do metody CLogger::LOGGetLastError.
/*! Makro to mo¿emy wywo³aæ w dowolnym miejscu kodu.
\param[in] msg Dowolny tekst dziêki, któremu bêdziemy wiedzieli co wywo³a³o ewentualny b³¹d. */
#define __LOG_LAST_ERROR( msg ) CWindowData::GetInstance()->GetLogger()->LOGLastError( msg )
//@}

/// Klasa loggera.
/*! Jest to klasa odpowiedzialna za zapisywanie do pliku przekazywanych jej parametrów. Dziêki temu w fazie
testowania naszej gry bêdziemy mogli rejestrowaæ to co siê dzieje w programie. Obiekt klasy CLogger zawsze
jest tworzony w CWindowData, jednak domyœlnie logowanie jest wy³¹czone. Aby je uaktywniæ nale¿y uruchomiæ
program z parametrem \a /logger. */
class CLogger
{
public:
	/// Konstruktor.
	/*! \param[in] lpFileName Nazwa pliku (a raczej pierwszy cz³on nazwy pliku), do którego bêd¹ zapisywane informacje.
	Pe³na nazwa pliku zawiera nastêpuj¹cy format:
	\n lpFileName – YYYY.MM.DD – HH.MM.SS.log
	\n gdzie:
	\n lpFileName – to opisywany tu parametr,
	\n YYYY.MM.DD – data (rok, miesi¹c i dzieñ) utworzenia pliku,
	\n HH.MM.SS – czas utworzenia pliku, tj. godzina, minuty i sekundy,
	\n .log – rozszerzenia pliku. */
	CLogger( LPCTSTR lpFileName );

	/// Destruktor.
	/*! Destruktor. */
	virtual ~CLogger();

	/// Metoda otwieraj¹ca plik loga.
	/*! Metoda ta jest ju¿ wywo³ywana w CWindowData::Initialization, ale pod warunkiem, ¿e program zosta³
	uruchomiony z parametrem \a /logger. \n
	Metoda ta w trakcie otwierania nowego pliku mo¿e jednoczeœnie usun¹æ stare pliki logger-a. To czy stare
	pliki bêd¹ usuwane zale¿y od ustawienia flagi CLogger:m_bClearOldLog. \n
	Wywo³anie tej metody jednoczeœnie ukatywnia logger-a, patrz CLogger::SetActive.
	\return \p GL_TRUE w przypadku powodzenia otwarcia pliku loggera. \p GL_FALSE w przypadku wyst¹pienia jakiegoœ b³êdu. */
	GLboolean Open();

	/// Metoda uaktywniaj¹ca b¹dŸ dezaktywuj¹ca logger.
	/*! \param[in] bMode Podaj¹c \p GL_TRUE uaktywnimy logger. Je¿eli logger zostanie uaktywniony bez wczeœniejszego
	wywo³ania metody CLogger::Open to podczas wywo³ania metody zapisuj¹cej do pliku (np. CLogger::LOG),
	automatycznie zostanie wywo³ana tak¿e motoda CLogger::Open. Czyli log zostanie utworzony. \n
	Podaj¹c \p GL_FALSE logger stanie siê nieaktywny, co oznacza, ¿e pomimo wywo³ania metody zapisuj¹cej do pliku
	(np. CLogger::LOG), do pliku nic nie zostanie zapisane.
	\sa IsActive, m_bActive. */
	GLvoid SetActive( GLboolean bMode );

	/// Metoda sprawdzaj¹ca czy logger jest aktywny.
	/*! \return \p GL_TRUE je¿eli logger jest aktywny. W przeciwnym wypadku \p GL_FALSE.
	\sa SetActive, m_bActive. */
	inline GLboolean IsActive() { return m_bActive; }

	/// Metoda usuwaj¹ca stare pliki log.
	GLvoid ClearOldLog();

	/// Metoda sprawdzaj¹ca stan flagi CLogger::m_bClearOldLog.
	inline GLboolean IsClearOldLog() { return m_bClearOldLog; }

	/// Metoda ustawiaj¹ca iloœæ tabulatorów przed ka¿ym z wpisów.
	/*! Za pomoc¹ tej metody mo¿emy tworzyæ wciêcia przed ka¿d¹ linijk¹ zapisywan¹ do pliku. Metodê tê nale¿y wywo³aæ
	przed wywo³aniem jakiejkolwiek metody zapisuj¹cej do pliku (np. CLogger::LOG), podaj¹c odpowiedni¹ liczbê
	tabulatorów. Teraz je¿eli wywo³amy jed¹ z funkcji zapisuj¹cej do pliku to nasza linia tekstu zostanie porzedzona
	odpwiedni¹ iloœci¹ wciêæ. W ka¿dej metodzie zapisuj¹cej do pliku, liczba tabulatorów jest zerowana.
	\param[in] iTabNumber Liczba okreœlaj¹ca iloœæ tabulatorów - wielkoœæ wciêcia jak¹ chcemy utworzyæ dla danej
	linii tekstu, dla lepszej czytelnoœci log-a.
	\sa m_iTabNumber. */
	inline GLvoid SetTabulatorNumber( GLint iTabNumber ) { m_iTabNumber = iTabNumber; }

	/// \name Metody zapisuj¹ce do pliku.
	//@{

	/// Metoda zapisuj¹ca do pliku, liniê tekstu (\p UNICODE), z mo¿liwoœci¹ formatowania.
	/*! Za pomoc¹ tej metody mo¿emy zapisaæ do pliku dowolny ci¹g znaków. Otwarta lista parametrów umo¿liwia
	formatowanie tekstu, dziêki czemu mo¿emy zapisywaæ wartoœci zmiennych, itp. \n
	Tekst zostanie zapisany do pliku tylko wtedy je¿eli logger jest aktywny, tj. je¿eli flaga CLogger::m_bActive
	ustawiona jest na \p GL_TRUE. \n
	Tekst zawsze bêdzie zapisywany w pliku od nowej linii.
	\param[in] fmt Dowolny tekst jaki chcemy zapisaæ do pliku. Tekst mo¿e zawieraæ znaki formatowania (%d, %s, %c itp.).
	\param[in] ... Opcjonalne parametry dla formatowania. */
	GLvoid LOG( const wchar_t *fmt, ... );

	/// Metoda zapisuj¹ca do pliku, liniê tekstu, z mo¿liwoœci¹ formatowania.
	/*! Za pomoc¹ tej metody mo¿emy zapisaæ do pliku dowolny ci¹g znaków. Otwarta lista parametrów umo¿liwia
	formatowanie tekstu, dziêki czemu mo¿emy zapisywaæ wartoœci zmiennych, itp. \n
	Tekst zostanie zapisany do pliku tylko wtedy je¿eli logger jest aktywny, tj. je¿eli flaga CLogger::m_bActive
	ustawiona jest na \p GL_TRUE. \n
	Tekst zawsze bêdzie zapisywany w pliku od nowej linii.
	\param[in] fmt Dowolny tekst jaki chcemy zapisaæ do pliku. Tekst mo¿e zawieraæ znaki formatowania (%d, %s, %c itp.).
	\param[in] ... Opcjonalne parametry dla formatowania. */
	GLvoid LOG( const char *fmt, ... );

	/// Metoda zapisuj¹ca do pliku pojedynczy znak \p UNICODE.
	/*! Znak zostanie zapisany do pliku tylko wtedy je¿eli logger jest aktywny, tj. je¿eli flaga CLogger::m_bActive
	ustawiona jest na \p GL_TRUE. \n
	Znak zawsze bêdzie dopisywany na koniec pliku.
	\param[in] cChar Znak jaki chemy zapisaæ. */
	GLvoid LOGFreeChar( wchar_t cChar );

	/// Metoda zapisuj¹ca do pliku pojedynczy znak.
	/*! Znak zostanie zapisany do pliku tylko wtedy je¿eli logger jest aktywny, tj. je¿eli flaga CLogger::m_bActive
	ustawiona jest na \p GL_TRUE. \n
	Znak zawsze bêdzie dopisywany na koniec pliku.
	\param[in] cChar Znak jaki chemy zapisaæ. */
	GLvoid LOGFreeChar( char cChar );

	/// Metoda zapisuj¹ca do pliku, liniê teksu, z ostatnio wywo³anym b³êdem WinAPI.
	/*! Metoda ta zapisuje do pliku opis b³êdu, zwróconego przez funkcjê WinAPI: \a GetLastError.
	\param[in] lpWhatCallError Dowolny tekst dziêki, któremu bêdziemy wiedzieli co wywo³a³o b³¹d. */
	GLvoid LOGLastError( LPCTSTR lpWhatCallError = _T("") );
	//@}

private:
	/// Metoda usuwaj¹ca stare pliki logger-a.
	/*! Metoda usuwa wszystkie pliku z rozszerzeniem \a .log znajduj¹ce siê w bierz¹cym katalogu. Metoda ta jest
	wywo³ywana w CLogger::Open pod warunkiem, ¿e flaga CLogger::m_bClearOldLog ustawiona jest na \p GL_TRUE.
	\param[in] lpCurDir Nazwa katalogu, z którego zostan¹ usuniête pliki logger-a.
	\sa m_bClearOldLog, ClearOldLog, IsClearOldLog. */
	GLvoid DeleteLogFiles( LPCTSTR lpCurDir );

	/// Metoda zamykaj¹ca plik logger-a.
	/*! Metoda ta jest wywo³ywana w destruktorze klasy. */
	GLvoid Close();

	FILE *m_hFile;			///< Uchwyt do pliku log-a.
	GLboolean m_bActive;	///< Flaga okreœlaj¹ca czy logger jest aktywny.
	GLuint m_uiCounterLine;	///< Licznik aktualnie zapisanych linii tekstu do pliku.

	/// Flaga czyszczenia starych plików logger-a.
	/*! Zmienna wskazuj¹ca czy w trakcie wywo³ywania metody CLogger:Open, maj¹ byæ jednoczeœnie usuwane stare pliki logger-a.
	\sa ClearOldLog, IsClearOldLog. */
	GLboolean m_bClearOldLog;
	GLint m_iTabNumber;		///< Zmienna przechowuj¹ca aktualnie ustawion¹ iloœæ tabulatorów.

	/// Przechowuje pe³n¹ nazwê pliku loga.
	TCHAR m_cFimeNameWithData[ 256 ];
};
