#include "dxshader.h"
#include <Shlwapi.h>
#include "C3DObject.h"
#include "CMyDXCam.h"

typedef struct _VS_CONST_BUFFER {
	XMMATRIX worldMatrix;
	XMMATRIX projectionMatrix;
	XMMATRIX viewMatrix;
	XMFLOAT4 lightPos;
	XMFLOAT4 camPos;
}VS_CONST_BUFFER,*PVS_CONST_BUFFER;


typedef struct _PS_CONST_BUFFER {
	XMFLOAT4 LightColor;
}PS_CONST_BUFFER,*PPS_CONST_BUFFER;

ID3D11Device* gDXDevice = NULL;
ID3D11DeviceContext* gDXDeviceContext = NULL;
IDXGISwapChain* gDXSwapChain = NULL;
ID3D11RenderTargetView* gDXRenderTargetView = NULL;
ID3D11Texture2D* gDXDepthStencilBuffer = NULL;
ID3D11DepthStencilView* gDXDepthStencilView = NULL;
ID3D11VertexShader* gVertexShader = NULL;    // the vertex shader
ID3D11PixelShader* gPixelShader = NULL;
ID3D11InputLayout* gInputLayout= NULL;
ID3D11Buffer* gVBuffer = NULL;
ID3D11Buffer* gIdxBuffer = NULL;
ID3D11Buffer* gVertexConstBuffer = NULL;
ID3D11Buffer* gPScbFrameBuffer = NULL;
VS_CONST_BUFFER gVSConstBuffer;
PS_CONST_BUFFER gPSConstBuffer;
ID3D11ShaderResourceView* gTextureArr[2] = { NULL, };
ID3D11SamplerState* gTextureSamplerStateArr[2] = { NULL, };


CMyDXCam gCamera;
C3DObject g3DObject;

float rot = 0.f;
const D3D11_INPUT_ELEMENT_DESC ied[] =
{
	{ "SV_POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0}
};

void DX3DCleanup()
{
	/*if (gDiffuseTexture)
	{
		gDiffuseTexture->Release();
		gDiffuseTexture = NULL;
	}
	if (gDiffuseTextureSamplerState)
	{
		gDiffuseTextureSamplerState->Release();
		gDiffuseTextureSamplerState = NULL;
	}*/
	if (gVertexConstBuffer)
	{
		gVertexConstBuffer->Release();
		gVertexConstBuffer = NULL;
	}
	if (gVBuffer)
	{
		gVBuffer->Release();
		gVBuffer = NULL;
	}
	if (gPixelShader)
	{
		gPixelShader->Release();
		gPixelShader = NULL;
	}
	if (gVertexShader)
	{
		gVertexShader->Release();
		gVertexShader = NULL;
	}
	if (gInputLayout)
	{
		gInputLayout->Release();
		gInputLayout = NULL;
	}
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

HRESULT LoadTexture(
	const WCHAR* strTextureFilePath,
	ID3D11ShaderResourceView** textureResourceView,
	ID3D11SamplerState** TextureSamplerState)
{
	DirectX::ScratchImage image;

	HRESULT hr = 0;
	hr = CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);
	if (FAILED(hr))
	{
		goto EXIT;
	}

	hr = LoadFromWICFile(strTextureFilePath, DirectX::WIC_FLAGS_NONE, nullptr, image);

	if (FAILED(hr))
	{
		goto EXIT;
	}

	hr = CreateShaderResourceView(gDXDevice, image.GetImages(), image.GetImageCount(), image.GetMetadata(), &(*textureResourceView));

	if (FAILED(hr))
	{
		if ((*textureResourceView))
		{
			(*textureResourceView)->Release();
			(*textureResourceView) = NULL;
		}
		goto EXIT;
	}


	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	hr = gDXDevice->CreateSamplerState(&sampDesc, &(*TextureSamplerState));

EXIT:

	return hr;
}
BOOL LoadModel(const WCHAR* objFile)
{
	DWORD dwVertexSize = 0;

	int nRet = g3DObject.LoadObjFile(objFile);
	if (nRet != 0)
		return FALSE;
	
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
	bd.ByteWidth = sizeof(VERTEX) * g3DObject.m_VertexListSize;             // size is the VERTEX struct * 3
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer

	if (FAILED(gDXDevice->CreateBuffer(&bd, NULL, &gVBuffer)))      // create the buffer
	{
		OutputDebugString(L"vertext buffer error");
		return FALSE;
	}
	D3D11_MAPPED_SUBRESOURCE ms;
	ZeroMemory(&ms, sizeof(D3D11_MAPPED_SUBRESOURCE));
	gDXDeviceContext->Map(gVBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    // map the buffer
	memcpy(ms.pData, g3DObject.m_pVERTEXList, sizeof(VERTEX) * g3DObject.m_VertexListSize);                 // copy the data
	gDXDeviceContext->Unmap(gVBuffer, NULL);


	D3D11_BUFFER_DESC idxBufDesc;
	ZeroMemory(&idxBufDesc, sizeof(idxBufDesc));
	idxBufDesc.Usage = D3D11_USAGE_DYNAMIC;
	idxBufDesc.ByteWidth = sizeof(UINT) * g3DObject.m_indexSize;
	idxBufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	idxBufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer

	if (FAILED(gDXDevice->CreateBuffer(&idxBufDesc, NULL, &gIdxBuffer)))
	{
		OutputDebugString(L"Index Buffer error\n");
	
		return FALSE;
	}
	ZeroMemory(&ms, sizeof(D3D11_MAPPED_SUBRESOURCE));
	gDXDeviceContext->Map(gIdxBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    // map the buffer
	memcpy(ms.pData, g3DObject.m_pindexList, sizeof(UINT) * g3DObject.m_indexSize);                 // copy the data
	gDXDeviceContext->Unmap(gIdxBuffer, NULL);

	return TRUE;
}
BOOL LoadShader(const WCHAR* shaderFile)
{
	ID3DBlob* VS; ID3DBlob* PS;
	HRESULT hResult = 0;
	if (!PathFileExists(shaderFile))
	{
		OutputDebugString(L"shader file not exists error \n");
		return FALSE;
	}
	hResult = D3DCompileFromFile(shaderFile, 0, 0, "vs_main","vs_4_0", 0, 0, &VS, 0);
	if (FAILED(hResult))
	{
		return FALSE;
	}
	hResult = D3DCompileFromFile(shaderFile, 0, 0, "ps_main", "ps_4_0", 0, 0, &PS, 0);
	if (FAILED(hResult))
	{
		return FALSE;
	}

	hResult = gDXDevice->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), 0, &gVertexShader);
	if (FAILED(hResult))
	{
		return FALSE;
	}
	hResult = gDXDevice->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), 0, &gPixelShader);
	if (FAILED(hResult))
	{
		return FALSE;
	}
	hResult =  gDXDevice->CreateInputLayout(ied, 3, VS->GetBufferPointer(), VS->GetBufferSize(), &gInputLayout);
	if (FAILED(hResult))
	{
		return FALSE;
	}
	return TRUE;
}
HRESULT InitializeDX3D(HWND hwnd, UINT width,UINT height, const WCHAR* obj,const WCHAR* shaderFile)
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

	if (!LoadShader(shaderFile))
	{
		hResult = E_FAIL;
		goto ERROR_EXIT;
	}
	if (!LoadModel(obj))
	{
		hResult = E_FAIL;
		OutputDebugString(L"LoadModel error\n");
		goto ERROR_EXIT;
	}
	hResult = LoadTexture(L"Fieldstone.png",&(gTextureArr[0]),&gTextureSamplerStateArr[0]);
	if (FAILED(hResult))
	{
		OutputDebugString(L"LoadTexture error\n");
		goto ERROR_EXIT;
	}

	hResult = LoadTexture(L"fieldstone_SM.png", &(gTextureArr[1]), &gTextureSamplerStateArr[1]);
	if (FAILED(hResult))
	{
		OutputDebugString(L"LoadTexture error\n");
		goto ERROR_EXIT;
	}

	D3D11_BUFFER_DESC cbbd;
	ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));

	cbbd.Usage = D3D11_USAGE_DYNAMIC;
	cbbd.ByteWidth = sizeof(VS_CONST_BUFFER);
	cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbbd.MiscFlags = 0;
	gDXDevice->CreateBuffer(&cbbd, NULL, &gVertexConstBuffer);

	ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));
	cbbd.Usage = D3D11_USAGE_DYNAMIC;
	cbbd.ByteWidth = sizeof(PS_CONST_BUFFER);
	cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbbd.MiscFlags = 0;
	gDXDevice->CreateBuffer(&cbbd, NULL, &gPScbFrameBuffer);

	
	gCamera.CameraInitialize(width, height, 
		XMVectorSet(0.0f, 0.0f, -10.0f, 0.0f), 
		XMVectorSet(0.f, 0.f, 0.f, 0.0f), 
		XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));

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
void Render()
{
	rot += .0100f;
	if (rot > 6.28f)
		rot = 0.0f;

	XMVECTOR rotaxis = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMMATRIX Rotation = XMMatrixRotationAxis(rotaxis, rot);
	D3D11_MAPPED_SUBRESOURCE ms;
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	gDXDeviceContext->VSSetShader(gVertexShader, 0, 0);
	gDXDeviceContext->PSSetShader(gPixelShader, 0, 0);
	gDXDeviceContext->PSSetShaderResources(0, 2, gTextureArr);
	gDXDeviceContext->PSSetSamplers(0, 2, gTextureSamplerStateArr);

	gDXDeviceContext->IASetInputLayout(gInputLayout);

	gDXDeviceContext->IASetVertexBuffers(0, 1, &gVBuffer, &stride, &offset);
	gDXDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	gDXDeviceContext->IASetIndexBuffer(gIdxBuffer, DXGI_FORMAT_R32_UINT, 0);
	

	gVSConstBuffer.projectionMatrix = XMMatrixTranspose(gCamera.GetProjectionMatrix());// XMMatrixIdentity();
	gVSConstBuffer.viewMatrix = XMMatrixTranspose(gCamera.GetCameraMetrix());
	gVSConstBuffer.worldMatrix = XMMatrixTranspose( gCamera.GetWorldMatrix() * Rotation);
	gVSConstBuffer.lightPos = XMFLOAT4(100.f, 100.f, -100.f, 1.0f);
	XMFLOAT4 pos;
	XMStoreFloat4(&pos, gCamera.GetCameraPosition());
	gVSConstBuffer.camPos = pos;
	ZeroMemory(&ms, sizeof(D3D11_MAPPED_SUBRESOURCE));
	gDXDeviceContext->Map(gVertexConstBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    // map the buffer
	memcpy(ms.pData, &gVSConstBuffer, sizeof(gVSConstBuffer));                 // copy the data
	gDXDeviceContext->Unmap(gVertexConstBuffer, NULL);                                      // unmap the buffer
	gDXDeviceContext->VSSetConstantBuffers(0, 1, &gVertexConstBuffer);

	ZeroMemory(&ms, sizeof(D3D11_MAPPED_SUBRESOURCE));
	XMStoreFloat4(&gPSConstBuffer.LightColor, XMVectorSet(0.7f, 0.7f, 1.0f, 1.0f));
	gDXDeviceContext->Map(gPScbFrameBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    // map the buffer
	memcpy(ms.pData, &gPSConstBuffer, sizeof(gPSConstBuffer));                 // copy the data
	gDXDeviceContext->Unmap(gPScbFrameBuffer, NULL);                                      // unmap the buffer
	gDXDeviceContext->PSSetConstantBuffers(0, 1, &gPScbFrameBuffer);



	gDXDeviceContext->DrawIndexed(g3DObject.m_indexSize, 0, 0);
	//gDXDeviceContext->Draw(g3DObject.m_VertexListSize, 0);

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