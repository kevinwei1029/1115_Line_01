
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

	void Draw(char* file, int ofx, int ofy, int flag, float angel) {//  1. Open the image file
		errno_t err;
		FILE* fp;
		if ((err = fopen_s(&fp, file, "rb")) != 0) {
			SetWindowText("File not found");
			return;
		}
		else {
			SetWindowText("File found");
		}

		//  2. Get the size of the image
		char head[100];
		int w, h;
		fread(head, 1, 54, fp);
		w = *(int*)&head[18];
		h = *(int*)&head[22];
		SetWindowText((std::to_string(w) + " " + std::to_string(h)).c_str());

		/*
		unsigned char* Data = new unsigned char[w * h * 3];
		fread(Data, 1, w * h * 3, fp);
		Draw_BitMap(w, h, ofx, ofy, Data);*/

		//  3. Read the image file and draw
		int x, y, d = (4 - ((3 * w) % 4)) % 4;
		HDC hdc = ::GetDC(m_hWnd);
		unsigned char c[100];

		float a = 3.14159 * angel / 180.0;

		for (y = 0; y < h; y++) {
			for (x = 0; x < w; x++) {
				fread(c, 1, 3, fp);

				//  Rotate the image
				if (angel && flag == 1) {
					int xx = (int)((x - w / 2) * cos(a) - (y - h / 2) * sin(a) + w / 2);
					int yy = (int)((x - w / 2) * sin(a) + (y - h / 2) * cos(a) + h / 2);
					if (!(c[1] > 128 && c[0] < 128 && c[2] < 128)) {
						SetPixel(hdc, xx + ofx, yy + ofy, RGB(c[2], c[1], c[0]));
					}
				}
				//  clear background
				else if (flag == 1) {
					if (!(c[1] > 128 && c[0] < 128 && c[2] < 128)) {
						SetPixel(hdc, x + ofx, h - y + ofy, RGB(c[2], c[1], c[0]));
					}
				}
				else {
					SetPixel(hdc, x + ofx, h - y + ofy, RGB(c[2], c[1], c[0]));
				}
			}
			fread(c, 1, d, fp);  //  Skip the padding
		}
	};
	// =============================================================
	// === Draw_BitMap()
	// =============================================================
	int Draw_BitMap(int x, int y, int PX, int PY, unsigned char* Data)//x=ｼe y=ｪ・ PX=xｶbｰ_ｩlﾂI PY=yｶbｰ_ｩlﾂI
	{
		HDC        hdc;
		HDRAWDIB   hdd;
		BITMAPINFO bmpinfo;
		char       ErrMsg[100];

		// ====== Draw Init ================= 
		bmpinfo.bmiHeader.biSize = 40;
		bmpinfo.bmiHeader.biWidth = x;
		bmpinfo.bmiHeader.biHeight = y;
		bmpinfo.bmiHeader.biPlanes = 1;
		bmpinfo.bmiHeader.biBitCount = 24;
		bmpinfo.bmiHeader.biCompression = 0;
		bmpinfo.bmiHeader.biSizeImage = x * y * 3;
		bmpinfo.bmiHeader.biXPelsPerMeter = 0;
		bmpinfo.bmiHeader.biYPelsPerMeter = 0;
		bmpinfo.bmiHeader.biClrUsed = 0;
		bmpinfo.bmiHeader.biClrImportant = 0;

		if (!(hdd = DrawDibOpen()))
		{
			strcpy_s(ErrMsg, sizeof(ErrMsg), "DrawDipOpen Error!"); return -1;
		}
		if (!(hdc = ::GetDC(m_hWnd)))
		{
			strcpy_s(ErrMsg, sizeof(ErrMsg), " GetDC Error!"); return -1;
		}
		if (!DrawDibBegin(hdd, hdc, -1, -1, &bmpinfo.bmiHeader, x, y, NULL))
		{
			strcpy_s(ErrMsg, sizeof(ErrMsg), "DrawDibBegin error!"); return -1;
		}
		if (!DrawDibDraw(hdd, hdc, PX, PY, x, y, &bmpinfo.bmiHeader, Data, 0, 0, x, y, DDF_SAME_DRAW | DDF_SAME_HDC))
		{
			strcpy_s(ErrMsg, sizeof(ErrMsg), "DrawDibDraw error"); return -1;
		}
		if (!DrawDibEnd(hdd))
		{
			strcpy_s(ErrMsg, sizeof(ErrMsg), "initDisplay DrawDibBegin Error!"); return -1;
		}
		return 0;
	}
};
