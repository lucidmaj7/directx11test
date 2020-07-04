#pragma once


#include "MyDXEngineCommon.h"
class C3DObject: public AlignedAllocationPolicy<16>
{


public:
	C3DObject();
	~C3DObject();
	enum INPUT_DATA_EUM {
		NONE = 0,
		VINPUT ,
		NINPUT,
		TINPUT,
		FINPUT,
	} ;
	typedef struct _VInput {
		_VInput(float _x, float _y, float _z)
		{
			x = _x;
			y = _y;
			z = _z;
			
		}
		float x;
		float y;
		float z;
	}VInput;
	typedef struct _NInput {
		_NInput(float _x, float _y, float _z)
		{
			x = _x;
			y = _y;
			z = _z;
		}
		float x;
		float y;
		float z;
	}NInput;
	typedef struct _TInput {
		_TInput(float _u, float _v)
		{
			u = _u;
			v = _v;
		}
		float u;
		float v;
	}TInput;
	typedef struct _FInput {
		_FInput(UINT _v, UINT _t, UINT _n)
		{
			v = _v;
			n = _n;
			t = _t;
		}
		UINT v;
		UINT n;
		UINT t;
	}FInput;
public:
	int LoadObjFile(LPCWSTR pszObjFilePath);
	void CleanUp();
	PVERTEX m_pVERTEXList;
	DWORD m_VertexListSize;
	unsigned int* m_pindexList;
	DWORD m_indexSize;


	


};

