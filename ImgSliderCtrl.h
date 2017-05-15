#if !defined(AFX_IMGSLIDERCTRL_H__2C5B343F_B452_4A0E_B206_AD956FB62A11__INCLUDED_)
#define AFX_IMGSLIDERCTRL_H__2C5B343F_B452_4A0E_B206_AD956FB62A11__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ImgSliderCtrl.h : header file

#define UM_IMGSLIDER_RELEASED_CAPTURE	WM_USER+57
#define UM_IMGSLIDER_SLIDED				WM_USER+58

/*
    *v0.1.0.130818_release Ϳ������watch_the_rain@163.com
	*����һ��ʹ��ͼƬ��Ϊ������slider�ؼ�����û�д�CSliderCtrl�̳У�����ֱ�Ӵ�
CWnd������
    *ʹ��pngͼƬ
	*ʹ��GDI+���л�ͼ
	*ͨ�������Զ�����Ϣ��UM_IMGSLIDER_RELEASED_CAPTURE��UM_IMGSLIDER_SLIDED���ؼ�
�����������Ի�ȡ������λ�ã���Ϣ�ĵ�һ��������slider�����ľ����ͨ������ж����Ǹ�
slider����Ϣ��
*/

/////////////////////////////////////////////////////////////////////////////
// CImgSliderCtrl window

class CImgSliderCtrl : public CWnd
{
public:
	BOOL Create(BOOL bHorizontal,CRect rect,CWnd *pParent,UINT nID);//����������,���bVertΪtrue,Ϊ��ֱ�����������Ϊˮƽ�����
	BOOL Create(BOOL bHorizontal,DWORD dwStyle,CRect rect,CWnd *pParent,UINT nID);//����������
	BOOL CreateEx(BOOL bHorizontal,DWORD dwExStyle,DWORD dwStyle,CRect rect,CWnd *pParent,UINT nID);//����������
	void SetSizes(int nMarginLeft,int nMarginTop,int nMarginRight,int nMarginBottom,
		int nThumbMarginLT,int nThumbMarginRB,int nThumbHW);//ͨ��������ؼ��ıߵĿհ����ù���ߴ磬���û���ĳߴ�
	
	BOOL LoadImagesFromResource(UINT nIDChannel,UINT nIDActive,UINT nIDThumb,LPCTSTR lpszType);//����Դ����ͼ��
	BOOL LoadImagesFromFile(WCHAR *lpszChannel,WCHAR *lpszActive,WCHAR *lpszThumb);//���ļ�����ͼ��
	BOOL LoadBitmapsFromResource(UINT nIDChannel,UINT nIDActive,UINT nIDThumb);//����Դ����λͼ
	void EnableOutline(BOOL bEnable);//���������߻��ƹ��ܣ����������ؼ��е�λͼ
	void SetOutLineColor(COLORREF clr);//������������ɫ
	void EnableBg(BOOL bEnable=TRUE);//���ñ������ƹ���
	void SetBgColor(COLORREF clr);//���ñ�����ɫ
	void SetPos(int n);//���õ�ǰλ��
	int GetPos();//��ȡ��ǰλ��
	void UpdateToolTip();//����ToolTip�����֣�������ʾToolTip��λ��Ҳ���Զ��ƶ�����ǰ��괦��
	void EnableToolTip(BOOL bEnable=TRUE);//����ʹ��ToolTip
	void SetUseGdiplus(){m_nUseGdiplus = TRUE;};//�Ƿ�ʹ��GID+����Createǰ���ú�
	
private:
	void Init();//ʹ��Ĭ��ֵ���г�ʼ��
	void DrawVert(CDC *pDC);//����ˮƽ����Ŀؼ�
	void DrawHorizontal(CDC *pDC);//������ֱ����Ŀؼ�
	void DrawVert_GDIPLUS(CDC *pDC);//����ˮƽ����Ŀؼ�
	void DrawHorizontal_GDIPLUS(CDC *pDC);//������ֱ����Ŀؼ�
	void LButtonDownVert(CPoint point);
	void LButtonDownHorizontal(CPoint point);
	
private:
	CWnd *m_pParent;
	BOOL m_bHorizontal;//�Ƿ�Ϊˮƽ�������Ĭ��TRUE
	int m_nMax;//������Χ�����ֵ�����绬����Χ[0,100]��100Ϊ���ֵ��Ĭ��100
	int m_nMin;//������Χ����Сֵ�����绬����Χ[0,100]��0Ϊ��Сֵ��Ĭ��0
	int m_nPos;//��ǰλ�ã���ס��ǰλ�ã�Ӧ�����ڴ�С����
	CRect rectWnd;//���ھ��Σ���ΪҪ��create֮ǰ���ô��ڴ�С���Ӷ���SetPos
	int m_nMarginLeft;//�����߾�ؼ���ߵľ���
	int m_nMarginTop;//����ϱ߾�ؼ��ϱߵľ���
	int m_nMarginRight;//����ұ߾�ؼ��ұߵľ���
	int m_nMarginBottom;////����±߾�ؼ��±ߵľ���
	
	int m_nThumbMarginLT;//�����ˮƽ���ˣ�Ϊ�����ϱ���Կؼ��ϱߵľ��룬�������ֱ���ˣ�Ϊ���������Կؼ���ߵľ��루LeftTop��
	int m_nThumbMarginRB;//�����ˮƽ���ˣ�Ϊ�����±���Կؼ��±ߵľ��룬�������ֱ���ˣ�Ϊ�����ұ���Կؼ��ұߵľ��루RightBottom��
	//ʹ�����ܹ�����λ���ƶ�
	int m_nThumbHW;//�����ˮƽ���ˣ�Ϊ�����ȣ��������ֱ���ͣ�Ϊ����߶�
	
	COLORREF m_clrBg;//������ɫ
	BOOL m_bEnableBg;//�Ƿ�ʹ�ñ�����ɫ���Ʊ���
	BOOL m_bEnableOutline;//�Ƿ��ڿؼ��ϻ���������
	COLORREF m_clrOutline;//��������ɫ
	
	CBitmap m_bmpChannel;
	CBitmap m_bmpActive;
	CBitmap m_bmpThumb;
	Image *m_pImgChannel;
	Image *m_pImgActive;
	Image *m_pImgThumb;

	int m_nThumbPos;//�����λ�ã���ͼ�ã����Ǿ�ؼ���ߣ��ϱߣ��ľ��룬�����ǻ�������λ�ö�Ӧ��ֵ
	//���绬�鴦���м䣬�������300��������Χ0~100����ôm_nCurPos��ֵΪ150
	//�������Чλ��������λ�ã����Ķ�Ӧ��ֵ���ǻ����Ӧ�Ķ�Ӧ��ֵ
	//�����λ��ȡֵ��Χ�ǣ�[0, ������� - ���鳤��]��[0, ����߶� - ����߶�]��
	
	
	BOOL m_bLButtonDown;//����Ƿ���
	BOOL m_bEnableToolTip;//�Ƿ�ʹ��ToolTip
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
