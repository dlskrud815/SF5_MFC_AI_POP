#pragma once
#include "afxdialogex.h"


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

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
