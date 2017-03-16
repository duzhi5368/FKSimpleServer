#pragma once
//------------------------------------------------------------------------
template <class T>
class TSingleton
{
public:
	TSingleton()
	{
		m_pInstance = (T*)this;
	}
	static T* GetInstance()
	{
		if (m_pInstance == NULL)
			m_pInstance = new T;
		return m_pInstance;
	}
protected:
	static T* m_pInstance;
};
//------------------------------------------------------------------------
template <class T>
T* TSingleton<T>::m_pInstance = (T*)nullptr;
//------------------------------------------------------------------------