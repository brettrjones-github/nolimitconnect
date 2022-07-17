/* bithelp.h  -  Some bit manipulation helpers
 *	Copyright (C) 1999, 2002 Free Software Foundation, Inc.
 *
 * This file is part of Libgcrypt.
 *
 * Libgcrypt is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser general Public License as
 * published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * Libgcrypt is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 */
#ifndef G10_BITHELP_H
#define G10_BITHELP_H

#include "types_gnu.h"


/****************
 * Rotate the 32 bit unsigned integer X by N bits left/right
 */
static NLC_INLINE uint32_t rol(uint32_t x, int n)
{
	return ( (x << (n&(32-1))) | (x >> ((32-n)&(32-1))) );
}

static NLC_INLINE uint32_t ror(uint32_t x, int n)
{
	return ( (x >> (n&(32-1))) | (x << ((32-n)&(32-1))) );
}

/* Byte swap for 32-bit and 64-bit integers.  If available, use compiler
   provided helpers.  */
#ifdef HAVE_BUILTIN_BSWAP32
# define _gcry_bswap32 __builtin_bswap32
#else
static NLC_INLINE uint32_t
_gcry_bswap32(uint32_t x)
{
	return ((rol(x, 8) & 0x00ff00ffL) | (ror(x, 8) & 0xff00ff00L));
}
#endif

#ifdef HAVE_U64_TYPEDEF
# ifdef HAVE_BUILTIN_BSWAP64
#  define _gcry_bswap64 __builtin_bswap64
# else
static NLC_INLINE uint64_t
_gcry_bswap64(uint64_t x)
{
	return ((uint64_t)_gcry_bswap32(x) << 32) | (_gcry_bswap32(x >> 32));
}
# endif
#endif

/* Endian dependent byte swap operations.  */
#ifdef GOTV_ARCH_BIGENDIAN
# define le_bswap32(x) _gcry_bswap32(x)
# define be_bswap32(x) ((uint32_t)(x))
# ifdef HAVE_U64_TYPEDEF
#  define le_bswap64(x) _gcry_bswap64(x)
#  define be_bswap64(x) ((uint64_t)(x))
# endif
#else
# define le_bswap32(x) ((uint32_t)(x))
# define be_bswap32(x) _gcry_bswap32(x)
# ifdef HAVE_U64_TYPEDEF
#  define le_bswap64(x) ((uint64_t)(x))
#  define be_bswap64(x) _gcry_bswap64(x)
# endif
#endif

#endif /*G10_BITHELP_H*/
