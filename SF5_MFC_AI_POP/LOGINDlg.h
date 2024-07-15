#pragma once
#include "afxdialogex.h"
#include "MySQL_Connector.h"
#include <string>

using namespace std;

// LOGINDlg dialog

class LOGINDlg : public CDialogEx
{
	DECLARE_DYNAMIC(LOGINDlg)

public:
	LOGINDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~LOGINDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LOGIN_DIALOG };
#endif

private:
	CString m_userID;
	CString m_userPW;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButtonSignin();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
