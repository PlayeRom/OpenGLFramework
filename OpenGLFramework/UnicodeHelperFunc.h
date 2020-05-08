/*! \file
	\brief Plik nag³ówkowy dla klasy CUHF.
*/
#pragma once

// Dla zwracania wskaznikow na tekst, mamy max po 5 buforow, co moze byc malo i powodowac nadpsiywanie potrzebnych
// jeszcze stringow bo np. ConvertCharToWchar wywolamy 6 razy w wsprintf();
#define	UHF_MAX_STRING		512
#define	UHF_MAX_BUFFER		5

/// Klasa wspieraj¹ca operacje na ³añcuchach tekstu, ze wzglêdu na makro UNICODE.
class CUHF
{
public:
	static void		ConvertCharToWchar( wchar_t* strWCharText, int iWcharLen, const char* strCharText );
	static wchar_t* ConvertCharToWchar( const char* strCharText, int iCharLen );
	static void		ConvertWcharToChar( char* strCharText, int iCharLen, const wchar_t* strWCharText );
	static char*	ConvertWcharToChar( const wchar_t* strWCharText, int iWcharLen );

private:
	static wchar_t	m_strWCharText[ UHF_MAX_BUFFER ][ UHF_MAX_STRING ];
	static char		m_strCharText[ UHF_MAX_BUFFER ][ UHF_MAX_STRING ];
	static int		m_iActialStringBuffer;
};
