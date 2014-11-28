
// TuiClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TuiClient.h"
#include "TuiClientDlg.h"
#include "afxdialogex.h"
#include "detect.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CTuiClientDlg dialog



CTuiClientDlg::CTuiClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTuiClientDlg::IDD, pParent)
{
	//m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON_TUI);
}

void CTuiClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTuiClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_SET_AUTO_RUN, &CTuiClientDlg::OnBnSetAutoRun)
	ON_BN_CLICKED(IDC_CANCEL_AUTO_RUN, &CTuiClientDlg::OnBnCancelAutoRun)
	ON_MESSAGE(WM_SHOWTASK, OnShowTask) //����WM_SHOWTASK����Ϣ��
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CTuiClientDlg message handlers

BOOL CTuiClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	Init();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void *CTuiClientDlg::DetectCallback(void *arg)
{
	easy_detect();
	int err = GetLastError();
	return (void*)0;
}

void CTuiClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else if (nID == SC_MINIMIZE)
	{
		ToTray();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTuiClientDlg::OnPaint()
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
HCURSOR CTuiClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CTuiClientDlg::OnBnSetAutoRun()
{
	// TODO: Add your control notification handler code here
	SetAutoRun(true, L"TuiClient");
}

int CTuiClientDlg::SetAutoRun(bool bAutoRun, CString sValueName)
{
	HKEY hKey;
	CString strRegPath = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run";
	if (RegOpenKeyEx(HKEY_CURRENT_USER, strRegPath, 0, KEY_ALL_ACCESS, &hKey) != ERROR_SUCCESS) return -1;

	if (bAutoRun)
	{
		if (RegOpenKeyEx(HKEY_CURRENT_USER, strRegPath, 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS) //��������       
		{
			TCHAR szModule[_MAX_PATH];
			
			GetModuleFileName(NULL, szModule, _MAX_PATH);//�õ������������ȫ·��
			//int len = wcslen(szModule) * 2;
			LONG ret = RegSetValueEx(hKey, sValueName, 0, REG_SZ, (const BYTE*)(LPCSTR)szModule, wcslen(szModule)*2); //���һ����Key,������ֵ��"Client"��Ӧ�ó������֣����Ӻ�׺.exe��  
			if (ret == ERROR_SUCCESS) AfxMessageBox(L"������ɣ�");
			else{
				CString msg;
				msg.Format(L"����ʧ��[%d]", ret);
				AfxMessageBox(msg);
			}
		}
		else
		{
			AfxMessageBox(L"ϵͳ��������,������ϵͳ����");
		}
	}
	else
	{
		if (RegOpenKeyEx(HKEY_CURRENT_USER, strRegPath, 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS)
		{
			LONG ret = RegDeleteValue(hKey, sValueName);
			if (ret == ERROR_SUCCESS) AfxMessageBox(L"ȡ��������ɣ�");
			else{
				CString msg;
				msg.Format(L"ȡ������ʧ��[%d]", ret);
				AfxMessageBox(msg);
			}
		}
	}

	RegCloseKey(hKey);
	return 0;
}

void CTuiClientDlg::OnBnCancelAutoRun()
{
	// TODO: Add your control notification handler code here
	SetAutoRun(false, L"TuiClient");
}

int CTuiClientDlg::Init()
{
	pthread_create(&m_hThread, NULL, &DetectCallback, this);

	// ��������ʾLogo
	CStatic* pWnd = (CStatic*)GetDlgItem(IDC_IMG_MAIN);   
	CImage image;
	image.Load(L"bkt-v1.0.png");
	HBITMAP hBmp = image.Detach();
	pWnd->SetBitmap(hBmp);
	pWnd->SetWindowPos(NULL,
		10,
		10,
		128,
		128,
		SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOZORDER);

	// ��С��������


	return 0;
}

LRESULT CTuiClientDlg::OnShowTask(WPARAM wParam, LPARAM lParam)
{
	if (wParam != IDI_ICON_TUI)
		return 1;
	switch (lParam)
	{
	case WM_RBUTTONUP://�Ҽ�����ʱ������ݲ˵�
	{
		LPPOINT lpoint = new tagPOINT;
		::GetCursorPos(lpoint);//�õ����λ�� 
		CMenu menu;
		menu.CreatePopupMenu();//����һ������ʽ�˵� 

		menu.AppendMenu(MF_STRING, WM_DESTROY, L"�˳�"); //���Ӳ˵���رա������������ϢWM_DESTROY�������ڣ������أ�������������� 
		menu.TrackPopupMenu(TPM_LEFTALIGN, lpoint->x, lpoint->y, this); //ȷ������ʽ�˵���λ�� 
		HMENU hmenu = menu.Detach();
		menu.DestroyMenu(); //��Դ���� 
		delete lpoint;
	} break;
	//case WM_LBUTTONDBLCLK: //˫������Ĵ��� 
	case WM_LBUTTONUP: //˫������Ĵ���
	{
		this->ShowWindow(SW_SHOW);//�򵥵���ʾ���������¶� 
		//DeleteTray();
	} break;
	default: break;
	}
	return 0;
}

void CTuiClientDlg::DeleteTray()
{
	NOTIFYICONDATA nid;
	nid.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
	nid.hWnd = this->m_hWnd;
	nid.uID = IDI_ICON_TUI;
	nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	nid.uCallbackMessage = WM_SHOWTASK; //�Զ������Ϣ���� 
	nid.hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_TUI));
	wcsncpy_s(nid.szTip, L"TuiClient", 128); //��Ϣ��ʾ��Ϊ���ƻ��������ѡ� 
	Shell_NotifyIcon(NIM_DELETE, &nid); //��������ɾ��ͼ�� 
}

void CTuiClientDlg::ToTray()
{
	NOTIFYICONDATA nid;
	nid.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
	nid.hWnd = this->m_hWnd;
	nid.uID = IDI_ICON_TUI;
	nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	nid.uCallbackMessage = WM_SHOWTASK;//�Զ������Ϣ���� 
	nid.hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_TUI));
	wcsncpy_s(nid.szTip, L"TuiClient", 128); //��Ϣ��ʾ�� 
	Shell_NotifyIcon(NIM_ADD, &nid); //�����������ͼ�� 
	ShowWindow(SW_HIDE); //���������� 
}

void CTuiClientDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	DeleteTray();
	CDialogEx::OnClose();
}
