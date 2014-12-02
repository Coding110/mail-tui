
// TuiClientDlg.h : header file
//

#pragma once

#include <pthread.h>
#include "afxwin.h"

#define WM_SHOWTASK (WM_USER +1)

// CTuiClientDlg dialog
class CTuiClientDlg : public CDialogEx
{
// Construction
public:
	CTuiClientDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_TUICLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	CEdit mDlgLog;
	pthread_t m_hThread;
	static void *DetectCallback(void *arg);
	int SetAutoRun(bool bAutoRun, CString sValueName);
	int Init();
	void ToTray();
	void DeleteTray();
	void ShowLog(char* log, int time_flag = 0); // time_flag, 0 - no time, 1 - with time
	static void DlgLogProc(char *log, void *arg);

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	CRITICAL_SECTION m_cs;

	afx_msg void OnBnSetAutoRun();
	afx_msg void OnBnCancelAutoRun();
	afx_msg LRESULT OnShowTask(WPARAM wParam, LPARAM lParam);
	afx_msg void OnClose();

	
};
