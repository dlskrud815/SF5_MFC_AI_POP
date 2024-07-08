// LOGINDlg.cpp : implementation file
//

#include "pch.h"
#include "SF5_MFC_AI_POP.h"
#include "afxdialogex.h"
#include "LOGINDlg.h"


// LOGINDlg dialog

IMPLEMENT_DYNAMIC(LOGINDlg, CDialogEx)

LOGINDlg::LOGINDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LOGIN_DIALOG, pParent)
{

}

LOGINDlg::~LOGINDlg()
{
}

void LOGINDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ID, m_userID);
	DDX_Text(pDX, IDC_EDIT_PW, m_userPW);
}


BEGIN_MESSAGE_MAP(LOGINDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &LOGINDlg::OnBnClickedOk)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// LOGINDlg message handlers


void LOGINDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//로그인

	UpdateData(TRUE);  // 다이얼로그 컨트롤 값 업데이트

	CStringA strIdA(m_userID), strPwA(m_userPW);
	string stdID(strIdA.GetBuffer()), stdPW(strPwA.GetBuffer());

	// MySQLConnector 객체 생성
	MySQL_Connector mysql;

	// 데이터베이스 서버 연결
	if (mysql.connect("tcp://127.0.0.1:3306", "user", "1234")) {
		// 로그인 처리
		if (mysql.login(stdID, stdPW)) {
			MessageBox(_T("로그인 성공"), _T("알림"), MB_OK | MB_ICONINFORMATION);
			// 로그인 성공 후 다음 작업 수행

			CDialogEx::OnOK();
		}
		else {
			MessageBox(_T("아이디 또는 비밀번호가 틀렸습니다."), _T("오류"), MB_OK | MB_ICONERROR);
		}
	}
	else {
		MessageBox(_T("데이터베이스 연결 실패"), _T("오류"), MB_OK | MB_ICONERROR);
	}
}


void LOGINDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	//다이얼로그 드래그
	SendMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));

	CDialogEx::OnLButtonDown(nFlags, point);
}
