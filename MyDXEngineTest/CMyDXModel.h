#pragma once


class CMyDXModel: public AlignedAllocationPolicy<16>
{
public:
	CMyDXModel();
	~CMyDXModel();
	BOOL InitalizeModel( ID3D11Device* pDxDev,  ID3D11DeviceContext* pDxDevCtx, const WCHAR* pszTextureFile);
	void Render(XMMATRIX worldMatrix, XMMATRIX projectionMatrix, XMMATRIX camMatrix);
	void CleanUp();

	void setTransformMatrix(XMMATRIX matrix);

	XMMATRIX GetTranslation();
private:
	BOOL LoadShader();
	HRESULT LoadTexture(const WCHAR* strTextureFilePath);
	
private:
	float rot;
	ID3D11Buffer* m_pCBPerBuffer;
	XMMATRIX m_TransformMatrix;
	
	PVERTEX m_pVertexList;
	DWORD m_vertexListSize;
	DWORD* m_dwIndexList;
	DWORD m_dwIndexListSize;
	ID3DBlob* VS; ID3DBlob *PS;
	ID3D11Buffer* m_pVBuffer;
	ID3D11Buffer* m_pIdxBuffer;
	 ID3D11Device* m_pDxDev;
	 ID3D11DeviceContext* m_pDxDevCtx;
	ID3D11ShaderResourceView* m_pTexture;
	ID3D11SamplerState* m_TextureSamplerState;
	ID3D11VertexShader* m_pVertexShader;    // the vertex shader
	ID3D11PixelShader* m_pPixelShader;     // the pixel shader
	ID3D11InputLayout* m_pLayout;
};

