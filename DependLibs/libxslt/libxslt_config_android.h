/*
* Summary: internal header only used during the compilation of libxslt
* Description: internal header only used during the compilation of libxslt
*
* Copy: See Copyright for the status of this software.
*
* Author: Daniel Veillard
*/

#ifndef LIBXSLT_CONFIG_ANDROID_H_
# define LIBXSLT_CONFIG_ANDROID_H_

/**
* Locale support
*/
#ifdef TARGET_OS_WINDOWS
#ifndef XSLT_LOCALE_WINAPI
#define XSLT_LOCALE_WINAPI
#endif // XSLT_LOCALE_WINAPI

#define _WINSOCKAPI_
#ifndef __LIBXSLT_WIN32_CONFIG__
# define __LIBXSLT_WIN32_CONFIG__
#endif // __LIBXSLT_WIN32_CONFIG__
#else // ! TARGET_OS_WINDOWS
#ifndef XSLT_LOCALE_XLOCALE
#define XSLT_LOCALE_XLOCALE
#endif
#endif // TARGET_OS_WINDOWS

/**
* XSLT_NEED_TRIO:
*
* should be activated if the existing libc library lacks some of the
* string formatting function, in that case reuse the Trio ones already
* compiled in the libxml2 library.
*/

#if 0
#define XSLT_NEED_TRIO
#endif
#ifdef __VMS
#define HAVE_MATH_H 1
#define HAVE_SYS_STAT_H 1
#ifndef XSLT_NEED_TRIO
#define XSLT_NEED_TRIO
#endif
#endif

#ifdef	XSLT_NEED_TRIO
#define	TRIO_REPLACE_STDIO
#endif

/**
* WITH_XSLT_DEBUGGER:
*
* Activate the compilation of the debugger support. Speed penalty
* is insignifiant.
* On by default unless --without-debugger is passed to configure
*/
#if 1
#ifndef WITH_DEBUGGER
#define WITH_DEBUGGER
#endif
#endif

/**
* WITH_MODULES:
*
* Whether module support is configured into libxslt
* Note: no default module path for win32 platforms
*/
#if 0
#ifndef WITH_MODULES
#define WITH_MODULES
#endif
#define LIBXSLT_DEFAULT_PLUGINS_PATH() "//home/jack/xbmc-depends/arm-linux-androideabi-android-21/lib/libxslt-plugins"
#endif



/**
* ATTRIBUTE_UNUSED:
*
* This macro is used to flag unused function parameters to GCC
*/
#ifdef __GNUC__
#ifdef HAVE_ANSIDECL_H
#include <ansidecl.h>
#endif
#ifndef ATTRIBUTE_UNUSED
#define ATTRIBUTE_UNUSED __attribute__((unused))
#endif
#else
#define ATTRIBUTE_UNUSED
#endif

/**
* LIBXSLT_ATTR_FORMAT:
*
* This macro is used to indicate to GCC the parameters are printf-like
*/
#ifdef __GNUC__
#define LIBXSLT_ATTR_FORMAT(fmt,args) __attribute__((__format__(__printf__,fmt,args)))
#else
#define LIBXSLT_ATTR_FORMAT(fmt,args)
#endif

/**
* LIBXSLT_PUBLIC:
*
* This macro is used to declare PUBLIC variables for Cygwin and for MSC on Windows
*/
#if !defined LIBXSLT_PUBLIC
#if (defined(__CYGWIN__) || defined _MSC_VER) && !defined IN_LIBXSLT && !defined LIBXSLT_STATIC && !defined(LIB_STATIC)
#define LIBXSLT_PUBLIC __declspec(dllimport)
#else
#define LIBXSLT_PUBLIC
#endif
#endif

#define HAVE_CTYPE_H 1
#define HAVE_STDLIB_H 1
#define HAVE_STDARG_H 1
#define HAVE_MALLOC_H 1
#define HAVE_TIME_H 1
#define HAVE_LOCALTIME 1
#define HAVE_GMTIME 1
#define HAVE_TIME 1
#define HAVE_MATH_H 1
#define HAVE_FCNTL_H 1

//#include <io.h>

#define HAVE_ISINF
#define HAVE_ISNAN

#include <math.h>
#if defined _MSC_VER || defined __MINGW32__
/* MS C-runtime has functions which can be used in order to determine if
a given floating-point variable contains NaN, (+-)INF. These are
preferred, because floating-point technology is considered propriatary
by MS and we can assume that their functions know more about their
oddities than we do. */
#include <float.h>
/* Bjorn Reese figured a quite nice construct for isinf() using the
_fpclass() function. */
#ifndef isinf
#define isinf(d) ((_fpclass(d) == _FPCLASS_PINF) ? 1 \
	: ((_fpclass(d) == _FPCLASS_NINF) ? -1 : 0))
#endif
/* _isnan(x) returns nonzero if (x == NaN) and zero otherwise. */
#ifndef isnan
#define isnan(d) (_isnan(d))
#endif
#else /* _MSC_VER */
#ifndef TARGET_OS_ANDROID
static int isinf( double d ) {
	int expon = 0;
	double val = frexp( d, &expon );
	if ( expon == 1025 ) {
		if ( val == 0.5 ) {
			return 1;
		}
		else if ( val == -0.5 ) {
			return -1;
		}
		else {
			return 0;
		}
	}
	else {
		return 0;
	}
}
static int isnan( double d ) {
	int expon = 0;
	double val = frexp( d, &expon );
	if ( expon == 1025 ) {
		if ( val == 0.5 ) {
			return 0;
		}
		else if ( val == -0.5 ) {
			return 0;
		}
		else {
			return 1;
		}
	}
	else {
		return 0;
	}
}
#endif // TARGET_OS_ANDROID
#endif /* _MSC_VER */

//#include <direct.h>

/* snprintf emulation taken from http://stackoverflow.com/a/8712996/1956010 */
#if defined(_MSC_VER) && _MSC_VER < 1900

#include <stdarg.h>
#include <stdio.h>

#define snprintf c99_snprintf
#define vsnprintf c99_vsnprintf

__inline int c99_vsnprintf( char *outBuf, size_t size, const char *format, va_list ap )
{
	int count = -1;

	if ( size != 0 )
		count = _vsnprintf_s( outBuf, size, _TRUNCATE, format, ap );
	if ( count == -1 )
		count = _vscprintf( format, ap );

	return count;
}

__inline int c99_snprintf( char *outBuf, size_t size, const char *format, ... )
{
	int count;
	va_list ap;

	va_start( ap, format );
	count = c99_vsnprintf( outBuf, size, format, ap );
	va_end( ap );

	return count;
}

#endif /* defined(_MSC_VER) && _MSC_VER < 1900 */

//#define HAVE_SYS_STAT_H // defined in AppCompilerConfig.h
#define HAVE__STAT
// #define HAVE_STRING_H // defined in AppCompilerConfig.h

#ifndef ATTRIBUTE_UNUSED
#define ATTRIBUTE_UNUSED
#endif


#endif /* ! __LIBXSLT_CONFIG_ANDROID_H__ */
