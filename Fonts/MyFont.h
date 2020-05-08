/*! \file
	\brief Plik nag³ówkowy dla klasy CMyFont.
*/
#pragma once

/// Klasa rysuj¹ca tekst.
class CMyFont  
{
public:
	/// Konstruktor.
	/** Konstruktor. */
	CMyFont();

	/// Destruktor.
	/** Destruktor. */
	virtual ~CMyFont();

	/// Metoda rysuj¹ca tekst.
	/** \param[in] lpText Tekst jaki chcemy wyœwietliæ.
	\param[in] fX Wspó³rzêdna x pozycji tekstu (tekst rysowany jest w przestrzeni 3D).
	\param[in] fY Wspó³rzêdna y pozycji tekstu (tekst rysowany jest w przestrzeni 3D).
	\param[in] fZ Wspó³rzêdna z pozycji tekstu (tekst rysowany jest w przestrzeni 3D).
	\param[in] crColor Sk³adowe koloru tekstu. Podajemy u¿ywaj¹c makra \p RGB.
	\param[in] fSize Gruboœci tekstu (linii rysuj¹cej tekst).
	\param[in] fScale Rozmiar tekstu. */
	GLvoid DrawText(LPCTSTR lpText, GLfloat fX, GLfloat fY, GLfloat fZ, COLORREF crColor = RGB(255, 255, 255), GLfloat fSize = 1.0f, GLfloat fScale = 0.1f);

	/// Metoda rysuj¹ca tekst.
	/** \param[in] fX Wspó³rzêdna x pozycji tekstu (tekst rysowany jest w przestrzeni 3D).
	\param[in] fY Wspó³rzêdna y pozycji tekstu (tekst rysowany jest w przestrzeni 3D).
	\param[in] fZ Wspó³rzêdna z pozycji tekstu (tekst rysowany jest w przestrzeni 3D).
	\param[in] crColor Sk³adowe koloru tekstu. Podajemy u¿ywaj¹c makra \p RGB.
	\param[in] fSize Gruboœci tekstu (linii rysuj¹cej tekst).
	\param[in] fScale Rozmiar tekstu.
	\param[in] fmt Tekst jaki chcemy wyœwietliæ.
	\param[in] ... Opcjonalne parametry dla formatowania. */
	GLvoid DrawTextFormat(GLfloat fX, GLfloat fY, GLfloat fZ, COLORREF crColor, GLfloat fSize, GLfloat fScale, LPCTSTR fmt, ...);

private:
	/// Metoda rysuj¹ca znak.
	/** \param[in] iChar Znak do narysowania.
	\param[in] fX Wspó³rzêdna x pozycji tekstu.
	\param[in] fY Wspó³rzêdna y pozycji tekstu.
	\param[in] fZ Wspó³rzêdna z pozycji tekstu.
	\param[in] fScale Rozmiar tekstu. */
	GLvoid DrawCharacter(GLint iChar, GLfloat fX, GLfloat fY, GLfloat fZ, GLfloat fScale);
};