#pragma once

#define WINSIZE_X 1024
#define WINSIZE_Y 768

#define PURE = 0

#define PI 3.141592f
#define GRAVITY_ACC 9.8f

#define KEY_MAX 256

/* Singleton Pattern */
#define DECLARE_SINGLETON(ClassName)	\
public:									\
	static ClassName* GetInstance()		\
	{									\
		if(NULL == m_pInstance)			\
			m_pInstance = new ClassName;\
		return m_pInstance;				\
	}									\
	void DestroyInstance()				\
	{									\
		if(m_pInstance)					\
		{								\
			delete m_pInstance;			\
			m_pInstance = NULL;			\
		}								\
	}									\
private:								\
	static ClassName* m_pInstance;

#define IMPLEMENT_SINGLETON(ClassName)	\
ClassName* ClassName::m_pInstance = NULL;

#define GET_SINGLETON(ClassName) ClassName::GetInstance()