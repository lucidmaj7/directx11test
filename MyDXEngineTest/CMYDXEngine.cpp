
#include "CMYDXEngine.h"


#pragma comment(lib, "d3d11.lib")

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

	return bSuccess;
}
void CMYDXEngine::Render()
{
	m_MyDX3D.BeginScene();


	m_MyDX3D.EndScene();
}