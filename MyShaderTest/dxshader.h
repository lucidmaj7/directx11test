#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib,"D3DCompiler.lib")




HRESULT InitializeDX3D(HWND hwnd, UINT width, UINT height);

void DX3DCleanup();
void BeginScene();
void EndScene();




