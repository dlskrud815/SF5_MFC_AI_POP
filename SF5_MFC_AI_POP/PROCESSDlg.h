#pragma once
#include "afxdialogex.h"
#include "ChartViewer.h"

#include <vector>
#include <stdlib.h> // for rand()
#include <time.h>   // for time()

#include "MySQL_Connector.h"
#include <string>

using namespace std;

// PROCESSDlg dialog

class PROCESSDlg : public CDialogEx
{
	DECLARE_DYNAMIC(PROCESSDlg)

public:
	PROCESSDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~PROCESSDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROCESS_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	
	void updateChart();
	void OnTimer(UINT_PTR nIDEvent);

	void winHttp(MySQL_Connector* mysql1, MySQL_Connector* mysql2);
	CCriticalSection cs; // 동기화 객체

	DECLARE_MESSAGE_MAP()

public:
	CChartViewer m_chartView;
	vector<int> xData, yData;

	// sync 맞추기
	static UINT Thread_DB_Wait(LPVOID _mothod);
	static UINT Thread_DB_Get_Cur(LPVOID _mothod);
	static UINT Thread_DB_Get_Vib(LPVOID _mothod);

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton1();
};

struct ThreadData
{
	PROCESSDlg* pDlg;
	MySQL_Connector* mysql;
};

struct ThreadWaitData
{
	PROCESSDlg* pDlg;
	HANDLE hThreads[2];
	MySQL_Connector* mysqlCur;
	MySQL_Connector* mysqlVib;
};