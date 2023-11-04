//-----------------------------------------------------------------------------
// File: CPlayer.cpp
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "Player.h"
#include "Shader.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CPlayer

CPlayer::CPlayer()
{
	m_pCamera = nullptr;

	m_xmf3Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_xmf3Right = XMFLOAT3(1.0f, 0.0f, 0.0f);
	m_xmf3Up = XMFLOAT3(0.0f, 1.0f, 0.0f);
	m_xmf3Look = XMFLOAT3(0.0f, 0.0f, 1.0f);

	m_xmf3Velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_xmf3Gravity = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_fMaxVelocityXZ = 0.0f;
	m_fMaxVelocityY = 0.0f;
	m_fFriction = 0.0f;

	m_fPitch = 0.0f;
	m_fRoll = 0.0f;
	m_fYaw = 0.0f;

	m_pPlayerUpdatedContext = nullptr;
	m_pCameraUpdatedContext = nullptr;
}

CPlayer::~CPlayer()
{
	ReleaseShaderVariables();

	if (m_pCamera) delete m_pCamera;

	if (m_pShader) m_pShader->Release();
}

void CPlayer::CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList)
{
	if (m_pCamera) m_pCamera->CreateShaderVariables(pd3dDevice, pd3dCommandList);
}

void CPlayer::UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList)
{
	CGameObject::UpdateShaderVariable(pd3dCommandList);
}

void CPlayer::ReleaseShaderVariables()
{
	if (m_pCamera) m_pCamera->ReleaseShaderVariables();
}

void CPlayer::Move(const XMFLOAT3& xmf3Shift, bool bUpdateVelocity)
{
	if (bUpdateVelocity)
	{
		m_xmf3Velocity = Vector3::Add(m_xmf3Velocity, xmf3Shift);
	}
	else
	{
		m_xmf3Position = Vector3::Add(m_xmf3Position, xmf3Shift);
		m_pCamera->Move(xmf3Shift);
	}
}

void CPlayer::Rotate(float x, float y, float z)
{
	DWORD nCurrentCameraMode = m_pCamera->GetMode();
	if ((nCurrentCameraMode == FIRST_PERSON_CAMERA) || (nCurrentCameraMode == THIRD_PERSON_CAMERA))
	{
		if (x != 0.0f)
		{
			m_fPitch += x;
			if (m_fPitch > +89.0f) { x -= (m_fPitch - 89.0f); m_fPitch = +89.0f; }
			if (m_fPitch < -89.0f) { x -= (m_fPitch + 89.0f); m_fPitch = -89.0f; }
		}
		if (y != 0.0f)
		{
			m_fYaw += y;
			if (m_fYaw > 360.0f) m_fYaw -= 360.0f;
			if (m_fYaw < 0.0f) m_fYaw += 360.0f;
		}
		if (z != 0.0f)
		{
			m_fRoll += z;
			if (m_fRoll > +20.0f) { z -= (m_fRoll - 20.0f); m_fRoll = +20.0f; }
			if (m_fRoll < -20.0f) { z -= (m_fRoll + 20.0f); m_fRoll = -20.0f; }
		}
		m_pCamera->Rotate(x, y, z);
		if (y != 0.0f)
		{
			XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_xmf3Up), XMConvertToRadians(y));
			m_xmf3Look = Vector3::TransformNormal(m_xmf3Look, xmmtxRotate);
			m_xmf3Right = Vector3::TransformNormal(m_xmf3Right, xmmtxRotate);
		}
	}
	else if (nCurrentCameraMode == SPACESHIP_CAMERA)
	{
		m_pCamera->Rotate(x, y, z);
		if (x != 0.0f)
		{
			XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_xmf3Right), XMConvertToRadians(x));
			m_xmf3Look = Vector3::TransformNormal(m_xmf3Look, xmmtxRotate);
			m_xmf3Up = Vector3::TransformNormal(m_xmf3Up, xmmtxRotate);
		}
		if (y != 0.0f)
		{
			XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_xmf3Up), XMConvertToRadians(y));
			m_xmf3Look = Vector3::TransformNormal(m_xmf3Look, xmmtxRotate);
			m_xmf3Right = Vector3::TransformNormal(m_xmf3Right, xmmtxRotate);
		}
		if (z != 0.0f)
		{
			XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_xmf3Look), XMConvertToRadians(z));
			m_xmf3Up = Vector3::TransformNormal(m_xmf3Up, xmmtxRotate);
			m_xmf3Right = Vector3::TransformNormal(m_xmf3Right, xmmtxRotate);
		}
	}

	m_xmf3Look = Vector3::Normalize(m_xmf3Look);
	m_xmf3Right = Vector3::CrossProduct(m_xmf3Up, m_xmf3Look, true);
	m_xmf3Up = Vector3::CrossProduct(m_xmf3Look, m_xmf3Right, true);
}

void CPlayer::Decelerate(float fTimeElapsed)
{
	float fLength = Vector3::Length(m_xmf3Velocity);
	float fDeceleration = (m_fFriction * fTimeElapsed);
	if (fDeceleration > fLength) fDeceleration = fLength;

	m_xmf3Velocity = Vector3::Add(m_xmf3Velocity, Vector3::ScalarProduct(m_xmf3Velocity, -fDeceleration, true));
}

void CPlayer::Update(float fTimeElapsed)
{
	m_xmf3Velocity = Vector3::Add(m_xmf3Velocity, m_xmf3Gravity);
	float fLength = sqrtf(m_xmf3Velocity.x * m_xmf3Velocity.x + m_xmf3Velocity.z * m_xmf3Velocity.z);
	float fMaxVelocityXZ = m_fMaxVelocityXZ;
	if (fLength > m_fMaxVelocityXZ)
	{
		m_xmf3Velocity.x *= (fMaxVelocityXZ / fLength);
		m_xmf3Velocity.z *= (fMaxVelocityXZ / fLength);
	}
	float fMaxVelocityY = m_fMaxVelocityY;
	fLength = sqrtf(m_xmf3Velocity.y * m_xmf3Velocity.y);
	if (fLength > m_fMaxVelocityY) m_xmf3Velocity.y *= (fMaxVelocityY / fLength);

	XMFLOAT3 xmf3Velocity = Vector3::ScalarProduct(m_xmf3Velocity, fTimeElapsed, false);
	Move(xmf3Velocity, false);

	if (m_pPlayerUpdatedContext) OnPlayerUpdateCallback(fTimeElapsed);

	DWORD nCurrentCameraMode = m_pCamera->GetMode();
	if (nCurrentCameraMode == THIRD_PERSON_CAMERA) m_pCamera->Update(m_xmf3Position, fTimeElapsed);
	if (m_pCameraUpdatedContext) OnCameraUpdateCallback(fTimeElapsed);
	if (nCurrentCameraMode == THIRD_PERSON_CAMERA) m_pCamera->SetLookAt(m_xmf3Position);
	m_pCamera->RegenerateViewMatrix();

	//Decelerate(fTimeElapsed);
}

CCamera *CPlayer::OnChangeCamera(DWORD nNewCameraMode, DWORD nCurrentCameraMode)
{
	CCamera *pNewCamera = nullptr;
	switch (nNewCameraMode)
	{
		case FIRST_PERSON_CAMERA:
			pNewCamera = new CFirstPersonCamera(m_pCamera);
			break;
		case THIRD_PERSON_CAMERA:
			pNewCamera = new CThirdPersonCamera(m_pCamera);
			break;
		case SPACESHIP_CAMERA:
			pNewCamera = new CSpaceShipCamera(m_pCamera);
			break;
	}
	if (nCurrentCameraMode == SPACESHIP_CAMERA)
	{
		m_xmf3Right = Vector3::Normalize(XMFLOAT3(m_xmf3Right.x, 0.0f, m_xmf3Right.z));
		m_xmf3Up = Vector3::Normalize(XMFLOAT3(0.0f, 1.0f, 0.0f));
		m_xmf3Look = Vector3::Normalize(XMFLOAT3(m_xmf3Look.x, 0.0f, m_xmf3Look.z));

		m_fPitch = 0.0f;
		m_fRoll = 0.0f;
		m_fYaw = Vector3::Angle(XMFLOAT3(0.0f, 0.0f, 1.0f), m_xmf3Look);
		if (m_xmf3Look.x < 0.0f) m_fYaw = -m_fYaw;
	}
	else if ((nNewCameraMode == SPACESHIP_CAMERA) && m_pCamera)
	{
		m_xmf3Right = m_pCamera->GetRightVector();
		m_xmf3Up = m_pCamera->GetUpVector();
		m_xmf3Look = m_pCamera->GetLookVector();
	}

	if (pNewCamera)
	{
		pNewCamera->SetMode(nNewCameraMode);
		pNewCamera->SetPlayer(this);
	}

	if (m_pCamera) delete m_pCamera;

	return(pNewCamera);
}

void CPlayer::Animate(float fTimeElapsed)
{
	if (!m_bAlive)
	{
		return;
	}

	CGameObject::Animate(fTimeElapsed);
}

void CPlayer::OnPrepareRender()
{
	CGameObject::OnPrepareRender();
}

void CPlayer::Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera)
{
	DWORD nCameraMode = (pCamera) ? pCamera->GetMode() : 0x00;
	if (nCameraMode == THIRD_PERSON_CAMERA)
	{
		if (m_pShader) m_pShader->Render(pd3dCommandList, pCamera, 0);
		UpdateTransform();
		CGameObject::Render(pd3dCommandList, pCamera);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CHelicopterPlayer::CHelicopterPlayer(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature)
{
	m_pCamera = ChangeCamera(THIRD_PERSON_CAMERA, 0.0f);

	m_pShader = new CPlayerShader();
	m_pShader->CreateShader(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature);

	CGameObject *pGameObject = CGameObject::LoadGeometryFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/Mi24.bin", m_pShader);
	SetChild(pGameObject);

	CGameObject* pMissileModel = CGameObject::LoadGeometryFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/hellfire.bin", m_pShader);
	
	m_pMissileObject = new CMissile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature);
	m_pMissileObject->SetChild(pMissileModel);
	pMissileModel->AddRef();
	
	for (auto& missile : m_arraypMissile)
	{
		missile = new CMissile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature);
		missile->SetChild(pMissileModel);
		missile->PrepareOOBB();
		//missile->SetOOBB();
		pMissileModel->AddRef();
	}

	PrepareOOBB();

	PrepareAnimate();

	CreateShaderVariables(pd3dDevice, pd3dCommandList);
}

CHelicopterPlayer::~CHelicopterPlayer()
{
}

void CHelicopterPlayer::UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList)
{
	CPlayer::UpdateShaderVariables(pd3dCommandList);

	m_pMissileObject->UpdateShaderVariable(pd3dCommandList);

	for (auto& missile : m_arraypMissile)
	{
		if (!missile->GetAlive())
			continue;

		missile->UpdateShaderVariable(pd3dCommandList);
	}
}

void CHelicopterPlayer::PrepareOOBB()
{
	m_pMainBodyFrame = FindFrame("Mi24");
	SetOOBB();
}

void CHelicopterPlayer::PrepareAnimate()
{
	m_pMainRotorFrame = FindFrame("Top_Rotor");
	m_pTailRotorFrame = FindFrame("Tail_Rotor");
}

void CHelicopterPlayer::Animate(float fTimeElapsed)
{
	if (m_bAlive)
	{
		m_fAliveTime += fTimeElapsed;
		if (m_fAliveTime > 1.0f)
		{
			m_nScore += 1;
			m_fAliveTime = 0.0f;
		}
	}

	//if (!m_bAlive)
	//{
	//	int i = 0;
	//	for (auto& explosion : m_ppExplosionAnimation)
	//	{
	//		explosion->SetAlive(true);
	//		XMFLOAT4X4 xmf4x4transform = Matrix4x4::Identity();
	//		if(i == 0)	xmf4x4transform = Matrix4x4::Multiply(xmf4x4transform, XMMatrixTranslation(-3.0f, -4.0f, -4.0f));
	//		else if(i == 1)	xmf4x4transform = Matrix4x4::Multiply(xmf4x4transform, XMMatrixTranslation(3.0f, 1.0f, -5.0f));
	//		else if(i == 2)	xmf4x4transform = Matrix4x4::Multiply(xmf4x4transform, XMMatrixTranslation(-1.0f, 5.0f, 0.0f));
	//		xmf4x4transform = Matrix4x4::Multiply(xmf4x4transform, m_xmf4x4Local);
	//		explosion->SetLocalTransform(xmf4x4transform);
	//		++i;
	//	}
	//	//return;
	//}

	if (m_fCoolTime > 0.0f)
	{
		m_fCoolTime -= fTimeElapsed;
	}

	m_xmf4x4Local._11 = m_xmf3Right.x; m_xmf4x4Local._12 = m_xmf3Right.y; m_xmf4x4Local._13 = m_xmf3Right.z;
	m_xmf4x4Local._21 = m_xmf3Up.x; m_xmf4x4Local._22 = m_xmf3Up.y; m_xmf4x4Local._23 = m_xmf3Up.z;
	m_xmf4x4Local._31 = m_xmf3Look.x; m_xmf4x4Local._32 = m_xmf3Look.y; m_xmf4x4Local._33 = m_xmf3Look.z;
	m_xmf4x4Local._41 = m_xmf3Position.x; m_xmf4x4Local._42 = m_xmf3Position.y; m_xmf4x4Local._43 = m_xmf3Position.z;

	if (m_pMainRotorFrame)
	{
		XMMATRIX xmmtxRotate = XMMatrixRotationY(XMConvertToRadians(360.0f * 2.0f) * fTimeElapsed);
		m_pMainRotorFrame->m_xmf4x4Local = Matrix4x4::Multiply(xmmtxRotate, m_pMainRotorFrame->m_xmf4x4Local);
	}
	if (m_pTailRotorFrame)
	{
		XMMATRIX xmmtxRotate = XMMatrixRotationX(XMConvertToRadians(360.0f * 4.0f) * fTimeElapsed);
		m_pTailRotorFrame->m_xmf4x4Local = Matrix4x4::Multiply(xmmtxRotate, m_pTailRotorFrame->m_xmf4x4Local);
	}
	
	SetOOBB();

	XMFLOAT3 missilePos = GetPosition();
	missilePos.x = 0.9970461f;
	missilePos.y = -3.221008f;
	missilePos.z = 10.85722f;

	XMFLOAT4X4 matrix = Matrix4x4::Identity();
	XMStoreFloat4x4(&matrix, XMMatrixTranslation(missilePos.x, missilePos.y, missilePos.z));

	m_pMissileObject->SetLocalTransform(matrix);

	for (auto& missile : m_arraypMissile)
	{
		if (!missile->GetAlive())
			continue;

		missile->Animate(fTimeElapsed);
	}

	if (!m_bAlive)
	{
		int i = 0;
		for (auto& explosion : m_ppExplosionAnimation)
		{
			explosion->SetAlive(true);
			XMFLOAT4X4 xmf4x4transform = Matrix4x4::Identity();
			if (i == 0)	xmf4x4transform = Matrix4x4::Multiply(xmf4x4transform, XMMatrixTranslation(-3.0f, -3.0f, -4.0f));
			else if (i == 1)	xmf4x4transform = Matrix4x4::Multiply(xmf4x4transform, XMMatrixTranslation(2.5f, 1.0f, -5.0f));
			else if (i == 2)	xmf4x4transform = Matrix4x4::Multiply(xmf4x4transform, XMMatrixTranslation(-1.0f, 4.0f, 0.0f));
			xmf4x4transform = Matrix4x4::Multiply(xmf4x4transform, m_xmf4x4Local);
			explosion->SetLocalTransform(xmf4x4transform);
			++i;
		}
		//return;
	}
}

void CHelicopterPlayer::Collide(CGameObject* pCollidedObject, float fTimeElapsed)
{
	if (!m_bAlive)
	{
		return;
	}

	if (pCollidedObject == nullptr)	// 맵 밖으로 나간 것
	{
		// 후진 중이었으면
		if (Vector3::DotProduct(m_xmf3Look, m_xmf3Velocity) < 0.0f)
		{
			XMFLOAT3 xmf3Shift = Vector3::ScalarProduct(m_xmf3Velocity, -fTimeElapsed, false);
			Move(xmf3Shift, false);
		}
		else if((m_xmf3Position.x > 5120.0f && m_xmf3Look.z < 0.0f) || (m_xmf3Position.x < 0.0f && m_xmf3Look.z >= 0.0f))
		{
			// 시계 회전
			XMMATRIX rotate = XMMatrixRotationY(XMConvertToRadians(45.0f));
			XMFLOAT4X4 xmf4x4Rotate;
			XMStoreFloat4x4(&xmf4x4Rotate, rotate);
			Rotate(0.0f, 45.0f, 0.0f);
			m_xmf3Velocity = Vector3::TransformCoord(m_xmf3Velocity, xmf4x4Rotate);
			Move(m_xmf3Look, false);
		}
		else if ((m_xmf3Position.x > 5120.0f && m_xmf3Look.z >= 0.0f) || (m_xmf3Position.x < 0.0f && m_xmf3Look.z < 0.0f))
		{
			// 반 시계 회전
			XMMATRIX rotate = XMMatrixRotationY(XMConvertToRadians(-45.0f));
			XMFLOAT4X4 xmf4x4Rotate;
			XMStoreFloat4x4(&xmf4x4Rotate, rotate);
			Rotate(0.0f, -45.0f, 0.0f);
			m_xmf3Velocity = Vector3::TransformCoord(m_xmf3Velocity, xmf4x4Rotate);
			Move(m_xmf3Look, false);
		}
		else if ((m_xmf3Position.z > 5120.0f && m_xmf3Look.x >= 0.0f) || (m_xmf3Position.z < 0.0f && m_xmf3Look.x < 0.0f))
		{
			// 시계 회전
			XMMATRIX rotate = XMMatrixRotationY(XMConvertToRadians(45.0f));
			XMFLOAT4X4 xmf4x4Rotate;
			XMStoreFloat4x4(&xmf4x4Rotate, rotate);
			Rotate(0.0f, 45.0f, 0.0f);
			m_xmf3Velocity = Vector3::TransformCoord(m_xmf3Velocity, xmf4x4Rotate);
			Move(m_xmf3Look, false);
		}
		else if ((m_xmf3Position.z > 5120.0f && m_xmf3Look.x < 0.0f) || (m_xmf3Position.z < 0.0f && m_xmf3Look.x >= 0.0f))
		{
			// 반 시계 회전
			XMMATRIX rotate = XMMatrixRotationY(XMConvertToRadians(-45.0f));
			XMFLOAT4X4 xmf4x4Rotate;
			XMStoreFloat4x4(&xmf4x4Rotate, rotate);
			Rotate(0.0f, -45.0f, 0.0f);
			m_xmf3Velocity = Vector3::TransformCoord(m_xmf3Velocity, xmf4x4Rotate);
			Move(m_xmf3Look, false);
		}
	}

	if (dynamic_cast<CHeightMapTerrain*>(pCollidedObject))
	{
		m_nHealthPoint = 0;
		m_bAlive = false;
		m_xmf3Velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
	}

	if (dynamic_cast<CMissile*>(pCollidedObject))
	{
		//hp-- 
	}

	if (dynamic_cast<CGunshipObject*>(pCollidedObject) || dynamic_cast<CSuperCobraObject*>(pCollidedObject))
	{
		//hp-- 
	}
}

void CHelicopterPlayer::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera)
{
	//if (!m_bAlive)
	//{
	//	return;
	//}

	CPlayer::Render(pd3dCommandList, pCamera);

	// Missile Render
	m_pMissileObject->UpdateTransform(&this->GetWorldTransform());
	m_pMissileObject->Render(pd3dCommandList, pCamera);

	for (auto& missile : m_arraypMissile)
	{
		if (!missile->GetAlive())
			continue;

		missile->UpdateTransform();
		missile->Render(pd3dCommandList, pCamera);
	}
}

void CHelicopterPlayer::OnPrepareRender()
{
	CPlayer::OnPrepareRender();
}

void CHelicopterPlayer::Fire()
{
	//미사일 중 하나를 깨워서 발사시킨다.
	for (auto& missile : m_arraypMissile)
	{
		if (missile->GetAlive())
		{
			continue;
		}

		missile->SetAlive(true);
		missile->SetLifeTime();

		XMFLOAT3 missilePos = GetPosition();
		missilePos.x = 0.0f;
		missilePos.y = 0.0f;
		missilePos.z = 10.85722f;

		XMFLOAT4X4 matrix = Matrix4x4::Identity();
		XMStoreFloat4x4(&matrix, XMMatrixTranslation(missilePos.x, missilePos.y, missilePos.z));

		missile->SetLocalTransform(Matrix4x4::Multiply(matrix, m_xmf4x4Local));
		break;
	}
}

void CHelicopterPlayer::SufferDamage(int nDamage)
{
	m_nHealthPoint -= nDamage;
	if (m_nHealthPoint <= 0)
	{
		m_bAlive = false;
	}
}

void CHelicopterPlayer::SetOOBB()
{
	m_pMainBodyFrame->GetMesh(0)->GetOOBB().Transform(m_OOBB, XMLoadFloat4x4(&m_xmf4x4Local));
	XMStoreFloat4(&m_OOBB.Orientation, XMQuaternionNormalize(XMLoadFloat4(&m_OOBB.Orientation)));
}

CCamera *CHelicopterPlayer::ChangeCamera(DWORD nNewCameraMode, float fTimeElapsed)
{
	DWORD nCurrentCameraMode = (m_pCamera) ? m_pCamera->GetMode() : 0x00;
	if (nCurrentCameraMode == nNewCameraMode) return(m_pCamera);
	switch (nNewCameraMode)
	{
		case FIRST_PERSON_CAMERA:
			SetFriction(2.0f);
			SetGravity(XMFLOAT3(0.0f, 0.0f, 0.0f));
			SetMaxVelocityXZ(2.5f);
			SetMaxVelocityY(40.0f);
			m_pCamera = OnChangeCamera(FIRST_PERSON_CAMERA, nCurrentCameraMode);
			m_pCamera->SetTimeLag(0.0f);
			m_pCamera->SetOffset(XMFLOAT3(0.0f, 20.0f, 0.0f));
			m_pCamera->GenerateProjectionMatrix(1.01f, 5000.0f, ASPECT_RATIO, 60.0f);
			m_pCamera->SetViewport(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT, 0.0f, 1.0f);
			m_pCamera->SetScissorRect(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT);
			break;
		case SPACESHIP_CAMERA:
			SetFriction(100.5f);
			SetGravity(XMFLOAT3(0.0f, 0.0f, 0.0f));
			SetMaxVelocityXZ(40.0f);
			SetMaxVelocityY(40.0f);
			m_pCamera = OnChangeCamera(SPACESHIP_CAMERA, nCurrentCameraMode);
			m_pCamera->SetTimeLag(0.0f);
			m_pCamera->SetOffset(XMFLOAT3(0.0f, 0.0f, 0.0f));
			m_pCamera->GenerateProjectionMatrix(1.01f, 5000.0f, ASPECT_RATIO, 60.0f);
			m_pCamera->SetViewport(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT, 0.0f, 1.0f);
			m_pCamera->SetScissorRect(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT);
			break;
		case THIRD_PERSON_CAMERA:
			SetFriction(150.0f);
			SetGravity(XMFLOAT3(0.0f, 0.0f, 0.0f));
			//SetGravity(XMFLOAT3(0.0f, -5.0f, 0.0f));
			SetMaxVelocityXZ(200.0f);
			SetMaxVelocityY(200.0f);
			m_pCamera = OnChangeCamera(THIRD_PERSON_CAMERA, nCurrentCameraMode);
			m_pCamera->SetTimeLag(0.25f);
			m_pCamera->SetOffset(XMFLOAT3(0.0f, 10.0f, -30.0f));
			m_pCamera->SetPosition(Vector3::Add(m_xmf3Position, m_pCamera->GetOffset()));
			m_pCamera->GenerateProjectionMatrix(1.01f, 5000.0f, ASPECT_RATIO, 60.0f);
			m_pCamera->SetViewport(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT, 0.0f, 1.0f);
			m_pCamera->SetScissorRect(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT);
			break;
		default:
			break;
	}
	Update(fTimeElapsed);

	return(m_pCamera);
}

