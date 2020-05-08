#pragma once

#include <al.h>
#include <alc.h>
#include <alut.h>
#include <fstream>

#if _USE_OGG_
	#include "../OpenGLFramework/include/codec.h"
	#include "../OpenGLFramework/include/vorbisfile.h"
#endif

#include "OpenALManager.h"

class COpenALSources;

#if _USE_OGG_
	#define NUM_OF_DYNBUF	4		// num buffers in queue
	#define DYNBUF_SIZE		16384	// Buffer size
#endif

class CSoundOpenAL
{
public:
	CSoundOpenAL( ALboolean bLoop = AL_FALSE, ALboolean bStream = AL_FALSE );
	virtual ~CSoundOpenAL();

	ALboolean Open( const char* lpFileName );
	ALboolean OpenFromBufferWAV( const ALuint &uiBuffer );
	ALvoid Play( ALboolean bLoadOggFile = AL_TRUE );
	ALvoid Pause();
	ALvoid Stop();
#if _USE_OGG_
	ALvoid UpdateOggStream();
#endif
	ALvoid Move( ALfloat fX, ALfloat fY, ALfloat fZ );
	ALvoid Move( const CVector3 &in_cVec );

	inline ALuint GetBuffer() { return m_uiBuffer; }
	inline ALfloat GetDistance() { return m_fDistance; } //odleglosc zrodla od sluchacza

	inline COpenALSources* GetOpenALSources() { return m_pOpenALManager->GetOpenALSources(); }

	ALboolean IsOggPlaying();

private:
	COpenALManager	*m_pOpenALManager;

	ALboolean IsExtension( const char* lpFileName, const char* lpExtension );
	ALboolean LoadWavFile( const char* lpFileName );
	ALboolean LoadWavFileFromBuffer( const ALuint &uiBuffer );
#if _USE_OGG_
	ALboolean LoadOggFile( const char* lpFileName );
	ALboolean ReadOggBlock( ALuint uiBufID, size_t uiSize );
#endif
	ALvoid Close();

	ALboolean m_bLoop;
	ALboolean m_bStream; //moze byc tylko dla OGG, nigdy dla WAV
	ALint m_iSourceID;	//tylko dla ogg, dla wav nie uzywane i rowne -1

	ALboolean m_bConstant;

	ALvoid CalculateDistance();
	ALfloat m_fDistance; //odleglosc dzwieku od listenera
	CVector3 m_cSourcePos;
	char *m_pOggFileName;

#if _USE_OGG_
	//dla ogg
	OggVorbis_File	*m_pOggFile;
	vorbis_info		*m_pOggInfo;
	FILE			*m_hOggFile;
	ALboolean		m_bUpdateStream;
#endif

	//dane dla bufora
	ALuint m_uiBuffer;
	ALuint m_uiRate;
	ALuint m_uiFormat;
};
