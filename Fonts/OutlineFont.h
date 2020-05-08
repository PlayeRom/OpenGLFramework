/*! \file
	\brief Plik nag³ówkowy dla klasy COutlineFont.
*/
#pragma once

/// Klasa umo¿liwiaj¹ca rysowanie tekstu czcionk¹ konturow¹.
class COutlineFont  
{
public:
	/// Konstruktor.
	/** \param[in] hDC Kontekst urz¹dzenia GDI (nale¿y pobraæ go z klasy CWindowData).
	\param[in] lpFontName Nazwa czcionki zarejestrowanej w systemie. */
	COutlineFont( HDC hDC, LPCTSTR lpFontName );

	/// Destruktor.
	/** Destruktor. */
	virtual ~COutlineFont();

	/// Metoda rysuj¹ca tekst.
	/** \param[in] fmt Tekst jaki chcemy wyœwietliæ.*/
	GLvoid DrawText( LPCTSTR fmt );

	/// Metoda rysuj¹ca tekst z mo¿liwoœci¹ formatowania.
	/** \param[in] fmt Tekst jaki chcemy wyœwietliæ.
	\param[in] ... Opcjonalane parametry dla formatowania. */
	GLvoid DrawTextFormat( LPCTSTR fmt, ... );

private:
	/// Metoda buduj¹ca czcionkê rastrow¹.
	/** \param[in] lpFontName Nazwa czcionki zarejestrowanej w systemie. */
	GLvoid BuildFont( LPCTSTR lpFontName );

	/// Metoda usuwaj¹ca zbudowan¹ czcionkê.
	GLvoid KillFont();

	GLuint	m_uiBase;				///< Identyfikator list wyœwietlania czcionki.
	HDC m_hDC;						///< Kontekst urz¹dzenia GDI.

	/// Tablica przechowuj¹ca informacje na temat znaków konturowych.
	GLYPHMETRICSFLOAT m_sGlyphMetric[ 256 ];

};