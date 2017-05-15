// ImgSliderDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ImgSlider.h"
#include "ImgSliderDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define  WS_EX_LAYERED  0x80000
#define  LWA_ALPHA      0x2
#define  LWA_COLORKEY   0x1

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImgSliderDlg dialog

CImgSliderDlg::CImgSliderDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CImgSliderDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CImgSliderDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CImgSliderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CImgSliderDlg)
	DDX_Control(pDX, IDC_SLIDER1, m_slider);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CImgSliderDlg, CDialog)
	//{{AFX_MSG_MAP(CImgSliderDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER1, OnReleasedcaptureSlider1)
	ON_WM_SIZE()
	ON_MESSAGE(UM_IMGSLIDER_RELEASED_CAPTURE,OnReleasedCaputrueImgSlider)
	ON_MESSAGE(UM_IMGSLIDER_SLIDED,OnSlidedImgSlider)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImgSliderDlg message handlers

BOOL CImgSliderDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	/*
	SetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE,
          GetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE)^WS_EX_LAYERED);
	HINSTANCE hInst = LoadLibrary("User32.DLL"); //显式加载DLL
	if(hInst!=NULL)
	{           
		typedef BOOL (WINAPI *MYFUNC)(HWND,COLORREF,BYTE,DWORD);         
		MYFUNC pFun = NULL;    
		pFun=(MYFUNC)GetProcAddress(hInst, "SetLayeredWindowAttributes");//取得SetLayeredWindowAttributes函数指针
		if(pFun!=NULL)
			pFun(this->GetSafeHwnd(),RGB(128,0,128),60,LWA_COLORKEY); //LWA_ALPHA LWA_COLORKEY   
		FreeLibrary(hInst);
	}
	//*/
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

	//GDI版本示例
	;
	m_imgsliderHorz.SetPos(0);
	m_imgsliderHorz.SetSizes(10,10,10,10,5,5,30);
	m_imgsliderHorz.Create(TRUE,CRect(150,50,450,100),this,0);
	m_imgsliderHorz.EnableBg(TRUE);
	m_imgsliderHorz.LoadBitmapsFromResource(IDB_BITMAP_CHANNEL,IDB_BITMAP_ACTIVE,IDB_BITMAP_THUMB);
	m_imgsliderHorz.EnableOutline(TRUE);
	m_imgsliderHorz.EnableToolTip(true);
	//
	m_imgsliderVert.SetSizes(10,10,10,10,20,20,30);
	m_imgsliderVert.SetPos(10);
	m_imgsliderVert.Create(FALSE,CRect(10,10,60,310),this,0);
	m_imgsliderVert.EnableBg(TRUE);
	m_imgsliderVert.LoadBitmapsFromResource(IDB_BITMAP_CHANNEL,IDB_BITMAP_ACTIVE,IDB_BITMAP_THUMB);
	m_imgsliderVert.EnableOutline(TRUE);
	m_imgsliderVert.EnableToolTip(true);
	
	//GDI+版本示例
	m_imgsliderGdiplusHorz.SetSizes(10,10,10,10,4,4,10);
	m_imgsliderGdiplusHorz.SetPos(20);
	m_imgsliderGdiplusHorz.SetUseGdiplus();
	m_imgsliderGdiplusHorz.Create(TRUE,CRect(150,150,450,200),this,0);
	m_imgsliderGdiplusHorz.LoadImagesFromFile(L"res\\channel.png", L"res\\active.png", L"res\\Thum.png");
	m_imgsliderGdiplusHorz.EnableOutline(TRUE);
	m_imgsliderGdiplusHorz.EnableToolTip(true);
	//
	m_imgsliderGdiplusVert.SetSizes(10,10,10,10,10,10,30);
	m_imgsliderGdiplusVert.SetPos(30);
	m_imgsliderGdiplusVert.SetUseGdiplus();
	m_imgsliderGdiplusVert.Create(FALSE,CRect(70,10,120,310),this,0);
 	m_imgsliderGdiplusVert.LoadImagesFromFile(L"res\\channel.png", L"res\\active.png", L"res\\Thum.png");
	m_imgsliderGdiplusVert.EnableOutline(TRUE);
	m_imgsliderGdiplusVert.EnableToolTip(true);
	
	//WINDOWPLACEMENT wp;
	//GetWindowPlacement(&wp);
	//CWnd* pWnd = new CStatic;
   //pWnd->CreateEx(WS_EX_CLIENTEDGE, // Make a client edge label.
   //   _T("STATIC"), "Hi",
    //  WS_CHILD | WS_TABSTOP | WS_VISIBLE,
    //  5, 5, 30, 30, m_hWnd, (HMENU)1234);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CImgSliderDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CImgSliderDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

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
		CPaintDC dc(this);
		CRect rect;
		GetClientRect(rect);
		dc.FillSolidRect(rect,RGB(128,128,0));
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CImgSliderDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CImgSliderDlg::OnReleasedcaptureSlider1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}
LRESULT CImgSliderDlg::OnReleasedCaputrueImgSlider(WPARAM wParam,LPARAM lParam)
{
	
	return 0;
}
LRESULT CImgSliderDlg::OnSlidedImgSlider(WPARAM wParam,LPARAM lParam)
{
	int nPos=m_imgsliderHorz.GetPos();
	TRACE("%d\n",nPos);
	return 0;
}

void CImgSliderDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if(m_imgsliderHorz.m_hWnd!=NULL)
	{
		//CRect rect;
		//m_imgslider.GetWindowRect(rect);
		//ScreenToClient(&rect);
		//m_imgsliderHorz.MoveWindow(0,0,cx,17);
	}
}

void CImgSliderDlg::OnOK() 
{
	// TODO: Add extra validation here
	//m_imgslider.SetSizes(30,10,30,10,20,20,30);
	
	//CDialog::OnOK();
}
