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

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//


class CMesh
{
public:
	CMesh(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	virtual ~CMesh();

private:
	int								m_nReferences = 0;

public:
	void AddRef() { m_nReferences++; }
	void Release() { if (--m_nReferences <= 0) delete this; }

	virtual void ReleaseUploadBuffers();
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, int nSubSet);

	// interface
	UINT GetType() { return(m_nType); }

protected:
	char							m_pstrMeshName[256] = { 0 };

	UINT							m_nType = 0x00;

	XMFLOAT3						m_xmf3AABBCenter = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3						m_xmf3AABBExtents = XMFLOAT3(0.0f, 0.0f, 0.0f);

	D3D12_PRIMITIVE_TOPOLOGY		m_d3dPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	UINT							m_nSlot = 0;
	UINT							m_nOffset = 0;

	int								m_nVertices = 0;

	XMFLOAT3						*m_pxmf3Positions = nullptr;

	ComPtr<ID3D12Resource>			m_pd3dPositionBuffer;
	ComPtr<ID3D12Resource>			m_pd3dPositionUploadBuffer;
	D3D12_VERTEX_BUFFER_VIEW		m_d3dPositionBufferView;

	int								m_nSubMeshes = 0;
	int								*m_pnSubSetIndices = nullptr;
	UINT							**m_ppnSubSetIndices = nullptr;

	VectorComptrResource			m_ppd3dSubSetIndexBuffers;
	VectorComptrResource			m_ppd3dSubSetIndexUploadBuffers;
	vector<D3D12_INDEX_BUFFER_VIEW>	m_pd3dSubSetIndexBufferViews;
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
class CTexturedRectMesh : public CMesh
{
public:
	CTexturedRectMesh(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, float fWidth=20.0f, float fHeight=20.0f, float fDepth=20.0f, float fxPosition=0.0f, float fyPosition=0.0f, float fzPosition=0.0f);
	virtual ~CTexturedRectMesh();

	virtual void ReleaseUploadBuffers();
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, int nSubSet);

protected:
	XMFLOAT2						*m_pxmf2TextureCoords0 = nullptr;

	ComPtr<ID3D12Resource>			m_pd3dTextureCoord0Buffer;
	ComPtr<ID3D12Resource>			m_pd3dTextureCoord0UploadBuffer;
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
//
class CRawFormatImage
{
public:
	CRawFormatImage(LPCTSTR pFileName, int nWidth, int nLength, bool bFlipY = false);
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
	CHeightMapImage(LPCTSTR pFileName, int nWidth, int nLength, XMFLOAT3 xmf3Scale);
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
