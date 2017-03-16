#pragma once
//-------------------------------------------------------------
class CRefCountable
{
public:
	CRefCountable();
	virtual ~CRefCountable();
public:
	void	IncRefCount();
	void	DecRefCount();
	int		GetRefCount();
private:
	int		m_nRefCount;
};