#include "pch.h"
#include "CDirect3D.h"
#include <DirectXPackedVector.h>
//#include <GeometricPrimitive.h>
#include "DirectXTex/DirectXTex.h"
#include <d3dcompiler.h>



#pragma comment(lib, "d3d11.lib")
#pragma comment(lib,"D3DCompiler.lib")

using namespace DirectX;
D3D11_INPUT_ELEMENT_DESC ied[] =
{


	   { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL",     0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0}
};

CDirect3D::CDirect3D()
{
	// color = { 0.0f, 0.2f, 0.4f, 1.0f };
	 ZeroMemory(color, sizeof(float) * 4);
	/*	OurVertices[0] = { -0.5f, -0.5f, 0.5f,  {1.0f, 0.0f, 0.0f, 1.0f}};
	OurVertices[1] = { -0.5f,  0.5f, 0.5f,  { 0.0f, 1.0f, 0.0f, 1.0f }};
	OurVertices[2] = { 0.5f,  0.5f, 0.5f, {  0.0f, 0.0f, 1.0f, 1.0f }};
	OurVertices[3] = {0.5f, -0.5f, 0.5f, {0.0f, 1.0f, 0.0f, 1.0f }};
	*/

	 m_light.dir = XMFLOAT3(0.25f, 0.5f, -1.0f);
	 m_light.ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	 m_light.diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	 rot = 0.0f;
	

	// Front Face
	int i = 0;
	// Front Face
	OurVertices[i++] = VERTEX(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, -1.0f, -1.0f);
	OurVertices[i++] = VERTEX(-1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, -1.0f);
	OurVertices[i++] = VERTEX(1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 1.0f, -1.0f);
	OurVertices[i++] = VERTEX(1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f, -1.0f);

	// Back Face
	OurVertices[i++] = VERTEX(-1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f);
	OurVertices[i++] = VERTEX(1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 1.0f, -1.0f, 1.0f);
	OurVertices[i++] = VERTEX(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
	OurVertices[i++] = VERTEX(-1.0f, 1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 1.0f, 1.0f);

	// Top Face
	OurVertices[i++] = VERTEX(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 1.0f, -1.0f);
	OurVertices[i++] = VERTEX(-1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);
	OurVertices[i++] = VERTEX(1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f);
	OurVertices[i++] = VERTEX(1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f);

	// Bottom Face
	OurVertices[i++] = VERTEX(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, -1.0f);
	OurVertices[i++] = VERTEX(1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, -1.0f, -1.0f);
	OurVertices[i++] = VERTEX(1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, -1.0f, 1.0f);
	OurVertices[i++] = VERTEX(-1.0f, -1.0f, 1.0f, 1.0f, 0.0f, -1.0f, -1.0f, 1.0f);

	// Left Face
	OurVertices[i++] = VERTEX(-1.0f, -1.0f, 1.0f, 0.0f, 1.0f, -1.0f, -1.0f, 1.0f);
	OurVertices[i++] = VERTEX(-1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);
	OurVertices[i++] = VERTEX(-1.0f, 1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 1.0f, -1.0f);
	OurVertices[i++] = VERTEX(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, -1.0f);

	// Right Face
	OurVertices[i++] = VERTEX(1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, -1.0f, -1.0f);
	OurVertices[i++] = VERTEX(1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, -1.0f);
	OurVertices[i++] = VERTEX(1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f);
	OurVertices[i++] = VERTEX(1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 1.0f);



	camPosition = XMVectorSet(0.0f, 3.0f, -5.0f, 0.0f);
	m_camTargetX = 0.0f;
	m_camTargetY = 0.0f;
	m_camTargetZ = 0.0f;

	camTarget = XMVectorSet(m_camTargetX, m_camTargetY, m_camTargetZ, 0.0f);
	camUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	
}
CDirect3D::~CDirect3D()
{

	
}
void CDirect3D::InitGraphics()
{
	// create a triangle using the VERTEX struct

	UINT indexBuf[] = {
		// Front Face
				0,  1,  2,
				0,  2,  3,

				// Back Face
				4,  5,  6,
				4,  6,  7,

				// Top Face
				8,  9, 10,
				8, 10, 11,

				// Bottom Face
				12, 13, 14,
				12, 14, 15,

				// Left Face
				16, 17, 18,
				16, 18, 19,

				// Right Face
				20, 21, 22,
				20, 22, 23
	};
	

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
	bd.ByteWidth = sizeof(VERTEX) * 24;             // size is the VERTEX struct * 3
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer

	m_dev->CreateBuffer(&bd, NULL, &m_pVBuffer);       // create the buffer

	if (FAILED(m_dev->CreateBuffer(&bd, NULL, &m_pVBuffer)))      // create the buffer
	{
		OutputDebugString(_T("vertext buffer error"));
	}
	
	D3D11_BUFFER_DESC idxBufDesc;
	ZeroMemory(&idxBufDesc, sizeof(idxBufDesc));

	idxBufDesc.Usage = D3D11_USAGE_DYNAMIC;
	idxBufDesc.ByteWidth = sizeof(UINT) * 36;
	idxBufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	idxBufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer


	m_dev->CreateBuffer(&idxBufDesc,NULL, &m_pIdxBuffer);

	D3D11_MAPPED_SUBRESOURCE ms;
	// unmap the buffer
	// copy the vertices into the buffer
	ZeroMemory(&ms, sizeof(D3D11_MAPPED_SUBRESOURCE));
	m_pDevCtx->Map(m_pIdxBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    // map the buffer
	memcpy(ms.pData, indexBuf, sizeof(indexBuf));                 // copy the data
	m_pDevCtx->Unmap(m_pIdxBuffer, NULL);


	

	


}

void CDirect3D::InitPipeline()
{ 
	ID3DBlob* VS = NULL, * PS = NULL;
	ID3D10Blob* D2D_PS_Buffer = NULL;

	
	HRESULT hr=	D3DCompileFromFile(L"shaders.shader", 0, 0, "PShader", "ps_4_0", 0, 0, &PS, 0);
	if (FAILED(D3DCompileFromFile(L"shaders.shader", 0, 0, "VShader", "vs_4_0", 0, 0, &VS, 0)))
	{
		AfxMessageBox(_T("error"));
	}
	 D3DCompileFromFile(L"shaders.shader", 0, 0, "D2D_PS", "ps_4_0", 0,  0, &D2D_PS_Buffer, 0);
	m_dev->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &m_pVS);
	m_dev->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &m_pPS);
	m_dev->CreatePixelShader(D2D_PS_Buffer->GetBufferPointer(), D2D_PS_Buffer->GetBufferSize(), NULL, &D2D_PS);
	m_pDevCtx->VSSetShader(m_pVS, 0, 0);
	m_pDevCtx->PSSetShader(m_pPS, 0, 0);



	m_dev->CreateInputLayout(ied, 3, VS->GetBufferPointer(), VS->GetBufferSize(), &m_pLayout);
	m_pDevCtx->IASetInputLayout(m_pLayout);



	
}

void CDirect3D::CamMove(float x, float y, float z)
{
	OutputDebugString(_T("Cam Move\n"));


	camTarget = XMVectorSet(m_camTargetX+=x, m_camTargetY+=y, m_camTargetZ+=z, 0.0f);

}

void CDirect3D::RanderFrame(void)
{
	
	if (!m_pDevCtx)
		return;
	float color[4] = { 0.0f, 0.0f, 0.0f,0.0f };
	
	rot += .030f;
	if (rot > 6.28f)
		rot = 0.0f;

	CString str;
	str.Format(_T("%f\n"), rot);
	OutputDebugString(str);
	// copy the vertices into the buffer
	D3D11_MAPPED_SUBRESOURCE ms;
	ZeroMemory(&ms, sizeof(D3D11_MAPPED_SUBRESOURCE));

	m_pDevCtx->Map(m_pVBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    // map the buffer
	memcpy(ms.pData, OurVertices, sizeof(OurVertices));                 // copy the data
	m_pDevCtx->Unmap(m_pVBuffer, NULL);                                      // unmap the buffer


	UINT stride = sizeof(VERTEX);
	UINT offset = 0;

	m_pDevCtx->IASetVertexBuffers(0, 1, &m_pVBuffer, &stride, &offset);
	m_pDevCtx->IASetIndexBuffer(m_pIdxBuffer, DXGI_FORMAT_R32_UINT, 0);
	m_pDevCtx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);



	m_pDevCtx->ClearRenderTargetView(m_pRenderTargetView, color);

	//Refresh the Depth/Stencil view
	m_pDevCtx->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	m_constbuffPerFrame.light = m_light;
	m_pDevCtx->UpdateSubresource(m_cbPerFrameBuffer, 0, NULL, &m_constbuffPerFrame, 0, 0);
	m_pDevCtx->PSSetConstantBuffers(0, 1, &m_cbPerFrameBuffer);
	//Camera information
	

	//Set the View matrix
	m_camMatrix = XMMatrixLookAtLH(camPosition, camTarget, camUp);

	//Set the Projection matrix
	m_projectionMatrix  = XMMatrixPerspectiveFovLH(0.4f * 3.14f, (float)800 / (float)600, 0.5f, 1000.0f);
	m_worldMatrix = XMMatrixIdentity();

	//Define cube1's world space matrix
	XMVECTOR rotaxis = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMMATRIX Rotation = XMMatrixRotationAxis(rotaxis, rot);
	XMMATRIX Translation = XMMatrixTranslation(0.0f, 0.0f, 0.0f);

	//Set cube1's world space using the transformations
	m_worldMatrix = Translation * Rotation;

	XMMATRIX WVP;
	WVP = m_worldMatrix *m_camMatrix* m_projectionMatrix;
	cbPerObj.WVP = XMMatrixTranspose(WVP);
	cbPerObj.World = XMMatrixTranspose(m_worldMatrix);

	//m_pDevCtx->UpdateSubresource(m_pCBPerBuffer, 0, NULL, &cbPerObj, 0, 0);
	
	ZeroMemory(&ms, sizeof(D3D11_MAPPED_SUBRESOURCE));

	m_pDevCtx->Map(m_pCBPerBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    // map the buffer
	memcpy(ms.pData, &cbPerObj, sizeof(cbPerObj));                 // copy the data
	m_pDevCtx->Unmap(m_pCBPerBuffer, NULL);                                      // unmap the buffer
	m_pDevCtx->VSSetConstantBuffers(0, 1, &m_pCBPerBuffer);


	m_pDevCtx->PSSetShaderResources(0, 1, &m_CubesTexture);
	m_pDevCtx->PSSetSamplers(0, 1, &m_CubesTexSamplerState);


	m_pDevCtx->DrawIndexed(36,0,0);

	
	m_worldMatrix = XMMatrixIdentity();

	//Define cube1's world space matrix
	 rotaxis = XMVectorSet(0.0f, 1.0f, 0.3f, 0.0f);
	 XMMATRIX Rotation2 = XMMatrixRotationAxis( XMVectorSet(0.0f, 0.0, 1.0f, 0.0f),rot);
	 Rotation = XMMatrixRotationAxis(rotaxis, rot);
	 Translation = XMMatrixTranslation(0.0f, 0.0f, 3.0f);
	 m_worldMatrix = XMMatrixScaling(0.8f, 0.8f, 0.8f)* Translation * Rotation ;


	WVP = m_worldMatrix * m_camMatrix * m_projectionMatrix;
	cbPerObj.WVP = XMMatrixTranspose(WVP);
	cbPerObj.World = XMMatrixTranspose(m_worldMatrix);
	//m_pDevCtx->UpdateSubresource(m_pCBPerBuffer, 0, NULL, &cbPerObj, 0, 0);

	ZeroMemory(&ms, sizeof(D3D11_MAPPED_SUBRESOURCE));

	m_pDevCtx->Map(m_pCBPerBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    // map the buffer
	memcpy(ms.pData, &cbPerObj, sizeof(cbPerObj));                 // copy the data
	m_pDevCtx->Unmap(m_pCBPerBuffer, NULL);                                      // unmap the buffer
	m_pDevCtx->VSSetConstantBuffers(0, 1, &m_pCBPerBuffer);

	m_pDevCtx->DrawIndexed(36, 0, 0);
	


	m_swapChain->Present(0, 0);
	
}

HRESULT CDirect3D::LoadTexture(CString strTextureFilePath)
{
	ScratchImage image;

	HRESULT hr = 0;
	hr = CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);
	if (FAILED(hr))
	{
		AfxMessageBox(_T("Error CoInitializeEx\r\n") + strTextureFilePath);
		goto EXIT;
	}

	hr= LoadFromWICFile(strTextureFilePath.GetBuffer(), WIC_FLAGS_NONE, nullptr, image);
	
	if (FAILED(hr))
	{
		CString str;
		str.Format(_T("%x\r\n"), hr);
		AfxMessageBox(_T("Error LoadFromWICFile\r\n ")+str + strTextureFilePath);
		goto EXIT;

		
	}

	hr = CreateShaderResourceView(m_dev, image.GetImages(), image.GetImageCount(), image.GetMetadata(), &m_CubesTexture);

	if (FAILED(hr))
	{
		AfxMessageBox(_T("Error CreateShaderResourceView\r\n") + strTextureFilePath);
		goto EXIT;
	}

EXIT:

	return hr;
}
BOOL CDirect3D::initD3D(HWND hWnd)
{

	DXGI_MODE_DESC bufferDesc;

	ZeroMemory(&bufferDesc, sizeof(DXGI_MODE_DESC));

	bufferDesc.Width = 800;
	bufferDesc.Height = 600;
	bufferDesc.RefreshRate.Numerator = 60;
	bufferDesc.RefreshRate.Denominator = 1;
	bufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	DXGI_SWAP_CHAIN_DESC scd;
	HRESULT hResult = 0;
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	scd.BufferDesc = bufferDesc;
	scd.BufferCount = 1;                                   // one back buffer
//	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;    // use 32-bit color
	//scd.BufferDesc.Width = 800;                   // set the back buffer width
//	scd.BufferDesc.Height = 600;                 // set the back buffer height
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;     // how swap chain is to be used
	scd.OutputWindow = hWnd;                               // the window to be used
	scd.SampleDesc.Count = 1;                              // how many multisamples
	scd.Windowed = TRUE;                                   // windowed/full-screen mode
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;    // allow full-screen switching


	hResult = D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		NULL,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&scd,
		&m_swapChain,
		&m_dev,
		NULL,
		&m_pDevCtx
	);

	if (!SUCCEEDED(hResult))
	{
		CleanD3D();
		return FALSE;
	}

	


	ID3D11Texture2D* pBackBuffer;
	m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	m_dev->CreateRenderTargetView(pBackBuffer, NULL, &m_pRenderTargetView);
	pBackBuffer->Release();
	///////////////**************new**************////////////////////

	//Describe our Depth/Stencil Buffer
	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width = 800;
	depthStencilDesc.Height = 600;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;
	m_dev->CreateTexture2D(&depthStencilDesc, NULL, &depthStencilBuffer);
	m_dev->CreateDepthStencilView(depthStencilBuffer, NULL, &depthStencilView);

	m_pDevCtx->OMSetRenderTargets(1, &m_pRenderTargetView, depthStencilView);
	D3D11_BUFFER_DESC cbbd;
	ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));

	cbbd.Usage = D3D11_USAGE_DEFAULT;
	cbbd.ByteWidth = sizeof(cbPerFrame);
	cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbbd.CPUAccessFlags = 0;
	cbbd.MiscFlags = 0;

	m_dev->CreateBuffer(&cbbd, NULL, &m_cbPerFrameBuffer);


	// Set the viewport

	ZeroMemory(&m_viewPort, sizeof(D3D11_VIEWPORT));



	m_viewPort.TopLeftX = 0;
	m_viewPort.TopLeftY = 0;
	m_viewPort.Width = 800;
	m_viewPort.Height = 600;
	///////////////**************new**************////////////////////
	m_viewPort.MinDepth = 0.0f;
	m_viewPort.MaxDepth = 1.0f;
	///////////////**************new**************////////////////////

	m_pDevCtx->RSSetViewports(1, &m_viewPort);



	

	
 //Create the buffer to send to the cbuffer in effect file
	//D3D11_BUFFER_DESC cbbd;
	ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));

	cbbd.Usage = D3D11_USAGE_DYNAMIC;
	cbbd.ByteWidth = sizeof(cbPerObject);
	cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbbd.MiscFlags = 0;

	HRESULT hr = m_dev->CreateBuffer(&cbbd, NULL, &m_pCBPerBuffer);

	

	if (FAILED(LoadTexture(_T("1.jpg"))))
	{
		CleanD3D();
		return FALSE;
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
	hr = m_dev->CreateSamplerState(&sampDesc, &m_CubesTexSamplerState);
	if (FAILED(hr))
	{

		CleanD3D();
		return FALSE;
	}

	InitPipeline();
	InitGraphics();

	return TRUE;
}

void CDirect3D::CleanD3D()
{
	if (m_pLayout)
	{
		m_pLayout->Release();

	}
	if (m_pPS)
	{
		m_pPS->Release();
		m_pPS = NULL;
	}
	if (m_pVS)
	{
		m_pVS->Release();
		m_pVS = NULL;
	}
	if (m_pVBuffer)
	{
		m_pVBuffer->Release();
	}
	if (m_pCBPerBuffer)
	{
		m_pCBPerBuffer->Release();
	}
	if (m_swapChain)
	{
		m_swapChain->Release();
		m_swapChain = NULL;
	}
	m_cbPerFrameBuffer->Release();
	depthStencilView->Release();
	depthStencilBuffer->Release();
	if (m_pRenderTargetView)
	{
		m_pRenderTargetView->Release();
		m_pRenderTargetView = NULL;
	}
	if (m_dev)
	{
		m_dev->Release();
		m_dev = NULL;
	}
	if (m_pDevCtx)
	{
		m_pDevCtx->Release();
		m_pDevCtx = NULL;
	}


}