#pragma once
//------------------------------------------------------------------------
#include "../Utils/Thread.h"
//------------------------------------------------------------------------
class CClientListenerThread : public CThread
{
public:
	///�̵߳���������
	virtual void Execute(LPVOID lpParam);
	///Terminate�¼�
	virtual void OnTerminated(void);
};