
// DX3dTestDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "DX3dTest.h"
#include "DX3dTestDlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CDX3dTestDlg dialog



CDX3dTestDlg::CDX3dTestDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DX3DTEST_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDX3dTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDX3dTestDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()


UINT Thread(LPVOID lpParam)
{
	//Sleep(1000);
	BOOL up = true;
	int i = 0;
	CDX3dTestDlg* pDlg = (CDX3dTestDlg*)lpParam;
	while (1)
	{
		
		if (up)
		{
		
			//pDlg->m_Direct3D.OurVertices[0].Color[i % 3] += 0.05f;
			pDlg->m_Direct3D.color[i % 3] += 0.05f;
			if (pDlg->m_Direct3D.color[i % 3] > 1.0f)
			{
				
				up = false;
			}
		}
		else 
		{
		
		//	pDlg->m_Direct3D.OurVertices[0].Color[i % 3] -= 0.05f;
			pDlg->m_Direct3D.color[i%3] -= 0.05f;
			if (pDlg->m_Direct3D.color[i % 3] < 0.0f)
			{
				i++;
				up = true;
			}
		}
		

		pDlg->m_Direct3D.RanderFrame();
		
		Sleep(1000 / 60);

	}

}
// CDX3dTestDlg message handlers

BOOL CDX3dTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	MoveWindow(0, 0, 800, 600, 1);
	CenterWindow();
	if (!m_Direct3D.initD3D(m_hWnd))
	{
		AfxMessageBox(_T("Fail to initialize Direct3D"));
		return FALSE;
	}

	//m_Direct3D.RanderFrame();
	AfxBeginThread(Thread, this);
	//AfxMessageBox(_T("success to initialize Direct3D"));


	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDX3dTestDlg::OnSysCommand(UINT nID, LPARAM lParam)

{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDX3dTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
	//m_Direct3D.RanderFrame();
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDX3dTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CDX3dTestDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	m_Direct3D.CleanD3D();
	// TODO: Add your message handler code here
}


void CDX3dTestDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	OutputDebugString(_T("OnKeyDown\n"));
	
	CDialogEx::OnKeyDown(nChar, nRepCnt, nFlags);

}


BOOL CDX3dTestDlg::PreTranslateMessage(MSG* pMsg)
{
	//OutputDebugString(_T("PreTranslateMessage\n"));
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN) { 
		switch (pMsg->wParam)
		{
		case VK_LEFT:
			m_Direct3D.CamMove(-0.01f, 0.f, 0.f);
			break;
		case VK_RIGHT:
			m_Direct3D.CamMove(+0.01f, 0.f, 0.f);
			break;
		case VK_UP:
			m_Direct3D.CamMove(0.f, +0.05f, 0.f);
			break;
		case VK_DOWN:
			m_Direct3D.CamMove(0.f, -0.05f, 0.f);
			break;
		}
	
	}
	

	return CDialogEx::PreTranslateMessage(pMsg);
}
