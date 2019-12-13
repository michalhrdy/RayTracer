
// RayTracerAppDlg.h : header file
//

#pragma once
#include "ImageDialog.h"
#include "RayTracerRender.h"

// CRayTracerAppDlg dialog
class CRayTracerAppDlg : public CDialogEx
{
// Construction
public:
	CRayTracerAppDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RAYTRACERAPP_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Graphics
	ImageDialog myImageWindow;
	CRayTracer RayTracer;

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedSetcanvas();
	afx_msg void OnBnClickedSetcamera();
	afx_msg void OnBnClickedClean();
	afx_msg void OnBnClickedAddlight();
	afx_msg void OnBnClickedAddsphere();
	afx_msg void OnBnClickedAddtriangle();
	afx_msg void OnBnClickedAddplane();
};
