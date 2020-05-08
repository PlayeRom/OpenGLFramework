/*! \file
	\brief Plik nag³ówkowy dla klasy CBillboard.
*/
#pragma once

/// Klasa rysuj¹ca billboardy.
/** Klasa ta zawiera metodê za pomoc¹ której mo¿emy narysowaæ tzw. billboard czyli sprite, który zawsze zwrócony
jest przodem do widza (kamery), nawet wtedy (a zw³aszcza wtedy) gdy kamera i/lub sprite zmienia swoje po³o¿enie. */
class CBillboard
{
public:
	/// Konstruktor.
	/** Konstruktor. */
	CBillboard();

	/// Destruktor.
	/** Destruktor. */
	~CBillboard();

	/// Metoda rysuj¹ca billboard.
	/** \param[in] in_cPos Centralny punkt po³o¿enia sprite'a w przestrzeni 3D.
	\param[in] fScale Rozmiar sprite'a, standardowo \p 1.0f, podaj¹c np. \p 2.0f sprite bêdzie dwa razy wiêkszy.
	\param[in] eMode Tryb rysowania sprite'a. Mo¿emy tu podaæ \p GL_TRIANGLE_STRIP (standardowo), wówczas narysowany
	zostanie quad lub \p GL_LINE_LOOP, gdzie narysowany zostanie kwadratowy kontur z linii.
	\param[in] fTexCoordX1 Koordynaty tekstury \a x1.
	\param[in] fTexCoordX2 Koordynaty tekstury \a x2.
	\param[in] fTexCoordY1 Koordynaty tekstury \a y1.
	\param[in] fTexCoordY2 Koordynaty tekstury \a y2.*/
	GLvoid DrawQuadBillboard( const CVector3 &in_cPos,
							  GLfloat fScale = 1.0f,
							  GLenum eMode = GL_TRIANGLE_STRIP,
							  GLfloat fTexCoordX1 = 0.0f,
							  GLfloat fTexCoordX2 = 1.0f,
							  GLfloat fTexCoordY1 = 0.0f,
							  GLfloat fTexCoordY2 = 1.0f );

	CVector2*	m_pTexCoord;
	CVector3*	m_pVertices;
};
