/*! \file
	\brief Plik nag³ówkowy dla klasy CGameControl.
*/
#pragma once

#include "SingletonBase.h"

class CWindowData;
class CSpeedControl;
class CTextureLoader;
class CBitmapFont;
class COutlineFont;
class CSDLFont;
class CTextureFont;
class CConsole;
class CFps;

#if _USE_SOUND_OPENAL_
class COpenALManager;
#else
class CSoundFMOD;
#endif

class CMasterVolume;
class CLighting;
class CCursor;
class CARBMultiTexturing;
class CWGLEXTSwapControl;
class CVertexArrays;
class CMatrixOperations;
class C3DObjManager;
class CEmbossBump;
class CParticles;
class CBillboard;
class CStencilShadow;
class CNetworkManager;
class CRMessageBox;

///  G³ówna klasa zarz¹dzaj¹ca gr¹. 
/*! Jest to klasa odpowiedzialna za zarz¹dzanie tworzon¹ gr¹. Jako, ¿e jest klas¹ newralgiczn¹ dla projektu i zawiera
wa¿ne dane programu, CGameControl jest singletonem. Klasa ta tworzy wiêkszoœæ obiektów opisywanych tu klas oraz
wskazane jest aby obiekty naszych nowo dodanych klas tak¿e by³y tutaj tworzone. \n
Poza tym klasa CGameControl otrzymuje wszelkie zdarzenia klawiatury i myszy oraz obs³uguje je, b¹dŸ przekazuje innym
klasom. W metodzie CGameControl::Draw, natomiast, rysowany jest ca³y wirtualny œwiat. Ogólnie mówi¹c jest to klasa,
od której rozpoczynamy tworzenie naszej gry. */
class CGameControl : public CSingletonBase <CGameControl>
{
public:
	/// Statyczna metoda tworz¹ca obiekt klasy CGameControl. 
	/*! CGameControl jest singletonem, wiêc je¿eli wywo³amy tê metodê po raz drugi to zwróci ona wskaŸnik na obiekt
	ju¿ istniej¹cy, utworzony wczeœniej.
	\return WskaŸnik na obiekt CGameControl. */
	static CGameControl* Construct();

	/// Destruktor.
	/*! Destruktor. */
	virtual ~CGameControl();

	/// Metoda inicjalizuje wszelkie inne obiekty utworzone w metodzie CGameControl::CreateObjects.
	/*! Tutaj wywo³ywana jest metoda CGameControl::CreateObjects oraz wczytywane s¹ zasoby dla innych
	obiektów oraz wywo³ywane s¹ inne funkcje takie jak CGameControl::SetVSync, CGameControl::CreateVertices,
	CGameControl::CreateQuadric. \n
	Wewn¹trz tej metody mo¿emy wywo³ywaæ inne inicjalizacje dla naszych klas. \n
	Metoda ta jest ju¿ wywo³ywana w CFramework::CreateGameControl.
	\return \p GL_TRUE w przypadku powodzenia inicjalizacji wszelkich obiektów. \p GL_FALSE w
	przypadku wyst¹pienia jakiegoœ b³êdu. */
	GLboolean Initialization();

	/// Metoda zwracaj¹ca wskaŸnik kwadryki.
	/*! \return WskaŸnik na obiekt kwadryki. Je¿eli kwadryka nie zosta³a stworzona, metoda zwróci NULL.
	\sa CreateQuadric, m_pQuadric. */
	GLUquadricObj* GetQuadric() { return m_pQuadric; }

	/// Metoda w³¹czaj¹ca lub wy³¹czaj¹ca synchronicazjê pionow¹.
	/*! Czy synchronizacja pionowa zostanie w³¹czona, zale¿y od wartoœæ zmiennej SETTING_FILE::bVSync.
	Metoda ta jest ju¿ domyœlnie wywo³ywana w CGameControl::Initialization.
	\sa CWGLEXTSwapControl, SETTING_FILE. */
	GLvoid SetVSync();

	/// Metoda zamykaj¹ca aplikacjê.
	/*! Je¿eli chemy zamkn¹æ program, nale¿y wywo³aæ tê metodê. */
	GLvoid Exit();

	/// Metoda restartuje wszelkie obiekty utworzone w CGameControl::CreateObjects, które restartu wymagaj¹.
	/*! Metoda ta jest wywo³ywana gdy restartowane jest okno, np. pod wp³ywem zmiany rozdzielczoœci, próbkowania
	dla antyaliasingu, itp. W metodzie tej nale¿y wywo³aæ funkcjê, np. tak¿e RestartObjects() naszych obiektów,
	które mog¹ zostaæ utracone pod wp³ywem zmian parametrów okna, jak np. rozdzielczoœæ. W ostatecznoœci mo¿emy
	tutaj zniszczyæ nasz obiekt i utworzyæ ponownie nowy. Pod warunkiem, ¿e obiekt ten nie bêdzie przechowywa³
	danych pamiêtaj¹cych aktualny stan gry. \n
	Przestrzegaj¹c tych regu³ uzyskujemy mo¿liwoœæ restaru okna (np. po zmianie rozdzielczoœci) bez utraty
	aktualnego stanu gry co bêdzie bardzo dogodne dla u¿ytkownika. */
	GLvoid RestartObjects();

	/// Zdarzenie naciœniêcia klawisza klawiatury, przekazywane z \ref WndProc.
	/*! Metoda ta zostanie wywo³ana automatycznie gdy nadejdzie komunikat \p WM_KEYDOWN. Tutaj klawisz mo¿e byæ
	przekazany do innego obiektu lub mo¿e zostaæ ustawiona odpowiednia wartoœæ tablicy CGameControl::m_bKeys w celu
	przetworzenia klawisza wewn¹trz metody CGameControl::Keyboard.
	\param[in] uiKey Wirtualny kod klawisza jaki zosta³ naciœniêty.
	\return \p GL_TRUE w przypadku przetworzenia zdarzenia. \p GL_FALSE gdy klawisz nie zosta³ obs³u¿ony.
	\sa m_bKeys, Keyboard, KeyUp. */
	GLboolean KeyDown( GLuint uiKey );

	/// Zdarzenie zwolnienia klawisza klawiatury, przekazywane z \ref WndProc.
	/*! Metoda ta zostanie wywo³ana automatycznie gdy nadejdzie komunikat \p WM_KEYUP. Tutaj klawisz mo¿e byæ
	przekazany do innego obiektu lub mo¿e zostaæ ustawiona odpowiednia wartoœæ tablicy CGameControl::m_bKeys w
	celu nie przetwarzania klawisza wewn¹trz metody CGameControl::Keyboard.
	\param[in] uiKey Wirtualny kod klawisza jaki zosta³ zwolniony.
	\return \p GL_TRUE w przypadku przetworzenia zdarzenia. \p GL_FALSE gdy klawisz nie zosta³ obs³u¿ony.
	\sa m_bKeys, Keyboard, KeyDown. */
	GLboolean KeyUp( GLuint uiKey );

	/// Metoda przetwarzaj¹ca naciœniête klawisze klawiatury.
	/*! Wewn¹trz tej metody mo¿emy zaimplementowaæ odpowiedni¹ reakcjê aplikacji na	naciskane klawisze klawiatury.
	Obs³u¿enie klawiszy tutaj a nie w CGameControl::KeyDown	umo¿liwia obs³ugê klawiszy, które s¹ naciskane
	jednoczeœnie. \n
	Metoda ta jest ju¿ wywo³ywana w g³ównej pêtli programu, tj. w CFramework::Run.
	\sa m_bKeys, KeyDown, KeyUp, GetKeyStatus. */
	GLvoid Keyboard();

	/// Metoda wype³niaj¹ca bufor znakami przekazywanymi z klawiatury.
	/** Metodê tê nale¿y wywo³ywaæ np. w KeyDown. Metoda WriteOnKeyboard zapewnia obs³ugê klawisza Backspace do
	kasowania ostatnio wpisanego znaku, oraz wszelkie pozosta³e klawisze, w³¹cznie z kombinacj¹ z klawiszem Shift.
	\param[in] uiKey Wirtualny kod klawisza, który zosta³ wciœniêty.
	\param[in] lpBuffer WskaŸnik na ³añcuch znaków w którym przechowywany bêdzie tekst wpisywany z klawiatury.
	\param[in] iBufferLenMax Wielkoœæ \a lpBuffer.
	\return \p GL_TRUE w przypadku przetworzenia klawisza. \p GL_FALSE gdy klawisz nie zosta³ obs³u¿ony. */
	GLboolean WriteOnKeyboard( GLuint uiKey, LPTSTR lpBuffer, GLint iBufferLenMax );

	/// Metoda za pomoc¹, której mo¿emy sprawdziæ czy dany klawisz jest wciœniêty.
	/*! \param[in] uiKey Wirtualny kod klawisza, który chcemy sprawdziæ.
	\return \p GL_TRUE je¿eli klawisz jest wciœniêty. \p GL_FALSE je¿eli klawisz nie jest wciœniêty.
	\sa m_bKeys, KeyDown, KeyUp, Keyboard. */
	inline GLboolean GetKeyStatus( GLuint uiKey ) { return m_bKeys[ uiKey ]; }

	/// Metoda konwertuj¹ca kod klawisza na odpowiadaj¹cy mu znak ASCII.
	/*! Funkcja uwzglêdnia przytrzymanie klawisza Shift.
	\param[in] uiKeyCode Wirtualny kod klawisza.
	\return Znak ASCII przypisany danemu klawiszowi wirtualnemu. */
	GLuint GetKeyChar( GLuint uiKeyCode );
	
	/// Zdarzenie klikniêcia lewym przyciskiem myszy, przekazywane z \ref WndProc.
	/*! Metoda wywo³ywana automatycznie gdy nadejdzie komunikat \p WM_LBUTTONDOWN. Tutaj mo¿emy przetworzyæ
	klikniêcie myszy lub przekazaæ je dalej do innej klasy.
	\param[in] iX Wspó³rzêdna \a X po³o¿enia kursora w momencie klikniêcia.
	\param[in] iY Wspó³rzêdna \a Y po³o¿enia kursora w momencie klikniêcia.
	\return \p GL_TRUE w przypadku przetworzenia zdarzenia. \p GL_FALSE gdy klawisz nie zosta³ obs³u¿ony. */
	GLboolean MouseLButtonDown( GLint iX, GLint iY );

	/// Zdarzenie zwolnienia lewego przycisku myszy, przekazywane z \ref WndProc.
	/*! Metoda wywo³ywana automatycznie gdy nadejdzie komunikat \p WM_LBUTTONUP. Tutaj mo¿emy przetworzyæ puszczenie
	myszy lub przekazaæ je dalej do innej klasy.
	\param[in] iX Wspó³rzêdna \a X po³o¿enia kursora w momencie klikniêcia.
	\param[in] iY Wspó³rzêdna \a Y po³o¿enia kursora w momencie klikniêcia.
	\return \p GL_TRUE w przypadku przetworzenia zdarzenia. \p GL_FALSE gdy klawisz nie zosta³ obs³u¿ony. */
	GLboolean MouseLButtonUp( GLint iX, GLint iY );

	/// Zdarzenie klikniêcia prawym przyciskiem myszy, przekazywane z \ref WndProc.
	/*! Metoda wywo³ywana automatycznie gdy nadejdzie komunikat \p WM_RBUTTONDOWN. Tutaj mo¿emy przetworzyæ
	klikniêcie myszy lub przekazaæ je dalej do innej klasy.
	\param[in] iX Wspó³rzêdna \a X po³o¿enia kursora w momencie klikniêcia.
	\param[in] iY Wspó³rzêdna \a Y po³o¿enia kursora w momencie klikniêcia.
	\return \p GL_TRUE w przypadku przetworzenia zdarzenia. \p GL_FALSE gdy klawisz nie zosta³ obs³u¿ony. */
	GLboolean MouseRButtonDown( GLint iX, GLint iY );

	/// Zdarzenie zwolnienia prawego przycisku myszy, przekazywane z \ref WndProc.
	/*! Metoda wywo³ywana automatycznie gdy nadejdzie komunikat \p WM_RBUTTONUP. Tutaj mo¿emy przetworzyæ puszczenie
	myszy lub przekazaæ je dalej do innej klasy.
	\param[in] iX Wspó³rzêdna \a X po³o¿enia kursora w momencie klikniêcia.
	\param[in] iY Wspó³rzêdna \a Y po³o¿enia kursora w momencie klikniêcia.
	\return \p GL_TRUE w przypadku przetworzenia zdarzenia. \p GL_FALSE gdy klawisz nie zosta³ obs³u¿ony. */
	GLboolean MouseRButtonUp( GLint iX, GLint iY );

	/// Zdarzenie przekrêcenia kó³ka myszy, przekazywane z \ref WndProc.
	/*! Metoda wywo³ywana automatycznie gdy nadejdzie komunikat \p WM_MOUSEWHEEL.
	\param[in] iScrollLines Mowi nam o ile linii i w która stronê wykonano obrót, wartoœæ ujemna - w dól, wartoœæ
	dodatnia - w górê.
	\return \p GL_TRUE w przypadku przetworzenia zdarzenia. W przeciwnym wypadku \p GL_FALSE. */
	GLboolean MouseWheel( GLint iScrollLines );

	///  G³ówna funkcja rysuj¹ca scenê.
	/*! Funkcja ta wywo³ywana jest z metody CFramework::DrawGLScene. Wew¹trz tej funkcji nale¿y wywo³ywaæ kolejne
	metody \a Draw() innych obiektów mog¹cych coœ rysowaæ.
	\sa CFramework::DrawGLScene. */
	GLvoid Draw();

	/// Metoda zwracaj¹ca aktualn¹ pozycjê kurosra myszy.
	/**	/return Struktura typu \p POINT zawieraj¹ca aktualne wspó³rzêdne kursora myszy.	*/
	POINT GetPointCursor() { return m_ptCursor; }

	TCHAR m_cCharKey; ///< Znak ASCII klawisza jaki zosta³ naciœniêty. Ustawiane przez komunikat \p WM_CHAR.

	/// Metoda w³¹czaj¹ca tryb grafiki 2D
	/*! Nale¿y wywo³aæ tê metodê, je¿eli chcemy rysowaæ w trybie 2D, na powierzchni ekranu.	Domyœlnie tryb 2D jest
	wy³¹czony - framework dzia³a w trybie 3D.
	\sa Disable2D, makro _USE_2D_ONLY_. */
	GLvoid Enable2D();

	/// Metoda wy³¹czaj¹ca tryb grafiki 2D - powraca do 3D.
	/*! Je¿eli w³¹czyliœmy tryb grafiki 2D za pomoc¹ metody CGameControl::Enable2D. To teraz za pomoc¹
	CGameControl::Disable2D mo¿emy powróciæ do trybu 3D.
	\sa Enable2D, makro _USE_2D_ONLY_. */
	GLvoid Disable2D();

	/// Metoda timer-a, wywo³ywana automatycznie gdy nadejdzie zdarzenie \p WM_TIMER, przekazywane z \ref WndProc.
	/*! Domyœlnie timer wywo³ywany jest co 1000 milisekund.
	\sa CWindowData::m_uiTimer. */
	GLvoid Timer();

	/// \name Metody get zwracaj¹ce wskaŸniki na ró¿ne obiekty tworzone w CGameControl.
	//@{
	inline CWindowData*			GetWinData()			{ return m_pWin; }
	inline CSpeedControl*		GetSpeedCtrl()			{ return m_pSpeedControl; }
	inline CTextureLoader*		GetTexLoader()			{ return m_pTextureLoader; }
	inline CBitmapFont*			GetBitmapFont()			{ return m_pBitmapFont; }
	inline COutlineFont*		GetOutlineFont()		{ return m_pOutlineFont; }
	inline CConsole*			GetConsole()			{ return m_pConsole; }
	inline CFps*				GetFps()				{ return m_pFps; }
#if _USE_SOUND_OPENAL_
	inline COpenALManager*		GetOpenALManager()		{ return m_pOpenALManager; }
#else
	inline CSoundFMOD*			GetSoundFMOD()			{ return m_pSoundFMOD; }
#endif
	inline CMasterVolume*		GetMasterVolume()		{ return m_pMasterVolume; }
	inline CSDLFont*			GetSDLFont()			{ return m_pSDLFont; }
	inline CLighting*			GetLighting()			{ return m_pLighting; }
	inline CTextureFont*		GetTextureFont()		{ return m_pTextureFont; }
	inline CCursor*				GetCursor()				{ return m_pCursor; }
	inline CARBMultiTexturing*	GetARBMultiTexturing()	{ return m_pARBMultiTexturing; }
	inline CWGLEXTSwapControl*	GetWGLEXTSwapControl()	{ return m_pWglExtSwapCtrl; }
	inline CVertexArrays*		GetVertexArrays()		{ return m_pVertexArrays; }
	inline CMatrixOperations*	GetMatrixOp()			{ return m_pMatrixOp; }
	inline C3DObjManager*		Get3DObjManager()		{ return m_p3DObjManager; }
	inline CEmbossBump*			GetEmbossBump()			{ return m_pEmbossBump; }
	inline CParticles*			GetParticles()			{ return m_pParticles; }
	inline CBillboard*			GetBillboard()			{ return m_pBillboard; }
	inline CStencilShadow*		GetStencilShadow()		{ return m_pStencilShadow; }
	inline CNetworkManager*		GetNetworkManager()		{ return m_pNetworkManager; }
	inline CRMessageBox*		GetRMessageBox()		{ return m_pRMessageBox; }
	//@}

	/// Tworzy wszelkie obiekty innych klasy, niezbêdnych do funkcjonowania gry.
	/*! Tutaj nale¿y tworzyæ obiekty w³asnych klas. Metoda ta jest ju¿ wywo³ywana w CGameControl::Initialization.
	\return \p GL_TRUE w przypadku powodzenia utworzenia wszelkich obiektów. \p GL_FALSE w przypadku wyst¹pienia
	jakiegoœ b³êdu. */
	GLboolean CreateObjects();

	/// Usuwa wszelkie obiekty innych klasy, utworzonych w CGameControl::CreateObjects.
	/*! Tutaj nale¿y usuwaæ obiekty w³asnych klas, które utworzyliœmy w metodzie CGameControl::CreateObjects. */
	GLvoid DeleteObjects();

	/// Metoda ustawia wstêpne parametry oœwietlenia.
	GLvoid SetLights();

	/// Metoda tworz¹ca wszelkie obiekty dla kontroli animacji.
	/*! Metoda ta jest ju¿ wywo³ywana w CGameControl::Initialization. Tutaj nale¿y wywo³ywaæ CSpeedControl::CreateAnimationControl.
	\sa CSpeedControl. */
	GLvoid CreateAllAnimCtrl();

	/// Metoda wczytuj¹ca tekstury.
	/*! Metoda ta jest ju¿ wywo³ywana w CGameControl::Initialization. Tutaj nale¿y wczytywaæ wszelkie tekstury.
	\return \p GL_TRUE w przypadku powodzenia wczytania tekstur. \p GL_FALSE w przypadku wyst¹pienia b³êdu.
	\sa CTextureLoader. */
	GLboolean LoadAllTextures();

	/// Metoda tworz¹ca emitery cz¹steczek.
	/*! Metoda ta jest ju¿ wywo³ywana w CGameControl::Initialization. Tutaj nale¿y tworzyæ wszelkie emitery
	cz¹steczek.
	\return \p GL_TRUE w przypadku powodzenia utworzenia emiterów. \p GL_FALSE w przypadku wyst¹pienia b³êdu.
	\sa CParticles. */
	GLboolean CreateAllParticles();

	/// Metoda wczytuj¹ca wszystkie obiekty 3D.
	/*! Metoda ta jest ju¿ wywo³ywana w CGameControl::Initialization. W tej metodzie nale¿y wczytywaæ wszelkie obiekty
	3D - pliki \a *.\a 3DObj.
	\return \p GL_TRUE w przypadku powodzenia wczytania obiektów. \p GL_FALSE w przypadku wyst¹pienia b³êdu.
	\sa C3DObjManager. */
	GLboolean LoadAll3DObjFiles();

	/// Metoda wczytuj¹ca wszelkie dŸwieki i muzykê.
	/*! Metoda ta jest ju¿ wywo³ywana w CGameControl::Initialization. Tutaj nale¿y wczytywaæ wszelkie dŸwiêki
	i muzykê.
	\return \p GL_TRUE w przypadku powodzenia wczytania dŸwiêków. \p GL_FALSE w przypadku wyst¹pienia b³êdu. */
	GLboolean LoadAllSounds();

	/// Metoda tworz¹ca w³asnorêcznie dodawane wierzcho³ki w celu ich póŸniejszego renderowania.
	/*! Wewn¹trz tej metody mo¿emy dodawaæ w³asne wierzcho³ki, poprzez rêczne wpisanie ich koordynatów jak i innych
	parametrów, jak werktor normalny, kolor itp.
	\sa CVertexArrays. */
	GLvoid CreateVertices();

	/// Metoda rysuj¹ca prostok¹d na ca³ym ekranie.
	/** Metoda rysuje prostok¹d na ca³ym ekranie.
	\sa SetIndicesQuadAllScreen, m_aIndicesQuadAllScreen.*/
	GLvoid DrawQuadOnAllScreen();

private:
	/// Konstruktor.
	/*! Konstruktor. */
	CGameControl();

	CWindowData		*m_pWin;	///< WskaŸnik na obiekt klasy CWindowData.

	/// Metoda tworz¹ca kwadrykê.
	/*! Tutaj tworzony jest obiekt kwadryki oraz ustawiane s¹ jej wszelkie domyœlne parametry. \n
	Metoda ta jest ju¿ domyœlnie wywo³ywana w CGameControl::Initialization.
	\sa GetQuadric, m_pQuadric. */
	GLvoid CreateQuadric();

	GLUquadricObj	*m_pQuadric; ///< WskaŸnik na obiekt kwadryki.
	
	/// Metoda pobieraj¹ca i obliczaj¹ca pozycjê kursora myszy.
	GLvoid GetCursorPosition();
	
	/// \name WskaŸniki na wszelkie obiekty jakie s¹ tworzone w CGameControl.
	//@{
	CSpeedControl		*m_pSpeedControl;
	CTextureLoader		*m_pTextureLoader;
	CBitmapFont			*m_pBitmapFont;
	COutlineFont		*m_pOutlineFont;
	CConsole			*m_pConsole;
	CFps				*m_pFps;
#if _USE_SOUND_OPENAL_
	COpenALManager		*m_pOpenALManager;
#else
	CSoundFMOD			*m_pSoundFMOD;
#endif
	CMasterVolume		*m_pMasterVolume;
	CSDLFont			*m_pSDLFont;
	CLighting			*m_pLighting;
	CTextureFont		*m_pTextureFont;
	CCursor				*m_pCursor;
	CARBMultiTexturing	*m_pARBMultiTexturing;
	CWGLEXTSwapControl	*m_pWglExtSwapCtrl;
	CVertexArrays		*m_pVertexArrays;
	CMatrixOperations	*m_pMatrixOp;
	C3DObjManager		*m_p3DObjManager;
	CEmbossBump			*m_pEmbossBump;
	CParticles			*m_pParticles;
	CBillboard			*m_pBillboard;
	CStencilShadow		*m_pStencilShadow;
	CNetworkManager		*m_pNetworkManager;
	CRMessageBox		*m_pRMessageBox;
	//@}

	POINT m_ptCursor;			///< Pozycja kursora w pikselach wzglêdem okna programu.
	GLboolean m_bKeys[ 256 ];	///< Tablica u¿ywana do obs³ugi klawiatury.
	GLboolean m_bIs2D;			///< Zmienna zapamiêtuj¹ca ostatnio u¿ywany tryb: 2D lub 3D.

	/// \cond DEV
	GLfloat m_fRotY;
	GLboolean m_bUseBump;
	/// \endcond DEV

	/// Metoda ustawiaj¹ca tablicê wierzcho³ków dla rysowania prostok¹ta na ca³ym ekranie
	/** Metoda ustawia tablicê wierzcho³ków CGameControl::m_aIndicesQuadAllScreen.
	\sa DrawQuadOnAllScreen, m_aIndicesQuadAllScreen.*/
	GLvoid SetIndicesQuadAllScreen();

	/// Tablica wierzcho³ków przekazywana do glDrawArrays dla rysowania prostok¹ta na ca³ym ekranie.
	GLint* m_aIndicesQuadAllScreen;
};