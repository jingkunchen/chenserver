#ifndef ___CHEN_ATOMIC_H_
#define ___CHEN_ATOMIC_H_


#include <sys/atomic.h>

     uint32_t
     atomic_cas_32(volatile uint32_t *ptr, uint32_t old, uint32_t new);

     unsigned int
     atomic_cas_uint(volatile unsigned int *ptr, unsigned int old,
         unsigned int new);

     unsigned long
     atomic_cas_ulong(volatile unsigned long *ptr, unsigned long old,
         unsigned long new);

     void *
     atomic_cas_ptr(volatile void *ptr, void *old, void *new);

     uint64_t
     atomic_cas_64(volatile uint64_t *ptr, uint64_t old, uint64_t new);

     uint32_t
     atomic_cas_32_ni(volatile uint32_t *ptr, uint32_t old, uint32_t new);

     unsigned int
     atomic_cas_uint_ni(volatile unsigned int *ptr, unsigned int old,
         unsigned int new);

     unsigned long
     atomic_cas_ulong_ni(volatile unsigned long *ptr, unsigned long old,
         unsigned long new);

     void *
     atomic_cas_ptr_ni(volatile void *ptr, void *old, void *new);

     uint64_t
     atomic_cas_64_ni(volatile uint64_t *ptr, uint64_t old, uint64_t new);

	 
#endif
