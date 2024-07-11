// PROCESSDlg.cpp : implementation file
//

#include "pch.h"
#include "SF5_MFC_AI_POP.h"
#include "afxdialogex.h"
#include "PROCESSDlg.h"

#include <winhttp.h>
#pragma comment(lib, "winhttp.lib")

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
	ON_BN_CLICKED(IDC_BUTTON1, &PROCESSDlg::OnBnClickedButton1)
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


void PROCESSDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	// Create and start the threads
	MySQL_Connector* mysql1 = new MySQL_Connector();
	MySQL_Connector* mysql2 = new MySQL_Connector();

	ThreadData* dataVib = new ThreadData{ this, mysql1 };
	ThreadData* dataCur = new ThreadData{ this, mysql2 };

	CWinThread* pThreadCur = AfxBeginThread(Thread_DB_Get_Cur, dataCur);
	CWinThread* pThreadVib = AfxBeginThread(Thread_DB_Get_Vib, dataVib);


	// Create and start the wait thread, passing the thread handles
	HANDLE hThreads[2] = { pThreadCur->m_hThread, pThreadVib->m_hThread };
	ThreadWaitData* dataWait = new ThreadWaitData{ this, { pThreadCur->m_hThread, pThreadVib->m_hThread }, mysql1, mysql2 };

	AfxBeginThread(Thread_DB_Wait, dataWait);
}


UINT PROCESSDlg::Thread_DB_Wait(LPVOID _mothod)
{
	ThreadWaitData* data = (ThreadWaitData*)_mothod;

	PROCESSDlg* pDlg = data->pDlg;

	MySQL_Connector* mysql1 = data->mysqlCur;
	MySQL_Connector* mysql2 = data->mysqlVib;


	// Wait for both threads to finish
	WaitForMultipleObjects(2, data->hThreads, TRUE, INFINITE);

	// Display the message box after both threads are done
	AfxMessageBox(_T("데이터베이스 양측 수신 완료"));
	
	if (pDlg != nullptr)
	{
		pDlg->winHttp(mysql1, mysql2);
	}
	
	return 0;
}


UINT PROCESSDlg::Thread_DB_Get_Cur(LPVOID _mothod)
{
	ThreadData* data = (ThreadData*)_mothod;
	PROCESSDlg* pDlg = data->pDlg;
	MySQL_Connector* mysql = data->mysql;

	// 데이터베이스 서버 연결
	if (mysql->connect("tcp://192.168.1.241:3306", "Nia", "0000", "pop")) // 수정
	{
		mysql->getTable(mysql->fetchDataFromTable("current", 10));
	}
	else {
		pDlg->MessageBox(_T("데이터베이스 연결 실패"), _T("오류"), MB_OK | MB_ICONERROR);
	}

	//스레드 함수 종료 시
	return 0;
}


UINT PROCESSDlg::Thread_DB_Get_Vib(LPVOID _mothod)
{
	ThreadData* data = (ThreadData*)_mothod;
	PROCESSDlg* pDlg = data->pDlg;
	MySQL_Connector* mysql = data->mysql;

	// 데이터베이스 서버 연결
	if (mysql->connect("tcp://192.168.1.241:3306", "Nia", "0000", "pop")) // 수정
	{
		mysql->getTable(mysql->fetchDataFromTable("vibration", 10));
	}
	else {
		pDlg->MessageBox(_T("데이터베이스 연결 실패"), _T("오류"), MB_OK | MB_ICONERROR);
	}

	//스레드 함수 종료 시
	return 0;
}

void PROCESSDlg::winHttp(MySQL_Connector* mysql1, MySQL_Connector* mysql2)
{
	HINTERNET hSession = WinHttpOpen(L"A WinHTTP Example Program/1.0",
		WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
		WINHTTP_NO_PROXY_NAME,
		WINHTTP_NO_PROXY_BYPASS, 0);

	if (hSession)
	{
		HINTERNET hConnect = WinHttpConnect(hSession, L"127.0.0.1", 5000, 0);

		if (hConnect)
		{
			CStringA jsonData;
			CStringA vib(mysql1->data2);
			CStringA cur(mysql2->data2);

			jsonData.Format("{\"vibration\": [%s], \"current\": [%s]}", vib, cur);

			HINTERNET hRequest = WinHttpOpenRequest(hConnect, L"POST", L"/api/robot_welding_predicitive_maintenance",
				NULL, WINHTTP_NO_REFERER,
				WINHTTP_DEFAULT_ACCEPT_TYPES,
				0);

			if (hRequest)
			{
				// Set request headers
				const wchar_t* headers = L"Content-Type: application/json";
				if (!WinHttpSendRequest(hRequest, headers, -1L,
					(LPVOID)(LPSTR)jsonData.GetBuffer(), jsonData.GetLength(), jsonData.GetLength(), 0))
				{
					AfxMessageBox(_T("Error sending request."));
				}
				else
				{
					if (!WinHttpReceiveResponse(hRequest, NULL))
					{
						AfxMessageBox(_T("Error receiving response."));
					}
					else
					{
						DWORD dwSize = 0;
						DWORD dwDownloaded = 0;
						LPSTR pszOutBuffer;
						CStringA response;

						do
						{
							dwSize = 0;
							if (!WinHttpQueryDataAvailable(hRequest, &dwSize))
							{
								AfxMessageBox(_T("Error in WinHttpQueryDataAvailable."));
								break;
							}

							pszOutBuffer = new char[dwSize + 1];

							if (!pszOutBuffer)
							{
								AfxMessageBox(_T("Out of memory."));
								dwSize = 0;
							}
							else
							{
								ZeroMemory(pszOutBuffer, dwSize + 1);

								if (!WinHttpReadData(hRequest, (LPVOID)pszOutBuffer,
									dwSize, &dwDownloaded))
								{
									AfxMessageBox(_T("Error in WinHttpReadData."));
								}
								else
								{
									response += CStringA(pszOutBuffer);
								}

								delete[] pszOutBuffer;
							}
						} while (dwSize > 0);

						AfxMessageBox(CString(response));
					}
				}

				WinHttpCloseHandle(hRequest);
			}

			WinHttpCloseHandle(hConnect);
		}

		WinHttpCloseHandle(hSession);
	}
}