
// TuiClientDlg.h : header file
//

#pragma once

#include <pthread.h>

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
	pthread_t m_hThread;
	static void *DetectCallback(void *arg);

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
};
