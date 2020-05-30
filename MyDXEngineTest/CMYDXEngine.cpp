
#include "CMYDXEngine.h"


CMYDXEngine::CMYDXEngine()
{
	
	
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

	
	light.ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);

	m_MyDX3D.BeginScene();

	for (auto iter = m_vtModelList.begin(); iter != m_vtModelList.end(); iter++)
	{
		(*iter)->Render(&m_Camera ,light);
	}
	
	m_MyDX3D.EndScene();
}

BOOL CMYDXEngine::AddModel(CMyDXModel* pModel)
{
	m_vtModelList.push_back(pModel);
	return TRUE;
}