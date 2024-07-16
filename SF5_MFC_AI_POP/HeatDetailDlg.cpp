﻿// HeatDetailDlg.cpp : implementation file
//

#include "pch.h"
#include "SF5_MFC_AI_POP.h"
#include "afxdialogex.h"
#include "HeatDetailDlg.h"


// HeatDetailDlg dialog

IMPLEMENT_DYNAMIC(HeatDetailDlg, CDialogEx)

HeatDetailDlg::HeatDetailDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_HEAT_DETAIL_DAILOG, pParent), m_nextTime(0)
{
	m_dataSets.resize(3);
	for (auto& dataSet : m_dataSets)
	{
		dataSet.reserve(10);
	}
	m_timestamps.reserve(10);
}

HeatDetailDlg::~HeatDetailDlg()
{
}

void HeatDetailDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_CHART3, m_ChartViewer);
}


BEGIN_MESSAGE_MAP(HeatDetailDlg, CDialogEx)
	ON_MESSAGE(WM_UPDATE_CHART3, &HeatDetailDlg::OnUpdateChart)
	ON_BN_CLICKED(IDC_BUTTON1, &HeatDetailDlg::OnBnClickedButton1)
    ON_WM_LBUTTONDOWN()
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// HeatDetailDlg message handlers
LRESULT HeatDetailDlg::OnUpdateChart(WPARAM wParam, LPARAM lParam)
{
    vector<vector<double>>* newDataPoints = reinterpret_cast<vector<vector<double>>*>(wParam);
    AddDataPoints(*newDataPoints);
    UpdateChart();
    Invalidate();
    return 0;
}

void HeatDetailDlg::AddDataPoints(const vector<vector<double>>& dataPoints)
{
    for (size_t i = 0; i < dataPoints.size(); ++i)
    {
        m_dataSets[i].push_back(dataPoints[i][0]);
    }

    m_timestamps.push_back(m_nextTime);
    m_nextTime += 1;

    if (m_timestamps.size() > 10)
    {
        for (auto& dataSet : m_dataSets)
        {
            dataSet.erase(dataSet.begin());
        }
        m_timestamps.erase(m_timestamps.begin());
    }
}

void HeatDetailDlg::UpdateChart()
{
    XYChart* c = new XYChart(600, 400);

    c->setPlotArea(55, 50, 520, 300, 0xffffff, -1, -1, c->dashLineColor(0xaaaaaa, Chart::DotLine), -1);

    LineLayer* layer = c->addLineLayer();
    layer->setXData(DoubleArray(&m_timestamps[0], m_timestamps.size()));

    const char* colors[] = { "0xff0000", "0x00ff00", "0x0000ff" };
    const char* labels[] = { "Data1", "Data2", "Data3" };

    for (size_t i = 0; i < m_dataSets.size(); ++i)
    {
        layer->addDataSet(DoubleArray(&m_dataSets[i][0], m_dataSets[i].size()), strtol(colors[i], nullptr, 16), labels[i]);
    }

    m_ChartViewer.setChart(c);

    delete c;
}

void HeatDetailDlg::OnBnClickedButton1()
{
    // TODO: Add your control notification handler code here
    ShowWindow(SW_HIDE);

    // 메인 다이얼로그로 포커스 이동
    if (AfxGetMainWnd() != nullptr)
    {
        AfxGetMainWnd()->SetForegroundWindow();
    }
}

void HeatDetailDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    SendMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));

    CDialogEx::OnLButtonDown(nFlags, point);
}


BOOL HeatDetailDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // 다이얼로그를 화면 중앙에 위치시키기 위한 코드 추가
    CRect rectDlg;
    GetWindowRect(rectDlg);

    int nDlgWidth = rectDlg.Width();
    int nDlgHeight = rectDlg.Height();

    int nScreenWidth = GetSystemMetrics(SM_CXSCREEN);
    int nScreenHeight = GetSystemMetrics(SM_CYSCREEN);

    int nX = (nScreenWidth - nDlgWidth) / 2;
    int nY = (nScreenHeight - nDlgHeight) / 2;

    SetWindowPos(NULL, nX, nY, nDlgWidth, nDlgHeight, SWP_NOZORDER | SWP_NOSIZE);

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}


BOOL HeatDetailDlg::OnEraseBkgnd(CDC* pDC)
{
    CRect rect;
    GetClientRect(rect);

    pDC->FillSolidRect(rect, RGB(69, 76, 84));
    return TRUE;

    return CDialogEx::OnEraseBkgnd(pDC);
}
