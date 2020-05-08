/*! \file
	\brief Plik nag³ówkowy dla klasy C3DObjManager.
*/
#pragma once

class C3DObjFile;
class CGameControl;
class CVector3;

/// Manager zarz¹dzajacy obiektami 3D, wczytanymi z autorskiego formatu pliku \a 3DObj.
/** Zadaniem tej klasy jest zapewnienie wczytania plików \a 3DObj oraz umo¿liwienie rysowania trójwymiarowych
obiektów zawartych w tych plikach. */
class C3DObjManager
{
public:
	/// Konstruktor.
	/** Konstruktor. */
	C3DObjManager();

	/// Destruktor.
	/** Destruktor. */
	~C3DObjManager();

	/// Metoda inicjalizuj¹ca ustawienia 3D Object Manager-a.
	/** Metoda ta jest ju¿ wywo³ywana w CGameControl::LoadAll3DObjFiles i musi byæ wywo³ana na samym koñcu, przed
	wczytywaniem plików \a 3DObj. */
	GLvoid Initialization();

	/// Metoda wczytuj¹ca plik \a 3DObj.
	/** Metodê t¹ nale¿y wywo³ywaæ wewn¹trz	CGameControl::LoadAll3DObjFiles tyle razy ile chcemy wczytaæ obiektów
	\a 3DObj. 
	\param[in] lpFileName Nazwa pliku z rozszerzeniem \a 3DObj, który chcemy wczytaæ.
	\param[in] iTextureFirst Indeks tekstury jaka zostanie na³o¿ona na obiekt. Indeksy liczone s¹ od \p 0 w kolejnoœci
	wczytywania tekstur w metodzie CGameControl::LoadAllTextures. Je¿eli zamierzamy wykorzystaæ multiteksturing,
	parametr ten stanowi indeks pierwszej tekstury jaka zostanie na³o¿ona na obiekt.
	\param[in] iTextureCount Tylko dla multiteksturingu – iloœæ kolejnych tekstur po \a iTextureFirst, jakie zostan¹
	na³o¿one na obiekt. Aby mia³o sens, nale¿y podaæ co najmniej wartoœæ \p 2. Je¿eli podamy wartoœæ mniejsz¹
	od \p 2, multiteksturing nie zostanie u¿yty. Domyœlna wartoœæ dla \a iTextureCount wynosi \p -1, czyli bez
	multiteksturingu.
	\param[in] iTextureEmbossBump Indeks tekstury jaka zostanie na³o¿ona na obiekt w celu uzyskania efektu wybojów.
	Musi to byæ specjalnie do tego celu przygotowana tekstura, najczêœciej w odcieniach szaroœci. Nale¿y pamiêtaæ,
	¿e je¿eli chcemy wykorzystaæ teksturê bump to parametr \a iTextureCount musi byæ równy wartoœci domyœlnej, tj.
	\p -1. Je¿eli nie chcemy wykorzystaæ mapowania wybojów to nale¿y parametr ten ustawiæ na wartoœæ domyœln¹,
	czyli \p -1.
	\return Indeks nowo stworzonego obiektu. W przypadku wyst¹pienia b³êdu, zwracana jest wartoœæ \p -1. */
	GLint Create3DObject( LPCTSTR lpFileName, GLint iTextureFirst, GLint iTextureCount = -1, GLint iTextureEmbossBump = -1 );
	
	/// Metoda rysuj¹ca pojedynczy obiekt.
	/** Za pomoc¹ tej metody mo¿emy narysowaæ pojedynczy obiekt wczytany z pliku \a 3DObj. Jednak dla lepszej
	wydajnoœci zaleca siê u¿yæ C3DObjManager::Draw3DObject_Lists.
	\param[in] iIndex Indeks obiektu jaki chcemy narysowaæ. Indeksy liczone s¹ od \p 0 w kolejnoœci wczytywania
	obiektów, tj. w kolejnoœci wywo³ywania metody C3DObjManager::Create3DObject.
	\param[in] bUseEmbossBump Ustawiaj¹c na \p GL_TRUE obiekt bêdzie rysowany z efektem mapowania wybojów. Aby
	jednak tak siê sta³o nale¿y uprzednio w metodzie C3DObjManager::Create3DObject podaæ indeks tekstury bump oraz
	iloœæ kolejnych tekstur dla multiteksturingu pozostawiæ w wartoœci domyœlnej. Ustawiaj¹c na \p GL_FALSE (wartoœæ
	domyœlna) efekt mapowania wybojów nie bêdzie u¿ywany przy rysowaniu obiektu, nawet je¿eli przy tworzeniu obiektu
	podaliœmy teksturê bump.
	\param[in] uiLight Identyfikator œwiat³a, u¿ywany jest tylko dla efektu mapowania wybojów. Nale¿y u¿yæ
	\p GL_LIGHTx, gdzie \p x to cyfra od \p 0 do \p 7.
	\sa Create3DObject_Lists. */
	GLvoid Draw3DObject( GLint iIndex, GLboolean bUseEmbossBump = GL_FALSE, GLuint uiLight = GL_LIGHT0 );

	/// Metoda rysuj¹ca pojedynczy obiekt.
	/** Za pomoc¹ tej metody mo¿emy narysowaæ pojedynczy obiekt wczytany z pliku \a 3DObj. Obiekt taki zostanie
	narysowany z wykorzystaniem listy wyœwietlania co znacznie zwiêkszy wydajnoœæ rysowania w prównaniu z
	metod¹ C3DObjManager::Draw3DObject.
	\param[in] iIndex Indeks obiektu jaki chcemy narysowaæ. Indeksy liczone s¹ od \p 0 w kolejnoœci wczytywania
	obiektów, tj. w kolejnoœci wywo³ywania metody C3DObjManager::Create3DObject.
	\param[in] bUseEmbossBump Ustawiaj¹c na \p GL_TRUE obiekt bêdzie rysowany z efektem mapowania wybojów. Aby
	jednak tak siê sta³o nale¿y uprzednio w metodzie C3DObjManager::Create3DObject podaæ indeks tekstury bump oraz
	iloœæ kolejnych tekstur dla multiteksturingu pozostawiæ w wartoœci domyœlnej. Ustawiaj¹c na \p GL_FALSE (wartoœæ
	domyœlna) efekt mapowania wybojów nie bêdzie u¿ywany przy rysowaniu obiektu, nawet je¿eli przy tworzeniu obiektu
	podaliœmy teksturê bump.
	\param[in] uiLight Identyfikator œwiat³a, u¿ywany jest tylko dla efektu mapowania wybojów. Nale¿y u¿yæ
	\p GL_LIGHTx, gdzie \p x to cyfra od \p 0 do \p 7.
	\sa Draw3DObject. */
	GLvoid Draw3DObject_Lists( GLint iIndex, GLboolean bUseEmbossBump = GL_FALSE, GLuint uiLight = GL_LIGHT0 );

	/// \cond DEV
#ifdef _3DSLOADER_
	GLboolean SaveObject( LPCTSTR lpPath,
						  GLboolean bIndexing,
						  GLboolean bFixed,
						  const CVector3& in_cScale = CVector3( 1.0f, 1.0f, 1.0f ),
						  const CVector3& in_cVector = CVector3( 1.0f, 1.0f, 1.0f ),
						  const CVector3& in_cRot90 = CVector3( 1.0f, 1.0f, 1.0f ),
						  GLboolean bSmoothNormal = GL_TRUE );
#endif // _3DSLOADER_
	/// \endcond DEV

	/// Metoda pobieraj¹ca pierwsz¹ teksturê danego obiektu
	/**
	\param[in] iIndex Indeks obiektu z którego chcemy pobraæ pierwsz¹ teksturê. Indeksy liczone s¹ od \p 0 w
	kolejnoœci wczytywania obiektów, tj. w kolejnoœci wywo³ywania metody C3DObjManager::Create3DObject.
	\return Index pierwszej tekstury nak³adanej na obiekt. */
	GLint GetTextureFirstIndex( GLint iIndex );

private:

	/// Struktura przechowuj¹ca niezbêdne informacje dla ka¿dego obiektu.
	/** Struktura ta przechowuje niezbêdne informacje dla ka¿dego wczytanego obiektu. Informacje te s¹ potrzebne
	przy rysowaniu obiektów. */
	struct S3DObject
	{
		GLint iIndexStart;			///< Pierwszy indeks z tablicy wierzcho³ków dla danego obiektu.
		GLint iIndexEnd;			///< Ostatni indeks z tablicy wierzcho³ków dla danego obiektu.
		GLint iTextureFirst;		///< Indeks pierwszej tekstury nak³adanej na obiekt.
		GLint iTextureCount;		///< Iloœæ kolejnych tekstur jakie bêd¹ nak³¹dane na obiekt.
		GLint iTextureEmbossBump;	///< Indeks tekstury emboss bump.
		GLboolean bIsElem;			///< Flaga wskazuj¹ca czy wierzcho³ki obiektu s¹ indeksowane.
	};

	CGameControl	*m_pGameCtrl;		///< WskaŸnik na obiekt klasy singletonu CGameControl.
	C3DObjFile		*m_p3DObjFile;	///< WskaŸnik na obiekt klasy C3DObjFile.
	std::vector<S3DObject> m_a3DObject; ///< Dynamiczna tablica obiektów C3DObjManager::S3DObject.
	
	/// Metoda zwracaj¹ca wielkoœæ tablicy C3DObjManager::m_a3DObject.
	/** \return Iloœæ elementów tablicy C3DObjManager::m_a3DObject. */
	inline GLint Get3DObjectSize() { return static_cast<GLint>( m_a3DObject.size() ); }

	/// Metoda rysuj¹ca pojedynczy obiekt.
	/** Metoda ta rysuje obiekt wczytany z pliku \a 3DObj, bez indeksowania wierzcho³ków.
	\param[in] iIndex Indeks obiektu jaki chcemy narysowaæ. Indeksy liczone s¹ od \p 0 w kolejnoœci wczytywania
	obiektów, tj. w kolejnoœci wywo³ywania metody C3DObjManager::Create3DObject.
	\param[in] bUseEmbossBump Ustawiaj¹c na \p GL_TRUE obiekt bêdzie rysowany z efektem mapowania wybojów. Aby
	jednak tak siê sta³o nale¿y uprzednio w metodzie C3DObjManager::Create3DObject podaæ indeks tekstury bump oraz
	iloœæ kolejnych tekstur dla multiteksturingu pozostawiæ w wartoœci domyœlnej. Ustawiaj¹c na \p GL_FALSE (wartoœæ
	domyœlna) efekt mapowania wybojów nie bêdzie u¿ywany przy rysowaniu obiektu, nawet je¿eli przy tworzeniu obiektu
	podaliœmy teksturê bump.
	\param[in] uiLight Identyfikator œwiat³a, u¿ywany jest tylko dla efektu mapowania wybojów. Nale¿y u¿yæ
	\p GL_LIGHTx, gdzie \p x to cyfra od \p 0 do \p 7.
	\sa Draw3DObjectElem. */
	GLvoid Draw3DObjectNoElem( GLint iIndex, GLboolean bUseEmbossBump = GL_FALSE, GLuint uiLight = GL_LIGHT0 );

	/// Metoda rysuj¹ca pojedynczy obiekt.
	/** Metoda ta rysuje obiekt wczytany z pliku \a 3DObj, z indeksowaniem wierzcho³ków.
	\param[in] iIndex Indeks obiektu jaki chcemy narysowaæ. Indeksy liczone s¹ od \p 0 w kolejnoœci wczytywania
	obiektów, tj. w kolejnoœci wywo³ywania metody C3DObjManager::Create3DObject.
	\param[in] bUseEmbossBump Ustawiaj¹c na \p GL_TRUE obiekt bêdzie rysowany z efektem mapowania wybojów. Aby
	jednak tak siê sta³o nale¿y uprzednio w metodzie C3DObjManager::Create3DObject podaæ indeks tekstury bump oraz
	iloœæ kolejnych tekstur dla multiteksturingu pozostawiæ w wartoœci domyœlnej. Ustawiaj¹c na \p GL_FALSE (wartoœæ
	domyœlna) efekt mapowania wybojów nie bêdzie u¿ywany przy rysowaniu obiektu, nawet je¿eli przy tworzeniu obiektu
	podaliœmy teksturê bump.
	\param[in] uiLight Identyfikator œwiat³a, u¿ywany jest tylko dla efektu mapowania wybojów. Nale¿y u¿yæ
	\p GL_LIGHTx, gdzie \p x to cyfra od \p 0 do \p 7.
	\sa Draw3DObjectNoElem. */
	GLvoid Draw3DObjectElem( GLint iIndex, GLboolean bUseEmbossBump = GL_FALSE, GLuint uiLight = GL_LIGHT0 );

	/// Metoda tworz¹ca listy wyœwietlania.
	/** Metoda ta tworzy listy wyœwietlania dla rysowania wczelkich wczytanych obiektów \a 3DObj bez u¿yca
	efektu emboss bump. Metoda ta jest ju¿ wywo³ywana w C3DObjManager::Initialization. 
	\sa CreateListsBump. */
	GLvoid CreateLists();

	/// Metoda tworz¹ca listy wyœwietlania z efektem bump.
	/** Metoda ta tworzy listy wyœwietlania dla rysowania wczelkich wczytanych obiektów \a 3DObj z u¿yciem
	efektu emboss bump. Metoda ta jest ju¿ wywo³ywana w C3DObjManager::Initialization. 
	\param[in] iMaxLights Dla ilu œwiate³ (rozpoczynaj¹c od \p GL_LIGHT0) maj¹ byæ tworzone listy wyœwietlania
	dla jednego obiektu.
	\sa CreateLists. */
	GLvoid CreateListsBump( GLint iMaxLights = 8 );

	/// Metoda usuwaj¹ca wszelkie listy wyœwietlania.
	/** Metoda ta usuwa listy wyœwietlania stworozne przez metody C3DObjManager::CreateLists oraz
	C3DObjManager::CreateListsBump. */
	GLvoid DeleteLists();
	
	GLuint m_ui3DObjLists;	///< Identyfikator przylegaj¹cych do siebie list wyœwietlania stworzonych metod¹ C3DObjManager::CreateLists.

	/// Tablica identyfikatorów przylegaj¹cych do siebie list wyœwietlania.
	/** Tablica identyfikatorów przylegaj¹cych do siebie list wyœwietlania stworzonych metod¹
	C3DObjManager::CreateListsBump. Indeks tablicy odnosi siê do indeksu œwiat³a, indeks \p 0 dla \p GL_LIGHT0,
	indeks \p 1 dla \p GL_LIGHT1, itd. do \p GL_LIGHT7.	*/
	GLuint m_ui3DObjListsBump[ 8 ];
	
	GLint m_iMaxLightsForLists; ///< Parametr \a iMaxLights pochodz¹cy z metody C3DObjManager::CreateListsBump.

	/// \cond DEV
#ifdef _3DSLOADER_
	GLboolean SaveObjectNoElem( LPCTSTR lpPath, const CVector3 &in_cScale, const CVector3 &in_cVector, const CVector3 &in_cRot90, GLboolean bSmoothNormal, GLboolean bFixed );
	GLboolean SaveObjectElem( LPCTSTR lpPath, GLboolean bFixed );
#endif // _3DSLOADER_
	/// \endcond DEV
};
