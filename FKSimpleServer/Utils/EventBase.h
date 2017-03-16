#pragma once
//------------------------------------------------------------------------
#include "../Base/BaseDepend.h"
//------------------------------------------------------------------------
class CEventSender;
class CEventListener
{
public:
	virtual void OnEvent(CEventSender * pSender, int iEvent, int iParam, 
		LPVOID lpParam) = 0;
};
//------------------------------------------------------------------------
class CEventSender
{
public:
	CEventSender();
public:
	void			setEventListener(CEventListener * pEventListener);
	CEventListener* getEventListener();
	void			sendEvent(int iEvent, int iParam, LPVOID lpParam);
protected:
	CEventListener * m_pEventListener;
};
//------------------------------------------------------------------------