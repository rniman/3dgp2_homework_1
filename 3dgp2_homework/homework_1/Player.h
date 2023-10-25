#pragma once
//#define DIR_FORWARD				0x01
//#define DIR_BACKWARD			0x02
//#define DIR_LEFT				0x04
//#define DIR_RIGHT				0x08
//#define DIR_UP					0x10
//#define DIR_DOWN				0x20

#include "Object.h"
#include "Camera.h"
#include "Missile.h"

class CPlayer : public CGameObject
{
public:
	CPlayer();
	virtual ~CPlayer();

	virtual void Animate(float fTimeElapsed);
	virtual void Update(float fTimeElapsed) override;

	virtual CCamera* ChangeCamera(DWORD nNewCameraMode, float fTimeElapsed) { return(nullptr); }
	virtual void OnPrepareRender();
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera = nullptr);

	virtual void Move(const XMFLOAT3& xmf3Shift, bool bVelocity = false) override;
	virtual void Rotate(float x, float y, float z) override;
	void Decelerate(float fTimeElapsed);

	virtual void CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void ReleaseShaderVariables();
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList);

	virtual void OnPlayerUpdateCallback(float fTimeElapsed) { }
	void SetPlayerUpdatedContext(LPVOID pContext) { m_pPlayerUpdatedContext = pContext; }

	virtual void OnCameraUpdateCallback(float fTimeElapsed) { }
	void SetCameraUpdatedContext(LPVOID pContext) { m_pCameraUpdatedContext = pContext; }

	CCamera *OnChangeCamera(DWORD nNewCameraMode, DWORD nCurrentCameraMode);

	// interface
	XMFLOAT3 GetPosition() { return(m_xmf3Position); }
	XMFLOAT3 GetLookVector() { return(m_xmf3Look); }
	XMFLOAT3 GetUpVector() { return(m_xmf3Up); }
	XMFLOAT3 GetRightVector() { return(m_xmf3Right); }

	void SetFriction(float fFriction) { m_fFriction = fFriction; }
	void SetGravity(const XMFLOAT3& xmf3Gravity) { m_xmf3Gravity = xmf3Gravity; }
	void SetMaxVelocityXZ(float fMaxVelocity) { m_fMaxVelocityXZ = fMaxVelocity; }
	void SetMaxVelocityY(float fMaxVelocity) { m_fMaxVelocityY = fMaxVelocity; }
	void SetVelocity(const XMFLOAT3& xmf3Velocity) { m_xmf3Velocity = xmf3Velocity; }
	void SetPosition(const XMFLOAT3& xmf3Position) { Move(XMFLOAT3(xmf3Position.x - m_xmf3Position.x, xmf3Position.y - m_xmf3Position.y, xmf3Position.z - m_xmf3Position.z), false); }

	const XMFLOAT3& GetVelocity() const { return(m_xmf3Velocity); }
	float GetYaw() const { return(m_fYaw); }
	float GetPitch() const { return(m_fPitch); }
	float GetRoll() const { return(m_fRoll); }

	CCamera* GetCamera() { return(m_pCamera); }
	void SetCamera(CCamera* pCamera) { m_pCamera = pCamera; }
protected:
	XMFLOAT3	m_xmf3Position;
	XMFLOAT3	m_xmf3Right;
	XMFLOAT3	m_xmf3Up;
	XMFLOAT3	m_xmf3Look;

	float       m_fPitch;
	float       m_fYaw;
	float       m_fRoll;

	XMFLOAT3	m_xmf3Velocity;
	XMFLOAT3    m_xmf3Gravity;
	float       m_fMaxVelocityXZ;
	float       m_fMaxVelocityY;
	float       m_fFriction;
	
	LPVOID		m_pPlayerUpdatedContext;
	LPVOID		m_pCameraUpdatedContext;

	CCamera*	m_pCamera = nullptr;
	CShader*	m_pShader = nullptr;
};

constexpr unsigned short MAX_NUM_MISSILE = 10;

class CHelicopterPlayer : public CPlayer
{
public:
	CHelicopterPlayer(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature);
	virtual ~CHelicopterPlayer();

	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList);

	void PrepareOOBB() override;

	virtual void PrepareAnimate() override;
	virtual void Animate(float fTimeElapsed) override;
	virtual void Collide(CGameObject* pCollidedObject = nullptr, float fTimeElapsed = 0.0f) override;
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera = nullptr) override;

	virtual CCamera* ChangeCamera(DWORD nNewCameraMode, float fTimeElapsed);
	virtual void OnPrepareRender();

	void Fire();

	// interface
	virtual void SetOOBB() override;
	
	float GetCoolTime() const { return m_fCoolTime; };
	void SetCoolTime(float fCoolTime) { m_fCoolTime = fCoolTime; };
	
	virtual class CMissile* GetMissile(int nIndex) {
		if (nIndex > MAX_NUM_MISSILE || nIndex < 0)
			return nullptr;
		return m_arraypMissile[nIndex]; 
	};
private:
	CGameObject* m_pMainBodyFrame = nullptr;
	CGameObject* m_pMainRotorFrame = nullptr;
	CGameObject* m_pTailRotorFrame = nullptr;
	CGameObject* m_pMissileObject = nullptr;

	std::array<CMissile*, MAX_NUM_MISSILE> m_arraypMissile;

	float m_fCoolTime = 0.0f;
};


