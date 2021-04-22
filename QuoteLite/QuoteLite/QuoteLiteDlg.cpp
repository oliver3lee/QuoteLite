// QuoteLiteDlg.cpp : ��@��
//

#include "stdafx.h"
#include "QuoteLite.h"
#include "QuoteLiteDlg.h"
#include "afxdialogex.h"
#include "windows.h"
#include <vector>
#include <thread>
#include <algorithm>
#include <mutex>

#define WM_READFILEFINISHONE WM_USER + 102
#define WM_READFILEFINISHTWO WM_USER + 103
#define WM_READFILEERRORMESSAGE WM_USER + 104

#define ShowReadFileProgressPartOne 1
#define ShowReadFileProgressPartTwo 2

#ifdef _DEBUG
#define new DEBUG_NEW
#define CLASS_DECLSPEC    __declspec(dllimport)
#endif


// �� App About �ϥ� CAboutDlg ��ܤ��

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ܤ�����
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �䴩

// �{���X��@
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CQuoteLiteDlg ��ܤ��



CQuoteLiteDlg::CQuoteLiteDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CQuoteLiteDlg::IDD, pParent)
	, m_bAlreadyRead(false)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	//m_SymbolsManager.QuoteInintialize("C:\\Users\\F62R\\Desktop\\Dat\\*.csv");
	m_SymbolsManager.QuoteInintialize(".\\Dat\\*.csv");
}

void CQuoteLiteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST5, m_TradeDetail);//
	DDX_Control(pDX, IDC_LIST_ITEM, m_ListItem);//
	DDX_Control(pDX, IDC_STATIC_PROGRESS, m_Progress);//
	DDX_Control(pDX, IDC_STATIC_USETIME, m_UseTime);//
	DDX_Control(pDX, IDC_STATIC_SYMBOLUSETIME, m_SymbolUseTime);
	DDX_Control(pDX, IDC_LIST_PRICEOFVALUME, m_PriceOfValume);
	DDX_Control(pDX, IDC_TREE_FILEITEMNAME, m_FileItemName);
	DDX_Control(pDX, IDC_LIST_SYMBOLITEM, m_SimilarItem);
	DDX_Control(pDX, IDC_LISTERRORFILE, m_ErrorFile);
}

BEGIN_MESSAGE_MAP(CQuoteLiteDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_START, &CQuoteLiteDlg::OnBnClickedStart)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_FILEITEMNAME, &CQuoteLiteDlg::OnTvnSelchangedTreeFileitemname)
	ON_MESSAGE(WM_READFILEFINISHONE, &CQuoteLiteDlg::ReadFilePartOneFinish)
	ON_MESSAGE(WM_READFILEFINISHTWO, &CQuoteLiteDlg::ReadFilePartTwoFinish)
	ON_MESSAGE(WM_READFILEERRORMESSAGE, &CQuoteLiteDlg::ReadFileErrorMessage)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CQuoteLiteDlg �T���B�z�`��

BOOL CQuoteLiteDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �N [����...] �\���[�J�t�Υ\���C

	// IDM_ABOUTBOX �����b�t�ΩR�O�d�򤧤��C
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// �]�w����ܤ�����ϥܡC�����ε{�����D�������O��ܤ���ɡA
	// �ج[�|�۰ʱq�Ʀ��@�~
	SetIcon(m_hIcon, TRUE);			// �]�w�j�ϥ�
	SetIcon(m_hIcon, FALSE);		// �]�w�p�ϥ�

	// TODO:  �b���[�J�B�~����l�]�w
	InitializeDialogForm();

	return TRUE;  // �Ǧ^ TRUE�A���D�z�ﱱ��]�w�J�I
}

void  CQuoteLiteDlg::InitializeDialogForm()
{
	m_Progress.SetWindowText(_T("0/0 , 0/0"));

	m_ListItem.SetExtendedStyle(m_ListItem.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP);
	m_ListItem.InsertColumn(0, _T("ItemNo"), LVCFMT_LEFT, 150, 0);
	m_ListItem.InsertColumn(1, _T("ItemValue"), LVCFMT_LEFT, 150, 1);

	m_ListItem.InsertItem(0, _T("SymbolID"));
	m_ListItem.InsertItem(1, _T("Time"));
	m_ListItem.InsertItem(2, _T("Price"));
	m_ListItem.InsertItem(3, _T("Volume"));
	m_ListItem.InsertItem(4, _T("AVG_Price"));
	m_ListItem.InsertItem(5, _T("Total_Volume"));
	m_ListItem.InsertItem(6, _T("High"));
	m_ListItem.InsertItem(7, _T("Low"));
	m_ListItem.InsertItem(8, _T("LikeSymbolCnt"));

	m_TradeDetail.SetExtendedStyle(m_ListItem.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP);
	m_TradeDetail.InsertColumn(0, _T("Time"), LVCFMT_LEFT, 100, 0);
	m_TradeDetail.InsertColumn(1, _T("Price"), LVCFMT_LEFT, 100, 1);
	m_TradeDetail.InsertColumn(2, _T("Volume"), LVCFMT_LEFT, 100, 1);

	m_PriceOfValume.SetExtendedStyle(m_ListItem.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP);
	m_PriceOfValume.InsertColumn(0, _T("Price"), LVCFMT_LEFT, 100, 0);
	m_PriceOfValume.InsertColumn(1, _T("Volume"), LVCFMT_LEFT, 100, 1);

	m_SimilarItem.SetExtendedStyle(m_ListItem.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP);
	m_SimilarItem.InsertColumn(0, _T("SymbolID"), LVCFMT_LEFT, 110, 0);
	m_SimilarItem.InsertColumn(1, _T("AVG_Price"), LVCFMT_LEFT, 110, 1);
	m_SimilarItem.InsertColumn(2, _T("Total_Volume"), LVCFMT_LEFT, 110, 1);

	m_ErrorFile.SetExtendedStyle(m_ErrorFile.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP);
	m_ErrorFile.InsertColumn(0, _T("Error File"), LVCFMT_LEFT, 179, 0);
}

void CQuoteLiteDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �p�G�N�̤p�ƫ��s�[�J�z����ܤ���A�z�ݭn�U�C���{���X�A
// �H�Kø�s�ϥܡC���ϥΤ��/�˵��Ҧ��� MFC ���ε{���A
// �ج[�|�۰ʧ������@�~�C

void CQuoteLiteDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ø�s���˸m���e

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// �N�ϥܸm����Τ�ݯx��
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �yø�ϥ�
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ��ϥΪ̩즲�̤p�Ƶ����ɡA
// �t�ΩI�s�o�ӥ\����o�����ܡC
HCURSOR CQuoteLiteDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

LRESULT CQuoteLiteDlg::ReadFilePartOneFinish(WPARAM wParam, LPARAM lParam)
{
	KillTimer(ShowReadFileProgressPartOne);
	SetTimer(ShowReadFileProgressPartTwo, 100, NULL);
	CString csProgress;
	csProgress.Format(_T("%d/%d [100 %%] , 0/%d [0 %%]"), m_SymbolsManager.GetFileNum(), m_SymbolsManager.GetFileNum(), m_SymbolsManager.GetFileNum());
	m_Progress.SetWindowText(csProgress);
	ShowFileItemName();
	CString* csUseTime = (CString*)lParam;
	m_UseTime.SetWindowText(*csUseTime);
	return LRESULT(1);
}

LRESULT CQuoteLiteDlg::ReadFilePartTwoFinish(WPARAM wParam, LPARAM lParam)
{
	KillTimer(ShowReadFileProgressPartTwo);
	CString csProgress;
	csProgress.Format(_T("%d/%d [100 %%] , %d/%d [100 %%]"), m_SymbolsManager.GetFileNum(), m_SymbolsManager.GetFileNum(), m_SymbolsManager.GetFileNum(), m_SymbolsManager.GetFileNum());
	m_Progress.SetWindowText(csProgress);
	CString* csUseTime = (CString*)lParam;
	m_UseTime.SetWindowText(*csUseTime);
	return LRESULT(1);
}

LRESULT CQuoteLiteDlg::ReadFileErrorMessage(WPARAM wParam, LPARAM lParam)
{
	CString *csMessage = (CString*)lParam;
	static int index;
	m_ErrorFile.InsertItem(++index, *csMessage);
	return LRESULT(1);
}

void CQuoteLiteDlg::OnTimer(UINT_PTR nIDEvent) 
{   
	CString csProgress;
	switch (nIDEvent){
	case ShowReadFileProgressPartOne:
		if (false == m_SymbolsManager.GetReadBasicInfostatus())
		{
			csProgress.Format(_T("%d/%d [%.0f %%] , 0/%d [0 %%]"), m_SymbolsManager.GetCurrentReadFileProgress(), m_SymbolsManager.GetFileNum(), ((double)(m_SymbolsManager.GetCurrentReadFileProgress()) / (double)m_SymbolsManager.GetFileNum()) * 100, m_SymbolsManager.GetFileNum());
			m_Progress.SetWindowText(csProgress);
		}
		break;
	case ShowReadFileProgressPartTwo:
		csProgress.Format(_T("%d/%d [100 %%] , %d/%d [%.0f %%]"), m_SymbolsManager.GetFileNum(), m_SymbolsManager.GetFileNum(), m_SymbolsManager.GetCurrentReadFileProgress(), m_SymbolsManager.GetFileNum(), ((double)(m_SymbolsManager.GetCurrentReadFileProgress()) / (double)m_SymbolsManager.GetFileNum()) * 100);
		m_Progress.SetWindowText(csProgress);
		break;
	default:
		break;
	}
	CDialog::OnTimer(nIDEvent);
}


////////////////////////////////////////////////////////////////////////////////////
// �I��Start���s
void CQuoteLiteDlg::OnBnClickedStart()
{
	DoClickedStart();
}


void CQuoteLiteDlg::DoClickedStart()
{
	if (true != m_bAlreadyRead) // �P�_��Ƨ��O�_�wŪ���L
	{
		int ifilenum = m_SymbolsManager.ReadSymbolsInfo();

		if (0 < ifilenum)
		{
			SetTimer(ShowReadFileProgressPartOne, 100, NULL);
		}
		else
		{
			m_UseTime.SetWindowText(_T("Can not find the path or no files under the path"));
		}

		m_bAlreadyRead = true;
	}
	else
	{
		MessageBox(_T("You have pressed the start button"));
	}
}



////////////////////////////////////////////////////////////////////////////////////
// �I��dialog�ɮצW�����
void CQuoteLiteDlg::OnTvnSelchangedTreeFileitemname(NMHDR *pNMHDR, LRESULT *pResult)	
{
	if (true == m_SymbolsManager.GetReadFilestatus())	// �ɮ׬O�_��Ū�����A
	{
		// ���o�ثe CPU frequency
		QueryPerformanceFrequency(&m_liPerfFreq);
		// ���o����e�ɶ�
		QueryPerformanceCounter(&m_liPerfStart);

		HTREEITEM hItem = m_FileItemName.GetSelectedItem();
		if (hItem != NULL)
		{
			string strText = CT2A(m_FileItemName.GetItemText(hItem));
			if ("0x10" != strText)
			{	
				if (false != m_SymbolsManager.GetSymbolStatus(strText))	// Ū�ɮ��ɮ�Ū�����\
				{
					ShowListItem(strText);
					ShowTradeDetail(strText);
					ShowPriceOfValume(strText);
					ShowSimilarItem(strText);

					// ���o����᪺�ɶ�
					QueryPerformanceCounter(&m_liPerfEnd);
					m_dReadSymbolCostTime = (double)(m_liPerfEnd.QuadPart - m_liPerfStart.QuadPart) / (double)m_liPerfFreq.QuadPart;

					CString csUseTime;
					csUseTime.Format(_T("%.6f"), m_dReadSymbolCostTime);
					m_SymbolUseTime.SetWindowText(csUseTime);
				}
				else
				{
					MessageBox(_T("Failed to read this file!!"));	// Ū�ɮ��ɮ�Ū������
				}		
			}
		}
		*pResult = 0;
	}
	else
	{
		MessageBox(_T("Wait for Load and find Symbol data"));
	}
}


////////////////////////////////////////////////////////////////////////////////////
//������ܸ�T
void CQuoteLiteDlg::ShowFileItemName()
{
	m_FileItemName.SetRedraw(FALSE);
	m_FileItemName.DeleteAllItems();

	vector<string> vecPackage;

	m_SymbolsManager.GetFileNameList(vecPackage);

	HTREEITEM hFileFloder;
	hFileFloder = m_FileItemName.InsertItem(_T("0x10"), TVI_ROOT);

	for (int i = 0; i < vecPackage.size(); ++i)
	{
		m_FileItemName.InsertItem((CString)vecPackage[i].c_str(), hFileFloder);
	}

	m_FileItemName.Expand(hFileFloder, TVE_EXPAND);
	m_FileItemName.SetRedraw(TRUE);
}

void CQuoteLiteDlg::ShowListItem(string &strText)
{
	CQuoteManager::CBasicInfo classPackage;
	m_SymbolsManager.GetBasicInfoPackage(strText, classPackage);
	CString csFloatToString;
	m_ListItem.SetItemText(0, 1, (CString)classPackage.m_strSymbolID.c_str());
	if (-1 != classPackage.m_iTime)
	{
		csFloatToString.Format(_T("%d"), classPackage.m_iTime);
		m_ListItem.SetItemText(1, 1, csFloatToString);
		csFloatToString.Format(_T("%.4f"), classPackage.m_iPrice);
		m_ListItem.SetItemText(2, 1, csFloatToString);
		csFloatToString.Format(_T("%d"), classPackage.m_iVolume);
		m_ListItem.SetItemText(3, 1, csFloatToString);
		if (-1 != classPackage.m_dAvgPrice)
		{
			csFloatToString.Format(_T("%.4f"), classPackage.m_dAvgPrice);
			m_ListItem.SetItemText(4, 1, csFloatToString);
		}
		else
		{
			m_ListItem.SetItemText(4, 1, _T("-"));
		}

		csFloatToString.Format(_T("%d"), classPackage.m_iTotalVolume);
		m_ListItem.SetItemText(5, 1, csFloatToString);
		csFloatToString.Format(_T("%.4f"), classPackage.m_dPriceHigh);
		m_ListItem.SetItemText(6, 1, csFloatToString);
		csFloatToString.Format(_T("%.4f"), classPackage.m_dPriceLow);
		m_ListItem.SetItemText(7, 1, csFloatToString);
	}
	else
	{
		m_ListItem.SetItemText(1, 1, _T("-"));
		m_ListItem.SetItemText(2, 1, _T("-"));
		m_ListItem.SetItemText(3, 1, _T("-"));
		m_ListItem.SetItemText(4, 1, _T("-"));
		m_ListItem.SetItemText(5, 1, _T("-"));
		m_ListItem.SetItemText(6, 1, _T("-"));
		m_ListItem.SetItemText(7, 1, _T("-"));
	}
	csFloatToString.Format(_T("%d"), classPackage.m_iLikeSymbolCnt);
	m_ListItem.SetItemText(8, 1, csFloatToString);
}

void CQuoteLiteDlg::ShowTradeDetail(string &strText)
{
	m_TradeDetail.SetRedraw(FALSE);
	m_TradeDetail.DeleteAllItems();

	vector<CQuoteManager::CTickInfo> vecPackage;
	m_SymbolsManager.GetPerTradeDetailInfoPackage(strText, vecPackage);
	CString csFloatToString;

	for (int i = 0; i < vecPackage.size(); i++)
	{
		csFloatToString.Format(_T("%d"), vecPackage.at(i).m_iTime);
		m_TradeDetail.InsertItem(i, csFloatToString);
		csFloatToString.Format(_T("%.4f"), vecPackage.at(i).m_dPrice);
		m_TradeDetail.SetItemText(i, 1, csFloatToString);
		csFloatToString.Format(_T("%d"), vecPackage.at(i).m_iVolume);
		m_TradeDetail.SetItemText(i, 2, csFloatToString);
	}
	m_TradeDetail.SetRedraw(TRUE);
}

void CQuoteLiteDlg::ShowPriceOfValume(string &strText)
{
	m_PriceOfValume.SetRedraw(FALSE);
	m_PriceOfValume.DeleteAllItems();

	map<double, int> mapPackage;
	m_SymbolsManager.GetPriceAndVolumeInfoPackage(strText, mapPackage);

	map<double, int>::iterator iter;
	int index = 0;
	CString csFloatToString;

	for (iter = mapPackage.begin(); iter != mapPackage.end(); ++iter)
	{
		csFloatToString.Format(_T("%.4f"), iter->first);
		m_PriceOfValume.InsertItem(index, csFloatToString);
		csFloatToString.Format(_T("%d"), iter->second);
		m_PriceOfValume.SetItemText(index, 1, csFloatToString);
		index++;
	}
	m_PriceOfValume.SetRedraw(TRUE);
}

void CQuoteLiteDlg::ShowSimilarItem(string &strText)
{
	m_SimilarItem.SetRedraw(FALSE);
	m_SimilarItem.DeleteAllItems();

	vector<CQuoteManager::CSimilarSymbolInfo> vecPackage;
	m_SymbolsManager.GetSimilarCommodityPackage(strText, vecPackage);
	CString csFloatToString;

	for (int i = 0; i < vecPackage.size(); i++)
	{
		m_SimilarItem.InsertItem(i, (CString)vecPackage.at(i).m_strSymbolID.c_str());
		csFloatToString.Format(_T("%.4f"), vecPackage.at(i).m_dAvgPrice);
		m_SimilarItem.SetItemText(i, 1, csFloatToString);
		csFloatToString.Format(_T("%d"), vecPackage.at(i).m_iTotalVolume);
		m_SimilarItem.SetItemText(i, 2, csFloatToString);
	}
	m_SimilarItem.SetRedraw(TRUE);
}