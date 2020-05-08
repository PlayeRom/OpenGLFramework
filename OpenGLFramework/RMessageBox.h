/*! \file
	\brief Plik nag³ówkowy dla klasy CRMessageBox.
*/
#pragma once

#include "RMenuBase.h"

class CSDLFont;

/// Sta³a definiuj¹ca maksymaln¹ d³ugoœæ tektu pisanego w MessageBox-ie.
#define MSG_TEXT_LENGTH 512

/// Klasa rysuj¹ca MessageBox-a.
/** Za pomoc¹ tej klasy mo¿emy w danej chwili programu narysowaæ planszê z MessageBox-em czyli modalnym oknem
w celu powiadomienia u¿ytkownika o jakiejœ zaistania³ej sytuacji lub zapytania u¿ytkownika o jak¹œ kwestiê. */

class CRMessageBox : public CRMenuBase
{
public:
	/// Konstruktor.
	/** Konstruktor. */
	CRMessageBox();

	/// Destruktor.
	/** Destruktor. */
	~CRMessageBox();

	/// Typ wyliczeniowy okreœlaj¹cy jakiego typu ma myæ MessageBox.
	/** Konkretnie za pomoc¹ tego typu wyliczeniowego definiujemy jakie przyciski maj¹ siê pojawiæ w Messagebox-ie. */
	enum EMsgType
	{
		EMsgOK,				///< MessageBox z przyciskiem OK, do zwyk³ego informowania.
		EMsgOKCancel,		///< MessageBox z przyciskami OK i Anuluj, do zatwierdzania i anulowania.
		EMsgYesNo,			///< MessageBox z przyciskami Tak i Nie, dla pytania rozstrzygaj¹cego.
		EMsgYesNoCancel		///< MessageBox z przyciskami Tak, Nie oraz Anuluj, dla pytania z mo¿liwoœci¹ anulowania.
	};

	/// Metoda nakazuj¹ca rysowania MessageBox-a.
	/** Wywo³anie tej metody w danym miejscu progamu spowoduje rysowanie planszy z MessageBox-em.
	\param[in] lpText Tekst (informacja, pytanie) jaki ma siê wyœwietliæ w MessageBox-ie.
	\param[in] eType Typ wyliczeniowy CRMessageBox::EMsgType okreœlaj¹cy zestaw przycisków jakie maj¹ siê wyœwietliæ.
	\param[in] in_lpFunc1 WskaŸnik na funkcjê "zatwierdzaj¹c¹". Tutaj nale¿y podaæ wskaŸnik na funkcjê, któr¹ chcemy
	wywo³aæ, jako reakcjê na zatwierdzenie MessageBox-a. Zatwierdzie tj. klikniêcie przycisku OK lub Tak. Je¿eli
	nie chcemy wywo³yw¹æ ¿adnej funkcji zwrotnej nale¿y podaæ NULL.
	\param[in] in_lpFunc2 WskaŸnik na funkcjê "odrzucaj¹c¹". Tutaj nale¿y podaæ wskaŸnik na funkcjê, któr¹ chcemy
	wywo³aæ, jako reakcjê na odrzucenie MessageBox-a. Odrzucenie tj. klikniêcie przycisku Nie lub Anuluj. Je¿eli
	nie chcemy wywo³yw¹æ ¿adnej funkcji zwrotnej nale¿y podaæ NULL. W przypadku trzech przycisków, tj. Tak, Nie,
	Anuluj dla Anuluj nie bêdzie wywo³ywana ¿adna akcja. */
	GLvoid DoMessageBox( LPCTSTR lpText, EMsgType eType, GLvoid( *in_lpFunc1 )( GLvoid ), GLvoid( *in_lpFunc2 )( GLvoid ) );

	/// Metoda rysuj¹ca planszê z MessageBox-em.
	/** Metoda ta rysuje MessageBox-a wedle parametrów jakie podaliœmy w funkcji CRMessageBox::DoMessageBox.
	Metoda ta jest ju¿ wywo³ywana w odpowiednim miejscu, wewn¹trz CGameControl::Draw i nie nale¿y ju¿ jej powtórnie
	wywo³ywaæ. */
	GLvoid Draw();

	/// Metoda restartuj¹ca ustawienie regionów reprezentuj¹cych planszê i przyciski MessageBox-a.
	/** Metoda ta jest ju¿ wywo³ywana w odpowiednim miejscu, wewn¹trz CGameControl::RestartObjects. Metoda ta jest
	wywo³ywana gdy restartowane jest okno, np. pod wp³ywem zmiany rozdzielczoœci, próbkowania dla antyaliasingu, itp. */
	GLvoid RestartObjects();

	/// Zdarzenie klikniêcia lewym przyciskiem myszy.
	/** Metoda ta jest ju¿ wywo³ywana w odpowiednim miejscu, wewn¹trz CGameControl::MouseLButtonDown i nie nale¿y
	ju¿ jej powtórnie wywo³ywaæ.
	\param[in] iX Wspó³rzêdna \a X po³o¿enia kursora w momencie klikniêcia.
	\param[in] iY Wspó³rzêdna \a Y po³o¿enia kursora w momencie klikniêcia.
	\return \p GL_TRUE w przypadku przetworzenia zdarzenia. \p GL_FALSE gdy klawisz nie zosta³ obs³u¿ony. */
	GLboolean MouseLButtonDown( GLint iX, GLint iY );

	/// Zdarzenie naciœniêcia klawisza klawiatury.
	/** Metoda ta jest ju¿ wywo³ywana w odpowiednim miejscu, wewn¹trz CGameControl::KeyDown i nie nale¿y
	ju¿ jej powtórnie wywo³ywaæ.
	\param[in] uiKeyCode Wirtualny kod klawisza jaki zosta³ naciœniêty.
	\return \p GL_TRUE w przypadku przetworzenia zdarzenia. \p GL_FALSE gdy klawisz nie zosta³ obs³u¿ony. */
	GLboolean KeyDown( GLuint uiKeyCode );

	/// Metoda sprawdzaj¹ca czy MessageBox jest aktualnie rysowany.
	/** \return \p GL_TRUE gdy MessageBox jest rysowany. W przeciwnym wypadku \p GL_FALSE. */
	inline GLboolean IsDraw() { return m_bDrawMsg; }

private:
	CSDLFont		*m_pSDLFontText;		///< WskaŸnik na obiekt klasy CSDLFont.

	/// WskaŸnik na funkcjê "zatwierdzaj¹c¹".
	/** WskaŸnik na funkcjê jaka ma siê wywo³aæ po zatwierdzeniu MessageBox-a przyciskami OK lub TAK. */
	void( *lpFunc1 )( void );

	/// WskaŸnik na funkcjê "odrzucaj¹c¹".
	/** WskaŸnik na funkcjê jaka ma siê wywo³aæ po odrzuceniu MessageBox-a przyciskami NIE lub ANULUJ. */
	void( *lpFunc2 )( void );

	/// \name Regiony prostok¹tów pod plansze i przyciski.
	//@{

	/// Tablica prostok¹tów-plansz.
	/** Indeks 0 dla t³a pe³noekranowego, indeks 1 jako ramka pod sam MessageBox. */
	RECT m_rRegionsBkg[ 2 ];
	RECT m_rRegions1Button[ 3 ];
	RECT m_rRegions2Button[ 3 ];
	RECT m_rRegions3Button[ 3 ];
	//@}

	/// \name Regiony prostok¹tów dla tekstów.
	//@{
	RECT m_rRegionsFont1Button[ 3 ];
	RECT m_rRegionsFont2Button[ 3 ];
	RECT m_rRegionsFont3Button[ 3 ];
	//@}

	/// Metoda ustawiaj¹ca wspórzêdne plansz i przycisków.
	GLvoid SetRegions();

	/// Metoda rysuj¹ca przyciski.
	/** \param[in] rc Trzyelementowa tablica obiektów \p RECT ze wspó³rzêdnymi przycisków do narysowania.
	\param[in] iSize Iloœæ przycisków do narysowania. */
	GLvoid DrawButtons( RECT rc[ 3 ], GLint iSize );

	/// Metoda rysuj¹ca teksty.
	/** \param[in] rcRegions Trzyelementowa tablica obiektów \p RECT ze wspó³rzêdnymi przycisków.
	\param[in] rcRegionsFont Trzyelementowa tablica obiektów \p RECT ze wspó³rzêdnymi tekstów pod przyciski.
	\param[in] iSize Iloœæ przycisków do narysowania.*/
	GLvoid DrawFonts( RECT rcRegions[ 3 ], RECT rcRegionsFont[ 3 ], GLint iSize );

	EMsgType m_eMsgType;				///< Zmienna pamiêtaj¹ca typ MessageBox-a.
	TCHAR m_cText[ MSG_TEXT_LENGTH ];	///< Zmienna pamiêtaj¹ca tekst rysowany na MessageBox-ie.
	GLboolean m_bDrawMsg;				///< Flaga wskazuj¹ca czy MessageBox jest rysowany i czy moze siê rysowaæ.

	GLint* m_aButtonIndices;
};