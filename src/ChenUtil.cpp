#include "ChenUtil.h"
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <stdarg.h>
#include <errno.h> 




void ChenUtil::uriencode_init(void) {
    int x;
	printf("uriencode_init\n");
    char *str = uriencode_str;
    for (x = 0; x < 256; x++) {
        if (isalnum(x) || x == '-' || x == '.' || x == '_' || x == '~') {
            uriencode_map[x] = NULL;
        } else {
            snprintf(str, 4, "%%%02X", x);
            uriencode_map[x] = str;
            str += 3; /* lobbing off the \0 is fine */
        }
    }
}

bool ChenUtil::uriencode(const char *src, char *dst, const size_t srclen, const size_t dstlen) {
    int x;
    size_t d = 0;
    for (x = 0; x < srclen; x++) {
        if (d + 4 >= dstlen)
            return false;
        if (uriencode_map[(unsigned char) src[x]] != NULL) {
            memcpy(&dst[d], uriencode_map[(unsigned char) src[x]], 3);
            d += 3;
        } else {
            dst[d] = src[x];
            d++;
        }
    }
    dst[d] = '\0';
    return true;
}

void ChenUtil::vperror(const char *fmt, ...) {
    int old_errno = errno;
    char buf[1024];
    va_list ap;

    va_start(ap, fmt);
    if (vsnprintf(buf, sizeof(buf), fmt, ap) == -1) {
        buf[sizeof(buf) - 1] = '\0';
    }
    va_end(ap);

    errno = old_errno;

    perror(buf);
}



