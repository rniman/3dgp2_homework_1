#pragma once
#include "Object.h"

class CMissile : public CGameObject
{
public:
	CMissile();;
	CMissile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature);
	~CMissile();

	void PrepareOOBB() override;
	virtual void Animate(float fTimeElapsed) override;
	virtual void Collide(CGameObject* pCollidedObject = nullptr, float fTimeElapsed = 0.0f) override;

	// interface
	void SetLifeTime(float fLifeTime = 5.0f) { m_fLifeTime = fLifeTime; };
	virtual void SetOOBB() override;

private:
	float m_fLifeTime = 0.0f;
	float m_fSpeed = 10.0f;
};

