﻿
// 1115_Line_01.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CMy1115Line01App:
// See 1115_Line_01.cpp for the implementation of this class
//

class CMy1115Line01App : public CWinApp
{
public:
	CMy1115Line01App();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CMy1115Line01App theApp;
