/** \file
	\brief Plik nag³ówkowy dla klasy CSDLFont.
*/
//////////////////////////////////////////////////////////////////////

#pragma once

#include <SDL.h>
#include <SDL_ttf.h>

class CWindowData;

/// Klasa renderuj¹ca tekst.
/*! Klasa umo¿liwiaj¹ca rysowanie tekstu przy u¿yciu biblioteki SDL_ttf. Aby umo¿liwiæ wyœwietlanie polskich znaków
nale¿y zbudowaæ aplikacjê w \p UNICODE (domyœlnie OpenGLFramework u¿ywa ju¿ \p UNICODE). */
class CSDLFont  
{
public:
	/// Konstruktor.
	/*! \param[in] lpFontFile Nazwa pliku z czcionk¹ jakiej chcemy u¿yæ (domyœlnie \p "fonts/FreeSansBold.ttf").
	OpenGLFramework zawiera w³asne darmowe czcionki – umieszczone s¹ one w katalogu \a fonts. Mo¿na tak¿e wczytaæ
	pliki zawarte w katalogu \a C:\\Windows\\Fonts, nale¿y jednak pamiêtaæ, ¿e musimy podaæ nazwê pliku a nie nazwê
	zarejestrowanej czcionki.
	\param[in] iSizeFont Wielkoœæ czcionki.
	\param[in] bUseStorageBuffer Ustawiaj¹c tê flagê na \p GL_TRUE, nakazujemy buforowania wyœwietlanych tekstów, dziêki
	czemu, rendering teksu bêdzie wykonywany znacznie szybciej. Oznacza to tak¿e trochê wiêksze absorbowanie pamiêci.
	Ustawiaj¹c na \p GL_FALSE, ka¿de to samo wywo³anie rysowania tekstu, bêdzie renderowane od nowa.
	\sa SStorageBuffer.	*/
	CSDLFont( const char *lpFontFile = "fonts/FreeSansBold.ttf", GLint iSizeFont = 20, GLboolean bUseStorageBuffer = GL_TRUE );

	/// Destruktor.
	/*! Destruktor */
	virtual ~CSDLFont();

	/// \name Zestaw metod rysuj¹cych tekst.
	//@{

	/// Metoda rysuj¹ca tekst.
	/*! \param[in] iPosX \param[in] iPosY Pozycja tekstu wzglêdem ekranu. Punkt \p 0x0 znajduje siê w lewym górnym
	rogu ekranu (tekst jest rysowany "na p³aszczyŸnie ekranu"). Wa¿ne jest aby rysowaæ tekst tak jakby mia³ byæ
	rozmieszczony dla ekranu \p 1024x768 – wówczas bêdziemy mieli prawid³owe skalowanie tekstu wzglêdem
	rozdzielczoœci. Oznacza to, ¿e je¿eli stosujemy rozdzielczoœæ, np. \p 800x600 i chcemy wyœwietliæ tekst na
	samym spodzie okna, to nie ustawiamy \a iPosY na \p 600 tylko na \p 768!
	\param[in] crColor Sk³adowe koloru tekstu. Nale¿y u¿yæ makra \p RGB().
	\param[in] lpText Tekst jaki chcemy wyœwietliæ.
	\param[in] bMidScreen Czy tekst ma byæ wyrównany do œrodka. Je¿eli ustawimy tê wartoœæ na \p GL_TRUE to tekst
	bêdzie wyrównany wzglêdem œrodka ekranu. Oznacza to, ¿e wartoœæ \a iPosX nie bêdzie brana pod uwagê, poniewa¿
	program sam j¹ wyliczy. Gdy podamy \p GL_FALSE (wartoœæ domyœlna), wówczas tekst bêdzie pozycjonowany na
	podstawie \a iPosX.	*/
	GLvoid DrawText( GLint iPosX, GLint iPosY, COLORREF crColor, LPCTSTR lpText, GLboolean bMidScreen = GL_FALSE );

	/// Metoda rysuj¹ca tekst z mo¿liwoœci¹ formatowania.
	/*! \param[in] iPosX \param[in] iPosY Pozycja tekstu wzglêdem ekranu. Punkt \p 0x0 znajduje siê w lewym górnym
	rogu ekranu (tekst jest rysowany "na p³aszczyŸnie ekranu"). Wa¿ne jest aby rysowaæ tekst tak jakby mia³ byæ
	rozmieszczony dla ekranu \p 1024x768 – wówczas bêdziemy mieli prawid³owe skalowanie tekstu wzglêdem
	rozdzielczoœci. Oznacza to, ¿e je¿eli stosujemy rozdzielczoœæ, np. \p 800x600 i chcemy wyœwietliæ tekst na
	samym spodzie okna, to nie ustawiamy \a iPosY na \p 600 tylko na \p 768!
	\param[in] bMidScreen Czy tekst ma byæ wyrównany do œrodka. Je¿eli ustawimy tê wartoœæ na \p GL_TRUE to tekst
	bêdzie wyrównany wzglêdem œrodka ekranu. Oznacza to, ¿e wartoœæ \a iPosX nie bêdzie brana pod uwagê, poniewa¿
	program sam j¹ wyliczy. Gdy podamy \p GL_FALSE (wartoœæ domyœlna), wówczas tekst bêdzie pozycjonowany na
	podstawie \a iPosX.
	\param[in] crColor Sk³adowe koloru tekstu. Nale¿y u¿yæ makra \p RGB().
	\param[in] fmt Tekst jaki chcemy wyœwietliæ.
	\param[in] ... Opcjonalne parametry dla formatowania.	*/
	GLvoid DrawTextFormat( GLint iPosX, GLint iPosY, GLboolean bMidScreen, COLORREF crColor, LPCTSTR fmt, ... );

	/// Metoda rysuj¹ca tekst w prostok¹cie.
	/*! \param[in] rcRectangle Prostok¹t opisuj¹cy po³o¿enie tekstu. Pozycja tekstu zostanie wyliczona tak aby tekst
	narysowa³ siê dok³adanie na œrodku prostok¹ta. Wa¿ne jest aby pola struktury \p RECT zawiera³y pozycjê prostok¹ta
	w pikselach dla rozdzielczoœci ekranu \p 1024x768 – wówczas bêdziemy mieli prawid³owe skalowanie tekstu wzglêdem
	rozdzielczoœci.
	\param[in] crColor Sk³adowe koloru tekstu. Nale¿y u¿yæ makra \p RGB().
	\param[in] lpText Tekst jaki chcemy wyœwietliæ.	*/
	GLvoid DrawText( const RECT &rcRectangle, COLORREF crColor, LPCTSTR lpText );

	/// Metoda rysuj¹ca tekst w prostok¹cie z mo¿liwoœci¹ formatowania.
	/*! \param[in] rcRectangle Prostok¹t opisuj¹cy po³o¿enie tekstu. Pozycja tekstu zostanie wyliczona tak aby tekst
	narysowa³ siê dok³adanie na œrodku prostok¹ta. Wa¿ne jest aby pola struktury \p RECT zawiera³y pozycjê prostok¹ta
	w pikselach dla rozdzielczoœci ekranu \p 1024x768 – wówczas bêdziemy mieli prawid³owe skalowanie tekstu wzglêdem
	rozdzielczoœci.
	\param[in] crColor Sk³adowe koloru tekstu. Nale¿y u¿yæ makra \p RGB().
	\param[in] fmt Tekst jaki chcemy wyœwietliæ.
	\param[in] ... Opcjonalne parametry dla formatowania. */
	GLvoid DrawTextFormat( const RECT &rcRectangle, COLORREF crColor, LPCTSTR fmt, ... );
	//@}

	/// Metoda ustawiaj¹ca wyœwietlanie t³a pod tekstem.
	/*! \param[in] bDrawBkgText Je¿eli podamy \p GL_TRUE to pod ka¿dym tekstem bêdzie wyœwietlane t³o. Podaj¹c
	\p GL_FALSE	t³o nie bêdzie rysowane.
	\param[in] crColor Sk³adowe koloru t³a. Nale¿y u¿yæ makra \p RGB(). Kolor bêdzie brany pod uwagê tylko wtedy je¿eli
	\a bDrawBkgText ustawimy na \p GL_TRUE.	*/
	GLvoid SetBkgText( GLboolean bDrawBkgText, COLORREF crColor = RGB( 0, 0, 0 ) );

	/// Metoda pobieraj¹ca wysokoœæ czcionki w pikselach.
	/*! \return Liczba ca³kowita okreœlaj¹ca wyskoœæ czcionki w pikselach. W przypadku b³êdu metoda zwróci \p -1. */
	GLint GetHeightFont();

	/// \name Metody dla StorageBuffer-a.
	//@{

	/// Metoda czyszcz¹ca ca³y StorageBuffer dla danego obiektu klasy CSDLFont.
	/*! \param[in] bLog Dodatkowy parametr dla rejestrowania do pliku. Podaj¹æ \p GL_TRUE,
	zapisana zostanie do pliku informacja o iloœci usuniêtych pozycji bufora (iloœæ sk³adowanych i usuniêtych tekstów).
	Je¿eli podamy \p GL_FALSE, to pomimo aktywnoœci logger-a wspomniana informacja nie zostanie zapisana do pliku.
	\sa SStorageBuffer, CLogger. */
	GLvoid ClearStorageBuffer( GLboolean bLog = GL_TRUE );

	/// Metoda sprawdzaj¹ca czy StorageBuffer jest aktywny.
	/*! \return \p GL_TRUE je¿eli u¿ywany jest StorageBuffer, w przeciwnym wypadku \p GL_FALSE.
	\sa SStorageBuffer, SetStorageBuffer, m_bUseStorageBuffer. */
	inline GLboolean IsStorageBuffer() { return m_bUseStorageBuffer; }

	//// Metoda ustawiaj¹ca czy StorageBuffer ma myæ u¿ywany czy te¿ nie.
	/*! Za pomoc¹ tej metody mo¿emy w³¹czyæ lub wy³¹czyæ u¿ycie StorageBuffer w trakcie "¿ycia" obiektu.
	\param[in] bUse Podaj¹c \p GL_TRUE uaktywniamy StorageBuffer, podaj¹c \p GL_FALSE wy³¹czamy go.	*/
	inline GLvoid SetStorageBuffer( GLboolean bUse ) { m_bUseStorageBuffer = bUse; }
	//@}

	/// Metoda timer-a wywo³ywana cyklicznie (domyœlnie co sekundê) przez metodê CGameControl::Timer().
	/*! W metodzie tej usuwane s¹ stare, nieu¿ywane teksty ze StorageBuffera. */
	GLvoid Timer();

	/// Metoda zwracaj¹ca kompilowan¹ wersjê bibliotek \p SDL i \p SDL_ttf.
	/*! \return £añcych znaków zawieraj¹cy nastêpuj¹ce informacje: g³owny numer wersji \p SDL, pomniejszy numer wersji
	\p SDL, numer ³atki \p SDL, nastêpnie mamy g³owny numer wersji \p SDL_ttf, pomniejszy numer wersji \p SDL_ttf
	oraz numer ³atki \p SDL_ttf. */
	LPTSTR GetCompileVersionSLD_ttf();

	/// Metoda zwracaj¹ca linkowan¹ wersjê bibliotek \p SDL i \p SDL_ttf.
	/*! \return £añcych znaków zawieraj¹cy nastêpuj¹ce informacje: g³owny numer wersji \p SDL, pomniejszy numer wersji
	\p SDL, numer ³atki \p SDL, nastêpnie mamy g³owny numer wersji \p SDL_ttf, pomniejszy numer wersji \p SDL_ttf
	oraz numer ³atki \p SDL_ttf. */
	LPTSTR GetLinkedVersionSLD_ttf();

private:
	/// Struktura StorageBuffer wspomagaj¹ca rysowanie tekstu za pomoc¹ klasy CSDLFont.
	/*! Aby zoptymalizowaæ rendering tekstu, ka¿dy obiekt klasy CSDLFont zwiera zmienn¹ CSDLFont::m_bUseStorageBuffer
	(ustawian¹ przez konstruktor CSDLFont) mówi¹c¹ o tym czy nale¿y zapamiêtywaæ wczeœniej wyrenderowane teksty.
	Je¿eli StorageBuffer jest w u¿yciu to przy ka¿dym wywo³aniu metod CSDLFont::DrawText czy CSDLFont::DrawTextFormat,
	najpierw sprawdzane jest czy podany tekst nie zosta³ ju¿ wczeœniej wyrenderowany (sprawdzany jest tekst i jego kolor).
	Je¿eli nie, to podany tekst zostanie wyrenderowany na nowo oraz zapisany do bufora. Czyli nastêpnym razem przy
	kolejnym wywo³aniu CSDLFont::DrawText tekst (o ile jest identyczny) nie bêdzie musia³ byæ renderowany od nowa tylko
	u¿yta zostanie wczeœniej przygotowana i buforowana tekstura. \n
	Je¿eli StorageBuffer nie jest u¿ywany to ka¿de wywo³anie metod rysuj¹cych tekst powoduje renderowanie tekstu od
	nowa co trwa znacznie d³u¿ej. \n
	Aby zminimalizowaæ zu¿ycie pamiêci, podczas u¿ywania StorageBuffer, ka¿demu buforowanemu tekstowi przypisywany jest
	czas ostatniego odwo³ania siê do niego. Nastêpnie co dany kwarta³ czasu (domyœlnie co 30 sekund) usuwane s¹ z bufora
	stare teksty, których czas ostatniego wykorzystania przekracza okreœlony limit (domyœlnie 30 sekund). \n
	Dodatkowo, ka¿dy obiekt klasy CSDLFont zawiera limit na buforowanie tekstów – domyœlnie obiekt mo¿e buforowaæ
	maksymalnie 200 tekstów. */
	struct SStorageBuffer
	{
		TCHAR *lpText;			///< Renderowany tekst.
		GLuint uiTexture;		///< Gotowa tekstura z wyrenderowanym tekstem.
		GLint iWidthPowOfTwo;	///< Szerokoœæ tekstury (w piselach) bêd¹ca potêg¹ dwójki.
		GLint iHeightPowOfTwo;	///< Wysokoœæ tekstury (w piselach) bêd¹ca potêg¹ dwójki.
		GLint iOriginalWidth;	///< Szerokoœæ tekstu w pikselach.
		GLint iOriginalHeight;	///< Wysokoœæ tekstu w pikselach.
		SDL_Color sdlColor;		///< Struktura przechowuj¹ca kolor tekstu.
		GLboolean bMidScreen;	///< Flaga okreœlaj¹ca czy tekst ma byæ pozycjonowany na œrodku ekranu.
		GLuint uiRefTime;		///< Czas ostatniego u¿ycia (w sekundach) od uruchomienia programu.
	};

	CWindowData	*m_pWinData;	///< WskaŸnik na obiekt klasy CWindowData.
	TTF_Font *m_pTTFFont;		///< WskaŸnik na obiekt struktury TTF_Font, reprezentuj¹cej czcionkê.

	/// Metoda zwracaj¹ca najbli¿sz¹, nastêpn¹ liczbê bêd¹c¹ potêg¹ dwójki.
	/*! \param[in] iNumber Liczba, od której szukana bêdzie najbli¿sza potêga dwójki.
	\return Liczba bêd¹ca potêg¹ dwójki. */
	GLint NextPowerOfTwo( GLint iNumber );

	/// Wstêpna metoda renderowania tekstu.
	/*! W metodzie tej podejmowana jest decyzja czy nele¿y renderowaæ tekst od nowa, czy skorzystaæ ju¿ z istniej¹cego,
	sk³adowanego w StorageBuffer.
	\param[in] lpText Tekst do wyœwietlenia.
	\param[in] pFont WskaŸnik na strukturê \p TTF_Font reprezentuj¹c¹ czcionkê.
	\param[in] in_sdlColor Obiekt struktury \p SDL_Color reprezentuj¹cej sk³adowe koloru tekstu.
	\param[in,out] in_out_sdlLocation Obiekt struktury \p SDL_Rect reprezentuj¹cej po³o¿enie tekstu.
	\param[in] bMidScreen Flaga wskazuj¹ca czy tekst ma byæ wyrównany do œrodka.
	\param[in] bIsRect Flaga wskazuj¹ca czy tekst ma byæ umieszczony w œrodku prostok¹ta \p SDL_Rect.
	\sa SStorageBuffer.	*/
	GLvoid RenderText( LPCTSTR lpText, TTF_Font *pFont, const SDL_Color &in_sdlColor, SDL_Rect &in_out_sdlLocation, GLboolean bMidScreen, GLboolean bIsRect = GL_FALSE );

	/// Metoda sprawdzaj¹ca czy dany tekst znajduje siê w StorageBuffer.
	/*!
	\param[in] lpText Tekst do wyœwietlenia.
	\param[in] in_sdlColor Obiekt struktury \p SDL_Color reprezentuj¹cej sk³adowe koloru tekstu.
	\param[in] bMidScreen Flaga wskazuj¹ca czy tekst ma byæ wyrównany do œrodka.
	\param[out] out_iIndex Indeks elementu tablicy StorageBuffer-a, w którym zosta³ znaleziony tekst.
	\return \p GL_TRUE je¿eli tekst znajduje siê w StorageBuffer, w przeciwnym wypadku \p GL_FALSE. Je¿eli metoda
	zwróci \p GL_TRUE to parametr \a out_iIndex zostanie ustawiony numerem indeksu tablicy StorageBuffer-a.
	\sa SStorageBuffer.	*/
	GLboolean IsTextInStorageBuffer( LPCTSTR lpText, const SDL_Color &in_sdlColor, GLboolean bMidScreen, GLint &out_iIndex );

	/// Metoda renderuj¹ca tekst ze StorageBuffer-a.
	/*! \param[in,out] in_out_sdlLocation Obiekt struktury \p SDL_Rect reprezentuj¹cej po³o¿enie tekstu.
	\param[in] iIndex Indeks tablicy StorageBuffer-a, z której zostan¹ pobrane informacje do wyrenderowania tekstu.
	\param[in] bIsRect Flaga wskazuj¹ca czy tekst ma byæ umieszczony w œrodku prostok¹ta \p SDL_Rect.
	\sa SStorageBuffer.	*/
	GLvoid RenderAlreadyExistText( SDL_Rect &in_out_sdlLocation, GLint iIndex, GLboolean bIsRect );

	/// Metoda renderuj¹ca nowy tekst.
	/*! Je¿eli u¿ywany jest StorageBuffer to informacje o nowo wyrenderowanym tekœcie zostan¹ do niego zapisane.
	Dziêki temu przy kolejnym rysowaniu tego samego tekstu, nie bêdzie on musia³ byæ na nowo tworzony.
	\param[in] lpText Tekst do wyœwietlenia.
	\param[in] pFont WskaŸnik na strukturê \p TTF_Font reprezentuj¹c¹ czcionkê.
	\param[in] in_sdlColor Obiekt struktury \p SDL_Color reprezentuj¹cej sk³adowe koloru tekstu.
	\param[in,out] in_out_sdlLocation Obiekt struktury \p SDL_Rect reprezentuj¹cej po³o¿enie tekstu.
	\param[in] bMidScreen Flaga wskazuj¹ca czy tekst ma byæ wyrównany do œrodka.
	\param[in] bIsRect Flaga wskazuj¹ca czy tekst ma byæ umieszczony w œrodku prostok¹ta \p SDL_Rect.
	\sa SStorageBuffer.	*/
	GLvoid RenderNewText( LPCTSTR lpText, TTF_Font *pFont, const SDL_Color &in_sdlColor, SDL_Rect &in_out_sdlLocation, GLboolean bMidScreen, GLboolean bIsRect );

	GLboolean m_bDrawBkgText;				///< Flaga wskazuj¹ca czy ma byæ rysowane t³o pod tekstem.
	GLubyte m_ubBkgR;						///< Sk³adowa \p R koloru t³a rysowanego pod tekstem.
	GLubyte m_ubBkgG;						///< Sk³adowa \p G koloru t³a rysowanego pod tekstem.
	GLubyte m_ubBkgB;						///< Sk³adowa \p B koloru t³a rysowanego pod tekstem.

	GLint m_iHeightFont;					///< Zmienna przechowuj¹ca wysokoœæ czcionki.
	static GLint ms_iCountRef;				///< Licznik zliczaj¹cy iloœæ stworzonych instancji klasy CSDLFont.
	GLint m_iID;							///< Identyfikator (indeks tablicy g_aPointerSDLFont) danej instancji klasy CSDLFont.
	TCHAR m_tcVersion[ 256 ];				///< Tekst zwracany przez metody CSDLFont::GetCompileVersionSLD_ttf i CSDLFont::GetLinkedVersionSLD_ttf.

	std::vector<SStorageBuffer> m_aStorageBuffer;	///< Dynamiczna tablica StorageBuffer.
	GLboolean m_bUseStorageBuffer;			///< Flaga wskazuj¹ca czy StorageBuffer jest u¿ywany.
	GLuint m_uiCountTimer;					///< Licznik timer-a wskazuj¹cy kiedy nale¿y usn¹æ stare elementy StorageBuffer-a.

	/// Metoda usuwaj¹ca jeden element StorageBuffer-a.
	/*! \param[in] iIndex Pozycja StorageBuffer-a do usuniêcia. */
	GLvoid DeleteOneStorageBuffer( GLint iIndex );

	/// Metoda usuwaj¹ca stare, nieu¿ywane elementy StorageBuffer-a.
	/*! Metoda ta wywo³ywana jest cyklicznie w metodzie CSDLFont::Timer co zadany interwa³ czasu. */
	GLvoid DeleteOldStorageBuffer();

	/// Metoda pobieraj¹ca iloœæ sk³adowanych elementów w StorageBuffer.
	/*! \return Iloœæ elementów tablicy CSDLFont::m_aStorageBuffer.	*/
	inline GLint GetStorageBufferSize() { return static_cast<GLint>( m_aStorageBuffer.size() ); }

	/// \name Metody pomocnicze przy renderowaniu teksu.
	//@{

	/// Metoda dopasowuj¹ca rozmiar tekstu do aktualnie u¿ywanej rozdzielczoœci okna.
	GLvoid ScaleFont();

	/// Metoda obliczaj¹ca pozycjê teksu na œrodku ekranu.
	/*! \param[in,out] in_out_sdlLocation Obiekt struktury \p SDL_Rect reprezentuj¹cej po³o¿enie tekstu.
	\param[in] iIndex Indeks tablicy StorageBuffer-a, z której zostan¹ pobrane informacje o renderowanym tekœcie. */
	GLvoid MidScreenFont( SDL_Rect &in_out_sdlLocation, GLint iIndex );

	/// Metoda wyliczaj¹ca po³o¿enie tekstu w œrodku prostok¹ta.
	/*! \param[in,out] in_out_sdlLocation Obiekt struktury prostok¹ta, w którym znajdowaæ ma siê tekst.
	\param[in] iIndex Indeks tablicy StorageBuffer-a, z której zostan¹ pobrane informacje o renderowanym tekœcie. */
	GLvoid CalculatePosTextFromRect( SDL_Rect &in_out_sdlLocation, GLint iIndex );

	/// Metoda rysuj¹ca t³o pod tekstem.
	/*! \param[in] in_sdlLocation Obiekt struktury \p SDL_Rect reprezentuj¹cej po³o¿enie tekstu.
	\param[in] iIndex Indeks tablicy StorageBuffer-a, z której zostan¹ pobrane informacje o renderowanym tekœcie. */
	GLvoid DrawBkgText( const SDL_Rect &in_sdlLocation, GLint iIndex );

	/// Metoda rysuj¹ca tekst.
	/*! \param[in] in_sdlLocation Obiekt struktury \p SDL_Rect reprezentuj¹cej po³o¿enie tekstu.
	\param[in] iIndex Indeks tablicy StorageBuffer-a, z której zostan¹ pobrane informacje o renderowanym tekœcie. */
	GLvoid DrawQuadText( const SDL_Rect &in_sdlLocation, GLint iIndex );
	//@}

	GLuint m_uiInterval;	///< Interwa³ czasu wskazuj¹cy co ile sekund kasowaæ stare elementy StorageBuffer-a.
	GLint m_iBufferLimit;	///< Limit buforowania tekstów w StorageBuffer.
	GLuint m_uiTimeOut;		///< Iloœæ sekund przedawnienia StorageBuffer-a.

	CVector2*	m_aTexCoord;
	CVector2*	m_aIndices;
};

/// Globalna tablica wskaŸników na wszelkie obiekty CSDLFont.
extern std::vector<CSDLFont*> g_aPointerSDLFont;