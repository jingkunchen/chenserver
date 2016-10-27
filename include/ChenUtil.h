#ifndef __CHEN_UTIL_H__
#define __CHEN_UTIL_H__
#include <sys/types.h>

class ChenUtil
{
public:
	void uriencode_init(void);
	
	bool uriencode(const char *src, char *dst, const size_t srclen, const size_t dstlen);
	
	static void vperror(const char *fmt, ...);
		
private:

	char *uriencode_map[256];
	
	char uriencode_str[768];
	
};
#endif
