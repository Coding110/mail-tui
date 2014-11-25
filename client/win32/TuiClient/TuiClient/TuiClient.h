
// TuiClient.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CTuiClientApp:
// See TuiClient.cpp for the implementation of this class
//

class CTuiClientApp : public CWinApp
{
public:
	CTuiClientApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CTuiClientApp theApp;