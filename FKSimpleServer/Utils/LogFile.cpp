#include "LogFile.h"
#include "FileFunc.h"
#include "StringFunc.h"
#include "../Base/BaseMacro.h"
#include "../Base/ServerConfig.h"
//------------------------------------------------------------------------
CLogFile::CLogFile(void)
	: m_bIsInit( false )
{
	m_fpLog = NULL;
	memset(&m_stFileTime, 0, sizeof(m_stFileTime));

	if (!PathIsFolder(DEFAULE_SERVER_LOG_DIR))
	{
		if (!CreateDirectory(DEFAULE_SERVER_LOG_DIR, NULL))
		{
			MessageBox(GetDesktopWindow(), L"创建 .\\log 目录失败，请手动创建！", L"LOG SYSTEM", 0);
		}
	}
}
//------------------------------------------------------------------------
CLogFile::~CLogFile(void)
{
	if (m_fpLog != NULL)
		fclose(m_fpLog);
	m_bIsInit = false;
}
//------------------------------------------------------------------------
bool	CLogFile::Init(const TCHAR * pszPath)
{
	if (!PathIsFolder(pszPath))
	{
		if (!CreateDirectory(pszPath, NULL))
			return false;
	}
	o_strncpy(m_szFile, pszPath, 1023);
	ChangeLogFile();
	m_bIsInit = true;
	return true;
}
//------------------------------------------------------------------------
void CLogFile::LogDate()
{
	SYSTEMTIME	st;
	GetLocalTime(&st);
	LogText(L"[%04u-%02u-%02u %02u:%02u:%02u] ", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
}
//------------------------------------------------------------------------
void CLogFile::LogText(const TCHAR * pszString, ...)
{
	THREAD_PROTECT;
	if (m_fpLog == NULL)return;
	va_list	vl;
	va_start(vl, pszString);
	wvsprintf(m_szCache, pszString, vl);
	va_end(vl);
	fputws(m_szCache, m_fpLog);
	fflush(m_fpLog);
}
//------------------------------------------------------------------------
void CLogFile::LogTextRt(const TCHAR * pszString, ...)
{
	if (NULL == pszString)
		return;

	SYSTEMTIME st;
	GetLocalTime(&st);
	TCHAR buf[256];
	wsprintf(buf, L"%04d-%02d-%02d %02d:%02d:%02d ", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
	LogText(buf);

	THREAD_PROTECT;
	if (m_fpLog == NULL)
		return;
	va_list	vl;
	va_start(vl, pszString);
	wvsprintf(m_szCache, pszString, vl);
	va_end(vl);
	fputws(m_szCache, m_fpLog);
	fputws(L"\n", m_fpLog);
	fflush(m_fpLog);
}
//------------------------------------------------------------------------
void CLogFile::LogTextWithDate(const TCHAR * pszString, ...)
{
	THREAD_PROTECT;
	if (m_fpLog == NULL)
		return;
	LogDate();
	va_list	vl;
	va_start(vl, pszString);
	wvsprintf(m_szCache, pszString, vl);
	va_end(vl);
	fputws(m_szCache, m_fpLog);
	fputws(L"\n", m_fpLog);
	fflush(m_fpLog);
}
//------------------------------------------------------------------------
void CLogFile::LogBinary(LPVOID lpData, int nSize, int nLineSize)
{
	THREAD_PROTECT;
	BYTE * pString = (BYTE*)lpData;
	char	szAscii[20];
	szAscii[1] = 0;
	if (m_fpLog == NULL || lpData == NULL)return;

	LogDate();
	NextLine();

	fprintf(m_fpLog, "binary data( 0x%08x ) size: %d\n<start>\n", lpData, nSize);
	int iLineCount = 0;
	for (int i = 0; i < nSize; i++)
	{
		if (*(pString + i) <= 0x20)
		{
			fputc('.', m_fpLog);
		}
		else
		{
			fputc(*(pString + i), m_fpLog);
		}
		iLineCount++;
		if (iLineCount >= nLineSize)
		{
			fprintf(m_fpLog, "\n");
			iLineCount = 0;
		}
	}
	fprintf(m_fpLog, "\n<end>\n");
	fflush(m_fpLog);

	fprintf(m_fpLog, "hex code of binary data\n<start>\n");
	for (int i = 0; i < nSize; i++)
	{
		fprintf(m_fpLog, "%02x ", *(pString + i));
		iLineCount++;
		if (iLineCount >= nLineSize)
		{
			fprintf(m_fpLog, "\n");
			iLineCount = 0;
		}
	}
	fprintf(m_fpLog, "\n<end>\n");
	fflush(m_fpLog);
}
//------------------------------------------------------------------------
void CLogFile::NextLine()
{
	if (m_fpLog != 0)
		fprintf(m_fpLog, "\n");
}
//------------------------------------------------------------------------
void CLogFile::ChangeLogFile()
{
	setlocale(LC_ALL, "chs");

	THREAD_PROTECT;
	SYSTEMTIME	stNow;
	CriticalSectionProtector	m_Protector;
	GetLocalTime(&stNow);
	if (stNow.wDay != m_stFileTime.wDay || m_fpLog == NULL)
	{
		TCHAR	szFile1[256];
		TCHAR	szFile[1024];
		wsprintf(szFile1, L"%04u-%02u-%02u.log", stNow.wYear, stNow.wMonth, stNow.wDay);
		_wmakepath(szFile, NULL, m_szFile, szFile1, NULL);
		if (m_fpLog)
		{
			fclose(m_fpLog);
		}
		m_fpLog = _wfopen(szFile, L"at+,ccs=UTF-8");
		if (m_fpLog == nullptr)
		{
			ERROR_MSG_LOG_WITH_TIME(L"新日志文件: %s 创建失败.", szFile);
		}
		else
		{
			INFO_MSG_LOG(L"日志文件更换为: %s .", szFile);
		}
		m_stFileTime = stNow;
	}
}
//------------------------------------------------------------------------