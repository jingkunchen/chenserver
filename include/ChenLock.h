#ifndef __CHEN_LOCK_H__
#define __CHEN_LCOK_H__
#include <ChenBaseLock.h>
#include "pthread.h"

class ChenMutex : public BaseLock
{
public:
	ChenMutex();
	~ChenMutex();  
	virtual void Lock() const; 	
	virtual void Unlock() const;  

private:  
	mutable pthread_mutex_t m_mutex;  

};

class ChenLock  
{  
public:  
    ChenLock(const BaseLock&);  
    ~ChenLock();  
  
private:  
    const BaseLock& m_lock;  
};  


#endif
