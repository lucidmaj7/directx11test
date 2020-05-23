// 3DObjLoaderTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "../MyDXEngineTest/C3DObject.h"
int main()
{
    std::cout << "Hello World!\n";
    C3DObject obj;
    obj.LoadObjFile(L"my.obj");
    return 0;
}
