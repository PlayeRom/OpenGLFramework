// MultiLanguage.cpp: implementation of the CMultiLanguage class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "../OpenGLFramework/GameControl.h"
#include "../OpenGLFramework/Console.h"
#include "MultiLanguage.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMultiLanguage* CMultiLanguage::Construct()
{
	return CSingletonBase::Construct( new CMultiLanguage );
}

CMultiLanguage::CMultiLanguage()
{
	m_bUseXML = GL_FALSE;
}

CMultiLanguage::~CMultiLanguage()
{
	for( GLint i = GetLngFileNameSize() - 1; i >= 0; --i )
		delete [] m_aLanguageFileName[ i ];

	m_aLanguageFileName.clear();

	ClearLineText();

	ClearXMLLngData();
}

GLvoid CMultiLanguage::ClearLineText()
{
	for( GLint i = GetLineTextSize() - 1; i >= 0; --i )
		delete [] m_aLineText[ i ];

	m_aLineText.clear();
}

LPTSTR CMultiLanguage::GetLangText( GLint iLineNumber )
{
	if( iLineNumber < 0 || iLineNumber >= GetLineTextSize() ) {
		::wsprintf( m_cErrText, _T("Error: GetLangText(%d)"), iLineNumber );
		return m_cErrText;
	}

	return m_aLineText[ iLineNumber ];
}

GLboolean CMultiLanguage::LoadLanguage( GLint iIndex )
{
	return LoadLanguageFile( GetLangFileName( iIndex ) );
}

GLint CMultiLanguage::GetLangFileNumber()
{
	if( m_bUseXML )
		return GetXMLLngDataSize();

	return GetLngFileNameSize();
}

LPTSTR CMultiLanguage::GetLangFileName( GLint iIndex )
{
	if( m_bUseXML )
		return GetXMLLngFileLNG( iIndex );

	return GetLngFileName( iIndex );
}

GLboolean CMultiLanguage::LoadLanguageFile( LPCTSTR lpFileName )
{
	/*
	Struktura pliku unicode
	na poczatku pliku zawsze sa 2 bajty = 255 i 254
	dalej sa juz normalne dane tekstowe - na kazdy znak po 2 bajty
	*/
	if( !lpFileName )
		return GL_FALSE;

	FILE *hFile = NULL;
	if( _tfopen_s( &hFile, lpFileName, _T("r") ) != 0 )
		return GL_FALSE;

	//sprawdzamy czy plik jest kodowany w unicode czy w ansi
	unsigned char cHeader[ 2 ];
	fread( &cHeader, 2, 1, hFile );

	GLboolean bUnicode = GL_FALSE;
	if( ( cHeader[ 0 ] == 255 && cHeader[ 1 ] == 254 ) || ( cHeader[ 0 ] == 254 && cHeader[ 1 ] == 255 ) ) {
//#ifdef UNICODE
		//wlanczamy flage jezeli mamy naglowek oraz zdefiniowane mamy UNICODE
		bUnicode = GL_TRUE;
//#endif
	}

	if( fseek( hFile, 0, SEEK_SET ) != 0 ) { //wracamy na poczatek pliku
		fclose( hFile );
		return GL_FALSE;
	}

	GLboolean bResult = GL_FALSE;
	if( bUnicode )
		bResult = LoadUnicodeFile( hFile );
	else
		bResult = LoadAnsiFile( hFile );

	if( bResult )
		CONSOLE_ADDTEXT( CConsole::ESuccess, _T("Load language file: \"%s\" -> OK"), lpFileName );
	else
		CONSOLE_ADDTEXT( CConsole::EError, _T("Load language file: \"%s\" -> failed"), lpFileName );

	return bResult;
}

GLboolean CMultiLanguage::LoadUnicodeFile( FILE *hFile )
{
	if( !hFile )
		return GL_FALSE;

	const int BUFFER_SIZE = 256;
	wchar_t cTempBuff[ BUFFER_SIZE ];
	TCHAR cLineTextTempBuffer[ BUFFER_SIZE ];

	::SecureZeroMemory( cTempBuff, sizeof( cTempBuff ) );
	::SecureZeroMemory( cLineTextTempBuffer, sizeof( cLineTextTempBuffer ) );

	ClearLineText();

	//wracamy na poczatek pliku, ale opuszczamy 2 pierwsze bajty - 255 i 254
	if( fseek( hFile, 2, SEEK_SET ) != 0 ) {
		fclose( hFile );
		return GL_FALSE;
	}

	//wczytujemy dane
	while( fgetws( cTempBuff, BUFFER_SIZE - 1, hFile ) != NULL ) {
		//sprawdzamy czy nie srednik, ale pierwszy bajt moze byc zerem wiec sprawdzamy takze drugi bajt
		if( cTempBuff[ 0 ] == 0x3B || cTempBuff[ 1 ] == 0x3B ) { }
		else {
			if( cTempBuff[ 0 ] == 0 ) {
				//jezeli pierwszy bajt rozpoczyna sie od zera to musimy przesunac
				for( GLint j = 0; j < BUFFER_SIZE - 1; ++j )
					cTempBuff[ j ] = cTempBuff[ j + 1 ];
			}

			GLint k = 0;
			for( GLint j = 0; j < BUFFER_SIZE - 1; ++j ) {
				cLineTextTempBuffer[ j ] = static_cast<unsigned short>( cTempBuff[ k ] + ( cTempBuff[ k + 1 ] * 256 ) ); //sklejam 2 oddzielne bajty
				
				//jezeli znajde kolejny srednik w srodku linii to koncze linie
				if( cLineTextTempBuffer[ j ] == 0x3B ) { //mamy srednik
					cLineTextTempBuffer[ j ] = 0; //koniec linii

					//usuwany biale znaki przed srednikiem
					for( GLint w = j - 1; w >= 0; --w ) {
						if( cLineTextTempBuffer[ w ] <= 32 ) //jezeli znak mniejszy/rowny spacji
							cLineTextTempBuffer[ w ] = 0;
						else break;
					}

					break;
				}

				k += 2;
			}

			//usuwanie znaku nowej lini 13 10
			for( GLint j = BUFFER_SIZE - 1; j > 0 ; --j ) {
				if( cLineTextTempBuffer[ j ] == 10 && cLineTextTempBuffer[ j - 1 ] == 13 ) {
					cLineTextTempBuffer[ j ] = 0;
					cLineTextTempBuffer[ j - 1 ] = 0;
					break;
				}
			}
			m_aLineText.push_back( NULL );
			GLint iIndex = GetLineTextSize() - 1;
			size_t uiSize = _tcslen( cLineTextTempBuffer ) + 1;
			m_aLineText[ iIndex ] = new TCHAR[ uiSize ];
			_tcscpy_s( m_aLineText[ iIndex ], uiSize, cLineTextTempBuffer );
		}
		::SecureZeroMemory( cTempBuff, sizeof( cTempBuff ) );
		::SecureZeroMemory( cLineTextTempBuffer, sizeof( cLineTextTempBuffer ) );
	}

	fclose( hFile );

	return GL_TRUE;
}

GLboolean CMultiLanguage::LoadAnsiFile( FILE *hFile )
{
	if( !hFile )
		return GL_FALSE;

	const int BUFFER_SIZE = 256;
	TCHAR cTempBuff[ BUFFER_SIZE ];
	::SecureZeroMemory( cTempBuff, sizeof( cTempBuff ) );

	ClearLineText();

	//wracamy na poczatek pliku
	if( fseek( hFile, 0, SEEK_SET ) != 0 ) {
		fclose( hFile );
		return GL_FALSE;
	}

	//wczytujemy dane
	while( _fgetts( cTempBuff, BUFFER_SIZE - 1, hFile ) != NULL ) {
		//sprawdzamy czy nie srednik
		if( cTempBuff[ 0 ] == 0x3B ) { }
		else {
			for( GLint j = 0; j < BUFFER_SIZE - 1; ++j ) {
				//jezeli srednik w srodku linii to koncze linie
				if( cTempBuff[ j ] == ';' ) {
					cTempBuff[ j ] = 0;

					//usuwamy biale znaki przed srednikiem
					for( GLint w = j - 1; w >= 0; --w ) {
						if( cTempBuff[ w ] <= 32 )
							cTempBuff[ w ] = 0;
						else break;
					}

					break;
				}

				//znajdzi i zniszcz znak nowej linii:
				if( cTempBuff[ j ] == '\n' ) {
					cTempBuff[ j ] = 0;
					break;
				}
			}
			m_aLineText.push_back( NULL );
			GLuint iIndex = GetLineTextSize() -1;
			size_t uiSize = _tcslen( cTempBuff ) + 1;
			m_aLineText[ iIndex ] = new TCHAR[ uiSize ];
			_tcscpy_s( m_aLineText[ iIndex ], uiSize, cTempBuff );
		}
		::SecureZeroMemory( cTempBuff, sizeof( cTempBuff ) );
	}

	fclose( hFile );

	return GL_TRUE;
}

GLvoid CMultiLanguage::InitializeLanguage()
{
	m_bUseXML = GL_FALSE;

	//sprawdzamy najpierw czy mamy plik XML, jezeli jest to wystarczy
	if( ParseXMLLngData() ) {
		m_bUseXML = GL_TRUE;
		return;
	}

	SearchLngFile();
}

GLvoid CMultiLanguage::SearchLngFile()
{
	LPCTSTR lpCurDir = _T("languages");
	WIN32_FIND_DATA wfd; 
	BOOL bIsFile = TRUE; 
	TCHAR cPath[ _MAX_DIR ]; 
	::SetCurrentDirectory( lpCurDir ); 
	HANDLE hFile = ::FindFirstFile( _T("*.*"), &wfd );
	GLuint i;
	GLint iLngCount = 0;

	while( bIsFile ) {
		TCHAR *pDest;
		::GetFullPathName( wfd.cFileName, _MAX_DIR, cPath, NULL );
		if( !( wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) && CheckExtensionLNG( cPath ) )
		{
			//cPath zawiera pe³n¹ sciezka C:\...\langauages\file.lng
			//dlatego tez obcinamy j¹ do samej: langauages\file.lng
			pDest = _tcsstr( cPath, lpCurDir );
			if( pDest ) {
				GLint iPos = static_cast< GLint >( pDest - cPath ); //pozycja od languages/file.lng
				size_t uiLength = _tcslen( cPath );
				m_aLanguageFileName.push_back( new TCHAR[ uiLength + 1 ] );
				for( i = 0; i < uiLength; ++i )
					m_aLanguageFileName[ iLngCount ][ i ] = cPath[ iPos + i ];
				m_aLanguageFileName[ iLngCount ][ i ] = 0;
				++iLngCount;
			}
		}

		bIsFile = ::FindNextFile( hFile, &wfd );
	}
    if( lpCurDir[ 0 ] > 3 )
		::SetCurrentDirectory( _T("..") );
}

GLboolean CMultiLanguage::CheckExtensionLNG( LPCTSTR lpPath )
{
	if( lpPath == NULL )
		return GL_FALSE;

	GLint i = static_cast< GLint >( _tcslen( lpPath ) ) - 4;
	
	if( i <= 0 )
		return GL_FALSE;

	return ( lpPath[ i ] == '.'
		&& ( lpPath[ i + 1 ] == 'l' || lpPath[ i + 1 ] == 'L' )
		&& ( lpPath[ i + 2 ] == 'n' || lpPath[ i + 2 ] == 'N' )
		&& ( lpPath[ i + 3 ] == 'g' || lpPath[ i + 3 ] == 'G' ) );
}

GLboolean CMultiLanguage::ParseXMLLngData( LPCTSTR lpFileName /*= _T("language/languages_def.xml")*/ )
{
	FILE *hFile = NULL;
	if( _tfopen_s( &hFile, lpFileName, _T("r") ) != 0 )
		return GL_FALSE;

	ClearXMLLngData();

	const int BUFFER_SIZE = 256;
	TCHAR cTempBuff[ BUFFER_SIZE ];
	TCHAR cParamName[ BUFFER_SIZE ]; //nazwa jezyka
	TCHAR cParamFile[ BUFFER_SIZE ]; //nazwa pliku lng
	TCHAR cFileTTF[ BUFFER_SIZE ]; //nazwa pliku TTF

	::SecureZeroMemory( cTempBuff, sizeof( cTempBuff ) );

	GLboolean bIsTagLanguages = GL_FALSE;
	GLboolean bIsTagLanguage = GL_FALSE;

	while( _fgetts( cTempBuff, BUFFER_SIZE - 1, hFile ) != NULL ) {
		//szukamy tagu: <languages>
		if( !bIsTagLanguages )
			bIsTagLanguages = SearchTag( cTempBuff, _T("<languages>") );
		else { //mamy tag: <languages>
			//szukamy tagu: <language ...>
			if( !bIsTagLanguage ) {
				bIsTagLanguage = SearchTag( cTempBuff, _T("<language") );
				if( bIsTagLanguage ) {
					//szukamy parametrów
					if( SearchParameter( cTempBuff, _T("name"), cParamName, BUFFER_SIZE ) ) {
						if( SearchParameter( cTempBuff, _T("file"), cParamFile, BUFFER_SIZE ) )
							AddXMLLngData( cParamName, cParamFile );
					}
				}
			}
			else { //mamy tag: <language ...>
				//szukamy <font ...>
				if( SearchTag( cTempBuff, _T("<font") ) ) {
					//szukamy parametru file
					if( SearchParameter( cTempBuff, _T("file"), cFileTTF, BUFFER_SIZE ) )
						AddXMLLngDataTTF( cFileTTF );
				}

				//szukamy tagu: </language>
				if( SearchTag( cTempBuff, _T("</language>") ) )
					bIsTagLanguage = GL_FALSE;
			}
		}
		::SecureZeroMemory( cTempBuff, sizeof( cTempBuff ) );
	}
	fclose( hFile );

	return GL_TRUE;
}

GLboolean CMultiLanguage::SearchTag( LPCTSTR lpSource, LPCTSTR lpTag )
{
	if( lpSource == NULL || lpTag == NULL )
		return GL_FALSE;

	size_t iSizeSource = _tcslen( lpSource );
	size_t iSizeTag = _tcslen( lpTag );

	if( iSizeSource < iSizeTag )
		return GL_FALSE;

	GLuint uiCounter = 0;
	for( GLuint i = 0; i < iSizeSource - iSizeTag + 1; ++i ) {
		for( GLuint j = 0; j < iSizeTag; ++j ) {
			if( lpSource[ i + j ] == lpTag[ j ] )
				++uiCounter;
		}
		if( uiCounter == iSizeTag )
			return GL_TRUE;
		else
			uiCounter = 0;
	}

	return GL_FALSE;
}

GLboolean CMultiLanguage::SearchParameter( LPCTSTR lpSource, LPCTSTR lpParameter, LPTSTR lpResult, GLsizei iSizeResult )
{
	if( lpResult == NULL )
		return GL_FALSE;
	::SecureZeroMemory( lpResult, iSizeResult );

	if( lpSource == NULL || lpParameter == NULL )
		return GL_FALSE;

	size_t iSizeSource = _tcslen( lpSource );
	size_t iSizeParam = _tcslen( lpParameter );

	if( iSizeSource < iSizeParam )
		return GL_FALSE;

	GLint iPosParam = -1;
	GLuint uiCounter = 0;
	for( GLuint i = 0; i < iSizeSource - iSizeParam + 1; ++i ) {
		if( iPosParam == -1 ) {
			//szukamy nazwy parametru
			for( GLuint j = 0; j < iSizeParam; ++j ) {
				if( lpSource[ i + j ] == lpParameter[ j ] )
					++uiCounter;
			}
			if( uiCounter == iSizeParam ) {
				iPosParam = i + iSizeParam + 2;
				break;
			}
			else
				uiCounter = 0;
		}
	}

	if( iPosParam == -1 )
		return GL_FALSE;

	//mamy nazwe wiec wydobywamy wartosc
	GLint j = 0;
	for( GLuint i = iPosParam; i < iSizeSource; ++i ) {
		if( lpSource[ i ] == '"' ) //koniec wartosci
			break;

		if( j < iSizeResult ) {
			lpResult[ j ] = lpSource[ i ];
			++j;
		}
	}
	lpResult[ j ] = 0;

	return GL_TRUE;
}

GLvoid CMultiLanguage::AddXMLLngData( LPCTSTR lpName, LPCTSTR lpLNG )
{
	if( lpName == NULL || lpLNG == NULL )
		return;

	SXMLLngData sXMLLngData;

	size_t uiSize = _tcslen( lpName ) + 1;
	sXMLLngData.lpName = new TCHAR[ uiSize ];
	_tcscpy_s( sXMLLngData.lpName, uiSize, lpName );

	uiSize = _tcslen( lpLNG ) + 1;
	sXMLLngData.lpFileLNG = new TCHAR[ uiSize ];
	_tcscpy_s( sXMLLngData.lpFileLNG, uiSize, lpLNG );

	m_aXMLLngData.push_back( sXMLLngData );
}

GLvoid CMultiLanguage::AddXMLLngDataTTF( LPCTSTR lpTTF )
{
	if( lpTTF == NULL )
		return;

	GLint iIndex = GetXMLLngDataSize() - 1;
	if( iIndex < 0 )
		return;

	size_t uiSize = _tcslen( lpTTF ) + 1;
	char* lpTemp = new char[ uiSize ];

	for( size_t i = 0; i < uiSize; ++i )
		lpTemp[ i ] = ( char )lpTTF[ i ];
	lpTemp[ uiSize - 1 ] = 0;

	m_aXMLLngData[ iIndex ].aFileTTF.push_back( lpTemp );
}

GLvoid CMultiLanguage::ClearXMLLngData()
{
	for( GLint i = GetXMLLngDataSize() - 1; i >= 0; --i ) {
		delete [] m_aXMLLngData[ i ].lpName;
		delete [] m_aXMLLngData[ i ].lpFileLNG;
		for( GLint j = GetXMLLngDataTTFSize( i ) - 1; j >= 0; --j )
			delete [] m_aXMLLngData[ i ].aFileTTF[ j ];

		m_aXMLLngData[ i ].aFileTTF.clear();
	}
	m_aXMLLngData.clear();
}