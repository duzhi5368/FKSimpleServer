#include "../Depend/SQLite3/sqlite3.h"
#include "DBHelper.h"
#include "Exception.h"
#include "../Base/BaseMacro.h"
#include "../Utils/StringFunc.h"
//-------------------------------------------------------------
sqlite3*		CDBHelper::m_pSqlite = nullptr;
//-------------------------------------------------------------
CDBHelper::CDBHelper(const char* sql)
	: m_pResult( nullptr )
	, m_nResultColCount( 0 )
	, m_nBindColCount( 0 )
	, m_nResultCurrentCol( 0 )
{
	char* pErrorMsg = nullptr;

	if (sqlite3_prepare_v2(m_pSqlite, sql, -1, &m_pResult, nullptr) != SQLITE_OK)
	{
		printf("DbHelper Query [%s] Prepare failed: %s\n", sql, sqlite3_errmsg(m_pSqlite));
		FK_CRASH_ASSERT(false);
	}
}
//-------------------------------------------------------------
CDBHelper::~CDBHelper()
{
	sqlite3_finalize(m_pResult);
}
//-------------------------------------------------------------
bool CDBHelper::Initialize(const char* pConnInfoStr)
{
	int nResult = sqlite3_open(pConnInfoStr, &m_pSqlite);

	if (m_pSqlite == nullptr || nResult != SQLITE_OK)
	{
		ERROR_MSG_LOG_WITH_TIME(L"数据库连接失败, DB文件 = %s, 错误码 = %d.", MBytesToWString(pConnInfoStr).c_str(), nResult);
		return false;
	}
	INFO_MSG_LOG_WITH_TIME(L"数据库连接成功, DB文件 = %s", MBytesToWString(pConnInfoStr).c_str());
	return true;
}
//-------------------------------------------------------------
void CDBHelper::Finalize()
{
	if (m_pSqlite)
	{
		INFO_MSG_LOG_WITH_TIME(L"数据库连接关闭.");
		sqlite3_close(m_pSqlite);
	}
}
//-------------------------------------------------------------
bool CDBHelper::Execute(const char* pFormat, ...)
{
	if (pFormat == nullptr)
		return false;
	if (m_pSqlite == nullptr)
		return false;

	va_list ap;
	char szSqlQuery[1024] = { 0 };
	va_start(ap, pFormat);
	int nRes = vsnprintf_s(szSqlQuery, 1024, pFormat, ap);
	va_end(ap);

	if (nRes == -1)
		return false;

	char* pErrorMsg = nullptr;
	if (sqlite3_exec(m_pSqlite, szSqlQuery, nullptr, nullptr, &pErrorMsg) != SQLITE_OK)
	{
		printf("SQL [%s] ERROR [%s] \n", szSqlQuery, pErrorMsg);
		return false;
	}
	return true;
}
//-------------------------------------------------------------
bool CDBHelper::BindParamInt(int nParam)
{
	if (sqlite3_bind_int(m_pResult, ++m_nBindColCount, nParam) != SQLITE_OK)
	{
		printf("DbHelper Bind Int failed: %s\n", sqlite3_errmsg(m_pSqlite));
		return false;
	}
	return true;
}
//-------------------------------------------------------------
bool CDBHelper::BindParamDouble(double dParam)
{
	if (sqlite3_bind_double(m_pResult, ++m_nBindColCount, dParam) != SQLITE_OK)
	{
		printf("DbHelper Bind Double failed: %s\n", sqlite3_errmsg(m_pSqlite));
		return false;
	}
	return true;
}
//-------------------------------------------------------------
bool CDBHelper::BindParamText(const char* szText, int nCount)
{
	if (sqlite3_bind_text(m_pResult, ++m_nBindColCount, szText, strlen(szText), nullptr) != SQLITE_OK)
	{
		printf("DbHelper Bind Text failed: %s\n", sqlite3_errmsg(m_pSqlite));
		return false;
	}
	return true;
}
//-------------------------------------------------------------
ENUM_RESULT_TYPE CDBHelper::FetchRow()
{
	int nResult = sqlite3_step(m_pResult);
	if (nResult != SQLITE_ROW && nResult != SQLITE_DONE)
	{
		printf("DbHelper FetchRow failed: %s\n", sqlite3_errmsg(m_pSqlite));
		return eResultType_Error;
	}

	if (nResult == SQLITE_DONE)
		return eResultType_Done;

	m_nResultColCount = sqlite3_column_count(m_pResult);
	m_nResultCurrentCol = 0;
	return eResultType_Row;
}
//-------------------------------------------------------------
int CDBHelper::GetResultParamInt()
{
	FK_CRASH_ASSERT(m_nResultCurrentCol < m_nResultColCount);
	return sqlite3_column_int(m_pResult, m_nResultCurrentCol++);
}
//-------------------------------------------------------------
double CDBHelper::GetResultParamDouble()
{
	FK_CRASH_ASSERT(m_nResultCurrentCol < m_nResultColCount);
	return sqlite3_column_double(m_pResult, m_nResultCurrentCol++);
}
//-------------------------------------------------------------
const unsigned char* CDBHelper::GetResultParamText()
{
	FK_CRASH_ASSERT(m_nResultCurrentCol < m_nResultColCount);
	return sqlite3_column_text(m_pResult, m_nResultCurrentCol++);
}
//-------------------------------------------------------------