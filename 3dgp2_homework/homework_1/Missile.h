#pragma once
#include "Object.h"


class CMissile : public CGameObject
{
public:
	CMissile();;
	CMissile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature);
	~CMissile();

private:

};

