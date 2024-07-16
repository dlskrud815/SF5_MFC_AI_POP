// HeatDetailDlg.cpp : implementation file
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
	m_dataSets.resize(18);
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
    DDX_Control(pDX, IDC_STATIC_CHART11, m_ChartViewer1);
    DDX_Control(pDX, IDC_STATIC_CHART12, m_ChartViewer2);
    DDX_Control(pDX, IDC_STATIC_CHART13, m_ChartViewer3);
    DDX_Control(pDX, IDC_STATIC_CHART14, m_ChartViewer4);
    DDX_Control(pDX, IDC_STATIC_CHART15, m_ChartViewer5);
    DDX_Control(pDX, IDC_STATIC_CHART16, m_ChartViewer6);
    DDX_Control(pDX, IDC_STATIC_CHART17, m_ChartViewer7);
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

    //// 메시지 박스를 통해 newDataPoints 벡터의 값을 출력
    //CString message;
    //for (const auto& data : *newDataPoints)
    //{
    //    for (double value : data)
    //    {
    //        message.AppendFormat(_T("%.2f\t"), value);
    //    }
    //    message.Append(_T("\n"));
    //}

    //AfxMessageBox(message);

    AddDataPoints(*newDataPoints);
    UpdateChart();
    Invalidate();
    return 0;
}

void HeatDetailDlg::AddDataPoints(const vector<vector<double>>& dataPoints)
{
    m_timestamps.push_back(m_nextTime);
    m_nextTime += 1;
    
    //Drying
    m_dataSets[0].push_back(dataPoints[0][0]); //OP
    
    m_dataSets[1].push_back(dataPoints[0][1]); //Temp
    m_dataSets[2].push_back(dataPoints[0][2]);

    //Quenching
    m_dataSets[3].push_back(dataPoints[0][4]); //OP
    m_dataSets[4].push_back(dataPoints[0][5]);
    m_dataSets[5].push_back(dataPoints[0][6]);
    
    m_dataSets[6].push_back(dataPoints[0][7]); //CP

    m_dataSets[7].push_back(dataPoints[0][8]); //Temp
    m_dataSets[8].push_back(dataPoints[0][9]);
    m_dataSets[9].push_back(dataPoints[0][10]);
    m_dataSets[10].push_back(dataPoints[0][11]);

    //Salt
    m_dataSets[11].push_back(dataPoints[0][12]); //OP
    m_dataSets[12].push_back(dataPoints[0][13]);
    
    m_dataSets[13].push_back(dataPoints[0][15]); //Temp
    m_dataSets[14].push_back(dataPoints[0][16]);
    m_dataSets[15].push_back(dataPoints[0][17]);
    m_dataSets[16].push_back(dataPoints[0][18]);
    
    m_dataSets[17].push_back(dataPoints[0][14]); //Sludge


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
    // 다이얼로그 크기에 맞게 차트 크기 설정
    int chartWidth = 320;
    int chartHeight = 200;

    // XYChart 객체 생성
    XYChart* c1 = new XYChart(chartWidth, chartHeight);
    XYChart* c2 = new XYChart(chartWidth, chartHeight);
    XYChart* c3 = new XYChart(chartWidth, chartHeight);
    XYChart* c4 = new XYChart(chartWidth, chartHeight);
    XYChart* c5 = new XYChart(chartWidth, chartHeight);
    XYChart* c6 = new XYChart(chartWidth, chartHeight);
    XYChart* c7 = new XYChart(chartWidth, chartHeight);
    XYChart* c8 = new XYChart(chartWidth, chartHeight);

    vector<XYChart*> charts = { c1, c2, c3, c4, c5, c6, c7, c8 };
    //vector<XYChart*> charts = { c1, c2, c3, c4, c5, c6, c7, c8 };

    for (auto& chart : charts)
    {
        // 플롯 영역을 설정. 매개변수를 조정하여 여백을 최소화
        chart->setPlotArea(40, 20, chartWidth - 50, chartHeight - 50, 0xffffff, -1, -1, chart->dashLineColor(0xaaaaaa, Chart::DotLine), -1);
        chart->setBackground(0X454C54);
    }

    // 레이어 추가 및 X 데이터 설정
    LineLayer* layer1 = charts[0]->addLineLayer(); //OP - c1

    LineLayer* layer2= charts[1]->addLineLayer(); //Temp - c2
    LineLayer* layer3 = charts[1]->addLineLayer();

    LineLayer* layer4 = charts[2]->addLineLayer(); //OP - c3
    LineLayer* layer5 = charts[2]->addLineLayer();
    LineLayer* layer6 = charts[2]->addLineLayer();

    LineLayer* layer7 = charts[3]->addLineLayer(); //CP - c4

    LineLayer* layer8 = charts[4]->addLineLayer(); //Temp - c5
    LineLayer* layer9 = charts[4]->addLineLayer();
    LineLayer* layer10 = charts[4]->addLineLayer();
    LineLayer* layer11 = charts[4]->addLineLayer();

    LineLayer* layer12 = charts[5]->addLineLayer(); //OP - c6
    LineLayer* layer13 = charts[5]->addLineLayer();

    LineLayer* layer14 = charts[6]->addLineLayer(); //Temp - c7
    LineLayer* layer15 = charts[6]->addLineLayer();
    LineLayer* layer16 = charts[6]->addLineLayer();
    LineLayer* layer17 = charts[6]->addLineLayer();

    LineLayer* layer18 = charts[7]->addLineLayer(); //Sludge - c8

    vector<LineLayer*> layers = { layer1, layer2, layer3, layer4, layer5, layer6, layer7, layer8, layer9, layer10, 
                                layer11, layer12, layer13, layer14, layer15, layer16, layer17, layer18 };
    // vector<LineLayer*> layers = { layer1, layer2, layer3, layer4, layer5, layer6, layer7, layer8, layer9,
    //                     layer10, layer11, layer12, layer13, layer14, layer15, layer16, layer17, layer18};

    for (auto& layer : layers)
    {
        layer->setXData(DoubleArray(&m_timestamps[0], m_timestamps.size()));
    }

    // 데이터셋 색상 및 라벨 설정
    const char* colors[] = { "0xff0000", "0x0000ff", "0x00ff00", "0x00ffff" };
    const char* labels[] = { "OP", "Temp", "CP", "Sludge" };

    layers[0]->addDataSet(DoubleArray(&m_dataSets[0][0], m_dataSets[0].size()), strtol(colors[0], nullptr, 16), labels[0]);

    layers[1]->addDataSet(DoubleArray(&m_dataSets[1][0], m_dataSets[1].size()), strtol(colors[0], nullptr, 16), labels[1]);
    layers[2]->addDataSet(DoubleArray(&m_dataSets[2][0], m_dataSets[2].size()), strtol(colors[1], nullptr, 16), labels[1]);

    layers[3]->addDataSet(DoubleArray(&m_dataSets[3][0], m_dataSets[3].size()), strtol(colors[0], nullptr, 16), labels[0]);
    layers[4]->addDataSet(DoubleArray(&m_dataSets[4][0], m_dataSets[4].size()), strtol(colors[1], nullptr, 16), labels[0]);
    layers[5]->addDataSet(DoubleArray(&m_dataSets[5][0], m_dataSets[5].size()), strtol(colors[2], nullptr, 16), labels[0]);

    layers[6]->addDataSet(DoubleArray(&m_dataSets[6][0], m_dataSets[6].size()), strtol(colors[0], nullptr, 16), labels[2]);

    layers[7]->addDataSet(DoubleArray(&m_dataSets[7][0], m_dataSets[7].size()), strtol(colors[0], nullptr, 16), labels[1]);
    layers[8]->addDataSet(DoubleArray(&m_dataSets[8][0], m_dataSets[8].size()), strtol(colors[1], nullptr, 16), labels[1]);
    layers[9]->addDataSet(DoubleArray(&m_dataSets[9][0], m_dataSets[9].size()), strtol(colors[2], nullptr, 16), labels[1]);
    layers[10]->addDataSet(DoubleArray(&m_dataSets[10][0], m_dataSets[10].size()), strtol(colors[3], nullptr, 16), labels[1]);

    layers[11]->addDataSet(DoubleArray(&m_dataSets[11][0], m_dataSets[11].size()), strtol(colors[0], nullptr, 16), labels[0]);
    layers[12]->addDataSet(DoubleArray(&m_dataSets[12][0], m_dataSets[12].size()), strtol(colors[1], nullptr, 16), labels[0]);

    layers[13]->addDataSet(DoubleArray(&m_dataSets[13][0], m_dataSets[13].size()), strtol(colors[0], nullptr, 16), labels[1]);
    layers[14]->addDataSet(DoubleArray(&m_dataSets[14][0], m_dataSets[14].size()), strtol(colors[1], nullptr, 16), labels[1]);
    layers[15]->addDataSet(DoubleArray(&m_dataSets[15][0], m_dataSets[15].size()), strtol(colors[2], nullptr, 16), labels[1]);
    layers[16]->addDataSet(DoubleArray(&m_dataSets[16][0], m_dataSets[16].size()), strtol(colors[3], nullptr, 16), labels[1]);

    layers[17]->addDataSet(DoubleArray(&m_dataSets[17][0], m_dataSets[17].size()), strtol(colors[0], nullptr, 16), labels[3]);


    vector<string> titles = { "Drying - OP", "Drying - Temperature", "Quenching - OP", "Quenching - CP",
                        "Quenching - Temperature", "Salt - OP", "Salt - Temperature", "Salt - Sludge" };

    int a = 0;
    for (auto& chart : charts)
    {
        chart->xAxis()->setLabelStyle("", 8, 0xffffff); // 축 레이블 폰트 크기 축소
        chart->yAxis()->setLabelStyle("", 8, 0xffffff);
        chart->addTitle(titles[a].c_str(), "", 10, 0xffffff); // 제목이 있는 경우 폰트 크기 축소
        a++;
    }

    // 차트를 ChartViewer에 설정
    m_ChartViewer.setChart(charts[0]);
    m_ChartViewer1.setChart(charts[1]);
    m_ChartViewer2.setChart(charts[2]);
    m_ChartViewer3.setChart(charts[3]);
    m_ChartViewer4.setChart(charts[4]);
    m_ChartViewer5.setChart(charts[5]);
    m_ChartViewer6.setChart(charts[6]);
    m_ChartViewer7.setChart(charts[7]);

    for (auto& chart : charts)
    {
        delete chart;
    }
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
