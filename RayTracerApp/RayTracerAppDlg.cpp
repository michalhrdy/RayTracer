
// RayTracerAppDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "RayTracerApp.h"
#include "RayTracerAppDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// GDI+
struct GdiplusInit {
	GdiplusInit() {
		GdiplusStartupInput inp;
		GdiplusStartupOutput outp;
		if (Ok != GdiplusStartup(&token_, &inp, &outp))
			throw runtime_error("GdiplusStartup");
	}
	~GdiplusInit() {
		GdiplusShutdown(token_);
	}
private:
	ULONG_PTR token_;
};


// CRayTracerAppDlg dialog
CRayTracerAppDlg::CRayTracerAppDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_RAYTRACERAPP_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRayTracerAppDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CRayTracerAppDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDCANCEL, &CRayTracerAppDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CRayTracerAppDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDSETCANVAS, &CRayTracerAppDlg::OnBnClickedSetcanvas)
	ON_BN_CLICKED(IDSETCAMERA, &CRayTracerAppDlg::OnBnClickedSetcamera)
	ON_BN_CLICKED(IDCLEAN, &CRayTracerAppDlg::OnBnClickedClean)
	ON_BN_CLICKED(IDADDLIGHT, &CRayTracerAppDlg::OnBnClickedAddlight)
	ON_BN_CLICKED(IDADDSPHERE, &CRayTracerAppDlg::OnBnClickedAddsphere)
	ON_BN_CLICKED(IDADDTRIANGLE, &CRayTracerAppDlg::OnBnClickedAddtriangle)
	ON_BN_CLICKED(IDADDPLANE, &CRayTracerAppDlg::OnBnClickedAddplane)
END_MESSAGE_MAP()


// CRayTracerAppDlg message handlers

BOOL CRayTracerAppDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	SetDlgItemInt(IDC_CANVAS_W, 800, false);
	SetDlgItemInt(IDC_CANVAS_H, 800, false);

	SetDlgItemInt(IDC_CAM_X, 0);
	SetDlgItemInt(IDC_CAM_Y, 0);
	SetDlgItemInt(IDC_CAM_Z, 0);
	SetDlgItemInt(IDC_CAMDIR_X, 0);
	SetDlgItemInt(IDC_CAMDIR_Y, 1);
	SetDlgItemInt(IDC_CAMDIR_Z, 0);

	SetDlgItemInt(IDC_LIGHT_X, 0);
	SetDlgItemInt(IDC_LIGHT_Y, -1);
	SetDlgItemInt(IDC_LIGHT_Z, 1);
	SetDlgItemInt(IDC_LIGHT_R, 100);
	SetDlgItemInt(IDC_LIGHT_B, 100);
	SetDlgItemInt(IDC_LIGHT_G, 100);

	SetDlgItemInt(IDC_SPHERE_X, 1);
	SetDlgItemInt(IDC_SPHERE_Y, 0);
	SetDlgItemInt(IDC_SPHERE_Z, 10);
	SetDlgItemInt(IDC_SPHERE_RADIUS, 1);
	SetDlgItemInt(IDC_SPHERE_R, 0);
	SetDlgItemInt(IDC_SPHERE_G, 0);
	SetDlgItemInt(IDC_SPHERE_B, 255);

	SetDlgItemInt(IDC_TRIANGLE_X1, -1);
	SetDlgItemInt(IDC_TRIANGLE_Y1, 3);
	SetDlgItemInt(IDC_TRIANGLE_Z1, 10);

	SetDlgItemInt(IDC_TRIANGLE_X3, -1);
	SetDlgItemInt(IDC_TRIANGLE_Y3, 0);
	SetDlgItemInt(IDC_TRIANGLE_Z3, 10);

	SetDlgItemInt(IDC_TRIANGLE_X2, 4);
	SetDlgItemInt(IDC_TRIANGLE_Y2, 3);
	SetDlgItemInt(IDC_TRIANGLE_Z2, 15);

	SetDlgItemInt(IDC_TRIANGLE_R, 0);
	SetDlgItemInt(IDC_TRIANGLE_G, 255);
	SetDlgItemInt(IDC_TRIANGLE_B, 0);

	SetDlgItemInt(IDC_PLANE_X, 0);
	SetDlgItemInt(IDC_PLANE_Y, 0);
	SetDlgItemInt(IDC_PLANE_Z, 30);
	SetDlgItemInt(IDC_PLANE_NX, 0);
	SetDlgItemInt(IDC_PLANE_NY, 1);
	SetDlgItemInt(IDC_PLANE_NZ, -1);
	SetDlgItemInt(IDC_PLANE_R, 255);
	SetDlgItemInt(IDC_PLANE_G, 0);
	SetDlgItemInt(IDC_PLANE_B, 0);

	//OnBnClickedOk();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CRayTracerAppDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

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
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CRayTracerAppDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CRayTracerAppDlg::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
}

UINT OnRender(LPVOID pParam)//CRayTracerAppDlg* window)
{
	CRayTracerAppDlg* window = (CRayTracerAppDlg*)pParam;

	//Scene rendering
	window->RayTracer.RenderScene();

	window->FillImgDialog();

	return 0;
}

void CRayTracerAppDlg::CreateImgDialog()
{
	LockAll();

	myImageWindow.DestroyWindow();
	myImageWindow.Create(IDD_DIALOG1, this);
	myImageWindow.SetWindowPos(this, 0, 0, RayTracer.GetCanvasWidth(), RayTracer.GetCanvasHeight(), NULL);
	myImageWindow.ShowWindow(SW_SHOW);

	CStatic* txtCtrl = (CStatic*)myImageWindow.GetDlgItem(IDC_RENDER);
	txtCtrl->ShowWindow(SW_SHOW);

	myImageWindow.SetTimer(1, 1000, NULL);
}

void CRayTracerAppDlg::FillImgDialog()
{
	myImageWindow.KillTimer(1);
	CStatic* txtCtrl = (CStatic*)myImageWindow.GetDlgItem(IDC_RENDER);
	txtCtrl->ShowWindow(SW_HIDE);

	//Initialization
	GdiplusInit gdiplusinit;

	Bitmap myBitmap(RayTracer.GetCanvasWidth(), RayTracer.GetCanvasHeight(), PixelFormat32bppARGB);

	for (int y = 0; y < RayTracer.GetCanvasHeight(); y++)
	{
		for (int x = 0; x < RayTracer.GetCanvasWidth(); x++)
		{
			Vec3 vecColor = RayTracer.GetPixelColor(x, y);
			Color col = Color::Color((BYTE)vecColor.x, (BYTE)vecColor.y, (BYTE)vecColor.z);

			myBitmap.SetPixel(x, y, col);
		}
	}

	//Drawing bitmap into window
	RECT rect;
	HBITMAP bmp;
	myBitmap.GetHBITMAP(RGB(0, 0, 0), &bmp);
	HDC hdc = ::GetDC(myImageWindow.GetSafeHwnd());
	HBRUSH brush = CreatePatternBrush(bmp);
	::GetWindowRect(myImageWindow.GetSafeHwnd(), &rect);
	FillRect(hdc, &rect, brush);
	DeleteObject(brush);
	DeleteDC(hdc);

	UnlockAll();
}

void CRayTracerAppDlg::LockAll()
{
	CButton* pButton = (CButton*)this->GetDlgItem(IDSETCAMERA);
	pButton->EnableWindow(FALSE);
	
	pButton = (CButton*)this->GetDlgItem(IDSETCANVAS);
	pButton->EnableWindow(FALSE);

	pButton = (CButton*)this->GetDlgItem(IDADDLIGHT);
	pButton->EnableWindow(FALSE);

	pButton = (CButton*)this->GetDlgItem(IDADDSPHERE);
	pButton->EnableWindow(FALSE);

	pButton = (CButton*)this->GetDlgItem(IDADDTRIANGLE);
	pButton->EnableWindow(FALSE);
	
	pButton = (CButton*)this->GetDlgItem(IDADDPLANE);
	pButton->EnableWindow(FALSE);

	pButton = (CButton*)this->GetDlgItem(IDOK);
	pButton->EnableWindow(FALSE);

	pButton = (CButton*)this->GetDlgItem(IDCLEAN);
	pButton->EnableWindow(FALSE);
}

void CRayTracerAppDlg::UnlockAll()
{
	CButton* pButton = (CButton*)this->GetDlgItem(IDSETCAMERA);
	pButton->EnableWindow(TRUE);

	pButton = (CButton*)this->GetDlgItem(IDSETCANVAS);
	pButton->EnableWindow(TRUE);

	pButton = (CButton*)this->GetDlgItem(IDADDLIGHT);
	pButton->EnableWindow(TRUE);

	pButton = (CButton*)this->GetDlgItem(IDADDSPHERE);
	pButton->EnableWindow(TRUE);

	pButton = (CButton*)this->GetDlgItem(IDADDTRIANGLE);
	pButton->EnableWindow(TRUE);

	pButton = (CButton*)this->GetDlgItem(IDADDPLANE);
	pButton->EnableWindow(TRUE);

	pButton = (CButton*)this->GetDlgItem(IDOK);
	pButton->EnableWindow(TRUE);

	pButton = (CButton*)this->GetDlgItem(IDCLEAN);
	pButton->EnableWindow(TRUE);
}

void CRayTracerAppDlg::OnBnClickedOk()
{	
	CreateImgDialog();
	
	CWinThread* T = AfxBeginThread(OnRender, this);

	//::WaitForSingleObject(T->m_hThread, INFINITE);

}


void CRayTracerAppDlg::OnBnClickedSetcanvas()
{
	RayTracer.SetCanvasSize(GetDlgItemInt(IDC_CANVAS_W), GetDlgItemInt(IDC_CANVAS_H));
}


void CRayTracerAppDlg::OnBnClickedSetcamera()
{
	Vec3 pos = Vec3(float((int)GetDlgItemInt(IDC_CAM_X)), float((int)GetDlgItemInt(IDC_CAM_Y)), float((int)GetDlgItemInt(IDC_CAM_Z)));
	Vec3 up = Vec3(float((int)GetDlgItemInt(IDC_CAMDIR_X)), float((int)GetDlgItemInt(IDC_CAMDIR_Y)), float((int)GetDlgItemInt(IDC_CAMDIR_Z)));

	RayTracer.SetCameraPosition(pos);
	RayTracer.SetCameraUp(up);
}


void CRayTracerAppDlg::OnBnClickedClean()
{
	RayTracer.ClearScene();
}


void CRayTracerAppDlg::OnBnClickedAddlight()
{
	Vec3 pos = Vec3(float((int)GetDlgItemInt(IDC_LIGHT_X)), float((int)GetDlgItemInt(IDC_LIGHT_Y)), float((int)GetDlgItemInt(IDC_LIGHT_Z)));
	Vec3 col = Vec3(float((int)GetDlgItemInt(IDC_LIGHT_R)), float((int)GetDlgItemInt(IDC_LIGHT_G)), float((int)GetDlgItemInt(IDC_LIGHT_B)));

	RayTracer.AddLight(pos, col);
}


void CRayTracerAppDlg::OnBnClickedAddsphere()
{
	Vec3 pos = Vec3(float((int)GetDlgItemInt(IDC_SPHERE_X)), float((int)GetDlgItemInt(IDC_SPHERE_Y)), float((int)GetDlgItemInt(IDC_SPHERE_Z)));
	Vec3 col = Vec3(float((int)GetDlgItemInt(IDC_SPHERE_R)), float((int)GetDlgItemInt(IDC_SPHERE_G)), float((int)GetDlgItemInt(IDC_SPHERE_B)));
	float rad = (float)((int)GetDlgItemInt(IDC_SPHERE_RADIUS));

	RayTracer.AddSphere(pos, rad, col);
}


void CRayTracerAppDlg::OnBnClickedAddtriangle()
{
	Vec3 p1 = Vec3(float((int)GetDlgItemInt(IDC_TRIANGLE_X1)), float((int)GetDlgItemInt(IDC_TRIANGLE_Y1)), float((int)GetDlgItemInt(IDC_TRIANGLE_Z1)));
	Vec3 p2 = Vec3(float((int)GetDlgItemInt(IDC_TRIANGLE_X2)), float((int)GetDlgItemInt(IDC_TRIANGLE_Y2)), float((int)GetDlgItemInt(IDC_TRIANGLE_Z2)));
	Vec3 p3 = Vec3(float((int)GetDlgItemInt(IDC_TRIANGLE_X3)), float((int)GetDlgItemInt(IDC_TRIANGLE_Y3)), float((int)GetDlgItemInt(IDC_TRIANGLE_Z3)));
	Vec3 col = Vec3(float((int)GetDlgItemInt(IDC_TRIANGLE_R)), float((int)GetDlgItemInt(IDC_TRIANGLE_G)), float((int)GetDlgItemInt(IDC_TRIANGLE_B)));

	RayTracer.AddTriangle(p1, p2, p3, col);
}


void CRayTracerAppDlg::OnBnClickedAddplane()
{
	Vec3 pos = Vec3(float((int)GetDlgItemInt(IDC_PLANE_X)), float((int)GetDlgItemInt(IDC_PLANE_Y)), float((int)GetDlgItemInt(IDC_PLANE_Z)));
	Vec3 nor = Vec3(float((int)GetDlgItemInt(IDC_PLANE_NX)), float((int)GetDlgItemInt(IDC_PLANE_NY)), float((int)GetDlgItemInt(IDC_PLANE_NZ)));
	Vec3 col = Vec3(float((int)GetDlgItemInt(IDC_PLANE_R)), float((int)GetDlgItemInt(IDC_PLANE_G)), float((int)GetDlgItemInt(IDC_PLANE_B)));

	RayTracer.AddPlane(pos, nor, col);
}
