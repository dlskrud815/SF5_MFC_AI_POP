
// SF5_MFC_AI_POPDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "SF5_MFC_AI_POP.h"
#include "SF5_MFC_AI_POPDlg.h"
#include "afxdialogex.h"
#include "LOGINDlg.h"
#include <string>
#include <iostream>

#include <winhttp.h>
#pragma comment(lib, "winhttp.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WM_UPDATE_TIME (WM_APP + 1)
#define WM_NOTICE_ROBOT (WM_APP + 2)
#define WM_NOTICE_PLASTIC (WM_APP + 3)
#define WM_NOTICE_HEAT (WM_APP + 4)
#define WM_NOTICE_LIST (WM_APP + 5)

CCriticalSection CSF5MFCAIPOPDlg::critSect;

int CSF5MFCAIPOPDlg::listIndex = 1;

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
	DDX_Control(pDX, IDC_LIST_ERROR2, m_listCtrl2);
	DDX_Control(pDX, IDC_LIST_ERROR3, m_listCtrl3);
}

BEGIN_MESSAGE_MAP(CSF5MFCAIPOPDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_ROBOT, &CSF5MFCAIPOPDlg::OnBnClickedButtonRobot)
	ON_BN_CLICKED(IDC_BUTTON_PLASTIC, &CSF5MFCAIPOPDlg::OnBnClickedButtonPlastic)
	ON_BN_CLICKED(IDC_BUTTON_HEAT, &CSF5MFCAIPOPDlg::OnBnClickedButtonHeat)
	ON_MESSAGE(WM_UPDATE_TIME, &CSF5MFCAIPOPDlg::OnUpdateTime)
	ON_MESSAGE(WM_NOTICE_ROBOT, &CSF5MFCAIPOPDlg::OnNoticeRobotError)
	ON_MESSAGE(WM_NOTICE_PLASTIC, &CSF5MFCAIPOPDlg::OnNoticePlasticError)
	ON_MESSAGE(WM_NOTICE_HEAT, &CSF5MFCAIPOPDlg::OnNoticeHeatError)
	ON_MESSAGE(WM_NOTICE_LIST, &CSF5MFCAIPOPDlg::OnNoticeList)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_ROBOT_NOTICE, &CSF5MFCAIPOPDlg::OnBnClickedButtonRobotNotice)
	ON_BN_CLICKED(IDC_BUTTON_PLASTIC_NOTICE, &CSF5MFCAIPOPDlg::OnBnClickedButtonPlasticNotice)
	ON_BN_CLICKED(IDC_BUTTON_HEAT_NOTICE, &CSF5MFCAIPOPDlg::OnBnClickedButtonHeatNotice)
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

	// Picture Control 초기화
	m_staticFacility.SubclassDlgItem(IDC_STATIC_FACILITY, this);

	CRect rect;
	GetClientRect(&rect);

	const int imgWidth = (rect.Width() - 3 * 50 - 800);
	const int imgHeight = (rect.Height() - 2 * 10 - 110 - 2 * 50) * 2 / 3;
	const int gap = ((rect.Height() - 2 * 10 - 110 - 2 * 50) - imgHeight) / 2;
	m_staticFacility.SetWindowPos(NULL, 50, rect.Height() - 50 - gap - imgHeight, imgWidth, imgHeight, 0);


	//CWnd* pTitle = GetDlgItem(IDC_STATIC_TITLE);
	//if (pTitle != nullptr)
	//{
	//	pTitle->SetWindowPos(&wndTop, 10, 10, rect.Width() - 2 * 10, 10 + 110, 0);
	//}

	m_font.CreatePointFont(150, L"나눔스퀘어");
	GetDlgItem(IDC_STATIC_CURRENT_TIME)->SetFont(&m_font);


	CRect rectHeader, rectNotice1, rectNotice2, rectNotice3;
	GetDlgItem(IDC_STATIC_HEADER)->GetWindowRect(&rectHeader);
	ScreenToClient(&rectHeader);
	m_staticHeader.Create(_T(""), WS_VISIBLE | WS_CHILD | SS_NOTIFY, rectHeader, this, IDC_STATIC_HEADER);
	GetDlgItem(IDC_STATIC_HEADER)->ShowWindow(SW_HIDE);



	GetDlgItem(IDC_STATIC_NOTICE1)->GetWindowRect(&rectNotice1);
	ScreenToClient(&rectNotice1);
	GetDlgItem(IDC_STATIC_NOTICE2)->GetWindowRect(&rectNotice2);
	ScreenToClient(&rectNotice2);
	GetDlgItem(IDC_STATIC_NOTICE3)->GetWindowRect(&rectNotice3);
	ScreenToClient(&rectNotice3);

	m_staticNotice1.Create(_T(""), WS_VISIBLE | WS_CHILD | SS_NOTIFY, rectNotice1, this);
	m_staticNotice2.Create(_T(""), WS_VISIBLE | WS_CHILD | SS_NOTIFY, rectNotice2, this);
	m_staticNotice3.Create(_T(""), WS_VISIBLE | WS_CHILD | SS_NOTIFY, rectNotice3, this);

	// 리스트 칼럼 넣기
	CListCtrl* listCtrls[] = { &m_listCtrl, &m_listCtrl2, &m_listCtrl3 };

	for (CListCtrl* list : listCtrls)
	{
		list->InsertColumn(0, L"메시지", LVCFMT_LEFT, 400, -1);
		list->InsertColumn(0, L"설비", LVCFMT_LEFT, 150, -1);
		list->InsertColumn(0, L"상태", LVCFMT_LEFT, 150, -1);
		list->InsertColumn(0, L"시간", LVCFMT_LEFT, 250, -1);
		list->InsertColumn(0, L"순번", LVCFMT_LEFT, 50, -1);
	}


	// RobotDetailDlg 초기화
	pChartDialog_Robot = new RobotDetailDlg();
	pChartDialog_Robot->Create(IDD_ROBOT_DETAIL_DAILOG, this);

	pChartDialog_Plastic = new PlasticDetailDlg();
	pChartDialog_Plastic->Create(IDD_PLASTIC_DETAIL_DAILOG, this);

	pChartDialog_Heat = new HeatDetailDlg();
	pChartDialog_Heat->Create(IDD_HEAT_DETAIL_DAILOG, this);

	pChartDialog_Robot->ShowWindow(SW_SHOW);
	pChartDialog_Plastic->ShowWindow(SW_SHOW);
	pChartDialog_Heat->ShowWindow(SW_SHOW);

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
		CDialogEx::OnPaint();

		// Picture Control에 이미지를 그림
		CStatic* pStaticFacility = (CStatic*)GetDlgItem(IDC_STATIC_FACILITY);
		if (pStaticFacility != nullptr)
		{
			CRect rect;
			pStaticFacility->GetClientRect(&rect);

			// 클라이언트 DC 가져오기
			CDC* pDC = pStaticFacility->GetDC();

			// 이미지를 Picture Control 크기에 맞게 스케일링하여 그리기
			m_imageFacility.Draw(pDC->m_hDC, rect);

			// DC 해제
			pStaticFacility->ReleaseDC(pDC);
		}
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

	// 헤더 컨트롤 위치 계산
	CWnd* pHeader = GetDlgItem(IDC_STATIC_HEADER);
	if (pHeader != nullptr)
	{
		pHeader->SetWindowPos(nullptr, 10, 10, cx - 2 * 10, 10 + 110 , 0);
	}
	 
	// 현재 시간 텍스트 위치 설정
	CWnd* pCurTime = GetDlgItem(IDC_STATIC_CURRENT_TIME);
	if (pCurTime != nullptr)
	{
		pCurTime->SetWindowPos(nullptr, cx - 2 * 10 - 400 - 30, 2 * 10 + 30, 400, 60, 0);
	}
	 
	const int listHeight = (cy - 4 * 50 - 110 - 10) / 3;

	// 리스트 컨트롤 위치 설정
	CWnd* pListControl = GetDlgItem(IDC_LIST_ERROR);
	if (pListControl != nullptr)
	{
		pListControl->SetWindowPos(nullptr, cx - 800 - 50, 2 * 10 + 110 + 50, 800, listHeight, 0);
	}
	 
	// 리스트 컨트롤 위치 설정
	CWnd* pListControl2 = GetDlgItem(IDC_LIST_ERROR2);
	if (pListControl2 != nullptr)
	{
		pListControl2->SetWindowPos(nullptr, cx - 800 - 50, 2 * 10 + 110 + 2 * 50 + listHeight, 800, listHeight, 0);
	}

	// 리스트 컨트롤 위치 설정
	CWnd* pListControl3 = GetDlgItem(IDC_LIST_ERROR3);
	if (pListControl3 != nullptr)
	{
		pListControl3->SetWindowPos(nullptr, cx - 800 - 50, 2 * 10 + 110 + 3 * 50 + 2 * listHeight, 800, listHeight, 0);
	}
	 
	// Cancel 버튼 위치 설정
	CWnd* pCancelButton = GetDlgItem(IDCANCEL);
	if (pCancelButton != nullptr)
	{
		pCancelButton->SetWindowPos(nullptr, 10, cy - 10 - 50, 100, 50, 0);
	}

	const int centerWidth = cx - 3 * 50 - 800;

	CWnd* pNotice1 = GetDlgItem(IDC_STATIC_NOTICE1);
	if (pNotice1 != nullptr)
	{
		pNotice1->SetWindowPos(nullptr, centerWidth / 2  - 500 / 2, 2 * 10 + 110 + 100, 500, 150, 0);
	}

	CWnd* pNotice2 = GetDlgItem(IDC_STATIC_NOTICE2);
	if (pNotice2 != nullptr)
	{
		pNotice2->SetWindowPos(nullptr, 50 + 100, cy - 50 - 300, 500, 150, 0);
	}

	CWnd* pNotice3 = GetDlgItem(IDC_STATIC_NOTICE3);
	if (pNotice3 != nullptr)
	{
		pNotice3->SetWindowPos(nullptr, cx - 2 * 50 - 800 - 200 - 500, cy - 50 - 300, 500, 150, 0);
	}

	CWnd* pRobotBtn = GetDlgItem(IDC_BUTTON_ROBOT);
	if (pRobotBtn != nullptr)
	{
		pRobotBtn->SetWindowPos(nullptr, cx - 2 * 50 - 800 - 200, 2 * 10 + 110 + 50, 200, 50, 0);
	}

	CWnd* pPlasticBtn = GetDlgItem(IDC_BUTTON_PLASTIC);
	if (pPlasticBtn != nullptr)
	{
		pPlasticBtn->SetWindowPos(nullptr, cx - 2 * 50 - 800 - 200, 2 * 10 + 110 + 50 + 20 + 50, 200, 50, 0);
	}

	CWnd* pHeatBtn = GetDlgItem(IDC_BUTTON_HEAT);
	if (pHeatBtn != nullptr)
	{
		pHeatBtn->SetWindowPos(nullptr, cx - 2 * 50 - 800 - 200, 2 * 10 + 110 + 50 + 2 * 20 + 2 * 50, 200, 50, 0);
	}

	CWnd* pRobotNotice = GetDlgItem(IDC_BUTTON_ROBOT_NOTICE);
	if (pRobotNotice != nullptr)
	{
		//pRobotNotice->SetWindowPos(nullptr, cx - 2 * 50 - 800 - 200 , 2 * 10 + 110 + 50, 200, 50, 0);
	}
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

vector<int> CSF5MFCAIPOPDlg::parsing_heat(CStringA response) {

	// JSON 파싱을 위한 간단한 문자열 처리
	CStringA heatToken = "\"prediction_result\":";

	// v0_Result  추출
	int heatResult = 0;
	int posHeat = response.Find(heatToken);
	if (posHeat >= 0)
	{
		posHeat += heatToken.GetLength();
		CStringA heatValueStr = response.Mid(posHeat);
		heatResult = atoi(heatValueStr); // atoi 함수를 사용하여 문자열을 정수로 변환
	}

	return { heatResult };
}

CStringA CSF5MFCAIPOPDlg::prepareData(tName process, LPVOID pParam) {
	CSF5MFCAIPOPDlg* pDlg = (CSF5MFCAIPOPDlg*)pParam;

	CStringA jsonData;
	switch (process) {
	case ROBOT: {
		string cur = pDlg->strCur;
		string vib = pDlg->strVib;

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
	case HEAT: {
		vector<string> colHeat = CThreadTest::strHeat;

		vector<CStringA> A_colHeat;
		for (string colH : colHeat) {

			A_colHeat.push_back(colH.c_str());
		}

		/*
		vector<vector<CStringA>> A_colHeat;
		for (string colH : colHeat) {
			vec.clear();
			vec.push_back(colH.c_str());
			A_colHeat.push_back(vec);
		}*/
		// 튜플의 값이 20개 모두 들어왔는지 확인.
		/*int i = 0;
		vector<CStringA> A_colHeat;
		for (string colH : colHeat) {
			A_colHeat.push_back(colH.c_str());
			i++;
		}
		CA2CT s = to_string(i).c_str();
		AfxMessageBox(s);*/

		jsonData.Format("{\"Drying_2_Zone_OP\": %s, \"Drying_Path_Tmp_1_Zone\": %s, \"Drying_Path_Tmp_2_Zone\": %s, \"Cleaner\": %s, \"Quenching_2_Zone_OP\": %s, \"Quenching_3_Zone_OP\": %s, \"Quenching_4_Zone_OP\": %s, \"Quenching_Path_CP_Value\": %s, \"Quenching_Path_Tmp_1_Zone\": %s, \"Quenching_Path_Tmp_2_Zone\": %s, \"Quenching_Path_Tmp_3_Zone\": %s, \"Quenching_Path_Tmp_4_Zone\": %s, \"Salt_1_Zone_OP\": %s, \"Salt_2_Zone_OP\": %s, \"Removing_Salt_Sludge\": %s, \"Salt_Conveyor_Tmp_1_Zone\": %s, \"Salt_Conveyor_Tmp_2_Zone\": %s, \"Salt_Bath_Tmp_1_Zone\": %s, \"Salt_Bath_Tmp_2_Zone\": %s, \"Abnormal_Num\": %s}", A_colHeat[0], A_colHeat[1], A_colHeat[2], A_colHeat[3], A_colHeat[4], A_colHeat[5], A_colHeat[6], A_colHeat[7], A_colHeat[8], A_colHeat[9], A_colHeat[10], A_colHeat[11], A_colHeat[12], A_colHeat[13], A_colHeat[14], A_colHeat[15], A_colHeat[16], A_colHeat[17], A_colHeat[18], A_colHeat[19]);
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
	case HEAT:
		endpoint = L"/api/heat_treatment_predicitive_maintenance";
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

						//AfxMessageBox(CString(response));
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


UINT CSF5MFCAIPOPDlg::MainThread(LPVOID _method)
{
	CSF5MFCAIPOPDlg* pDlg = (CSF5MFCAIPOPDlg*)_method;
	MySQL_Connector* mysql = new MySQL_Connector();

	// Create the time update thread
	CWinThread* pTimeThread = AfxBeginThread(TimeUpdateThread, (LPVOID)pDlg);
	if (pTimeThread == NULL) AfxMessageBox(L"TimeUpdateThread Create Error");

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



string CSF5MFCAIPOPDlg::vectorToString(vector<double> vec) {
	string output;
	int i = 0;

	for (double num : vec)
	{
		i++;

		if (i >= vec.size())
		{
			output += std::to_string(num);
		}
		else
		{
			output += std::to_string(num) + ", ";
		}
	}
	return output;
}

UINT CSF5MFCAIPOPDlg::Thread1(LPVOID pParam)
{
	CSF5MFCAIPOPDlg* pDlg = (CSF5MFCAIPOPDlg*)pParam;
	MySQL_Connector* mysql = new MySQL_Connector();

	if (mysql->connect("tcp://127.0.0.1:3306", "Nia", "0000", "pop"))
	{
		pDlg->cur = mysql->fetchDataFromTable(ROBOT_CUR, pDlg->offsetCur);

		pDlg->strCur = pDlg->vectorToString(pDlg->cur);
	}

	delete mysql;

	return 0;
}

UINT CSF5MFCAIPOPDlg::Thread2(LPVOID pParam)
{
	CSF5MFCAIPOPDlg* pDlg = (CSF5MFCAIPOPDlg*)pParam;
	MySQL_Connector* mysql = new MySQL_Connector();

	if (mysql->connect("tcp://127.0.0.1:3306", "Nia", "0000", "pop"))
	{
		pDlg->vib = mysql->fetchDataFromTable(ROBOT_VIB, pDlg->offsetVib);

		pDlg->strVib = pDlg->vectorToString(pDlg->vib);
	}

	delete mysql;

	return 0;
}



UINT CSF5MFCAIPOPDlg::RobotThread(LPVOID pParam)
{
	CSF5MFCAIPOPDlg* pDlg = (CSF5MFCAIPOPDlg*)pParam;

	// 대기 상태에서 이벤트를 기다립니다.
	DWORD dwWaitResult = WaitForSingleObject(pDlg->m_eventRobotThread, INFINITE);

	// 이벤트가 설정되면 다음 작업을 수행합니다.
	if (dwWaitResult == WAIT_OBJECT_0)
	{
		int outlier = 0;
		bool outCur, outVib;
		while (outlier < 5)
		{
			// Create the thread
			CWinThread* pThread1 = AfxBeginThread(Thread1, pDlg);
			if (pThread1 == NULL) AfxMessageBox(L"pThread1 Create Error");

			CWinThread* pThread2 = AfxBeginThread(Thread2, pDlg);
			if (pThread2 == NULL) AfxMessageBox(L"pThread2 Create Error");

			HANDLE hThreads[2] = { pThread1->m_hThread, pThread2->m_hThread };

			// Wait for both threads to complete
			DWORD dwThreadResult = WaitForMultipleObjects(2, hThreads, TRUE, INFINITE);
			if (dwThreadResult == WAIT_FAILED)
			{
				AfxMessageBox(L"Error waiting for threads to complete");
			}

			// Close the thread handles
			CloseHandle(pThread1->m_hThread);
			CloseHandle(pThread2->m_hThread);
			
			pDlg->offsetCur++;
			pDlg->offsetVib++;

			//// 데이터 전달
			//vector<double> v_cur = pDlg->cur;
			//vector<double> v_vib = pDlg->vib;

			//vector<vector<double>> v_cur_vib;
			//v_cur_vib.push_back(v_cur);
			//v_cur_vib.push_back(v_vib);

			//pDlg->SendChartUpdateMessage_Robot(v_cur_vib);
			//// 데이터 전달

			CStringA jsonData;
			jsonData = prepareData(ROBOT, pDlg);
			wstring endpoint = SendPostRequest(ROBOT);
			CStringA result = pDlg->winHttp(jsonData, endpoint, 5001);
			vector<int> parse = pDlg->parsing_robot(result);
			 
			CString notice;

			if (parse.size() >= 2) {
				outCur = false, outVib = false;

				if (parse[0] == 1)
				{
					notice = L"전류 이상";
					outCur = true;
					pDlg->PostMessage(WM_NOTICE_LIST, (WPARAM)new CString(notice));
				}
				else
				{
					notice = L"전류 정상";
					//outlier++;
					pDlg->PostMessage(WM_NOTICE_LIST, (WPARAM)new CString(notice));
				}

				if (parse[1] == 1)
				{
					notice = L"진동 이상";
					outVib = true;
					pDlg->PostMessage(WM_NOTICE_LIST, (WPARAM)new CString(notice));
				}
				else
				{
					notice = L"진동 정상";
					//outlier++;
					pDlg->PostMessage(WM_NOTICE_LIST, (WPARAM)new CString(notice));
				}
				if (outCur == true || outVib == true) {
					outlier++;
				}
			}

			pDlg->PostMessage(WM_NOTICE_ROBOT, (WPARAM)new CString(notice)); // CString 동적 할당 없이 수정
		}
	}

	// 스레드 종료 후 이벤트 초기화
	pDlg->m_eventRobotThread.ResetEvent();

	return 0;
}

UINT CSF5MFCAIPOPDlg::PlasticThread(LPVOID pParam)
{
	CSF5MFCAIPOPDlg* pDlg = (CSF5MFCAIPOPDlg*)pParam;

	// 대기 상태에서 이벤트를 기다립니다.
	DWORD dwWaitResult = WaitForSingleObject(pDlg->m_eventPlasticThread, INFINITE);

	// 이벤트가 설정되면 다음 작업을 수행합니다.
	if (dwWaitResult == WAIT_OBJECT_0)
	{
		CThreadTest::Thread_DB_Wait_Plastic();

		CStringA jsonData;
		jsonData = prepareData(PLASTIC, pDlg);

		wstring endpoint = SendPostRequest(PLASTIC);

		CStringA result = pDlg->winHttp(jsonData, endpoint, 5002);

		vector<int> parse = pDlg->parsing_plastic(result);

		CString notice;

		if (parse.size() >= 1) {
			if (parse[0] == 1)
			{
				notice = L"플라스틱 이상";
				pDlg->PostMessage(WM_NOTICE_LIST, (WPARAM)new CString(notice));
			}
			else
			{
				notice = L"플라스틱 정상";
				pDlg->PostMessage(WM_NOTICE_LIST, (WPARAM)new CString(notice));
			}
		}

		pDlg->PostMessage(WM_NOTICE_PLASTIC, (WPARAM)new CString(notice)); // CString 동적 할당 없이 수정
	}

	// 스레드 종료 후 이벤트 초기화
	pDlg->m_eventPlasticThread.ResetEvent();

	return 0;
}

UINT CSF5MFCAIPOPDlg::HeatThread(LPVOID pParam)
{
	CSF5MFCAIPOPDlg* pDlg = (CSF5MFCAIPOPDlg*)pParam;

	// 대기 상태에서 이벤트를 기다립니다.
	DWORD dwWaitResult = WaitForSingleObject(pDlg->m_eventHeatThread, INFINITE);

	// 이벤트가 설정되면 다음 작업을 수행합니다.
	if (dwWaitResult == WAIT_OBJECT_0)
	{
		CThreadTest::Thread_DB_Wait_Heat();

		CStringA jsonData;
		jsonData = prepareData(HEAT, pDlg);

		wstring endpoint = SendPostRequest(HEAT);

		CStringA result = pDlg->winHttp(jsonData, endpoint, 5003);

		vector<int> parse = pDlg->parsing_heat(result);

		CString notice;

		if (parse.size() >= 1) {
			if (parse[0] == 1)
			{
				notice = L"열처리 이상";
				pDlg->PostMessage(WM_NOTICE_LIST, (WPARAM)new CString(notice));
			}
			else
			{
				notice = L"열처리 정상";
				pDlg->PostMessage(WM_NOTICE_LIST, (WPARAM)new CString(notice));
			}
		}

		pDlg->PostMessage(WM_NOTICE_HEAT, (WPARAM)new CString(notice)); // CString 동적 할당 없이 수정
	}

	// 스레드 종료 후 이벤트 초기화
	pDlg->m_eventHeatThread.ResetEvent();

	return 0;
}



void CSF5MFCAIPOPDlg::OnBnClickedButtonRobot()
{
	// 이전 스레드 핸들이 있는 경우 종료
	if (m_hRobotThread != nullptr)
	{
		::CloseHandle(m_hRobotThread);
		m_hRobotThread = nullptr;
	}

	// 스레드 핸들 초기화
	m_hRobotThread = (HANDLE)_beginthreadex(NULL, 0, &RobotThread, this, 0, NULL);
	if (m_hRobotThread == nullptr)
	{
		AfxMessageBox(_T("Error creating robot thread."));
		return;
	}

	// 이벤트 설정하여 스레드를 깨움
	m_eventRobotThread.SetEvent();
}

void CSF5MFCAIPOPDlg::OnBnClickedButtonPlastic()
{
	// 이전 스레드 핸들이 있는 경우 종료
	if (m_hPlasticThread != nullptr)
	{
		::CloseHandle(m_hPlasticThread);
		m_hPlasticThread = nullptr;
	}

	// 스레드 핸들 초기화
	m_hPlasticThread = (HANDLE)_beginthreadex(NULL, 0, &PlasticThread, this, 0, NULL);
	if (m_hPlasticThread == nullptr)
	{
		AfxMessageBox(_T("Error creating plastic thread."));
		return;
	}

	// 이벤트 설정하여 스레드를 깨움
	m_eventPlasticThread.SetEvent();
}

void CSF5MFCAIPOPDlg::OnBnClickedButtonHeat()
{
	// 이전 스레드 핸들이 있는 경우 종료
	if (m_hHeatThread != nullptr)
	{
		::CloseHandle(m_hHeatThread);
		m_hHeatThread = nullptr;
	}

	// 스레드 핸들 초기화
	m_hHeatThread = (HANDLE)_beginthreadex(NULL, 0, &HeatThread, this, 0, NULL);
	if (m_hHeatThread == nullptr)
	{
		AfxMessageBox(_T("Error creating heat thread."));
		return;
	}

	// 이벤트 설정하여 스레드를 깨움
	m_eventHeatThread.SetEvent();
}



LRESULT CSF5MFCAIPOPDlg::OnNoticeRobotError(WPARAM wParam, LPARAM lParam)
{
	CString* pNotice = (CString*)wParam;

	if (pNotice)
	{
		SetDlgItemText(IDC_STATIC_ROBOT_NOTICE, *pNotice);
		delete pNotice; // 메모리 해제
	}
	else
	{
		SetDlgItemText(IDC_STATIC_ROBOT_NOTICE, L"에러");
	}

	return 0;
}

LRESULT CSF5MFCAIPOPDlg::OnNoticePlasticError(WPARAM wParam, LPARAM lParam)
{
	CString* pNotice = (CString*)wParam;

	if (pNotice)
	{
		SetDlgItemText(IDC_STATIC_PLASTIC_NOTICE, *pNotice);
		delete pNotice; // 메모리 해제
	}
	else
	{
		SetDlgItemText(IDC_STATIC_PLASTIC_NOTICE, L"에러");
	}

	return 0;
}

LRESULT CSF5MFCAIPOPDlg::OnNoticeHeatError(WPARAM wParam, LPARAM lParam)
{
	CString* pNotice = (CString*)wParam;

	if (pNotice)
	{
		SetDlgItemText(IDC_STATIC_HEAT_NOTICE, *pNotice);
		delete pNotice; // 메모리 해제
	}
	else
	{
		SetDlgItemText(IDC_STATIC_HEAT_NOTICE, L"에러");
	}

	return 0;
}


LRESULT CSF5MFCAIPOPDlg::OnNoticeList(WPARAM wParam, LPARAM lParam)
{
	const CString& notice = *((CString*)wParam);

	CString strIndex;
	strIndex.Format(_T("%d"), listIndex++);

	CTime currentTime = CTime::GetCurrentTime();
	CString strTime = currentTime.Format(L"%Y-%m-%d %H:%M:%S");

	CString result1, result2, result3;

	if (notice.Compare(L"플라스틱 이상") == 0)
	{
		result1 = L"경보 발생";
		result2 = L"소성가공";
		result3 = L"설비 이상";
	}
	else if (notice.Compare(L"플라스틱 정상") == 0)
	{
		result1 = L"정상";
		result2 = L"소성가공";
		result3 = L"설비 정상";
	}
	else if (notice.Compare(L"전류 이상") == 0)
	{
		result1 = L"경보 발생";
		result2 = L"로봇 용접";
		result3 = L"진동 이상";
	}
	else if (notice.Compare(L"진동 이상") == 0)
	{
		result1 = L"경보 발생";
		result2 = L"로봇 용접";
		result3 = L"진동 이상";
	}
	else if (notice.Compare(L"전류 정상") == 0)
	{
		result1 = L"정상";
		result2 = L"로봇 용접";
		result3 = L"전류 정상";
	}
	else if (notice.Compare(L"진동 정상") == 0)
	{
		result1 = L"정상";
		result2 = L"로봇 용접";
		result3 = L"진동 정상";
	}
	else if (notice.Compare(L"열처리 이상") == 0)
	{
		result1 = L"경보 발생";
		result2 = L"열처리";
		result3 = L"설비 이상";
	}
	else if (notice.Compare(L"열처리 정상") == 0)
	{
		result1 = L"정상";
		result2 = L"열처리";
		result3 = L"설비 정상";
	}


	if (!result1.IsEmpty() && !result2.IsEmpty() && !result3.IsEmpty())
	{
		m_listCtrl.InsertItem(0, CString(strIndex));
		m_listCtrl.SetItem(0, 1, LVIF_TEXT, CString(strTime), 0, 0, 0, NULL);
		m_listCtrl.SetItem(0, 2, LVIF_TEXT, CString(result1), 0, 0, 0, NULL);
		m_listCtrl.SetItem(0, 3, LVIF_TEXT, CString(result2), 0, 0, 0, NULL);
		m_listCtrl.SetItem(0, 4, LVIF_TEXT, CString(result3), 0, 0, 0, NULL);
	}

	delete (CString*)wParam; // 메모리 해제
	return 0;
}



BOOL CSF5MFCAIPOPDlg::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	GetClientRect(rect);

	pDC->FillSolidRect(rect, RGB(53, 58, 64));
	return TRUE;

	return CDialogEx::OnEraseBkgnd(pDC);
}

HBRUSH CSF5MFCAIPOPDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here
	if (pWnd->GetDlgCtrlID() == IDC_STATIC_CURRENT_TIME
		|| pWnd->GetDlgCtrlID() == IDC_STATIC_TITLE
		|| pWnd->GetDlgCtrlID() == IDC_STATIC_HEAT_NOTICE
		|| pWnd->GetDlgCtrlID() == IDC_STATIC_PLASTIC_NOTICE
		|| pWnd->GetDlgCtrlID() == IDC_STATIC_ROBOT_NOTICE)
	{
		// Set text color to white
		pDC->SetTextColor(RGB(255, 255, 255));

		// Transparent background
		pDC->SetBkMode(TRANSPARENT);

		// Create a solid brush with background color
		CBrush brush(RGB(64, 70, 76));

		// Get the control's rectangle
		CRect rect;
		pWnd->GetClientRect(&rect);

		// Fill the background with the solid brush
		pDC->FillRect(&rect, &brush);

		// Return a brush that is not used
		return (HBRUSH)GetStockObject(NULL_BRUSH);
	}

	// TODO:  Return a different brush if the default is not desired
	return hbr;
}


void CSF5MFCAIPOPDlg::OnBnClickedButtonRobotNotice()
{
	pChartDialog_Robot->ShowWindow(SW_SHOW);
}


void CSF5MFCAIPOPDlg::OnBnClickedButtonPlasticNotice()
{
	pChartDialog_Plastic->ShowWindow(SW_SHOW);
}


void CSF5MFCAIPOPDlg::OnBnClickedButtonHeatNotice()
{
	pChartDialog_Heat->ShowWindow(SW_SHOW);
}


void CSF5MFCAIPOPDlg::SendChartUpdateMessage_Robot(vector<vector<double>> newValue)
{
	if (pChartDialog_Robot != nullptr)
	{
		pChartDialog_Robot->SendMessage(WM_UPDATE_CHART1, reinterpret_cast<WPARAM>(&newValue), 0);
	}
}

void CSF5MFCAIPOPDlg::SendChartUpdateMessage_Plastic(vector<vector<double>> newValue)
{
	if (pChartDialog_Plastic != nullptr)
	{
		pChartDialog_Plastic->SendMessage(WM_UPDATE_CHART2, reinterpret_cast<WPARAM>(&newValue), 0);
	}
}

void CSF5MFCAIPOPDlg::SendChartUpdateMessage_Heat(vector<vector<double>> newValue)
{
	if (pChartDialog_Heat != nullptr)
	{
		pChartDialog_Heat->SendMessage(WM_UPDATE_CHART3, reinterpret_cast<WPARAM>(&newValue), 0);
	}
}