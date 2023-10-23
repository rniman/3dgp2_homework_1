#include "stdafx.h"
#include "Missile.h"

CMissile::CMissile() 
	: CGameObject(0, 0) 
{

	m_bAlive = false;
	m_fLifeTime = 0.0f;
}

CMissile::CMissile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature)
	: CGameObject(0, 0)
{
	m_bAlive = false;
	m_fLifeTime = 0.0f;
}

CMissile::~CMissile()
{
}

void CMissile::Animate(float fTimeElapsed)
{
	if (m_fLifeTime <= 0.0f)
	{
		m_bAlive = false;
		return;
	}

	m_fLifeTime -= fTimeElapsed;
	XMFLOAT3 xmf3Forward =  XMFLOAT3(m_xmf4x4Local._31, m_xmf4x4Local._32, m_xmf4x4Local._33);
	XMStoreFloat3(&xmf3Forward, XMVector3Normalize(XMLoadFloat3(&xmf3Forward)) * m_fSpeed * fTimeElapsed);

	XMMATRIX translation = XMMatrixTranslation(xmf3Forward.x * m_fSpeed, xmf3Forward.y * m_fSpeed, xmf3Forward.z * m_fSpeed);
	
	m_xmf4x4Local = Matrix4x4::Multiply(m_xmf4x4Local, translation);

	SetOOBB();
}

void CMissile::PrepareOOBB()
{
	// 미사일 바운딩 박스 가져옴
	m_OOBB = m_pChild->GetMesh(0)->GetOOBB();
}

void CMissile::SetOOBB()
{
	m_pChild->GetMesh(0)->GetOOBB().Transform(m_OOBB, XMLoadFloat4x4(&m_xmf4x4Local));
	XMStoreFloat4(&m_OOBB.Orientation, XMQuaternionNormalize(XMLoadFloat4(&m_OOBB.Orientation)));
}

