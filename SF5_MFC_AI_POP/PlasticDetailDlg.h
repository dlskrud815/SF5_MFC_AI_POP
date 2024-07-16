#pragma once
#include "afxdialogex.h"
#include "MyButton.h"
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
	CChartViewer m_ChartViewer, m_ChartViewer1;
	double m_nextTime;

	void UpdateChart();
	void AddDataPoints(const vector<vector<double>>& dataPoints);

	CFont m_font;
	HCURSOR m_hCursor;

public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CMyButton* m_plastic_x;
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
};
