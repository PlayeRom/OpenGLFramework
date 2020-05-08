// Lighting.cpp: implementation of the CLighting class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "Lighting.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLighting::CLighting()
{
	m_fArmLength = 0.2f;
	m_fArmLength2 = 0.15f;

	m_aIndices = new CVector3[ 28 ];
}

CLighting::~CLighting()
{
	delete [] m_aIndices;
}

//ustaw oswietlenie otoczenia
GLvoid CLighting::SetAmbient( GLuint uiLight, GLfloat r, GLfloat g, GLfloat b, GLfloat a )
{
	if( uiLight < GL_LIGHT0 || uiLight > GL_LIGHT7 )
		return;

	GLint iIndex = GetIndex( uiLight );

	if( iIndex < 0 || iIndex > 7 )
		return;

	m_sLight[ iIndex ].fLightAmbient[ 0 ] = r;
	m_sLight[ iIndex ].fLightAmbient[ 1 ] = g;
	m_sLight[ iIndex ].fLightAmbient[ 2 ] = b;
	m_sLight[ iIndex ].fLightAmbient[ 3 ] = a;

	glLightfv( uiLight, GL_AMBIENT, m_sLight[ iIndex ].fLightAmbient );
}

// ustaw swiatlo rozproszone
GLvoid CLighting::SetDiffuse( GLuint uiLight, GLfloat r, GLfloat g, GLfloat b, GLfloat a )
{
	if( uiLight < GL_LIGHT0 || uiLight > GL_LIGHT7 )
		return;

	GLint iIndex = GetIndex( uiLight );

	if( iIndex < 0 || iIndex > 7 )
		return;

	m_sLight[ iIndex ].fLightDiffuse[ 0 ] = r;
	m_sLight[ iIndex ].fLightDiffuse[ 1 ] = g;
	m_sLight[ iIndex ].fLightDiffuse[ 2 ] = b;
	m_sLight[ iIndex ].fLightDiffuse[ 3 ] = a;

	glLightfv( uiLight, GL_DIFFUSE, m_sLight[ iIndex ].fLightDiffuse );
}

// ustaw swiatlo odblyskow
GLvoid CLighting::SetSpecular( GLuint uiLight, GLfloat r, GLfloat g, GLfloat b, GLfloat a )
{
	if( uiLight < GL_LIGHT0 || uiLight > GL_LIGHT7 )
		return;

	GLint iIndex = GetIndex( uiLight );

	if( iIndex < 0 || iIndex > 7 )
		return;

	m_sLight[ iIndex ].fLightSpecular[ 0 ] = r;
	m_sLight[ iIndex ].fLightSpecular[ 1 ] = g;
	m_sLight[ iIndex ].fLightSpecular[ 2 ] = b;
	m_sLight[ iIndex ].fLightSpecular[ 3 ] = a;

	glLightfv( uiLight, GL_SPECULAR, m_sLight[ iIndex ].fLightSpecular );
}

GLvoid CLighting::SetColorMaterial( ELightMode eMode, GLfloat r, GLfloat g, GLfloat b, GLfloat a )
{
	glEnable( GL_COLOR_MATERIAL );
	glColorMaterial( GL_FRONT, eMode );
	glColor4f( r, g, b, a );
}

GLvoid CLighting::SetColorMaterial( ELightMode eMode, const CVector4 &in_cColor )
{
	glEnable( GL_COLOR_MATERIAL );
	glColorMaterial( GL_FRONT, eMode );
	glColor4f( in_cColor.x, in_cColor.y, in_cColor.z, in_cColor.w );
}

GLvoid CLighting::SetColorMaterial( ELightMode eMode )
{
	glEnable( GL_COLOR_MATERIAL );
	glColorMaterial( GL_FRONT, eMode );
}

GLvoid CLighting::DisableColorMaterial()
{
	glDisable( GL_COLOR_MATERIAL );
}

GLvoid CLighting::SetPosition( GLuint uiLight, GLfloat x, GLfloat y, GLfloat z, GLfloat w )
{
	if( uiLight < GL_LIGHT0 || uiLight > GL_LIGHT7 )
		return;

	GLint iIndex = GetIndex( uiLight );

	if( iIndex < 0 || iIndex > 7 )
		return;

	m_sLight[ iIndex ].cLightPosition.x = x;
	m_sLight[ iIndex ].cLightPosition.y = y;
	m_sLight[ iIndex ].cLightPosition.z = z;
	m_sLight[ iIndex ].cLightPosition.w = w;

	glLightfv( uiLight, GL_POSITION, &m_sLight[ iIndex ].cLightPosition.x );
}

GLvoid CLighting::SetPosition( GLuint uiLight, const CVector4 &in_cPos )
{
	if( uiLight < GL_LIGHT0 || uiLight > GL_LIGHT7 )
		return;

	GLint iIndex = GetIndex( uiLight );

	if( iIndex < 0 || iIndex > 7 )
		return;

	m_sLight[ iIndex ].cLightPosition = in_cPos;

	glLightfv( uiLight, GL_POSITION, &m_sLight[ iIndex ].cLightPosition.x );
}

CVector4 CLighting::GetPosition( GLuint uiLight )
{
	CVector4 cResult;
	if( uiLight < GL_LIGHT0 || uiLight > GL_LIGHT7 )
		return cResult;

	GLint iIndex = GetIndex( uiLight );

	if( iIndex < 0 || iIndex > 7 )
		return cResult;

	//glGetLightfv( uiLight, GL_POSITION, &m_sLight[ iIndex ].cLightPosition.x );
	
	return m_sLight[ iIndex ].cLightPosition;
}

GLvoid CLighting::SetMatAmbient( GLfloat r, GLfloat g, GLfloat b, GLfloat a )
{
	m_fMatAmbient[ 0 ] = r;
	m_fMatAmbient[ 1 ] = g;
	m_fMatAmbient[ 2 ] = b;
	m_fMatAmbient[ 3 ] = a;

	glMaterialfv( GL_FRONT, GL_AMBIENT, m_fMatAmbient );
}

GLvoid CLighting::SetMatDiffuse( GLfloat r, GLfloat g, GLfloat b, GLfloat a )
{
	m_fMatDiffuse[ 0 ] = r;
	m_fMatDiffuse[ 1 ] = g;
	m_fMatDiffuse[ 2 ] = b;
	m_fMatDiffuse[ 3 ] = a;

	glMaterialfv( GL_FRONT, GL_DIFFUSE, m_fMatDiffuse );
}

GLvoid CLighting::SetMatSpecular( GLfloat r, GLfloat g, GLfloat b, GLfloat a )
{
	m_fMatSpecular[ 0 ] = r;
	m_fMatSpecular[ 1 ] = g;
	m_fMatSpecular[ 2 ] = b;
	m_fMatSpecular[ 3 ] = a;

	glMaterialfv( GL_FRONT, GL_SPECULAR, m_fMatSpecular );
}

GLvoid CLighting::SetMatEmission( GLfloat r, GLfloat g, GLfloat b, GLfloat a )
{
	m_fMatEmission[ 0 ] = r;
	m_fMatEmission[ 1 ] = g;
	m_fMatEmission[ 2 ] = b;
	m_fMatEmission[ 3 ] = a;

	glMaterialfv( GL_FRONT, GL_EMISSION, m_fMatEmission );
}

//stopieñ po³ysku materia³u
GLvoid CLighting::SetMatShinness( GLfloat s )
{
	m_fMatShininess = s;

	glMaterialfv( GL_FRONT, GL_SHININESS, &m_fMatShininess );
}

//rysuje gwiazdke w punkcji zawieszenia danego swiatla, w rgb mozemy podac kolor rysowanej
//gwiazdki - domyslnie bialy
GLvoid CLighting::DrawPositionLight( GLuint uiLight, GLfloat r, GLfloat g, GLfloat b )
{
	if( uiLight < GL_LIGHT0 || uiLight > GL_LIGHT7 )
		return;

	GLint i = GetIndex( uiLight );

	if( i < 0 || i > 7 )
		return;

	glDisable( GL_TEXTURE_2D );

	GLboolean bLightEnabled = GL_FALSE;
	if( glIsEnabled( GL_LIGHTING ) ) {
		glDisable( GL_LIGHTING );
		bLightEnabled = GL_TRUE;
	}

	glColor3f( r, g, b );

	//do gory
	m_aIndices[ 0 ] = CVector3( m_sLight[ i ].cLightPosition.x, m_sLight[ i ].cLightPosition.y, m_sLight[ i ].cLightPosition.z );
	m_aIndices[ 1 ] = CVector3( m_sLight[ i ].cLightPosition.x, m_sLight[ i ].cLightPosition.y + m_fArmLength, m_sLight[ i ].cLightPosition.z );
	//na dol
	m_aIndices[ 2 ] = CVector3( m_sLight[ i ].cLightPosition.x, m_sLight[ i ].cLightPosition.y, m_sLight[ i ].cLightPosition.z );
	m_aIndices[ 3 ] = CVector3( m_sLight[ i ].cLightPosition.x, m_sLight[ i ].cLightPosition.y - m_fArmLength, m_sLight[ i ].cLightPosition.z );
	//w lewo
	m_aIndices[ 4 ] = CVector3( m_sLight[ i ].cLightPosition.x, m_sLight[ i ].cLightPosition.y, m_sLight[ i ].cLightPosition.z );
	m_aIndices[ 5 ] = CVector3( m_sLight[ i ].cLightPosition.x - m_fArmLength, m_sLight[ i ].cLightPosition.y, m_sLight[ i ].cLightPosition.z );
	//w prawo
	m_aIndices[ 6 ] = CVector3( m_sLight[ i ].cLightPosition.x, m_sLight[ i ].cLightPosition.y, m_sLight[ i ].cLightPosition.z );
	m_aIndices[ 7 ] = CVector3( m_sLight[ i ].cLightPosition.x + m_fArmLength, m_sLight[ i ].cLightPosition.y, m_sLight[ i ].cLightPosition.z );
	//do przodu
	m_aIndices[ 8 ] = CVector3( m_sLight[ i ].cLightPosition.x, m_sLight[ i ].cLightPosition.y, m_sLight[ i ].cLightPosition.z );
	m_aIndices[ 9 ] = CVector3( m_sLight[ i ].cLightPosition.x, m_sLight[ i ].cLightPosition.y, m_sLight[ i ].cLightPosition.z + m_fArmLength );
	//do tylu
	m_aIndices[ 10 ] = CVector3( m_sLight[ i ].cLightPosition.x, m_sLight[ i ].cLightPosition.y, m_sLight[ i ].cLightPosition.z );
	m_aIndices[ 11 ] = CVector3( m_sLight[ i ].cLightPosition.x, m_sLight[ i ].cLightPosition.y, m_sLight[ i ].cLightPosition.z - m_fArmLength );
	//po skosach
	//w lewo
	m_aIndices[ 12 ] = CVector3( m_sLight[ i ].cLightPosition.x, m_sLight[ i ].cLightPosition.y, m_sLight[ i ].cLightPosition.z );
	m_aIndices[ 13 ] = CVector3( m_sLight[ i ].cLightPosition.x - m_fArmLength2, m_sLight[ i ].cLightPosition.y + m_fArmLength2, m_sLight[ i ].cLightPosition.z - m_fArmLength2 );
	//w prawo
	m_aIndices[ 14 ] = CVector3( m_sLight[ i ].cLightPosition.x, m_sLight[ i ].cLightPosition.y, m_sLight[ i ].cLightPosition.z );
	m_aIndices[ 15 ] = CVector3( m_sLight[ i ].cLightPosition.x + m_fArmLength2, m_sLight[ i ].cLightPosition.y + m_fArmLength2, m_sLight[ i ].cLightPosition.z - m_fArmLength2 );
	//do przodu
	m_aIndices[ 16 ] = CVector3( m_sLight[ i ].cLightPosition.x, m_sLight[ i ].cLightPosition.y, m_sLight[ i ].cLightPosition.z );
	m_aIndices[ 17 ] = CVector3( m_sLight[ i ].cLightPosition.x - m_fArmLength2, m_sLight[ i ].cLightPosition.y - m_fArmLength2, m_sLight[ i ].cLightPosition.z - m_fArmLength2 );
	//do tylu
	m_aIndices[ 18 ] = CVector3( m_sLight[ i ].cLightPosition.x, m_sLight[ i ].cLightPosition.y, m_sLight[ i ].cLightPosition.z );
	m_aIndices[ 19 ] = CVector3( m_sLight[ i ].cLightPosition.x + m_fArmLength2, m_sLight[ i ].cLightPosition.y - m_fArmLength2, m_sLight[ i ].cLightPosition.z - m_fArmLength2 );
	//w lewo
	m_aIndices[ 20 ] = CVector3( m_sLight[ i ].cLightPosition.x, m_sLight[ i ].cLightPosition.y, m_sLight[ i ].cLightPosition.z );
	m_aIndices[ 21 ] = CVector3( m_sLight[ i ].cLightPosition.x - m_fArmLength2, m_sLight[ i ].cLightPosition.y + m_fArmLength2, m_sLight[ i ].cLightPosition.z + m_fArmLength2 );
	//w prawo
	m_aIndices[ 22 ] = CVector3( m_sLight[ i ].cLightPosition.x, m_sLight[ i ].cLightPosition.y, m_sLight[ i ].cLightPosition.z );
	m_aIndices[ 23 ] = CVector3( m_sLight[ i ].cLightPosition.x + m_fArmLength2, m_sLight[ i ].cLightPosition.y + m_fArmLength2, m_sLight[ i ].cLightPosition.z + m_fArmLength2 );
	//do przodu
	m_aIndices[ 24 ] = CVector3( m_sLight[ i ].cLightPosition.x, m_sLight[ i ].cLightPosition.y, m_sLight[ i ].cLightPosition.z );
	m_aIndices[ 25 ] = CVector3( m_sLight[ i ].cLightPosition.x - m_fArmLength2, m_sLight[ i ].cLightPosition.y - m_fArmLength2, m_sLight[ i ].cLightPosition.z + m_fArmLength2 );
	//do tylu
	m_aIndices[ 26 ] = CVector3( m_sLight[ i ].cLightPosition.x, m_sLight[ i ].cLightPosition.y, m_sLight[ i ].cLightPosition.z );
	m_aIndices[ 27 ] = CVector3( m_sLight[ i ].cLightPosition.x + m_fArmLength2, m_sLight[ i ].cLightPosition.y - m_fArmLength2, m_sLight[ i ].cLightPosition.z + m_fArmLength2 );

	glEnableClientState( GL_VERTEX_ARRAY );
	glDisableClientState( GL_NORMAL_ARRAY );
	glDisableClientState( GL_COLOR_ARRAY );
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );

	glVertexPointer( 3, GL_FLOAT, 0, m_aIndices );
	glDrawArrays( GL_LINES, 0, 28 );

	glColor3f( 1.0f, 1.0f, 1.0f );

	if( bLightEnabled )
		glEnable( GL_LIGHTING );
}

GLint CLighting::GetIndex( GLuint uiLight )
{
	if( uiLight < GL_LIGHT0 || uiLight > GL_LIGHT7 )
		return -1;

	return uiLight - GL_LIGHT0;
}