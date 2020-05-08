#pragma once

#include <al.h>
#include <alc.h>
#include <alut.h>
#include "../OpenGLFramework/SingletonBase.h"

class CSoundOpenAL;
class COpenALSources;

// 500 a nie 100 gdy¿ zakres 0.0f - 1.0f jest zbyt du¿y. Dopiero przy zakresie 0.0f - 0.2f
// schy³aæ ró¿nice g³oœnoœci.
#define LISTENER_GAIN_VARIABLE	100.0f
#define SOUND_GAIN_VARIABLE		500.0f
#define MUSIC_GAIN_VARIABLE		100.0f

class COpenALManager : public CSingletonBase <COpenALManager>
{
public:
	static COpenALManager* Construct();
	~COpenALManager();

	ALint LoadSound( const char* lpFileName, ALboolean bLoop = AL_FALSE, ALboolean bStream = AL_FALSE );
	ALint LoadSoundFromBuffer( const ALuint &uiBuffer, ALboolean bLoop = AL_FALSE, ALboolean bStream = AL_FALSE );
	ALboolean Play( ALint iIndex );
	ALboolean PlayNo3D( ALint iIndex );
	ALboolean Play2D( ALint iIndex );
	ALboolean Pause( ALint iIndex );
	ALboolean Stop( ALint iIndex );
	ALboolean Move( ALint iIndex, ALfloat fX, ALfloat fY, ALfloat fZ );
	ALboolean Move( ALint iIndex, const CVector3 &in_cVec );
	ALboolean DoUpdateAllStream();
	ALvoid StopUpdateAllStream();

	// indeks musi nalezec do pliku ogg i nie wazne ktory wskazemy, bo source dla ogg i tak jest tylko jeden
	ALboolean IsOggPlaying( ALint iIndex );

	inline ALint GetSoundOALSize() { return static_cast<ALint>( m_aSoundOAL.size() ); }
	ALuint GetBufferFromSoundOAL( ALint iIndexSoundOAL );
	ALboolean IsError();

	inline ALint GetPercentSoundGain() { return static_cast<ALint>( m_fSoundGain * SOUND_GAIN_VARIABLE ); }
	ALvoid SetSoundGain( ALfloat fGain );
	ALvoid SetSoundGain( ALint iPecentGain );

	inline ALint GetPercentMusicGain() { return static_cast<ALint>( m_fMusicGain * MUSIC_GAIN_VARIABLE ); }
	ALvoid SetMusicGain( ALfloat fGain );
	ALvoid SetMusicGain( ALint iPecentGain );

	inline ALint GetPercentListenerGain() { return static_cast<ALint>( m_fListenerGain * LISTENER_GAIN_VARIABLE ); }
	ALvoid SetListenerGain( ALfloat fGain );
	ALvoid SetListenerGain( ALint iPecentGain );

	inline ALvoid SetListenerPosition( const CVector3 &in_cVec ) {
		m_cListenerPos = in_cVec;
		alListener3f( AL_POSITION, m_cListenerPos.x, m_cListenerPos.y, m_cListenerPos.z );
	}
	inline CVector3 GetListenerPosition() { return m_cListenerPos; }

	inline COpenALSources* GetOpenALSources() { return m_pOpenALSources; }

	ALvoid Timer();

	/// Metoda sprawdzaj¹ca czy inicjalizacja OpenAL zakoñczy³a siê pomyœlnie.
	inline ALboolean IsInitOK() { return m_bInitOK; }

	inline CSoundOpenAL* GetSoundOAL( ALint iIndex ) { return m_aSoundOAL[ iIndex ]; }

private:
	COpenALManager();

	COpenALSources	*m_pOpenALSources;

	ALboolean Initialization();
	ALboolean m_bInitOK;

	ALvoid SetListener();
	ALboolean Destroy();

	CVector3 m_cListenerPos;
	CVector3 m_cListenerVel;

	ALCdevice	*m_pDevice;
	ALCcontext	*m_pContext;

	ALfloat m_fListenerGain;
	ALfloat m_fSoundGain;
	ALfloat m_fMusicGain;

	std::vector<CSoundOpenAL*> m_aSoundOAL;
	
	HANDLE m_hUpdateAllStreamThread;
};
