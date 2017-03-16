
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
	CString			m_strListViewMsg;			// ����б��е���Ϣ
	CString			m_strSendMsg;				// �ȴ����͵���Ϣ
	unsigned char	m_pReceiveBuffer[4096];		// ���յ�����Ϣ
	int				m_nReceiveBufferLen;		// ���յ�����Ϣ����
	SYSTEMTIME		m_SystemTime;				// ϵͳʱ��

	CString			m_strIP;					// ����ȥ���ӵ�Զ��IP
	int				m_nRemotePort;				// Զ�����Ӷ˿�
	int				m_nLocalPort;				// ���ؼ����˿�
	int				m_nSelectedServiceType;		// ��ǰѡ��ķ������� - UDP/TCP S/TCP C
	BOOL			C_16_scale_Receive;
	BOOL			m_bIsSendMsgWith16Bits;		// �Ƿ�16��ֹ������Ϣ
	BOOL			m_bIsReciveMsgWith16Bits;	// �Ƿ�16���ƽ�����Ϣ
	BOOL			m_bISAutoSend;				// �Ƿ��Զ�����
	int				m_nAutoSendInterval;		// �Զ��������ʱ��
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
	CButton					m_BtnConnectAndDisconnect;	// ����/�Ͽ� ��ť
	CButton					m_RadioBtnStartTCPServer;	// ������������ť
	CButton					m_BtnDeleteTCPServer;		// ɾ����������ť
	CListBox				m_ListViewReceives;			// ��Ϣ���մ���
	CEdit					m_ListViewSends;			// ��Ϣ���ʹ���
public:
	afx_msg void			OnBnClickedButtonLink();							// ���������/�Ͽ�����ť
	afx_msg void			OnBnClickedTcpServer();								// �����TCPServer����ѡ��
	afx_msg void			OnBnClickedUdp();									// �����UDP����ѡ��
	afx_msg void			OnBnClickedTcpClient();								// �����TCPClient����ѡ��
	afx_msg void			OnBnClickedTcpServerCreate();						// ���������TCP����������ť
	afx_msg void			OnBnClickedButtonSend();							// ��������͡���ť
	afx_msg void			OnBnClickedButtonDeleteTcpserver();					// �����ɾ��TCP����������ť
	afx_msg void			OnBnClickedButtonClearRecived();					// �������ս��տ򡿰�ť
	afx_msg void			OnBnClickedButtonClearSend();						// �������շ��Ϳ򡿰�ť
	afx_msg void			OnClose();											// ������رա�
	afx_msg void			OnBnClickedAutoSend();								// ������Զ����Ͱ�ť����ѡ��
	afx_msg void			OnEnChangeEditSendTime();							// �޸ķ���ʱ��Edit
	afx_msg void			OnCbnSelchangeCombo1View();							// ���ġ����������Ƿ���ʾ����ComboBox
	afx_msg void			OnBnClickedButtonHtml();							// �����������ҳ����ť

	afx_msg void			OnTimer(UINT_PTR nIDEvent);							// ��ʱ�������¼�
	afx_msg	LRESULT			OnProcess_Receive(WPARAM wParam, LPARAM lParam);	// �յ�������Ϣ
private:
	CString					__AscII2Hex(char ch);
	CString					__Hex2AscII(char ch);
	unsigned char			__Str2Byte(CString need_conver);
};
