
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
	//static const UINT_PTR m_nTimerID = 0;
	//void UpdateCurrentTime();
	//afx_msg void OnTimer(UINT_PTR nIDEvent);


	CStringA winHttp(CStringA jsonData, wstring endpoint, int port);
	vector<int> parsing_robot(CStringA response);
	vector<int> parsing_plastic(CStringA response);

	static wstring StringToWideString(const string& str);
	static CStringA prepareData(tName process);
	static wstring SendPostRequest(tName process);

	//static UINT Thread_DB_Get_Cur(LPVOID _method);
	//static UINT Thread_DB_Get_Vib(LPVOID _method);

	//static CString strCur;
	//static CString strVib;
	static CCriticalSection critSect;
	static int offsetCur;
	static int offsetVib;


	CListCtrl m_listCtrl;

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButtonPlastic();

	CStatic m_test1, m_test2, m_result;


	static UINT MainThread(LPVOID _mothod);

	static UINT TimeUpdateThread(LPVOID _mothod);
	afx_msg LRESULT OnUpdateTime(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnNoticePlasticError(WPARAM wParam, LPARAM lParam);
	CEvent m_eventPlasticThread; // 이벤트 객체 선언
	static UINT PlasticThread(LPVOID pParam); // 대기 스레드 함수 선언
};