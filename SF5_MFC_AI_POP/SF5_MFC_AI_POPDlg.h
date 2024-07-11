
// SF5_MFC_AI_POPDlg.h : header file
//

#pragma once

#include "MySQL_Connector.h"
#include "ThreadTest.h"

using namespace std;

// CSF5MFCAIPOPDlg dialog
class CSF5MFCAIPOPDlg : public CDialogEx
{
// Construction
public:
	CSF5MFCAIPOPDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SF5_MFC_AI_POP_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	//CImage back;
	COLORREF m_backgroundColor;

	// 타이머 ID
	static const UINT_PTR m_nTimerID = 0;
	void UpdateCurrentTime();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	static UINT ThreadTest(LPVOID _mothod);
	CListCtrl m_listCtrl;

	// sync 맞추기
	//static UINT Thread_DB_Wait(LPVOID _mothod);
	//static UINT Thread_DB_Get_Cur(LPVOID _mothod);
	//static UINT Thread_DB_Get_Vib(LPVOID _mothod);
	//void winHttp(MySQL_Connector* mysql1, MySQL_Connector* mysql2);

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();

	CStatic m_test1, m_test2, m_result;
};