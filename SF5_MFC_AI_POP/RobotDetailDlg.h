#pragma once
#include "afxdialogex.h"
#include "MyButton.h"
#include "ChartViewer.h"
#include <vector>

using namespace std;


#define WM_UPDATE_CHART1 (WM_USER + 11)


// RobotDetailDlg dialog

class RobotDetailDlg : public CDialogEx
{
	DECLARE_DYNAMIC(RobotDetailDlg)

public:
	RobotDetailDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~RobotDetailDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ROBOT_DETAIL_DAILOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
    afx_msg LRESULT OnUpdateChart(WPARAM wParam, LPARAM lParam);

private:
    vector<vector<double>> m_dataSets;
    vector<double> m_timestamps;
    CChartViewer m_ChartViewer1, m_ChartViewer2;
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
    CMyButton* m_robot_x;
    afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
};
