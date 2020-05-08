
#include "StdAfx.h"
#include "../OpenGLFramework/Console.h"
#include "../OpenGLFramework/GameControl.h"
#include "SoundOpenAL.h"
#include "OpenALSources.h"
#include "OpenALManager.h"
#include <process.h>

#if _USE_OGG_
	unsigned int __stdcall UpdateAllStreamThread( void *lpParam );
	ALboolean g_bUpdateAllStreamThread = AL_TRUE;
#endif

COpenALManager* COpenALManager::Construct()
{
	return CSingletonBase::Construct( new COpenALManager );
}

COpenALManager::COpenALManager()
{
	m_hUpdateAllStreamThread = NULL;
	m_fListenerGain = 1.0f;
	m_fSoundGain = 1.0f;
	m_fMusicGain = 1.0f;
	m_bInitOK = Initialization();
	if( !m_bInitOK )
		CONSOLE_ADDTEXT( CConsole::EError, _T("ERROR: OpenAL - initialization failed") );
}

COpenALManager::~COpenALManager()
{
#if _USE_OGG_
	g_bUpdateAllStreamThread = AL_FALSE;
#endif

	Destroy();
}

ALboolean COpenALManager::Initialization()
{
	// Open default sound device
	m_pDevice = alcOpenDevice( NULL );
	if( !m_pDevice ) {
		CONSOLE_ADDTEXT( CConsole::EError, _T("ERROR: CSoundOpenAL::Initialization - alcOpenDevice failed") );
		return AL_FALSE;
	}

	// Creating rendering context
	alcGetError( m_pDevice );
	m_pContext = alcCreateContext( m_pDevice, NULL );
	ALCenum eError = alcGetError( m_pDevice );
	if( eError != AL_NO_ERROR ) {
		CONSOLE_ADDTEXT( CConsole::EError, "%s", alcGetString( m_pDevice, eError ) );
		return AL_FALSE;
	}

	alGetError();

	alcMakeContextCurrent( m_pContext );

	SetListener();

	m_pOpenALSources = new COpenALSources();

	//wypisz informacje na konsole
	CONSOLE_ADDTEXT_NORMAL( "OpenALC info:" );
	CONSOLE_ADDTEXT_NORMAL( "DEFAULT_DEVICE_SPECIFIER: %s", alcGetString( m_pDevice, ALC_DEFAULT_DEVICE_SPECIFIER ) );
	CONSOLE_ADDTEXT_NORMAL( "DEVICE_SPECIFIER: %s", alcGetString( m_pDevice, ALC_DEVICE_SPECIFIER ) );
	CONSOLE_ADDTEXT_NORMAL( "EXTENSIONS: %s", alcGetString( m_pDevice, ALC_EXTENSIONS ) );
	CONSOLE_ADDTEXT_NORMAL( "OpenAL info:" );
	CONSOLE_ADDTEXT_NORMAL( "VERSION: %s", alGetString( AL_VERSION ) );
	CONSOLE_ADDTEXT_NORMAL( "RENDERER: %s", alGetString( AL_RENDERER ) );
	CONSOLE_ADDTEXT_NORMAL( "VENDOR: %s", alGetString( AL_VENDOR ) );
	CONSOLE_ADDTEXT_NORMAL( "EXTENSIONS: %s", alGetString( AL_EXTENSIONS ) );

	return AL_TRUE;
}

ALvoid COpenALManager::SetListener()
{
	// Position of the Listener.
	m_cListenerPos = CVector3( 0.0f, 0.0f, 0.0f );

	// Velocity of the Listener.
	m_cListenerVel = CVector3( 0.0f, 0.0f, 0.0f );

	// Orientation of the Listener. ( first 3 elements are "at", second 3 are "up")
	// Also note that these should be units of '1'.
	ALfloat aListenerOri[] = { 0.0, 0.0, -1.0,  0.0, 1.0, 0.0 };

	alListenerf( AL_GAIN, m_fListenerGain );
	alListener3f( AL_POSITION, m_cListenerPos.x, m_cListenerPos.y, m_cListenerPos.z );
    alListener3f( AL_VELOCITY, m_cListenerVel.x, m_cListenerVel.y, m_cListenerVel.z );
    alListenerfv( AL_ORIENTATION, aListenerOri );
}

ALboolean COpenALManager::Destroy()
{
	delete m_pOpenALSources;
	for( ALint i = GetSoundOALSize() - 1; i >= 0; --i )
		delete m_aSoundOAL[ i ];
	m_aSoundOAL.clear();

	alcMakeContextCurrent( NULL );
	alcDestroyContext( m_pContext );
	alcCloseDevice( m_pDevice );

	return AL_TRUE;
}

ALboolean COpenALManager::IsError()
{
	ALenum eError = alGetError();
	if( eError != AL_NO_ERROR ) {
		CONSOLE_ADDTEXT( CConsole::EError, "ERROR - OpenALManager: %s", alGetString( eError ) );
		alGetError();
		return AL_TRUE;
	}
	return AL_FALSE;
}

ALint COpenALManager::LoadSound( const char* lpFileName, ALboolean bLoop, ALboolean bStream )
{
	m_aSoundOAL.push_back( new CSoundOpenAL( bLoop, bStream ) );
	ALint iIndex = GetSoundOALSize() - 1;
	if( m_aSoundOAL[ iIndex ]->Open( lpFileName ) )
		CONSOLE_ADDTEXT( CConsole::ESuccess, "Load sample: \"%s\" -> OK", lpFileName );
	else
		CONSOLE_ADDTEXT( CConsole::EError, "Load sample: \"%s\" -> failed", lpFileName );

	return iIndex;
}

ALint COpenALManager::LoadSoundFromBuffer( const ALuint &uiBuffer, ALboolean bLoop, ALboolean bStream )
{
	m_aSoundOAL.push_back( new CSoundOpenAL( bLoop, bStream ) );
	ALint iIndex = GetSoundOALSize() - 1;
	if( m_aSoundOAL[ iIndex ]->OpenFromBufferWAV( uiBuffer ) )
		CONSOLE_ADDTEXT( CConsole::ESuccess, "Load sample from buffer: \"%d\" -> OK", uiBuffer );
	else
		CONSOLE_ADDTEXT( CConsole::EError, "Load sample from buffer: \"%d\" -> failed", uiBuffer );

	return iIndex;
}

ALboolean COpenALManager::Play( ALint iIndex )
{
	if( iIndex < 0 || iIndex >= GetSoundOALSize() )
		return AL_FALSE;

	m_aSoundOAL[ iIndex ]->Play();
	return AL_TRUE;
}

ALboolean COpenALManager::PlayNo3D( ALint iIndex )
{
	if( iIndex < 0 || iIndex >= GetSoundOALSize() )
		return AL_FALSE;

	m_aSoundOAL[ iIndex ]->Move( m_cListenerPos );
	m_aSoundOAL[ iIndex ]->Play();
	return AL_TRUE;
}

ALboolean COpenALManager::Play2D( ALint iIndex )
{
	if( iIndex < 0 || iIndex >= GetSoundOALSize() )
		return AL_FALSE;

	SetListenerPosition( CVector3() );
	m_aSoundOAL[ iIndex ]->Move( CVector3() );
	m_aSoundOAL[ iIndex ]->Play();
	return AL_TRUE;
}

ALboolean COpenALManager::Pause( ALint iIndex )
{
	if( iIndex < 0 || iIndex >= GetSoundOALSize() )
		return AL_FALSE;

	m_aSoundOAL[ iIndex ]->Pause();
	return AL_TRUE;
}

ALboolean COpenALManager::Stop( ALint iIndex )
{
	if( iIndex < 0 || iIndex >= GetSoundOALSize() )
		return AL_FALSE;

	m_aSoundOAL[ iIndex ]->Stop();
	return AL_TRUE;
}

ALboolean COpenALManager::Move( ALint iIndex, ALfloat fX, ALfloat fY, ALfloat fZ )
{
	return Move( iIndex, CVector3( fX, fY, fZ ) );
}

ALboolean COpenALManager::Move( ALint iIndex, const CVector3 &in_cVec )
{
	if( iIndex < 0 || iIndex >= GetSoundOALSize() )
		return AL_FALSE;

	m_aSoundOAL[ iIndex ]->Move( in_cVec );
	return AL_TRUE;
}

#if _USE_OGG_
unsigned int __stdcall UpdateAllStreamThread( void *lpParam )
{
	COpenALManager *pOpenALManager = ( COpenALManager* )lpParam;

	while( g_bUpdateAllStreamThread ) {
		for( ALint i = 0; i < pOpenALManager->GetSoundOALSize(); ++i )
			pOpenALManager->GetSoundOAL( i )->UpdateOggStream();
		::Sleep( 16 );
	}

	return 0;
}
#endif

ALboolean COpenALManager::DoUpdateAllStream()
{
#if _USE_OGG_
	g_bUpdateAllStreamThread = AL_TRUE;
	unsigned int uiThreadId;
	m_hUpdateAllStreamThread = ( HANDLE )::_beginthreadex( NULL, 0, &UpdateAllStreamThread, ( void* )this, 0, &uiThreadId );
	
	return m_hUpdateAllStreamThread ? AL_TRUE : AL_FALSE;
#else
	return AL_FALSE;
#endif
}

ALvoid COpenALManager::StopUpdateAllStream()
{
#if _USE_OGG_
	g_bUpdateAllStreamThread = AL_FALSE;
#endif
}

ALboolean COpenALManager::IsOggPlaying( ALint iIndex )
{
	if( iIndex < 0 || iIndex >= GetSoundOALSize() )
		return AL_FALSE;

	return m_aSoundOAL[ iIndex ]->IsOggPlaying();
}

ALvoid COpenALManager::SetListenerGain( ALint iPecentGain )
{
	if( iPecentGain > 100 )
		iPecentGain = 100;

	if( iPecentGain < 0 )
		iPecentGain = 0;

	SetListenerGain( static_cast<ALfloat>( iPecentGain ) / LISTENER_GAIN_VARIABLE );
}

ALvoid COpenALManager::SetListenerGain( ALfloat fGain )
{
	m_fListenerGain = fGain;
	alListenerf( AL_GAIN, m_fListenerGain );
}

ALvoid COpenALManager::SetSoundGain( ALint iPecentGain )
{
	if( iPecentGain > 100 )
		iPecentGain = 100;

	if( iPecentGain < 0 )
		iPecentGain = 0;

	SetSoundGain( static_cast<ALfloat>( iPecentGain ) / SOUND_GAIN_VARIABLE );
}

ALvoid COpenALManager::SetSoundGain( ALfloat fGain )
{
	m_fSoundGain = fGain;
	for( ALint i = 0; i < GetOpenALSources()->GetSourcesSize(); ++i )
		GetOpenALSources()->SetGain( i, fGain, AL_TRUE );
}

ALvoid COpenALManager::SetMusicGain( ALint iPecentGain )
{
	if( iPecentGain > 100 )
		iPecentGain = 100;

	if( iPecentGain < 0 )
		iPecentGain = 0;

	SetMusicGain( static_cast<ALfloat>( iPecentGain ) / MUSIC_GAIN_VARIABLE );
}

ALvoid COpenALManager::SetMusicGain( ALfloat fGain )
{
	m_fMusicGain = fGain;
	for( ALint i = 0; i < GetOpenALSources()->GetSourcesSize(); ++i )
		GetOpenALSources()->SetGain( i, fGain, AL_FALSE );
}

ALvoid COpenALManager::Timer()
{
	//wywolywane co sekunde
	GetOpenALSources()->Timer();
}

ALuint COpenALManager::GetBufferFromSoundOAL( ALint iIndexSoundOAL )
{
	return m_aSoundOAL[ iIndexSoundOAL ]->GetBuffer();
}