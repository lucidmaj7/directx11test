#include "dxshader.h"

ID3D11Device* gDXDevice = NULL;
ID3D11DeviceContext* gDXDeviceContext = NULL;
IDXGISwapChain* gDXSwapChain = NULL;
ID3D11RenderTargetView* gDXRenderTargetView = NULL;
ID3D11Texture2D* gDXDepthStencilBuffer = NULL;
ID3D11DepthStencilView* gDXDepthStencilView = NULL;
void DX3DCleanup()
{

	if (gDXDepthStencilBuffer)
	{
		gDXDepthStencilBuffer->Release();
		gDXDepthStencilBuffer = NULL;
	}
	if (gDXRenderTargetView)
	{
		gDXRenderTargetView->Release();
		gDXRenderTargetView = NULL;
	}
	if (gDXSwapChain)
	{
		gDXSwapChain->Release();
		gDXSwapChain = NULL;
	}
	if (gDXDeviceContext)
	{
		gDXDeviceContext->Release();
		gDXDeviceContext = NULL;
	}
	if (gDXDevice)
	{
		gDXDevice->Release();
		gDXDevice = NULL;
	}

}

HRESULT InitializeDX3D(HWND hwnd, UINT width,UINT height)
{

	HRESULT hResult = 0;
	DXGI_MODE_DESC swapBufferDesc;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ID3D11Texture2D* pBackBuffer = NULL;
	D3D11_TEXTURE2D_DESC depthStencilDesc;
	D3D11_VIEWPORT viewPort;
	ZeroMemory(&swapBufferDesc, sizeof(swapBufferDesc));

	swapBufferDesc.Width = width;
	swapBufferDesc.Height = height;
	swapBufferDesc.RefreshRate.Numerator = 60;
	swapBufferDesc.RefreshRate.Denominator = 0;
	swapBufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; 
	//A four-component, 32-bit unsigned-normalized-integer 
	//format that supports 8 bits for each color channel and 8-bit alpha.
	swapBufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapBufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;


	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	swapChainDesc.BufferDesc = swapBufferDesc;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hwnd;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.Windowed = TRUE;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;


	hResult = D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		NULL,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&gDXSwapChain,
		&gDXDevice,
		NULL,
		&gDXDeviceContext
	);

	if (FAILED(hResult))
	{
		OutputDebugString(L"Error D3D11CreateDeviceAndSwapChain\n");
		goto ERROR_EXIT;
	}

	hResult = gDXSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	
	if (FAILED(hResult))
	{
		OutputDebugString(L"Error GetBuffer\n");
		goto ERROR_EXIT;
	}

	hResult = gDXDevice->CreateRenderTargetView(pBackBuffer, NULL, &gDXRenderTargetView);

	if (FAILED(hResult))
	{
		OutputDebugString(L"Error CreateRenderTargetView\n");
		goto ERROR_EXIT;
	}

	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	depthStencilDesc.Width = (UINT)width;
	depthStencilDesc.Height = (UINT)height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;
	hResult = gDXDevice->CreateTexture2D(&depthStencilDesc, NULL, &gDXDepthStencilBuffer);
	if (FAILED(hResult))
	{
		OutputDebugString(L"Error CreateTexture2D gDXDepthStencilBuffer\n");
		goto ERROR_EXIT;
	}

	hResult = gDXDevice->CreateDepthStencilView(gDXDepthStencilBuffer, NULL, &gDXDepthStencilView);

	if (FAILED(hResult))
	{
		OutputDebugString(L"Error CreateDepthStencilView\n");
		goto ERROR_EXIT;
	}

	gDXDeviceContext->OMSetRenderTargets(1, &gDXRenderTargetView, gDXDepthStencilView);

	ZeroMemory(&viewPort, sizeof(viewPort));

	viewPort.TopLeftX = 0;
	viewPort.TopLeftY = 0;
	viewPort.Width = (FLOAT)width;
	viewPort.Height = (FLOAT)height;
	viewPort.MinDepth = 0.0f;
	viewPort.MaxDepth = 1.0f;

	gDXDeviceContext->RSSetViewports(1, &viewPort);

	hResult = ERROR_SUCCESS;
	goto EXIT;

ERROR_EXIT:
	DX3DCleanup();

EXIT:
	if (pBackBuffer)
	{
		pBackBuffer->Release();
		pBackBuffer = NULL;
	}
	return hResult;

}

void BeginScene()
{
	const float bgColor[4] = { 0.f,0.f,0.f,1.0f };
	gDXDeviceContext->ClearRenderTargetView(gDXRenderTargetView, bgColor);
	gDXDeviceContext->ClearDepthStencilView(gDXDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

}

void EndScene()
{
	gDXSwapChain->Present(1, 0);
}