// ImgSliderCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "ImgSlider.h"
#include "ImgSliderCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma comment(lib,"msimg32.lib")

#define  WS_EX_LAYERED  0x80000
#define  LWA_ALPHA       0x2
#define  LWA_COLORKEY   0x1

//�������Ҫʹ��GDI+�İ汾���Ƴ�GDI+�Ĵ��뼴��

//����Դ����ͼƬ��GDI+
static BOOL ImageFromIDResource(UINT nID, LPCTSTR szType, Image* &pImg)
{
	HINSTANCE hInst = AfxGetResourceHandle();
	HRSRC hRsrc = ::FindResource(hInst, MAKEINTRESOURCE(nID), szType);
	if (!hRsrc) return FALSE;
	// load resource into memory
	DWORD len = SizeofResource(hInst, hRsrc);
	BYTE* lpRsrc = (BYTE*)LoadResource(hInst, hRsrc);
	if (!lpRsrc)
		return FALSE;
	// Allocate global memory on which to create stream
	HGLOBAL m_hMem = GlobalAlloc(GMEM_FIXED, len);
	BYTE* pmem = (BYTE*)GlobalLock(m_hMem);
	memcpy(pmem,lpRsrc,len);
	IStream* pstm;
	CreateStreamOnHGlobal(m_hMem,FALSE,&pstm);
	// load from stream

	pImg=Gdiplus::Image::FromStream(pstm); 

	// free/release stuff
	GlobalUnlock(m_hMem);
	pstm->Release();
	FreeResource(lpRsrc);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CImgSliderCtrl
CImgSliderCtrl::CImgSliderCtrl()
{
	Init();
}

//ʹ��Ĭ��ֵ���г�ʼ��
void CImgSliderCtrl::Init()
{
	m_pParent=NULL;
	m_bHorizontal=TRUE;
	m_nMax=100;
	m_nMin=0;
	m_nPos=0;
	
	m_nMarginLeft=0;
	m_nMarginTop=0;
	m_nMarginRight=0;
	m_nMarginBottom=0;
	m_nThumbMarginLT=0;
	m_nThumbMarginRB=0;
	m_nThumbHW=0;
	
	m_bEnableBg=TRUE;
	m_clrBg=RGB(128,0,128);
	m_nThumbPos=0;
	m_bLButtonDown=FALSE;
	m_bEnableOutline=FALSE;
	m_bEnableToolTip=FALSE;
	m_clrOutline=RGB(255,255,255);
	
	m_pImgChannel=NULL;
	m_pImgActive=NULL;
	m_pImgThumb=NULL;

	m_nUseGdiplus = FALSE;
}
CImgSliderCtrl::~CImgSliderCtrl()
{
	if(!m_pImgChannel){
		delete m_pImgChannel;
		m_pImgChannel=NULL;
	}
	if(!m_pImgActive){
		delete m_pImgActive;
		m_pImgActive=NULL;
	}
	if(!m_pImgThumb){
		delete m_pImgThumb;
		m_pImgActive=NULL;
	}
}


BEGIN_MESSAGE_MAP(CImgSliderCtrl, CWnd)
	//{{AFX_MSG_MAP(CImgSliderCtrl)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//���������,���bVertΪtrue,Ϊ��ֱ�����������Ϊˮƽ�����
BOOL CImgSliderCtrl::Create(BOOL bHorizontal,CRect rect,CWnd *pParent,UINT nID)
{
	ASSERT(pParent!=NULL);
	m_pParent=pParent;
	m_bHorizontal=bHorizontal;
	//m_nThumbPos����OnSize�м���
	return CWnd::Create(NULL,"ImgSlider",WS_CHILD|WS_VISIBLE,rect,pParent,nID,NULL);//|WS_BORDER
}
//���������,���bVertΪtrue,Ϊ��ֱ�����������Ϊˮƽ�����
//����ָ�����ڷ��
BOOL CImgSliderCtrl::Create(BOOL bHorizontal,DWORD dwStyle,CRect rect,CWnd *pParent,UINT nID)
{
	ASSERT(pParent!=NULL);
	m_pParent=pParent;
	m_bHorizontal=bHorizontal;
	//m_nThumbPos����OnSize�м���
	return CWnd::Create(NULL,"ImgSlider",dwStyle,rect,pParent,nID,NULL);//|WS_BORDER
}
//���������,���bVertΪtrue,Ϊ��ֱ�����������Ϊˮƽ�����
//����ָ�����ڷ�����չ���
BOOL CImgSliderCtrl::CreateEx(BOOL bHorizontal,DWORD dwExStyle,DWORD dwStyle,CRect rect,CWnd *pParent,UINT nID)
{
	ASSERT(pParent!=NULL);
	m_pParent=pParent;
	m_bHorizontal=bHorizontal;
	//m_nThumbPos����OnSize�м���
	return CWnd::CreateEx(dwExStyle,NULL,"ImgSlider",dwStyle,rect,pParent,nID,NULL);
}

//���ù���ĸ߶ȡ�����ĳߴ�
void CImgSliderCtrl::SetSizes(int nMarginLeft,int nMarginTop,int nMarginRight,int nMarginBottom,
		int nThumbMarginLT,int nThumbMarginRB,int nThumbHW)
{
	if(nMarginLeft<0)		nMarginLeft=0;
	if(nMarginTop<0)		nMarginTop=0;
	if(nMarginRight<0)		nMarginRight=0;
	if(nMarginBottom<0)		nMarginBottom=0;
	if(nThumbMarginLT<0)	nThumbMarginLT=0;
	if(nThumbMarginRB<0)	nThumbMarginRB=0;
	if(m_nThumbHW<0)		m_nThumbHW=0;
	m_nMarginLeft=nMarginLeft;
	m_nMarginTop=nMarginTop;
	m_nMarginRight=nMarginRight;
	m_nMarginBottom=nMarginBottom;
	m_nThumbMarginLT=nThumbMarginLT;
	m_nThumbMarginRB=nThumbMarginRB;
	m_nThumbHW=nThumbHW;
	
	//�ı�nThumbPos��ֵ����������Ѿ������Ļ�
	if(::IsWindow(m_hWnd))//�����Ѵ�������ô���ܳ����ؼ��ķ�Χ
	{
		SetPos(m_nPos);
		Invalidate();
	}
}
//���������߻��ƹ��ܣ����������ؼ��е�λͼ
void CImgSliderCtrl::EnableOutline(BOOL bEnable)
{
	m_bEnableOutline=bEnable;
	if(m_hWnd!=NULL) Invalidate();
}
//������������ɫ
void CImgSliderCtrl::SetOutLineColor(COLORREF clr)
{
	m_clrOutline=clr;
	if(m_hWnd!=NULL) Invalidate();
}

//���ñ������ƹ���
void CImgSliderCtrl::EnableBg(BOOL bEnable)
{
	m_bEnableBg=bEnable;
	if(m_hWnd!=NULL) Invalidate();
}
//���ñ�����ɫ
void CImgSliderCtrl::SetBgColor(COLORREF clr)
{
	m_clrBg=clr;
	if(m_hWnd!=NULL) Invalidate();
}
//����Դ����λͼ
BOOL CImgSliderCtrl::LoadBitmapsFromResource(UINT nIDChannel,UINT nIDActive,UINT nIDThumb)
{
	m_bmpChannel.DeleteObject();
	BOOL b1=m_bmpChannel.LoadBitmap(nIDChannel);
	m_bmpActive.DeleteObject();
	BOOL b2=m_bmpActive.LoadBitmap(nIDActive);
	m_bmpThumb.DeleteObject();
	BOOL b3=m_bmpThumb.LoadBitmap(nIDThumb);
	if(b1&&b2&&b3){
		if(m_hWnd!=NULL) Invalidate();
		return TRUE;
	}
	else return FALSE;
}
//����Դ����ͼ��
//lpszType����Դ���ͣ�����Դ��ͼ����ʾ���ļ������͡�
BOOL CImgSliderCtrl::LoadImagesFromResource(UINT nIDChannel,UINT nIDActive,UINT nIDThumb,LPCTSTR lpszType)
{
	if(m_pImgChannel) delete m_pImgChannel;
	if(!ImageFromIDResource(nIDChannel,lpszType,m_pImgChannel))
		return FALSE;
	if(m_pImgActive) delete m_pImgActive;
	if(!ImageFromIDResource(nIDActive,lpszType,(Image*&)m_pImgActive))
		return FALSE;
	if(m_pImgThumb) delete m_pImgThumb;
	if(!ImageFromIDResource(nIDThumb,lpszType,(Image*&)m_pImgThumb))
		return FALSE;
	if(m_hWnd!=NULL)
		Invalidate();
	return TRUE;
}
//���ļ�����ͼ��
BOOL CImgSliderCtrl::LoadImagesFromFile(WCHAR *lpszChannel,WCHAR *lpszActive,WCHAR *lpszThumb)
{
	if(m_pImgChannel) delete m_pImgChannel;
	m_pImgChannel=Image::FromFile( lpszChannel); 
	if(!m_pImgChannel)
		return FALSE;
	
	if(m_pImgActive) delete m_pImgActive;
	m_pImgActive=Image::FromFile(lpszActive); 
	if(!m_pImgActive)
		return FALSE;
	
	if(m_pImgThumb) delete m_pImgThumb;
	m_pImgThumb=Image::FromFile(lpszThumb); 
	if(!m_pImgThumb)
		return FALSE;
	
	if(m_hWnd!=NULL)
		Invalidate();
	return TRUE;
}
//���õ�ǰλ��
void CImgSliderCtrl::SetPos(int n)
{
	if(n<m_nMin || n>m_nMax)
		return;
	m_nPos=n;
	if(m_hWnd==NULL)//������ڻ���û�д�����m_nThumbPos�ļ���ͷ���������OnSize
		return;
	CRect rectClient;
	GetClientRect(rectClient);
	double dRate=0;
	if(m_bHorizontal){
		dRate=((float)rectClient.Width()-m_nThumbHW-m_nMarginLeft-m_nMarginRight)/(float(m_nMax-m_nMin));
		m_nThumbPos=(int)(dRate*(n-m_nMin)+m_nMarginLeft);
	}
	else{
		dRate=((float)rectClient.Height()-m_nThumbHW-m_nMarginTop-m_nMarginBottom)/(float(m_nMax-m_nMin));
		m_nThumbPos=(int)(dRate*(n-m_nMin)+m_nMarginTop);
	}
	Invalidate();
}

//��ȡ��ǰλ��
int CImgSliderCtrl::GetPos()
{
	if(m_hWnd==NULL)
		return m_nPos;
	CRect rectClient;
	GetClientRect(rectClient);
	double dRate=0;
	if(m_bHorizontal){
		dRate=((float)rectClient.Width()-m_nThumbHW-m_nMarginLeft-m_nMarginRight)/(float(m_nMax-m_nMin));
		m_nPos=(int)((m_nThumbPos-m_nMarginLeft)/dRate);
	}
	else{
		dRate=((float)rectClient.Height()-m_nThumbHW-m_nMarginTop-m_nMarginBottom)/(float(m_nMax-m_nMin));
		m_nPos=(int)((m_nThumbPos-m_nMarginTop)/dRate);
	}
	return m_nMax-m_nPos;
}
void CImgSliderCtrl::EnableToolTip(BOOL bEnable)
{
	if(bEnable && !m_bEnableToolTip)
		m_tooltip.AddTool(this,"0");
	else if(!bEnable && m_bEnableToolTip)
		m_tooltip.DelTool(this);
	m_bEnableToolTip=bEnable;
}
/////////////////////////////////////////////////////////////////////////////
// CImgSliderCtrl message handlers

BOOL CImgSliderCtrl::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message==WM_MOUSEMOVE)
		m_tooltip.RelayEvent(pMsg);
	return CWnd::PreTranslateMessage(pMsg);
}

int CImgSliderCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	m_tooltip.Create(this,TTS_ALWAYSTIP);
	EnableToolTip(m_bEnableToolTip);
	//*
	//SetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE,
    //        GetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE)^WS_EX_LAYERED);
	HINSTANCE hInst = LoadLibrary("User32.DLL"); //��ʽ����DLL
	if(hInst!=NULL)
	{           
		typedef BOOL (WINAPI *MYFUNC)(HWND,COLORREF,BYTE,DWORD);         
		MYFUNC pFun = NULL;    
		pFun=(MYFUNC)GetProcAddress(hInst, "SetLayeredWindowAttributes");//ȡ��SetLayeredWindowAttributes����ָ��
		if(pFun!=NULL)
			pFun(this->GetSafeHwnd(),RGB(128,0,128),60,LWA_COLORKEY); //LWA_ALPHA LWA_COLORKEY   
		FreeLibrary(hInst);
	}
	//*/
	RedrawWindow();
	return 0;
}
void CImgSliderCtrl::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	if(m_nUseGdiplus)
	{
		if(m_pImgActive == NULL || m_pImgChannel == NULL || m_pImgThumb == NULL)
		{
			TRACE(_T("Btimap is not set\n"));
			return;
		}
	}
	else
	{
		if(m_bmpActive.m_hObject == NULL || m_bmpChannel.m_hObject == NULL || m_bmpThumb.m_hObject == NULL)
		{
			TRACE(_T("Btimap is not set\n"));
			return;
		}
	}

	CDC dcMem;
	dcMem.CreateCompatibleDC(NULL);
	CBitmap bmpMem,*pOldBmpMem;
	CRect rectClient;
	GetClientRect(rectClient);
	bmpMem.CreateCompatibleBitmap(&dc,rectClient.Width(),rectClient.Height());
	pOldBmpMem=dcMem.SelectObject(&bmpMem);
	
	if(m_nUseGdiplus)
	{
		//dcMem.FillSolidRect(rectClient,m_clrBg);
		dcMem.SelectStockObject(NULL_PEN);
	}
	else
	{
		if(m_bEnableBg)
			dcMem.FillSolidRect(rectClient,m_clrBg);//����
		CPen pen(PS_SOLID,1,m_clrOutline);
		CPen *pOldPen=dcMem.SelectObject(&pen);
		dcMem.SelectStockObject(NULL_BRUSH);
		if(m_bEnableOutline)
			dcMem.Rectangle(rectClient);//�ؼ�����
		
		dcMem.SelectObject(pOldPen);
	}
	
	if(m_bHorizontal)
	{
		if(m_nUseGdiplus)
			DrawHorizontal_GDIPLUS(&dcMem);
		else
			DrawHorizontal(&dcMem);
	}
	else
	{
		if(m_nUseGdiplus)
			DrawVert_GDIPLUS(&dcMem);
		else
			DrawVert(&dcMem);
	}
	
	dc.BitBlt(0,0,rectClient.Width(),rectClient.Height(),&dcMem,0,0,SRCCOPY);
	dcMem.SelectObject(&pOldBmpMem);
	
	// Do not call CWnd::OnPaint() for painting messages
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//GDI�汾

//����ˮƽ����Ŀؼ�
void CImgSliderCtrl::DrawHorizontal(CDC *pDC)
{
	CRect rectClient;
	GetClientRect(rectClient);
	
	//���ƹ��
	int i=0;
	int r,b;
	b=rectClient.Height()-m_nMarginBottom;
	r=rectClient.Width()-m_nMarginRight;
	if(m_bEnableOutline)
		pDC->Rectangle(m_nMarginLeft,m_nMarginTop,r,b);
	//��x�����������
	BITMAP bitmap;
	m_bmpChannel.GetBitmap(&bitmap);
	int nBmpW=bitmap.bmWidth;
	CDC dcBmp;
	dcBmp.CreateCompatibleDC(pDC);
	CBitmap *pOldBitmap=dcBmp.SelectObject(&m_bmpChannel);
	while((i+1)*nBmpW<=r-m_nMarginLeft)
	{
		pDC->BitBlt(m_nMarginLeft+i*nBmpW,m_nMarginTop,nBmpW,b-m_nMarginTop,
			&dcBmp,0,0,SRCCOPY);
		i++;
	}
	if(r-m_nMarginLeft>i*nBmpW)
		pDC->BitBlt(m_nMarginLeft+i*nBmpW,m_nMarginTop,r-m_nMarginLeft-i*nBmpW,b-m_nMarginTop,
			&dcBmp,0,0,SRCCOPY);
	
	//����Active����x��������䣬ˮƽ�����ϲ��ܳ���Active����
	i=0;
	m_bmpActive.GetBitmap(&bitmap);
	nBmpW=bitmap.bmWidth;
	dcBmp.SelectObject(&m_bmpActive);
	//x���������active
	while((i+1)*nBmpW<=m_nThumbPos-m_nMarginLeft)
	{
		pDC->BitBlt(m_nMarginLeft+i*nBmpW,m_nMarginTop,nBmpW,b-m_nMarginTop,
			&dcBmp,0,0,SRCCOPY);
		i++;
	}
	if(m_nThumbPos-m_nMarginLeft>i*nBmpW)
		pDC->BitBlt(m_nMarginLeft+i*nBmpW,m_nMarginTop,m_nThumbPos-m_nMarginLeft-i*nBmpW,b-m_nMarginTop,
			&dcBmp,0,0,SRCCOPY);
	
	//���ƻ���
	//���ݻ���Ĵ�С�������Ǹ���ͼ��Ĵ�С������˲���Ҫ��ͼ����вü�
	dcBmp.SelectObject(&m_bmpThumb);
	if(m_bEnableOutline)
		pDC->Rectangle(m_nThumbPos,m_nThumbMarginLT,m_nThumbPos+m_nThumbHW,rectClient.bottom-m_nThumbMarginRB);
	pDC->BitBlt(m_nThumbPos,m_nThumbMarginLT,m_nThumbHW,rectClient.bottom-m_nThumbMarginRB-m_nThumbMarginLT,
			&dcBmp,0,0,SRCCOPY);
	
	
	dcBmp.SelectObject(pOldBitmap);
}

void CImgSliderCtrl::DrawVert(CDC *pDC)
{
	CRect rectClient;
	GetClientRect(rectClient);
	
	//���ƹ��
	int i=0;
	int r,b;
	b=rectClient.Height()-m_nMarginBottom;
	r=rectClient.Width()-m_nMarginRight;
	if(m_bEnableOutline)
		pDC->Rectangle(m_nMarginLeft,m_nMarginTop,r,b);
	//��y�����������
	BITMAP bitmap;
	m_bmpChannel.GetBitmap(&bitmap);
	int nBmpH=bitmap.bmHeight;
	CDC dcBmp;
	dcBmp.CreateCompatibleDC(pDC);
	CBitmap *pOldBitmap=dcBmp.SelectObject(&m_bmpChannel);
	while((i+1)*nBmpH<=b-m_nMarginTop)//���ϵ���
	{
		pDC->BitBlt(m_nMarginLeft,m_nMarginTop+i*nBmpH,r-m_nMarginLeft,nBmpH,
			&dcBmp,0,0,SRCCOPY);
		i++;
	}
	if(b-m_nMarginTop>i*nBmpH)
		pDC->BitBlt(m_nMarginLeft,m_nMarginTop+i*nBmpH,r-m_nMarginLeft,b-m_nMarginTop-i*nBmpH,
			&dcBmp,0,0,SRCCOPY);
	
	//����Active����y���������
	i=0;
	m_bmpActive.GetBitmap(&bitmap);
	nBmpH=bitmap.bmHeight;
	dcBmp.SelectObject(&m_bmpActive);
	//return;
	//y���������active
	while((i+1)*nBmpH<=b-m_nThumbHW-m_nThumbPos)//���ϵ���
	{
		pDC->BitBlt(m_nMarginLeft,m_nThumbPos+m_nThumbHW+i*nBmpH,r-m_nMarginLeft,nBmpH,
			&dcBmp,0,0,SRCCOPY);
		i++;
	}
	if(b-m_nThumbHW-m_nThumbPos>i*nBmpH)
		pDC->BitBlt(m_nMarginLeft,m_nThumbPos+m_nThumbHW+i*nBmpH,r-m_nMarginLeft,b-m_nThumbHW-m_nThumbPos-i*nBmpH,
			&dcBmp,0,0,SRCCOPY);
	
	//���ƻ���
	//���ݻ���Ĵ�С�������Ǹ���ͼ��Ĵ�С������˲���Ҫ��ͼ����вü�
	dcBmp.SelectObject(&m_bmpThumb);
	if(m_bEnableOutline)
		pDC->Rectangle(m_nThumbMarginLT,m_nThumbPos,rectClient.right-m_nThumbMarginRB,m_nThumbPos+m_nThumbHW);
	pDC->BitBlt(m_nThumbMarginLT,m_nThumbPos,rectClient.right-m_nThumbMarginRB-m_nThumbMarginLT,m_nThumbHW,
			&dcBmp,0,0,SRCCOPY);
	
	
	dcBmp.SelectObject(pOldBitmap);
}
////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////
//GDI+�汾

//����ˮƽ����Ŀؼ�
void CImgSliderCtrl::DrawHorizontal_GDIPLUS(CDC *pDC)
{
	CRect rectClient;
	GetClientRect(rectClient);
	
	Pen pen(Color(GetRValue(m_clrOutline),GetGValue(m_clrOutline),GetBValue(m_clrOutline),1));
	Graphics g(pDC->m_hDC);
	if(m_bEnableOutline)
		g.DrawRectangle(&pen,rectClient.top,rectClient.left,rectClient.Width()-1,rectClient.Height()-1);//�໭��һ��px
	
	//���ƹ��
	int h,w;
	h=rectClient.Height()-m_nMarginTop-m_nMarginBottom;
	w=rectClient.Width()-m_nMarginLeft-m_nMarginRight;
	if(m_bEnableOutline)
		g.DrawRectangle(&pen,m_nMarginLeft,m_nMarginTop,w-1,h-1);
	//��x�����������
	int i=0;
	int nImgW=m_pImgChannel->GetWidth();
	int nImgH=m_pImgChannel->GetHeight();
	if(nImgH>h) nImgH=h;
	while((i+1)*nImgW<=w)
	{
		g.DrawImage(m_pImgChannel,nImgW*i+m_nMarginLeft,m_nMarginTop,0,0,nImgW,nImgH,UnitPixel);
		i++;
	}
	if(w>i*nImgW)
		g.DrawImage(m_pImgChannel,nImgW*i+m_nMarginLeft,m_nMarginTop,0,0,w-i*nImgW,nImgH,UnitPixel);
	
	//����Active����x��������䣬ˮƽ�����ϲ��ܳ���Active����
	i=0;
	nImgW=m_pImgActive->GetWidth();
	nImgH=m_pImgActive->GetHeight();
	if(nImgH>h) nImgH=h;
	//x���������active
	while((i+1)*nImgW<=m_nThumbPos-m_nMarginLeft)
	{
		g.DrawImage(m_pImgActive,nImgW*i+m_nMarginLeft,m_nMarginTop,0,0,nImgW,nImgH,UnitPixel);
		i++;
	}
	if(m_nThumbPos-m_nMarginLeft>i*nImgW)
	g.DrawImage(m_pImgActive,nImgW*i+m_nMarginLeft,m_nMarginTop,0,0,m_nThumbPos-m_nMarginLeft-i*nImgW,nImgH,UnitPixel);

	//���ƻ���
	//���ݻ���Ĵ�С�������Ǹ���ͼ��Ĵ�С������˲���Ҫ��ͼ����вü�
	h=rectClient.Height()-m_nThumbMarginLT-m_nThumbMarginRB;
	if(m_bEnableOutline)
		g.DrawRectangle(&pen,m_nThumbPos,m_nThumbMarginLT,m_nThumbHW-1,h-1);
	g.DrawImage(m_pImgThumb,m_nThumbPos,m_nThumbMarginLT,0,0,m_nThumbHW,h,UnitPixel);
}

void CImgSliderCtrl::DrawVert_GDIPLUS(CDC *pDC)
{
	CRect rectClient;
	GetClientRect(rectClient);
	
	Pen pen(Color(GetRValue(m_clrOutline),GetGValue(m_clrOutline),GetBValue(m_clrOutline),1));
	Graphics g(pDC->m_hDC);
	if(m_bEnableOutline)
		g.DrawRectangle(&pen,rectClient.top,rectClient.left,rectClient.Width()-1,rectClient.Height()-1);
	
	//���ƹ��
	int h,w;
	h=rectClient.Height()-m_nMarginTop-m_nMarginBottom;
	w=rectClient.Width()-m_nMarginLeft-m_nMarginRight;
	if(m_bEnableOutline)
		g.DrawRectangle(&pen,m_nMarginLeft,m_nMarginTop,w-1,h-1);	
	//��y����������������ϵ���
	int i=0;
	int nImgW=m_pImgChannel->GetWidth();
	int nImgH=m_pImgChannel->GetHeight();
	if(nImgW>w) nImgW=w;
	while((i+1)*nImgH<=h)
	{
		g.DrawImage(m_pImgChannel,m_nMarginLeft,nImgH*i+m_nMarginTop,0,0,nImgW,nImgH,UnitPixel);
		i++;
	}
	if(h>i*nImgH)
		g.DrawImage(m_pImgChannel,m_nMarginLeft,nImgH*i+m_nMarginTop,0,0,nImgW,h-i*nImgH,UnitPixel);
	
	//����Active����x��������䣬���ϵ���
	i=0;
	nImgW=m_pImgActive->GetWidth();
	nImgH=m_pImgActive->GetHeight();
	if(nImgW>w) nImgW=w;
	while((i+1)*nImgH<=h-m_nThumbHW+m_nMarginTop-m_nThumbPos)//Active�����ڻ�������
	{
		g.DrawImage(m_pImgActive,m_nMarginLeft,m_nThumbPos+m_nThumbHW+nImgH*i,0,0,nImgW,nImgH,UnitPixel);
		i++;
	}
	if(h-m_nThumbHW+m_nMarginTop-m_nThumbPos>i*nImgH)
		g.DrawImage(m_pImgActive,m_nMarginLeft,m_nThumbPos+m_nThumbHW+nImgH*i,0,0,
			nImgW,h+m_nMarginTop-m_nThumbPos-m_nThumbHW-i*nImgH,UnitPixel);
	
	//���ƻ���
	//���ݻ���Ĵ�С�������Ǹ���ͼ��Ĵ�С������˲���Ҫ��ͼ����вü�
	w=rectClient.Width()-m_nThumbMarginLT-m_nThumbMarginRB;
	if(m_bEnableOutline)
		g.DrawRectangle(&pen,m_nThumbMarginLT,m_nThumbPos,w-1,m_nThumbHW-1);
	g.DrawImage(m_pImgThumb,m_nThumbMarginLT,m_nThumbPos,0,0,w,m_nThumbHW,UnitPixel);
}
////////////////////////////////////////////////////////////////////////////////////////////////////

//*
void CImgSliderCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CRect rectClient;
	GetClientRect(rectClient);
	if(m_bHorizontal)//ˮƽ������
	{
		int top,bottom;//����ĸ�
		top=m_nThumbMarginLT;//����ĸ�
		bottom=rectClient.bottom-m_nThumbMarginRB;
		if( (m_nThumbPos<=point.x && point.x<=m_nThumbPos+m_nThumbHW) &&
			(top<=point.y && point.y<=bottom) )//�ж��Ƿ��ڻ����У��ڵĻ����ƶ�
		{
			m_bLButtonDown=TRUE;
			m_nPos=GetPos();
			SetCapture();
			//��ʾ����ȥ�˵Ļ���ͼ��
			Invalidate();
			return;
		}
		top=m_nMarginTop;//�����ĸ�
		bottom=rectClient.bottom-m_nMarginBottom;//�����ĸ�
		if( (m_nMarginLeft<=point.x && point.x<=rectClient.Width()-m_nMarginRight) &&
			(top<=point.y && point.y<=bottom) )//�ж��Ƿ��ڹ���У��ڵĻ��ƶ���������ڹ���е����ˣ�
		{
			//������Ϊ��point.x��
			if(point.x<=m_nThumbHW/2+m_nMarginLeft)//λ��(~,������/2+��հ�]�����������������,~Ϊ��������߸������
				m_nThumbPos=m_nMarginLeft;
			else if( point.x>m_nThumbHW/2+m_nMarginLeft && point.x< rectClient.Width()-(m_nThumbHW/2)-m_nMarginRight)
			//����λ�ã�λ��(������/2+��հף��������-������/2-�ҿհ�)
				m_nThumbPos=point.x-(m_nThumbHW/2);
			else
				m_nThumbPos=rectClient.Width()-m_nThumbHW-m_nMarginRight;//λ��[�������-������/2-�ҿհ�,~]���������������ұ�
			m_nPos=GetPos();
			Invalidate();
		}
	}
	else
	{
		int left,right;//����Ŀ�
		left=m_nThumbMarginLT;//����Ŀ�
		right=rectClient.right-m_nThumbMarginRB;
		if( (left<=point.x && point.x<=right) &&
			(m_nThumbPos<=point.y && point.y<=m_nThumbPos+m_nThumbHW) )//�ж��Ƿ��ڻ����У��ڵĻ����ƶ�
		{
			m_bLButtonDown=TRUE;
			m_nPos=GetPos();
			SetCapture();
			//��ʾ����ȥ�˵Ļ���ͼ��
			Invalidate();
			return;
		}
		left=m_nMarginLeft;//�����Ŀ�
		right=rectClient.right-m_nMarginRight;//�����Ŀ�
		if( (left<=point.x && point.x<=right) &&
			(m_nMarginTop<=point.y && point.y<=rectClient.Height()-m_nMarginRight) )//�ж��Ƿ��ڹ���У��ڵĻ��ƶ���������ڹ���е����ˣ�
		{
			//������Ϊ��point.y��
			if(point.y<=m_nThumbHW/2+m_nMarginTop)//λ��(~,����߶�/2+�Ͽհ�]������������������
				m_nThumbPos=m_nMarginTop;
			else if( point.y>m_nThumbHW/2+m_nMarginTop && point.y< rectClient.Height()-(m_nThumbHW/2)-m_nMarginBottom)
			//����λ�ã�λ��(����߶�/2+�Ͽհף�����߶�-����߶�/2-�¿հ�)
				m_nThumbPos=point.y-(m_nThumbHW/2);
			else
				m_nThumbPos=rectClient.Height()-m_nThumbHW-m_nMarginBottom;//λ��[����߶�-����߶�/2-�¿հ�,~)���������������±�
			m_nPos=GetPos();
			Invalidate();
		}
	}
	CWnd::OnLButtonDown(nFlags, point);
}

void CImgSliderCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if(m_bLButtonDown)
	{
		m_bLButtonDown=FALSE;
		::ReleaseCapture();
		m_pParent->PostMessage(UM_IMGSLIDER_RELEASED_CAPTURE);
	}
	CWnd::OnLButtonUp(nFlags, point);
}

void CImgSliderCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CRect rectClient;
	GetClientRect(rectClient);
	
	if(m_bHorizontal)
	{
		//��������SetCapture�����Բ���Ҫ��y�������ж�
		if(m_bLButtonDown)
		{
			//������Ϊ��point.x��
			if(point.x<=m_nThumbHW/2+m_nMarginLeft)//λ��(~,������/2+��հ�]�����������������,~Ϊ��������߸������
				m_nThumbPos=m_nMarginLeft;
			else if( point.x>m_nThumbHW/2+m_nMarginLeft && point.x< rectClient.Width()-(m_nThumbHW/2)-m_nMarginRight)
			//����λ�ã�λ��(������/2+��հף��������-������/2-�ҿհ�)
				m_nThumbPos=point.x-(m_nThumbHW/2);
			else
				m_nThumbPos=rectClient.Width()-m_nThumbHW-m_nMarginRight;//λ��[�������-������/2-�ҿհ�,~]���������������ұ�
			
			m_pParent->PostMessage(UM_IMGSLIDER_SLIDED,(WPARAM)m_hWnd);
			if(m_bEnableToolTip) UpdateToolTip();
			Invalidate();
		}
	}
	else
	{
		if(m_bLButtonDown)
		{
			//��������SetCapture�����Բ���Ҫ��x�������ж�
			//������Ϊ��point.y��
			if(point.y<=m_nThumbHW/2+m_nMarginTop)//λ��(~,����߶�/2+�Ͽհ�]������������������
				m_nThumbPos=m_nMarginTop;
			else if( point.y>m_nThumbHW/2+m_nMarginTop && point.y< rectClient.Height()-(m_nThumbHW/2)-m_nMarginBottom)
			//����λ�ã�λ��(����߶�/2+�Ͽհף�����߶�-����߶�/2-�¿հ�)
				m_nThumbPos=point.y-(m_nThumbHW/2);
			else
				m_nThumbPos=rectClient.Height()-m_nThumbHW-m_nMarginBottom;//λ��[����߶�-����߶�/2-�¿հ�,~)���������������±�
			
			m_pParent->PostMessage(UM_IMGSLIDER_SLIDED,(WPARAM)m_hWnd);
			if(m_bEnableToolTip) UpdateToolTip();
			Invalidate();
		}
	}
	CWnd::OnMouseMove(nFlags, point);
}
//����ToolTip�����֣�������ʾToolTip
void CImgSliderCtrl::UpdateToolTip()
{
	m_nPos=GetPos();
	CString str;
	str.Format("%d%%",m_nPos*100/(m_nMax-m_nMin));
	m_tooltip.UpdateTipText(str,this);
}

void CImgSliderCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	//����m_nPos�����µ�m_nThumbPos
	double dRate=0;
	if(m_bHorizontal){
		dRate=((float)cx-m_nThumbHW-m_nMarginLeft-m_nMarginRight)/(float(m_nMax-m_nMin));
		m_nThumbPos=(int)(dRate*(m_nPos-m_nMin))+m_nMarginLeft;
	}
	else{
		dRate=((float)cy-m_nThumbHW-m_nMarginTop-m_nMarginBottom)/(float(m_nMax-m_nMin));
		m_nThumbPos=cy-m_nMarginBottom-m_nThumbHW-(int)(dRate*(m_nPos-m_nMin));
	}
}
