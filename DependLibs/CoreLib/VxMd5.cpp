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
    LogMsg( LOG_DEBUG, "0 a0x%X b0x%X c0x%X d0x%X", a, b, c, d );
    VxMD5STEP(VxF1, d, a, b, c, in[1] + 0xe8c7b756, 12);
    LogMsg( LOG_DEBUG, "1 a0x%X b0x%X c0x%X d0x%X", a, b, c, d );
    VxMD5STEP(VxF1, c, d, a, b, in[2] + 0x242070db, 17);
    LogMsg( LOG_DEBUG, "2 a0x%X b0x%X c0x%X d0x%X", a, b, c, d );
    VxMD5STEP(VxF1, b, c, d, a, in[3] + 0xc1bdceee, 22);
    LogMsg( LOG_DEBUG, "3 a0x%X b0x%X c0x%X d0x%X", a, b, c, d );
    VxMD5STEP(VxF1, a, b, c, d, in[4] + 0xf57c0faf, 7);
    LogMsg( LOG_DEBUG, "4 a0x%X b0x%X c0x%X d0x%X", a, b, c, d );
    VxMD5STEP(VxF1, d, a, b, c, in[5] + 0x4787c62a, 12);
    LogMsg( LOG_DEBUG, "5 a0x%X b0x%X c0x%X d0x%X", a, b, c, d );
    VxMD5STEP(VxF1, c, d, a, b, in[6] + 0xa8304613, 17);
    LogMsg( LOG_DEBUG, "6 a0x%X b0x%X c0x%X d0x%X", a, b, c, d );
    VxMD5STEP(VxF1, b, c, d, a, in[7] + 0xfd469501, 22);
    LogMsg( LOG_DEBUG, "7 a0x%X b0x%X c0x%X d0x%X", a, b, c, d );
    VxMD5STEP(VxF1, a, b, c, d, in[8] + 0x698098d8, 7);
    LogMsg( LOG_DEBUG, "8 a0x%X b0x%X c0x%X d0x%X", a, b, c, d );
    VxMD5STEP(VxF1, d, a, b, c, in[9] + 0x8b44f7af, 12);
    LogMsg( LOG_DEBUG, "9 a0x%X b0x%X c0x%X d0x%X", a, b, c, d );
    VxMD5STEP(VxF1, c, d, a, b, in[10] + 0xffff5bb1, 17);
    LogMsg( LOG_DEBUG, "10 a0x%X b0x%X c0x%X d0x%X", a, b, c, d );
    VxMD5STEP(VxF1, b, c, d, a, in[11] + 0x895cd7be, 22);
    LogMsg( LOG_DEBUG, "11 a0x%X b0x%X c0x%X d0x%X", a, b, c, d );
    VxMD5STEP(VxF1, a, b, c, d, in[12] + 0x6b901122, 7);
    LogMsg( LOG_DEBUG, "12 a0x%X b0x%X c0x%X d0x%X", a, b, c, d );
    VxMD5STEP(VxF1, d, a, b, c, in[13] + 0xfd987193, 12);
    LogMsg( LOG_DEBUG, "13 a0x%X b0x%X c0x%X d0x%X", a, b, c, d );
    VxMD5STEP(VxF1, c, d, a, b, in[14] + 0xa679438e, 17);
    LogMsg( LOG_DEBUG, "14 a0x%X b0x%X c0x%X d0x%X", a, b, c, d );
    VxMD5STEP(VxF1, b, c, d, a, in[15] + 0x49b40821, 22);
    LogMsg( LOG_DEBUG, "15 a0x%X b0x%X c0x%X d0x%X", a, b, c, d );

    VxMD5STEP(VxF2, a, b, c, d, in[1] + 0xf61e2562, 5);
    LogMsg( LOG_DEBUG, "16 a0x%X b0x%X c0x%X d0x%X", a, b, c, d );
    VxMD5STEP(VxF2, d, a, b, c, in[6] + 0xc040b340, 9);
    LogMsg( LOG_DEBUG, "17 a0x%X b0x%X c0x%X d0x%X", a, b, c, d );
    VxMD5STEP(VxF2, c, d, a, b, in[11] + 0x265e5a51, 14);
    LogMsg( LOG_DEBUG, "18 a0x%X b0x%X c0x%X d0x%X", a, b, c, d );
    VxMD5STEP(VxF2, b, c, d, a, in[0] + 0xe9b6c7aa, 20);
    LogMsg( LOG_DEBUG, "19 a0x%X b0x%X c0x%X d0x%X", a, b, c, d );
    VxMD5STEP(VxF2, a, b, c, d, in[5] + 0xd62f105d, 5);
    LogMsg( LOG_DEBUG, "20 a0x%X b0x%X c0x%X d0x%X", a, b, c, d );
    VxMD5STEP(VxF2, d, a, b, c, in[10] + 0x02441453, 9);
    LogMsg( LOG_DEBUG, "21 a0x%X b0x%X c0x%X d0x%X", a, b, c, d );
    VxMD5STEP(VxF2, c, d, a, b, in[15] + 0xd8a1e681, 14);
    LogMsg( LOG_DEBUG, "22 a0x%X b0x%X c0x%X d0x%X", a, b, c, d );
    VxMD5STEP(VxF2, b, c, d, a, in[4] + 0xe7d3fbc8, 20);
    LogMsg( LOG_DEBUG, "23 a0x%X b0x%X c0x%X d0x%X", a, b, c, d );
    VxMD5STEP(VxF2, a, b, c, d, in[9] + 0x21e1cde6, 5);
    LogMsg( LOG_DEBUG, "24 a0x%X b0x%X c0x%X d0x%X", a, b, c, d );
    VxMD5STEP(VxF2, d, a, b, c, in[14] + 0xc33707d6, 9);
    LogMsg( LOG_DEBUG, "25 a0x%X b0x%X c0x%X d0x%X", a, b, c, d );
    VxMD5STEP(VxF2, c, d, a, b, in[3] + 0xf4d50d87, 14);
    LogMsg( LOG_DEBUG, "26 a0x%X b0x%X c0x%X d0x%X", a, b, c, d );
    VxMD5STEP(VxF2, b, c, d, a, in[8] + 0x455a14ed, 20);
    LogMsg( LOG_DEBUG, "27 a0x%X b0x%X c0x%X d0x%X", a, b, c, d );
    VxMD5STEP(VxF2, a, b, c, d, in[13] + 0xa9e3e905, 5);
    LogMsg( LOG_DEBUG, "28 a0x%X b0x%X c0x%X d0x%X", a, b, c, d );
    VxMD5STEP(VxF2, d, a, b, c, in[2] + 0xfcefa3f8, 9);
    LogMsg( LOG_DEBUG, "29 a0x%X b0x%X c0x%X d0x%X", a, b, c, d );
    VxMD5STEP(VxF2, c, d, a, b, in[7] + 0x676f02d9, 14);
    LogMsg( LOG_DEBUG, "30 a0x%X b0x%X c0x%X d0x%X", a, b, c, d );
    VxMD5STEP(VxF2, b, c, d, a, in[12] + 0x8d2a4c8a, 20);
    LogMsg( LOG_DEBUG, "31 a0x%X b0x%X c0x%X d0x%X", a, b, c, d );

    VxMD5STEP(VxF3, a, b, c, d, in[5] + 0xfffa3942, 4);
    LogMsg( LOG_DEBUG, "32 a0x%X b0x%X c0x%X d0x%X", a, b, c, d );
    VxMD5STEP(VxF3, d, a, b, c, in[8] + 0x8771f681, 11);
    LogMsg( LOG_DEBUG, "33 a0x%X b0x%X c0x%X d0x%X", a, b, c, d );
    VxMD5STEP(VxF3, c, d, a, b, in[11] + 0x6d9d6122, 16);
    LogMsg( LOG_DEBUG, "34 a0x%X b0x%X c0x%X d0x%X", a, b, c, d );
    VxMD5STEP(VxF3, b, c, d, a, in[14] + 0xfde5380c, 23);
    LogMsg( LOG_DEBUG, "35 a0x%X b0x%X c0x%X d0x%X", a, b, c, d );
    VxMD5STEP(VxF3, a, b, c, d, in[1] + 0xa4beea44, 4);
    LogMsg( LOG_DEBUG, "36 a0x%X b0x%X c0x%X d0x%X", a, b, c, d );
    VxMD5STEP(VxF3, d, a, b, c, in[4] + 0x4bdecfa9, 11);
    LogMsg( LOG_DEBUG, "37 a0x%X b0x%X c0x%X d0x%X", a, b, c, d );
    VxMD5STEP(VxF3, c, d, a, b, in[7] + 0xf6bb4b60, 16);
    LogMsg( LOG_DEBUG, "38 a0x%X b0x%X c0x%X d0x%X", a, b, c, d );
    VxMD5STEP(VxF3, b, c, d, a, in[10] + 0xbebfbc70, 23);
    LogMsg( LOG_DEBUG, "39 a0x%X b0x%X c0x%X d0x%X", a, b, c, d );
    VxMD5STEP(VxF3, a, b, c, d, in[13] + 0x289b7ec6, 4);
    LogMsg( LOG_DEBUG, "40 a0x%X b0x%X c0x%X d0x%X", a, b, c, d );
    VxMD5STEP(VxF3, d, a, b, c, in[0] + 0xeaa127fa, 11);
    LogMsg( LOG_DEBUG, "41 a0x%X b0x%X c0x%X d0x%X", a, b, c, d );
    VxMD5STEP(VxF3, c, d, a, b, in[3] + 0xd4ef3085, 16);
    LogMsg( LOG_DEBUG, "42 a0x%X b0x%X c0x%X d0x%X", a, b, c, d );
    VxMD5STEP(VxF3, b, c, d, a, in[6] + 0x04881d05, 23);
    LogMsg( LOG_DEBUG, "43 a0x%X b0x%X c0x%X d0x%X", a, b, c, d );
    VxMD5STEP(VxF3, a, b, c, d, in[9] + 0xd9d4d039, 4);
    LogMsg( LOG_DEBUG, "44 a0x%X b0x%X c0x%X d0x%X", a, b, c, d );
    VxMD5STEP(VxF3, d, a, b, c, in[12] + 0xe6db99e5, 11);
    LogMsg( LOG_DEBUG, "45 a0x%X b0x%X c0x%X d0x%X", a, b, c, d );
    VxMD5STEP(VxF3, c, d, a, b, in[15] + 0x1fa27cf8, 16);
    LogMsg( LOG_DEBUG, "46 a0x%X b0x%X c0x%X d0x%X", a, b, c, d );
    VxMD5STEP(VxF3, b, c, d, a, in[2] + 0xc4ac5665, 23);
    LogMsg( LOG_DEBUG, "47 a0x%X b0x%X c0x%X d0x%X", a, b, c, d );

    VxMD5STEP(VxF4, a, b, c, d, in[0] + 0xf4292244, 6);
    LogMsg( LOG_DEBUG, "48 a0x%X b0x%X c0x%X d0x%X", a, b, c, d );
    VxMD5STEP(VxF4, d, a, b, c, in[7] + 0x432aff97, 10);
    LogMsg( LOG_DEBUG, "49 a0x%X b0x%X c0x%X d0x%X", a, b, c, d );
    VxMD5STEP(VxF4, c, d, a, b, in[14] + 0xab9423a7, 15);
    LogMsg( LOG_DEBUG, "50 a0x%X b0x%X c0x%X d0x%X", a, b, c, d );
    VxMD5STEP(VxF4, b, c, d, a, in[5] + 0xfc93a039, 21);
    LogMsg( LOG_DEBUG, "51 a0x%X b0x%X c0x%X d0x%X", a, b, c, d );
    VxMD5STEP(VxF4, a, b, c, d, in[12] + 0x655b59c3, 6);
    LogMsg( LOG_DEBUG, "52 a0x%X b0x%X c0x%X d0x%X", a, b, c, d );
    VxMD5STEP(VxF4, d, a, b, c, in[3] + 0x8f0ccc92, 10);
    LogMsg( LOG_DEBUG, "53 a0x%X b0x%X c0x%X d0x%X", a, b, c, d );
    VxMD5STEP(VxF4, c, d, a, b, in[10] + 0xffeff47d, 15);
    LogMsg( LOG_DEBUG, "54 a0x%X b0x%X c0x%X d0x%X", a, b, c, d );
    VxMD5STEP(VxF4, b, c, d, a, in[1] + 0x85845dd1, 21);
    LogMsg( LOG_DEBUG, "55 a0x%X b0x%X c0x%X d0x%X", a, b, c, d );
    VxMD5STEP(VxF4, a, b, c, d, in[8] + 0x6fa87e4f, 6);
    LogMsg( LOG_DEBUG, "56 a0x%X b0x%X c0x%X d0x%X", a, b, c, d );
    VxMD5STEP(VxF4, d, a, b, c, in[15] + 0xfe2ce6e0, 10);
    LogMsg( LOG_DEBUG, "57 a0x%X b0x%X c0x%X d0x%X", a, b, c, d );
    VxMD5STEP(VxF4, c, d, a, b, in[6] + 0xa3014314, 15);
    LogMsg( LOG_DEBUG, "58 a0x%X b0x%X c0x%X d0x%X", a, b, c, d );
    VxMD5STEP(VxF4, b, c, d, a, in[13] + 0x4e0811a1, 21);
    LogMsg( LOG_DEBUG, "59 a0x%X b0x%X c0x%X d0x%X", a, b, c, d );
    VxMD5STEP(VxF4, a, b, c, d, in[4] + 0xf7537e82, 6);
    LogMsg( LOG_DEBUG, "60 a0x%X b0x%X c0x%X d0x%X", a, b, c, d );
    VxMD5STEP(VxF4, d, a, b, c, in[11] + 0xbd3af235, 10);
    LogMsg( LOG_DEBUG, "61 a0x%X b0x%X c0x%X d0x%X", a, b, c, d );
    VxMD5STEP(VxF4, c, d, a, b, in[2] + 0x2ad7d2bb, 15);
    LogMsg( LOG_DEBUG, "62 a0x%X b0x%X c0x%X d0x%X", a, b, c, d );
    VxMD5STEP(VxF4, b, c, d, a, in[9] + 0xeb86d391, 21);
    LogMsg( LOG_DEBUG, "63 a0x%X b0x%X c0x%X d0x%X", a, b, c, d );

    LogMsg( LOG_DEBUG, "Result a0x%X b0x%X c0x%X d0x%X", a, b, c, d );
    buf[0] += a;
    buf[1] += b;
    buf[2] += c;
    buf[3] += d;
}
