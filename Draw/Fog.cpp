
#include "StdAfx.h"
#include "Fog.h"

CFog::CFog()
{
	m_aColor[ 0 ] = 1.0f;
	m_aColor[ 1 ] = 1.0f;
	m_aColor[ 2 ] = 1.0f;
	m_aColor[ 3 ] = 1.0f;
	SetColor( m_aColor );

	SetHint( GL_NICEST );
}

CFog::~CFog()
{
}
