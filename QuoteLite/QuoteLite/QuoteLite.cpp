
// QuoteLite.cpp : �w�q���ε{�������O�欰�C
//

#include "stdafx.h"
#include "QuoteLite.h"
#include "QuoteLiteDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CQuoteLiteApp

BEGIN_MESSAGE_MAP(CQuoteLiteApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CQuoteLiteApp �غc

CQuoteLiteApp::CQuoteLiteApp()
{
	// TODO:  �b���[�J�غc�{���X�A
	// �N�Ҧ����n����l�]�w�[�J InitInstance ��
}


// �Ȧ����@�� CQuoteLiteApp ����

CQuoteLiteApp theApp;


// CQuoteLiteApp ��l�]�w

BOOL CQuoteLiteApp::InitInstance()
{
	CWinApp::InitInstance();


	// �إߴ߼h�޲z���A�H����ܤ���]�t
	// ����߼h���˵��δ߼h�M���˵�����C
	CShellManager *pShellManager = new CShellManager;

	// �Ұ� [Windows ���] ��ı�ƺ޲z���i�ҥ� MFC ��������D�D
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// �зǪ�l�]�w
	// �p�G�z���ϥγo�ǥ\��åB�Q���
	// �̫᧹�����i�����ɤj�p�A�z�i�H
	// �q�U�C�{���X�������ݭn����l�Ʊ`���A
	// �ܧ��x�s�]�w�Ȫ��n�����X
	// TODO:  �z���ӾA�׭ק惡�r��
	// (�Ҧp�A���q�W�٩β�´�W��)
	SetRegistryKey(_T("���� AppWizard �Ҳ��ͪ����ε{��"));

	CQuoteLiteDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	//if (nResponse == IDOK)
	//{
	//	// TODO:  �b����m��ϥ� [�T�w] �Ӱ���ϥι�ܤ����
	//	// �B�z���{���X
	//}
	//else if (nResponse == IDCANCEL)
	//{
	//	// TODO:  �b����m��ϥ� [����] �Ӱ���ϥι�ܤ����
	//	// �B�z���{���X
	//}
	//else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "ĵ�i: ��ܤ���إߥ��ѡA�]���A���ε{���N�~�פ�C\n");
		TRACE(traceAppMsg, 0, "ĵ�i: �p�G�z�n�b��ܤ���W�ϥ� MFC ����A�h�L�k #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS�C\n");
	}

	// �R���W���ҫإߪ��߼h�޲z���C
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// �]���w�g������ܤ���A�Ǧ^ FALSE�A�ҥH�ڭ̷|�������ε{���A
	// �ӫD���ܶ}�l���ε{�����T���C
	return FALSE;
}

