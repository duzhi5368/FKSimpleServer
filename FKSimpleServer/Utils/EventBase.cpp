#include "EventBase.h"
//------------------------------------------------------------------------
CEventSender::CEventSender()
{
	m_pEventListener = nullptr;
}
//------------------------------------------------------------------------
void CEventSender::setEventListener(CEventListener * pEventListener)
{ 
	m_pEventListener = pEventListener; 
}
//------------------------------------------------------------------------
CEventListener* CEventSender::getEventListener()
{ 
	return m_pEventListener; 
}
//------------------------------------------------------------------------
void CEventSender::sendEvent(int iEvent, int iParam, LPVOID lpParam)
{ 
	if (m_pEventListener) 
		m_pEventListener->OnEvent(this, iEvent, iParam, lpParam); 
}
//------------------------------------------------------------------------