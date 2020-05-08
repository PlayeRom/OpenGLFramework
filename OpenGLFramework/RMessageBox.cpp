// RMessageBox.cpp: implementation of the CRMessageBox class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GameControl.h"
#include "../Fonts/SDLFont.h"
#include "WindowData.h"
#include "Cursor.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRMessageBox::CRMessageBox()
{
	m_pSDLFontText = new CSDLFont( CMultiLanguage::GetInstance()->GetXMLLngFileTTF( 0 ), 20 );

	SetRegions();

	m_bDrawMsg = GL_FALSE;

	lpFunc1 = NULL;
	lpFunc2 = NULL;

	m_aButtonIndices = new GLint[ 8 ];
}

CRMessageBox::~CRMessageBox()
{
	delete m_pSDLFontText;
	delete [] m_aButtonIndices;
}

GLvoid CRMessageBox::SetRegions()
{
	//ustawioamy tlo na caly ekran
	m_rRegionsBkg[ 0 ].left = 0;
	m_rRegionsBkg[ 0 ].right = CWindowData::GetInstance()->SettingFile().iWidth;
	m_rRegionsBkg[ 0 ].top = 0;
	m_rRegionsBkg[ 0 ].bottom = CWindowData::GetInstance()->SettingFile().iHeight;
	//ustawiamy tlo jako ramke
	SetRegionX( m_rRegionsBkg[ 1 ].left, 5 );
	SetRegionX( m_rRegionsBkg[ 1 ].right, 95 );
	SetRegionY( m_rRegionsBkg[ 1 ].top, 40 );
	SetRegionY( m_rRegionsBkg[ 1 ].bottom, 62 );

	GLint iTop = 55;
	GLint iBottom = 60;
	//dla ok
	SetRegionX( m_rRegions1Button[ 0 ].left, 40 );
	SetRegionX( m_rRegions1Button[ 0 ].right, 60 );
	SetRegionY( m_rRegions1Button[ 0 ].top, iTop );
	SetRegionY( m_rRegions1Button[ 0 ].bottom, iBottom );
	SetRegionX( m_rRegionsFont1Button[ 0 ].left, 40, GL_TRUE );
	SetRegionX( m_rRegionsFont1Button[ 0 ].right, 60, GL_TRUE );
	SetRegionY( m_rRegionsFont1Button[ 0 ].top, iTop, GL_TRUE );
	SetRegionY( m_rRegionsFont1Button[ 0 ].bottom, iBottom, GL_TRUE );

	//dla dwoch przyciskow
	SetRegionX( m_rRegions2Button[ 0 ].left, 29 );
	SetRegionX( m_rRegions2Button[ 0 ].right, 49 );
	SetRegionY( m_rRegions2Button[ 0 ].top, iTop );
	SetRegionY( m_rRegions2Button[ 0 ].bottom, iBottom );
	SetRegionX( m_rRegionsFont2Button[ 0 ].left, 29, GL_TRUE );
	SetRegionX( m_rRegionsFont2Button[ 0 ].right, 49, GL_TRUE );
	SetRegionY( m_rRegionsFont2Button[ 0 ].top, iTop, GL_TRUE );
	SetRegionY( m_rRegionsFont2Button[ 0 ].bottom, iBottom, GL_TRUE );
	//
	SetRegionX( m_rRegions2Button[ 1 ].left, 51 );
	SetRegionX( m_rRegions2Button[ 1 ].right, 71 );
	SetRegionY( m_rRegions2Button[ 1 ].top, iTop );
	SetRegionY( m_rRegions2Button[ 1 ].bottom, iBottom );
	SetRegionX( m_rRegionsFont2Button[ 1 ].left, 51, GL_TRUE );
	SetRegionX( m_rRegionsFont2Button[ 1 ].right, 71, GL_TRUE );
	SetRegionY( m_rRegionsFont2Button[ 1 ].top, iTop, GL_TRUE );
	SetRegionY( m_rRegionsFont2Button[ 1 ].bottom, iBottom, GL_TRUE );

	//dla trzech przyciskow
	SetRegionX( m_rRegions3Button[ 0 ].left, 19 );
	SetRegionX( m_rRegions3Button[ 0 ].right, 39 );
	SetRegionY( m_rRegions3Button[ 0 ].top, iTop );
	SetRegionY( m_rRegions3Button[ 0 ].bottom, iBottom );
	SetRegionX( m_rRegionsFont3Button[ 0 ].left, 19, GL_TRUE );
	SetRegionX( m_rRegionsFont3Button[ 0 ].right, 39, GL_TRUE );
	SetRegionY( m_rRegionsFont3Button[ 0 ].top, iTop, GL_TRUE );
	SetRegionY( m_rRegionsFont3Button[ 0 ].bottom, iBottom, GL_TRUE );
	//
	SetRegionX( m_rRegions3Button[ 1 ].left, 40 );
	SetRegionX( m_rRegions3Button[ 1 ].right, 60 );
	SetRegionY( m_rRegions3Button[ 1 ].top, iTop );
	SetRegionY( m_rRegions3Button[ 1 ].bottom, iBottom );
	SetRegionX( m_rRegionsFont3Button[ 1 ].left, 40, GL_TRUE );
	SetRegionX( m_rRegionsFont3Button[ 1 ].right, 60, GL_TRUE );
	SetRegionY( m_rRegionsFont3Button[ 1 ].top, iTop, GL_TRUE );
	SetRegionY( m_rRegionsFont3Button[ 1 ].bottom, iBottom, GL_TRUE );
	//
	SetRegionX( m_rRegions3Button[ 2 ].left, 61 );
	SetRegionX( m_rRegions3Button[ 2 ].right, 81 );
	SetRegionY( m_rRegions3Button[ 2 ].top, iTop );
	SetRegionY( m_rRegions3Button[ 2 ].bottom, iBottom );
	SetRegionX( m_rRegionsFont3Button[ 2 ].left, 61, GL_TRUE );
	SetRegionX( m_rRegionsFont3Button[ 2 ].right, 81, GL_TRUE );
	SetRegionY( m_rRegionsFont3Button[ 2 ].top, iTop, GL_TRUE );
	SetRegionY( m_rRegionsFont3Button[ 2 ].bottom, iBottom, GL_TRUE );
}

GLvoid CRMessageBox::RestartObjects()
{
	SetRegions();
}

GLboolean CRMessageBox::KeyDown( GLuint uiKeyCode )
{
	if( !IsDraw() ) 
		return GL_FALSE;

	switch( uiKeyCode ) {
		case VK_RETURN:
			m_bDrawMsg = GL_FALSE; //nie rysuj juz MessageBox'a
			if( lpFunc1 ) lpFunc1();
			return GL_TRUE;

		case VK_ESCAPE:
			m_bDrawMsg = GL_FALSE; //nie rysuj juz MessageBox'a
			if( lpFunc2 ) lpFunc2();
			return GL_TRUE;
	}

	return GL_TRUE;
}

GLboolean CRMessageBox::MouseLButtonDown( GLint iX, GLint iY )
{
	if( !IsDraw() ) 
		return GL_FALSE;

	GLint i = 0;
	//sprawdzamy gdzie kliknal user
	switch( m_eMsgType ) {
		case EMsgOK:
			if( !CheckWhereClickedUser( iX, iY, m_rRegions1Button, 1, i ) )
				return GL_TRUE;
			break;

		case EMsgOKCancel:
		case EMsgYesNo:
			if( !CheckWhereClickedUser( iX, iY, m_rRegions2Button, 2, i ) )
				return GL_TRUE;
			break;

		case EMsgYesNoCancel:
			if( !CheckWhereClickedUser( iX, iY, m_rRegions3Button, 3, i ) )
				return GL_TRUE;
			break;
	}

	switch( m_eMsgType ) {
		case EMsgOK:
		{
			m_bDrawMsg = GL_FALSE; //nie rysuj juz MessageBox'a
			if( lpFunc1 )
				lpFunc1();

			return GL_TRUE;
		}
		case EMsgOKCancel:
		{
			m_bDrawMsg = GL_FALSE;

			if( i == 0 && lpFunc1 )
				lpFunc1();
			else if( lpFunc2 )
				lpFunc2();

			return GL_TRUE;
		}
		case EMsgYesNo:
		{
			m_bDrawMsg = GL_FALSE;

			if( i == 0 && lpFunc1 ) 
				lpFunc1();
			else if( lpFunc2 ) 
				lpFunc2();

			return GL_TRUE;
		}
		case EMsgYesNoCancel:
		{
			m_bDrawMsg = GL_FALSE;

			if( i == 0 && lpFunc1 ) 
				lpFunc1();
			else if( i == 1 && lpFunc2 )
				lpFunc2();
			else { //anuluj - nie rob nic
			}

			return GL_TRUE;
		}
	}

	return GL_TRUE;
}

GLvoid CRMessageBox::DoMessageBox( LPCTSTR lpText, EMsgType eType, GLvoid( *in_lpFunc1 )( GLvoid ), GLvoid( *in_lpFunc2 )( GLvoid ) )
{
	if( m_bDrawMsg ) return; //MsgBox juz sie rysuje, wiec 2 razy nie moze byc wywolane

	m_eMsgType = eType;
	lpFunc1 = in_lpFunc1;
	lpFunc2 = in_lpFunc2;
	_tcscpy_s( m_cText, MSG_TEXT_LENGTH, lpText );
	m_bDrawMsg = GL_TRUE; //rysuj MessageBox'a
}

GLvoid CRMessageBox::DrawButtons( RECT rc[ 3 ], GLint iSize )
{
	glEnableClientState( GL_VERTEX_ARRAY );
	glDisableClientState( GL_NORMAL_ARRAY );
	glDisableClientState( GL_COLOR_ARRAY );
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );

	for( GLint i = 0; i < iSize; ++i ) {
		m_aButtonIndices[ 0 ] = rc[ i ].right;
		m_aButtonIndices[ 1 ] = rc[ i ].bottom;
		m_aButtonIndices[ 2 ] = rc[ i ].right;
		m_aButtonIndices[ 3 ] = rc[ i ].top;
		m_aButtonIndices[ 4 ] = rc[ i ].left;
		m_aButtonIndices[ 5 ] = rc[ i ].bottom;
		m_aButtonIndices[ 6 ] = rc[ i ].left;
		m_aButtonIndices[ 7 ] = rc[ i ].top;

		glVertexPointer( 2, GL_INT, 0, m_aButtonIndices );

		glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
	}
}

GLvoid CRMessageBox::DrawFonts( RECT rcRegions[ 3 ], RECT rcRegionsFont[ 3 ], GLint iSize )
{
	glDisable( GL_BLEND );
	glEnable( GL_DEPTH_TEST );

	COLORREF crColor;
	GLboolean bPtCursorInRect = GL_FALSE;
	GLint i;
	for( i = 0; i < iSize; ++i ) {
		//DrawRegion( m_rRegions[ 0 ] );
		if( CheckPtCursorInRect( CGameControl::GetInstance()->GetPointCursor(), rcRegions[ i ] ) ) {
			bPtCursorInRect = GL_TRUE;
			m_iWhereIsMouse = i;
			DrawRegionIfMouse( rcRegions[ i ], GL_FALSE );
		}
		else {
			bPtCursorInRect = GL_FALSE;
			if( m_iWhereIsMouse == i )
				m_iWhereIsMouseOld = -1; //nigdzie
		}

		if( i == 0 ) { //ok/tak lekko na zielono
			crColor = bPtCursorInRect ? RGB( 0, 255, 0 ) : RGB( 50, 255, 0 );

			switch( m_eMsgType ) {
				case EMsgOK:
					m_pSDLFontText->DrawText( rcRegionsFont[ i ], crColor, LNG_LINE( 2 ) );
					break;

				case EMsgOKCancel:
					m_pSDLFontText->DrawText( rcRegionsFont[ i ], crColor, LNG_LINE( 2 ) );
					break;

				case EMsgYesNo:
				case EMsgYesNoCancel:
					m_pSDLFontText->DrawText( rcRegionsFont[ i ], crColor, LNG_LINE( 4 ) );
					break;
			}
		}
		else if( i == 1 ) { //cancel/nie lekko na czerwono
			crColor = bPtCursorInRect ? RGB( 255, 0, 0 ) : RGB( 255, 50, 50 );

			switch( m_eMsgType ) {
				case EMsgOKCancel:
					m_pSDLFontText->DrawText( rcRegionsFont[ i ], crColor, LNG_LINE( 2 ) );
					break;

				case EMsgYesNo:
				case EMsgYesNoCancel:
					m_pSDLFontText->DrawText( rcRegionsFont[ i ], crColor, LNG_LINE( 5 ) );
					break;
			}
		}
		else { //na bialo
			crColor = bPtCursorInRect ? RGB( 255, 255, 255 ) : RGB( 128, 128, 128 );
			m_pSDLFontText->DrawText( rcRegionsFont[ i ], crColor, LNG_LINE( 3 ) );
		}
	}

	//w jednej linii max 70 znakow, jak za duzo to trzeba przeniesc do nowej linii
	GLint iLength = static_cast< GLint >( _tcslen( m_cText ) );
	if( iLength <= 80 )
		m_pSDLFontText->DrawText( 100, 380, RGB( 100, 100, 255 ), m_cText, GL_TRUE );
	else if( iLength > 80 ) {
		//jezeli dluzszy tekst to rozwalamy go na maksymalnie 3 linijki
		//szukaj spacji i tnij
		TCHAR cFirst[ 128 ];
		TCHAR cSecond[ 128 ];
		GLint j;
		for( i = 70; i >= 0; --i ) {
			if( m_cText[ i ] == ' ')
				break;
		}
		for( j = 0; j < i; ++j )
			cFirst[ j ] = m_cText[ j ];
		cFirst[ j ] = 0;

		if( iLength > 160 ) {
			TCHAR cThird[ 128 ];
			GLint k;
			for( k = 160; k >= 0; --k ) {
				if( m_cText[ k ] == ' ')
					break;
			}

			GLint s = 0;
			for( j = i + 1; j < k; ++j ) {
				cSecond[ s ] = m_cText[ j ];
				++s;
			}
			cSecond[ s ] = 0;

			s = 0;
			for( j = k + 1; j < iLength; ++j ) {
				cThird[ s ] = m_cText[ j ];
				++s;
			}
			cThird[ s ] = 0;

			m_pSDLFontText->DrawText( 100, 350, RGB( 100, 100, 255 ), cFirst, GL_TRUE );
			m_pSDLFontText->DrawText( 100, 380, RGB( 100, 100, 255 ), cSecond, GL_TRUE );
			m_pSDLFontText->DrawText( 100, 410, RGB( 100, 100, 255 ), cThird, GL_TRUE );
		}
		else {
			GLint s = 0;
			for( j = i + 1; j < iLength; ++j ) {
				cSecond[ s ] = m_cText[ j ];	
				++s;
			}
			cSecond[ s ] = 0;

			m_pSDLFontText->DrawText( 100, 350, RGB( 100, 100, 255 ), cFirst, GL_TRUE );
			m_pSDLFontText->DrawText( 100, 380, RGB( 100, 100, 255 ), cSecond, GL_TRUE );
		}
	}
}

GLvoid CRMessageBox::Draw()
{
	if( !m_bDrawMsg ) return;

	glLoadIdentity();
	glDisable( GL_TEXTURE_2D );
	glDisable( GL_LIGHTING );

	//rysujemy przyciski
	DrawRegions( m_rRegionsBkg, 2 );
	//rysujemy ramkê - niby okna
	DrawRegionBorder( m_rRegionsBkg[ 1 ] );

	glColor4ub( 12, 18, 12, 200 );
	switch( m_eMsgType ) {
		case EMsgOK:
			DrawButtons( m_rRegions1Button, 1 );
			DrawFonts( m_rRegions1Button, m_rRegionsFont1Button, 1 );
			break;

		case EMsgOKCancel:
		case EMsgYesNo:
			DrawButtons( m_rRegions2Button, 2 );
			DrawFonts( m_rRegions2Button, m_rRegionsFont2Button, 2 );
			break;

		case EMsgYesNoCancel:
			DrawButtons( m_rRegions3Button, 3 );
			DrawFonts( m_rRegions3Button, m_rRegionsFont3Button, 3 );
			break;
	}
}