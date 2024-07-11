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
END_MESSAGE_MAP()


// SIGNINDlg message handlers


void SIGNINDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here

	UpdateData(TRUE);  // 다이얼로그 컨트롤 값 업데이트
	
	CString code = _T("0000");
	

	//CStringA strIdA(m_userID), strPwA(m_userPW);
	//string stdID(strIdA.GetBuffer()), stdPW(strPwA.GetBuffer());

	//// MySQLConnector 객체 생성
	//MySQL_Connector mysql;

	//// 데이터베이스 서버 연결
	//if (mysql.connect("tcp://192.168.1.245", "Nia", "0000", "pop"))
	//	//if (mysql.connect("tcp://127.0.0.1:3306", "user", "1234", "chatting_project")) // 수정
	//{
	//	// 로그인 처리
	//	if (mysql.login(stdID, stdPW)) {
	//		MessageBox(_T("로그인 성공"), _T("알림"), MB_OK | MB_ICONINFORMATION);
	//		// 로그인 성공 후 다음 작업 수행

	//		CDialogEx::OnOK();
	//	}
	//	else {
	//		MessageBox(_T("아이디 또는 비밀번호가 틀렸습니다."), _T("오류"), MB_OK | MB_ICONERROR);
	//	}
	//}
	//else {
	//	MessageBox(_T("데이터베이스 연결 실패"), _T("오류"), MB_OK | MB_ICONERROR);
	//}

	CDialogEx::OnOK();
}
