#include "stdafx.h"
#include "Collision.h"
#include "Object.h"
#include "Player.h"
#include "Missile.h"

void CCollision::CheckPlayerCollision(float fTimeElapsed)
{
	XMFLOAT3 position = m_pPlayer->GetPosition();

	if (position.x < 0.0f || position.x > 5120.0f || position.z < 0.0f || position.z > 5120.0f)
	{
		m_pPlayer->Collide(nullptr, fTimeElapsed);
	}
	dynamic_cast<CPlayer*>(m_pPlayer)->Decelerate(fTimeElapsed);

	float fTerrainHeight = m_pTerrain->GetHeight(position.x, position.z);
	if (fTerrainHeight + 4.0f >= position.y || position.y < 150.0f)
	{
		m_pPlayer->Collide(m_pTerrain);
	}

	for (auto& enemy : m_pEnemyObjects)
	{
		// 플레이어와 적오브젝트의 충돌 검사
		if (!enemy->GetOOBB().Intersects(m_pPlayer->GetOOBB()))
		{
			continue;
		}
	}
}

void CCollision::CheckPlayerBulletCollisions(float fTimeElapsed)
{
	for (auto& enemy : m_pEnemyObjects)
	{
		for (int i = 0; i < MAX_NUM_MISSILE; ++i)
		{
			if (!m_pPlayerMissiles[i]->GetAlive())
			{
				continue;
			}

			if (!enemy->GetOOBB().Intersects(m_pPlayerMissiles[i]->GetOOBB()))
			{
				continue;
			}

			enemy->SufferDamage(1);
			m_pPlayerMissiles[i]->Collide();
		}
	}
}

void CCollision::CheckEnemyCollisions(float fTimeElapsed)
{
}

void CCollision::CheckEnemyBulletCollisions(float fTimeElapsed)
{
	for (auto& enemyMissile : m_pEnemyMissiles)
	{
		if(!enemyMissile->GetAlive())
		{
			continue;
		}

		if (!enemyMissile->GetOOBB().Intersects(m_pPlayer->GetOOBB()))
		{
			continue;
		}

		m_pPlayer->SufferDamage(1);
		enemyMissile->Collide();
	}
}

void CCollision::Collide(float fTimeElapsed)
{
	if (!m_pPlayer->GetAlive())
	{
		return;
	}

	CheckPlayerCollision(fTimeElapsed);
	CheckPlayerBulletCollisions(fTimeElapsed);
	CheckEnemyCollisions(fTimeElapsed);
	CheckEnemyBulletCollisions(fTimeElapsed);
}

void CCollision::SetTerrain(CHeightMapTerrain* pTerrain)
{
	m_pTerrain = pTerrain;
}

void CCollision::SetPlayer(CGameObject* pPlayer)
{
	m_pPlayer = pPlayer;

	//CHelicopterPlayer* pHelicopterPlayer = dynamic_cast<CHelicopterPlayer*>(m_pPlayer);
	//if (pHelicopterPlayer)
	//{
	for (int i = 0; i < MAX_NUM_MISSILE; ++i)
	{
		m_pPlayerMissiles.reserve(MAX_NUM_MISSILE);
		m_pPlayerMissiles.push_back(pPlayer->GetMissile(i));
	}
	//}
}

void CCollision::SetReserveObjects(int nNum)
{
	m_pEnemyObjects.reserve(nNum);
	m_pEnemyMissiles.reserve(nNum * MAX_NUM_MISSILE);
	m_nEnemy = nNum;
}

void CCollision::SetEnemyObjects(CGameObject** pEnemy, int nNum)
{
	for (int i = 0; i < nNum; ++i)
	{
		m_pEnemyObjects.push_back(pEnemy[i]);
		SetEnemyMissiles(pEnemy[i]);
	}
}

void CCollision::SetEnemyMissiles(CGameObject* pEnemy)
{
	for (int i = 0; i < MAX_NUM_MISSILE; ++i)
	{
		m_pEnemyMissiles.push_back(pEnemy->GetMissile(i));
	}
}