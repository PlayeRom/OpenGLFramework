
#include "StdAfx.h"
#include "../OpenGLFramework/Console.h"
#include "../OpenGLFramework/GameControl.h"
#include "OpenALSources.h"
#include "SoundOpenAL.h"

// Funkcja alutLoadWAVFile wczytuj¹ca WAVa jest potêpiana, dlatego te¿
// ustawiajaæ to makto na 1 mo¿emy wczytaæ WAVa za pomoc¹ biblioteki SDL.
#define __USE_SDL_TO_LOAD_WAV__		0

#if __USE_SDL_TO_LOAD_WAV__
	#include <SDL.h>
#endif


CSoundOpenAL::CSoundOpenAL( ALboolean bLoop, ALboolean bStream )
{
	m_pOpenALManager = COpenALManager::GetInstance();

	m_bLoop = bLoop;
	m_bStream = bStream;
	m_iSourceID = -1;
	m_pOggFileName = NULL;

	m_fDistance = 0.0f;
	m_cSourcePos = CVector3();

#if _USE_OGG_
	m_pOggFile = NULL;
	m_pOggInfo = NULL;
	m_hOggFile = NULL;
	m_bUpdateStream = AL_FALSE;
#endif
}

CSoundOpenAL::~CSoundOpenAL()
{
	Close();

#if _USE_OGG_
	delete [] m_pOggFileName;
#endif
}

ALboolean CSoundOpenAL::Open( const char* lpFileName )
{
	m_pOggFileName = NULL;
	m_bConstant = AL_TRUE;

	//sprawdzenie czy plik istnieje
	FILE *hFile = NULL;
	if( fopen_s( &hFile, lpFileName, "rb" ) != 0 )
		return AL_FALSE;
	fclose( hFile );

	if( IsExtension( lpFileName, "WAV" ) || IsExtension( lpFileName, "wav" ) ) {
		m_bStream = AL_FALSE;
		if( !m_bLoop )
			m_bConstant = AL_FALSE;
		return LoadWavFile( lpFileName );
	}
#if _USE_OGG_
	if( IsExtension( lpFileName, "OGG" ) || IsExtension( lpFileName, "ogg" ) ) {
		//zachowaj nazwe pliku, LoadOggFile wywolamy przy Play()
		size_t uiSize = strlen( lpFileName ) + 1;
		m_pOggFileName = new char[ uiSize ];
		strcpy_s( m_pOggFileName, uiSize, lpFileName );
		return AL_TRUE;
	}
#endif
	return AL_FALSE;
}

ALboolean CSoundOpenAL::OpenFromBufferWAV( const ALuint &uiBuffer )
{
	m_bStream = AL_FALSE;
	if( !m_bLoop )
		m_bConstant = AL_TRUE;
	else
		m_bConstant = AL_FALSE;

	return LoadWavFileFromBuffer( uiBuffer );
}

ALboolean CSoundOpenAL::IsExtension( const char* lpFileName, const char* lpExtension )
{
	size_t uiLengthExt = strlen( lpExtension );
	if( uiLengthExt != 3 )
		return AL_FALSE;

	size_t uiLength = strlen( lpFileName );
	if( uiLength < 5 )
		return AL_FALSE;

	if( lpFileName[ uiLength - 1 ] == lpExtension[ uiLengthExt - 1 ] &&
		lpFileName[ uiLength - 2 ] == lpExtension[ uiLengthExt - 2 ] &&
		lpFileName[ uiLength - 3 ] == lpExtension[ uiLengthExt - 3 ] )
	{
		return AL_TRUE;
	}

	return AL_FALSE;
}

ALboolean CSoundOpenAL::LoadWavFile( const char* lpFileName )
{
	// Generate buffers
	alGetError();
	alGenBuffers( 1, &m_uiBuffer );
	if( m_pOpenALManager->IsError() )
		return AL_FALSE;

#if !__USE_SDL_TO_LOAD_WAV__

	// Wczytanie WAVa za pomoc¹ potêpianych funkcji alutLoadWAVFile, alutUnloadWAV.

	ALenum		eFormat;
	ALvoid		*pData;
	ALsizei		uiSize;
	ALsizei		uiFreq;
	ALboolean	bLoop;

	// Load wav data
	alutLoadWAVFile( const_cast<ALbyte*>( lpFileName ), &eFormat, &pData, &uiSize, &uiFreq, &bLoop );
	if( m_pOpenALManager->IsError() )
		return AL_FALSE;

	m_uiFormat = eFormat;
	m_uiRate = uiFreq;

	// Fill buffer
	alBufferData( m_uiBuffer, eFormat, pData, uiSize, uiFreq );
	if( m_pOpenALManager->IsError() ) {
		alutUnloadWAV( eFormat, pData, uiSize, uiFreq );
		return AL_FALSE;
	}

	// Unload wav
	alutUnloadWAV( eFormat, pData, uiSize, uiFreq );
	if( m_pOpenALManager->IsError() )
		return AL_FALSE;
	
#else // !__USE_SDL_TO_LOAD_WAV__
	// Wczytanie WAVa za pomoc¹ biblioteki SDL.

	SDL_AudioSpec wav_spec;
	Uint32 uiSize;
	Uint8 *pData;

	// Load the WAV
	if( !SDL_LoadWAV( lpFileName, &wav_spec, &pData, &uiSize ) ) {
		__LOG_FORMATTED( "Could not open WAV %s: %s\n", lpFileName, SDL_GetError() );
		return AL_FALSE;
	}

	// SDL zawiera inne definicje dla okreslenia formatu niz OpenAL, dlatego konwertujemy na format OpenALa
	switch( wav_spec.format ) {
		case AUDIO_S16: // sprawdzone
			wav_spec.format = AL_FORMAT_MONO16;
			break;

		case AUDIO_U8: // sprawdzone
			wav_spec.format = AL_FORMAT_MONO8;
			break;

		case AUDIO_U16: // nie sprawdzone
		case AUDIO_U16MSB:
		case AUDIO_S16MSB:
		case AUDIO_S8:
			// TODO: tutaj AL_FORMAT_MONO16 jest domyœlnie i trzeba sprawdziæ co tak na prawdê podstawiæ
			// dla zawartych tu przypodków.
			wav_spec.format = AL_FORMAT_MONO16;
			break;
	}

	m_uiFormat = wav_spec.format;
	m_uiRate = wav_spec.freq;

	// Fill buffer
	alBufferData( m_uiBuffer, ALenum( wav_spec.format ), pData, uiSize, wav_spec.freq );
	if( m_pOpenALManager->IsError() ) {
		SDL_FreeWAV( pData );
		return AL_FALSE;
	}

	// Unload wav
	SDL_FreeWAV( pData );
#endif // !__USE_SDL_TO_LOAD_WAV__

	return AL_TRUE;
}

ALboolean CSoundOpenAL::LoadWavFileFromBuffer( const ALuint &uiBuffer )
{
	m_uiBuffer = uiBuffer;

	return AL_TRUE;
}

ALvoid CSoundOpenAL::Play( ALboolean bLoadOggFile /*= AL_TRUE*/ )
{
#if _USE_OGG_
	if( m_pOggFileName ) {
		ALboolean bLoadOK = AL_TRUE;
		if( bLoadOggFile )
			bLoadOK = LoadOggFile( m_pOggFileName );

		if( bLoadOK ) {
			m_bUpdateStream = AL_TRUE;
			GetOpenALSources()->PlayOgg( m_iSourceID );
		}
	}
	else
#endif
		GetOpenALSources()->Play( m_uiBuffer, m_bLoop, m_bConstant, m_cSourcePos );
}

ALvoid CSoundOpenAL::Pause()
{
#if _USE_OGG_
	if( m_iSourceID > -1 )
		GetOpenALSources()->PauseOgg( m_iSourceID );
	else
#endif
		GetOpenALSources()->Pause( m_uiBuffer );
}

ALvoid CSoundOpenAL::Stop()
{
#if _USE_OGG_
	if( m_iSourceID > -1 ) {
		GetOpenALSources()->StopOgg( m_iSourceID );
		Close();
	}
	else
#endif
		GetOpenALSources()->Stop( m_uiBuffer );
}

ALvoid CSoundOpenAL::Move( ALfloat fX, ALfloat fY, ALfloat fZ )
{
	Move( CVector3( fX, fY, fZ ) );
}

ALvoid CSoundOpenAL::Move( const CVector3 &in_cVec )
{
	m_cSourcePos = in_cVec;
}

ALboolean CSoundOpenAL::IsOggPlaying()
{
#if _USE_OGG_
	if( m_iSourceID < 0 )
		return AL_FALSE;

	ALint iState;
	alGetSourcei( GetOpenALSources()->GetSource( m_iSourceID ), AL_SOURCE_STATE, &iState );

    return ( iState == AL_PLAYING );
#else
	return AL_FALSE;
#endif
}

ALvoid CSoundOpenAL::Close()
{
#if _USE_OGG_
	m_bUpdateStream = AL_FALSE;
	if( m_hOggFile ) {
		fclose( m_hOggFile );
		m_hOggFile = NULL;
	}
	if( m_iSourceID > -1 && GetOpenALSources()->GetSourcesSize() > m_iSourceID )
		alSourcei( GetOpenALSources()->GetSource( m_iSourceID ), AL_BUFFER, 0 );
	else
#endif
		GetOpenALSources()->DetachSource( m_iSourceID );

#if _USE_OGG_
	if( m_pOggFile ) {	
		ov_clear( m_pOggFile );
		delete m_pOggFile;
		m_pOggFile = NULL;
	}
#endif
	if( alIsBuffer( m_uiBuffer ) )
		alDeleteBuffers( 1, &m_uiBuffer );
}

ALvoid CSoundOpenAL::CalculateDistance()
{
	m_fDistance = m_cSourcePos.CalcDistance3D( m_pOpenALManager->GetListenerPosition() );
}

#if _USE_OGG_
////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////// OGG ///////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------
//    ov_callback specific functions
//-----------------------------------------------------------

size_t ReadOgg( void *pDestBuf, size_t iElemSize, size_t iCount, void *pDataSource )
{
	return fread( pDestBuf, iElemSize, iCount, ( FILE* )pDataSource );
}

int SeekOgg( void *pDataSource, ogg_int64_t iOffset, int iWhence )
{
	switch( iWhence ) {
		case SEEK_SET:
		case SEEK_CUR:
		case SEEK_END:
			break;

		default:
			return -1; // error
	}
	return ( fseek( ( FILE* )pDataSource, static_cast<long>( iOffset ), iWhence ) == 0 ) ? 0 : -1;
}

long TellOgg( void *pDataSource )
{
	return ftell( ( FILE* )pDataSource );
}

int CloseOgg( void* pDataSource ) 
{
	return fclose( ( FILE* )pDataSource );
}

ALboolean CSoundOpenAL::LoadOggFile( const char* lpFileName )
{
	if( m_pOggFile )
		Stop();

	ALint iDynBuffs = 1;
	size_t uiBlockSize;
	ov_callbacks cb; // OggVorbis specific structures
	cb.close_func	= CloseOgg;
	cb.read_func	= ReadOgg;
	cb.seek_func	= SeekOgg;
	cb.tell_func	= TellOgg;

	// Create OggVorbis_File struct
	m_pOggFile = new OggVorbis_File;

	// Open Ogg file
	if( ::fopen_s( &m_hOggFile, lpFileName, "rb" ) != 0 )
		return AL_FALSE;

	// Generate local buffers
	if( ov_open_callbacks( ( void* )m_hOggFile, m_pOggFile, NULL, -1, cb ) < 0 ) {
		// This is not ogg bitstream. Return
		fclose( m_hOggFile );
		m_hOggFile = NULL;
		return AL_FALSE;
	}

	//zarezerwowanie source-a, jako bufor przekazujemy 0 bo nie wiazemy
	m_iSourceID = GetOpenALSources()->AttachBufferAndSetting( 0, m_bLoop, m_bConstant );
	if( m_iSourceID == -1 ) {
		fclose( m_hOggFile );
		m_hOggFile = NULL;
		return AL_FALSE;
	}

	// Check for existance of sound
	if( m_bStream ) {
		uiBlockSize	= DYNBUF_SIZE;
		iDynBuffs	= NUM_OF_DYNBUF;
		GetOpenALSources()->SetLooping( m_iSourceID, AL_FALSE );
	}
	else
		uiBlockSize = static_cast<size_t>( ov_pcm_total( m_pOggFile, -1 ) * 4 );
	
	// Return vorbis_comment and vorbis_info structures
	//m_pOggComment	= ov_comment( m_pOggFile, -1 );
	m_pOggInfo		= ov_info( m_pOggFile, -1 );

	// Fill buffer infos
	m_uiRate = m_pOggInfo->rate;
	m_uiFormat = ( m_pOggInfo->channels == 1 ) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;

	// Fill buffers with data each block by DYNBUF_SIZE bytes
	for( ALint i = 0; i < iDynBuffs; ++i ) {
		// Create buffers
		alGetError();
		alGenBuffers( 1, &m_uiBuffer );
		if( m_pOpenALManager->IsError() )
			return AL_FALSE;
		// Read amount ( DYNBUF_SIZE ) data into each buffer
		ReadOggBlock( m_uiBuffer, uiBlockSize );
		if( m_bStream ) { // Place buffer to queue
			alSourceQueueBuffers( GetOpenALSources()->GetSource( m_iSourceID ), 1, &m_uiBuffer );
			if( m_pOpenALManager->IsError() )
				return AL_FALSE;
		}
		else
			GetOpenALSources()->AttachBuffer( m_iSourceID, m_uiBuffer );
	}

	return AL_TRUE;
}

ALboolean CSoundOpenAL::ReadOggBlock( ALuint uiBufID, size_t uiSize )
{
	ALint		iCurrentSection;
	ALint		iTotalRet = 0;
	ALint		iRet = 0;

	if( uiSize < 1 )
		return AL_FALSE;

	ALchar *pBuffer = new ALchar[ uiSize ];

	// Read loop
	while( static_cast<size_t>( iTotalRet ) < uiSize ) {
		iRet = ov_read( m_pOggFile, pBuffer + iTotalRet, static_cast<int>( uiSize - iTotalRet ), 0, 2, 1, &iCurrentSection );

		// if end of file or read limit exceeded
		if( iRet == 0 )
			break;
		else if( iRet < 0 ) {}		// Error in bitstream
		else
			iTotalRet += iRet;
	}
	if( iTotalRet > 0 ) {
		alBufferData( uiBufID, m_uiFormat, ( void* )pBuffer, iTotalRet, m_uiRate );
		m_pOpenALManager->IsError();
	}
	delete [] pBuffer;

	return ( iRet > 0 );
}

ALvoid CSoundOpenAL::UpdateOggStream()
{
	if( !m_bStream || m_iSourceID == -1 || !m_bUpdateStream || !m_pOggFileName )
		return;
	
	ALint iProcessed = 0;
	ALuint uiBufID;

	alGetSourcei( GetOpenALSources()->GetSource( m_iSourceID ), AL_BUFFERS_PROCESSED, &iProcessed );
	if( m_pOpenALManager->IsError() ) {
		CONSOLE_ADDTEXT( CConsole::EWarning, _T("alGetSourcei") );
		return;
	}

	// We still have processed buffers
	while( iProcessed-- )
	{
		alSourceUnqueueBuffers( GetOpenALSources()->GetSource( m_iSourceID ), 1, &uiBufID );
		if( m_pOpenALManager->IsError() )
			CONSOLE_ADDTEXT( CConsole::EError, _T("alSourceUnqueueBuffers") );
		
		if( ReadOggBlock( uiBufID, DYNBUF_SIZE ) != 0 ) {
			alSourceQueueBuffers( GetOpenALSources()->GetSource( m_iSourceID ), 1, &uiBufID );
			if( m_pOpenALManager->IsError() ) {
				CONSOLE_ADDTEXT( CConsole::EError, _T("alSourceQueueBuffers 1") );
				return;
			}
		}
		else {
			ov_pcm_seek( m_pOggFile, 0 );
			alSourceQueueBuffers( GetOpenALSources()->GetSource( m_iSourceID ), 1, &uiBufID );
			if( m_pOpenALManager->IsError() ) {
				CONSOLE_ADDTEXT( CConsole::EError, _T("alSourceQueueBuffers 2") );
				return;
			}

			if( m_bLoop )
				Play( AL_FALSE );
			else
				Stop();
		}
	}
}
#endif //_USE_OGG_