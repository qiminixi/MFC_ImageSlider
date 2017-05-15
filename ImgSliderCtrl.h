#if !defined(AFX_IMGSLIDERCTRL_H__2C5B343F_B452_4A0E_B206_AD956FB62A11__INCLUDED_)
#define AFX_IMGSLIDERCTRL_H__2C5B343F_B452_4A0E_B206_AD956FB62A11__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ImgSliderCtrl.h : header file

#define UM_IMGSLIDER_RELEASED_CAPTURE	WM_USER+57
#define UM_IMGSLIDER_SLIDED				WM_USER+58

/*
    *v0.1.0.130818_release 涂启明，watch_the_rain@163.com
	*这是一个使用图片作为背景的slider控件，它没有从CSliderCtrl继承，而是直接从
CWnd派生。
    *使用png图片
	*使用GDI+进行绘图
	*通过接受自定义消息，UM_IMGSLIDER_RELEASED_CAPTURE和UM_IMGSLIDER_SLIDED，控件
所在容器可以获取滑动的位置，消息的第一个参数是slider创建的句柄，通过句柄判断是那个
slider的消息。
*/

/////////////////////////////////////////////////////////////////////////////
// CImgSliderCtrl window

class CImgSliderCtrl : public CWnd
{
public:
	BOOL Create(BOOL bHorizontal,CRect rect,CWnd *pParent,UINT nID);//创建滑竿条,如果bVert为true,为竖直轨道条，否则为水平轨道条
	BOOL Create(BOOL bHorizontal,DWORD dwStyle,CRect rect,CWnd *pParent,UINT nID);//创建滑竿条
	BOOL CreateEx(BOOL bHorizontal,DWORD dwExStyle,DWORD dwStyle,CRect rect,CWnd *pParent,UINT nID);//创建滑竿条
	void SetSizes(int nMarginLeft,int nMarginTop,int nMarginRight,int nMarginBottom,
		int nThumbMarginLT,int nThumbMarginRB,int nThumbHW);//通过设置与控件四边的空白设置轨道尺寸，设置滑块的尺寸
	
	BOOL LoadImagesFromResource(UINT nIDChannel,UINT nIDActive,UINT nIDThumb,LPCTSTR lpszType);//从资源载入图像
	BOOL LoadImagesFromFile(WCHAR *lpszChannel,WCHAR *lpszActive,WCHAR *lpszThumb);//从文件载入图像
	BOOL LoadBitmapsFromResource(UINT nIDChannel,UINT nIDActive,UINT nIDThumb);//从资源载入位图
	void EnableOutline(BOOL bEnable);//启用轮廓线绘制功能，帮助调整控件中的位图
	void SetOutLineColor(COLORREF clr);//设置轮廓线颜色
	void EnableBg(BOOL bEnable=TRUE);//启用背景绘制功能
	void SetBgColor(COLORREF clr);//设置背景颜色
	void SetPos(int n);//设置当前位置
	int GetPos();//获取当前位置
	void UpdateToolTip();//更新ToolTip的文字，重新显示ToolTip（位置也会自动移动至当前光标处）
	void EnableToolTip(BOOL bEnable=TRUE);//启用使用ToolTip
	void SetUseGdiplus(){m_nUseGdiplus = TRUE;};//是否使用GID+，在Create前设置好
	
private:
	void Init();//使用默认值进行初始化
	void DrawVert(CDC *pDC);//绘制水平方向的控件
	void DrawHorizontal(CDC *pDC);//绘制竖直方向的控件
	void DrawVert_GDIPLUS(CDC *pDC);//绘制水平方向的控件
	void DrawHorizontal_GDIPLUS(CDC *pDC);//绘制竖直方向的控件
	void LButtonDownVert(CPoint point);
	void LButtonDownHorizontal(CPoint point);
	
private:
	CWnd *m_pParent;
	BOOL m_bHorizontal;//是否为水平轨道条，默认TRUE
	int m_nMax;//滑动范围的最大值，比如滑动范围[0,100]，100为最大值，默认100
	int m_nMin;//滑动范围的最小值，比如滑动范围[0,100]，0为最小值，默认0
	int m_nPos;//当前位置，记住当前位置，应付窗口大小更改
	CRect rectWnd;//窗口矩形，因为要在create之前设置窗口大小，从而让SetPos
	int m_nMarginLeft;//轨道左边距控件左边的距离
	int m_nMarginTop;//轨道上边距控件上边的距离
	int m_nMarginRight;//轨道右边距控件右边的距离
	int m_nMarginBottom;////轨道下边距控件下边的距离
	
	int m_nThumbMarginLT;//如果是水平滑杆，为滑块上边相对控件上边的距离，如果是竖直滑杆，为滑块左边相对控件左边的距离（LeftTop）
	int m_nThumbMarginRB;//如果是水平滑杆，为滑块下边相对控件下边的距离，如果是竖直滑杆，为滑块右边相对控件右边的距离（RightBottom）
	//使滑块能够随轨道位置移动
	int m_nThumbHW;//如果是水平滑杆，为滑块宽度，如果是竖直滑竿，为滑块高度
	
	COLORREF m_clrBg;//背景颜色
	BOOL m_bEnableBg;//是否使用背景颜色绘制背景
	BOOL m_bEnableOutline;//是否在控件上绘制轮廓线
	COLORREF m_clrOutline;//轮廓线颜色
	
	CBitmap m_bmpChannel;
	CBitmap m_bmpActive;
	CBitmap m_bmpThumb;
	Image *m_pImgChannel;
	Image *m_pImgActive;
	Image *m_pImgThumb;

	int m_nThumbPos;//滑块的位置（绘图用），是距控件左边（上边）的距离，而不是滑块所处位置对应的值
	//比如滑块处在中间，轨道长度300，滚动范围0~100，那么m_nCurPos的值为150
	//滑块的有效位置是中心位置，中心对应的值才是滑块对应的对应的值
	//滑块的位置取值范围是：[0, 轨道长度 - 滑块长度]（[0, 轨道高度 - 滑块高度]）
	
	
	BOOL m_bLButtonDown;//鼠标是否按下
	BOOL m_bEnableToolTip;//是否使用ToolTip
	CToolTipCtrl m_tooltip;

	BOOL m_nUseGdiplus;
// Construction	
public:
	CImgSliderCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImgSliderCtrl)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CImgSliderCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CImgSliderCtrl)
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMGSLIDERCTRL_H__2C5B343F_B452_4A0E_B206_AD956FB62A11__INCLUDED_)
