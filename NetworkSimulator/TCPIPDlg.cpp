
// TCPIPDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TCPIP.h"
#include "TCPIPDlg.h"
#include "afxdialogex.h"
#include "Winsock2.h"
#include "GlobalVar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
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


// CTCPIPDlg dialog
// 显示列表每行最多显示的字符数
#define LIST_VIEW_ROW_CHAR_SIZE		75



CTCPIPDlg::CTCPIPDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTCPIPDlg::IDD, pParent)
	, m_strSendMsg(_T(""))
	, C_16_scale_Receive(FALSE)
	, m_bIsSendMsgWith16Bits(FALSE)
	, m_bIsReciveMsgWith16Bits(FALSE)
{
	m_hIcon					= AfxGetApp()->LoadIcon(IDI_ICON1);
	m_strIP					= _T("");
	m_nRemotePort			= 0;
	m_nSelectedServiceType	= 0;
	m_nLocalPort			= 0;
	m_nAutoSendInterval		= 0;
}

void CTCPIPDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BUTTON_Link, m_BtnConnectAndDisconnect);
	DDX_Text(pDX, EDIT_IP, m_strIP);
	DDV_MaxChars(pDX, m_strIP, 20);
	DDX_Text(pDX, EDIT_Link_Port, m_nRemotePort);
	DDV_MinMaxInt(pDX, m_nRemotePort, 0, 65535);
	DDX_Radio(pDX, RADIO_UDP, m_nSelectedServiceType);
	DDV_MinMaxInt(pDX, m_nSelectedServiceType, 0, GlobalVar::eLinkMode_Max);
	DDX_Control(pDX, BUTTON_TCP_Server, m_RadioBtnStartTCPServer);
	DDX_Text(pDX, EDIT_My_Port, m_nLocalPort);
	DDV_MinMaxInt(pDX, m_nLocalPort, 0, 65535);
	DDX_Control(pDX, IDC_LIST_Receive, m_ListViewReceives);
	DDX_Text(pDX, IDC_EDIT_Send, m_strSendMsg);
	DDX_Control(pDX, IDC_BUTTON_DELETE_TCPSERVER, m_BtnDeleteTCPServer);
	DDX_Control(pDX, IDC_EDIT_Send, m_ListViewSends);
	DDX_Check(pDX, IDC_CHECK1, C_16_scale_Receive);
	DDX_Check(pDX, IDC_CHECK2_16Send, m_bIsSendMsgWith16Bits);
	DDX_Check(pDX, IDC_CHECK1_16_receie, m_bIsReciveMsgWith16Bits);
	DDX_Check(pDX, IDC_Auto_Send, m_bISAutoSend);
	DDX_Text(pDX, IDC_EDIT_Send_Time, m_nAutoSendInterval);
	DDX_Control(pDX, IDC_COMBO1_View, m_ComboBoxMsgShowType);
}

BEGIN_MESSAGE_MAP(CTCPIPDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_Link, &CTCPIPDlg::OnBnClickedButtonLink)
	ON_BN_CLICKED(RADIO_TCP_Server, &CTCPIPDlg::OnBnClickedTcpServer)
	ON_BN_CLICKED(RADIO_UDP, &CTCPIPDlg::OnBnClickedUdp)
	ON_BN_CLICKED(RADIO_TCP_Client, &CTCPIPDlg::OnBnClickedTcpClient)
	ON_BN_CLICKED(BUTTON_TCP_Server_Creat, &CTCPIPDlg::OnBnClickedTcpServerCreate)
	ON_MESSAGE(WM_PROCESS_SHOW_RECEIVE,OnProcess_Receive)
	ON_BN_CLICKED(IDC_BUTTON_Send, &CTCPIPDlg::OnBnClickedButtonSend)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_TCPSERVER, &CTCPIPDlg::OnBnClickedButtonDeleteTcpserver)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_Clear, &CTCPIPDlg::OnBnClickedButtonClearRecived)
	ON_BN_CLICKED(IDC_BUTTON_Clear_Send, &CTCPIPDlg::OnBnClickedButtonClearSend)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_Auto_Send, &CTCPIPDlg::OnBnClickedAutoSend)
	ON_EN_CHANGE(IDC_EDIT_Send_Time, &CTCPIPDlg::OnEnChangeEditSendTime)
	ON_WM_TIMER()
	ON_CBN_SELCHANGE(IDC_COMBO1_View, &CTCPIPDlg::OnCbnSelchangeCombo1View)
	ON_BN_CLICKED(IDC_BUTTON_Html, &CTCPIPDlg::OnBnClickedButtonHtml)
END_MESSAGE_MAP()


// CTCPIPDlg message handlers

BOOL CTCPIPDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	{// 关于
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
	}
	{// ICON
		SetIcon(m_hIcon, TRUE);			// Set big icon
		SetIcon(m_hIcon, FALSE);		// Set small icon
	}

	{// 加载配置文件
		char szTmp[64];
		memset(&szTmp, 0, sizeof(char) * 64);
		m_nLocalPort = GetPrivateProfileInt("Setting", "LocalPort", DEFAULT_LISTEN_PORT, DEFAULT_CONFIG_FILE);
		m_nRemotePort = GetPrivateProfileInt("Setting", "RemotePort", DEFAULT_REMOTE_PORT, DEFAULT_CONFIG_FILE);
		GetPrivateProfileString("Setting", "RemoteIP", DEFAULT_REMOTE_IP, szTmp, sizeof(szTmp), DEFAULT_CONFIG_FILE);
		m_strIP = szTmp;
		memset(&szTmp, 0, sizeof(char) * 64);
		GetPrivateProfileString("Setting", "DefaultProtocalType", DEFAULT_PROTOCAL_TYPE, szTmp, sizeof(szTmp), DEFAULT_CONFIG_FILE);
		CString strTemp = szTmp;
		if (strTemp == UDP_NAME)
		{
			m_nSelectedServiceType = GlobalVar::eLinkMode_UDP;
			m_RadioBtnStartTCPServer.EnableWindow(FALSE);
			m_BtnDeleteTCPServer.EnableWindow(FALSE);
		}
		else if (strTemp == TCP_CLIENT_NAME)
		{
			m_RadioBtnStartTCPServer.EnableWindow(FALSE);
			m_BtnDeleteTCPServer.EnableWindow(FALSE);
			m_nSelectedServiceType = GlobalVar::eLinkMode_TCPClient;
		}
		else if (strTemp == TCP_SERVER_NAME)
		{
			m_nSelectedServiceType = GlobalVar::eLinkMode_TCPServer;
			m_BtnConnectAndDisconnect.EnableWindow(FALSE);
			m_BtnDeleteTCPServer.EnableWindow(FALSE);
		}
		m_nAutoSendInterval = GetPrivateProfileInt("Setting", "AutoSendTime", DEFAULT_AUTO_SEND_TIME, DEFAULT_CONFIG_FILE);
		UpdateData(FALSE);


		if (GetPrivateProfileInt("Setting", "SendStyle", DEFAULT_SEND_TYPE, DEFAULT_CONFIG_FILE) == 0)
		{
			((CButton *)GetDlgItem(IDC_CHECK2_16Send))->SetCheck(0);
		}
		else
		{
			((CButton *)GetDlgItem(IDC_CHECK2_16Send))->SetCheck(1);
		}
		if (GetPrivateProfileInt("Setting", "ReceiveStyle", DEFAULT_RECEIVE_TYPE, DEFAULT_CONFIG_FILE) == 0)
		{
			((CButton *)GetDlgItem(IDC_CHECK1_16_receie))->SetCheck(0);
		}
		else
		{
			((CButton *)GetDlgItem(IDC_CHECK1_16_receie))->SetCheck(1);
		}
		if (GetPrivateProfileInt("Setting", "ShowSendMSG", DEFAULT_SHOW_SEND_MSG, DEFAULT_CONFIG_FILE) == 0)
		{
			m_bIsShowSendMsg = false;
			m_ComboBoxMsgShowType.SetCurSel(0);
		}
		else
		{
			m_bIsShowSendMsg = true;
			m_ComboBoxMsgShowType.SetCurSel(1);
		}
		UpdateData(TRUE);
	}
	{// 初始化显示选项控件
		m_ComboBoxMsgShowType.ResetContent();
		m_ComboBoxMsgShowType.AddString("不显示发送数据");
		m_ComboBoxMsgShowType.AddString("显示发送的数据");
		m_ComboBoxMsgShowType.SetCurSel(1);
		UpdateData(FALSE);
	}
	{// 初始化网络
		WSADATA wsaData;
		int err;
		WORD wVersionRequested;
		wVersionRequested = MAKEWORD(1, 1);
		err = WSAStartup(wVersionRequested, &wsaData);
		if (err != 0)
		{
			return TRUE;
		}
	}
	{// 添加一个下方信息条
		m_pStatusBar = new CStatusBarCtrl;
		m_pStatusBar->Create(WS_CHILD | WS_VISIBLE | SBT_OWNERDRAW, CRect(0, 0, 0, 0), this, 0);
		int strPartDim[2] = { 150, 500 };
		m_pStatusBar->SetParts(2, strPartDim);
		m_pStatusBar->SetText(_T("Version: 1.2"), 0, 0);
		m_pStatusBar->SetText(_T("FreeKnight  http://duzhi5368.blog.163.com"), 1, 0);
	}
	{// 设置文字笔刷颜色
		HBRUSH hBrush;
		hBrush = CreateSolidBrush(RGB(254, 67, 101));
	}
	{// 初始化日志
		GetSystemTime(&m_SystemTime);
		m_strListViewMsg.Format("%02d:%02d:%02d  欢迎使用网络模拟测试软件，若发现Bug请发送Mail至duzhi5368@163.com", m_SystemTime.wHour, m_SystemTime.wMinute, m_SystemTime.wSecond);
		m_ListViewReceives.AddString(m_strListViewMsg);
	}
	return TRUE;
}

void CTCPIPDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTCPIPDlg::OnPaint()
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
HCURSOR CTCPIPDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


// 点击【连接/断开】按钮
void CTCPIPDlg::OnBnClickedButtonLink()
{
	UpdateData(TRUE);

	// 查看当前选择的模式
	if(m_nSelectedServiceType == 0)
	{
		m_RadioBtnStartTCPServer.EnableWindow(FALSE);
		m_BtnDeleteTCPServer.EnableWindow(FALSE);

		GlobalVar::s_strServiceTypeName = UDP_NAME;
		GlobalVar::s_eLinkMode = GlobalVar::eLinkMode_UDP;
	}
	else if(m_nSelectedServiceType == 1)
	{
		m_RadioBtnStartTCPServer.EnableWindow(FALSE);
		m_BtnDeleteTCPServer.EnableWindow(FALSE);

		GlobalVar::s_strServiceTypeName = TCP_CLIENT_NAME;
		GlobalVar::s_eLinkMode = GlobalVar::eLinkMode_TCPClient;
	}
	else if(m_nSelectedServiceType == 2)
	{
		GlobalVar::s_strServiceTypeName = TCP_SERVER_NAME;
		GlobalVar::s_eLinkMode = GlobalVar::eLinkMode_TCPServer;
	}

	// 根据选择的模式进行处理
	if(GlobalVar::s_strServiceTypeName == TCP_CLIENT_NAME)
	{
		if( !GlobalVar::s_bIsTcpClientStart && (m_ClientSocket.m_hSocket == INVALID_SOCKET))
		{	
			m_ClientSocket.Create(m_nLocalPort, SOCK_STREAM);
			if (m_ClientSocket.m_hSocket == INVALID_SOCKET)
			{
				m_strListViewMsg.Format("%02d:%02d:%02d  创建Socket失败，请检查端口是否被占用： IP:%s 端口:%d\n",
					m_SystemTime.wHour, m_SystemTime.wMinute, m_SystemTime.wSecond, m_strIP, m_nRemotePort);
				m_ListViewReceives.InsertString(0, m_strListViewMsg);
				return;
			}
			m_ClientSocket.Connect(m_strIP, m_nRemotePort);
			const BOOL bReuseaddr = TRUE;
			const BOOL bDontLinger = FALSE;
			m_ClientSocket.SetSockOpt(SO_DONTLINGER, &bDontLinger, sizeof(BOOL), SOL_SOCKET);

			m_ClientSocket.SetSockOpt(SO_REUSEADDR, &bReuseaddr, sizeof(BOOL), SOL_SOCKET);
			m_ClientSocket.GetLastError();
			m_ClientSocket.AsyncSelect(FD_READ | FD_CONNECT | FD_CLOSE);
		}
		else
		{
			m_ClientSocket.Close();
			m_BtnConnectAndDisconnect.SetWindowText("连接");
			m_strListViewMsg.Format("%02d:%02d:%02d  与 IP:%s 端口:%d 断开连接 ！\n",
				m_SystemTime.wHour,m_SystemTime.wMinute,m_SystemTime.wSecond,m_strIP,m_nRemotePort);
		    m_ListViewReceives.InsertString(0,m_strListViewMsg);
			GlobalVar::s_bIsTcpClientStart = false;
		}
        m_BtnConnectAndDisconnect.EnableWindow(TRUE);
	}
	else if(GlobalVar::s_strServiceTypeName == UDP_NAME)
	{
		if( GlobalVar::s_bIsUdpStart==FALSE)
		{
			UDP_Server.Create(m_nLocalPort,SOCK_DGRAM);
			if (UDP_Server == INVALID_SOCKET)
			{
				GlobalVar::s_bIsUdpStart = FALSE;
				m_BtnConnectAndDisconnect.SetWindowText("连接");
				m_strListViewMsg.Format("%02d:%02d:%02d  与 IP:%s 端口:%d 连接失败，端口不可用 ！\n",
					m_SystemTime.wHour, m_SystemTime.wMinute, m_SystemTime.wSecond, m_strIP, m_nRemotePort);
				m_ListViewReceives.InsertString(0, m_strListViewMsg);
				GetDlgItem(RADIO_UDP)->EnableWindow(TRUE);
				GetDlgItem(RADIO_TCP_Client)->EnableWindow(TRUE);
				GetDlgItem(RADIO_TCP_Server)->EnableWindow(TRUE);
				return;
			}
			else
			{
				GlobalVar::s_bIsUdpStart = TRUE;
				m_BtnConnectAndDisconnect.SetWindowText("断开");
				m_strListViewMsg.Format("%02d:%02d:%02d  与 IP:%s 端口:%d 建立UDP连接 ！\n",
					m_SystemTime.wHour, m_SystemTime.wMinute, m_SystemTime.wSecond, m_strIP, m_nRemotePort);
				m_ListViewReceives.InsertString(0, m_strListViewMsg);
				GetDlgItem(RADIO_TCP_Client)->EnableWindow(FALSE);
				GetDlgItem(RADIO_TCP_Server)->EnableWindow(FALSE);
				GetDlgItem(EDIT_My_Port)->EnableWindow(FALSE);
				GetDlgItem(EDIT_Link_Port)->EnableWindow(FALSE);
				GetDlgItem(EDIT_IP)->EnableWindow(FALSE);
				return;
			}
		}
		else
		{
			UDP_Server.Close();
			
			GlobalVar::s_bIsUdpStart=FALSE;
			m_BtnConnectAndDisconnect.SetWindowText("连接");
			m_strListViewMsg.Format("%02d:%02d:%02d  与 IP:%s 端口:%d 断开连接 ！\n",m_SystemTime.wHour,m_SystemTime.wMinute,m_SystemTime.wSecond,m_strIP,m_nRemotePort);
			m_ListViewReceives.InsertString(0,m_strListViewMsg);
			GetDlgItem(RADIO_UDP)->EnableWindow(TRUE);
			GetDlgItem(RADIO_TCP_Client)->EnableWindow(TRUE);
			GetDlgItem(RADIO_TCP_Server)->EnableWindow(TRUE);
			GetDlgItem(EDIT_My_Port)->EnableWindow(TRUE);
			GetDlgItem(EDIT_Link_Port)->EnableWindow(TRUE);
			GetDlgItem(EDIT_IP)->EnableWindow(TRUE);
			return;
		}
	}
	else if(GlobalVar::s_strServiceTypeName == TCP_SERVER_NAME)
	{
		m_BtnConnectAndDisconnect.EnableWindow(FALSE);
		m_ListenSocket.Create(m_nLocalPort);
		m_ListenSocket.Listen();
		const char chOpt=1;
		m_DealTcpSocket.SetSockOpt(TCP_NODELAY,&chOpt,sizeof(char),IPPROTO_TCP);
	}
}

// 点击【TCPServer】单选框
void CTCPIPDlg::OnBnClickedTcpServer()
{
	 UpdateData(TRUE);

	 if(!GlobalVar::s_bIsTcpServerStart)
	 {
		 if(m_nSelectedServiceType == 2)
		 {
			 m_BtnConnectAndDisconnect.EnableWindow(FALSE); 
			 m_RadioBtnStartTCPServer.EnableWindow(TRUE);
		 }
		 else
		 {
			 m_BtnConnectAndDisconnect.EnableWindow(TRUE); 
			 m_RadioBtnStartTCPServer.EnableWindow(FALSE);
		 }
	 }
}

// 点击【UDP】单选框
void CTCPIPDlg::OnBnClickedUdp()
{
	UpdateData(TRUE);

	m_BtnConnectAndDisconnect.EnableWindow(TRUE);    
	m_RadioBtnStartTCPServer.EnableWindow(FALSE);
}

// 点击【TCP Client】单选框
void CTCPIPDlg::OnBnClickedTcpClient()
{
	 UpdateData(TRUE);
	 m_BtnConnectAndDisconnect.EnableWindow(TRUE); 
	 m_RadioBtnStartTCPServer.EnableWindow(FALSE);
}

// 点击【创建TCPServer】按钮
void CTCPIPDlg::OnBnClickedTcpServerCreate()
{
	UpdateData(TRUE);

	// 开启服务器监听
	m_ListenSocket.Create(m_nLocalPort);
	if (m_ListenSocket.m_hSocket == INVALID_SOCKET)
	{
		GetSystemTime(&m_SystemTime);
		m_strListViewMsg.Format("%02d:%02d:%02d  建立TCP服务器失败 ！请检查端口是否被占用：Port = %d\n",
			m_SystemTime.wHour, m_SystemTime.wMinute, m_SystemTime.wSecond, m_nLocalPort);
		m_ListViewReceives.InsertString(0, m_strListViewMsg);
		return;
	}
	GlobalVar::s_eLinkMode = GlobalVar::eLinkMode_TCPServer;
	GlobalVar::s_strServiceTypeName = TCP_SERVER_NAME;
	m_RadioBtnStartTCPServer.EnableWindow(FALSE);
	m_BtnDeleteTCPServer.EnableWindow(TRUE);
	m_ListenSocket.Listen();
	GlobalVar::s_bIsTcpServerStart = true;
	const char chOpt = 1;
	m_DealTcpSocket.SetSockOpt(TCP_NODELAY,&chOpt,sizeof(char),IPPROTO_TCP);

	GetDlgItem(RADIO_UDP)->EnableWindow(FALSE);
	GetDlgItem(RADIO_TCP_Client)->EnableWindow(FALSE);
	GetDlgItem(RADIO_TCP_Server)->EnableWindow(TRUE);
	GetDlgItem(EDIT_My_Port)->EnableWindow(FALSE);
	GetDlgItem(EDIT_Link_Port)->EnableWindow(FALSE);
	GetDlgItem(EDIT_IP)->EnableWindow(FALSE);

	{
		GetSystemTime(&m_SystemTime);
		m_strListViewMsg.Format("%02d:%02d:%02d  建立TCP服务器成功 ！Port = %d\n",
			m_SystemTime.wHour, m_SystemTime.wMinute, m_SystemTime.wSecond, m_nLocalPort);
		m_ListViewReceives.InsertString(0, m_strListViewMsg);
	}
}

// 收到网络消息
LRESULT CTCPIPDlg::OnProcess_Receive(WPARAM wParam,LPARAM lParam)
{
	UpdateData(TRUE);

	if (m_nReceiveBufferLen < 1)
		return 0;

	GetSystemTime(&m_SystemTime);
	CString strLeftStr;
	if (!m_bIsReciveMsgWith16Bits)
	{
		m_strListViewMsg.Format("%02d:%02d:%02d  接收: ", 
			m_SystemTime.wHour, m_SystemTime.wMinute, m_SystemTime.wSecond);
		m_strListViewMsg = m_strListViewMsg + CString(m_pReceiveBuffer);
		int nStrRow = 1;
		nStrRow = (m_strListViewMsg).GetLength() / LIST_VIEW_ROW_CHAR_SIZE;
		nStrRow = nStrRow + 1;
		while (nStrRow != 0)
		{
			strLeftStr = m_strListViewMsg.Right((m_strListViewMsg).GetLength() - (nStrRow - 1) * LIST_VIEW_ROW_CHAR_SIZE);
			m_ListViewReceives.InsertString(0, strLeftStr);
			m_strListViewMsg = m_strListViewMsg.Left((nStrRow - 1) * LIST_VIEW_ROW_CHAR_SIZE);
			nStrRow--;
		}
	}
	else
	{
		if (m_nReceiveBufferLen != 0)
		{
			for (int k = 0; k < m_nReceiveBufferLen; k++)
			{
				strLeftStr = strLeftStr + __AscII2Hex(m_pReceiveBuffer[k]) + " ";
			}
			m_strListViewMsg.Format("%02d:%02d:%02d  接收: %s\n",
				m_SystemTime.wHour, m_SystemTime.wMinute, m_SystemTime.wSecond, strLeftStr);
			m_nReceiveBufferLen = 0;
		}
		m_ListViewReceives.InsertString(0, m_strListViewMsg);
	}
	return 0;
}

CString CTCPIPDlg::__AscII2Hex(char ch)
{
	char ch1;
	CString returnvalue;
	ch1=(ch>>4)&0x0f;
	returnvalue = __Hex2AscII(ch1);
	ch1=ch&0x0f;
	returnvalue = returnvalue + __Hex2AscII(ch1);
	return returnvalue;
}


unsigned char CTCPIPDlg::__Str2Byte(CString need_conver)
{
	int the_first=0;
	int the_second=0;
	switch (need_conver.GetAt(0))
	{
		case 0x30:	the_first=0;		break;
		case 0x31:	the_first=1;		break;
		case 0x32:	the_first=2;		break;
		case 0x33:	the_first=3;		break;
		case 0x34:	the_first=4;		break;
		case 0x35:	the_first=5;		break;
		case 0x36:	the_first=6;		break;
		case 0x37:  the_first=7;		break;
		case 0x38:  the_first=8;		break;
		case 0x39:  the_first=9;		break;
		case 0x41:  the_first=10;		break;
		case 0x42:  the_first=11;		break;
		case 0x43:  the_first=12;		break;
		case 0x44:  the_first=13;		break;
		case 0x45:  the_first=14;		break;
		case 0x46:  the_first=15;		break;
	}
	switch (need_conver.GetAt(1))
	{
		case 0x30:	the_second = 0;		break;
		case 0x31:	the_second = 1;		break;
		case 0x32:	the_second = 2;		break;
		case 0x33:	the_second = 3;		break;
		case 0x34:	the_second = 4;		break;
		case 0x35:	the_second = 5;		break;
		case 0x36:	the_second = 6;		break;
		case 0x37:  the_second = 7;		break;
		case 0x38:  the_second = 8;		break;
		case 0x39:  the_second = 9;		break;
		case 0x41:  the_second = 10;	break;
		case 0x42:  the_second = 11;	break;
		case 0x43:  the_second = 12;	break;
		case 0x44:  the_second = 13;	break;
		case 0x45:  the_second = 14;	break;
		case 0x46:  the_second = 15;	break;
	}
	return (the_first*16+the_second);
}

CString CTCPIPDlg::__Hex2AscII(char ch)
{
	CString rtvalue;
	switch(ch)
	{
		case 0:  rtvalue="0";break;
		case 1:  rtvalue="1";break;
		case 2:  rtvalue="2";break;
		case 3:  rtvalue="3";break;
		case 4:  rtvalue="4";break;
		case 5:  rtvalue="5";break;
		case 6:  rtvalue="6";break;
		case 7:  rtvalue="7";break;
		case 8:  rtvalue="8";break;
		case 9:  rtvalue="9";break;
		case 10: rtvalue="A";break;
		case 11: rtvalue="B";break;
		case 12: rtvalue="C";break;
		case 13: rtvalue="D";break;
		case 14: rtvalue="E";break;
		case 15: rtvalue="F";break;
	}
	return rtvalue;
}


void CTCPIPDlg::OnBnClickedButtonSend()
{
	UpdateData(TRUE);
	if (m_strSendMsg.IsEmpty() || m_strSendMsg == "")
	{
		GetSystemTime(&m_SystemTime);
		m_strListViewMsg.Format("%02d:%02d:%02d  请输入您要发送的数据 ！\n",
			m_SystemTime.wHour, m_SystemTime.wMinute, m_SystemTime.wSecond);
		m_ListViewReceives.InsertString(0, m_strListViewMsg);

		m_bISAutoSend = false;
		UpdateData(FALSE);
		KillTimer(WM_TIMER_ID);

		m_strListViewMsg.Format("%02d:%02d:%02d  已经关闭自动发送 ！\n", 
			m_SystemTime.wHour, m_SystemTime.wMinute, m_SystemTime.wSecond);
		m_ListViewReceives.InsertString(0, m_strListViewMsg);
		return;
	}

	GetSystemTime(&m_SystemTime);
	m_strSendMsg = m_strSendMsg + " ";//在准备发送的数据后面补空格
	if (m_bIsShowSendMsg)
	{
		m_strListViewMsg.Format("%02d:%02d:%02d  发送: %s", m_SystemTime.wHour, m_SystemTime.wMinute, m_SystemTime.wSecond, m_strSendMsg);
	}

	int nStrRow = 1;
	CString strLeftString;

	if(!m_bIsSendMsgWith16Bits)
	{
		nStrRow = (m_strListViewMsg).GetLength() / LIST_VIEW_ROW_CHAR_SIZE;
		nStrRow = nStrRow + 1;
		while (nStrRow != 0)
		{
			strLeftString = m_strListViewMsg.Right((m_strListViewMsg).GetLength() - (nStrRow - 1) * LIST_VIEW_ROW_CHAR_SIZE);
			if (m_bIsShowSendMsg == true)
			{
				m_ListViewReceives.InsertString(0, strLeftString);
			}
			m_strListViewMsg = m_strListViewMsg.Left((nStrRow - 1) * LIST_VIEW_ROW_CHAR_SIZE);
			nStrRow--;
		}
		if (GlobalVar::s_eLinkMode == GlobalVar::eLinkMode_TCPServer)
		{
			m_DealTcpSocket.Send(m_strSendMsg,m_strSendMsg.GetLength(),0);
		}
		else if(GlobalVar::s_eLinkMode == GlobalVar::eLinkMode_UDP)
		{
			UDP_Server.SendTo(m_strSendMsg,m_strSendMsg.GetLength(),m_nRemotePort,m_strIP,0);
		}
		else if(GlobalVar::s_eLinkMode == GlobalVar::eLinkMode_TCPClient)
		{
			m_ClientSocket.Send(m_strSendMsg,m_strSendMsg.GetLength(),0);
		}
	}
	else
	{
		int nSendBufferLen = 0;
		int nSendedBufferLen = 0;

		nStrRow = (m_strListViewMsg).GetLength() / LIST_VIEW_ROW_CHAR_SIZE;
		nStrRow = nStrRow + 1;
		while (nStrRow != 0)
		{
			strLeftString = m_strListViewMsg.Right((m_strListViewMsg).GetLength() - (nStrRow - 1)*LIST_VIEW_ROW_CHAR_SIZE);
			if (m_bIsShowSendMsg)
			{
				m_ListViewReceives.InsertString(0, strLeftString);
			}
			m_strListViewMsg = m_strListViewMsg.Left((nStrRow - 1)*LIST_VIEW_ROW_CHAR_SIZE);
			nStrRow--;
		}

		nSendBufferLen = m_strSendMsg.GetAllocLength();
		for (int i = 0; i < nSendBufferLen; i++)			// 去除前面的N个空格
		{
			if (m_strSendMsg.Left(1) == " ")
			{
				m_strSendMsg = m_strSendMsg.Right((m_strSendMsg.GetAllocLength()) - 1);
			}
		}
		/*
		while(m_strSendMsg.GetAllocLength() > 1)
		{
		nSendBufferLen = m_strSendMsg.GetAllocLength();	// 重新获得此字串的长度
		if (m_strSendMsg.Find(" ", 1) == 2)
		{
		GlobalVar::s_Temp_Send_Sigel[nSendedBufferLen] = m_strSendMsg.Left(2);
		m_strSendMsg = m_strSendMsg.Right((m_strSendMsg.GetAllocLength()) - 3);
		nSendedBufferLen++;
		}
		else
		{
		m_strSendMsg = m_strSendMsg.Right((m_strSendMsg.GetAllocLength()) - m_strSendMsg.Find(" ", 1));
		}
		}
		for (int i = 0; i<nSendedBufferLen; i++)
		{
		GlobalVar::s_SendDataBuffer[i] = __Str2Byte(GlobalVar::s_Temp_Send_Sigel[i]);
		}
		*/
		memset(GlobalVar::s_SendDataBuffer, 0, 4096);
		for (int i = 0; i < m_strSendMsg.GetAllocLength() - 1; ++i)
		{
			GlobalVar::s_SendDataBuffer[i] = m_strSendMsg.GetAt(i);
		}

		if(GlobalVar::s_eLinkMode == GlobalVar::eLinkMode_TCPServer)
		{
			if (m_DealTcpSocket.Send(GlobalVar::s_SendDataBuffer, nSendedBufferLen) != TRUE)
			{
				m_strListViewMsg.Format("%02d:%02d:%02d  发送失败 ！\n",
					m_SystemTime.wHour, m_SystemTime.wMinute, m_SystemTime.wSecond);
				m_ListViewReceives.InsertString(0, m_strListViewMsg);
			}
		}
		else if(GlobalVar::s_eLinkMode == GlobalVar::eLinkMode_UDP)
		{
			if (CTCPIPDlg::UDP_Server.SendTo(GlobalVar::s_SendDataBuffer, nSendedBufferLen, m_nRemotePort, m_strIP, 0) != TRUE)			
			{
				m_strListViewMsg.Format("%02d:%02d:%02d  发送失败 ！\n",
					m_SystemTime.wHour, m_SystemTime.wMinute, m_SystemTime.wSecond);
				m_ListViewReceives.InsertString(0, m_strListViewMsg);
			}
		}
		else if(GlobalVar::s_eLinkMode == GlobalVar::eLinkMode_TCPClient)
		{
			if(CTCPIPDlg::m_ClientSocket.Send(GlobalVar::s_SendDataBuffer, nSendedBufferLen, 0) != TRUE)
			{
				m_strListViewMsg.Format("%02d:%02d:%02d  发送失败 ！\n",
					m_SystemTime.wHour, m_SystemTime.wMinute, m_SystemTime.wSecond);
				m_ListViewReceives.InsertString(0, m_strListViewMsg);
			}
		}
	}
}

// 点击【删除TCP服务器】按钮
void CTCPIPDlg::OnBnClickedButtonDeleteTcpserver()
{
	m_DealTcpSocket.Close();
	m_ListenSocket.Close();
	GlobalVar::s_bIsTcpServerStart = false;

	m_RadioBtnStartTCPServer.EnableWindow(TRUE);
	m_BtnDeleteTCPServer.EnableWindow(FALSE);
    GetDlgItem(RADIO_UDP)->EnableWindow(TRUE);
	GetDlgItem(RADIO_TCP_Client)->EnableWindow(TRUE);
    GetDlgItem(RADIO_TCP_Server)->EnableWindow(TRUE);
	GetDlgItem(EDIT_My_Port)->EnableWindow(TRUE);
    GetDlgItem(EDIT_Link_Port)->EnableWindow(TRUE);
	GetDlgItem(EDIT_IP)->EnableWindow(TRUE);

	{
		GetSystemTime(&m_SystemTime);
		m_strListViewMsg.Format("%02d:%02d:%02d  已删除TCP服务器 ！\n",
			m_SystemTime.wHour, m_SystemTime.wMinute, m_SystemTime.wSecond);
		m_ListViewReceives.InsertString(0, m_strListViewMsg);
	}
}

// 点击【清空接收框】按钮
void CTCPIPDlg::OnBnClickedButtonClearRecived()
{
	m_ListViewReceives.ResetContent();
}

// 点击【清空发送框】按钮
void CTCPIPDlg::OnBnClickedButtonClearSend()
{
	m_ListViewSends.SetWindowText(_T(""));	
}

// 点击【关闭】
void CTCPIPDlg::OnClose()
{
	if (IDYES != MessageBox("是否要关闭程序", "提示", MB_YESNO))
		return;

	UpdateData(TRUE);

	CString C_Local_port;
	CString C_Link_port;
	CString C_Link_IP_Address;
	CString C_16_Send;
	CString C_16_Receive;
	CString C_Protocal;
	CString C_Send_Time;
	CString C_Show_Send_Message;
	CString C_Whether_Auto_Send;
	
	if(!m_bISAutoSend)
	{
		C_Whether_Auto_Send="0";
	}
	else
	{
		C_Whether_Auto_Send="1";
	}
	if(!m_bIsReciveMsgWith16Bits)
	{
		C_16_Receive="0";
	}
	else
	{
		C_16_Receive="1";
	}
	
	if(m_nSelectedServiceType==0)
	{
		C_Protocal = UDP_NAME;
	}
	else if(m_nSelectedServiceType==1)
	{
		C_Protocal = TCP_CLIENT_NAME;
	}
	else if(m_nSelectedServiceType==2)
	{
		C_Protocal = TCP_SERVER_NAME;
	}
	
	if(!m_bIsSendMsgWith16Bits)
	{
		C_16_Send="0";
	}
	else
	{
		C_16_Send="1";
	}
	if(!m_bIsShowSendMsg)
	{
		C_Show_Send_Message="0";
	}
	else
	{
		C_Show_Send_Message="1";
	}
	GetDlgItem(EDIT_My_Port)->GetWindowTextA(C_Local_port);
	GetDlgItem(EDIT_Link_Port)->GetWindowTextA(C_Link_port);
	GetDlgItem(EDIT_IP)->GetWindowTextA(C_Link_IP_Address);
	GetDlgItem(IDC_EDIT_Send_Time)->GetWindowTextA(C_Send_Time);
	
	WritePrivateProfileString("Setting","LocalPort",			C_Local_port,			DEFAULT_CONFIG_FILE);
	WritePrivateProfileString("Setting","RemotePort",			C_Link_port,			DEFAULT_CONFIG_FILE);
	WritePrivateProfileString("Setting","RemoteIP",				C_Link_IP_Address,		DEFAULT_CONFIG_FILE);
	WritePrivateProfileString("Setting","SendStyle",			C_16_Send,				DEFAULT_CONFIG_FILE);
	WritePrivateProfileString("Setting","ReceiveStyle",			C_16_Receive,			DEFAULT_CONFIG_FILE);
	WritePrivateProfileString("Setting","DefaultProtocalType",	C_Protocal,				DEFAULT_CONFIG_FILE);
	WritePrivateProfileString("Setting", "AutoSendTime",		C_Send_Time,			DEFAULT_CONFIG_FILE);
	WritePrivateProfileString("Setting", "ShowSendMSG",			C_Show_Send_Message,	DEFAULT_CONFIG_FILE);
	
	if (m_pStatusBar != nullptr)
	{
		 delete m_pStatusBar;
		 m_pStatusBar = nullptr;
	}
	
	// 父类的关闭
	CDialogEx::OnClose();
}

// 点击【自动发送按钮】复选框
void CTCPIPDlg::OnBnClickedAutoSend()
{
	UpdateData(TRUE);

	if(!m_bISAutoSend)
	{
		KillTimer(WM_TIMER_ID);
		GetSystemTime(&m_SystemTime);
		m_strListViewMsg.Format("%02d:%02d:%02d  已经关闭自动发送 ！\n",
			m_SystemTime.wHour,m_SystemTime.wMinute,m_SystemTime.wSecond);
		m_ListViewReceives.InsertString(0,m_strListViewMsg);
	}
	else
	{
		SetTimer(WM_TIMER_ID, m_nAutoSendInterval, 0);
		GetSystemTime(&m_SystemTime);
		m_strListViewMsg.Format("%02d:%02d:%02d  已经开启自动发送 ！\n",
			m_SystemTime.wHour,m_SystemTime.wMinute,m_SystemTime.wSecond);
		m_ListViewReceives.InsertString(0,m_strListViewMsg);
	}
}

// 修改发送时间Edit
void CTCPIPDlg::OnEnChangeEditSendTime()
{
	if (m_bISAutoSend)
	{
		UpdateData(TRUE);
	}
}

// 定时器触发事件
void CTCPIPDlg::OnTimer(UINT_PTR nIDEvent)
{
	OnBnClickedButtonSend();
	CDialogEx::OnTimer(nIDEvent);
}

// 更改【发送数据是否显示】的ComboBox
void CTCPIPDlg::OnCbnSelchangeCombo1View()
{
	CString strText;
	int nIndex = m_ComboBoxMsgShowType.GetCurSel();
	m_ComboBoxMsgShowType.GetLBText( nIndex, strText); 
	if(strText=="不显示发送数据")
	{
	   m_bIsShowSendMsg = false;
	}
	else if(strText=="显示发送的数据")
	{
       m_bIsShowSendMsg = true;
	}
}

// 点击【访问主页】按钮
void CTCPIPDlg::OnBnClickedButtonHtml()
{
	ShellExecute(this->m_hWnd,"open","http://duzhi5368.blog.163.com/","","", SW_SHOW ); 
}
