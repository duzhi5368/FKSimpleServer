#pragma once
//------------------------------------------------------------------------
#include "../Utils/Thread.h"
//------------------------------------------------------------------------
class CDBJobMgrThread : public CThread
{
public:
	///�̵߳���������
	virtual void Execute(LPVOID lpParam);
	///Terminate�¼�
	virtual void OnTerminated(void);
};