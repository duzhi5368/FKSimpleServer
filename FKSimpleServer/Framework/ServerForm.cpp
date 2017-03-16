#include "ServerForm.h"
#include "../Utils/NetFunc.h"
#include "../Base/BaseMacro.h"
#include "../Utils/ServerTimer.h"
#include "../Utils/LogFile.h"
#include "../Base/ServerConfig.h"
#include "../Utils/StringFunc.h"
#include "../res/resource.h"
//-------------------------------------------------------------
CServerForm* g_pSvrForm = nullptr;
//-------------------------------------------------------------
CServerForm::CServerForm(void)
{
	m_pServer = NULL;
	m_pCmdLine = NULL;
	NetStartup(2, 2);
	m_iThroughCount = 0;
	this->m_pProgressForm = NULL;

	CLogFile::GetInstance()->Init(DEFAULE_SERVER_LOG_DIR);
	Init(DEFAULT_SERVER_NAME);
}
//------------------------------------------------------------------------
CServerForm::CServerForm(const TCHAR* pszTitle)
{
	m_pServer = NULL;
	m_pCmdLine = NULL;
	NetStartup(2, 2);
	m_iThroughCount = 0;
	this->m_pProgressForm = NULL;

	CLogFile::GetInstance()->Init(DEFAULE_SERVER_LOG_DIR);
	Init(pszTitle);
}
//------------------------------------------------------------------------
CServerForm::CServerForm(const TCHAR* pszTitle, const TCHAR* pszCmdLine)
{
	m_pServer = NULL;
	m_pCmdLine = NULL;
	NetStartup(2, 2);
	m_iThroughCount = 0;
	this->m_pProgressForm = NULL;

	CLogFile::GetInstance()->Init(DEFAULE_SERVER_LOG_DIR);
	INFO_MSG_LOG_WITH_TIME(L"============================================================");
	Init(pszTitle, pszCmdLine);
}
//------------------------------------------------------------------------
CServerForm::~CServerForm(void)
{
	NetCleanup();
}
//------------------------------------------------------------------------
bool CServerForm::Init(const TCHAR * pszTitle, const TCHAR * pszCmdLine)
{
	m_pszServerName = (TCHAR*)pszTitle;
	m_pCmdLine = (TCHAR*)pszCmdLine;
	bool bRet = true;
	if (!InitForm())
	{
		bRet = false;
	}
	if (!InitProgressWnd())
	{
		bRet = false;
	}
	if (!InitToolbar(IDB_TOOLBAR))
	{
		bRet = false;
	}
	if (!InitStatueBar())
	{
		bRet = false;
	}

	if (bRet)
	{
		INFO_MSG_LOG_WITH_TIME(L"服务器窗口初始化完成.");
	}
	return bRet;
}
//------------------------------------------------------------------------
VOID CServerForm::OnCreate(HWND hWnd)
{
	RECT winrc;
	GetWindowRect(hWnd, &winrc);
	int winwidth = winrc.right - winrc.left;

	int	top = 50;
	DWORD	style1 = SS_CENTER, style2 = SS_CENTER;
	int	x = winwidth - 130;
	int width = 92;

	//	控制台RichEdit控件
	m_reChat.Create(hWnd, ES_READONLY |ES_NOHIDESEL | ES_WANTRETURN | WS_BORDER, 30, 30, 609, 490, 0);
	m_reChat.SetId(ID_CHAT);
	m_reChat.SetBkGndColor(RGB(250, 219, 217));

	//	控制台输入控件
	m_edInput.Create(hWnd, WS_BORDER | ES_WANTRETURN, 30, 524, 539, 20, L"");
	m_edInput.SetId(ID_INPUT);

	//	输入按钮
	m_btnInput.Create(hWnd, L"发送", 0, 571, 524, 70, 20);
	m_btnInput.SetId(IDBTN_INPUT);

	//	组
	m_grp01.Create(hWnd, BS_GROUPBOX, winwidth - 132, 30, 102, 510, L"状态", 0);

	//	labels
	m_lbType.Create(hWnd, style2, x, top, width, 15, m_pszServerName);
	m_lbText01.Create(hWnd, style1, x, (top += 20), width, 15, L"当前活动连接");
	m_lbConnections.Create(hWnd, style2, x, (top += 20), width, 15, L"0");
	m_lbText02.Create(hWnd, style1, x, (top += 20), width, 15, L"累计接受连接");		//	连接次数
	m_lbConnectedTimes.Create(hWnd, style2, x, (top += 20), width, 15, L"0");
	m_lbText03.Create(hWnd, style1, x, (top += 20), width, 15, L"累计断开次数");		//	断开次数
	m_lbDisconnectedTimes.Create(hWnd, style2, x, (top += 20), width, 15, L"0");
	m_lbText04.Create(hWnd, style1, x, (top += 20), width, 15, L"累计发送字节");		//	发送字节数
	m_lbSendBytes.Create(hWnd, style2, x, (top += 20), width, 15, L"0");
	m_lbText05.Create(hWnd, style1, x, (top += 20), width, 15, L"累计接收字节");		//	接收字节数
	m_lbRecvBytes.Create(hWnd, style2, x, (top += 20), width, 15, L"0");
	m_lbText06.Create(hWnd, style1, x, (top += 20), width, 15, L"循环时间");			//	循环时间
	m_lbLooptime.Create(hWnd, style2, x, (top += 20), width, 15, L"0.0 ms");

	m_edInput.SetMsgFilter(this);

	HDC	dc = GetDC();
	ReleaseDC(dc);
	MakeAllFont();
	GetTickCount();
}
//------------------------------------------------------------------------
void CServerForm::MakeAllFont()
{
	LOGFONT LogFont;
	ZeroMemory(&LogFont, sizeof(LogFont));
	LogFont.lfHeight = -13;
	LogFont.lfWidth = 0;
	LogFont.lfEscapement = 0;
	LogFont.lfOrientation = 0;
	LogFont.lfWeight = FW_NORMAL;
	LogFont.lfItalic = FALSE;
	LogFont.lfUnderline = FALSE;
	LogFont.lfStrikeOut = FALSE;
	LogFont.lfCharSet = GB2312_CHARSET;
	LogFont.lfOutPrecision = 3;
	LogFont.lfClipPrecision = 2;
	LogFont.lfQuality = 1;
	LogFont.lfPitchAndFamily = 2;
	wcscpy(LogFont.lfFaceName, L"宋体");

	HFONT	hFont = CreateFontIndirect(&LogFont);
	SetFont(hFont);
}
//------------------------------------------------------------------------
void CServerForm::DoExeCmdLineParam()
{
	if (_tcsicmp(m_pCmdLine, L"start") == 0)
	{
		OnCommand(IDM_STARTSERVER);
	}
}
//------------------------------------------------------------------------
void CServerForm::DumpConfigInfo()
{
	INFO_MSG_LOG(L"==================[配置文件信息]==================");
	INFO_MSG_LOG(L"    关闭服务器前确认 = %d", (int)m_tagConfigInfo.m_bShowConfirm);
	INFO_MSG_LOG(L"==================[配置文件信息]==================");
}
//------------------------------------------------------------------------
bool CServerForm::InitToolbar(UINT nBitmapId)
{
	TBBUTTON	buttons[2];
	ZeroMemory(buttons, sizeof(buttons));
	buttons[0].iBitmap = 0;
	buttons[0].idCommand = IDM_STARTSERVER;
	buttons[0].iString = 0;
	buttons[0].fsStyle = TBSTYLE_BUTTON;
	buttons[0].fsState = TBSTATE_ENABLED;
	buttons[1].iBitmap = 1;
	buttons[1].idCommand = IDM_STOPSERVER;
	buttons[1].iString = 0;
	buttons[1].fsStyle = TBSTYLE_BUTTON;
	buttons[1].fsState = 0;

	HINSTANCE	hInst = GetModuleHandle(NULL);
	if ( hInst == NULL)
	{
		ERROR_MSG_LOG_WITH_TIME(L"创建工具栏失败, 错误码 = %d", GetLastError());
		return false;
	}

	m_hToolsBar = CreateToolbarEx(m_hWnd, WS_CHILD | CCS_TOP | WS_VISIBLE | TBSTYLE_TOOLTIPS, ID_TOOLBAR,
		2, hInst, (nBitmapId), buttons, 2, 16, 16, 16, 16, sizeof(TBBUTTON));
	if (m_hToolsBar == NULL)
	{
		ERROR_MSG_LOG_WITH_TIME(L"创建工具栏失败, 错误码 = %d", GetLastError());
		return false;
	}
	return true;
}
//------------------------------------------------------------------------
bool CServerForm::InitForm()
{
	BOOL b = CEGForm::Create(GetDesktopWindow(), 40, 40, 800, 610, WS_OVERLAPPED, WS_MAXIMIZEBOX | WS_THICKFRAME, 
		m_pszServerName, SW_SHOW, NULL, NULL, NULL, NULL,
		(HBRUSH)(COLOR_BTNFACE + 1));
	if (b != TRUE)
	{
		ERROR_MSG_LOG_WITH_TIME(L"创建服务器窗口失败，错误码 = %d", GetLastError());
		return false;
	}
	return true;
}
//------------------------------------------------------------------------
bool CServerForm::InitProgressWnd()
{
	m_pProgressForm = new CProgressForm;
	if (m_pProgressForm->Create(m_hWnd, L"默认Form", L"Loading", TRUE) == FALSE)
	{
		ERROR_MSG_LOG_WITH_TIME(L"创建进度条窗口失败，错误码 = %d", GetLastError());
		return false;
	}
	return true;
}
//------------------------------------------------------------------------
bool CServerForm::InitStatueBar()
{
	m_hStatueBar = CreateStatusWindow(WS_CHILD | WS_VISIBLE, NULL, m_hWnd, ID_STATUEBAR);
	if (m_hStatueBar == NULL)
	{
		ERROR_MSG_LOG_WITH_TIME(L"创建状态栏失败, 错误码 = %d", GetLastError());
		return false;
	}
	return true;
}
//------------------------------------------------------------------------
VOID CServerForm::OnCommand(UINT id)
{
	switch (id)
	{
		// 启动服务器
	case	IDM_STARTSERVER:
	{
		SendMessage(m_hToolsBar, TB_ENABLEBUTTON, (WPARAM)IDM_STARTSERVER, (LPARAM)MAKELONG(FALSE, 0));
		if (!StartServer())
		{
			SendMessage(m_hToolsBar, TB_ENABLEBUTTON, (WPARAM)IDM_STARTSERVER, (LPARAM)MAKELONG(TRUE, 0));
		}
		else
		{
			SetTimer(m_hWnd, ID_STATUETIMER, 1000, NULL);
			SendMessage(m_hToolsBar, TB_ENABLEBUTTON, (WPARAM)IDM_STOPSERVER, (LPARAM)MAKELONG(TRUE, 0));
		}
	}
	break;
	// 停止服务器
	case	IDM_STOPSERVER:
	{
		SendMessage(m_hToolsBar, TB_ENABLEBUTTON, (WPARAM)IDM_STOPSERVER, (LPARAM)MAKELONG(FALSE, 0));
		if (!StopServer())
		{
			SendMessage(m_hToolsBar, TB_ENABLEBUTTON, (WPARAM)IDM_STOPSERVER, (LPARAM)MAKELONG(TRUE, 0));
		}
		else
		{
			KillTimer(m_hWnd, ID_STATUETIMER);
			SendMessage(m_hToolsBar, TB_ENABLEBUTTON, (WPARAM)IDM_STARTSERVER, (LPARAM)MAKELONG(TRUE, 0));
		}
	}
	break;
	case IDBTN_INPUT:
	{
		TCHAR	szText[1024];
		int count = m_edInput.GetText(szText, sizeof(szText));
		if (count > 0)
		{
			m_pInputListener->OnInput(szText);
		}
		m_edInput.SetText(L"");
	}
	break;
	}
}
//------------------------------------------------------------------------
bool CServerForm::StartServer()
{
	if (m_pServer != NULL)
	{
		CSettingFile sf;
		if (!sf.Open(DEFAULE_SERVER_CONFIG_FILE))
		{
			ERROR_MSG_LOG_WITH_TIME(L"启动服务器失败：无法打开配置文件%s.", _T(DEFAULE_SERVER_CONFIG_FILE));
			return false;
		}
		m_tagConfigInfo.m_bShowConfirm = (sf.GetInteger(NULL, "是否关闭前提示", 0) != 0);
		DumpConfigInfo();
		return m_pServer->Start(sf);
	}
	ERROR_MSG_LOG_WITH_TIME(L"启动服务器失败：%s", L"服务器指针没有初始化");
	return false;
}
//------------------------------------------------------------------------
BOOL CServerForm::StopServer()
{
	if (m_pServer != NULL)
		return m_pServer->Stop();
	return TRUE;
}
//------------------------------------------------------------------------
BOOL CServerForm::OnMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case	WM_TIMER:
		{
			OnTimer();
		}
		break;
	}
	return CEGForm::OnMessage(hWnd, uMsg, wParam, lParam);
}
//------------------------------------------------------------------------
VOID CServerForm::OnTimer()
{
	m_iThroughCount++;

	SERVERSTATE	serverstate;
	char * pServerName = NULL;
	if (m_pServer != NULL)
	{
		m_pServer->GetServerState(serverstate);
	}
	TCHAR	text[256];
	wsprintf(text, L"[%s]%s-%s", L"FreeKnight", m_pszServerName, serverstate.pServerDescript);
	SetText(text);

	m_lbConnections.printf(L"%d", serverstate.numConnection);
	m_lbConnectedTimes.printf(L"%d", serverstate.numConnected);
	m_lbDisconnectedTimes.printf(L"%d", serverstate.numDisconnected);

	// windows的wsvprintf不支持%f...我靠
	char	loopTime[16];
	sprintf(loopTime, "%f", serverstate.fLoopTime);
	m_lbLooptime.printf(L"%s ms", MBytesToWString(loopTime).c_str());
	CServerTimer::SetTimeJump(((int)(serverstate.fLoopTime + 0.5f)) + 5);
	m_lbRecvBytes.printf(L"%u", serverstate.dwRecvBytes);
	m_lbSendBytes.printf(L"%u", serverstate.dwSendBytes);
}
//------------------------------------------------------------------------
bool CServerForm::ConfirmMessageBox(TCHAR * pszCaption, TCHAR * pszText)
{
	if (m_hWnd == NULL)
		return false;
	if (ConfirmBox(m_hWnd, pszCaption, pszText) == FALSE)
		return false;
	return true;
}
//------------------------------------------------------------------------
void CServerForm::OutPutStatic(DWORD dwColor, const TCHAR * pszString)
{
	if (NULL == pszString)
		return;

	SYSTEMTIME st;
	GetLocalTime(&st);
	TCHAR buf[4096];
	wsprintf(buf, L"%04d-%02d-%02d %02d:%02d:%02d ", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

	wcscat(buf, pszString);

	m_reChat.printf(dwColor, L"%s\n", buf);
}
//------------------------------------------------------------------------
CBaseServer* CServerForm::GetServer()
{ 
	return m_pServer; 
}
//------------------------------------------------------------------------
void CServerForm::SetServer(CBaseServer * pServer)
{ 
	INFO_MSG_LOG_WITH_TIME(L"设置服务器对象内核完成.");
	m_pServer = pServer; 
}
//------------------------------------------------------------------------
BOOL CServerForm::FilterMessage(CEGBaseWnd * pWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (pWnd->GetId() == ID_INPUT)
	{
		if (uMsg == WM_KEYDOWN && wParam == VK_RETURN)
		{
			TCHAR	szText[1024];
			int count = m_edInput.GetText(szText, sizeof(szText));
			if (count > 0)
			{
				m_pInputListener->OnInput(szText);
			}
			m_edInput.SetText(L"");
		}
	}
	return FALSE;
}
//------------------------------------------------------------------------
int CServerForm::EnterMessageLoop()
{
	DoExeCmdLineParam();
	return CEGForm::EnterMessageLoop();
}
//------------------------------------------------------------------------
BOOL CServerForm::OnClose()
{
	if (m_tagConfigInfo.m_bShowConfirm)
	{
		if (!ConfirmMessageBox(L"请确认", L"您真的要关闭服务器么？"))
			return FALSE;
	}
	INFO_MSG_LOG_WITH_TIME(L"================== [关闭服务器] ==================");
	if (m_pServer)
	{
		m_pServer->SubmitTerminateRequest();
	}
	return TRUE;
}
//------------------------------------------------------------------------