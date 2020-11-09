
// SW_BANDIADlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "SW_BANDIA.h"
#include "SW_BANDIADlg.h"
#include "afxdialogex.h"
#include "KBLoc.h"
#include <atlstr.h> 
#include <setupapi.h>
#include <atlstr.h>
#include<string>
#include <psapi.h>
#include<sstream>
#include<iostream>
#include <Dbt.h>
#include <stdio.h>
#include <tchar.h>
#include<fstream>
using namespace std;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#pragma comment(lib, "Psapi.Lib")
#pragma comment(lib,"KBLoc.lib")

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
char bantextlist[8192];
char input[128];
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CAboutDlg::OnBnClickedButton1)
END_MESSAGE_MAP()

string ltos(long l)
{
	ostringstream os;
	os << l;
	string result;
	istringstream is(os.str());
	is >> result;
	return result;
}

// CSWBANDIADlg 对话框

CSWBANDIADlg::CSWBANDIADlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SW_BANDIA_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	clock = 0;
	
}

void CSWBANDIADlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

//设置程序开机启动
BOOL AutoBootSet()
{
	HKEY hKey;
	CString lpRun = _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run");
	long lRet = RegOpenKeyEx(HKEY_CURRENT_USER, lpRun, 0, KEY_ALL_ACCESS, &hKey);
	if (lRet == ERROR_SUCCESS)
	{
		TCHAR pFileName[MAX_PATH] = { 0 };
		DWORD dwRet = GetModuleFileName(NULL, pFileName, MAX_PATH);
		_tcscat(pFileName, _T(" -auto"));
		lRet = RegSetValueEx(hKey, _T("SW_BANDIADlg"), 0, REG_SZ, (LPBYTE)pFileName, (lstrlen(pFileName) + 1) * sizeof(TCHAR));
		RegCloseKey(hKey);
		if (lRet != ERROR_SUCCESS)
		{
			return false;
		}
		return true;
	}
	return false;
}


//取消程序开机启动
BOOL AutoBootCancel()
{
	HKEY hKey;
	CString lpRun = _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run");

	long lRet = RegOpenKeyEx(HKEY_CURRENT_USER, lpRun, 0, KEY_ALL_ACCESS, &hKey);
	if (lRet == ERROR_SUCCESS)
	{
		RegDeleteValue(hKey, _T("SW_BANDIADlg"));
		RegCloseKey(hKey);

		return true;
	}
	return false;
}

//判断是否开机自启
BOOL IsAutoBoot()
{
	LPWSTR* szArglist = NULL;
	int nArgs = -1;
	BOOL bRet = FALSE;
	szArglist = CommandLineToArgvW(GetCommandLine(), &nArgs);
	if ((NULL == szArglist) || (nArgs == 2))
	{
		if (_tcsicmp(szArglist[1], _T("-auto")) == 0)
		{
			bRet = TRUE;			
		}
		else
		{
			bRet = FALSE;
		}
	}
	else
	{
		bRet = FALSE;
	}

	LocalFree(szArglist);

	return bRet;
}

BEGIN_MESSAGE_MAP(CSWBANDIADlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CSWBANDIADlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CSWBANDIADlg::OnBnClickedButton2)
	ON_EN_CHANGE(IDC_EDIT1, &CSWBANDIADlg::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_BUTTON3, &CSWBANDIADlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CSWBANDIADlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CSWBANDIADlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CSWBANDIADlg::OnBnClickedButton6)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON7, &CSWBANDIADlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, &CSWBANDIADlg::OnBnClickedButton8)
END_MESSAGE_MAP()


// CSWBANDIADlg 消息处理程序

BOOL CSWBANDIADlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);
	SetTimer(1, 500, NULL);
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	initEnum();
	char*rt=getroot();
	string file = "/banlist.txt";
	string root = string(rt);
	ifstream readfile((root+file).c_str());
	string temp;
	string banli = "BanList\n";
	while (getline(readfile, temp)) {
		banli += temp;
		banli += "\n";
	}
	SetDlgItemText(IDC_STATIC, CString(banli.c_str()));
	file = "/weblist.txt";
	root = string(rt);
	ifstream wreadfile((root + file).c_str());
	string webli = "WebNames\n";
	while (getline(wreadfile, temp)) {
		webli += temp;
		webli += "\n";
	}
	SetDlgItemText(IDC_STATIC4, CString(webli.c_str()));
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CSWBANDIADlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSWBANDIADlg::OnPaint()
{
	
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CSWBANDIADlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CAboutDlg::OnBnClickedButton1()
{
	
	CString str;	
	str.Format(_T("HelloWorld"));	
	SetDlgItemText(IDC_BUTTON1, str);	
}


void CSWBANDIADlg::OnBnClickedButton1()
{
	SetWinCreateHook();
	}


void CSWBANDIADlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	EndWinCreateHook();
}


void CSWBANDIADlg::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}

void addBanlist() {
	char* rt = getroot();
	string filee = "/banlist.txt";
	string root = string(rt);
	ifstream readfile((root + filee).c_str());
	string temp;
	while (getline(readfile, temp)) {
		string banwin(input);
		if (banwin == temp) {
			MessageBox(NULL, NULL, L"alredy exist!", 0);
			return;
		}
	}
	readfile.close();
	ofstream file;
	file.open((root + filee).c_str(), std::ios::app);
	file << input << "\n";
	file.close();
	
}

void addWeblist() {
	char* rt = getroot();
	string filee = "/weblist.txt";
	string root = string(rt);
	ifstream readfile((root + filee).c_str());
	string temp;
	while (getline(readfile, temp)) {
		string banwin(input);
		if (banwin == temp) {
			MessageBox(NULL, NULL, L"alredy exist!", 0);
			return;
		}
	}
	readfile.close();
	ofstream file;
	file.open((root + filee).c_str(), std::ios::app);
	file << input << "\n";
	file.close();

}

void CSWBANDIADlg::OnBnClickedButton3()
{
	CString text;
	char a[128];
	GetDlgItemText(IDC_EDIT1,text);
	strcpy_s(a, (CStringA)text);
	strcpy_s(input, a);
	addBanlist();
}


void CSWBANDIADlg::OnBnClickedButton4()
{
	CString text;
	char a[128];
	GetDlgItemText(IDC_EDIT1, text);
	strcpy_s(a, (CStringA)text);
	strcpy_s(input, a);
	addWeblist();
}


void CSWBANDIADlg::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	SetWebCreateHook();
}


void CSWBANDIADlg::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码
	EndWebCreateHook();
}


void CSWBANDIADlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch (nIDEvent) {
	case 1:
		string temp;
		string result(getlastban());
		SetDlgItemText(IDC_STATIC3, CString(result.c_str()));
		string numre;
		numre += ltos(get_Count());
		CFont m_font;
		m_font.CreatePointFont(180, _T("Microsoft Sans Serif"));
		GetDlgItem(IDC_STATIC5)->SetFont(&m_font);
		SetDlgItemText(IDC_STATIC5, CString(numre.c_str()));
		char* rt = getroot();
		string filee = "/banlist.txt";
		string root = string(rt);
		ifstream breadfile((root + filee).c_str());
		string banli = "BanList\n";
		while (getline(breadfile, temp)) {
			banli += temp;
			banli += "\n";
		}
		SetDlgItemText(IDC_STATIC, CString(banli.c_str()));
		filee = "/weblist.txt";
		ifstream wreadfile((root + filee).c_str());
		string webli = "WebNames\n";
		while (getline(wreadfile, temp)) {
			webli += temp;
			webli += "\n";
		}
		SetDlgItemText(IDC_STATIC4, CString(webli.c_str()));
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CSWBANDIADlg::OnBnClickedButton7()
{
	AutoBootSet();
}


void CSWBANDIADlg::OnBnClickedButton8()
{
	AutoBootCancel();
}
