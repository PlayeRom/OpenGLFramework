
#include "StdAfx.h"
#include "GameControl.h"
#include "RMenuBase.h"

CRMenuBase::CRMenuBase()
{
	m_iWhereIsMouse = -1;
	m_iWhereIsMouseOld = -1;
	m_aIndices = new GLint[ 8 ];
}

CRMenuBase::~CRMenuBase()
{
	delete [] m_aIndices;
}

GLvoid CRMenuBase::DrawRegionIfMouse( const RECT &rcRect, GLboolean bCheckMessageBox /*= GL_TRUE*/ )
{
	if( bCheckMessageBox && CGameControl::GetInstance()->GetRMessageBox()->IsDraw() )
		return;

	if( m_iWhereIsMouse != m_iWhereIsMouseOld ) {
		m_iWhereIsMouseOld = m_iWhereIsMouse;
		//if( CWindowData::GetInstance()->SettingFile().bSound )
		//	COpenALManager::GetInstance()->PlayNo3D( id_sound_mouse_on_button );
	}

	DrawRegionBorder( rcRect );
	glEnable( GL_DEPTH_TEST );
}

GLvoid CRMenuBase::DrawRegionBorder( const RECT &rcRect )
{
	CGameControl::GetInstance()->Enable2D();
	glColor4f( 0.5f, 0.5f, 0.5f, 1.0f );

	m_aIndices[ 0 ] = rcRect.left;
	m_aIndices[ 1 ] = rcRect.top;
	m_aIndices[ 2 ] = rcRect.left;
	m_aIndices[ 3 ] = rcRect.bottom;
	m_aIndices[ 4 ] = rcRect.right;
	m_aIndices[ 5 ] = rcRect.bottom;
	m_aIndices[ 6 ] = rcRect.right;
	m_aIndices[ 7 ] = rcRect.top;

	glEnableClientState( GL_VERTEX_ARRAY );
	glDisableClientState( GL_NORMAL_ARRAY );
	glDisableClientState( GL_COLOR_ARRAY );
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );

	glVertexPointer( 2, GL_INT, 0, m_aIndices );

	glDrawArrays( GL_LINE_LOOP, 0, 4 );
}

GLboolean CRMenuBase::CheckWhereClickedUser( GLint iX, GLint iY, const RECT *prcRect, GLint iMaxCount, GLint &out_iIndex )
{
	//sprawdzamy gdzie kliknal user
	POINT sPoint = { iX, iY };
	for( out_iIndex = 0; out_iIndex < iMaxCount; ++out_iIndex ) {
		if( ::PtInRect( &prcRect[ out_iIndex ], sPoint ) )
			return GL_TRUE;
	}

	return GL_FALSE;
}

GLvoid CRMenuBase::DrawRegions( const RECT *prcRect, GLint iMaxCount, GLint iStart /*= 0*/, GLboolean bSetDrawParam /*= GL_TRUE*/ )
{
	if( !prcRect || iMaxCount <= iStart )
		return;

	if( bSetDrawParam ) {
		glDisable( GL_TEXTURE_2D );
		CGameControl::GetInstance()->Enable2D();
		glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
		glDisable( GL_DEPTH_TEST );
		glColor4ub( 12, 18, 12, 200 );
	}
	glEnable( GL_BLEND );

	glEnableClientState( GL_VERTEX_ARRAY );
	glDisableClientState( GL_NORMAL_ARRAY );
	glDisableClientState( GL_COLOR_ARRAY );
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );

	for( GLint i = iStart; i < iMaxCount; ++i )
	{
		m_aIndices[ 0 ] = prcRect[ i ].right;
		m_aIndices[ 1 ] = prcRect[ i ].bottom;
		m_aIndices[ 2 ] = prcRect[ i ].right;
		m_aIndices[ 3 ] = prcRect[ i ].top;
		m_aIndices[ 4 ] = prcRect[ i ].left;
		m_aIndices[ 5 ] = prcRect[ i ].bottom;
		m_aIndices[ 6 ] = prcRect[ i ].left;
		m_aIndices[ 7 ] = prcRect[ i ].top;

		glVertexPointer( 2, GL_INT, 0, m_aIndices );
		glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
	}
	glDisable( GL_BLEND );
}

GLvoid CRMenuBase::SetRegionX( LONG &out_lPixelsVal, GLint iPercent, GLboolean bFont )
{
	if( bFont ) 
		out_lPixelsVal = ( 1024 * iPercent ) / 100;
	else 
		out_lPixelsVal = ( CWindowData::GetInstance()->SettingFile().iWidth * iPercent ) / 100;
}

GLvoid CRMenuBase::SetRegionY( LONG &out_lPixelsVal, GLint iPercent, GLboolean bFont )
{
	if( bFont ) 
		out_lPixelsVal = ( 768 * iPercent ) / 100;
	else 
		out_lPixelsVal = ( CWindowData::GetInstance()->SettingFile().iHeight * iPercent ) / 100;
}

GLvoid CRMenuBase::SetRegionX( LONG &out_lPixelsVal, GLfloat fPercent, GLboolean bFont )
{
	if( bFont ) 
		out_lPixelsVal = LONG( 10.24f * fPercent );
	else {
		GLfloat fWidth = ( GLfloat )CWindowData::GetInstance()->SettingFile().iWidth;
		out_lPixelsVal = LONG( ( fWidth * fPercent ) / 100.0f );
	}
}

GLvoid CRMenuBase::SetRegionY( LONG &out_lPixelsVal, GLfloat fPercent, GLboolean bFont )
{
	if( bFont ) 
		out_lPixelsVal = LONG( 7.68f * fPercent );
	else {
		GLfloat fHeight = ( GLfloat )CWindowData::GetInstance()->SettingFile().iHeight;
		out_lPixelsVal = LONG( ( fHeight * fPercent ) / 100.0f );
	}
}

GLboolean CRMenuBase::CheckPtCursorInRect( const POINT &ptCursor, const RECT &rcRect )
{
	return ( ptCursor.x > rcRect.left && ptCursor.x < rcRect.right
			&& ptCursor.y > rcRect.top && ptCursor.y < rcRect.bottom ) ? GL_TRUE : GL_FALSE;
}