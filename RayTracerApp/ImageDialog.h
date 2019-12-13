#pragma once


// ImageDialog dialog

class ImageDialog : public CDialogEx
{
	DECLARE_DYNAMIC(ImageDialog)

public:
	ImageDialog(CWnd* pParent = nullptr);   // standard constructor
	virtual ~ImageDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
