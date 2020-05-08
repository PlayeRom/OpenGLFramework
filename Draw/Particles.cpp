
#include "StdAfx.h"
#include "Vector.h"
#include "VertexArrays.h"
#include "TextureLoader.h"
#include "Billboard.h"
#include "../OpenGLFramework/GameControl.h"
#include "../SpeedControl/SpeedControl.h"
#include "Particles.h"

CParticles::CParticles()
{
	m_pGameCtrl = CGameControl::GetInstance();
	m_fSlowDown = 2.0f;
}

CParticles::~CParticles()
{
	for( GLint i = 0; i < GetEmittersSize(); ++i )
		m_aEmitters[ i ].aParticles.clear();

	m_aEmitters.clear();
}

GLint CParticles::CreateEmitter( const SEmitter &in_sEmitter )
{
	return AddAndCreateEmitter( in_sEmitter );
}

GLint CParticles::CreateEmitter( const CVector3 &in_cStartPos,
								 GLuint uiTexture,
								 const CVector3 &in_cDirection,
								 const CVector3 &in_cDivergence,
								 GLfloat fScale,
								 GLfloat fLife,
								 GLboolean bAlive,
								 GLfloat fColorR,
								 GLfloat fColorG,
								 GLfloat fColorB,
								 GLfloat fSpeedX,
								 GLfloat fSpeedY,
								 GLfloat fSpeedZ,
								 GLint iMaxParticles,
								 GLfloat fMoveSpeed /*= 60.0f*/,
								 GLint iRandSpeedRange1 /*= 60*/ )
{
	SEmitter sEmitter;
	sEmitter.cStartPos = in_cStartPos;
	sEmitter.uiTexture = uiTexture;
	sEmitter.cDirection = in_cDirection;
	sEmitter.cDivergence = in_cDivergence;
	sEmitter.fScale = fScale;
	sEmitter.fMaxLife = fLife;
	sEmitter.bIsAlive = bAlive; //czy jak wygasnie to moze ozyc
	sEmitter.fColorR = fColorR;
	sEmitter.fColorG = fColorG;
	sEmitter.fColorB = fColorB;
	sEmitter.fSpeedX = fSpeedX;
	sEmitter.fSpeedY = fSpeedY;
	sEmitter.fSpeedZ = fSpeedZ;
	sEmitter.iMaxParticles = iMaxParticles;
	sEmitter.fMoveSpeed = fMoveSpeed;
	sEmitter.iRandSpeedRange1 = iRandSpeedRange1;
	sEmitter.iRandSpeedRange2 = iRandSpeedRange1 / 2;

	return AddAndCreateEmitter( sEmitter );
}

GLint CParticles::AddAndCreateEmitter( const SEmitter &in_sEmitter )
{
	m_aEmitters.push_back( in_sEmitter );

	GLint iIndex = GetEmittersSize() - 1;
	CreateParticles( iIndex );

	return iIndex; //zwracamy indeks nowego emitera
}

GLvoid CParticles::CreateParticles( GLint iIndex )
{
	if( iIndex < 0 || iIndex >= GetEmittersSize() )
		return;

	m_aEmitters[ iIndex ].aParticles.clear();

	for( GLint i = 0; i < m_aEmitters[ iIndex ].iMaxParticles; ++i ) {
		SParticle sParticle;
		sParticle.cActualPos = m_aEmitters[ iIndex ].cStartPos;

		if( m_aEmitters [ iIndex ].iRandSpeedRange1 == 60 ) { //czyli ustawienie domyslne
			//to dla m_aEmitters[ iIndex ].bIsAlive = false ustaw inne domyslne, tj. 50 i 25
			if( !m_aEmitters[ iIndex ].bIsAlive ) {
				m_aEmitters[ iIndex ].iRandSpeedRange1 = 50;
				m_aEmitters[ iIndex ].iRandSpeedRange2 = m_aEmitters[ iIndex ].iRandSpeedRange1 / 2;
			}
		}
		if( !m_aEmitters[ iIndex ].bIsAlive ) {
			sParticle.cSpeed.x = GLfloat( ( rand() % m_aEmitters[ iIndex ].iRandSpeedRange1 ) - m_aEmitters[ iIndex ].iRandSpeedRange2 ) * 10.0f;
			sParticle.cSpeed.y = GLfloat( ( rand() % m_aEmitters[ iIndex ].iRandSpeedRange1 ) - m_aEmitters[ iIndex ].iRandSpeedRange2 ) * 10.0f;
			sParticle.cSpeed.z = GLfloat( ( rand() % m_aEmitters[ iIndex ].iRandSpeedRange1 ) - m_aEmitters[ iIndex ].iRandSpeedRange2 ) * 10.0f;
		}
		else {
			sParticle.cSpeed.x = m_aEmitters[ iIndex ].fSpeedX + GLfloat( ( rand() % m_aEmitters[ iIndex ].iRandSpeedRange1 - m_aEmitters[ iIndex ].iRandSpeedRange2 ) );
			sParticle.cSpeed.y = m_aEmitters[ iIndex ].fSpeedY + GLfloat( ( rand() % m_aEmitters[ iIndex ].iRandSpeedRange1 - m_aEmitters[ iIndex ].iRandSpeedRange2 ) );
			sParticle.cSpeed.z = m_aEmitters[ iIndex ].fSpeedZ + GLfloat( ( rand() % m_aEmitters[ iIndex ].iRandSpeedRange1 - m_aEmitters[ iIndex ].iRandSpeedRange2 ) );
		}

		sParticle.fLife = m_aEmitters[ iIndex ].fMaxLife;
		sParticle.fFade = GetFade();
		sParticle.fScale = m_aEmitters[ iIndex ].fScale;
		sParticle.fColorR = m_aEmitters[ iIndex ].fColorR;
		sParticle.fColorG = m_aEmitters[ iIndex ].fColorG;
		sParticle.fColorB = m_aEmitters[ iIndex ].fColorB;

		m_aEmitters[ iIndex ].aParticles.push_back( sParticle );
	}
}

GLvoid CParticles::DrawEmitter( GLint iIndex, const CVector3 &cPos /*= CVector3()*/ )
{
	/*
	cPos - umoziwia pozycjonowanie emitera w trakcie dzialania programu, a nie tylko przy
			tworzeniu emitera. Jezeli cPos bedzie = 0.0f ( domyslnie tak jest ) to uzyta
			zostanie pozycja podana	przy tworzeniu emitera.
	*/
	if( iIndex < 0 || iIndex >= GetEmittersSize() )
		return;

	glPushMatrix();

	if( cPos == 0.0f )
		glTranslatef( m_aEmitters[ iIndex ].cStartPos.x,
					  m_aEmitters[ iIndex ].cStartPos.y,
					  m_aEmitters[ iIndex ].cStartPos.z );
	else
		glTranslatef( cPos.x, cPos.y, cPos.z );

	glDisable( GL_LIGHTING );
	glDisable( GL_DEPTH_TEST );
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE );
	m_pGameCtrl->GetTexLoader()->SetTexture( m_aEmitters[ iIndex ].uiTexture );

	for( GLint i = 0; i < m_aEmitters[ iIndex ].iMaxParticles; ++i ) {
		glColor4f( m_aEmitters[ iIndex ].aParticles[ i ].fColorR,
				   m_aEmitters[ iIndex ].aParticles[ i ].fColorG,
				   m_aEmitters[ iIndex ].aParticles[ i ].fColorB,
				   m_aEmitters[ iIndex ].aParticles[ i ].fLife );

		m_pGameCtrl->GetBillboard()->DrawQuadBillboard(	m_aEmitters[ iIndex ].aParticles[ i ].cActualPos,
														m_aEmitters[ iIndex ].fScale );

		GLfloat fSpeedMultiplier = m_aEmitters[ iIndex ].fMoveSpeed/*60.0f*/ * m_pGameCtrl->GetSpeedCtrl()->GetMultiplier();
		m_aEmitters[ iIndex ].aParticles[ i ].cActualPos += ( ( m_aEmitters[ iIndex ].aParticles[ i ].cSpeed * fSpeedMultiplier ) / ( m_fSlowDown * 1000.0f ) );
		m_aEmitters[ iIndex ].aParticles[ i ].fLife -= ( m_aEmitters[ iIndex ].aParticles[ i ].fFade * fSpeedMultiplier );

		if( m_aEmitters[ iIndex ].bIsAlive ) {
			if( m_aEmitters[ iIndex ].aParticles[ i ].fLife < 0.0f )
				SetAliveParticle( iIndex, i );
		}
	}

	glPopMatrix();
}

GLvoid CParticles::SetAliveParticle( GLint iEmitterIndex, GLint iParticleIndex )
{
	if( iEmitterIndex < 0 || iEmitterIndex >= GetEmittersSize() )
		return;

	if( iParticleIndex < 0 || iParticleIndex >= static_cast<GLint>( m_aEmitters[ iEmitterIndex ].aParticles.size() ) )
		return;

	m_aEmitters[ iEmitterIndex ].aParticles[ iParticleIndex ].fLife = m_aEmitters[ iEmitterIndex ].fMaxLife;
	m_aEmitters[ iEmitterIndex ].aParticles[ iParticleIndex ].fFade = GetFade();
	m_aEmitters[ iEmitterIndex ].aParticles[ iParticleIndex ].cActualPos = m_aEmitters[ iEmitterIndex ].cStartPos;
	
	GLint iRange1 = m_aEmitters[ iEmitterIndex ].iRandSpeedRange1;
	GLint iRange2 = m_aEmitters[ iEmitterIndex ].iRandSpeedRange2;

	if( !m_aEmitters[ iEmitterIndex ].bIsAlive ) {
		m_aEmitters[ iEmitterIndex ].aParticles[ iParticleIndex ].cSpeed.x = GLfloat( ( rand() % iRange1 ) - iRange2 ) * 10.0f;
		m_aEmitters[ iEmitterIndex ].aParticles[ iParticleIndex ].cSpeed.y = GLfloat( ( rand() % iRange1 ) - iRange2 ) * 10.0f;
		m_aEmitters[ iEmitterIndex ].aParticles[ iParticleIndex ].cSpeed.z = GLfloat( ( rand() % iRange1 ) - iRange2 ) * 10.0f;
	}
	else {
		m_aEmitters[ iEmitterIndex ].aParticles[ iParticleIndex ].cSpeed.x = m_aEmitters[ iEmitterIndex ].fSpeedX + GLfloat( ( rand() % iRange1 - iRange2 ) );
		m_aEmitters[ iEmitterIndex ].aParticles[ iParticleIndex ].cSpeed.y = m_aEmitters[ iEmitterIndex ].fSpeedY + GLfloat( ( rand() % iRange1 - iRange2 ) );
		m_aEmitters[ iEmitterIndex ].aParticles[ iParticleIndex ].cSpeed.z = m_aEmitters[ iEmitterIndex ].fSpeedZ + GLfloat( ( rand() % iRange1 - iRange2 ) );
	}
}

GLboolean CParticles::IsAliveParticleInEmitter( GLint iIndex )
{
	if( iIndex < 0 || iIndex >= GetEmittersSize() )
		return GL_FALSE;

	for( GLint i = 0; i < static_cast<GLint>( m_aEmitters[ iIndex ].aParticles.size() ); ++i ) {
		if( m_aEmitters[ iIndex ].aParticles[ i ].fLife > 0.0f )
			return GL_TRUE;
	}

	return GL_FALSE;
}

GLvoid CParticles::SetParticleInEmitterAsLive( GLint iIndex )
{
	if( iIndex < 0 || iIndex >= GetEmittersSize() )
		return;

	for( GLint i = 0; i < static_cast<GLint>( m_aEmitters[ iIndex ].aParticles.size() ); ++i )
		SetAliveParticle( iIndex, i );
}

GLvoid CParticles::DeleteEmitter( GLint iIndex )
{
	if( iIndex < 0 || iIndex >= GetEmittersSize() )
		return;

	m_aEmitters[ iIndex ].aParticles.clear();
	m_aEmitters.erase( m_aEmitters.begin() + iIndex );
}

GLvoid CParticles::SetMaxParticlesForEmitter( GLint iIndex, GLint iMaxParticles )
{
	if( iIndex < 0 || iIndex >= GetEmittersSize() )
		return;

	if( m_aEmitters[ iIndex ].iMaxParticles == iMaxParticles )
		return;

	if( m_aEmitters[ iIndex ].iMaxParticles < iMaxParticles ) {
		AddParticles( iIndex, iMaxParticles );
		m_aEmitters[ iIndex ].iMaxParticles = iMaxParticles;
	}
	else
		m_aEmitters[ iIndex ].iMaxParticles = iMaxParticles;
}

GLvoid CParticles::AddParticles( GLint iIndex, GLint iMaxParticles )
{
	//dokladamy wicej czastek, nie ruszajac istniejacych

	if( iIndex < 0 || iIndex >= GetEmittersSize() )
		return;

	for( GLint i = m_aEmitters[ iIndex ].iMaxParticles - 1; i < iMaxParticles; ++i ) {
		SParticle sParticle;
		sParticle.cActualPos = m_aEmitters[ iIndex ].cStartPos;
		
		if( m_aEmitters[ iIndex ].iRandSpeedRange1 == 60 ) { //czyli ustawienie domyslne
			//to dla m_aEmitters[ iIndex ].bIsAlive = false ustaw inne domyslne, tj. 50 i 25
			if( !m_aEmitters[ iIndex ].bIsAlive ) {
				m_aEmitters[ iIndex ].iRandSpeedRange1 = 50;
				m_aEmitters[ iIndex ].iRandSpeedRange2 = m_aEmitters[ iIndex ].iRandSpeedRange1 / 2;
			}
		}
		if( !m_aEmitters[ iIndex ].bIsAlive ) {
			sParticle.cSpeed.x = GLfloat( ( rand() % m_aEmitters[ iIndex ].iRandSpeedRange1 ) - m_aEmitters[ iIndex ].iRandSpeedRange2 ) * 10.0f;
			sParticle.cSpeed.y = GLfloat( ( rand() % m_aEmitters[ iIndex ].iRandSpeedRange1 ) - m_aEmitters[ iIndex ].iRandSpeedRange2 ) * 10.0f;
			sParticle.cSpeed.z = GLfloat( ( rand() % m_aEmitters[ iIndex ].iRandSpeedRange1 ) - m_aEmitters[ iIndex ].iRandSpeedRange2 ) * 10.0f;
		}
		else {
			sParticle.cSpeed.x = m_aEmitters[ iIndex ].fSpeedX + GLfloat( ( rand() % m_aEmitters[ iIndex ].iRandSpeedRange1 - m_aEmitters[ iIndex ].iRandSpeedRange2 ) );
			sParticle.cSpeed.y = m_aEmitters[ iIndex ].fSpeedY + GLfloat( ( rand() % m_aEmitters[ iIndex ].iRandSpeedRange1 - m_aEmitters[ iIndex ].iRandSpeedRange2 ) );
			sParticle.cSpeed.z = m_aEmitters[ iIndex ].fSpeedZ + GLfloat( ( rand() % m_aEmitters[ iIndex ].iRandSpeedRange1 - m_aEmitters[ iIndex ].iRandSpeedRange2 ) );
		}

		sParticle.fLife = m_aEmitters[ iIndex ].fMaxLife;
		sParticle.fFade = GetFade();
		sParticle.fScale = m_aEmitters[ iIndex ].fScale;
		sParticle.fColorR = m_aEmitters[ iIndex ].fColorR;
		sParticle.fColorG = m_aEmitters[ iIndex ].fColorG;
		sParticle.fColorB = m_aEmitters[ iIndex ].fColorB;

		m_aEmitters[ iIndex ].aParticles.push_back( sParticle );
	}
}

GLint CParticles::GetMaxParticlesForEmitter( GLint iIndex )
{
	if( iIndex < 0 || iIndex >= GetEmittersSize() )
		return -1;

	return m_aEmitters[ iIndex ].iMaxParticles;
}

GLvoid CParticles::DeleteParticle( GLint iIndex, GLint iIndexParticle )
{
	if( iIndex < 0 || iIndex >= GetEmittersSize() )
		return;

	if( iIndexParticle < 0 || iIndexParticle >= GetMaxParticlesForEmitter( iIndex ) )
		return;

	m_aEmitters[ iIndex ].aParticles.erase( m_aEmitters[ iIndex ].aParticles.begin() + iIndexParticle );
	--m_aEmitters[ iIndex ].iMaxParticles;
}