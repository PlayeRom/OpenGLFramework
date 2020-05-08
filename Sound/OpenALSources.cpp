#include "StdAfx.h"
#include "../OpenGLFramework/Console.h"
#include "../OpenGLFramework/GameControl.h"
#include "OpenALManager.h"
#include "OpenALSources.h"

COpenALSources::COpenALSources()
{
	m_pOpenALManager = COpenALManager::GetInstance();
	
	m_uiCountTimer = 0;
	m_uiInterval = 2;		//sekundy co ile czyscic stare bufory
	m_uiTimeOut = 2;		//sekundy przedawnienia bufora

	CreateSources();
}

COpenALSources::~COpenALSources()
{
	for( ALint i = GetSourcesSize() - 1; i >= 0; --i )
		alDeleteSources( 1, &m_aSources[ i ].uiSource );
	m_aSources.clear();
}

ALvoid COpenALSources::CreateSources()
{
	SSource sSource = { 0 };
	ALint iIndex = 0;
	do{
		sSource.iID = iIndex; //iID musi byc rowne iIndex
		sSource.fGain = -1.0f;
		m_aSources.push_back( sSource );
		
		alGetError();
		alGenSources( 1, &m_aSources[ iIndex ].uiSource );

		ALenum eError = alGetError();
		if( eError != AL_NO_ERROR )
			CONSOLE_ADDTEXT( CConsole::EWarning, "WARNING - COpenALSources::CreateSources(): %s", alGetString( eError ) );
		
		++iIndex;
	}
	while( alIsSource( m_aSources[ GetSourcesSize() - 1 ].uiSource ) );

	//teraz mamy o jeden source nadmiarowo, wiec usuwamy ostatni source
	alDeleteSources( 1, &m_aSources[ GetSourcesSize() - 1 ].uiSource );
	m_aSources.erase( m_aSources.begin() + ( GetSourcesSize() - 1 ) );

	CONSOLE_ADDTEXT_NORMAL( _T("OpenALSources - sources number: %d"), GetSourcesSize() );

	//ustaw source zerowy dla muzyki
	if( GetSourcesSize() > 0 )
		m_aSources[ 0 ].bConstant = AL_TRUE;
}

ALboolean COpenALSources::IsAttached( ALint iID, const ALuint &uiBuffer )
{
	//sprawdzenie czy source o danym id jest nadal powiazany z zadanym buforem
	for( ALint i = 0; i < GetSourcesSize(); ++i ) {
		if( m_aSources[ i ].iID == iID ) {
			if( m_aSources[ i ].bAttached && m_aSources[ i ].uiBuffer == uiBuffer )
				return AL_TRUE;
		}
	}

	return AL_FALSE;
}

ALint COpenALSources::IsAttached( const ALuint &uiBuffer )
{
	//sprawdzenie czy source nadal powiazany z zadanym buforem
	for( ALint i = 0; i < GetSourcesSize(); ++i ) {
		if( m_aSources[ i ].bAttached && m_aSources[ i ].uiBuffer == uiBuffer )
			return m_aSources[ i ].iID;
	}

	return -1;
}

ALint COpenALSources::AttachBufferAndSetting( const ALuint &uiBuffer, ALboolean bLoop, ALboolean bConstant, const CVector3 &cPos /*= CVector3()*/ )
{
	if( uiBuffer == 0 ) { //dla OGG
		//najpierw sprawdzamy czy jest juz bConstant, jezeli jest to dla OGG, zwracamy wlasnie jego, bo
		//dla ogg - czyli muzyki, przeznaczamy tylko jednego source'a, bo i tak moze byc odgrywana tylko
		//jedna muzyka jednoczesnie. Czyli kazdy utwor ogg bedzie mial przypisanego tego samego source'a
		for( ALint i = 0; i < GetSourcesSize(); ++i ) {
			if( m_aSources[ i ].bConstant ) {
				m_aSources[ i ].bLoop = bLoop;
				m_aSources[ i ].cPos = cPos;
				m_aSources[ i ].bAttached = AL_TRUE;
				m_aSources[ i ].uiBuffer = uiBuffer;

				SetSouceSetting( m_aSources[ i ], uiBuffer );
				return m_aSources[ i ].iID; //zwroc ID source'a
			}
		}
	}

	//znajdz pierwszy wolny source i polacz z buffer
	for( ALint i = 0; i < GetSourcesSize(); ++i ) {
		if( !m_aSources[ i ].bAttached ) {
			m_aSources[ i ].bConstant = bConstant;
			m_aSources[ i ].bLoop = bLoop;
			m_aSources[ i ].cPos = cPos;
			m_aSources[ i ].bAttached = AL_TRUE;
			m_aSources[ i ].uiBuffer = uiBuffer;

			SetSouceSetting( m_aSources[ i ], uiBuffer );
			return m_aSources[ i ].iID; //zwroc ID source'a
		}
	}

	//TODO: nie znaleziono wolnego soucre-a, sortowanie[?]
	return -1;
}

ALvoid COpenALSources::SetSouceSetting( const SSource &sSource, const ALuint &uiBuffer /*= 0*/ )
{
	alSourcef( sSource.uiSource, AL_PITCH, 1.0f );
	if( sSource.fGain == -1.0f )
		alSourcef( sSource.uiSource, AL_GAIN, 1.0f );
	else
		alSourcef( sSource.uiSource, AL_GAIN, sSource.fGain );
	alSource3f( sSource.uiSource, AL_POSITION, sSource.cPos.x, sSource.cPos.y, sSource.cPos.z );
	alSource3f( sSource.uiSource, AL_VELOCITY, 0.0f, 0.0f, 0.0f );
	alSourcei(  sSource.uiSource, AL_LOOPING, sSource.bLoop );

	if( uiBuffer > 0 )
		AttachBuffer( sSource.iID, uiBuffer );
}

ALvoid COpenALSources::SetLooping( ALint iID, ALboolean bLooping )
{
	if( iID < 0 || iID >= GetSourcesSize() )
		return;

	m_aSources[ iID ].bLoop = bLooping;
	alSourcei( m_aSources[ iID ].uiSource, AL_LOOPING, bLooping );
}

ALvoid COpenALSources::SetGain( ALint iID, ALfloat fGain, ALboolean bForSound )
{
	if( iID < 0 || iID >= GetSourcesSize() )
		return;

	if( bForSound && !m_aSources[ iID ].bConstant ) {
		m_aSources[ iID ].fGain = fGain;
		alSourcef( m_aSources[ iID ].uiSource, AL_GAIN, fGain );
	}
	else if( !bForSound && m_aSources[ iID ].bConstant ) {
		m_aSources[ iID ].fGain = fGain;
		alSourcef( m_aSources[ iID ].uiSource, AL_GAIN, fGain );
	}
}

ALvoid COpenALSources::AttachBuffer( ALint iID, const ALuint &uiBuffer )
{
	if( iID < 0 || iID >= GetSourcesSize() )
		return;

	m_aSources[ iID ].uiBuffer = uiBuffer;
	alSourcei( m_aSources[ iID ].uiSource, AL_BUFFER, uiBuffer );
}

ALvoid COpenALSources::DetachSource( ALint iID )
{
	if( iID < 0 || iID >= GetSourcesSize() )
		return;

	alSourcei( m_aSources[ iID ].uiSource, AL_BUFFER, 0 );
	m_aSources[ iID ].bAttached = AL_FALSE;
	m_aSources[ iID ].bConstant = AL_FALSE;
}

ALvoid COpenALSources::Timer()
{
	if( ++m_uiCountTimer == m_uiInterval ) { //usuwamy co m_uiInterval sekund
		DetachOldSources();
		m_uiCountTimer = 0;
	}
}

ALvoid COpenALSources::DetachOldSources()
{
	ALuint uiCurrentSeconds = ::timeGetTime() / 1000;
	
	for( ALint i = GetSourcesSize() - 1; i >= 0; --i ) {
		if( uiCurrentSeconds - m_uiTimeOut <= m_aSources[ i ].uiRefTime )
			continue;
	
		DetachNoConstSource( i );
	}
}

ALvoid COpenALSources::DetachNoConstSource( ALint iIndex )
{
	if( iIndex < 0 || iIndex >= GetSourcesSize() )
		return;

	if( m_aSources[ iIndex ].bConstant )
		return;

	DetachSource( iIndex );
}

ALvoid COpenALSources::Play( const ALuint &uiBuffer, ALboolean bLoop, ALboolean bConstant, const CVector3 &cPos )
{
	ALint iIndex = IsAttached( uiBuffer );
	if( iIndex == -1 ) {
		iIndex = AttachBufferAndSetting( uiBuffer, bLoop, bConstant, cPos );
		if( iIndex == -1 )
			return;
	}
	else {
		//odswiezenie pozycji
		m_aSources[ iIndex ].cPos = cPos;
		alSource3f( m_aSources[ iIndex ].uiSource, AL_POSITION, m_aSources[ iIndex ].cPos.x,
																m_aSources[ iIndex ].cPos.y,
																m_aSources[ iIndex ].cPos.z );
	}
	m_aSources[ iIndex ].uiRefTime = ( ::timeGetTime() / 1000 ); //aktualizujemy czas ostatniego odwolania sie

	alGetError();
	alSourcePlay( m_aSources[ iIndex ].uiSource );
	m_pOpenALManager->IsError();
}

ALvoid COpenALSources::Pause( const ALuint &uiBuffer )
{
	ALint iIndex = IsAttached( uiBuffer );
	if( iIndex == -1 )
		return;

	m_aSources[ iIndex ].uiRefTime = ( ::timeGetTime() / 1000 ); //aktualizujemy czas ostatniego odwolania sie
	alSourcePause( m_aSources[ iIndex ].uiSource );
}

ALvoid COpenALSources::Stop( const ALuint &uiBuffer )
{
	ALint iIndex = IsAttached( uiBuffer );
	if( iIndex == -1 )
		return;

	alSourceStop( m_aSources[ iIndex ].uiSource );
	DetachNoConstSource( iIndex );
}

#if _USE_OGG_
ALvoid COpenALSources::PlayOgg( ALint iID )
{
	m_aSources[ iID ].uiRefTime = ( ::timeGetTime() / 1000 ); //aktualizujemy czas ostatniego odwolania sie
	alSourcePlay( m_aSources[ iID ].uiSource );
}

ALvoid COpenALSources::PauseOgg( ALint iID )
{
	m_aSources[ iID ].uiRefTime = ( ::timeGetTime() / 1000 ); //aktualizujemy czas ostatniego odwolania sie
	alSourcePause( m_aSources[ iID ].uiSource );
}

ALvoid COpenALSources::StopOgg( ALint iID )
{
	alSourceStop( m_aSources[ iID ].uiSource );
}
#endif