#pragma once
//------------------------------------------------------------------------
#include "../Utils/SettingFile.h"
#include "../Base/BaseDepend.h"
//------------------------------------------------------------------------
typedef struct SServerState
{
	SServerState()
	{
		memset(this, 0, sizeof(*this));
	}
	int		numConnection;
	int		numConnected;
	int		numDisconnected;
	DWORD	dwRecvBytes;
	DWORD	dwSendBytes;
	DWORD	dwRunningSeconds;
	DWORD	dwLoopTimes;
	float	fLoopTime;
	bool	bUpdated;

	TCHAR*	pServerDescript;
	char*	pServerState;
}SERVERSTATE;
//------------------------------------------------------------------------
class CVirtualServer
{
public:
	virtual void	GetServerState(SServerState & state) = 0;
	virtual bool	Start(CSettingFile &) = 0;
	virtual bool	Stop() = 0;
};