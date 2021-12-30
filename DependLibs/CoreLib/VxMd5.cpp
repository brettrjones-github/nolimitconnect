/*
 * This code implements the MD5 message-digest algorithm.
 * The algorithm is due to Ron Rivest.	This code was
 * written by Colin Plumb in 1993, no copyright is claimed.
 * This code is in the public domain; do with it what you wish.
 *
 * Equivalent code is available from RSA Data Security, Inc.
 * This code has been tested against that, and is equivalent,
 * except that you don't need to include two pages of legalese
 * with every copy.
 *
 * To compute the message digest of a chunk of bytes, declare an
 * MD5Context structure, pass it to MD5Init, call MD5Update as
 * needed on buffers full of bytes, and then call MD5Final, which
 * will fill a supplied 16-byte array with the digest.
 */

/* Brutally hacked by John Walker back from ANSI C to K&R (no
   prototypes) to maintain the tradition that Netfone will compile
   with Sun's original "cc". */

#include "VxMd5.h"
#include "VxDebug.h"

#include <memory.h>		 /* for memcpy() */
#include <stdlib.h>		 /* for size_t */

#ifdef sgi
#define HIGHFIRST
#endif

#ifdef sun
#define HIGHFIRST
#endif

#ifndef HIGHFIRST
#define byteReverse(buf, len)	/* Nothing */
#else
/*
 * Note: this code is harmless on little-endian machines.
 */
void byteReverse(buf, longs)
    unsigned char *buf; uint32_t longs;
{
    uint32_t t;
    do {
    t = (uint32_t) ((uint32_t) buf[3] << 8 | buf[2]) << 16 |
	    ((uint32_t) buf[1] << 8 | buf[0]);
    *(uint32_t *) buf = t;
	buf += 4;
    } while (--longs);
}
#endif

/*
 * Start MD5 accumulation.  Set bit count to 0 and buffer to mysterious
 * initialization constants.
 */
void VxMD5Init(struct VxMD5Context *ctx)
{
    ctx->buf[0] = 0x67452301;
    ctx->buf[1] = 0xefcdab89;
    ctx->buf[2] = 0x98badcfe;
    ctx->buf[3] = 0x10325476;

    ctx->bits[0] = 0;
    ctx->bits[1] = 0;
}

/*
 * Update context to reflect the concatenation of another buffer full
 * of bytes.
 */
void VxMD5Update(struct VxMD5Context *ctx, unsigned char const *buf, uint32_t len)
{
    uint32_t t;

    /* Update bitcount */

    t = ctx->bits[0];
    if ((ctx->bits[0] = t + ((uint32_t) len << 3)) < t)
	ctx->bits[1]++; 	/* Carry from low to high */
    ctx->bits[1] += len >> 29;

    t = (t >> 3) & 0x3f;	/* Bytes already in shsInfo->data */

    /* Handle any leading odd-sized chunks */

    if (t != 0) 
    {
	    unsigned char *p = (unsigned char *) ctx->in + t;

	    t = 64 - t;
        if ((uint32_t)len < t) 
        {
	        memcpy(p, buf, (size_t)len);
	        return;
	    }

	    memcpy(p, buf, (size_t)t);
	    byteReverse(ctx->in, 16);
        VxMD5Transform(ctx->buf, (uint32_t *) ctx->in);
	    buf += t;
	    len -= t;
    }
    /* Process data in 64-byte chunks */

    while (len >= 64) 
    {
	    memcpy(ctx->in, buf, 64);
	    byteReverse(ctx->in, 16);
        VxMD5Transform(ctx->buf, (uint32_t *) ctx->in);
	    buf += 64;
	    len -= 64;
    }

    /* Handle any remaining bytes of data. */

    memcpy(ctx->in, buf, (size_t)len);
}

/*
 * Final wrapup - pad to 64-byte boundary with the bit pattern 
 * 1 0* (64-bit count of bits processed, MSB-first)
 */
void VxMD5Final( unsigned char digest[16], struct VxMD5Context *ctx )
{
    uint32_t count;
    unsigned char *p;

    /* Compute number of bytes mod 64 */
    count = (uint32_t)(ctx->bits[0] >> 3) & 0x3F;

    /* Set the first char of padding to 0x80.  This is safe since there is
       always at least one byte free */
    p = ctx->in + count;
    *p++ = (unsigned char)0x80;

    /* Bytes of padding needed to make 64 bytes */
    count = 64 - 1 - count;

    /* Pad out to 56 mod 64 */
    if (count < 8) 
    {
	    /* Two lots of padding:  Pad the first block to 64 bytes */
	    memset(p, 0, (size_t)count);
	    byteReverse(ctx->in, 16);
        VxMD5Transform(ctx->buf, (uint32_t *) ctx->in);

	    /* Now fill the next block with 56 bytes */
	    memset(ctx->in, 0, 56);
    } 
    else 
    {
	    /* Pad block to 56 bytes */
	    memset(p, 0, count - 8);
    }

    byteReverse(ctx->in, 14);

    /* Append length in bits and transform */
    ((uint32_t *) ctx->in)[14] = ctx->bits[0];
    ((uint32_t *) ctx->in)[15] = ctx->bits[1];

    VxMD5Transform(ctx->buf, (uint32_t *) ctx->in);
    byteReverse((unsigned char *) ctx->buf, 4);
    memcpy(digest, ctx->buf, 16);
    memset(ctx, 0, sizeof(struct VxMD5Context));        /* In case it's sensitive */
}

/* #define VxF1(x, y, z) (x & y | ~x & z) */
#define VxF1(x, y, z) (z ^ (x & (y ^ z)))
#define VxF2(x, y, z) VxF1(z, x, y)
#define VxF3(x, y, z) (x ^ y ^ z)
#define VxF4(x, y, z) (y ^ (x | ~z))

/* This is the central step in the MD5 algorithm. */
#define VxMD5STEP(f,w,x,y,z,in,s) \
	 (w += f(x,y,z) + in, w = (w<<s | w>>(32-s)) + x)

/*
 * The core of the MD5 algorithm, this alters an existing MD5 hash to
 * reflect the addition of 16 longwords of new data.  MD5Update blocks
 * the data and converts bytes into longwords for this routine.
 */
void VxMD5Transform(uint32_t buf[4], uint32_t in[16])
{
    uint32_t a, b, c, d;

    a = buf[0];
    b = buf[1];
    c = buf[2];
    d = buf[3];

    VxMD5STEP( VxF1, a, b, c, d, in[0] + 0xd76aa478, 7);
    VxMD5STEP(VxF1, d, a, b, c, in[1] + 0xe8c7b756, 12);
    VxMD5STEP(VxF1, c, d, a, b, in[2] + 0x242070db, 17);
    VxMD5STEP(VxF1, b, c, d, a, in[3] + 0xc1bdceee, 22);
    VxMD5STEP(VxF1, a, b, c, d, in[4] + 0xf57c0faf, 7);
    VxMD5STEP(VxF1, d, a, b, c, in[5] + 0x4787c62a, 12);
    VxMD5STEP(VxF1, c, d, a, b, in[6] + 0xa8304613, 17);
    VxMD5STEP(VxF1, b, c, d, a, in[7] + 0xfd469501, 22);
    VxMD5STEP(VxF1, a, b, c, d, in[8] + 0x698098d8, 7);
    VxMD5STEP(VxF1, d, a, b, c, in[9] + 0x8b44f7af, 12);
    VxMD5STEP(VxF1, c, d, a, b, in[10] + 0xffff5bb1, 17);
    VxMD5STEP(VxF1, b, c, d, a, in[11] + 0x895cd7be, 22);
    VxMD5STEP(VxF1, a, b, c, d, in[12] + 0x6b901122, 7);
    VxMD5STEP(VxF1, d, a, b, c, in[13] + 0xfd987193, 12);
    VxMD5STEP(VxF1, c, d, a, b, in[14] + 0xa679438e, 17);
    VxMD5STEP(VxF1, b, c, d, a, in[15] + 0x49b40821, 22);

    VxMD5STEP(VxF2, a, b, c, d, in[1] + 0xf61e2562, 5);
    VxMD5STEP(VxF2, d, a, b, c, in[6] + 0xc040b340, 9);
    VxMD5STEP(VxF2, c, d, a, b, in[11] + 0x265e5a51, 14);
    VxMD5STEP(VxF2, b, c, d, a, in[0] + 0xe9b6c7aa, 20);
    VxMD5STEP(VxF2, a, b, c, d, in[5] + 0xd62f105d, 5);
    VxMD5STEP(VxF2, d, a, b, c, in[10] + 0x02441453, 9);
    VxMD5STEP(VxF2, c, d, a, b, in[15] + 0xd8a1e681, 14);
    VxMD5STEP(VxF2, b, c, d, a, in[4] + 0xe7d3fbc8, 20);
    VxMD5STEP(VxF2, a, b, c, d, in[9] + 0x21e1cde6, 5);
    VxMD5STEP(VxF2, d, a, b, c, in[14] + 0xc33707d6, 9);
    VxMD5STEP(VxF2, c, d, a, b, in[3] + 0xf4d50d87, 14);
    VxMD5STEP(VxF2, b, c, d, a, in[8] + 0x455a14ed, 20);
    VxMD5STEP(VxF2, a, b, c, d, in[13] + 0xa9e3e905, 5);
    VxMD5STEP(VxF2, d, a, b, c, in[2] + 0xfcefa3f8, 9);
    VxMD5STEP(VxF2, c, d, a, b, in[7] + 0x676f02d9, 14);
    VxMD5STEP(VxF2, b, c, d, a, in[12] + 0x8d2a4c8a, 20);

    VxMD5STEP(VxF3, a, b, c, d, in[5] + 0xfffa3942, 4);
    VxMD5STEP(VxF3, d, a, b, c, in[8] + 0x8771f681, 11);
    VxMD5STEP(VxF3, c, d, a, b, in[11] + 0x6d9d6122, 16);
    VxMD5STEP(VxF3, b, c, d, a, in[14] + 0xfde5380c, 23);
    VxMD5STEP(VxF3, a, b, c, d, in[1] + 0xa4beea44, 4);
    VxMD5STEP(VxF3, d, a, b, c, in[4] + 0x4bdecfa9, 11);
    VxMD5STEP(VxF3, c, d, a, b, in[7] + 0xf6bb4b60, 16);
    VxMD5STEP(VxF3, b, c, d, a, in[10] + 0xbebfbc70, 23);
    VxMD5STEP(VxF3, a, b, c, d, in[13] + 0x289b7ec6, 4);
    VxMD5STEP(VxF3, d, a, b, c, in[0] + 0xeaa127fa, 11);
    VxMD5STEP(VxF3, c, d, a, b, in[3] + 0xd4ef3085, 16);
    VxMD5STEP(VxF3, b, c, d, a, in[6] + 0x04881d05, 23);
    VxMD5STEP(VxF3, a, b, c, d, in[9] + 0xd9d4d039, 4);
    VxMD5STEP(VxF3, d, a, b, c, in[12] + 0xe6db99e5, 11);
    VxMD5STEP(VxF3, c, d, a, b, in[15] + 0x1fa27cf8, 16);
    VxMD5STEP(VxF3, b, c, d, a, in[2] + 0xc4ac5665, 23);

    VxMD5STEP(VxF4, a, b, c, d, in[0] + 0xf4292244, 6);
    VxMD5STEP(VxF4, d, a, b, c, in[7] + 0x432aff97, 10);
    VxMD5STEP(VxF4, c, d, a, b, in[14] + 0xab9423a7, 15);
    VxMD5STEP(VxF4, b, c, d, a, in[5] + 0xfc93a039, 21);
    VxMD5STEP(VxF4, a, b, c, d, in[12] + 0x655b59c3, 6);
    VxMD5STEP(VxF4, d, a, b, c, in[3] + 0x8f0ccc92, 10);
    VxMD5STEP(VxF4, c, d, a, b, in[10] + 0xffeff47d, 15);
    VxMD5STEP(VxF4, b, c, d, a, in[1] + 0x85845dd1, 21);
    VxMD5STEP(VxF4, a, b, c, d, in[8] + 0x6fa87e4f, 6);
    VxMD5STEP(VxF4, d, a, b, c, in[15] + 0xfe2ce6e0, 10);
    VxMD5STEP(VxF4, c, d, a, b, in[6] + 0xa3014314, 15);
    VxMD5STEP(VxF4, b, c, d, a, in[13] + 0x4e0811a1, 21);
    VxMD5STEP(VxF4, a, b, c, d, in[4] + 0xf7537e82, 6);
    VxMD5STEP(VxF4, d, a, b, c, in[11] + 0xbd3af235, 10);
    VxMD5STEP(VxF4, c, d, a, b, in[2] + 0x2ad7d2bb, 15);
    VxMD5STEP(VxF4, b, c, d, a, in[9] + 0xeb86d391, 21);

    buf[0] += a;
    buf[1] += b;
    buf[2] += c;
    buf[3] += d;
}
