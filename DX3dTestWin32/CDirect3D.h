#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

struct VERTEX { 
	VERTEX() {};
	VERTEX(float x, float y, float z, float u, float v,float norX, float norY, float norZ )
	{
		X = x;
		Y = y;
		Z = z;
		texCoord.x = u;
		texCoord.y = v;
		normal.x = norX;
		normal.y = norY;
		normal.z = norZ;
	}

	float X, Y, Z; 	
//	float Color[4];

	XMFLOAT2 texCoord;
	XMFLOAT3 normal;
};    // a struct to define a vertex
	// create the input layout object

struct cbPerObject
{
	XMMATRIX  WVP;
	XMMATRIX  World;
};

struct Light
{
	Light()
	{
		ZeroMemory(this, sizeof(Light));
	}
	XMFLOAT3 dir;
	float pad;
	XMFLOAT4 ambient;
	XMFLOAT4 diffuse;
};

struct cbPerFrame
{
	Light  light;
};


class CDirect3D
{
public:
	VERTEX OurVertices[24];
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
	void CamMove(float x, float y, float z);
private:
	Light m_light;
	cbPerFrame m_constbuffPerFrame;
	HRESULT LoadTexture(const WCHAR* strTextureFilePath);
	ID3D11ShaderResourceView* m_CubesTexture;
	ID3D11SamplerState* m_CubesTexSamplerState;
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
	ID3D11Buffer* m_cbPerFrameBuffer;
	ID3D11Device* m_dev;
	ID3D11VertexShader* m_pVS;    // the vertex shader
	ID3D11PixelShader* m_pPS;     // the pixel shader
	ID3D11PixelShader* D2D_PS;

	ID3D11DeviceContext* m_pDevCtx;
	IDXGISwapChain* m_swapChain;
	ID3D11InputLayout* m_pLayout;
	ID3D11RenderTargetView* m_pRenderTargetView; 
	cbPerObject cbPerObj;
private:
	void InitPipeline();

	void InitGraphics();
};

