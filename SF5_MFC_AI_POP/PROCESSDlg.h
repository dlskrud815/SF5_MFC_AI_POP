#pragma once
#include "afxdialogex.h"


// PROCESSDlg dialog

class PROCESSDlg : public CDialogEx
{
	DECLARE_DYNAMIC(PROCESSDlg)

public:
	PROCESSDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~PROCESSDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROCESS_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
