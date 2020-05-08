
#include "StdAfx.h"
#include "Vector.h"

////////////////////////////////////////////////////////////////////////////////////
//////////////////////////// CVector2 //////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

CVector2::CVector2(): x( 0.0f ), y( 0.0f )
{
}

CVector2::CVector2( GLfloat fX, GLfloat fY ): x( fX ), y( fY )
{
}

CVector2::CVector2( const CVector2 &cVec )
{
	x = cVec.x;
	y = cVec.y;
}
	
CVector2::~CVector2()
{
}

GLvoid CVector2::Normalize()
{
	GLfloat fLength = GetLength();

	if( fLength == 0.0f ) fLength = 1.0f;
	GLfloat fTemp = 1 / fLength;
	x *= fTemp;
	y *= fTemp;
}

CVector2 CVector2::operator+( const CVector2 &cVec ) const
{
	return CVector2( x + cVec.x, y + cVec.y );
}

CVector2 CVector2::operator-( const CVector2 &cVec ) const
{
	return CVector2( x - cVec.x, y - cVec.y );
}

CVector2 CVector2::operator*( const CVector2 &cVec ) const
{
	return CVector2( x * cVec.x, y * cVec.y );
}

CVector2 CVector2::operator/( const CVector2 &cVec ) const
{
	return CVector2( x / cVec.x, y / cVec.y );
}

CVector2 CVector2::operator+( GLfloat fValue ) const
{
	return CVector2( x + fValue, y + fValue );
}

CVector2 CVector2::operator-( GLfloat fValue ) const
{
	return CVector2( x - fValue, y - fValue );
}

CVector2 CVector2::operator*( GLfloat fValue ) const
{
	return CVector2( x * fValue, y * fValue );
}

CVector2 CVector2::operator/( GLfloat fValue ) const
{
	return CVector2( x / fValue, y / fValue );
}

CVector2& CVector2::operator+=( const CVector2 &cVec )
{
	x += cVec.x;
	y += cVec.y;
	return *this;
}

CVector2& CVector2::operator-=( const CVector2 &cVec )
{
	x -= cVec.x;
	y -= cVec.y;
	return *this;
}

CVector2& CVector2::operator*=( const CVector2 &cVec )
{
	x *= cVec.x;
	y *= cVec.y;
	return *this;
}

CVector2& CVector2::operator+=( GLfloat fValue )
{
	x += fValue;
	y += fValue;
	return *this;
}

CVector2& CVector2::operator-=( GLfloat fValue )
{
	x -= fValue;
	y -= fValue;
	return *this;
}

CVector2& CVector2::operator*=( GLfloat fValue )
{
	x *= fValue;
	y *= fValue;
	return *this;
}

CVector2 CVector2::operator-() const
{
	return CVector2(-x, -y );
}

CVector2 CVector2::operator+() const
{
	return *this;
}

CVector2& CVector2::operator=( const CVector2 &cVec )
{
	x = cVec.x;
	y = cVec.y;
	return *this;
}

GLboolean CVector2::operator==( const CVector2 &cVec ) const
{
	return ( x == cVec.x && y == cVec.y );
}

////////////////////////////////////////////////////////////////////////////////////
//////////////////////////// CVector3 //////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
GLvoid CVector3::Normalize()
{
	GLfloat fLength = GetLength();

	if( fLength == 0.0f ) fLength = 1.0f;
	GLfloat fTemp = 1.0f / fLength;
	x *= fTemp;
	y *= fTemp;
	z *= fTemp;
}

////////////////////////////////////////////////////////////////////////////////////
//////////////////////////// CVector3d //////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
GLvoid CVector3d::Normalize()
{
	GLdouble dLength = GetLength();
	if( dLength == 0.0 ) dLength = 1.0;
	GLdouble dTemp = 1.0 / dLength;
	x *= dTemp;
	y *= dTemp;
	z *= dTemp;
}

////////////////////////////////////////////////////////////////////////////////////
//////////////////////////// CVector4 //////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

CVector4::CVector4(): x( 0.0f ), y( 0.0f ), z( 0.0f ), w( 0.0f )
{
}

CVector4::CVector4( GLfloat fX, GLfloat fY, GLfloat fZ, GLfloat fW ): x( fX ), y( fY ), z( fZ ), w( fW )
{
}

CVector4::CVector4( const CVector4 &cVec )
{
	x = cVec.x;
	y = cVec.y;
	z = cVec.z;
	w = cVec.w;
}

CVector4::~CVector4()
{
}

CVector4 CVector4::operator+( const CVector4 &cVec ) const
{
	return CVector4( x + cVec.x,
					 y + cVec.y,
					 z + cVec.z,
					 w + cVec.w );
}

CVector4 CVector4::operator-( const CVector4 &cVec ) const
{
	return CVector4( x - cVec.x,
					 y - cVec.y,
					 z - cVec.z,
					 w - cVec.w );
}

CVector4& CVector4::operator=( const CVector4 &cVec )
{
	x = cVec.x;
	y = cVec.y;
	z = cVec.z;
	w = cVec.w;
	return *this;
}

GLboolean CVector4::operator==( const CVector4 &cVec ) const
{
	return ( x == cVec.x && y == cVec.y && z == cVec.z && w == cVec.w );
}