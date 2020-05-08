// SoundFMOD.h: interface for the CSoundFMOD class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#if !_USE_SOUND_OPENAL_

#include <fmod.h>

class CSoundFMOD  
{
public:
	CSoundFMOD();
	virtual ~CSoundFMOD();

	GLboolean LoadSample( const char* lpSample, GLboolean bLoopSound = GL_FALSE );
	GLboolean LoadStreamSample( const char* lpSample, GLboolean bLoopSound = GL_FALSE );

	GLvoid PlaySample( GLint iIndex );
	GLvoid StopSample( GLint iIndex );

	GLvoid PlayStreamSample( GLint iIndex );
	GLvoid StopStreamSample( GLint iIndex );

	GLvoid VolumeIncrease( GLint iValue = 10 );
	GLvoid VolumeDecrease( GLint iValue = 10 );

private:
	struct SSample
	{
		FSOUND_SAMPLE *sample;
		FSOUND_STREAM *stream;
		GLint iChannel; //jezeli == -1 to byl error przy play
	};

	std::vector<SSample> m_aSoundSamples;
	inline GLint GetSoundSamplesSize() { return static_cast<GLint>( m_aSoundSamples.size() ); }
};

#endif // !_USE_SOUND_OPENAL_