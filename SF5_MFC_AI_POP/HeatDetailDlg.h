#pragma once
#include "afxdialogex.h"

#include "ChartViewer.h"
#include <vector>

using namespace std;


#define WM_UPDATE_CHART3 (WM_USER + 13)


// HeatDetailDlg dialog

class HeatDetailDlg : public CDialogEx
{
	DECLARE_DYNAMIC(HeatDetailDlg)

public:
	HeatDetailDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~HeatDetailDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HEAT_DETAIL_DAILOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	afx_msg LRESULT OnUpdateChart(WPARAM wParam, LPARAM lParam);

private:
	vector<vector<double>> m_dataSets;
	vector<double> m_timestamps;
	CChartViewer m_ChartViewer;
	double m_nextTime;

	void UpdateChart();
	void AddDataPoints(const vector<vector<double>>& dataPoints);

public:
	afx_msg void OnBnClickedButton1();
};
