#pragma once
//============================================================================
// Created by Brett R. Jones in 2017 and issued to public domain
//============================================================================

#include "GoTvCpuArchDefines.h"

#include <inttypes.h> // all compilers must support this header to avoid massive typedef clashes
#include <sys/types.h> // for size_t in linux

#ifdef TARGET_OS_WINDOWS
# define PRIdS       "Id"
# define PRIuS       "Iu"
# ifndef _MSC_VER
// if visual studio then already defined by windows
#  define PRId64     "I64d"
# endif // _MSC_VER
#else
// not windows
# define PRIdS      "zd"
# define PRIuS      "u"
# ifndef PRId64
#  define PRId64    "lld"
# endif // PRId64
#endif // TARGET_OS_WINDOWS

#define GOTV_UINT64_FORMAT	"%llu"	// specifier for printing unsigned long long type ( works in Windows and Linux and Android )
#define GOTV_INT64_FORMAT	"%lld"	// specifier for printing long long type ( works in Windows and Linux and Android )

//============================================================================
// Compiler directives
//============================================================================
#if defined(_MSC_VER)
# define GOTV_EXPORT			// GoTv uses Static libs so no more import/export crap
# define GOTV_IMPORT extern
# define GOTV_HIDDEN
# define GOTV_DLL_VARIABLE		// GoTv uses Static libs so no more import/export crap
#elif defined(__GNUC__) && (__GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 3))
# define  GOTV_EXPORT
# define  GOTV_IMPORT extern
# define  GOTV_HIDDEN //__attribute__ ((visibility ("hidden")))
#else
# define  GOTV_EXPORT
# define  GOTV_IMPORT extern
# define  GOTV_HIDDEN
#endif

#if __GNUC__ >= 4
# define GOTV_API    __attribute__ ((visibility ("default")))
# define GOTV_LOCAL  __attribute__ ((visibility ("hidden")))
#else
# define GOTV_API
# define GOTV_LOCAL
#endif

#define GOTV_EXTERN		extern
//#define GOTV_CALLCONV __stdcall
// seems best not to specify any convention with static linked libs
#define GOTV_CALLCONV

#if defined __cplusplus
# define GOTV_EXTERN_C extern "C"
# define GOTV_BEGIN_CDECLARES extern "C" {
# define GOTV_END_CDECLARES }
#else
# define GOTV_EXTERN_C extern
# define GOTV_BEGIN_CDECLARES
# define GOTV_END_CDECLARES
#endif // __cplusplus

#if defined __cplusplus
GOTV_BEGIN_CDECLARES
#endif // defined __cplusplus
    int VxSleep( int milliSec ); // microsoft sleep returns void so had to make a function to return no error
#if defined __cplusplus
GOTV_END_CDECLARES
#endif // defined __cplusplus


# if !defined(GOTV_GNUC_PREREQ)
#  if defined(__GNUC__)&&defined(__GNUC_MINOR__)
#   define GOTV_GNUC_PREREQ(_maj,_min) \
 ((__GNUC__<<16)+__GNUC_MINOR__>=((_maj)<<16)+(_min))
#  else
#   define GOTV_GNUC_PREREQ(_maj,_min) 0
#  endif
# endif

// Routine name. Uses ISO __func__ where possible. Otherwise, uses the best thing that is available (if anything).
#if ( defined( __MWERKS__ ) || ( __GNUC__ > 2 ) || ( ( __GNUC__ == 2 ) && ( __GNUC_MINOR__ >= 9 ) ) )
# define __ROUTINE__                 __func__
#elif ( defined( __GNUC__ ) )
# define __ROUTINE__                 __PRETTY_FUNCTION__
#elif ( defined( _MSC_VER ) && !defined( _WIN32_WCE ) )
# define __ROUTINE__                 __FUNCTION__
#else
# define __ROUTINE__                 ""
#endif

#define GOTV_ROUTINE		__ROUTINE__

/* _GL_ARG_NONNULL((n,...,m)) tells the compiler and static analyzer tools
that the values passed as arguments n, ..., m must be non-NULL pointers.
n = 1 stands for the first argument, n = 2 for the second argument etc.  */
#ifndef GOTV_ARG_NONNULL
//# if (__GNUC__ == 3 && __GNUC_MINOR__ >= 3) || __GNUC__ > 3
//#  define GOTV_ARG_NONNULL(params) __attribute__ ((__nonnull__ params))
//# else
#  define GOTV_ARG_NONNULL(params)
//# endif
#endif

/* deprecated attribute */
#if (__GNUC__ > 4 || (__GNUC__ == 4 && (__GNUC_MINOR__ >= 5)))
# ifndef GOTV_WARN_DEPRECATED
#  define GOTV_WARN_DEPRECATED(x) __attribute__((deprecated(x)))
# endif
#elif defined(_MSC_VER)
# ifndef GOTV_WARN_DEPRECATED
#  define GOTV_WARN_DEPRECATED(x) __declspec(deprecated(x))
# endif
#elif defined(__clang__)
# ifndef GOTV_WARN_DEPRECATED
#  define GOTV_WARN_DEPRECATED(x) __attribute__((deprecated(x)))
# endif
#else
# define GOTV_WARN_DEPRECATED(x)
# pragma message("WARNING: You need to implement DEPRECATED for this compiler")
#endif

#ifdef _MSC_VER
# define GOTV_INLINE			_inline
# define GOTV_FORCE_INLINE		__forceinline
#elif defined(__GNUC__)
# define GOTV_INLINE			inline
# define GOTV_FORCE_INLINE		inline // __attribute((always_inline))
#elif defined(__MWERKS__)
# define GOTV_FORCE_INLINE		inline
# define GOTV_FORCE_INLINE		inline
#else
/* add other compilers here ... */
# define GOTV_INLINE
# define GOTV_FORCE_INLINE
#endif

#if defined(__GNUC__)
# if __GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ >= 95)
#  define GOTV_ATTRIBUTE_PACKED __attribute__ ((packed))
#  define GOTV_PRAGMA_PACK 0
# endif // if __GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ >= 95)
#endif // defined(__GNUC__)

#if !defined(GOTV_ATTRIBUTE_PACKED)
# define GOTV_ATTRIBUTE_PACKED
# define GOTV_PRAGMA_PACK 1
#endif // !defined(GOTV_ATTRIBUTE_PACKED)

#if (!defined(__STDC_VERSION__) || (__STDC_VERSION__ < 199901L) )
# if GOTV_GNUC_PREREQ(3,0)
#  define GOTV_RESTRICT __restrict__
# elif (defined(_MSC_VER) && _MSC_VER >= 1400)
#  define GOTV_RESTRICT __restrict
# else
#  define GOTV_RESTRICT
# endif
#else
# define GOTV_RESTRICT restrict
#endif

/* The __pure__ attribute was added in gcc 2.96.  */
#if __GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ >= 96)
# define GOTV_ATTRIBUTE_PURE __attribute__ ((__pure__))
#else
# define GOTV_ATTRIBUTE_PURE
#endif

/* The __const__ attribute was added in gcc 2.95.  */
#if __GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ >= 95)
# define GOTV_ATTRIBUTE_CONST __attribute__ ((__const__))
#else
# define GOTV_ATTRIBUTE_CONST
#endif

/**Warning attributes for functions
* NONNULL is not used to avoid the compiler optimizing out
* some paranoid null checks. */
#if defined(__GNUC__) && GOTV_GNUC_PREREQ(3, 4)
# define GOTV_WARN_UNUSED_RESULT __attribute__ ((__warn_unused_result__))
#else
# define GOTV_WARN_UNUSED_RESULT
#endif

// Borrowed from Chromium's base/compiler_specific.h.
// Annotate a virtual method indicating it must be overriding a virtual
// method in the parent class.
// Use like:
//   virtual void foo() OVERRIDE;
#if defined(_MSC_VER)
# define GOTV_OVERRIDE override
#elif defined(__clang__)
// Clang defaults to C++03 and warns about using override. Squelch that.
// Intentionally no push/pop here so all users of OVERRIDE ignore the warning
// too. This is like passing -Wno-c++11-extensions, except that GCC won't die
// (because it won't see this pragma).
# pragma clang diagnostic ignored "-Wc++11-extensions"
# define GOTV_OVERRIDE override
#elif defined(__GNUC__) && __cplusplus >= 201103 && \
    (__GNUC__ * 10000 + __GNUC_MINOR__ * 100) >= 40700
// GCC 4.7 supports explicit virtual overrides when C++11 support is enabled.
# define GOTV_OVERRIDE override
#else
# define GOTV_OVERRIDE
#endif

# ifdef _MSC_VER
#  define GOTV_ALLOC(var, size, type) var = ((type*)_alloca(sizeof(type)*(size)))
# else
#  define GOTV_ALLOC(var, size, type) var = ((type*)alloca(sizeof(type)*(size)))
# endif

//#define GOTV_DECLARE_UNUSED(a,b) a __attribute__((unused)) b;
#define GOTV_DECLARE_UNUSED(x) ((void)(x))
#define GOTV_UNUSED				GOTV_DECLARE_UNUSED // alias for ease of use

// Annotate a function that will not return control flow to the caller.
#if defined(_MSC_VER)
# define GOTV_NO_RETURN __declspec(noreturn)
#elif defined(__GNUC__)
# define GOTV_NO_RETURN __attribute__((noreturn))
#else
# define GOTV_NO_RETURN
#endif

# ifndef __STDC_CONSTANT_MACROS
#  define __STDC_CONSTANT_MACROS
# endif // __STDC_CONSTANT_MACROS



//============================================================================
// === common to all OSs and compilers ====
//============================================================================

#define SYSTEM_URL_SIZE (sizeof(SYSTEM_URL)-1)

#ifdef TARGET_OS_WINDOWS
# define SYSTEM_URL     "win:"
#ifdef _MSC_VER
# define MAX_PATH        260 // not needed if compiled with migwin
#endif // _MSC_VER
# define MAXPATHLEN		MAX_PATH
# define LINE_ENDING	"\r\n"
# define EXEEXT			".exe"
#else
# define SYSTEM_URL     "system:"
# define MAXPATHLEN		PATH_MAX // path max is defined in limits.h
# define LINE_ENDING	"\n"
# define EXEEXT			""
#endif // TARGET_OS_WINDOWS

#define U64_C(c) (c ## ULL)

# ifdef _MSC_VER
//============================================================================
// === Visual Studio/Windows Specific defines and replacement functions ===
//============================================================================
// NOTE: these disables really slow down then visual studio compiler
# pragma warning( disable : 4100 ) // ignore warning "unreferenced formal parameter" ( too annoying to not turn off )

//# pragma warning( disable : 4018 ) // '<' : signed/unsigned mismatch
//# pragma warning( disable : 4090 ) //  different 'const' qualifiers
//# pragma warning( disable : 4101)  // unreferenced local variable
//# pragma warning( disable : 4133 ) // : incompatible types - from 'const int *' to 'const AVPixelFormat *'
//# pragma warning( disable : 4146 ) // unary minus operator applied to unsigned type, result still unsigned
//# pragma warning( disable : 4167 ) // 'function': conversion from 'size_t' to 'uint16_t', possible loss of data
//# pragma warning( disable : 4221 ) // This object file does not define any previously undefined public symbols, so it will not be used by any link operation that consumes this library
//# pragma warning( disable : 4244 ) // conversion from 'double' to 'float', possible loss of data
//# pragma warning( disable : 4267 ) // 'function': conversion from 'size_t' to 'uint16_t', possible loss of data
//# pragma warning( disable : 4305 ) // initializing': truncation from '__int64' to 'double'
//# pragma warning( disable : 4355 ) // 'this' : used in base member initializer list
//# pragma warning( disable : 4503 ) //  decorated name length exceeded, name was truncated ( safe to ignore )
//# pragma warning( disable : 4800 ) // 'int' : forcing value to bool 'true' or 'false' ( performance warning )
//# pragma warning( error : 4005 ) // make macro redefinition from warning to error


# ifndef TARGET_OS_WINDOWS
#  define TARGET_OS_WINDOWS
# endif // TARGET_OS_WINDOWS

//#ifdef _DEBUG
////# ifndef _SECURE_SCL
//#  define _SECURE_SCL 1
////# endif // _SECURE_SCL
//
////# ifndef _HAS_ITERATOR_DEBUGGING
//#  define _HAS_ITERATOR_DEBUGGING 1
////# endif // _HAS_ITERATOR_DEBUGGING
//
//#  define _ITERATOR_DEBUG_LEVEL 2
//#endif // _DEBUG

# ifndef _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES
#  define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES 1
# endif // _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES

# ifndef _CRT_SECURE_NO_WARNINGS
#  define _CRT_SECURE_NO_WARNINGS
# endif // _CRT_SECURE_NO_WARNINGS

# ifndef _CRT_NONSTDC_NO_DEPRECATE
#  define _CRT_NONSTDC_NO_DEPRECATE
# endif // _CRT_NONSTDC_NO_DEPRECATE

# ifndef _WINSOCK_DEPRECATED_NO_WARNINGS
#  define _WINSOCK_DEPRECATED_NO_WARNINGS
# endif // _WINSOCK_DEPRECATED_NO_WARNINGS

# ifndef _SCL_SECURE_NO_WARNINGS
#  define _SCL_SECURE_NO_WARNINGS
# endif // _SCL_SECURE_NO_WARNINGS

#ifndef NOMINMAX
# define NOMINMAX
#endif // NOMINMAX

// Bo not define
//#ifndef _CRT_NO_TIME_T
//#define  _CRT_NO_TIME_T
//#endif // _CRT_NO_TIME_T

#ifndef _WINDOWS
# define _WINDOWS
#endif // _WINDOWS


#ifndef NTDDI_VERSION
# define NTDDI_VERSION 0x0A000002 //NTDDI_WIN10_RS1
#endif // NTDDI_VERSION
#ifndef _WIN32_WINNT
# define _WIN32_WINNT 0x0A00
#endif // _WIN32_WINNT
#ifndef WINVER
# define WINVER 0x0A00
#endif // WINVER

#ifndef __STDC_FORMAT_MACROS
# define __STDC_FORMAT_MACROS
#endif // __STDC_FORMAT_MACROS

#ifndef _CRT_RAND_S
# define _CRT_RAND_S  // required or rand_s is not defined in stdlib.h
#endif // _CRT_RAND_S

#if !defined(DEBUG) && !defined(NDEBUG) && !defined(_DEBUG)
# define NDEBUG
#endif // !defined(DEBUG) && !defined(NDEBUG) && !defined(_DEBUG)

// there seems to be a lot of issues with defines of microsoft compiler so just make sure we define first
//#define WIN32_LEAN_AND_MEAN // to many compile errors if use lean and mean :(
#include <WinSock2.h> // has to be first all the time or network stuff gets redefined
#include <windows.h>
#include <time.h>

// bunch of crap to keep windows from freaking out over <sys/utime>
#define  _CRT_DECLARE_NONSTDC_NAMES 0
#define  __STDC__ 1
// end crap to keep windows from freaking out over <sys/utime>

#include <sys/utime.h>
#include <inttypes.h>
#include <stdbool.h> // otherwise declarations may be different if included elsewhere
#include <CoreLib/dirent_sim.h>


#define alloca				_alloca
#define access				_access
#define atoi64				_atoi64
#define chdir				_chdir
#define chmod_os			_chmod
//#define close				_close // name conflict
#define close_os			_close
#define dup					_dup
#define dup2				_dup2
#define execv				_execv	// requires include <process.h>
#define execve				_execve
#define ftello64			_ftelli64
#define fseeko64			_fseeki64
#define fsync				_commit
#define ftime				_ftime // requires include  <sys/types.h> and <sys/timeb.h>
#define ftruncate			_chsize_s
#define fcntl				_fctrl
#define fdopen				_fdopen
#define fileno				_fileno
#define getpid				_getpid		// requires include <process.h>
#define getgid				_getpid		// windows does not have user id's so fake it for now
#define getegid				_getpid		// windows does not have user id's so fake it for now
#define getuid				_getpid		// windows does not have user id's so fake it for now
#define geteuid				_getpid		// windows does not have user id's so fake it for now
#define getcwd				_getcwd
#define isatty				_isatty
#define ioctl				ioctlsocket
//#define lrint				_lrint		// nearest integer function lrint requires math.h in VS2017
//#define lrintf				_lrintf
#define lseek				_lseek
#define tell_os(a)			_lseek(a, 0, SEEK_CUR)
#define mbsstr				_mbsstr // requires #include <mbstring.h>
#define mbslen				_mbslen
// name conflict #define mkdir(x,y)			(_mkdir)((x))  // requires  #include <direct.h>
// use mkdir_os instead
#define on_exit(exp1,exp2)	atexit( exp1) // OnExit 
#define mkdir_os(x,y)		(_mkdir)((x))  // requires  #include <direct.h>
#define open_os				_open
#define read_os				_read
#define write_os			_write
#define open64				_open
#define popen				_popen
#define pclose				_pclose
#define random				rand		
// name conflict #define read				_read
#define rmdir				_rmdir
#define rmdir_os			_rmdir
#define secure_getenv		getenv		// windows has getenv in stdlib.h
#define setmode				_setmode	// requires #include <io.h> 
#define srandom				srand
//#define stpncpy				strncpy_s
//#define stpcpy				strcpy_s
//#define strdup				_strdup
#define strcasecmp			_stricmp
#define strncasecmp			_strnicmp 
//#define strtok_r			_mbstok
#define strtok_r			strtok_s
#define strtoll				_strtoi64
#define strtoull			_strtoui64
#if ARCH_64_BITS
# define timeb				__timeb64
#else // ARCH_32_BITS
# define timeb				__timeb32
#endif // ARCH_64_BITS

#define unlink				_unlink
//#define umask				_unmask	// requires #include <io.h>, <sys/stat.h>, <sys/types.h> 
#define utime				_utime
//#define wcscpy		   _mbscpy_s	// do not redefine.. just include  <wchar.h> or <string.h>
//#define write				_write		// cannot redefine because other code uses write
#define lstat				stat		// does not do links but good enough for most things
#define off_t				_off_t
#define posix_memalign(p, a, s) (((*(p)) = _aligned_malloc((s), (a))), *(p) ?0 :errno)

/* Values for the second argument to access. These may be OR'd together.  */
#define R_OK				4       /* Test for read permission.  */
#define W_OK				2       /* Test for write permission.  */
//#define   X_OK			1       /* execute permission - unsupported in windows*/
#define F_OK				0       /* Test for existence.  */

#define STDIN_FILENO		0
#define STDOUT_FILENO		1
#define STDERR_FILENO		2

#define O_RDONLY		_O_RDONLY		// these require io.h
#define O_WRONLY		_O_WRONLY
#define O_RDWR			_O_RDWR
#define O_APPEND		_O_APPEND
#define O_CREAT			_O_CREAT
#define O_TRUNC			_O_TRUNC
#define O_EXCL			_O_EXCL
#define O_TEXT			_O_TEXT
#define O_BINARY		_O_BINARY
#define O_RAW			_O_BINARY
#define O_TEMPORARY		_O_TEMPORARY
#define O_NOINHERIT		_O_NOINHERIT
#define O_SEQUENTIAL	_O_SEQUENTIAL
#define O_RANDOM		_O_RANDOM
#define O_LARGEFILE		0

#define _SH_DENYRW      0x10    // deny read/write mode
#define _SH_DENYWR      0x20    // deny write mode
#define _SH_DENYRD      0x30    // deny read mode
#define _SH_DENYNO      0x40    // deny none mode
#define _SH_SECURE      0x80    // secure mode

#define S_IRUSR 0000400
#define S_IWUSR 0000200
#define S_IXUSR 0000100
#define	S_IRWXG	0000070			/* RWX mask for group */
#define S_IRGRP 0000040
#define S_IWGRP 0000020
#define S_IXGRP 0000010
#define	S_IRWXO	0000007			/* RWX mask for other */
#define S_IROTH 0000004
#define S_IWOTH 0000002
#define S_IXOTH 0000001

#define SH_COMPAT		0x0000
#define SH_DENYRW		_SH_DENYRW // defined in wchar.h
#define SH_DENYWR		_SH_DENYWR
#define SH_DENYRD		_SH_DENYRD
#define SH_DENYNO		_SH_DENYNO


//#define volatile		// use the /volatile:iso switch in compiler additional command line options

//#ifndef _CRT_INTERNAL_NONSTDC_NAMES
# ifdef ARCH_64_BITS
#  define stat			_stati64
#  define fstat			_fstati64
# else
#  define stat			_stat32	
#  define fstat			_fstat32
# endif // ARCH_64_BITS
//#endif // _CRT_INTERNAL_NONSTDC_NAMES

#include <BaseTsd.h>
typedef __int64				off64_t;
typedef __int64				fpos64_t;
typedef __int64				__off64_t;
typedef long				__off_t;
typedef int					socklen_t;
typedef int					locale_t;
typedef unsigned short		_ino_t;
typedef	_ino_t				ino_t;
typedef unsigned int		_dev_t;
typedef _dev_t				dev_t;

# if !GNULIB_defined_sigset_t
typedef unsigned int sigset_t;
#  define GNULIB_defined_sigset_t 1
# endif

#if !defined(_SSIZE_T_DEFINED) && !defined(HAVE_SSIZE_T)
#define HAVE_SSIZE_T 1
#define _SSIZE_T

#ifdef _MSC_VER
    // sigh for python this needs to be __int64 or int if not 64 bit windows
# if ARCH_64_BITS
   typedef SSIZE_T ssize_t;
# else 
   typedef int			ssize_t;
# endif // ARCH_64_BITS
#else
  typedef int			ssize_t;
#endif // _MSC_VER

# define _SSIZE_T_DEFINED
# ifndef SSIZE_MAX
#  define SSIZE_MAX INTPTR_MAX
# endif // !SSIZE_MAX
#endif // !defined(_SSIZE_T_DEFINED) && !defined(HAVE_SSIZE_T)


# if _MSC_VER >= 1400 && !defined(_WIN32_WCE)
#  define mkstemp(a)			_mktemp_s(a,strlen(a))
# endif // _MSC_VER >= 1400 && !defined(_WIN32_WCE)

# if !defined(_IOWR) && defined(_MSC_VER)
#  define _IOWR(exp1,exp2,exp3)     (IOC_INOUT | (( (long)sizeof(exp3)&IOCPARM_MASK) <<16 ) | ((exp1)<<8) | (exp2))
# endif // if !defined(_IORW) && defined(_MSC_VER)



GOTV_BEGIN_CDECLARES

// functions missing from windows .. implemented in CoreLib/VxFuntionsMissingInWindows.cpp
// simulate gettimeofday
struct timezone
{
	int  tz_minuteswest; /* minutes W of Greenwich */
	int  tz_dsttime;     /* type of dst correction */
};

int								clock_gettime( int X, struct timeval *tv );// implemented in VxFuntionsMissingInWindows
int								gettimeofday( struct timeval *tv/*in*/, struct timezone *tz/*in*/ );
int								asprintf( char **strp, const char *fmt, ... );
/* Return the position of the first bit set in the argument.  */
int								ffs( int );// implemented in VxFuntionsMissingInWindows
uint32_t						getpagesize( void );
// Find the first occurrence of the byte string s in byte string
const void *					memmem( const void *l, size_t l_len, const void *s, size_t s_len );
size_t							getline( char **lineptr, size_t *n, FILE *stream );
GOTV_INLINE uint32_t			setuid( uint32_t id ) { return id; } // windows does not have user id's so fake it for now
char *							strchrnul( const char *s, int c_in ); // implemented in libgnu/strchrnul.c
char *							stpcpy( char *a, const char *b );
char *							stpncpy( char *dest, const char *src, size_t n ); // implemented in libgnu/stpncpy
char *							strptime( const char *buf, const char *fmt, struct tm *tm ); // implemented in VxFuntionsMissingInWindows.cpp
int								strverscmp( const char *s1, const char *s2 );
char *							strcasestr( const char* haystack, const char* needle );	// implemented in VxFuntionsMissingInWindows.cpp
int								unsetenv( const char * ); // implemented in libgnu/unsetenv.c
int								vasprintf( char **strp, const char *fmt, va_list ap );

struct tm *						gmtime_r( const time_t *timep, struct tm *result );		// implemented in VxFuntionsMissingInWindows.cpp
time_t							timegm( struct tm *tm );								// implemented in VxFuntionsMissingInWindows.cpp
struct tm *						localtime_r( const time_t *timep, struct tm *result );	// implemented in VxFuntionsMissingInWindows.cpp
char *							strndup( const char *_s, size_t _n );
//char *							strdup( const char *_s, size_t _n );
// create temp unique directory in given path 
char *							mkdtemp( char * path );	// implemented in VxFuntionsMissingInWindows.cpp
int								setenv( const char *name, const char *value, int overwrite ); // implemented in VxFuntionsMissingInWindows.cpp
GOTV_END_CDECLARES

#define HAVE_SECURE_GETENV		1 // have secure_getenv
/* Define if you have the <winsock2.h> header file. */
#define HAVE_WINSOCK2_H			1
/* Define if you have the <windows.h> header file. */
#define HAVE_WINDOWS_H			1
/* Define if you have the <ws2tcpip.h> header file. */
#define HAVE_WS2TCPIP_H			1


# define VxMkDir(exp,exp2)	_mkdir(exp)     //make directory command we can use in windows like linux
# define sleep_os(exp)		VxSleep(exp*1000)
# define msleep_os(exp)		VxSleep(exp)
# define usleep_os(exp)		VxSleep(exp/1000)

# define VxGetLastError		GetLastError
# define VX_MAX_PATH		260

#ifndef MIN
# define MIN(x,y) (((x) < (y)) ? (x) : (y))
#endif // MIN

#ifndef MAX
# define MAX(x,y) (((x) > (y)) ? (x) : (y))
#endif // MAX

typedef struct _stat64 structStatOsDef;

#else 
//================= EVERY OS THAT IS NOT WINDOWS ===================//
#define HAVE_WINDOWS_H			0  // only windows has <windows.h>
#define HAVE_WS2TCPIP_H			0  // only windows has <ws2tcpip.h>

#ifdef TARGET_OS_LINUX
# define HAVE_FSEEKO            1
# define HAVE_FTELLO            1
#endif // TARGET_OS_LINUX

#include <stdbool.h> 
#include <sys/stat.h>
#include <stddef.h> // required for wchar_t in linux
#include <time.h> // for time_t

uint64_t GetTickCount64(); // implemented in VxFunctionsMissingInPosix.cpp

typedef struct stat structStatOsDef; // linux without underscore ( a windows thing )

typedef int					INT;
typedef unsigned int		UINT;

#ifndef NULL
# define NULL 0
#endif // NULL

#if defined(TARGET_OS_LINUX) || defined(TARGET_OS_ANDROID)
# include <sys/types.h>
# include <netinet/in.h>
# include <inttypes.h>

# define ntohll be64toh
# define htonll htobe64
//#elif defined(TARGET_OS_ANDROID)

#endif // TARGET_OS_LINUX

#ifndef SOCKET
typedef int SOCKET;
#endif // SOCKET
#ifndef SOCKADDR_IN
# define SOCKADDR_IN             struct sockaddr_in
#endif // SOCKADDR_IN

#define atoi64                  atoll // ascii to long long

#if defined(TARGET_OS_WINDOWS)
# ifndef __cdecl
#  define __cdecl
# endif

# ifndef __declspec
#  define __declspec(X)
# endif
#endif // defined(TARGET_OS_WINDOWS)

typedef void*				HINSTANCE;
typedef void*				HMODULE;
//typedef void*				HANDLE;
//typedef HANDLE*             LPHANDLE;

typedef unsigned int		DWORD;
typedef unsigned short		WORD;
typedef unsigned char		BYTE;
typedef char				CHAR;
typedef wchar_t				WCHAR;
#ifndef DO_NOT_DEFINE_BOOL // some projects such as glew typedef BOOL
 typedef int					BOOL;
#endif// DO_NOT_DEFINE_BOOL
#ifndef BOOL
//# define BOOL int // have to use define so that linux can typedef it
#endif // BOOL
typedef int					INT;
typedef unsigned int		UINT;
// typedef int INT32;              // unused; must match Xmd.h but why bother
//typedef long long			INT64;
//#define INT64 int64_t // have to use define so that linux can typedef it
typedef unsigned long long    UINT64;
typedef long				LONG;
typedef long long			LONGLONG;
#if defined(TARGET_OS_APPLE)
typedef UInt32				ULONG;
#else
typedef unsigned long		ULONG;
#endif
typedef size_t				SIZE_T;
typedef void*				PVOID;
typedef void*				LPVOID;
//typedef PVOID				HANDLE;
//#define INVALID_HANDLE_VALUE     ((HANDLE)~0U)
//typedef HANDLE				HDC;
typedef void*				HWND;
typedef BYTE*				LPBYTE;
typedef DWORD*				LPDWORD;
typedef const CHAR*			LPCSTR;
typedef const WCHAR*		LPCWSTR;
typedef CHAR*				LPTSTR;
typedef WCHAR				*PWSTR, *LPWSTR, *NWPSTR;
typedef CHAR				*PSTR, *LPSTR, *NPSTR;
typedef LONG				*PLONG, *LPLONG;
#ifdef UNICODE
typedef LPCWSTR				LPCTSTR;
#else
typedef LPCSTR				LPCTSTR;
#endif
typedef uint64_t            ULONGLONG;
typedef unsigned long		ULONG_PTR;
typedef ULONG_PTR			DWORD_PTR;

//typedef intptr_t( *FARPROC )( void );

#define MAXWORD   0xffff

typedef DWORD               LCID;
typedef WORD*               LPWORD;
#define LPBOOL BOOL*
typedef CHAR*               LPCHAR;
typedef CHAR*               PCHAR;
typedef const void*         LPCVOID;
#ifndef TARGET_OS_WINDOWS
typedef unsigned short      LANGID;
#endif // TARGET_OS_WINDOWS

typedef union _LARGE_INTEGER
{
	struct {
		DWORD LowPart;
		int32_t HighPart;
	} u;
	LONGLONG QuadPart;
} LARGE_INTEGER, *PLARGE_INTEGER;

typedef union _ULARGE_INTEGER {
	struct {
		DWORD LowPart;
		DWORD HighPart;
	} u;
	ULONGLONG QuadPart;
} ULARGE_INTEGER, *PULARGE_INTEGER;

#define MAKELONG(low,high)     ((LONG)(((WORD)((DWORD_PTR)(low) & 0xFFFF)) | ((DWORD)((WORD)((DWORD_PTR)(high) & 0xFFFF))) << 16))
#define MAKEFOURCC(a,b,c,d)  \
    ((DWORD)(BYTE)(a) | ((DWORD)(BYTE)(b) << 8) |  \
    ((DWORD)(BYTE)(c) << 16) | ((DWORD)(BYTE)(d) << 24 ))

#define SUCCEEDED(hr)   ((hr) >= 0)
#define FAILED(hr)      ((hr) < 0)
#define Int32x32To64(a,b) ((LONGLONG)(a)*(LONGLONG)(b))
#define UInt32x32To64(a,b) ((DWORDLONG)(a)*(DWORDLONG)(b))
#define Int64ShllMod32(a,b) ((DWORDLONG)(a)<<(b))
#define Int64ShraMod32(a,b) ((LONGLONG)(a)>>(b))
#define Int64ShrlMod32(a,b) ((DWORDLONG)(a)>>(b))

// Date / Time

typedef struct _SYSTEMTIME
{
	WORD wYear;
	WORD wMonth;
	WORD wDayOfWeek;
	WORD wDay;
	WORD wHour;
	WORD wMinute;
	WORD wSecond;
	WORD wMilliseconds;
} SYSTEMTIME, *PSYSTEMTIME, *LPSYSTEMTIME;

typedef struct _TIME_ZONE_INFORMATION {
	LONG Bias;
	WCHAR StandardName[32];
	SYSTEMTIME StandardDate;
	LONG StandardBias;
	WCHAR DaylightName[32];
	SYSTEMTIME DaylightDate;
	LONG DaylightBias;
} TIME_ZONE_INFORMATION, *PTIME_ZONE_INFORMATION, *LPTIME_ZONE_INFORMATION;

#define TIME_ZONE_ID_INVALID    ((DWORD)0xFFFFFFFF)
#define TIME_ZONE_ID_UNKNOWN    0
#define TIME_ZONE_ID_STANDARD   1
#define TIME_ZONE_ID_DAYLIGHT   2

// Thread
#define THREAD_BASE_PRIORITY_LOWRT  15
#define THREAD_BASE_PRIORITY_MAX    2
#define THREAD_BASE_PRIORITY_MIN   -2
#define THREAD_BASE_PRIORITY_IDLE  -15
#define THREAD_PRIORITY_LOWEST          THREAD_BASE_PRIORITY_MIN
#define THREAD_PRIORITY_BELOW_NORMAL    (THREAD_PRIORITY_LOWEST+1)
#define THREAD_PRIORITY_NORMAL          0
#define THREAD_PRIORITY_HIGHEST         THREAD_BASE_PRIORITY_MAX
#define THREAD_PRIORITY_ABOVE_NORMAL    (THREAD_PRIORITY_HIGHEST-1)
#define THREAD_PRIORITY_ERROR_RETURN    (0x7fffffff)
#define THREAD_PRIORITY_TIME_CRITICAL   THREAD_BASE_PRIORITY_LOWRT
#define THREAD_PRIORITY_IDLE            THREAD_BASE_PRIORITY_IDLE

#ifdef TARGET_POSIX

#define CONST   const

#define _fdopen						fdopen
#define _vsnprintf					vsnprintf
#define _stricmp					strcasecmp
#ifdef TARGET_OS_ANDROID
# define stricmp						strcasecmp
#endif // TARGET_OS_ANDROID
#define strcmpi						strcasecmp
#define strnicmp					strncasecmp
#define _atoi64(x)					atoll(x)
#define CopyMemory(dst,src,size)	memmove(dst, src, size)
#define ZeroMemory(dst,size)		memset(dst, 0, size)

#define VOID    void
#if defined(TARGET_OS_WINDOWS)
#define __int8    char
#define __int16   int16_t
#define __int32   int32_t
#define __int64   int64_t
#define __uint64  uint64_t

#define __stdcall
#define __cdecl
#endif // defined(TARGET_OS_WINDOWS)
#define WINBASEAPI
#define NTAPI       __stdcall
#define CALLBACK    __stdcall
#define WINAPI      __stdcall
#define WINAPIV     __cdecl
#if !defined( APIENTRY )
# if defined(TARGET_OS_WINDOWS)
#  define APIENTRY    WINAPI
//# else
//#  define APIENTRY
# endif // defined(TARGET_OS_WINDOWS)
#endif // !defined(APIENTRY)
#define APIPRIVATE  __stdcall
#define IN
#define OUT
#define OPTIONAL
#define _declspec(X)
//#define __declspec(X)

// Network
#define SOCKADDR_IN         struct sockaddr_in
#define IN_ADDR             struct in_addr
typedef int					SOCKET;
#define closesocket(s)		close(s)
#if !defined( ioctlsocket )
# define ioctlsocket(s, f, v) ioctl(s, f, v)
#endif // !defined( ioctlsocket )
#define WSAGetLastError()	(errno)
#define WSASetLastError(e)	(errno = e)
#define WSAECONNRESET		ECONNRESET
#define WSAHOST_NOT_FOUND	ENOENT
#define WSAETIMEDOUT		ETIMEDOUT
#define WSAEADDRINUSE		EADDRINUSE
#define WSAECANCELLED		EINTR
#define WSAECONNREFUSED		ECONNREFUSED
#define WSAECONNABORTED		ECONNABORTED

// Thread
typedef int( *LPTHREAD_START_ROUTINE )( void * );

// File
#define O_BINARY		0
#define O_TEXT			0
# define _O_TRUNC		O_TRUNC
# define _O_RDONLY		O_RDONLY
# define _O_WRONLY		O_WRONLY

#if defined(TARGET_OS_APPLE) || defined(TARGET_FREEBSD)
# define stat64 stat
# define __stat64 stat
# define fstat64 fstat
typedef int64_t off64_t;
# if defined(TARGET_FREEBSD)
#  define statfs64 statfs
# endif
#else
# ifdef TARGET_OS_WINDOWS
#  define __stat64 stat64
# endif // TARGET_OS_WINDOWS
#endif

struct _stati64 {
	dev_t st_dev;
	ino_t st_ino;
	unsigned short st_mode;
	short          st_nlink;
	short          st_uid;
	short          st_gid;
	dev_t st_rdev;
    int64_t  st_size;
	time_t _st_atime;
	time_t _st_mtime;
	time_t _st_ctime;
};

typedef struct _FILETIME
{
	DWORD dwLowDateTime;
	DWORD dwHighDateTime;
} FILETIME, *PFILETIME, *LPFILETIME;

typedef struct _WIN32_FIND_DATA
{
	DWORD     dwFileAttributes;
	FILETIME  ftCreationTime;
	FILETIME  ftLastAccessTime;
	FILETIME  ftLastWriteTime;
	DWORD     nFileSizeHigh;
	DWORD     nFileSizeLow;
	DWORD     dwReserved0;
	DWORD     dwReserved1;
	CHAR      cFileName[260];
	CHAR      cAlternateFileName[14];
} WIN32_FIND_DATA, *PWIN32_FIND_DATA, *LPWIN32_FIND_DATA;

#define LPWIN32_FIND_DATAA LPWIN32_FIND_DATA

#define FILE_ATTRIBUTE_DIRECTORY           0x00000010

typedef struct _SECURITY_ATTRIBUTES {
	DWORD nLength;
	LPVOID lpSecurityDescriptor;
    int bInheritHandle; //BOOL bInheritHandle;
} SECURITY_ATTRIBUTES, *PSECURITY_ATTRIBUTES, *LPSECURITY_ATTRIBUTES;

#define FILE_BEGIN			0
#define FILE_CURRENT        1
#define FILE_END            2
# define _S_IFREG			S_IFREG
# define _S_IFDIR			S_IFDIR

#define _stat				stat

#define chmod_os			chmod
#define close_os			close

#if defined(TARGET_OS_ANDROID)
# include <unistd.h> // for open etc on android
#endif
# define open_os			open
# define read_os			read
# define write_os			write
# define tell_os			tell
# define lseek_os			lseek

#define mkdir_os			mkdir
#define rmdir_os			rmdir

#define sleep_os			sleep
#define msleep_os			msleep
#define usleep_os			usleep

// Memory
typedef struct _MEMORYSTATUSEX
{
	DWORD dwLength;
	DWORD dwMemoryLoad;

	uint64_t ullTotalPhys;
	uint64_t ullAvailPhys;
	uint64_t ullTotalPageFile;
	uint64_t ullAvailPageFile;
	uint64_t ullTotalVirtual;
	uint64_t ullAvailVirtual;
} MEMORYSTATUSEX, *LPMEMORYSTATUSEX;

// Basic D3D stuff
typedef struct _RECT {
	LONG left;
	LONG top;
	LONG right;
	LONG bottom;
} RECT, *PRECT;

/* already defined in guilib/DirectXGraphics.h
typedef enum _D3DFORMAT
{
  D3DFMT_A8R8G8B8     = 0x00000006,
  D3DFMT_DXT1         = 0x0000000C,
  D3DFMT_DXT2         = 0x0000000E,
  D3DFMT_DXT4         = 0x0000000F,
  D3DFMT_UNKNOWN      = 0xFFFFFFFF
} D3DFORMAT;*/

// Misc stuff found in the code, not really important
#define PAGE_READWRITE    0x04
#define MAXULONG_PTR    ((ULONG) 0xffffffff)

// CreateFile defines
#define FILE_FLAG_WRITE_THROUGH         0x80000000
#define FILE_FLAG_OVERLAPPED            0x40000000
#define FILE_FLAG_NO_BUFFERING          0x20000000
#define FILE_FLAG_RANDOM_ACCESS         0x10000000
#define FILE_FLAG_SEQUENTIAL_SCAN       0x08000000
#define FILE_FLAG_DELETE_ON_CLOSE       0x04000000
#define FILE_FLAG_BACKUP_SEMANTICS      0x02000000
#define FILE_FLAG_POSIX_SEMANTICS       0x01000000
#define FILE_FLAG_OPEN_REPARSE_POINT    0x00200000
#define FILE_FLAG_OPEN_NO_RECALL        0x00100000
#define FILE_FLAG_FIRST_PIPE_INSTANCE   0x00080000

#define CREATE_NEW          1
#define CREATE_ALWAYS       2
#define OPEN_EXISTING       3
#define OPEN_ALWAYS         4
#define TRUNCATE_EXISTING   5

#define FILE_ATTRIBUTE_NORMAL       0x00000080
#define FILE_ATTRIBUTE_READONLY     0x00000001
#define FILE_ATTRIBUTE_HIDDEN       0x00000002
#define FILE_ATTRIBUTE_SYSTEM       0x00000004
#define FILE_ATTRIBUTE_DIRECTORY    0x00000010

#define FILE_READ_DATA   ( 0x0001 )
#define FILE_WRITE_DATA  ( 0x0002 )
#define FILE_APPEND_DATA ( 0x0004 )

#define GENERIC_READ  FILE_READ_DATA
#define GENERIC_WRITE FILE_WRITE_DATA
#define FILE_SHARE_READ                  0x00000001
#define FILE_SHARE_WRITE                 0x00000002
#define FILE_SHARE_DELETE                0x00000004

// Audio stuff
typedef struct tWAVEFORMATEX
{
	WORD    wFormatTag;
	WORD    nChannels;
	DWORD   nSamplesPerSec;
	DWORD   nAvgBytesPerSec;
	WORD    nBlockAlign;
	WORD    wBitsPerSample;
	WORD    cbSize;
} __attribute__( ( __packed__ ) ) WAVEFORMATEX, *PWAVEFORMATEX, *LPWAVEFORMATEX;

#define WAVE_FORMAT_UNKNOWN           0x0000
#define WAVE_FORMAT_PCM               0x0001
#define WAVE_FORMAT_ADPCM             0x0002
#define WAVE_FORMAT_IEEE_FLOAT        0x0003
#define WAVE_FORMAT_EXTENSIBLE        0xFFFE

#endif // TARGET_POSIX

# define     VxGetLastError()	errno
# define	 VxMkDir(exp,exp2)	mkdir(exp,exp2)     //make directory command we can use in windows like linux
# define     VX_MAX_PATH			260
# define     INVALID_SOCKET		-1		// use windows style socket define
# define     SOCKET_ERROR		-1		// use windows style socket define

# endif // _MSC_VER

#if defined( _MSC_VER ) || defined( TARGET_OS_WINDOWS )
#if defined( _MSC_VER )
# include <io.h>
typedef int          pid_t;
typedef uint32_t          uid_t;
typedef uint32_t          gid_t;
typedef int				  mode_t;

typedef uint8_t			  __u8;
typedef uint16_t          __u16;
typedef uint32_t          __u32;
typedef uint64_t          __u64;
#endif // defined( _MSC_VER )

#else

typedef int64_t              time64_t;

# include <stddef.h>
# include <errno.h>
# include <arpa/inet.h> // for htonl etc

#endif // defined( _MSC_VER ) || defined( TARGET_OS_WINDOWS )


// Visual Studio does not allow rename of keywords so cannot do this but should
/* Define to empty if `const' does not conform to ANSI C. */
//#define  const

/* Define to 1 if you have the <libgnu/alloca_gnu.h> header file. */
#define HAVE_ALLOCA_H			1
/* Define to 1 if you have the <arpa/inet.h> header file. */
#ifdef _MSC_VER
# define HAVE_ARPA_INET_H		9
#else
# define HAVE_ARPA_INET_H		1
#endif // _MSC_VER

#if defined( TARGET_OS_LINUX )
# define HAVE_ASM_TYPES_H       0 // really 1 for python but causes other fails if defined 1 here
#else
# define HAVE_ASM_TYPES_H		0
#endif // defined( TARGET_OS_LINUX )

/* Define to 1 if you have the `asprintf' function. */
#define HAVE_ASPRINTF			1 // windows simulated in CoreLib

#ifdef _MSC_VER
# define HAVE_ATOMICS_GCC		0
# define HAVE_ATOMICS_SUNCC		0
# define HAVE_ATOMICS_WIN32		1
# define HAVE_ATOMIC_CAS_PTR		0
# define HAVE_ATOMIC_COMPARE_EXCHANGE 1
#else
# define HAVE_ATOMICS_GCC		1
# define HAVE_ATOMICS_SUNCC		0
# define HAVE_ATOMICS_WIN32		1
# define HAVE_ATOMIC_CAS_PTR		0
# define HAVE_ATOMIC_COMPARE_EXCHANGE 1
#endif //_MSC_VER


#define HAVE_BOOL_T				1

//#define	HAS_FCNTL_H	0		/* /usr/include/fcntl.h		*/
#define	HAS_ERRNO_DECL			1		/* errno.h declares errno	*/
//#define HAVE_DLFCN_H			1	// define if have <dlfcn.h>
/* define to 1 if Cocoa is available */
#define HAVE_COCOA				1

#define HAVE_CONFIG_H			1	// define so opus uses GoTvDependLibrariesConfig.h local file ( most everything requires one anyway )
#ifndef HAVE_CHAR16_T
# define HAVE_CHAR16_T			1
#endif // HAVE_CHAR16_T

#ifndef HAVE_CHAR32_T
# define HAVE_CHAR32_T			1
#endif // HAVE_CHAR32_T

/* Define to 1 if you have the `clock' function. */
#define HAVE_CLOCK				1
/* Define to 1 if you have the `copysign' function. */
#define HAVE_COPYSIGN			1
#define HAVE_DIRENT_H			1	// simulated through Platform Lib for windows
/* Define to 1 if you have the <dlfcn.h> header file. */
#ifndef _MSC_VER
# define HAVE_DLFCN_H			1
/* Define to 1 if you have the `fchmod' function. */
# define HAVE_FCHMOD			1
/* Define to 1 if you have the `fchown' function. */
#define HAVE_FCHOWN				1
#endif // _MSC_VER

#ifdef TARGET_OS_WINDOWS
/* Define to 1 if you have the `fcntl' function. */
# define HAVE_FCNTL				0
/* Define to 1 if you have the <fcntl.h> header file. */
# define HAVE_FCNTL_H			1
/* Define to 1 if you have the `ftruncate' function. */
/* Define to 1 if you have the `fstat' function. */
# define HAVE_FSTAT				1
# define HAVE_FTRUNCATE			1
/* Define to 1 if you have the <features.h> header file. */
# define HAVE_FEATURES_H		0
#else
/* Define to 1 if you have the `fcntl' function. */
# define HAVE_FCNTL             1
/* Define to 1 if you have the <fcntl.h> header file. */
# define HAVE_FCNTL_H			1
/* Define to 1 if you have the `ftruncate' function. */
/* Define to 1 if you have the `fstat' function. */
# define HAVE_FSTAT
# define HAVE_FTRUNCATE			1
/* Define to 1 if you have the <features.h> header file. */
# define HAVE_FEATURES_H		1
/* Define to 1 if you have the `fork' function. */
# define HAVE_FORK				1

# define HAVE_SYS_IOCTL_H       1
/* Define to 1 if you have the `poll' function. */
# define HAVE_POLL_H			1
# define HAVE_NET_IF_H          1
#endif // TARGET_OS_WINDOWS

/* Define to 1 if you have the `getcwd' function. */
#define HAVE_GETCWD				1
/* Define to 1 if you have the `getuid' function. */
/* Define to 1 if you have the `getpid' function. */
#define HAVE_GETPID				1

#ifndef TARGET_OS_WINDOWS
# define HAVE_GETUID			1
/* Define to 1 if you have the `getegid' function. */
#define HAVE_GETEGID			1
/* Define to 1 if you have the `geteuid' function. */
#define HAVE_GETEUID			1
/* Define to 1 if you have the `getgid' function. */
#define HAVE_GETGID             1
/* Define to 1 if you have the `mempcpy' function. */
#define HAVE_MEMPCPY            1
#endif // TARGET_OS_WINDOWS

#ifdef _MSC_VER
# define HAVE_GETTIMEOFDAY		1 // simulated
# define HAVE_INLINE_ASM		0
#else
# define HAVE_GETTIMEOFDAY		1
# define HAVE_INLINE_ASM		0
# define	HAS_CHMOD 			1		/* chmod syscall		*/
# define	HAS_CHOWN 			1		/* chown syscall		*/
# define	HAS_FCHMOD			1		/* fchmod syscall		*/
# define	HAS_FCHOWN 			1		/* fchown syscall		*/
# define	HAS_FSETMODE 		1		// _fsetmode -- set file mode	
# define	HAS_FSTAT 			1		/* fstat syscall		*/
#endif // _MSC_VER

/* Define if GOTV_INLINE asm memory barrier is supported */
#ifndef TARGET_OS_WINDOWS
# define HAVE_GCC_ASM_VOLATILE_MEMORY 1
#endif // TARGET_OS_WINDOWS

/* Define to 1 if you have the `getaddrinfo' function. */
#define HAVE_GETADDRINFO			1
/* Define to 1 if you have the `getenv' function. */
#define HAVE_GETENV					1
/* Define to 1 if you have the `gethostbyname' function. */
#define HAVE_GETHOSTBYNAME			1
/* Define to 1 if you have the getc_unlocked() function. */
//#define HAVE_GETC_UNLOCKED 1		// causes link error in python if defined
/* Define to 1 if you have the `gmtime_r' function. */
#define HAVE_GMTIME_R				1		// simulated in VxFunctionsMissingInWindows
/* Define if you have the iconv() function and it works. */
#define HAVE_ICONV					1
/* Define to 1 if you have the <libiconv/iconv.h> header file. */
#define HAVE_ICONV_H				1

#define HAVE_INLINE					1
/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H				1
#ifdef TARGET_OS_WINDOWS
# define HAVE_IO_H					1
#endif // TARGET_OS_WINDOWS

#ifdef TARGET_OS_WINDOWS
/* Define to 1 if you have the `lstat' function. */
# define HAVE_LSTAT                 0
#else
# define HAVE_LSTAT                 1
#endif // TARGET_OS_WINDOWS

/* Define to 1 if you have the `mprotect' function. */
/* Define to 1 if you have the `lrint' function. */
#define HAVE_LRINT					1	// simulated in vs with _lrint definition
/* Define to 1 if you have the `lrintf' function. */
#define HAVE_LRINTF					1	// simulated in vs with _lrintf definition
/* Define to 1 if you have the `localtime_r' function. */
#define HAVE_LOCALTIME_R			1	// supported in windows with macro to change localtime_r to localtime_s 
/* Define to 1 if the system has the type 'long long int'. */
#define HAVE_LONG_LONG_INT			1
/* Define to 1 if you have the `mbrtowc' function. */
#define HAVE_MBRTOWC				1
/* Define to 1 if <wchar_gnu.h> declares mbstate_t. */
#define HAVE_MBSTATE_T				1
#define HAVE_MKSTEMP				1
#define HAVE_MALLOC_H				1
/* Define if asm/types.h defines __s64 and __u64 */
#define HAVE_S64_U64				1
/* Define to 1 if you have the `memalign' function. */

#ifndef _MSC_VER
//# define HAVE_MEMALIGN				0 // some libs assumes by if defined instead of by value
# define HAVE_MEMALIGN				1
#endif // _MSC_VER

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H				1
/* Define to 1 if you have the `mktime' function. */
#define HAVE_MKTIME					1
#ifdef _MSC_VER
# define HAVE_MPROTECT				0
#else
# define HAVE_MPROTECT				1
#endif // _MSC_VER

/* Define to 1 if you have the `newlocale' function. */
# define HAVE_NEWLOCALE				1	// windows simulated in libgnulib
/* Define to 1 if you have the <malloc.h> header file. */
#define HAVE_MALLOC_H				1
/* Define to 1 if you have the `memalign' function. */
//#define HAVE_MEMALIGN				1
/* Define to 1 if you have the `memmem' function. */
#define HAVE_MEMMEM					1
/* Define to 1 if you have the `memmove' function. */
#define HAVE_MEMMOVE				1
/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H				1
/* Define to 1 if you have the `mmap' function. */
#ifdef _MSC_VER
# define HAVE_MAPVIEWOFFILE			1
# define HAVE_MMAP					0
#else 
# define HAVE_MAPVIEWOFFILE			0
# define HAVE_MMAP					1
#endif 

/* Define to 1 if you have the <mntent.h> header file. */
#define HAVE_MNTENT_H				1
/* Define to 1 if you have the <ndir.h> header file, and it defines `DIR'. */
/* #undef HAVE_NDIR_H */
/* Define to 1 if you have the <netdb.h> header file. */
#define HAVE_NETDB_H				1
/* We have AF_NETLINK sockets */
#define HAVE_NETLINK	

/* Define if you have POSIX threads libraries and header files. */
#define HAVE_PTHREAD				1
/* Define to 1 if you have the `putenv' function. */
#define HAVE_PUTENV					1
/* Define to 1 if you have the `round' function. */
#define HAVE_ROUND					1
/* Define to 1 if you have the `stat' function. */
#define HAVE_STAT					1
/* Define to 1 if you have the `strftime' function. */
#define HAVE_STRFTIME				1
/* Define to 1 if you have the <sys/statfs.h> header file. */
#define HAVE_SYS_STATFS_H			1

/* Define to 1 if you have the <sys/statvfs.h> header file. */
#if !defined(TARGET_OS_WINDOWS)
# define HAVE_SYS_STATVFS_H			1
#endif // TARGET_OS_WINDOWS

#define HAVE_SSIZE_T				1

#ifndef _MSC_VER
/* Define to 1 if `st_blksize' is a member of `struct stat'. */
# define HAVE_STRUCT_STAT_ST_BLKSIZE	1
/* Define to 1 if `st_blocks' is a member of `struct stat'. */
# define HAVE_STRUCT_STAT_ST_BLOCKS		1
#endif // _MSC_VER

#ifdef _MSC_VER
/* Define to 1 if you have the <sys/mman.h> header file. */
# define HAVE_SYS_MMAN_H			0
/* Define to 1 if you have the <sys/select.h> header file. */
# define HAVE_SYS_SELECT_H			0
#else
/* Define to 1 if you have the <sys/mman.h> header file. */
#define HAVE_SYS_MMAN_H				1
# define HAVE_SYS_SELECT_H			1
#endif // _MSC_VER

/* Define to 1 if you have the <sys/param.h> header file. */

#ifdef TARGET_OS_WINDOWS
/* Define to 1 if you have the <sys/socket.h> header file. */
# define HAVE_SYS_SOCKET_H			0
# define HAVE_SYS_PARAM_H			0
#else
/* Define to 1 if you have the <sys/socket.h> header file. */
# define HAVE_SYS_SOCKET_H			1
# define HAVE_SYS_PARAM_H			1
#endif // TARGET_OS_WINDOWS

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H				1
/* Define to 1 if you have the <sys/timeb.h> header file. */
/* #undef HAVE_SYS_TIMEB_H */
/* Define to 1 if you have the <sys/time.h> header file. */
#define HAVE_SYS_TIME_H				1
/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H			1
/* Define to 1 if you have the <sys/wait.h> header file. */
#define HAVE_SYS_WAIT_H				1

#ifdef _MSC_VER
/* Define to 1 if you have the `readlink' function. */
# define HAVE_READLINK				0
/* Define to 1 if you have the `readlinkat' function. */
# define HAVE_READLINKAT			0
/* Define to 1 if you have the `realpath' function. */
//# define HAVE_REALPATH				0
/* Define to 1 if you have the <search.h> header file. */
# define HAVE_SEARCH_H				0
/* Define to 1 if you have the <resolv.h> header file. */
# define HAVE_RESOLV_H				0
#else
# define HAVE_READLINK				1
/* Define to 1 if you have the `readlinkat' function. */
# define HAVE_READLINKAT			1
/* Define to 1 if you have the `realpath' function. */
# define HAVE_REALPATH				1
/* Define to 1 if you have the <search.h> header file. */
# define HAVE_SEARCH_H				1
/* Define to 1 if you have the <resolv.h> header file. */
# define HAVE_RESOLV_H				1
#endif // _MSC_VER
/* Define to 1 if 'long double' and 'double' have the same representation. */
#define HAVE_SAME_LONG_DOUBLE_AS_DOUBLE 1
/* Define to 1 if you have the <sched.h> header file. */
#define HAVE_SCHED_H				1
/* Define to 1 if you have the `sched_setparam' function. */
#define HAVE_SCHED_SETPARAM			1
/* Define to 1 if you have the `sched_setscheduler' function. */
#define HAVE_SCHED_SETSCHEDULER		1
/* Define to 1 if you have the `select' function. */
#define HAVE_SELECT					1
/* Define to 1 if you have the `setegid' function. */
#ifndef TARGET_OS_WINDOWS
# define HAVE_SETEGID				1
#endif // TARGET_OS_WINDOWS
/* Define to 1 if you have the `setenv' function. */
#define HAVE_SETENV					1
/* Define to 1 if you have the `seteuid' function. */
#ifndef TARGET_OS_WINDOWS
# define HAVE_SETEUID               1
# define HAVE_SETEGID               1
#endif // TARGET_OS_WINDOWS

/* Define to 1 if you have the `setenv' function. */
# define HAVE_SETENV				1 // windows simulated in this header
/* Define to 1 if you have the `setlocale' function. */
#define HAVE_SETLOCALE				1
/* Define to 1 if you have the `setrlimit' function. */
#define HAVE_SETRLIMIT				0
/* Define to 1 if you have the `shlload' function. */
/* #undef HAVE_SHLLOAD */

#ifndef TARGET_OS_WINDOWS
// windows does not nomally handle signals
  /* Define to 1 if you have the `sigaction' function. */
# define HAVE_SIGACTION 1
  /* Define to 1 if you have the `sigaltstack' function. */
# define HAVE_SIGALTSTACK 1
  /* Define to 1 if <signal_gnu.h> defines the siginfo_t type, and struct sigaction
 has the sa_sigaction member and the SA_SIGINFO flag. */
# define HAVE_SIGINFO 1
 /* Define to 1 if the system has the type `siginfo_t'. */
 /* #undef HAVE_SIGINFO_T */
 /* Define to 1 if you have the `siginterrupt' function. */
# define HAVE_SIGINTERRUPT 1
#endif // TARGET_OS_WINDOWS

/* Define to 1 if you have the <signal.h> header file. */
#define HAVE_SIGNAL_H 1
/* Define to 1 if `st_mtim.tv_nsec' is a member of `struct stat'. */
#ifdef _MSC_VER
# define HAVE_STRUCT_STAT_ST_MTIM_TV_NSEC 0
#else
# define HAVE_STRUCT_STAT_ST_MTIM_TV_NSEC 1
#endif // _MSC_VER
/* Define to 1 if `d_type' is a member of `struct dirent'. */
#define HAVE_STRUCT_DIRENT_D_TYPE	1 // windows simulated CoreLib/dirent_sim.h
/* Define to 1 if you have the `settimeofday' function. */
//#define HAVE_SETTIMEOFDAY 1
/* Define if the sincos() function is available */
#define HAVE_SINCOS					1
/* Define to 1 if you have the `strlcpy' function. */
/* #undef HAVE_STRLCPY */
/* Define to 1 if you have the `strndup' function. */
#define HAVE_STRNDUP				1
/* Define to 1 if you have the `strptime' function. */
#define HAVE_STRPTIME				1
/* Define to 1 if you have the SUSV3 version of the strerror_r() function. */
/* #undef HAVE_SUSV3_STRERROR_R */


/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H			1
/* Define to 1 if you have the `syslog' function. */
#define HAVE_SYSLOG					1
/* Define to 1 if you have the `timegm' function. */
//#define HAVE_TIMEGM					1
/* Define if struct tm has a tm_gmtoff field */
#ifndef TARGET_OS_WINDOWS
// in windows gm time offset is tm->tm_isdst instead of tm->tm_gmtoff
# define HAVE_TM_GMTOFF			1
#endif // TARGET_OS_WINDOWS

#define HAVE_S_ISLNK			1 // simulated for windows in CoreLib/dirent_sim.h	
#define HAVE_S_ISSOCK			1 // simulated for windows in CoreLib/dirent_sim.h	

#ifndef TARGET_OS_WINDOWS
/* Define to 1 if the system has the type `sigset_t'. */
# define HAVE_SIGSET_T			1
#endif // TARGET_OS_WINDOWS

/* Define to 1 if you have the `snprintf' function. */
#define HAVE_SNPRINTF			1
#define HAVE_STDBOOL_H			1
/* Define to 1 if you have the <stddef.h> header file. */
#define HAVE_STDDEF_H			1		
/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H			1
/* Define if <stdint.h> exists, doesn't clash with <sys/types.h>, and declares uintmax_t. */
#define HAVE_STDINT_H_WITH_UINTMAX 1
/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H			1
#define HAS_STDLIB_H			1

/* Define to 1 if you have the `stpcpy' function. */
#define HAVE_STPCPY				1
/* Define to 1 if you have the `strcasecmp' function. */
#define HAVE_STRCASECMP			1
/* Define to 1 if you have the `strdup' function. */
#define HAVE_STRDUP				1
/* Define to 1 if you have the `strerror' function. */
#define HAVE_STRERROR			1
/* Define to 1 if you have the `strerror_r' function. */
#ifndef _MSC_VER
# define HAVE_STRERROR_R        1
# define HAVE_POSIX_STRERROR_R  1
#endif // _MSC_VER

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H			1
#define	HAS_STRING_H			1 // some libs use this instead of HAVE_STRING_H
/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H			1

/* Define to 1 if you have the `stricmp' function. */
/* #undef HAVE_STRICMP */
/* Define to 1 if you have the `strnlen' function. */
#define HAVE_STRNLEN			1
/* Define to 1 if you have the `strtol' function. */
#define HAVE_STRTOL				1
/* Define to 1 if you have the `strtoul' function. */
#define HAVE_STRTOUL			1

#ifdef _MSC_VER
//# define HAVE_POSIX_MEMALIGN		0
#else
# define HAVE_POSIX_MEMALIGN		1
#endif // _MSC_VER

// common VxSleep( int milliseconds ) for all platforms is in CoreLib/VxDefs
#ifdef TARGET_OS_WINDOWS
# define HAVE_SLEEP				1
# define HAVE_NANOSLEEP			0
#elif defined(TARGET_OS_ANDROID)
# define HAVE_NANOSLEEP			1
#else
# define HAVE_USLEEP			1
# define HAVE_NANOSLEEP			0
#endif // TARGET_OS_WINDOWS

#ifdef _MSC_VER
# if  (_WIN32_WINNT >= 0x0600)
#  define HAVE_STRUCT_POLLFD		1
# else
#  define HAVE_STRUCT_POLLFD		0
# endif //(_WIN32_WINNT >= 0x0600)
#else
# define HAVE_STRUCT_POLLFD		0
#endif // _MSC_VER

/* Define if you have the <signal.h> header file.  */
#define HAVE_SIGNAL_H			1			
/* Define to 1 if you have the <stdint.h> header file. */
#ifndef HAVE_STDINT_H
# define HAVE_STDINT_H			1
#endif // HAVE_STDINT_H


/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H		1

/* Define to 1 if you have the <sys/un.h> header file. */
#ifdef _MSC_VER
# define HAVE_SYS_UN_H			0
/* Define to 1 if you have the <sys/msg.h> header file. */
# define HAVE_SYS_MSG_H			0
#else
# define HAVE_SYS_UN_H			1
/* Define to 1 if you have the <sys/msg.h> header file. */
# define HAVE_SYS_MSG_H			1
#endif // _MSC_VER

/* Define to 1 if you have the `sysconf' function. */
#ifndef _MSC_VER
# define HAVE_SYSCONF			1
#endif // _MSC_VER

/* Define to 1 if you have the `symlink' function. */
#ifndef TARGET_OS_WINDOWS
# define HAVE_SYMLINK			1
#endif // TARGET_OS_WINDOWS

/* Define to 1 if you have the <sys/time.h> header file. */
#define HAVE_SYS_TIME_H			1
/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H		1
/* Define to 1 if you have the `vasprintf' function. */
#define HAVE_VASPRINTF			1
/* Define if you have the 'wchar_t' type. */
#define HAVE_WCHAR_T			1
/* Define to 1 if you have the `wcrtomb' function. */
#define HAVE_WCRTOMB			1
/* Define to 1 if you have the `wcslen' function. */
#define HAVE_WCSLEN				1
/* Define to 1 if you have the `wcsnlen' function. */
#define HAVE_WCSNLEN			1
/* Define if you have the 'wint_t' type. */
#ifdef _MSC_VER
# define HAVE_WINT_T				1
#else
# define HAVE_WINT_T				0
#endif // _MSC_VER

#ifndef HAVE_STD__U16_STRING
# define HAVE_STD__U16_STRING	1
#endif // HAVE_STD__U16_STRING
#ifndef HAVE_STD__U32_STRING
# define HAVE_STD__U32_STRING	1 
#endif // HAVE_STD__U32_STRING

#ifndef TARGET_OS_WINDOWS
# define HAVE_SYS_SELECT_H		1
# define HAVE_SYS_TIME_H		1
#endif // TARGET_OS_WINDOWS

/* Define to 1 if you have the <termios.h> header file. */
#ifdef _MSC_VER
//# define HAVE_TERMIOS_H		0
#else
# define HAVE_TERMIOS_H			1
#endif // _MSC_VER

/* Define to 1 if you have the `uname' function. */
#ifndef _MSC_VER
# define HAVE_UNAME 1
#endif // _MSC_VER

#define HAVE_UNISTD_H				1
#define HAVE_VALGRIND_VALGRIND_H	0
/* Define to 1 if you have the `vasprintf' function. */
#define HAVE_VASPRINTF				1 // windows simulated in CoreLib

/* Defined if variable length arrays are supported */
#ifndef _MSC_VER
# define HAVE_VLA					1
#endif // _MSC_VER

/* Define to 1 if you have the `uselocale' function. */
#ifdef _MSC_VER
# define HAVE_USELOCALE				0
#else
# define HAVE_USELOCALE				0
#endif // _MSC_VER

/* Define to 1 or 0, depending whether the compiler supports simple visibility declarations. */
#ifdef _MSC_VER
# define HAVE_VISIBILITY			0
#else
# define HAVE_VISIBILITY				1
#endif // _MSC_VER

/* Define to 1 if you have the `vprintf' function. */
#define HAVE_VPRINTF				1

/* Define to 1 if you have the <winsock2.h> header file. */
#ifdef _MSC_VER
# define HAVE_WINSOCK2_H			1
# define HAVE_WINDOWS_H				1
#else
# define HAVE_WINSOCK2_H			0
# define HAVE_WINDOWS_H				0
#endif // _MSC_VER

/* Define to 1 if you have the `wait4' function. */
#ifndef TARGET_OS_WINDOWS
# define HAVE_WAIT4					1
#endif // TARGET_OS_WINDOWS

/* Define to 1 if you have the `waitpid' function. */
#ifndef TARGET_OS_WINDOWS
# define HAVE_WAITPID				1
#endif // TARGET_OS_WINDOWS

/* Define to 1 if you have the `waitid' function. */
#ifndef TARGET_OS_WINDOWS
# define HAVE_WAITID				1
#endif // TARGET_OS_WINDOWS


#ifndef TARGET_OS_WINDOWS
# define HAS_UTIME					1		/* POSIX utime(path, times)	*/
# define HAS_UTIME_H					1			/* UTIME header file		*/
# define HAS_UTIMBUF					1		/* struct utimbuf		*/
# define HAS_UTIMES					1		/* use utimes()	syscall instead	*/
#endif // TARGET_OS_WINDOWS

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS				1

#if defined(USE_STATIC_LIBS) && !defined(TAGLIB_STATIC)
# define TAGLIB_STATIC
#endif // TAGLIB_STATIC

#ifndef __PRETTY_FUNCTION__
# ifdef TARGET_OS_WINDOWS
#  define __PRETTY_FUNCTION__ __FUNCTION__
# else
#  define __PRETTY_FUNCTION__ __ROUTINE__
# endif // TARGET_OS_WINDOWS
#endif // __PRETTY_FUNCTION__

#if !defined(SIG_BLOCK)
# define SIG_BLOCK 0
#endif /* SIG_BLOCK */

#if !defined(SIG_UNBLOCK)
# define SIG_UNBLOCK 1
#endif /* SIG_UNBLOCK */

#if !defined(SIG_SETMASK)
# define SIG_SETMASK 2
#endif /* SIG_SETMASK */

/* Make use of alloca */
#define USE_ALLOCA					1
/* Define if the POSIX multithreading library can be used. */
#define USE_POSIX_THREADS			1

// VS does not have #pragma weak
//#define USE_POSIX_THREADS_WEAK		1 // use weak reference to pthread ( seem alot of code requires it )

/* Define to 1 if you want getc etc. to use unlocked I/O if available.
Unlocked I/O can improve performance in unithreaded apps, but it is not
safe for multithreaded apps. */
#define USE_UNLOCKED_IO 1

/* Define if the native Windows multithreading API can be used. */
// for now stick with only posix threads ( We have libpthread for windows )
//#ifdef _MSC_VER
//#define USE_WINDOWS_THREADS  1
//#endif // _MSC_VER

#define HAVE_U64_TYPEDEF	1
#define HAVE_U32_TYPEDEF	1
/* Define as the return type of signal handlers (`int' or `void'). */
#define RETSIGTYPE void
/* Define to the type of arg 1 for `select'. */
#define SELECT_TYPE_ARG1 int
/* Define to the type of args 2, 3 and 4 for `select'. */
#define SELECT_TYPE_ARG234 (fd_set *)
/* Define to the type of arg 5 for `select'. */
#define SELECT_TYPE_ARG5 (struct timeval *)
/* Define to the type qualifier of arg 5 for select. */
//#define SELECT_QUAL_ARG5 


#define RCODE long // clearer way to specify return error code


//============================================================================
//=== target windows specific ===//
//============================================================================
#ifdef TARGET_OS_WINDOWS

# define HAS_WIN32_NETWORK
# define DECLARE_UNUSED(a,b) a b;

#define D3D_DEBUG_INFO

#define HAVE_STD__U16_STRING 1 
#define HAVE_STD__U32_STRING 1 
#define HAVE_CHAR16_T 1 
#define HAVE_CHAR32_T 1 
#define HAVE_STDINT_H 1 
#define HAVE_INTTYPES_H 1 
#define FFMPEG_VER_SHA "3.4" 
#define PCRE_STATIC 1 
#define TIXML_USE_STL 1 

// must be last so all the correct preprocessor defines are defined
# include <limits.h>
# ifdef _MSC_VER
#  include <mbstring.h>
#  include <process.h>
# endif // _MSC_VER

#endif // TARGET_OS_WINDOWS

/* Define as the maximum value of type 'size_t', if the system doesn't define
it. */
#ifndef SIZE_MAX
# define SIZE_MAX ( ( size_t )-1 )
#endif // SIZE_MAX

#ifndef	FALSE
# define	FALSE	(0)
#endif

#ifndef	TRUE
# define	TRUE	(!FALSE)
#endif

// these must be same as Microsoft's or get error in d3dglobal.h
#define SAFE_RELEASE(p)       { if (p) { (p)->Release();  (p) = nullptr; } }
#define SAFE_ADDREF(p)        { if (p) { (p)->AddRef(); } }
#define SAFE_DELETE_ARRAY(p)  { delete [](p); p = nullptr; }
#define SAFE_DELETE(p)        { delete (p); p = nullptr;  }


/* By default, we use the hardwired pathnames.  */
# define relocate(pathname) (pathname)
# define GETTEXTDATADIR						".textdata/"
# define GETTEXTJAR							".textjar/"
# define PACKAGE_SUFFIX						".gnu"
# define PROJECTSDIR						".gotvp2p/"


