#include "DBJobManager.h"
#include "../Utils/Thread.h"
#include "../Base/DBJobContext.h"
//-------------------------------------------------------------
CDBJobManager::CDBJobManager()
{
}
//-------------------------------------------------------------
CDBJobManager::~CDBJobManager()
{
}
//-------------------------------------------------------------
void CDBJobManager::ExecuteDBJobs()
{
	SDBJobContext* pJobContext = nullptr;
	m_DBJobRequestQueue.Consume(pJobContext);
	pJobContext->m_bIsSuccess = pJobContext->OnExecute();
	m_DBJobResultQueue.Produce(pJobContext);
}
//-------------------------------------------------------------
void CDBJobManager::PushDBJobRequest(SDBJobContext* pJobContext)
{
	m_DBJobRequestQueue.Produce(pJobContext);
}
//-------------------------------------------------------------
bool CDBJobManager::PopDBJobResult(SDBJobContext*& jobContext)
{
	return m_DBJobResultQueue.Consume(jobContext, false);
}
//-------------------------------------------------------------