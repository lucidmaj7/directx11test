#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

struct VERTEX { 
	float X, Y, Z; 	
	float Color[4];
};    // a struct to define a vertex


struct cbPerObject
{
	XMMATRIX  WVP;
};


class CDirect3D
{
public:
	VERTEX OurVertices[8];
	CDirect3D();
	~CDirect3D();
	float rot;
	BOOL initD3D(HWND hWnd);
	void CleanD3D();
	void RanderFrame(void);
	
	float color[4];
	XMVECTOR	camPosition;
	XMVECTOR	camTarget;
	XMVECTOR	camUp;
	float m_camTargetX; float m_camTargetY; float m_camTargetZ;
	void CDirect3D::CamMove(float x, float y, float z);
private:
	ID3D11DepthStencilState* m_depthStencilState;
	ID3D11DepthStencilView* depthStencilView;
	ID3D11Texture2D* depthStencilBuffer;
	XMMATRIX m_projectionMatrix;
	XMMATRIX m_worldMatrix;
	XMMATRIX m_camMatrix;;
	D3D11_VIEWPORT m_viewPort;
	ID3D11Buffer* m_pVBuffer;
	ID3D11Buffer* m_pIdxBuffer;
	ID3D11Buffer* m_pCBPerBuffer;
	ID3D11Device* m_dev;
	ID3D11VertexShader* m_pVS;    // the vertex shader
	ID3D11PixelShader* m_pPS;     // the pixel shader
	ID3D11DeviceContext* m_pDevCtx;
	IDXGISwapChain* m_swapChain;
	ID3D11InputLayout* m_pLayout;
	ID3D11RenderTargetView* m_pRenderTargetView; 
	cbPerObject cbPerObj;
private:
	void InitPipeline();

	void InitGraphics();
};

