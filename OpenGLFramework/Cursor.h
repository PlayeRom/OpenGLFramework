/*! \file
	\brief Plik nag³ówkowy dla klasy CCursor.
*/
#pragma once

/// Definicja sta³ej maksymalnej liczby kursorów.
#define MAX_CURSOR 3

/// Klasa zarz¹dzaj¹ca kursorami.
class CCursor  
{
public:
	/// Konstruktor.
	/** Konstruktor. */
	CCursor();

	/// Destruktor.
	/** Destruktor. */
	virtual ~CCursor();

	/// Metoda ustawiaj¹ca kursor.
	/** \param[in] iIndex Indeks kursora, który chcemy uaktywniæ. */
	GLvoid SetMyCursor(GLint iIndex);

	/// Metoda zwracaj¹ca indeks aktualnie ustawionego kursora.
	/** \return Liczba ca³kowita zwracaj¹ca indeks aktualnie ustawionego ursora. */
	inline GLint GetMyCursor()	{ return m_iCursor; }

private:
	HCURSOR m_hCursor[MAX_CURSOR];	///< Tablica przechowuj¹ca wczytane kursory.

	/// Zmienna pamiêtaj¹ca indeks ostatnio ustawionego kursora.
	GLint m_iCursor;
};