/*! \file
	\brief Plik nag³ówkowy dla klasy CMatrixOperations.
*/
#pragma once

class CVector2;
class CVector3;

/// Klasa wspieraj¹ca obliczenia dla przekszta³ceñ dwu i trzywymiarowych.
class CMatrixOperations
{
public:
	/// Konstruktor.
	/** Konstruktor. */
	CMatrixOperations();

	/// Destruktor.
	/** Destruktor */
	virtual ~CMatrixOperations();

	/// Metoda przesuwaj¹ca wierzcho³ek 3D.
	/** \param[in] in_cVecActual Referencja na wektor, który chcemy przesun¹æ. 
	\param[in] in_cVecTrans Referencja na wektor przesuniêcia.
	\return Nowy, przesuniêty wektor. */
	CVector3 TranslateXYZ3(const CVector3 &in_cVecActual, const CVector3 &in_cVecTrans);

	/// Metoda przesuwaj¹ca wierzcho³ek 3D po osi X.
	/** \param[in] in_cVecActual Referencja na wektor, którego wspó³rzêdn¹ X chcemy przesun¹æ. 
	\param[in] fTransX Skalar przesuniêcia po osi X.
	\return Nowy, przesuniêty wektor po osi X. */
	CVector3 TranslateX3(const CVector3 &in_cVecActual, GLfloat fTransX);

	/// Metoda przesuwaj¹ca wierzcho³ek 3D po osi Y.
	/** \param[in] in_cVecActual Referencja na wektor, którego wspó³rzêdn¹ Y chcemy przesun¹æ. 
	\param[in] fTransY Skalar przesuniêcia po osi Y.
	\return Nowy, przesuniêty wektor po osi Y. */
	CVector3 TranslateY3(const CVector3 &in_cVecActual, GLfloat fTransY);

	/// Metoda przesuwaj¹ca wierzcho³ek 3D po osi Z.
	/** \param[in] in_cVecActual Referencja na wektor, którego wspó³rzêdn¹ Z chcemy przesun¹æ. 
	\param[in] fTransZ Skalar przesuniêcia po osi Z.
	\return Nowy, przesuniêty wektor po osi Z. */
	CVector3 TranslateZ3(const CVector3 &in_cVecActual, GLfloat fTransZ);

	/// Metoda skaluj¹ca wierzcho³ek 3D.
	/** \param[in] in_cVecActual Referencja na wektor, który chcemy przeskalowaæ.
	\param[in] in_cVecScale Referencja na wektor skalowania.
	\return Nowy, przeskalowany wektor. */
	CVector3 ScaleXYZ3(const CVector3 &in_cVecActual, const CVector3 &in_cVecScale);

	/// Metoda obracaj¹ca wierzcho³ek 3D po osi X.
	/** \param[in] in_cVecActual Referencja na wektor, który chcemy obróciæ po osi X.
	\param[in] fAngle K¹t o jaki chcemy obróciæ wektor po osi X.
	\return Nowy, obrócony wektor po osi X.	*/
	CVector3 RotateX3(const CVector3 &in_cVecActual, GLfloat fAngle);

	/// Metoda obracaj¹ca wierzcho³ek 3D po osi Y.
	/** \param[in] in_cVecActual Referencja na wektor, który chcemy obróciæ po osi Y.
	\param[in] fAngle K¹t o jaki chcemy obróciæ wektor po osi Y.
	\return Nowy, obrócony wektor po osi Y.	*/
	CVector3 RotateY3(const CVector3 &in_cVecActual, GLfloat fAngle);

	/// Metoda obracaj¹ca wierzcho³ek 3D po osi Z.
	/** \param[in] in_cVecActual Referencja na wektor, który chcemy obróciæ po osi Z.
	\param[in] fAngle K¹t o jaki chcemy obróciæ wektor po osi Z.
	\return Nowy, obrócony wektor po osi Z.	*/
	CVector3 RotateZ3(const CVector3 &in_cVecActual, GLfloat fAngle);

	/// Metoda obracaj¹ca wierzcho³ek 3D.
	/** \param[in] in_cVecActual Referencja na wektor, który chcemy obróciæ.
	\param[in] in_cVecAngle Referencja na wektor, któych przechowuje k¹ty obrotu odpowiednich osi.
	\return Nowy, obrócony wektor. */
	CVector3 RotateXYZ3(const CVector3 &in_cVecActual, const CVector3 &in_cVecAngle);

	/// Metoda obracaj¹ca wierzcho³ek 3D.
	/** \param[in] in_out_cVecActual Referencja na wektor, który chcemy obróciæ i w którym zostanie zwrócony rezultat
	obrotu.
	\param[in] in_cVecAngle Referencja na wektor, któych przechowuje k¹ty obrotu odpowiednich osi. */
	GLvoid RotateXYZ3_ref(CVector3 &in_out_cVecActual, const CVector3 &in_cVecAngle);

	/////////////////////////// 2D ///////////////////////////////////

	/// Metoda przesuwaj¹ca wierzcho³ek 2D.
	/** \param[in] in_cVecActual Referencja na wektor, który chcemy przesun¹æ. 
	\param[in] in_cVecTrans Referencja na wektor przesuniêcia.
	\return Nowy, przesuniêty wektor. */
	CVector2 TranslateXY2(const CVector2 &in_cVecActual, const CVector2 &in_cVecTrans);

	/// Metoda skaluj¹ca wierzcho³ek 2D.
	/** \param[in] in_cVecActual Referencja na wektor, który chcemy przeskalowaæ.
	\param[in] in_cVecScale Referencja na wektor skalowania.
	\return Nowy, przeskalowany wektor. */
	CVector2 ScaleXY2(const CVector2 &in_cVecActual, const CVector2 &in_cVecScale);

	/// Metoda obracaj¹ca wierzcho³ek 2D po osi X.
	/** \param[in] in_cVecActual Referencja na wektor, który chcemy obróciæ po osi X.
	\param[in] fAngle K¹t o jaki chcemy obróciæ wektor po osi X.
	\return Nowy, obrócony wektor po osi X.	*/
	CVector2 RotateX2(const CVector2 &in_cVecActual, GLfloat fAngle);

	/// Metoda obracaj¹ca wierzcho³ek 2D po osi Y.
	/** \param[in] in_cVecActual Referencja na wektor, który chcemy obróciæ po osi Y.
	\param[in] fAngle K¹t o jaki chcemy obróciæ wektor po osi Y.
	\return Nowy, obrócony wektor po osi Y.	*/
	CVector2 RotateY2(const CVector2 &in_cVecActual, GLfloat fAngle);

	/// Metoda obracaj¹ca wierzcho³ek 2D po osi Z.
	/** \param[in] in_cVecActual Referencja na wektor, który chcemy obróciæ po osi Z.
	\param[in] fAngle K¹t o jaki chcemy obróciæ wektor po osi Z.
	\return Nowy, obrócony wektor po osi Z.	*/
	CVector2 RotateZ2(const CVector2 &in_cVecActual, GLfloat fAngle);
};
