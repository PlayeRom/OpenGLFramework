#pragma once

class CGameControl;

#define BUFFER_OFFSET( i ) ( ( GLchar * )NULL + ( i ) )

class CARBVertexBufferObject
{
public:
	CARBVertexBufferObject();
	~CARBVertexBufferObject();

	GLvoid SetUse( GLboolean bUse );
	GLboolean IsUse();
	GLboolean IsSupported();

	GLvoid CreateVerticesBuffer( const GLvoid *lpVoid, GLsizei iSize );
	GLboolean BindVerticesBuffer();
	GLvoid DeleteVerticesBuffers();

	GLvoid CreateNormalsBuffer( const GLvoid *lpVoid, GLsizei iSize );
	GLboolean BindNormalsBuffer();
	GLvoid DeleteNormalsBuffers();

	GLvoid CreateColorsBuffer( const GLvoid *lpVoid, GLsizei iSize );
	GLboolean BindColorsBuffer();
	GLvoid DeleteColorsBuffers();

	GLvoid CreateTexCoordsBuffer( const GLvoid *lpVoid, GLsizei iSize );
	GLboolean BindTexCoordsBuffer();
	GLvoid DeleteTexCoordsBuffers();

	GLvoid CreateTexCoordsEmbossBumpBuffer( const GLvoid *lpVoid, GLsizei iSize );
	GLboolean BindTexCoordsEmbossBumpBuffer();
	GLvoid DeleteTexCoordsEmbossBumpBuffers();

private:
	CGameControl *m_pGameCtrl;
	GLboolean m_bUse;
	GLboolean m_bIsSupported;

	GLboolean InitExtension();

	GLuint m_uiVerticesBuffer;
	GLuint m_uiNormalsBuffer;
	GLuint m_uiColorsBuffer;
	GLuint m_uiTexCoordsBuffer;
	GLuint m_uiTexCoordsEmbossBumpBuffer;
};
