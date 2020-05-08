// Lighting.h: interface for the CLighting class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

class CVector4;

/*
Œwiat³o otoczenia to œwiat³o, które nie pochodzi z
¿adnego okreœlonego kierunku i oœwietla wszystkie obiekty w twojej prezentacji.
Drugi nazywa siê œwiat³em rozproszonym. Takie œwiat³o ma swoje Ÿród³o i odbija siê
od powierzchni obiektów. Ka¿da powierzchnia, na któr¹ œwiat³o pada bezpoœrednio
bêdzie bardzo jasna, zaœ obszary, do których œwiat³o ledwo dochodzi bêd¹ ciemniejsze.
Dziêki temu uzyskamy ³adny efekt cieniowania na naszej skrzynce.
Gdyby nie oœwietlenie otoczenia, to miejsca, do których nie dochodzi œwiat³o
rozproszone wydawa³yby siê bardzo ciemne.
*/

class CLighting  
{
public:
	enum ELightMode
	{
		EEmission = GL_EMISSION,
		EAmbient = GL_AMBIENT,
		EDiffuse = GL_DIFFUSE,
		ESpecular = GL_SPECULAR,
		EAmbientAndDiffuse = GL_AMBIENT_AND_DIFFUSE
	};

	CLighting();
	virtual ~CLighting();

	//ustaw swiatlo otoczenia
	GLvoid SetAmbient( GLuint uiLight, GLfloat r = 0.5f, GLfloat g = 0.5f, GLfloat b = 0.5f, GLfloat a = 1.0f );
	//ustaw swiatlo rozproszone
	GLvoid SetDiffuse( GLuint uiLight, GLfloat r = 1.0f, GLfloat g = 1.0f, GLfloat b = 1.0f, GLfloat a = 1.0f );
	// ustaw swiatlo odblyskow
	GLvoid SetSpecular( GLuint uiLight, GLfloat r = 0.0f, GLfloat g = 0.0f, GLfloat b = 0.0f, GLfloat a = 1.0f );

	GLvoid SetColorMaterial( ELightMode eMode, GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.0f );
	GLvoid SetColorMaterial( ELightMode eMode, const CVector4 &in_cColor );
	GLvoid SetColorMaterial( ELightMode eMode );
	GLvoid DisableColorMaterial();

	GLvoid SetPosition( GLuint uiLight, GLfloat x = 0.0f, GLfloat y = 0.0f, GLfloat z = 1.0f, GLfloat w = 1.0f );
	GLvoid SetPosition( GLuint uiLight, const CVector4 &in_cPos );
	CVector4 GetPosition( GLuint uiLight );
	
	//matrial
	GLvoid SetMatAmbient( GLfloat r = 0.2f, GLfloat g = 0.2f, GLfloat b = 0.2f, GLfloat a = 1.0f );
	GLvoid SetMatDiffuse( GLfloat r = 0.8f, GLfloat g = 0.8f, GLfloat b = 0.8f, GLfloat a = 1.0f );
	GLvoid SetMatSpecular( GLfloat r = 0.0f, GLfloat g = 0.0f, GLfloat b = 0.0f, GLfloat a = 1.0f );
	GLvoid SetMatEmission( GLfloat r = 0.0f, GLfloat g = 0.0f, GLfloat b = 0.0f, GLfloat a = 1.0f );
	GLvoid SetMatShinness( GLfloat s = 50.0f );

	GLvoid DrawPositionLight( GLuint uiLight, GLfloat r = 1.0f, GLfloat g = 1.0f, GLfloat b = 1.0f );

	GLint GetIndex( GLuint uiLight );

private:
	struct SLighting
	{
		GLfloat fLightAmbient[ 4 ];		// Wartoœci oœwietlenia otoczenia
		GLfloat fLightDiffuse[ 4 ];		// Wartoœci œwiat³a rozproszonego (zarowka)
		GLfloat fLightSpecular[ 4 ];	// Wartoœæ œwiat³¹ odblasków
		CVector4 cLightPosition;		// Pozycja œwiat³a
	};

	SLighting m_sLight[ 8 ];

	GLfloat m_fMatAmbient[ 4 ];		// definiuje kolor i natê¿enie odbicia œwiat³a otaczaj¹cego
	GLfloat m_fMatDiffuse[ 4 ];		// definiuje kolor i natê¿enie odbicia œwiat³a rozproszonego 
	GLfloat m_fMatSpecular[ 4 ];	// rozpraszanie swiat³a
	GLfloat m_fMatEmission[ 4 ];	// definiuje stopieñ i kolor emisji œwiat³a. Parametr ten s³u¿y jedynie nadaniu obiektowi takiego koloru, jakby sam by³ Ÿród³em œwiat³a.
	GLfloat m_fMatShininess;		// po³ysk, range [0,128] 

	GLfloat m_fArmLength;
	GLfloat m_fArmLength2;

	CVector3* m_aIndices;
};