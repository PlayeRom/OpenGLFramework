/*! \file
	\brief Plik nag³ówkowy dla klasy CStencilShadow.
*/
#pragma once

/// Klasa wspomagaj¹ca rysowanie dynamicznych cieni shadow volume.
/** Klasa udostêpnia metody rysuj¹ce cieñ w oparciu o bufor szablonowy. */
class CStencilShadow
{
public:
	/// Konstruktor.
	/** Konstruktor. */
	CStencilShadow();

	/// Destruktor.
	/** Destruktor. */
	~CStencilShadow();

	/// Metoda restartuj¹ca wszelkie niezbêdne dane.
	/** Metoda ta jest ju¿ wywo³ywana w trakcie restartowania okna aplikacji. */
	GLvoid RestartObjects();
	
	/// Metoda obliczaj¹ca równanie p³aszczyzny dla danego obiektu 3D bez indeksowania wierzcho³ków.
	/** Metoda ta dla ka¿dego trój¹ta obiektu 3D, oblicza równanie p³aszczyzny. Wyniki obliczeñ zapisywane s¹ w
	dynamicznie tworzonej tablicy CStencilShadow::m_aObjFaces. Metoda ta jest ju¿ wywo³ywana dla ka¿dego wczytywanego
	obiektu w C3DObjManager::Create3DObject.
	\param[in] iIndexObj Indeks obiektu 3D wczytanego z pliku \a 3DObj.
	\param[in] iFirstVertex Indeks pierwszego wierzcho³ka obiektu 3D z tablicy wierzcho³ków CVertexArray.
	\param[in] iCountVertices Iloœæ wierzcho³ków jak¹ zawiera obiekt 3D. */
	GLvoid CreateFaceEquation( GLint iIndexObj, GLint iFirstVertex, GLsizei iCountVertices );

	/// Metoda obliczaj¹ca równanie p³aszczyzny dla danego obiektu 3D z indeksowaniem wierzcho³ków.
	/** Metoda ta dla ka¿dego trój¹ta obiektu 3D, oblicza równanie p³aszczyzny. Wyniki obliczeñ zapisywane s¹ w
	dynamicznie tworzonej tablicy CStencilShadow::m_aObjFaces. Metoda ta jest ju¿ wywo³ywana dla ka¿dego wczytywanego
	obiektu w C3DObjManager::Create3DObject.
	\param[in] iIndexObj Indeks obiektu 3D wczytanego z pliku \a 3DObj.
	\param[in] iCountVertices Iloœæ wierzcho³ków jak¹ zawiera obiekt 3D. */
	GLvoid CreateFaceEquationElem( GLint iIndexObj, GLsizei iCountVertices );

	/// Metoda obliczajac¹ pozycjê œwiat³a niezbêd¹ dla narysowania cienia.
	/** Metodê t¹ nale¿y wywo³aæ po przekszta³ceniu macierzy widoku w ca³kowicie odwrotny sposób od tego jak
	byœmy przekszta³cali dla normalnego rysowania obiektu.
	\param[in] in_cLightPos Oryginalna pozycja œwiat³a w przestrzeni 3D. 
	\return Nowa pozycja œwiat³a wyliczona z odróconej macierzy widoku. */
	CVector4 GetLightPos( const CVector4 &in_cLightPos = CVector4( 0.0f, 0.0f, 0.0f, 1.0f ) );

	/// Metoda rysuj¹ca cieñ dla danego obiektu 3D.
	/** \param[in] iIndexObj Indeks obiektu 3D wczytanego z pliku \a 3DObj.
	\param[in] in_cLightPos Pozycja œwiat³a wyliczona metod¹ CStencilShadow::GetLightPos. */
	GLvoid DrawShadowForObj( GLint iIndexObj, const CVector4 &in_cLightPos );

	/// Metoda rysuj¹ca cienie dla kilku obiektów na raz.
	/** Aby skorzystaæ z tej metody nale¿y uprzednio, odpowiednio wywo³aæ seriê metod \a ..._ObjTransfInfo.
	\sa Begin_ObjTransfInfo, Set_ObjTransfInfo, Translate_ObjTransfInfo, Rotate_ObjTransfInfo, End_ObjTransfInfo. */
	GLvoid DrawShadowForObjects();

	/// \name Zestaw metod do rysowania cieni dla kilku obiektów na raz.
	//@{

	/// Metod inicjalizuj¹ca zbieranie informacji o przekszta³ceniach macierzy widoku dla kilku obiektów 3D.
	/** Metodê t¹ nale¿y wywo³aæ na samym pocz¹tku rysowania danych obiektów 3D, które maj¹ rzucaæ cieñ.
	\sa End_ObjTransfInfo. */
	GLvoid Begin_ObjTransfInfo();

	/// Metoda ustawiaj¹ca indeks obiektu 3D oraz pozycjê œwiat³a.
	/** Przed wywyo³aniem tej metody nale¿y uprzednio wywo³aæ CStencilShadow::Begin_ObjTransfInfo.
	\param[in] iIndexObj Indeks obiektu 3D wczytanego z pliku \a 3DObj, który bêdzie rzuca³ cieñ.
	\param[in] in_cLightPos Pozycja œwiat³a wyliczona z odwonie przekszta³conej macierzy widoku.
	\sa GetLightPos. */
	GLvoid Set_ObjTransfInfo( GLint iIndexObj, const CVector4 &in_cLightPos );

	/// Metoda zapamietuj¹ca pojedyncz¹ opracjê translacji.
	/** Przed wywyo³aniem tej metody nale¿y uprzednio wywo³aæ CStencilShadow::Begin_ObjTransfInfo.
	Metodê tê nale¿y wywo³aæ zawsze wraz z wykonaniem operacji glTranslate podczas normalnego rysowania obiektu 3D.
	\param[in] in_cTranslate Wektor o jaki zostanie wykonane przesuniêcie, zgodnie z glTranslate. */
	GLvoid Translate_ObjTransfInfo( const CVector3 &in_cTranslate );

	/// Metoda zapamietuj¹ca pojedyncz¹ opracjê rotacji.
	/** Przed wywyo³aniem tej metody nale¿y uprzednio wywo³aæ CStencilShadow::Begin_ObjTransfInfo.
	Metodê tê nale¿y wywo³aæ zawsze wraz z wykonaniem operacji glRotate podczas normalnego rysowania obiektu 3D.
	\param[in] fAngle K¹t o jaki zostanie wykonany obrót, zgodnie z glRotate.
	\param[in] in_cAxesRot Wektor wskazuj¹cy, wokó³ których osi zostanie wykonany obrót - zgodnie z tym co podajemy w 
	glRotate. */
	GLvoid Rotate_ObjTransfInfo( GLfloat fAngle, const CVector3 &in_cAxesRot );

	/// Metoda zapamietuj¹ca pojedyncz¹ opracjê skalowania.
	/** Przed wywyo³aniem tej metody nale¿y uprzednio wywo³aæ CStencilShadow::Begin_ObjTransfInfo.
	Metodê tê nale¿y wywo³aæ zawsze wraz z wykonaniem operacji glScale podczas normalnego rysowania obiektu 3D.
	\param[in] in_cScale Wektor o jaki zostanie wykonane skalowanie, zgodnie z glScale. */
	GLvoid Scale_ObjTransfInfo( const CVector3 &in_cScale );

	/// Metod zamykajaca zbieranie informacji o przekszta³ceniach macierzy widoku dla kilku obiektów 3D.
	/** Metodê t¹ nale¿y wywo³aæ na samym koñcu rysowania danych obiektów 3D, które maj¹ rzucaæ cieñ.
	\sa Begin_ObjTransfInfo. */
	GLvoid End_ObjTransfInfo();

	/// Metoda czyszcz¹ca wszelkie tablice zwi¹zane ze zbieraniem informacji o przekszta³ceniach macierzy widoku dla kilku obiektów 3D.
	GLvoid ClearObjTransformInfo();
	//@}

private:
	enum EDrawArrayMode
	{
		EFloat,
		EInt
	};

	/// Struktura równania p³aszczyzny.
	/** Struktura ta przechowuje wszelkie informacje o danym trójk¹cie obiektu 3D potrzebnych do wyliczenia cieni. */
	struct SFaceEquation
	{
		/// Tablica przechowuj¹ca indeksy trzech wierzcho³ków.
		/** Tablica przechowuj¹ca indeksy (z tablicy CVertexArray::m_aShortIndices) trzech wierzcho³ków tworz¹cych
		trójk¹, dla którego obliczane jest rówanie p³aszczyzny. */
		GLint aIndexFace[ 3 ];

		/// Dane dla rownania plaszczyzny ax + by + cz + d = 0.
		CVector4 cFaceEq;
		//GLfloat a, b, c, d;
		GLboolean bVisible;		///< Flaga wskazuj¹ca czy trójk¹t jest oœwietlony.
		GLint iNeighbour[ 3 ];	///< Indeksy innych trójk¹tów s¹siaduj¹cych z dan¹ krawêdzi¹ trójk¹ta.
	};

	/// Struktura trójkatów obiektu 3D.
	/** Struktura ta przechowuje niezbêdne informacje o wszystkich trójk¹tach dla danego obiektu 3D. */
	struct SObjFaces
	{
		GLint iIndexObj;		///< Index obiektu któremu przynale¿y dana stuktura.
		GLboolean bIsElem;		///< Flaga wskazuj¹ca czy dla danego obiektu 3D u¿ywane jest indeksowanie wierzcho³ków.
		std::vector<SFaceEquation> aFaceArray; ///< Tablica wszystkich trójkatów z jakich sk³¹da siê obiekt 3D.
	};

	//idea jest taka aby wywolac operacje na stencil buforze tylko raz, i pomiedzy 
	//DECR i INCR rysowac wszytskie obiekty na raz, ktore maja rzucic cien.
	//Dlatego potrzebujemy struktury, ktora przechowa wszelkie informacje o
	//przeksztalceniach dla danego obiektu

	/// Typ wyliczeniowy rodzajów przekszta³ceñ macierzy widoku.
	enum ETrnasformMode
	{
		ETransfTranslate,
		ETransfRotate,
		ETransfScale
	};

	/// Struktura przechowuj¹ca pojedyncz¹ informacjê o przekszta³ceniach macierzy widoku.
	struct STransform
	{
		ETrnasformMode eTransfMode;		///< Rodzaj przekszta³cenia.
		CVector3 cTransform;			///< Wektor przesuniêcia, osie obrotu lub wartoœci skalowania.
		GLfloat fAngleRot;				///< K¹t obrotu, je¿eli eTransfMode = ETrnasformMode::ETransfRotate.
	};

	/// Struktura przechowuj¹ca wszelkie informacje o przekszta³ceniach macierzy widoku.
	/** Struktura przechowuj¹ca wszelkie informacje o przekszta³ceniach macierzy widoku dla danego obiektu 3D. */
	struct SObjectTransformInfo
	{
		GLint iIndexObj;		///< Indeks obiektu 3D wczytanego z pliku \a 3DObj.
		CVector4 cLightPos;		///< Pozycja œwiat³a wyliczona przez CStencilShadow::GetLightPos.
		std::vector<STransform> aTransform;	///< Tablica wszelkich przekszta³aceñ macierzy widoku.
	};

	std::vector< SObjFaces > m_aObjFaces;		///< Tablica trójk¹tów dla danego obiektu 3D.

	/// Metoda zwracaj¹ca iloœæ elementów tablicy CStencilShadow::m_aObjFaces.
	/** \return Liczba ca³kowita okreœlaj¹ca iloœæ elementów tablicy CStencilShadow::m_aObjFaces. */
	inline GLint GetObjFacesSize() { return static_cast<GLint>( m_aObjFaces.size() ); }

	/// Metoda pobieraj¹ca iloœæ trójk¹tów danego obiektu 3D.
	/** \param[in] iIndex Indeks tablicy CStencilShadow::m_aObjFaces z którego chcemy pobtaæ iloœæ trójkatów. */
	inline GLint GetFaceArraySize( GLint iIndex )	{ 
		return static_cast<GLint>( m_aObjFaces[ iIndex ].aFaceArray.size() );
	}

	/// Metoda czyszcz¹ca tablicê CStencilShadow::m_aObjFaces i jego podtablice.
	GLvoid ClearObjFaces();

	SObjectTransformInfo m_sObjTransfInfo;	///< Obiekt struktury CStencilShadow::SObjectTransformInfo.
	std::vector<SObjectTransformInfo> m_aObjTransformInfo; ///< Tablica przechowuj¹ca informacje o wszelkch przekszta³ceniach macierzy widoku.

	/// Metoda zwracajaca wielkoœæ tablicy CStencilShadow::m_aObjTransformInfo.
	/** \return Liczba ca³kowita okreœlaj¹ca iloœæ elementów tablicy CStencilShadow::m_aObjTransformInfo. */
	inline GLint GetObjTransformInfoSize() { return static_cast<GLint>( m_aObjTransformInfo.size() ); }

	/// Metoda wyszuj¹ca s¹siaduj¹ce trójkaty.
	/** \param[in] iIndex Indeks tablicy CStencilShadow::m_aObjFaces obiektu 3D, dla którego trójkatów bêd¹
	wyszukiwanie s¹siadu¹ce trójk¹ty. */
	GLvoid CalculateNeighbours( GLint iIndex );

	/// Metoda rysuj¹ca cieñ dla obiektu 3D z indeksowaniem wierzcho³ków.
	/** \param[in] iIndexObj Indeks obiektu 3D wczytanego z pliku \a 3DObj.
	\param[in] in_cLightPos Pozycja œwiat³a wyliczona metod¹ CStencilShadow::GetLightPos. */
	GLvoid DoDrawShadowForObjElem( GLint iIndexObj, const CVector4 &in_cLightPos );

	/// Metoda rysuj¹ca cieñ dla obiektu 3D bez indeksowania wierzcho³ków.
	/** \param[in] iIndexObj Indeks obiektu 3D wczytanego z pliku \a 3DObj.
	\param[in] in_cLightPos Pozycja œwiat³a wyliczona metod¹ CStencilShadow::GetLightPos. */
	GLvoid DoDrawShadowForObjNoElem( GLint iIndexObj, const CVector4 &in_cLightPos );

	/// Metoda mno¿¹ca wektor pozycji œwiat³¹ przez macierz.
	/** \param[in] M Tablica opisuj¹ca macierz 4x4. 
	\param[in,out] in_out_cLightPos Oryginalna pozycja œwiat³a w przestrzeni 3D.  */
	GLvoid VMatMult( GLfloat M[ 16 ], CVector4 &in_out_cLightPos );

	GLboolean m_bIsGL_EXT_stencil_wrap; ///< Flaga wskazuj¹ca czy karta graficzna obs³uguje rozszerzenie "GL_EXT_stencil_wrap".

	/// Metoda wyznaczajaca czy dany trójk¹ obiektu 3D jest oœwietlony przez œwiat³o.
	/** \param[in] iIndexObj Indeks obiektu 3D wczytanego z pliku \a 3DObj.
	\param[in] in_cLightPos Pozycja œwiat³a wyliczona metod¹ CStencilShadow::GetLightPos. */
	GLvoid SetVisibleFaces( GLint iIndexObj, const CVector4 &in_cLightPos );

	/// Metoda aktywuj¹ca u¿ycie bufora szablonowego.
	GLvoid StartStencil();

	/// Metoda dezaktywuj¹ca u¿ycie bufora szablonowego oraz ostatecznie rysuj¹ca prostok¹t z cieniem.
	GLvoid FinishStencil();

	GLvoid DrawArrays( EDrawArrayMode eMode, GLvoid* pArray, GLenum uiDAMode, GLsizei iCount );

	std::vector< CVector3 > m_aVertices;
};
