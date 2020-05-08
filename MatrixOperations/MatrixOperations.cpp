
#include "StdAfx.h"
#include "MatrixOperations.h"

CMatrixOperations::CMatrixOperations()
{
}

CMatrixOperations::~CMatrixOperations()
{
}

CVector3 CMatrixOperations::TranslateXYZ3( const CVector3 &in_cVecActual, const CVector3 &in_cVecTrans )
{
	return in_cVecActual + in_cVecTrans;
}

CVector3 CMatrixOperations::TranslateX3( const CVector3 &in_cVecActual, GLfloat fTransX )
{
	CVector3 cResult;
	cResult.x = in_cVecActual.x + fTransX;
	cResult.y = in_cVecActual.y;
	cResult.z = in_cVecActual.z;

	return cResult;
}

CVector3 CMatrixOperations::TranslateY3( const CVector3 &in_cVecActual, GLfloat fTransY )
{
	CVector3 cResult;
	cResult.x = in_cVecActual.x;
	cResult.y = in_cVecActual.y + fTransY;
	cResult.z = in_cVecActual.z;

	return cResult;
}

CVector3 CMatrixOperations::TranslateZ3( const CVector3 &in_cVecActual, GLfloat fTransZ )
{
	CVector3 cResult;
	cResult.x = in_cVecActual.x;
	cResult.y = in_cVecActual.y;
	cResult.z = in_cVecActual.z + fTransZ;

	return cResult;
}

CVector3 CMatrixOperations::ScaleXYZ3( const CVector3 &in_cVecActual, const CVector3 &in_cVecScale )
{
	return in_cVecActual * in_cVecScale;
}

CVector3 CMatrixOperations::RotateX3( const CVector3 &in_cVecActual, GLfloat fAngle )
{
	CVector3 cResult;
	fAngle = static_cast<GLfloat>( DEG2RAD( fAngle ) );
	cResult.x = in_cVecActual.x;
	cResult.y = ( in_cVecActual.y * cos( fAngle ) ) - ( in_cVecActual.z * sin( fAngle ) );
	cResult.z = ( in_cVecActual.y * sin( fAngle ) ) + ( in_cVecActual.z * cos( fAngle ) );

	return cResult;
}

CVector3 CMatrixOperations::RotateY3( const CVector3 &in_cVecActual, GLfloat fAngle )
{
	CVector3 cResult;
	fAngle = static_cast<GLfloat>( DEG2RAD( fAngle ) );
	cResult.x = ( in_cVecActual.x * cos( fAngle ) ) + ( in_cVecActual.z * sin( fAngle ) );
	cResult.y = in_cVecActual.y;
	cResult.z = -( in_cVecActual.x * sin( fAngle ) ) + ( in_cVecActual.z * cos( fAngle ) );

	return cResult;
}

CVector3 CMatrixOperations::RotateZ3( const CVector3 &in_cVecActual, GLfloat fAngle )
{
	CVector3 cResult;
	fAngle = static_cast<GLfloat>( DEG2RAD( fAngle ) );
	cResult.x = ( in_cVecActual.x * cos( fAngle ) ) - ( in_cVecActual.y * sin( fAngle ) );
	cResult.y = ( in_cVecActual.x * sin( fAngle ) ) + ( in_cVecActual.y * cos( fAngle ) );
	cResult.z = in_cVecActual.z;

	return cResult;
}

CVector3 CMatrixOperations::RotateXYZ3( const CVector3 &in_cVecActual, const CVector3 &in_cVecAngle )
{
	if( in_cVecAngle == CVector3() )
		return in_cVecActual;

	CVector3 cResult;
	cResult = RotateX3( in_cVecActual, in_cVecAngle.x );
	cResult = RotateY3( cResult, in_cVecAngle.y );
	cResult = RotateZ3( cResult, in_cVecAngle.z );

	return cResult;
}

GLvoid CMatrixOperations::RotateXYZ3_ref( CVector3 &in_out_cVecActual, const CVector3 &in_cVecAngle )
{
	if( in_cVecAngle == CVector3() )
		return;

	in_out_cVecActual = RotateX3( in_out_cVecActual, in_cVecAngle.x );
	in_out_cVecActual = RotateY3( in_out_cVecActual, in_cVecAngle.y );
	in_out_cVecActual = RotateZ3( in_out_cVecActual, in_cVecAngle.z );

	return;
}

CVector2 CMatrixOperations::TranslateXY2( const CVector2 &in_cVecActual, const CVector2 &in_cVecTrans )
{
	return in_cVecActual + in_cVecTrans;
}

CVector2 CMatrixOperations::ScaleXY2( const CVector2 &in_cVecActual, const CVector2 &in_cVecScale )
{
	return in_cVecActual * in_cVecScale;
}

CVector2 CMatrixOperations::RotateX2( const CVector2 &in_cVecActual, GLfloat fAngle )
{
	CVector2 cResult;
	fAngle = static_cast<GLfloat>( DEG2RAD( fAngle ) );
	cResult.x = in_cVecActual.x;
	cResult.y = ( in_cVecActual.y * cos( fAngle ) );

	return cResult;
}

CVector2 CMatrixOperations::RotateY2( const CVector2 &in_cVecActual, GLfloat fAngle )
{
	CVector2 cResult;
	fAngle = static_cast<GLfloat>( DEG2RAD( fAngle ) );
	cResult.x = ( in_cVecActual.x * cos( fAngle ) );
	cResult.y = in_cVecActual.y;

	return cResult;
}

CVector2 CMatrixOperations::RotateZ2( const CVector2 &in_cVecActual, GLfloat fAngle )
{
	CVector2 cResult;
	fAngle = static_cast<GLfloat>( DEG2RAD( fAngle ) );
	cResult.x = ( in_cVecActual.x * cos( fAngle ) ) - ( in_cVecActual.y * sin( fAngle ) );
	cResult.y = ( in_cVecActual.x * sin( fAngle ) ) + ( in_cVecActual.y * cos( fAngle ) );

	return cResult;
}