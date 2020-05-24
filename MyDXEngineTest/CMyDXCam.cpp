#include "MyDXEngineCommon.h"
#include "CMyDXCam.h"
CMyDXCam::CMyDXCam() {

	m_DefaultForward = XMVectorSet(0.f, 0.f, 1.0f, 0.0f);
	m_DefaultRight = XMVectorSet(1.f, 0.f, 0.f, 0.f);
	m_camForward = XMVectorSet(0.f, 0.f, 1.0f, 0.0f);
	m_camRight = XMVectorSet(1.f, 0.f, 0.f, 0.f);
	m_moveLeftRight = 0.f;
	m_moveBackForward = 0.f;
	m_camYaw = 0.f;
	m_camPitch = 0.f;


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

void CMyDXCam::MoveLeftPosition()
{
	m_moveLeftRight -= 0.5f;
	UpdateCamera();
}

void CMyDXCam::MoveRightPosition()
{
	m_moveLeftRight += 0.5f;
	UpdateCamera();
}

void CMyDXCam::MoveFrontPosition()
{
	m_moveBackForward += 0.5f;
	UpdateCamera();
}

void CMyDXCam::MoveBackPosition()
{
	m_moveBackForward -= 0.5f;
	UpdateCamera();
}

void CMyDXCam::MoveLeftTarget()
{
	m_camYaw -= 0.02f;
	UpdateCamera();
}

void CMyDXCam::MoveRightTarget()
{
	m_camYaw += 0.02f;
	UpdateCamera();
}

void CMyDXCam::MoveUpTarget()
{
	m_camPitch -= 0.02f;
	UpdateCamera();
}

void CMyDXCam::MoveDownTarget()
{
	m_camPitch += 0.02f;
	UpdateCamera();

}
void CMyDXCam::UpdateCamera()
{
	m_camRotationMatrix = XMMatrixRotationRollPitchYaw(m_camPitch, m_camYaw, 0);
	m_camTarget = XMVector3TransformCoord(m_DefaultForward, m_camRotationMatrix);
	m_camTarget = XMVector3Normalize(m_camTarget);

	XMMATRIX RotateTempMatrix;
	RotateTempMatrix = XMMatrixRotationY(m_camYaw);

	m_camRight = XMVector3TransformCoord(m_DefaultRight, RotateTempMatrix);
	m_camUp = XMVector3TransformCoord(m_camUp, RotateTempMatrix);
	m_camForward = XMVector3TransformCoord(m_DefaultForward, RotateTempMatrix);

	m_camPosition  += m_moveLeftRight * m_camRight;
	m_camPosition += m_moveBackForward * m_camForward;
	m_moveLeftRight = 0.0f;
	m_moveBackForward = 0.f;
	
	m_camTarget = m_camPosition + m_camTarget;
	m_camMatrix = XMMatrixLookAtLH(m_camPosition, m_camTarget, m_camUp);

}

BOOL CMyDXCam::CameraInitialize(
	DWORD dwWidth,
	DWORD dwHeight
	) {
	m_dwWidth = dwWidth;
	m_dwHeight = dwHeight;
	m_camPosition = XMVectorSet(0.0f, 40.0f, -70.0f, 0.0f);
	m_camTarget = XMVectorSet(0.f, 0.f, 0.f, 0.0f);
	m_camUp = XMVectorSet(0.0f, 0.5f, 0.0f, 0.0f);

	
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