/*! \file
	\brief Plik nag³ówkowy dla klasy CEmbossBump.
*/
#pragma once

class CVector2;
class CVector3;
class CVector4;

/// Wartoœæ przesuniêcia tekstur dla uzyskania efektu emboss bump mapping.
#define MAX_EMBOSS 0.0025f

/// Klasa tworzenia efektu t³oczonego mapowania wybojów.
/** Zadaniem tej klasy jest obliczenie wartoœci tangent i binormal dla przesuniêcia koordynatów tekstur, utworzenie
tablicy nowych, przesuniêtych koordynatów tekstur, wsparcie dla rysowania mapowania wybojów.*/
class CEmbossBump
{
public:
	/// Konstruktor.
	/** Konstruktor. */
	CEmbossBump();

	/// Destruktor.
	/** Destruktor. */
	~CEmbossBump();

	/// Metoda mno¿¹ca macierz przez wektor.
	/** Metoda ta mno¿y macierz widoku modelu przez wektor pozycji œwiat³a i ustawia now¹ wartoœæ wektoru w zmiennej
	CEmbossBump::m_cLight.
	\param[in] in_cLightPos Pozycja œwiat³a, dla którego bêdziemy obliczali efekt mapowania wybojów.  */
	GLvoid VMatMult( const CVector4 &in_cLightPos );

	/// Metoda obliczaj¹ca wartoœci tangent i binormal dla obiektu 3D bez indeksowania wierzcho³ków.
	/** Motodê tê nale¿y wywo³aæ przed rysowaniem, np. w trakcie inicjalizacji programu.
	\param[in] iIndexObj Indeks obiektu 3D jaki chcemy narysowaæ. Indeksy liczone s¹ od \p 0 w kolejnoœci
	wczytywania	obiektów, tj. w kolejnoœci wywo³ywania metody C3DObjManager::Create3DObject. 
	\param[in] iFirstVertex Indeks pierwszego wierzcho³ka danego obiektu z tablicy wierzcho³ków CVertexArray. 
	\param[in] iCountVertices Iloœæ wierzcho³ków przypadaj¹ca na dany obiekt. 
	\return Indeks tablicy CEmbossBump::m_aEmbossBumpData przechowuj¹cej obliczone wartoœci tangent i binormal. */
	GLint CalcTangents( GLint iIndexObj, GLint iFirstVertex, GLsizei iCountVertices );

	/// Metoda ustawiaj¹ca koordynaty przesuniêcia tekstury dla obiektu 3D bez indeksowania wierzcho³ków.
	/** Metodê tê nale¿y wywo³aæ za ka¿dym razem gdy chcemy narysowaæ obiekt z efektem bump mappingu.
	\param[in] iIndexObj Indeks obiektu 3D jaki chcemy narysowaæ. Indeksy liczone s¹ od \p 0 w kolejnoœci
	wczytywania	obiektów, tj. w kolejnoœci wywo³ywania metody C3DObjManager::Create3DObject. 
	\param[in] iFirstVertex Indeks pierwszego wierzcho³ka danego obiektu z tablicy wierzcho³ków CVertexArray. 
	\param[in] iCountVertices Iloœæ wierzcho³ków przypadaj¹ca na dany obiekt. */
	GLvoid SetUpBumps( GLint iIndexObj, GLint iFirstVertex, GLsizei iCountVertices );

	/// Metoda obliczaj¹ca wartoœci tangent i binormal dla obiektu 3D z indeksowaniem wierzcho³ków.
	/** Motodê tê nale¿y wywo³aæ przed rysowaniem, np. w trakcie inicjalizacji programu.
	\param[in] iIndexObj Indeks obiektu 3D jaki chcemy narysowaæ. Indeksy liczone s¹ od \p 0 w kolejnoœci
	wczytywania	obiektów, tj. w kolejnoœci wywo³ywania metody C3DObjManager::Create3DObject. 
	\param[in] iCountVertices Iloœæ wierzcho³ków przypadaj¹ca na dany obiekt. 
	\return Indeks tablicy CEmbossBump::m_aEmbossBumpData przechowuj¹cej obliczone wartoœci tangent i binormal. */
	GLint CalcTangentsElem( GLint iIndexObj, GLsizei iCountVertices );

	/// Metoda ustawiaj¹ca koordynaty przesuniêcia tekstury dla obiektu 3D z indeksowaniem wierzcho³ków.
	/** Metodê tê nale¿y wywo³aæ za ka¿dym razem gdy chcemy narysowaæ obiekt z efektem bump mappingu.
	\param[in] iIndexObj Indeks obiektu 3D jaki chcemy narysowaæ. Indeksy liczone s¹ od \p 0 w kolejnoœci
	wczytywania	obiektów, tj. w kolejnoœci wywo³ywania metody C3DObjManager::Create3DObject. 
	\param[in] iCountVertices Iloœæ wierzcho³ków przypadaj¹ca na dany obiekt. */
	GLvoid SetUpBumpsElem( GLint iIndexObj, GLsizei iCountVertices );

	/// \name Zestaw metod do pobierania danych o koordynatach tekstury dla obiektu bez indeksowania wierzcho³ków.
	//@{

	/// Metoda pobieraj¹ca tablicê przesuniêtych koordynatów tekstury.
	/** \return WskaŸnik na tablicê CEmbossBump::m_aTexCoords. */
	inline GLvoid* GetTexCoordsArray() { return &m_aTexCoords[ 0 ]; }

	/// Metoda pobieraj¹ca iloœæ przesuniêtych koordynatów tekstury.
	/** \return Liczba ca³kowita okreœlaj¹ca wielkoœæ tablicy CEmbossBump::m_aTexCoords. */
	inline GLint GetTexCoordsSize() { return static_cast<GLint>( m_aTexCoords.size() ); }

	/// Metoda pobieraj¹ca koordynaty przesuniêtej tekstury dla danego wierzcho³ka.
	/** \param[in] iIndex Indeks tabicy przesuniêtych koordynatów testury, z którego chcemy pobraæ dane koordynaty.
	\return Obiekt klasy CVector2 zawieraj¹cy koordynaty tekstury. */
	CVector2 GetTexCoord( GLint iIndex );
	///@}

	/// \name Zestaw metod do pobierania danych o koordynatach tekstury dla obiektu bez indeksowania wierzcho³ków.
	//@{

	/// Metoda pobieraj¹ca tablicê przesuniêtych koordynatów tekstury.
	/** \return WskaŸnik na tablicê CEmbossBump::m_aTexCoordsElem. */
	inline GLvoid* GetTexCoordsArrayElem() { return &m_aTexCoordsElem[ 0 ]; }

	/// Metoda pobieraj¹ca iloœæ przesuniêtych koordynatów tekstury.
	/** \return Liczba ca³kowita okreœlaj¹ca wielkoœæ tablicy CEmbossBump::m_aTexCoordsElem. */
	inline GLint GetTexCoordsElemSize() { return static_cast<GLint>( m_aTexCoordsElem.size() ); }

	/// Metoda pobieraj¹ca koordynaty przesuniêtej tekstury dla danego wierzcho³ka.
	/** \param[in] iIndex Indeks tabicy przesuniêtych koordynatów testury, z którego chcemy pobraæ dane koordynaty.
	\return Obiekt klasy CVector2 zawieraj¹cy koordynaty tekstury. */
	CVector2 GetTexCoordElem( GLint iIndex );
	///@}
	
	/// Metoda czyszcz¹ca wszelkie tablice.
	/** Metoda usuwa z pamiêci wszelkie obliczone tangentsy i binormale oraz koordynaty przesuniêcia tekstury. */
	GLvoid ClearAllArrays();

private:
	/// Struktura przechowuj¹ca wyliczone wartoœci tangent i binormal dla danego obiektu 3D.
	/** Struktura ta przechowuje tablice wyliczonych wartoœci tangent i binormal dla ka¿dego wierzcho³ka
	danego obiektu 3D. */
	struct SEmbossBumpData
	{
		GLint iIndexObj;					///< Indeks obiektu 3D wczytanego z pliku \a 3DObj.
		std::vector<CVector3> aTangent;		///< Tablica wyliczonych wektorów tangent dla ka¿dego wierzcho³ka obiektu.
		std::vector<CVector3> aBinormal;	///< Tablica wyliczonych wektorów binormal dla ka¿dego wierzcho³ka obiektu.
	};

	CVector4 m_cLight;			///< Pomocniczy wektor œwiat³a do obliczenia przesuniêcia koordynatów tekstury.
	GLboolean m_bIsSetLight;	///< Flaga wskazuj¹ca czy wywo³ano metodê CEmbossBump::VMatMult ustawiaj¹c¹ wektor CEmbossBump::m_cLight.

	/// Tablica obiektów struktury CEmbossBump::SEmbossBumpData.
	std::vector<SEmbossBumpData> m_aEmbossBumpData;

	/// Metoda zwracaj¹ca iloœæ elementów tablicy CEmbossBump::m_aEmbossBumpData.
	inline GLint GetEmbossBumpDataSize() { return static_cast<GLint>( m_aEmbossBumpData.size() ); }

	/// Tablica przesuniêtych koordynatów tekstury bump dla rysowania glDrawArrays.
	std::vector<CVector2> m_aTexCoords;

	/// Tablica przesuniêtych koordynatów tekstury bump dla rysowania glDrawElements.
	std::vector<CVector2> m_aTexCoordsElem;
};
