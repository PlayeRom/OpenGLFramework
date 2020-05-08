// ProfilerLogParser.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <vector>
using namespace std;

vector<char*> g_aFileName;
int GetFileNameSize() { return static_cast<int>( g_aFileName.size() ); }

void SearchFile( LPCTSTR lpCurDir, LPCTSTR lpExtension )
{
	WIN32_FIND_DATA wfd; 
	BOOL bIsFile = TRUE; 
	HANDLE hFile;
	TCHAR cPath[ 256 ];
	::SetCurrentDirectory( lpCurDir ); 
	hFile = ::FindFirstFile( _T("*.*"), &wfd ); 

	while( bIsFile ) {
		::GetFullPathName( wfd.cFileName, 255, cPath, NULL );
		if( !( wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) && wcsstr( cPath, lpExtension ) ) {
			//przepisanie z TCHAR na char
			unsigned int i;
			g_aFileName.push_back( new char[wcslen( cPath ) + 1 ] );
			int iIndex = GetFileNameSize() - 1;
			for( i = 0; i < wcslen( cPath ); ++i )
				g_aFileName[ iIndex ][ i ] = static_cast<char>( cPath[ i ] );
			g_aFileName[ iIndex ][ i ] = 0;
		}

		bIsFile = ::FindNextFile( hFile, &wfd );
	}
    if( lpCurDir[ 0 ] > 3 )
		::SetCurrentDirectory( _T("..") );
}

void ClearFileName()
{
	for( int i = GetFileNameSize()-1; i >= 0; --i )
		delete [] g_aFileName[ i ];

	g_aFileName.clear();
}

struct SParseData
{
	char *pName; 	//text podawany w Begin();
	int iTimeMs; 	//ilosc milisekund dla danego wywolania
	int iFrameMs; 	//czas w milisekundach wykonania jednej klatki
	float fPortion; //procent udzialu iTimeMs we iFrameMs
};

vector<SParseData> g_aParseData;
int GetParseDataSize() { return static_cast<int>( g_aParseData.size() ); }

void ClearParseData()
{
	for( int i = GetParseDataSize()-1; i >= 0; --i )
		delete [] g_aParseData[ i ].pName;

	g_aParseData.clear();
}

void Parse( const char *pFileName )
{
	ClearParseData();
	FILE *hFile = NULL;
	if( fopen_s( &hFile, pFileName, "rt") != 0 ) {
		cout << "Blad wczytania pliku: " << pFileName << endl;
		system("PAUSE");
		return;
	}

	const int LINE_BUFFOR_SIZE = 512;
	char cLine[LINE_BUFFOR_SIZE + 1 ];
	while( fgets( cLine, LINE_BUFFOR_SIZE, hFile ) != NULL ) {
		int iStartName = 9;
		int iEndName = -1;

		//szukamy ". Time: "
		for( int i = iStartName; i < LINE_BUFFOR_SIZE - 9; ++i ) {
			if(	   cLine[ i ]     == '.'
				&& cLine[ i + 1 ] == ' '
				&& cLine[ i + 2 ] == 'T'
				&& cLine[ i + 3 ] == 'i'
				&& cLine[ i + 4 ] == 'm'
				&& cLine[ i + 5 ] == 'e'
				&& cLine[ i + 6 ] == ':'
				&& cLine[ i + 7 ] == ' ')
			{
				iEndName = i;
				break;
			}
		}
		if( iEndName == -1 )
			continue;

		SParseData sParseData;
		sParseData.pName = new char[ iEndName - iStartName + 1 ];
		//kopiujemy nazwe do bufora
		int j = 0;
		for( int i = iStartName; i < iEndName; ++i, ++j )
			sParseData.pName[ j ] = cLine[ i ];

		sParseData.pName[ j ] = 0;

		//wez liczbe "Time:"
		char cTimeMs[ 32 ];
		j = 0;
		for( int i = iEndName + 8; i < LINE_BUFFOR_SIZE; ++i ) {
			if( cLine[ i ] == ' ')
				break;
			cTimeMs[ j ] = cLine[ i ];
			++j;
			if( j == 32 ) {
				cout << "Blad: Za duza liczba [ms]" << endl;
				system("PAUSE");
				--j;
				break;
			}
		}
		cTimeMs[ j ] = 0;
		sParseData.iTimeMs = atoi( cTimeMs );

		//szukamy ", Frame: "
		int iStartFrame = -1;
		for( int i = iStartName; i < LINE_BUFFOR_SIZE - 10; ++i ) {
			if(	   cLine[ i ]     == ','
				&& cLine[ i + 1 ] == ' '
				&& cLine[ i + 2 ] == 'F'
				&& cLine[ i + 3 ] == 'r'
				&& cLine[ i + 4 ] == 'a'
				&& cLine[ i + 5 ] == 'm'
				&& cLine[ i + 6 ] == 'e'
				&& cLine[ i + 7 ] == ':'
				&& cLine[ i + 8 ] == ' ')
			{
				iStartFrame = i;
				break;
			}
		}
		if( iStartFrame == -1 ) {
			delete [] sParseData.pName;
			continue;
		}

		//wez liczbe "Frame:"
		char cFrameMs[ 32 ];
		j = 0;
		for( int i = iStartFrame + 9; i < LINE_BUFFOR_SIZE; ++i ) {
			if( cLine[ i ] == ' ')
				break;
			cFrameMs[ j ] = cLine[ i ];
			++j;
			if( j == 32 ) {
				cout << "Blad: Za duza liczba [ms]" << endl;
				system("PAUSE");
				--j;
				break;
			}
		}
		cFrameMs[ j ] = 0;
		sParseData.iFrameMs = atoi( cFrameMs );

		if( sParseData.iTimeMs == 0 && sParseData.iFrameMs == 0 ) {
			delete [] sParseData.pName;
			continue;
		}

		if( sParseData.iFrameMs > 0 )
			sParseData.fPortion = ( static_cast<float>( sParseData.iTimeMs ) * 100.0f ) / static_cast<float>( sParseData.iFrameMs );
		else
			sParseData.fPortion = 0.0f;

		g_aParseData.push_back( sParseData);
	}
	fclose( hFile );
}

struct SDataForDisplay
{
	char* pName;
	int iCounter;	//ilosc wystapien danej nazwy
	int iSumTimeMs; //suma iTimeMs ze wszytskich wystapien danej nazy
	int iMinValue;	//najmniejsza wartosc ms
	int iMaxValue;	//najwieksza wartosc ms
	float fSumPortion;
};

void ShowParseData()
{
	//wyswietl sredni czas dla poszczegolnych nazw
	vector<SDataForDisplay> aDataForDisplay;
	for( int i = 0; i < GetParseDataSize(); ++i ) {
		bool bAddName = false;
		for( int j = 0; j < static_cast<int>( aDataForDisplay.size() ); ++j ) {
			if( strcmp( g_aParseData[ i ].pName, aDataForDisplay[ j ].pName ) == 0 ) {
				++aDataForDisplay[ j ].iCounter;
				aDataForDisplay[ j ].iSumTimeMs += g_aParseData[ i ].iTimeMs;
				aDataForDisplay[ j ].fSumPortion += g_aParseData[ i ].fPortion;

				if( g_aParseData[ i ].iTimeMs < aDataForDisplay[ j ].iMinValue )
					aDataForDisplay[ j ].iMinValue = g_aParseData[ i ].iTimeMs;

				if( g_aParseData[ i ].iTimeMs > aDataForDisplay[ j ].iMaxValue )
					aDataForDisplay[ j ].iMaxValue = g_aParseData[ i ].iTimeMs;

				bAddName = true;
				break;
			}
		}
		if( !bAddName ) { //mamy nowa nazwe
			SDataForDisplay sDataForDisplay;
			sDataForDisplay.pName = g_aParseData[ i ].pName;
			sDataForDisplay.iCounter = 1;
			sDataForDisplay.iSumTimeMs = g_aParseData[ i ].iTimeMs;
			sDataForDisplay.iMinValue = g_aParseData[ i ].iTimeMs;
			sDataForDisplay.iMaxValue = g_aParseData[ i ].iTimeMs;
			sDataForDisplay.fSumPortion = g_aParseData[ i ].fPortion;
			aDataForDisplay.push_back( sDataForDisplay );
		}
	}

	//wyswietlamy srednia arytmetyczna ms dla danej nazwy
	cout << endl << "----------------------------------------------------------------";
	for( int j = 0; j < static_cast<int>( aDataForDisplay.size() ); ++j ) {
		cout << endl << aDataForDisplay[ j ].pName;
		cout << endl << "Ilosc wystapien: " << aDataForDisplay[ j ].iCounter;
		cout << endl << "Wartosc min. i maks. Time [ms]: " << aDataForDisplay[ j ].iMinValue << " - " << aDataForDisplay[ j ].iMaxValue;
		cout << endl << "Srednia Time [ms]: " << float( aDataForDisplay[ j ].iSumTimeMs ) / float( aDataForDisplay[ j ].iCounter );
		cout << endl << "Srednia Portion %: " << aDataForDisplay[ j ].fSumPortion / float( aDataForDisplay[ j ].iCounter );
		cout << endl << "----------------------------------------------------------------";
	}

	aDataForDisplay.clear();
}

int _tmain( int argc, _TCHAR* argv[] )
{
	SearchFile( argv[ 0 ], _T(".log") );

	char cMenu = 0;
	do{
		cout << endl << "[A]nalizuj plik loga profilera";
		cout << endl << "[W]yjscie";
		cout << endl;
		cin >> cMenu;
		switch( cMenu ) {
			case 'A':
			case 'a':
			{
				if( GetFileNameSize() == 0 ) {
					cout << "Brak plikow!" << endl;
					system("PAUSE");
					break;
				}
				//wyswietlamy liste plikow
				for( int i = 0; i < GetFileNameSize(); ++i )
					cout << i << ". " << g_aFileName[ i ] << endl;

				cout << endl << "Wprowadz liczbe indeksu pliku, ktory zostanie przeanalizowany: ";
				int iFile = 0;
				cin >> iFile;
				if( iFile < 0 || iFile >= GetFileNameSize() ) {
					cout << "Nieprawidlowe dane" << endl;
					system("PAUSE");
					break;
				}
				Parse( g_aFileName[ iFile ] );
				ShowParseData();
				break;
			}

		}
	}
	while( cMenu != 'W' && cMenu != 'w');

	ClearParseData();
	ClearFileName();
	return 0;
}

