#include "MyDXEngineCommon.h"
#include "CMyDXModel.h"



CMyDXModel::CMyDXModel()
{
	
	m_TransformMatrix = XMMatrixIdentity();
}
CMyDXModel::~CMyDXModel()
{

}
void CMyDXModel::setTransformMatrix(XMMATRIX matrix)
{
	m_TransformMatrix = matrix;
}


void CMyDXModel::Render(XMMATRIX worldMatrix, XMMATRIX projectionMatrix, XMMATRIX camMatrix, Light light)
{

	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	D3D11_MAPPED_SUBRESOURCE ms;
	
	cbPerObject cbPerObj;
	DirectX::XMMATRIX WVP;

	m_pDxDevCtx->VSSetShader(m_pVertexShader, 0, 0);
	m_pDxDevCtx->PSSetShader(m_pPixelShader, 0, 0);
	m_pDxDevCtx->PSSetShaderResources(0, 1, &m_pTexture);
	m_pDxDevCtx->PSSetSamplers(0, 1, &m_TextureSamplerState);
	m_pDxDevCtx->IASetInputLayout(m_pLayout);

	m_pDxDevCtx->IASetVertexBuffers(0, 1, &m_pVBuffer, &stride, &offset);
//	m_pDxDevCtx->IASetIndexBuffer(m_pIdxBuffer, DXGI_FORMAT_R32_UINT, 0);
	m_pDxDevCtx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	m_constbuffPerFrame.light = light;

	ZeroMemory(&ms, sizeof(D3D11_MAPPED_SUBRESOURCE));
	m_pDxDevCtx->Map(m_cbPerFrameBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    // map the buffer
	memcpy(ms.pData, &m_constbuffPerFrame, sizeof(m_constbuffPerFrame));                 // copy the data
	m_pDxDevCtx->Unmap(m_cbPerFrameBuffer, NULL);                                      // unmap the buffer
	m_pDxDevCtx->PSSetConstantBuffers(0, 1, &m_cbPerFrameBuffer);
	
	WVP = worldMatrix * m_TransformMatrix * camMatrix * projectionMatrix;
	cbPerObj.WVP = XMMatrixTranspose(WVP);
	cbPerObj.World = XMMatrixTranspose(worldMatrix* m_TransformMatrix);

	ZeroMemory(&ms, sizeof(D3D11_MAPPED_SUBRESOURCE));
	m_pDxDevCtx->Map(m_pCBPerBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    // map the buffer
	memcpy(ms.pData, &cbPerObj, sizeof(cbPerObj));                 // copy the data
	m_pDxDevCtx->Unmap(m_pCBPerBuffer, NULL);                                      // unmap the buffer
	m_pDxDevCtx->VSSetConstantBuffers(0, 1, &m_pCBPerBuffer);

	m_pDxDevCtx->Draw(m_vertexListSize, 0);
	//m_pDxDevCtx->DrawIndexed(m_dwIndexListSize, 0, 0);
}

HRESULT CMyDXModel::LoadTexture(const WCHAR* strTextureFilePath)
{
	DirectX::ScratchImage image;

	HRESULT hr = 0;
	hr = CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);
	if (FAILED(hr))
	{
		//AfxMessageBox(_T("Error CoInitializeEx\r\n") + strTextureFilePath);
		goto EXIT;
	}

	hr = LoadFromWICFile(strTextureFilePath, DirectX::WIC_FLAGS_NONE, nullptr, image);

	if (FAILED(hr))
	{

		goto EXIT;


	}

	hr = CreateShaderResourceView(m_pDxDev, image.GetImages(), image.GetImageCount(), image.GetMetadata(), &m_pTexture);

	if (FAILED(hr))
	{
		if (m_pTexture)
		{
			m_pTexture->Release();
			m_pTexture = NULL;
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
	hr = m_pDxDev->CreateSamplerState(&sampDesc, &m_TextureSamplerState);


EXIT:

	return hr;
}
BOOL CMyDXModel::LoadShader()
{

	


	if (FAILED(D3DCompileFromFile(L"shaders.shader", 0, 0, "PShader", "ps_4_0", 0, 0, &PS, 0)))
	{
		OutputDebugString(L"D3DCompileFromFile error ps\n");
		return FALSE;
	}
	if (FAILED(D3DCompileFromFile(L"shaders.shader", 0, 0, "VShader", "vs_4_0", 0, 0, &VS, 0)))
	{
		OutputDebugString(L"D3DCompileFromFile error vs\n");
		return FALSE;
	}

	m_pDxDev ->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &m_pVertexShader);
	m_pDxDev->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &m_pPixelShader);
	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	m_pDxDev->CreateInputLayout(ied, 3, VS->GetBufferPointer(), VS->GetBufferSize(), &m_pLayout);
	

	//VS->Release();
	//PS->Release();

	return TRUE;


}
void CMyDXModel::CleanUp()
{
	m_Object.CleanUp();

	if (m_cbPerFrameBuffer)
	{
		m_cbPerFrameBuffer->Release();
		m_cbPerFrameBuffer = NULL;
	}
	if (m_TextureSamplerState)
	{
		m_TextureSamplerState->Release();
		m_TextureSamplerState = NULL;
	}
	if (m_pTexture)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
	if (m_pVBuffer)
	{
		m_pVBuffer->Release();
		m_pVBuffer = NULL;
	}
	if (m_pIdxBuffer)
	{
		m_pIdxBuffer->Release();
		m_pIdxBuffer = NULL;
	}
	if (m_dwIndexList)
	{
		delete[] m_dwIndexList;
		m_dwIndexList = NULL;
	}
	if (m_pPixelShader)
	{
		m_pPixelShader->Release();
		m_pPixelShader = NULL;
	}
	if (m_pVertexShader)
	{
		m_pVertexShader->Release();
		m_pVertexShader = NULL;
	}
	
	if (m_pVertexList)
	{
		delete[] m_pVertexList;
		m_pVertexList = NULL;
	}
}

BOOL CMyDXModel::InitalizeModel( ID3D11Device* pDxDev,  ID3D11DeviceContext* pDxDevCtx, const WCHAR* pszModelObjFile, const WCHAR* pszTextureFile)
{

	BOOL bSuccess = TRUE;
	D3D11_BUFFER_DESC bd;

	m_pDxDev = pDxDev;
	m_pDxDevCtx = pDxDevCtx;



	if (m_Object.LoadObjFile(pszModelObjFile) != 0)
	{
		bSuccess = FALSE;
		goto EXIT;
	}
	m_vertexListSize = m_Object.m_VertexListSize;
	m_pVertexList = m_Object.m_pVERTEXList;

	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
	bd.ByteWidth = sizeof(VERTEX) * m_vertexListSize;             // size is the VERTEX struct * 3
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer
	
	if (FAILED(m_pDxDev->CreateBuffer(&bd, NULL, &m_pVBuffer)))      // create the buffer
	{
		OutputDebugString(L"vertext buffer error");
		bSuccess = FALSE;
		goto EXIT;
	}

/*	D3D11_BUFFER_DESC idxBufDesc;
	ZeroMemory(&idxBufDesc, sizeof(idxBufDesc));
	idxBufDesc.Usage = D3D11_USAGE_DYNAMIC;
	idxBufDesc.ByteWidth = sizeof(UINT) * m_dwIndexListSize;
	idxBufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	idxBufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer

	if (FAILED(m_pDxDev->CreateBuffer(&idxBufDesc, NULL, &m_pIdxBuffer)))
	{
		OutputDebugString(L"Index Buffer error\n");
		bSuccess = FALSE;
		goto EXIT;
	}

	*/
	if (LoadShader() == FALSE)
	{
		OutputDebugString(L"LoadShader error\n");
		bSuccess = FALSE;
		goto EXIT;
	}
	if (FAILED(LoadTexture(pszTextureFile)))
	{
		OutputDebugString(L"LoadTexture error\n");
		bSuccess = FALSE;
		goto EXIT;
	}

	D3D11_BUFFER_DESC cbbd;
	ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));

	cbbd.Usage = D3D11_USAGE_DYNAMIC;
	cbbd.ByteWidth = sizeof(cbPerObject);
	cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbbd.MiscFlags = 0;
	m_pDxDev->CreateBuffer(&cbbd, NULL, &m_pCBPerBuffer);

	ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));

	cbbd.Usage = D3D11_USAGE_DYNAMIC;
	cbbd.ByteWidth = sizeof(cbPerFrame);
	cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	
	cbbd.MiscFlags = 0;
	m_pDxDev->CreateBuffer(&cbbd, NULL, &m_cbPerFrameBuffer);

	D3D11_MAPPED_SUBRESOURCE ms;
	ZeroMemory(&ms, sizeof(D3D11_MAPPED_SUBRESOURCE));
	m_pDxDevCtx->Map(m_pVBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    // map the buffer
	memcpy(ms.pData, m_pVertexList, sizeof(VERTEX) * m_vertexListSize);                 // copy the data
	m_pDxDevCtx->Unmap(m_pVBuffer, NULL);                                      // unmap the buffer
	/*
	ZeroMemory(&ms, sizeof(D3D11_MAPPED_SUBRESOURCE));
	m_pDxDevCtx->Map(m_pIdxBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    // map the buffer
	memcpy(ms.pData, m_dwIndexList, sizeof(DWORD) * m_dwIndexListSize);                 // copy the data
	m_pDxDevCtx->Unmap(m_pIdxBuffer, NULL);

	*/

	

EXIT:
	if(bSuccess == FALSE)
		CleanUp();

	return bSuccess;

}