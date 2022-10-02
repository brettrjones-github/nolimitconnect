
#include "config_corelib.h"
#ifdef TARGET_OS_ANDROID

#include <stdio.h>
#include <memory.h>
#include <malloc.h>
#include <stdlib.h>  
#include <stdarg.h>  


	/*
	* We do not implement alternate representations. However, we always
	* check whether a given modifier is allowed for a certain conversion.
	*/
#define ALT_E      0x01
#define ALT_O      0x02
#define  LEGAL_ALT(x)    { if (alt_format & ~(x)) return NULL; }


static const u_char *conv_num( const unsigned char *, int *, uint, uint );


//============================================================================
static const u_char * conv_num( const unsigned char *buf, int *dest, uint llim, uint ulim )
{
	uint result = 0;
	unsigned char ch;

	/* The limit also determines the number of valid digits. */
	uint rulim = ulim;

	ch = *buf;
	if( ch < '0' || ch > '9' )
		return NULL;

	do {
		result *= 10;
		result += ch - '0';
		rulim /= 10;
		ch = *++buf;
	} while( ( result * 10 <= ulim ) && rulim && ch >= '0' && ch <= '9' );

	if( result < llim || result > ulim )
		return NULL;

	*dest = result;
	return buf;
}



//============================================================================
/* Compare strings while treating digits characters numerically.
	Copyright (C) 1997, 2000 Free Software Foundation, Inc.
	This file is part of the GNU C Library.
	Contributed by Jean-Franois Bignolles <bignolle@ecoledoc.ibp.fr>, 1997.

	The GNU C Library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Library General Public License as
	published by the Free Software Foundation; either version 2 of the
	License, or (at your option) any later version.

	The GNU C Library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Library General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with XBMC; see the file COPYING.  If not, see
	<http://www.gnu.org/licenses/>.
*/

	#include <string.h>
	#include <ctype.h>
#ifdef __cplusplus
extern "C" {
#endif
	/* states: S_N: normal, S_I: comparing integral part, S_F: comparing
	fractional parts, S_Z: idem but with leading Zeroes only */
#define S_N    0x0
#define S_I    0x4
#define S_F    0x8
#define S_Z    0xC

	/* result_type: CMP: return diff; LEN: compare using len_diff/diff */
#define CMP    2
#define LEN    3


	/* ISDIGIT differs from isdigit, as follows:
	- Its arg may be any int or unsigned int; it need not be an unsigned char.
	- It's guaranteed to evaluate its argument exactly once.
	- It's typically faster.
	Posix 1003.2-1992 section 2.5.2.1 page 50 lines 1556-1558 says that
	only '0' through '9' are digits.  Prefer ISDIGIT to isdigit unless
	it's important to use the locale's definition of `digit' even when the
	host does not conform to Posix.  */
#define ISDIGIT(c) ((unsigned) (c) - '0' <= 9)

#undef __strverscmp
#undef strverscmp

#ifndef weak_alias
# define __strverscmp strverscmp
#endif

	/* Compare S1 and S2 as strings holding indices/version numbers,
	returning less than, equal to or greater than zero if S1 is less than,
	equal to or greater than S2 (for more info, see the texinfo doc).
	*/

int __strverscmp( const char*s1, const char*s2 )
{
	const unsigned char *p1 = ( const unsigned char * )s1;
	const unsigned char *p2 = ( const unsigned char * )s2;
	unsigned char c1, c2;
	int state;
	int diff;

	/* Symbol(s)    0       [1-9]   others  (padding)
	Transition   (10) 0  (01) d  (00) x  (11) -   */
	static const unsigned int next_state[] =
	{
		/* state    x    d    0    - */
		/* S_N */  S_N, S_I, S_Z, S_N,
		/* S_I */  S_N, S_I, S_I, S_I,
		/* S_F */  S_N, S_F, S_F, S_F,
		/* S_Z */  S_N, S_F, S_Z, S_Z
	};

	static const int result_type[] =
	{
		/* state   x/x  x/d  x/0  x/-  d/x  d/d  d/0  d/-
		0/x  0/d  0/0  0/-  -/x  -/d  -/0  -/- */

		/* S_N */  CMP, CMP, CMP, CMP, CMP, LEN, CMP, CMP,
		CMP, CMP, CMP, CMP, CMP, CMP, CMP, CMP,
		/* S_I */  CMP, -1,  -1,  CMP,  1,  LEN, LEN, CMP,
		1,  LEN, LEN, CMP, CMP, CMP, CMP, CMP,
		/* S_F */  CMP, CMP, CMP, CMP, CMP, LEN, CMP, CMP,
		CMP, CMP, CMP, CMP, CMP, CMP, CMP, CMP,
		/* S_Z */  CMP,  1,   1,  CMP, -1,  CMP, CMP, CMP,
		-1,  CMP, CMP, CMP
	};

	if( p1 == p2 )
		return 0;

	c1 = *p1++;
	c2 = *p2++;
	/* Hint: '0' is a digit too.  */
	state = S_N | ( ( c1 == '0' ) + ( ISDIGIT( c1 ) != 0 ) );

	while( ( diff = c1 - c2 ) == 0 && c1 != '\0' )
	{
		state = next_state[ state ];
		c1 = *p1++;
		c2 = *p2++;
		state |= ( c1 == '0' ) + ( ISDIGIT( c1 ) != 0 );
	}

	state = result_type[ state << 2 | ( ( c2 == '0' ) + ( ISDIGIT( c2 ) != 0 ) ) ];

	switch( state )
	{
	case CMP:
		return diff;

	case LEN:
		while( ISDIGIT( *p1++ ) )
			if( !ISDIGIT( *p2++ ) )
				return 1;

		return ISDIGIT( *p2 ) ? -1 : diff;

	default:
		return state;
	}
}

/* this is not missing when using ndk 22b
//============================================================================
int ffs( int i )
{
#if __GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 4)
	return __builtin_ffs( i );
#else
    // http://graphics.stanford.edu/~seander/bithacks.html#ZerosOnRightMultLookup
    // gives this deBruijn constant for a branch-less computation, although
    // that table counted trailing zeros rather than bit position.  This
    // requires 32-bit int, we fall back to a naive algorithm on the rare
    // platforms where that assumption is not true.
	if( CHAR_BIT * sizeof i == 32 )
	{
		static unsigned int table[] = {
			1, 2, 29, 3, 30, 15, 25, 4, 31, 23, 21, 16, 26, 18, 5, 9,
			32, 28, 14, 24, 22, 20, 17, 8, 27, 13, 19, 7, 12, 6, 11, 10
		};
		unsigned int u = i;
		unsigned int bit = u & -u;
		return table[ ( bit * 0x077cb531U ) >> 27 ] - !i;
	}
	else
	{
		unsigned int j;
		for( j = 0; j < CHAR_BIT * sizeof i; j++ )
			if( i & ( 1U << j ) )
				return j + 1;
		return 0;
	}
#endif
}
*/

#ifdef weak_alias
weak_alias( __strverscmp, strverscmp )
#endif

void * mempcpy (void *dest, const void *src, size_t n)
{
  return (void *) ((char *) memcpy (dest, src, n) + n);
}

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // TARGET_OS_ANDROID
