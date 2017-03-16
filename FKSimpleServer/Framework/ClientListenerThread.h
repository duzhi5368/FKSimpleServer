#pragma once
//------------------------------------------------------------------------
#include "../Utils/Thread.h"
//------------------------------------------------------------------------
class CClientListenerThread : public CThread
{
public:
	///线程的主运行体
	virtual void Execute(LPVOID lpParam);
	///Terminate事件
	virtual void OnTerminated(void);
};