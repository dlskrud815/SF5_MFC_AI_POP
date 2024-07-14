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

	pDC->FillSolidRect(rect, RGB(67, 76, 84));	// 변경하고 싶은 색상 RGB색
	return TRUE;

	return CDialogEx::OnEraseBkgnd(pDC);
}
