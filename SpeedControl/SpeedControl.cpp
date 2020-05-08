// SpeedControl.cpp: implementation of the CSpeedControl class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "../OpenGLFramework/Console.h"
#include "../OpenGLFramework/GameControl.h"
#include "SpeedControl.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSpeedControl::CSpeedControl()
{
	m_fMultiplier = 0.0f;
	m_fTimeLastFrame = 0.0f;
	m_fRegulationSpeed = 0.001f;
	m_bPause = GL_FALSE;
}

CSpeedControl::~CSpeedControl()
{
	m_aAnimation.clear();
}

GLvoid CSpeedControl::RefreshTime()
{
	GLfloat fCurrentTime = ::timeGetTime() * m_fRegulationSpeed;
	m_fMultiplier = fCurrentTime - m_fTimeLastFrame;
	m_fTimeLastFrame = fCurrentTime;
}

GLint CSpeedControl::CreateAnimationControl()
{
	m_aAnimation.push_back( GetTimeInMs() );
	GLint iIndex = GetAnimationSize() - 1;
	if( CGameControl::GetInstance()->GetConsole() )
		CONSOLE_ADDTEXT_NORMAL( _T("CreateAnimationControl: index = %d"), iIndex );
	
	return iIndex;
}

GLuint CSpeedControl::CheckAnimationTime( unsigned long ulMs, GLint iIndex, GLboolean bUpdateIfPause /*= GL_TRUE*/ )
{
	if( iIndex < 0 || iIndex >= GetAnimationSize() ) 
		return 0;

	if( m_bPause ) {
		if( bUpdateIfPause )
			m_aAnimation[ iIndex ] = GetTimeInMs();
		return 0;
	}

	if( ulMs == 0 ) ulMs = 1;

	unsigned long ulActualMs = GetTimeInMs() - m_aAnimation[ iIndex ];
	if( ulActualMs > ulMs ) {
		m_aAnimation[ iIndex ] = GetTimeInMs();
		return ulActualMs; //zwracamy aktualny czas animacji
	}

	return 0;
}

GLint CSpeedControl::GetActualAnimationTime( GLint iIndex )
{
	if( iIndex < 0 || iIndex >= GetAnimationSize() ) 
		return 0;

	//zwraca ilosc milisekund jaka aktualnie uplynela danej animacji
	return static_cast<GLint>( GetTimeInMs() - m_aAnimation[ iIndex ] );
}

/*
Jezeli nie zawsze wywolamy CheckAnimationTime bo, np. uzaleznione jest to od jakiegos
if'a i jezeli po dlugim okresie czasu, np. 2 sekundach wywolamy, np. 
CheckAnimationTime( 500, x ) to zwsze bedzie spelnione. Jezeli chcemy aby jednak odczekal
te pol sekundy to w miejsu tegoz if'a tworzymy else i tam wywolujemy UpdateAnimationTime
np.
int iIndex = CreateAnimationControl();
if( costam == true )
{
	if( CheckAnimationTime( 500, iIndex ) )
	{
	}
}
else
	UpdateAnimationTime( iIndex );
*/
GLvoid CSpeedControl::UpdateAnimationTime( GLint iIndex )
{
	if( iIndex < 0 || iIndex >= GetAnimationSize() ) 
		return;

	if( m_bPause )
		return;

	m_aAnimation[ iIndex ] = GetTimeInMs();
}

unsigned long CSpeedControl::GetTimeInMs()
{
	static __int64 i64Frequency = 0;
	if( ::QueryPerformanceFrequency( ( LARGE_INTEGER* )&i64Frequency ) ) {
		__int64 i64Counter = 0;
		if( ::QueryPerformanceCounter( ( LARGE_INTEGER* )&i64Counter ) )
			return static_cast<unsigned long>( i64Counter / ( i64Frequency / 1000 ) );
	}
	
	return ::timeGetTime();
}