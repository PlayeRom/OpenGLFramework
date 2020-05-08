/*! \file
	\brief Plik nag³ówkowy dla klasy CTextureLoader.
*/
#pragma once

/// Definicja nag³ówka pliku JEH.
#define JEH_HEADER "JEH"

/// Klasa odpowiedzialana za wczytywanie tekstur.
/** Zadaniem tej klasy jest wczytywanie plików graficznych BMP, TGA oraz JEH i sporz¹dzenie z nich tekstur gotowych
do renderingu. */

class CTextureLoader  
{
public:
	/// Konstruktor.
	/** Konstruktor. */
	CTextureLoader();

	/// Destruktor.
	/** Destruktor. */
	virtual ~CTextureLoader();

	/// Metoda tworz¹ca teksturê typu mipmap.
	/** Metoda ta utworzy najlepiej wygl¹daj¹c¹ teksturê mipmap-ow¹.
	\param[in] lpFileName Pe³na nazwa pliku wraz ze œcie¿k¹ i rozszerzeniem, który chcemy wczytaæ. Obs³ugiwane formaty
	pliku to BMP, TGA oraz JEH.
	\return Indeks utworzonej tekstury licz¹c od \p 0. W przypadku b³êdu zwraca \p -1. */
	GLint LoadTexMipmaps( LPCTSTR lpFileName );

	/// Metoda tworz¹ca teksturê typu linear.
	/** \param[in] lpFileName Pe³na nazwa pliku wraz ze œcie¿k¹ i rozszerzeniem, który chcemy wczytaæ. Obs³ugiwane
	formaty	pliku to BMP, TGA oraz JEH.
	\return Indeks utworzonej tekstury licz¹c od \p 0. W przypadku b³êdu zwraca \p -1. */
	GLint LoadTex( LPCTSTR lpFileName );

	/// Metoda tworz¹ca teksturê typu nearest.
	/** Metoda ta utworzy najgorzej wygl¹daj¹c¹ teksturê nearest.
	\param[in] lpFileName Pe³na nazwa pliku wraz ze œcie¿k¹ i rozszerzeniem, który chcemy wczytaæ. Obs³ugiwane formaty
	pliku to BMP, TGA oraz JEH.
	\return Indeks utworzonej tekstury licz¹c od \p 0. W przypadku b³êdu zwraca \p -1. */
	GLint LoadTexLowQuality( LPCTSTR lpFileName );

	/// Metoda tworz¹ca teksturê mipmap-ow¹ z mask¹ wczytywan¹ z pliku.
	/** \param[in] lpFileNameMask Pe³na nazwa pliku maski wraz ze œcie¿k¹ i rozszerzeniem. Obraz maski musi byæ w
	odcienich szaroœci, im ciemniejszy kolor tym mniej widoczny, gdzie czarny zupe³nie przezroczysty. 
	\param[in] lpFileNameRGB Pe³na nazwa pliku wraz ze œcie¿k¹ i rozszerzeniem, w którym zosytanie wyciêta maska.
	\return Indeks utworzonej tekstury licz¹c od \p 0. W przypadku b³êdu zwraca \p -1.*/
	GLint LoadMultiTexMaskMipmaps( LPCTSTR lpFileNameMask, LPCTSTR lpFileNameRGB );

	/// Metoda tworz¹ca teksturê mipmap-ow¹ z mask¹ wczytywan¹ z pliku.
	/** Odmiana powy¿szej metody, ale tutaj zamiast drugiej tekstury podajemy kolor RGB. U¿ywamy tej metody, gdy
	druga, kolorowa tekstura, ma jednolity kolor. Wtedy nie musimy tworzyæ zbêdnej tektury, tylko od razu podajemy
	ten kolor jako COLORREF. Dzieki temu zaoszczêdzamy pamiêæ i miejsce na dysku.
	\param[in] lpFileNameMask Pe³na nazwa pliku maski wraz ze œcie¿k¹ i rozszerzeniem. Obraz maski musi byæ w
	odcienich szaroœci, im ciemniejszy kolor tym mniej widoczny, gdzie czarny zupe³nie przezroczysty. 
	\param[in] crColor Kolor, w którym zostanie wyciêta maska. Podajemy w makrze RGB.
	\return Indeks utworzonej tekstury licz¹c od \p 0. W przypadku b³êdu zwraca \p -1.*/
	GLint LoadMultiTexMaskMipmaps( LPCTSTR lpFileNameMask, COLORREF crColor );

	/// Metoda tworz¹ca teksturê linear z mask¹ wczytywan¹ z pliku.
	/** \param[in] lpFileNameMask Pe³na nazwa pliku maski wraz ze œcie¿k¹ i rozszerzeniem. Obraz maski musi byæ w
	odcienich szaroœci, im ciemniejszy kolor tym mniej widoczny, gdzie czarny zupe³nie przezroczysty. 
	\param[in] lpFileNameRGB Pe³na nazwa pliku wraz ze œcie¿k¹ i rozszerzeniem, w którym zosytanie wyciêta maska.
	\return Indeks utworzonej tekstury licz¹c od \p 0. W przypadku b³êdu zwraca \p -1.*/
	GLint LoadMultiTexMask( LPCTSTR lpFileNameMask, LPCTSTR lpFileNameRGB );

	/// Metoda tworz¹ca teksturê linear z mask¹ wczytywan¹ z pliku.
	/** \param[in] lpFileNameMask Pe³na nazwa pliku maski wraz ze œcie¿k¹ i rozszerzeniem. Obraz maski musi byæ w
	odcienich szaroœci, im ciemniejszy kolor tym mniej widoczny, gdzie czarny zupe³nie przezroczysty. 
	\param[in] crColor Kolor, w którym zostanie wyciêta maska. Podajemy w makrze RGB.
	\return Indeks utworzonej tekstury licz¹c od \p 0. W przypadku b³êdu zwraca \p -1.*/
	GLint LoadMultiTexMask( LPCTSTR lpFileNameMask, COLORREF crColor );

	/// Metoda tworz¹ca teksturê nearest z mask¹ wczytywan¹ z pliku.
	/** \param[in] lpFileNameMask Pe³na nazwa pliku maski wraz ze œcie¿k¹ i rozszerzeniem. Obraz maski musi byæ w
	odcienich szaroœci, im ciemniejszy kolor tym mniej widoczny, gdzie czarny zupe³nie przezroczysty. 
	\param[in] lpFileNameRGB Pe³na nazwa pliku wraz ze œcie¿k¹ i rozszerzeniem, w którym zosytanie wyciêta maska.
	\return Indeks utworzonej tekstury licz¹c od \p 0. W przypadku b³êdu zwraca \p -1.*/
	GLint LoadMultiTexMaskLowQuality( LPCTSTR lpFileNameMask, LPCTSTR lpFileNameRGB );

	/// Metoda tworz¹ca teksturê nearest z mask¹ wczytywan¹ z pliku.
	/** \param[in] lpFileNameMask Pe³na nazwa pliku maski wraz ze œcie¿k¹ i rozszerzeniem. Obraz maski musi byæ w
	odcienich szaroœci, im ciemniejszy kolor tym mniej widoczny, gdzie czarny zupe³nie przezroczysty. 
	\param[in] crColor Kolor, w którym zostanie wyciêta maska. Podajemy w makrze RGB.
	\return Indeks utworzonej tekstury licz¹c od \p 0. W przypadku b³êdu zwraca \p -1. */
	GLint LoadMultiTexMaskLowQuality( LPCTSTR lpFileNameMask, COLORREF crColor );

	/// Metoda wczytuj¹ca teksturê dla efektu emboss bump.
	/** Tekstura emboss bump zawsze jest tworozna jako mipmapa.
	\param[in] lpFileName Pe³na nazwa pliku bump wraz ze œcie¿k¹ i rozszerzeniem. Obraz bump musi byæ w
	odcienich szaroœci.
	\return Indeks utworzonej tekstury licz¹c od \p 0. W przypadku b³êdu zwraca \p -1. */
	GLint LoadEmbossBump( LPCTSTR lpFileName );

	/// Metoda ustawiaj¹ca teksturê dla aktualnie ustawionej jednostki teksturuj¹cej.
	/** Metodê tê nale¿y wywo³aæ w celu rysowania obiektu z na³o¿on¹ dan¹ tekstur¹.
	\param[in] iIndex Indeks tekstury jak¹ chcemy uczyniæ aktywn¹. Nale¿y podaæ tu indeks zwrócony przez takie
	matody jak: CTextureLoader::LoadTexMipmaps, CTextureLoader::LoadTex,
	CTextureLoader::LoadTexLowQuality.
	\return \p GL_TRUE w przypadku powodzenia ustawienia tekstury. \p GL_FALSE w przypadku niepowodzenia, tj.
	podania b³êdnego indeksu. */
	GLboolean SetTexture( GLint iIndex = 0 );

	/// Metoda ustawiaj¹ca multi-teksturê dla aktualnie ustawionej jednostki teksturuj¹cej.
	/** Metodê tê nale¿y wywo³aæ w celu rysowania obiektu z na³o¿on¹ multi-tekstur¹.
	\param[in] iIndex Indeks tekstury jak¹ chcemy uczyniæ aktywn¹. Nale¿y podaæ tu indeks zwrócony przez takie
	matody jak: CTextureLoader::LoadMultiTexMaskMipmaps, CTextureLoader::LoadMultiTexMask,
	CTextureLoader::LoadMultiTexMaskLowQuality.
	\return \p GL_TRUE w przypadku powodzenia ustawienia tekstury. \p GL_FALSE w przypadku niepowodzenia, tj.
	podania b³êdnego indeksu. */
	GLboolean SetMultiTextures( GLint iIndex );

	/// Metoda wyœwietlaj¹ca identyfikatory i przypisane im nazwy tekstur.
	GLvoid DisplayIDTextureName();

	/// Metoda zwracaj¹ca liczbê okreœlajac¹ iloœæ wczytanych tekstur.
	/** \return Liczba ca³kowita okreœlaj¹ca iloœæ wczytanych tekstur.  */
	inline GLint GetTextureSize() { return static_cast<GLint>( m_aTexture.size() ); }

	/// Metoda usuwaj¹ca wskazan¹ teksturê.
	/** \param[in] iIndex Indeks tekstury, któr¹ chcemy usun¹æ. */
	GLvoid DeleteTexture( GLint iIndex );

private:

	/// Struktura przechowuj¹ca identyfikatory wczytanych tekstur.
	struct STextureData
	{
		GLuint uiTexture;		///< Identyfikator tekstury.
		TCHAR *lpFileName;		///< Nazwa pliku tekstury, której przypisany jest identyfikator. 
	};

	/// Struktua pomocnicza, przechowuj¹ca dane obrazu.
	struct STexture
	{
		GLubyte* ubImageData;		///< WskaŸnik na dane obrazu.
		GLuint uiBitsPerPixel;		///< Iloœæ bitów na piksel.
		GLuint uiWidth;				///< Szerokoœæ obrazu w pikselach.
		GLuint uiHeight;			///< Wysokoœæ obrazu w pikselach.
		GLuint uiType;				///< Format tekstury, tj. GL_RGB, GL_RGBA.
	};

	/// Struktura pomocnicza przy wczytywaniu pliku TGA.
	struct STGAFile
	{
		GLubyte ubHeader[ 18 ];		///< Nag³owek pliku TGA.
		GLuint uiBytesPerPixel;		///< Iloœæ bajtów na piksel.
		GLuint uiImageSize;			///< Rozmiar obrazu w bajtach/
		GLuint uiType;				///< Format tekstury, tj. GL_RGB, GL_RGBA.
		GLuint uiWidth;				///< Szerokoœæ obrazu w pikselach.
		GLuint uiHeight;			///< Wysokoœæ obrazu w pikselach.
		GLuint uiBitsPerPixel;		///< Iloœæ bitów na piksel.
	};

	/// Struktura reprezentuj¹ca nag³ówek pliku JEH.
	struct SHeaderJEH
	{
		char cHeader[ 4 ];			///< Nag³owek identyfikuj¹cy format pliku.
		unsigned int uiWidth;		///< Szerokoœæ obrazu w pikselach.
		unsigned int uiHeight;		///< Wysokoœæ obrazu w pikselach.
		unsigned int uiSizeImage;	///< Wielkoœæ danych obrazu w bajtach.
		unsigned int uiBitCount;	///< Iloœæ bitów na piksel.
	};

	/// Metoda sprawdzaj¹ca rozszerzenie pliku.
	/** \param[in] lpFileName Pe³na nazwa pliku wraz z rozszerzeniem, któr¹ chcemy sprawdziæ.
	\return Liczba ca³kowita okreœlaj¹ca rozszerzenie pliku, gdzie \p 0 to BMP, \p 1 TGA, \p 2 JEH. W przypadku
	innego, nie rozpoznawalnego rozszerzenia zwraca \p -1. */
	GLint CheckExtension( LPCTSTR lpFileName );

	/// Metoda wczytuj¹c oraz tworz¹ca normaln¹ teksturê.
	/** \param[in] lpFileName Pe³na nazwa pliku wraz ze œcie¿k¹ i rozszerzeniem, który chcemy wczytaæ. 
	\param[in] iMode Typ tekstury, gdzie \p 0 - mipmapa, \p 1 - linear, \p 2 - nearest. W przypadku podania
	nieobs³ugiwanego typu, przyjête zostanie \p 0.
	\param[in] iFormat Format pliku podanego w \a lpFileName. Mo¿liwe wartoœci: \p 0 - BMP, \p 1 - TGA oraz \p 2 - 
	JEH.
	\param[in] bEmbossBump Flaga wskazuj¹ca czy utworzona ma byæ tekstura emboss bump.
	\return Indeks utworzonej tekstury licz¹c od \p 0. W przypadku b³êdu zwraca \p -1. */
	GLint LoadGLTex( LPCTSTR lpFileName, GLint iMode = 0, GLint iFormat = 0, GLboolean bEmbossBump = GL_FALSE );
	
	/// Metoda wczytuj¹c oraz tworz¹ca multi-teksturê z mask¹ wczytan¹ z pliku.
	/** \param[in] lpFileName1 Pe³na nazwa pliku maski wraz ze œcie¿k¹ i rozszerzeniem. Obraz maski musi byæ w
	odcienich szaroœci, im ciemniejszy kolor tym mniej widoczny, gdzie czarny zupe³nie przezroczysty.
	\param[in] lpFileName2 Pe³na nazwa pliku wraz ze œcie¿k¹ i rozszerzeniem, w którym zosytanie wyciêta maska.
	\param[in] iMode Typ tekstury, gdzie \p 0 - mipmapa, \p 1 - linear, \p 2 - nearest. W przypadku podania
	nieobs³ugiwanego typu, przyjête zostanie \p 0.
	\param[in] iFormat1 Format pliku podanego w \a lpFileName1. Mo¿liwe wartoœci: \p 0 - BMP, \p 1 - TGA oraz \p 2 - 
	JEH.
	\param[in] iFormat2 Format pliku podanego w \a lpFileName2. Mo¿liwe wartoœci: \p 0 - BMP, \p 1 - TGA oraz \p 2 - 
	JEH.
	\return Indeks utworzonej tekstury licz¹c od \p 0. W przypadku b³êdu zwraca \p -1. */
	GLint LoadGLMultiTexMask( LPCTSTR lpFileName1, LPCTSTR lpFileName2, GLint iMode = 0, GLint iFormat1 = 0, GLint iFormat2 = 0 );

	/// Metoda wczytuj¹c oraz tworz¹ca multi-teksturê z mask¹ wczytan¹ z pliku.
	/** Odmiana powy¿szej metody, ale tutaj zamiast drugiej tekstury podajemy kolor RGB. U¿ywamy tej metody, gdy
	druga, kolorowa tekstura, ma jednolity kolor. Wtedy nie musimy tworzyæ zbêdnej tektury, tylko od razu podajemy
	ten kolor jako COLORREF. Dzieki temu zaoszczêdzamy pamiêæ i miejsce na dysku.
	\param[in] lpFileNameMask Pe³na nazwa pliku maski wraz ze œcie¿k¹ i rozszerzeniem. Obraz maski musi byæ w
	odcienich szaroœci, im ciemniejszy kolor tym mniej widoczny, gdzie czarny zupe³nie przezroczysty.
	\param[in] crColor Kolor, w którym zostanie wyciêta maska. Podajemy w makrze RGB.
	\param[in] iMode Typ tekstury, gdzie \p 0 - mipmapa, \p 1 - linear, \p 2 - nearest. W przypadku podania
	nieobs³ugiwanego typu, przyjête zostanie \p 0.
	\param[in] iFormat Format pliku podanego w \a lpFileNameMask. Mo¿liwe wartoœci: \p 0 - BMP, \p 1 - TGA oraz \p 2 - 
	JEH.
	\return Indeks utworzonej tekstury licz¹c od \p 0. W przypadku b³êdu zwraca \p -1. */
	GLint LoadGLMultiTexMask( LPCTSTR lpFileNameMask, COLORREF crColor, GLint iMode = 0, GLint iFormat = 0 );

	/// Metoda tworz¹ca teksturê dla efektu emboss bump.
	/** \param[in] iIndex Indeks tablicy CTextureLoader::m_aTexture.
	\param[in] texTexture Referencja na obiekt typu CTextureLoader::STexture, gdzie przechowywane s¹ dane obrazu.
	\param[in] iMode Typ tekstury, gdzie \p 0 - mipmapa, \p 1 - linear, \p 2 - nearest. W przypadku podania
	nieobs³ugiwanego typu, przyjête zostanie \p 0.
	\return Indeks utworzonej tekstury licz¹c od \p 0. W przypadku b³êdu zwraca \p -1. */
	GLint CreateEmbossBumpTexture( GLint iIndex, STexture &texTexture, GLint iMode );

	/// Metoda wczytuj¹ca plik BMP.
	/** \param[out] out_sTexture Referencja na obiekt typu CTextureLoader::STexture, przez który zwracane s¹ dane
	dotycz¹ce wczytanego obrazu.
	\param[in] lpFileName Nazwa pliku, który zostanie wczytany. 
	\return \p GL_TRUE w przypadku powodzenia wczytania pliku. W przeciwnym wypadku \p GL_FALSE. */
	GLboolean LoadBMP( STexture &out_sTexture, LPCTSTR lpFileName );

	/// Metoda wczytuj¹ca plik TGA.
	/** \param[out] out_sTexture Referencja na obiekt typu CTextureLoader::STexture, przez który zwracane s¹ dane
	dotycz¹ce wczytanego obrazu.
	\param[in] lpFileName Nazwa pliku, który zostanie wczytany. 
	\return \p GL_TRUE w przypadku powodzenia wczytania pliku. W przeciwnym wypadku \p GL_FALSE. */
	GLboolean LoadTGA( STexture &out_sTexture, LPCTSTR lpFileName );

	/// Metoda wczytuj¹ca plik JEH.
	/** \param[out] out_sTexture Referencja na obiekt typu CTextureLoader::STexture, przez który zwracane s¹ dane
	dotycz¹ce wczytanego obrazu.
	\param[in] lpFileName Nazwa pliku, który zostanie wczytany. 
	\return \p GL_TRUE w przypadku powodzenia wczytania pliku. W przeciwnym wypadku \p GL_FALSE. */
	GLboolean LoadJEH( STexture &out_sTexture, LPCTSTR lpFileName );

	/// Licznik kontrolny, iloœci wczytywanych tekstur.
	GLint m_iCounterTexture;

	/// Tablica przechowuj¹ca identyfikatory wszytskich wczytanych tekstur.
	std::vector<STextureData> m_aTexture;
};