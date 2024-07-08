
// SF5_MFC_AI_POPDlg.h : header file
//

#pragma once


// CSF5MFCAIPOPDlg dialog
class CSF5MFCAIPOPDlg : public CDialogEx
{
// Construction
public:
	CSF5MFCAIPOPDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SF5_MFC_AI_POP_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
};
