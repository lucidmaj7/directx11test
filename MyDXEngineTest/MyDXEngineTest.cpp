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
CMyDXModel* pModel4;
CMyDXModel* pModel5;
CMyDXModel* pModel6;
CMyDXModel* pStaue1;
float rot = 0;


HWND gWindow = NULL;
// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
void UpdateModel();
BOOL CenterWindow(HWND hwndWindow)
{
    HWND hwndParent;
    RECT rectWindow, rectParent;

    // make the window relative to its parent
    if ((hwndParent = GetParent(hwndWindow)) != NULL)
    {
        GetWindowRect(hwndWindow, &rectWindow);
        GetWindowRect(hwndParent, &rectParent);

        int nWidth = rectWindow.right - rectWindow.left;
        int nHeight = rectWindow.bottom - rectWindow.top;

        int nX = ((rectParent.right - rectParent.left) - nWidth) / 2 + rectParent.left;
        int nY = ((rectParent.bottom - rectParent.top) - nHeight) / 2 + rectParent.top;

        int nScreenWidth = GetSystemMetrics(SM_CXSCREEN);
        int nScreenHeight = GetSystemMetrics(SM_CYSCREEN);

        // make sure that the dialog box never moves outside of the screen
        if (nX < 0) nX = 0;
        if (nY < 0) nY = 0;
        if (nX + nWidth > nScreenWidth) nX = nScreenWidth - nWidth;
        if (nY + nHeight > nScreenHeight) nY = nScreenHeight - nHeight;

        MoveWindow(hwndWindow, nX, nY, nWidth, nHeight, FALSE);

        return TRUE;
    }

    return FALSE;
}
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
    CenterWindow(gWindow);
    MSG msg;

    if (!gDXEngine.Initialize(gWindow, 1920, 1080))
    {
        ::MessageBox(gWindow, L"Fail Engine", L"ERROR", MB_ICONERROR | MB_OK);
        return -1;
    }
    //////////////////////////

    pModel1 = new CMyDXModel();
    pModel2 = new CMyDXModel();
    pModel3 = new CMyDXModel();
    pModel4 = new CMyDXModel();
    pModel5 = new CMyDXModel();
    pModel6 = new CMyDXModel();
    pStaue1 = new CMyDXModel();
    if (pModel1->InitalizeModel(gDXEngine.m_MyDX3D.GetDXDevice(), gDXEngine.m_MyDX3D.GetDXDeviceContext(),L"my.obj" , L"1.jpg", L"shaders.shader", Material(10.f)))
    {
        gDXEngine.AddModel(pModel1);
    }
    else
    {
        ::MessageBox(gWindow, L"Fail to initalize Model 1", L"ERROR", MB_ICONERROR | MB_OK);
        return -1;
    }

    if (pModel2->InitalizeModel(gDXEngine.m_MyDX3D.GetDXDevice(), gDXEngine.m_MyDX3D.GetDXDeviceContext(), L"plane.obj", L"2.jpg", L"shaders.shader", Material(0.f)))
    {
        gDXEngine.AddModel(pModel2);
    }
   
    if (pModel3->InitalizeModel(gDXEngine.m_MyDX3D.GetDXDevice(), gDXEngine.m_MyDX3D.GetDXDeviceContext(),L"jessie.obj", L"Jess_Casual_Walking_001_D.jpg", L"shaders.shader", Material(0.f)))
    {
        gDXEngine.AddModel(pModel3);
    }
    if (pModel4->InitalizeModel(gDXEngine.m_MyDX3D.GetDXDevice(), gDXEngine.m_MyDX3D.GetDXDeviceContext(), L"chair.obj", L"13494_Folding_Chairs_diff.jpg", L"shaders.shader", Material(50.f)))
    {
        gDXEngine.AddModel(pModel4);
    }
    if (pModel5->InitalizeModel(gDXEngine.m_MyDX3D.GetDXDevice(), gDXEngine.m_MyDX3D.GetDXDeviceContext(), L"fish.obj", L"fish.png", L"shaders.shader", Material(50.f)))
    {
        gDXEngine.AddModel(pModel5);
    }
    if (pModel6->InitalizeModel(gDXEngine.m_MyDX3D.GetDXDevice(), gDXEngine.m_MyDX3D.GetDXDeviceContext(), L"skull.obj", L"skull.jpg", L"shaders.shader", Material(200.f)))
    {
        gDXEngine.AddModel(pModel6);
    }
 
    if (pStaue1->InitalizeModel(gDXEngine.m_MyDX3D.GetDXDevice(), gDXEngine.m_MyDX3D.GetDXDeviceContext(), L"statue.obj", L"statue.jpg", L"shaders.shader", Material(200.f)))
    {
        gDXEngine.AddModel(pStaue1);
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
            if (msg.message == WM_MOUSEMOVE)
            {
               OutputDebugString(L"mouse move\n");
               POINT pt;
               GetCursorPos(&pt);
               POINT centerPT;
               centerPT.x = 1280 / 2;
               centerPT.y = 720 / 2;

               if (centerPT.x - pt.x < 0)
               {
                   gDXEngine.m_Camera.MoveRightTarget();
               }
               if (centerPT.x - pt.x > 0)
               {
                   gDXEngine.m_Camera.MoveLeftTarget();
               }
               if (centerPT.y- pt.y > 0)
               {
                   gDXEngine.m_Camera.MoveUpTarget();
               }
               if (centerPT.y - pt.y < 0)
               {
                   gDXEngine.m_Camera.MoveDownTarget();
               }
             

               SetCursorPos(centerPT.x,centerPT.y);
            }
            if (msg.message == WM_CHAR) {
                switch (msg.wParam)
                {
                case 'w':
                    gDXEngine.m_Camera.MoveFrontPosition();
                    break;
                case 's':
                    gDXEngine.m_Camera.MoveBackPosition();
                    break;
                case 'a':
                    gDXEngine.m_Camera.MoveLeftPosition();
                    break;
                case 'd':
                    gDXEngine.m_Camera.MoveRightPosition();
                    break;
                }

            }
            if (msg.message == WM_KEYDOWN && msg.wParam == VK_ESCAPE)
            {
                break;
            }
            if (msg.message == WM_KEYDOWN)
            {
                switch (msg.wParam)
                {
                case VK_LEFT:
                    gDXEngine.m_Camera.MoveLeftTarget();
                    break;
                case VK_RIGHT:
                    gDXEngine.m_Camera.MoveRightTarget();
                    break;
                case VK_UP:
                    gDXEngine.m_Camera.MoveUpTarget();
                    break;
                case VK_DOWN:
                    gDXEngine.m_Camera.MoveDownTarget();
                    break;
                }
            }
            
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

    XMVECTOR rotaxis = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    XMMATRIX Rotation = XMMatrixRotationAxis(rotaxis, rot);
    XMMATRIX Translation;

    pModel1->setTransformMatrix(Rotation);


    rotaxis = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    Rotation = XMMatrixRotationAxis(rotaxis, rot);
   Translation = XMMatrixTranslation(-0.0f, 0.0f, 0.0f);
  // pModel2->setTransformMatrix(Rotation  );

    rotaxis = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    Rotation = XMMatrixRotationAxis(rotaxis, rot);
    Translation = XMMatrixTranslation(20.f, 0.0f, 0.0f);
    pModel3->setTransformMatrix(Rotation *Translation);
    
    rotaxis = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    Rotation = XMMatrixRotationAxis(rotaxis, -rot);
    Translation = XMMatrixTranslation(-30.0f, 30.0f, 0.0f);
    pModel5->setTransformMatrix(Translation*Rotation);

    rotaxis = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    Rotation = XMMatrixRotationAxis(rotaxis, -rot);
    Translation = XMMatrixTranslation(50.0f, 0.f, 0.0f);
    pModel6->setTransformMatrix(Rotation* Translation);

    rotaxis = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    Rotation = XMMatrixRotationAxis(rotaxis, -rot);
    Translation = XMMatrixTranslation(-50.0f, 0.f, -50.0f);
    pStaue1->setTransformMatrix(Rotation * Translation);
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
   int nScreenWidth = GetSystemMetrics(SM_CXSCREEN);
   int nScreenHeight = GetSystemMetrics(SM_CYSCREEN);
   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
       CW_USEDEFAULT, CW_USEDEFAULT, 1920, 1080, nullptr, nullptr, hInstance, nullptr);

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
