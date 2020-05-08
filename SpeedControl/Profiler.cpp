
#include "StdAfx.h"
#include "../Logger/Logger.h"
#include "Profiler.h"

std::auto_ptr<CProfiler> CProfiler::m_pProfiler;

CProfiler* CProfiler::GetInstance()
{
	if( !m_pProfiler.get() )
		m_pProfiler.reset( new CProfiler() );
	return m_pProfiler.get();
}

CProfiler::CProfiler()
{
	m_pLogger = new CLogger( _T("ProfilerLog") );
	m_pLogger->SetActive( GL_TRUE );
	m_iIndentCounter = 0;
}

CProfiler::~CProfiler()
{
	delete m_pLogger;
	m_aBeginMs.clear();
	m_aSaveToLog.clear();
	m_pProfiler.release();
}

unsigned long CProfiler::GetTimeInMs() //to samo co w CSpeedControl
{
	static __int64 i64Frequency = 0;
	if( ::QueryPerformanceFrequency( ( LARGE_INTEGER* )&i64Frequency ) ) {
		__int64 i64Counter = 0;
		if( ::QueryPerformanceCounter( ( LARGE_INTEGER* )&i64Counter ) )
			return static_cast<unsigned long>( i64Counter / ( i64Frequency / 1000 ) );
	}
	
	return ::timeGetTime();
}

GLvoid CProfiler::Begin( LPCTSTR lpComment )
{
	SBegin sBegin;
	if( lpComment ) {
		size_t uiSize = _tcslen( lpComment ) + 1;
		if( uiSize >= MAX_SIZE_OF_STRING )
			uiSize = MAX_SIZE_OF_STRING - 1;
		_tcscpy_s( sBegin.lpComment, uiSize, lpComment );
	}
	else
		sBegin.lpComment[ 0 ] = 0;

	sBegin.ulBeginMs = GetTimeInMs();
	m_aBeginMs.push_back( sBegin );

	if( ++m_iIndentCounter == 512 )
		m_iIndentCounter = 0; //przegiecie, zeroj
}

GLvoid CProfiler::End()
{
	if( GetBeginMsSize() == 0 ) //blad - najpierw trzeba uzyc CProfiler::GetInstance()->Begin()
		return;

	--m_iIndentCounter;

	//oblicz ms jako roznice aktualnego czasu a czasu z begin
	GLint iIndex = GetBeginMsSize() - 1;
	unsigned long ulDifference = GetTimeInMs() - m_aBeginMs[ iIndex ].ulBeginMs;

	SSaveToLog sSaveToLog;
	sSaveToLog.iIndentNumber = m_iIndentCounter;
	sSaveToLog.ulTimeMs = ulDifference;

	if( m_aBeginMs[ iIndex ].lpComment[ 0 ] ) {
		size_t uiSize = _tcslen( m_aBeginMs[ iIndex ].lpComment ) + 1;
		if( uiSize >= MAX_SIZE_OF_STRING )
			uiSize = MAX_SIZE_OF_STRING - 1;
		_tcscpy_s( sSaveToLog.lpComment, uiSize, m_aBeginMs[ iIndex ].lpComment );
	}
	else
		sSaveToLog.lpComment[ 0 ] = 0;

	m_aSaveToLog.push_back( sSaveToLog );

	//usun ostatni element begin
	m_aBeginMs.erase( m_aBeginMs.begin() + iIndex );
}

GLvoid CProfiler::Flush()
{
	if( m_aSaveToLog.size() < 1 )
		return;

	// polegamy na tym, ze ostatni element m_aSaveToLog to "MAIN LOOP"
	GLuint uiFrameMs = m_aSaveToLog[ m_aSaveToLog.size() - 1 ].ulTimeMs;

	//zapisz w kolejnosci odwrotnej i usun
	for( GLint i = static_cast<GLint>( m_aSaveToLog.size() ) - 1; i >= 0; --i ) {
		if( m_aSaveToLog[ i ].iIndentNumber > 0 ) //sa wciecia
			m_pLogger->SetTabulatorNumber( m_aSaveToLog[ i ].iIndentNumber );

		//oblicz procentowy udzial ulTimeMs w fFrameMs
		GLfloat fPercent = 0.0f;
		if( uiFrameMs > 0 )
			fPercent = ( static_cast<GLfloat>( m_aSaveToLog[ i ].ulTimeMs ) * 100.0f ) / static_cast<GLfloat>( uiFrameMs );

		if( m_aSaveToLog[ i ].lpComment[ 0 ] )
			m_pLogger->LOG( _T("%s. Time: %d [ms], Frame: %d [ms], Portion: %.2f%%"),
							m_aSaveToLog[ i ].lpComment, m_aSaveToLog[ i ].ulTimeMs, uiFrameMs, fPercent );
		else
			m_pLogger->LOG( _T("Time: %d [ms], Frame: %d [ms], Portion: %.2f%%"),
							m_aSaveToLog[ i ].ulTimeMs, uiFrameMs, fPercent );		
	}
	m_aSaveToLog.clear();
}