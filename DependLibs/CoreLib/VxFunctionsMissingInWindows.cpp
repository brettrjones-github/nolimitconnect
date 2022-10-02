
#include "config_corelib.h"
#ifdef TARGET_OS_WINDOWS
#include <direct.h>  

#include <stdio.h>
#include <memory.h>
#include <malloc.h>
#include <stdlib.h>  
#include <stdarg.h>  
#include <io.h>  

// this function was copied from libnfs .. implements linux like poll() function
int win32_poll( struct pollfd *fds, unsigned int nfds, int timo )
{
    struct timeval timeout, *toptr;
    fd_set ifds, ofds, efds, *ip, *op;
    unsigned int i;
    int  rc;

    // Set up the file-descriptor sets in ifds, ofds and efds. 
    FD_ZERO( &ifds );
    FD_ZERO( &ofds );
    FD_ZERO( &efds );
    for( i = 0, op = ip = 0; i < nfds; ++i )
    {
        fds[ i ].revents = 0;
        if( fds[ i ].events & ( POLLIN | POLLPRI ) )
        {
            ip = &ifds;
            FD_SET( fds[ i ].fd, ip );
        }
        if( fds[ i ].events & POLLOUT )
        {
            op = &ofds;
            FD_SET( fds[ i ].fd, op );
        }
        FD_SET( fds[ i ].fd, &efds );
    }

    // Set up the timeval structure for the timeout parameter
    if( timo < 0 )
    {
        toptr = 0;
    }
    else
    {
        toptr = &timeout;
        timeout.tv_sec = timo / 1000;
        timeout.tv_usec = ( timo - timeout.tv_sec * 1000 ) * 1000;
    }

#ifdef DEBUG_POLL
    printf( "Entering select() sec=%ld usec=%ld ip=%lx op=%lx\n",
        ( long )timeout.tv_sec, ( long )timeout.tv_usec, ( long )ip, ( long )op );
#endif
    rc = select( 0, ip, op, &efds, toptr );
#ifdef DEBUG_POLL
    printf( "Exiting select rc=%d\n", rc );
#endif

    if( rc <= 0 )
        return rc;

    if( rc > 0 )
    {
        for( i = 0; i < nfds; ++i )
        {
            int fd = fds[ i ].fd;
            if( fds[ i ].events & ( POLLIN | POLLPRI ) && FD_ISSET( fd, &ifds ) )
                fds[ i ].revents |= POLLIN;
            if( fds[ i ].events & POLLOUT && FD_ISSET( fd, &ofds ) )
                fds[ i ].revents |= POLLOUT;
            if( FD_ISSET( fd, &efds ) ) // Some error was detected ... should be some way to know.
                fds[ i ].revents |= POLLHUP;
#ifdef DEBUG_POLL
            printf( "%d %d %d revent = %x\n",
                    FD_ISSET( fd, &ifds ), FD_ISSET( fd, &ofds ), FD_ISSET( fd, &efds ),
                    fds[ i ].revents
            );
#endif
        }
    }
    return rc;
}


// create gettimeofday function that acts like the unix version
#if defined(_MSC_VER) || defined(_MSC_EXTENSIONS)
#define DEF_DELTA_EPOCH_IN_MICROSECS  11644473600000000Ui64
#else
#define DEF_DELTA_EPOCH_IN_MICROSECS  11644473600000000ULL
#endif

const __int64 DELTA_EPOCH_IN_MICROSECS = DEF_DELTA_EPOCH_IN_MICROSECS;

#ifdef __cplusplus
extern "C" {
#endif

	//============================================================================
	// mostly snippets from stack overflow.. no license
	//============================================================================
	time_t timegm( struct tm *tm )
	{
		time_t t;
		int tmYear = tm->tm_year + 1900, tmMonth = tm->tm_mon + 1, tmDay = tm->tm_mday;
		if( tmMonth < 3 )
		{
			tmMonth += 12;
			tmYear--;
		}

		t = 86400LL *
			( tmDay + ( 153 * tmMonth - 457 ) / 5 + 365 * tmYear + tmYear / 4 - tmYear / 100 + tmYear / 400 - 719469 );

		t += 3600 * tm->tm_hour + 60 * tm->tm_min + tm->tm_sec;
		return t;
	}


	//============================================================================
	char * strndup( const char*_s, size_t _n )
	{
		if( _s == NULL )
			return NULL;
		else
		{
            const size_t length = strlen( _s );
            const size_t bytes = length > _n ? _n : length;
            char *new_string = (char *)malloc( bytes + 1 );

			if( new_string )
			{
				memcpy( new_string, _s, bytes );
				new_string[ bytes ] = '\0';
			}

			return new_string;
		}
	}


	//============================================================================
	struct tm *	gmtime_r( const time_t * timep, struct tm * result )
	{
		errno_t err = gmtime_s( result, timep );
		if( 0 != err )
		{
			return 0;
		}

		return result;
	}

	//============================================================================
	struct tm * localtime_r( const time_t * timep, struct tm * result )
	{
		errno_t err = localtime_s( result, timep );
		if( 0 != err )
		{
			return 0;
		}

		return result;
	}

	//============================================================================
	size_t getline( char **lineptr, size_t *n, FILE *stream ) 
	{
		char *bufptr = NULL;
		char *p = bufptr;
		size_t size;
		int c;

		if( lineptr == NULL ) {
			return -1;
		}

		if( stream == NULL ) 
		{
			return -1;
		}

		if( n == NULL ) 
		{
			return -1;
		}

		bufptr = *lineptr;
		size = *n;

		c = fgetc( stream );
		if( c == EOF ) 
		{
			return -1;
		}

		if( bufptr == NULL ) 
		{
			bufptr = (char *)malloc( 256 );
			if( bufptr == NULL ) 
			{
				return -1;
			}

			size = 256;
		}

		p = bufptr;
		while( c != EOF ) 
		{
			if( ( p - bufptr ) > ( size - 1 ) ) 
			{
				size = size + 256;
				bufptr = (char *)realloc( bufptr, size );
				if( bufptr == NULL ) 
				{
					return -1;
				}
			}

			*p++ = c;
			if( c == '\n' ) 
			{
				break;
			}

			c = fgetc( stream );
		}

		*p++ = '\0';
		*lineptr = bufptr;
		*n = size;

		return p - bufptr - 1;
	}

	//============================================================================
	// create temp unique directory in given path 
	char* mkdtemp( char* path )
	{
		if( path == NULL ) {
			errno = EINVAL;
			return NULL;
		}

		// 'path' must be terminated with six 'X'
		const char kSuffix[] = "XXXXXX";
		const size_t kSuffixLen = strlen( kSuffix );
		char* path_end = path + strlen( path );

		if( static_cast< size_t >( path_end - path ) < kSuffixLen ||
			memcmp( path_end - kSuffixLen, kSuffix, kSuffixLen ) != 0 ) {
			errno = EINVAL;
			return NULL;
		}

		// If 'path' contains a directory separator, check that it exists to
		// avoid looping later.
		char* sep = strrchr( path, '/' );
		if( sep != NULL ) {
			struct stat st;
			int ret;
			*sep = '\0';  // temporarily zero-terminate the dirname.
			ret = stat( path, &st );
			*sep = '/';   // restore full path.
			if( ret < 0 )
				return NULL;
			if( !S_ISDIR( st.st_mode ) ) {
				errno = ENOTDIR;
				return NULL;
			}
		}

		// Loop. On each iteration, replace the XXXXXX suffix with a random
		// number.
		int tries;
		for( tries = 128; tries > 0; tries-- ) {
			int randomVal = rand() % 1000000;

			snprintf( path_end - kSuffixLen, kSuffixLen + 1, "%0d", randomVal );
			if( mkdir_os( path, 0700 ) == 0 )
				return path;  // Success

			if( errno != EEXIST )
				return NULL;
		}

		//assert( errno == EEXIST );
		return NULL;
	}

	//============================================================================
	int	setenv( const char*name, const char*value, int overwrite )
	{
		int errcode = 0;
		if( !overwrite )
		{
			size_t envsize = 0;
			errcode = getenv_s( &envsize, NULL, 0, name );
			if( errcode || envsize ) return errcode;
		}

		return _putenv_s( name, value );
	}

	//============================================================================
	// Find the first occurrence of the byte string s in byte string
	const void * memmem( const void *l, size_t l_len, const void *s, size_t s_len )
	{
		register char *cur, *last;
		const char*cl = ( const char* )l;
		const char*cs = ( const char* )s;

		/* we need something to compare */
		if( l_len == 0 || s_len == 0 )
			return NULL;

		/* "s" must be smaller or equal to "l" */
		if( l_len < s_len )
			return NULL;

		/* special case where s_len == 1 */
		if( s_len == 1 )
			return memchr( l, ( int )*cs, l_len );

		/* the last position where its possible to find "s" in "l" */
		last = ( char * )cl + l_len - s_len;

		for( cur = ( char * )cl; cur <= last; cur++ )
			if( cur[ 0 ] == cs[ 0 ] && memcmp( cur, cs, s_len ) == 0 )
				return cur;

		return NULL;
	}

	//============================================================================
	char * stpcpy( char *a, const char*b )
	{
		while( *b )
			*a++ = *b++;
		*a = 0;

		return ( char* )a;
	}

	//============================================================================
	/* IN UNIX the use of the timezone struct is obsolete;
	I don't know why you use it. See http://linux.about.com/od/commands/l/blcmdl2_gettime.htm
	But if you want to use this structure to know about GMT(UTC) difference from your local time
	it will be next: tz_minuteswest is the real difference in minutes from GMT(UTC) and a tz_dsttime is a flag
	indicates whether daylight is now in use
	*/
	//struct timezone2
	//{
	//	__int32  tz_minuteswest; /* minutes W of Greenwich */
	//	bool  tz_dsttime;     /* type of dst correction */
	//};
	//
	//struct timeval2 {
	//	__int32    tv_sec;         /* seconds */
	//	__int32    tv_usec;        /* microseconds */
	//};

	//============================================================================
	int gettimeofday( struct timeval *tv/*in*/, struct timezone *tz/*in*/ )
	{
		FILETIME ft;
		__int64 tmpres = 0;
		TIME_ZONE_INFORMATION tz_winapi;
		int rez = 0;
		ZeroMemory( &ft, sizeof( ft ) );
		ZeroMemory( &tz_winapi, sizeof( tz_winapi ) );

		GetSystemTimeAsFileTime( &ft );

		tmpres = ft.dwHighDateTime;
		tmpres <<= 32;
		tmpres |= ft.dwLowDateTime;

		/*converting file time to unix epoch*/
		tmpres /= 10;  /*convert into microseconds*/
		tmpres -= DELTA_EPOCH_IN_MICROSECS;
		tv->tv_sec = ( __int32 )( tmpres*0.000001 );
		tv->tv_usec = ( tmpres % 1000000 );


		//_tzset(),don't work properly, so we use GetTimeZoneInformation
        if( tz ) // it is normal for tz to be passed as null so do check for null
        {
		    rez = GetTimeZoneInformation( &tz_winapi );
		    tz->tz_dsttime = ( rez == 2 ) ? true : false;
		    tz->tz_minuteswest = tz_winapi.Bias + ( ( rez == 2 ) ? tz_winapi.DaylightBias : 0 );
        }

		return 0;
	}

	//============================================================================
		//void UnixTimeToFileTime( time_t t, LPFILETIME pft )
		//{
		//	// Note that LONGLONG is a 64-bit value
		//	LONGLONG ll;

		//	ll = Int32x32To64( t, 10000000 ) + 116444736000000000;
		//	pft->dwLowDateTime = (DWORD)ll;
		//	pft->dwHighDateTime = ll >> 32;
		//}

		////============================================================================
		//int gettimeofday( struct timeval *tv, struct timezone *tz )
		//{
		//	FILETIME ft;
		//	unsigned __int64 tmpres = 0;
		//	static int tzflag;

		//	if ( NULL != tv )
		//	{
		//		GetSystemTimeAsFileTime( &ft );

		//		tmpres |= ft.dwHighDateTime;
		//		tmpres <<= 32;
		//		tmpres |= ft.dwLowDateTime;

		//		/*converting file time to unix epoch*/
		//		tmpres -= DELTA_EPOCH_IN_MICROSECS;
		//		tmpres /= 10;  /*convert into microseconds*/
		//		tv->tv_sec = (long)( tmpres / 1000000UL );
		//		tv->tv_usec = (long)( tmpres % 1000000UL );
		//	}

		//	if ( NULL != tz )
		//	{
		//		if ( !tzflag )
		//		{
		//			_tzset();
		//			tzflag++;
		//		}
		//		//tz->tz_minuteswest = _timezone / 60;
		//		//tz->tz_dsttime = _daylight;
		//	}
		//	return 0;
		//}


	//============================================================================
	int vasprintf( char **strp, const char*fmt, va_list ap )
	{
		// _vscprintf tells you how big the buffer needs to be
		int len = _vscprintf( fmt, ap );
		if( len == -1 )
		{
			return -1;
		}

		size_t size = ( size_t )len + 1;
		char * str = ( char * )malloc( size );
		if( !str )
		{
			return -1;
		}

		// _vsprintf_s is the "secure" version of vsprintf
		int r = vsprintf_s( str, len + 1, fmt, ap );
		if( r == -1 )
		{
			free( str );
			return -1;
		}

		*strp = str;
		return r;
	}

	//============================================================================
	int asprintf( char **strp, const char*fmt, ... )
	{
		va_list ap;
		va_start( ap, fmt );
		int r = vasprintf( strp, fmt, ap );
		va_end( ap );
		return r;
	}

	//============================================================================
	uint32_t getpagesize( void )
	{
		SYSTEM_INFO si;
		GetSystemInfo( &si );
		return si.dwPageSize;
	}


	//============================================================================
	// from KODI platform lib
		/* case-independent string matching, similar to strstr but
		* matching */
    /* exists in kodi so is commented out for now
	char * strcasestr( const char* haystack, const char* needle )
	{
		int i;
		int nlength = ( int )strlen( needle );
		int hlength = ( int )strlen( haystack );

		if( nlength > hlength ) return NULL;
		if( hlength <= 0 ) return NULL;
		if( nlength <= 0 ) return ( char * )haystack;
		// hlength and nlength > 0, nlength <= hlength 
		for( i = 0; i <= ( hlength - nlength ); i++ )
		{
			if( strncasecmp( haystack + i, needle, nlength ) == 0 )
			{
				return ( char * )haystack + i;
			}
		}
		// substring not found 
		return NULL;
	}
    */


	LARGE_INTEGER getFILETIMEoffset()
	{
		SYSTEMTIME s;
		FILETIME f;
		LARGE_INTEGER t;

		s.wYear = 1970;
		s.wMonth = 1;
		s.wDay = 1;
		s.wHour = 0;
		s.wMinute = 0;
		s.wSecond = 0;
		s.wMilliseconds = 0;
		SystemTimeToFileTime( &s, &f );
		t.QuadPart = f.dwHighDateTime;
		t.QuadPart <<= 32;
		t.QuadPart |= f.dwLowDateTime;
		return ( t );
	}

	int clock_gettime( int X, struct timeval *tv )
	{
		LARGE_INTEGER           t;
		FILETIME            f;
		double                  microseconds;
		static LARGE_INTEGER    offset;
		static double           frequencyToMicroseconds;
		static int              initialized = 0;
		static BOOL             usePerformanceCounter = 0;

		if( !initialized ) {
			LARGE_INTEGER performanceFrequency;
			initialized = 1;
			usePerformanceCounter = QueryPerformanceFrequency( &performanceFrequency );
			if( usePerformanceCounter ) {
				QueryPerformanceCounter( &offset );
				frequencyToMicroseconds = ( double )performanceFrequency.QuadPart / 1000000.;
			}
			else {
				offset = getFILETIMEoffset();
				frequencyToMicroseconds = 10.;
			}
		}
		if( usePerformanceCounter ) QueryPerformanceCounter( &t );
		else {
			GetSystemTimeAsFileTime( &f );
			t.QuadPart = f.dwHighDateTime;
			t.QuadPart <<= 32;
			t.QuadPart |= f.dwLowDateTime;
		}

		t.QuadPart -= offset.QuadPart;
		microseconds = ( double )t.QuadPart / frequencyToMicroseconds;
		t.QuadPart = microseconds;
		tv->tv_sec = t.QuadPart / 1000000;
		tv->tv_usec = t.QuadPart % 1000000;
		return ( 0 );
	}



	//============================================================================
	/*
	* Ported from NetBSD to Windows by Ron Koenderink, 2007
	*/

	/*  $NetBSD: strptime.c,v 1.25 2005/11/29 03:12:00 christos Exp $  */

	/*-
	* Copyright (c) 1997, 1998, 2005 The NetBSD Foundation, Inc.
	* All rights reserved.
	*
	* This code was contributed to The NetBSD Foundation by Klaus Klein.
	* Heavily optimised by David Laight
	*
	* Redistribution and use in source and binary forms, with or without
	* modification, are permitted provided that the following conditions
	* are met:
	* 1. Redistributions of source code must retain the above copyright
	*    notice, this list of conditions and the following disclaimer.
	* 2. Redistributions in binary form must reproduce the above copyright
	*    notice, this list of conditions and the following disclaimer in the
	*    documentation and/or other materials provided with the distribution.
	* 3. All advertising materials mentioning features or use of this software
	*    must display the following acknowledgement:
	*        This product includes software developed by the NetBSD
	*        Foundation, Inc. and its contributors.
	* 4. Neither the name of The NetBSD Foundation nor the names of its
	*    contributors may be used to endorse or promote products derived
	*    from this software without specific prior written permission.
	*
	* THIS SOFTWARE IS PROVIDED BY THE NETBSD FOUNDATION, INC. AND CONTRIBUTORS
	* ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
	* TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
	* PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR CONTRIBUTORS
	* BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
	* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
	* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
	* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
	* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
	* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
	* POSSIBILITY OF SUCH DAMAGE.
	*/

#if !defined(TARGET_OS_WINDOWS)
#include <sys/cdefs.h>
#endif

#if defined(LIBC_SCCS) && !defined(lint)
	__RCSID( "$NetBSD: strptime.c,v 1.25 2005/11/29 03:12:00 christos Exp $" );
#endif

#if !defined(TARGET_OS_WINDOWS)
#include "namespace.h"
#include <sys/localedef.h>
#else
	typedef unsigned char u_char;
	typedef unsigned int uint;
#endif
#include <ctype.h>
#include <locale.h>
#include <string.h>
#include <time.h>
#if !defined(TARGET_OS_WINDOWS)
#include <tzfile.h>
#endif

#ifdef __weak_alias
	__weak_alias( strptime, _strptime )
#endif

#if !defined(TARGET_OS_WINDOWS)
#define  _ctloc(x)    (_CurrentTimeLocale->x)
#else
#define _ctloc(x)   (x)
		const char*vx_abday[] = {
		"Sun", "Mon", "Tue", "Wed",
		"Thu", "Fri", "Sat"
	};
	const char*vx_day[] = {
		"Sunday", "Monday", "Tuesday", "Wednesday",
		"Thursday", "Friday", "Saturday"
	};
	const char*vx_abmon[] = {
		"Jan", "Feb", "Mar", "Apr", "May", "Jun",
		"Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
	};
	const char*vx_mon[] = {
		"January", "February", "March", "April", "May", "June",
		"July", "August", "September", "October", "November", "December"
	};
	const char*vx_am_pm[] = {
		"AM", "PM"
	};

    const char*vx_d_t_fmt = "%a %Ef %T %Y";
    const char*vx_t_fmt_ampm = "%I:%M:%S %p";
    const char*vx_t_fmt = "%H:%M:%S";
    const char*vx_d_fmt = "%m/%d/%y";
#define TM_YEAR_BASE 1900
#define __UNCONST(x) ((char *)(((const char*)(x) - (const char*)0) + (char *)0))

#endif
	/*
	* We do not implement alternate representations. However, we always
	* check whether a given modifier is allowed for a certain conversion.
	*/
#define ALT_E      0x01
#define ALT_O      0x02
#define  LEGAL_ALT(x)    { if (alt_format & ~(x)) return NULL; }


static const u_char *conv_num( const unsigned char *, int *, uint, uint );
static const u_char *find_string( const u_char *, int *, const char* const *,
									const char* const *, int );

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
static const u_char * find_string( const u_char *bp, int *tgt, const char* const *n1,
									const char* const *n2, int c )
{
	int i;
	unsigned int len;

	/* check full name - then abbreviated ones */
	for( ; n1 != NULL; n1 = n2, n2 = NULL ) {
		for( i = 0; i < c; i++, n1++ ) {
			len = strlen( *n1 );
			if( strnicmp( *n1, ( const char* )bp, len ) == 0 ) {
				*tgt = i;
				return bp + len;
			}
		}
	}

	/* Nothing matched */
	return NULL;
}

//============================================================================
/* defined in kodi ( libxbmc )
char * strptime( const char*buf, const char*fmt, struct tm *tm )
{
	unsigned char c;
	const unsigned char *bp;
	int alt_format, i, split_year = 0;
	const char*new_fmt;

	bp = ( const u_char * )buf;

	while( bp != NULL && ( c = *fmt++ ) != '\0' )
	{
		// Clear `alternate' modifier prior to new conversion. 
		alt_format = 0;
		i = 0;

		// Eat up white-space. 
		if( isspace( c ) )
		{
			while( isspace( *bp ) )
				bp++;
			continue;
		}

		if( c != '%' )
			goto literal;


	again:
		switch( c = *fmt++ )
		{
		case '%':  // "%%" is converted to "%". 
			literal :
				if( c != *bp++ )
					return NULL;
			LEGAL_ALT( 0 );
			continue;

			//
			// "Alternative" modifiers. Just set the appropriate flag
			// and start over again.
			//
		case 'E':  // "%E?" alternative conversion modifier. 
			LEGAL_ALT( 0 );
			alt_format |= ALT_E;
			goto again;

		case 'O':  // "%O?" alternative conversion modifier. 
			LEGAL_ALT( 0 );
			alt_format |= ALT_O;
			goto again;

			//
			// "Complex" conversion rules, implemented through recursion.
			//
		case 'c':  // Date and time, using the locale's format. 
			new_fmt = _ctloc( vx_d_t_fmt );
			goto recurse;

		case 'D':  // The date as "%m/%d/%y". 
			new_fmt = "%m/%d/%y";
			LEGAL_ALT( 0 );
			goto recurse;

		case 'R':  // The time as "%H:%M". 
			new_fmt = "%H:%M";
			LEGAL_ALT( 0 );
			goto recurse;

		case 'r':  // The time in 12-hour clock representation. 
			new_fmt = _ctloc( vx_t_fmt_ampm );
			LEGAL_ALT( 0 );
			goto recurse;

		case 'T':  // The time as "%H:%M:%S". 
			new_fmt = "%H:%M:%S";
			LEGAL_ALT( 0 );
			goto recurse;

		case 'X':  // The time, using the locale's format. 
			new_fmt = _ctloc( vx_t_fmt );
			goto recurse;

		case 'x':  // The date, using the locale's format. 
			new_fmt = _ctloc( vx_d_fmt );
		recurse:
			bp = ( const u_char * )strptime( ( const char* )bp,
											 new_fmt, tm );
			LEGAL_ALT( ALT_E );
			continue;

			//
			// "Elementary" conversion rules.
			//
		case 'A':  // The day of week, using the locale's form. 
		case 'a':
			bp = find_string( bp, &tm->tm_wday, _ctloc( vx_day ),
							  _ctloc( vx_abday ), 7 );
			LEGAL_ALT( 0 );
			continue;

		case 'B':  // The month, using the locale's form. 
		case 'b':
		case 'h':
			bp = find_string( bp, &tm->tm_mon, _ctloc( vx_mon ),
							  _ctloc( vx_abmon ), 12 );
			LEGAL_ALT( 0 );
			continue;

		case 'C':  // The century number. 
			i = 20;
			bp = conv_num( bp, &i, 0, 99 );

			i = i * 100 - TM_YEAR_BASE;
			if( split_year )
				i += tm->tm_year % 100;
			split_year = 1;
			tm->tm_year = i;
			LEGAL_ALT( ALT_E );
			continue;

		case 'd':  // The day of month. 
		case 'e':
			bp = conv_num( bp, &tm->tm_mday, 1, 31 );
			LEGAL_ALT( ALT_O );
			continue;

		case 'k':  // The hour (24-hour clock representation). 
			LEGAL_ALT( 0 );
			// FALLTHROUGH 
		case 'H':
			bp = conv_num( bp, &tm->tm_hour, 0, 23 );
			LEGAL_ALT( ALT_O );
			continue;

		case 'l':  // The hour (12-hour clock representation). 
			LEGAL_ALT( 0 );
			// FALLTHROUGH 
		case 'I':
			bp = conv_num( bp, &tm->tm_hour, 1, 12 );
			if( tm->tm_hour == 12 )
				tm->tm_hour = 0;
			LEGAL_ALT( ALT_O );
			continue;

		case 'j':  // The day of year. 
			i = 1;
			bp = conv_num( bp, &i, 1, 366 );
			tm->tm_yday = i - 1;
			LEGAL_ALT( 0 );
			continue;

		case 'M':  // The minute. 
			bp = conv_num( bp, &tm->tm_min, 0, 59 );
			LEGAL_ALT( ALT_O );
			continue;

		case 'm':  // The month. 
			i = 1;
			bp = conv_num( bp, &i, 1, 12 );
			tm->tm_mon = i - 1;
			LEGAL_ALT( ALT_O );
			continue;

		case 'p':  // The locale's equivalent of AM/PM. 
			bp = find_string( bp, &i, _ctloc( vx_am_pm ), NULL, 2 );
			if( tm->tm_hour > 11 )
				return NULL;
			tm->tm_hour += i * 12;
			LEGAL_ALT( 0 );
			continue;

		case 'S':  // The seconds. 
			bp = conv_num( bp, &tm->tm_sec, 0, 61 );
			LEGAL_ALT( ALT_O );
			continue;

		case 'U':  // The week of year, beginning on sunday. 
		case 'W':  // The week of year, beginning on monday. 
					//
					// XXX This is bogus, as we can not assume any valid
					// information present in the tm structure at this
					// point to calculate a real value, so just check the
					// range for now.
					//
			bp = conv_num( bp, &i, 0, 53 );
			LEGAL_ALT( ALT_O );
			continue;

		case 'w':  // The day of week, beginning on sunday. 
			bp = conv_num( bp, &tm->tm_wday, 0, 6 );
			LEGAL_ALT( ALT_O );
			continue;

		case 'Y':  // The year. 
			i = TM_YEAR_BASE;  // just for data sanity... 
			bp = conv_num( bp, &i, 0, 9999 );
			tm->tm_year = i - TM_YEAR_BASE;
			LEGAL_ALT( ALT_E );
			continue;

		case 'y':  // The year within 100 years of the epoch. 
					// LEGAL_ALT(ALT_E | ALT_O); 
			bp = conv_num( bp, &i, 0, 99 );

			if( split_year )
				// preserve century 
				i += ( tm->tm_year / 100 ) * 100;
			else {
				split_year = 1;
				if( i <= 68 )
					i = i + 2000 - TM_YEAR_BASE;
				else
					i = i + 1900 - TM_YEAR_BASE;
			}
			tm->tm_year = i;
			continue;

			//
			// Miscellaneous conversions.
			//
		case 'n':  // Any kind of white-space. 
		case 't':
			while( isspace( *bp ) )
				bp++;
			LEGAL_ALT( 0 );
			continue;


		default:  // Unknown/unsupported conversion. 
			return NULL;
		} // end switch
	} // end while

	return __UNCONST( bp );
}
*/

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

	#if HAVE_CONFIG_H
	# include <config.h>
	#endif

	#include <string.h>
	#include <ctype.h>

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


//============================================================================
int ffs( int i )
{
#if __GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 4)
	return __builtin_ffs( i );
#else
	/* http://graphics.stanford.edu/~seander/bithacks.html#ZerosOnRightMultLookup
	gives this deBruijn constant for a branch-less computation, although
	that table counted trailing zeros rather than bit position.  This
	requires 32-bit int, we fall back to a naive algorithm on the rare
	platforms where that assumption is not true.  */
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

//#ifdef weak_alias
//weak_alias( __strverscmp, strverscmp )
//#endif

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // TARGET_OS_WINDOWS
