#include "MyDXEngineCommon.h"
#include "CMyDXCam.h"
CMyDXCam::CMyDXCam() {

}
CMyDXCam::~CMyDXCam(){

}
XMMATRIX CMyDXCam::GetWorldMatrix()
{
	return m_worldMatrix;
}
XMMATRIX CMyDXCam::GetProjectionMatrix()
{
	return m_projectionMatrix;
}
XMMATRIX CMyDXCam::GetCameraMetrix()
{
	return m_camMatrix;
}

BOOL CMyDXCam::CameraInitialize(
	DWORD dwWidth,
	DWORD dwHeight
	) {
	m_dwWidth = dwWidth;
	m_dwHeight = dwHeight;
	m_camPosition = XMVectorSet(0.0f, 3.0f, -5.0f, 0.0f);
	m_camTarget = XMVectorSet(0.f, 0.f, 0.f, 0.0f);
	m_camUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	
	m_camMatrix = XMMatrixLookAtLH(m_camPosition, m_camTarget, m_camUp);
	m_worldMatrix = XMMatrixIdentity();
	m_projectionMatrix = XMMatrixPerspectiveFovLH(0.4f * 3.14f, (float)m_dwWidth / (float)m_dwHeight, 0.5f, 1000.0f);
	return TRUE;
}

BOOL CMyDXCam::CameraInitialize(
	DWORD dwWidth, 
	DWORD dwHeight, 
	XMVECTOR camPosition, 
	XMVECTOR camTarget, 
	XMVECTOR camUp)
{
	BOOL bSuccess = TRUE;

	m_dwWidth = dwWidth;
	m_dwHeight = dwHeight;
	m_camPosition = camPosition;
	m_camTarget = camTarget;
	m_camUp = camUp;
	
	m_camMatrix = XMMatrixLookAtLH(camPosition, camTarget, camUp);
	m_worldMatrix = XMMatrixIdentity();
	m_projectionMatrix = XMMatrixPerspectiveFovLH(0.4f * 3.14f, (float)m_dwWidth / (float)m_dwHeight, 0.5f, 1000.0f);

	return bSuccess;
}
