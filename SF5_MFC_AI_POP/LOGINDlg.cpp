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
}


BEGIN_MESSAGE_MAP(LOGINDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &LOGINDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// LOGINDlg message handlers


void LOGINDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}
