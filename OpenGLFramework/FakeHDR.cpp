
#include "stdafx.h"
#include "../Extensions/ARBMultiTexturing.h"
#include "FakeHDR.h"

#define NUM_BUFS	2

#define TEX_MAX		2048
#define TEX_MAXf	2048.0f

#define TEX_MIN		512

GLint fx_buff[NUM_BUFS];
GLint fxb_w[NUM_BUFS] = {TEX_MAX, TEX_MIN};
GLint fxb_h[NUM_BUFS] = {TEX_MAX, TEX_MIN};

CFakeHDR::CFakeHDR()
{
}

CFakeHDR::~CFakeHDR()
{
}

GLvoid CFakeHDR::init_postprocess()
{
	glGenTextures(8, (GLuint *)fx_buff);

	GLint s = TEX_MAX, t = TEX_MAX;
	GLuint *data = new GLuint[((s * t)* 4 * sizeof(GLubyte))];

	for(GLint i = 0; i < NUM_BUFS; ++i)
	{
		glBindTexture(GL_TEXTURE_2D, fx_buff[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, fxb_w[i], fxb_h[i], 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	
	delete [] data;
}

GLvoid CFakeHDR::fx_get(GLint id)
{
	glFlush();
	CARBMultiTexturing::GetInstance()->ActiveTextureARB(0);
	glBindTexture(GL_TEXTURE_2D, fx_buff[id]);
	glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, fxb_w[id], fxb_h[id]);
}

GLvoid CFakeHDR::fx_quad(GLfloat tx, GLfloat ty)
{
	glBegin(GL_QUADS);
		CARBMultiTexturing::GetInstance()->MultiTexCoord2fARB(0, 0.0f, 0.0f ); CARBMultiTexturing::GetInstance()->MultiTexCoord2fARB(1, 0.0f, 0.0f ); glVertex2f(-1.0f, -1.0f);
		CARBMultiTexturing::GetInstance()->MultiTexCoord2fARB(0, 0.0f, ty);    CARBMultiTexturing::GetInstance()->MultiTexCoord2fARB(1, 0.0f, ty);    glVertex2f(-1.0f,  1.0f);
		CARBMultiTexturing::GetInstance()->MultiTexCoord2fARB(0, tx,   ty);    CARBMultiTexturing::GetInstance()->MultiTexCoord2fARB(1, tx,	  ty);    glVertex2f( 1.0f,  1.0f);
		CARBMultiTexturing::GetInstance()->MultiTexCoord2fARB(0, tx,   0.0f);  CARBMultiTexturing::GetInstance()->MultiTexCoord2fARB(1, tx,   0.0f ); glVertex2f( 1.0f, -1.0f);
	glEnd();
}

GLvoid CFakeHDR::fx_put(GLint id, GLboolean square)
{
	CARBMultiTexturing::GetInstance()->ActiveTextureARB(0);
	glBindTexture(GL_TEXTURE_2D, fx_buff[id]);
	glEnable(GL_TEXTURE_2D);

 	if(square)
 	{
		CARBMultiTexturing::GetInstance()->ActiveTextureARB(1);
		glBindTexture(GL_TEXTURE_2D, fx_buff[id]);
		glEnable(GL_TEXTURE_2D);
	}
	
	GLfloat tx = 1.0f; // koordynaty tekstury
	GLfloat ty = 1.0f; // koordynaty tekstury
	if( id == 0 ){
		tx = (GLfloat(CWindowData::GetInstance()->m_sSettingFile.iWidth ) / TEX_MAXf );
		ty = (GLfloat(CWindowData::GetInstance()->m_sSettingFile.iHeight) / TEX_MAXf );
	}

	fx_quad( tx, ty );
	//fx_quad( (id == 0) ? (GLfloat(CWindowData::GetInstance()->m_sSettingFile.iWidth)/1024) : 1.0f, (id == 0) ? (GLfloat(CWindowData::GetInstance()->m_sSettingFile.iHeight)/1024) : 1.0f);
	
	CARBMultiTexturing::GetInstance()->ActiveTextureARB(0);
	glDisable(GL_TEXTURE_2D);
	
	if(square)
	{
		CARBMultiTexturing::GetInstance()->ActiveTextureARB(1);
		glDisable(GL_TEXTURE_2D);
	}
}

GLvoid CFakeHDR::fx_fix_256()
{
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	CARBMultiTexturing::GetInstance()->ActiveTextureARB(0);
	glBindTexture(GL_TEXTURE_2D, fx_buff[0]);
	glEnable(GL_TEXTURE_2D);

	glBegin(GL_QUADS);
		CARBMultiTexturing::GetInstance()->MultiTexCoord2fARB(0, 0.0f , 0.0f ); glVertex2f(-1.0f, -1.0f);
		CARBMultiTexturing::GetInstance()->MultiTexCoord2fARB(0, 0.0f , 0.25f); glVertex2f(-1.0f,  1.0f);
		CARBMultiTexturing::GetInstance()->MultiTexCoord2fARB(0, 0.25f, 0.25f); glVertex2f( 1.0f,  1.0f);
		CARBMultiTexturing::GetInstance()->MultiTexCoord2fARB(0, 0.25f, 0.0f ); glVertex2f( 1.0f, -1.0f);
	glEnd();

	CARBMultiTexturing::GetInstance()->ActiveTextureARB(0);
	glDisable(GL_TEXTURE_2D);
}

GLvoid CFakeHDR::do_blur(GLfloat sx, GLfloat sy)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);

//	const GLint n_layers = 4;
//	const GLfloat d[n_layers]={ -2, -1,  1,  2};
//	const GLfloat c[n_layers]={ .3, .5, .5, .3};
	
	const GLint n_layers = 8;
	const GLfloat d[n_layers] = { -4.0f, -3.0f, -2.0f, -1.0f,  1.0f,  2.0f,  3.0f,  4.0f};
	const GLfloat c[n_layers] = { .1f, .2f, .3f, .4f, .4f, .3f, .2f, .1f};

	sx *= 2.5f;
	sy *= 2.5f;
	for(GLint i = 1; i <= n_layers; ++i)
	{
		glLoadIdentity();
		glTranslatef(d[i] * sx, d[i] * sy, 0.0f);
		glColor4f(c[i], c[i], c[i], 0.7f);
		fx_put(1, GL_FALSE);
	}
	
	glDisable(GL_BLEND);
}


GLvoid CFakeHDR::do_postprocess()
{
	glLoadIdentity();
	glDisable(GL_DEPTH_TEST);
	glDepthMask( GL_FALSE );

	glColor4f(1.0f, 1.0f, 1.0f, 0.7f);

	fx_get(0); //kopiuje ramke do tekstury 1024x1024, przed blur-em
	glViewport(0, 0, TEX_MIN, TEX_MIN);
	glColor4f(0.5f, 0.5f, 0.5f, 0.7f);
	fx_put(0, GL_TRUE); //< rysuje quada z multitexturingiem, z ta sama tekstura, dopiero co skopiowana
	
	fx_get(1); //kopiuje ramke do tekstury 256x256
	do_blur(0.01f, 0.0f); // rysuje 8 quadow, przezroczystych, z odpowiednim natezeniem kolorow, z tekstura 256x256
	fx_get(1);
	do_blur(0.0f, 0.01f);
	fx_get(1);

	fx_fix_256(); // rysuje quad-a z 1/4 tekstury 1024x1024, czyli 256x256
	glViewport(0, 0,
		CWindowData::GetInstance()->m_sSettingFile.iWidth,
		CWindowData::GetInstance()->m_sSettingFile.iHeight);
	
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	glColor4f(0.5f, 0.5f, 0.5f, 0.7f);
	fx_put(1, GL_FALSE); // rysuje quad-a z tekstura 256x256
	glDisable(GL_BLEND);

	glDepthMask( GL_TRUE );

	glLoadIdentity();
}