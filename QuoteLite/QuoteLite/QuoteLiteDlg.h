
// QuoteLiteDlg.h : ���Y��
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include <atomic>
#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <io.h>
#include <memory>

#include "SymbolInfo.h"
#include "QuoteManager.h"

// CQuoteLiteDlg ��ܤ��
class CQuoteLiteDlg : public CDialogEx
{
// �غc
public:
	CQuoteLiteDlg(CWnd* pParent = NULL);	// �зǫغc�禡

// ��ܤ�����
	enum { IDD = IDD_QUOTELITE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �䴩


// �{���X��@
protected:
	HICON m_hIcon;

	// ���ͪ��T�������禡
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	afx_msg void OnBnClickedStart();
	afx_msg void OnTvnSelchangedTreeFileitemname(NMHDR *pNMHDR, LRESULT *pResult);
private:
	CListCtrl m_TradeDetail;
	CListCtrl m_ListItem;
	CStatic m_Progress;
	CStatic m_UseTime;
	CStatic m_SymbolUseTime;
	CListCtrl m_PriceOfValume;
	CTreeCtrl m_FileItemName;
	CListCtrl m_SimilarItem;
	CListCtrl m_ErrorFile;

private:
	void InitializeDialogForm();

	CQuoteManager m_SymbolsManager;

	void DoClickedStart();

	void ShowFileItemName();
	void ShowListItem(string &strText);
	void ShowTradeDetail(string &strText);
	void ShowPriceOfValume(string &strText);
	void ShowSimilarItem(string &strText);

	afx_msg LRESULT ReadFileErrorMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT ReadFilePartOneFinish(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT ReadFilePartTwoFinish(WPARAM wParam, LPARAM lParam);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	bool m_bAlreadyRead;

	LARGE_INTEGER m_liPerfFreq;
	LARGE_INTEGER m_liPerfStart;
	LARGE_INTEGER m_liPerfEnd;
	double m_dReadSymbolCostTime;
};
