// SoundFMOD.cpp: implementation of the CSoundFMOD class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#if !_USE_SOUND_OPENAL_

#include "../OpenGLFramework/Console.h"
#include "../OpenGLFramework/GameControl.h"
#include "../OpenGLFramework/WindowData.h"
#include "../Logger/Logger.h"
#include "SoundFMOD.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSoundFMOD::CSoundFMOD()
{
	if( !FSOUND_Init( 44100, 32, 0 ) )
		__LOG( _T("ERROR: CSoundFMOD::CSoundFMOD - Initialization FMOD failed") );


	VolumeIncrease( 255 );
}

CSoundFMOD::~CSoundFMOD()
{
	for( GLint i = GetSoundSamplesSize() - 1; i >= 0; --i ) {
		if( m_aSoundSamples[ i ].sample ) {
			if( FSOUND_IsPlaying( m_aSoundSamples[ i ].iChannel ) )
				FSOUND_StopSound( m_aSoundSamples[ i ].iChannel );
		}
		if( m_aSoundSamples[ i ].stream )
			FSOUND_Stream_Stop( m_aSoundSamples[ i ].stream );
	}

	m_aSoundSamples.clear();
	FSOUND_Close();
}

GLvoid CSoundFMOD::PlaySample( GLint iIndex )
{
	if( iIndex < 0 || iIndex >= GetSoundSamplesSize() ) {
		CONSOLE_ADDTEXT( CConsole::EError, _T("ERROR: CSoundFMOD::PlaySample - incorrect index %d"), iIndex );
		return;
	}

	if( FSOUND_IsPlaying( m_aSoundSamples[ iIndex ].iChannel ) )
		return;

	if( m_aSoundSamples[ iIndex ].sample == NULL )
		return;

	m_aSoundSamples[ iIndex ].iChannel = FSOUND_PlaySound( FSOUND_FREE, m_aSoundSamples[ iIndex ].sample );
	if( m_aSoundSamples[ iIndex ].iChannel == -1 )
		CONSOLE_ADDTEXT( CConsole::EError, _T("ERROR: CSoundFMOD::PlaySample - FSOUND_PlaySound failed") );
}

GLvoid CSoundFMOD::StopSample( GLint iIndex )
{
	if( iIndex < 0 || iIndex >= GetSoundSamplesSize() ) {
		CONSOLE_ADDTEXT( CConsole::EError, _T("ERROR: CSoundFMOD::StopSample - incorrect index %d"), iIndex );
		return;
	}

	if( m_aSoundSamples[ iIndex ].iChannel == -1 )
		return;

	if( FSOUND_IsPlaying( m_aSoundSamples[ iIndex ].iChannel ) )
		if( !FSOUND_StopSound( m_aSoundSamples[ iIndex ].iChannel ) )
			CONSOLE_ADDTEXT( CConsole::EWarning, _T("WARNING: CSoundFMOD::StopSample - FSOUND_StopSound failed") );
}

GLvoid CSoundFMOD::PlayStreamSample( GLint iIndex )
{
	if( iIndex < 0 || iIndex >= GetSoundSamplesSize() - 1 ) {
		CONSOLE_ADDTEXT( CConsole::EError, _T("ERROR: CSoundFMOD::PlayStreamSample - incorrect index %d"), iIndex );
		return;
	}

	if( FSOUND_IsPlaying( m_aSoundSamples[ iIndex ].iChannel ) )
		return;

	if( m_aSoundSamples[ iIndex ].stream == NULL )
		return;

	m_aSoundSamples[ iIndex ].iChannel = FSOUND_Stream_Play( FSOUND_FREE, m_aSoundSamples[ iIndex ].stream );
	if( m_aSoundSamples[ iIndex ].iChannel == -1 )
		CONSOLE_ADDTEXT( CConsole::EError, _T("ERROR: CSoundFMOD::PlayStreamSample - FSOUND_Stream_Play failed") );
}

GLvoid CSoundFMOD::StopStreamSample( GLint iIndex )
{
	if( iIndex < 0 || iIndex >= GetSoundSamplesSize() ) {
		CONSOLE_ADDTEXT( CConsole::EError, _T("ERROR: CSoundFMOD::StopStreamSample incorrect index %d"), iIndex );
		return;
	}

	if( m_aSoundSamples[ iIndex ].iChannel == -1 )
		return;

	if( m_aSoundSamples[ iIndex ].stream == NULL )
		return;

	if( !FSOUND_Stream_Stop( m_aSoundSamples[ iIndex ].stream ) )
		CONSOLE_ADDTEXT( CConsole::EWarning, _T("WARNING: CSoundFMOD::StopStreamSample - FSOUND_Stream_Stop failed") );
}

GLvoid CSoundFMOD::VolumeIncrease( GLint iValue )
{
	if( GetSoundSamplesSize() == 0 )
		return;

	GLint iNewVolume = FSOUND_GetSFXMasterVolume() + iValue;

	if( iNewVolume > 255 )
		iNewVolume = 255;

	FSOUND_SetSFXMasterVolume( iNewVolume );
}

GLvoid CSoundFMOD::VolumeDecrease( GLint iValue )
{
	if( GetSoundSamplesSize() == 0 )
		return;

	GLint iNewVolume = FSOUND_GetSFXMasterVolume() - iValue;

	if( iNewVolume < 0 )
		iNewVolume = 0;

	FSOUND_SetSFXMasterVolume( iNewVolume );
}

GLboolean CSoundFMOD::LoadSample( const char* lpSample, GLboolean bLoopSound )
{
	GLboolean bResult = GL_TRUE;

	SSample sample;
	m_aSoundSamples.push_back( sample );
	GLint iIndex = GetSoundSamplesSize() - 1;

	m_aSoundSamples[ iIndex ].stream = NULL;

	if( bLoopSound )
		m_aSoundSamples[ iIndex ].sample = FSOUND_Sample_Load( FSOUND_FREE, lpSample, FSOUND_STEREO | FSOUND_LOOP_NORMAL, 0, 0 );
	else 
		m_aSoundSamples[ iIndex ].sample = FSOUND_Sample_Load( FSOUND_FREE, lpSample, FSOUND_STEREO, 0, 0 );
		
	if( m_aSoundSamples[ iIndex ].sample == NULL )
		bResult = GL_FALSE;

	TCHAR cSample[ 256 ];
	for( GLint i = 0; i < 256; ++i ) {
		cSample[ i ] = lpSample[ i ];
		if( cSample[ i ] == 0 )
			break;
	}

	if( bResult )
		CONSOLE_ADDTEXT( CConsole::ESuccess, _T("Load sample: \"%s\" -> OK"), cSample );
	else
		CONSOLE_ADDTEXT( CConsole::EError, _T("Load sample: \"%s\" -> failed"), cSample );

	return bResult;
}

GLboolean CSoundFMOD::LoadStreamSample( const char* lpSample, GLboolean bLoopSound )
{
	GLboolean bResult = GL_TRUE;

	SSample sample;
	m_aSoundSamples.push_back( sample );
	GLint iIndex = GetSoundSamplesSize() - 1;

	m_aSoundSamples[ iIndex ].sample = NULL;

	if( bLoopSound )
		m_aSoundSamples[ iIndex ].stream = FSOUND_Stream_Open( lpSample, FSOUND_LOOP_NORMAL, 0, 0 );
	else 
		m_aSoundSamples[ iIndex ].stream = FSOUND_Stream_Open( lpSample, 0, 0, 0 );
	
	if( m_aSoundSamples[ iIndex ].stream == NULL ) 
		bResult = GL_FALSE;

	TCHAR cSample[ 256 ];
	for( GLint i = 0; i < 256; ++i ) {
		cSample[ i ] = lpSample[ i ];
		if( cSample[ i ] == 0 )
			break;
	}

	if( bResult )
		CONSOLE_ADDTEXT( CConsole::ESuccess, _T("Load stream sample: \"%s\" -> OK"), cSample );
	else
		CONSOLE_ADDTEXT( CConsole::EError, _T("Load stream sample: \"%s\" -> failed"), cSample );

	return bResult;
}

#endif // !_USE_SOUND_OPENAL_