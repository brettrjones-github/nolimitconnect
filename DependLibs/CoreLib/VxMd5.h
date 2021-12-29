#pragma once
#include <stdint.h>

struct VxMD5Context {
        uint32_t buf[4];
        uint32_t bits[2];
        unsigned char in[64];
};

void VxMD5Init(struct VxMD5Context*ctx);
void VxMD5Update(struct VxMD5Context*ctx, unsigned char const *buf, uint32_t len);
void VxMD5Final(unsigned char digest[16], struct VxMD5Context*ctx);
void VxMD5Transform( uint32_t buf[4], uint32_t in[16]);

/*
 * This is needed to make RSAREF happy on some MS-DOS compilers.
 */
//typedef struct MD5Context MD5_CTX;


