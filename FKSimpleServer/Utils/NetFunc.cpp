#include "NetFunc.h"
#include "../Base/BaseDepend.h"
#include "../Base/BaseMacro.h"
//------------------------------------------------------------------------
// 网络初始化
bool NetStartup(int ver1, int ver2)
{
	WSADATA m_wsaData;
	if (WSAStartup(MAKEWORD(ver1, ver2), &m_wsaData) != SOCKET_ERROR)
	{
		INFO_MSG_LOG(L"初始化Winsock %d.%d 完成.", ver1, ver2);
		return true;
	}
	ERROR_MSG_LOG_WITH_TIME(L"初始化Winsock %d.%d 失败.", ver1, ver2);
	return false;
}
//------------------------------------------------------------------------
bool NetCleanup()
{
	if (WSACleanup() == 0)
	{
		return true;
	}
	return false;
}
//------------------------------------------------------------------------