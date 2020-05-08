/*! \file
	\brief Plik nag³ówkowy dla klasy CRMenuBase.
*/
#pragma once

/// Klasa bazowa dla klas GUI z przyciskami.
/** Klasa ta zawiera metody przeznaczone do odziedziczenia przez inne klasy, reprezentuj¹ce graficzny interfejs
u¿ytkownika. */

class CRMenuBase
{
public:
	/// Konstruktor
	/** Konstruktor. */
	CRMenuBase();

	/// Destruktor.
	/** Destruktor. */
	virtual ~CRMenuBase();

protected:
	/// Metoda rysuj¹ca obramowanie wokó³ zadanego prostok¹ta po najechaniu na niego mysz¹.
	/** Metodê tê mo¿na wykorzystaæ, np. obramowuj¹c dany przycisk po najechaniu na niego kursorem myszy.
	\param[in] rcRect Referencja na strukturê \p RECT reprezentuj¹ca po³o¿enie, np. przycisku.
	\param[in] bCheckMessageBox Flaga mówi¹ca czy sprawdzane ma byæ, czy rysowana jest plansza z MessageBox-em klasy
	CRMessageBox. Je¿eli podamy \p GL_TRUE to obramowanie nie zostanie narysowane je¿eli rysowana bêdzie plansza z
	MessageBox-em. Je¿eli podamy \p GL_FALSE to obramowanie zostanie zawsze narysowane. */
	GLvoid DrawRegionIfMouse(const RECT &rcRect, GLboolean bCheckMessageBox = GL_TRUE);

	/// Metoda rysuj¹ca obramowanie wokó³ zadanego prostok¹ta.
	/** Metodê tê mo¿na wykorzystaæ do rysowania ramki.
	\param[in] rcRect Referencja na strukturê \p RECT reprezentuj¹ca po³o¿enie, np. ramki. */
	GLvoid DrawRegionBorder(const RECT &rcRect);

	/// Metoda sprawdzaj¹ca czy klikniêto w obrêbie danego prostok¹ta.
	/** \param[in] iX Wspó³rzêdna X po³o¿enia kursora myszy.
	\param[in] iY Wspó³rzêdna Y po³o¿enia kursora myszy. 
	\param[in] prcRect WskaŸnik na tablicê prostok¹tów.
	\param[in] iMaxCount Iloœæ prostok¹tów podana w \a prRect.
	\param[out] out_iIndex Zmienna do której zapisany zostanie indeks prostok¹ta, na który klikniêto. Pod warunkiem,
	¿e metoda zwróci \p GL_TRUE.
	\return \p GL_TRUE w przypadku gdy kusor znajduje siê w którymœ z regionów. W przeciwnym wypadku \p GL_FALSE. */
	GLboolean CheckWhereClickedUser(GLint iX, GLint iY, const RECT *prcRect, GLint iMaxCount, GLint &out_iIndex);

	/// Metoda rysuj¹ca zadany prostok¹t.
	/** Za pomoc¹ tej metody mo¿emy rysowaæ, np. przyciski.
	\param[in] prcRect WskaŸnik na tablicê obiektów \p RECT, które maj¹ byæ narysowane.
	\param[in] iMaxCount Maksymala iloœæ elementów tablicy obiektów \p RECT. 
	\param[in] iStart Pocz¹tkowy indeks tablicy obiektów \p RECT od którego rozpocznie siê rysowanie.
	\param[in] bSetDrawParam Flaga wskazuj¹ca czy przed rysowanie maj¹ zostaæ wywo³ane odpowiednie funkcje ustawiaj¹ce
	prametry rysowania prostok¹tów. */
	GLvoid DrawRegions(const RECT *prcRect, GLint iMaxCount, GLint iStart = 0, GLboolean bSetDrawParam = GL_TRUE);

	/// Metoda przeliczaj¹ca procent na piksele po osi X.
	/** Za pomoc¹ tej metody mo¿emy ustaliæ, np. pozycjê przycisku wzd³u¿ szerokoœci ekranu podaj¹c wartoœæ procentow¹
	po³o¿enia. Dziêki temu mo¿emy uzyskaæ wizualne t¹ sam¹ pozycjê przycisku niezale¿nie od rozdzielczoœci ekranu.
	\param[out] out_lPixelsVal Referencja na wartoœæ typu \p LONG. Do tej zmiennej zostanie zapisana wartoœæ wynikowa,
	reprezentuj¹ca iloœæ pikseli wyliczon¹ na podstawie podanej wartoœci procentowej oraz aktualnej rozdzielczoœci
	ekranu na jakiej program pracuje.
	\param[in] iPercent Wartoœæ procentowa szerokoœci ekranu, gdzie 0% to lewa a 100% prawa strona ekranu.
	\param[in] bFont Flaga wskazuj¹ca czy obliczenia przeprowadzane s¹ dla ustalenia pozycji tekstu. Dotyczy
	tylko rysowania tekstu za pomoc¹ klasy CSDLFont. */
	GLvoid SetRegionX(LONG &out_lPixelsVal, GLint iPercent, GLboolean bFont = GL_FALSE);

	/// Metoda przeliczaj¹ca procent na piksele po osi Y.
	/** Za pomoc¹ tej metody mo¿emy ustaliæ, np. pozycjê przycisku wzd³u¿ wysokoœci ekranu podaj¹c wartoœæ procentow¹
	po³o¿enia. Dziêki temu mo¿emy uzyskaæ wizualne t¹ sam¹ pozycjê przycisku niezale¿nie od rozdzielczoœci ekranu.
	\param[out] out_lPixelsVal Referencja na wartoœæ typu \p LONG. Do tej zmiennej zostanie zapisana wartoœæ wynikowa,
	reprezentuj¹ca iloœæ pikseli wyliczon¹ na podstawie podanej wartoœci procentowej oraz aktualnej rozdzielczoœci
	ekranu na jakiej program pracuje.
	\param[in] iPercent Wartoœæ procentowa wysokoœci ekranu, gdzie 0% to górna a 100% dolna strona ekranu.
	\param[in] bFont Flaga wskazuj¹ca czy obliczenia przeprowadzane s¹ dla ustalenia pozycji tekstu. Dotyczy
	tylko rysowania tekstu za pomoc¹ klasy CSDLFont. */
	GLvoid SetRegionY(LONG &out_lPixelsVal, GLint iPercent, GLboolean bFont = GL_FALSE);

	/// Metoda przeliczaj¹ca procent na piksele po osi X z dok³adnoœci¹ liczby rzeczywistej.
	/** Za pomoc¹ tej metody mo¿emy ustaliæ, np. pozycjê przycisku wzd³u¿ szerokoœci ekranu podaj¹c wartoœæ procentow¹
	po³o¿enia. Dziêki temu mo¿emy uzyskaæ wizualne t¹ sam¹ pozycjê przycisku niezale¿nie od rozdzielczoœci ekranu.
	\param[out] out_lPixelsVal Referencja na wartoœæ typu \p LONG. Do tej zmiennej zostanie zapisana wartoœæ wynikowa,
	reprezentuj¹ca iloœæ pikseli wyliczon¹ na podstawie podanej wartoœci procentowej oraz aktualnej rozdzielczoœci
	ekranu na jakiej program pracuje.
	\param[in] fPercent Wartoœæ procentowa szerokoœci ekranu, gdzie 0% to lewa a 100% prawa strona ekranu.
	\param[in] bFont Flaga wskazuj¹ca czy obliczenia przeprowadzane s¹ dla ustalenia pozycji tekstu. Dotyczy
	tylko rysowania tekstu za pomoc¹ klasy CSDLFont. */
	GLvoid SetRegionX(LONG &out_lPixelsVal, GLfloat fPercent, GLboolean bFont = GL_FALSE);

	/// Metoda przeliczaj¹ca procent na piksele po osi Y z dok³adnoœci¹ liczby rzeczywistej.
	/** Za pomoc¹ tej metody mo¿emy ustaliæ, np. pozycjê przycisku wzd³u¿ wysokoœci ekranu podaj¹c wartoœæ procentow¹
	po³o¿enia. Dziêki temu mo¿emy uzyskaæ wizualne t¹ sam¹ pozycjê przycisku niezale¿nie od rozdzielczoœci ekranu.
	\param[out] out_lPixelsVal Referencja na wartoœæ typu \p LONG. Do tej zmiennej zostanie zapisana wartoœæ wynikowa,
	reprezentuj¹ca iloœæ pikseli wyliczon¹ na podstawie podanej wartoœci procentowej oraz aktualnej rozdzielczoœci
	ekranu na jakiej program pracuje.
	\param[in] fPercent Wartoœæ procentowa wysokoœci ekranu, gdzie 0% to górna a 100% dolna strona ekranu.
	\param[in] bFont Flaga wskazuj¹ca czy obliczenia przeprowadzane s¹ dla ustalenia pozycji tekstu. Dotyczy
	tylko rysowania tekstu za pomoc¹ klasy CSDLFont. */
	GLvoid SetRegionY(LONG &out_lPixelsVal, GLfloat fPercent, GLboolean bFont = GL_FALSE);

	/// Metoda sprawdzaj¹ca czy dany punkt zawiera siê w zadanym prostok¹cie.
	/** \param[in] ptCursor Referencja na obiekt \p POINT, np. pozycja kursora myszy.
	\param[in] rcRect Referencja na obiekt \p RECT, np. przycisk. 
	\return \p GL_TRUE w przypadku gdy wspó³rzêdne punktu znajduj¹ siê wewn¹trz prostok¹ta. W przeciwnym wypadku
	\p GL_FALSE. */
	GLboolean CheckPtCursorInRect(const POINT &ptCursor, const RECT &rcRect);

	/// Zmienna zapamiêtuj¹ca aktualny indeks prostok¹ta (np. przycisku), nad którym znajduje siê kursor myszy.
	/** Zmienna ta wykorzystywana jest dla stwierdzenia faktu najechania kursora myszy na dany prostok¹t. */
	GLint m_iWhereIsMouse;

	/// Zmienna zapamiêtuj¹ca ostatni indeks prostok¹ta (np. przycisku), nad którym znajduje siê kursor myszy.
	/** Zmienna ta wykorzystywana jest dla stwierdzenia faktu najechania kursora myszy na dany prostok¹t. */
	GLint m_iWhereIsMouseOld;

	/// Tablica wierzcho³ków przekazywana do glDrawArrays.
	GLint* m_aIndices;
};
