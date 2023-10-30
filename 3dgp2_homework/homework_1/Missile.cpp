#include "stdafx.h"
#include "Missile.h"

CMissile::CMissile() 
	: CGameObject(0, 0) 
{
	m_bAlive = false;
	m_fLifeTime = -1.0f;
}

CMissile::CMissile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature)
	: CGameObject(0, 0)
{
	m_bAlive = false;
	m_fLifeTime = -1.0f;
}

CMissile::~CMissile()
{
}

CGameObject* CMissile::BuildExplosion(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, CMesh* pMesh, CTexture* pTexture)
{
	m_pExplisionAnimation = new CSpriteObject(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, 1, 1, 8, 8);

	CMaterial* pMaterial = new CMaterial();
	pMaterial->SetTexture(pTexture);
	m_pExplisionAnimation->SetMaterial(0, pMaterial);
	m_pExplisionAnimation->SetMesh(0, pMesh);
	m_pExplisionAnimation->SetAlive(false);
	m_pExplisionAnimation->SetSpeed(0.1f);

	return m_pExplisionAnimation;
}

void CMissile::Animate(float fTimeElapsed)
{
	if (m_fLifeTime <= 0.0f)
	{
		m_pExplisionAnimation->SetAlive(true);
		XMFLOAT4X4 xmf4x4transform = Matrix4x4::Identity();
		xmf4x4transform = Matrix4x4::Multiply(xmf4x4transform, m_xmf4x4Local);
		m_pExplisionAnimation->SetLocalTransform(xmf4x4transform);
		m_fExplosionTime += fTimeElapsed;

		if(m_fExplosionTime > 0.8f)
		{
			m_fExplosionTime = 0.0f;
			m_bAlive = false;		
			m_pExplisionAnimation->SetAlive(false);
			m_pExplisionAnimation->GetMaterial(0)->m_pTexture->SetColumnAndRow(0, 0);
			m_pExplisionAnimation->SetTime(0.0f);
		}

		return;
	}
	else
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

void CMissile::Collide(CGameObject* pCollidedObject, float fTimeElapsed)
{
	m_fLifeTime = -1.0f;
	//m_bAlive = false;
}

void CMissile::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera)
{
	if (m_fLifeTime <= 0.0f)
	{
		return;
	}

	CGameObject::Render(pd3dCommandList, pCamera);
}

