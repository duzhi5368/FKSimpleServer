#pragma once
//-------------------------------------------------------------
#include "BaseDepend.h"
#include "../Utils/ObjectPool.h"
#include "../Utils/DBHelper.h"
//-------------------------------------------------------------
struct SDBJobContext
{
	SDBJobContext()
		: m_bIsSuccess(false)
	{}

	SDBJobContext(SOCKET socketKey)
		: m_SocketKey(socketKey)
		, m_bIsSuccess(false)
	{}

	virtual ~SDBJobContext()
	{}

	void SetSuccess(bool bSuccess){
		m_bIsSuccess = bSuccess;
	}
	// Ç¿ÖÆ¼Ì³Ð
	virtual bool	OnExecute() = 0;
public:
	SOCKET		m_SocketKey;
	bool		m_bIsSuccess;
};
//-------------------------------------------------------------
struct STestDBJobContext : public SDBJobContext, public TObjectPool<STestDBJobContext>
{
	STestDBJobContext(SOCKET socketKey, int nPlaceHolder)
		: SDBJobContext(socketKey)
		, m_nPlaceHolder(nPlaceHolder)
	{}

	virtual bool OnExecute()
	{
		const char* pTestSQL1 = "INSERT INTO tableName VALUES(?, ?)";
		const char* pTestSQL2 = "SELECT sectionName FROM tableName WHERE sectionID=?";
		const char* pTestSQL3 = "UPDATE tableName SET sectionName=? WHERE sectionID=?";
		const char* pTestSQL4 = "DELETE FROM tableName WHERE sectionID=?";

		CDBHelper helper(pTestSQL2);
		helper.BindParamInt(m_nPlaceHolder);

		if (eResultType_Row != helper.FetchRow())
			return false;

		const unsigned char* szSectionInfo = helper.GetResultParamText();
		return true;
	}

	int			m_nPlaceHolder;
};