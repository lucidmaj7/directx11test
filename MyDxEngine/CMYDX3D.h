#pragma once

#include "MyDXEngineCommon.h"


class CMYDX3D : public AlignedAllocationPolicy<16>
{
public:
	CMYDX3D();
	~CMYDX3D();
	BOOL Initialize(HWND hWnd, float fWidth, float fHeight);
	void CleanUp();
	void BeginScene();
	void EndScene();
	 ID3D11Device* GetDXDevice();
	 ID3D11DeviceContext* GetDXDeviceContext();
private:
	ID3D11DeviceContext* m_pDevCtx;
	IDXGISwapChain* m_pSwapChain;
	ID3D11Device* m_pDev;
	ID3D11RenderTargetView* m_pRenderTargetView;
	ID3D11DepthStencilView* m_pDepthStencilView;
	D3D11_VIEWPORT m_viewPort;
	ID3D11Texture2D* m_pDepthStencilBuffer;
};

