#include "Base/BaseDepend.h"
#include "Utils/Exception.h"
#include "Utils/Thread.h"
#include "Base/ServerConfig.h"
#include "Framework/ClientManager.h"
#include "Framework/ClientSession.h"
#include "Framework/DBJobManager.h"
#include "Utils/DBHelper.h"
#include "Utils/Scheduler.h"
#include "Framework/ServerForm.h"
#include "Base/BaseMacro.h"
//-------------------------------------------------------------
int APIENTRY _tWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{
	TRY_INIT
	SetUnhandledExceptionFilter(ExceptionFilter);

	TRY_BEGIN
	g_pSvrForm = new CServerForm(L"自由骑士服务器", lpCmdLine);
	g_pSvr = new CBaseServer(g_pSvrForm);
	g_pSvr->Init();
	g_pSvrForm->SetInputListener(g_pSvr);
	g_pSvrForm->SetServer(g_pSvr);

	g_pSvrForm->EnterMessageLoop();

	SAFE_DELETE(g_pSvr);
	SAFE_DELETE(g_pSvrForm);
	TRY_END_NOTHIS

	return 0;
}