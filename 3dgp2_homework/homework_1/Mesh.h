//------------------------------------------------------- ----------------------
// File: Mesh.h
//-----------------------------------------------------------------------------

#pragma once

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
#define VERTEXT_POSITION				0x01
#define VERTEXT_COLOR					0x02
#define VERTEXT_NORMAL					0x04
#define VERTEXT_TANGENT					0x08
#define VERTEXT_TEXTURE_COORD0			0x10
#define VERTEXT_TEXTURE_COORD1			0x20

#define VERTEXT_TEXTURE					(VERTEXT_POSITION | VERTEXT_TEXTURE_COORD0)
#define VERTEXT_DETAIL					(VERTEXT_POSITION | VERTEXT_TEXTURE_COORD0 | VERTEXT_TEXTURE_COORD1)
#define VERTEXT_NORMAL_TEXTURE			(VERTEXT_POSITION | VERTEXT_NORMAL | VERTEXT_TEXTURE_COORD0)
#define VERTEXT_NORMAL_TANGENT_TEXTURE	(VERTEXT_POSITION | VERTEXT_NORMAL | VERTEXT_TANGENT | VERTEXT_TEXTURE_COORD0)
#define VERTEXT_NORMAL_DETAIL			(VERTEXT_POSITION | VERTEXT_NORMAL | VERTEXT_TEXTURE_COORD0 | VERTEXT_TEXTURE_COORD1)
#define VERTEXT_NORMAL_TANGENT__DETAIL	(VERTEXT_POSITION | VERTEXT_NORMAL | VERTEXT_TANGENT | VERTEXT_TEXTURE_COORD0 | VERTEXT_TEXTURE_COORD1)

class CTreeVertex
{
public:
	CTreeVertex();
	CTreeVertex(XMFLOAT3 xmf3Position, XMFLOAT2 xmf2Size)
		: m_xmf3Position{ xmf3Position }
		, m_xmf2Size{ xmf2Size }
	{}
	~CTreeVertex();

	XMFLOAT3 m_xmf3Position;
	XMFLOAT2 m_xmf2Size;

};

//
class CVertex
{
public:
	XMFLOAT3						m_xmf3Position;

public:
	CVertex() { m_xmf3Position = XMFLOAT3(0.0f, 0.0f, 0.0f); }
	CVertex(XMFLOAT3 xmf3Position) { m_xmf3Position = xmf3Position; }
	~CVertex() { }
};

class CDiffusedVertex : public CVertex
{
public:
	XMFLOAT4						m_xmf4Diffuse;

public:
	CDiffusedVertex() { m_xmf3Position = XMFLOAT3(0.0f, 0.0f, 0.0f); m_xmf4Diffuse = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f); }
	CDiffusedVertex(float x, float y, float z, XMFLOAT4 xmf4Diffuse) { m_xmf3Position = XMFLOAT3(x, y, z); m_xmf4Diffuse = xmf4Diffuse; }
	CDiffusedVertex(XMFLOAT3 xmf3Position, XMFLOAT4 xmf4Diffuse = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f)) { m_xmf3Position = xmf3Position; m_xmf4Diffuse = xmf4Diffuse; }
	~CDiffusedVertex() { }
};

class CTexturedVertex : public CVertex
{
public:
	XMFLOAT2						m_xmf2TexCoord;

public:
	CTexturedVertex() { m_xmf3Position = XMFLOAT3(0.0f, 0.0f, 0.0f); m_xmf2TexCoord = XMFLOAT2(0.0f, 0.0f); }
	
	CTexturedVertex(float x, float y, float z, XMFLOAT2 xmf2TexCoord) { m_xmf3Position = XMFLOAT3(x, y, z); m_xmf2TexCoord = xmf2TexCoord; }
	
	CTexturedVertex(XMFLOAT3 xmf3Position, XMFLOAT2 xmf2TexCoord = XMFLOAT2(0.0f, 0.0f)) { m_xmf3Position = xmf3Position; m_xmf2TexCoord = xmf2TexCoord; }
	
	~CTexturedVertex() { }
};

class CDiffusedTexturedVertex : public CDiffusedVertex
{
public:
	XMFLOAT2						m_xmf2TexCoord;

public:
	CDiffusedTexturedVertex() { m_xmf3Position = XMFLOAT3(0.0f, 0.0f, 0.0f); m_xmf4Diffuse = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f); m_xmf2TexCoord = XMFLOAT2(0.0f, 0.0f); }
	
	CDiffusedTexturedVertex(float x, float y, float z, XMFLOAT4 xmf4Diffuse, XMFLOAT2 xmf2TexCoord) 
	{ m_xmf3Position = XMFLOAT3(x, y, z); m_xmf4Diffuse = xmf4Diffuse; m_xmf2TexCoord = xmf2TexCoord; }
	
	CDiffusedTexturedVertex(XMFLOAT3 xmf3Position, XMFLOAT4 xmf4Diffuse = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f), XMFLOAT2 xmf2TexCoord = XMFLOAT2(0.0f, 0.0f)) 
	{ m_xmf3Position = xmf3Position; m_xmf4Diffuse = xmf4Diffuse; m_xmf2TexCoord = xmf2TexCoord; }
	
	~CDiffusedTexturedVertex() { }
};

class CDiffused2TexturedVertex : public CDiffusedVertex
{
public:
	XMFLOAT2						m_xmf2TexCoord0;
	XMFLOAT2						m_xmf2TexCoord1;

public:
	CDiffused2TexturedVertex(){ m_xmf3Position = XMFLOAT3(0.0f, 0.0f, 0.0f); m_xmf4Diffuse = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	m_xmf2TexCoord0 = m_xmf2TexCoord1 = XMFLOAT2(0.0f, 0.0f); }

	CDiffused2TexturedVertex(float x, float y, float z, XMFLOAT4 xmf4Diffuse, XMFLOAT2 xmf2TexCoord0, XMFLOAT2 xmf2TexCoord1) 
	{ m_xmf3Position = XMFLOAT3(x, y, z); m_xmf4Diffuse = xmf4Diffuse; m_xmf2TexCoord0 = xmf2TexCoord0; m_xmf2TexCoord1 = xmf2TexCoord1; }

	CDiffused2TexturedVertex(XMFLOAT3 xmf3Position, XMFLOAT4 xmf4Diffuse = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f), XMFLOAT2 xmf2TexCoord0 = XMFLOAT2(0.0f, 0.0f), XMFLOAT2 xmf2TexCoord1 = XMFLOAT2(0.0f, 0.0f)) 
	{ m_xmf3Position = xmf3Position; m_xmf4Diffuse = xmf4Diffuse; m_xmf2TexCoord0 = xmf2TexCoord0; m_xmf2TexCoord1 = xmf2TexCoord1; }
	
	~CDiffused2TexturedVertex() { }
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

class CMesh
{
public:
	CMesh(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	virtual ~CMesh();

	void AddRef() { m_nReferences++; }
	void Release() { if (--m_nReferences <= 0) delete this; }

	virtual void ReleaseUploadBuffers();
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, int nSubSet);
	virtual void RenderInstance(ID3D12GraphicsCommandList* pd3dCommandList, int nInstances, D3D12_VERTEX_BUFFER_VIEW d3dInstancingBufferView, int nStartInstance);

	// interface
	UINT GetType() const { return(m_nType); }
	BoundingOrientedBox GetOOBB() const { return m_OOBB; };

private:
	int								m_nReferences = 0;

protected:
	UINT							m_nType = 0x00;

	XMFLOAT3						m_xmf3AABBCenter = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3						m_xmf3AABBExtents = XMFLOAT3(0.0f, 0.0f, 0.0f);

	D3D12_PRIMITIVE_TOPOLOGY		m_d3dPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	UINT							m_nSlot = 0;
	UINT							m_nOffset = 0;

	int								m_nVertices = 0;

	XMFLOAT3						*m_pxmf3Positions = nullptr;

	ComPtr<ID3D12Resource>			m_pd3dVertexBuffer;
	ComPtr<ID3D12Resource>			m_pd3dVertexUploadBuffer;
	D3D12_VERTEX_BUFFER_VIEW		m_d3dVertexBufferView;

	int								m_nSubMeshes = 0;
	int								*m_pnSubSetIndices = nullptr;
	UINT							**m_ppnSubSetIndices = nullptr;

	VectorComptrResource			m_ppd3dSubSetIndexBuffers;
	VectorComptrResource			m_ppd3dSubSetIndexUploadBuffers;
	vector<D3D12_INDEX_BUFFER_VIEW>	m_pd3dSubSetIndexBufferViews;

	BoundingOrientedBox				m_OOBB{ BoundingOrientedBox() };	//�� ��ǥ���� �ٿ�� �ڽ�
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
class CMissileMesh : public CMesh
{
	CMissileMesh(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, float fWidth = 5.0f, float fHeight = 5.0f, float fDepth = 10.0f);
	virtual ~CMissileMesh();
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
class CSkyBoxMesh : public CMesh
{
public:
	CSkyBoxMesh(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, float fWidth = 20.0f, float fHeight = 20.0f, float fDepth = 20.0f);
	virtual ~CSkyBoxMesh();
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
class CBillboardMesh : public CMesh
{
	CBillboardMesh(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, int nBillboard);
	~CBillboardMesh();

protected:
	XMFLOAT2* m_pxmf2TextureCoords0 = nullptr;

	ComPtr<ID3D12Resource>			m_pd3dTextureCoord0Buffer;
	ComPtr<ID3D12Resource>			m_pd3dTextureCoord0UploadBuffer;
	D3D12_VERTEX_BUFFER_VIEW		m_d3dTextureCoord0BufferView;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
class CTexturedRectMesh : public CMesh
{
public:
	//CTexturedRectMesh(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, float fWidth=20.0f, float fHeight=20.0f, float fDepth=20.0f, float fxPosition=0.0f, float fyPosition=0.0f, float fzPosition=0.0f);
	CTexturedRectMesh(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, float fWidth = 20.0f, float fHeight = 20.0f, float fDepth = 20.0f, 
					  float fMaxU = 1.0f, float fMaxV = 1.0f, float fMinU = 0.0f, float fMinV = 0.0f, float fxPosition = 0.0f, float fyPosition = 0.0f, float fzPosition = 0.0f);
	virtual ~CTexturedRectMesh();

	virtual void ReleaseUploadBuffers();
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, int nSubSet);
	virtual void RenderInstance(ID3D12GraphicsCommandList* pd3dCommandList, int nInstances, D3D12_VERTEX_BUFFER_VIEW d3dInstancingBufferView, int nStartInstance);
	virtual void RenderInstance(ID3D12GraphicsCommandList* pd3dCommandList, int nInstances, int nStartInstance);

protected:
	XMFLOAT2						*m_pxmf2TextureCoords0 = nullptr;

	ComPtr<ID3D12Resource>			m_pd3dTextureCoord0Buffer;
	ComPtr<ID3D12Resource>			m_pd3dTextureCoord0UploadBuffer;
	D3D12_VERTEX_BUFFER_VIEW		m_d3dTextureCoord0BufferView;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
class CUserInterfaceRectMesh : public CMesh
{
public:
	CUserInterfaceRectMesh(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, float fWidth = 20.0f, float fHeight = 20.0f, float fDepth = 20.0f,
						   float fMaxU = 1.0f, float fMaxV = 1.0f, float fMinU = 0.0f, float fMinV = 0.0f);
	virtual ~CUserInterfaceRectMesh();

	virtual void ReleaseUploadBuffers();
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, int nSubSet);

	// ���� �ϳ� �ʺ�,���� 50�ȼ�
	// �� �ؽ��� �ʺ� 500�ȼ�
	void SetUV(float fMinU, float fMinV = 0.0f);

protected:
	XMFLOAT2* m_pxmf2TextureCoords0 = nullptr;

	ComPtr<ID3D12Resource>			m_pd3dTextureCoord0Buffer;
	//ComPtr<ID3D12Resource>			m_pd3dTextureCoord0UploadBuffer;
	D3D12_VERTEX_BUFFER_VIEW		m_d3dTextureCoord0BufferView;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
class CStandardMesh : public CMesh
{
public:
	CStandardMesh(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	virtual ~CStandardMesh();

	void LoadMeshFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, FILE* pInFile);

	virtual void ReleaseUploadBuffers();
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, int nSubSet);

protected:
	char							m_pstrMeshName[256] = { 0 };

	XMFLOAT4						*m_pxmf4Colors = nullptr;
	XMFLOAT3						*m_pxmf3Normals = nullptr;
	XMFLOAT3						*m_pxmf3Tangents = nullptr;
	XMFLOAT3						*m_pxmf3BiTangents = nullptr;
	XMFLOAT2						*m_pxmf2TextureCoords0 = nullptr;
	XMFLOAT2						*m_pxmf2TextureCoords1 = nullptr;

	ComPtr<ID3D12Resource>			m_pd3dTextureCoord0Buffer;
	ComPtr<ID3D12Resource>			m_pd3dTextureCoord0UploadBuffer;
	D3D12_VERTEX_BUFFER_VIEW		m_d3dTextureCoord0BufferView;

	ComPtr<ID3D12Resource>			m_pd3dTextureCoord1Buffer;
	ComPtr<ID3D12Resource>			m_pd3dTextureCoord1UploadBuffer;
	D3D12_VERTEX_BUFFER_VIEW		m_d3dTextureCoord1BufferView;

	ComPtr<ID3D12Resource>			m_pd3dNormalBuffer;
	ComPtr<ID3D12Resource>			m_pd3dNormalUploadBuffer;
	D3D12_VERTEX_BUFFER_VIEW		m_d3dNormalBufferView;

	ComPtr<ID3D12Resource>			m_pd3dTangentBuffer;
	ComPtr<ID3D12Resource>			m_pd3dTangentUploadBuffer;
	D3D12_VERTEX_BUFFER_VIEW		m_d3dTangentBufferView;

	ComPtr<ID3D12Resource>			m_pd3dBiTangentBuffer;
	ComPtr<ID3D12Resource>			m_pd3dBiTangentUploadBuffer;
	D3D12_VERTEX_BUFFER_VIEW		m_d3dBiTangentBufferView;

};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CRawFormatImage
{
public:
	CRawFormatImage(LPCTSTR pFileName, int nWidth, int nLength, bool bFlipY = false, int nPixelNum = 1);
	~CRawFormatImage(void);

	// interface
	BYTE GetRawImagePixel(int x, int z) { return(m_pRawImagePixels[x + (z * m_nWidth)]); }
	void SetRawImagePixel(int x, int z, BYTE nPixel) { m_pRawImagePixels[x + (z * m_nWidth)] = nPixel; }

	BYTE* GetRawImagePixels() { return(m_pRawImagePixels); }

	int GetRawImageWidth() { return(m_nWidth); }
	int GetRawImageLength() { return(m_nLength); }

protected:
	BYTE* m_pRawImagePixels = nullptr;

	int	m_nWidth;
	int	m_nLength;
};

class CHeightMapImage : public CRawFormatImage
{
public:
	CHeightMapImage(LPCTSTR pFileName, int nWidth, int nLength, XMFLOAT3 xmf3Scale, bool bFlipY = true, int nPixelNum = 1);
	~CHeightMapImage(void);

	// interface
	XMFLOAT3 GetScale() { return(m_xmf3Scale); }
	float GetHeight(float x, float z, bool bReverseQuad = false);
	XMFLOAT3 GetHeightMapNormal(int x, int z);

protected:
	XMFLOAT3					m_xmf3Scale;
};

class CHeightMapGridMesh : public CMesh
{
public:
	CHeightMapGridMesh(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, int xStart, int zStart, int nWidth, int nLength, XMFLOAT3 xmf3Scale = XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT4 xmf4Color = XMFLOAT4(1.0f, 1.0f, 0.0f, 0.0f), void* pContext = nullptr);
	virtual ~CHeightMapGridMesh();

	virtual void ReleaseUploadBuffers();
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, int nSubSet);

	virtual float OnGetHeight(int x, int z, void* pContext);
	virtual XMFLOAT4 OnGetColor(int x, int z, void* pContext);

	// interface
	XMFLOAT3 GetScale() { return(m_xmf3Scale); }
	int GetWidth() { return(m_nWidth); }
	int GetLength() { return(m_nLength); }

protected:
	int							m_nWidth;
	int							m_nLength;
	XMFLOAT3					m_xmf3Scale;

	XMFLOAT4*					m_pxmf4Colors = nullptr;
	XMFLOAT2*					m_pxmf2TextureCoords0 = nullptr;
	XMFLOAT2*					m_pxmf2TextureCoords1 = nullptr;

	ComPtr<ID3D12Resource>		m_pd3dColorBuffer;
	ComPtr<ID3D12Resource>		m_pd3dColorUploadBuffer;
	D3D12_VERTEX_BUFFER_VIEW	m_d3dColorBufferView;

	ComPtr<ID3D12Resource>		m_pd3dTextureCoord0Buffer;
	ComPtr<ID3D12Resource>		m_pd3dTextureCoord0UploadBuffer;
	D3D12_VERTEX_BUFFER_VIEW	m_d3dTextureCoord0BufferView;

	ComPtr<ID3D12Resource>		m_pd3dTextureCoord1Buffer;
	ComPtr<ID3D12Resource>		m_pd3dTextureCoord1UploadBuffer;
	D3D12_VERTEX_BUFFER_VIEW	m_d3dTextureCoord1BufferView;
};

