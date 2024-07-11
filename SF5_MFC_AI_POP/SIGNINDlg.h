#pragma once
#include "afxdialogex.h"


// SIGNINDlg dialog

class SIGNINDlg : public CDialogEx
{
	DECLARE_DYNAMIC(SIGNINDlg)

public:
	SIGNINDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~SIGNINDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SIGNIN_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
