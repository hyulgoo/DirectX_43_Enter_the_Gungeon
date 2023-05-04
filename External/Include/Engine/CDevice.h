#pragma once
#include "ptr.h"
#include "CTexture.h"
class CConstBuffer;

class CDevice
	:public CSingleton<CDevice>
{
private:
	HWND							m_hWnd;

	// 참조한 헤더의 dll에서 자체 생성 후 주소값을 받아옴
	ComPtr<ID3D11Device>			m_Device;  // GPU 메모리 할당
	ComPtr<ID3D11DeviceContext>		m_Context; // GPU 제어, 렌더링, 동작 수행
	
	ComPtr<IDXGISwapChain>			m_SwapChain;
	// 2차원 형태의 이미지데이터
	// 2개의 이미지데이터는 각각 그래픽카드 메모리 영역에 있는 프론트버퍼, 백버퍼	
	Ptr<CTexture>					m_RTTex;
	Ptr<CTexture>					m_DSTex;


	ComPtr<ID3D11SamplerState>		m_Sampler[2];
	
	// Rasterizer
	ComPtr<ID3D11RasterizerState>	m_RSState[(UINT)RS_TYPE::END];
	// DepthStencil
	ComPtr<ID3D11DepthStencilState>	m_DSState[(UINT)DS_TYPE::END];
	// Blend
	ComPtr<ID3D11BlendState>		m_BSState[(UINT)BS_TYPE::END];

	D3D11_VIEWPORT					m_ViewPort;

	Vec2							m_vRenderResolution;

	CConstBuffer*					m_arrConstBuffer[(UINT)CB_TYPE::END];

public:
	int init(HWND _hWnd, UINT _iWidth, UINT _iHeight);
	void ClearTarget(float(&color)[4]);
	void OMSet() { m_Context->OMSetRenderTargets(1, m_RTTex->GetRTV().GetAddressOf(), m_DSTex->GetDSV().Get()); }
	void Present() { m_SwapChain->Present(0, 0); }

	Vec2 GetRenderResolution() { return m_vRenderResolution; }

private:
	int CreateSwapChain();
	int CreateView();
	int CreateRasterizerState();
	int CreateBlendState();
	int CreateDepthStencilState();
	int CreateSampler();
	void CreateConstBuffer();

public:
	ID3D11Device* GetDevice() { return m_Device.Get(); }
	ID3D11DeviceContext* GetDeviceContext() { return m_Context.Get(); }
	CConstBuffer* GetConstBuffer(CB_TYPE _Type) { return m_arrConstBuffer[(UINT)_Type]; }

	ComPtr<ID3D11RasterizerState> GetRSState(RS_TYPE _Type) { return m_RSState[(UINT)_Type]; }
	ComPtr<ID3D11DepthStencilState> GetDSState(DS_TYPE _Type) { return m_DSState[(UINT)_Type]; }
	ComPtr<ID3D11BlendState> GetBSState(BS_TYPE _Type) { return m_BSState[(UINT)_Type]; }

public:
	D3D11_VIEWPORT GetViewport() { return m_ViewPort; }

public:
	CDevice();
	~CDevice();
};

