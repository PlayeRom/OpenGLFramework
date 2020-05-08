// Cursor.cpp: implementation of the CCursor class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "WindowData.h"
#include "Cursor.h"
#include "resource.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCursor::CCursor()
{
	m_hCursor[ 0 ] = ::LoadCursor( CWindowData::GetInstance()->GetHINSTANCE(), MAKEINTRESOURCE( IDC_CURSOR1 ) );
	m_hCursor[ 1 ] = ::LoadCursor( CWindowData::GetInstance()->GetHINSTANCE(), MAKEINTRESOURCE( IDC_CURSOR2 ) );
	m_hCursor[ 2 ] = ::LoadCursor( CWindowData::GetInstance()->GetHINSTANCE(), MAKEINTRESOURCE( IDC_CURSOR3 ) );

	SetMyCursor( 1 );
}

CCursor::~CCursor()
{
	for( GLint i = MAX_CURSOR - 1; i >= 0; --i )
		::DestroyCursor( m_hCursor[ i ] );
}

GLvoid CCursor::SetMyCursor( GLint iIndex )
{
	if( iIndex < 0 || iIndex >= MAX_CURSOR ) return;

	m_iCursor = iIndex;

	::SetCursor( m_hCursor[ iIndex ] );
}
