#pragma once
//-------------------------------------------------------------
#include "Singleton.h"
#include "../Base/BaseDepend.h"
#include "Lock.h"
//-------------------------------------------------------------
#define	LGERR			CLogFile::GetInstance()->LogTextWithDate( "Error @ %s (%d) => ", __FILE__, __LINE__ );\
								CLogFile::GetInstance()->LogTextRt
//------------------------------------------------------------------------
#define	LG				CLogFile::GetInstance()->LogTextWithDate
//------------------------------------------------------------------------
class CLogFile :
	public TSingleton < CLogFile >
{
public:
	CLogFile(void);
	virtual ~CLogFile(void);
public:
	bool			Init(const TCHAR * pszPath);
	void 			NextLine();
	void 			LogDate();
	void 			LogText(const TCHAR * pszString, ...);
	void 			LogTextRt(const TCHAR * pszString, ...);
	void 			LogTextWithDate(const TCHAR * pszString, ...);
	void 			LogBinary(LPVOID lpData, int nSize, int nLineSize = 80);
	void 			ChangeLogFile();
	bool			IsHadInit()const{ return m_bIsInit; }
private:
	FILE *			m_fpLog;
	SYSTEMTIME		m_stFileTime;
	TCHAR			m_szFile[1024];
	TCHAR			m_szCache[16384];
	CriticalSection	m_CriticalSection;
	bool			m_bIsInit;
};
//------------------------------------------------------------------------