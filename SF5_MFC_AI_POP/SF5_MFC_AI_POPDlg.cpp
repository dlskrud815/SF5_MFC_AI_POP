
// SF5_MFC_AI_POPDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "SF5_MFC_AI_POP.h"
#include "SF5_MFC_AI_POPDlg.h"
#include "afxdialogex.h"
#include "LOGINDlg.h"
#include "PROCESSDlg.h"
#include <string>
#include <iostream>

#include <winhttp.h>
#pragma comment(lib, "winhttp.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WM_UPDATE_TIME (WM_APP + 1)

//CString CSF5MFCAIPOPDlg::strCur;
//CString CSF5MFCAIPOPDlg::strVib;
//int CSF5MFCAIPOPDlg::offsetCur = 10;
//int CSF5MFCAIPOPDlg::offsetVib = 10;
CCriticalSection CSF5MFCAIPOPDlg::critSect;

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CSF5MFCAIPOPDlg dialog

CSF5MFCAIPOPDlg::CSF5MFCAIPOPDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SF5_MFC_AI_POP_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSF5MFCAIPOPDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ERROR, m_listCtrl);
	DDX_Control(pDX, IDC_STATIC_Test1, m_test1);
	DDX_Control(pDX, IDC_STATIC_Test2, m_test2);
	DDX_Control(pDX, IDC_STATIC_RESULT, m_result);
}

BEGIN_MESSAGE_MAP(CSF5MFCAIPOPDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON1, &CSF5MFCAIPOPDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CSF5MFCAIPOPDlg::OnBnClickedButton2)
	ON_MESSAGE(WM_UPDATE_TIME, &CSF5MFCAIPOPDlg::OnUpdateTime)
	//ON_WM_TIMER()
END_MESSAGE_MAP()


// CSF5MFCAIPOPDlg message handlers

BOOL CSF5MFCAIPOPDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 로그인 다이얼로그 먼저 띄움
	LOGINDlg loginDlg;
	if (loginDlg.DoModal() != IDOK)
	{
		// Handle login failure or cancellation
		EndDialog(IDCANCEL); // Close the main dialog if login fails or is cancelled
		return FALSE;
	}

	// If here, login was successful, continue initializing the main dialog
	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	ShowWindow(SW_SHOWMAXIMIZED); // 최대화
	//back.Load(_T("res\\img\\BACK1.png"));

	// 타이머를 설정하여 1초마다 시간을 업데이트합니다.
	//SetTimer(m_nTimerID, 1000, nullptr);


	// 리스트 칼럼 넣기
	m_listCtrl.InsertColumn(0, L"메시지", LVCFMT_LEFT, 400, -1);
	m_listCtrl.InsertColumn(0, L"설비", LVCFMT_LEFT, 200, -1);
	m_listCtrl.InsertColumn(0, L"상태", LVCFMT_LEFT, 100, -1);
	m_listCtrl.InsertColumn(0, L"시간", LVCFMT_LEFT, 200, -1);
	m_listCtrl.InsertColumn(0, L"순번", LVCFMT_LEFT, 100, -1);

	// 메인 스레드 생성
	CWinThread* pMainThread = AfxBeginThread(MainThread, this);
	if (pMainThread == NULL) AfxMessageBox(L"MainThread Create Error");

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSF5MFCAIPOPDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSF5MFCAIPOPDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		//CPaintDC dc(this); // device context for painting

		//CRect rect;//픽쳐 컨트롤의 크기를 저장할 CRect 객체
		//GetClientRect(&rect);

		//back.StretchBlt(dc.m_hDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);//이미지를 픽쳐 컨트롤 크기로 조정

		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSF5MFCAIPOPDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CSF5MFCAIPOPDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	const int buttonWidth = 150;
	const int buttonHeight = 40;
	const int margin = 20;

	const int timeWidth = 300;
	const int timeHeight = 40;

	const int listHeight = 500;

	int xPosCancel = cx - buttonWidth - margin;
	int xPosOK = xPosCancel - buttonWidth - margin;
	int yPos = timeHeight + 2 * margin;
	int yPosListControl = cy - listHeight - margin;

	int xPosTime = cx - timeWidth - margin;
	int yPosTime = margin;

	CWnd* pOKButton = GetDlgItem(IDOK);
	CWnd* pCancelButton = GetDlgItem(IDCANCEL);
	CWnd* pListControl = GetDlgItem(IDC_LIST_ERROR);
	CWnd* pCurTime = GetDlgItem(IDC_STATIC_CURRENT_TIME);

	if (pOKButton != nullptr)
	{
		pOKButton->SetWindowPos(nullptr, xPosOK, yPos, buttonWidth, buttonHeight, SWP_NOZORDER);
	}

	if (pCancelButton != nullptr)
	{
		pCancelButton->SetWindowPos(nullptr, xPosCancel, yPos, buttonWidth, buttonHeight, SWP_NOZORDER);
	}

	if (pListControl != nullptr)
	{
		pListControl->SetWindowPos(nullptr, margin, yPosListControl, cx - 2 * margin, listHeight, SWP_NOZORDER);
	}

	if (pCurTime != nullptr)
	{
		pCurTime->SetWindowPos(nullptr, xPosTime, yPosTime, timeWidth, timeHeight, SWP_NOZORDER);
	}
}


//void CSF5MFCAIPOPDlg::OnTimer(UINT_PTR nIDEvent)
//{
//	// 타이머가 발생하면 현재 시각을 업데이트합니다.
//	if (nIDEvent == m_nTimerID)
//	{
//		UpdateCurrentTime();
//	}
//
//	CDialogEx::OnTimer(nIDEvent);
//}
//
//void CSF5MFCAIPOPDlg::UpdateCurrentTime()
//{
//	// 현재 시간을 가져옵니다.
//	CTime currentTime = CTime::GetCurrentTime();
//
//	// 포맷을 설정하고 문자열로 변환합니다.
//	CString strTime = currentTime.Format(_T("%Y-%m-%d %H:%M:%S"));
//
//	// IDC_STATIC_TIME은 다이얼로그 디자이너에서 생성된 스태틱 텍스트 컨트롤의 ID입니다.
//	CWnd* pWnd = GetDlgItem(IDC_STATIC_CURRENT_TIME);
//
//	if (pWnd)
//	{
//		pWnd->SetWindowText(strTime);
//	}
//}


void CSF5MFCAIPOPDlg::OnBnClickedButton1()
{
	//CWinThread* p1 = AfxBeginThread(ThreadTest, this);

	//CWinThread* pThreadCur = AfxBeginThread(Thread_DB_Get_Cur, this);
	//CWinThread* pThreadVib = AfxBeginThread(Thread_DB_Get_Vib, this);

	//HANDLE hThreads[2] = { pThreadCur->m_hThread, pThreadVib->m_hThread };
	//WaitForMultipleObjects(2, hThreads, TRUE, INFINITE);

	CThreadTest::Thread_DB_Wait();

	CStringA jsonData;
	jsonData = prepareData(ROBOT);

	wstring endpoint = SendPostRequest(ROBOT);

	CStringA result = winHttp(jsonData, endpoint, 5001);

	vector<int> parse = parsing_robot(result);

	string cur_result, vib_result;

	if (parse.size() >= 2) {
		if (parse[0] == 1)
		{
			cur_result = "전류 이상";
		}
		else
		{
			cur_result = "전류 정상";
		}

		if (parse[1] == 1)
		{
			vib_result = "진동 이상";
		}
		else
		{
			vib_result = "진동 정상";
		}
	}

	m_listCtrl.InsertItem(0, CString(cur_result.c_str()));
	m_listCtrl.SetItem(0, 1, LVIF_TEXT, CString(vib_result.c_str()), 0, 0, 0, NULL);
}

void CSF5MFCAIPOPDlg::OnBnClickedButton2()
{
	//PROCESSDlg proDlg;
	//proDlg.DoModal();

	CThreadTest::Thread_DB_Wait_Plastic();

	CStringA jsonData;
	jsonData = prepareData(PLASTIC);

	wstring endpoint = SendPostRequest(PLASTIC);

	CStringA result = winHttp(jsonData, endpoint, 5002);

	vector<int> parse = parsing_plastic(result);

	string v0Result, v1Result, c1Result;

	if (parse.size() >= 3) {
		if (parse[0] == 1)
		{
			v0Result = "prediction 0";
		}
		else
		{
			v0Result = "prediction 1";
		}
	}

	m_listCtrl.InsertItem(0, CString(v0Result.c_str()));
}

vector<int> CSF5MFCAIPOPDlg::parsing_robot(CStringA response)
{
	// JSON 파싱을 위한 간단한 문자열 처리
	CStringA vibToken = "\"vib_result\":";
	CStringA curToken = "\"cur_result\":";

	// vib_result 추출
	int vibResult = 0;
	int posVib = response.Find(vibToken);
	if (posVib >= 0)
	{
		posVib += vibToken.GetLength();
		CStringA vibValueStr = response.Mid(posVib);
		vibResult = atoi(vibValueStr); // atoi 함수를 사용하여 문자열을 정수로 변환
	}

	// cur_result 추출
	int curResult = 0;
	int posCur = response.Find(curToken);
	if (posCur >= 0)
	{
		posCur += curToken.GetLength();
		CStringA curValueStr = response.Mid(posCur);
		curResult = atoi(curValueStr); // atoi 함수를 사용하여 문자열을 정수로 변환
	}

	return { vibResult, curResult };
}

vector<int> CSF5MFCAIPOPDlg::parsing_plastic(CStringA response) {

	// JSON 파싱을 위한 간단한 문자열 처리
	CStringA v0Token = "\"prediction_result\":";

	// v0_Result  추출
	int v0Result = 0;
	int posV0 = response.Find(v0Token);
	if (posV0 >= 0)
	{
		posV0 += v0Token.GetLength();
		CStringA v0ValueStr = response.Mid(posV0);
		v0Result = atoi(v0ValueStr); // atoi 함수를 사용하여 문자열을 정수로 변환
	}

	return { v0Result };
}

CStringA CSF5MFCAIPOPDlg::prepareData(tName process) {
	CStringA jsonData;
	switch (process) {
	case ROBOT: {
		string cur = CThreadTest::strCur;
		string vib = CThreadTest::strVib;

		CStringA A_cur(cur.c_str());
		CStringA A_vib(vib.c_str());

		jsonData.Format("{\"vibration\": [%s], \"current\": [%s]}", A_vib, A_cur);
		break;
	}
	case PLASTIC: {
		string v0 = CThreadTest::strV0;
		string v1 = CThreadTest::strV1;
		string c1 = CThreadTest::strC1;

		CStringA A_v0(v0.c_str());
		CStringA A_v1(v1.c_str());
		CStringA A_c1(c1.c_str());

		jsonData.Format("{\"V0\": [%s], \"V1\": [%s], \"C1\": [%s]}", A_v0, A_v1, A_c1);
		break;
	}
	default:
		AfxMessageBox(_T("Unknown process"));
		return CStringA("");
	}

	return jsonData;
}

wstring CSF5MFCAIPOPDlg::StringToWideString(const string& str) {
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), NULL, 0);
	wstring wstrTo(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), &wstrTo[0], size_needed);
	return wstrTo;
}


wstring CSF5MFCAIPOPDlg::SendPostRequest(tName process)
{
	wstring endpoint;
	switch (process) {
	case ROBOT:
		endpoint = L"/api/robot_welding_predicitive_maintenance";
		break;
	case PLASTIC:
		endpoint = L"/api/plastic_working_predicitive_maintenance";
		break;
	default:
		AfxMessageBox(_T("Unknown process"));
		return  L" ";
	}


	return endpoint;
}

CStringA CSF5MFCAIPOPDlg::winHttp(CStringA jsonData, wstring endpoint, int port)
{
	CStringA response;

	HINTERNET hSession = WinHttpOpen(L"A WinHTTP Example Program/1.0",
		WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
		WINHTTP_NO_PROXY_NAME,
		WINHTTP_NO_PROXY_BYPASS, 0);

	if (hSession)
	{
		HINTERNET hConnect = WinHttpConnect(hSession, L"127.0.0.1", port, 0);

		if (hConnect)
		{
			HINTERNET hRequest = WinHttpOpenRequest(hConnect, L"POST", endpoint.c_str(),
				NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, 0);

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

	return response;
}



//UINT CSF5MFCAIPOPDlg::Thread_DB_Get_Cur(LPVOID _method)
//{
//	MySQL_Connector* mysql = new MySQL_Connector();
//	int i = 0;
//	string output;
//
//	if (mysql->connect("tcp://192.168.1.241:3306", "Nia", "0000", "pop"))
//	{
//		vector<double> cur = mysql->fetchDataFromTable("current", offsetCur);
//
//		// 크리티컬 영역 지정
//		critSect.Lock();
//
//		for (double num : cur)
//		{
//			i++;
//			if (i >= cur.size())
//			{
//				output += to_string(num);
//			}
//			else
//			{
//				output += to_string(num) + ", ";
//			}
//		}
//
//		strCur = output.c_str();
//		critSect.Unlock();
//	}
//
//	delete mysql;
//	return 0;
//}
//
//
//UINT CSF5MFCAIPOPDlg::Thread_DB_Get_Vib(LPVOID _method)
//{
//	MySQL_Connector* mysql = new MySQL_Connector();
//	int i = 0;
//	string output;
//
//	if (mysql->connect("tcp://192.168.1.241:3306", "Nia", "0000", "pop"))
//	{
//		vector<double> vib = mysql->fetchDataFromTable("vibration", offsetVib);
//
//		// 크리티컬 영역 지정
//		critSect.Lock();
//
//		for (double num : vib)
//		{
//			i++;
//			if (i >= vib.size())
//			{
//				output += to_string(num);
//			}
//			else
//			{
//				output += to_string(num) + ", ";
//			}
//		}
//
//		strVib = output.c_str();
//		critSect.Unlock();
//	}
//
//	delete mysql;
//	return 0;
//}

UINT CSF5MFCAIPOPDlg::MainThread(LPVOID _method)
{
	CSF5MFCAIPOPDlg* pDlg = (CSF5MFCAIPOPDlg*)_method;
	MySQL_Connector* mysql = new MySQL_Connector();

	// Create the time update thread
	CWinThread* pTimeThread = AfxBeginThread(TimeUpdateThread, (LPVOID)pDlg);
	if (pTimeThread == NULL)
	{
		AfxMessageBox(L"TimeUpdateThread Create Error");
	}

	return 0;
}

UINT CSF5MFCAIPOPDlg::TimeUpdateThread(LPVOID pParam)
{
	CSF5MFCAIPOPDlg* pDlg = (CSF5MFCAIPOPDlg*)pParam;

	// AfxMessageBox(L"진입");

	while (true)
	{
		// Get the current time
		CTime currentTime = CTime::GetCurrentTime();
		CString strTime = currentTime.Format(L"%Y-%m-%d %H:%M:%S");

		//AfxMessageBox(L"Current Time: " + strTime);

		pDlg->PostMessage(WM_UPDATE_TIME, (WPARAM)new CString(strTime));

		Sleep(1000);
	}

	return 0;
}


LRESULT CSF5MFCAIPOPDlg::OnUpdateTime(WPARAM wParam, LPARAM lParam)
{
	CString* pStrTime = (CString*)wParam;

	if (pStrTime)
	{
		//AfxMessageBox(L"Received Time: " + *pStrTime);

		SetDlgItemText(IDC_STATIC_CURRENT_TIME, *pStrTime); // Update the static text control with ID IDC_STATIC_TIME
		delete pStrTime; // Clean up the allocated memory
	}

	return 0;
}