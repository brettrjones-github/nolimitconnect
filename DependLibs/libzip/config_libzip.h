#pragma once
#include <NlcDependLibrariesConfig.h>
#define ZIP_EXTERN		NLC_EXTERN
#define ZIP_EXPORT		NLC_EXPORT
#define Z_NULL			0

#define LIBZIP_VERSION "1.1.2"
#define LIBZIP_VERSION_MAJOR 1
#define LIBZIP_VERSION_MINOR 1
#define LIBZIP_VERSION_MICRO 2

#ifdef TARGET_OS_ANDROID
# define HAVE_FSEEKO 1
# define HAVE_FTELLO 1
#endif // TARGET_OS_ANDROID

#define  zip_int8_t			int8_t
#define ZIP_INT8_MIN INT8_MIN
#define ZIP_INT8_MAX INT8_MAX

#define  zip_uint8_t		uint8_t
#define ZIP_UINT8_MAX UINT8_MAX

#define  zip_int16_t		int16_t
#define ZIP_INT16_MIN INT16_MIN
#define ZIP_INT16_MAX INT16_MAX

#define zip_uint16_t		uint16_t
#define ZIP_UINT16_MAX UINT16_MAX

#define zip_int32_t			int32_t 
#define ZIP_INT32_MIN INT32_MIN
#define ZIP_INT32_MAX INT32_MAX

#define  zip_uint32_t		uint32_t
#define ZIP_UINT32_MAX UINT32_MAX

#define  zip_int64_t		int64_t
#define ZIP_INT64_MIN INT64_MIN
#define ZIP_INT64_MAX INT64_MAX

#define  zip_uint64_t		uint64_t
#define ZIP_UINT64_MAX UINT64_MAX

#define  uint_t				unsigned int

/* at least MinGW does not provide EOPNOTSUPP, see
* http://sourceforge.net/p/mingw/bugs/263/
*/
#ifndef EOPNOTSUPP
#define EOPNOTSUPP EINVAL
#endif

/* at least MinGW does not provide EOVERFLOW, see
* http://sourceforge.net/p/mingw/bugs/242/
*/
#ifndef EOVERFLOW
#define EOVERFLOW EFBIG
#endif

#ifdef TARGET_OS_WINDOWS
#if defined(HAVE__CLOSE)
#define close		_close
#endif
#if defined(HAVE__DUP)
#define dup		_dup
#endif
/* crashes reported when using fdopen instead of _fdopen on Windows/Visual Studio 10/Win64 */
#if defined(HAVE__FDOPEN)
#define fdopen		_fdopen
#endif
#if !defined(HAVE_FILENO) && defined(HAVE__FILENO)
#define fileno		_fileno
#endif
/* Windows' open() doesn't understand Unix permissions */
#if defined(HAVE__OPEN)
#define open(a, b, c)	_open((a), (b))
#endif
#if defined(HAVE__SNPRINTF)
#define snprintf	_snprintf
#endif
#if defined(HAVE__STRDUP)
#if !defined(HAVE_STRDUP) || defined(_WIN32)
#undef strdup
#define strdup		_strdup
#endif
#endif
#if !defined(HAVE__SETMODE) && defined(HAVE_SETMODE)
#define _setmode	setmode
#endif
#endif

#ifndef HAVE_FSEEKO
# define fseeko(s, o, w)	(fseek((s), (long int)(o), (w)))
#endif

#ifndef HAVE_FTELLO
# define ftello(s)	((long)ftell((s)))
#endif

#ifndef HAVE_MKSTEMP
int _zip_mkstemp( char * );
#define mkstemp _zip_mkstemp
#endif

#if !defined(HAVE_STRCASECMP)
#if defined(HAVE__STRICMP)
#define strcasecmp	_stricmp
#elif defined(HAVE_STRICMP)
#define strcasecmp	stricmp
#endif
#endif

#define SIZEOF_OFF_T 4

#if SIZEOF_OFF_T == 8
#define ZIP_OFF_MAX ZIP_INT64_MAX
#define ZIP_OFF_MIN ZIP_INT64_MIN
#elif SIZEOF_OFF_T == 4
#define ZIP_OFF_MAX ZIP_INT32_MAX
#define ZIP_OFF_MIN ZIP_INT32_MIN
#elif SIZEOF_OFF_T == 2
#define ZIP_OFF_MAX ZIP_INT16_MAX
#define ZIP_OFF_MIN ZIP_INT16_MIN
#else
#error unsupported size of off_t
#endif

#if defined(HAVE_FTELLO) && defined(HAVE_FSEEKO)
#define ZIP_FSEEK_MAX ZIP_OFF_MAX
#define ZIP_FSEEK_MIN ZIP_OFF_MIN
#else
#include <limits.h>
#define ZIP_FSEEK_MAX LONG_MAX
#define ZIP_FSEEK_MIN LONG_MIN
#endif

#ifndef SIZE_MAX
#if SIZEOF_SIZE_T == 8
#define SIZE_MAX ZIP_INT64_MAX
#elif SIZEOF_SIZE_T == 4
#define SIZE_MAX ZIP_INT32_MAX
#elif SIZEOF_SIZE_T == 2
#define SIZE_MAX ZIP_INT16_MAX
#else
#error unsupported size of size_t
#endif
#endif
