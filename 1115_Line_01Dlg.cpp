
// 1115_Line_01Dlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "1115_Line_01.h"
#include "1115_Line_01Dlg.h"
#include "afxdialogex.h"
#include "TCPIP.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//  0. Variables
#define LINE_EVENT  (WM_USER + 100)
TCPIP* lpTCPIP;
SOCKET Line_Sock;
BOOL TCPIP_F = 0;  //  if registered
char Server_IP[100] = {"140.113.170.78"};
int  Server_Port = 6000;

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMy1115Line01Dlg dialog



CMy1115Line01Dlg::CMy1115Line01Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MY1115_LINE_01_DIALOG, pParent)
	, m_Account(_T("Kevin"))
	, m_Password(_T("112511142"))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMy1115Line01Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_Tab1);
	DDX_Text(pDX, IDC_EDIT1, m_Account);
	DDX_Text(pDX, IDC_EDIT2, m_Password);
}

BEGIN_MESSAGE_MAP(CMy1115Line01Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CMy1115Line01Dlg::OnSelchangeTab1)
	ON_BN_CLICKED(IDC_BUTTON1, &CMy1115Line01Dlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CMy1115Line01Dlg message handlers

BOOL CMy1115Line01Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	m_Tab1.InsertItem(0, "LOGO");            //  week 1
	m_Tab1.InsertItem(1, "Register");        //  week 1
	m_Tab1.InsertItem(2, "Chatroom");        //  week 1
	m_Tab1.InsertItem(3, "Friends");         //  week 2
	m_Tab1.InsertItem(4, "Phone call");	     //  week 3, 4
	m_Tab1.InsertItem(5, "Video call");	     //  week 4, 5
	m_Tab1.InsertItem(6, "Sticker");         //  week 5

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMy1115Line01Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMy1115Line01Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMy1115Line01Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMy1115Line01Dlg::OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: Add your control notification handler code here
	char S1[200];
	int i = m_Tab1.GetCurSel();
	sprintf_s(S1, sizeof(S1), "%d", m_Tab1.GetCurSel());
	SetWindowText(S1);

	switch (i)
	{
	case 0:
		SetWindowText("LOGO");
		break;

	case 1:
		SetWindowText("Register");
		break;

	case 2:
		SetWindowText("Chatroom");
		break;
	}

	*pResult = 0;
}


void CMy1115Line01Dlg::OnBnClickedButton1()  //  Register button
{
	//  1. New a TCPIP object and start TCPIP client
	if (!TCPIP_F)
	{
		lpTCPIP = new TCPIP();
		lpTCPIP->Start_TCP_Client(&Line_Sock, Server_Port, Server_IP, LINE_EVENT, m_hWnd);
		TCPIP_F = 1;
	}
}


LRESULT CMy1115Line01Dlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	char S1[2000];
	int i = 0;
	//  3. Receive the response from server
	
	if (message == LINE_EVENT)
	{
		switch (lParam)
		{
		case FD_CONNECT:
			SetWindowText("FD_CONNECT");
			//  2. Send account and password to server & "REGISTER/Account/Password" command
			char S1[2000];
			UpdateData(TRUE);
			sprintf_s(S1, sizeof(S1), "REGISTER/%s/%s", m_Account, m_Password);
			UpdateData(FALSE);
			send(wParam, S1, strlen(S1), 0);
			break;

		case FD_READ:
			i = recv(wParam, S1, sizeof(S1), 0);
			if (i > 0)
			{
				S1[i] = 0;
				SetWindowText(S1);
			}
			break;

		case FD_CLOSE:
			SetWindowText("FD_CLOSE");
			break;

		default:
			break;
		}
	}

	return CDialogEx::WindowProc(message, wParam, lParam);
}
