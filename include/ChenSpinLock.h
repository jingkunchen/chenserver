#ifndef __CHEN_SPIN_LOCK_H__
#define __CHEN_SPIN_LCOK_H__
#include <ChenBaseLock.h>

class ChenSpinMutex : public BaseLock
{
public:
	ChenSpinMutex();
	~ChenSpinMutex();  
	virtual void Lock() const; 	
	virtual void Unlock() const;  

private:  
	mutable unsigned long m_mutex;  

};

class ChenSpinLock  
{  
public:  
    ChenSpinLock(const BaseLock&);  
    ~ChenSpinLock();  
  
private:  
    const BaseLock& m_lock;  
};  

#endif
