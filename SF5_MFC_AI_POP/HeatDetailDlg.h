#pragma once
#include "afxdialogex.h"

#include "ChartViewer.h"
#include <vector>
#include <string>

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
	CChartViewer m_ChartViewer, m_ChartViewer1, m_ChartViewer2, m_ChartViewer3, m_ChartViewer4, m_ChartViewer5, m_ChartViewer6, m_ChartViewer7;
	double m_nextTime;

	void UpdateChart();
	void AddDataPoints(const vector<vector<double>>& dataPoints);

	CFont m_font;

public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
