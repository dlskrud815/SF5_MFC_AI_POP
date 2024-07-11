// PROCESSDlg.cpp : implementation file
//

#include "pch.h"
#include "SF5_MFC_AI_POP.h"
#include "afxdialogex.h"
#include "PROCESSDlg.h"


// PROCESSDlg dialog

IMPLEMENT_DYNAMIC(PROCESSDlg, CDialogEx)

PROCESSDlg::PROCESSDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PROCESS_DIALOG, pParent)
{
	// Initialize the random seed
	srand(static_cast<unsigned int>(time(NULL)));

	// Initialize x and y data buffers
	for (int i = 0; i < 10; ++i)
	{
		xData.push_back(i);
		yData.push_back(rand() % 100);
	}
}

PROCESSDlg::~PROCESSDlg()
{
}

void PROCESSDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHART1, m_chartView);
}


BEGIN_MESSAGE_MAP(PROCESSDlg, CDialogEx)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// PROCESSDlg message handlers


BOOL PROCESSDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	//ShowWindow(SW_SHOWMAXIMIZED); // 최대화

	SetTimer(1, 1000, NULL);
	updateChart();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void PROCESSDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1)
	{
		// Update the chart with new data
		updateChart();
	}

	CDialogEx::OnTimer(nIDEvent);
}

void PROCESSDlg::updateChart()
{
	// Generate a new random value and update yData
	yData.erase(yData.begin());
	yData.push_back(rand() % 100);

	// Create a XYChart object of size 250 x 250 pixels
	XYChart* c = new XYChart(250, 250);

	// Set the plot area at (30, 20) and of size 200 x 200 pixels
	c->setPlotArea(30, 20, 200, 200);
	c->setBackground(0x454C54);

	// Hide x-axis and y-axis labels
	c->xAxis()->setLabelStyle("arial.ttf", 8, 0xffffff); // Adjust font and color as needed
	c->yAxis()->setLabelStyle("arial.ttf", 8, 0xffffff); // Adjust font and color as needed

	// Convert yData to DoubleArray
	double* yArray = new double[yData.size()];
	for (size_t i = 0; i < yData.size(); ++i) {
		yArray[i] = yData[i];
	}
	DoubleArray dataY(yArray, static_cast<int>(yData.size()));

	// Add a line layer to the chart using xData and yData
	c->addLineLayer(dataY);

	// Set the chart to the chart viewer
	m_chartView.setChart(c);

	// Dispose of the chart object to avoid memory leaks
	delete c;
	delete[] yArray;
}
