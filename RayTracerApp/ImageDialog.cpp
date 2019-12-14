// ImageDialog.cpp : implementation file
//

#include "pch.h"
#include "RayTracerApp.h"
#include "ImageDialog.h"
#include "afxdialogex.h"


// ImageDialog dialog

IMPLEMENT_DYNAMIC(ImageDialog, CDialogEx)

ImageDialog::ImageDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{
}

ImageDialog::~ImageDialog()
{
}

void ImageDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ImageDialog, CDialogEx)
	ON_WM_TIMER()
	ON_WM_CREATE()
END_MESSAGE_MAP()


// ImageDialog message handlers



void ImageDialog::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	CStatic* control = (CStatic*)GetDlgItem(IDC_RENDER);
	control->SetWindowTextA(txtTable[rotation]);

	if (rotation == 2) rotation = 0;
	else ++rotation;

	UpdateData(FALSE);
	CDialogEx::OnTimer(nIDEvent);
}


int ImageDialog::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	txtTable[0] = "Rendering.";
	txtTable[1] = "Rendering..";
	txtTable[2] = "Rendering...";
	
	return 0;
}
