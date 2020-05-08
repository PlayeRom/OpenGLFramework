/** \file
	\brief Plik nag³ówkowy dla klasy CMultiLanguage.
*/
#pragma once

#include "../OpenGLFramework/WindowData.h"
#include "../OpenGLFramework/SingletonBase.h"

/// Makro pobrania wskazanej linii tekstu z pliku jêzykowego.
/** Makro to wywo³uje metodê CMultiLanguage::GetLangText, zwracaj¹c¹ ³añcuch znaków z pliku jêzykowego.
\param[in] x Numer linii z pliku tekstowego, który chcemy pobraæ, np. w celu wyœwietlenia. Numery linii
liczone s¹ od 0.
\return £añcuch znaków zapisany w podanej linii pliku \a *.\a lng. */
#define LNG_LINE( x )	CMultiLanguage::GetInstance()->GetLangText( x )

/// Klasa wspieraj¹ca wielojêzycznoœæ.
/*! Zadanie tej klasy polega na wczytaniu pliku \a lng z linijkami tekstów przet³umaczonych na dany jêzyk. Nastêpnie
zwracania odpowiednich linii tekstu z wykorzystywanego jêzyka. Odpowiedni plik \a *.\a lng mo¿e byæ wczytany na
podstawie pliku \a languages_def.xml lub bezpoœrednio gdy nie wykorzystujemy pliku XML. */
class CMultiLanguage : public CSingletonBase <CMultiLanguage>
{
public:
	/// Statyczna metoda tworz¹ca obiekt klasy CMultiLanguage. 
	/*! CMultiLanguage jest singletonem, wiêc je¿eli wywo³amy tê metodê po raz drugi to zwróci ona wskaŸnik na obiekt
	ju¿ istniej¹cy, utworzony wczeœniej.
	\return WskaŸnik na obiekt CMultiLanguage. */
	static CMultiLanguage* Construct();

	/// Destruktor.
	/*! Destruktor. */
	virtual ~CMultiLanguage();

	/// \name Interfejs uniwersalny, niezale¿nie czy u¿ywamy pliku XML czy nie.
	//@{

	/// Metoda inicjalizuj¹ca wsparcie wielojêzycznoœci.
	/*! Metoda ta jest ju¿ wywo³ywana w CWindowData::Initialization. */
	GLvoid InitializeLanguage();

	/// Metoda wczytuj¹ca podany plik jêzykowy.
	/*! Metoda ta jest ju¿ wywo³ywana w CGameControl::Initialization.
	\param[in] iIndex Indeks pliku jêzykowego jaki chcemy wczytaæ. Najlepiej podaæ tutaj pole
	CWindowData::SSettingFile::iIndexLanguage.
	\return \p GL_TRUE w przypadku powodzenia wczytania pliku. \p GL_FALSE w przypadku wyst¹pienia jakiegoœ b³êdu. */
	GLboolean LoadLanguage(GLint iIndex);

	/// Metoda zwracaj¹ca wskazan¹ liniê tekstu z wczeœniej wczytanego pliku jêzykowego.
	/*! Metoda ta s³u¿y do pobrania konkretnej linii tekstu zapisanej w pliku jêzykowym (\a *.\a lng).
	\param[in] iLineNumber Numer linii z pliku tekstowego, który chcemy pobraæ, np. w celu wyœwietlenia. Numery linii
	liczone s¹ od 0.
	\return £añcuch znaków zapisany w podanej linii pliku \a *.\a lng. */
	LPTSTR GetLangText(GLint iLineNumber);

	/// Metoda zwracaj¹ca iloœæ plików jêzykowych.
	/*! \return Iloœæ plików jêzykowych zawartych w katalogu \a languages lub opisanych w pliku
	\a languages/languages_def.xml. */
	GLint GetLangFileNumber();

	/// Metoda zwracaj¹ca nazwê pliku jêzykowego.
	/*! Za pomoc¹ tej metody mo¿emy sprawdziæ jaki plik \a *.\a lng jest przypisany dla danego indeksu. 
	\param[in] iIndex Indeks pliku jêzykowego.
	\return Nazwa pliku jêzykowego. */
	LPTSTR GetLangFileName(GLint iIndex);
	//@}

	/// \name Interfejs tylko je¿eli wykorzystywany jest plik XML.
	//@{

	/// Metoda zwracaj¹ca nazwê jêzyka dla danego jêzyka.
	/*! Metoda zwraca wartoœæ parametru \p name, znacznika \p \<language\> dla indeksu jêzyka zawartego w polu
	CWindowData::SSettingFile::iIndexLanguage.
	\return Nazwa jêzyka zdefiniowana w pliku \a languages_def.xml. W przypadku jakiegoœ b³êdu, np. nie u¿ywania
	pliku XML, zwrócony zostanie pusty ³añcuch znaków: \a "". */
	inline LPTSTR GetXMLLngName() {
		GLint iIndex = CWindowData::GetInstance()->SettingFile().iIndexLanguage;
		if( !m_bUseXML || ( iIndex < 0 || iIndex >= GetXMLLngDataSize() ) )
			return _T("");

		return m_aXMLLngData[ iIndex ].lpName;
	}

	/// Metoda zwracaj¹ca nazwê pliku czcionki (\a *.\a ttf).
	/*! Metoda zwraca wartoœæ parametru \p file, znacznika \p \<font\> dla indeksu jêzyka zawartego w polu
	CWindowData::SSettingFile::iIndexLanguage.
	\param[in] iIndexTTF Indeks pliku \a *.\a ttf zawartego w pliku \a languages_def.xml w znaczniku \p \<font\>.
	Indeks jest wartoœci¹ umown¹ liczon¹ od zera, pobieran¹ w kolejnoœci podawania znaczników \p \<font\>.
	\return Nazwa pliku czcionki zdefiniowana w pliku \a languages_def.xml. W przypadku jakiegoœ b³êdu, np. nie
	u¿ywania pliku XML, zwrócony zostanie pusty ³añcuch znaków: \a "". */
	inline char* GetXMLLngFileTTF(GLint iIndexTTF){
		GLint iIndex = CWindowData::GetInstance()->SettingFile().iIndexLanguage;
		if( !m_bUseXML || ( iIndex < 0 || iIndex >= GetXMLLngDataSize() ) )
			return "";
		if( iIndexTTF < 0 || iIndexTTF >= GetXMLLngDataTTFSize( iIndex ) )
			return "";

		return m_aXMLLngData[iIndex].aFileTTF[iIndexTTF];
	}
	//@}
private:
	/// Konstruktor.
	/*! Konstruktor. */
	CMultiLanguage();

	/// Struktura danych dla pliku XML.
	/*! Struktura ta przechowuje wartoœci parametrów znaczników opisanych w pliku \a languages_def.xml. */
	struct SXMLLngData
	{
		/// Nazwa jêzyka.
		/*! Przechowuje wartoœæ parametru \p name znacznika \p \<language\>. */
		TCHAR *lpName;

		/// Nazwa pliku (\a *.\a lng) zawieraj¹cego tekst.
		/*! Przechowuje wartoœæ parametru \p file znacznika \p \<language\>. */
		TCHAR *lpFileLNG;

		/// Tablica nazw czcionek.
		/*! Przechowuje wartoœci parametu \p file znaczników \p \<font\>. */
		std::vector<char*> aFileTTF;
	};

	/// Tablica przechowuj¹ca linie tekstu pliku jêzykowego (\a *.\a lng).
	/*! Tablica ta przechowuje linie tekstu aktualnie wczytanego pliku jêzykowego (\a *.\a lng). */
	std::vector<TCHAR*> m_aLineText;

	/// Metoda czyszcz¹ca tablicê CMultiLanguage::m_aLineText.
	GLvoid ClearLineText();

	/// Metoda zwracaj¹ca iloœæ linii tekstu pliku jêzykowego (\a *.\a lng).
	/*! \return Iloœæ linii tekstu zapisanych w pliku jêzykowym \a *.\a lng. Jest to rozmiar tablicy
	CMultiLanguage::m_aLineText. */
	inline GLint GetLineTextSize() { return static_cast<GLint>( m_aLineText.size() ); }

	/// Metoda wczytuj¹ca plik jêzykowy \a *.\a lng.
	/*! Metoda ta jest wywo³ywana bezpoœrednio z CMultiLanguage::LoadLanguage.
	\param[in] lpFileName Nazwa pliku jêzykowego \a *.\a lng. 
	\return \p GL_TRUE w przypadku powodzenia wczytania pliku. \p GL_FALSE w przypadku wyst¹pienia jakiegoœ b³êdu. */
	GLboolean LoadLanguageFile(LPCTSTR lpFileName);

	/// Metoda wczytuj¹ca plik jêzykowy \a *.\a lng z kodowaniem \p UNICODE.
	/*! Metoda ta jest wywo³ywana z CMultiLanguage::LoadLanguageFile. W metodzie tej tworzona jest tablica
	CMultiLanguage::m_aLineText.
	\param[in] f Uchwyt do otwartego pliku \a *.\a lng. 
	\return \p GL_TRUE w przypadku powodzenia wczytania pliku. \p GL_FALSE w przypadku wyst¹pienia jakiegoœ b³êdu. */
	GLboolean LoadUnicodeFile(FILE *f);

	/// Metoda wczytuj¹ca plik jêzykowy \a *.\a lng z kodowaniem \p ANSI.
	/*! Metoda ta jest wywo³ywana z CMultiLanguage::LoadLanguageFile. W metodzie tej tworzona jest tablica
	CMultiLanguage::m_aLineText.
	\param[in] f Uchwyt do otwartego pliku \a *.\a lng. 
	\return \p GL_TRUE w przypadku powodzenia wczytania pliku. \p GL_FALSE w przypadku wyst¹pienia jakiegoœ b³êdu. */
	GLboolean LoadAnsiFile(FILE *f);

	TCHAR m_cErrText[128]; ///< Pomocniczy tekst do zwracania informacji o b³êdzie.

	/// \name Metody u¿ywane tylko wtedy gdy nie wykorzystywany jest XML.
	//@{

	/// Metoda wyszukuj¹ca pliki \a *.\a lng.
	/*! Metoda ta przeszukuje katalog \a languages pod wzglêdem wystêpowania w nim plików z rozszerzeniem \a lng.
	Ka¿dy nazwa znalezionego pliku \a lng zapisywana jest w tablicy CMultiLanguage::m_aLanguageFileName. \n
	Metoda ta jest wywo³ywana w CMultiLanguage::InitializeLanguage pod warunkiem, ¿e nie istnieje plik 
	\a languages\\languages_def.xml. */
	GLvoid SearchLngFile();

	/// Metoda sprawdzaj¹ca rozszerzenie pliku.
	/*! Metoda ta sprawdza czy podana nazwa pliku posiada rozszerzenie \a lng.
	\param[in] lpPath Pe³na nazwa pliku wraz ze œcie¿k¹.
	\return \p GL_TRUE je¿eli nazwa koñczy siê rozszerzeniem \a lng. W przeciwnym wypadku \p GL_FALSE. */
	GLboolean CheckExtensionLNG(LPCTSTR lpPath);

	/// Metoda zwracaj¹ca iloœæ znalezionych plików \a lng.
	/*! \return Iloœæ elementów tablicy CMultiLanguage::m_aLanguageFileName. */
	inline GLint GetLngFileNameSize() { return static_cast<GLint>( m_aLanguageFileName.size() ); }

	/// Metoda zwracaj¹ca nazwê pliku jêzykowego.
	/*! Metoda ta jest wywo³ywana z CMultiLanguage::GetLangFileName. 
	\param[in] iIndex Indeks pliku jêzykowego.
	\return Nazwa pliku jêzykowego. W przypadku b³êdu zwraca \a "languages\\english.lng". */
	inline LPTSTR GetLngFileName(GLint iIndex){
		if( iIndex < 0 || iIndex >= GetLngFileNameSize() )
			return _T("languages\\english.lng");
		return m_aLanguageFileName[ iIndex ];
	}
	//@}

	/// \name Pola u¿ywane tylko wtedy gdy nie wykorzystywany jest XML.
	//@{

	/// Tablica nazw plików \a *.\a lng.
	/*! Tablica ta przechowuje nazwy znalezionych plików z rozszerzeniem \a lng w katalogu \a languages. */
	std::vector<TCHAR*> m_aLanguageFileName;
	//@}

	/// \name Metody u¿ywane tylko wtedy gdy wykorzystywany jest XML.
	//@{

	/// Metoda parsuj¹ca plik XML.
	/*! Metoda ta parsuje specjalnie przygotowany plik XML ze zdefiniowanymi jêzykami.
	\param[in] lpFileName Nazwa pliku XML jaki zostanie sparsowany. Domyœlnie jest to \a "languages\\languages_def.xml".
	\return \p GL_TRUE w przypadku powodzenia otwarcia pliku XML. W przeciwnym wypadku \p GL_FALSE. */
	GLboolean ParseXMLLngData(LPCTSTR lpFileName = _T("languages\\languages_def.xml"));

	/// Metoda szukaj¹ca znacznika w zadanym tekœcie.
	/*! Metoda ta wywo³ywana jest z CMultiLanguage::ParseXMLLngData.
	\param[in] lpSource Tekst, w którym poszukiwany bêdzie znacznik.
	\param[in] lpTag Nazwa poszukiwanego znacznika.
	\return \p GL_TRUE w przypadku znalezienia znacznika. W przeciwnym wypadku \p GL_FALSE. */
	GLboolean SearchTag(LPCTSTR lpSource, LPCTSTR lpTag);

	/// Metoda szukaj¹ca parametru w zadanym tekœcie.
	/*! Metoda ta wywo³ywana jest z CMultiLanguage::ParseXMLLngData. Zadaniem tej metody jest odnaleŸæ w tekœcie
	dany parametr i wy³uskaæ z niego wartoœæ.
	\param[in] lpSource Tekst, w którym poszukiwany bêdzie parametr.
	\param[in] lpParameter Nazwa poszukiwanego parametru.
	\param[out] lpResult WskaŸnik na bufor, do którego zapisana zostanie wartoœæ parametru. Je¿eli parametr
	nie zostanie odnaleziony to lpResult bêdzie wype³niony zerami.
	\param[in] iSizeResult Rozmiar bufora lpResult.
	\return \p GL_TRUE w przypadku odnalezienia szukanego parametru. W przeciwnym wypadku \p GL_FALSE. */
	GLboolean SearchParameter(LPCTSTR lpSource, LPCTSTR lpParameter, LPTSTR lpResult, GLsizei iSizeResult);

	/// Metoda tworz¹ca nowy jêzyk na podstawie parsowania pliku XML.
	/*! Metoda ta tworzy nowy element tablicy CMultiLanguage::m_aXMLLngData, do którego zapisuje nazwê jêzyka
	oraz nazwê pliku \a lng.
	\param[in] lpName Nazwa jêzyka jako wartoœæ parametru \p name znacznika \p \<language\>.
	\param[in] lpLNG Nazwa pliku \a lng jako wartoœæ parametru \p file znacznika \p \<language\>. */
	GLvoid AddXMLLngData(LPCTSTR lpName, LPCTSTR lpLNG);

	/// Metoda dodaj¹ca nazy plików z czcionk¹.
	/*! Metoda ta dodaje nowy element do tablicy CMultiLanguage::SXMLLngData::aFileTTF.
	\param[in] lpTTF Nazwa pliku z czcionk¹ jako wartoœæ parametru \p file znacznika \p \<font\>. */
	GLvoid AddXMLLngDataTTF(LPCTSTR lpTTF);

	/// Metoda usuwaj¹ca wszelkie dane zapisane w tablicy CMultiLanguage::m_aXMLLngData.
	GLvoid ClearXMLLngData();

	/// Metoda zwracaj¹ca iloœæ zdefiniowanych jêzyków w pliku \a languages_def.xml.
	/*! \return Iloœæ elementów tablicy CMultiLanguage::m_aXMLLngData. */
	inline GLint GetXMLLngDataSize() { return static_cast<GLint>(m_aXMLLngData.size()); }

	/// Metoda zwracaj¹ca nazwê pliku jêzykowego dla danego jêzyka.
	/*! Metoda ta jest wywo³ywana z CMultiLanguage::GetLangFileName. 
	\param[in] iIndex Indeks pliku jêzykowego.
	\return Nazwa pliku jêzykowego. W przypadku b³êdu zwraca \a "languages\\english.lng". */
	inline LPTSTR GetXMLLngFileLNG(GLint iIndex){
		if( iIndex < 0 || iIndex >= GetXMLLngDataSize() )
			return _T("languages\\english.lng");
		return m_aXMLLngData[ iIndex ].lpFileLNG;
	}

	/// Metoda zwracaj¹ca iloœæ zdefiniowanych czcionek dla danego jêzyka.
	/*! Metoda zwracaj¹ca iloœæ zdefiniowanych czcionek dla indeksu jêzyka zawartego w polu
	CWindowData::SSettingFile::iIndexLanguage.
	\param[in] iIndex Indeks jêzyka zdefiniowanego w pliku XML.
	\return Iloœæ zdefiniowanych czcionek dla danego jêzyka. W przypadku jakiegoœ b³êdu, np. nie u¿ywania pliku XML,
	zwrócona zostanie wartoœæ \p -1. */
	inline GLint GetXMLLngDataTTFSize(GLint iIndex) {
		if( !m_bUseXML || ( iIndex < 0 || iIndex > GetXMLLngDataSize() ) )
			return -1;
		return static_cast<GLint>( m_aXMLLngData[iIndex].aFileTTF.size() );
	}
	//@}

	/// \name Pola u¿ywane tylko wtedy gdy wykorzystywany jest XML.
	//@{

	std::vector<SXMLLngData> m_aXMLLngData;		///< Tablica danych jêzyków opisanych w pliku XML.
	//@}

	GLboolean m_bUseXML;						///< Flaga wskazuj¹ca czy wykorzystywany jest plik XML.
};