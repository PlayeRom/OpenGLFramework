/*! \file
	\brief Plik nag³ówkowy dla klasy CSpeedControl.
*/
#pragma once

/// Klasa kontroli szybkoœci animacji.
/*! Klasa ta dostarcza szereg opcji zwi¹zanych z kontrol¹ prêdkoœci animacji. Dziêki temu wszelkie animacje bêd¹
uniezale¿nione od prêdkoœci renderowania sceny. */
class CSpeedControl  
{
public:
	/// Konstruktor.
	/*! Konstruktor. */
	CSpeedControl();

	/// Destruktor.
	/*! Destruktor. */
	virtual ~CSpeedControl();

	/// Metoda odœwie¿aj¹ca aktualny czas dla kontroli prêdkoœci animacji.
	/*! Metoda ta jest ju¿ wywo³ywana co klatkê w metodzie CGameControl::Draw. G³ownym zadaniem tej metody
	jest ustawienie zmiennej CSpeedControl::m_fMultiplier. */
	GLvoid RefreshTime();

	/// Metoda pobieraj¹ca mno¿nik dla wszelkich animacji.
	/*! \return Funkcja ta zwróci nam wartoœæ \p float. Wartoœæ t¹ nale¿y mno¿yæ przez wszelkie zmienne wykorzystywane
	do animacji obiektów, tj. przesuwania, obracania, itp. Dziêki temu uzykamy szybkoœæ animacji niezale¿n¹ od
	iloœci FPS. Je¿eli aktywujemy pauzê to metoda ta zawsze zwróci \p 0.0f.
	\sa RefreshTime, m_fMultiplier, SetPause. */
	inline GLfloat GetMultiplier() {
		if( m_bPause ) 
			return 0.0f;
		return m_fMultiplier;
	}

	/// Metoda ustawiaj¹ca pauzê.
	/*! Je¿eli uaktywnimy pauzê to funkcja CSpeedControl::GetMultiplier bêdzie zwraca³a \p 0.0f. Je¿eli wartoœæ ta
	bêdzie mno¿ona przez wszelkie zmienne zwi¹zane z ruchem i obrotem obiektów to w oczywisty sposób je unieruchomimy
	daj¹c efekt spauzowania gry.
	\param[in] bPause Podaj¹c \p GL_TRUE uaktywniamy pauzê. Podaj¹c \p GL_FALSE dezaktywujemy pauzê.
	\sa GetMultiplier, m_fMultiplier. */
	inline GLvoid SetPause( GLboolean bPause ) { m_bPause = bPause; }

	/// \name Zestaw metod kontroli animacji.
	//@{

	/// Metoda tworz¹ca nezale¿n¹ instancjê kontroli animacji.
	/*! Metoda ta utworzy dla nas zmienn¹ pamiêtaj¹c¹ czas (liczbê milisekund) ostatniego wywo³ania animacji.
	Jest to niezbêdne do kontroli naszej animacji. Je¿eli potrzebujemy kontrolowaæ wiêcej animacji, np. piêæ to
	musimy po prostu piêæ razy wywo³aæ tê metodê. Metodê t¹ nale¿y wywo³ywaæ w CGameControl::CreateAllAnimCtrl.
	\return Indeks kontroli animacji. */
	GLint CreateAnimationControl();

	/// Metoda sprawdzaj¹ca czy min¹³ zadany czas dla danej animacji.
	/** Metodê t¹ nale¿y wywo³ywaæ w trakcie renderowania sceny. Dziêki tej metodzie mo¿emy kontrolowaæ animacjê
	poprzez sprawdzenie czy up³yne³a zadana liczbma milisekund dla danej instancji animacji stworzonej przez metodê
	CSpeedControl::CreateAnimationControl.
	\param[in] ulMs Liczba milisekund, czyli tutaj okreœlamy szybkoœæ naszej animacji.
	\param[in] iIndex Indeks (liczony od 0) zwrócony przez CSpeedControl::CreateAnimationControl. Indeksy przypisane
	s¹ danym animacj¹. Przypisanie to jest dowolne i okreœlane przez nas, np. ustalamy sobie, ¿e indeks 0 bêdzie
	odnosi³ siê do animacji ¿o³nierza, indeks 1 do animacji lotu pociski, itd.
	\param[in] bUpdateIfPause Czy aktualizowaæ czas je¿eli jest pauze. Je¿eli ustawimy tê wartoœæ na \p GL_TRUE
	(i tak jest domyœlnie) to gdy aktywna bêdzie pauza to czas szybkoœci naszej animacji zostanie odœwie¿ony
	aktualnym czasem. Jest to wymagane w przypadkach gdy nie chcemy aby po anulowaniu pauzy dana animacja od razu
	ruszy³a (znaczy aby CSpeedControl::CheckAnimationTime od razu zwróci³ \p TRUE) a stanie siê tak je¿eli czas
	pauza potrwa d³u¿ej ni¿ podana \a ulMs. Dziêki ustawieniu tej wartoœci na \p GL_TRUE pauza nie bêdzie mia³a
	wp³ywu na czas – czas jakby stanie w miejscu.
	\return \p 0 je¿eli jeszcze nie up³ynê³a podana w \a ulMs liczba milisekund. Liczba wiêksza od zera – je¿eli
	up³ynê³a podana liczba milisekund i wtedy mo¿emy wykonaæ dan¹ animacjê, np. podmieniæ tekstury. Zwracana liczba
	wiêksza od \p 0 to aktualny czas animacji jaki up³yn¹³.
	*/
	GLuint CheckAnimationTime( unsigned long ulMs, GLint iIndex, GLboolean bUpdateIfPause = GL_TRUE );

	/// Metoda zwracajaca aktualny czas jaki up³yn¹³ dla dnaje instancji animacji.
	/**
	\param[in] iIndex Indeks (liczony od 0) zwrócony przez CSpeedControl::CreateAnimationControl. Indeksy przypisane
	s¹ danym animacj¹. Przypisanie to jest dowolne i okreœlane przez nas.
	\return Aktualna liczba milisekund jaka up³ynê³a dla danej instancji animacji. */
	GLint GetActualAnimationTime( GLint iIndex );

	/// Metoda aktualizuj¹ca czas dla danej instancji animacji.
	/** Je¿eli nie zawsze w ka¿dej klatce bêdziemy mogli wywo³aæ CSpeedControl::CheckAnimationTime() bo, np.
	uzale¿nione to bêdzie od jakiejœ instrukcji warunkowej, to je¿eli po d³ugim okresie czasu, np. dwóch sekundach
	wywo³amy, np. \p CheckAnimationTime(500, ...) to funkcja ta zawsze zwróci wynik wiêkszy od \p 0. Je¿eli jednak
	chcemy aby odczeka³ te \p 500 milisekund to w miejscu tego¿ warunku tworzymy \p else i tam wywo³ujemy
	\p UpdateAnimationTime.
	\param[in] iIndex Indeks (liczony od 0) zwrócony przez CSpeedControl::CreateAnimationControl. Indeksy przypisane
	s¹ danym animacj¹. Przypisanie to jest dowolne i okreœlane przez nas. */
	GLvoid UpdateAnimationTime( GLint iIndex );
	//@}
private:
	/// Mno¿nik dla wszelkiego ruchu animacji w renderingu.
	/** Wartoœæ tego mno¿lika jest ustawiana co klatkê w metodze CSpeedControl::RefreshTime. Wartoœæ mno¿lika nale¿y
	pobraæ metod¹ CSpeedControl::GetMultiplier i mno¿liæ przez wszelkie zmienne odpowiedzialne za animacjê, jak
	przesuwanie, obracanie, itp.*/
	GLfloat m_fMultiplier;

	/// Zmienna pomocnicza przy obliczaniu aktualnego czasu dla CSpeedControl::m_fMultiplier.
	GLfloat m_fTimeLastFrame;

	/// Zmienna pomocnicza przy obliczaniu aktualnego czasu dla CSpeedControl::m_fMultiplier.
	/** Je¿eli nie odpowiada nam szybkoœæ zwracana przez CSpeedControl::GetMultiplier, mo¿emy w konstruktorze klasy
	CSpeedControl spróbowaæ zmodyfikowaæ wartoœæ przypisywan¹ do zmiennej CSpeedControl::m_fRegulationSpeed.
	Je¿eli zmniejszymy przypisywan¹ wartoœæ dla tej zmiennej, wówczas animacja bêdzie wolniejsza. */
	GLfloat m_fRegulationSpeed;

	/// Metoda zwracaj¹ca iloœæ instancji animacji.
	inline GLint GetAnimationSize() { return static_cast<GLint>( m_aAnimation.size() ); }

	/// Tablica przechowuj¹ca aktualne czasy (w milisekundach) dla danych instancji animacji.
	std::vector<unsigned long> m_aAnimation;

	/// Metoda zwracaj¹ca aktualny czas w milisekundach.
	/*! \return Aktualny czas (w milisekundach) jaki up³yn¹³ od uruchomienia systemu.	*/
	unsigned long GetTimeInMs();

	GLboolean m_bPause; ///< Flaga wskazuj¹ca czy aktywna jest pauza.
};