#pragma once

class CGameObject;
class CHeightMapTerrain;

class CCollision
{
public:
	CCollision() {};
	~CCollision() {};

	void CheckPlayerCollision(float fTimeElapsed = 0.0f);		// �÷��̾���� �浹 �˻�
	void CheckPlayerBulletCollisions(float fTimeElapsed = 0.0f); // �÷��̾� �Ѿ˰��� �浹 �˻�
	void CheckEnemyCollisions(float fTimeElapsed = 0.0f);		// ���鳢���� �浹 �˻�
	void CheckEnemyBulletCollisions(float fTimeElapsed = 0.0f);	// �� �Ѿ˰��� �浹 �˻�

	void Collide(float fTimeElapsed = 0.0f);

	// interface
	void SetTerrain(CHeightMapTerrain* pTerrain);
	void SetPlayer(CGameObject* pPlayer);
	//void SetPlayerMissiles(CGameObject* pPlayer, int nNum);
	void SetReserveObjects(int nGunshipObjects, int nSuperCobraObjects);
	void SetEnemyObjects(CGameObject** pEnemy, int nGunshipObjects, int nSuperCobraObjects);
	void SetEnemyMissiles(CGameObject* pEnemy);

private:
	CHeightMapTerrain* m_pTerrain = nullptr;

	CGameObject* m_pPlayer = nullptr;
	std::vector<CGameObject*> m_pPlayerMissiles;

	int m_nEnemy= 0;
	int m_nGunshipObjects = 0;
	int m_nSuperCobraObjects = 0;
	std::vector<CGameObject*> m_pEnemyObjects;
	std::vector<CGameObject*> m_pEnemyMissiles;
};

