#pragma once
#include "afxdialogex.h"
#include "MySQL_Connector.h"
#include <string>
#include "MyButton.h"

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

private:
	CString m_userCode, m_userID_sign, m_userPW_sign, m_userRePW_sign;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CFont m_font;

	afx_msg void OnBnClickedOk();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	CMyButton* pBtnConfirm, * pBtnCancel;

	HCURSOR m_hCursor;
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
};
