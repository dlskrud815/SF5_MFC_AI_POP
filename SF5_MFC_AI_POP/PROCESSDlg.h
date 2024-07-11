#pragma once
#include "afxdialogex.h"
#include "ChartViewer.h"

#include <vector>
#include <stdlib.h> // for rand()
#include <time.h>   // for time()

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

	DECLARE_MESSAGE_MAP()

public:
	CChartViewer m_chartView;
	vector<int> xData, yData;


	virtual BOOL OnInitDialog();
};
