#include "ChenLock.h"
#include "pthread.h"

ChenMutex::ChenMutex()
{
	pthread_mutex_init(&m_mutex, NULL);
}

ChenMutex::~ChenMutex()
{
	pthread_mutex_destroy(&m_mutex);
}

void ChenMutex::Lock() const
{
	pthread_mutex_lock(&m_mutex);
}

void ChenMutex::Unlock() const
{
	pthread_mutex_unlock(&m_mutex);
}

ChenLock::ChenLock(const BaseLock& m) : m_lock(m)
{
	m_lock.Lock();
}

ChenLock::~ChenLock()
{
	m_lock.Unlock();
}


