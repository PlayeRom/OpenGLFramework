/*! \file
	\brief Plik nag³ówkowy dla klasy CBrightness.
*/
#pragma once

#include "../OpenGLFramework/SingletonBase.h"

/// Klasa kontroli jasnoœci ekranu.
/** Zadaniem tej klasy jest rysowanie prostok¹ta na ca³ym ekranie z odpowienio ustawion¹ przezroczystoœci¹
oraz kolorem dla uzyskania efektu rozjaœnienia lub przyciemnienie ekranu. Obiekt tej klasy jest ju¿ tworzony w
CGameControl oraz „rysowanie jasnoœci” tak¿e jest ju¿ wywo³ywane w CGameControl::Draw. */
class CBrightness : public CSingletonBase <CBrightness>
{
public:
	/// Statyczna metoda tworz¹ca obiekt klasy CBrightness. 
	/*! CBrightness jest singletonem, wiêc je¿eli wywo³amy tê metodê po raz drugi to zwróci ona wskaŸnik na obiekt
	ju¿ istniej¹cy, utworzony wczeœniej.
	\return WskaŸnik na obiekt CBrightness. */
	static CBrightness* Construct();

	/// Destruktor.
	/** Destruktor. */
	~CBrightness();

	/// Metoda ustawiaj¹ca wartoœæ jasnoœci ekranu.
	/** \param[in] iBrightness Wartoœæ jasnoœci od \p -255 do \p 255, gdzie \p -255 to ca³kowite przyciemnienie
	(obraz bêdzie czarny) a \p 255 to ca³kowite rozjaœnienie (obraz bêdzie bia³y). Podaj¹c \p 0 (wartoœæ domyœlna)
	obraz bêdzie rysowany w oryginalnej jasnoœci. */
	GLvoid SetBrightness( GLint iBrightness = 0 );

	/// Metoda zwracaj¹ca aktualnie ustawion¹ wartoœæ jasnoœci ekranu.
	/** \return Liczba ca³kowita z przedzia³u od \p -255 do \p 255 okreœlaj¹ca aktualn¹ wartoœæ jasnoœci ekranu. */
	inline GLint GetBrightness() { return m_iBrightness; }

	/// Metoda rysuj¹ca "prostok¹t jasnoœci".
	/** Metoda ta jest ju¿ wywo³ywana w CGameControl::Draw. */
	GLvoid Draw();

private:
	/// Konstruktor.
	/** Konstruktor. */
	CBrightness();

	/// Zmienna przechowuj¹ca aktualn¹ wartoœæ jasnoœci.
	/** Wartoœæ jasnoœci od \p -255 do \p 255, gdzie \p -255 to ca³kowite przyciemnienie (obraz bêdzie czarny) 
	a \p 255 to ca³kowite rozjaœnienie (obraz bêdzie bia³y). W zmiennej ten ustotna jest mo¿liwoœæ usawienia wartoœci
	ujemnej, która wskazuje na przyciemnienie obrazu. */
	GLint m_iBrightness;

	/// Zmienna przechowuj¹ca aktualn¹ wartoœæ przezroczystoœci dla rysowania.
	/** Wartoœæ przeroczystoœci od \p 0 do \p 255, gdzie \p 0 to ca³kowita przezroczystoœæ (jasnoœæ bez z mian) 
	a \p 255 to	ca³kowite rozjaœnienie lub przyciemnienie. */
	GLubyte m_ubBrightness;
};
