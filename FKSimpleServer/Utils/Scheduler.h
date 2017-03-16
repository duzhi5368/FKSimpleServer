#pragma once
//-------------------------------------------------------------
#include "../Base/BaseDepend.h"
#include "RefCountable.h"
#include "Thread.h"
//-------------------------------------------------------------
typedef std::function<void()>		Task;
//-------------------------------------------------------------
struct SJobElement
{
	SJobElement(CRefCountable* pOwner, Task&& taskFunc, int64_t i64ExecTick)
		: m_pOwner(pOwner)
		, m_TaskFunc(taskFunc)
		, m_i64ExecutionTick(i64ExecTick)
	{}
	
	CRefCountable*	m_pOwner;
	Task			m_TaskFunc;
	int64_t			m_i64ExecutionTick;
};
//-------------------------------------------------------------
struct SJobComparator
{
	bool operator()(const SJobElement& lhs, const SJobElement& rhs)
	{
		return lhs.m_i64ExecutionTick > rhs.m_i64ExecutionTick;
	}
};
//-------------------------------------------------------------
typedef std::priority_queue<SJobElement, std::vector<SJobElement>, SJobComparator>	TaskPriorityQueue;
//-------------------------------------------------------------
class CScheduler
{
public:
	CScheduler();
public:
	void	PushTask(CRefCountable* pOwner, Task&& taskFunc, uint32_t unAfter);
	void	DoTasks();
private:
	TaskPriorityQueue		m_TaskQueue;
	DWORD					m_dwCurrentTick;
};
//-------------------------------------------------------------
template< class T, class F, class... Args>
void CallFuncAfter(uint32_t unAfter, T instance, F memFunc, Args&&... args)
{
	static_assert(true == std::is_convertible<T, CRefCountable*>::value, "only allowed when CRefCountable*");

	s_pScheduler->PushTask(static_cast<CRefCountable*>(instance),
		std::bind(memFunc, instance, std::forward<Args>(args)...), unAfter);
}
//-------------------------------------------------------------