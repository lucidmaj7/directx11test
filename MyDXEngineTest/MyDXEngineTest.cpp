// MyDXEngineTest.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "MyDXEngineTest.h"
#include "CMYDXEngine.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

CMYDXEngine gDXEngine;
CMyDXModel* pModel1;
CMyDXModel* pModel2;
CMyDXModel* pModel3;
float rot = 0;


HWND gWindow = NULL;
// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
void UpdateModel();
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MYDXENGINETEST, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MYDXENGINETEST));

    MSG msg;

    if (!gDXEngine.Initialize(gWindow, 800, 600))
    {
        ::MessageBox(gWindow, L"Fail Engine", L"ERROR", MB_ICONERROR | MB_OK);
        return -1;
    }
    //////////////////////////

    pModel1 = new CMyDXModel();
    pModel2 = new CMyDXModel();
    pModel3 = new CMyDXModel();
    if (pModel1->InitalizeModel(gDXEngine.m_MyDX3D.GetDXDevice(), gDXEngine.m_MyDX3D.GetDXDeviceContext(), L"1.jpg"))
    {
        gDXEngine.AddModel(pModel1);
    }
    if (pModel2->InitalizeModel(gDXEngine.m_MyDX3D.GetDXDevice(), gDXEngine.m_MyDX3D.GetDXDeviceContext(), L"2.jpg"))
    {
        gDXEngine.AddModel(pModel2);
    }
    if (pModel3->InitalizeModel(gDXEngine.m_MyDX3D.GetDXDevice(), gDXEngine.m_MyDX3D.GetDXDeviceContext(), L"3.jpg"))
    {
        gDXEngine.AddModel(pModel3);
    }

   

    ///////////////

    while (true)
    {
        BOOL PeekMessageL(
            LPMSG lpMsg,
            HWND hWnd,
            UINT wMsgFilterMin,
            UINT wMsgFilterMax,
            UINT wRemoveMsg
        );

        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                break;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else {
         
            UpdateModel();
            gDXEngine.Render();
        
        }
    }

    return (int) msg.wParam;
}


void UpdateModel()
{
    rot += .0100f;
    if (rot > 6.28f)
        rot = 0.0f;

    XMVECTOR rotaxis = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
    XMMATRIX Rotation = XMMatrixRotationAxis(rotaxis, rot);
    XMMATRIX Translation;

    pModel1->setTransformMatrix(Rotation);


    rotaxis = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    Rotation = XMMatrixRotationAxis(rotaxis, rot);
    Translation = XMMatrixTranslation(-3.0f, 0.0f, 0.0f);
    pModel2->setTransformMatrix(Rotation * Translation * Rotation);

    rotaxis = XMVectorSet(1.0f, 1.0f, 0.0f, 0.0f);
    Rotation = XMMatrixRotationAxis(rotaxis, rot);
    Translation = XMMatrixTranslation(0.0f, 0.0f, 5.0f);
    pModel3->setTransformMatrix(Rotation * Translation * Rotation);

}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MYDXENGINETEST));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MYDXENGINETEST);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
       CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }
   gWindow = hWnd;
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}