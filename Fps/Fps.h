/*! \file
	\brief Plik nag³ówkowy dla klasy CFps.
*/
#pragma once

class CSDLFont;

#include "../Fonts/SDLFont.h"

/// Klasa zliczaj¹ca i wyœwietlaj¹ca iloœæ generowanych klatek na sekundê.
class CFps  
{
public:
	/// Konstruktor.
	/*! Konstruktor. */
	CFps();

	/// Destruktor.
	/*! Destruktor. */
	virtual ~CFps();

	/// Metoda ustawiaj¹ca tekst z iloœci¹ FPS jaki zostanie wyœwietlony
	/*! Metoda ta jest ju¿ wywo³ywana w metodzie CGameControl::Timer. */
	inline GLvoid SetTextFps() { wsprintf( m_cFpsText, _T("fps: %d"), m_uiFps ); }

	/// Metoda zeruj¹ca licznik FPS.
	/*! Metoda ta jest ju¿ wywo³ywana w metodzie CGameControl::Timer. */
	inline GLvoid CleanFps() { m_uiFps = 0; }

	/// Metoda inkrementuj¹ca licznik FPS.
	/*! Metoda ta jest ju¿ wywo³ywana co klatkê podczas renderowania sceny.
	\sa m_uiFps. */
	inline GLvoid IncreaseFps()	{ ++m_uiFps; }

	/// Metoda zwracaj¹ca aktualn¹ wartoœæ licznika FPS.
	/*! \return Aktualna iloœæ klatek na sekundê.
	\sa m_uiFps. */
	inline GLuint GetFps()		{ return m_uiFps; }

	/// Metoda wyœwietlaj¹ca napis z aktualnym stanem licznika FPS.
	/*! Aktualny stan licznika wyœwietli siê tylko wtedy je¿eli flaga CFps::m_bShowFps bêdzie ustawiona na
	\p GL_TRUE.
	\sa m_uiFps. */
	inline GLvoid DrawFps() {
		if( !m_bShowFps )
			return;
		m_pSDLFont->DrawText( 10, 768, RGB( 255, 255, 255 ), m_cFpsText );
	}

	/// Metoda uaktywniaj¹ca (lub dezaktywuj¹ca) wyœwietlanie aktualnego stanu licznika FPS.
	/*! Metoda ustawia flagê CFps::m_bShowFps na \p GL_TRUE.
	\param[in] bShow Je¿eli \p GL_TURE (wartoœæ domyœlna), to wyœwietlanie FPSa zostanie aktywowane. Je¿eli \p GL_FALSE, to
	wyœwietlanie FPSa zostanie dezaktywowane.
	\sa m_uiFps. */
	inline GLvoid ShowFps( GLboolean bShow = GL_TRUE )		{ m_bShowFps = bShow; }

	/// Metoda sprawdzaj¹ca czy licznik FPS jest wyœwietlany.
	/*! \return \p GL_TRUE je¿eli licznik FPS jest wyœwietlany. W przeciwnym wypadku \p GL_FALSE.
	\sa m_uiFps. */
	inline GLboolean IsFps()	{ return m_bShowFps; }

private:
	CSDLFont	*m_pSDLFont;		///< WskaŸnik na obiekt klasy CSDLFont, za pomoc¹ którego rysowny jest tekst z aktualnym stanem licznika FPS.
	GLuint		m_uiFps;			///< Licznik FPS.
	GLboolean	m_bShowFps;			///< Flaga wskazuj¹ca czy aktualny stan licznika FPS ma myæ wyœwietlany.

	TCHAR		m_cFpsText[ 20 ];	///< Wyœwietlany napis z aktualnym stanem licznika FPS.
};