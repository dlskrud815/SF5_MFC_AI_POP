
// SF5_MFC_AI_POP.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CSF5MFCAIPOPApp:
// See SF5_MFC_AI_POP.cpp for the implementation of this class
//

class CSF5MFCAIPOPApp : public CWinApp
{
public:
	CSF5MFCAIPOPApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CSF5MFCAIPOPApp theApp;
