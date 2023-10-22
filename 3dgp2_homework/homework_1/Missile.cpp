#include "stdafx.h"
#include "Missile.h"

CMissile::CMissile() 
	: CGameObject(0, 0) 
{
	m_bAlive = false;
}

CMissile::CMissile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature)
	: CGameObject(0, 0)
{
	m_bAlive = false;
}

CMissile::~CMissile()
{
}
