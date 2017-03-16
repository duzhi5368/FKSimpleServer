#include "RefCountable.h"
//-------------------------------------------------------------
CRefCountable::CRefCountable()
	: m_nRefCount(0)
{

}
//-------------------------------------------------------------
CRefCountable::~CRefCountable()
{

}
//-------------------------------------------------------------
void CRefCountable::IncRefCount()
{
	++m_nRefCount;
}
//-------------------------------------------------------------
void CRefCountable::DecRefCount()
{
	--m_nRefCount;
}
//-------------------------------------------------------------
int CRefCountable::GetRefCount()
{
	return m_nRefCount;
}
//-------------------------------------------------------------