#include "NetFunc.h"
#include "../Base/BaseDepend.h"
#include "../Base/BaseMacro.h"
//------------------------------------------------------------------------
// �����ʼ��
bool NetStartup(int ver1, int ver2)
{
	WSADATA m_wsaData;
	if (WSAStartup(MAKEWORD(ver1, ver2), &m_wsaData) != SOCKET_ERROR)
	{
		INFO_MSG_LOG(L"��ʼ��Winsock %d.%d ���.", ver1, ver2);
		return true;
	}
	ERROR_MSG_LOG_WITH_TIME(L"��ʼ��Winsock %d.%d ʧ��.", ver1, ver2);
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