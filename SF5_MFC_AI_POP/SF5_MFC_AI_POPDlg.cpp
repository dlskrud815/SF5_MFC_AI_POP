
// SF5_MFC_AI_POPDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "SF5_MFC_AI_POP.h"
#include "SF5_MFC_AI_POPDlg.h"
#include "afxdialogex.h"
#include "LOGINDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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
}

BEGIN_MESSAGE_MAP(CSF5MFCAIPOPDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON1, &CSF5MFCAIPOPDlg::OnBnClickedButton1)
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

	//리스트 칼럼 넣기
	m_listCtrl.InsertColumn(0, L"메시지", LVCFMT_LEFT, 400, -1);
	m_listCtrl.InsertColumn(0, L"설비", LVCFMT_LEFT, 200, -1);
	m_listCtrl.InsertColumn(0, L"상태", LVCFMT_LEFT, 100, -1);
	m_listCtrl.InsertColumn(0, L"시간", LVCFMT_LEFT, 200, -1);
	m_listCtrl.InsertColumn(0, L"순번", LVCFMT_LEFT, 50, -1);

	CWinThread* p1 = NULL;
	p1 = AfxBeginThread(ThreadTest, this);
	//새 스레드 시작 - 현재 객체를 스레드 함수에 전달

	if (p1 == NULL) //스레드 생성 실패 시
	{
		AfxMessageBox(L"Error");
	}

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

	const int listHeight = 500;

	int xPosCancel = cx - buttonWidth - margin;
	int xPosOK = xPosCancel - buttonWidth - margin;
	int yPos = margin; //- buttonHeight - margin;

	int yPosListControl = cy - listHeight - margin;

	CWnd* pOKButton = GetDlgItem(IDOK);
	CWnd* pCancelButton = GetDlgItem(IDCANCEL);
	CWnd* pListControl = GetDlgItem(IDC_LIST_ERROR);


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
}


void CSF5MFCAIPOPDlg::OnBnClickedButton1()
{
	// Create and start the threads
	CWinThread* pThreadCur = AfxBeginThread(Thread_DB_Get_Cur, this);
	CWinThread* pThreadVib = AfxBeginThread(Thread_DB_Get_Vib, this);

	// Create and start the wait thread, passing the thread handles
	HANDLE hThreads[2] = { pThreadCur->m_hThread, pThreadVib->m_hThread };
	AfxBeginThread(Thread_DB_Wait, hThreads);
}


UINT CSF5MFCAIPOPDlg::ThreadTest(LPVOID _mothod)
{
	// pParam을 CSF5MFCAIPOPDlg 객체로 변환
	CSF5MFCAIPOPDlg* pDlg = (CSF5MFCAIPOPDlg*)_mothod;

	// MySQLConnector 객체 생성
	MySQL_Connector mysql;
	int i = 0;

	// 데이터베이스 서버 연결
	if (mysql.connect("tcp://127.0.0.1:3306", "user", "1234", "chatting_project")) // 수정
	{
		while (mysql.getData(i)) // db에 다음행이 있을 때까지
		{
			CString message = mysql.getMessage();
			CString strIndex;
			strIndex.Format(_T("%d"), i);  // i 값을 문자열로 변환

			pDlg->m_listCtrl.InsertItem(0, strIndex);
			pDlg->m_listCtrl.SetItem(0, 1, LVIF_TEXT, message, 0, 0, 0, NULL);

			Sleep(1000); //1초 대기
			i++;
		}
	}
	else {
		pDlg->MessageBox(_T("데이터베이스 연결 실패"), _T("오류"), MB_OK | MB_ICONERROR);
	}

	//스레드 함수 종료 시
	return 0;
}


UINT CSF5MFCAIPOPDlg::Thread_DB_Wait(LPVOID _mothod)
{
	HANDLE* hThreads = (HANDLE*)_mothod;

	// Wait for both threads to finish
	WaitForMultipleObjects(2, hThreads, TRUE, INFINITE);

	// Display the message box after both threads are done
	AfxMessageBox(_T("데이터베이스 양측 수신 완료"));

	return 0;
}


UINT CSF5MFCAIPOPDlg::Thread_DB_Get_Cur(LPVOID _mothod)
{
	CSF5MFCAIPOPDlg* pDlg = (CSF5MFCAIPOPDlg*)_mothod; // pParam을 CSF5MFCAIPOPDlg 객체로 변환	
	MySQL_Connector mysql; // MySQLConnector 객체 생성

	// 데이터베이스 서버 연결
	if (mysql.connect("tcp://127.0.0.1:3306", "user", "1234", "chatting_project")) // 수정
	{ 
		Sleep(2000);

		if (mysql.getID()) 
		{
			CString message = mysql.getMessage();
			pDlg->m_test1.SetWindowText(message);
		}
		else 
		{
			pDlg->MessageBox(_T("데이터를 받아오지 못 했습니다."), _T("오류"), MB_OK | MB_ICONERROR);
		}
	}
	else {
		pDlg->MessageBox(_T("데이터베이스 연결 실패"), _T("오류"), MB_OK | MB_ICONERROR);
	}

	//스레드 함수 종료 시
	return 0;
}


UINT CSF5MFCAIPOPDlg::Thread_DB_Get_Vib(LPVOID _mothod)
{
	CSF5MFCAIPOPDlg* pDlg = (CSF5MFCAIPOPDlg*)_mothod; // pParam을 CSF5MFCAIPOPDlg 객체로 변환	
	MySQL_Connector mysql; // MySQLConnector 객체 생성

	// 데이터베이스 서버 연결
	if (mysql.connect("tcp://127.0.0.1:3306", "user", "1234", "chatting_project")) // 수정
	{
		Sleep(5000);

		if (mysql.getFrom()) 
		{
			CString message = mysql.getMessage();
			pDlg->m_test2.SetWindowText(message);
		}
		else 
		{
			pDlg->MessageBox(_T("데이터를 받아오지 못 했습니다."), _T("오류"), MB_OK | MB_ICONERROR);
		}	
	}
	else {
		pDlg->MessageBox(_T("데이터베이스 연결 실패"), _T("오류"), MB_OK | MB_ICONERROR);
	}

	//스레드 함수 종료 시
	return 0;
}
