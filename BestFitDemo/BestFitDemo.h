
// BestFitDemo.h : main header file for the BestFitDemo application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
#include <vector>

#define USERMESSAGE_ADDPOINT WM_USER + 1000
#define USERMESSAGE_REMPOINT WM_USER + 1001

typedef std::pair<std::vector<double>::const_iterator, std::vector<double>::const_iterator> IteratorRange;

// CBestFitDemoApp:
// See BestFitDemo.cpp for the implementation of this class
//

class CBestFitDemoApp : public CWinAppEx
{
public:
	CBestFitDemoApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CBestFitDemoApp theApp;
