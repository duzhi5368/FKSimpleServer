#pragma once
//------------------------------------------------------------------------
#include "../Utils/Thread.h"
//------------------------------------------------------------------------
class CClientMgrThread : public CThread
{
public:
	///�̵߳���������
	virtual void Execute(LPVOID lpParam);
	///Terminate�¼�
	virtual void OnTerminated(void);
};