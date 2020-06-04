#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <vector>
#include <atlstr.h>
#include "DirectXTex/DirectXTex.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib,"D3DCompiler.lib")
#pragma comment(lib, "DirectXTex\\lib\\DirectXTexd.lib")



using namespace std;
using namespace DirectX;

template<size_t T>
class AlignedAllocationPolicy {
public: 
	static void* operator new(size_t size) {
		return _aligned_malloc(size, T);

	}
	static void operator delete(void* memory)
	{
		_aligned_free(memory);
	}
};

typedef struct _VERTEX {
	_VERTEX() {};
	_VERTEX(float x, float y, float z, float u, float v, float norX, float norY, float norZ)
	{
		X = x;
		Y = y;
		Z = z;
		texCoord.x = u;
		texCoord.y = v;
		normal.x = norX;
		normal.y = norY;
		normal.z = norZ;
	}
	float X, Y, Z;
	DirectX::XMFLOAT2 texCoord;
	DirectX::XMFLOAT3 normal;
}VERTEX ,*PVERTEX;


struct Light
{
	Light()
	{
		ZeroMemory(this, sizeof(Light));
	}

	XMFLOAT4 ambient;
};

struct Material {
	Material(float specular)
	{
		this->specular = XMFLOAT4(specular, specular, specular, 1.f);
	}
	Material(XMFLOAT4 specular) {
		this->specular = specular;

	}
	Material() {
		this->specular = XMFLOAT4(0.f,0.f,0.f,1.f);

	}
	XMFLOAT4 specular;

};

struct cbPerFrame {
	Material matrial;
	Light light;
};

struct cbPerObject
{
	DirectX::XMMATRIX  WorldMatrix;
	DirectX::XMMATRIX ViewMatrix;
	DirectX::XMMATRIX ProjectionMatrix;
	XMFLOAT4 lightPos;
	XMFLOAT4 camPos;
};

