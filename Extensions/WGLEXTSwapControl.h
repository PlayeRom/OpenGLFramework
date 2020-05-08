/*! \file
	\brief Plik nag³ówkowy dla klasy CWGLEXTSwapControl.
*/

#pragma once

#include "../OpenGLFramework/SingletonBase.h"

/// Klasa kontroluj¹ca synchronizacjê renderowania klatek z odœwie¿aniem ekranu monitora.
/** Za pomoc¹ tej klasy kontrolujemy synchronizacjê pionow¹ dla aplikacji. */

class CWGLEXTSwapControl : public CSingletonBase <CWGLEXTSwapControl>
{
public:
	/// Statyczna metoda tworz¹ca obiekt klasy CWGLEXTSwapControl. 
	/** CWGLEXTSwapControl jest singletonem, wiêc je¿eli wywo³amy tê metodê po raz drugi to zwróci ona wskaŸnik
	na obiekt ju¿ istniej¹cy, utworzony wczeœniej.
	\return WskaŸnik na obiekt CWGLEXTSwapControl. */
	static CWGLEXTSwapControl* Construct();

	/// Destruktor.
	/** Destruktor. */
	~CWGLEXTSwapControl();

	/// Metoda sprawdzaj¹ca czy WGL_EXT_swap_control jest wspierany.
	/** \return \p GL_TRUE je¿eli WGL_EXT_swap_control jest wspierany. W przeciwnym wypadku \p GL_FALSE. */
	inline GLboolean IsSupported() { return m_bIsSupported; }

	/// Metoda w³¹czaj¹ca / wy³¹czajaca synchronizacjê pionow¹ wedle uznania.
	/** \param[in] iInterval Nowa wartoœæ interwa³u. */
	GLvoid SetSwapInterval( GLint iInterval );

	/// Metoda ustawiaj¹ca synchronizacjê pionow¹ na domyœln¹.
	GLvoid DefaultVSync();

	/// Metoda w³¹czaj¹ca synchronizacjê pionow¹.
	/** Metoda ustawia interwa³ na \p 1. */
	GLvoid EnableVSync();

	/// Metoda wy³¹czaj¹ca synchronizacjê pionow¹.
	/** Metoda ustawia interwa³ na \p 0. */
	GLvoid DisableVSync();

private:
	/// Konstruktor.
	/** Konstruktor. */
	CWGLEXTSwapControl();

	/// Metoda inicjalizuj¹ca WGL_EXT_swap_control.
	/** \return \p GL_TRUE w przypadku powodzenia inicjalizaji tj. wsparcia dla WGL_EXT_swap_control. W przeciwnym
	wypadku	\p GL_FALSE.*/
	GLboolean InitSwapControl();

	GLboolean m_bIsSupported;			///< Flaga wskazuj¹ca czy WGL_EXT_swap_control jest wspierany.
	GLint m_iDefaultSwapInterval;		///< Zmienna zapamiêtuj¹ca domyœln¹ wartoœæ interwa³u.
};
