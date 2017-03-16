
// TCPIPDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "FanceSocket.h"

// CTCPIPDlg dialog
class CTCPIPDlg : public CDialogEx
{
// Construction
public:
	FanceSocket		m_ListenSocket;
	FanceSocket		m_DealTcpSocket;
	FanceSocket		m_ClientSocket;
	FanceSocket		UDP_Server;
	bool			m_bIsShowSendMsg;
	CString			m_strListViewMsg;			// 输出列表中的消息
	CString			m_strSendMsg;				// 等待发送的消息
	unsigned char	m_pReceiveBuffer[4096];		// 接收到的消息
	int				m_nReceiveBufferLen;		// 接收到的消息长度
	SYSTEMTIME		m_SystemTime;				// 系统时间

	CString			m_strIP;					// 尝试去连接的远程IP
	int				m_nRemotePort;				// 远程连接端口
	int				m_nLocalPort;				// 本地监听端口
	int				m_nSelectedServiceType;		// 当前选择的服务类型 - UDP/TCP S/TCP C
	BOOL			C_16_scale_Receive;
	BOOL			m_bIsSendMsgWith16Bits;		// 是否16禁止发送消息
	BOOL			m_bIsReciveMsgWith16Bits;	// 是否16进制接收消息
	BOOL			m_bISAutoSend;				// 是否自动发包
	int				m_nAutoSendInterval;		// 自动发包间隔时间
public:
	CTCPIPDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_TCPIP_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL			OnInitDialog();
	afx_msg void			OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void			OnPaint();
	afx_msg HCURSOR			OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CComboBox				m_ComboBoxMsgShowType;
	CStatusBarCtrl*			m_pStatusBar;
	CButton					m_BtnConnectAndDisconnect;	// 连接/断开 按钮
	CButton					m_RadioBtnStartTCPServer;	// 启动服务器按钮
	CButton					m_BtnDeleteTCPServer;		// 删除服务器按钮
	CListBox				m_ListViewReceives;			// 消息接收窗口
	CEdit					m_ListViewSends;			// 消息发送窗口
public:
	afx_msg void			OnBnClickedButtonLink();							// 点击【连接/断开】按钮
	afx_msg void			OnBnClickedTcpServer();								// 点击【TCPServer】单选框
	afx_msg void			OnBnClickedUdp();									// 点击【UDP】单选框
	afx_msg void			OnBnClickedTcpClient();								// 点击【TCPClient】单选框
	afx_msg void			OnBnClickedTcpServerCreate();						// 点击【创建TCP服务器】按钮
	afx_msg void			OnBnClickedButtonSend();							// 点击【发送】按钮
	afx_msg void			OnBnClickedButtonDeleteTcpserver();					// 点击【删除TCP服务器】按钮
	afx_msg void			OnBnClickedButtonClearRecived();					// 点击【清空接收框】按钮
	afx_msg void			OnBnClickedButtonClearSend();						// 点击【清空发送框】按钮
	afx_msg void			OnClose();											// 点击【关闭】
	afx_msg void			OnBnClickedAutoSend();								// 点击【自动发送按钮】复选框
	afx_msg void			OnEnChangeEditSendTime();							// 修改发送时间Edit
	afx_msg void			OnCbnSelchangeCombo1View();							// 更改【发送数据是否显示】的ComboBox
	afx_msg void			OnBnClickedButtonHtml();							// 点击【访问主页】按钮

	afx_msg void			OnTimer(UINT_PTR nIDEvent);							// 定时器触发事件
	afx_msg	LRESULT			OnProcess_Receive(WPARAM wParam, LPARAM lParam);	// 收到网络消息
private:
	CString					__AscII2Hex(char ch);
	CString					__Hex2AscII(char ch);
	unsigned char			__Str2Byte(CString need_conver);
};
