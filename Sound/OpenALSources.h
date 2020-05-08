#pragma once

#include <al.h>
#include <alc.h>
#include <alut.h>

class COpenALManager;

class COpenALSources
{
public:
	COpenALSources();
	~COpenALSources();

	ALboolean IsAttached( ALint iID, const ALuint &uiBuffer );
	ALint IsAttached( const ALuint &uiBuffer );
	ALint AttachBufferAndSetting( const ALuint &uiBuffer, ALboolean bLoop, ALboolean bConstant, const CVector3 &cPos = CVector3() );
	inline ALuint& GetSource( ALint iID ) { return m_aSources[ iID ].uiSource; }
	ALvoid SetLooping( ALint iID, ALboolean bLooping );
	ALvoid SetGain( ALint iID, ALfloat fGain, ALboolean bForSound );
	ALvoid AttachBuffer( ALint iID, const ALuint &uiBuffer );
	ALvoid DetachSource( ALint iID );
	ALvoid Timer();

	ALvoid Play( const ALuint &uiBuffer, ALboolean bLoop, ALboolean bConstant, const CVector3 &cPos = CVector3() );
	ALvoid Pause( const ALuint &uiBuffer );
	ALvoid Stop( const ALuint &uiBuffer );

#if _USE_OGG_
	ALvoid PlayOgg( ALint iID );
	ALvoid PauseOgg( ALint iID );
	ALvoid StopOgg( ALint iID );
#endif

	inline ALint GetSourcesSize() { return static_cast<ALint>( m_aSources.size() ); }

private:
	struct SSource
	{
		ALint iID; //iID = indeks tablicy
		ALuint uiSource;
		ALuint uiBuffer;
		ALboolean bAttached; //AL_TRUE jezeli powiazany z buforem
		CVector3 cPos; //pozycja w 3d
		ALboolean bLoop;
		ALboolean bConstant; //AL_TURE jezeli nie moze byc przebindowany do innego bufora
		ALuint uiRefTime; //czas ostatniego uzycia w sekundach od uruchomienia programu
		ALfloat fGain; //glosnosc danego dzwieku
	};

	COpenALManager	*m_pOpenALManager;
	ALvoid CreateSources();
	ALvoid SetSouceSetting( const SSource &sSource, const ALuint &uiBuffer = 0 );
	std::vector<SSource> m_aSources;

	ALuint m_uiCountTimer;
	ALuint m_uiInterval;
	ALuint m_uiTimeOut;
	ALvoid DetachOldSources();
	ALvoid DetachNoConstSource( ALint iIndex );
};
