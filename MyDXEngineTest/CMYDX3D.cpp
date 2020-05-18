
#include "CMYDX3D.h"


CMYDX3D::CMYDX3D()
{

}
CMYDX3D::~CMYDX3D()
{
	CleanUp();

}

void CMYDX3D::BeginScene()
{
	const float bgColor[4] = { 0.f,0.f,0.f,1.0f };
	m_pDevCtx->ClearRenderTargetView(m_pRenderTargetView, bgColor);
	m_pDevCtx->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}
void CMYDX3D::EndScene()
{
	m_pSwapChain->Present(0, 0);
}

void CMYDX3D::CleanUp()
{
	ZeroMemory(&m_viewPort, sizeof(D3D11_VIEWPORT));
	if (m_pDepthStencilView)
	{
		m_pDepthStencilView->Release();
		m_pDepthStencilView = NULL;
	}
	if (m_pDepthStencilBuffer)
	{
		m_pDepthStencilBuffer->Release();
		m_pDepthStencilBuffer = NULL;
	}
	if (m_pSwapChain)
	{
		m_pSwapChain->Release();
		m_pSwapChain = NULL;
	}
	if (m_pDevCtx)
	{
		m_pDevCtx->Release();
		m_pDevCtx = NULL;
	}
	if (m_pDev)
	{
		m_pDev->Release();
		m_pDev = NULL;
	}

}

const ID3D11Device* CMYDX3D::GetDXDevice()
{
	return m_pDev;
}
const ID3D11DeviceContext* CMYDX3D::GetDXDeviceContext()
{
	return m_pDevCtx;
}

BOOL CMYDX3D::Initialize(HWND hWnd,float fWidth, float fHeight)
{
	BOOL bSuccess = FALSE;
	DXGI_MODE_DESC bufferDesc;
	HRESULT hResult = 0;
	ID3D11Texture2D* pBackBuffer = NULL;

	ZeroMemory(&bufferDesc, sizeof(DXGI_MODE_DESC));

	bufferDesc.Width =(UINT) fWidth;
	bufferDesc.Height = (UINT)fHeight;
	bufferDesc.RefreshRate.Numerator = 60;
	bufferDesc.RefreshRate.Denominator = 1;
	bufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	DXGI_SWAP_CHAIN_DESC scd;

	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	scd.BufferDesc = bufferDesc;
	scd.BufferCount = 1;                                   // one back buffer
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;     // how swap chain is to be used
	scd.OutputWindow = hWnd;                               // the window to be used
	scd.SampleDesc.Count = 1;                              // how many multisamples
	scd.Windowed = TRUE;                                   // windowed/full-screen mode
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;    // allow full-screen switching

	if (FAILED(D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		NULL,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&scd,
		&m_pSwapChain,
		&m_pDev,
		NULL,
		&m_pDevCtx
	)))
	{
		bSuccess = FALSE;
		goto EXIT;
	}

	bSuccess = TRUE;

	if (FAILED(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer)))
	{
		bSuccess = FALSE;
		goto EXIT;
	}
	bSuccess = TRUE;

	if (FAILED(m_pDev->CreateRenderTargetView(pBackBuffer, NULL, &m_pRenderTargetView)))
	{
		bSuccess = FALSE;
		goto EXIT;
	}

	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width = (UINT)fWidth;
	depthStencilDesc.Height = (UINT)fHeight;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	if (FAILED(m_pDev->CreateTexture2D(&depthStencilDesc, NULL, &m_pDepthStencilBuffer)))
	{
		bSuccess = FALSE;
		goto EXIT;
	}
	bSuccess = TRUE;
	if (FAILED(m_pDev->CreateDepthStencilView(m_pDepthStencilBuffer, NULL, &m_pDepthStencilView)))
	{
		bSuccess = FALSE;
		goto EXIT;
	}
	bSuccess = TRUE;

	m_pDevCtx->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);
	
	ZeroMemory(&m_viewPort, sizeof(D3D11_VIEWPORT));

	m_viewPort.TopLeftX = 0;
	m_viewPort.TopLeftY = 0;
	m_viewPort.Width = fWidth;
	m_viewPort.Height = fHeight;
	m_viewPort.MinDepth = 0.0f;
	m_viewPort.MaxDepth = 1.0f;
	m_pDevCtx->RSSetViewports(1, &m_viewPort);

EXIT:
	if (pBackBuffer)
	{
		pBackBuffer->Release();
	}
	if (!bSuccess)
	{
	
		CleanUp();
	}
	
	return bSuccess;
}