
#include "CMYDXEngine.h"


CMYDXEngine::CMYDXEngine()
{
	
	light.dir = XMFLOAT3(0.0f, 0.8f, -0.5f);
	light.ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	light.diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
}



CMYDXEngine::~CMYDXEngine()
{
	
	m_MyDX3D.CleanUp();
}
BOOL CMYDXEngine::Initialize(HWND hWnd, float fWidth, float fHeight)
{
	BOOL bSuccess = FALSE;

	bSuccess = m_MyDX3D.Initialize(hWnd, fWidth, fHeight);

	bSuccess = m_Camera.CameraInitialize(fWidth, fHeight);
	
	return bSuccess;
}

void CMYDXEngine::Render()
{

	
	m_MyDX3D.BeginScene();

	for (auto iter = m_vtModelList.begin(); iter != m_vtModelList.end(); iter++)
	{
		(*iter)->Render(m_Camera.GetWorldMatrix(), m_Camera.GetProjectionMatrix(), m_Camera.GetCameraMetrix(), light);
	}
	
	m_MyDX3D.EndScene();
}

BOOL CMYDXEngine::AddModel(CMyDXModel* pModel)
{
	m_vtModelList.push_back(pModel);
	return TRUE;
}