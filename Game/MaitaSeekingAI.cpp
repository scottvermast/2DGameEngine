#include "MaitaSeekingAI.h"
#include "LevelManager.h"
#include "Maita.h"
#include "BobblePlayer1.h"
#include "utils.h"

MaitaSeekingAI::MaitaSeekingAI(Maita* pMaita)
	: MaitaStateAI(pMaita)
	, m_pLevelManager(LevelManager::GetInstance())
	, m_pBobblePlayer(nullptr)
	, m_BobblePosition({ 0, 0 })
	, m_MaitaPosition(pMaita->GetPosition())
{

}

void MaitaSeekingAI::Update()
{
	GetBobblePosition();
	m_MaitaPosition = m_pMaita->GetPosition();

	if (m_BobblePosition != glm::vec2{ 0, 0 })
	{
		if (utils::Distance(m_MaitaPosition.x, m_BobblePosition.x) < 32)
		{
			if (m_BobblePosition.y < m_MaitaPosition.y)
			{
				if (m_pMaita->GetCanJump())
				{
					m_CommandMap.at("jump")->Execute();
					m_pMaita->SetJump(false);
				}
				m_pMaita->ChangeState("idle");
			}
		}
		else if (m_BobblePosition.x > m_MaitaPosition.x)
		{
			m_CommandMap.at("moveright")->Execute();
		}
		else if (m_BobblePosition.x < m_MaitaPosition.x)
		{
			m_CommandMap.at("moveleft")->Execute();
		}
	}
}

void MaitaSeekingAI::GetBobblePosition()
{
	if (!m_pBobblePlayer)
		m_pBobblePlayer = m_pLevelManager->GetBobblePlayer()->GetGameObject();
	else
	{
		m_BobblePosition = m_pBobblePlayer->GetTransform()->GetPosition();
	}
}

void MaitaSeekingAI::OnEnter()
{
	m_pMaita->SetAnimationClip(0);
}

void MaitaSeekingAI::OnExit()
{

}