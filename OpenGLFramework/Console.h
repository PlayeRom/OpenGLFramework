/*! \file
	\brief Plik nag³ówkowy dla klasy CConsole.
*/
#pragma once

#include "RMenuBase.h"

class CBitmapFont;
class CSDLFont;
class CMyFont;
class CSpeedControl;

/// Makro dodaj¹ce linijkê tekstu do konsoli w kodowaniu ASCII lub UNICODE.
/** \param[in] type Typ dodawanej informacji. 
\param[in] msg Tekst jaki chcemy wyœwietliæ w konsoli.
\param[in] ... Opcjonalne parametry dla formatowania. */
#define CONSOLE_ADDTEXT( type, msg, ... ) CGameControl::GetInstance()->GetConsole()->AddText( type, msg, __VA_ARGS__ )

/// Makro dodaj¹ce linijkê tekstu do konsoli w kodowaniu ASCII lub UNICODE z typem ENormal.
/** \param[in] msg Tekst jaki chcemy wyœwietliæ w konsoli.
\param[in] ... Opcjonalne parametry dla formatowania. */
#define CONSOLE_ADDTEXT_NORMAL( msg, ... ) CGameControl::GetInstance()->GetConsole()->AddText( msg, __VA_ARGS__ )

/// Wielkoœæ bufora tekstu pojedynczej linii tekstu.
#define MAX_ADDTEXT_BUFFER 256

/// Klasa konsoli.
/** Klasa ta odpowiada za wyœwietlanie oraz obs³ugê konsoli. */
class CConsole : public CRMenuBase
{
public:

	/// Typ wyliczeniowy reprezentuj¹cy rodzaje wiadomoœci.
	/** Ka¿d¹ informacjê wyœwietlan¹ w konsoli mo¿na okreœliæ danym typem przypisuj¹c jej odpowiedni kolor.  */
	enum EConsoleMsgType
	{
		ENormal,		///< Tekst normalny w kolorze bia³ym.
		EWarning,		///< Ostrze¿enie w kolorze ¿ó³tym.
		EError,			///< B³¹d w kolorze czerwonym.
		ESuccess		///< Sukces w kolorze zielonym.
	};

	/// Konstruktor.
	/** Konstruktor. */
	CConsole();

	/// Destruktor.
	/** Destruktor. */
	virtual ~CConsole();

	/// Metoda restartuj¹ca dane konsoli, które tego wymagaj¹.
	/** Metoda ta jest wywo³ywana gdy restartowane jest okno, np. pod wp³ywem zmiany rozdzielczoœci, próbkowania
	dla antyaliasingu, itp.  */
	GLvoid RestartObjects();

	/// Metoda wyœwietlaj¹ca konsolê.
	/** Nale¿y wywo³aæ tê metod¹ aby konsola by³a rysowana. */
	GLvoid ShowConsole();

	/// Metoda ukrywaj¹ca konsolê.
	/** Nale¿y wywo³aæ tê metodê aby zaprzestaæ rysowania konsoli. */
	inline GLvoid HideConsole() { m_bShowConsole = GL_FALSE; }

	/// Metoda sprawdzaj¹ca czy konsola jest rysowana.
	/** \return \p GL_TRUE je¿eli konsola jest rysowana. W przeciwnym wypadku \p GL_FALSE. */
	inline GLboolean IsConsole() { return m_bShowConsole; }

	/// Metoda dodaj¹ca linijkê tekstu do konsoli w kodowaniu ASCII.
	/** \param[in] eType Typ dodawanej informacji. 
	\param[in] fmt Tekst jaki chcemy wyœwietliæ w konsoli.
	\param[in] ... Opcjonalne parametry dla formatowania. */
	GLvoid AddText(EConsoleMsgType eType, const char *fmt, ...);

	/// Metoda dodaj¹ca linijkê tekstu do konsoli w kodowaniu ASCII z typem ENormal.
	/** \param[in] fmt Tekst jaki chcemy wyœwietliæ w konsoli.
	\param[in] ... Opcjonalne parametry dla formatowania. */
	GLvoid AddText(const char *fmt, ...);

#ifdef UNICODE
	/// Metoda dodaj¹ca linijkê tekstu do konsoli w kodowaniu UNICODE.
	/** \param[in] eType Typ dodawanej informacji. 
	\param[in] fmt Tekst jaki chcemy wyœwietliæ w konsoli.
	\param[in] ... Opcjonalne parametry dla formatowania. */
	GLvoid AddText(EConsoleMsgType eType, const wchar_t *fmt, ...);

	/// Metoda dodaj¹ca linijkê tekstu do konsoli w kodowaniu UNICODE z typem ENormal.
	/** \param[in] fmt Tekst jaki chcemy wyœwietliæ w konsoli.
	\param[in] ... Opcjonalne parametry dla formatowania. */
	GLvoid AddText(const wchar_t *fmt, ...);
#endif

	/// Metoda rysuj¹ca konsolê.
	/** Metoda rysuje konsolê z wykorzystaniem klasy CSDLFont. */
	GLvoid DrawConsoleWithSDLFont();

#if _USE_CONSOLE_WITH_BITMAP_FONT_
	/// Metoda rysuj¹ca konsolê.
	/** Metoda rysuje konsolê z wykorzystaniem klasy CBitmapFont. */
	GLvoid DrawConsoleWithBitmapFont();
#endif //_USE_CONSOLE_WITH_BITMAP_FONT_
#if _USE_CONSOLE_WITH_MY_FONT_
	/// Metoda rysuj¹ca konsolê.
	/** Metoda rysuje konsolê z wykorzystaniem klasy CMyFont. */
	GLvoid DrawConsoleMyFont();
#endif //_USE_CONSOLE_WITH_MY_FONT_

	/// Zdarzenie naciœniêcia klawisza klawiatury, przekazywane z CGameControl.
	/** \param[in] uiKeyCode Wirtualny kod klawisza jaki zosta³ naciœniêty.
	\return \p GL_TRUE w przypadku przetworzenia zdarzenia. \p GL_FALSE gdy klawisz nie zosta³ obs³u¿ony. */
	GLboolean KeyDown(GLuint uiKeyCode);

	/// Zdarzenie zwolnienia klawisza klawiatury, przekazywane z CGameControl.
	/** \param[in] uiKeyCode Wirtualny kod klawisza jaki zosta³ zwolniony.
	\return \p GL_TRUE w przypadku przetworzenia zdarzenia. \p GL_FALSE gdy klawisz nie zosta³ obs³u¿ony. */
	GLboolean KeyUp(GLuint uiKeyCode);

	/// Zdarzenie przekrêcenia kó³ka myszy, przekazywane z CGameControl.
	/** \param[in] iScrollLines Mowi nam o ile linii i w która stronê wykonano obrót, wartoœæ ujemna - w dól, wartoœæ
	dodatnia - w górê. 
	\return \p GL_TRUE w przypadku przetworzenia zdarzenia. W przeciwnym wypadku \p GL_FALSE.*/
	GLboolean MouseWheel(GLint iScrollLines);

	/// Metoda ustawiaj¹ca blokadê na wyœwietlanie konsoli.
	/** \param[in] bBlockConsole Podaj¹c \p GL_TRUE zablokujemy wyœwietlanie konsoli. Podaj¹c \p GL_FALSE
	odblokujemy.*/
	inline GLvoid SetBlockConsole(GLboolean bBlockConsole) { m_bBlockConsole = bBlockConsole; }
	
private:

	/// Struktura przechowuj¹ca informacjê o pojedynczej linijce tekstu.
	struct SConsoleLine
	{
		TCHAR *lpMsg;				///< Wyœwietlany wiadomoœæ.
		EConsoleMsgType eType;		///< Typ wiadomoœci.
	};

	CSpeedControl	*m_pSpeedControl;		///< WskaŸnik na obiekt klasy CSpeedControl.
	CMyFont			*m_pMyFont;			///< WskaŸnik na obiekt klasy CMyFont.
	CBitmapFont		*m_pBitmapFont;		///< WskaŸnik na obiekt klasy CBitmapFont.
	CSDLFont		*m_pSDLFont;			///< WskaŸnik na obiekt klasy CSDLFont.

	/// Metoda inicjalizuj¹ca wszelkie niezbêdne zmienne odpowiednimi wartoœciami.
	GLvoid SetVariables();

	/// Metoda tworz¹ca obiekt klasy CSpeedControl.
	GLvoid CreateSpeedControl();

	GLint m_iAnimCtrl1;		///< Indeks kontroli animacji wysuwania siê konsoli.
	GLint m_iAnimCtrl2;		///< Indeks kontroli animacji mrugania znaku zachêty w konsoli.

	/////////////////////// Start LastCommand /////////////////////////////////////////

	std::vector< TCHAR* > m_aLastCommand;		///< Tablica przechowuj¹ca ostatnio wpisywane komendy.

	/// Metoda zwracaj¹ca iloœæ zapamiêtanych ostatnio wpisywanych komend.
	/** \return Liczba ca³kowita okreœlaj¹ca iloœæ zapamiêtanych komend. */
	inline GLint GetLastCommandSize() { return static_cast< GLint >( m_aLastCommand.size() ); }
	
	GLint m_iCurrentLastCommand;	///< Zmienna zapamiêtuj¹ca indeks ostatnio zapamiêtanej komendy.

	/// Metoda zapamietuj¹ca nastêpn¹ komendê.
	/** \param[in] lpCommand Tekst ostatnio wpisanej komendy. */
	GLvoid AddLastCommand( LPCTSTR lpCommand );

	/// Metoda usuwaj¹ca wszelkie pamiêtane komendy.
	GLvoid DeleteLastCommands();

	/////////////////////// End LastCommand /////////////////////////////////////////

	/// Tablica sk³aduj¹ca linijki tekstu wyœwietlane w konsoli.
	std::vector<SConsoleLine> m_aConsoleLine;

	/// Metoda zwracaj¹ca iloœæ sk³¹dowanych linijek tekstu wyœwietlanych w konsoli.
	/** \return Liczba ca³kowita reprezentuj¹ca iloœæ linijek tekstu wyœwietlanych w konsoli. */
	inline GLint GetConsoleLineSize() { return static_cast< GLint >( m_aConsoleLine.size() ); }

	/// Zmienna zapamietuj¹ca jakie typy wiadomoœci maj¹ byæ wyœwietlany w konsoli.
	EConsoleMsgType m_eFilter;

	/// Metoda usuwaj¹ca wszelkie linijki tekstu wyœwietlane w konsoli.
	GLvoid DeleteConsoleLines();

	GLint m_iStartLine;			///< Indeks linijki tekstu jaki ma byæ wyœwietlany w pierwszej linijce konsoli.
	GLint m_iEndLine;			///< Indeks linijki tekstu jaki ma byæ wyœwietlany w ostatniej linijce konsoli.
	GLint m_iPageCountLine;		///< Maksymalna iloœæ linijek tekstu wyœwietlana aktualnie w konsoli.

	GLfloat m_fPosX;			///< Wspó³rzêdna x pozycji tekstu w linijce.
	GLfloat m_fPosY;			///< Wspó³rzêdna y pozycji tekstu w linijce.
	GLfloat m_fPosYConsole;		///< Pozycja ca³ego okna konsoli dla efektu wysuwania siê.

	RECT m_rcRegionBoard;		///< Prostok¹t okreœlaj¹cy po³orzenie i rozmiar konsoli.
	GLint m_iPosYConsole;		///< Aktualna pozycja konsoli w osi Y dla efektu wysuwania siê.
	GLint m_iRateYConsole;		///< Skok wysuwania siê konsoli.
	GLboolean m_bDrawCursor;	///< Flaga wskazuj¹ca czy w danej chili ma byæ wyœwietlany znak zachêty.

	GLboolean m_bShowConsole;	///< Flaga wskazuj¹ca czy konsola jest rysowana.

	TCHAR m_cLastLineConst[8];	///< Znak zachêty konsoli.
	TCHAR m_cLastLine[128];		///< Aktualnie pisana linia w linii komend.

	GLboolean m_bBlockConsole;	///< Flaga wskazuj¹ca czy wyœwitlanie konsoli ma byæ blokowane.

	/// Metoda wykonuj¹ca podan¹ komendê.
	/** \param[in] lpCommand Komenda do przeanalizowania i wykonania. */
	GLvoid ExecuteCommand(LPCTSTR lpCommand);

	/// Metoda ustawiaj¹ca wyœwietlanie w konsoli tak aby widoczna by³a ostatnia linijka.
	GLvoid SetStartEndLineOnEnd();

	/// Metoda wyci¹gaj¹ca parametr z komendy.
	/** \param[in] lpCommand Komenda do przeanalizowania.
	\param[in] lpParameter Zwracany rezultat w postaci parametru komendy.
	\return \p GL_TRUE w przypadku powodzenia wyci¹gniêcia parametru. W przeciwnym wypadku \p GL_FALSE. */
	GLboolean GetParameterCommand(LPCTSTR lpCommand, LPTSTR lpParameter);

	/// Metoda przygotowuj¹ca tekst do wyœwietlenia w konsoli.
	/** \param[in] eType Typ wiadomoœci.
	\param[in] lpText Tekst wiadomoœci. */
	GLvoid PrepareText(EConsoleMsgType eType, const char *lpText);

	/// Motoda dodaj¹c¹ linijkê tekstu do wyœwietlenia w konsoli.
	/** \param[in] eType Typ wiadomoœci.
	\param[in] lpText Tekst wiadomoœci.
	\param uiLength D³ugoœæ tekstu. */
	GLvoid AddTextFinally(EConsoleMsgType eType, LPCTSTR lpText, size_t uiLength);

	/// Metoda skroluj¹ca tekst o jedn¹ linijkê w górê.
	GLvoid LineScrollUp();

	/// Metoda skroluj¹ca tekst o jedn¹ linijkê w dó³.
	GLvoid LineScrollDown();

	GLvoid DrawRect( GLboolean bTexCoord = GL_TRUE );

	CVector2* m_aTextCoord;
	CVector2* m_aIndices;
};