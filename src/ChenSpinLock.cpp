#include "ChenSpinLock.h"
#include "ChenAtomic.h"
#include <unistd.h>
#include <pthread.h>

ChenSpinMutex::ChenSpinMutex()
{
	m_mutex = 0;
}

ChenSpinMutex::~ChenSpinMutex()
{
	
}

void ChenSpinMutex::Lock() const
{
	unsigned long id = (unsigned long)pthread_self();
	if (m_mutex == id)
        goto END;

    while(1)
    {
        unsigned long old = 0;
        atomic_cas_32(&m_mutex, &old, id);
        if (m_mutex == id)
            break;
        chen_cpu_pause();
    }
END:
    chen_cpu_pause();
}

void ChenSpinMutex::Unlock() const
{
	m_mutex = 0;
}

ChenSpinLock::ChenSpinLock(const BaseLock& m) : m_lock(m)
{
	m_lock.Lock();
}

ChenSpinLock::~ChenSpinLock()
{
	m_lock.Unlock();
}

