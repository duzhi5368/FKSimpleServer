#include "Scheduler.h"
//-------------------------------------------------------------
CScheduler::CScheduler()
{
	m_dwCurrentTick = GetTickCount();
}
//-------------------------------------------------------------
void CScheduler::PushTask(CRefCountable* pOwner, Task&& taskFunc, uint32_t unAfter)
{
	if (pOwner == nullptr)
		return;

	int64_t i64DueTimeTick = unAfter + m_dwCurrentTick;
	pOwner->IncRefCount();
	m_TaskQueue.push(SJobElement(pOwner, std::move(taskFunc), i64DueTimeTick));
}
//-------------------------------------------------------------
void CScheduler::DoTasks()
{
	// Ö¡¸üÐÂ
	m_dwCurrentTick = GetTickCount();

	while (!m_TaskQueue.empty())
	{
		SJobElement jobElem = m_TaskQueue.top();

		if ( m_dwCurrentTick < jobElem.m_i64ExecutionTick )
			break;

		// Ö´ÐÐTask
		jobElem.m_TaskFunc();
		jobElem.m_pOwner->DecRefCount();
		m_TaskQueue.pop();
	}
}
//-------------------------------------------------------------