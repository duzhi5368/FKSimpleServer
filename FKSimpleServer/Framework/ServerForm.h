#pragma once
//------------------------------------------------------------------------
#include "../Depend/EasyGUI/easygui.h"
#include "../Utils/IOConsole.h"
#include "BaseServer.h"
//------------------------------------------------------------------------
// 启动服务器Win消息
#define IDM_STARTSERVER	10000
// 关闭服务器Win消息
#define IDM_STOPSERVER	10001
//------------------------------------------------------------------------
// 控件ID
#define ID_CHAT			20000
#define ID_TOOLBAR		20001
#define ID_STATUEBAR	20002
#define ID_INPUT		20003
#define IDBTN_INPUT		20004
//------------------------------------------------------------------------
// 定时器事件ID
#define ID_STATUETIMER	30005
//------------------------------------------------------------------------
class CProgressForm;
class CStdProgressForm;
//------------------------------------------------------------------------
struct SServerConfigInfo
{
	SServerConfigInfo()
		: m_bShowConfirm( true )
	{
	}
	bool			m_bShowConfirm;
};
//------------------------------------------------------------------------
class CServerForm :
	public CEGForm, public CIOConsole, public CEGWndMsgFilter
{
public:
	CServerForm(void);
	CServerForm(const TCHAR* pszTitle);
	CServerForm(const TCHAR* pszTitle, const TCHAR* pszCmdLine);
	virtual ~CServerForm(void);
public:
	BOOL			OnClose();
	VOID			OnCreate(HWND hWnd);
	void			OnCommand(UINT id);
	virtual bool	StartServer();
	virtual BOOL	StopServer();
	virtual VOID	OnTimer();
	CBaseServer *	GetServer();
	void			SetServer(CBaseServer * pServer);
	// 确定面板
	bool			ConfirmMessageBox(TCHAR * pszCaption, TCHAR * pszText);
	// 继承自父类，进行字符输出
	virtual void	OutPutStatic(DWORD dwColor, const TCHAR * pszString);
	// 第一道Windows消息处理
	BOOL			FilterMessage(CEGBaseWnd * pWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	// 第二道Windows消息处理
	BOOL			OnMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	// 进入Windows消息循环
	int				EnterMessageLoop();
	// 弹出的进度条面板
	CProgressForm * GetProgressForm(){ return m_pProgressForm; }
private:
	bool			Init(const TCHAR* pszTitle, const TCHAR * pszCmdLine = nullptr);
	bool			InitForm();
	bool			InitProgressWnd();
	bool			InitToolbar(UINT nBitmapId);
	bool			InitStatueBar();

	void			MakeAllFont();

	void			DoExeCmdLineParam();
	void			DumpConfigInfo();
protected:
	HWND			m_hStatueBar;
	HWND			m_hToolsBar;
	CEGRichEdit		m_reChat;
	CEGEditBox		m_edInput;
	CEGPushButton	m_btnInput;
	CEGButton		m_grp01;
	CBaseServer*	m_pServer;
	CEGLabel		m_lbType;
	CEGLabel		m_lbText01;	//	连接数
	CEGLabel		m_lbText02;	//	连接次数
	CEGLabel		m_lbText03;	//	断开次数
	CEGLabel		m_lbText04;	//	发送字节数
	CEGLabel		m_lbText05;	//	接收字节数
	CEGLabel		m_lbText06;	//	循环时间
	CEGLabel		m_lbConnections;
	CEGLabel		m_lbConnectedTimes;
	CEGLabel		m_lbDisconnectedTimes;
	CEGLabel		m_lbRecvBytes;
	CEGLabel		m_lbSendBytes;
	CEGLabel		m_lbLooptime;
	CEGListBox		m_lbWorkThread;
	UINT			m_iThroughCount;
	TCHAR *			m_pCmdLine;
	CProgressForm * m_pProgressForm;
	TCHAR *			m_pszServerName;
	SServerConfigInfo	m_tagConfigInfo;
};
//------------------------------------------------------------------------
extern CServerForm* g_pSvrForm;
//------------------------------------------------------------------------