/*! \file
	\brief Plik nag³ówkowy dla klasy CParticles.
*/
#pragma once

class CVector3;
class CGameControl;

/// Klasa cz¹steczek.
/** Klasa ta zarz¹dza tworzeniem i rysowaniem emiterów cz¹steczek. */
class CParticles
{
private:
	/// Struktura cz¹steczki.
	/** Struktura ta przechowuje wszelkie informacje opisuj¹ce pojedyncz¹ cz¹steczkê. */
	struct SParticle
	{
		CVector3 cActualPos;	///< Aktuale po³o¿enie cz¹steczki w przestrzeni 3D.
		CVector3 cSpeed;		///< Szybkoœæ poruszania siê czosteczki

		GLfloat fLife;			///< Wartoœæ czasu ¿ycia cz¹steczki, gdy = 0.0 to martwa, maksywmalnie 1.0.
		GLfloat fFade;			///< Wartoœæ szybkoœci wymierania cz¹steczki.

		GLfloat	fScale;			///< Rozmiar cz¹steczki.

		GLfloat fColorR;		///< Sk³adowa koloru Red cz¹steczki.
		GLfloat fColorG;		///< Sk³adowa koloru Green cz¹steczki.
		GLfloat fColorB;		///< Sk³adowa koloru Blue cz¹steczki.
	};

	/// Struktura emitera cz¹steczek.
	/** Struktura ta przechowuje wszelkie informacje opisuj¹ce pojedynczy emiter cz¹steczek. */
	struct SEmitter
	{
		CVector3 cStartPos;		///< Po³o¿enie emitera w przestrzeni 3D. Jest to tak¿e pocz¹tkowe po³o¿enie ka¿dej cz¹steczki emitera.
		GLuint uiTexture;		///< Indeks tekstury obrazuj¹cej ka¿d¹ cz¹steczkê.

		CVector3 cDirection;	///< Wektor kierunku w jakim maj¹ przesuwaæ siê cz¹steczki.
		CVector3 cDivergence;	///< Wektor odchylenia od kierunku poruszania siê cz¹steczek.

		GLfloat fMaxLife;		///< Maksymalna wartoœæ czasu ¿ycia ka¿dej cz¹steczki.
		GLboolean bIsAlive;		///< Flaga wskazuj¹ca czy jak dana cz¹steczka wygaœnie to czy mo¿e o¿yæ, \p GL_TRUE - tak, \p GL_FALSE - nie.

		GLfloat	fScale;			///< Rozmiar cz¹steczek.

		GLfloat fColorR;		///< Sk³adowa koloru Red cz¹steczek.
		GLfloat fColorG;		///< Sk³adowa koloru Green cz¹steczek.
		GLfloat fColorB;		///< Sk³adowa koloru Blue cz¹steczek.

		GLfloat fSpeedX;		///< Szybkoœæ przemieszczania siê cz¹steczek po osi X. Obowi¹zuje tylko gdy \a bIsAlive \p = \p GL_TRUE.
		GLfloat fSpeedY;		///< Szybkoœæ przemieszczania siê cz¹steczek po osi Y. Obowi¹zuje tylko gdy \a bIsAlive \p = \p GL_TRUE.
		GLfloat fSpeedZ;		///< Szybkoœæ przemieszczania siê cz¹steczek po osi Z. Obowi¹zuje tylko gdy \a bIsAlive \p = \p GL_TRUE.

		GLint iMaxParticles;	///< Maksymalna iloœæ cz¹steczek wydawanych z emitera.

		GLfloat fMoveSpeed;		///< Maksymalna wartoœæ szybkoœci przemieszczania siê cz¹steczek.

		GLint iRandSpeedRange1;	///< Pierwsza wartoœæ zakresu losowoœci przemieszczania siê cz¹steczek.
		GLint iRandSpeedRange2;	///< Druga wartoœæ zakresu losowoœci przemieszczania siê cz¹steczek.

		std::vector<SParticle> aParticles;	///< Tablica cz¹steczek.
	};

public:
	/// Konstruktor.
	/** Konstruktor. */
	CParticles();

	/// Destruktor.
	/** Destruktor. */
	~CParticles();

	/// Metoda tworz¹ca nowy emiter.
	/** \param[in] in_sEmitter Referencja na strukturê CParticles::SEmitter, z którego stworzony zostanie nowy emiter.
	\return Indeks nowego emitera. */
	GLint CreateEmitter( const SEmitter &in_sEmitter );

	/// Metoda tworz¹ca nowy emiter.
	/** \param[in] in_cStartPos Po³o¿enie emitera w przestrzeni 3D. Jest to tak¿e pocz¹tkowe po³o¿enie ka¿dej cz¹steczki emitera.
	\param[in] uiTexture Indeks tekstury obrazuj¹cej ka¿d¹ cz¹steczkê.
	\param[in] in_cDirection Wektor kierunku w jakim maj¹ przesuwaæ siê cz¹steczki. Aktualnie nieu¿ywane.
	\param[in] in_cDivergence Wektor odchylenia od kierunku poruszania siê cz¹steczek. Aktualnie nieu¿ywane.
	\param[in] fScale Rozmiar cz¹steczek. Zalecany rozmiar to \p 1.0f.
	\param[in] fLife Maksymalna wartoœæ czasu ¿ycia ka¿dej cz¹steczki. Zalecana wartoœæ to \p 1.0f.
	\param[in] bAlive Czy cz¹œteczki mog¹ o¿ywaæ po wygaœniêciu.
	\param[in] fColorR Sk³adowa koloru Red cz¹steczek.
	\param[in] fColorG Sk³adowa koloru Green cz¹steczek.
	\param[in] fColorB Sk³adowa koloru Blue cz¹steczek.
	\param[in] fSpeedX Szybkoœæ przemieszczania siê cz¹steczek po osi X.
	\param[in] fSpeedY Szybkoœæ przemieszczania siê cz¹steczek po osi Y.
	\param[in] fSpeedZ Szybkoœæ przemieszczania siê cz¹steczek po osi Z.
	\param[in] iMaxParticles Maksymalna iloœæ cz¹steczek wydawanych z emitera.
	\param[in] fMoveSpeed Maksymalna wartoœæ szybkoœci przemieszczania siê cz¹steczek.
	\param[in] iRandSpeedRange1 Pierwsza wartoœæ zakresu losowoœci przemieszczania siê cz¹steczek.
	\return Indeks nowego emitera. */
	GLint CreateEmitter( const CVector3 &in_cStartPos,
						 GLuint uiTexture,
						 const CVector3 &in_cDirection,
						 const CVector3 &in_cDivergence,
						 GLfloat fScale, // 1.0f
						 GLfloat fLife, // 1.0f
						 GLboolean bAlive,
						 GLfloat fColorR,
						 GLfloat fColorG,
						 GLfloat fColorB,
						 GLfloat fSpeedX, // 0.0f
						 GLfloat fSpeedY, // 0.0f
						 GLfloat fSpeedZ, // 0.0f
						 GLint iMaxParticles,
						 GLfloat fMoveSpeed = 60.0f, //szybkosc poruszania sie czasteczek
						 GLint iRandSpeedRange1 = 60 //dla okreslenia rozprysku czaseteczki emitera
						);

	/// Metoda ustawiaj¹ca now¹ wartoœci iloœci cz¹steczek dla danego emitera.
	/** \param[in] iIndex Indeks emitera, dla którego chcemy zmieniæ iloœæ cz¹steczek.
	\param[in] iMaxParticles Nowa wartoœæ iloœci cz¹steczek dla danego emitera. */
	GLvoid SetMaxParticlesForEmitter( GLint iIndex, GLint iMaxParticles );

	/// Metoda pobieraj¹ca aktualn¹ iloœæ cz¹steczek danego emitera.
	/** \param[in] iIndex Indeks emitera, z którego chcemy pobraæ wartoœci ilosci cz¹steczek.
	\return Liczba ca³kowita okreœlaj¹ca maksymaln¹ iloœæ cz¹steczek danego emitera. */
	GLint GetMaxParticlesForEmitter( GLint iIndex );

	/// Metoda sprawdzaj¹ca czy emiter zawiera przynajmniej jedn¹ "¿yw¹", tj. niewygasniêt¹ cz¹steczkê.
	/** \param[in] iIndex Indeks emitera, którego chcemy poddaæ sprawdzeniu.
	\return \p GL_TRUE w przypadku gdy emiter zawiera przynajmniej jedn¹ niewygaœniêt¹ cz¹steczkê. W przeciwnym
	wypadku \p GL_FALSE. */
	GLboolean IsAliveParticleInEmitter( GLint iIndex ); 

	/// Metoda przywracaj¹ca do "¿ycia" wszytskie cz¹steczki danego emitera.
	/** \param[in] iIndex Indeks emitera, którego cz¹steczki chemy przywróciæ do "¿ycia". */
	GLvoid SetParticleInEmitterAsLive( GLint iIndex );

	/// Metoda zwracajaca iloœæ stworzonych emiterów.
	/** \return Liczba ca³kowita okreœlaj¹ca iloœæ stworzonych emiterów. */
	inline GLint GetEmittersSize() { return static_cast<GLint>( m_aEmitters.size() ); }

	/// Metoda rysuj¹ca cz¹steczki danego emitera.
	/** \param[in] iIndex Indeks emitera, którego cz¹steczki chcemy rysowaæ.
	\param[in] cPos Pozycja po³o¿enia emitera w przestrzeni 3D. Je¿li pozostawimy wartoœæ domyœln¹ (czyli zerow¹), to
	u¿yta zostanie wartoœæ po³o¿enia podana przy tworzeniu emitera. */
	GLvoid DrawEmitter( GLint iIndex, const CVector3 &cPos = CVector3() );

	/// Metoda usuwaj¹ca pojedyncz¹ cz¹steczkê z danego emitera.
	/** \param[in] iIndex Indeks emitera, z którego chcemy usun¹æ cz¹steczkê.
	\param[in] iIndexParticle Indeks cz¹steczki, któr¹ chcemy usun¹æ. */
	GLvoid DeleteParticle( GLint iIndex, GLint iIndexParticle );

	/// Metoda usuwaj¹ca stworzyny wczeœniej emiter.
	/** \param[in] iIndex Indeks emitera jaki chcemy usun¹æ. */
	GLvoid DeleteEmitter( GLint iIndex );

private:
	CGameControl		*m_pGameCtrl;		///< WskaŸnik na obiekt klasy CGameControl.

	std::vector<SEmitter> m_aEmitters;		///< Tablica emiterów.

	/// Metoda tworz¹ca nowy emiter.
	/** \param[in] in_sEmitter Referencja na strukturê CParticles::SEmitter, z którego stworzony zostanie nowy emiter.
	\return Indeks nowego emitera. */
	GLint AddAndCreateEmitter( const SEmitter &in_sEmitter );

	/// Metoda tworz¹ca cz¹steczki dla danego emitera.
	/** Metoda ta ca³kowicie na nowo tworzy wszystkie cz¹steczki dla danego emitera.
	\param[in] iIndex Indeks emitera, dla którego zostan¹ stworzone cz¹steczki. */
	GLvoid CreateParticles( GLint iIndex );

	/// Metoda dodaj¹ca nowe cz¹steczki do ju¿ istniej¹cych dla danego emitera.
	/** \param[in] iIndex Indeks emitera, dla którego zostan¹ dodane nowe cz¹steczki.
	\param[in] iMaxParticles Nowa wartoœæ iloœci cz¹steczek dla danego emitera. */
	GLvoid AddParticles( GLint iIndex, GLint iMaxParticles );

	/// Metoda przywracaj¹ca do "¿ycia" pojedyncz¹ cz¹steczkê.
	/** \param[in] iEmitterIndex Indeks emitera, którego cz¹steczkê chcmy przywróciæ do "¿ycia".
	\param[in] iParticleIndex Indeks cz¹steczki, któr¹ chcemy na nowo przywróciæ do "¿ycia". */
	GLvoid SetAliveParticle( GLint iEmitterIndex, GLint iParticleIndex );

	GLfloat m_fSlowDown;	///< Zmienna zwalniaj¹ca przemieszczanie siê cz¹steczek.
	
	/// Metoda zwracaj¹ca losow¹ wartoœæ dla szybkoœci wygasania cz¹steczki.
	/** \return Liczba rzeczywista okreœlaj¹ca losow¹ wartoœæ dla szybkoœci wygasania cz¹steczki. */
	inline GLfloat GetFade() {
		return static_cast<GLfloat>( rand() % 100 + 25 ) / 1000.0f + 0.003f; //od 0.0 do 0.099 + 0.003
	}
};
