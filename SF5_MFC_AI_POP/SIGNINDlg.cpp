// SIGNINDlg.cpp : implementation file
//

#include "pch.h"
#include "SF5_MFC_AI_POP.h"
#include "afxdialogex.h"
#include "SIGNINDlg.h"


// SIGNINDlg dialog

IMPLEMENT_DYNAMIC(SIGNINDlg, CDialogEx)

SIGNINDlg::SIGNINDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SIGNIN_DIALOG, pParent)
{

}

SIGNINDlg::~SIGNINDlg()
{
	if (m_hCursor != nullptr)
		::DestroyCursor(m_hCursor);
}

void SIGNINDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_CODE, m_userCode);
	DDX_Text(pDX, IDC_EDIT_ID_SIGN, m_userID_sign);
	DDX_Text(pDX, IDC_EDIT_PW_SIGN, m_userPW_sign);
	DDX_Text(pDX, IDC_EDIT_RE_PW_SIGN, m_userRePW_sign);
}


BEGIN_MESSAGE_MAP(SIGNINDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &SIGNINDlg::OnBnClickedOk)
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_CTLCOLOR()
	ON_WM_SETCURSOR()
END_MESSAGE_MAP()


// SIGNINDlg message handlers


void SIGNINDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here

	UpdateData(TRUE);  // 다이얼로그 컨트롤 값 업데이트
	
	CString code = _T("0000");
	
	if (m_userCode == "")
	{
		MessageBox(_T("관리자 코드를 입력해주세요."), _T("오류"), MB_OK | MB_ICONERROR);
	}
	else if (m_userCode != code)
	{
		MessageBox(_T("관리자 코드가 아닙니다."), _T("오류"), MB_OK | MB_ICONERROR);
	}
	else if (m_userID_sign == "")
	{
		MessageBox(_T("아이디를 입력해주세요."), _T("오류"), MB_OK | MB_ICONERROR);
	}
	else if (m_userPW_sign == "")
	{
		MessageBox(_T("비밀번호를 입력해주세요."), _T("오류"), MB_OK | MB_ICONERROR);
	}
	else if (m_userRePW_sign != m_userPW_sign)
	{
		MessageBox(_T("비밀번호가 일치하지 않습니다."), _T("오류"), MB_OK | MB_ICONERROR);
	}
	else
	{
		// MySQLConnector 객체 생성
		MySQL_Connector mysql;

		string id = string(CT2CA(m_userID_sign));
		string pw = string(CT2CA(m_userPW_sign));

		// 데이터베이스 서버 연결
		if (mysql.connect("tcp://127.0.0.1:3306", "Nia", "0000", "pop"))
		{
			// 로그인 처리
			if (mysql.newAdmin(id, pw)) 
			{
				MessageBox(_T("새로운 관리자님을 환영합니다:)"), _T("알림"), MB_OK | MB_ICONINFORMATION);
				CDialogEx::OnOK();
			}
			else {
				MessageBox(_T("관리자 계정 생성 실패"), _T("오류"), MB_OK | MB_ICONERROR);
			}
		}
		else {
			MessageBox(_T("데이터베이스 연결 실패"), _T("오류"), MB_OK | MB_ICONERROR);
		}
	}
}


BOOL SIGNINDlg::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	GetClientRect(rect);

	pDC->FillSolidRect(rect, RGB(53, 58, 64));
	return TRUE;

	return CDialogEx::OnEraseBkgnd(pDC);
}


void SIGNINDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	SendMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));

	CDialogEx::OnLButtonDown(nFlags, point);
}


BOOL SIGNINDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	m_hCursor = ::LoadCursor(NULL, IDC_HAND);  // Example: IDC_HAND is a standard hand cursor

	m_font.CreatePointFont(170, L"나눔스퀘어");
	GetDlgItem(IDC_STATIC_TEXT2)->SetFont(&m_font);

	pBtnConfirm = new CMyButton();
	pBtnCancel = new CMyButton();

	CRect rect1, rect2;
	GetDlgItem(IDOK)->GetWindowRect(&rect1);
	GetDlgItem(IDCANCEL)->GetWindowRect(&rect2);

	ScreenToClient(&rect1);
	ScreenToClient(&rect2);


	pBtnConfirm->Create(_T("확인"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_CENTER | BS_VCENTER,
		rect1, this, IDOK);
	pBtnCancel->Create(_T("X"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_CENTER | BS_VCENTER,
		rect2, this, IDCANCEL);

	GetDlgItem(IDOK)->ShowWindow(SW_HIDE);
	GetDlgItem(IDCANCEL)->ShowWindow(SW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


HBRUSH SIGNINDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// IDC_STATIC_TEXT1에 대해 배경을 투명하게 설정
	if (pWnd->GetDlgCtrlID() == IDC_STATIC_TEXT2
		|| pWnd->GetDlgCtrlID() == IDC_STATIC_ADMIN_CODE
		|| pWnd->GetDlgCtrlID() == IDC_STATIC_IN_ID
		|| pWnd->GetDlgCtrlID() == IDC_STATIC_IN_PW
		|| pWnd->GetDlgCtrlID() == IDC_STATIC_IN_PW_RE)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(255, 255, 255));  // 흰색으로 글자 색 설정
		return (HBRUSH)GetStockObject(NULL_BRUSH);  // 배경 브러시를 NULL로 설정하여 투명하게 만듦
	}

	// TODO:  Return a different brush if the default is not desired
	return hbr;
}


BOOL SIGNINDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: Add your message handler code here and/or call default
	if (pWnd == pBtnConfirm || pWnd == pBtnCancel)
	{
		::SetCursor(m_hCursor);
		return TRUE;
	}

	return CDialogEx::OnSetCursor(pWnd, nHitTest, message);
}
