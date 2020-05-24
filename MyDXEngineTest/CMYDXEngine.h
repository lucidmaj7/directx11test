#pragma once
#include "MyDXEngineCommon.h"
#include "CMYDX3D.h"
#include "CMyDXModel.h"
#include "CMyDXCam.h"

class CMYDXEngine : public AlignedAllocationPolicy<16>
{
public:
	CMYDXEngine();
	~CMYDXEngine();
	BOOL Initialize(HWND hWnd, float fWidth, float fHeight);
	BOOL AddModel(CMyDXModel* pModel);
	void Render();
	CMYDX3D m_MyDX3D;
	CMyDXCam m_Camera;
	Light light;
private:
	vector< CMyDXModel*> m_vtModelList;
	
	
};

