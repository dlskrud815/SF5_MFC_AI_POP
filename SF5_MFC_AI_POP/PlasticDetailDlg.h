#pragma once
#include "afxdialogex.h"

#include "ChartViewer.h"
#include <vector>

using namespace std;


#define WM_UPDATE_CHART2 (WM_USER + 12)


// PlasticDetailDlg dialog

class PlasticDetailDlg : public CDialogEx
{
	DECLARE_DYNAMIC(PlasticDetailDlg)

public:
	PlasticDetailDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~PlasticDetailDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PLASTIC_DETAIL_DAILOG };
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
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
