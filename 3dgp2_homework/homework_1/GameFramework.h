#pragma once

#include "Timer.h"
#include "Player.h"
#include "Scene.h"

UINT constexpr NUM_SWAP_CHAIN_BUFFER{ 2 };

using ArrayComPtrResource = std::array<ComPtr<ID3D12Resource>, NUM_SWAP_CHAIN_BUFFER>;

class CGameFramework
{
public:
	CGameFramework();
	~CGameFramework();

	bool OnCreate(HINSTANCE hInstance, HWND hMainWnd);
	void OnDestroy();

	void CreateSwapChain();
	void CreateDirect3DDevice();
	void CreateCommandQueueAndList();

	void CreateRtvAndDsvDescriptorHeaps();

	void CreateRenderTargetViews();
	void CreateDepthStencilView();

	void ChangeSwapChainState();

    void BuildObjects();
    void ReleaseObjects();

    void ProcessInput();
    void AnimateObjects();
    void FrameAdvance();

	void WaitForGpuComplete();
	void MoveToNextFrame();

	void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

private:
	HINSTANCE						m_hInstance;
	HWND							m_hWnd; 
		
	int								m_nWndClientWidth;
	int								m_nWndClientHeight;
        
	ComPtr<IDXGIFactory4>			m_pdxgiFactory;
	ComPtr<IDXGISwapChain3>			m_pdxgiSwapChain;
	ComPtr<ID3D12Device>			m_pd3dDevice;

	bool							m_bMsaa4xEnable = false;
	UINT							m_nMsaa4xQualityLevels = 0;

	static const UINT				m_nSwapChainBuffers = NUM_SWAP_CHAIN_BUFFER;
	UINT							m_nSwapChainBufferIndex;

	ArrayComPtrResource				m_ppd3dSwapChainBackBuffers;
	ComPtr<ID3D12DescriptorHeap>	m_pd3dRtvDescriptorHeap;

	ComPtr<ID3D12Resource>			 m_pd3dDepthStencilBuffer;
	ComPtr<ID3D12DescriptorHeap>	 m_pd3dDsvDescriptorHeap;

	ComPtr<ID3D12CommandAllocator>		m_pd3dCommandAllocator;
	ComPtr<ID3D12CommandQueue>			m_pd3dCommandQueue;
	ComPtr<ID3D12GraphicsCommandList>	m_pd3dCommandList;

	ComPtr<ID3D12Fence>				m_pd3dFence;
	UINT64							m_nFenceValues[m_nSwapChainBuffers];
	HANDLE							m_hFenceEvent;

#if defined(_DEBUG)
	ID3D12Debug						*m_pd3dDebugController;
#endif

	CGameTimer						m_GameTimer;

	CScene							*m_pScene = nullptr;
	CPlayer							*m_pPlayer = nullptr;
	CCamera							*m_pCamera = nullptr;

	POINT							m_ptOldCursorPos;

	_TCHAR							m_pszFrameRate[70];
};

