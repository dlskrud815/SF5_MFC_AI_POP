﻿
// SF5_MFC_AI_POPDlg.h : header file
//

#pragma once

#include "MySQL_Connector.h"
#include "CustomStatic.h" // CCustomStatic 클래스 포함

using namespace std;

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
	afx_msg void OnSize(UINT nType, int cx, int cy);

	//CImage back;
	COLORREF m_backgroundColor;

	// 타이머 ID
	//static const UINT_PTR m_nTimerID = 0;
	//void UpdateCurrentTime();
	//afx_msg void OnTimer(UINT_PTR nIDEvent);


	CStringA winHttp(CStringA jsonData, wstring endpoint, int port);

	vector<int> parsing_robot(CStringA response);
	vector<int> parsing_plastic(CStringA response);
	vector<int> parsing_heat(CStringA response);

	static wstring StringToWideString(const string& str);
	static CStringA prepareData(tName process, LPVOID pParam);
	static wstring SendPostRequest(tName process);

	static CCriticalSection critSect;

	CCustomStatic m_staticHeader, m_staticNotice1, m_staticNotice2, m_staticNotice3;
	CListCtrl m_listCtrl;

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedButtonRobot();
	afx_msg void OnBnClickedButtonPlastic();
	afx_msg void OnBnClickedButtonHeat();

	static int listIndex;
	afx_msg LRESULT OnNoticeList(WPARAM wParam, LPARAM lParam);


	static UINT MainThread(LPVOID _mothod);

	static UINT TimeUpdateThread(LPVOID _mothod);
	afx_msg LRESULT OnUpdateTime(WPARAM wParam, LPARAM lParam);
	
	HANDLE m_hPlasticThread = nullptr; // 소성가공 스레드 핸들 추가
	HANDLE m_hRobotThread = nullptr; // 로봇용접 스레드 핸들 추가
	HANDLE m_hHeatThread = nullptr; // 열처리 스레드 핸들 추가


	static UINT Thread_DB_Get_Cur(LPVOID pParam);
	static UINT Thread_DB_Get_Vib(LPVOID pParam);

	CEvent m_eventRobotThread; // 이벤트 객체 선언
	static UINT RobotThread(LPVOID pParam); // 대기 스레드 함수 선언
	afx_msg LRESULT OnNoticeRobotError(WPARAM wParam, LPARAM lParam);

	
	static UINT Thread_DB_Get_Plastic(LPVOID pParam);

	CEvent m_eventPlasticThread; // 이벤트 객체 선언
	static UINT PlasticThread(LPVOID pParam); // 대기 스레드 함수 선언
	afx_msg LRESULT OnNoticePlasticError(WPARAM wParam, LPARAM lParam);

	static UINT Thread_DB_Get_Heat(LPVOID pParam);

	CEvent m_eventHeatThread; // 이벤트 객체 선언
	static UINT HeatThread(LPVOID pParam); // 대기 스레드 함수 선언
	afx_msg LRESULT OnNoticeHeatError(WPARAM wParam, LPARAM lParam);
	
	
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	static string vectorToString(vector<double> vec);

	int offsetCur = 1, offsetVib = 1, offsetV0 = 1, offsetV1 = 1, offsetC1 = 1, offsetHeat = 1;

	string strCur, strVib, strV0, strV1, strC1;
	vector<string> strHeat;
};