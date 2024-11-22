
// 1115_Line_01Dlg.h : header file
//
#pragma once
#pragma comment(lib, "Vfw32.lib")
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "Vfw.h"
#pragma once


// CMy1115Line01Dlg dialog
class CMy1115Line01Dlg : public CDialogEx
{
// Construction
public:
	CMy1115Line01Dlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MY1115_LINE_01_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	HICON m_hIcon2;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_Tab1;
	afx_msg void OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedButton1();
	CString m_Account;
	CString m_Password;
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedButton2();
	CString m_Display;
	CString m_Msg;
	CString m_Reciver;
	CEdit m_Display2;
	int Hiden_All();
	CListCtrl m_list1;
//	CListCtrl m_List1;
	afx_msg void OnEnChangeEdit5();
	afx_msg void OnBnClickedButton3();
	CString m_friend;
	afx_msg void OnEnChangeEdit6();
};
