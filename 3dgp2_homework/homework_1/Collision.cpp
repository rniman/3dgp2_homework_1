#include "stdafx.h"
#include "Collision.h"
#include "Object.h"
#include "Player.h"
#include "Missile.h"

void CCollision::CheckPlayerCollision(float fTimeElapsed)
{
	XMFLOAT3 position = m_pPlayer->GetPosition();

	if (position.y > 2500.0f)
	{
		XMFLOAT3 xmf3Velocity = dynamic_cast<CPlayer*>(m_pPlayer)->GetVelocity();
		xmf3Velocity.x = 0.0f;
		xmf3Velocity.z = 0.0f;
		XMFLOAT3 xmf3Shift = Vector3::ScalarProduct(xmf3Velocity, -fTimeElapsed, false);
		m_pPlayer->Move(xmf3Shift, false);
	}

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

			if (dynamic_cast<CMissile*>(m_pPlayerMissiles[i])->GetExplosionTime() > 0.0f)
			{
				continue;
			}

			if (!enemy->GetOOBB().Intersects(m_pPlayerMissiles[i]->GetOOBB()))
			{
				continue;
			}
			
			dynamic_cast<CHelicopterPlayer*>(m_pPlayer)->AddScore(10);
			enemy->SufferDamage(1);
			m_pPlayerMissiles[i]->Collide();
		}
	}
}

void CCollision::CheckEnemyCollisions(float fTimeElapsed)
{
	//for (int i = 0; i < m_nGunshipObjects; ++i)
	//{

	//}

	for (int i = m_nGunshipObjects; i < m_nGunshipObjects + m_nSuperCobraObjects; ++i)
	{
		if (!m_pEnemyObjects[i]->GetAlive())
		{
			continue;
		}

		if (!m_pEnemyObjects[i]->GetOOBB().Intersects(m_pPlayer->GetOOBB()))
		{
			continue;
		}

		m_pPlayer->SufferDamage(3);
		m_pEnemyObjects[i]->Collide(m_pPlayer, fTimeElapsed);		
	}
}

void CCollision::CheckEnemyBulletCollisions(float fTimeElapsed)
{
	for (auto& enemyMissile : m_pEnemyMissiles)
	{
		if(!enemyMissile->GetAlive())
		{
			continue;
		}

		if (dynamic_cast<CMissile*>(enemyMissile)->GetExplosionTime() > 0.0f)
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

void CCollision::SetReserveObjects(int nGunshipObjects, int nSuperCobraObjects)
{
	m_pEnemyObjects.reserve(nGunshipObjects + nSuperCobraObjects);
	m_pEnemyMissiles.reserve(nGunshipObjects * MAX_NUM_MISSILE);
	m_nEnemy = nGunshipObjects + nSuperCobraObjects;
}

void CCollision::SetEnemyObjects(CGameObject** pEnemy, int nGunshipObjects, int nSuperCobraObjects)
{
	m_nGunshipObjects = nGunshipObjects;
	m_nSuperCobraObjects = nSuperCobraObjects;
	for (int i = 0; i < nGunshipObjects; ++i)
	{
		m_pEnemyObjects.push_back(pEnemy[i]);
		SetEnemyMissiles(pEnemy[i]);
	}

	for (int i = nGunshipObjects; i < nGunshipObjects + nSuperCobraObjects; ++i)
	{
		m_pEnemyObjects.push_back(pEnemy[i]);
	}
}

void CCollision::SetEnemyMissiles(CGameObject* pEnemy)
{
	for (int i = 0; i < MAX_NUM_MISSILE; ++i)
	{
		m_pEnemyMissiles.push_back(pEnemy->GetMissile(i));
	}
}