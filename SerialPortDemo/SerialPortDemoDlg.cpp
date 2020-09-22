
// SerialPortDemoDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "SerialPortDemo.h"
#include "SerialPortDemoDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// CSerialPortDemoDlg 对话框



CSerialPortDemoDlg::CSerialPortDemoDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SERIALPORTDEMO_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSerialPortDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_PortNo, m_PortNo);
	DDX_Control(pDX, IDC_COMBO_Baud, m_Baud);
	DDX_Control(pDX, IDC_BUTTON_Connect, m_btn_Connect);
	DDX_Control(pDX, IDC_BUTTON_Send, m_btn_Send);
	DDX_Control(pDX, IDC_EDIT_Send, m_text_Send);
	DDX_Control(pDX, IDC_EDIT_Recieved, m_text_Recieved);
}

BEGIN_MESSAGE_MAP(CSerialPortDemoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_Connect, &CSerialPortDemoDlg::OnBnClickedButtonConnect)
	ON_BN_CLICKED(IDC_BUTTON_Send, &CSerialPortDemoDlg::OnBnClickedButtonSend)
END_MESSAGE_MAP()


// CSerialPortDemoDlg 消息处理程序

BOOL CSerialPortDemoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	
	if (DetectPort())
	{

		for (int i = 0; i < sizeof(port); i++)
		{
			if (port[i])
			{
				CString str;
				str.Format(_T("COM %d"), i + 1);
				m_PortNo.AddString(str);
			}
		}
		m_PortNo.SetCurSel(0);
	}
	

	m_Baud.AddString(_T("9600"));
	m_Baud.AddString(_T("19200"));
	m_Baud.AddString(_T("38400"));
	m_Baud.AddString(_T("57600"));
	m_Baud.AddString(_T("115200"));
	m_Baud.SetCurSel(0);
	//绑定串口数据的回调事件
	std::function<void(byte*, UINT)> _fun = std::bind(&CSerialPortDemoDlg::m_CallBack, this, std::placeholders::_1, std::placeholders::_2);
	m_com.CallBack(_fun);
	


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CSerialPortDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSerialPortDemoDlg::OnPaint()
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
HCURSOR CSerialPortDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CSerialPortDemoDlg::OnBnClickedButtonConnect()
{
	// TODO: 在此添加控件通知处理程序代码
	if (isConnected)
	{
		m_com.ClosePort();
		m_com.CloseListenTread();
		

		m_btn_Connect.SetWindowTextW(_T("建立通信"));
		m_PortNo.EnableWindow(true);
		m_Baud.EnableWindow(true);
		m_btn_Send.EnableWindow(false);

		isConnected = false;
		
	}
	else
	{
		CString str;
		CString str1;

		m_PortNo.GetWindowTextW(str);
		AfxExtractSubString(str1, str, 1, ' ');
		int port = _ttoi(str1);

		m_Baud.GetWindowTextW(str1);

		UINT Baud = _ttoi(str1);

		m_com.InitPort(port, Baud, 'N', 8, 1, EV_RXCHAR);
		m_com.OpenListenThread();



		m_btn_Connect.SetWindowTextW(_T("断开通信"));
		isConnected = true;

		m_PortNo.EnableWindow(false);
		m_Baud.EnableWindow(false);
		m_btn_Send.EnableWindow(true);
	}
}


void CSerialPortDemoDlg::OnBnClickedButtonSend()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	CStringA data;
	m_text_Send.GetWindowTextW(str);
	data = str;
	SendCmd(data);

}

void CSerialPortDemoDlg::m_CallBack(byte* data, UINT length)
{
	int L = length;
	CString strtemp;
	for (int k = 0; k < L; k++) // 将数组转换为CString型变量
	{
		BYTE bt = *(char*)(data + k); // 字符型
		strtemp += bt;
	}
	strtemp += "\r\n";
	int TextL = m_text_Recieved.GetWindowTextLengthW();

	m_text_Recieved.SetSel(TextL, TextL,true);
	m_text_Recieved.ReplaceSel(strtemp, false);
}
int CSerialPortDemoDlg::DetectPort()
{
	memset(port, 0, sizeof(port));

	CString com;

	int cnt = 0;

	for (int i = 0; i < 10; i++)
	{
		if (!m_com.DetectPort(i + 1))     // 如果没有该设备，或者被其他应用程序在用    *******************
		{
			port[i] = false;
		}
		else
		{
			cnt++;     // 如果存在，则记录下来。
			port[i] = true;
		}
	}

	if (cnt)     // 如果串口存在，则执行相应的初始化（采用控件）
	{
	}
	else     // 如果不存在，则显示错误信息，而不进行串口操作，防止系统异常造成界面的初始化不完全
	{
		CString str = _T("没有可用串口");
		AfxMessageBox(str, MB_OK);
	}
	return cnt;
}
int CSerialPortDemoDlg::SendCmd(CStringA cmd)
{
	int len = cmd.GetLength();
	byte* data = new byte[len];
	memcpy(data, cmd, len);
	m_com.WriteData(data, len);
	return 1;
}
