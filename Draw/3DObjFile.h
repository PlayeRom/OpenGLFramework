/*! \file
	\brief Plik nag³ówkowy dla klasy C3DObjFile.
*/
#pragma once

#ifdef _3DSLOADER_
	#include "3DSLoader.h"
	class CVector3;
#endif

class CGameControl;

/// Nag³owek pliku \p 3DObj jako identyfikator pliku.
#define _3DOBJFILE_HEADER "OGLF PlayeRom"

/// G³owna wersja pliku \p 3DObj.
#define _3DOBJFILEVERSION_MAJOR 1

/// Pomniejsza wersja pliku \p 3DObj.
#define _3DOBJFILEVERSION_MINOR 0


#define INT_TO_FIXED( x )	GLint( x << 16 )
#define FIXED_TO_INT( x )	GLint( x >> 16 )

#define FLOAT_TO_FIXED( x )	GLint( 65536.0f * x )
#define FIXED_TO_FLOAT( x )	( GLfloat( x ) * ( 1.0f / 65536.0f ) )

#define FIXED_ONE			INT_TO_FIXED( 1 )


/// Klasa zapisu i odczytu pliku \a 3DObj.
/** Za pomoc¹ tej klasy C3DObjManager wykonuje operacje wczytania i zapisu plików \a 3DObj, reprezentuj¹cych
trójwymiarowe obiekty. */
class C3DObjFile
{
public:
	/// Konstruktor.
	/** \param[in] lpFileName Nazwa pliku \a 3DObj wraz ze œcie¿k¹. Je¿eli podamy nazwê pliku, to plik ten od razu
	zostanie wczytany. Mo¿emy tak¿e podaæ tutaj NULL, wówczas zostanie tylko	stworzony obiekt klasy. */
	C3DObjFile( LPCTSTR lpFileName = NULL );

	/// Destruktor.
	/** Destruktor. */
	~C3DObjFile();

#ifdef _3DSLOADER_
	GLboolean GetFileNameSaveObject( LPCTSTR lpFileName, TCHAR* out_cFileName, GLboolean bFixed );

	GLboolean SaveObjectNoElem( LPCTSTR lpFileName, const S3DSObject &in_s3DSObject, const CVector3 &in_cScale,
								const CVector3 &in_cOffsetVector, const CVector3 &in_cRot90,
								GLboolean bSmoothNormal, GLboolean bFixed = GL_FALSE );
	GLboolean SaveObjectElem( GLint iIndexObject, LPCTSTR lpFileName, GLboolean bFixed = GL_FALSE );
#endif

	/// Metoda wczytuj¹ca obiekt 3D z pliku \a 3DObj.
	/** \param[in] lpFileName Nazwa pliku \a 3DObj wraz ze œcie¿k¹, który chcemy wczytaæ.
	\param[out] out_bIsElem Do zmiennej tej zostanie zapisana wartoœæ \p GL_TRUE je¿eli wczytywany obiekt korzysta
	z indeksowania wierzcho³ków. Je¿eli nie, to zmienna ta zostanie ustawiona na \p GL_FALSE.
	\return \p GL_TRUE w przypadku powodzenia wczytania pliku \a 3DObj. W przeciwnym wypadku \p GL_FALSE. */
	GLboolean LoadObject( LPCTSTR lpFileName, GLboolean &out_bIsElem );

	/// Metoda zwracaj¹ca iloœæ wierzcho³ków ostatnio wczytanego pliku.
	/** \return Liczba ca³kowita okreœlaj¹ca iloœæ wierzcho³ków jak¹ zawiera obiekt z ostatnio wczytanego pliku	\a 3DObj.
	\sa LoadObject, m_iVerticesNum. */
	inline GLint GetVerticesNum() { return m_iVerticesNum; }

private:
	CGameControl	*m_pGameCtrl;	///< WskaŸnik na obiekt klasy CGameControl.
	GLint			m_iVerticesNum;	///< Zmienna przechowuj¹ca iloœæ wierzcho³ków jak¹ zawiera obiekt z ostatnio wczytanego pliku.

#ifdef _3DSLOADER_
	/// Metoda zapisuj¹ca nag³ówek pliku \a 3DObj.
	/** \param[in] hFile WskaŸnik na uchwyt do wczeœniej otwartego pliku \a 3DObj. Nie mo¿e byæ NULL.
	\param[in] bIsElem Flaga wskazuj¹ca czy zapisywany obiekt 3D korzysta z indeksowania wierzcho³ków.
	\return \p GL_TRUE w przypadku powodzenia zapisu nag³ówka. W przeciwnym wypadku \p GL_FALSE.*/
	GLboolean SaveHeader( FILE *hFile, GLboolean bIsElem );

	GLvoid SaveVertex( FILE* hFile, const CVector3& in_cVertex, const CVector2& in_cTexCoord, const CVector3& in_cNormal, GLboolean bFixed = GL_FALSE );
	GLvoid SaveFixedVector( FILE* hFile, const CVector3& in_cVec );
	GLvoid SaveFixedVector( FILE* hFile, const CVector2& in_cVec );
#endif

	/// Metoda wczytuj¹ca nag³ówek pliku \a 3DObj.
	/** \param[in] hFile WskaŸnik na uchwyt do wczeœniej otwartego pliku \a 3DObj. Nie mo¿e byæ NULL.
	\param[in] lpFileName Nazwa pliku \a 3DObj wraz ze œcie¿k¹, z którego chcemy wczytaæ nag³ówek.
	\param[out] out_bIsElem Do zmiennej tej zostanie zapisana wartoœæ \p GL_TRUE je¿eli wczytywany obiekt korzysta
	z indeksowania wierzcho³ków. Je¿eli nie, to zmienna ta zostanie ustawiona na \p GL_FALSE.
	\return \p GL_TRUE w przypadku powodzenia wczytania nag³ówka. W przeciwnym wypadku \p GL_FALSE. */
	GLboolean LoadHeader( FILE *hFile, LPCTSTR lpFileName, GLboolean &out_bIsElem );

	/// Metoda wczytuj¹ca dane z pliku \a 3DObj dla obiektu 3D bez indeksowania wierzcho³ków.
	/** \param[in] hFile WskaŸnik na uchwyt do wczeœniej otwartego pliku \a 3DObj. */
	GLboolean LoadDataNoElem( FILE *hFile );

	/// Metoda wczytuj¹ca dane z pliku \a 3DObj dla obiektu 3D z indeksowaniem wierzcho³ków.
	/** \param[in] hFile WskaŸnik na uchwyt do wczeœniej otwartego pliku \a 3DObj. */
	GLboolean LoadDataElem( FILE *hFile );
};
