/*! \file
	\brief Plik nag³ówkowy dla klasy CARBMultiTexturing.
*/

#pragma once

#include "../OpenGLFramework/SingletonBase.h"

class CGameControl;

/// Klasa obs³uguj¹ca multiteksturing.
/** Klasa ta daje wsparcie do obs³ugi rozszerzenia GL_ARB_multitexture oraz zarz¹dza dostêpnymi jednostami
teksturuj¹cymi. */
class CARBMultiTexturing : public CSingletonBase <CARBMultiTexturing>
{
public:
	/// Statyczna metoda tworz¹ca obiekt klasy CARBMultiTexturing. 
	/*! CARBMultiTexturing jest singletonem, wiêc je¿eli wywo³amy tê metodê po raz drugi to zwróci ona wskaŸnik na
	obiekt ju¿ istniej¹cy, utworzony wczeœniej.
	\return WskaŸnik na obiekt CARBMultiTexturing. */
	static CARBMultiTexturing* Construct();

	/// Destruktor.
	/** Destruktor. */
	virtual ~CARBMultiTexturing();

	/// Metoda sprawdzaj¹ca czy multiteksturing jest wspierany przez kartê graficzn¹.
	/** \return \p GL_TRUE je¿eli multiteksturing jest wspierany. W przeciwnym wypadku \p GL_FALSE. */
	inline GLboolean IsSupported() { return m_bIsSupported; }

	/// Metoda ustawiaj¹ce tekstury do renderingu.
	/** Metodê tê nale¿y wywo³aæ przed rysowaniem obiektu, na który chcemy nalo¿yæ kilka tekstur.
	Warunkiem jest aby tekstury te by³y wczytane (w CGameControl::LoadAllTextures) jedna po drugiej.
	\param[in] iFirstTex Indeks pierwszej tekstury. 
	\param[in] iCount Iloœæ kolejnych tekstur jaka ma byæ aktywowana. Musi byæ co najmniej 2.
	\param[in] bEmbossBump Flaga wskazuj¹ca czy chcemy na³o¿yæ na obiekt teksturê bump. Je¿eli tak, to
	wymagane jest aby \a iCount by³ równy \p 2.
	\return \p GL_TRUE w przypadku powodzenia aktywowania tekstur. W przeciwnym wypadku \p GL_FALSE. */
	GLboolean BindMultiTextures( GLint iFirstTex, GLsizei iCount = 2, GLboolean bEmbossBump = GL_FALSE );

	/// Metoda aktywuj¹ca dan¹ jednostkê teksturuj¹c¹.
	/** \param[in] iIndex Indeks jednostki teksturuj¹cej, któr¹ chcemy aktywowaæ.
	\return \p GL_TRUE w przypadku powodzenia aktywacji. W przeciwnym razie \p GL_FALSE. */
	GLboolean ClientActiveTextureARB( GLint iIndex );

	/// Metoda aktywuj¹ca dan¹ jednostkê teksturuj¹c¹.
	/** \param[in] iIndex Indeks jednostki teksturuj¹cej, któr¹ chcemy aktywowaæ.
	\return \p GL_TRUE w przypadku powodzenia aktywacji. W przeciwnym razie \p GL_FALSE. */
	GLboolean ActiveTextureARB( GLint iIndex = 0 );

	/// Metoda dezaktywuj¹ca dan¹ jednostkê teksturuj¹c¹.
	/** \param[in] iIndex Indeks jednostki teksturuj¹cej, któr¹ chcemy dezaktywowaæ.
	\return \p GL_TRUE w przypadku powodzenia dezaktywacji. W przeciwnym razie \p GL_FALSE. */
	GLboolean InactiveTextureARB( GLint iIndex = 0 );

	/// Metoda wy³¹czaj¹ca multitekturing.
	/** Metoda ta dezaktywuje wszystkie jednostki teksturuj¹ce poza pierwsz¹. */
	GLvoid DisableMultiTeksturing();

	/// Metoda ustawiaj¹ca recznie koordynaty tekstury dla danej jednostki teksturuj¹cej.
	/** \param[in] iIndex Indeks jednostki teksturuj¹cej dla której chcemy ustawiæ koordydany.
	\param[in] s Koordynata tekstury s.
	\param[in] t Koordynata tekstury t.
	\return \p GL_TRUE w przypadku powodzenia ustawienia koordynatów tekstury. \p GL_FALSE w przypadku niepowodzenia.*/
	GLboolean MultiTexCoord2fARB( GLint iIndex, GLfloat s, GLfloat t );

	/// Metoda aktywuj¹ca tylko jedn¹ zadan¹ jednostkê teksturuj¹c¹.
	/** Metoda ta aktywuje tylko jedn¹ podan¹ jednostkê teksturuj¹c¹ a wszytskie pozosta³e dezaktywuje.
	\param[in] iIndex Index jednostki teksturuj¹cej jak¹ chcemy ukatywniæ.
	\return \p GL_TRUE w przypadku powodzenia. W przeciwnym razie \p GL_FALSE.*/
	GLboolean ActiveOnlyOneUnit( GLint iIndex );

	/// Metoda zwracaj¹ca iloœæ jednostek teksturuj¹cych.
	/** \return Liczba ca³kowita reprezentuj¹ca iloœæ jednostek teksturuj¹cych jakimi dysponuje karta graficzna. */
	inline GLint GetMaxTexelUnits() { return m_iMaxTexUnits; }

private:
	/// Konstruktor.
	/** Konstruktor. */
	CARBMultiTexturing();

	/// Metoda inicjalizuj¹ca multiteksturing.
	/** \return \p GL_TRUE w przypadku powodzenia inicjalizaji tj. wsparcia dla multiteksturingu. W przeciwnym wypadku
	\p GL_FALSE.*/
	GLboolean InitMultiTexturing();

	GLboolean m_bIsSupported;	///< Flaga wskazuj¹ca czy multiteksturing jest wspierany.
	GLint m_iMaxTexUnits;		///< Zmienna przechowuj¹ca liczbê iloœæ jednostek teksturuj¹cych.
	
	/// Zmienna zpamiêtuj¹ca iloœæ podanych tekstur w metodzie CARBMultiTexturing::BindMultiTextures.
	GLint m_iCountUnit;
};