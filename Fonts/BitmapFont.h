/*! \file
	\brief Plik nag³ówkowy dla klasy CBitmapFont.
*/
#pragma once

/// Klasa umo¿liwiaj¹ca rysowanie tekstu czcionk¹ rastrow¹.
class CBitmapFont  
{
public:
	/// Konstruktor.
	/** \param[in] hDC Kontekst urz¹dzenia GDI (nale¿y pobraæ go z klasy CWindowData).
	\param[in] lpFontName Nazwa czcionki zarejestrowanej w systemie.
	\param[in] iHeight Wielkoœæ czcionki (nale¿y podawaæ w wartoœciach ujemnych).*/
	CBitmapFont( HDC hDC, LPCTSTR lpFontName, GLint iHeight = -24 );

	/// Destruktor.
	/** Destruktor. */
	virtual ~CBitmapFont();

	/// Metoda rysuj¹ca tekst.
	/** \param[in] fX Wspó³rêdna x po³o¿enia tekstu, gdzie 0, 0 to œrodek ekranu. Tekst jest rysowany na p³aszczyŸnie
	ekranu.
	\param[in] fY Wspó³rêdna y po³o¿enia tekstu, gdzie 0, 0 to œrodek ekranu. Tekst jest rysowany na p³aszczyŸnie
	ekranu.
	\param[in] fmt Tekst jaki chcemy wyœwietliæ.*/
	GLvoid DrawText( GLfloat fX, GLfloat fY, LPCTSTR fmt );

	/// Metoda rysuj¹ca tekst z mo¿liwoœci¹ formatowania.
	/** \param[in] fX Wspó³rêdna x po³o¿enia tekstu, gdzie 0, 0 to œrodek ekranu. Tekst jest rysowany na p³aszczyŸnie
	ekranu.
	\param[in] fY Wspó³rêdna x po³o¿enia tekstu, gdzie 0, 0 to œrodek ekranu. Tekst jest rysowany na p³aszczyŸnie
	ekranu.
	\param[in] fmt Tekst jaki chcemy wyœwietliæ.
	\param[in] ... Opcjonalane parametry dla formatowania. */
	GLvoid DrawTextFormat( GLfloat fX, GLfloat fY, LPCTSTR fmt, ... );

private:
	/// Metoda buduj¹ca czcionkê rastrow¹.
	/** \param[in] lpFontName Nazwa czcionki zarejestrowanej w systemie.
	\param[in] iHeight Wielkoœæ czcionki (nale¿y podawaæ w wartoœciach ujemnych).*/
	GLvoid BuildFont( LPCTSTR lpFontName, GLint iHeight = -24 );

	/// Metoda usuwaj¹ca zbudowan¹ czcionkê.
	GLvoid KillFont();

	GLuint	m_uiBase;	///< Identyfikator list wyœwietlania czcionki.
	HDC m_hDC;			///< Kontekst urz¹dzenia GDI.
};