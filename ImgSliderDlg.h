// ImgSliderDlg.h : header file
//

#if !defined(AFX_IMGSLIDERDLG_H__8DE24223_C115_495B_B956_37314BD094A6__INCLUDED_)
#define AFX_IMGSLIDERDLG_H__8DE24223_C115_495B_B956_37314BD094A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ImgSliderCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CImgSliderDlg dialog

class CImgSliderDlg : public CDialog
{
// Construction
public:
	CImgSliderDlg(CWnd* pParent = NULL);	// standard constructor
	CImgSliderCtrl m_imgsliderVert;
	CImgSliderCtrl m_imgsliderHorz;
	CImgSliderCtrl m_imgsliderGdiplusVert;//使用Gdiplus的
	CImgSliderCtrl m_imgsliderGdiplusHorz;//使用Gdiplus的
// Dialog Data
	//{{AFX_DATA(CImgSliderDlg)
	enum { IDD = IDD_IMGSLIDER_DIALOG };
	CSliderCtrl	m_slider;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImgSliderDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CImgSliderDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnReleasedcaptureSlider1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual void OnOK();
	//}}AFX_MSG
	afx_msg LRESULT OnReleasedCaputrueImgSlider(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnSlidedImgSlider(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMGSLIDERDLG_H__8DE24223_C115_495B_B956_37314BD094A6__INCLUDED_)
