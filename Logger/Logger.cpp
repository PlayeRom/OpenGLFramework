// Logger.cpp: implementation of the CLogger class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Logger.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLogger::CLogger( LPCTSTR lpFileName )
{
	m_bActive = GL_FALSE;
	m_hFile = NULL;
	m_uiCounterLine = 0;
	m_bClearOldLog = GL_TRUE;
	m_iTabNumber = 0;

	SYSTEMTIME sSysTime;
	::GetLocalTime( &sSysTime );
	::wsprintf( m_cFimeNameWithData, _T("%s - %d.%02d.%02d - %02d.%02d.%02d.log"),
				lpFileName,
				sSysTime.wYear, sSysTime.wMonth, sSysTime.wDay,
				sSysTime.wHour, sSysTime.wMinute, sSysTime.wSecond );
}

CLogger::~CLogger()
{
	Close();
}

GLvoid CLogger::ClearOldLog()
{
	DeleteLogFiles( _T("") );
}

GLboolean CLogger::Open()
{
	if( m_hFile )
		Close();

	if( _tfopen_s( &m_hFile, m_cFimeNameWithData, _T("at") ) != 0 )
		return GL_FALSE;

//	LOG( _T("----- Open log -----") );
	
	return GL_TRUE;
}

GLvoid CLogger::Close()
{
//	LOG( _T("----- Close log -----") );
	if( m_hFile )
		fclose( m_hFile );
	m_hFile = NULL;
}

GLvoid CLogger::SetActive( GLboolean bMode )
{
	if( bMode ) {
		m_bActive = GL_TRUE;
		//LOG( _T("----- LOG ON -----") );
	}
	else {
		//LOG( _T("----- LOG OFF -----") );
		m_bActive = GL_FALSE;
	}
}

GLvoid CLogger::LOG( const wchar_t *fmt, ... )
{
	if( !m_bActive || !fmt )
		return;

	Open();

	wchar_t cText[ 512 ];
	va_list ap;

	va_start( ap, fmt );
	vswprintf_s( cText, 512, fmt, ap );
	va_end( ap );

	if( m_iTabNumber == 0 )
		fwprintf( m_hFile, L" %06d. %s\n", ++m_uiCounterLine, cText );
	else {
		fwprintf( m_hFile, L" %06d. ", ++m_uiCounterLine );
		while( m_iTabNumber > 0 ) {
			putwc( L'\t', m_hFile );
			--m_iTabNumber;
		}
		fwprintf( m_hFile, L"%s\n", cText );
	}
	m_iTabNumber = 0;
	Close();
}

GLvoid CLogger::LOG( const char *fmt, ... )
{
	if( !m_bActive || !fmt )
		return;

	Open();

	char cText[ 512 ];
	va_list ap;
	va_start( ap, fmt );
	vsprintf_s( cText, 512, fmt, ap );
	va_end( ap );

	if( m_iTabNumber == 0 )
		fprintf( m_hFile, " %06d. %s\n", ++m_uiCounterLine, cText );
	else {
		fprintf( m_hFile, " %06d. ", ++m_uiCounterLine );
		while( m_iTabNumber > 0 ) {
			putc( '\t', m_hFile );
			--m_iTabNumber;
		}
		fprintf( m_hFile, "%s\n", cText );
	}
	m_iTabNumber = 0;
	Close();
}

GLvoid CLogger::LOGFreeChar( char cChar )
{
	if( !m_bActive || cChar == 0 )
		return;

	Open();

	fprintf( m_hFile, "%c", cChar );
	m_iTabNumber = 0;
	Close();
}

GLvoid CLogger::LOGFreeChar( wchar_t cChar )
{
	if( !m_bActive || cChar == 0 )
		return;

	Open();

	fwprintf( m_hFile, L"%c", cChar );
	m_iTabNumber = 0;
	Close();
}

GLvoid CLogger::LOGLastError( LPCTSTR lpWhatCallError )
{
	if( !m_bActive || !lpWhatCallError )
		return;

	Open();

	GLuint uiError = GetLastError();

	LPVOID lpMsgBuf = NULL;
	::FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER
					 | FORMAT_MESSAGE_FROM_SYSTEM
					 | FORMAT_MESSAGE_IGNORE_INSERTS,
					 NULL, uiError,
					 MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ), //MAKELANGID( LANG_ENGLISH, SUBLANG_ENGLISH_US ), // Default language
					 ( LPTSTR )&lpMsgBuf, 0, NULL );

	if( lpMsgBuf == NULL )
		return;

#ifdef UNICODE
	wchar_t cErrorDes[ 256 ];
	wcscpy_s( cErrorDes, ( LPCTSTR )lpMsgBuf );
	wchar_t z;
	for( GLuint i = 0; i < wcslen( cErrorDes ); ++i ) {
		z = cErrorDes[ i ];
		//taka dziwna zamiana polskich znaków z UNICODE na ascii, aby siê prawid³owo do pliku pisa³y
		if( z == 261 ) z = 185;//¹ 261
		else if( z == 281 ) z = 234;//ê 281
		else if( z == 347 ) z = 156;//œ 347
		else if( z == 263 ) z = 230;//æ 263
		else if( z == 380 ) z = 191;//¿ 380
		else if( z == 378 ) z = 159;//Ÿ 378
		else if( z == 324 ) z = 241;//ñ 324
		else if( z == 322 ) z = 179;//³ 322
		else if( z == 234 ) z = 243;//ó 243
		else if( z == 260 ) z = 165;//¥
		else if( z == 346 ) z = 140;//Œ
		else if( z == 280 ) z = 202;//Ê
		else if( z == 262 ) z = 198;//Æ
		else if( z == 379 ) z = 175;//¯
		else if( z == 377 ) z = 143;//
		else if( z == 323 ) z = 209;//Ñ
		else if( z == 321 ) z = 163;//£
		else if( z == 211 ) z = 211;//Ó
		cErrorDes[ i ] = z;
	}
	fwprintf( m_hFile, _T(" %06d. %s - ErrCode: %d, %s"), ++m_uiCounterLine, lpWhatCallError, uiError, cErrorDes );
#else
	fprintf( m_hFile, _T(" %06d. %s - ErrCode: %d, %s"), ++m_uiCounterLine, lpWhatCallError, uiError, ( LPCTSTR )lpMsgBuf );
#endif

	::LocalFree( lpMsgBuf );

	m_iTabNumber = 0;
	Close();
}

GLvoid CLogger::DeleteLogFiles( LPCTSTR lpCurDir )
{
	WIN32_FIND_DATA wfd; 
	BOOL bIsFile = TRUE; 
	HANDLE hFile;
	TCHAR cPath[ 256 ]; 
	::SetCurrentDirectory( lpCurDir ); 
	hFile = ::FindFirstFile( _T("*.*"), &wfd );

	while( bIsFile ) {
		::GetFullPathName( wfd.cFileName, 255, cPath, NULL );

		if( !( wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
			&& ( _tcsstr( cPath, _T(".log") ) || _tcsstr( cPath, _T(".LOG") ) ) )
		{
			::DeleteFile( cPath );
		}

		bIsFile = ::FindNextFile( hFile, &wfd );
	}

    if( lpCurDir[ 0 ] > 3 )
		::SetCurrentDirectory( _T("..") );
}