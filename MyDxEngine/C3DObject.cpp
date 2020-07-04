#include "MyDXEngineCommon.h"
#include "C3DObject.h"

#include <Shlwapi.h>

using namespace std;

C3DObject::C3DObject()
{
	m_pVERTEXList = NULL;
	m_VertexListSize = 0;
}

C3DObject::~C3DObject()
{
}

void C3DObject::CleanUp()
{
	if (m_pVERTEXList)
	{
		delete[] m_pVERTEXList;
		
	}
	m_VertexListSize = 0;
	if (m_pindexList)
	{
		delete[] m_pindexList;
	}
	m_indexSize = 0;
}

int C3DObject::LoadObjFile(LPCWSTR pszObjFilePath)
{
	int nRet = 0;
	char readLine[1024] = { 0, };
	HANDLE hFile = NULL;
	DWORD dwHighFilelen = 0;
	PBYTE fileContents = NULL;
	LARGE_INTEGER filelen;
	DWORD numOfReadByte = 0;
	DWORD filePos = 0;
	DWORD vertexCount = 0;
	DWORD textureCount = 0;
	DWORD normalCount = 0;
	DWORD faceCount = 0;
	vector<VInput> vinput;
	vector<TInput> tinput;
	vector<NInput> ninput;
	vector<FInput> finput;
	map<CString, VERTEX> vertexMap;
	map<CString, UINT> vertexIdxMap;
	filelen.QuadPart = 0;
	int i = 0;
	int j = 0;
	if (!PathFileExists(pszObjFilePath))
	{
		nRet = -1;
		goto EXIT;
	}

	hFile = CreateFile(pszObjFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);
	if (hFile == NULL || hFile == INVALID_HANDLE_VALUE)
	{
		nRet = -2;
		goto EXIT;
	}

	if (!GetFileSizeEx(hFile, &filelen))
	{
		nRet = -3;
		goto EXIT;
	}

	fileContents= new BYTE[filelen.QuadPart];


	if (!ReadFile(hFile, fileContents, filelen.QuadPart, &numOfReadByte, NULL))
	{
		nRet = -4;
		goto EXIT;
	}

	//get Count!!



	while (filePos < filelen.QuadPart)
	{

		char data =(char) fileContents[filePos];
		DWORD curLineStartPos = filePos;
		DWORD curLineEndPos = filePos;
		int curINPUT = NONE;
		BOOL lineIgnore = FALSE;
		while (fileContents[filePos] != '\n')
		{

			if (lineIgnore)
			{
				filePos++;
				continue;
			}
			if (filePos - curLineStartPos == 0)
			{	
				if (fileContents[filePos] != 'v' && fileContents[filePos] != 'f')
				{
					lineIgnore = TRUE;
					filePos++;
					continue;
				}
				else
				{
					lineIgnore = FALSE;

				}
			}
			
			if (fileContents[filePos] == 'v')
			{
				curINPUT = VINPUT;
				filePos++;
				if (fileContents[filePos] == ' ')
				{

					vertexCount++;
				}
				if (fileContents[filePos] == 't')
				{
					curINPUT = TINPUT;
					textureCount++;
				}
				if (fileContents[filePos] == 'n')
				{
					curINPUT = NINPUT;
					normalCount++;
				}
			}
			if (fileContents[filePos] == 'f')
			{
				curINPUT = FINPUT;
				filePos++;
				if (fileContents[filePos] == ' ')
				{
					faceCount++;
				}
			}
			

			filePos++;
		}
		ZeroMemory(readLine, sizeof(readLine));
		curLineEndPos = filePos;
		memcpy(readLine, &fileContents[curLineStartPos], curLineEndPos - curLineStartPos);
		readLine[curLineEndPos - curLineStartPos] = 0;

		if (lineIgnore == FALSE)
		{
			curLineEndPos = filePos;
			memcpy(readLine, &fileContents[curLineStartPos], curLineEndPos - curLineStartPos);
			readLine[curLineEndPos - curLineStartPos] = 0;

			if (curINPUT == VINPUT)
			{
				VInput input(0.f, 0.f, 0.f);

				sscanf_s(readLine, "v %f %f %f\n", &input.x, &input.y, &input.z);
		//		printf("v %f %f %f\n", input.x, input.y, input.z);
				vinput.push_back(input);
			}
			else if (curINPUT == NINPUT)
			{
				NInput input(0.f, 0.f, 0.f);

				sscanf_s(readLine, "vn %f %f %f\n", &input.x, &input.y, &input.z);
			//	printf("vn %f %f %f\n", input.x, input.y, input.z);
				ninput.push_back(input);
			}
			else if (curINPUT == TINPUT)
			{
				TInput input(0.f, 0.f);

				sscanf_s(readLine, "vt %f %f\n", &input.v, &input.u); //for blender texture..?
				
				tinput.push_back(input);
			}
			else if (curINPUT == FINPUT)
			{
				FInput input1(0, 0, 0);
				FInput input2(0, 0, 0);
				FInput input3(0, 0, 0);

				sscanf_s(readLine, "f %d/%d/%d %d/%d/%d %d/%d/%d\n", 
					&input1.v, &input1.t, &input1.n,
					 &input2.v, &input2.t, &input2.n,
					 &input3.v, &input3.t, &input3.n);

				finput.push_back(input1);
				finput.push_back(input2);
				finput.push_back(input3);
			}


		}

		filePos++;
	}
	


	for (auto iter = finput.begin(); iter != finput.end(); iter++)
	{
		CString str;
		str.Format(_T("%d/%d/%d"), (iter->v) - 1, (iter->t) - 1, (iter->n) - 1);
		
		vertexMap[str] = VERTEX(
			vinput[(iter->v) - 1].x, vinput[(iter->v) - 1].y, vinput[(iter->v) - 1].z,
			tinput[(iter->t) - 1].u, tinput[(iter->t) - 1].v,
			ninput[(iter->n) - 1].x, ninput[(iter->n) - 1].y, ninput[(iter->n) - 1].z);

	}
	m_VertexListSize= vertexMap.size();
	
	m_pVERTEXList = new VERTEX[m_VertexListSize];
	i = 0;
	for (auto iter = vertexMap.begin(); iter != vertexMap.end(); iter++)
	{
		
		m_pVERTEXList[i++] = iter->second;
		CString str;
		str.Format(_T("%d,%d,%d,%d,%d,%d"), iter->second.X, iter->second.Y, iter->second.Z, iter->second.normal.x, iter->second.normal.y, iter->second.normal.z);
		vertexIdxMap[str] = i-1;
		
	}

	m_indexSize = finput.size();
	m_pindexList = new unsigned int[m_indexSize];

	for (auto iter = finput.begin(); iter != finput.end(); iter++)
	{
		VERTEX vtx = VERTEX(
			vinput[(iter->v) - 1].x, vinput[(iter->v) - 1].y, vinput[(iter->v) - 1].z,
			tinput[(iter->t) - 1].u, tinput[(iter->t) - 1].v,
			ninput[(iter->n) - 1].x, ninput[(iter->n) - 1].y, ninput[(iter->n) - 1].z);
		int idx = 0;
		CString str;
		str.Format(_T("%d,%d,%d,%d,%d,%d"), vtx.X, vtx.Y, vtx.Z, vtx.normal.x, vtx.normal.y, vtx.normal.z);

		m_pindexList[j++] = vertexIdxMap[str];

	
	}

	nRet = 0;
EXIT:
	if (fileContents)
	{
		delete[] fileContents;
	}
	if (hFile)
	{
		CloseHandle(hFile);
		hFile = NULL;
	}

	return nRet;
}

