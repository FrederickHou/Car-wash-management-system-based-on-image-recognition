
// ServerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Server.h"
#include "ServerDlg.h"
#include "afxdialogex.h"
#include <excpt.h>
#include <afxsock.h>
#include "MyReceive.h"
#include "AdoSql.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
CServerDlg *pDlog;
extern MyReceive* SocketSend;
DWORD WINAPI Accept(LPVOID lpParam)
{
  
	return 0;
}


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

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
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
	
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CServerDlg 对话框



CServerDlg::CServerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SERVER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	pDlog = this;
}

void CServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Control(pDX, IDC_EDIT1, ReceiveText);
	//	DDX_Control(pDX, IDC_BUTTON1, ListenButton);
	//  DDX_Control(pDX, IDC_EDIT1, ReceiveText);
	//DDX_Control(pDX, IDC_EDIT1, ReceiveText);
	//  DDX_Control(pDX, IDC_EDIT1, SocketReceText);
	DDX_Control(pDX, IDC_EDIT1, Text);
	DDX_Control(pDX, IDC_EDIT2, SendText);
	DDX_Control(pDX, IDC_EDIT3, CLogNum);
}

BEGIN_MESSAGE_MAP(CServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CServerDlg::OnBnClickedButton1)
//	ON_BN_CLICKED(IDC_BUTTON3, &CServerDlg::OnBnClickedButton3)
//	ON_BN_CLICKED(IDC_BUTTON4, &CServerDlg::OnBnClickedButton4)
ON_EN_CHANGE(IDC_EDIT1, &CServerDlg::OnEnChangeEdit1)
END_MESSAGE_MAP()


// CServerDlg 消息处理程序

BOOL CServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	SqlConnect.OnInitADOConn();//数据库初始化
	
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
	
	if (SocketServer = new MyReceive())
	{
		if (SocketServer->Create(6000))
		{
			Text.SetWindowTextW(_T("正在建立连接......"));
			SocketServer->Listen();
		}
		else
		{
			Text.SetWindowTextW(_T("初始化失败，请重启程序"));
			delete SocketServer;
		}
	}
	else
	{
		Text.SetWindowTextW(_T("初始化失败，请重启程序"));
		delete SocketServer;
	}
	
	
	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CServerDlg::OnPaint()
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
HCURSOR CServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CServerDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CString SendText;
	pDlog->SendText.GetWindowTextW(SendText);
	char *p;
	p = (LPSTR)(LPCTSTR)SendText;
	SocketSend->Send(p, 255, 0);
}




void CServerDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CServerDlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	
		/*SocketRecv = new MyReceive();
		if (SocketServer->Accept(*SocketRecv))
		{
			/*char buf[255] = { 0 };
			CString strServerRecText;
			SocketRecv->Receive(buf, 255);
			strServerRecText.Format(_T("%s"), buf);
			Text.SetWindowTextW(strServerRecText);
			SocketRecv = NULL;
			delete SocketRecv;
		}*/
	
//	handle_1 = CreateThread(NULL, 0, &Accept, NULL, 0, NULL);
	
}


void CAboutDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
	//CDialogEx::OnTimer(nIDEvent);
}


void CServerDlg::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}
