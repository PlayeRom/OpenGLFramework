/*! \file
	\brief Plik nag³ówkowy dla klasy CFog.
*/
#pragma once

/// Klasa zarz¹dzaj¹ca ustawieniami mg³y.
/** Klasa zawiera metody i dane niezbêdne do ustawienia wszelkich parametrów mg³y. */
class CFog
{
public:
	/// Konstruktor.
	/** Konstruktor. */
	CFog();

	/// Destruktor.
	/** Destruktor. */
	~CFog();

	/// Metoda ustawiaj¹ca kolor mg³y.
	/** \param[in] r Ska³adowa koloru Red.
	\param[in] g Ska³adowa koloru Green.
	\param[in] b Ska³adowa koloru Blue.
	\param[in] a Ska³adowa koloru Alpha. */
	inline GLvoid SetColor( GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.0f ) {
		m_aColor[ 0 ] = r;
		m_aColor[ 1 ] = g;
		m_aColor[ 2 ] = b;
		m_aColor[ 3 ] = a;
		glFogfv( GL_FOG_COLOR, m_aColor );
	}

	/// Metoda ustawiaj¹ca kolor mg³y.
	/** \param[in] aColour Tablica koloru RGBA. */
	inline GLvoid SetColor( GLfloat aColour[ 4 ] ) {
		m_aColor[ 0 ] = aColour[ 0 ];
		m_aColor[ 1 ] = aColour[ 1 ];
		m_aColor[ 2 ] = aColour[ 2 ];
		m_aColor[ 3 ] = aColour[ 3 ];
		glFogfv( GL_FOG_COLOR, m_aColor );
	}

	/// Metoda ustawiaj¹ca tryb mg³y.
	/** \param[in] fParam Parametr trybu mg³y. Mo¿e przyj¹æ \p GL_LINEAR, \p GL_EXP lub \p GL_EXP2. */
	inline GLvoid SetFogMode( GLfloat fParam = GL_EXP ) {
		glFogf( GL_FOG_MODE, fParam );
	}

	/// Metoda ustawiaj¹ca pocz¹tek mg³y.
	/** \param[in] fStart Wartoœæ pocz¹tku mg³y - jak blisko ekranu powinna zacz¹æ siê mg³a. */
	inline GLvoid SetStart( GLfloat fStart ) {
		m_fStart = fStart;
		glFogf( GL_FOG_START, m_fStart );
	}

	/// Metoda ustawiaj¹ca koniec mg³y.
	/** \param[in] fEnd Wartoœæ koñca mg³y - jak daleko od ekranu powinna skoñczyæ siê mg³a. */
	inline GLvoid SetEnd( GLfloat fEnd ) {
		m_fEnd = fEnd;
		glFogf( GL_FOG_END, m_fEnd );
	}

	/// Metoda ustawiaj¹ca pocz¹tek i koniec mg³y.
	/** \param[in] fStart Wartoœæ pocz¹tku mg³y - jak blisko ekranu powinna zacz¹æ siê mg³a.
	\param[in] fEnd Wartoœæ koñca mg³y - jak daleko od ekranu powinna skoñczyæ siê mg³a. */
	inline GLvoid SetStartEnd( GLfloat fStart, GLfloat fEnd ) {
		m_fStart = fStart;
		m_fEnd = fEnd;
		glFogf( GL_FOG_START, m_fStart );
		glFogf( GL_FOG_END, m_fEnd );
	}

	/// Metoda ustawiaj¹ca gêstoœæ mg³y
	/** \param[in] fDensity Wartoœæ gêstoœci mg³y. */
	inline GLvoid SetDensity( GLfloat fDensity = 1.0f ) {
		m_fDensity = fDensity;
		glFogf( GL_FOG_DENSITY, m_fDensity );
	}

	/// Metoda ustawiaj¹ca sposób tworzenia mg³y.
	/** \param[in] fMode Sposób tworzenia mg³y, mo¿na podaæ: \p GL_DONT_CARE lub \p GL_FASTEST dla wierzcho³kowego
	tworzenia mg³y lub \p GL_NICEST dla tworzenia ³adniejszej mg³y pikselowej. */
	inline GLvoid SetHint( GLenum fMode = GL_DONT_CARE ) {
		glHint( GL_FOG_HINT, fMode );
	}

private:
	GLfloat m_aColor[ 4 ];	///< Tablica aktualnie ustawionego koloru mg³y.
	GLfloat m_fStart;		///< Wartoœæ aktualnie ustawionego pocz¹tku mg³y.
	GLfloat m_fEnd;			///< Wartoœæ aktualnie ustawionego koñca mg³y.
	GLfloat m_fDensity;		///< Wartoœæ aktualnej gêstoœci mg³y
};
