#pragma once

class CFakeHDR
{
public:
	CFakeHDR();
	~CFakeHDR();
	GLvoid init_postprocess();
	GLvoid do_postprocess();

private:
	GLvoid fx_get(GLint id);
	GLvoid fx_quad(GLfloat tx, GLfloat ty);
	GLvoid fx_put(GLint id, GLboolean square);
	GLvoid fx_fix_256();
	GLvoid do_blur(GLfloat sx, GLfloat sy);
};