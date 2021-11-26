#ifndef CORELIB_MD5_H
#define CORELIB_MD5_H
#include <CoreLib/config_corelib.h>

//#ifdef __cplusplus
//extern "C" {
//#endif

struct MD5Context {
        uint32_t buf[4];
        uint32_t bits[2];
        unsigned char in[64];
};

void MD5Init(struct MD5Context *ctx);
void MD5Update(struct MD5Context *ctx, unsigned char const *buf, uint32_t len);
void MD5Final(unsigned char digest[16], struct MD5Context *ctx);
void MD5Transform( uint32_t buf[4], uint32_t in[16]);

/*
 * This is needed to make RSAREF happy on some MS-DOS compilers.
 */
typedef struct MD5Context MD5_CTX;

//#ifdef __cplusplus
//}  /* extern "C" */
//#endif

#endif /* !CORELIB_MD5_H */
