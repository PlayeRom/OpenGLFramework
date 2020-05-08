
#include "StdAfx.h"
#include "../OpenGLFramework/WindowData.h"
#include "../OpenGLFramework/GameControl.h"
#include "../OpenGLFramework/Console.h"
#include "VolumeOutMaster.h"
#include "MasterVolume.h"

CMasterVolume* CMasterVolume::Construct()
{
	return CSingletonBase::Construct( new CMasterVolume );
}

CMasterVolume::CMasterVolume()
{
	// Volume control Initialization
	m_pMasterVolume = ( IVolume* )new CVolumeOutMaster( CGameControl::GetInstance()->GetWinData()->GetHWND() );
	if( !m_pMasterVolume || !m_pMasterVolume->IsAvailable() ) {
		__LOG( _T("ERROR: Create VolumeOutMaster failed.") );
	}
	m_pMasterVolume->Enable();

	m_uiOryginalVolume = m_pMasterVolume->GetCurrentVolume();
}

CMasterVolume::~CMasterVolume()
{
	m_pMasterVolume->SetCurrentVolume( m_uiOryginalVolume ); //przywracamy oryginalna glosnosc
	delete m_pMasterVolume;
	m_pMasterVolume = NULL;
}

GLvoid CMasterVolume::SetPercentVolume( GLuint uiPercent /*= 50*/ )
{
	if( uiPercent > 100 )
		uiPercent = 100;

	if( uiPercent < 0 )
		uiPercent = 0;

	GLuint uiMin = m_pMasterVolume->GetMinimalVolume();
	GLuint uiMax = m_pMasterVolume->GetMaximalVolume();

	GLuint uiRange = uiMax - uiMin;

	GLuint uiVolume = ( ( uiPercent * uiRange ) / 100 ) + uiMin;

	m_pMasterVolume->SetCurrentVolume( uiVolume );
}

GLuint CMasterVolume::GetPercentVolume()
{
	GLuint uiMin = m_pMasterVolume->GetMinimalVolume();
	GLuint uiMax = m_pMasterVolume->GetMaximalVolume();

	GLuint uiCurrentVolume = m_pMasterVolume->GetCurrentVolume() - uiMin;

	GLuint uiRange = uiMax - uiMin;

	return ( ( uiCurrentVolume * 100 ) / uiRange );
}

GLvoid CMasterVolume::IncreasePercentVolume( GLuint uiPercent /*= 10*/ )
{
	GLuint uiSetPercent = GetPercentVolume() + uiPercent;
	SetPercentVolume( uiSetPercent );
}

GLvoid CMasterVolume::DecreasePercentVolume( GLuint uiPercent /*= 10*/ )
{
	GLuint uiSetPercent = GetPercentVolume() - uiPercent;
	SetPercentVolume( uiSetPercent );
}

GLvoid CMasterVolume::SetVolume( GLuint uiVolume )
{
	m_pMasterVolume->SetCurrentVolume( uiVolume );
}

GLuint CMasterVolume::GetVolume()
{
	return m_pMasterVolume->GetCurrentVolume();
}

GLvoid CMasterVolume::IncreaseVolume( GLuint uiVolume /*= 4096*/ )
{
	GLuint uiCurrentVolume = m_pMasterVolume->GetCurrentVolume();
	m_pMasterVolume->SetCurrentVolume( uiCurrentVolume + uiVolume );
}

GLvoid CMasterVolume::DecreaseVolume( GLuint uiVolume /*= 4096*/ )
{
	GLuint uiCurrentVolume = m_pMasterVolume->GetCurrentVolume();
	m_pMasterVolume->SetCurrentVolume( uiCurrentVolume - uiVolume );
}