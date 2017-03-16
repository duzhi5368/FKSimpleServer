#pragma once
//-------------------------------------------------------------
#include "../Base/BaseDepend.h"
//-------------------------------------------------------------
class CInputListener
{
public:
	virtual void OnInput(const TCHAR * pszString) = 0;
};
//-------------------------------------------------------------
class CIOConsole
{
public:
	CIOConsole(void);
	virtual ~CIOConsole(void);
public:
	void				OutPut(DWORD dwColor, const TCHAR * pszString, ...);
public:
	virtual void		OutPutStatic(DWORD dwColor, const TCHAR * pszString){};

	virtual void		Input(const TCHAR * pszString);
	CInputListener*		GetInputListener();
	void				SetInputListener(CInputListener * pInputListener);
protected:
	CInputListener*		m_pInputListener;
};
//-------------------------------------------------------------