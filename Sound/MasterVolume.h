#pragma once

#include "IVolume.h"
#include "../OpenGLFramework/SingletonBase.h"

class CMasterVolume : public CSingletonBase <CMasterVolume>
{
public:
	static CMasterVolume* Construct();
	~CMasterVolume();

	GLvoid SetPercentVolume( GLuint uiPercent = 50 );
	GLuint GetPercentVolume();
	GLvoid IncreasePercentVolume( GLuint uiPercent = 10 ); //co 10%
	GLvoid DecreasePercentVolume( GLuint uiPercent = 10 ); //co 10%

	GLvoid SetVolume( GLuint uiVolume ); //max 65535
	GLuint GetVolume();
	GLvoid IncreaseVolume( GLuint uiVolume = 4096 ); //co 1/16
	GLvoid DecreaseVolume( GLuint uiVolume = 4096 ); //co 1/16

private:
	CMasterVolume();

	IVolume			*m_pMasterVolume;

	GLuint m_uiOryginalVolume;
};
