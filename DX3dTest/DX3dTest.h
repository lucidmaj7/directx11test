
// DX3dTest.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CDX3dTestApp:
// See DX3dTest.cpp for the implementation of this class
//

class CDX3dTestApp : public CWinApp
{
public:
	CDX3dTestApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CDX3dTestApp theApp;
