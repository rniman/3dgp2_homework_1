//-----------------------------------------------------------------------------
// File: Shader.cpp
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "Shader.h"
#include "Scene.h"

std::random_device rd;
default_random_engine dre(rd());

CShader::CShader()
{
}

CShader::~CShader()
{
	ReleaseShaderVariables();
}

D3D12_SHADER_BYTECODE CShader::CreateVertexShader()
{
	D3D12_SHADER_BYTECODE d3dShaderByteCode;
	d3dShaderByteCode.BytecodeLength = 0;
	d3dShaderByteCode.pShaderBytecode = nullptr;

	return(d3dShaderByteCode);
}

D3D12_SHADER_BYTECODE CShader::CreatePixelShader()
{
	D3D12_SHADER_BYTECODE d3dShaderByteCode;
	d3dShaderByteCode.BytecodeLength = 0;
	d3dShaderByteCode.pShaderBytecode = nullptr;

	return(d3dShaderByteCode);
}

D3D12_SHADER_BYTECODE CShader::CompileShaderFromFile(WCHAR *pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderProfile, ID3DBlob **ppd3dShaderBlob)
{
	UINT nCompileFlags = 0;
#if defined(_DEBUG)
	nCompileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	ID3DBlob *pd3dErrorBlob = nullptr;
	HRESULT hResult = ::D3DCompileFromFile(pszFileName, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, pszShaderName, pszShaderProfile, nCompileFlags, 0, ppd3dShaderBlob, &pd3dErrorBlob);
	char *pErrorString = nullptr;
	if (pd3dErrorBlob) pErrorString = (char *)pd3dErrorBlob->GetBufferPointer();

	D3D12_SHADER_BYTECODE d3dShaderByteCode;
	d3dShaderByteCode.BytecodeLength = (*ppd3dShaderBlob)->GetBufferSize();
	d3dShaderByteCode.pShaderBytecode = (*ppd3dShaderBlob)->GetBufferPointer();

	return(d3dShaderByteCode);
}

D3D12_SHADER_BYTECODE CShader::ReadCompiledShaderFromFile(WCHAR *pszFileName, ID3DBlob **ppd3dShaderBlob)
{
	std::ifstream readFile(pszFileName, std::ios::binary | std::ios::ate);

	if (!readFile)
	{
		assert("파일 열기 실패!");
	}

	std::streamoff fileSize = readFile.tellg();
	readFile.seekg(0, std::ios::beg);

	std::vector<BYTE> byteCode;
	byteCode.reserve(fileSize);
	readFile.read((char*)byteCode.data(), fileSize);
	D3D12_SHADER_BYTECODE d3dByteCode;

	if (ppd3dShaderBlob)
	{
		HRESULT hResult = D3DCreateBlob(fileSize, ppd3dShaderBlob);
		memcpy((*ppd3dShaderBlob)->GetBufferPointer(), byteCode.data(), fileSize);
		d3dByteCode.BytecodeLength = (*ppd3dShaderBlob)->GetBufferSize();
		d3dByteCode.pShaderBytecode = (*ppd3dShaderBlob)->GetBufferPointer();
}
	else
	{
		d3dByteCode.BytecodeLength = fileSize;
		d3dByteCode.pShaderBytecode = byteCode.data();
	}

	return d3dByteCode;
}

D3D12_INPUT_LAYOUT_DESC CShader::CreateInputLayout()
{
	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
	d3dInputLayoutDesc.pInputElementDescs = nullptr;
	d3dInputLayoutDesc.NumElements = 0;

	return(d3dInputLayoutDesc);
}

D3D12_RASTERIZER_DESC CShader::CreateRasterizerState()
{
	D3D12_RASTERIZER_DESC d3dRasterizerDesc;
	::ZeroMemory(&d3dRasterizerDesc, sizeof(D3D12_RASTERIZER_DESC));
	//	d3dRasterizerDesc.FillMode = D3D12_FILL_MODE_WIREFRAME;
	d3dRasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
	d3dRasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	d3dRasterizerDesc.FrontCounterClockwise = FALSE;
	d3dRasterizerDesc.DepthBias = 0;
	d3dRasterizerDesc.DepthBiasClamp = 0.0f;
	d3dRasterizerDesc.SlopeScaledDepthBias = 0.0f;
	d3dRasterizerDesc.DepthClipEnable = TRUE;
	d3dRasterizerDesc.MultisampleEnable = FALSE;
	d3dRasterizerDesc.AntialiasedLineEnable = FALSE;
	d3dRasterizerDesc.ForcedSampleCount = 0;
	d3dRasterizerDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

	return(d3dRasterizerDesc);
}

D3D12_DEPTH_STENCIL_DESC CShader::CreateDepthStencilState()
{
	D3D12_DEPTH_STENCIL_DESC d3dDepthStencilDesc;
	::ZeroMemory(&d3dDepthStencilDesc, sizeof(D3D12_DEPTH_STENCIL_DESC));
	d3dDepthStencilDesc.DepthEnable = TRUE;
	d3dDepthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	d3dDepthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	d3dDepthStencilDesc.StencilEnable = FALSE;
	d3dDepthStencilDesc.StencilReadMask = 0x00;
	d3dDepthStencilDesc.StencilWriteMask = 0x00;
	d3dDepthStencilDesc.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_NEVER;
	d3dDepthStencilDesc.BackFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.BackFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_NEVER;

	return(d3dDepthStencilDesc);
}

D3D12_BLEND_DESC CShader::CreateBlendState()
{
	D3D12_BLEND_DESC d3dBlendDesc;
	::ZeroMemory(&d3dBlendDesc, sizeof(D3D12_BLEND_DESC));
	d3dBlendDesc.AlphaToCoverageEnable = TRUE;
	d3dBlendDesc.IndependentBlendEnable = FALSE;
	d3dBlendDesc.RenderTarget[0].BlendEnable = FALSE;
	d3dBlendDesc.RenderTarget[0].LogicOpEnable = FALSE;
	d3dBlendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_ONE;
	d3dBlendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ZERO;
	d3dBlendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	d3dBlendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	d3dBlendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
	d3dBlendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	d3dBlendDesc.RenderTarget[0].LogicOp = D3D12_LOGIC_OP_NOOP;
	d3dBlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	//d3dBlendDesc.AlphaToCoverageEnable = FALSE;
	//d3dBlendDesc.IndependentBlendEnable = FALSE;
	//d3dBlendDesc.RenderTarget[0].BlendEnable = TRUE;
	//d3dBlendDesc.RenderTarget[0].LogicOpEnable = FALSE;
	//d3dBlendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	//d3dBlendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	////d3dBlendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_DEST_COLOR;
	////d3dBlendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ZERO;
	//d3dBlendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	//d3dBlendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	//d3dBlendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
	//d3dBlendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	//d3dBlendDesc.RenderTarget[0].LogicOp = D3D12_LOGIC_OP_NOOP;
	//d3dBlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	return(d3dBlendDesc);
}

void CShader::CreateShader(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature)
{
	::ZeroMemory(&m_d3dPipelineStateDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));

	m_d3dPipelineStateDesc.pRootSignature = pd3dGraphicsRootSignature;
	m_d3dPipelineStateDesc.VS = CreateVertexShader();
	m_d3dPipelineStateDesc.PS = CreatePixelShader();
	m_d3dPipelineStateDesc.RasterizerState = CreateRasterizerState();
	m_d3dPipelineStateDesc.BlendState = CreateBlendState();
	m_d3dPipelineStateDesc.DepthStencilState = CreateDepthStencilState();
	m_d3dPipelineStateDesc.InputLayout = CreateInputLayout();
	m_d3dPipelineStateDesc.SampleMask = UINT_MAX;
	m_d3dPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	m_d3dPipelineStateDesc.NumRenderTargets = 1;
	m_d3dPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	m_d3dPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	m_d3dPipelineStateDesc.SampleDesc.Count = 1;
	m_d3dPipelineStateDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;

	HRESULT hResult = pd3dDevice->CreateGraphicsPipelineState(&m_d3dPipelineStateDesc, __uuidof(ID3D12PipelineState), (void **)&m_ppd3dPipelineStates[0]);
}

void CShader::OnPrepareRender(ID3D12GraphicsCommandList *pd3dCommandList, int nPipelineState)
{
	if (m_ppd3dPipelineStates[nPipelineState].Get())
	{
		pd3dCommandList->SetPipelineState(m_ppd3dPipelineStates[nPipelineState].Get());
	}
}

void CShader::Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera, int nPipelineState)
{
	OnPrepareRender(pd3dCommandList, nPipelineState);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CSkyBoxShader::CSkyBoxShader()
{
}

CSkyBoxShader::~CSkyBoxShader()
{
}

D3D12_INPUT_LAYOUT_DESC CSkyBoxShader::CreateInputLayout()
{
	UINT nInputElementDescs = 1;
	D3D12_INPUT_ELEMENT_DESC *pd3dInputElementDescs = new D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];

	pd3dInputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
	d3dInputLayoutDesc.pInputElementDescs = pd3dInputElementDescs;
	d3dInputLayoutDesc.NumElements = nInputElementDescs;

	return(d3dInputLayoutDesc);
}

D3D12_DEPTH_STENCIL_DESC CSkyBoxShader::CreateDepthStencilState()
{
	D3D12_DEPTH_STENCIL_DESC d3dDepthStencilDesc;
	d3dDepthStencilDesc.DepthEnable = FALSE;
	d3dDepthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	d3dDepthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_NEVER;
	d3dDepthStencilDesc.StencilEnable = FALSE;
	d3dDepthStencilDesc.StencilReadMask = 0xff;
	d3dDepthStencilDesc.StencilWriteMask = 0xff;
	d3dDepthStencilDesc.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_INCR;
	d3dDepthStencilDesc.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	d3dDepthStencilDesc.BackFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_DECR;
	d3dDepthStencilDesc.BackFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;

	return(d3dDepthStencilDesc);
}

D3D12_SHADER_BYTECODE CSkyBoxShader::CreateVertexShader()
{
	return CShader::ReadCompiledShaderFromFile(L"./../Debug/VSSkyBox.cso", &m_pd3dVertexShaderBlob);
}

D3D12_SHADER_BYTECODE CSkyBoxShader::CreatePixelShader()
{
	return CShader::ReadCompiledShaderFromFile(L"./../Debug/PSSkyBox.cso", &m_pd3dPixelShaderBlob);
}

void CSkyBoxShader::CreateShader(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature)
{
	m_nPipelineStates = 1;
	m_ppd3dPipelineStates.reserve(m_nPipelineStates);

	for (int i = 0; i < m_nPipelineStates; ++i)
	{
		m_ppd3dPipelineStates.emplace_back(ComPtr<ID3D12PipelineState>());
	}

	CShader::CreateShader(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature);

	if (m_pd3dVertexShaderBlob) m_pd3dVertexShaderBlob->Release();
	if (m_pd3dPixelShaderBlob) m_pd3dPixelShaderBlob->Release();

	if (m_d3dPipelineStateDesc.InputLayout.pInputElementDescs) delete[] m_d3dPipelineStateDesc.InputLayout.pInputElementDescs;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CStandardShader::CStandardShader()
{
}

CStandardShader::~CStandardShader()
{
}

D3D12_INPUT_LAYOUT_DESC CStandardShader::CreateInputLayout()
{
	UINT nInputElementDescs = 5;
	D3D12_INPUT_ELEMENT_DESC *pd3dInputElementDescs = new D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];

	pd3dInputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[1] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 1, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[2] = { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 2, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[3] = { "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 3, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[4] = { "BITANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 4, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
	d3dInputLayoutDesc.pInputElementDescs = pd3dInputElementDescs;
	d3dInputLayoutDesc.NumElements = nInputElementDescs;

	return(d3dInputLayoutDesc);
}

D3D12_SHADER_BYTECODE CStandardShader::CreateVertexShader()
{
	return CShader::ReadCompiledShaderFromFile(L"./../Debug/VSStandard.cso", &m_pd3dVertexShaderBlob);
}

D3D12_SHADER_BYTECODE CStandardShader::CreatePixelShader()
{
	return CShader::ReadCompiledShaderFromFile(L"./../Debug/PSStandard.cso", &m_pd3dPixelShaderBlob);
}

void CStandardShader::CreateShader(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature)
{
	m_nPipelineStates = 1;
	m_ppd3dPipelineStates.reserve(m_nPipelineStates);

	for (int i = 0; i < m_nPipelineStates; ++i)
	{
		m_ppd3dPipelineStates.emplace_back(ComPtr<ID3D12PipelineState>());
	}

	CShader::CreateShader(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature);

	if (m_pd3dVertexShaderBlob) m_pd3dVertexShaderBlob->Release();
	if (m_pd3dPixelShaderBlob) m_pd3dPixelShaderBlob->Release();

	if (m_d3dPipelineStateDesc.InputLayout.pInputElementDescs) delete[] m_d3dPipelineStateDesc.InputLayout.pInputElementDescs;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CObjectsShader::CObjectsShader()
{
	m_nObjects = 1;
}

CObjectsShader::~CObjectsShader()
{
}

float Random(float fMin, float fMax)
{
	float fRandomValue = (float)rand();
	if (fRandomValue < fMin) fRandomValue = fMin;
	if (fRandomValue > fMax) fRandomValue = fMax;
	return(fRandomValue);
}

float Random()
{
	return(rand() / float(RAND_MAX));
}

XMFLOAT3 RandomPositionInSphere(XMFLOAT3 xmf3Center, float fRadius, int nColumn, int nColumnSpace)
{
    float fAngle = Random() * 360.0f * (2.0f * 3.14159f / 360.0f);

	XMFLOAT3 xmf3Position;
    xmf3Position.x = xmf3Center.x + fRadius * sin(fAngle);
    //xmf3Position.y = xmf3Center.y - (nColumn * float(nColumnSpace) / 2.0f) + (nColumn * nColumnSpace) + Random();
	xmf3Position.y = 1000.0f;
    xmf3Position.z = xmf3Center.z + fRadius * cos(fAngle);

	return(xmf3Position);
}

void CObjectsShader::BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature, void *pContext)
{
	m_ppObjects = new CGameObject*[m_nObjects];

	CGameObject *pSuperCobraModel = CGameObject::LoadGeometryFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/SuperCobra.bin", this);
	CGameObject* pGunshipModel = CGameObject::LoadGeometryFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/Gunship.bin", this);
	//CGameObject* pMissileModel = CGameObject::LoadGeometryFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/hellfire.bin", this);
	
	int nColumnSpace = 5, nColumnSize = 30;           
    int nFirstPassColumnSize = (m_nObjects % nColumnSize) > 0 ? (nColumnSize - 1) : nColumnSize;

  //  for (int h = 0; h < nFirstPassColumnSize; h++)
  //  {
  //      for (int i = 0; i < floor(float(m_nObjects) / float(nColumnSize)); i++)
  //      {
		//	if (nObjects % 2)
		//	{
		//		m_ppObjects[nObjects] = new CSuperCobraObject(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature);
		//		m_ppObjects[nObjects]->SetChild(pSuperCobraModel);
		//		m_ppObjects[nObjects]->PrepareOOBB();
		//		pSuperCobraModel->AddRef();
		//	}
		//	else
		//	{
		//		m_ppObjects[nObjects] = new CGunshipObject(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature);
		//		m_ppObjects[nObjects]->SetChild(pGunshipModel);
		//		m_ppObjects[nObjects]->PrepareOOBB();
		//		pGunshipModel->AddRef();
		//	}
		//	XMFLOAT3 xmf3RandomPosition = RandomPositionInSphere(XMFLOAT3(920.0f, 0.0f, 1200.0f), Random(20.0f, 150.0f), h - int(floor(nColumnSize / 2.0f)), nColumnSpace);
		//	m_ppObjects[nObjects]->SetPosition(xmf3RandomPosition.x, xmf3RandomPosition.y + 750.0f, xmf3RandomPosition.z);
		//	m_ppObjects[nObjects]->Rotate(0.0f, 90.0f, 0.0f);
		//	m_ppObjects[nObjects++]->PrepareAnimate();
		//}
  //  }
	CGameObject* pMissileModel = CGameObject::LoadGeometryFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/hellfire.bin", this);
	int nObjects = 0;
	std::uniform_int_distribution<int> uid(20, 150);
	for (int i = 0; i < m_nObjects; ++i)
	{
		m_ppObjects[i] = new CGunshipObject(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, pMissileModel, pContext);
		m_ppObjects[i]->SetChild(pGunshipModel);
		m_ppObjects[i]->PrepareOOBB();
		pGunshipModel->AddRef();

		XMFLOAT3 xmf3RandomPosition = RandomPositionInSphere(XMFLOAT3(2560.0f, 0.0f, 2560.0f), 2560.0f, 0, 0);
		m_ppObjects[nObjects]->SetPosition(xmf3RandomPosition.x, xmf3RandomPosition.y, xmf3RandomPosition.z);
		m_ppObjects[nObjects]->Rotate(0.0f, 90.0f, 0.0f);
		m_ppObjects[nObjects++]->PrepareAnimate();
	}

   // if (nFirstPassColumnSize != nColumnSize)
   // {
   //     for (int i = 0; i < m_nObjects - int(floor(float(m_nObjects) / float(nColumnSize)) * nFirstPassColumnSize); i++)
   //     {
			//if (nObjects % 2)
			//{
			//	m_ppObjects[nObjects] = new CSuperCobraObject(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature);
			//	m_ppObjects[nObjects]->SetChild(pSuperCobraModel);
			//	m_ppObjects[nObjects]->PrepareOOBB();
			//	pSuperCobraModel->AddRef();
			//}
			//else
			//{
			//	m_ppObjects[nObjects] = new CGunshipObject(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature);
			//	m_ppObjects[nObjects]->SetChild(pGunshipModel);
			//	m_ppObjects[nObjects]->PrepareOOBB();
			//	pGunshipModel->AddRef();
			//}
			//XMFLOAT3 xmf3RandomPosition = RandomPositionInSphere(XMFLOAT3(920.0f, 0.0f, 1200.0f), Random(20.0f, 150.0f), nColumnSize - int(floor(nColumnSize / 2.0f)), nColumnSpace);
			//m_ppObjects[nObjects]->SetPosition(xmf3RandomPosition.x, xmf3RandomPosition.y + 850.0f, xmf3RandomPosition.z);
			//m_ppObjects[nObjects]->Rotate(0.0f, 90.0f, 0.0f);
			//m_ppObjects[nObjects++]->PrepareAnimate();
   //     }
   // }

	CreateShaderVariables(pd3dDevice, pd3dCommandList);
}

void CObjectsShader::ReleaseObjects()
{
	if (m_ppObjects)
	{
		for (int j = 0; j < m_nObjects; j++) if (m_ppObjects[j]) m_ppObjects[j]->Release();
		delete[] m_ppObjects;
	}
}

void CObjectsShader::AnimateObjects(float fTimeElapsed)
{
	for (int j = 0; j < m_nObjects; j++)
	{
		if (m_ppObjects[j])
		{
			m_ppObjects[j]->Animate(fTimeElapsed);
		}
	}
}

void CObjectsShader::ReleaseUploadBuffers()
{
	for (int j = 0; j < m_nObjects; j++) if (m_ppObjects[j]) m_ppObjects[j]->ReleaseUploadBuffers();
}

void CObjectsShader::Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera, int nPipelineState)
{
	CShader::Render(pd3dCommandList, pCamera, nPipelineState);

	for (int j = 0; j < m_nObjects; j++)
	{
		if (m_ppObjects[j])
		{
			m_ppObjects[j]->UpdateTransform();
			m_ppObjects[j]->Render(pd3dCommandList, pCamera);
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CPlayerShader::CPlayerShader()
{
}

CPlayerShader::~CPlayerShader()
{
}

D3D12_INPUT_LAYOUT_DESC CPlayerShader::CreateInputLayout()
{
	UINT nInputElementDescs = 5;
	D3D12_INPUT_ELEMENT_DESC *pd3dInputElementDescs = new D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];

	pd3dInputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[1] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 1, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[2] = { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 2, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[3] = { "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 3, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[4] = { "BITANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 4, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
	d3dInputLayoutDesc.pInputElementDescs = pd3dInputElementDescs;
	d3dInputLayoutDesc.NumElements = nInputElementDescs;

	return(d3dInputLayoutDesc);
}

D3D12_SHADER_BYTECODE CPlayerShader::CreateVertexShader()
{
	return CShader::ReadCompiledShaderFromFile(L"./../Debug/VSStandard.cso", &m_pd3dVertexShaderBlob);
}

D3D12_SHADER_BYTECODE CPlayerShader::CreatePixelShader()
{
	return CShader::ReadCompiledShaderFromFile(L"./../Debug/PSStandard.cso", &m_pd3dPixelShaderBlob);
}

void CPlayerShader::CreateShader(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature)
{
	m_nPipelineStates = 1;
	m_ppd3dPipelineStates.reserve(m_nPipelineStates);

	for (int i = 0; i < m_nPipelineStates; ++i)
	{
		m_ppd3dPipelineStates.emplace_back(ComPtr<ID3D12PipelineState>());
	}

	CShader::CreateShader(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature);

	if (m_pd3dVertexShaderBlob) m_pd3dVertexShaderBlob->Release();
	if (m_pd3dPixelShaderBlob) m_pd3dPixelShaderBlob->Release();

	if (m_d3dPipelineStateDesc.InputLayout.pInputElementDescs)
	{
		delete[] m_d3dPipelineStateDesc.InputLayout.pInputElementDescs;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CTerrainShader::CTerrainShader()
{
}

CTerrainShader::~CTerrainShader()
{
}

D3D12_INPUT_LAYOUT_DESC CTerrainShader::CreateInputLayout()
{
	UINT nInputElementDescs = 4;
	D3D12_INPUT_ELEMENT_DESC* pd3dInputElementDescs = new D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];

	pd3dInputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[1] = { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[2] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 2, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[3] = { "TEXCOORD", 1, DXGI_FORMAT_R32G32_FLOAT, 3, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
	d3dInputLayoutDesc.pInputElementDescs = pd3dInputElementDescs;
	d3dInputLayoutDesc.NumElements = nInputElementDescs;

	return(d3dInputLayoutDesc);
}

D3D12_SHADER_BYTECODE CTerrainShader::CreateVertexShader()
{
	return CShader::ReadCompiledShaderFromFile(L"./../Debug/VSTerrain.cso", &m_pd3dVertexShaderBlob);
}

D3D12_SHADER_BYTECODE CTerrainShader::CreatePixelShader()
{
	return CShader::ReadCompiledShaderFromFile(L"./../Debug/PSTerrain.cso", &m_pd3dPixelShaderBlob);
}

void CTerrainShader::CreateShader(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature)
{
	m_nPipelineStates = 1;
	m_ppd3dPipelineStates.reserve(m_nPipelineStates);

	for (int i = 0; i < m_nPipelineStates; ++i)
	{
		m_ppd3dPipelineStates.emplace_back(ComPtr<ID3D12PipelineState>());
	}

	CShader::CreateShader(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature);

	if (m_pd3dVertexShaderBlob) m_pd3dVertexShaderBlob->Release();
	if (m_pd3dPixelShaderBlob) m_pd3dPixelShaderBlob->Release();

	if (m_d3dPipelineStateDesc.InputLayout.pInputElementDescs) delete[] m_d3dPipelineStateDesc.InputLayout.pInputElementDescs;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CTransparentOjectsShader::CTransparentOjectsShader()
{
	m_nObjects = 1;
}

CTransparentOjectsShader::~CTransparentOjectsShader()
{
}

D3D12_INPUT_LAYOUT_DESC CTransparentOjectsShader::CreateInputLayout()
{
	UINT nInputElementDescs = 2;
	D3D12_INPUT_ELEMENT_DESC* pd3dInputElementDescs = new D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];

	pd3dInputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[1] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 1, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
	d3dInputLayoutDesc.pInputElementDescs = pd3dInputElementDescs;
	d3dInputLayoutDesc.NumElements = nInputElementDescs;

	return(d3dInputLayoutDesc);
}

D3D12_SHADER_BYTECODE CTransparentOjectsShader::CreateVertexShader()
{
	return CShader::ReadCompiledShaderFromFile(L"./../Debug/VSOcean.cso", &m_pd3dVertexShaderBlob);
}

D3D12_SHADER_BYTECODE CTransparentOjectsShader::CreatePixelShader()
{
	return CShader::ReadCompiledShaderFromFile(L"./../Debug/PSOcean.cso", &m_pd3dPixelShaderBlob);
}

D3D12_RASTERIZER_DESC CTransparentOjectsShader::CreateRasterizerState()
{
	D3D12_RASTERIZER_DESC d3dRasterizerDesc;
	::ZeroMemory(&d3dRasterizerDesc, sizeof(D3D12_RASTERIZER_DESC));
	d3dRasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
	//	d3dRasterizerDesc.CullMode = D3D12_CULL_MODE_NONE;
	d3dRasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	d3dRasterizerDesc.FrontCounterClockwise = FALSE;
	d3dRasterizerDesc.DepthBias = 0;
	d3dRasterizerDesc.DepthBiasClamp = 0.0f;
	d3dRasterizerDesc.SlopeScaledDepthBias = 0.0f;
	d3dRasterizerDesc.DepthClipEnable = TRUE;
	d3dRasterizerDesc.MultisampleEnable = FALSE;
	d3dRasterizerDesc.AntialiasedLineEnable = FALSE;
	d3dRasterizerDesc.ForcedSampleCount = 0;
	d3dRasterizerDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

	return(d3dRasterizerDesc);
}

D3D12_BLEND_DESC CTransparentOjectsShader::CreateBlendState()
{
	D3D12_BLEND_DESC d3dBlendDesc;
	::ZeroMemory(&d3dBlendDesc, sizeof(D3D12_BLEND_DESC));
	d3dBlendDesc.AlphaToCoverageEnable = TRUE;
	d3dBlendDesc.IndependentBlendEnable = FALSE;
	d3dBlendDesc.RenderTarget[0].BlendEnable = TRUE;
	d3dBlendDesc.RenderTarget[0].LogicOpEnable = FALSE;
	d3dBlendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_INV_SRC_ALPHA;
	d3dBlendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_SRC_ALPHA;
	d3dBlendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	d3dBlendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	d3dBlendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
	d3dBlendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	d3dBlendDesc.RenderTarget[0].LogicOp = D3D12_LOGIC_OP_NOOP;
	d3dBlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	return(d3dBlendDesc);
}

D3D12_DEPTH_STENCIL_DESC CTransparentOjectsShader::CreateDepthStencilState()
{
	D3D12_DEPTH_STENCIL_DESC d3dDepthStencilDesc;
	::ZeroMemory(&d3dDepthStencilDesc, sizeof(D3D12_DEPTH_STENCIL_DESC));
	d3dDepthStencilDesc.DepthEnable = TRUE;
	d3dDepthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	d3dDepthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	d3dDepthStencilDesc.StencilEnable = FALSE;
	d3dDepthStencilDesc.StencilReadMask = 0x00;
	d3dDepthStencilDesc.StencilWriteMask = 0x00;
	d3dDepthStencilDesc.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_NEVER;
	d3dDepthStencilDesc.BackFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.BackFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_NEVER;

	return(d3dDepthStencilDesc);
}

void CTransparentOjectsShader::CreateShader(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature)
{
	m_nPipelineStates = 1;
	m_ppd3dPipelineStates.reserve(m_nPipelineStates);

	for (int i = 0; i < m_nPipelineStates; ++i)
	{
		m_ppd3dPipelineStates.emplace_back(ComPtr<ID3D12PipelineState>());
	}

	CShader::CreateShader(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature);

	if (m_pd3dVertexShaderBlob) m_pd3dVertexShaderBlob->Release();
	if (m_pd3dPixelShaderBlob) m_pd3dPixelShaderBlob->Release();

	if (m_d3dPipelineStateDesc.InputLayout.pInputElementDescs) delete[] m_d3dPipelineStateDesc.InputLayout.pInputElementDescs;
}

void CTransparentOjectsShader::BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, void* pContext)
{
	m_ppObjects = new CGameObject* [m_nObjects];
	COceanObjcet* floor = new COceanObjcet(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, pContext);
	m_ppObjects[0] = floor;
	
	CreateShaderVariables(pd3dDevice, pd3dCommandList);
}

void CTransparentOjectsShader::ReleaseObjects()
{
	if (m_ppObjects)
	{
		for (int j = 0; j < m_nObjects; j++) if (m_ppObjects[j]) m_ppObjects[j]->Release();
		delete[] m_ppObjects;
	}
}

void CTransparentOjectsShader::AnimateObjects(float fTimeElapsed)
{
	for (int j = 0; j < m_nObjects; j++)
	{
		if (m_ppObjects[j])
		{
			m_ppObjects[j]->Animate(fTimeElapsed);
		}
	}
}

void CTransparentOjectsShader::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera, int nPipelineState)
{
	CShader::Render(pd3dCommandList, pCamera, nPipelineState);

	for (int j = 0; j < m_nObjects; j++)
	{
		if (m_ppObjects[j])
		{
			m_ppObjects[j]->UpdateTransform();
			m_ppObjects[j]->Render(pd3dCommandList, pCamera);
		}
	}
}

void CTransparentOjectsShader::ReleaseUploadBuffers()
{
	for (int j = 0; j < m_nObjects; j++) 
	{
		if (m_ppObjects[j]) m_ppObjects[j]->ReleaseUploadBuffers();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CBillBoardObjectsShader::CBillBoardObjectsShader(LPCTSTR pFileName, int nWidth, int nLength, XMFLOAT3 xmf3Scale)
{
	m_pHeightMapImage = new CHeightMapImage(pFileName, nWidth, nLength, xmf3Scale, true);

	//m_nObjects = 10;
}

CBillBoardObjectsShader::~CBillBoardObjectsShader()
{
	//if (m_pHeightMapImage) delete m_pHeightMapImage;
}

D3D12_INPUT_LAYOUT_DESC CBillBoardObjectsShader::CreateInputLayout()
{
	UINT nInputElementDescs = 6;
	D3D12_INPUT_ELEMENT_DESC* pd3dInputElementDescs = new D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];

	pd3dInputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[1] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 1, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

	pd3dInputElementDescs[2] = { "WORLDMATRIX", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 2, 0, D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1 };
	pd3dInputElementDescs[3] = { "WORLDMATRIX", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 2, 16, D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1 };
	pd3dInputElementDescs[4] = { "WORLDMATRIX", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 2, 32, D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1 };
	pd3dInputElementDescs[5] = { "WORLDMATRIX", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 2, 48, D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1 };

	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
	d3dInputLayoutDesc.pInputElementDescs = pd3dInputElementDescs;
	d3dInputLayoutDesc.NumElements = nInputElementDescs;

	return(d3dInputLayoutDesc);
}

D3D12_SHADER_BYTECODE CBillBoardObjectsShader::CreateVertexShader()
{
	return CShader::ReadCompiledShaderFromFile(L"./../Debug/VSBillboard.cso", &m_pd3dVertexShaderBlob);
}

D3D12_SHADER_BYTECODE CBillBoardObjectsShader::CreatePixelShader()
{
	return CShader::ReadCompiledShaderFromFile(L"./../Debug/PSTextured.cso", &m_pd3dPixelShaderBlob);
}

D3D12_BLEND_DESC CBillBoardObjectsShader::CreateBlendState()
{
	D3D12_BLEND_DESC d3dBlendDesc;
	::ZeroMemory(&d3dBlendDesc, sizeof(D3D12_BLEND_DESC));
	d3dBlendDesc.AlphaToCoverageEnable = TRUE;
	d3dBlendDesc.IndependentBlendEnable = FALSE;
	d3dBlendDesc.RenderTarget[0].BlendEnable = TRUE;
	d3dBlendDesc.RenderTarget[0].LogicOpEnable = FALSE;
	d3dBlendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	d3dBlendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	d3dBlendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	d3dBlendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	d3dBlendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
	d3dBlendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	d3dBlendDesc.RenderTarget[0].LogicOp = D3D12_LOGIC_OP_NOOP;
	d3dBlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	return(d3dBlendDesc);
}

void CBillBoardObjectsShader::CreateShader(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature)
{
	m_nPipelineStates = 1;
	m_ppd3dPipelineStates.reserve(m_nPipelineStates);

	for (int i = 0; i < m_nPipelineStates; ++i)
	{
		m_ppd3dPipelineStates.emplace_back(ComPtr<ID3D12PipelineState>());
	}

	CShader::CreateShader(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature);

	if (m_pd3dVertexShaderBlob) m_pd3dVertexShaderBlob->Release();
	if (m_pd3dPixelShaderBlob) m_pd3dPixelShaderBlob->Release();

	if (m_d3dPipelineStateDesc.InputLayout.pInputElementDescs) delete[] m_d3dPipelineStateDesc.InputLayout.pInputElementDescs;
}

void CBillBoardObjectsShader::CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
	m_pd3dcbGameObjects = ::CreateBufferResource(pd3dDevice, pd3dCommandList, nullptr, sizeof(VS_VB_INSTANCE) * m_nObjects, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, nullptr);

	m_pd3dcbGameObjects->Map(0, nullptr, (void**)&m_pcbMappedGameObjects);

	m_d3dInstancingBufferView.BufferLocation = m_pd3dcbGameObjects->GetGPUVirtualAddress();
	m_d3dInstancingBufferView.StrideInBytes = sizeof(VS_VB_INSTANCE);
	m_d3dInstancingBufferView.SizeInBytes = sizeof(VS_VB_INSTANCE) * m_nObjects;
}

void CBillBoardObjectsShader::UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList)
{
	for (int i = 0, j = 0; j < m_nObjectsType; j++)
	{
		for (auto& gameObject : m_ppObjects[j])
		{
			//gameObject.SetLookAt(m_pCamera->GetPosition(), XMFLOAT3(0.0f, 1.0f, 0.0f));
			gameObject.UpdateTransform();
			XMStoreFloat4x4(&m_pcbMappedGameObjects[i++].m_xmf4x4Transform, XMMatrixTranspose(XMLoadFloat4x4(&gameObject.GetWorldTransform())));
		}
	}
}

void CBillBoardObjectsShader::BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, void* pContext)
{
	std::uniform_int_distribution<int> random_int(0, 100);

	CHeightMapTerrain* pTerrain = (CHeightMapTerrain*)pContext;

	XMFLOAT3 scale = pTerrain->GetScale();
	float width = m_pHeightMapImage->GetRawImageWidth();
	float length = m_pHeightMapImage->GetRawImageLength();

	int nGrassObjects = 0;
	for (int x = 0; x < width; x++)
	{
		for (int z = 0; z < length; z++)
		{
			BYTE nPixel = m_pHeightMapImage->GetRawImagePixel(x, z);
			switch (nPixel)
			{
			case 255:
				nGrassObjects++;
				break;
			case 200:
				nGrassObjects++;
				break;
			case 131:
				nGrassObjects++;
				break;
			case 23:
				nGrassObjects++;
				break;
			default: break;
			}
		}
	}
	m_nObjectsType = 6;

	//std::vector<std::vector<CGameObject>> ppObjects(5);	
	m_ppObjects.resize(m_nObjectsType);
	for (auto& gameObject : m_ppObjects)
		gameObject.reserve(nGrassObjects / 6);
	
	CTexturedRectMesh* grassMesh = new CTexturedRectMesh(pd3dDevice, pd3dCommandList, 10.0f, 10.0f, 0.0f);
	CTexturedRectMesh* treeMesh = new CTexturedRectMesh(pd3dDevice, pd3dCommandList, 120.0f, 120.0f, 0.0f);

	CTexture* pTextures[6];
	pTextures[0] = new CTexture(1, RESOURCE_TEXTURE2D, 0, 1);
	pTextures[0]->LoadTextureFromDDSFile(pd3dDevice, pd3dCommandList, L"Billboard/Grass01.dds", RESOURCE_TEXTURE2D, 0);
	CScene::CreateShaderResourceViews(pd3dDevice, pTextures[0], 0, 3);

	pTextures[1] = new CTexture(1, RESOURCE_TEXTURE2D, 0, 1);
	pTextures[1]->LoadTextureFromDDSFile(pd3dDevice, pd3dCommandList, L"Billboard/Grass02.dds", RESOURCE_TEXTURE2D, 0);
	CScene::CreateShaderResourceViews(pd3dDevice, pTextures[1], 0, 3);

	pTextures[2] = new CTexture(1, RESOURCE_TEXTURE2D, 0, 1);
	pTextures[2]->LoadTextureFromDDSFile(pd3dDevice, pd3dCommandList, L"Billboard/pngegg.dds", RESOURCE_TEXTURE2D, 0);
	CScene::CreateShaderResourceViews(pd3dDevice, pTextures[2], 0, 3);

	pTextures[3] = new CTexture(1, RESOURCE_TEXTURE2D, 0, 1);
	pTextures[3]->LoadTextureFromDDSFile(pd3dDevice, pd3dCommandList, L"Billboard/dryGrass.dds", RESOURCE_TEXTURE2D, 0);
	CScene::CreateShaderResourceViews(pd3dDevice, pTextures[3], 0, 3);

	pTextures[4] = new CTexture(1, RESOURCE_TEXTURE2D, 0, 1);
	pTextures[4]->LoadTextureFromDDSFile(pd3dDevice, pd3dCommandList, L"Billboard/dryTree.dds", RESOURCE_TEXTURE2D, 0);
	CScene::CreateShaderResourceViews(pd3dDevice, pTextures[4], 0, 3);

	pTextures[5] = new CTexture(1, RESOURCE_TEXTURE2D, 0, 1);
	pTextures[5]->LoadTextureFromDDSFile(pd3dDevice, pd3dCommandList, L"Billboard/dryTree2.dds", RESOURCE_TEXTURE2D, 0);
	CScene::CreateShaderResourceViews(pd3dDevice, pTextures[5], 0, 3);

	CMaterial* pMaterials[6];
	pMaterials[0] = new CMaterial();
	pMaterials[0]->SetTexture(pTextures[0]);
	pMaterials[1] = new CMaterial();
	pMaterials[1]->SetTexture(pTextures[1]);
	pMaterials[2] = new CMaterial();
	pMaterials[2]->SetTexture(pTextures[2]);
	pMaterials[3] = new CMaterial();
	pMaterials[3]->SetTexture(pTextures[3]);
	pMaterials[4] = new CMaterial();
	pMaterials[4]->SetTexture(pTextures[4]);
	pMaterials[5] = new CMaterial();
	pMaterials[5]->SetTexture(pTextures[5]);


	int i = 0;
	int nTypeDryTexture;
	for (int x = 0; x < width; ++x)
	{
		for (int z = 0; z < length; ++z)
		{
			CMesh* pMesh = NULL;
			int nMaterialType;
			float fyOffset;
			switch (m_pHeightMapImage->GetRawImagePixel(x,z))
			{
			case 255:
				if (random_int(dre) < 95)
					continue;
				
				nTypeDryTexture = random_int(dre);
				if (nTypeDryTexture < 5)
				{
					pMesh = treeMesh;
					nMaterialType = 4;
					fyOffset = 120.0f * 0.30f;
				}
				else if (nTypeDryTexture < 10)
				{
					pMesh = grassMesh;
					nMaterialType = 5;
					fyOffset = 120.0f * 0.40f;
				}
				else if (nTypeDryTexture < 50)
				{
					pMesh = grassMesh;
					nMaterialType = 2;
					fyOffset = -10.0f * 0.01f;
				}
				else
				{
					pMesh = grassMesh;
					nMaterialType = 3;
					fyOffset = 10.0f * 0.3f;
				}
				break;
			case 200:
				pMesh = grassMesh;
				if(random_int(dre) < 30)
				{
					nMaterialType = 0;
					fyOffset = 10.0f * 0.3f;
				}
				else
				{
					nMaterialType = 1;
					fyOffset = 10.0f * 0.4f;
				}				
				break;
			case 131:
				if (random_int(dre) < 95)
					continue;

				nTypeDryTexture = random_int(dre);
				if (nTypeDryTexture < 10)
				{
					pMesh = treeMesh;
					nMaterialType = 4;
					fyOffset = 120.0f * 0.30f;
				}
				else if (nTypeDryTexture < 20)
				{
					pMesh = treeMesh;
					nMaterialType = 5;
					fyOffset = 120.0f * 0.40f;
				}
				else
				{
					pMesh = grassMesh;
					nMaterialType = 3;
					fyOffset = 10.0f * 0.3f;
				}
				break;
			case 23:
				pMesh = grassMesh;
				if (random_int(dre) < 70)
				{
					nMaterialType = 0;
					fyOffset = 10.0f * 0.3f;
				}
				else
				{
					nMaterialType = 1;
					fyOffset = 10.0f * 0.4f;
				}
				break;
			default:
				break;
			}

			if (pMesh)
			{
				float xPosition = x * scale.x;
				float zPosition = z * scale.z;
				float fHeight = pTerrain->GetHeight(xPosition, zPosition);
				if (fHeight < 80.0f)
					continue;

				m_ppObjects[nMaterialType].emplace_back(1,1);
				m_ppObjects[nMaterialType].rbegin()->SetMesh(0, pMesh);
				m_ppObjects[nMaterialType].rbegin()->SetMaterial(0, pMaterials[nMaterialType]);
				m_ppObjects[nMaterialType].rbegin()->SetPosition(xPosition, fHeight + fyOffset, zPosition);
			}
		}
	}

	for (auto& objects : m_ppObjects)
	{
		m_nObjects += objects.size();
	}

	CreateShaderVariables(pd3dDevice, pd3dCommandList);
}

void CBillBoardObjectsShader::ReleaseObjects()
{
	CShader::ReleaseObjects();

	if (m_pHeightMapImage) delete m_pHeightMapImage;

	//if (m_ppObjects)
	//{
	//	for (int j = 0; j < m_nObjects; j++) 
	//	{
	//		if (m_ppObjects[j]) 
	//		{
	//			m_ppObjects[j]->Release();
	//		}
	//	}
	//	delete[] m_ppObjects;
	//}
}

void CBillBoardObjectsShader::AnimateObjects(float fTimeElapsed)
{
}

void CBillBoardObjectsShader::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera, int nPipelineState)
{
	CShader::Render(pd3dCommandList, pCamera, nPipelineState);

	UpdateShaderVariables(pd3dCommandList);

	int nStartInstance = 0;
	for (auto& objects : m_ppObjects)
	{
		objects[0].RenderInstance(pd3dCommandList, pCamera, objects.size(), m_d3dInstancingBufferView, nStartInstance);
		nStartInstance += objects.size();
	}
}

void CBillBoardObjectsShader::ReleaseUploadBuffers()
{
	for (auto& objects : m_ppObjects)
	{
		objects[0].ReleaseUploadBuffers();
	}	
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CSpriteObjectsShader::CSpriteObjectsShader()
{
}

CSpriteObjectsShader::~CSpriteObjectsShader()
{
}

D3D12_INPUT_LAYOUT_DESC CSpriteObjectsShader::CreateInputLayout()
{
	UINT nInputElementDescs = 2;
	D3D12_INPUT_ELEMENT_DESC* pd3dInputElementDescs = new D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];

	pd3dInputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[1] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 1, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
	d3dInputLayoutDesc.pInputElementDescs = pd3dInputElementDescs;
	d3dInputLayoutDesc.NumElements = nInputElementDescs;

	return(d3dInputLayoutDesc);
}

D3D12_SHADER_BYTECODE CSpriteObjectsShader::CreateVertexShader()
{
	return CShader::ReadCompiledShaderFromFile(L"./../Debug/VSSpriteAnimation.cso", &m_pd3dVertexShaderBlob);
}

D3D12_SHADER_BYTECODE CSpriteObjectsShader::CreatePixelShader()
{
	return CShader::ReadCompiledShaderFromFile(L"./../Debug/PSTextured.cso", &m_pd3dPixelShaderBlob);
}

D3D12_RASTERIZER_DESC CSpriteObjectsShader::CreateRasterizerState()
{
	D3D12_RASTERIZER_DESC d3dRasterizerDesc;
	::ZeroMemory(&d3dRasterizerDesc, sizeof(D3D12_RASTERIZER_DESC));
	d3dRasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
	//	d3dRasterizerDesc.CullMode = D3D12_CULL_MODE_NONE;
	d3dRasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	d3dRasterizerDesc.FrontCounterClockwise = FALSE;
	d3dRasterizerDesc.DepthBias = 0;
	d3dRasterizerDesc.DepthBiasClamp = 0.0f;
	d3dRasterizerDesc.SlopeScaledDepthBias = 0.0f;
	d3dRasterizerDesc.DepthClipEnable = TRUE;
	d3dRasterizerDesc.MultisampleEnable = FALSE;
	d3dRasterizerDesc.AntialiasedLineEnable = FALSE;
	d3dRasterizerDesc.ForcedSampleCount = 0;
	d3dRasterizerDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

	return(d3dRasterizerDesc);
}

D3D12_BLEND_DESC CSpriteObjectsShader::CreateBlendState()
{
	D3D12_BLEND_DESC d3dBlendDesc;
	::ZeroMemory(&d3dBlendDesc, sizeof(D3D12_BLEND_DESC));
	d3dBlendDesc.AlphaToCoverageEnable = TRUE;
	d3dBlendDesc.IndependentBlendEnable = FALSE;
	d3dBlendDesc.RenderTarget[0].BlendEnable = TRUE;
	d3dBlendDesc.RenderTarget[0].LogicOpEnable = FALSE;
	d3dBlendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	d3dBlendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	d3dBlendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	d3dBlendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	d3dBlendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
	d3dBlendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	d3dBlendDesc.RenderTarget[0].LogicOp = D3D12_LOGIC_OP_NOOP;
	d3dBlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	return(d3dBlendDesc);
}

void CSpriteObjectsShader::CreateShader(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature)
{
	m_nPipelineStates = 1;
	m_ppd3dPipelineStates.reserve(m_nPipelineStates);

	for (int i = 0; i < m_nPipelineStates; ++i)
	{
		m_ppd3dPipelineStates.emplace_back(ComPtr<ID3D12PipelineState>());
	}

	CShader::CreateShader(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature);

	if (m_pd3dVertexShaderBlob) m_pd3dVertexShaderBlob->Release();
	if (m_pd3dPixelShaderBlob) m_pd3dPixelShaderBlob->Release();

	if (m_d3dPipelineStateDesc.InputLayout.pInputElementDescs) delete[] m_d3dPipelineStateDesc.InputLayout.pInputElementDescs;
}

void CSpriteObjectsShader::BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, void* pContext)
{

}

void CSpriteObjectsShader::BuildPlayerSpriteObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, void* pContext)
{
	CTexture* pSpriteTexture[3];

	pSpriteTexture[0] = new CTexture(1, RESOURCE_TEXTURE2D, 0, 1, 6, 6);
	pSpriteTexture[0]->LoadTextureFromDDSFile(pd3dDevice, pd3dCommandList, L"Billboard/Explosion_6x6.dds", RESOURCE_TEXTURE2D, 0);

	pSpriteTexture[1] = new CTexture(1, RESOURCE_TEXTURE2D, 0, 1, 4, 4);
	pSpriteTexture[1]->LoadTextureFromDDSFile(pd3dDevice, pd3dCommandList, L"Billboard/explode.dds", RESOURCE_TEXTURE2D, 0);

	pSpriteTexture[2] = new CTexture(1, RESOURCE_TEXTURE2D, 0, 1, 4, 4);
	pSpriteTexture[2]->LoadTextureFromDDSFile(pd3dDevice, pd3dCommandList, L"Billboard/Explosion_Smoke.dds", RESOURCE_TEXTURE2D, 0);

	CScene::CreateShaderResourceViews(pd3dDevice, pSpriteTexture[0], 0, 3);
	CScene::CreateShaderResourceViews(pd3dDevice, pSpriteTexture[1], 0, 3);
	CScene::CreateShaderResourceViews(pd3dDevice, pSpriteTexture[2], 0, 3);

	CMaterial* pMaterial[3];
	pMaterial[0] = new CMaterial();
	pMaterial[0]->SetTexture(pSpriteTexture[0]);
	pMaterial[1] = new CMaterial();
	pMaterial[1]->SetTexture(pSpriteTexture[1]);
	pMaterial[2] = new CMaterial();
	pMaterial[2]->SetTexture(pSpriteTexture[2]);

	CTexturedRectMesh* pSpriteMesh = new CTexturedRectMesh(pd3dDevice, pd3dCommandList, 7.0f, 7.0f, 0.0f);
	CSpriteObject* pSpriteObject[3];

	pSpriteObject[0] = new CSpriteObject(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, 1, 1, 6, 6);
	pSpriteObject[0]->SetMaterial(0, pMaterial[0]);
	pSpriteObject[0]->SetMesh(0, pSpriteMesh);
	pSpriteObject[0]->SetAlive(false);
	pSpriteObject[0]->SetSpeed(0.05f);

	pSpriteObject[1] = new CSpriteObject(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, 1, 1, 4, 4);
	pSpriteObject[1]->SetMaterial(0, pMaterial[1]);
	pSpriteObject[1]->SetMesh(0, pSpriteMesh);
	pSpriteObject[1]->SetAlive(false);
	pSpriteObject[1]->SetSpeed(0.08f);

	pSpriteObject[2] = new CSpriteObject(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, 1, 1, 4, 4);
	pSpriteObject[2]->SetMaterial(0, pMaterial[2]);
	pSpriteObject[2]->SetMesh(0, pSpriteMesh);
	pSpriteObject[2]->SetAlive(false);
	pSpriteObject[2]->SetSpeed(0.1f);

	CHelicopterPlayer* pPlayer = (CHelicopterPlayer*)pContext;
	pPlayer->SetExplosion(pSpriteObject[0], 0);
	pPlayer->SetExplosion(pSpriteObject[1], 1);
	pPlayer->SetExplosion(pSpriteObject[2], 2);

	m_nObjects = 13;
	m_ppObjects = new CGameObject* [m_nObjects];
	m_ppObjects[0] = pSpriteObject[0];
	m_ppObjects[1] = pSpriteObject[1];
	m_ppObjects[2] = pSpriteObject[2];

	CTexture* pExplosionSpriteTexture = new CTexture(1, RESOURCE_TEXTURE2D, 0, 1, 8, 8);
	pExplosionSpriteTexture->LoadTextureFromDDSFile(pd3dDevice, pd3dCommandList, L"Billboard/Explode_8x8.dds", RESOURCE_TEXTURE2D, 0);
	CScene::CreateShaderResourceViews(pd3dDevice, pExplosionSpriteTexture, 0, 3);

	int i = 0;
	for (auto& missile : pPlayer->GetMissiles())
	{
		m_ppObjects[3 + i] = missile->BuildExplosion(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, pSpriteMesh, pExplosionSpriteTexture);
		++i;
	}
}

void CSpriteObjectsShader::BuildEnemySpriteObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, void* pContext, int nObjects)
{
	m_nEnemyObjects = nObjects * 11;
	m_ppEnemyObjects = new CGameObject* [m_nEnemyObjects];

	CTexture* pSpriteTexture;

	pSpriteTexture = new CTexture(1, RESOURCE_TEXTURE2D, 0, 1, 6, 6);
	pSpriteTexture->LoadTextureFromDDSFile(pd3dDevice, pd3dCommandList, L"Billboard/Explosion_6x6.dds", RESOURCE_TEXTURE2D, 0);
	CScene::CreateShaderResourceViews(pd3dDevice, pSpriteTexture, 0, 3);

	CMaterial* pMaterial;
	pMaterial = new CMaterial();
	pMaterial->SetTexture(pSpriteTexture);

	CTexturedRectMesh* pSpriteMesh1 = new CTexturedRectMesh(pd3dDevice, pd3dCommandList, 20.0f, 20.0f, 0.0f);
	CTexturedRectMesh* pSpriteMesh2 = new CTexturedRectMesh(pd3dDevice, pd3dCommandList, 7.0f, 7.0f, 0.0f);

	CGameObject** ppEnemy = (CGameObject**)pContext;

	CTexture* pExplosionSpriteTexture = new CTexture(1, RESOURCE_TEXTURE2D, 0, 1, 8, 8);
	pExplosionSpriteTexture->LoadTextureFromDDSFile(pd3dDevice, pd3dCommandList, L"Billboard/Explode_8x8.dds", RESOURCE_TEXTURE2D, 0);
	CScene::CreateShaderResourceViews(pd3dDevice, pExplosionSpriteTexture, 0, 3);

	for (int i = 0; i < nObjects; ++i)
	{
		if (dynamic_cast<CGunshipObject*>(ppEnemy[i]) == nullptr)
		{
			continue;
		}

		CSpriteObject* pSpriteObject;
		pSpriteObject = new CSpriteObject(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, 1, 1, 6, 6);
		pSpriteObject->SetMaterial(0, pMaterial);
		pSpriteObject->SetMesh(0, pSpriteMesh1);
		pSpriteObject->SetAlive(false);
		pSpriteObject->SetSpeed(0.05f);
		dynamic_cast<CGunshipObject*>(ppEnemy[i])->SetExplosion(pSpriteObject);

		m_ppEnemyObjects[11 * i] = pSpriteObject;

		for (int j = 0; j < MAX_NUM_MISSILE; ++j)
		{
			m_ppEnemyObjects[11 * i + j + 1] = ppEnemy[i]->GetMissile(j)->BuildExplosion(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, pSpriteMesh2, pExplosionSpriteTexture);
		}
	}

}

void CSpriteObjectsShader::ReleaseObjects()
{
	if (m_ppObjects)
	{
		for (int j = 0; j < m_nObjects; j++) if (m_ppObjects[j]) m_ppObjects[j]->Release();
		delete[] m_ppObjects;
	}

	if (m_ppEnemyObjects)
	{
		for (int j = 0; j < m_nEnemyObjects; j++) if (m_ppEnemyObjects[j]) m_ppEnemyObjects[j]->Release();
		delete[] m_ppEnemyObjects;
	}
}

void CSpriteObjectsShader::AnimateObjects(float fTimeElapsed)
{
	for (int i = 0; i < m_nObjects; i++)
	{
		if (m_ppObjects[i])
		{
			if (!m_ppObjects[i]->GetAlive())
				continue;

			m_ppObjects[i]->Animate(fTimeElapsed);
		}
	}

	for (int i = 0; i < m_nEnemyObjects; i++)
	{
		if (m_ppEnemyObjects[i])
		{
			if (!m_ppEnemyObjects[i]->GetAlive())
				continue;

			m_ppEnemyObjects[i]->Animate(fTimeElapsed);
		}
	}
}

void CSpriteObjectsShader::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera, int nPipelineState)
{
	CShader::Render(pd3dCommandList, pCamera, nPipelineState);

	for (int i = 0; i < m_nObjects; i++)
	{
		if (m_ppObjects[i])
		{
			if (!m_ppObjects[i]->GetAlive())
				continue;

			m_ppObjects[i]->UpdateTransform();
			m_ppObjects[i]->Render(pd3dCommandList, pCamera);
		}
	}

	for (int i = 0; i < m_nEnemyObjects; i++)
	{
		if (m_ppEnemyObjects[i])
		{
			if (!m_ppEnemyObjects[i]->GetAlive())
				continue;

			m_ppEnemyObjects[i]->UpdateTransform();
			m_ppEnemyObjects[i]->Render(pd3dCommandList, pCamera);
		}
	}
}

void CSpriteObjectsShader::ReleaseUploadBuffers()
{
	for (int i = 0; i < m_nObjects; ++i)
	{
		if (m_ppObjects[i])
		{
			m_ppObjects[i]->ReleaseUploadBuffers();
		}
	}

	for (int i = 0; i < m_nEnemyObjects; ++i)
	{
		if (m_ppEnemyObjects[i])
		{
			m_ppEnemyObjects[i]->ReleaseUploadBuffers();
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CUserInterfaceShader::CUserInterfaceShader()
{
}

CUserInterfaceShader::~CUserInterfaceShader()
{
}

D3D12_INPUT_LAYOUT_DESC CUserInterfaceShader::CreateInputLayout()
{
	UINT nInputElementDescs = 2;
	D3D12_INPUT_ELEMENT_DESC* pd3dInputElementDescs = new D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];

	pd3dInputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[1] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 1, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
	d3dInputLayoutDesc.pInputElementDescs = pd3dInputElementDescs;
	d3dInputLayoutDesc.NumElements = nInputElementDescs;

	return(d3dInputLayoutDesc);
}

D3D12_SHADER_BYTECODE CUserInterfaceShader::CreateVertexShader()
{
	return CShader::ReadCompiledShaderFromFile(L"./../Debug/VSUserInterface.cso", &m_pd3dVertexShaderBlob);
}

D3D12_SHADER_BYTECODE CUserInterfaceShader::CreatePixelShader()
{
	return CShader::ReadCompiledShaderFromFile(L"./../Debug/PSTextured.cso", &m_pd3dPixelShaderBlob);
}

D3D12_RASTERIZER_DESC CUserInterfaceShader::CreateRasterizerState()
{
	D3D12_RASTERIZER_DESC d3dRasterizerDesc;
	::ZeroMemory(&d3dRasterizerDesc, sizeof(D3D12_RASTERIZER_DESC));
	d3dRasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
	d3dRasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	d3dRasterizerDesc.FrontCounterClockwise = FALSE;
	d3dRasterizerDesc.DepthBias = 0;
	d3dRasterizerDesc.DepthBiasClamp = 0.0f;
	d3dRasterizerDesc.SlopeScaledDepthBias = 0.0f;
	d3dRasterizerDesc.DepthClipEnable = TRUE;
	d3dRasterizerDesc.MultisampleEnable = FALSE;
	d3dRasterizerDesc.AntialiasedLineEnable = FALSE;
	d3dRasterizerDesc.ForcedSampleCount = 0;
	d3dRasterizerDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

	return(d3dRasterizerDesc);
}

D3D12_BLEND_DESC CUserInterfaceShader::CreateBlendState()
{
	D3D12_BLEND_DESC d3dBlendDesc;
	::ZeroMemory(&d3dBlendDesc, sizeof(D3D12_BLEND_DESC));
	d3dBlendDesc.AlphaToCoverageEnable = TRUE;
	d3dBlendDesc.IndependentBlendEnable = FALSE;
	d3dBlendDesc.RenderTarget[0].BlendEnable = TRUE;
	d3dBlendDesc.RenderTarget[0].LogicOpEnable = FALSE;
	d3dBlendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	d3dBlendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	d3dBlendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	d3dBlendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	d3dBlendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
	d3dBlendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	d3dBlendDesc.RenderTarget[0].LogicOp = D3D12_LOGIC_OP_NOOP;
	d3dBlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	return(d3dBlendDesc);
}

void CUserInterfaceShader::CreateShader(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature)
{
	m_nPipelineStates = 1;
	m_ppd3dPipelineStates.reserve(m_nPipelineStates);

	for (int i = 0; i < m_nPipelineStates; ++i)
	{
		m_ppd3dPipelineStates.emplace_back(ComPtr<ID3D12PipelineState>());
	}

	CShader::CreateShader(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature);

	if (m_pd3dVertexShaderBlob) m_pd3dVertexShaderBlob->Release();
	if (m_pd3dPixelShaderBlob) m_pd3dPixelShaderBlob->Release();

	if (m_d3dPipelineStateDesc.InputLayout.pInputElementDescs) delete[] m_d3dPipelineStateDesc.InputLayout.pInputElementDescs;
}

void CUserInterfaceShader::BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, void* pContext)
{
	CGameObject* pHpObject = new CGameObject(1, 1);
	CTexturedRectMesh* hpMesh = new CTexturedRectMesh(pd3dDevice, pd3dCommandList, 0.2f, 0.2f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f ,0.0f, 1.0f);

	CTexture* pHpTexture = new CTexture(1, RESOURCE_TEXTURE2D, 0, 1, 1, 1);
	pHpTexture->LoadTextureFromDDSFile(pd3dDevice, pd3dCommandList, L"Text/hpTexture.dds", RESOURCE_TEXTURE2D, 0);
	CScene::CreateShaderResourceViews(pd3dDevice, pHpTexture, 0, 3);

	CMaterial* pHpMaterial = new CMaterial();
	pHpMaterial->SetTexture(pHpTexture);
	pHpObject->SetMaterial(0, pHpMaterial);
	pHpObject->SetMesh(0, hpMesh);
	pHpObject->SetPosition(-0.9f, -0.95f, 0.0f);
	
	m_pHp = pHpObject;

	CGameObject* pScoreObject = new CGameObject(1, 1);
	CTexturedRectMesh* scoreMesh = new CTexturedRectMesh(pd3dDevice, pd3dCommandList, 0.4f, 0.2f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);

	CTexture* pScoreTexture = new CTexture(1, RESOURCE_TEXTURE2D, 0, 1, 1, 1);
	pScoreTexture->LoadTextureFromDDSFile(pd3dDevice, pd3dCommandList, L"Text/ScoreTexture.dds", RESOURCE_TEXTURE2D, 0);
	CScene::CreateShaderResourceViews(pd3dDevice, pScoreTexture, 0, 3);

	CMaterial* pScoreMaterial = new CMaterial();
	pScoreMaterial->SetTexture(pScoreTexture);
	pScoreObject->SetMaterial(0, pScoreMaterial);
	pScoreObject->SetMesh(0, scoreMesh);
	pScoreObject->SetPosition(-0.8f, 0.9f, 0.0f);

	m_pScore = pScoreObject;

	CGameObject** pNumberScoreObject = new CGameObject*[5];
	CUserInterfaceRectMesh* numberScoreMesh[5];

	CTexture* pNumberScoreTexture = new CTexture(1, RESOURCE_TEXTURE2D, 0, 1, 1, 1);
	pNumberScoreTexture->LoadTextureFromDDSFile(pd3dDevice, pd3dCommandList, L"Text/NumberTexture.dds", RESOURCE_TEXTURE2D, 0);
	CScene::CreateShaderResourceViews(pd3dDevice, pNumberScoreTexture, 0, 3);

	CMaterial* pNumberScoreMaterial = new CMaterial();
	pNumberScoreMaterial->SetTexture(pNumberScoreTexture);
	for(int i=0 ;i<5;++i)
	{
		pNumberScoreObject[i] = new CGameObject(1, 1);
		pNumberScoreObject[i]->SetMaterial(0, pNumberScoreMaterial);
		numberScoreMesh[i] = new CUserInterfaceRectMesh(pd3dDevice, pd3dCommandList, 0.1f, 0.1f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f);
		numberScoreMesh[i]->SetUV(m_fScoreU[i]);
		pNumberScoreObject[i]->SetMesh(0, numberScoreMesh[i]);
		pNumberScoreObject[i]->SetPosition(-0.57f + i * 0.1f, 0.91f, 0.0f);
		m_pNumberScore[i] = pNumberScoreObject[i];
	}

	CGameObject** pNumberHpObject = new CGameObject* [2];
	CUserInterfaceRectMesh* numberHpMesh[2];
	for (int i = 0; i < 2; ++i)
	{
		pNumberHpObject[i] = new CGameObject(1, 1);
		pNumberHpObject[i]->SetMaterial(0, pNumberScoreMaterial);
		numberHpMesh[i] = new CUserInterfaceRectMesh(pd3dDevice, pd3dCommandList, 0.1f, 0.1f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f);
		numberHpMesh[i]->SetUV(m_fHpU[i]);
		pNumberHpObject[i]->SetMesh(0, numberHpMesh[i]);
		pNumberHpObject[i]->SetPosition(-0.77f + i * 0.1f, -0.94f, 0.0f);
		m_pNumberHp[i] = pNumberHpObject[i];
	}

	m_pPlayer = (CHelicopterPlayer*)pContext;
}

void CUserInterfaceShader::ReleaseObjects()
{
}

void CUserInterfaceShader::AnimateObjects(float fTimeElapsed)
{
	m_nHp = dynamic_cast<CHelicopterPlayer*>(m_pPlayer)->GetHp();
	for (int i = 1, j = 1; i <= 10; i = i * 10, --j)
	{
		m_fHpU[j] = 50.0f * (m_nHp % (i * 10) / i);
		if (m_fHpU[j] >= 500.0f)
			m_fHpU[j] = 0.0f;
	}

	if (m_nHp < 0)
		return;

	m_nScore = dynamic_cast<CHelicopterPlayer*>(m_pPlayer)->GetScore();
	if (m_nScore == 0)
	{
		return;
	}

	if (m_nScore > 99'999)
	{
		//clear
	}

	for (int i = 1, j = 4; i <= 10'000; i = i * 10, --j)
	{
		m_fScoreU[j] = 50.0f * (m_nScore % (i * 10) / i);
		if (m_fScoreU[j] >= 500.0f)
			m_fScoreU[j] = 0.0f;
	}


}

void CUserInterfaceShader::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera, int nPipelineState)
{
	CShader::Render(pd3dCommandList, pCamera, nPipelineState);

	m_pHp->UpdateTransform();
	m_pHp->Render(pd3dCommandList, pCamera);

	m_pScore->UpdateTransform();
	m_pScore->Render(pd3dCommandList, pCamera);

	for (int i = 0; i < 5; ++i)
	{
		m_pNumberScore[i]->UpdateTransform();
		dynamic_cast<CUserInterfaceRectMesh*>(m_pNumberScore[i]->GetMesh(0))->SetUV(m_fScoreU[i] / 500.0f);
		m_pNumberScore[i]->Render(pd3dCommandList, pCamera);
	}

	for (int i = 0; i < 2; ++i)
	{
		m_pNumberHp[i]->UpdateTransform();
		dynamic_cast<CUserInterfaceRectMesh*>(m_pNumberHp[i]->GetMesh(0))->SetUV(m_fHpU[i] / 500.0f);
		m_pNumberHp[i]->Render(pd3dCommandList, pCamera);
	}
}
