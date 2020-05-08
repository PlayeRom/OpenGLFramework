/*! \file
	\brief Plik nag³ówkowy dla klasy CTextureFont.
*/
#pragma once

class CTextureLoader;

/// Klasa umo¿liwiaj¹ca rysowanie tekstu za pomoc¹ tekstury z czcionk¹.
class CTextureFont  
{
public:
	/// Konstruktor.
	/** Konstruktor. */
	CTextureFont();

	/// Destruktor.
	/** Destruktor. */
	virtual ~CTextureFont();

	/// Metoda rysj¹ca tekst z mo¿liwoœci¹ formatowania.
	/**	\param[in] fPosX Wspó³rzêdna x pozycji tekstu w przestrzeni 3D.
	\param[in] fPosY Wspó³rzêdna y pozycji tekstu w przestrzeni 3D.
	\param[in] iFont Rodzaj czcionki, do wyboru 0 i 1.
	\param[in] fR Sk³adowa R koloru tekstu.
	\param[in] fG Sk³adowa G koloru tekstu.
	\param[in] fB Sk³adowa B koloru tekstu.
	\param[in] fScale Wielkoœæ wyœwietlanej linijki tekstu.
	\param[in] lpFormat Tekst jaki chcemy wyœwietliæ.
	\param[in] ... Opcjonalne parametry dla formatowania. */
	GLvoid DrawTextFormat( GLfloat fPosX, GLfloat fPosY,
						   GLint iFont,
						   GLfloat fR, GLfloat fG, GLfloat fB,
						   GLfloat fScale, LPCTSTR lpFormat, ... );

	/// Metoda rysj¹ca tekst.
	/**	\param[in] fPosX Wspó³rzêdna x pozycji tekstu w przestrzeni 3D.
	\param[in] fPosY Wspó³rzêdna y pozycji tekstu w przestrzeni 3D.
	\param[in] lpText Tekst jaki chcemy wyœwietliæ.
	\param[in] iFont Rodzaj czcionki, do wyboru 0 i 1.
	\param[in] fR Sk³adowa R koloru tekstu.
	\param[in] fG Sk³adowa G koloru tekstu.
	\param[in] fB Sk³adowa B koloru tekstu.
	\param[in] fScale Wielkoœæ wyœwietlanej linijki tekstu. */
	GLvoid DrawText( GLfloat fPosX, GLfloat fPosY,
					 LPCTSTR lpText, GLint iFont,
					 GLfloat fR = 1.0f, GLfloat fG = 1.0f, GLfloat fB = 1.0f,
					 GLfloat fScale = 0.00002f );

private:
	CTextureLoader *m_pTextureLoader;		///< WskaŸnik na obiekt klasy  CTextureLoader.
	GLuint m_uiBase;						///< Identyfikator list wyœwietlania czcionki.

	/// Metoda buduj¹ca czcionkê rastrow¹.
	GLvoid BuildFont();
	GLvoid ScaleFont( GLfloat fScale );

	CVector2* m_aTextCoord;
	CVector2* m_aIndices;
};