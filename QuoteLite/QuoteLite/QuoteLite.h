
// QuoteLite.h : PROJECT_NAME ���ε{�����D�n���Y��
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�� PCH �]�t���ɮ׫e���]�t 'stdafx.h'"
#endif

#include "resource.h"		// �D�n�Ÿ�


// CQuoteLiteApp: 
// �аѾ\��@�����O�� QuoteLite.cpp
//

class CQuoteLiteApp : public CWinApp
{
public:
	CQuoteLiteApp();

// �мg
public:
	virtual BOOL InitInstance();

// �{���X��@

	DECLARE_MESSAGE_MAP()
};

extern CQuoteLiteApp theApp;