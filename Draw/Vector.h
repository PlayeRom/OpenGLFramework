/*! \file
	\brief Plik nag³ówkowy dla klas CVector2, CVector3, CVector3d oraz CVector4.
*/
#pragma once

#include <math.h>
/*
#pragma pack(push)
#pragma pack(1)
...
#pragma pack(pop)
*/

/// Klasa reprezentuj¹ca wektor 2D.
class CVector2
{
public:
	/// Konstruktor.
	/** Konstruktor domyœlny inicjalizuj¹cy atrybuty zerami. */
	CVector2();

	/// Konstruktor z przypisaniem.
	/** \param[in] fX Wspórzêdna X.
	\param[in] fY Wspó³rzêdna Y. */
	CVector2(GLfloat fX, GLfloat fY);

	/// Konstruktor z przypisaniem.
	/** \param[in] cVec Referencja na obiekt klasy CVector2 inicjalizuj¹ca atrybuty. */
	CVector2(const CVector2 &cVec);

	/// Destruktor.
	/** Destruktor. */
	~CVector2();

	/// Metoda normalizuj¹ca wektor.
	/** Metoda przycina d³ugoœæ wektora do rozmiaru równego dok³adnie \p 1. */
	GLvoid Normalize();

	/// Przeci¹¿ony operator dodawania wektorów. 
	/** \param[in] cVec Referencja na wektor, który bêdzie dodawany. 
	\return Nowy wektor jako rezultat dodawania. */
	CVector2 operator+(const CVector2 &cVec) const;

	/// Przeci¹¿ony operator odejmowania wektorów. 
	/** \param[in] cVec Referencja na wektor, który bêdzie odejmowany. 
	\return Nowy wektor jako rezultat odejmowania. */
	CVector2 operator-(const CVector2 &cVec) const;

	/// Przeci¹¿ony operator mno¿enia wektorów. 
	/** \param[in] cVec Referencja na wektor, przez który bêdziemy mno¿yli. 
	\return Nowy wektor jako rezultat mno¿enia. */
	CVector2 operator*(const CVector2 &cVec) const;

	/// Przeci¹¿ony operator dzielenia wektorów. 
	/** \param[in] cVec Referencja na wektor, przez który bêdziemy dzieliæ. 
	\return Nowy wektor jako rezultat dzielenia. */
	CVector2 operator/(const CVector2 &cVec) const;

	/// Przeci¹¿ony operator dodawania skalara do wektora. 
	/** \param[in] fValue Skalar, który bêdzie dodany do wektora. 
	\return Nowy wektor jako rezultat dodania skalara. */
	CVector2 operator+(GLfloat fValue) const;

	/// Przeci¹¿ony operator odejmowania skalara od wektora. 
	/** \param[in] fValue Skalar, który bêdzie odejmowany od wektora. 
	\return Nowy wektor jako rezultat odejmowania skalara. */
	CVector2 operator-(GLfloat fValue) const;

	/// Przeci¹¿ony operator mno¿enia wektora przez skalar. 
	/** \param[in] fValue Skalar, przez który bêdzie mno¿ony wektor. 
	\return Nowy wektor jako rezultat mno¿enia przez skalara. */
	CVector2 operator*(GLfloat fValue) const;

	/// Przeci¹¿ony operator dzielenia wektora przez skalar. 
	/** \param[in] fValue Skalar, przez który bêdzie dzielony wektor. 
	\return Nowy wektor jako rezultat dzielenia przez skalara. */
	CVector2 operator/(GLfloat fValue) const;

	/// Przeci¹¿ony operator przypisania dodawania wektora. 
	/** \param[in] cVec Referencja na wektor, który bêdzie dodawany. 
	\return Zmodyfikowany wektor jako rezultat dodawania. */
	CVector2& operator+=(const CVector2 &cVec);

	/// Przeci¹¿ony operator przypisania odejmowania wektora. 
	/** \param[in] cVec Referencja na wektor, który bêdzie odejmowany. 
	\return Zmodyfikowany wektor jako rezultat odejmowania. */
	CVector2& operator-=(const CVector2 &cVec);

	/// Przeci¹¿ony operator przypisania mno¿enia wektora. 
	/** \param[in] cVec Referencja na wektor, który bêdzie mno¿ony. 
	\return Zmodyfikowany wektor jako rezultat mno¿enia. */
	CVector2& operator*=(const CVector2 &cVec);

	/// Przeci¹¿ony operator przypisania dodawania skalara. 
	/** \param[in] fValue Skalar, który bêdzie dodawany do wektora. 
	\return Zmodyfikowany wektor jako rezultat dodawania skalara. */
	CVector2& operator+=(GLfloat fValue);

	/// Przeci¹¿ony operator przypisania odejmowania skalara. 
	/** \param[in] fValue Skalar, który bêdzie odejmowany od wektora. 
	\return Zmodyfikowany wektor jako rezultat odejmowania skalara. */
	CVector2& operator-=(GLfloat fValue);

	/// Przeci¹¿ony operator przypisania mno¿enia przez skalara. 
	/** \param[in] fValue Skalar, który bêdzie mno¿ony przez wektor. 
	\return Zmodyfikowany wektor jako rezultat mno¿enia przez skalar. */
	CVector2& operator*=(GLfloat fValue);

	/// Przeci¹¿ony operator minus, jako zmiana zwrotu wektora na przeciwny.
	/** \return Nowy wektor z przeciwnym zwrotem. */
	CVector2 operator-() const;

	/// Przeci¹¿enie operatora plus.
	/** \return Ten sam wektor. */
	CVector2 operator+() const ;

	/// Przeci¹¿ony operator przypisania wektora.
	/** \param[in] cVec Referenzja na wektor, który zostanie przypisany aktualnemu.
	\return Zmodyfikowany, aktualny wektor o przypisany. */
	CVector2& operator=(const CVector2 &cVec);

	/// Przeci¹¿ony operator porównania wektorów.
	/** \param[in] cVec Referencja na wektor, do którego bêdzie porównywany aktualny.
	\return \p GL_TRUE gdy wektory s¹ sobie równe, w przeciwnym wypadku \p GL_FALSE. */
	GLboolean operator==(const CVector2 &cVec) const;

	/// Metoda obliczaj¹ca odleg³oœæ miêdzy aktualnym i podanym wektorem.
	/** \param[in] cVec Referencja na drugi wektor, do którego bêdzie obliczana odleg³oœæ. 
	\return Liczba rzeczywista okreœlaj¹ca odleg³oœæ miêdzy punktami wektorów. */
	inline GLfloat CalcDistance2D(const CVector2 &cVec){
		GLfloat fX = (x - cVec.x) * (x - cVec.x);
		GLfloat fY = (y - cVec.y) * (y - cVec.y);
		return sqrt(fX + fY);
	}

	/// Metoda obliczaj¹ca d³ugoœæ wektoar.
	/** \return Liczba rzeczywista okreœlaj¹ca d³ugoœæ wektora. */
	inline GLfloat GetLength(){
		return sqrt((x * x) + (y * y));
	}

	GLfloat x;	///< Wspó³rzêdna x wektora.
	GLfloat y;	///< Wspó³rzêdna y wektora.
};

/// Klasa reprezentuj¹ca wektor 4D.
class CVector4
{
public:
	/// Konstruktor.
	/** Konstruktor domyœlny inicjalizuj¹cy atrybuty zerami. */
	CVector4();

	/// Konstruktor z przypisaniem.
	/** \param[in] fX Wspórzêdna X.
	\param[in] fY Wspó³rzêdna Y
	\param[in] fZ Wspó³rzêdna Z.
	\param[in] fW Wspó³rzêdna W. */
	CVector4(GLfloat fX, GLfloat fY, GLfloat fZ, GLfloat fW);

	/// Konstruktor z przypisaniem.
	/** \param[in] cVec Referencja na obiekt klasy CVector4 inicjalizuj¹ca atrybuty. */
	CVector4(const CVector4 &cVec);

	/// Destruktor.
	/** Destruktor. */
	~CVector4();

	/// Przeci¹¿ony operator dodawania wektorów. 
	/** \param[in] cVec Referencja na wektor, który bêdzie dodawany. 
	\return Nowy wektor jako rezultat dodawania. */
	CVector4 operator+(const CVector4 &cVec) const;

	/// Przeci¹¿ony operator odejmowania wektorów. 
	/** \param[in] cVec Referencja na wektor, który bêdzie odejmowany. 
	\return Nowy wektor jako rezultat odejmowania. */
	CVector4 operator-(const CVector4 &cVec) const;

	/// Przeci¹¿ony operator przypisania wektora.
	/** \param[in] cVec Referenzja na wektor, który zostanie przypisany aktualnemu.
	\return Zmodyfikowany, aktualny wektor o przypisany. */
	CVector4& operator=(const CVector4 &cVec);

	/// Przeci¹¿ony operator porównania wektorów.
	/** \param[in] cVec Referencja na wektor, do którego bêdzie porównywany aktualny.
	\return \p GL_TRUE gdy wektory s¹ sobie równe, w przeciwnym wypadku \p GL_FALSE. */
	GLboolean operator==(const CVector4 &cVec) const;

	/// Przeci¹¿ony operator przypisania dodawania wektora. 
	/** \param[in] cVec Referencja na wektor, który bêdzie dodawany. 
	\return Zmodyfikowany wektor jako rezultat dodawania. */
	inline CVector4& operator+=(const CVector4 &cVec) {
		x += cVec.x; y += cVec.y; z += cVec.z;
		return *this;
	}

	/// Przeci¹¿ony operator przypisania odejmowania wektora. 
	/** \param[in] cVec Referencja na wektor, który bêdzie odejmowany. 
	\return Zmodyfikowany wektor jako rezultat odejmowania. */
	inline CVector4& operator-=(const CVector4 &cVec) {
		x -= cVec.x; y -= cVec.y; z -= cVec.z; 
		return *this;
	}

	/// Przeci¹¿ony operator przypisania mno¿enia wektora. 
	/** \param[in] cVec Referencja na wektor, który bêdzie mno¿ony. 
	\return Zmodyfikowany wektor jako rezultat mno¿enia. */
	inline CVector4& operator*=(const CVector4 &cVec) {
		x *= cVec.x; y *= cVec.y; z *= cVec.z;
		return *this;
	}

	/// Metoda obliczaj¹ca iloczyn skalarny miêdzy aktualnym i podanym wektorem.
	/** \param[in] cVec Referencja na drugi wektor, miêdzy którym bêdzie liczony iloczyn skalarny.
	\return Liczba rzeczywista okreœlaj¹ca iloczyn skalarny. */
	inline GLfloat DotProduct(const CVector4 &cVec){
		return	x * cVec.x +
				y * cVec.y +
				z * cVec.z +
				w * cVec.w;
	}

	GLfloat x;	///< Wspó³rzêdna x wektora.
	GLfloat y;	///< Wspó³rzêdna y wektora.
	GLfloat z;	///< Wspó³rzêdna z wektora.
	GLfloat w;	///< Wspó³rzêdna w wektora.
};

/// Klasa reprezentuj¹ca wektor 3D typu float.
class CVector3
{
public:
	/// Konstruktor.
	/** Konstruktor domyœlny inicjalizuj¹cy atrybuty zerami. */
	CVector3(): x(0.0f), y(0.0f), z(0.0f) {}

	/// Konstruktor z przypisaniem.
	/** \param[in] fX Wspórzêdna X.
	\param[in] fY Wspó³rzêdna Y
	\param[in] fZ Wspó³rzêdna Z. */
	CVector3(GLfloat fX, GLfloat fY, GLfloat fZ): x(fX), y(fY), z(fZ) {}

	/// Konstruktor z przypisaniem.
	/** \param[in] cVec Referencja na obiekt klasy CVector3 inicjalizuj¹ca atrybuty. */
	CVector3(const CVector3 &cVec) { x = cVec.x; y = cVec.y; z = cVec.z; }
	
	/// Destruktor.
	/** Destruktor. */
	~CVector3() {}

	/// Metoda normalizuj¹ca wektor.
	/** Metoda przycina d³ugoœæ wektora do rozmiaru równego dok³adnie \p 1. */
	GLvoid Normalize();

	/// Przeci¹¿ony operator dodawania wektorów. 
	/** \param[in] cVec Referencja na wektor, który bêdzie dodawany. 
	\return Nowy wektor jako rezultat dodawania. */
	inline CVector3 operator+(const CVector3 &cVec) const {
		return CVector3(x + cVec.x, y + cVec.y, z + cVec.z);
	}

	/// Przeci¹¿ony operator odejmowania wektorów. 
	/** \param[in] cVec Referencja na wektor, który bêdzie odejmowany. 
	\return Nowy wektor jako rezultat odejmowania. */
	inline CVector3 operator-(const CVector3 &cVec) const {
		return CVector3(x - cVec.x,	y - cVec.y,	z - cVec.z);
	}

	/// Przeci¹¿ony operator odejmowania wektorów. 
	/** \param[in] cVec Referencja na wektor CVector4, który bêdzie odejmowany z pominiêciem sk³adowej CVector4::w.
	\return Nowy wektor jako rezultat odejmowania. */
	inline CVector3 operator-(const CVector4 &cVec) const {
		return CVector3(x - cVec.x,	y - cVec.y,	z - cVec.z);
	}

	/// Przeci¹¿ony operator mno¿enia wektorów. 
	/** \param[in] cVec Referencja na wektor, przez który bêdziemy mno¿yli. 
	\return Nowy wektor jako rezultat mno¿enia. */
	inline CVector3 operator*(const CVector3 &cVec) const {
		return CVector3(x * cVec.x, y * cVec.y,	z * cVec.z);
	}

	/// Przeci¹¿ony operator dzielenia wektorów. 
	/** \param[in] cVec Referencja na wektor, przez który bêdziemy dzieliæ. 
	\return Nowy wektor jako rezultat dzielenia. */
	inline CVector3 operator/(const CVector3 &cVec) const {
		return CVector3(x / cVec.x,	y / cVec.y,	z / cVec.z);
	}

	/// Przeci¹¿ony operator dodawania skalara do wektora. 
	/** \param[in] fValue Skalar, który bêdzie dodany do wektora. 
	\return Nowy wektor jako rezultat dodania skalara. */
	inline CVector3 operator+(GLfloat fValue) const {
		return CVector3(x + fValue,	y + fValue,	z + fValue);
	}

	/// Przeci¹¿ony operator odejmowania skalara od wektora. 
	/** \param[in] fValue Skalar, który bêdzie odejmowany od wektora. 
	\return Nowy wektor jako rezultat odejmowania skalara. */
	inline CVector3 operator-(GLfloat fValue) const {
		return CVector3(x - fValue,	y - fValue,	z - fValue);
	}

	/// Przeci¹¿ony operator mno¿enia wektora przez skalar. 
	/** \param[in] fValue Skalar, przez który bêdzie mno¿ony wektor. 
	\return Nowy wektor jako rezultat mno¿enia przez skalara. */
	inline CVector3 operator*(GLfloat fValue) const {
		return CVector3(x * fValue,	y * fValue,	z * fValue);
	}

	/// Przeci¹¿ony operator dzielenia wektora przez skalar. 
	/** \param[in] fValue Skalar, przez który bêdzie dzielony wektor. 
	\return Nowy wektor jako rezultat dzielenia przez skalara. */
	inline CVector3 operator/(GLfloat fValue) const {
		return CVector3(x / fValue,	y / fValue,	z / fValue);
	}

	/// Przeci¹¿ony operator przypisania dodawania wektora. 
	/** \param[in] cVec Referencja na wektor, który bêdzie dodawany. 
	\return Zmodyfikowany wektor jako rezultat dodawania. */
	inline CVector3& operator+=(const CVector3 &cVec) {
		x += cVec.x; y += cVec.y; z += cVec.z;
		return *this;
	}

	/// Przeci¹¿ony operator przypisania odejmowania wektora. 
	/** \param[in] cVec Referencja na wektor, który bêdzie odejmowany. 
	\return Zmodyfikowany wektor jako rezultat odejmowania. */
	inline CVector3& operator-=(const CVector3 &cVec) {
		x -= cVec.x; y -= cVec.y; z -= cVec.z; 
		return *this;
	}

	/// Przeci¹¿ony operator przypisania mno¿enia wektora. 
	/** \param[in] cVec Referencja na wektor, który bêdzie mno¿ony. 
	\return Zmodyfikowany wektor jako rezultat mno¿enia. */
	inline CVector3& operator*=(const CVector3 &cVec) {
		x *= cVec.x; y *= cVec.y; z *= cVec.z;
		return *this;
	}

	/// Przeci¹¿ony operator przypisania dodawania skalara. 
	/** \param[in] fValue Skalar, który bêdzie dodawany do wektora. 
	\return Zmodyfikowany wektor jako rezultat dodawania skalara. */
	inline CVector3& operator+=(GLfloat fValue) {
		x += fValue; y += fValue; z += fValue;
		return *this;
	}

	/// Przeci¹¿ony operator przypisania odejmowania skalara. 
	/** \param[in] fValue Skalar, który bêdzie odejmowany od wektora. 
	\return Zmodyfikowany wektor jako rezultat odejmowania skalara. */
	inline CVector3& operator-=(GLfloat fValue) {
		x -= fValue; y -= fValue; z -= fValue;
		return *this;
	}

	/// Przeci¹¿ony operator przypisania mno¿enia przez skalara. 
	/** \param[in] fValue Skalar, który bêdzie mno¿ony przez wektor. 
	\return Zmodyfikowany wektor jako rezultat mno¿enia przez skalar. */
	inline CVector3& operator*=(GLfloat fValue) {
		x *= fValue; y *= fValue; z *= fValue;
		return *this;
	}

	/// Przeci¹¿ony operator minus, jako zmiana zwrotu wektora na przeciwny.
	/** \return Nowy wektor z przeciwnym zwrotem. */
	inline CVector3 operator-() const { return CVector3(-x, -y, -z); }

	/// Przeci¹¿enie operatora plus.
	/** \return Ten sam wektor. */
	inline CVector3 operator+() const { return *this; }

	/// Przeci¹¿ony operator przypisania wektora.
	/** \param[in] cVec Referenzja na wektor, który zostanie przypisany aktualnemu.
	\return Zmodyfikowany, aktualny wektor. */
	inline CVector3& operator=(const CVector3 &cVec) {
		x = cVec.x; y = cVec.y; z = cVec.z;
		return *this;
	}

	/// Przeci¹¿ony operator przypisania wektora.
	/** \param[in] fValue Wartoœæ, która zostanie przypisana wektorowi.
	\return Zmodyfikowany, aktualny wektor. */
	inline CVector3& operator=(GLfloat fValue) {
		x = fValue; y = fValue; z = fValue;
		return *this;
	}

	/// Przeci¹¿ony operator porównania wektorów.
	/** \param[in] cVec Referencja na wektor, do którego bêdzie porównywany aktualny.
	\return \p GL_TRUE gdy wektory s¹ sobie równe, w przeciwnym wypadku \p GL_FALSE. */
	inline GLboolean operator==(const CVector3 &cVec) const {
		return (x == cVec.x && y == cVec.y && z == cVec.z);
	}

	/// Przeci¹¿ony operator porównania wektora ze skalarem.
	/** \param[in] fValue Skalar, z którym bêdzie porównywany wektor.
	\return \p GL_TRUE gdy wspó³rzêdne wektora s¹ równy skalarowi, w przeciwnym wypadku \p GL_FALSE. */
	inline GLboolean operator==(GLfloat fValue) const {
		return (x == fValue && y == fValue && z == fValue);
	}

	/// Przeci¹¿ony operator ró¿nicy wektorów.
	/** \param[in] cVec Referencja na wektor, do którego bêdzie porównywany aktualny.
	\return \p GL_TRUE gdy wektory s¹ sobie ró¿ne, w przeciwnym wypadku \p GL_FALSE. */
	inline GLboolean operator!=(const CVector3 &cVec) const { return !( this->operator ==( cVec ) ); }

	/// Przeci¹¿ony operator ró¿nicy wektora ze skalarem.
	/** \param[in] fValue Skalar, z którym bêdzie porównywany wektor.
	\return \p GL_TRUE gdy przynajmniej jedna ze wspó³rzêdnych wektora jest ró¿na od skalara,
	w przeciwnym wypadku \p GL_FALSE. */
	inline GLboolean operator!=(GLfloat fValue) const { return !( this->operator ==( fValue ) ); }

	/// Metoda obliczaj¹ca d³ugoœæ wektoar.
	/** \return Liczba rzeczywista okreœlaj¹ca d³ugoœæ wektora. */
	inline GLfloat GetLength(){
		return sqrt((x * x) + (y * y) + (z * z));
	}
	
	/// Metoda obliczaj¹ca odleg³oœæ miêdzy aktualnym i podanym wektorem.
	/** \param[in] cVec Referencja na drugi wektor, do którego bêdzie obliczana odleg³oœæ. 
	\return Liczba rzeczywista okreœlaj¹ca odleg³oœæ miêdzy punktami wektorów. */
	inline GLfloat CalcDistance3D(const CVector3 &cVec){
		GLfloat fX = (x - cVec.x) * (x - cVec.x);
		GLfloat fY = (y - cVec.y) * (y - cVec.y);
		GLfloat fZ = (z - cVec.z) * (z - cVec.z);
		return sqrt(fX + fY + fZ);
	}

	/// Metoda obliczaj¹ca odleg³oœæ miêdzy aktualnym i podanym wektorem na p³aszczyŸnie X-Y.
	/** \param[in] cVec Referencja na drugi wektor, do którego bêdzie obliczana odleg³oœæ. 
	\return Liczba rzeczywista okreœlaj¹ca odleg³oœæ miêdzy punktami wektorów na p³aszczyŸnie X-Y. */
	inline GLfloat CalcDistanceXY(const CVector3 &cVec){
		GLfloat fX = (x - cVec.x) * (x - cVec.x);
		GLfloat fY = (y - cVec.y) * (y - cVec.y);
		return sqrt(fX + fY);
	}

	/// Metoda obliczaj¹ca odleg³oœæ miêdzy aktualnym i podanym wektorem na p³aszczyŸnie X-Z.
	/** \param[in] cVec Referencja na drugi wektor, do którego bêdzie obliczana odleg³oœæ. 
	\return Liczba rzeczywista okreœlaj¹ca odleg³oœæ miêdzy punktami wektorów na p³aszczyŸnie X-Z. */
	inline GLfloat CalcDistanceXZ(const CVector3 &cVec){
		GLfloat fX = (x - cVec.x) * (x - cVec.x);
		GLfloat fZ = (z - cVec.z) * (z - cVec.z);
		return sqrt(fX + fZ);
	}

	/// Metoda obliczaj¹ca odleg³oœæ miêdzy aktualnym i podanym wektorem na p³aszczyŸnie Y-Z.
	/** \param[in] cVec Referencja na drugi wektor, do którego bêdzie obliczana odleg³oœæ. 
	\return Liczba rzeczywista okreœlaj¹ca odleg³oœæ miêdzy punktami wektorów na p³aszczyŸnie Y-Z. */
	inline GLfloat CalcDistanceYZ(const CVector3 &cVec){
		GLfloat fY = (y - cVec.y) * (y - cVec.y);
		GLfloat fZ = (z - cVec.z) * (z - cVec.z);
		return sqrt(fY + fZ);
	}

	/// Metoda obliczaj¹ca odleg³oœæ miêdzy aktualnym i podanym wektorem na osi X.
	/** \param[in] cVec Referencja na drugi wektor, do którego bêdzie obliczana odleg³oœæ. 
	\return Liczba rzeczywista okreœlaj¹ca odleg³oœæ miêdzy punktami wektorów na osi X. */
	inline GLfloat CalcDistanceX(const CVector3 &cVec){
		return sqrt((x - cVec.x) * (x - cVec.x));
	}

	/// Metoda obliczaj¹ca odleg³oœæ miêdzy aktualnym i podanym wektorem na osi Y.
	/** \param[in] cVec Referencja na drugi wektor, do którego bêdzie obliczana odleg³oœæ. 
	\return Liczba rzeczywista okreœlaj¹ca odleg³oœæ miêdzy punktami wektorów na osi Y. */
	inline GLfloat CalcDistanceY(const CVector3 &cVec){
		return sqrt((y - cVec.y) * (y - cVec.y));
	}

	/// Metoda obliczaj¹ca odleg³oœæ miêdzy aktualnym i podanym wektorem na osi Z.
	/** \param[in] cVec Referencja na drugi wektor, do którego bêdzie obliczana odleg³oœæ. 
	\return Liczba rzeczywista okreœlaj¹ca odleg³oœæ miêdzy punktami wektorów na osi Z. */
	inline GLfloat CalcDistanceZ(const CVector3 &cVec){
		return sqrt((z - cVec.z) * (z - cVec.z));
	}

	/// Przeci¹zony operator [] do wy³uskania sk³adowych wektora.
	/** \param[in] iIndex Indeks sk³adowej wektora, gdzie odpowiednio indeks \p 0 odpowiada wartoœci CVector3::x,
	\p 1 CVector3::y, \p 2 CVector3::z. 
	\return Wy³uskany jeden z atrybutów klasy. */
	inline GLfloat& operator[](GLint iIndex){
		return (&x)[iIndex];
	}

	/// Metoda obliczaj¹ca iloczyn skalarny miêdzy aktualnym i podanym wektorem.
	/** \param[in] cVec Referencja na drugi wektor, miêdzy którym bêdzie liczony iloczyn skalarny.
	\return Liczba rzeczywista okreœlaj¹ca iloczyn skalarny. */
	inline GLfloat DotProduct(const CVector3 &cVec){
		return	x * cVec.x +
				y * cVec.y +
				z * cVec.z;
	}

	/// Metoda obliczaj¹ca iloczyn wektorowy miêdzy aktualnym i podanym wektorem.
	/** \param[in] cVec Referencja na drugi wektor, miêdzy którym bêdzie obliczany iloczyn wektorowy.
	\return Liczba rzeczywista okreœlaj¹ca iloczyn wektorowy. */
	inline CVector3 CrossProduct(const CVector3 &cVec){
		CVector3 cTemp;
		cTemp.x = (y * cVec.z) - (z * cVec.y);
		cTemp.y = (z * cVec.x) - (x * cVec.z);
		cTemp.z = (x * cVec.y) - (y * cVec.x); 
		return cTemp;
	}

	GLfloat x;	///< Wspó³rzêdna x wektora.
	GLfloat y;	///< Wspó³rzêdna y wektora.
	GLfloat z;	///< Wspó³rzêdna z wektora.
};

/// Klasa reprezentuj¹ca wektor 3D typu double.
class CVector3d
{
public:
	/// Konstruktor.
	/** Konstruktor domyœlny inicjalizuj¹cy atrybuty zerami. */
	CVector3d() { x = y = z = 0.0; }

	/// Konstruktor z przypisaniem.
	/** \param[in] dX Wspórzêdna X.
	\param[in] dY Wspó³rzêdna Y
	\param[in] dZ Wspó³rzêdna Z. */
	CVector3d(GLdouble dX, GLdouble dY, GLdouble dZ){ x = dX; y = dY; z = dZ; }

	/// Konstruktor z przypisaniem.
	/** \param[in] cVec Referencja na obiekt klasy CVector3d inicjalizuj¹ca atrybuty. */
	CVector3d(const CVector3d &cVec){ x = cVec.x; y = cVec.y; z = cVec.z; }

	/// Destruktor.
	/** Destruktor. */
	~CVector3d() {}

	/// Metoda normalizuj¹ca wektor.
	/** Metoda przycina d³ugoœæ wektora do rozmiaru równego dok³adnie \p 1. */
	GLvoid Normalize();

	/// Przeci¹¿ony operator dodawania wektorów. 
	/** \param[in] cVec Referencja na wektor, który bêdzie dodawany. 
	\return Nowy wektor jako rezultat dodawania. */
	inline CVector3d operator+(const CVector3d &cVec) const{
		return CVector3d(x + cVec.x, y + cVec.y, z + cVec.z);
	}

	/// Przeci¹¿ony operator odejmowania wektorów. 
	/** \param[in] cVec Referencja na wektor, który bêdzie odejmowany. 
	\return Nowy wektor jako rezultat odejmowania. */
	inline CVector3d operator-(const CVector3d &cVec) const{
		return CVector3d(x - cVec.x, y - cVec.y, z - cVec.z);
	}

	/// Przeci¹¿ony operator mno¿enia wektorów. 
	/** \param[in] cVec Referencja na wektor, przez który bêdziemy mno¿yli. 
	\return Nowy wektor jako rezultat mno¿enia. */
	inline CVector3d operator*(const CVector3d &cVec) const{
		return CVector3d(x * cVec.x, y * cVec.y, z * cVec.z);
	}

	/// Przeci¹¿ony operator dzielenia wektorów. 
	/** \param[in] cVec Referencja na wektor, przez który bêdziemy dzieliæ. 
	\return Nowy wektor jako rezultat dzielenia. */
	inline CVector3d operator/(const CVector3d &cVec) const{
		return CVector3d(x / cVec.x, y / cVec.y, z / cVec.z);
	}

	/// Przeci¹¿ony operator dodawania skalara do wektora. 
	/** \param[in] dValue Skalar, który bêdzie dodany do wektora. 
	\return Nowy wektor jako rezultat dodania skalara. */
	inline CVector3d operator+(GLdouble dValue) const{
		return CVector3d(x + dValue, y + dValue, z + dValue);
	}

	/// Przeci¹¿ony operator odejmowania skalara od wektora. 
	/** \param[in] dValue Skalar, który bêdzie odejmowany od wektora. 
	\return Nowy wektor jako rezultat odejmowania skalara. */
	inline CVector3d operator-(GLdouble dValue) const{
		return CVector3d(x - dValue, y - dValue, z - dValue);
	}

	/// Przeci¹¿ony operator mno¿enia wektora przez skalar. 
	/** \param[in] dValue Skalar, przez który bêdzie mno¿ony wektor. 
	\return Nowy wektor jako rezultat mno¿enia przez skalara. */
	inline CVector3d operator*(GLdouble dValue) const{
		return CVector3d(x * dValue, y * dValue, z * dValue);
	}

	/// Przeci¹¿ony operator dzielenia wektora przez skalar. 
	/** \param[in] dValue Skalar, przez który bêdzie dzielony wektor. 
	\return Nowy wektor jako rezultat dzielenia przez skalara. */
	inline CVector3d operator/(GLdouble dValue) const{
		return CVector3d(x / dValue, y / dValue, z / dValue);
	}

	/// Przeci¹¿ony operator przypisania dodawania wektora. 
	/** \param[in] cVec Referencja na wektor, który bêdzie dodawany. 
	\return Zmodyfikowany wektor jako rezultat dodawania. */
	inline CVector3d& operator+=(const CVector3d &cVec){
		x += cVec.x; y += cVec.y; z += cVec.z;
		return *this;
	}

	/// Przeci¹¿ony operator przypisania odejmowania wektora. 
	/** \param[in] cVec Referencja na wektor, który bêdzie odejmowany. 
	\return Zmodyfikowany wektor jako rezultat odejmowania. */
	inline CVector3d& operator-=(const CVector3d &cVec){
		x -= cVec.x; y -= cVec.y; z -= cVec.z;
		return *this;
	}

	/// Przeci¹¿ony operator przypisania mno¿enia wektora. 
	/** \param[in] cVec Referencja na wektor, który bêdzie mno¿ony. 
	\return Zmodyfikowany wektor jako rezultat mno¿enia. */
	inline CVector3d& operator*=(const CVector3d &cVec){
		x *= cVec.x; y *= cVec.y; z *= cVec.z;
		return *this;
	}

	/// Przeci¹¿ony operator przypisania dodawania skalara. 
	/** \param[in] dValue Skalar, który bêdzie dodawany do wektora. 
	\return Zmodyfikowany wektor jako rezultat dodawania skalara. */
	inline CVector3d& operator+=(GLdouble dValue){
		x += dValue; y += dValue; z += dValue;
		return *this;
	}

	/// Przeci¹¿ony operator przypisania odejmowania skalara. 
	/** \param[in] dValue Skalar, który bêdzie odejmowany od wektora. 
	\return Zmodyfikowany wektor jako rezultat odejmowania skalara. */
	inline CVector3d& operator-=(GLdouble dValue){
		x -= dValue; y -= dValue; z -= dValue;
		return *this;
	}

	/// Przeci¹¿ony operator przypisania mno¿enia przez skalara. 
	/** \param[in] dValue Skalar, który bêdzie mno¿ony przez wektor. 
	\return Zmodyfikowany wektor jako rezultat mno¿enia przez skalar. */
	inline CVector3d& operator*=(GLdouble dValue){
		x *= dValue; y *= dValue; z *= dValue;
		return *this;
	}

	/// Przeci¹¿ony operator minus, jako zmiana zwrotu wektora na przeciwny.
	/** \return Nowy wektor z przeciwnym zwrotem. */
	inline CVector3d operator-() const{
		return CVector3d(-x, -y, -z);
	}

	/// Przeci¹¿enie operatora plus.
	/** \return Ten sam wektor. */
	inline CVector3d operator+() const { return *this; }

	/// Przeci¹¿ony operator przypisania wektora.
	/** \param[in] cVec Referenzja na wektor, który zostanie przypisany aktualnemu.
	\return Zmodyfikowany, aktualny wektor o przypisany. */
	inline CVector3d& operator=(const CVector3d &cVec){
		x = cVec.x; y = cVec.y; z = cVec.z;
		return *this;
	}

	/// Przeci¹¿ony operator porównania wektorów.
	/** \param[in] cVec Referencja na wektor, do którego bêdzie porównywany aktualny.
	\return \p GL_TRUE gdy wektory s¹ sobie równe, w przeciwnym wypadku \p GL_FALSE. */
	inline GLboolean operator==(const CVector3d &cVec) const{
		return (x == cVec.x && y == cVec.y && z == cVec.z);
	}

	/// Przeci¹¿ony operator porównania wektora ze skalarem.
	/** \param[in] dValue Skalar, z którym bêdzie porównywany wektor.
	\return \p GL_TRUE gdy wspó³rzêdne wektora s¹ równy skalarowi, w przeciwnym wypadku \p GL_FALSE. */
	inline GLboolean operator==(GLdouble dValue) const{
		return (x == dValue && y == dValue && z == dValue);
	}

	/// Metoda obliczaj¹ca d³ugoœæ wektoar.
	/** \return Liczba rzeczywista okreœlaj¹ca d³ugoœæ wektora. */
	inline GLdouble GetLength(){
		return sqrt((x * x) + (y * y) + (z * z));
	}

	GLdouble x;	///< Wspó³rzêdna x wektora.
	GLdouble y;	///< Wspó³rzêdna y wektora.
	GLdouble z;	///< Wspó³rzêdna z wektora.
};