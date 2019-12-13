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
END_MESSAGE_MAP()


// ImageDialog message handlers
