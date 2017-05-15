// ImgSlider.h : main header file for the IMGSLIDER application
//

#if !defined(AFX_IMGSLIDER_H__46EA9B06_32E9_4D60_B297_A3AC9B8B0E27__INCLUDED_)
#define AFX_IMGSLIDER_H__46EA9B06_32E9_4D60_B297_A3AC9B8B0E27__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CImgSliderApp:
// See ImgSlider.cpp for the implementation of this class
//

class CImgSliderApp : public CWinApp
{
public:
	CImgSliderApp();
	ULONG_PTR m_gdiplusToken;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImgSliderApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CImgSliderApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMGSLIDER_H__46EA9B06_32E9_4D60_B297_A3AC9B8B0E27__INCLUDED_)
