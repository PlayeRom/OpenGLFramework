/*! \file
	\brief Plik nag³ówkowy dla klasy CProfiler.
*/
#pragma once

#define MAX_SIZE_OF_STRING	512

class CLogger;

/// Klasa profilera.
/** Za pomoc¹ profilera mo¿emy zmierzyæ czas wykonywania siê poszczególnych funkcji. Dziêki temu mo¿emy
zdiagnozowaæ, które metody spowalniaj¹ nam program i które nale¿a³oby zoptymalizowaæ. Zadanie pomiaru czasu
wykonywania siê kodu spoczywa w³aœnie na klasie CProfiler. Klasa CProfiler jest singletonem, jej instancja jest
ju¿ we frameworku tworzona i usuwana automatycznie, wiêc naszym zadaniem jest tylko wywo³ywanie pary metod
CProfiler::Begin oraz CProfiler::End. Dla ³atwego wy³¹czenia u¿ycia profilera zaleca siê wywo³ywaæ powy¿sze metody
poprzez makra \ref PROFILER_BEGIN oraz \ref PROFILER_END po uprzednim zdefiniowaniu \ref _USE_PROFILER_ na 1. */
class CProfiler
{
public:
	/// Statyczna metoda za pomoc¹, której pobieramy wskaŸnik na obiekt klasy CProfiler. 
	/** Przy pierwszym wywo³aniu tej metody, jednoczeœnie zostanie stworzony obiekt klasy.
	\return WskaŸnik na obiekt CProfiler. */
	static CProfiler* GetInstance();

	/// Destruktor.
	/** Destruktor. */
	~CProfiler();

	/// Metoda rozpoczynaj¹ca pomiar czasu.
	/** Wywo³uj¹c t¹ metodê zawsze nale¿y wywo³aæ póŸniej metodê CProfiler::End.
	\param[in] lpComment Komentarz jaki zostanie zapisany w logu profilera. Najlepiej jest podaæ tu nazwê
	funkcji, któr¹ testujemy.
	\sa End. */
	GLvoid Begin( LPCTSTR lpComment );

	/// Metoda koñcz¹ca pomiar czasu.
	/** Wywo³anie tej metody zawsze musi byæ poprzedzone wywo³aniem metody CProfiler::Begin.
	\sa Begin. */
	GLvoid End();

	/// Motoda zapisuj¹ca do pliku zebrane informacje.
	/** Metoda ta musi byæ wywo³ana zawsze co klatkê w g³ównej pêtli gry. Zadniem tej metody jest zapisanie wszelkich
	zebranych informacji do pliku. */
	GLvoid Flush();

private:
	/// Struktura zapamiêtuj¹ca wszelkie wywo³ania metody CProfiler::Begin.
	/** Struktura przechowuje takie informacje jak aktualny czas wywo³ania metody CProfiler::Begin oraz
	komentarz jaki zosta³ podany jako parametr metody CProfiler::Begin. */
	struct SBegin
	{
		GLboolean bIsUse;						///< Czy jest w u¿yciu?
		unsigned long ulBeginMs;				///< Czas w milisekundach w chwili wywo³ania metody CProfiler::Begin.
		TCHAR lpComment[ MAX_SIZE_OF_STRING ];	///< Komentarz jaki zosta³ podany przy wywo³aniu metody CProfiler::Begin.
	};

	/// Struktura przechowuj¹ca wszelkie informacje jakie zostan¹ zapisaane do pliku.
	/** Struktura przechowuje takie informacje jak komentarz wywo³ania metody CProfiler::Begin, czas jaki
	up³yn¹³ miêdzy wywo³aniami CProfiler::Begin a CProfiler::End oraz iloœæ wciêæ dla lepszej czytelnoœci loga. */
	struct SSaveToLog
	{
		GLboolean bIsUse;						///< Czy jest w u¿yciu?
		GLint iIndentNumber;					///< Iloœæ wciêæ.
		TCHAR lpComment[ MAX_SIZE_OF_STRING ];	///< Komentarz jaki zosta³ podany przy wywo³aniu metody CProfiler::Begin.
		unsigned long ulTimeMs;					///< Czas w milisekundach jaki up³yn¹³ miêdzy wywo³aniem CProfiler::Begin a CProfiler::End.
	};

	static std::auto_ptr<CProfiler> m_pProfiler; ///< Instancja klasy.

	/// Konstruktor.
	/** Konstruktor. */
	CProfiler();

	/// WskaŸnik na obiekt klasy CLogger.
	/** Obiekt za pomoc¹ którego wykonywany jest zapis do pliku loga profilera. */
	CLogger *m_pLogger;

	/// Metoda zwracaj¹ca aktualny czas w milisekundach.
	/*! \return Aktualny czas (w milisekundach) jaki up³yn¹³ od uruchomienia systemu.	*/
	unsigned long GetTimeInMs();

	/// Metoda zwracaj¹ca iloœæ elementów tablicy CProfiler::m_aBeginMs.
	/** \return Iloœæ elementów tablicy CProfiler::m_aBeginMs. */
	GLint GetBeginMsSize() { return static_cast<GLint>( m_aBeginMs.size() ); }

	/// Tablica obiektów struktury SBegin.
	/** Tablica przechowuj¹ca czas wywo³ania ka¿dej metody CProfiler::Begin. */
	std::vector<SBegin> m_aBeginMs;

	/// Zmienna przechowuj¹ca aktualn¹ iloœæ wciêæ - dla formatowania loga.
	GLint m_iIndentCounter;

	/// Tablica obiektów struktury SSaveToLog.
	/** Tablica przechowuj¹ca rekordy jakie zostan¹ zapisane do pliku. */
	std::vector<SSaveToLog> m_aSaveToLog;
};
