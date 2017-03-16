#pragma once
//-------------------------------------------------------------
#include "../Utils/ProduceConsumerQueue.h"
#include "../Base/ServerConfig.h"
//-------------------------------------------------------------
struct SDBJobContext;
//-------------------------------------------------------------
class CDBJobManager
{
public:
	CDBJobManager();
	~CDBJobManager();
public:
	void		ExecuteDBJobs();
	void		PushDBJobRequest(SDBJobContext* pJobContext);
	bool		PopDBJobResult(SDBJobContext*& jobContext);
private:
	TProducerConsumerQueue<SDBJobContext*, MAX_DB_JOB_NUM>	m_DBJobRequestQueue;
	TProducerConsumerQueue<SDBJobContext*, MAX_DB_JOB_NUM>	m_DBJobResultQueue;
};
//-------------------------------------------------------------