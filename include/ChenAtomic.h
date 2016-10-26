#ifndef ___CHEN_ATOMIC_H_
#define ___CHEN_ATOMIC_H_
#define chen_cpu_pause()             __asm__ ("pause")

static inline int atomic_cas_32(volatile unsigned long* ptr,
										unsigned long* _old,
										unsigned long _new)
{
		 unsigned char res;

		__asm volatile(
			"lock; cmpxchgq %2,%1 ;"
			"		 sete	 %0 ;	 "
			 : "=a" (res),"+m" (*ptr): "r" (_new),"a" (*_old): "cc","memory");
	 
		 return (res);

}
#endif
