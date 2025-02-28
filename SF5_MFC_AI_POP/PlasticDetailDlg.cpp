﻿// PlasticDetailDlg.cpp : implementation file
//

#include "pch.h"
#include "SF5_MFC_AI_POP.h"
#include "afxdialogex.h"
#include "PlasticDetailDlg.h"



// PlasticDetailDlg dialog

IMPLEMENT_DYNAMIC(PlasticDetailDlg, CDialogEx)

PlasticDetailDlg::PlasticDetailDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PLASTIC_DETAIL_DAILOG, pParent), m_nextTime(0)
{
	m_dataSets.resize(3);
	for (auto& dataSet : m_dataSets)
	{
		dataSet.reserve(10);
	}
	m_timestamps.reserve(10);
}

PlasticDetailDlg::~PlasticDetailDlg()
{
}

void PlasticDetailDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_STATIC_CHART2, m_ChartViewer);
    DDX_Control(pDX, IDC_STATIC_CHART5, m_ChartViewer1);
}


BEGIN_MESSAGE_MAP(PlasticDetailDlg, CDialogEx)
	ON_MESSAGE(WM_UPDATE_CHART2, &PlasticDetailDlg::OnUpdateChart)
	ON_BN_CLICKED(IDC_BUTTON_PLASTIC_X, &PlasticDetailDlg::OnBnClickedButton1)
    ON_WM_LBUTTONDOWN()
    ON_WM_ERASEBKGND()
    ON_WM_CTLCOLOR()
    ON_WM_SETCURSOR()
END_MESSAGE_MAP()


// PlasticDetailDlg message handlers


LRESULT PlasticDetailDlg::OnUpdateChart(WPARAM wParam, LPARAM lParam)
{
    vector<vector<double>>* newDataPoints = reinterpret_cast<vector<vector<double>>*>(wParam);
    AddDataPoints(*newDataPoints);
    UpdateChart();
    Invalidate();
    return 0;
}

void PlasticDetailDlg::AddDataPoints(const vector<vector<double>>& dataPoints)
{
    m_timestamps.push_back(m_nextTime);
    m_nextTime += 1;

    int i = 0, j = 0, k = 0;;
    double avgVib0 = 0, avgVib1 = 0, avgCur1 = 0;
    for (double vib0 : dataPoints[0])
    {
        avgVib0 += vib0;
        i++;
    }
    for (double vib1 : dataPoints[1])
    {
        avgVib1 += vib1;
        j++;
    }
    for (double cur1 : dataPoints[2])
    {
        avgCur1 += cur1;
        k++;
    }

    m_dataSets[0].push_back(avgCur1 / k);
    m_dataSets[1].push_back(avgVib0 / i);
    m_dataSets[2].push_back(avgVib1 / j);

    if (m_timestamps.size() > 10)
    {
        for (auto& dataSet : m_dataSets)
        {
            dataSet.erase(dataSet.begin());
        }
        m_timestamps.erase(m_timestamps.begin());
    }
}

void PlasticDetailDlg::UpdateChart()
{
    // 다이얼로그 크기에 맞게 차트 크기 설정
    int chartWidth = 320;
    int chartHeight = 200;

    // XYChart 객체 생성
    XYChart* c1 = new XYChart(chartWidth, chartHeight);
    XYChart* c2 = new XYChart(chartWidth, chartHeight);

    // 플롯 영역을 설정. 매개변수를 조정하여 여백을 최소화
    c1->setPlotArea(40, 20, chartWidth - 50, chartHeight - 50, 0xffffff, -1, -1, c1->dashLineColor(0xaaaaaa, Chart::DotLine), -1);
    c2->setPlotArea(40, 20, chartWidth - 50, chartHeight - 50, 0xffffff, -1, -1, c2->dashLineColor(0xaaaaaa, Chart::DotLine), -1);
    c1->setBackground(0X454C54);
    c2->setBackground(0X454C54);

    // 레이어 추가 및 X 데이터 설정
    LineLayer* layer1 = c1->addLineLayer();
    layer1->setXData(DoubleArray(&m_timestamps[0], m_timestamps.size()));

    LineLayer* layer2 = c2->addLineLayer();
    layer2->setXData(DoubleArray(&m_timestamps[0], m_timestamps.size()));

    LineLayer* layer3 = c2->addLineLayer();
    layer3->setXData(DoubleArray(&m_timestamps[0], m_timestamps.size()));


    // 데이터셋 색상 및 라벨 설정
    const char* colors[] = { "0xff0000", "0x0000ff", "0x00ff00"};
    const char* labels[] = { "Cur", "Vib" };

    layer1->addDataSet(DoubleArray(&m_dataSets[0][0], m_dataSets[0].size()), strtol(colors[0], nullptr, 16), labels[0]);
    layer2->addDataSet(DoubleArray(&m_dataSets[1][0], m_dataSets[1].size()), strtol(colors[1], nullptr, 16), labels[1]);
    layer3->addDataSet(DoubleArray(&m_dataSets[2][0], m_dataSets[2].size()), strtol(colors[2], nullptr, 16), labels[1]);

    c1->xAxis()->setLabelStyle("", 8, 0xffffff); // 축 레이블 폰트 크기 축소
    c1->yAxis()->setLabelStyle("", 8, 0xffffff);
    c1->addTitle("Plastic Working - Current", "", 10, 0xffffff); // 제목이 있는 경우 폰트 크기 축소

    c2->xAxis()->setLabelStyle("", 8, 0xffffff); // 축 레이블 폰트 크기 축소
    c2->yAxis()->setLabelStyle("", 8, 0xffffff);
    c2->addTitle("Plastic Working - Vibration", "", 10, 0xffffff); // 제목이 있는 경우 폰트 크기 축소

    // 차트를 ChartViewer에 설정
    m_ChartViewer.setChart(c1);
    m_ChartViewer1.setChart(c2);

    delete c1, c2;
}

void PlasticDetailDlg::OnBnClickedButton1()
{
    // TODO: Add your control notification handler code here
    ShowWindow(SW_HIDE);

    // 메인 다이얼로그로 포커스 이동
    if (AfxGetMainWnd() != nullptr)
    {
        AfxGetMainWnd()->SetForegroundWindow();
    }
}


void PlasticDetailDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    SendMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));

    CDialogEx::OnLButtonDown(nFlags, point);
}


BOOL PlasticDetailDlg::OnInitDialog()
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

    m_hCursor = ::LoadCursor(NULL, IDC_HAND);

    m_font.CreatePointFont(150, L"나눔스퀘어");
    GetDlgItem(IDC_STATIC_PLASTIC_DETAIL_TEXT)->SetFont(&m_font);

    m_plastic_x = new CMyButton();

    CRect rect_btn1;
    GetDlgItem(IDC_BUTTON_PLASTIC_X)->GetWindowRect(&rect_btn1);
    ScreenToClient(&rect_btn1);
    m_plastic_x->Create(_T("X"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_CENTER | BS_VCENTER,
        rect_btn1, this, IDC_BUTTON_PLASTIC_X);

    GetDlgItem(IDC_BUTTON_PLASTIC_X)->ShowWindow(SW_HIDE);
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}


BOOL PlasticDetailDlg::OnEraseBkgnd(CDC* pDC)
{
    // TODO: Add your message handler code here and/or call default
    CRect rect;
    GetClientRect(rect);

    pDC->FillSolidRect(rect, RGB(69, 76, 84));
    return TRUE;

    return CDialogEx::OnEraseBkgnd(pDC);
}


HBRUSH PlasticDetailDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

    // IDC_STATIC_TEXT1에 대해 배경을 투명하게 설정
    if (pWnd->GetDlgCtrlID() == IDC_STATIC_PLASTIC_DETAIL_TEXT)
    {
        pDC->SetBkMode(TRANSPARENT);
        pDC->SetTextColor(RGB(255, 255, 255));  // 흰색으로 글자 색 설정
        return (HBRUSH)GetStockObject(NULL_BRUSH);  // 배경 브러시를 NULL로 설정하여 투명하게 만듦
    }

    // TODO:  Return a different brush if the default is not desired
    return hbr;
}


BOOL PlasticDetailDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
    // TODO: Add your message handler code here and/or call default
    if (pWnd == m_plastic_x)
    {
        ::SetCursor(m_hCursor);
        return TRUE;
    }

    return CDialogEx::OnSetCursor(pWnd, nHitTest, message);
}
