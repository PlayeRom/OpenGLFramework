/*! \file
	\brief Plik nag³ówkowy dla klasy CFramework.
*/

#pragma once

class CWindowData;
class CGameControl;

/// Klasa szkieletu aplikacji. 
/*! Zadaniem tej klasy jest inicjalizacja oraz niszczenie ca³ego frameworka oraz zarz¹dzanie g³ówn¹ pêtl¹ programu. \n
Nale¿y utworzyæ obiekt tej klasy w g³ównej metodzie programu WinMain oraz wywo³aæ jej jedyn¹ publiczn¹ metodê
CFramework::Run. */
class CFramework
{
public:
	/// Konstruktor.
	/*! Konstruktor. */
	CFramework();

	/// Destruktor.
	/*! Destruktor. */
	virtual ~CFramework();

	/// Metoda zarz¹dzaj¹ca ca³ym frameworkiem.
	/*! \return Kod wyjœcia programu. Je¿eli \p 0 to program zakoñczy³ siê prawid³owo. Wartoœæ ta musi byæ tak¿e
	zwrócona przez g³ówn¹ funkcjê programu WinMain. */
	GLint Run();

	/// G³ówna metoda, od której rozpoczyna siê rysowanie ca³ej sceny.
	/*! Wewn¹trz tej metody wywo³ywana jest nie mniej wa¿na metoda CGameControl::Draw.
	\return \p GL_TRUE w przypadku powodzenia. \p GL_FALSE w przypadku wyst¹pienia jakiegoœ b³êdu. */
	GLboolean DrawGLScene();

	/// Metoda usuwaj¹ca ca³e okno frameworka oraz obiekt klasy CGameControl.
	/*! \param[in] bKillGLWindow Podaj¹c jako \p GL_FALSE, zniszczony zostanie tylko obiekt klasy CGameControl.
	Podaj¹c \p GL_TRUE, oprócz zniszczenia CGameControl, zniszczone zostanie tak¿e okno apliakcji. */
	GLvoid Destroy( GLboolean bKillGLWindow );

	static CWindowData*		g_pWinData;					///< WskaŸnik na obiekt klasy singletonu CWindowData.
	static CGameControl*	g_pGameCtrl;				///< WskaŸnik na obiekt klasy singletonu CGameControl.
	static GLboolean		g_bActive;					///< Znacznik aktywnoœci okna, domyœlnie ustawiony na \p GL_TRUE np. okno nie zminimalizowane.
	static GLint			g_iCompleteWheelRotate;		///< Licznik kompletnego obrotu rolki myszy.

protected:
	/// G³ówna metoda inicjalizuj¹ca wstêpne parametry OpenGL-a.
	/*! Wewn¹trz tej metody wywo³ywana jest tak¿e metoda CFramework::CreateGameControl.
	\return \p GL_TRUE w przypadku powodzenia inicjalizacji. \p GL_FALSE w przypadku wyst¹pienia jakiegoœ b³êdu. */
	GLboolean InitalizationGL();

	/// Metoda tworz¹ca oraz inicjalizuj¹ca obiekt klasy CGameControl.
	/*! \return \p GL_TRUE w przypadku powodzenia inicjalizacji. \p GL_FALSE w przypadku wyst¹pienia jakiegoœ b³êdu. */
	GLboolean CreateGameControl();
};
