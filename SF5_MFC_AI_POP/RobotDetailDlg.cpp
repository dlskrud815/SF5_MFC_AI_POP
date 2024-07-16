// RobotDetailDlg.cpp : implementation file
//

#include "pch.h"
#include "SF5_MFC_AI_POP.h"
#include "afxdialogex.h"
#include "RobotDetailDlg.h"



// RobotDetailDlg dialog

IMPLEMENT_DYNAMIC(RobotDetailDlg, CDialogEx)

RobotDetailDlg::RobotDetailDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ROBOT_DETAIL_DAILOG, pParent), m_nextTime(0)
{
    m_dataSets.resize(3);
    for (auto& dataSet : m_dataSets)
    {
        dataSet.reserve(10);
    }
    m_timestamps.reserve(10);
}

RobotDetailDlg::~RobotDetailDlg()
{
}

void RobotDetailDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_STATIC_CHART1, m_ChartViewer1);
    DDX_Control(pDX, IDC_STATIC_CHART4, m_ChartViewer2);
}


BEGIN_MESSAGE_MAP(RobotDetailDlg, CDialogEx)
	ON_MESSAGE(WM_UPDATE_CHART1, &RobotDetailDlg::OnUpdateChart)
    ON_BN_CLICKED(IDC_BUTTON1, &RobotDetailDlg::OnBnClickedButton1)
    ON_WM_LBUTTONDOWN()
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


LRESULT RobotDetailDlg::OnUpdateChart(WPARAM wParam, LPARAM lParam)
{
    vector<vector<double>>* newDataPoints = reinterpret_cast<vector<vector<double>>*>(wParam);
    AddDataPoints(*newDataPoints);
    UpdateChart();
    Invalidate();
    return 0;
}

void RobotDetailDlg::AddDataPoints(const vector<vector<double>>& dataPoints)
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

void RobotDetailDlg::UpdateChart()
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


    // 데이터셋 색상 및 라벨 설정
    const char* colors[] = { "0xff0000", "0x0000ff" };
    const char* labels[] = { "Data1", "Data2" };

    //for (size_t i = 0; i < m_dataSets.size(); ++i)
    //{
    //    layer->addDataSet(DoubleArray(&m_dataSets[i][0], m_dataSets[i].size()), strtol(colors[i], nullptr, 16), labels[i]);
    //}

    layer1->addDataSet(DoubleArray(&m_dataSets[0][0], m_dataSets[0].size()), strtol(colors[0], nullptr, 16), labels[0]);
    layer2->addDataSet(DoubleArray(&m_dataSets[1][0], m_dataSets[1].size()), strtol(colors[1], nullptr, 16), labels[1]);

    
    c1->xAxis()->setLabelStyle("", 8, 0xffffff); // 축 레이블 폰트 크기 축소
    c1->yAxis()->setLabelStyle("", 8, 0xffffff);
    c1->addTitle("Current Chart", "", 10, 0xffffff); // 제목이 있는 경우 폰트 크기 축소

    c2->xAxis()->setLabelStyle("", 8, 0xffffff); // 축 레이블 폰트 크기 축소
    c2->yAxis()->setLabelStyle("", 8, 0xffffff);
    c2->addTitle("Vibration Chart", "", 10, 0xffffff); // 제목이 있는 경우 폰트 크기 축소

    // 차트를 ChartViewer에 설정
    m_ChartViewer1.setChart(c1);
    m_ChartViewer2.setChart(c2);

    delete c1, c2;
}

void RobotDetailDlg::OnBnClickedButton1()
{
    // TODO: Add your control notification handler code here
    ShowWindow(SW_HIDE);

    // 메인 다이얼로그로 포커스 이동
    if (AfxGetMainWnd() != nullptr)
    {
        AfxGetMainWnd()->SetForegroundWindow();
    }
}


void RobotDetailDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    SendMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));

    CDialogEx::OnLButtonDown(nFlags, point);
}


BOOL RobotDetailDlg::OnInitDialog()
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


BOOL RobotDetailDlg::OnEraseBkgnd(CDC* pDC)
{
    // TODO: Add your message handler code here and/or call default
    CRect rect;
    GetClientRect(rect);

    pDC->FillSolidRect(rect, RGB(69, 76, 84));
    return TRUE;

    return CDialogEx::OnEraseBkgnd(pDC);
}
