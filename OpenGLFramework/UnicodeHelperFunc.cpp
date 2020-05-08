
#include "stdafx.h"
#include "UnicodeHelperFunc.h"

wchar_t		CUHF::m_strWCharText[ UHF_MAX_BUFFER ][ UHF_MAX_STRING ] = { 0 };
char		CUHF::m_strCharText[ UHF_MAX_BUFFER][ UHF_MAX_STRING ] = { 0 };
int			CUHF::m_iActialStringBuffer = -1;

void CUHF::ConvertCharToWchar( wchar_t* strWCharText, int iWcharLen, const char* strCharText )
{
	if( !strWCharText || !strCharText )
		return;

	//konwertujemy na unicode
	for( int i = 0; i < iWcharLen; ++i ) {
		strWCharText[ i ] = strCharText[ i ];
		if( strCharText[ i ] == 0 )
			break;
	}
}

wchar_t* CUHF::ConvertCharToWchar( const char* strCharText, int iCharLen )
{
	if( !strCharText )
		return NULL;

	if( ++m_iActialStringBuffer >= UHF_MAX_BUFFER )
		m_iActialStringBuffer = 0;

	::SecureZeroMemory( m_strWCharText[ m_iActialStringBuffer ], UHF_MAX_STRING );
	//konwertujemy na unicode
	for( int i = 0; i < min( iCharLen, UHF_MAX_STRING ); ++i ) {
		m_strWCharText[ m_iActialStringBuffer ][ i ] = strCharText[ i ];
		if( strCharText[ i ] == 0 )
			break;
	}

	return m_strWCharText[ m_iActialStringBuffer ];
}

void CUHF::ConvertWcharToChar( char* strCharText, int iCharLen, const wchar_t* strWCharText )
{
	if( !strCharText || !strWCharText )
		return;
	//konwertujemy na ascii z unicode
	for( int i = 0; i < iCharLen; ++i ) {
		strCharText[ i ] = static_cast< char >( strWCharText[ i ] );
		if( strCharText[ i ] == 0 )
			break;
	}
}

char* CUHF::ConvertWcharToChar( const wchar_t* strWCharText, int iWcharLen )
{
	if( !strWCharText )
		return NULL;

	if( ++m_iActialStringBuffer >= UHF_MAX_BUFFER )
		m_iActialStringBuffer = 0;

	::SecureZeroMemory( m_strCharText[ m_iActialStringBuffer ], UHF_MAX_STRING );

	for( int i = 0; i < min( iWcharLen, UHF_MAX_STRING ); ++i ) {
		m_strCharText[ m_iActialStringBuffer ][ i ] = static_cast< char >( strWCharText[ i ] );
		if( m_strCharText[ m_iActialStringBuffer ][ i ] == 0 )
			break;
	}

	return m_strCharText[ m_iActialStringBuffer ];
}