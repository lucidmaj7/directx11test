// 3DObjLoaderTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "C3DObject.h"
int main()
{
    std::cout << "Hello World!\n";
    C3DObject obj;
    obj.LoadObjFile(L"my.obj");
    
    std::cout << "vertex count : "<< obj.m_VertexListSize << " index size: "<< obj.m_indexSize<<endl;
    return 0;
}
