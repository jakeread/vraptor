/////////////////////////////////////////////////////////////////////////////
// VRaptorApp.h : main header file for the VRaptor DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "Resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CVRaptorApp
// See VRaptorApp.cpp for the implementation of this class
//

class CVRaptorApp : public CWinApp
{
public:
	CVRaptorApp();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};
