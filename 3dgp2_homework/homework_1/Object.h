//------------------------------------------------------- ----------------------
// File: Object.h
//-----------------------------------------------------------------------------

#pragma once

#include "Mesh.h"
#include "Camera.h"

class CShader;
class CStandardShader;
constexpr unsigned short MAX_NUM_MISSILE = 10;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
#define RESOURCE_TEXTURE2D			0x01
#define RESOURCE_TEXTURE2D_ARRAY	0x02	//[]
#define RESOURCE_TEXTURE2DARRAY		0x03
#define RESOURCE_TEXTURE_CUBE		0x04
#define RESOURCE_BUFFER				0x05

class CGameObject;

class CTexture
{
public:
	CTexture(int nTextureResources, UINT nResourceType, int nSamplers, int nRootParameters, int nRows = 1, int nCols = 1);
	virtual ~CTexture();

	void AddRef() { m_nReferences++; }
	void Release() { if (--m_nReferences <= 0) delete this; }

	void ReleaseUploadBuffers();

	void SetSampler(int nIndex, D3D12_GPU_DESCRIPTOR_HANDLE d3dSamplerGpuDescriptorHandle);

	void UpdateShaderVariable(ID3D12GraphicsCommandList* pd3dCommandList, int nParameterIndex, int nTextureIndex);
	void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList);
	void ReleaseShaderVariables();

	void LoadTextureFromDDSFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, wchar_t* pszFileName, UINT nResourceType, UINT nIndex);
	void LoadBuffer(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, void* pData, UINT nElements, UINT nStride, DXGI_FORMAT ndxgiFormat, UINT nIndex);
	ID3D12Resource* CreateTexture(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, UINT nIndex, UINT nResourceType, UINT nWidth, UINT nHeight, UINT nElements, UINT nMipLevels, DXGI_FORMAT dxgiFormat, D3D12_RESOURCE_FLAGS d3dResourceFlags, D3D12_RESOURCE_STATES d3dResourceStates, D3D12_CLEAR_VALUE* pd3dClearValue);

	int LoadTextureFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, CGameObject* pParent, FILE* pInFile, CShader* pShader, UINT nIndex);

	void SetRootParameterIndex(int nIndex, UINT nRootParameterIndex);
	void SetGpuDescriptorHandle(int nIndex, D3D12_GPU_DESCRIPTOR_HANDLE d3dSrvGpuDescriptorHandle);
	D3D12_GPU_DESCRIPTOR_HANDLE GetGpuDescriptorHandle(int nIndex) { return(m_pd3dSrvGpuDescriptorHandles[nIndex]); }

	int GetRootParameters() { return(m_nRootParameters); }
	int GetTextures() { return(m_nTextures); }
	_TCHAR* GetTextureName(int nIndex) { return(m_ppstrTextureNames[nIndex]); }
	ID3D12Resource* GetResource(int nIndex) { return(m_ppd3dTextures[nIndex].Get()); }
	int GetRootParameter(int nIndex) { return(m_pnRootParameterIndices[nIndex]); }

	UINT GetTextureType() { return(m_nTextureType); }
	UINT GetTextureType(int nIndex) { return(m_pnResourceTypes[nIndex]); }
	DXGI_FORMAT GetBufferFormat(int nIndex) { return(m_pdxgiBufferFormats[nIndex]); }
	int GetBufferElements(int nIndex) { return(m_pnBufferElements[nIndex]); }

	D3D12_SHADER_RESOURCE_VIEW_DESC GetShaderResourceViewDesc(int nIndex);
	
	// for sprite texture
	//void AnimateRowColumn(float fTime);
	UINT GetColumns() const { return m_nColumns; };
	UINT GetRows() const { return m_nRows; };

	void SetColumnAndRow(int nColumn, int nRow) { m_nColumn = nColumn; m_nRow = m_nRow; };

private:
	int								m_nReferences = 0;

	UINT							m_nTextureType;

	int								m_nTextures = 0;
	_TCHAR							(*m_ppstrTextureNames)[64] = nullptr;
	
	VectorComptrResource			m_ppd3dTextures;
	VectorComptrResource			m_ppd3dTextureUploadBuffers;

	UINT*							m_pnResourceTypes = nullptr;

	DXGI_FORMAT*					m_pdxgiBufferFormats = nullptr;
	int*							m_pnBufferElements = nullptr;

	int								m_nRootParameters = 0;
	int*							m_pnRootParameterIndices = nullptr;
	D3D12_GPU_DESCRIPTOR_HANDLE*	m_pd3dSrvGpuDescriptorHandles = nullptr;
	
	int								m_nSamplers = 0;
	D3D12_GPU_DESCRIPTOR_HANDLE*	m_pd3dSamplerGpuDescriptorHandles = nullptr;

	UINT							m_nColumns = 1;
	UINT							m_nRows = 1;

	UINT							m_nColumn = 0;
	UINT							m_nRow = 0;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
#define MATERIAL_ALBEDO_MAP			0x01
#define MATERIAL_SPECULAR_MAP		0x02
#define MATERIAL_NORMAL_MAP			0x04
#define MATERIAL_METALLIC_MAP		0x08
#define MATERIAL_EMISSION_MAP		0x10
#define MATERIAL_DETAIL_ALBEDO_MAP	0x20
#define MATERIAL_DETAIL_NORMAL_MAP	0x40

class CGameObject;

//struct MATERIAL
//{
//	XMFLOAT4						m_xmf4Ambient;
//	XMFLOAT4						m_xmf4Diffuse;
//	XMFLOAT4						m_xmf4Specular; //(r,g,b,a=power)
//	XMFLOAT4						m_xmf4Emissive;
//};

class CMaterial
{
public:
	CMaterial();
	virtual ~CMaterial();

public:
	void AddRef() { m_nReferences++; }
	void Release() { if (--m_nReferences <= 0) delete this; }

	void SetShader(CShader* pShader);
	void SetMaterialType(UINT nType) { m_nType |= nType; }
	void SetTexture(CTexture* pTexture);

	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void ReleaseShaderVariables();

	virtual void ReleaseUploadBuffers();

public:
	CShader*						m_pShader = nullptr;
	CTexture*						m_pTexture = nullptr;

	XMFLOAT4						m_xmf4AlbedoColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	XMFLOAT4						m_xmf4EmissiveColor = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	XMFLOAT4						m_xmf4SpecularColor = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	XMFLOAT4						m_xmf4AmbientColor = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);

	UINT							m_nType = 0x00;

	float							m_fGlossiness = 0.0f;
	float							m_fSmoothness = 0.0f;
	float							m_fSpecularHighlight = 0.0f;
	float							m_fMetallic = 0.0f;
	float							m_fGlossyReflection = 0.0f;

private:
	int								m_nReferences = 0;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
class CGameObject
{
private:
	int								m_nReferences = 0;

public:
	void AddRef();
	void Release();

public:
	CGameObject();
	CGameObject(int nMeshes, int nMaterials);
	virtual ~CGameObject();

	virtual void SetMesh(int nIndex, CMesh* pMesh);
	void SetShader(int nMaterial, CShader* pShader);
	void SetMaterial(int nMaterial, CMaterial* pMaterial);

	virtual void BuildMaterials(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList) { }

	virtual void PrepareOOBB(){ };
	virtual void PrepareAnimate() { }
	virtual void Animate(float fTimeElapsed);
	virtual void Collide(CGameObject* pCollidedObject = nullptr, float fTimeElapsed = 0.0f) {};
	virtual void Update(float fTimeElapsed) {};

	virtual void OnPrepareRender() { }
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera = nullptr);
	virtual void RenderInstance(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera, int nInstances, D3D12_VERTEX_BUFFER_VIEW d3dInstancingBufferView, int nStartInstance);

	virtual void CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void ReleaseShaderVariables();

	virtual void UpdateShaderVariable(ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void UpdateShaderVariable(ID3D12GraphicsCommandList* pd3dCommandList, CMaterial* pMaterial);

	virtual void ReleaseUploadBuffers();

	XMFLOAT3 GetPosition();
	XMFLOAT3 GetLook();
	XMFLOAT3 GetUp();
	XMFLOAT3 GetRight();
	XMFLOAT4X4 GetLocalTransform() const;
	XMFLOAT4X4 GetWorldTransform() const;
	void SetPosition(float x, float y, float z);
	void SetPosition(XMFLOAT3 xmf3Position);
	void SetScale(float x, float y, float z);
	void SetLookAt(XMFLOAT3& xmf3Target, XMFLOAT3& xmf3Up);
	void SetLookTo(XMFLOAT3& xmf3Look, XMFLOAT3& xmf3Up);
	void SetLocalTransform(XMFLOAT4X4& xmf4x4World);

	virtual void Move(const XMFLOAT3& xmf3Shift, bool bVelocity = false) {};
	void MoveStrafe(float fDistance = 1.0f);
	void MoveUp(float fDistance = 1.0f);
	void MoveForward(float fDistance = 1.0f);

	virtual void Rotate(float fPitch = 10.0f, float fYaw = 10.0f, float fRoll = 10.0f);
	void Rotate(XMFLOAT3* pxmf3Axis, float fAngle);
	void Rotate(XMFLOAT4* pxmf4Quaternion);

	CGameObject* GetParent() { return(m_pParent); }
	void UpdateTransform(XMFLOAT4X4* pxmf4x4Parent = nullptr);
	CGameObject* FindFrame(char* pstrFrameName);

	int FindReplicatedTexture(_TCHAR* pstrTextureName, D3D12_GPU_DESCRIPTOR_HANDLE* pd3dSrvGpuDescriptorHandle);

	CMesh* GetMesh(UINT nIndex) const { return m_ppMeshes[nIndex]; };
	UINT GetMeshType(UINT nIndex) { return((m_ppMeshes[nIndex]) ? m_ppMeshes[nIndex]->GetType() : 0x00); }

	void LoadMaterialsFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, CGameObject* pParent, FILE* pInFile, CShader* pShader);

	static CGameObject* LoadFrameHierarchyFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, CGameObject* pParent, FILE* pInFile, CShader* pShader);
	static CGameObject* LoadGeometryFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, char* pstrFileName, CShader* pShader);

	static void PrintFrameInfo(CGameObject* pGameObject, CGameObject* pParent);

	virtual void SufferDamage(int nDamage) {};
	// interface
	virtual void SetOOBB();
	BoundingOrientedBox GetOOBB() const { return m_OOBB; };

	bool GetAlive() const { return m_bAlive; };
	void SetAlive(bool bAlive) {  m_bAlive = bAlive; };

	void SetChild(CGameObject* pChild);

	CGameObject* GetChild() const { return m_pChild; };
	CGameObject* GetSibling() const { return m_pSibling; };
	CGameObject* GetParent() const { return m_pParent; };
	
	virtual class CMissile* GetMissile(int nIndex) { return nullptr; };
public:
	bool		 m_bAlive = true;
	char		 m_pstrFrameName[64];

	int			 m_nMeshes = 0;
	CMesh**		 m_ppMeshes = nullptr;

	int			 m_nMaterials = 0;
	CMaterial**	 m_ppMaterials = nullptr;

	XMFLOAT4X4	 m_xmf4x4Local;
	XMFLOAT4X4	 m_xmf4x4World;

	CGameObject* m_pParent = nullptr;
	CGameObject* m_pChild = nullptr;
	CGameObject* m_pSibling = nullptr;

	BoundingOrientedBox m_OOBB{ BoundingOrientedBox() };
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
class CSuperCobraObject : public CGameObject
{
public:
	CSuperCobraObject(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature);
	virtual ~CSuperCobraObject();

	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList) {};

	virtual void PrepareOOBB() override;
	virtual void PrepareAnimate() override;
	virtual void Animate(float fTimeElapsed) override;
	virtual void Collide(CGameObject* pCollidedObject = nullptr, float fTimeElapsed = 0.0f) override;
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera = nullptr) override {};

	virtual void Move(const XMFLOAT3& xmf3Shift, bool bVelocity = false) override;
	void Fire();
	
	// interface
	virtual void SetOOBB() override;

	float GetCoolTime() const { return m_fCoolTime; };
	void SetCoolTime(float fCoolTime) { m_fCoolTime = fCoolTime; };

	virtual class CMissile* GetMissile(int nIndex) { return m_arraypMissile[nIndex]; };
private:
	CGameObject* m_pMainRotorFrame = nullptr;
	CGameObject* m_pTailRotorFrame = nullptr;
	CGameObject* m_pMainBodyFrame = nullptr;

	XMFLOAT3	m_xmf3Velocity;
	XMFLOAT3    m_xmf3Gravity;
	float       m_fMaxVelocityXZ;
	float       m_fMaxVelocityY;
	float       m_fFriction;

	CGameObject* m_pTarget = nullptr;

	CGameObject* m_pMissileObject = nullptr;
	std::array<CMissile*, MAX_NUM_MISSILE> m_arraypMissile;

	float m_fCoolTime = 0.0f;
};

class CGunshipObject : public CGameObject
{
public:
	CGunshipObject(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature);
	CGunshipObject(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature, CGameObject* pMissileObjectModel, void* pTarget);
	virtual ~CGunshipObject();

	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList);

	virtual void PrepareOOBB() override;
	virtual void PrepareAnimate() override;
	virtual void Animate(float fTimeElapsed) override;
	virtual void Collide(CGameObject* pCollidedObject = nullptr, float fTimeElapsed = 0.0f) override;
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera = nullptr) override;

	virtual void Move(const XMFLOAT3& xmf3Shift, bool bVelocity = false) override;
	void Decelerate(float fTimeElapsed);

	void Fire();
	void Find(float fTimeElapsed);
	void Chase(float fTimeElapsed);
	virtual void SufferDamage(int nDamage) override;

	// interface
	virtual void SetOOBB() override;
	
	void SetTarget(CGameObject* pTarget) { m_pTarget = pTarget; };

	float GetCoolTime() const { return m_fCoolTime; };
	void SetCoolTime(float fCoolTime) { m_fCoolTime = fCoolTime; };

	virtual class CMissile* GetMissile(int nIndex) { return m_arraypMissile[nIndex]; };
	std::array<CMissile*, MAX_NUM_MISSILE> GetMissiles() { return m_arraypMissile; };

	void SetExplosion(CGameObject* pExplosion) { m_pExplosionAnimation = pExplosion; };
private:
	CGameObject* m_pMainRotorFrame = nullptr;
	CGameObject* m_pTailRotorFrame = nullptr;
	CGameObject* m_pMainBodyFrame = nullptr;
	CGameObject* m_pExplosionAnimation;

	XMFLOAT3	mxmf3Position;

	XMFLOAT3	m_xmf3Velocity;
	XMFLOAT3    m_xmf3Gravity;
	float       m_fMaxVelocityXZ;
	float       m_fMaxVelocityY;
	float       m_fFriction;

	CGameObject* m_pTarget= nullptr;

	std::array<CMissile*, MAX_NUM_MISSILE> m_arraypMissile;

	int   m_nHealthPoint = 1;
	float m_fFireCoolTime = 0.0f;
	float m_fCoolTime = 0.0f;
	float m_fRotateSpeed = 180.0f;
	float m_fRange = 0.0f;
};

class CMi24Object : public CGameObject
{
public:
	CMi24Object(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature);
	virtual ~CMi24Object();

	virtual void PrepareAnimate();
	virtual void Animate(float fTimeElapsed);

private:
	CGameObject	*m_pMainRotorFrame = nullptr;
	CGameObject	*m_pTailRotorFrame = nullptr;
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CSkyBox : public CGameObject
{
public:
	CSkyBox(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature);
	virtual ~CSkyBox();

	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera = nullptr);
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CHeightMapTerrain : public CGameObject
{
public:
	CHeightMapTerrain(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, LPCTSTR pFileName,
					  int nWidth, int nLength, int nBlockWidth, int nBlockLength, XMFLOAT3 xmf3Scale, XMFLOAT4 xmf4Color, int nPixelNum = 1);
	virtual ~CHeightMapTerrain();

	float GetHeight(float x, float z, bool bReverseQuad = false) { return(m_pHeightMapImage->GetHeight(x, z, bReverseQuad) * m_xmf3Scale.y); } //World
	XMFLOAT3 GetNormal(float x, float z) { return(m_pHeightMapImage->GetHeightMapNormal(int(x / m_xmf3Scale.x), int(z / m_xmf3Scale.z))); }

	int GetRawImageWidth() { return(m_pHeightMapImage->GetRawImageWidth()); }
	int GetRawImageLength() { return(m_pHeightMapImage->GetRawImageLength()); }

	XMFLOAT3 GetScale() { return(m_xmf3Scale); }
	float GetWidth() { return(m_nWidth * m_xmf3Scale.x); }
	float GetLength() { return(m_nLength * m_xmf3Scale.z); }

private:
	CHeightMapImage*	m_pHeightMapImage;
	int					m_nWidth;
	int					m_nLength;
	XMFLOAT3			m_xmf3Scale;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class COceanObjcet :public CGameObject
{
public:
	COceanObjcet(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, void* pContext = nullptr);
	virtual ~COceanObjcet();

	virtual void Animate(float fTimeElapsed);

	virtual void SetPlayer(class CPlayer* pPlayer);

private:
	CPlayer* m_pPlayer = nullptr;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CSpriteObject : public CGameObject
{
public:
	CSpriteObject(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, int nMesh, int nMaterial, int nColumns, int nRows);
	~CSpriteObject();

	virtual void Animate(float fTimeElapsed) override;
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera = nullptr) override;

	void SetSpeed(float fSpeed) { m_fSpeed = fSpeed; };
	void SetTime(float fTime) { m_fTime = fTime; };
	CMaterial* GetMaterial(int nIndex)const { return m_ppMaterials[nIndex]; };

	void SetRow(UINT nRow) { m_nRow = nRow; };
	void SetColumn(UINT nColumn) { m_nColumn = nColumn; };
private:
	float m_fSpeed = 0.1f;
	float m_fTime = 0.0f;

	UINT m_nColumns = 1;
	UINT m_nRows = 1;
	UINT m_nColumn = 0;
	UINT m_nRow = 0;
};