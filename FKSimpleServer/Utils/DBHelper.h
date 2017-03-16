#pragma once
//-------------------------------------------------------------
struct sqlite3;
struct sqlite3_stmt;
//-------------------------------------------------------------
enum ENUM_RESULT_TYPE
{
	eResultType_Error,
	eResultType_Row,
	eResultType_Done
};
//-------------------------------------------------------------
class CDBHelper
{
private:
	static sqlite3*		m_pSqlite;
	sqlite3_stmt*		m_pResult;
	int					m_nResultColCount;
	int					m_nBindColCount;
	int					m_nResultCurrentCol;
public:
	CDBHelper(const char* sql);
	~CDBHelper();
public:
	static bool			Initialize(const char* pConnInfoStr);
	static void			Finalize();
	static bool			Execute(const char* pFormat, ...);
public:
	bool				BindParamInt(int nParam);
	bool				BindParamDouble(double dParam);
	bool				BindParamText(const char* szText, int nCount);
	ENUM_RESULT_TYPE	FetchRow();
	int					GetResultParamInt();
	double				GetResultParamDouble();
	const unsigned char* GetResultParamText();
};