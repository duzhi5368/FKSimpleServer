#include "IOConsole.h"
#include "../Base/BaseMacro.h"
//-------------------------------------------------------------
CIOConsole::CIOConsole()
	: m_pInputListener( nullptr )
{

}
//-------------------------------------------------------------
CIOConsole::~CIOConsole()
{

}
//-------------------------------------------------------------
void CIOConsole::OutPut(DWORD dwColor, const TCHAR * pszString, ...)
{
	if (this == NULL)return;

	TCHAR	szBuff[4096];
	va_list	vl;
	va_start(vl, pszString);
	vswprintf(szBuff, pszString, vl);
	va_end(vl);
	OutPutStatic(dwColor, szBuff);
}
//------------------------------------------------------------------------
void CIOConsole::Input(const TCHAR * pszString)
{ 
	if (m_pInputListener != NULL)
		m_pInputListener->OnInput(pszString); 
}
//-------------------------------------------------------------
CInputListener* CIOConsole::GetInputListener()
{ 
	return m_pInputListener; 
}
//-------------------------------------------------------------
void CIOConsole::SetInputListener(CInputListener * pInputListener)
{ 
	INFO_MSG_LOG_WITH_TIME(L"设置输入监听器完成.");
	m_pInputListener = pInputListener; 
}
//-------------------------------------------------------------