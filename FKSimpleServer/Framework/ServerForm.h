#pragma once
//------------------------------------------------------------------------
#include "../Depend/EasyGUI/easygui.h"
#include "../Utils/IOConsole.h"
#include "BaseServer.h"
//------------------------------------------------------------------------
// ����������Win��Ϣ
#define IDM_STARTSERVER	10000
// �رշ�����Win��Ϣ
#define IDM_STOPSERVER	10001
//------------------------------------------------------------------------
// �ؼ�ID
#define ID_CHAT			20000
#define ID_TOOLBAR		20001
#define ID_STATUEBAR	20002
#define ID_INPUT		20003
#define IDBTN_INPUT		20004
//------------------------------------------------------------------------
// ��ʱ���¼�ID
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
	// ȷ�����
	bool			ConfirmMessageBox(TCHAR * pszCaption, TCHAR * pszText);
	// �̳��Ը��࣬�����ַ����
	virtual void	OutPutStatic(DWORD dwColor, const TCHAR * pszString);
	// ��һ��Windows��Ϣ����
	BOOL			FilterMessage(CEGBaseWnd * pWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	// �ڶ���Windows��Ϣ����
	BOOL			OnMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	// ����Windows��Ϣѭ��
	int				EnterMessageLoop();
	// �����Ľ��������
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
	CEGLabel		m_lbText01;	//	������
	CEGLabel		m_lbText02;	//	���Ӵ���
	CEGLabel		m_lbText03;	//	�Ͽ�����
	CEGLabel		m_lbText04;	//	�����ֽ���
	CEGLabel		m_lbText05;	//	�����ֽ���
	CEGLabel		m_lbText06;	//	ѭ��ʱ��
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