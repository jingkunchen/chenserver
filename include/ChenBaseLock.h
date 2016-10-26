#ifndef __CHEN_BASE_LOCK_H__
#define __CHEN_BASE_LOCK_H__

// BaseLock.h
  
class BaseLock
{ 
public:
   virtual void Lock() const =0;
   virtual void Unlock()const=0; 
};

#endif