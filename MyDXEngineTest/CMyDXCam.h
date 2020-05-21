#pragma once
class CMyDXCam : public AlignedAllocationPolicy<16>
{
public:
	CMyDXCam();
	~CMyDXCam();
	BOOL CameraInitialize(
		DWORD dwWidth,
		DWORD dwHeight
		);
	BOOL CameraInitialize(
	
		DWORD dwWidth, 
		DWORD dwHeight,
		XMVECTOR camPosition, 
		XMVECTOR camTarget, 
		XMVECTOR camUp
	);
	XMMATRIX GetWorldMatrix();
	XMMATRIX GetProjectionMatrix();
	XMMATRIX GetCameraMetrix();

private:
	DWORD m_dwWidth;
	DWORD m_dwHeight;
	DirectX::XMVECTOR m_camPosition;
	DirectX::XMVECTOR m_camTarget;
	DirectX::XMVECTOR m_camUp;
	DirectX::XMMATRIX m_camMatrix;
	DirectX::XMMATRIX m_projectionMatrix;
	DirectX::XMMATRIX m_worldMatrix;

};

