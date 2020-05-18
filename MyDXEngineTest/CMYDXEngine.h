#pragma once
#include "MyDXEngineCommon.h"
#include "CMYDX3D.h"
class CMYDXEngine
{
public:
	CMYDXEngine();
	~CMYDXEngine();
	BOOL Initialize(HWND hWnd, float fWidth, float fHeight);
	void Render();
private:
	CMYDX3D m_MyDX3D;
};

