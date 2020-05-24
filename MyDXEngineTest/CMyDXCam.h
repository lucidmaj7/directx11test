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

	void MoveLeftPosition();
	void MoveRightPosition();
	void MoveFrontPosition();
	void MoveBackPosition();

	void MoveLeftTarget();
	void MoveRightTarget();
	void MoveUpTarget();
	void MoveDownTarget();
	void UpdateCamera();
private:
	DWORD m_dwWidth;
	DWORD m_dwHeight;
	DirectX::XMVECTOR m_camPosition;
	DirectX::XMVECTOR m_camTarget;
	DirectX::XMVECTOR m_camUp;
	DirectX::XMMATRIX m_camMatrix;
	DirectX::XMMATRIX m_projectionMatrix;
	DirectX::XMMATRIX m_worldMatrix;

	XMVECTOR m_DefaultForward;
	XMVECTOR m_DefaultRight;
	XMVECTOR m_camForward;
	XMVECTOR m_camRight;

	XMMATRIX m_camRotationMatrix;
	XMMATRIX m_groundWorld;

	float m_moveLeftRight;
	float m_moveBackForward;

	float m_camYaw;
	float m_camPitch;

};

