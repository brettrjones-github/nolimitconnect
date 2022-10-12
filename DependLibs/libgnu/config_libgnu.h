
#pragma once

#include <NlcDependLibrariesConfig.h>
#include <libgnu/g-gnulib.h>

#include <libiconv/iconv.h>
#include <libgnu/xalloc.h>

/* The normal alignment of `guint32', in bytes. */
#if ARCH_64_BITS
# define ALIGNOF_GUINT32 8
# define BITSIZEOF_PTRDIFF_T 64
# define BITSIZEOF_SIZE_T 64
#else
# define ALIGNOF_GUINT32 4
# define BITSIZEOF_PTRDIFF_T 32
# define BITSIZEOF_SIZE_T 32
#endif // ARCH_64_BITS

/* Define to the number of bits in type 'sig_atomic_t'. */
/* #undef BITSIZEOF_SIG_ATOMIC_T */

/* Define to the number of bits in type 'wchar_t'. */
/* #undef BITSIZEOF_WCHAR_T */

/* Define to the number of bits in type 'wint_t'. */
/* #undef BITSIZEOF_WINT_T */

/* The normal alignment of `guint64', in bytes. */
#define ALIGNOF_GUINT64 8

/* The normal alignment of `unsigned long', in bytes. */
#if ARCH_64_BITS
# define ALIGNOF_UNSIGNED_LONG 8
#else
# define ALIGNOF_UNSIGNED_LONG 4
#endif // ARCH_64_BITS

/* poll doesn't work on devices */
#ifdef _MSC_VER
# define BROKEN_POLL 1
#endif // _MSC_VER

/* Define to 1 if using `alloca.c'. */
#ifdef _MSC_VER
# define C_ALLOCA 1
#endif // _MSC_VER

/* Whether to disable memory pools */
#ifndef _MSC_VER
# define DISABLE_MEM_POOLS 1
#endif // _MSC_VER

/* Whether to enable GC friendliness by default */
/* #undef ENABLE_GC_FRIENDLY_DEFAULT */

/* A 'va_copy' style function */
#ifndef _MSC_VER
# define G_VA_COPY va_copy
#endif // _MSC_VER

/* 'va_lists' cannot be copies as values */
#ifndef _MSC_VER
# define G_VA_COPY_AS_ARRAY 1
#endif // _MSC_VER


/* Define to 1 if the C locale may have encoding errors. */
#ifdef _MSC_VER
# define C_LOCALE_MAYBE_EILSEQ 0
#else
# define C_LOCALE_MAYBE_EILSEQ 1
#endif // _MSC_VER

/* Define to 1 if // is a file system root distinct from /. */
/* #undef DOUBLE_SLASH_IS_DISTINCT_ROOT */

/* Define to 1 if you have the `nanosleep' function. */
#ifdef TARGET_OS_ANDROID
# define HAVE_NANOSLEEP 1
#else
# define HAVE_NANOSLEEP 0
#endif // TARGET_OS_ANDROID

/* Define to 1 if translation of program messages to the user's native
   language is requested. */
#define ENABLE_NLS 1


/* Define this to 'void' or 'struct timezone' to match the system's
   declaration of the second argument to gettimeofday. */
#define GETTIMEOFDAY_TIMEZONE struct timezone

/* Define to a C preprocessor expression that evaluates to 1 or 0, depending
   whether the gnulib module canonicalize-lgpl shall be considered present. */
#define GNULIB_CANONICALIZE_LGPL 1

/* Define to a C preprocessor expression that evaluates to 1 or 0, depending
   whether the gnulib module fscanf shall be considered present. */
#define GNULIB_FSCANF 1

/* Define to a C preprocessor expression that evaluates to 1 or 0, depending
   whether the gnulib module fwriteerror shall be considered present. */
#define GNULIB_FWRITEERROR 1

/* Define to a C preprocessor expression that evaluates to 1 or 0, depending
   whether the gnulib module lock shall be considered present. */
#define GNULIB_LOCK 1

/* Define to 1 if printf and friends should be labeled with attribute
   "__gnu_printf__" instead of "__printf__" */
/* #undef GNULIB_PRINTF_ATTRIBUTE_FLAVOR_GNU */

/* Define to a C preprocessor expression that evaluates to 1 or 0, depending
   whether the gnulib module scanf shall be considered present. */
#define GNULIB_SCANF 1

/* Define to a C preprocessor expression that evaluates to 1 or 0, depending
   whether the gnulib module sigpipe shall be considered present. */
#define GNULIB_SIGPIPE 1

/* Define to a C preprocessor expression that evaluates to 1 or 0, depending
   whether the gnulib module strerror shall be considered present. */
#define GNULIB_STRERROR 1


/* Define to a C preprocessor expression that evaluates to 1 or 0, depending
   whether the gnulib module unistr/u8-mbtoucr shall be considered present. */
#define GNULIB_UNISTR_U8_MBTOUCR 1

/* Define to a C preprocessor expression that evaluates to 1 or 0, depending
   whether the gnulib module unistr/u8-uctomb shall be considered present. */
#define GNULIB_UNISTR_U8_UCTOMB 1

/* Define to 1 if you have the `asprintf' function. */
#define HAVE_ASPRINTF 1

#ifdef _MSC_VER
   /* Define to 1 if you have the declaration of `getchar_unlocked', and to 0 if you don't. */
# define HAVE_DECL_GETCHAR_UNLOCKED		0
   /* Define to 1 if you have the declaration of `getc_unlocked', and to 0 if you don't. */
# define HAVE_DECL_GETC_UNLOCKED		0
   /* Define to 1 if you have the declaration of `clearerr_unlocked', and to 0 if you don't. */
# define HAVE_DECL_CLEARERR_UNLOCKED		0
/* Define to 1 if you have the declaration of `feof_unlocked', and to 0 if you don't. */
# define HAVE_DECL_FEOF_UNLOCKED			0
/* Define to 1 if you have the declaration of `ferror_unlocked', and to 0 if you don't. */
# define HAVE_DECL_FERROR_UNLOCKED		0
/* Define to 1 if you have the declaration of `fflush_unlocked', and to 0 if you don't. */
# define HAVE_DECL_FFLUSH_UNLOCKED		0
/* Define to 1 if you have the declaration of `fgets_unlocked', and to 0 if you don't. */
# define HAVE_DECL_FGETS_UNLOCKED		0
/* Define to 1 if you have the declaration of `fputc_unlocked', and to 0 if you don't. */
# define HAVE_DECL_FPUTC_UNLOCKED		0
/* Define to 1 if you have the declaration of `fputs_unlocked', and to 0 if you don't. */
# define HAVE_DECL_FPUTS_UNLOCKED		0
/* Define to 1 if you have the declaration of `fread_unlocked', and to 0 if you don't. */
# define HAVE_DECL_FREAD_UNLOCKED		0
   /* Define to 1 if you have the declaration of `fwrite_unlocked', and to 0 if you don't. */
# define HAVE_DECL_FWRITE_UNLOCKED		0
#else
# if defined(TARGET_OS_ANDROID)
#  define HAVE_DECL_GETC_UNLOCKED		0
#  define HAVE_DECL_GETCHAR_UNLOCKED    0
#  define HAVE_DECL_CLEARERR_UNLOCKED	0
#  define HAVE_DECL_FEOF_UNLOCKED		0
#  define HAVE_DECL_FGETS_UNLOCKED		0
#  define HAVE_DECL_FERROR_UNLOCKED		0
#  define HAVE_DECL_FFLUSH_UNLOCKED		0
#  define HAVE_DECL_FPUTC_UNLOCKED		0
#  define HAVE_DECL_FPUTS_UNLOCKED		0
#  define HAVE_DECL_FREAD_UNLOCKED		0
#  define HAVE_DECL_FWRITE_UNLOCKED		0
# else
#  define HAVE_DECL_GETC_UNLOCKED		1
#  define HAVE_DECL_GETCHAR_UNLOCKED		1
#  define HAVE_DECL_CLEARERR_UNLOCKED	1
#  define HAVE_DECL_FEOF_UNLOCKED		1
#  define HAVE_DECL_FGETS_UNLOCKED		1
#  define HAVE_DECL_FERROR_UNLOCKED		1
#  define HAVE_DECL_FFLUSH_UNLOCKED		1
#  define HAVE_DECL_FPUTC_UNLOCKED		1
#  define HAVE_DECL_FPUTS_UNLOCKED		1
#  define HAVE_DECL_FREAD_UNLOCKED		1
#  define HAVE_DECL_FWRITE_UNLOCKED		1
# endif // defined(TARGET_OS_ANDROID)
#endif // _MSC_VER

/* Define to 1 if you have the declaration of `getenv', and to 0 if you don't.
   */
#define HAVE_DECL_GETENV 1

/* Define to 1 if you have the declaration of `iswblank', and to 0 if you
   don't. */
#define HAVE_DECL_ISWBLANK 1


/* Define to 1 if you have the declaration of `program_invocation_name', and
   to 0 if you don't. */
#define HAVE_DECL_PROGRAM_INVOCATION_NAME 0

/* Define to 1 if you have the declaration of `program_invocation_short_name',
   and to 0 if you don't. */
#define HAVE_DECL_PROGRAM_INVOCATION_SHORT_NAME 0

/* Define to 1 if you have the declaration of `putchar_unlocked', and to 0 if
   you don't. */

/* Define to 1 if you have the declaration of `putc_unlocked', and to 0 if you don't. */
#ifdef _MSC_VER
# define HAVE_DECL_PUTCHAR_UNLOCKED 0
# define HAVE_DECL_PUTC_UNLOCKED 0
#else
# define HAVE_DECL_PUTCHAR_UNLOCKED 1
# define HAVE_DECL_PUTC_UNLOCKED 1
#endif // _MSC_VER

/* Define to 1 if you have the declaration of `setenv', and to 0 if you don't.
   */
#define HAVE_DECL_SETENV 1

/* Define to 1 if you have the declaration of `strerror_r', and to 0 if you
   don't. */
#define HAVE_DECL_STRERROR_R 1

/* Define to 1 if you have the declaration of `strnlen', and to 0 if you
   don't. */
#define HAVE_DECL_STRNLEN 1

/* Define to 1 if you have the declaration of `towlower', and to 0 if you
   don't. */
/* #undef HAVE_DECL_TOWLOWER */

/* Define to 1 if you have the declaration of `wcwidth', and to 0 if you
   don't. */
#ifdef _MSC_VER
# define HAVE_DECL_WCWIDTH 0
#else
# define HAVE_DECL_WCWIDTH 1
#endif // _MSC_VER

/* Define to 1 if you have the declaration of `_snprintf', and to 0 if you
   don't. */
#define HAVE_DECL__SNPRINTF 0

/* Define to 1 if you have the declaration of `_snwprintf', and to 0 if you
   don't. */
#define HAVE_DECL__SNWPRINTF 0



/* Define if you have the declaration of environ. */
//#define HAVE_ENVIRON_DECL 1





/* Define to 1 if you have the <getopt.h> header file. */
#define HAVE_GETOPT_H 1

/* Define to 1 if you have the `getopt_long_only' function. */
#define HAVE_GETOPT_LONG_ONLY 1

/* Define to 1 if you have the `getpagesize' function. */
#define HAVE_GETPAGESIZE 1



/* Define to 1 if you have the `iswblank' function. */
#define HAVE_ISWBLANK 1

/* Define to 1 if you have the `iswcntrl' function. */
#define HAVE_ISWCNTRL 1

/* Define if you have <libgnu/langinfo.h> and nl_langinfo(CODESET). */
#define HAVE_LANGINFO_CODESET 1

/* Define to 1 if you have the <libgnu/langinfo.h> header file. */
#define HAVE_LANGINFO_H  1

/* Define to 1 if <sys/param.h> defines the MIN and MAX macros. */
#ifdef _MSC_VER
# define HAVE_MINMAX_IN_SYS_PARAM_H 0
#else
# define HAVE_MINMAX_IN_SYS_PARAM_H 1
#endif // _MSC_VER

/* Define to 1 on MSVC platforms that have the "invalid parameter handler"
   concept. */
#ifdef _MSC_VER
#define HAVE_MSVC_INVALID_PARAMETER_HANDLER 1
#else
# define HAVE_MSVC_INVALID_PARAMETER_HANDLER 0
#endif // _MSC_VER




/* Define to 1 if you have the `munmap' function. */
#ifdef _MSC_VER
# define HAVE_MUNMAP 0
#else
# define HAVE_MUNMAP 1
#endif // _MSC_VER


/* Define if the <libpthread/pthread.h> defines PTHREAD_MUTEX_RECURSIVE. */
#define HAVE_PTHREAD_MUTEX_RECURSIVE 1

/* Define if the POSIX multithreading library has read/write locks. */
#define HAVE_PTHREAD_RWLOCK 1

/* Define to 1 if you have the `raise' function. */
#define HAVE_RAISE 1

/* Define to 1 if atoll is declared even after undefining macros. */
#define HAVE_RAW_DECL_ATOLL 1

/* Define to 1 if btowc is declared even after undefining macros. */
#define HAVE_RAW_DECL_BTOWC 1

/* Define to 1 if duplocale is declared even after undefining macros. */
#define HAVE_RAW_DECL_DUPLOCALE 1


/* Define to 1 if environ is declared even after undefining macros. */
#ifdef _MSC_VER
# define HAVE_RAW_DECL_ENVIRON 1
#endif // _MSC_VER

/* Define to 1 if faccessat is declared even after undefining macros. */
#define HAVE_RAW_DECL_FACCESSAT 1

/* Define to 1 if fchdir is declared even after undefining macros. */
#define HAVE_RAW_DECL_FCHDIR 1

/* Define to 1 if fchmodat is declared even after undefining macros. */
#define HAVE_RAW_DECL_FCHMODAT 1

/* Define to 1 if fchownat is declared even after undefining macros. */
#define HAVE_RAW_DECL_FCHOWNAT 1

/* Define to 1 if fdatasync is declared even after undefining macros. */
#define HAVE_RAW_DECL_FDATASYNC 1


/* Define to 1 if fpurge is declared even after undefining macros. */
#define HAVE_RAW_DECL_FPURGE 1

/* Define to 1 if fseeko is declared even after undefining macros. */
#define HAVE_RAW_DECL_FSEEKO 1


/* Define to 1 if fsync is declared even after undefining macros. */
#define HAVE_RAW_DECL_FSYNC 1

/* Define to 1 if ftello is declared even after undefining macros. */
#define HAVE_RAW_DECL_FTELLO 1

/* Define to 1 if ftruncate is declared even after undefining macros. */
#define HAVE_RAW_DECL_FTRUNCATE 1

/* Define to 1 if futimens is declared even after undefining macros. */
#define HAVE_RAW_DECL_FUTIMENS 1

/* Define to 1 if getcwd is declared even after undefining macros. */
#define HAVE_RAW_DECL_GETCWD 1

/* Define to 1 if getdelim is declared even after undefining macros. */
#define HAVE_RAW_DECL_GETDELIM 1

/* Define to 1 if getgroups is declared even after undefining macros. */
#if _MSC_VER
# define HAVE_RAW_DECL_GETGROUPS	0
#else
# define HAVE_RAW_DECL_GETGROUPS	1
#endif // _MSC_VER

/* Define to 1 if gethostname is declared even after undefining macros. */
#define HAVE_RAW_DECL_GETHOSTNAME 1

/* Define to 1 if getline is declared even after undefining macros. */
#if _MSC_VER
# define HAVE_RAW_DECL_GETLINE	0
#else
# define HAVE_RAW_DECL_GETLINE	1
#endif // _MSC_VER

/* Define to 1 if getlogin is declared even after undefining macros. */
#if _MSC_VER
# define HAVE_RAW_DECL_GETLOGIN	0
#else
# define HAVE_RAW_DECL_GETLOGIN	1
#endif // _MSC_VER

/* Define to 1 if getpagesize is declared even after undefining macros. */
#if _MSC_VER
# define HAVE_RAW_DECL_GETPAGESIZE	0
#else
# define HAVE_RAW_DECL_GETPAGESIZE	1
#endif // _MSC_VER

/* Define to 1 if gets is declared even after undefining macros. */
#if _MSC_VER
# define HAVE_RAW_DECL_GETS	0
#else
# define HAVE_RAW_DECL_GETS	1
#endif // _MSC_VER

/* Define to 1 if gettimeofday is declared even after undefining macros. */
#define HAVE_RAW_DECL_GETTIMEOFDAY 1
/* Define to 1 if grantpt is declared even after undefining macros. */
#define HAVE_RAW_DECL_GRANTPT 1
/* Define to 1 if initstate is declared even after undefining macros. */
#define HAVE_RAW_DECL_INITSTATE 1
/* Define to 1 if isatty is declared even after undefining macros. */
#define HAVE_RAW_DECL_ISATTY 1
/* Define to 1 if iswctype is declared even after undefining macros. */
#define HAVE_RAW_DECL_ISWCTYPE 1
/* Define to 1 if lchown is declared even after undefining macros. */
#define HAVE_RAW_DECL_LCHOWN 1
/* Define to 1 if link is declared even after undefining macros. */
#define HAVE_RAW_DECL_LINK 1
/* Define to 1 if linkat is declared even after undefining macros. */
#define HAVE_RAW_DECL_LINKAT 1
/* Define to 1 if lseek is declared even after undefining macros. */
#define HAVE_RAW_DECL_LSEEK 1
/* Define to 1 if lstat is declared even after undefining macros. */
#define HAVE_RAW_DECL_LSTAT 1
/* Define to 1 if mbrlen is declared even after undefining macros. */
#define HAVE_RAW_DECL_MBRLEN 1
/* Define to 1 if mbrtowc is declared even after undefining macros. */
#define HAVE_RAW_DECL_MBRTOWC 1
/* Define to 1 if mbsinit is declared even after undefining macros. */
#define HAVE_RAW_DECL_MBSINIT 1

/* Define to 1 if mbsnrtowcs is declared even after undefining macros. */
#define HAVE_RAW_DECL_MBSNRTOWCS 1

/* Define to 1 if mbsrtowcs is declared even after undefining macros. */
#define HAVE_RAW_DECL_MBSRTOWCS 1

/* Define to 1 if memmem is declared even after undefining macros. */
#define HAVE_RAW_DECL_MEMMEM 1


/* Define to 1 if memrchr is declared even after undefining macros. */
#define HAVE_RAW_DECL_MEMRCHR 1

/* Define to 1 if mkdirat is declared even after undefining macros. */
#define HAVE_RAW_DECL_MKDIRAT 1

/* Define to 1 if mkdtemp is declared even after undefining macros. */
#define HAVE_RAW_DECL_MKDTEMP 1

/* Define to 1 if mkfifo is declared even after undefining macros. */
#define HAVE_RAW_DECL_MKFIFO 1


/* Define to 1 if mknod is declared even after undefining macros. */
#define HAVE_RAW_DECL_MKNOD 1

/* Define to 1 if mknodat is declared even after undefining macros. */
#define HAVE_RAW_DECL_MKNODAT 1


/* Define to 1 if mkstemp is declared even after undefining macros. */
#define HAVE_RAW_DECL_MKSTEMP 1


/* Define to 1 if pclose is declared even after undefining macros. */
#define HAVE_RAW_DECL_PCLOSE 1

/* Define to 1 if pipe is declared even after undefining macros. */
#define HAVE_RAW_DECL_PIPE 1

/* Define to 1 if pipe2 is declared even after undefining macros. */
#define HAVE_RAW_DECL_PIPE2 1

/* Define to 1 if popen is declared even after undefining macros. */
#define HAVE_RAW_DECL_POPEN 1

/* Define to 1 if posix_openpt is declared even after undefining macros. */
#define HAVE_RAW_DECL_POSIX_OPENPT 1

/* Define to 1 if pread is declared even after undefining macros. */
#define HAVE_RAW_DECL_PREAD 1


/* Define to 1 if ptsname is declared even after undefining macros. */
#define HAVE_RAW_DECL_PTSNAME 1

/* Define to 1 if ptsname_r is declared even after undefining macros. */
#define HAVE_RAW_DECL_PTSNAME_R 1

/* Define to 1 if pwrite is declared even after undefining macros. */
#define HAVE_RAW_DECL_PWRITE 1

/* Define to 1 if random is declared even after undefining macros. */
#define HAVE_RAW_DECL_RANDOM 1

/* Define to 1 if unlink is declared even after undefining macros. */
#define HAVE_RAW_DECL_UNLINK 1

/* Define to 1 if wcsncasecmp is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCSNCASECMP 1

/* Define to 1 if wcsncat is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCSNCAT 1

/* Define to 1 if wcsncmp is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCSNCMP 1

/* Define to 1 if wcsncpy is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCSNCPY 1

/* Define to 1 if wcsnlen is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCSNLEN 1

/* Define to 1 if wcsnrtombs is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCSNRTOMBS 1

/* Define to 1 if wcspbrk is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCSPBRK 1

/* Define to 1 if wcsrchr is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCSRCHR 1

/* Define to 1 if wcsrtombs is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCSRTOMBS 1

/* Define to 1 if wcsspn is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCSSPN 1

/* Define to 1 if wcsstr is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCSSTR 1

/* Define to 1 if wcstok is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCSTOK 1

/* Define to 1 if wcswidth is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCSWIDTH 1

/* Define to 1 if wcsxfrm is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCSXFRM 1

/* Define to 1 if wctob is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCTOB 1

/* Define to 1 if wctrans is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCTRANS 1

/* Define to 1 if wctype is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCTYPE 1

/* Define to 1 if wcwidth is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCWIDTH 1

/* Define to 1 if wmemchr is declared even after undefining macros. */
#define HAVE_RAW_DECL_WMEMCHR 1

/* Define to 1 if wmemcmp is declared even after undefining macros. */
#define HAVE_RAW_DECL_WMEMCMP 1

/* Define to 1 if wmemcpy is declared even after undefining macros. */
#define HAVE_RAW_DECL_WMEMCPY 1

/* Define to 1 if wmemmove is declared even after undefining macros. */
#define HAVE_RAW_DECL_WMEMMOVE 1

/* Define to 1 if wmemset is declared even after undefining macros. */
#define HAVE_RAW_DECL_WMEMSET 1

/* Define to 1 if _Exit is declared even after undefining macros. */
#define HAVE_RAW_DECL__EXIT 1


/* Define to 1 if you have the <sys/param.h> header file. */
//#define HAVE_SYS_PARAM_H 1 // defined in NlcCompilerConfig.h

/* Define to 1 if you have the <sys/socket.h> header file. */
//#define HAVE_SYS_SOCKET_H 1 // defined in NlcCompilerConfig.h

/* Define to 1 if you have the <sys/types.h> header file. */
//#define HAVE_SYS_TYPES_H 1 // defined in NlcCompilerConfig.h

/* Define to 1 if you have the `towlower' function. */
#define HAVE_TOWLOWER 1

/* Define to 1 if you have the `tsearch' function. */
#if defined(TARGET_OS_ANDROID)
# define HAVE_TSEARCH 1
#endif //

/* Define if you have the 'uintmax_t' type in <stdint.h> or <inttypes.h>. */
#define HAVE_UINTMAX_T 1


/* Define to 1 if the system has the type 'unsigned long long int'. */
#define HAVE_UNSIGNED_LONG_LONG_INT 1



/* Define to 1 if you have the <wchar_gnu.h> header file. */
#define HAVE_WCHAR_H 1

/* Define if you have the 'wchar_t' type. */
#define HAVE_WCHAR_T 1

/* Define to 1 if you have the `wcrtomb' function. */
#define HAVE_WCRTOMB 1

/* Define to 1 if you have the `wcslen' function. */
#define HAVE_WCSLEN 1

/* Define to 1 if you have the `wcsnlen' function. */
#define HAVE_WCSNLEN 1

/* Define to 1 if you have the <wctype.h> header file. */
#define HAVE_WCTYPE_H 1

/* Define to 1 if you have the `wcwidth' function. */
#define HAVE_WCWIDTH 1

/* Define if you have the 'wint_t' type. */
//#define HAVE_WINT_T 1 defined in NlcCompileConfig.h

/* Define to 1 if O_NOATIME works. */
#define HAVE_WORKING_O_NOATIME 0

/* Define to 1 if O_NOFOLLOW works. */
#define HAVE_WORKING_O_NOFOLLOW 0


/* Define as const if the declaration of iconv() needs const. */
#define ICONV_CONST 

/* Define if integer division by zero raises signal SIGFPE. */
#define INTDIV0_RAISES_SIGFPE 0


/* Define to the prefix of C symbols at the assembler and linker level, either
an underscore or empty. */
#define USER_LABEL_PREFIX 









/* Define to rpl_ if the getopt replacement functions and variables should be
   used. */
#define __GETOPT_PREFIX rpl_


#define __libc_lock_t                   gl_lock_t
#define __libc_lock_define              gl_lock_define
#define __libc_lock_define_initialized  gl_lock_define_initialized
#define __libc_lock_init                gl_lock_init
#define __libc_lock_lock                gl_lock_lock
#define __libc_lock_unlock              gl_lock_unlock
#define __libc_lock_recursive_t                   gl_recursive_lock_t
#define __libc_lock_define_recursive              gl_recursive_lock_define
#define __libc_lock_define_initialized_recursive  gl_recursive_lock_define_initialized
#define __libc_lock_init_recursive                gl_recursive_lock_init
#define __libc_lock_lock_recursive                gl_recursive_lock_lock
#define __libc_lock_unlock_recursive              gl_recursive_lock_unlock
#define glthread_in_use  libintl_thread_in_use
#define glthread_lock_init_func     libintl_lock_init_func
#define glthread_lock_lock_func     libintl_lock_lock_func
#define glthread_lock_unlock_func   libintl_lock_unlock_func
#define glthread_lock_destroy_func  libintl_lock_destroy_func
#define glthread_rwlock_init_multithreaded     libintl_rwlock_init_multithreaded
#define glthread_rwlock_init_func              libintl_rwlock_init_func
#define glthread_rwlock_rdlock_multithreaded   libintl_rwlock_rdlock_multithreaded
#define glthread_rwlock_rdlock_func            libintl_rwlock_rdlock_func
#define glthread_rwlock_wrlock_multithreaded   libintl_rwlock_wrlock_multithreaded
#define glthread_rwlock_wrlock_func            libintl_rwlock_wrlock_func
#define glthread_rwlock_unlock_multithreaded   libintl_rwlock_unlock_multithreaded
#define glthread_rwlock_unlock_func            libintl_rwlock_unlock_func
#define glthread_rwlock_destroy_multithreaded  libintl_rwlock_destroy_multithreaded
#define glthread_rwlock_destroy_func           libintl_rwlock_destroy_func
#define glthread_recursive_lock_init_multithreaded     libintl_recursive_lock_init_multithreaded
#define glthread_recursive_lock_init_func              libintl_recursive_lock_init_func
#define glthread_recursive_lock_lock_multithreaded     libintl_recursive_lock_lock_multithreaded
#define glthread_recursive_lock_lock_func              libintl_recursive_lock_lock_func
#define glthread_recursive_lock_unlock_multithreaded   libintl_recursive_lock_unlock_multithreaded
#define glthread_recursive_lock_unlock_func            libintl_recursive_lock_unlock_func
#define glthread_recursive_lock_destroy_multithreaded  libintl_recursive_lock_destroy_multithreaded
#define glthread_recursive_lock_destroy_func           libintl_recursive_lock_destroy_func
#define glthread_once_func            libintl_once_func
#define glthread_once_singlethreaded  libintl_once_singlethreaded
#define glthread_once_multithreaded   libintl_once_multithreaded




//
/* Define to 1 if translation of program messages to the user's native
language is requested. */
#define ENABLE_NLS 1

/* Define to 1 if the package shall run at any location in the file system. */
/* #undef ENABLE_RELOCATABLE */

/* Define to 1 if realpath() can malloc memory, always gives an absolute path,
and handles trailing slash correctly. */
/* #undef FUNC_REALPATH_WORKS */

/* Define if gettimeofday clobbers the localtime buffer. */
/* #undef GETTIMEOFDAY_CLOBBERS_LOCALTIME */

/* Define this to 'void' or 'struct timezone' to match the system's
declaration of the second argument to gettimeofday. */
#define GETTIMEOFDAY_TIMEZONE struct timezone

/* Define to a C preprocessor expression that evaluates to 1 or 0, depending
whether the gnulib module canonicalize-lgpl shall be considered present. */
#define GNULIB_CANONICALIZE_LGPL 1

/* Define to a C preprocessor expression that evaluates to 1 or 0, depending
whether the gnulib module fscanf shall be considered present. */
#define GNULIB_FSCANF 1

/* Define to a C preprocessor expression that evaluates to 1 or 0, depending
whether the gnulib module fwriteerror shall be considered present. */
#define GNULIB_FWRITEERROR 1

/* Define to a C preprocessor expression that evaluates to 1 or 0, depending
whether the gnulib module lock shall be considered present. */
#define GNULIB_LOCK 1

/* Define to 1 if printf and friends should be labeled with attribute
"__gnu_printf__" instead of "__printf__" */
/* #undef GNULIB_PRINTF_ATTRIBUTE_FLAVOR_GNU */

/* Define to a C preprocessor expression that evaluates to 1 or 0, depending
whether the gnulib module scanf shall be considered present. */
#define GNULIB_SCANF 1

/* Define to a C preprocessor expression that evaluates to 1 or 0, depending
whether the gnulib module sigpipe shall be considered present. */
#define GNULIB_SIGPIPE 1

/* Define to a C preprocessor expression that evaluates to 1 or 0, depending
whether the gnulib module strerror shall be considered present. */
#define GNULIB_STRERROR 1

///* Define to 1 when the gnulib module canonicalize_file_name should be tested.
//*/
//#define GNULIB_TEST_CANONICALIZE_FILE_NAME 1
//
///* Define to 1 when the gnulib module environ should be tested. */
//#define GNULIB_TEST_ENVIRON 1
//
///* Define to 1 when the gnulib module getopt-gnu should be tested. */
//#define GNULIB_TEST_GETOPT_GNU 1
//
///* Define to 1 when the gnulib module gettimeofday should be tested. */
//#define GNULIB_TEST_GETTIMEOFDAY 1
//
///* Define to 1 when the gnulib module iswblank should be tested. */
//#define GNULIB_TEST_ISWBLANK 1
//
///* Define to 1 when the gnulib module lstat should be tested. */
//#define GNULIB_TEST_LSTAT 1
//
///* Define to 1 when the gnulib module mbrtowc should be tested. */
//#define GNULIB_TEST_MBRTOWC 1
//
///* Define to 1 when the gnulib module mbsinit should be tested. */
//#define GNULIB_TEST_MBSINIT 1
//
///* Define to 1 when the gnulib module mbslen should be tested. */
//#define GNULIB_TEST_MBSLEN 1
//
///* Define to 1 when the gnulib module mbsstr should be tested. */
//#define GNULIB_TEST_MBSSTR 1
//
///* Define to 1 when the gnulib module memchr should be tested. */
//#define GNULIB_TEST_MEMCHR 1
//
///* Define to 1 when the gnulib module raise should be tested. */
//#define GNULIB_TEST_RAISE 1
//
///* Define to 1 when the gnulib module readlink should be tested. */
//#define GNULIB_TEST_READLINK 1
//
///* Define to 1 when the gnulib module realpath should be tested. */
//#define GNULIB_TEST_REALPATH 1
//
///* Define to 1 when the gnulib module setlocale should be tested. */
//#define GNULIB_TEST_SETLOCALE 1
//
///* Define to 1 when the gnulib module sigprocmask should be tested. */
//#define GNULIB_TEST_SIGPROCMASK 1

///* Define to 1 when the gnulib module stat should be tested. */
//#define GNULIB_TEST_STAT 1
//
///* Define to 1 when the gnulib module strerror should be tested. */
//#define GNULIB_TEST_STRERROR 1
//
///* Define to 1 when the gnulib module strnlen should be tested. */
//#define GNULIB_TEST_STRNLEN 1
//
///* Define to 1 when the gnulib module wcwidth should be tested. */
//#define GNULIB_TEST_WCWIDTH 1

/* Define to a C preprocessor expression that evaluates to 1 or 0, depending
whether the gnulib module unistr/u8-mbtoucr shall be considered present. */
#define GNULIB_UNISTR_U8_MBTOUCR 1

/* Define to a C preprocessor expression that evaluates to 1 or 0, depending
whether the gnulib module unistr/u8-uctomb shall be considered present. */
#define GNULIB_UNISTR_U8_UCTOMB 1

/* Define to 1 if you have `alloca', as a function or macro. */
#define HAVE_ALLOCA 1

/* Define to 1 if you have <libgnu/alloca_gnu.h> and it should be used (not on Ultrix).
*/
#define HAVE_ALLOCA_H 1

/* Define to 1 if you have the `argz_count' function. */
/* #undef HAVE_ARGZ_COUNT */

/* Define to 1 if you have the <argz.h> header file. */
/* #undef HAVE_ARGZ_H */

/* Define to 1 if you have the `argz_next' function. */
/* #undef HAVE_ARGZ_NEXT */

/* Define to 1 if you have the `argz_stringify' function. */
/* #undef HAVE_ARGZ_STRINGIFY */

/* Define to 1 if you have the `asprintf' function. */
#define HAVE_ASPRINTF 1

/* Define to 1 if you have the `atexit' function. */
#define HAVE_ATEXIT 1

/* Define to 1 if you have the <bp-sym.h> header file. */
/* #undef HAVE_BP_SYM_H */

/* Define to 1 if the compiler understands __builtin_expect. */
#ifdef _MSC_VER
# define __builtin_expect(expr, val) (expr)
#else
# define HAVE_BUILTIN_EXPECT		1
#endif // _MSC_VER

/* Define to 1 if you have the `canonicalize_file_name' function. */
/* #undef HAVE_CANONICALIZE_FILE_NAME */

/* Define to 1 if you have the Mac OS X function CFLocaleCopyCurrent in the
CoreFoundation framework. */
/* #undef HAVE_CFLOCALECOPYCURRENT */

/* Define to 1 if you have the Mac OS X function CFPreferencesCopyAppValue in
the CoreFoundation framework. */
/* #undef HAVE_CFPREFERENCESCOPYAPPVALUE */

/* Define if the GNU dcgettext() function is already present or preinstalled.
*/
/* #undef HAVE_DCGETTEXT */


/* Define to 1 if you have the declaration of `getenv', and to 0 if you don't.
*/
#define HAVE_DECL_GETENV 1

/* Define to 1 if you have the declaration of `iswblank', and to 0 if you
don't. */
#define HAVE_DECL_ISWBLANK 1

/* Define to 1 if you have the declaration of `mbrtowc', and to 0 if you
don't. */
/* #undef HAVE_DECL_MBRTOWC */

/* Define to 1 if you have the declaration of `mbsinit', and to 0 if you
don't. */
/* #undef HAVE_DECL_MBSINIT */

/* Define to 1 if you have the declaration of `program_invocation_name', and
to 0 if you don't. */
#define HAVE_DECL_PROGRAM_INVOCATION_NAME 0

/* Define to 1 if you have the declaration of `program_invocation_short_name',
and to 0 if you don't. */
#define HAVE_DECL_PROGRAM_INVOCATION_SHORT_NAME 0

/* Define to 1 if you have the declaration of `setenv', and to 0 if you don't.
*/
#define HAVE_DECL_SETENV 1


/* Define to 1 if you have the declaration of `strnlen', and to 0 if you
don't. */
#define HAVE_DECL_STRNLEN 1

/* Define to 1 if you have the declaration of `towlower', and to 0 if you
don't. */
/* #undef HAVE_DECL_TOWLOWER */


/* Define to 1 if you have the declaration of `_snprintf', and to 0 if you
don't. */
#define HAVE_DECL__SNPRINTF 0

/* Define to 1 if you have the declaration of `_snwprintf', and to 0 if you
don't. */
#define HAVE_DECL__SNWPRINTF 0

/* Define to 1 if you have the <dlfcn.h> header file. */
#define HAVE_DLFCN_H 1

/* Define if you have the declaration of environ. */
#define HAVE_ENVIRON_DECL 1


/* Define to 1 if you have the `fwprintf' function. */
#define HAVE_FWPRINTF 0

/* Define to 1 if you have the `getlocalename_l' function. */
/* #undef HAVE_GETLOCALENAME_L */

/* Define to 1 if you have the <getopt.h> header file. */
#define HAVE_GETOPT_H 1

/* Define to 1 if you have the `getopt_long_only' function. */
#define HAVE_GETOPT_LONG_ONLY 1

/* Define to 1 if you have the `getpagesize' function. */
#define HAVE_GETPAGESIZE 1

/* Define if the GNU gettext() function is already present or preinstalled. */
/* #undef HAVE_GETTEXT */

/* Define to 1 if you have the `gettimeofday' function. */
#define HAVE_GETTIMEOFDAY 1

/* Define to 1 if you have the <libiconv/iconv.h> header file. */
#define HAVE_ICONV_H 1

/* Define to 1 if the compiler supports one of the keywords 'NLC_INLINE',
'__inline__', '__inline' and effectively inlines functions marked as such.
*/
#define HAVE_INLINE 1

/* Define if you have the 'intmax_t' type in <stdint.h> or <inttypes.h>. */
#define HAVE_INTMAX_T 1

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define if <inttypes.h> exists, doesn't clash with <sys/types.h>, and
declares uintmax_t. */
#define HAVE_INTTYPES_H_WITH_UINTMAX 1

/* Define to 1 if you have the `iswblank' function. */
#define HAVE_ISWBLANK 1

/* Define to 1 if you have the `iswcntrl' function. */
#define HAVE_ISWCNTRL 1

/* Define if you have <libgnu/langinfo.h> and nl_langinfo(CODESET). */
/* #undef HAVE_LANGINFO_CODESET */

/* Define to 1 if you have the <libgnu/langinfo.h> header file. */
/* #undef HAVE_LANGINFO_H */

/* Define if your <locale.h> file defines LC_MESSAGES. */
#define HAVE_LC_MESSAGES 1

/* Define to 1 if you have the <limits.h> header file. */
#define HAVE_LIMITS_H 1

/* Define to 1 if the system has the type 'long long int'. */
#define HAVE_LONG_LONG_INT 1


/* Define to 1 if you have the <mach-o/dyld.h> header file. */
/* #undef HAVE_MACH_O_DYLD_H */

/* Define to 1 if mmap()'s MAP_ANONYMOUS flag is available after including
config.h and <sys/mman.h>. */
#define HAVE_MAP_ANONYMOUS 1

/* Define to 1 if you have the `mbrtowc' function. */
#define HAVE_MBRTOWC 1

/* Define to 1 if you have the `mbsinit' function. */
#define HAVE_MBSINIT 1

/* Define to 1 if you have the `mbslen' function. */
/* #undef HAVE_MBSLEN */

/* Define to 1 if <wchar_gnu.h> declares mbstate_t. */
#define HAVE_MBSTATE_T 1

/* Define to 1 if you have the `memmove' function. */
#define HAVE_MEMMOVE 1

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define to 1 if you have the `mempcpy' function. */
/* #undef HAVE_MEMPCPY */

/* Define to 1 if you have a working `mmap' system call. */
/* #undef HAVE_MMAP */



/* Define if your printf() function supports format strings with positions. */
#define HAVE_POSIX_PRINTF 1

/* Define if the <libpthread/pthread.h> defines PTHREAD_MUTEX_RECURSIVE. */
#define HAVE_PTHREAD_MUTEX_RECURSIVE 1

/* Define if the POSIX multithreading library has read/write locks. */
#define HAVE_PTHREAD_RWLOCK 1

/* Define to 1 if you have the `putenv' function. */
#define HAVE_PUTENV 1

/* Define to 1 if you have the `raise' function. */
#define HAVE_RAISE 1

/* Define to 1 if atoll is declared even after undefining macros. */
#define HAVE_RAW_DECL_ATOLL 1

/* Define to 1 if btowc is declared even after undefining macros. */
#define HAVE_RAW_DECL_BTOWC 1

/* Define to 1 if canonicalize_file_name is declared even after undefining
macros. */
/* #undef HAVE_RAW_DECL_CANONICALIZE_FILE_NAME */

/* Define to 1 if chdir is declared even after undefining macros. */
#define HAVE_RAW_DECL_CHDIR 1

/* Define to 1 if chown is declared even after undefining macros. */
#define HAVE_RAW_DECL_CHOWN 1

/* Define to 1 if dprintf is declared even after undefining macros. */
#define HAVE_RAW_DECL_DPRINTF 1

/* Define to 1 if dup is declared even after undefining macros. */
#define HAVE_RAW_DECL_DUP 1

/* Define to 1 if dup2 is declared even after undefining macros. */
#define HAVE_RAW_DECL_DUP2 1

/* Define to 1 if dup3 is declared even after undefining macros. */
#define HAVE_RAW_DECL_DUP3 1

/* Define to 1 if duplocale is declared even after undefining macros. */
#define HAVE_RAW_DECL_DUPLOCALE 1

/* Define to 1 if endusershell is declared even after undefining macros. */
/* #undef HAVE_RAW_DECL_ENDUSERSHELL */

/* Define to 1 if euidaccess is declared even after undefining macros. */
/* #undef HAVE_RAW_DECL_EUIDACCESS */

/* Define to 1 if faccessat is declared even after undefining macros. */
#define HAVE_RAW_DECL_FACCESSAT 1

/* Define to 1 if fchdir is declared even after undefining macros. */
#define HAVE_RAW_DECL_FCHDIR 1

/* Define to 1 if fchmodat is declared even after undefining macros. */
#define HAVE_RAW_DECL_FCHMODAT 1

/* Define to 1 if fchownat is declared even after undefining macros. */
#define HAVE_RAW_DECL_FCHOWNAT 1

/* Define to 1 if fdatasync is declared even after undefining macros. */
#define HAVE_RAW_DECL_FDATASYNC 1

/* Define to 1 if ffsl is declared even after undefining macros. */
/* #undef HAVE_RAW_DECL_FFSL */

/* Define to 1 if ffsll is declared even after undefining macros. */
/* #undef HAVE_RAW_DECL_FFSLL */

/* Define to 1 if fpurge is declared even after undefining macros. */
#define HAVE_RAW_DECL_FPURGE 1

/* Define to 1 if fseeko is declared even after undefining macros. */
#define HAVE_RAW_DECL_FSEEKO 1

/* Define to 1 if fstat is declared even after undefining macros. */
#define HAVE_RAW_DECL_FSTAT 1

/* Define to 1 if fstatat is declared even after undefining macros. */
#define HAVE_RAW_DECL_FSTATAT 1

/* Define to 1 if fsync is declared even after undefining macros. */
#define HAVE_RAW_DECL_FSYNC 1

/* Define to 1 if ftello is declared even after undefining macros. */
#define HAVE_RAW_DECL_FTELLO 1

/* Define to 1 if ftruncate is declared even after undefining macros. */
#define HAVE_RAW_DECL_FTRUNCATE 1

/* Define to 1 if futimens is declared even after undefining macros. */
#define HAVE_RAW_DECL_FUTIMENS 1

/* Define to 1 if getcwd is declared even after undefining macros. */
#define HAVE_RAW_DECL_GETCWD 1

/* Define to 1 if getdelim is declared even after undefining macros. */
#define HAVE_RAW_DECL_GETDELIM 1

/* Define to 1 if getdomainname is declared even after undefining macros. */
/* #undef HAVE_RAW_DECL_GETDOMAINNAME */

/* Define to 1 if getdtablesize is declared even after undefining macros. */
/* #undef HAVE_RAW_DECL_GETDTABLESIZE */


/* Define to 1 if setenv is declared even after undefining macros. */
#define HAVE_RAW_DECL_SETENV 1

/* Define to 1 if sethostname is declared even after undefining macros. */
/* #undef HAVE_RAW_DECL_SETHOSTNAME */

/* Define to 1 if setlocale is declared even after undefining macros. */
#define HAVE_RAW_DECL_SETLOCALE 1

/* Define to 1 if setstate is declared even after undefining macros. */
#define HAVE_RAW_DECL_SETSTATE 1

/* Define to 1 if setstate_r is declared even after undefining macros. */
/* #undef HAVE_RAW_DECL_SETSTATE_R */

/* Define to 1 if setusershell is declared even after undefining macros. */
/* #undef HAVE_RAW_DECL_SETUSERSHELL */

#ifndef _MSC_VER
/* Define to 1 if sigaction is declared even after undefining macros. */
#define HAVE_RAW_DECL_SIGACTION 1
/* Define to 1 if sigaddset is declared even after undefining macros. */
#define HAVE_RAW_DECL_SIGADDSET 1
/* Define to 1 if sigdelset is declared even after undefining macros. */
#define HAVE_RAW_DECL_SIGDELSET 1
/* Define to 1 if sigemptyset is declared even after undefining macros. */
#define HAVE_RAW_DECL_SIGEMPTYSET 1
/* Define to 1 if sigfillset is declared even after undefining macros. */
#define HAVE_RAW_DECL_SIGFILLSET 1
/* Define to 1 if sigismember is declared even after undefining macros. */
#define HAVE_RAW_DECL_SIGISMEMBER 1
/* Define to 1 if sigpending is declared even after undefining macros. */
#define HAVE_RAW_DECL_SIGPENDING 1
/* Define to 1 if sigprocmask is declared even after undefining macros. */
#define HAVE_RAW_DECL_SIGPROCMASK 1
#endif // _MSC_VER

/* Define to 1 if sleep is declared even after undefining macros. */
#define HAVE_RAW_DECL_SLEEP 1

/* Define to 1 if snprintf is declared even after undefining macros. */
#define HAVE_RAW_DECL_SNPRINTF 1

/* Define to 1 if srandom is declared even after undefining macros. */
#define HAVE_RAW_DECL_SRANDOM 1

/* Define to 1 if srandom_r is declared even after undefining macros. */
/* #undef HAVE_RAW_DECL_SRANDOM_R */

/* Define to 1 if stat is declared even after undefining macros. */
#define HAVE_RAW_DECL_STAT 1

/* Define to 1 if stpcpy is declared even after undefining macros. */
#define HAVE_RAW_DECL_STPCPY 1

/* Define to 1 if stpncpy is declared even after undefining macros. */
#define HAVE_RAW_DECL_STPNCPY 1

/* Define to 1 if strcasestr is declared even after undefining macros. */
#define HAVE_RAW_DECL_STRCASESTR 1

/* Define to 1 if strchrnul is declared even after undefining macros. */
/* #undef HAVE_RAW_DECL_STRCHRNUL */

/* Define to 1 if strdup is declared even after undefining macros. */
#define HAVE_RAW_DECL_STRDUP 1

/* Define to 1 if strerror_r is declared even after undefining macros. */
#define HAVE_RAW_DECL_STRERROR_R 1

/* Define to 1 if strncat is declared even after undefining macros. */
#define HAVE_RAW_DECL_STRNCAT 1

/* Define to 1 if strndup is declared even after undefining macros. */
#define HAVE_RAW_DECL_STRNDUP 1

/* Define to 1 if strnlen is declared even after undefining macros. */
#define HAVE_RAW_DECL_STRNLEN 1

/* Define to 1 if strpbrk is declared even after undefining macros. */
#define HAVE_RAW_DECL_STRPBRK 1

/* Define to 1 if strsep is declared even after undefining macros. */
#define HAVE_RAW_DECL_STRSEP 1

/* Define to 1 if strsignal is declared even after undefining macros. */
#define HAVE_RAW_DECL_STRSIGNAL 1

/* Define to 1 if strtod is declared even after undefining macros. */
#define HAVE_RAW_DECL_STRTOD 1

/* Define to 1 if strtok_r is declared even after undefining macros. */
#define HAVE_RAW_DECL_STRTOK_R 1

/* Define to 1 if strtoll is declared even after undefining macros. */
#define HAVE_RAW_DECL_STRTOLL 1

/* Define to 1 if strtoull is declared even after undefining macros. */
#define HAVE_RAW_DECL_STRTOULL 1

/* Define to 1 if strverscmp is declared even after undefining macros. */
/* #undef HAVE_RAW_DECL_STRVERSCMP */

/* Define to 1 if symlink is declared even after undefining macros. */
#define HAVE_RAW_DECL_SYMLINK 1

/* Define to 1 if symlinkat is declared even after undefining macros. */
#define HAVE_RAW_DECL_SYMLINKAT 1

/* Define to 1 if tmpfile is declared even after undefining macros. */
#define HAVE_RAW_DECL_TMPFILE 1

/* Define to 1 if towctrans is declared even after undefining macros. */
#define HAVE_RAW_DECL_TOWCTRANS 1

/* Define to 1 if ttyname_r is declared even after undefining macros. */
#define HAVE_RAW_DECL_TTYNAME_R 1

/* Define to 1 if unlink is declared even after undefining macros. */
#define HAVE_RAW_DECL_UNLINK 1

/* Define to 1 if unlinkat is declared even after undefining macros. */
#define HAVE_RAW_DECL_UNLINKAT 1

/* Define to 1 if unlockpt is declared even after undefining macros. */
#define HAVE_RAW_DECL_UNLOCKPT 1

/* Define to 1 if unsetenv is declared even after undefining macros. */
#define HAVE_RAW_DECL_UNSETENV 1

/* Define to 1 if usleep is declared even after undefining macros. */
#define HAVE_RAW_DECL_USLEEP 1

/* Define to 1 if utimensat is declared even after undefining macros. */
#define HAVE_RAW_DECL_UTIMENSAT 1

/* Define to 1 if vdprintf is declared even after undefining macros. */
#define HAVE_RAW_DECL_VDPRINTF 1

/* Define to 1 if vsnprintf is declared even after undefining macros. */
#define HAVE_RAW_DECL_VSNPRINTF 1

/* Define to 1 if wcpcpy is declared even after undefining macros. */
/* #undef HAVE_RAW_DECL_WCPCPY */

/* Define to 1 if wcpncpy is declared even after undefining macros. */
/* #undef HAVE_RAW_DECL_WCPNCPY */

/* Define to 1 if wcrtomb is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCRTOMB 1

/* Define to 1 if wcscasecmp is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCSCASECMP 1

/* Define to 1 if wcscat is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCSCAT 1

/* Define to 1 if wcschr is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCSCHR 1

/* Define to 1 if wcscmp is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCSCMP 1

/* Define to 1 if wcscoll is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCSCOLL 1

/* Define to 1 if wcscpy is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCSCPY 1

/* Define to 1 if wcscspn is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCSCSPN 1

/* Define to 1 if wcsdup is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCSDUP 1

/* Define to 1 if wcslen is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCSLEN 1

/* Define to 1 if wcsncasecmp is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCSNCASECMP 1

/* Define to 1 if wcsncat is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCSNCAT 1

/* Define to 1 if wcsncmp is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCSNCMP 1

/* Define to 1 if wcsncpy is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCSNCPY 1

/* Define to 1 if wcsnlen is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCSNLEN 1

/* Define to 1 if wcsnrtombs is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCSNRTOMBS 1

/* Define to 1 if wcspbrk is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCSPBRK 1

/* Define to 1 if wcsrchr is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCSRCHR 1

/* Define to 1 if wcsrtombs is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCSRTOMBS 1

/* Define to 1 if wcsspn is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCSSPN 1

/* Define to 1 if wcsstr is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCSSTR 1

/* Define to 1 if wcstok is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCSTOK 1

/* Define to 1 if wcswidth is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCSWIDTH 1

/* Define to 1 if wcsxfrm is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCSXFRM 1

/* Define to 1 if wctob is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCTOB 1

/* Define to 1 if wctrans is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCTRANS 1

/* Define to 1 if wctype is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCTYPE 1

/* Define to 1 if wcwidth is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCWIDTH 1

/* Define to 1 if wmemchr is declared even after undefining macros. */
#define HAVE_RAW_DECL_WMEMCHR 1

/* Define to 1 if wmemcmp is declared even after undefining macros. */
#define HAVE_RAW_DECL_WMEMCMP 1

/* Define to 1 if wmemcpy is declared even after undefining macros. */
#define HAVE_RAW_DECL_WMEMCPY 1

/* Define to 1 if wmemmove is declared even after undefining macros. */
#define HAVE_RAW_DECL_WMEMMOVE 1

/* Define to 1 if wmemset is declared even after undefining macros. */
#define HAVE_RAW_DECL_WMEMSET 1

/* Define to 1 if _Exit is declared even after undefining macros. */
#define HAVE_RAW_DECL__EXIT 1

/* Define to 1 if 'sig_atomic_t' is a signed integer type. */
/* #undef HAVE_SIGNED_SIG_ATOMIC_T */

/* Define to 1 if 'wchar_t' is a signed integer type. */
/* #undef HAVE_SIGNED_WCHAR_T */

/* Define to 1 if 'wint_t' is a signed integer type. */
/* #undef HAVE_SIGNED_WINT_T */

/* Define to 1 if you have the `snprintf' function. */
#define HAVE_SNPRINTF 1

/* Define to 1 if you have the <stddef.h> header file. */
#define HAVE_STDDEF_H 1

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define if <stdint.h> exists, doesn't clash with <sys/types.h>, and declares
uintmax_t. */
#define HAVE_STDINT_H_WITH_UINTMAX 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the `stpcpy' function. */
#define HAVE_STPCPY 1

/* Define to 1 if you have the `strcasecmp' function. */
#define HAVE_STRCASECMP 1

/* Define to 1 if you have the `strdup' function. */
#define HAVE_STRDUP 1


/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the `strnlen' function. */
#define HAVE_STRNLEN 1

/* Define to 1 if you have the `strtoul' function. */
#define HAVE_STRTOUL 1

/* Define to 1 if you have the <sys/bitypes.h> header file. */
/* #undef HAVE_SYS_BITYPES_H */

/* Define to 1 if you have the <sys/inttypes.h> header file. */
/* #undef HAVE_SYS_INTTYPES_H */

/* Define to 1 if you have the <sys/socket.h> header file. */
//#define HAVE_SYS_SOCKET_H 1 // defined in NlcCompilerConfig.h

/* Define to 1 if you have the <sys/timeb.h> header file. */
/* #undef HAVE_SYS_TIMEB_H */

/* Define to 1 if you have the <sys/time.h> header file. */
#define HAVE_SYS_TIME_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the `towlower' function. */
#define HAVE_TOWLOWER 1

/* Define if you have the 'uintmax_t' type in <stdint.h> or <inttypes.h>. */
#define HAVE_UINTMAX_T 1
/* Define to 1 if the system has the type `uint16_t'. */
#define HAVE_UINT16_T 1

/* Define to 1 if the system has the type `uint32_t'. */
#define HAVE_UINT32_T 1

/* Define to 1 if the system has the type `uint8_t'. */
#define HAVE_UINT8_T 1

/* Define to 1 if the system has the type `uintptr_t'. */
#define HAVE_UINTPTR_T 1

/* Define to 1 if the system has the type `uint_t'. */
#define HAVE_UINT_T 1

/* Define to 1 if you have the <wchar_gnu.h> header file. */
#define HAVE_WCHAR_H 1

/* Define if you have the 'wchar_t' type. */
#define HAVE_WCHAR_T 1

/* Define to 1 if you have the `wcrtomb' function. */
#define HAVE_WCRTOMB 1

/* Define to 1 if you have the `wcslen' function. */
#define HAVE_WCSLEN 1

/* Define to 1 if you have the `wcsnlen' function. */
#define HAVE_WCSNLEN 1

/* Define to 1 if you have the <wctype.h> header file. */
#define HAVE_WCTYPE_H 1

/* Define to 1 if you have the `wcwidth' function. */
#define HAVE_WCWIDTH 1

/* Define to 1 if you have the <winsock2.h> header file. */
/* #undef HAVE_WINSOCK2_H */

/* Define if you have the 'wint_t' type. */
//#define HAVE_WINT_T 1

/* Define to 1 if O_NOATIME works. */
#define HAVE_WORKING_O_NOATIME 0

/* Define to 1 if O_NOFOLLOW works. */
#define HAVE_WORKING_O_NOFOLLOW 0

/* Define to 1 if you have the <xlocale.h> header file. */
#define HAVE_XLOCALE_H 1

/* Define to 1 if the system has the type `_Bool'. */
#define HAVE__BOOL 1

/* Define to 1 if you have the `_ftime' function. */
/* #undef HAVE__FTIME */

/* Define to 1 if you have the `_NSGetExecutablePath' function. */
/* #undef HAVE__NSGETEXECUTABLEPATH */

/* Define to 1 if you have the `_set_invalid_parameter_handler' function. */
/* #undef HAVE__SET_INVALID_PARAMETER_HANDLER */

/* Define to 1 if you have the `__fsetlocking' function. */
/* #undef HAVE___FSETLOCKING */

/* Define as const if the declaration of iconv() needs const. */
#define ICONV_CONST 

/* Define to a symbolic name denoting the flavor of iconv_open()
implementation. */
/* #undef ICONV_FLAVOR */

/* Define to the value of ${prefix}, as a string. */
#define INSTALLPREFIX "//home/jack/xbmc-depends/arm-linux-androideabi-android-21"

/* Define if integer division by zero raises signal SIGFPE. */
#define INTDIV0_RAISES_SIGFPE 0

/* Define to 1 if 'lstat' dereferences a symlink specified with a trailing
slash. */
/* #undef LSTAT_FOLLOWS_SLASHED_SYMLINK */

/* Define to the sub-directory where libtool stores uninstalled libraries. */
#define LT_OBJDIR ".libs/"

/* If malloc(0) is != NULL, define this to 1. Otherwise define this to 0. */
#define MALLOC_0_IS_NONNULL 1


/* Define if the pthread_in_use() detection is hard. */
#define PTHREAD_IN_USE_DETECTION_HARD		1


/* Define to 1 if readlink fails to recognize a trailing slash. */
#define READLINK_TRAILING_SLASH_BUG 1


/* Define to 1 if strerror(0) does not return a message implying success. */
//#define REPLACE_STRERROR_0 0

/* Define to l, ll, u, ul, ull, etc., as suitable for constants of type
'sig_atomic_t'. */
/* #undef SIG_ATOMIC_T_SUFFIX */

/* Define to the prefix of C symbols at the assembler and linker level, either
an underscore or empty. */
#define USER_LABEL_PREFIX 

/* Define if the POSIX multithreading library can be used. */
#define USE_POSIX_THREADS 1

/* Define if references to the POSIX multithreading library should be made
weak. */
/* #undef USE_POSIX_THREADS_WEAK */

/* Define if the GNU Pth multithreading library can be used. */
/* #undef USE_PTH_THREADS */

/* Define if references to the GNU Pth multithreading library should be made
weak. */
/* #undef USE_PTH_THREADS_WEAK */

/* Define if the old Solaris multithreading library can be used. */
/* #undef USE_SOLARIS_THREADS */

/* Define if references to the old Solaris multithreading library should be
made weak. */
/* #undef USE_SOLARIS_THREADS_WEAK */

/* Enable extensions on AIX 3, Interix.  */
#ifndef _ALL_SOURCE
# define _ALL_SOURCE 1
#endif
/* Enable general extensions on OS X.  */
#ifndef _DARWIN_C_SOURCE
# define _DARWIN_C_SOURCE 1
#endif
/* Enable GNU extensions on systems that have them.  */
#ifndef _GNU_SOURCE
# define _GNU_SOURCE 1
#endif
/* Use GNU style printf and scanf.  */
#ifndef __USE_MINGW_ANSI_STDIO
# define __USE_MINGW_ANSI_STDIO 1
#endif
/* Enable threading extensions on Solaris.  */
#ifndef _POSIX_PTHREAD_SEMANTICS
# define _POSIX_PTHREAD_SEMANTICS 1
#endif
/* Enable extensions on HP NonStop.  */
#ifndef _TANDEM_SOURCE
# define _TANDEM_SOURCE 1
#endif
/* Enable X/Open extensions if necessary.  HP-UX 11.11 defines
mbstate_t only if _XOPEN_SOURCE is defined to 500, regardless of
whether compiling with -Ae or -D_HPUX_SOURCE=1.  */
#ifndef _XOPEN_SOURCE
/* # undef _XOPEN_SOURCE */
#endif
/* Enable general extensions on Solaris.  */
#ifndef __EXTENSIONS__
# define __EXTENSIONS__ 1
#endif

/* Define to l, ll, u, ul, ull, etc., as suitable for constants of type
'wchar_t'. */
/* #undef WCHAR_T_SUFFIX */

/* Define to l, ll, u, ul, ull, etc., as suitable for constants of type
'wint_t'. */
/* #undef WINT_T_SUFFIX */

/* Define when --enable-shared is used on mingw or Cygwin. */
/* #undef WOE32DLL */

/* Enable large inode numbers on Mac OS X 10.5. */
#define _DARWIN_USE_64_BIT_INODE 1

/* Number of bits in a file offset, on hosts where this is settable. */
/* #undef _FILE_OFFSET_BITS */

/* Define to 1 if Gnulib overrides 'struct stat' on Windows so that struct
stat.st_size becomes 64-bit. */
/* #undef _GL_WINDOWS_64_BIT_ST_SIZE */

/* Define for large files, on AIX-style hosts. */
/* #undef _LARGE_FILES */

/* Define to 1 on Solaris. */
/* #undef _LCONV_C99 */

/* Define to 1 if on MINIX. */
/* #undef _MINIX */

/* Define to 1 to make NetBSD features available. MINIX 3 needs this. */
/* #undef _NETBSD_SOURCE */

/* The _Noreturn keyword of C11.  */
#if ! (defined _Noreturn \
       || (defined __STDC_VERSION__ && 201112 <= __STDC_VERSION__))
# if (3 <= __GNUC__ || (__GNUC__ == 2 && 8 <= __GNUC_MINOR__) \
      || 0x5110 <= __SUNPRO_C)
#  define _Noreturn __attribute__ ((__noreturn__))
# elif defined _MSC_VER 
#  define _Noreturn
# endif
#endif


/* Define to 2 if the system does not provide POSIX.1 features except with
this defined. */
/* #undef _POSIX_1_SOURCE */

/* Define to 1 if you need to in order for 'stat' and other things to work. */
/* #undef _POSIX_SOURCE */

/* Define to rpl_ if the getopt replacement functions and variables should be
used. */
#define __GETOPT_PREFIX rpl_

/* Define to 1 if the system <stdint.h> predates C++11. */
/* #undef __STDC_CONSTANT_MACROS */

/* Define to 1 if the system <stdint.h> predates C++11. */
/* #undef __STDC_LIMIT_MACROS */

/* Please see the Gnulib manual for how to use these macros.

Suppress extern NLC_INLINE with HP-UX cc, as it appears to be broken; see
<http://lists.gnu.org/archive/html/bug-texinfo/2013-02/msg00030.html>.

Suppress extern NLC_INLINE with Sun C in standards-conformance mode, as it
mishandles NLC_INLINE functions that call each other.  E.g., for 'NLC_INLINE void f
(void) { } NLC_INLINE void g (void) { f (); }', c99 incorrectly complains
'reference to static identifier "f" in extern NLC_INLINE function'.
This bug was observed with Sun C 5.12 SunOS_i386 2011/11/16.

Suppress extern NLC_INLINE (with or without __attribute__ ((__gnu_inline__)))
on configurations that mistakenly use 'static NLC_INLINE' to implement
functions or macros in standard C headers like <ctype.h>.  For example,
if isdigit is mistakenly implemented via a static NLC_INLINE function,
a program containing an extern NLC_INLINE function that calls isdigit
may not work since the C standard prohibits extern NLC_INLINE functions
from calling static functions.  This bug is known to occur on:

OS X 10.8 and earlier; see:
http://lists.gnu.org/archive/html/bug-gnulib/2012-12/msg00023.html

DragonFly; see
http://muscles.dragonflybsd.org/bulk/bleeding-edge-potential/latest-per-pkg/ah-tty-0.3.12.log

FreeBSD; see:
http://lists.gnu.org/archive/html/bug-gnulib/2014-07/msg00104.html

OS X 10.9 has a macro __header_inline indicating the bug is fixed for C and
for clang but remains for g++; see <http://trac.macports.org/ticket/41033>.
Assume DragonFly and FreeBSD will be similar.  */
#if (((defined __APPLE__ && defined __MACH__) \
      || defined __DragonFly__ || defined __FreeBSD__) \
     && (defined __header_inline \
         ? (defined __cplusplus && defined __GNUC_STDC_INLINE__ \
            && ! defined __clang__) \
         : ((! defined _DONT_USE_CTYPE_INLINE_ \
             && (defined __GNUC__ || defined __cplusplus)) \
            || (defined _FORTIFY_SOURCE && 0 < _FORTIFY_SOURCE \
                && defined __GNUC__ && ! defined __cplusplus))))
# define _GL_EXTERN_INLINE_STDHEADER_BUG
#endif

# define _GL_INLINE NLC_INLINE



/* In GCC 4.6 (inclusive) to 5.1 (exclusive),
suppress bogus "no previous prototype for 'FOO'"
and "no previous declaration for 'FOO'" diagnostics,
when FOO is an NLC_INLINE function in the header; see
<https://gcc.gnu.org/bugzilla/show_bug.cgi?id=54113> and
<https://gcc.gnu.org/bugzilla/show_bug.cgi?id=63877>.  */
#if __GNUC__ == 4 && 6 <= __GNUC_MINOR__
# if defined __GNUC_STDC_INLINE__ && __GNUC_STDC_INLINE__
#  define _GL_INLINE_HEADER_CONST_PRAGMA
# else
#  define _GL_INLINE_HEADER_CONST_PRAGMA \
     _Pragma ("GCC diagnostic ignored \"-Wsuggest-attribute=const\"")
# endif
# define _GL_INLINE_HEADER_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wmissing-prototypes\"") \
    _Pragma ("GCC diagnostic ignored \"-Wmissing-declarations\"") \
    _GL_INLINE_HEADER_CONST_PRAGMA
# define _GL_INLINE_HEADER_END \
    _Pragma ("GCC diagnostic pop")
#else
# define _GL_INLINE_HEADER_BEGIN
# define _GL_INLINE_HEADER_END
#endif

/* Define to `int' if <sys/types.h> doesn't define. */
/* #undef gid_t */


/* Define to `__inline__' or `__inline' if that's what the C compiler
calls it, or to nothing if 'NLC_INLINE' is not supported under any name.  */
#ifndef __cplusplus
/* #undef NLC_INLINE */
#endif

/* Work around a bug in Apple GCC 4.0.1 build 5465: In C99 mode, it supports
the ISO C 99 semantics of 'extern NLC_INLINE' (unlike the GNU C semantics of
earlier versions), but does not display it by setting __GNUC_STDC_INLINE__.
__APPLE__ && __MACH__ test for Mac OS X.
__APPLE_CC__ tests for the Apple compiler and its version.
__STDC_VERSION__ tests for the C99 mode.  */
#if defined __APPLE__ && defined __MACH__ && __APPLE_CC__ >= 5465 && !defined __cplusplus && __STDC_VERSION__ >= 199901L && !defined __GNUC_STDC_INLINE__
# define __GNUC_STDC_INLINE__ 1
#endif

/* Define to a type if <wchar_gnu.h> does not define. */
/* #undef mbstate_t */

/* Define to `int' if <sys/types.h> does not define. */
/* #undef mode_t */

/* Define to the type of st_nlink in struct stat, or a supertype. */
/* #undef nlink_t */

/* Define to `int' if <sys/types.h> does not define. */
/* #undef pid_t */

/* Define as the type of the result of subtracting two pointers, if the system
doesn't define it. */
/* #undef ptrdiff_t */

/* Define to the equivalent of the C99 'restrict' keyword, or to
nothing if this is not supported.  Do not define if NLC_RESTRICT is
supported directly.  */
//#define NLC_RESTRICT __restrict // defined in NlcDependLibrariesConfig.h

/* Work around a bug in Sun C++: it does not support _Restrict or
__restrict__, even though the corresponding Sun C compiler ends up with
"#define NLC_RESTRICT _Restrict" or "#define NLC_RESTRICT __restrict__" in the
previous line.  Perhaps some future version of Sun C++ will work with
NLC_RESTRICT; if so, hopefully it defines __RESTRICT like Sun C does.  */
#if defined __SUNPRO_CC && !defined __RESTRICT
# define _Restrict
# define __restrict__
#endif

/* Define to `unsigned int' if <sys/types.h> does not define. */
/* #undef size_t */

/* Define as a signed type of the same size as size_t. */
/* #undef ssize_t */

/* Define to `int' if <sys/types.h> doesn't define. */
/* #undef uid_t */

/* Define to unsigned long or unsigned long long if <stdint.h> and
<inttypes.h> don't define. */
/* #undef uintmax_t */

/* Define as a marker that can be attached to declarations that might not
be used.  This helps to reduce warnings, such as from
GCC -Wunused-parameter.  */
#define _GL_UNUSED				NLC_UNUSED							

/* The name _UNUSED_PARAMETER_ is an earlier spelling, although the name
is a misnomer outside of parameter lists.  */
#define  _UNUSED_PARAMETER_		NLC_UNUSED				

/* gcc supports the "unused" attribute on possibly unused labels, and
g++ has since version 4.5.  Note to support C++ as well as C,
_GL_UNUSED_LABEL should be used with a trailing ;  */
#if !defined __cplusplus || __GNUC__ > 4 \
    || (__GNUC__ == 4 && __GNUC_MINOR__ >= 5)
# define _GL_UNUSED_LABEL _GL_UNUSED
#else
# define _GL_UNUSED_LABEL
#endif

# define _GL_ATTRIBUTE_PURE				NLC_ATTRIBUTE_PURE
# define _GL_ATTRIBUTE_CONST			NLC_ATTRIBUTE_CONST


#define __libc_lock_t                   gl_lock_t
#define __libc_lock_define              gl_lock_define
#define __libc_lock_define_initialized  gl_lock_define_initialized
#define __libc_lock_init                gl_lock_init
#define __libc_lock_lock                gl_lock_lock
#define __libc_lock_unlock              gl_lock_unlock
#define __libc_lock_recursive_t                   gl_recursive_lock_t
#define __libc_lock_define_recursive              gl_recursive_lock_define
#define __libc_lock_define_initialized_recursive  gl_recursive_lock_define_initialized
#define __libc_lock_init_recursive                gl_recursive_lock_init
#define __libc_lock_lock_recursive                gl_recursive_lock_lock
#define __libc_lock_unlock_recursive              gl_recursive_lock_unlock
#define glthread_in_use  libintl_thread_in_use
#define glthread_lock_init_func     libintl_lock_init_func
#define glthread_lock_lock_func     libintl_lock_lock_func
#define glthread_lock_unlock_func   libintl_lock_unlock_func
#define glthread_lock_destroy_func  libintl_lock_destroy_func
#define glthread_rwlock_init_multithreaded     libintl_rwlock_init_multithreaded
#define glthread_rwlock_init_func              libintl_rwlock_init_func
#define glthread_rwlock_rdlock_multithreaded   libintl_rwlock_rdlock_multithreaded
#define glthread_rwlock_rdlock_func            libintl_rwlock_rdlock_func
#define glthread_rwlock_wrlock_multithreaded   libintl_rwlock_wrlock_multithreaded
#define glthread_rwlock_wrlock_func            libintl_rwlock_wrlock_func
#define glthread_rwlock_unlock_multithreaded   libintl_rwlock_unlock_multithreaded
#define glthread_rwlock_unlock_func            libintl_rwlock_unlock_func
#define glthread_rwlock_destroy_multithreaded  libintl_rwlock_destroy_multithreaded
#define glthread_rwlock_destroy_func           libintl_rwlock_destroy_func
#define glthread_recursive_lock_init_multithreaded     libintl_recursive_lock_init_multithreaded
#define glthread_recursive_lock_init_func              libintl_recursive_lock_init_func
#define glthread_recursive_lock_lock_multithreaded     libintl_recursive_lock_lock_multithreaded
#define glthread_recursive_lock_lock_func              libintl_recursive_lock_lock_func
#define glthread_recursive_lock_unlock_multithreaded   libintl_recursive_lock_unlock_multithreaded
#define glthread_recursive_lock_unlock_func            libintl_recursive_lock_unlock_func
#define glthread_recursive_lock_destroy_multithreaded  libintl_recursive_lock_destroy_multithreaded
#define glthread_recursive_lock_destroy_func           libintl_recursive_lock_destroy_func
#define glthread_once_func            libintl_once_func
#define glthread_once_singlethreaded  libintl_once_singlethreaded
#define glthread_once_multithreaded   libintl_once_multithreaded



/* On Windows, variables that may be in a DLL must be marked specially.  */
# define NLC_DLL_VARIABLE


/* Default value for alignment of strings in .mo file.  */
#define DEFAULT_OUTPUT_ALIGNMENT 1


/* Define this to an absolute name of
<libcroco-0.6/libcroco/libcroco-config.h>. */
/* #undef ABSOLUTE_LIBCROCO_0_6_LIBCROCO_LIBCROCO_CONFIG_H */

/* Define this to an absolute name of <libxml2/libxml/xmlexports.h>. */
/* #undef ABSOLUTE_LIBXML2_LIBXML_XMLEXPORTS_H */

/* Define this to an absolute name of <libxml2/libxml/xmlversion.h>. */
/* #undef ABSOLUTE_LIBXML2_LIBXML_XMLVERSION_H */



/* Define as the bit index in the word where to find bit 0 of the exponent of
'double'. */
/* #undef DBL_EXPBIT0_BIT */

/* Define as the word index where to find the exponent of 'double'. */
/* #undef DBL_EXPBIT0_WORD */

/* Define as the bit index in the word where to find the sign of 'double'. */
/* #undef DBL_SIGNBIT_BIT */

/* Define as the word index where to find the sign of 'double'. */
/* #undef DBL_SIGNBIT_WORD */

/* the name of the file descriptor member of DIR */
/* #undef DIR_FD_MEMBER_NAME */

#ifdef DIR_FD_MEMBER_NAME
# define DIR_TO_FD(Dir_p) ((Dir_p)->DIR_FD_MEMBER_NAME)
#else
# define DIR_TO_FD(Dir_p) -1
#endif


/* Define to 1 if // is a file system root distinct from /. */
/* #undef DOUBLE_SLASH_IS_DISTINCT_ROOT */

/* Define to 1 if translation of program messages to the user's native
language is requested. */
//#define ENABLE_NLS 1

/* Define to 1 if the package shall run at any location in the file system. */
/* #undef ENABLE_RELOCATABLE */

/* Define according to the byte order of the target machine: 1 for big endian,
0 for little endian. */
#define ENDIANNESS 0

/* Define this to 1 if F_DUPFD behavior does not match POSIX */
/* #undef FCNTL_DUPFD_BUGGY */

/* Define as the bit index in the word where to find bit 0 of the exponent of
'float'. */
#define FLT_EXPBIT0_BIT 23

/* Define as the word index where to find the exponent of 'float'. */
#define FLT_EXPBIT0_WORD 0

/* Define as the bit index in the word where to find the sign of 'float'. */
/* #undef FLT_SIGNBIT_BIT */

/* Define as the word index where to find the sign of 'float'. */
/* #undef FLT_SIGNBIT_WORD */

/* Define to 1 if fopen() fails to recognize a trailing slash. */
/* #undef FOPEN_TRAILING_SLASH_BUG */

/* Define to 1 if the system's ftello function has the Solaris bug. */
/* #undef FTELLO_BROKEN_AFTER_SWITCHING_FROM_READ_TO_WRITE */

/* Define to 1 if nl_langinfo (YESEXPR) returns a non-empty string. */
/* #undef FUNC_NL_LANGINFO_YESEXPR_WORKS */

/* Define to 1 if realpath() can malloc memory, always gives an absolute path,
and handles trailing slash correctly. */
/* #undef FUNC_REALPATH_WORKS */

/* Define to 1 if ungetc is broken when used on arbitrary bytes. */
/* #undef FUNC_UNGETC_BROKEN */

/* Define if gettimeofday clobbers the localtime buffer. */
/* #undef GETTIMEOFDAY_CLOBBERS_LOCALTIME */

/* Define this to 'void' or 'struct timezone' to match the system's
declaration of the second argument to gettimeofday. */
#define GETTIMEOFDAY_TIMEZONE struct timezone

/* Define to 1 if getxattr works with XATTR_NAME_POSIX_ACL_ACCESS and
XATTR_NAME_POSIX_ACL_DEFAULT. */
#define GETXATTR_WITH_POSIX_ACLS 1

/* Define to a C preprocessor expression that evaluates to 1 or 0, depending
whether the gnulib module canonicalize-lgpl shall be considered present. */
#define GNULIB_CANONICALIZE_LGPL 1

/* Define to a C preprocessor expression that evaluates to 1 or 0, depending
whether the gnulib module fd-safer-flag shall be considered present. */
#define GNULIB_FD_SAFER_FLAG 1

/* Define to a C preprocessor expression that evaluates to 1 or 0, depending
whether the gnulib module fscanf shall be considered present. */
#define GNULIB_FSCANF 1

/* Define to a C preprocessor expression that evaluates to 1 or 0, depending
whether the gnulib module fwriteerror shall be considered present. */
#define GNULIB_FWRITEERROR 1

/* Define to a C preprocessor expression that evaluates to 1 or 0, depending
whether the gnulib module lock shall be considered present. */
#define GNULIB_LOCK 1

/* Define to a C preprocessor expression that evaluates to 1 or 0, depending
whether the gnulib module pipe2-safer shall be considered present. */
#define GNULIB_PIPE2_SAFER 1

/* Define to 1 if printf and friends should be labeled with attribute
"__gnu_printf__" instead of "__printf__" */
/* #undef GNULIB_PRINTF_ATTRIBUTE_FLAVOR_GNU */

/* Define when the error facility is replaced by gnulib. */
#define GNULIB_REPLACE_ERROR 1

/* Define to a C preprocessor expression that evaluates to 1 or 0, depending
whether the gnulib module scanf shall be considered present. */
#define GNULIB_SCANF 1

/* Define to a C preprocessor expression that evaluates to 1 or 0, depending
whether the gnulib module sigpipe shall be considered present. */
#ifndef TARGET_OS_WINDOWS
# define GNULIB_SIGPIPE 1
#endif // TARGET_OS_WINDOWS

/* Define to a C preprocessor expression that evaluates to 1 or 0, depending
whether the gnulib module snprintf shall be considered present. */
#define GNULIB_SNPRINTF 1

/* Define to a C preprocessor expression that evaluates to 1 or 0, depending
whether the gnulib module strerror shall be considered present. */
#define GNULIB_STRERROR 1

/* Define to a C preprocessor expression that evaluates to 1 or 0, depending
whether the gnulib module unistr/uint16_t-mbtouc shall be considered present. */
#define GNULIB_UNISTR_U16_MBTOUC 1

/* Define to a C preprocessor expression that evaluates to 1 or 0, depending
whether the gnulib module unistr/u8-mbtouc shall be considered present. */
#define GNULIB_UNISTR_U8_MBTOUC 1

/* Define to a C preprocessor expression that evaluates to 1 or 0, depending
whether the gnulib module unistr/u8-mbtoucr shall be considered present. */
#define GNULIB_UNISTR_U8_MBTOUCR 1

/* Define to a C preprocessor expression that evaluates to 1 or 0, depending
whether the gnulib module unistr/u8-mbtouc-unsafe shall be considered
present. */
#define GNULIB_UNISTR_U8_MBTOUC_UNSAFE 1

/* Define to a C preprocessor expression that evaluates to 1 or 0, depending
whether the gnulib module unistr/u8-uctomb shall be considered present. */
#define GNULIB_UNISTR_U8_UCTOMB 1

/* Define to 1 if you have the `aclsort' function. */
/* #undef HAVE_ACLSORT */

/* Define to 1 if you have the <aclv.h> header file. */
/* #undef HAVE_ACLV_H */

/* Define to 1 if you have the `aclx_get' function. */
/* #undef HAVE_ACLX_GET */

/* Define to 1 if you have the `acl_copy_ext_native' function. */
/* #undef HAVE_ACL_COPY_EXT_NATIVE */

/* Define to 1 if you have the `acl_create_entry_np' function. */
/* #undef HAVE_ACL_CREATE_ENTRY_NP */

/* Define to 1 if you have the `acl_delete_def_file' function. */
/* #undef HAVE_ACL_DELETE_DEF_FILE */

/* Define to 1 if you have the `acl_delete_fd_np' function. */
/* #undef HAVE_ACL_DELETE_FD_NP */

/* Define to 1 if you have the `acl_delete_file_np' function. */
/* #undef HAVE_ACL_DELETE_FILE_NP */

/* Define to 1 if you have the `acl_entries' function. */
/* #undef HAVE_ACL_ENTRIES */

/* Define to 1 if you have the `acl_extended_file' function. */
/* #undef HAVE_ACL_EXTENDED_FILE */

/* Define to 1 if the constant ACL_FIRST_ENTRY exists. */
/* #undef HAVE_ACL_FIRST_ENTRY */

/* Define to 1 if you have the `acl_free' function. */
/* #undef HAVE_ACL_FREE */

/* Define to 1 if you have the `acl_free_text' function. */
/* #undef HAVE_ACL_FREE_TEXT */

/* Define to 1 if you have the `acl_from_mode' function. */
/* #undef HAVE_ACL_FROM_MODE */

/* Define to 1 if you have the `acl_from_text' function. */
/* #undef HAVE_ACL_FROM_TEXT */

/* Define to 1 if you have the `acl_get_fd' function. */
/* #undef HAVE_ACL_GET_FD */

/* Define to 1 if you have the `acl_get_file' function. */
/* #undef HAVE_ACL_GET_FILE */

/* Define to 1 if you have the <acl/libacl.h> header file. */
/* #undef HAVE_ACL_LIBACL_H */

/* Define to 1 if you have the `acl_set_fd' function. */
/* #undef HAVE_ACL_SET_FD */

/* Define to 1 if you have the `acl_set_file' function. */
/* #undef HAVE_ACL_SET_FILE */

/* Define to 1 if you have the `acl_to_short_text' function. */
/* #undef HAVE_ACL_TO_SHORT_TEXT */

/* Define to 1 if you have the `acl_trivial' function. */
/* #undef HAVE_ACL_TRIVIAL */

/* Define to 1 if the ACL type ACL_TYPE_EXTENDED exists. */
/* #undef HAVE_ACL_TYPE_EXTENDED */

/* Define to 1 if you have `alloca', as a function or macro. */
#define HAVE_ALLOCA 1

/* Define to 1 if you have <libgnu/alloca_gnu.h> and it should be used (not on Ultrix).
*/
#define HAVE_ALLOCA_H 1

/* Define to 1 if you have the `argz_count' function. */
/* #undef HAVE_ARGZ_COUNT */

/* Define to 1 if you have the <argz.h> header file. */
/* #undef HAVE_ARGZ_H */

/* Define to 1 if you have the `argz_next' function. */
/* #undef HAVE_ARGZ_NEXT */

/* Define to 1 if you have the `argz_stringify' function. */
/* #undef HAVE_ARGZ_STRINGIFY */


/* Define to 1 if you have the <arpa/nameser.h> header file. else do not define */
#ifndef TARGET_OS_WINDOWS
# define HAVE_ARPA_NAMESER_H		1
#endif // TARGET_OS_WINDOWS

/* Define to 1 if you have the `asprintf' function. */
#define HAVE_ASPRINTF 1

/* Define to 1 if you have the <bp-sym.h> header file. */
/* #undef HAVE_BP_SYM_H */

/* Define to 1 if you have the `btowc' function. */
#define HAVE_BTOWC 1


/* Define to 1 if you have the <byteswap.h> header file. */
#define HAVE_BYTESWAP_H 1

/* Define to 1 if you have the `canonicalize_file_name' function. */
/* #undef HAVE_CANONICALIZE_FILE_NAME */

/* Define to 1 if you have the Mac OS X function CFLocaleCopyCurrent in the
CoreFoundation framework. */
/* #undef HAVE_CFLOCALECOPYCURRENT */

/* Define to 1 if you have the Mac OS X function CFPreferencesCopyAppValue in
the CoreFoundation framework. */
/* #undef HAVE_CFPREFERENCESCOPYAPPVALUE */


/* Define to 1 if you have the `closedir' function. */
#define HAVE_CLOSEDIR 1

/* Define to 1 if you have the `confstr' function. */
/* #undef HAVE_CONFSTR */

/* Define if the copysignf function is declared in <math.h> and available in
libc. */
#define HAVE_COPYSIGNF_IN_LIBC 1

/* Define if the copysignl function is declared in <math.h> and available in
libc. */
#define HAVE_COPYSIGNL_IN_LIBC 1

/* Define if the copysign function is declared in <math.h> and available in
libc. */
#define HAVE_COPYSIGN_IN_LIBC 1

/* Define to 1 if you have the <ctype.h> header file. */
#define HAVE_CTYPE_H 1

/* Define if the GNU dcgettext() function is already present or preinstalled.
*/
/* #undef HAVE_DCGETTEXT */

/* Define to 1 if you have the declaration of `alarm', and to 0 if you don't.
*/
#define HAVE_DECL_ALARM 1

/* Define to 1 if you have the declaration of `clearerr_unlocked', and to 0 if
you don't. */
//#define HAVE_DECL_CLEARERR_UNLOCKED 0

/* Define to 1 if you have the declaration of `copysign', and to 0 if you
don't. */
#define HAVE_DECL_COPYSIGN 1

/* Define to 1 if you have the declaration of `copysignf', and to 0 if you
don't. */
#define HAVE_DECL_COPYSIGNF 1

/* Define to 1 if you have the declaration of `copysignl', and to 0 if you
don't. */
#define HAVE_DECL_COPYSIGNL 1

/* Define to 1 if you have the declaration of `dirfd', and to 0 if you don't.
*/
#define HAVE_DECL_DIRFD 1

/* Define to 1 if you have the declaration of `feof_unlocked', and to 0 if you
don't. */
//#define HAVE_DECL_FEOF_UNLOCKED 0 // defined in NlcDependLibrariesConfig.h

/* Define to 1 if you have the declaration of `ferror_unlocked', and to 0 if
you don't. */
//#define HAVE_DECL_FERROR_UNLOCKED 0 // defined in NlcDependLibrariesConfig.h

/* Define to 1 if you have the declaration of `fflush_unlocked', and to 0 if
you don't. */
//#define HAVE_DECL_FFLUSH_UNLOCKED 0 // defined in NlcDependLibrariesConfig.h

/* Define to 1 if you have the declaration of `fgets_unlocked', and to 0 if
you don't. */
//#define HAVE_DECL_FGETS_UNLOCKED 0 // defined in NlcDependLibrariesConfig.h

/* Define to 1 if you have the declaration of `fputc_unlocked', and to 0 if
you don't. */
//#define HAVE_DECL_FPUTC_UNLOCKED 0 // defined in NlcDependLibrariesConfig.h

/* Define to 1 if you have the declaration of `fputs_unlocked', and to 0 if
you don't. */
//#define HAVE_DECL_FPUTS_UNLOCKED 0 // defined in NlcDependLibrariesConfig.h

/* Define to 1 if you have the declaration of `fread_unlocked', and to 0 if
you don't. */
//#define HAVE_DECL_FREAD_UNLOCKED 0 // defined in NlcDependLibrariesConfig.h

/* Define to 1 if you have the declaration of `ftello', and to 0 if you don't.
*/
#define HAVE_DECL_FTELLO 1


/* Define to 1 if you have the declaration of `getdelim', and to 0 if you
don't. */
#define HAVE_DECL_GETDELIM 1

/* Define to 1 if you have the declaration of `getdtablesize', and to 0 if you
don't. */
#define HAVE_DECL_GETDTABLESIZE 0

/* Define to 1 if you have the declaration of `getenv', and to 0 if you don't.
*/
#define HAVE_DECL_GETENV 1

/* Define to 1 if you have the declaration of `getline', and to 0 if you
don't. */
#ifdef _MSC_VER
# define HAVE_DECL_GETLINE		0
#else
# define HAVE_DECL_GETLINE		1
#endif // _MSC_VER


/* Define to 1 if you have the declaration of `isblank', and to 0 if you
don't. */
#define HAVE_DECL_ISBLANK 1

/* Define to 1 if you have the declaration of `isinf', and to 0 if you don't.
*/
#define HAVE_DECL_ISINF 1

/* Define to 1 if you have the declaration of `iswblank', and to 0 if you
don't. */
#define HAVE_DECL_ISWBLANK 1

/* Define to 1 if you have the declaration of `mbrlen', and to 0 if you don't.
*/
/* #undef HAVE_DECL_MBRLEN */

/* Define to 1 if you have the declaration of `mbrtowc', and to 0 if you
don't. */
/* #undef HAVE_DECL_MBRTOWC */

/* Define to 1 if you have the declaration of `mbsinit', and to 0 if you
don't. */
/* #undef HAVE_DECL_MBSINIT */

/* Define to 1 if you have the declaration of `mbsrtowcs', and to 0 if you
don't. */
/* #undef HAVE_DECL_MBSRTOWCS */

/* Define to 1 if you have a declaration of mbswidth() in <wchar_gnu.h>, and to 0
otherwise. */
#define HAVE_DECL_MBSWIDTH_IN_WCHAR_H 0

/* Define to 1 if you have the declaration of `program_invocation_name', and
to 0 if you don't. */
#define HAVE_DECL_PROGRAM_INVOCATION_NAME 0

/* Define to 1 if you have the declaration of `program_invocation_short_name',
and to 0 if you don't. */
#define HAVE_DECL_PROGRAM_INVOCATION_SHORT_NAME 0

/* Define to 1 if you have the declaration of `sleep', and to 0 if you don't.
*/
#define HAVE_DECL_SLEEP 1

/* Define to 1 if you have the declaration of `snprintf', and to 0 if you
don't. */
#define HAVE_DECL_SNPRINTF 1

/* Define to 1 if you have the declaration of `stpncpy', and to 0 if you
don't. */
#define HAVE_DECL_STPNCPY 1

/* Define to 1 if you have the declaration of `strerror_r', and to 0 if you
don't. */
#define HAVE_DECL_STRERROR_R 1

/* Define to 1 if you have the declaration of `strnlen', and to 0 if you
don't. */
#define HAVE_DECL_STRNLEN 1

/* Define to 1 if you have the declaration of `towlower', and to 0 if you
don't. */
/* #undef HAVE_DECL_TOWLOWER */

/* Define to 1 if you have the declaration of `unsetenv', and to 0 if you
don't. */
#define HAVE_DECL_UNSETENV 1

/* Define to 1 if you have the declaration of `vsnprintf', and to 0 if you
don't. */
#define HAVE_DECL_VSNPRINTF 1

/* Define to 1 if you have the declaration of `wcrtomb', and to 0 if you
don't. */
/* #undef HAVE_DECL_WCRTOMB */

/* Define to 1 if you have the declaration of `wctob', and to 0 if you don't.
*/
#define HAVE_DECL_WCTOB 1


/* Define to 1 if you have the declaration of `_putenv', and to 0 if you
don't. */
#define HAVE_DECL__PUTENV 0

/* Define to 1 if you have the declaration of `_snprintf', and to 0 if you
don't. */
#define HAVE_DECL__SNPRINTF 0

/* Define to 1 if you have the declaration of `_snwprintf', and to 0 if you
don't. */
#define HAVE_DECL__SNWPRINTF 0

/* Define to 1 if you have the <dirent.h> header file. */
#define HAVE_DIRENT_H 1

/* Define to 1 if you have the `dirfd' function. */
#define HAVE_DIRFD 1

/* Define to 1 if you have the <dlfcn.h> header file. */
#define HAVE_DLFCN_H 1

/* Define to 1 if you have the `dlopen' function. */
#define HAVE_DLOPEN 1

/* Define to 1 if you have the <dl.h> header file. */
/* #undef HAVE_DL_H */

/* Define to 1 if you have the 'dup2' function. */
#define HAVE_DUP2 1

/* Define if you have the declaration of environ. */
#define HAVE_ENVIRON_DECL 1

/* Define to 1 if you have the <errno.h> header file. */
#define HAVE_ERRNO_H 1

/* Define to 1 if you have the `facl' function. */
/* #undef HAVE_FACL */


/* Define to 1 if you have the <features.h> header file. */
//#define HAVE_FEATURES_H 1

/* Define to 1 if you have the <float.h> header file. */
#define HAVE_FLOAT_H 1

/* Define to 1 if you have the `flockfile' function. */
#define HAVE_FLOCKFILE 1

/* Define to 1 if you have the `funlockfile' function. */
#define HAVE_FUNLOCKFILE 0

/* Define to 1 if you have the `getcwd' function. */
#define HAVE_GETCWD 1

/* Define to 1 if you have the `getdelim' function. */
#define HAVE_GETDELIM 0

/* Define to 1 if you have the `getdtablesize' function. */
/* #undef HAVE_GETDTABLESIZE */



/* Define to 1 if you have the `getlocalename_l' function. */
/* #undef HAVE_GETLOCALENAME_L */

/* Define to 1 if you have the <getopt.h> header file. */
#define HAVE_GETOPT_H 1

/* Define to 1 if you have the `getopt_long_only' function. */
#define HAVE_GETOPT_LONG_ONLY 1

/* Define to 1 if you have the `getpagesize' function. */
#define HAVE_GETPAGESIZE 1

/* Define to 1 if you have the `getrlimit' function. */
#define HAVE_GETRLIMIT 1

/* Define if the GNU gettext() function is already present or preinstalled. */
/* #undef HAVE_GETTEXT */

/* Define to 1 if you have the `inet_ntop' function. */
#define HAVE_INET_NTOP 1

/* Define to 1 if the compiler supports one of the keywords 'NLC_INLINE',
'__inline__', '__inline' and effectively inlines functions marked as such.
*/
#define HAVE_INLINE 1

/* Define if you have the 'intmax_t' type in <stdint.h> or <inttypes.h>. */
#define HAVE_INTMAX_T 1

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define if <inttypes.h> exists, doesn't clash with <sys/types.h>, and
declares uintmax_t. */
#define HAVE_INTTYPES_H_WITH_UINTMAX 1

/* Define if <sys/socket.h> defines AF_INET6. */
#define HAVE_IPV6 1

/* Define to 1 if you have the `isascii' function. */
#define HAVE_ISASCII 1

/* Define to 1 if you have the `isblank' function. */
#define HAVE_ISBLANK 1

/* Define if the isnan(double) function is available in libc. */
#define HAVE_ISNAND_IN_LIBC 1

/* Define if the isnan(float) function is available in libc. */
#define HAVE_ISNANF_IN_LIBC 1

/* Define if the isnan(long double) function is available in libc. */
#define HAVE_ISNANL_IN_LIBC 1

/* Define to 1 if you have the `issetugid' function. */
/* #undef HAVE_ISSETUGID */

/* Define to 1 if you have the `iswblank' function. */
#define HAVE_ISWBLANK 1

/* Define to 1 if you have the `iswcntrl' function. */
#define HAVE_ISWCNTRL 1

/* Define to 1 if you have the `iswctype' function. */
#define HAVE_ISWCTYPE 1

/* Define if you have <libgnu/langinfo.h> and nl_langinfo(CODESET). */
/* #undef HAVE_LANGINFO_CODESET */

/* Define to 1 if you have the <libgnu/langinfo.h> header file. */
/* #undef HAVE_LANGINFO_H */

/* Define if your <locale.h> file defines LC_MESSAGES. */
#define HAVE_LC_MESSAGES 1

/* Define to 1 if you have the <libcroco-0.6/libcroco/libcroco-config.h>
header file. */
/* #undef HAVE_LIBCROCO_0_6_LIBCROCO_LIBCROCO_CONFIG_H */

/* Define to 1 if you have the <libintl.h> header file. */
/* #undef HAVE_LIBINTL_H */

/* Define if you have the libunistring library. */
/* #undef HAVE_LIBUNISTRING */

/* Define to 1 if you have the <libxml2/libxml/xmlexports.h> header file. */
/* #undef HAVE_LIBXML2_LIBXML_XMLEXPORTS_H */

/* Define to 1 if you have the <libxml2/libxml/xmlversion.h> header file. */
/* #undef HAVE_LIBXML2_LIBXML_XMLVERSION_H */

/* Define to 1 if you have the <limits.h> header file. */
#define HAVE_LIMITS_H 1

/* Define to 1 if you have the `localtime' function. */
#define HAVE_LOCALTIME 1

/* Define to 1 if you support file names longer than 14 characters. */
#define HAVE_LONG_FILE_NAMES 1

/* Define to 1 if the system has the type 'long long int'. */
#define HAVE_LONG_LONG_INT 1


/* Define to 1 if you have the <mach-o/dyld.h> header file. */
/* #undef HAVE_MACH_O_DYLD_H */

/* Define to 1 if you have the <malloc.h> header file. */
#define HAVE_MALLOC_H 1

/* Define if the 'malloc' function is POSIX compliant. */
#define HAVE_MALLOC_POSIX 1

/* Define to 1 if mmap()'s MAP_ANONYMOUS flag is available after including
config.h and <sys/mman.h>. */
#define HAVE_MAP_ANONYMOUS 1

/* Define to 1 if you have the <math.h> header file. */
#define HAVE_MATH_H 1

/* Define to 1 if you have the `mbrlen' function. */
#define HAVE_MBRLEN 1

/* Define to 1 if you have the `mbrtowc' function. */
#define HAVE_MBRTOWC 1

/* Define to 1 if you have the `mbsinit' function. */
#define HAVE_MBSINIT 1

/* Define to 1 if you have the `mbslen' function. */
/* #undef HAVE_MBSLEN */

/* Define to 1 if you have the `mbsrtowcs' function. */
#define HAVE_MBSRTOWCS 1

/* Define to 1 if <wchar_gnu.h> declares mbstate_t. */
#define HAVE_MBSTATE_T 1

/* Define to 1 if you have the `memmove' function. */
#define HAVE_MEMMOVE 1

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define to 1 if you have the `mempcpy' function. */
/* #undef HAVE_MEMPCPY */

/* Define to 1 if you have the `memset' function. */
#define HAVE_MEMSET 1

/* Define to 1 if <limits.h> defines the MIN and MAX macros. */
/* #undef HAVE_MINMAX_IN_LIMITS_H */

/* Define to 1 if <sys/param.h> defines the MIN and MAX macros. */
#ifndef _MSC_VER
# define HAVE_MINMAX_IN_SYS_PARAM_H 1
#endif // _MSC_VER

/* Define to 1 if you have the `mkdtemp' function. */
#ifndef _MSC_VER
# define HAVE_MKDTEMP 1
#endif // _MSC_VER

/* Define to 1 if you have a working `mmap' system call. */
/* #undef HAVE_MMAP */

/* Define to 1 if you have the `mprotect' function. */
#ifndef _MSC_VER
# define HAVE_MPROTECT 1
#endif // _MSC_VER

/* Define to 1 if you have the <netdb.h> header file. */
#ifndef _MSC_VER
# define HAVE_NETDB_H 1
#endif // _MSC_VER

/* Define to 1 if you have the <netinet/in.h> header file. */
#ifndef _MSC_VER
# define HAVE_NETINET_IN_H 1
#endif // _MSC_VER


/* Define to 1 if you have the `nl_langinfo' function. */
/* #undef HAVE_NL_LANGINFO */

/* Define to 1 if the system has obstacks that work with any size object. */
/* #undef HAVE_OBSTACK */

/* Define to 1 if you have the `opendir' function. */
#ifndef _MSC_VER
# define HAVE_OPENDIR 1
#endif // _MSC_VER

/* Define to 1 if you have the <OS.h> header file. */
/* #undef HAVE_OS_H */

/* Define to 1 if you have the `pathconf' function. */
#ifndef _MSC_VER
# define HAVE_PATHCONF 1
#endif // _MSC_VER

/* Define to 1 if you have the <paths.h> header file. */
#ifndef _MSC_VER
# define HAVE_PATHS_H 1
#endif // _MSC_VER

/* Define to 1 if you have the `pipe' function. */
#ifndef _MSC_VER
# define HAVE_PIPE 1
#endif // _MSC_VER

/* Define to 1 if you have the `pipe2' function. */
#ifndef _MSC_VER
# define HAVE_PIPE2 1
#endif // _MSC_VER

/* Define if your printf() function supports format strings with positions. */
#ifndef _MSC_VER
# define HAVE_POSIX_PRINTF 1
#endif // _MSC_VER

/* Define to 1 if you have the `posix_spawn' function. */
/* #undef HAVE_POSIX_SPAWN */

/* Define to 1 if the system has the type `posix_spawnattr_t'. */
/* #undef HAVE_POSIX_SPAWNATTR_T */

/* Define to 1 if the system has the type `posix_spawn_file_actions_t'. */
/* #undef HAVE_POSIX_SPAWN_FILE_ACTIONS_T */

/* Define to 1 if you have the `pthread_atfork' function. */
#ifndef _MSC_VER
# define HAVE_PTHREAD_ATFORK 1
#endif // _MSC_VER

/* Define if the <libpthread/pthread.h> defines PTHREAD_MUTEX_RECURSIVE. */
#ifndef _MSC_VER
# define HAVE_PTHREAD_MUTEX_RECURSIVE 1
#endif // _MSC_VER

/* Define if the POSIX multithreading library has read/write locks. */
#define HAVE_PTHREAD_RWLOCK 1

/* Define to 1 if the system has the type `ptrdiff_t'. */
#define HAVE_PTRDIFF_T 1

/* Define to 1 if you have the `putenv' function. */
#define HAVE_PUTENV 1

/* Define to 1 if you have the <pwd.h> header file. */
#define HAVE_PWD_H 1

/* Define to 1 if you have the `raise' function. */
#define HAVE_RAISE 1

/* Define to 1 if you have the `rawmemchr' function. */
/* #undef HAVE_RAWMEMCHR */

/* Define to 1 if acosf is declared even after undefining macros. */
#define HAVE_RAW_DECL_ACOSF 1

/* Define to 1 if acosl is declared even after undefining macros. */
#define HAVE_RAW_DECL_ACOSL 1

/* Define to 1 if alphasort is declared even after undefining macros. */
#define HAVE_RAW_DECL_ALPHASORT 1

/* Define to 1 if asinf is declared even after undefining macros. */
#define HAVE_RAW_DECL_ASINF 1

/* Define to 1 if asinl is declared even after undefining macros. */
#define HAVE_RAW_DECL_ASINL 1

/* Define to 1 if atanf is declared even after undefining macros. */
#define HAVE_RAW_DECL_ATANF 1

/* Define to 1 if atanl is declared even after undefining macros. */
#define HAVE_RAW_DECL_ATANL 1

/* Define to 1 if atoll is declared even after undefining macros. */
#define HAVE_RAW_DECL_ATOLL 1

/* Define to 1 if btowc is declared even after undefining macros. */
#define HAVE_RAW_DECL_BTOWC 1

/* Define to 1 if canonicalize_file_name is declared even after undefining
macros. */
/* #undef HAVE_RAW_DECL_CANONICALIZE_FILE_NAME */

/* Define to 1 if cbrt is declared even after undefining macros. */
#define HAVE_RAW_DECL_CBRT 1

/* Define to 1 if cbrtf is declared even after undefining macros. */
#define HAVE_RAW_DECL_CBRTF 1

/* Define to 1 if cbrtl is declared even after undefining macros. */
#define HAVE_RAW_DECL_CBRTL 1

/* Define to 1 if ceilf is declared even after undefining macros. */
#define HAVE_RAW_DECL_CEILF 1

/* Define to 1 if ceill is declared even after undefining macros. */
#define HAVE_RAW_DECL_CEILL 1

/* Define to 1 if chdir is declared even after undefining macros. */
#define HAVE_RAW_DECL_CHDIR 1

/* Define to 1 if chown is declared even after undefining macros. */
#define HAVE_RAW_DECL_CHOWN 1

/* Define to 1 if closedir is declared even after undefining macros. */
#define HAVE_RAW_DECL_CLOSEDIR 1

/* Define to 1 if copysign is declared even after undefining macros. */
#define HAVE_RAW_DECL_COPYSIGN 1

/* Define to 1 if copysignf is declared even after undefining macros. */
#define HAVE_RAW_DECL_COPYSIGNF 1

/* Define to 1 if copysignl is declared even after undefining macros. */
#define HAVE_RAW_DECL_COPYSIGNL 1

/* Define to 1 if cosf is declared even after undefining macros. */
#define HAVE_RAW_DECL_COSF 1

/* Define to 1 if coshf is declared even after undefining macros. */
#define HAVE_RAW_DECL_COSHF 1

/* Define to 1 if cosl is declared even after undefining macros. */
#define HAVE_RAW_DECL_COSL 1

/* Define to 1 if dirfd is declared even after undefining macros. */
#define HAVE_RAW_DECL_DIRFD 1

/* Define to 1 if dprintf is declared even after undefining macros. */
#define HAVE_RAW_DECL_DPRINTF 1

/* Define to 1 if dup is declared even after undefining macros. */
#define HAVE_RAW_DECL_DUP 1

/* Define to 1 if dup2 is declared even after undefining macros. */
#define HAVE_RAW_DECL_DUP2 1

/* Define to 1 if dup3 is declared even after undefining macros. */
#define HAVE_RAW_DECL_DUP3 1

/* Define to 1 if duplocale is declared even after undefining macros. */
#define HAVE_RAW_DECL_DUPLOCALE 1

/* Define to 1 if endusershell is declared even after undefining macros. */
/* #undef HAVE_RAW_DECL_ENDUSERSHELL */

/* Define to 1 if euidaccess is declared even after undefining macros. */
/* #undef HAVE_RAW_DECL_EUIDACCESS */

/* Define to 1 if exp2 is declared even after undefining macros. */
#define HAVE_RAW_DECL_EXP2 1

/* Define to 1 if exp2f is declared even after undefining macros. */
#define HAVE_RAW_DECL_EXP2F 1

/* Define to 1 if exp2l is declared even after undefining macros. */
#define HAVE_RAW_DECL_EXP2L 1

/* Define to 1 if expf is declared even after undefining macros. */
#define HAVE_RAW_DECL_EXPF 1

/* Define to 1 if expl is declared even after undefining macros. */
#define HAVE_RAW_DECL_EXPL 1

/* Define to 1 if expm1 is declared even after undefining macros. */
#define HAVE_RAW_DECL_EXPM1 1

/* Define to 1 if expm1f is declared even after undefining macros. */
#define HAVE_RAW_DECL_EXPM1F 1

/* Define to 1 if expm1l is declared even after undefining macros. */
#define HAVE_RAW_DECL_EXPM1L 1

/* Define to 1 if fabsf is declared even after undefining macros. */
#define HAVE_RAW_DECL_FABSF 1

/* Define to 1 if fabsl is declared even after undefining macros. */
#define HAVE_RAW_DECL_FABSL 1

/* Define to 1 if faccessat is declared even after undefining macros. */
#define HAVE_RAW_DECL_FACCESSAT 1

/* Define to 1 if fchdir is declared even after undefining macros. */
#define HAVE_RAW_DECL_FCHDIR 1

/* Define to 1 if fchmodat is declared even after undefining macros. */
#define HAVE_RAW_DECL_FCHMODAT 1

/* Define to 1 if fchownat is declared even after undefining macros. */
#define HAVE_RAW_DECL_FCHOWNAT 1

/* Define to 1 if fcntl is declared even after undefining macros. */
#define HAVE_RAW_DECL_FCNTL 1

/* Define to 1 if fdatasync is declared even after undefining macros. */
#define HAVE_RAW_DECL_FDATASYNC 1

/* Define to 1 if fdopendir is declared even after undefining macros. */
#define HAVE_RAW_DECL_FDOPENDIR 1

/* Define to 1 if ffsl is declared even after undefining macros. */
/* #undef HAVE_RAW_DECL_FFSL */

/* Define to 1 if ffsll is declared even after undefining macros. */
/* #undef HAVE_RAW_DECL_FFSLL */

/* Define to 1 if floorf is declared even after undefining macros. */
#define HAVE_RAW_DECL_FLOORF 1

/* Define to 1 if floorl is declared even after undefining macros. */
#define HAVE_RAW_DECL_FLOORL 1

/* Define to 1 if fma is declared even after undefining macros. */
#define HAVE_RAW_DECL_FMA 1

/* Define to 1 if fmaf is declared even after undefining macros. */
#define HAVE_RAW_DECL_FMAF 1

/* Define to 1 if fmal is declared even after undefining macros. */
#define HAVE_RAW_DECL_FMAL 1

/* Define to 1 if fmod is declared even after undefining macros. */
#define HAVE_RAW_DECL_FMOD 1

/* Define to 1 if fmodf is declared even after undefining macros. */
#define HAVE_RAW_DECL_FMODF 1

/* Define to 1 if fmodl is declared even after undefining macros. */
#define HAVE_RAW_DECL_FMODL 1

/* Define to 1 if fpurge is declared even after undefining macros. */
#define HAVE_RAW_DECL_FPURGE 1

/* Define to 1 if frexpf is declared even after undefining macros. */
#define HAVE_RAW_DECL_FREXPF 1

/* Define to 1 if frexpl is declared even after undefining macros. */
#define HAVE_RAW_DECL_FREXPL 1

/* Define to 1 if fseeko is declared even after undefining macros. */
#define HAVE_RAW_DECL_FSEEKO 1

/* Define to 1 if fstat is declared even after undefining macros. */
#define HAVE_RAW_DECL_FSTAT 1

/* Define to 1 if fstatat is declared even after undefining macros. */
#define HAVE_RAW_DECL_FSTATAT 1

/* Define to 1 if fsync is declared even after undefining macros. */
#define HAVE_RAW_DECL_FSYNC 1

/* Define to 1 if ftello is declared even after undefining macros. */
#define HAVE_RAW_DECL_FTELLO 1

/* Define to 1 if ftruncate is declared even after undefining macros. */
#define HAVE_RAW_DECL_FTRUNCATE 1

/* Define to 1 if futimens is declared even after undefining macros. */
#define HAVE_RAW_DECL_FUTIMENS 1

/* Define to 1 if getcwd is declared even after undefining macros. */
#define HAVE_RAW_DECL_GETCWD 1

/* Define to 1 if getdelim is declared even after undefining macros. */
#define HAVE_RAW_DECL_GETDELIM 1

/* Define to 1 if getdomainname is declared even after undefining macros. */
/* #undef HAVE_RAW_DECL_GETDOMAINNAME */

/* Define to 1 if getdtablesize is declared even after undefining macros. */
/* #undef HAVE_RAW_DECL_GETDTABLESIZE */

/* Define to 1 if getlogin_r is declared even after undefining macros. */
/* #undef HAVE_RAW_DECL_GETLOGIN_R */

/* Define to 1 if getsubopt is declared even after undefining macros. */
/* #undef HAVE_RAW_DECL_GETSUBOPT */

/* Define to 1 if gettimeofday is declared even after undefining macros. */
#define HAVE_RAW_DECL_GETTIMEOFDAY 1

/* Define to 1 if getusershell is declared even after undefining macros. */
/* #undef HAVE_RAW_DECL_GETUSERSHELL */

/* Define to 1 if grantpt is declared even after undefining macros. */
#define HAVE_RAW_DECL_GRANTPT 1

/* Define to 1 if group_member is declared even after undefining macros. */
/* #undef HAVE_RAW_DECL_GROUP_MEMBER */

/* Define to 1 if hypotf is declared even after undefining macros. */
#define HAVE_RAW_DECL_HYPOTF 1

/* Define to 1 if hypotl is declared even after undefining macros. */
#define HAVE_RAW_DECL_HYPOTL 1

/* Define to 1 if ilogb is declared even after undefining macros. */
#define HAVE_RAW_DECL_ILOGB 1

/* Define to 1 if ilogbf is declared even after undefining macros. */
#define HAVE_RAW_DECL_ILOGBF 1

/* Define to 1 if ilogbl is declared even after undefining macros. */
#define HAVE_RAW_DECL_ILOGBL 1

/* Define to 1 if imaxabs is declared even after undefining macros. */
#define HAVE_RAW_DECL_IMAXABS 1

/* Define to 1 if imaxdiv is declared even after undefining macros. */
#define HAVE_RAW_DECL_IMAXDIV 1

/* Define to 1 if initstate is declared even after undefining macros. */
#define HAVE_RAW_DECL_INITSTATE 1

/* Define to 1 if initstate_r is declared even after undefining macros. */
/* #undef HAVE_RAW_DECL_INITSTATE_R */

/* Define to 1 if isatty is declared even after undefining macros. */
#define HAVE_RAW_DECL_ISATTY 1

/* Define to 1 if isblank is declared even after undefining macros. */
#define HAVE_RAW_DECL_ISBLANK 1

/* Define to 1 if iswctype is declared even after undefining macros. */
#define HAVE_RAW_DECL_ISWCTYPE 1

/* Define to 1 if lchmod is declared even after undefining macros. */
/* #undef HAVE_RAW_DECL_LCHMOD */

/* Define to 1 if lchown is declared even after undefining macros. */
#define HAVE_RAW_DECL_LCHOWN 1

/* Define to 1 if ldexpf is declared even after undefining macros. */
#define HAVE_RAW_DECL_LDEXPF 1

/* Define to 1 if ldexpl is declared even after undefining macros. */
#define HAVE_RAW_DECL_LDEXPL 1

/* Define to 1 if link is declared even after undefining macros. */
#define HAVE_RAW_DECL_LINK 1

/* Define to 1 if linkat is declared even after undefining macros. */
#define HAVE_RAW_DECL_LINKAT 1

/* Define to 1 if log is declared even after undefining macros. */
#define HAVE_RAW_DECL_LOG 1

/* Define to 1 if log10 is declared even after undefining macros. */
#define HAVE_RAW_DECL_LOG10 1

/* Define to 1 if log10f is declared even after undefining macros. */
#define HAVE_RAW_DECL_LOG10F 1

/* Define to 1 if log10l is declared even after undefining macros. */
#define HAVE_RAW_DECL_LOG10L 1

/* Define to 1 if log1p is declared even after undefining macros. */
#define HAVE_RAW_DECL_LOG1P 1

/* Define to 1 if log1pf is declared even after undefining macros. */
#define HAVE_RAW_DECL_LOG1PF 1

/* Define to 1 if log1pl is declared even after undefining macros. */
#define HAVE_RAW_DECL_LOG1PL 1

/* Define to 1 if log2 is declared even after undefining macros. */
#define HAVE_RAW_DECL_LOG2 1

/* Define to 1 if log2f is declared even after undefining macros. */
#define HAVE_RAW_DECL_LOG2F 1

/* Define to 1 if log2l is declared even after undefining macros. */
#define HAVE_RAW_DECL_LOG2L 1

/* Define to 1 if logb is declared even after undefining macros. */
#define HAVE_RAW_DECL_LOGB 1

/* Define to 1 if logbf is declared even after undefining macros. */
#define HAVE_RAW_DECL_LOGBF 1

/* Define to 1 if logbl is declared even after undefining macros. */
#define HAVE_RAW_DECL_LOGBL 1

/* Define to 1 if logf is declared even after undefining macros. */
#define HAVE_RAW_DECL_LOGF 1

/* Define to 1 if logl is declared even after undefining macros. */
#define HAVE_RAW_DECL_LOGL 1

/* Define to 1 if lseek is declared even after undefining macros. */
#define HAVE_RAW_DECL_LSEEK 1

/* Define to 1 if lstat is declared even after undefining macros. */
#define HAVE_RAW_DECL_LSTAT 1

/* Define to 1 if mbrlen is declared even after undefining macros. */
#define HAVE_RAW_DECL_MBRLEN 1

/* Define to 1 if mbrtowc is declared even after undefining macros. */
#define HAVE_RAW_DECL_MBRTOWC 1

/* Define to 1 if mbsinit is declared even after undefining macros. */
#define HAVE_RAW_DECL_MBSINIT 1

/* Define to 1 if mbsnrtowcs is declared even after undefining macros. */
#define HAVE_RAW_DECL_MBSNRTOWCS 1

/* Define to 1 if mbsrtowcs is declared even after undefining macros. */
#define HAVE_RAW_DECL_MBSRTOWCS 1

/* Define to 1 if memmem is declared even after undefining macros. */
#define HAVE_RAW_DECL_MEMMEM 1

/* Define to 1 if mempcpy is declared even after undefining macros. */
/* #undef HAVE_RAW_DECL_MEMPCPY */

/* Define to 1 if memrchr is declared even after undefining macros. */
#define HAVE_RAW_DECL_MEMRCHR 1

/* Define to 1 if mkdirat is declared even after undefining macros. */
#define HAVE_RAW_DECL_MKDIRAT 1

/* Define to 1 if mkdtemp is declared even after undefining macros. */
#define HAVE_RAW_DECL_MKDTEMP 1

/* Define to 1 if mkfifo is declared even after undefining macros. */
#define HAVE_RAW_DECL_MKFIFO 1

/* Define to 1 if mkfifoat is declared even after undefining macros. */
/* #undef HAVE_RAW_DECL_MKFIFOAT */

/* Define to 1 if mknod is declared even after undefining macros. */
#define HAVE_RAW_DECL_MKNOD 1

/* Define to 1 if mknodat is declared even after undefining macros. */
#define HAVE_RAW_DECL_MKNODAT 1

/* Define to 1 if mkostemp is declared even after undefining macros. */
/* #undef HAVE_RAW_DECL_MKOSTEMP */

/* Define to 1 if mkostemps is declared even after undefining macros. */
/* #undef HAVE_RAW_DECL_MKOSTEMPS */

/* Define to 1 if mkstemp is declared even after undefining macros. */
#define HAVE_RAW_DECL_MKSTEMP 1

/* Define to 1 if mkstemps is declared even after undefining macros. */
/* #undef HAVE_RAW_DECL_MKSTEMPS */

/* Define to 1 if modf is declared even after undefining macros. */
#define HAVE_RAW_DECL_MODF 1

/* Define to 1 if modff is declared even after undefining macros. */
#define HAVE_RAW_DECL_MODFF 1

/* Define to 1 if modfl is declared even after undefining macros. */
#define HAVE_RAW_DECL_MODFL 1

/* Define to 1 if nl_langinfo is declared even after undefining macros. */
/* #undef HAVE_RAW_DECL_NL_LANGINFO */

/* Define to 1 if openat is declared even after undefining macros. */
#define HAVE_RAW_DECL_OPENAT 1

/* Define to 1 if opendir is declared even after undefining macros. */
#define HAVE_RAW_DECL_OPENDIR 1

/* Define to 1 if pclose is declared even after undefining macros. */
#define HAVE_RAW_DECL_PCLOSE 1

/* Define to 1 if pipe is declared even after undefining macros. */
#define HAVE_RAW_DECL_PIPE 1

/* Define to 1 if pipe2 is declared even after undefining macros. */
#define HAVE_RAW_DECL_PIPE2 1

/* Define to 1 if popen is declared even after undefining macros. */
#define HAVE_RAW_DECL_POPEN 1

/* Define to 1 if posix_openpt is declared even after undefining macros. */
#define HAVE_RAW_DECL_POSIX_OPENPT 1

/* Define to 1 if posix_spawn is declared even after undefining macros. */
/* #undef HAVE_RAW_DECL_POSIX_SPAWN */

/* Define to 1 if posix_spawnattr_destroy is declared even after undefining
macros. */
/* #undef HAVE_RAW_DECL_POSIX_SPAWNATTR_DESTROY */

/* Define to 1 if posix_spawnattr_getflags is declared even after undefining
macros. */
/* #undef HAVE_RAW_DECL_POSIX_SPAWNATTR_GETFLAGS */

/* Define to 1 if posix_spawnattr_getpgroup is declared even after undefining
macros. */
/* #undef HAVE_RAW_DECL_POSIX_SPAWNATTR_GETPGROUP */

/* Define to 1 if posix_spawnattr_getschedparam is declared even after
undefining macros. */
/* #undef HAVE_RAW_DECL_POSIX_SPAWNATTR_GETSCHEDPARAM */

/* Define to 1 if posix_spawnattr_getschedpolicy is declared even after
undefining macros. */
/* #undef HAVE_RAW_DECL_POSIX_SPAWNATTR_GETSCHEDPOLICY */

/* Define to 1 if posix_spawnattr_getsigdefault is declared even after
undefining macros. */
/* #undef HAVE_RAW_DECL_POSIX_SPAWNATTR_GETSIGDEFAULT */

/* Define to 1 if posix_spawnattr_getsigmask is declared even after undefining
macros. */
/* #undef HAVE_RAW_DECL_POSIX_SPAWNATTR_GETSIGMASK */

/* Define to 1 if posix_spawnattr_init is declared even after undefining
macros. */
/* #undef HAVE_RAW_DECL_POSIX_SPAWNATTR_INIT */

/* Define to 1 if posix_spawnattr_setflags is declared even after undefining
macros. */
/* #undef HAVE_RAW_DECL_POSIX_SPAWNATTR_SETFLAGS */

/* Define to 1 if posix_spawnattr_setpgroup is declared even after undefining
macros. */
/* #undef HAVE_RAW_DECL_POSIX_SPAWNATTR_SETPGROUP */

/* Define to 1 if posix_spawnattr_setschedparam is declared even after
undefining macros. */
/* #undef HAVE_RAW_DECL_POSIX_SPAWNATTR_SETSCHEDPARAM */

/* Define to 1 if posix_spawnattr_setschedpolicy is declared even after
undefining macros. */
/* #undef HAVE_RAW_DECL_POSIX_SPAWNATTR_SETSCHEDPOLICY */

/* Define to 1 if posix_spawnattr_setsigdefault is declared even after
undefining macros. */
/* #undef HAVE_RAW_DECL_POSIX_SPAWNATTR_SETSIGDEFAULT */

/* Define to 1 if posix_spawnattr_setsigmask is declared even after undefining
macros. */
/* #undef HAVE_RAW_DECL_POSIX_SPAWNATTR_SETSIGMASK */

/* Define to 1 if posix_spawnp is declared even after undefining macros. */
/* #undef HAVE_RAW_DECL_POSIX_SPAWNP */

/* Define to 1 if posix_spawn_file_actions_addclose is declared even after
undefining macros. */
/* #undef HAVE_RAW_DECL_POSIX_SPAWN_FILE_ACTIONS_ADDCLOSE */

/* Define to 1 if posix_spawn_file_actions_adddup2 is declared even after
undefining macros. */
/* #undef HAVE_RAW_DECL_POSIX_SPAWN_FILE_ACTIONS_ADDDUP2 */

/* Define to 1 if posix_spawn_file_actions_addopen is declared even after
undefining macros. */
/* #undef HAVE_RAW_DECL_POSIX_SPAWN_FILE_ACTIONS_ADDOPEN */

/* Define to 1 if posix_spawn_file_actions_destroy is declared even after
undefining macros. */
/* #undef HAVE_RAW_DECL_POSIX_SPAWN_FILE_ACTIONS_DESTROY */

/* Define to 1 if posix_spawn_file_actions_init is declared even after
undefining macros. */
/* #undef HAVE_RAW_DECL_POSIX_SPAWN_FILE_ACTIONS_INIT */

/* Define to 1 if powf is declared even after undefining macros. */
#define HAVE_RAW_DECL_POWF 1

/* Define to 1 if pread is declared even after undefining macros. */
#define HAVE_RAW_DECL_PREAD 1

/* Define to 1 if pselect is declared even after undefining macros. */
#define HAVE_RAW_DECL_PSELECT 1

/* Define to 1 if pthread_sigmask is declared even after undefining macros. */
/* #undef HAVE_RAW_DECL_PTHREAD_SIGMASK */

/* Define to 1 if ptsname is declared even after undefining macros. */
#define HAVE_RAW_DECL_PTSNAME 1

/* Define to 1 if ptsname_r is declared even after undefining macros. */
#define HAVE_RAW_DECL_PTSNAME_R 1

/* Define to 1 if pwrite is declared even after undefining macros. */
#define HAVE_RAW_DECL_PWRITE 1

/* Define to 1 if random is declared even after undefining macros. */
#define HAVE_RAW_DECL_RANDOM 1

/* Define to 1 if random_r is declared even after undefining macros. */
/* #undef HAVE_RAW_DECL_RANDOM_R */

/* Define to 1 if rawmemchr is declared even after undefining macros. */
/* #undef HAVE_RAW_DECL_RAWMEMCHR */

/* Define to 1 if readdir is declared even after undefining macros. */
#define HAVE_RAW_DECL_READDIR 1

/* Define to 1 if readlink is declared even after undefining macros. */
#define HAVE_RAW_DECL_READLINK 1

/* Define to 1 if readlinkat is declared even after undefining macros. */
#define HAVE_RAW_DECL_READLINKAT 1

/* Define to 1 if realpath is declared even after undefining macros. */
#define HAVE_RAW_DECL_REALPATH 1

/* Define to 1 if remainder is declared even after undefining macros. */
#define HAVE_RAW_DECL_REMAINDER 1

/* Define to 1 if remainderf is declared even after undefining macros. */
#define HAVE_RAW_DECL_REMAINDERF 1

/* Define to 1 if remainderl is declared even after undefining macros. */
#define HAVE_RAW_DECL_REMAINDERL 1

/* Define to 1 if renameat is declared even after undefining macros. */
#define HAVE_RAW_DECL_RENAMEAT 1

/* Define to 1 if rewinddir is declared even after undefining macros. */
#define HAVE_RAW_DECL_REWINDDIR 1

/* Define to 1 if rint is declared even after undefining macros. */
#define HAVE_RAW_DECL_RINT 1

/* Define to 1 if rintf is declared even after undefining macros. */
#define HAVE_RAW_DECL_RINTF 1

/* Define to 1 if rintl is declared even after undefining macros. */
#define HAVE_RAW_DECL_RINTL 1

/* Define to 1 if rmdir is declared even after undefining macros. */
#define HAVE_RAW_DECL_RMDIR 1

/* Define to 1 if round is declared even after undefining macros. */
#define HAVE_RAW_DECL_ROUND 1

/* Define to 1 if roundf is declared even after undefining macros. */
#define HAVE_RAW_DECL_ROUNDF 1

/* Define to 1 if roundl is declared even after undefining macros. */
#define HAVE_RAW_DECL_ROUNDL 1

/* Define to 1 if rpmatch is declared even after undefining macros. */
/* #undef HAVE_RAW_DECL_RPMATCH */

/* Define to 1 if scandir is declared even after undefining macros. */
#define HAVE_RAW_DECL_SCANDIR 1

/* Define to 1 if secure_getenv is declared even after undefining macros. */
/* #undef HAVE_RAW_DECL_SECURE_GETENV */

/* Define to 1 if select is declared even after undefining macros. */
#define HAVE_RAW_DECL_SELECT 1

/* Define to 1 if setenv is declared even after undefining macros. */
#define HAVE_RAW_DECL_SETENV 1

/* Define to 1 if sethostname is declared even after undefining macros. */
/* #undef HAVE_RAW_DECL_SETHOSTNAME */

/* Define to 1 if setlocale is declared even after undefining macros. */
#define HAVE_RAW_DECL_SETLOCALE 1

/* Define to 1 if setstate is declared even after undefining macros. */
#define HAVE_RAW_DECL_SETSTATE 1

/* Define to 1 if setstate_r is declared even after undefining macros. */
/* #undef HAVE_RAW_DECL_SETSTATE_R */

/* Define to 1 if setusershell is declared even after undefining macros. */
/* #undef HAVE_RAW_DECL_SETUSERSHELL */

/* Define to 1 if sinf is declared even after undefining macros. */
#define HAVE_RAW_DECL_SINF 1

/* Define to 1 if sinhf is declared even after undefining macros. */
#define HAVE_RAW_DECL_SINHF 1

/* Define to 1 if sinl is declared even after undefining macros. */
#define HAVE_RAW_DECL_SINL 1

/* Define to 1 if sleep is declared even after undefining macros. */
#define HAVE_RAW_DECL_SLEEP 1

/* Define to 1 if snprintf is declared even after undefining macros. */
#define HAVE_RAW_DECL_SNPRINTF 1

/* Define to 1 if sqrtf is declared even after undefining macros. */
#define HAVE_RAW_DECL_SQRTF 1

/* Define to 1 if sqrtl is declared even after undefining macros. */
#define HAVE_RAW_DECL_SQRTL 1

/* Define to 1 if srandom is declared even after undefining macros. */
#define HAVE_RAW_DECL_SRANDOM 1

/* Define to 1 if srandom_r is declared even after undefining macros. */
/* #undef HAVE_RAW_DECL_SRANDOM_R */

/* Define to 1 if stat is declared even after undefining macros. */
#define HAVE_RAW_DECL_STAT 1

/* Define to 1 if stpcpy is declared even after undefining macros. */
#define HAVE_RAW_DECL_STPCPY 1

/* Define to 1 if stpncpy is declared even after undefining macros. */
#define HAVE_RAW_DECL_STPNCPY 1

/* Define to 1 if strcasestr is declared even after undefining macros. */
#define HAVE_RAW_DECL_STRCASESTR 1

/* Define to 1 if strchrnul is declared even after undefining macros. */
/* #undef HAVE_RAW_DECL_STRCHRNUL */

/* Define to 1 if strdup is declared even after undefining macros. */
#define HAVE_RAW_DECL_STRDUP 1

/* Define to 1 if strerror_r is declared even after undefining macros. */
#define HAVE_RAW_DECL_STRERROR_R 1

/* Define to 1 if strncat is declared even after undefining macros. */
#define HAVE_RAW_DECL_STRNCAT 1

/* Define to 1 if strndup is declared even after undefining macros. */
#define HAVE_RAW_DECL_STRNDUP 1

/* Define to 1 if strnlen is declared even after undefining macros. */
#define HAVE_RAW_DECL_STRNLEN 1

/* Define to 1 if strpbrk is declared even after undefining macros. */
#define HAVE_RAW_DECL_STRPBRK 1

/* Define to 1 if strsep is declared even after undefining macros. */
#define HAVE_RAW_DECL_STRSEP 1

/* Define to 1 if strsignal is declared even after undefining macros. */
#define HAVE_RAW_DECL_STRSIGNAL 1

/* Define to 1 if strtod is declared even after undefining macros. */
#define HAVE_RAW_DECL_STRTOD 1

/* Define to 1 if strtoimax is declared even after undefining macros. */
#define HAVE_RAW_DECL_STRTOIMAX 1

/* Define to 1 if strtok_r is declared even after undefining macros. */
#define HAVE_RAW_DECL_STRTOK_R 1

/* Define to 1 if strtoll is declared even after undefining macros. */
#define HAVE_RAW_DECL_STRTOLL 1

/* Define to 1 if strtoull is declared even after undefining macros. */
#define HAVE_RAW_DECL_STRTOULL 1

/* Define to 1 if strtoumax is declared even after undefining macros. */
#define HAVE_RAW_DECL_STRTOUMAX 1

/* Define to 1 if strverscmp is declared even after undefining macros. */
/* #undef HAVE_RAW_DECL_STRVERSCMP */

/* Define to 1 if symlink is declared even after undefining macros. */
#define HAVE_RAW_DECL_SYMLINK 1

/* Define to 1 if symlinkat is declared even after undefining macros. */
#define HAVE_RAW_DECL_SYMLINKAT 1

/* Define to 1 if tanf is declared even after undefining macros. */
#define HAVE_RAW_DECL_TANF 1

/* Define to 1 if tanhf is declared even after undefining macros. */
#define HAVE_RAW_DECL_TANHF 1

/* Define to 1 if tanl is declared even after undefining macros. */
#define HAVE_RAW_DECL_TANL 1

/* Define to 1 if tmpfile is declared even after undefining macros. */
#define HAVE_RAW_DECL_TMPFILE 1

/* Define to 1 if towctrans is declared even after undefining macros. */
#define HAVE_RAW_DECL_TOWCTRANS 1

/* Define to 1 if trunc is declared even after undefining macros. */
#define HAVE_RAW_DECL_TRUNC 1

/* Define to 1 if truncf is declared even after undefining macros. */
#define HAVE_RAW_DECL_TRUNCF 1

/* Define to 1 if truncl is declared even after undefining macros. */
#define HAVE_RAW_DECL_TRUNCL 1

/* Define to 1 if ttyname_r is declared even after undefining macros. */
#define HAVE_RAW_DECL_TTYNAME_R 1

/* Define to 1 if unlink is declared even after undefining macros. */
#define HAVE_RAW_DECL_UNLINK 1

/* Define to 1 if unlinkat is declared even after undefining macros. */
#define HAVE_RAW_DECL_UNLINKAT 1

/* Define to 1 if unlockpt is declared even after undefining macros. */
#define HAVE_RAW_DECL_UNLOCKPT 1

/* Define to 1 if unsetenv is declared even after undefining macros. */
#define HAVE_RAW_DECL_UNSETENV 1

/* Define to 1 if usleep is declared even after undefining macros. */
#define HAVE_RAW_DECL_USLEEP 1

/* Define to 1 if utimensat is declared even after undefining macros. */
#define HAVE_RAW_DECL_UTIMENSAT 1

/* Define to 1 if vdprintf is declared even after undefining macros. */
#define HAVE_RAW_DECL_VDPRINTF 1

/* Define to 1 if vsnprintf is declared even after undefining macros. */
#define HAVE_RAW_DECL_VSNPRINTF 1

/* Define to 1 if waitpid is declared even after undefining macros. */
#define HAVE_RAW_DECL_WAITPID 1

/* Define to 1 if wcpcpy is declared even after undefining macros. */
/* #undef HAVE_RAW_DECL_WCPCPY */

/* Define to 1 if wcpncpy is declared even after undefining macros. */
/* #undef HAVE_RAW_DECL_WCPNCPY */

/* Define to 1 if wcrtomb is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCRTOMB 1

/* Define to 1 if wcscasecmp is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCSCASECMP 1

/* Define to 1 if wcscat is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCSCAT 1

/* Define to 1 if wcschr is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCSCHR 1

/* Define to 1 if wcscmp is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCSCMP 1

/* Define to 1 if wcscoll is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCSCOLL 1

/* Define to 1 if wcscpy is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCSCPY 1

/* Define to 1 if wcscspn is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCSCSPN 1

/* Define to 1 if wcsdup is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCSDUP 1

/* Define to 1 if wcslen is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCSLEN 1

/* Define to 1 if wcsncasecmp is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCSNCASECMP 1

/* Define to 1 if wcsncat is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCSNCAT 1

/* Define to 1 if wcsncmp is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCSNCMP 1

/* Define to 1 if wcsncpy is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCSNCPY 1

/* Define to 1 if wcsnlen is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCSNLEN 1

/* Define to 1 if wcsnrtombs is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCSNRTOMBS 1

/* Define to 1 if wcspbrk is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCSPBRK 1

/* Define to 1 if wcsrchr is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCSRCHR 1

/* Define to 1 if wcsrtombs is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCSRTOMBS 1

/* Define to 1 if wcsspn is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCSSPN 1

/* Define to 1 if wcsstr is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCSSTR 1

/* Define to 1 if wcstok is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCSTOK 1

/* Define to 1 if wcswidth is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCSWIDTH 1

/* Define to 1 if wcsxfrm is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCSXFRM 1

/* Define to 1 if wctob is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCTOB 1

/* Define to 1 if wctrans is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCTRANS 1

/* Define to 1 if wctype is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCTYPE 1

/* Define to 1 if wcwidth is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCWIDTH 1

/* Define to 1 if wmemchr is declared even after undefining macros. */
#define HAVE_RAW_DECL_WMEMCHR 1

/* Define to 1 if wmemcmp is declared even after undefining macros. */
#define HAVE_RAW_DECL_WMEMCMP 1

/* Define to 1 if wmemcpy is declared even after undefining macros. */
#define HAVE_RAW_DECL_WMEMCPY 1

/* Define to 1 if wmemmove is declared even after undefining macros. */
#define HAVE_RAW_DECL_WMEMMOVE 1

/* Define to 1 if wmemset is declared even after undefining macros. */
#define HAVE_RAW_DECL_WMEMSET 1

/* Define to 1 if _Exit is declared even after undefining macros. */
#define HAVE_RAW_DECL__EXIT 1

/* Define to 1 if you have the `readdir' function. */
#define HAVE_READDIR 1


/* Define if the 'realloc' function is POSIX compliant. */
#define HAVE_REALLOC_POSIX 1

/* Define to 1 if 'sig_atomic_t' is a signed integer type. */
/* #undef HAVE_SIGNED_SIG_ATOMIC_T */

/* Define to 1 if 'wchar_t' is a signed integer type. */
/* #undef HAVE_SIGNED_WCHAR_T */

/* Define to 1 if 'wint_t' is a signed integer type. */
/* #undef HAVE_SIGNED_WINT_T */

/* Define to 1 if the system has the type `sig_atomic_t'. */
#define HAVE_SIG_ATOMIC_T 1

/* Define to 1 if you have the `sleep' function. */
#define HAVE_SLEEP 1

/* Define to 1 if you have the `snprintf' function. */
#define HAVE_SNPRINTF 1

/* Define if the return value of the snprintf function is the number of of
bytes (excluding the terminating NUL) that would have been produced if the
buffer had been large enough. */
/* #undef HAVE_SNPRINTF_RETVAL_C99 */

/* Define to 1 if you have the <spawn.h> header file. */
/* #undef HAVE_SPAWN_H */


/* Define to 1 if you have the `statacl' function. */
/* #undef HAVE_STATACL */

/* Define to 1 if you have the <stddef.h> header file. */
#define HAVE_STDDEF_H 1

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define if <stdint.h> exists, doesn't clash with <sys/types.h>, and declares
uintmax_t. */
#define HAVE_STDINT_H_WITH_UINTMAX 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the `stpcpy' function. */
#define HAVE_STPCPY 1

/* Define if you have the stpncpy() function and it works. */
#define HAVE_STPNCPY 1

/* Define to 1 if you have the `strcasecmp' function. */
#define HAVE_STRCASECMP 1

/* Define to 1 if you have the `strchrnul' function. */
/* #undef HAVE_STRCHRNUL */

/* Define to 1 if you have the `strcspn' function. */
#define HAVE_STRCSPN 1

/* Define to 1 if you have the `strdup' function. */
#define HAVE_STRDUP 1

/* Define to 1 if you have the `stricmp' function. */
#if defined(TARGET_OS_ANDROID)
# define HAVE_STRICMP 1
#endif // defined(TARGET_OS_ANDROID)

/* Define to 1 if you have the `strpbrk' function. */
#define HAVE_STRPBRK 1

/* Define to 1 if `decimal_point' is a member of `struct lconv'. */
/* #undef HAVE_STRUCT_LCONV_DECIMAL_POINT */

/* Define to 1 if `sa_sigaction' is a member of `struct sigaction'. */
#define HAVE_STRUCT_SIGACTION_SA_SIGACTION 1

/* Define to 1 if `__names' is a member of `struct __locale_struct'. */
/* #undef HAVE_STRUCT___LOCALE_STRUCT___NAMES */

/* Define to 1 if you have the <sys/acl.h> header file. */
/* #undef HAVE_SYS_ACL_H */

/* Define to 1 if you have the <sys/bitypes.h> header file. */
/* #undef HAVE_SYS_BITYPES_H */

/* Define to 1 if you have the <sys/inttypes.h> header file. */
/* #undef HAVE_SYS_INTTYPES_H */


/* Define if tgetent(), tgetnum(), tgetstr(), tgetflag() are among the termcap
library functions. */
/* #undef HAVE_TERMCAP */

/* Define if setupterm(), tigetnum(), tigetstr(), tigetflag() are among the
termcap library functions. */
/* #undef HAVE_TERMINFO */

/* Define to 1 if you have the <time.h> header file. */
#define HAVE_TIME_H 1

/* Define to 1 if you have the `towlower' function. */
#define HAVE_TOWLOWER 1

/* Define if tparam() is among the termcap library functions. */
/* #undef HAVE_TPARAM */

/* Define if you have the 'uintmax_t' type in <stdint.h> or <inttypes.h>. */
#define HAVE_UINTMAX_T 1

/* Define if <sys/wait.h> defines the 'union wait' type. */
/* #undef HAVE_UNION_WAIT */

/* Define to 1 if you have the `unsetenv' function. */
#define HAVE_UNSETENV 1
#ifndef _MSC_VER
/* Define to 1 if you have the `utime' function. */
#define HAVE_UTIME 1
/* Define to 1 if you have the `utimes' function. */
#define HAVE_UTIMES 1
/* Define to 1 if you have the <utime.h> header file. */
#define HAVE_UTIME_H 1
#endif // _MSC_VER

/* Define to 1 if you have the `vasnprintf' function. */
/* #undef HAVE_VASNPRINTF */

/* Define to 1 if you have the `vasprintf' function. */
#define HAVE_VASPRINTF 1

/* Define to 1 if you have the `vfork' function. */
#define HAVE_VFORK 1

/* Define to 1 if you have the <vfork.h> header file. */
/* #undef HAVE_VFORK_H */


/* Define to 1 if you have the <wchar_gnu.h> header file. */
#define HAVE_WCHAR_H 1

/* Define if you have the 'wchar_t' type. */
#define HAVE_WCHAR_T 1

/* Define to 1 if you have the `wcrtomb' function. */
#define HAVE_WCRTOMB 1

/* Define to 1 if you have the `wcslen' function. */
#define HAVE_WCSLEN 1

/* Define to 1 if you have the `wcsnlen' function. */
#define HAVE_WCSNLEN 1

/* Define to 1 if you have the `wctob' function. */
#define HAVE_WCTOB 1

/* Define to 1 if you have the <wctype.h> header file. */
#ifdef _MSC_VER
#define HAVE_WCTYPE_H 1
#endif // _MSC_VER

/* Define to 1 if you have the `wcwidth' function. */
#define HAVE_WCWIDTH 1

/* Define to 1 if you have the `wmemchr' function. */
#define HAVE_WMEMCHR 1

/* Define to 1 if you have the `wmemcpy' function. */
#define HAVE_WMEMCPY 1

/* Define to 1 if you have the `wmempcpy' function. */
/* #undef HAVE_WMEMPCPY */

/* Define to 1 if `fork' works. */
#define HAVE_WORKING_FORK 1

/* Define to 1 if O_NOATIME works. */
#define HAVE_WORKING_O_NOATIME 0

/* Define to 1 if O_NOFOLLOW works. */
#define HAVE_WORKING_O_NOFOLLOW 0

/* Define if you have the posix_spawn and posix_spawnp functions and they
work. */
/* #undef HAVE_WORKING_POSIX_SPAWN */

/* Define to 1 if `vfork' works. */
#define HAVE_WORKING_VFORK 1

/* Define to 1 if you have the <xlocale.h> header file. */
#define HAVE_XLOCALE_H 1

/* Define to 1 if the system has the type `_Bool'. */
#define HAVE__BOOL 1

/* Define to 1 if you have the `_ftelli64' function. */
/* #undef HAVE__FTELLI64 */

/* Define to 1 if you have the `_ftime' function. */
/* #undef HAVE__FTIME */

/* Define to 1 if you have the `_NSGetExecutablePath' function. */
/* #undef HAVE__NSGETEXECUTABLEPATH */

/* Define to 1 if you have the `_set_invalid_parameter_handler' function. */
/* #undef HAVE__SET_INVALID_PARAMETER_HANDLER */

/* Define to 1 if you have the `_stat' function. */
/* #undef HAVE__STAT */

/* Define to 1 if you have the `__fsetlocking' function. */
/* #undef HAVE___FSETLOCKING */

/* Define to 1 if you have the `__secure_getenv' function. */
/* #undef HAVE___SECURE_GETENV */

/* Define as const if the declaration of iconv() needs const. */
#define ICONV_CONST 

/* Define to a symbolic name denoting the flavor of iconv_open()
implementation. */
/* #undef ICONV_FLAVOR */

/* Define to the value of ${prefix}, as a string. */
#define INSTALLPREFIX "//home/jack/xbmc-depends/arm-linux-androideabi-android-21"

/* Define if integer division by zero raises signal SIGFPE. */
#define INTDIV0_RAISES_SIGFPE 0

/* Define to 1 if the C compiler is actually a C++ compiler. */
#define IS_CPLUSPLUS		0

/* Define as the bit index in the word where to find bit 0 of the exponent of
'long double'. */
/* #undef LDBL_EXPBIT0_BIT */

/* Define as the word index where to find the exponent of 'long double'. */
/* #undef LDBL_EXPBIT0_WORD */

/* Define as the bit index in the word where to find the sign of 'long
double'. */
/* #undef LDBL_SIGNBIT_BIT */

/* Define as the word index where to find the sign of 'long double'. */
/* #undef LDBL_SIGNBIT_WORD */

/* Define to 1 if lseek does not detect pipes. */
/* #undef LSEEK_PIPE_BROKEN */

/* Define to 1 if 'lstat' dereferences a symlink specified with a trailing
slash. */
/* #undef LSTAT_FOLLOWS_SLASHED_SYMLINK */

/* Define to the sub-directory where libtool stores uninstalled libraries. */
#define LT_OBJDIR ".libs/"

/* If malloc(0) is != NULL, define this to 1. Otherwise define this to 0. */
#define MALLOC_0_IS_NONNULL 1

/* Define to a substitute value for mmap()'s MAP_ANONYMOUS flag. */
/* #undef MAP_ANONYMOUS */

/* Define if the mbrtowc function does not return (size_t) -2 for empty input.
*/
/* #undef MBRTOWC_EMPTY_INPUT_BUG */

/* Define if the mbrtowc function has the NULL pwc argument bug. */
/* #undef MBRTOWC_NULL_ARG1_BUG */

/* Define if the mbrtowc function has the NULL string argument bug. */
/* #undef MBRTOWC_NULL_ARG2_BUG */

/* Define if the mbrtowc function does not return 0 for a NUL character. */
/* #undef MBRTOWC_NUL_RETVAL_BUG */

/* Define if the mbrtowc function returns a wrong return value. */
/* #undef MBRTOWC_RETVAL_BUG */

/* Define to the type that is the result of default argument promotions of
type mode_t. */
#define PROMOTED_MODE_T int


/* Define to 1 if readlink fails to recognize a trailing slash. */
#define READLINK_TRAILING_SLASH_BUG 1

/* Define as const if the declaration of setlocale() needs const. */
#define SETLOCALE_CONST const

/* Define if lists must be signal-safe. */
#define SIGNAL_SAFE_LIST 1

/* Define to l, ll, u, ul, ull, etc., as suitable for constants of type
'sig_atomic_t'. */
/* #undef SIG_ATOMIC_T_SUFFIX */

/* Define as the maximum value of type 'size_t', if the system doesn't define
it. */
#ifndef SIZE_MAX
/* # undef SIZE_MAX */
#endif

/* Define to l, ll, u, ul, ull, etc., as suitable for constants of type
'size_t'. */
/* #undef SIZE_T_SUFFIX */

/* If using the C implementation of alloca, define if you know the
direction of stack growth for your system; otherwise it will be
automatically deduced at runtime.
STACK_DIRECTION > 0 => grows toward higher addresses
STACK_DIRECTION < 0 => grows toward lower addresses
STACK_DIRECTION = 0 => direction of growth unknown */
/* #undef STACK_DIRECTION */

/* Define to 1 if the `S_IS*' macros in <sys/stat.h> do not work properly. */
/* #undef STAT_MACROS_BROKEN */

/* Define to 1 if strerror_r returns char *. */
/* #undef STRERROR_R_CHAR_P */

/* Define to the prefix of C symbols at the assembler and linker level, either
an underscore or empty. */
#define USER_LABEL_PREFIX 

/* Define to nonzero if you want access control list support. */
#define USE_ACL 0

/* Define if the POSIX multithreading library can be used. */
#define USE_POSIX_THREADS 1

/* Define if references to the POSIX multithreading library should be made
weak. */
/* #undef USE_POSIX_THREADS_WEAK */

/* Define if the GNU Pth multithreading library can be used. */
/* #undef USE_PTH_THREADS */

/* Define if references to the GNU Pth multithreading library should be made
weak. */
/* #undef USE_PTH_THREADS_WEAK */

/* Define if the old Solaris multithreading library can be used. */
/* #undef USE_SOLARIS_THREADS */

/* Define if references to the old Solaris multithreading library should be
made weak. */
/* #undef USE_SOLARIS_THREADS_WEAK */

/* Enable extensions on AIX 3, Interix.  */
#ifndef _ALL_SOURCE
# define _ALL_SOURCE 1
#endif
/* Enable general extensions on OS X.  */
#ifndef _DARWIN_C_SOURCE
# define _DARWIN_C_SOURCE 1
#endif
/* Enable GNU extensions on systems that have them.  */
#ifndef _GNU_SOURCE
# define _GNU_SOURCE 1
#endif
/* Use GNU style printf and scanf.  */
#ifndef __USE_MINGW_ANSI_STDIO
# define __USE_MINGW_ANSI_STDIO 1
#endif
/* Enable threading extensions on Solaris.  */
#ifndef _POSIX_PTHREAD_SEMANTICS
# define _POSIX_PTHREAD_SEMANTICS 1
#endif
/* Enable extensions on HP NonStop.  */
#ifndef _TANDEM_SOURCE
# define _TANDEM_SOURCE 1
#endif
/* Enable X/Open extensions if necessary.  HP-UX 11.11 defines
mbstate_t only if _XOPEN_SOURCE is defined to 500, regardless of
whether compiling with -Ae or -D_HPUX_SOURCE=1.  */
#ifndef _XOPEN_SOURCE
/* # undef _XOPEN_SOURCE */
#endif
/* Enable general extensions on Solaris.  */
#ifndef __EXTENSIONS__
# define __EXTENSIONS__ 1
#endif

/* Define to a working va_copy macro or replacement. */
#define VA_COPY va_copy

/* Define to 1 if unsetenv returns void instead of int. */
/* #undef VOID_UNSETENV */

/* Define to l, ll, u, ul, ull, etc., as suitable for constants of type
'wchar_t'. */
/* #undef WCHAR_T_SUFFIX */

/* Define to l, ll, u, ul, ull, etc., as suitable for constants of type
'wint_t'. */
/* #undef WINT_T_SUFFIX */

/* Define when --enable-shared is used on mingw or Cygwin. */
/* #undef WOE32DLL */

/* Define to 1 to internationalize bison runtime messages. */
/* #undef YYENABLE_NLS */

/* Enable large inode numbers on Mac OS X 10.5. */
#define _DARWIN_USE_64_BIT_INODE 1

/* Number of bits in a file offset, on hosts where this is settable. */
/* #undef _FILE_OFFSET_BITS */

/* Define to 1 if Gnulib overrides 'struct stat' on Windows so that struct
stat.st_size becomes 64-bit. */
/* #undef _GL_WINDOWS_64_BIT_ST_SIZE */

/* Define to 1 to make fseeko visible on some hosts (e.g. glibc 2.2). */
/* #undef _LARGEFILE_SOURCE */

/* Define for large files, on AIX-style hosts. */
/* #undef _LARGE_FILES */

/* Define to 1 on Solaris. */
/* #undef _LCONV_C99 */

/* Define to 1 if on MINIX. */
/* #undef _MINIX */

/* Define to 1 to make NetBSD features available. MINIX 3 needs this. */
/* #undef _NETBSD_SOURCE */

/* The _Noreturn keyword of C11.  */
#if ! (defined _Noreturn \
       || (defined __STDC_VERSION__ && 201112 <= __STDC_VERSION__))
# if (3 <= __GNUC__ || (__GNUC__ == 2 && 8 <= __GNUC_MINOR__) \
      || 0x5110 <= __SUNPRO_C)
#  define _Noreturn __attribute__ ((__noreturn__))
# elif defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn
# endif
#endif


/* Define to 2 if the system does not provide POSIX.1 features except with
this defined. */
/* #undef _POSIX_1_SOURCE */

/* Define to 1 if you need to in order for 'stat' and other things to work. */
/* #undef _POSIX_SOURCE */

/* Define if you want <regex.h> to include <limits.h>, so that it consistently
overrides <limits.h>'s RE_DUP_MAX. */
#define _REGEX_INCLUDE_LIMITS_H 1

/* Define if you want regoff_t to be at least as wide POSIX requires. */
#define _REGEX_LARGE_OFFSETS 1

/* Define to rpl_ if the getopt replacement functions and variables should be
used. */
#define __GETOPT_PREFIX rpl_

/* Define to 1 if the system <stdint.h> predates C++11. */
/* #undef __STDC_CONSTANT_MACROS */

/* Define to 1 if the system <stdint.h> predates C++11. */
/* #undef __STDC_LIMIT_MACROS */

/* Define as 'access' if you don't have the eaccess() function. */
#define eaccess access

/* Please see the Gnulib manual for how to use these macros.

Suppress extern NLC_INLINE with HP-UX cc, as it appears to be broken; see
<http://lists.gnu.org/archive/html/bug-texinfo/2013-02/msg00030.html>.

Suppress extern NLC_INLINE with Sun C in standards-conformance mode, as it
mishandles NLC_INLINE functions that call each other.  E.g., for 'NLC_INLINE void f
(void) { } NLC_INLINE void g (void) { f (); }', c99 incorrectly complains
'reference to static identifier "f" in extern NLC_INLINE function'.
This bug was observed with Sun C 5.12 SunOS_i386 2011/11/16.

Suppress extern NLC_INLINE (with or without __attribute__ ((__gnu_inline__)))
on configurations that mistakenly use 'static NLC_INLINE' to implement
functions or macros in standard C headers like <ctype.h>.  For example,
if isdigit is mistakenly implemented via a static NLC_INLINE function,
a program containing an extern NLC_INLINE function that calls isdigit
may not work since the C standard prohibits extern NLC_INLINE functions
from calling static functions.  This bug is known to occur on:

OS X 10.8 and earlier; see:
http://lists.gnu.org/archive/html/bug-gnulib/2012-12/msg00023.html

DragonFly; see
http://muscles.dragonflybsd.org/bulk/bleeding-edge-potential/latest-per-pkg/ah-tty-0.3.12.log

FreeBSD; see:
http://lists.gnu.org/archive/html/bug-gnulib/2014-07/msg00104.html

OS X 10.9 has a macro __header_inline indicating the bug is fixed for C and
for clang but remains for g++; see <http://trac.macports.org/ticket/41033>.
Assume DragonFly and FreeBSD will be similar.  */
#if (((defined __APPLE__ && defined __MACH__) \
      || defined __DragonFly__ || defined __FreeBSD__) \
     && (defined __header_inline \
         ? (defined __cplusplus && defined __GNUC_STDC_INLINE__ \
            && ! defined __clang__) \
         : ((! defined _DONT_USE_CTYPE_INLINE_ \
             && (defined __GNUC__ || defined __cplusplus)) \
            || (defined _FORTIFY_SOURCE && 0 < _FORTIFY_SOURCE \
                && defined __GNUC__ && ! defined __cplusplus))))
# define _GL_EXTERN_INLINE_STDHEADER_BUG
#endif

/* In GCC 4.6 (inclusive) to 5.1 (exclusive),
suppress bogus "no previous prototype for 'FOO'"
and "no previous declaration for 'FOO'" diagnostics,
when FOO is an NLC_INLINE function in the header; see
<https://gcc.gnu.org/bugzilla/show_bug.cgi?id=54113> and
<https://gcc.gnu.org/bugzilla/show_bug.cgi?id=63877>.  */
#if __GNUC__ == 4 && 6 <= __GNUC_MINOR__
# if defined __GNUC_STDC_INLINE__ && __GNUC_STDC_INLINE__
#  define _GL_INLINE_HEADER_CONST_PRAGMA
# else
#  define _GL_INLINE_HEADER_CONST_PRAGMA \
     _Pragma ("GCC diagnostic ignored \"-Wsuggest-attribute=const\"")
# endif
# define _GL_INLINE_HEADER_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wmissing-prototypes\"") \
    _Pragma ("GCC diagnostic ignored \"-Wmissing-declarations\"") \
    _GL_INLINE_HEADER_CONST_PRAGMA
# define _GL_INLINE_HEADER_END \
    _Pragma ("GCC diagnostic pop")
#else
# define _GL_INLINE_HEADER_BEGIN
# define _GL_INLINE_HEADER_END
#endif

/* Define to a replacement function name for fnmatch(). */
#define fnmatch posix_fnmatch

/* Define to `int' if <sys/types.h> doesn't define. */
/* #undef gid_t */

/* A replacement for va_copy, if needed.  */
#define gl_va_copy(a,b) ((a) = (b))

/* Define as a marker that can be attached to declarations that might not
be used.  This helps to reduce warnings, such as from
GCC -Wunused-parameter.  */
#ifndef _GL_UNUSED
# if __GNUC__ >= 3 || (__GNUC__ == 2 && __GNUC_MINOR__ >= 7)
#  define _GL_UNUSED __attribute__ ((__unused__))
# else
#  define _GL_UNUSED
# endif
#endif

/* The __pure__ attribute was added in gcc 2.96.  */
#ifndef _GL_ATTRIBUTE_PURE
# if __GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ >= 96)
#  define _GL_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define _GL_ATTRIBUTE_PURE /* empty */
# endif
#endif


/* Define to `__inline__' or `__inline' if that's what the C compiler
calls it, or to nothing if 'NLC_INLINE' is not supported under any name.  */
#ifndef __cplusplus
/* #undef NLC_INLINE */
#endif

/* Define to long or long long if <stdint.h> and <inttypes.h> don't define. */
/* #undef intmax_t */

/* Work around a bug in Apple GCC 4.0.1 build 5465: In C99 mode, it supports
the ISO C 99 semantics of 'extern NLC_INLINE' (unlike the GNU C semantics of
earlier versions), but does not display it by setting __GNUC_STDC_INLINE__.
__APPLE__ && __MACH__ test for Mac OS X.
__APPLE_CC__ tests for the Apple compiler and its version.
__STDC_VERSION__ tests for the C99 mode.  */
#if defined __APPLE__ && defined __MACH__ && __APPLE_CC__ >= 5465 && !defined __cplusplus && __STDC_VERSION__ >= 199901L && !defined __GNUC_STDC_INLINE__
# define __GNUC_STDC_INLINE__ 1
#endif

/* Define to a type if <wchar_gnu.h> does not define. */
/* #undef mbstate_t */

/* Define to `int' if <sys/types.h> does not define. */
/* #undef mode_t */

/* Define to the type of st_nlink in struct stat, or a supertype. */
/* #undef nlink_t */

/* Define to `int' if <sys/types.h> does not define. */
/* #undef pid_t */

/* Define as the type of the result of subtracting two pointers, if the system
doesn't define it. */
/* #undef ptrdiff_t */

/* Define to rpl_re_comp if the replacement should be used. */
#define re_comp rpl_re_comp

/* Define to rpl_re_compile_fastmap if the replacement should be used. */
#define re_compile_fastmap rpl_re_compile_fastmap

/* Define to rpl_re_compile_pattern if the replacement should be used. */
#define re_compile_pattern rpl_re_compile_pattern

/* Define to rpl_re_exec if the replacement should be used. */
#define re_exec rpl_re_exec

/* Define to rpl_re_match if the replacement should be used. */
#define re_match rpl_re_match

/* Define to rpl_re_match_2 if the replacement should be used. */
#define re_match_2 rpl_re_match_2

/* Define to rpl_re_search if the replacement should be used. */
#define re_search rpl_re_search

/* Define to rpl_re_search_2 if the replacement should be used. */
#define re_search_2 rpl_re_search_2

/* Define to rpl_re_set_registers if the replacement should be used. */
#define re_set_registers rpl_re_set_registers

/* Define to rpl_re_set_syntax if the replacement should be used. */
#define re_set_syntax rpl_re_set_syntax

/* Define to rpl_re_syntax_options if the replacement should be used. */
#define re_syntax_options rpl_re_syntax_options

/* Define to rpl_regcomp if the replacement should be used. */
#define regcomp rpl_regcomp

/* Define to rpl_regerror if the replacement should be used. */
#define regerror rpl_regerror

/* Define to rpl_regexec if the replacement should be used. */
#define regexec rpl_regexec

/* Define to rpl_regfree if the replacement should be used. */
#define regfree rpl_regfree

/* Define to the equivalent of the C99 'NLC_RESTRICT' keyword, or to
nothing if this is not supported.  Do not define if NLC_RESTRICT is
supported directly.  */
//#define NLC_RESTRICT __restrict

/* Work around a bug in Sun C++: it does not support _Restrict or
__restrict__, even though the corresponding Sun C compiler ends up with
"#define NLC_RESTRICT _Restrict" or "#define NLC_RESTRICT __restrict__" in the
previous line.  Perhaps some future version of Sun C++ will work with
NLC_RESTRICT; if so, hopefully it defines __RESTRICT like Sun C does.  */
#if defined __SUNPRO_CC && !defined __RESTRICT
# define _Restrict
# define __restrict__
#endif

/* Define as an integer type suitable for memory locations that can be
accessed atomically even in the presence of asynchronous signals. */
/* #undef sig_atomic_t */

/* Define to `unsigned int' if <sys/types.h> does not define. */
/* #undef size_t */

/* Define as a signed type of the same size as size_t. */
/* #undef ssize_t */

/* Define to `int' if <sys/types.h> doesn't define. */
/* #undef uid_t */

/* Define to unsigned long or unsigned long long if <stdint.h> and
<inttypes.h> don't define. */
/* #undef uintmax_t */

/* The name _UNUSED_PARAMETER_ is an earlier spelling, although the name
is a misnomer outside of parameter lists.  */
#define _UNUSED_PARAMETER_			NLC_UNUSED

/* gcc supports the "unused" attribute on possibly unused labels, and
g++ has since version 4.5.  Note to support C++ as well as C,
_GL_UNUSED_LABEL should be used with a trailing ;  */
#if !defined __cplusplus || __GNUC__ > 4 \
    || (__GNUC__ == 4 && __GNUC_MINOR__ >= 5)
# define _GL_UNUSED_LABEL _GL_UNUSED
#else
# define _GL_UNUSED_LABEL
#endif


# define _GL_ATTRIBUTE_PURE   NLC_ATTRIBUTE_PURE
# define _GL_ATTRIBUTE_CONST   NLC_ATTRIBUTE_CONST


/* Define as a macro for copying va_list variables. */
/* #undef va_copy */

/* Define as `fork' if `vfork' does not work. */
/* #undef vfork */


#define __libc_lock_t                   gl_lock_t
#define __libc_lock_define              gl_lock_define
#define __libc_lock_define_initialized  gl_lock_define_initialized
#define __libc_lock_init                gl_lock_init
#define __libc_lock_lock                gl_lock_lock
#define __libc_lock_unlock              gl_lock_unlock
#define __libc_lock_recursive_t                   gl_recursive_lock_t
#define __libc_lock_define_recursive              gl_recursive_lock_define
#define __libc_lock_define_initialized_recursive  gl_recursive_lock_define_initialized
#define __libc_lock_init_recursive                gl_recursive_lock_init
#define __libc_lock_lock_recursive                gl_recursive_lock_lock
#define __libc_lock_unlock_recursive              gl_recursive_lock_unlock
#define glthread_in_use  libintl_thread_in_use
#define glthread_lock_init_func     libintl_lock_init_func
#define glthread_lock_lock_func     libintl_lock_lock_func
#define glthread_lock_unlock_func   libintl_lock_unlock_func
#define glthread_lock_destroy_func  libintl_lock_destroy_func
#define glthread_rwlock_init_multithreaded     libintl_rwlock_init_multithreaded
#define glthread_rwlock_init_func              libintl_rwlock_init_func
#define glthread_rwlock_rdlock_multithreaded   libintl_rwlock_rdlock_multithreaded
#define glthread_rwlock_rdlock_func            libintl_rwlock_rdlock_func
#define glthread_rwlock_wrlock_multithreaded   libintl_rwlock_wrlock_multithreaded
#define glthread_rwlock_wrlock_func            libintl_rwlock_wrlock_func
#define glthread_rwlock_unlock_multithreaded   libintl_rwlock_unlock_multithreaded
#define glthread_rwlock_unlock_func            libintl_rwlock_unlock_func
#define glthread_rwlock_destroy_multithreaded  libintl_rwlock_destroy_multithreaded
#define glthread_rwlock_destroy_func           libintl_rwlock_destroy_func
#define glthread_recursive_lock_init_multithreaded     libintl_recursive_lock_init_multithreaded
#define glthread_recursive_lock_init_func              libintl_recursive_lock_init_func
#define glthread_recursive_lock_lock_multithreaded     libintl_recursive_lock_lock_multithreaded
#define glthread_recursive_lock_lock_func              libintl_recursive_lock_lock_func
#define glthread_recursive_lock_unlock_multithreaded   libintl_recursive_lock_unlock_multithreaded
#define glthread_recursive_lock_unlock_func            libintl_recursive_lock_unlock_func
#define glthread_recursive_lock_destroy_multithreaded  libintl_recursive_lock_destroy_multithreaded
#define glthread_recursive_lock_destroy_func           libintl_recursive_lock_destroy_func
#define glthread_once_func            libintl_once_func
#define glthread_once_singlethreaded  libintl_once_singlethreaded
#define glthread_once_multithreaded   libintl_once_multithreaded



#ifndef ENDIANNESS
# if defined __BIG_ENDIAN__
#  define ENDIANNESS 0
# endif
# if defined __LITTLE_ENDIAN__
#  define ENDIANNESS 0
# endif
#endif



/* A file name cannot consist of any character possible.  INVALID_PATH_CHAR
contains the characters not allowed.  */
#if defined _MSC_VER || defined __MINGW32__
/* Woe32.  This string is valid for Windows NT/2000.  On Windows 95/98/ME some
few characters in the range 0x80..0xff are invalid as well, but this doesn't
matter much for our purposes.  */
# define INVALID_PATH_CHAR "\001\002\003\004\005\006\007\010\011\012\013\014\015\016\017\020\021\022\023\024\025\026\027\030\031\032\033\034\035\036\037\"*/:<>?\\|"
#elif defined MSDOS
/* Something like this for MSDOG.  */
# define INVALID_PATH_CHAR "\001\002\003\004\005\006\007\010\011\012\013\014\015\016\017\020\021\022\023\024\025\026\027\030\031\032\033\034\035\036\037 \177\\:."
#else
/* Unix.  */
# define INVALID_PATH_CHAR "\001\002\003\004\005\006\007\010\011\012\013\014\015\016\017\020\021\022\023\024\025\026\027\030\031\032\033\034\035\036\037 \177/"
#endif

/* This is the page width for the message_print function.  It should
not be set to more than 79 characters (Emacs users will appreciate
it).  It is used to wrap the msgid and msgstr strings, and also to
wrap the file position (#:) comments.  */
#define PAGE_WIDTH 79


/* A 'va_copy' style function */
#define G_VA_COPY va_copy

/* 'va_lists' cannot be copies as values */
#define G_VA_COPY_AS_ARRAY 1

/* Define to 1 if you have `alloca', as a function or macro. */
#define HAVE_ALLOCA 1

/* Define to 1 if you have <libgnu/alloca_gnu.h> and it should be used (not on Ultrix).
*/
#define HAVE_ALLOCA_H 1

/* Define to 1 if you have the <arpa/nameser_compat.h> header file. */
#define HAVE_ARPA_NAMESER_COMPAT_H 1

/* Define to 1 if you have the <attr/xattr.h> header file. */
/* #undef HAVE_ATTR_XATTR_H */

/* Define if you have a version of the snprintf function with semantics as
specified by the ISO C99 standard. */
#define HAVE_C99_SNPRINTF 1

/* Define if you have a version of the vsnprintf function with semantics as
specified by the ISO C99 standard. */
#define HAVE_C99_VSNPRINTF 1

/* define to 1 if Carbon is available */
/* #undef HAVE_CARBON */

/* Define to 1 if you have the `chown' function. */
//#define HAVE_CHOWN 1

/* Define to 1 if you have the `clock_gettime' function. */
#define HAVE_CLOCK_GETTIME 1


/* Have nl_langinfo (CODESET) */
#define HAVE_CODESET 1

/* Define to 1 if you have the <crt_externs.h> header file. */
/* #undef HAVE_CRT_EXTERNS_H */

/* Define to 1 if you have the <dlfcn.h> header file. */
//#define HAVE_DLFCN_H 1

/* Define to 1 if you don't have `vprintf' but do have `_doprnt.' */
/* #undef HAVE_DOPRNT */

/* define for working do while(0) macros */
#define HAVE_DOWHILE_MACROS 1

/* Define to 1 if you have the `endmntent' function. */
#define HAVE_ENDMNTENT 1

/* Define to 1 if you have the `endservent' function. */
#define HAVE_ENDSERVENT 1

/* we have the eventfd(2) system call */
#define HAVE_EVENTFD 1

/* Define to 1 if you have the `fdwalk' function. */
/* #undef HAVE_FDWALK */

/* Define to 1 if you have the <float.h> header file. */
#define HAVE_FLOAT_H 1

/* Define to 1 if you have the <fstab.h> header file. */
#define HAVE_FSTAB_H 1

/* Define to 1 if you have the `fsync' function. */
#if !defined(TARGET_OS_WINDOWS)
# define HAVE_FSYNC 1
#endif //!defined(TARGET_OS_WINDOWS)

/* we have the futex(2) system call */
#define HAVE_FUTEX 1

/* Define to 1 if you have the `getcwd' function. */
#define HAVE_GETCWD 1


/* Define to 1 if you have the `getfsstat' function. */
/* #undef HAVE_GETFSSTAT */

/* Define to 1 if you have the `getgrgid' function. */
#define HAVE_GETGRGID 1

/* Define to 1 if you have the `getmntent_r' function. */
#define HAVE_GETMNTENT_R 1

/* Define to 1 if you have the `getprotobyname_r' function. */
#define HAVE_GETPROTOBYNAME_R 1

/* Define to 1 if you have the `getpwuid' function. */
#define HAVE_GETPWUID 1

/* Define to 1 if you have the `getvfsstat' function. */
/* #undef HAVE_GETVFSSTAT */

/* Define to 1 if you have the `gmtime_r' function. */
#define HAVE_GMTIME_R 1

/* define to use system printf */
#define HAVE_GOOD_PRINTF 1

/* Define to 1 if you have the <grp.h> header file. */
#define HAVE_GRP_H 1

/* Define to 1 if you have the `hasmntopt' function. */
#define HAVE_HASMNTOPT 1

/* Define to 1 if you have the `if_nametoindex' function. */
#define HAVE_IF_NAMETOINDEX 1

/* Define to 1 if you have the `inotify_init1' function. */
#define HAVE_INOTIFY_INIT1 1

/* define to support printing 64-bit integers with format I64 */
/* #undef HAVE_INT64_AND_I64 */

/* Define if you have the 'intmax_t' type in <stdint.h> or <inttypes.h>. */
//#define HAVE_INTMAX_T 1

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define if <inttypes.h> exists, doesn't clash with <sys/types.h>, and
declares uintmax_t. */
#define HAVE_INTTYPES_H_WITH_UINTMAX 1

/* Define if we have struct ip_mreqn */
#define HAVE_IP_MREQN /**/

/* Define if you have <libgnu/langinfo.h> and nl_langinfo(CODESET). */
#define HAVE_LANGINFO_CODESET 1

/* Have nl_langinfo (_NL_CTYPE_OUTDIGITn_MB) */
#define HAVE_LANGINFO_OUTDIGIT 1

/* Have nl_langinfo (PM_STR) */
#define HAVE_LANGINFO_TIME 1

/* Define to 1 if you have the `lchmod' function. */
/* #undef HAVE_LCHMOD */

/* Define to 1 if you have the `lchown' function. */
//#define HAVE_LCHOWN 1

/* Define to 1 if you have the <limits.h> header file. */
#define HAVE_LIMITS_H 1

/* Define to 1 if you have the `link' function. */
//#define HAVE_LINK 1

/* Define to 1 if you have the <linux/magic.h> header file. */
//#define HAVE_LINUX_MAGIC_H 1

/* Define to 1 if you have the `localtime_r' function. */
#define HAVE_LOCALTIME_R 1

/* Define if you have the 'long double' type. */
//#define HAVE_LONG_DOUBLE 1

/* Define if you have the 'long long' type. */
#define HAVE_LONG_LONG 1

/* define if system printf can print long long */
#define HAVE_LONG_LONG_FORMAT 1

/* Have non-POSIX function getgrgid_r */
/* #undef HAVE_NONPOSIX_GETGRGID_R */

/* Have non-POSIX function getpwuid_r */
/* #undef HAVE_NONPOSIX_GETPWUID_R */

/* Define to 1 if you have the `on_exit' function. */
#define HAVE_ON_EXIT 1

/* Define to 1 if you have the `pipe2' function. */
#define HAVE_PIPE2 1

/* Have POSIX function getgrgid_r */
#define HAVE_POSIX_GETGRGID_R 1

/* Have POSIX function getpwuid_r */
#define HAVE_POSIX_GETPWUID_R 1


/* Define to 1 if you have the `prlimit' function. */
#define HAVE_PRLIMIT 1

/* Have function pthread_attr_setstacksize */
#define HAVE_PTHREAD_ATTR_SETSTACKSIZE 1

/* Have function pthread_condattr_setclock */
#define HAVE_PTHREAD_CONDATTR_SETCLOCK 1

/* Define to 1 if the system has the type `ptrdiff_t'. */
#define HAVE_PTRDIFF_T 1

/* Define to 1 if you have the <pwd.h> header file. */
#define HAVE_PWD_H 1


/* Define to 1 if you have the <sched.h> header file. */
#define HAVE_SCHED_H 1

/* Define to 1 if libselinux is available */
/* #undef HAVE_SELINUX */

/* Define to 1 if you have the <selinux/selinux.h> header file. */
/* #undef HAVE_SELINUX_SELINUX_H */

/* Define to 1 if you have the `setmntent' function. */
#define HAVE_SETMNTENT 1

/* Define if you have the 'sig_atomic_t' type. */
#define HAVE_SIG_ATOMIC_T 1

/* Define to 1 if you have the `splice' function. */
#define HAVE_SPLICE 1

/* Define to 1 if you have the `statfs' function. */
#define HAVE_STATFS 1

/* Define to 1 if you have the `statvfs' function. */
#define HAVE_STATVFS 1


/* Have functions strlcpy and strlcat */
/* #undef HAVE_STRLCPY */

/* Define to 1 if you have the `strsignal' function. */
#define HAVE_STRSIGNAL 1

/* Define to 1 if you have the `strtod_l' function. */
#define HAVE_STRTOD_L 1

/* Define to 1 if you have the `strtoll_l' function. */
#define HAVE_STRTOLL_L 1

/* Define to 1 if you have the `strtoull_l' function. */
#define HAVE_STRTOULL_L 1
/* Define to 1 if `f_bavail' is a member of `struct statfs'. */
#define HAVE_STRUCT_STATFS_F_BAVAIL 1

/* Define to 1 if `f_fstypename' is a member of `struct statfs'. */
/* #undef HAVE_STRUCT_STATFS_F_FSTYPENAME */

/* Define to 1 if `f_basetype' is a member of `struct statvfs'. */
/* #undef HAVE_STRUCT_STATVFS_F_BASETYPE */

/* Define to 1 if `f_fstypename' is a member of `struct statvfs'. */
/* #undef HAVE_STRUCT_STATVFS_F_FSTYPENAME */

/* Define to 1 if `st_atimensec' is a member of `struct stat'. */
/* #undef HAVE_STRUCT_STAT_ST_ATIMENSEC */

/* Define to 1 if `st_atim.tv_nsec' is a member of `struct stat'. */
#define HAVE_STRUCT_STAT_ST_ATIM_TV_NSEC 1


/* Define to 1 if `st_ctimensec' is a member of `struct stat'. */
/* #undef HAVE_STRUCT_STAT_ST_CTIMENSEC */

/* Define to 1 if `st_ctim.tv_nsec' is a member of `struct stat'. */
#define HAVE_STRUCT_STAT_ST_CTIM_TV_NSEC 1

/* Define to 1 if `st_mtimensec' is a member of `struct stat'. */
/* #undef HAVE_STRUCT_STAT_ST_MTIMENSEC */

/* Define to 1 if `tm_gmtoff' is a member of `struct tm'. */
#define HAVE_STRUCT_TM_TM_GMTOFF 1

/* Define to 1 if `__tm_gmtoff' is a member of `struct tm'. */
/* #undef HAVE_STRUCT_TM___TM_GMTOFF */

/* Define to 1 if you have the <sys/inotify.h> header file. */
#define HAVE_SYS_INOTIFY_H 1

/* Define to 1 if you have the <sys/mkdev.h> header file. */
/* #undef HAVE_SYS_MKDEV_H */

/* Define to 1 if you have the <sys/mntctl.h> header file. */
/* #undef HAVE_SYS_MNTCTL_H */

/* Define to 1 if you have the <sys/mnttab.h> header file. */
/* #undef HAVE_SYS_MNTTAB_H */

/* Define to 1 if you have the <sys/mount.h> header file. */
#define HAVE_SYS_MOUNT_H 1

/* Define to 1 if you have the <sys/ndir.h> header file, and it defines `DIR'.
*/
/* #undef HAVE_SYS_NDIR_H */

#ifndef _MSC_VER
/* Define to 1 if you have the <sys/resource.h> header file. */
#define HAVE_SYS_RESOURCE_H 1
#endif // _MSC_VER

/* found fd_set in sys/select.h */
//#define HAVE_SYS_SELECT_H 1 // defined in NlcCompilerConfig.h

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/sysctl.h> header file. */
#define HAVE_SYS_SYSCTL_H 1

/* Define to 1 if you have the <sys/times.h> header file. */
#define HAVE_SYS_TIMES_H 1

/* Define to 1 if you have the <sys/time.h> header file. */
#define HAVE_SYS_TIME_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the <sys/uio.h> header file. */
#define HAVE_SYS_UIO_H 1

/* Define to 1 if you have the <sys/vfstab.h> header file. */
/* #undef HAVE_SYS_VFSTAB_H */

/* Define to 1 if you have the <sys/vfs.h> header file. */
#ifndef TARGET_OS_WINDOWS
#define HAVE_SYS_VFS_H 1
#endif // TARGET_OS_WINDOWS

/* Define to 1 if you have the <sys/vmount.h> header file. */
/* #undef HAVE_SYS_VMOUNT_H */

/* Define to 1 if you have the <sys/xattr.h> header file. */
#define HAVE_SYS_XATTR_H 1

/* Define to 1 if you have the `timegm' function. */
#define HAVE_TIMEGM 1


/* Define if your printf function family supports positional parameters as
specified by Unix98. */
#define HAVE_UNIX98_PRINTF 1


/* Define to 1 if you have the `valloc' function. */
#define HAVE_VALLOC 1

/* Define to 1 if you have the <values.h> header file. */
#define HAVE_VALUES_H 1

/* Define to 1 if you have the `vasprintf' function. */
#define HAVE_VASPRINTF 1

/* Define to 1 if you have the `vprintf' function. */
#define HAVE_VPRINTF 1

/* Define to 1 if you have the `vsnprintf' function. */
#define HAVE_VSNPRINTF 1

/* Define if you have the 'wchar_t' type. */
#define HAVE_WCHAR_T 1

/* Define to 1 if you have the `wcslen' function. */
#define HAVE_WCSLEN 1

/* Have a working bcopy */
/* #undef HAVE_WORKING_BCOPY */

/* Define to 1 if you have the <wspiapi.h> header file. */
/* #undef HAVE_WSPIAPI_H */

/* Define to 1 if xattr is available */
#define HAVE_XATTR 1

/* Define to 1 if xattr API uses XATTR_NOFOLLOW */
/* #undef HAVE_XATTR_NOFOLLOW */

/* Define to 1 if you have the <xlocale.h> header file. */
#define HAVE_XLOCALE_H 1

/* Define to 1 if you have the `_NSGetEnviron' function. */
/* #undef HAVE__NSGETENVIRON */

/* Define to the sub-directory in which libtool stores uninstalled libraries.
*/
#define LT_OBJDIR ".libs/"

/* Do we cache iconv descriptors */
/* #undef NEED_ICONV_CACHE */

/* didn't find fd_set */
/* #undef NO_FD_SET */

/* Define to 1 if your C compiler doesn't accept -c and -o together. */
/* #undef NO_MINUS_C_MINUS_O */

/* define if posix_memalign() can allocate any size */
#define POSIX_MEMALIGN_WITH_COMPLIANT_ALLOCS 1

/* whether realloc (NULL,) works */
#define REALLOC_0_WORKS 1

/* If using the C implementation of alloca, define if you know the
direction of stack growth for your system; otherwise it will be
automatically deduced at runtime.
STACK_DIRECTION > 0 => grows toward higher addresses
STACK_DIRECTION < 0 => grows toward lower addresses
STACK_DIRECTION = 0 => direction of growth unknown */
/* #undef STACK_DIRECTION */

/* Number of arguments to statfs() */
#define STATFS_ARGS 2

/* Define to use statfs() */
#define USE_STATFS 1

#define GLIB_STATIC_COMPILATION 1
#define G_GNUC_INTERNAL
#define G_STMT_START
#define G_STMT_END

#if NLC_ARCH_BIG_ENDIAN
# define G_BYTE_ORDER 4321 // for little endian us 1234 and for big endian use 4321
#else
# define G_BYTE_ORDER 1234 // for little endian us 1234 and for big endian use 4321
#endif // NLC_ARCH_BIG_ENDIAN

#ifdef TARGET_OS_WINDOWS
#define G_OS_WIN32 1
#define G_PLATFORM_WIN32 1

#endif // TARGET_OS_WINDOWS

typedef uint8_t		guint8;
typedef uint16_t	guint16;
typedef uint32_t	guint32;
typedef uint64_t	guint64;

typedef int8_t		gint8;
typedef int16_t		gint16;
typedef int32_t		gint32;
typedef int64_t		gint64;

#ifdef _MSC_VER
typedef void *		gpointer;
#else
typedef ssize_t		gssize;
#endif

typedef size_t		gsize;
typedef ssize_t		gssize;

#include "gtypes.h"

#define GPOINTER_TO_INT(p)	((gint)  (intptr_t)  (p))
#define GPOINTER_TO_UINT(p)	((guint) (uintptr_t) (p))

#define GINT_TO_POINTER(i)	((gpointer) (intptr_t)  (i))
#define GUINT_TO_POINTER(u)	((gpointer) (uintptr_t) (u))

#define g_memmove memmove


/* ====================== Substitute for <libgnu/gmem.h> ====================== */

#include "xalloc.h"

#define g_malloc(n) xmalloc (n)
#define g_malloc0(n) xzalloc (n)
#define g_realloc(p,n) xrealloc (p, n)
#define g_free(p) free (p)
#define g_try_malloc(n) xmalloc (n)
#define g_try_malloc0(n) xzalloc (n)
#define g_try_realloc(p,n) xrealloc (p, n)

#define g_new(t,n) ((t *) xnmalloc (n, sizeof (t)))
#define g_new0(t,n) ((t *) xcalloc (n, sizeof (t)))
#define g_try_new(t,n) ((t *) xnmalloc (n, sizeof (t)))
#define g_try_new0(t,n) ((t *) xcalloc (n, sizeof (t)))

/* =================== Substitute for <libgnu/gmessages.h> =================== */

#include <stdlib.h>

#define g_assert(expr)                 if (!(expr)) abort ()
#define g_assert_not_reached()         abort ()

#define g_return_if_fail(expr)         if (!(expr)) return
#define g_return_val_if_fail(expr,val) if (!(expr)) return (val)
#define g_return_if_reached()          return
#define g_return_val_if_reached(val)   return (val)

#define G_LOG_LEVEL_CRITICAL	LOG_CRIT
#define G_LOG_LEVEL_INFO		LOG_INFO
#define G_LOG_LEVEL_DEBUG		LOG_DEBUG

extern void g_printerr( const char *format, ... );
extern void g_warning( const char *format, ... );
extern void g_log( const char *domain, int level, const char *format, ... );

/* ==================== Substitute for <libgnu/gprintf.h> ==================== */

#include <stdio.h>
#include <stdarg.h>

#define g_printf printf
#define g_fprintf fprintf
#define g_sprintf sprintf
#define g_vprintf vprintf
#define g_vfprintf vfprintf
#define g_vsprintf vsprintf
#define g_vasprintf vasprintf

/* ===================== Substitute for <libgnu/gslice.h> ===================== */

#define g_slice_new(t) XMALLOC (t)
#define g_slice_new0(t) XZALLOC (t)

#define g_slice_free(t,p) free (p)

/* ======================= Helper for <libgnu/gtypes.h> ======================= */

/* We don't need to export variables from a shared library.  */
#define GLIB_VAR extern

/* ==================== Substitute for <libgnu/gunicode.h> ==================== */

typedef unsigned int gunichar;


/* C99 macros are supported */
#define C99_MACROS 1


/* enable ALPN support */
#define ENABLE_ALPN 1

/* enable anonymous authentication */
#define ENABLE_ANON 1

/* enable DHE */
#define ENABLE_DHE 1

/* enable DTLS-SRTP support */
#define ENABLE_DTLS_SRTP 1

/* enable DHE */
#define ENABLE_ECDHE 1

/* enable heartbeat support */
#define ENABLE_HEARTBEAT 1

/* Enable all curves */
#define ENABLE_NON_SUITEB_CURVES 1

/* enable OCSP support */
#define ENABLE_OCSP 1

/* use openpgp authentication */
#define ENABLE_OPENPGP 1

/* enable PSK authentication */
#define ENABLE_PSK 1

/* enable session tickets support */
#define ENABLE_SESSION_TICKETS 1

/* enable SRP authentication */
#define ENABLE_SRP 1

/* fopen(3) accepts a 'b' in the mode flag */
#define FOPEN_BINARY_FLAG ""

/* fopen(3) accepts a 't' in the mode flag */
#define FOPEN_TEXT_FLAG ""

/* Define this to 'void' or 'struct timezone' to match the system's
declaration of the second argument to gettimeofday. */
#define GETTIMEOFDAY_TIMEZONE struct timezone

/* Define to a C preprocessor expression that evaluates to 1 or 0, depending
whether the gnulib module fscanf shall be considered present. */
#define GNULIB_FSCANF 1

/* Define to 1 if printf and friends should be labeled with attribute
"__gnu_printf__" instead of "__printf__" */
/* #undef GNULIB_PRINTF_ATTRIBUTE_FLAVOR_GNU */

/* Define to a C preprocessor expression that evaluates to 1 or 0, depending
whether the gnulib module scanf shall be considered present. */
#define GNULIB_SCANF 1

/* Define to a C preprocessor expression that evaluates to 1 or 0, depending
whether the gnulib module snprintf shall be considered present. */
#define GNULIB_SNPRINTF 1

/* Define to a C preprocessor expression that evaluates to 1 or 0, depending
whether the gnulib module strerror shall be considered present. */
#define GNULIB_STRERROR 1

/* Make sure we don't use old features in code. */
#define GNUTLS_COMPAT_H 1

/* We allow temporarily usage of deprecated functions - until they are
removed. */
#define GNUTLS_INTERNAL_BUILD		1

/* Additional cast to bring void* to a type castable to int. */
#define GNUTLS_POINTER_TO_INT_CAST (long)

/* Define to 1 if you have the `alarm' function. */
//#define HAVE_ALARM					1

/* Define to 1 if you have the <byteswap.h> header file. */
#define HAVE_BYTESWAP_H				1

/* Define to 1 if you have the declaration of `flockfile', and to 0 if you
don't. */
#define HAVE_DECL_FLOCKFILE			1

/* Define to 1 if you have the declaration of `freeaddrinfo', and to 0 if you
don't. */
#define HAVE_DECL_FREEADDRINFO		1

/* Define to 1 if you have the declaration of `fseeko', and to 0 if you don't.*/
#define HAVE_DECL_FSEEKO			1

/* Define to 1 if you have the declaration of `ftello', and to 0 if you don't.*/
#define HAVE_DECL_FTELLO			1

/* Define to 1 if you have the declaration of `funlockfile', and to 0 if you
don't. */
#define HAVE_DECL_FUNLOCKFILE 1

/* Define to 1 if you have the declaration of `gai_strerror', and to 0 if you
don't. */
#define HAVE_DECL_GAI_STRERROR 1

/* Define to 1 if you have the declaration of `gai_strerrorA', and to 0 if you
don't. */
#define HAVE_DECL_GAI_STRERRORA 0

/* Define to 1 if you have the declaration of `getaddrinfo', and to 0 if you
don't. */
#define HAVE_DECL_GETADDRINFO 1

/* Define to 1 if you have the declaration of `getdelim', and to 0 if you
don't. */
#define HAVE_DECL_GETDELIM 1


/* Define to 1 if you have the declaration of `getnameinfo', and to 0 if you
don't. */
#define HAVE_DECL_GETNAMEINFO 1

/* Define to 1 if you have the declaration of `getpass', and to 0 if you
don't. */
#define HAVE_DECL_GETPASS 0

/* Define to 1 if you have the declaration of `inet_ntop', and to 0 if you
don't. */
#define HAVE_DECL_INET_NTOP 1

/* Define to 1 if you have the declaration of `inet_pton', and to 0 if you
don't. */
#define HAVE_DECL_INET_PTON 1

/* Define to 1 if you have the declaration of `localtime_r', and to 0 if you
don't. */
#define HAVE_DECL_LOCALTIME_R 1

/* Define to 1 if you have the declaration of `memmem', and to 0 if you don't.
*/
#define HAVE_DECL_MEMMEM 1

/* Define to 1 if you have the declaration of `program_invocation_name', and
to 0 if you don't. */
#define HAVE_DECL_PROGRAM_INVOCATION_NAME 0

/* Define to 1 if you have the declaration of `program_invocation_short_name',
and to 0 if you don't. */

/* Define to 1 if you have the declaration of `snprintf', and to 0 if you
don't. */
#define HAVE_DECL_SNPRINTF 1

/* Define to 1 if you have the declaration of `strdup', and to 0 if you don't.
*/
#define HAVE_DECL_STRDUP 1

/* Define to 1 if you have the declaration of `strerror_r', and to 0 if you
don't. */
#define HAVE_DECL_STRERROR_R 1

/* Define to 1 if you have the declaration of `strncasecmp', and to 0 if you
don't. */
#define HAVE_DECL_STRNCASECMP 1

/* Define to 1 if you have the declaration of `strndup', and to 0 if you
don't. */
#define HAVE_DECL_STRNDUP 1

/* Define to 1 if you have the declaration of `strnlen', and to 0 if you
don't. */
#define HAVE_DECL_STRNLEN 1

/* Define to 1 if you have the declaration of `strtok_r', and to 0 if you
don't. */
#define HAVE_DECL_STRTOK_R 1

/* Define to 1 if you have the declaration of `unsetenv', and to 0 if you
don't. */
#define HAVE_DECL_UNSETENV 1

/* Define to 1 if you have the declaration of `vsnprintf', and to 0 if you
don't. */
#define HAVE_DECL_VSNPRINTF 1

/* Define to 1 if you have the declaration of `_snprintf', and to 0 if you
don't. */
#define HAVE_DECL__SNPRINTF 0

/* Define to 1 if you have the declaration of `__fsetlocking', and to 0 if you
don't. */
#define HAVE_DECL___FSETLOCKING 0

/* Define this if /dev/zero is readable device */
#define HAVE_DEV_ZERO 1

/* Define to 1 if you have the <dirent.h> header file, and it defines `DIR'.
*/
#define HAVE_DIRENT_H 1

/* Define to 1 if you have the <dlfcn.h> header file. */
#define HAVE_DLFCN_H 1

/* Define to 1 if you don't have `vprintf' but do have `_doprnt.' */
/* #undef HAVE_DOPRNT */

/* Define to 1 if you have the `dup2' function. */
#define HAVE_DUP2 1

/* Define if you have the declaration of environ. */
#define HAVE_ENVIRON_DECL 1

/* Define to 1 if you have the `epoll_create' function. */
#define HAVE_EPOLL_CREATE 1

/* Define to 1 if you have the <errno.h> header file. */
#define HAVE_ERRNO_H 1


/* Define to 1 if you have the <fcntl.h> header file. */
#define HAVE_FCNTL_H 1


/* Define to 1 if you have the `flockfile' function. */
#define HAVE_FLOCKFILE 1

/* Define to 1 if you have the `funlockfile' function. */
#define HAVE_FUNLOCKFILE 0

/* Define to 1 if you have the `gethostbyname' function. */
#define HAVE_GETHOSTBYNAME 1

/* Define to 1 if you have the `getpagesize' function. */
#define HAVE_GETPAGESIZE 1

/* Define to 1 if you have the `getpass' function. */
/* #undef HAVE_GETPASS */


/* Define to 1 if you have the `getpwuid_r' function. */
#define HAVE_GETPWUID_R 1

/* Define to 1 if you have the `getrusage' function. */
#ifdef _MSC_VER
//# define HAVE_GETRUSAGE 0
#else
# define HAVE_GETRUSAGE 1
#endif // _MSC_VER

/* Define to 1 if you have the `getservbyname' function. */
#define HAVE_GETSERVBYNAME 1

/* Define to 1 if you have the `gettimeofday' function. */
#define HAVE_GETTIMEOFDAY 1

/* Define to 1 if you have the `inet_ntop' function. */
#define HAVE_INET_NTOP 1

/* Define to 1 if you have the `inet_pton' function. */
#define HAVE_INET_PTON 1

/* Define to 1 if the system has the type `int16_t'. */
#define HAVE_INT16_T 1

/* Define to 1 if the system has the type `int32_t'. */
#define HAVE_INT32_T 1

/* Define to 1 if the system has the type `int8_t'. */
#define HAVE_INT8_T 1

/* Define if you have the 'intmax_t' type in <stdint.h> or <inttypes.h>. */
#define HAVE_INTMAX_T 1

/* Define to 1 if the system has the type `intptr_t'. */
#define HAVE_INTPTR_T 1

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define if <inttypes.h> exists, doesn't clash with <sys/types.h>, and
declares uintmax_t. */
#define HAVE_INTTYPES_H_WITH_UINTMAX 1

/* Define to 1 if <sys/socket.h> defines AF_INET. */
#define HAVE_IPV4 1

/* Define to 1 if <sys/socket.h> defines AF_INET6. */
#define HAVE_IPV6 1

/* Define to 1 if you have the <libgen.h> header file. */
//#define HAVE_LIBGEN_H 1 // defined in NlcDependLibrariesConfig.h

/* Define if you have the libiconv library. */
//#define HAVE_LIBICONV 1 // defined in NlcDependLibrariesConfig.h

/* Define to 1 if you have the <libintl.h> header file. */
//#define HAVE_LIBINTL_H 1 // defined in NlcDependLibrariesConfig.h

/* nettle is enabled */
// #define HAVE_LIBNETTLE 1 // defined in NlcDependLibrariesConfig.h

/* Define if you have the libz library. */
// #define HAVE_LIBZ 1 // defined in NlcDependLibrariesConfig.h

/* Define to 1 if you have the <limits.h> header file. */
#define HAVE_LIMITS_H 1

/* Define to 1 if you have the `localtime' function. */
#define HAVE_LOCALTIME 1

/* Define to 1 if you have the `localtime_r' function. */
#define HAVE_LOCALTIME_R 1

/* Define to 1 if the system has the type `long long int'. */
#define HAVE_LONG_LONG_INT 1

/* Define if the 'malloc' function is POSIX compliant. */
#define HAVE_MALLOC_POSIX 1

/* Define to 1 if mmap()'s MAP_ANONYMOUS flag is available after including
config.h and <sys/mman.h>. */
#define HAVE_MAP_ANONYMOUS 1

/* Define to 1 if you have the `mbrtowc' function. */
#define HAVE_MBRTOWC 1

/* Define to 1 if you have the `memchr' function. */
#define HAVE_MEMCHR 1

/* Define to 1 if you have the `memmem' function. */
#define HAVE_MEMMEM 1

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define to 1 if you have the `memset' function. */
#define HAVE_MEMSET 1

/* Define to 1 if <limits.h> defines the MIN and MAX macros. */
/* #undef HAVE_MINMAX_IN_LIMITS_H */


/* Define to 1 on MSVC platforms that have the "invalid parameter handler"
concept. */
/* #undef HAVE_MSVC_INVALID_PARAMETER_HANDLER */

/* Define to 1 if you have the `nanotime' function. */
/* #undef HAVE_NANOTIME */

/* Define to 1 if you have the <ndir.h> header file, and it defines `DIR'. */
/* #undef HAVE_NDIR_H */

/* Define to 1 if you have the <netdb.h> header file. */
#define HAVE_NETDB_H 1

/* Define to 1 if you have the <netinet/in.h> header file. */
#define HAVE_NETINET_IN_H 1

/* Define to 1 if you have the <OS.h> header file. */
/* #undef HAVE_OS_H */

/* Define this if pathfind(3) works */
/* #undef HAVE_PATHFIND */

/* Define to 1 if the system has the type `pid_t'. */
#define HAVE_PID_T 1

/* Define to 1 if you have the `pthread_mutex_lock' function. */
#define HAVE_PTHREAD_MUTEX_LOCK 1

/* Define to 1 if the system has the type `ptrdiff_t'. */
#define HAVE_PTRDIFF_T 1

/* Define to 1 if you have the `putenv' function. */
#define HAVE_PUTENV 1

/* Define to 1 if accept is declared even after undefining macros. */
#define HAVE_RAW_DECL_ACCEPT 1

/* Define to 1 if accept4 is declared even after undefining macros. */
#define HAVE_RAW_DECL_ACCEPT4 1

/* Define to 1 if atoll is declared even after undefining macros. */
#define HAVE_RAW_DECL_ATOLL 1

/* Define to 1 if bind is declared even after undefining macros. */
#define HAVE_RAW_DECL_BIND 1

/* Define to 1 if btowc is declared even after undefining macros. */
#define HAVE_RAW_DECL_BTOWC 1

/* Define to 1 if canonicalize_file_name is declared even after undefining
macros. */
/* #undef HAVE_RAW_DECL_CANONICALIZE_FILE_NAME */

/* Define to 1 if chdir is declared even after undefining macros. */
#define HAVE_RAW_DECL_CHDIR 1

/* Define to 1 if chown is declared even after undefining macros. */
#define HAVE_RAW_DECL_CHOWN 1

/* Define to 1 if connect is declared even after undefining macros. */
#define HAVE_RAW_DECL_CONNECT 1

/* Define to 1 if dprintf is declared even after undefining macros. */
#define HAVE_RAW_DECL_DPRINTF 1

/* Define to 1 if dup is declared even after undefining macros. */
#define HAVE_RAW_DECL_DUP 1

/* Define to 1 if dup2 is declared even after undefining macros. */
#define HAVE_RAW_DECL_DUP2 1

/* Define to 1 if dup3 is declared even after undefining macros. */
#define HAVE_RAW_DECL_DUP3 1

/* Define to 1 if endusershell is declared even after undefining macros. */
/* #undef HAVE_RAW_DECL_ENDUSERSHELL */

/* Define to 1 if euidaccess is declared even after undefining macros. */
/* #undef HAVE_RAW_DECL_EUIDACCESS */

/* Define to 1 if faccessat is declared even after undefining macros. */
#define HAVE_RAW_DECL_FACCESSAT 1

/* Define to 1 if fchdir is declared even after undefining macros. */
#define HAVE_RAW_DECL_FCHDIR 1

/* Define to 1 if fchmodat is declared even after undefining macros. */
#define HAVE_RAW_DECL_FCHMODAT 1

/* Define to 1 if fchownat is declared even after undefining macros. */
#define HAVE_RAW_DECL_FCHOWNAT 1

/* Define to 1 if fcntl is declared even after undefining macros. */
#define HAVE_RAW_DECL_FCNTL 1

/* Define to 1 if fdatasync is declared even after undefining macros. */
#define HAVE_RAW_DECL_FDATASYNC 1

/* Define to 1 if ffs is declared even after undefining macros. */
#define HAVE_RAW_DECL_FFS 1

/* Define to 1 if ffsl is declared even after undefining macros. */
/* #undef HAVE_RAW_DECL_FFSL */

/* Define to 1 if ffsll is declared even after undefining macros. */
/* #undef HAVE_RAW_DECL_FFSLL */

/* Define to 1 if fpurge is declared even after undefining macros. */
#define HAVE_RAW_DECL_FPURGE 1

/* Define to 1 if freeaddrinfo is declared even after undefining macros. */
#define HAVE_RAW_DECL_FREEADDRINFO 1

/* Define to 1 if fseeko is declared even after undefining macros. */
#define HAVE_RAW_DECL_FSEEKO 1

/* Define to 1 if fstat is declared even after undefining macros. */
#define HAVE_RAW_DECL_FSTAT 1

/* Define to 1 if fstatat is declared even after undefining macros. */
#define HAVE_RAW_DECL_FSTATAT 1

/* Define to 1 if fsync is declared even after undefining macros. */
#define HAVE_RAW_DECL_FSYNC 1

/* Define to 1 if ftello is declared even after undefining macros. */
#define HAVE_RAW_DECL_FTELLO 1

/* Define to 1 if ftruncate is declared even after undefining macros. */
#define HAVE_RAW_DECL_FTRUNCATE 1

/* Define to 1 if futimens is declared even after undefining macros. */
#define HAVE_RAW_DECL_FUTIMENS 1

/* Define to 1 if gai_strerror is declared even after undefining macros. */
#define HAVE_RAW_DECL_GAI_STRERROR 1

/* Define to 1 if getaddrinfo is declared even after undefining macros. */
#define HAVE_RAW_DECL_GETADDRINFO 1

/* Define to 1 if getcwd is declared even after undefining macros. */
#define HAVE_RAW_DECL_GETCWD 1

/* Define to 1 if getdelim is declared even after undefining macros. */
#define HAVE_RAW_DECL_GETDELIM 1

/* Define to 1 if getdomainname is declared even after undefining macros. */
/* #undef HAVE_RAW_DECL_GETDOMAINNAME */

/* Define to 1 if getdtablesize is declared even after undefining macros. */
/* #undef HAVE_RAW_DECL_GETDTABLESIZE */


/* Define to 1 if getnameinfo is declared even after undefining macros. */
#define HAVE_RAW_DECL_GETNAMEINFO 1

/* Define to 1 if getpeername is declared even after undefining macros. */
#define HAVE_RAW_DECL_GETPEERNAME 1

/* Define to 1 if getsockname is declared even after undefining macros. */
#define HAVE_RAW_DECL_GETSOCKNAME 1

/* Define to 1 if getsockopt is declared even after undefining macros. */
#define HAVE_RAW_DECL_GETSOCKOPT 1

/* Define to 1 if getsubopt is declared even after undefining macros. */
/* #undef HAVE_RAW_DECL_GETSUBOPT */

/* Define to 1 if gettimeofday is declared even after undefining macros. */
#define HAVE_RAW_DECL_GETTIMEOFDAY 1

/* Define to 1 if getusershell is declared even after undefining macros. */
/* #undef HAVE_RAW_DECL_GETUSERSHELL */

/* Define to 1 if grantpt is declared even after undefining macros. */
#define HAVE_RAW_DECL_GRANTPT 1


/* Define to 1 if imaxabs is declared even after undefining macros. */
#define HAVE_RAW_DECL_IMAXABS 1

/* Define to 1 if imaxdiv is declared even after undefining macros. */
#define HAVE_RAW_DECL_IMAXDIV 1

/* Define to 1 if inet_ntop is declared even after undefining macros. */
#define HAVE_RAW_DECL_INET_NTOP 1

/* Define to 1 if inet_pton is declared even after undefining macros. */
#define HAVE_RAW_DECL_INET_PTON 1

/* Define to 1 if initstate is declared even after undefining macros. */
#define HAVE_RAW_DECL_INITSTATE 1

/* Define to 1 if initstate_r is declared even after undefining macros. */
/* #undef HAVE_RAW_DECL_INITSTATE_R */

/* Define to 1 if isatty is declared even after undefining macros. */
#define HAVE_RAW_DECL_ISATTY 1

/* Define to 1 if isblank is declared even after undefining macros. */
#define HAVE_RAW_DECL_ISBLANK 1

/* Define to 1 if lchmod is declared even after undefining macros. */
/* #undef HAVE_RAW_DECL_LCHMOD */

/* Define to 1 if lchown is declared even after undefining macros. */
#define HAVE_RAW_DECL_LCHOWN 1

/* Define to 1 if link is declared even after undefining macros. */
#define HAVE_RAW_DECL_LINK 1

/* Define to 1 if linkat is declared even after undefining macros. */
#define HAVE_RAW_DECL_LINKAT 1

/* Define to 1 if listen is declared even after undefining macros. */
#define HAVE_RAW_DECL_LISTEN 1

/* Define to 1 if lseek is declared even after undefining macros. */
#define HAVE_RAW_DECL_LSEEK 1

/* Define to 1 if lstat is declared even after undefining macros. */
#define HAVE_RAW_DECL_LSTAT 1

/* Define to 1 if mbrlen is declared even after undefining macros. */
#define HAVE_RAW_DECL_MBRLEN 1

/* Define to 1 if mbrtowc is declared even after undefining macros. */
#define HAVE_RAW_DECL_MBRTOWC 1

/* Define to 1 if mbsinit is declared even after undefining macros. */
#define HAVE_RAW_DECL_MBSINIT 1

/* Define to 1 if mbsnrtowcs is declared even after undefining macros. */
#define HAVE_RAW_DECL_MBSNRTOWCS 1

/* Define to 1 if mbsrtowcs is declared even after undefining macros. */
#define HAVE_RAW_DECL_MBSRTOWCS 1

/* Define to 1 if memmem is declared even after undefining macros. */
#define HAVE_RAW_DECL_MEMMEM 1

/* Define to 1 if mempcpy is declared even after undefining macros. */
/* #undef HAVE_RAW_DECL_MEMPCPY */

/* Define to 1 if memrchr is declared even after undefining macros. */
#define HAVE_RAW_DECL_MEMRCHR 1

/* Define to 1 if mkdirat is declared even after undefining macros. */
#define HAVE_RAW_DECL_MKDIRAT 1

/* Define to 1 if mkdtemp is declared even after undefining macros. */
#define HAVE_RAW_DECL_MKDTEMP 1

/* Define to 1 if mkfifo is declared even after undefining macros. */
#define HAVE_RAW_DECL_MKFIFO 1

/* Define to 1 if mkfifoat is declared even after undefining macros. */
/* #undef HAVE_RAW_DECL_MKFIFOAT */

/* Define to 1 if mknod is declared even after undefining macros. */
#define HAVE_RAW_DECL_MKNOD 1

/* Define to 1 if mknodat is declared even after undefining macros. */
#define HAVE_RAW_DECL_MKNODAT 1

/* Define to 1 if mkostemp is declared even after undefining macros. */
/* #undef HAVE_RAW_DECL_MKOSTEMP */

/* Define to 1 if mkostemps is declared even after undefining macros. */
/* #undef HAVE_RAW_DECL_MKOSTEMPS */

/* Define to 1 if mkstemp is declared even after undefining macros. */
#define HAVE_RAW_DECL_MKSTEMP 1

/* Define to 1 if mkstemps is declared even after undefining macros. */
/* #undef HAVE_RAW_DECL_MKSTEMPS */

/* Define to 1 if openat is declared even after undefining macros. */
#define HAVE_RAW_DECL_OPENAT 1

/* Define to 1 if pclose is declared even after undefining macros. */
#define HAVE_RAW_DECL_PCLOSE 1

/* Define to 1 if pipe is declared even after undefining macros. */
#define HAVE_RAW_DECL_PIPE 1

/* Define to 1 if pipe2 is declared even after undefining macros. */
#define HAVE_RAW_DECL_PIPE2 1

/* Define to 1 if popen is declared even after undefining macros. */
#define HAVE_RAW_DECL_POPEN 1

/* Define to 1 if posix_openpt is declared even after undefining macros. */
#define HAVE_RAW_DECL_POSIX_OPENPT 1

/* Define to 1 if pread is declared even after undefining macros. */
#define HAVE_RAW_DECL_PREAD 1

/* Define to 1 if pselect is declared even after undefining macros. */
#define HAVE_RAW_DECL_PSELECT 1

/* Define to 1 if pthread_sigmask is declared even after undefining macros. */
/* #undef HAVE_RAW_DECL_PTHREAD_SIGMASK */

/* Define to 1 if ptsname is declared even after undefining macros. */
#define HAVE_RAW_DECL_PTSNAME 1

/* Define to 1 if ptsname_r is declared even after undefining macros. */
#define HAVE_RAW_DECL_PTSNAME_R 1

/* Define to 1 if pwrite is declared even after undefining macros. */
#define HAVE_RAW_DECL_PWRITE 1

/* Define to 1 if random is declared even after undefining macros. */
#define HAVE_RAW_DECL_RANDOM 1

/* Define to 1 if random_r is declared even after undefining macros. */
/* #undef HAVE_RAW_DECL_RANDOM_R */

/* Define to 1 if rawmemchr is declared even after undefining macros. */
/* #undef HAVE_RAW_DECL_RAWMEMCHR */

/* Define to 1 if readlink is declared even after undefining macros. */
#define HAVE_RAW_DECL_READLINK 1

/* Define to 1 if readlinkat is declared even after undefining macros. */
#define HAVE_RAW_DECL_READLINKAT 1

/* Define to 1 if realpath is declared even after undefining macros. */
#define HAVE_RAW_DECL_REALPATH 1

/* Define to 1 if recv is declared even after undefining macros. */
#define HAVE_RAW_DECL_RECV 1

/* Define to 1 if recvfrom is declared even after undefining macros. */
#define HAVE_RAW_DECL_RECVFROM 1

/* Define to 1 if renameat is declared even after undefining macros. */
#define HAVE_RAW_DECL_RENAMEAT 1

/* Define to 1 if rmdir is declared even after undefining macros. */
#define HAVE_RAW_DECL_RMDIR 1

/* Define to 1 if rpmatch is declared even after undefining macros. */
/* #undef HAVE_RAW_DECL_RPMATCH */


/* Define to 1 if select is declared even after undefining macros. */
#define HAVE_RAW_DECL_SELECT 1

/* Define to 1 if send is declared even after undefining macros. */
#define HAVE_RAW_DECL_SEND 1

/* Define to 1 if sendto is declared even after undefining macros. */
#define HAVE_RAW_DECL_SENDTO 1

/* Define to 1 if setenv is declared even after undefining macros. */
#define HAVE_RAW_DECL_SETENV 1

/* Define to 1 if sethostname is declared even after undefining macros. */
/* #undef HAVE_RAW_DECL_SETHOSTNAME */

/* Define to 1 if setsockopt is declared even after undefining macros. */
#define HAVE_RAW_DECL_SETSOCKOPT 1

/* Define to 1 if setstate is declared even after undefining macros. */
#define HAVE_RAW_DECL_SETSTATE 1

/* Define to 1 if setstate_r is declared even after undefining macros. */
/* #undef HAVE_RAW_DECL_SETSTATE_R */

/* Define to 1 if setusershell is declared even after undefining macros. */
/* #undef HAVE_RAW_DECL_SETUSERSHELL */

/* Define to 1 if shutdown is declared even after undefining macros. */
#define HAVE_RAW_DECL_SHUTDOWN 1

/* Define to 1 if sleep is declared even after undefining macros. */
#define HAVE_RAW_DECL_SLEEP 1

/* Define to 1 if snprintf is declared even after undefining macros. */
#define HAVE_RAW_DECL_SNPRINTF 1

/* Define to 1 if socket is declared even after undefining macros. */
#define HAVE_RAW_DECL_SOCKET 1

/* Define to 1 if srandom is declared even after undefining macros. */
#define HAVE_RAW_DECL_SRANDOM 1

/* Define to 1 if srandom_r is declared even after undefining macros. */
/* #undef HAVE_RAW_DECL_SRANDOM_R */

/* Define to 1 if stat is declared even after undefining macros. */
#define HAVE_RAW_DECL_STAT 1

/* Define to 1 if stpcpy is declared even after undefining macros. */
#define HAVE_RAW_DECL_STPCPY 1

/* Define to 1 if stpncpy is declared even after undefining macros. */
#define HAVE_RAW_DECL_STPNCPY 1

/* Define to 1 if strcasecmp is declared even after undefining macros. */
#define HAVE_RAW_DECL_STRCASECMP 1

/* Define to 1 if strcasestr is declared even after undefining macros. */
#define HAVE_RAW_DECL_STRCASESTR 1

/* Define to 1 if strchrnul is declared even after undefining macros. */
/* #undef HAVE_RAW_DECL_STRCHRNUL */

/* Define to 1 if strdup is declared even after undefining macros. */
#define HAVE_RAW_DECL_STRDUP 1

/* Define to 1 if strerror_r is declared even after undefining macros. */
#define HAVE_RAW_DECL_STRERROR_R 1

/* Define to 1 if strncasecmp is declared even after undefining macros. */
#define HAVE_RAW_DECL_STRNCASECMP 1

/* Define to 1 if strncat is declared even after undefining macros. */
#define HAVE_RAW_DECL_STRNCAT 1

/* Define to 1 if strndup is declared even after undefining macros. */
#define HAVE_RAW_DECL_STRNDUP 1

/* Define to 1 if strnlen is declared even after undefining macros. */
#define HAVE_RAW_DECL_STRNLEN 1

/* Define to 1 if strpbrk is declared even after undefining macros. */
#define HAVE_RAW_DECL_STRPBRK 1

/* Define to 1 if strsep is declared even after undefining macros. */
#define HAVE_RAW_DECL_STRSEP 1

/* Define to 1 if strsignal is declared even after undefining macros. */
#define HAVE_RAW_DECL_STRSIGNAL 1

/* Define to 1 if strtod is declared even after undefining macros. */
#define HAVE_RAW_DECL_STRTOD 1

/* Define to 1 if strtoimax is declared even after undefining macros. */
#define HAVE_RAW_DECL_STRTOIMAX 1

/* Define to 1 if strtok_r is declared even after undefining macros. */
#define HAVE_RAW_DECL_STRTOK_R 1

/* Define to 1 if strtoll is declared even after undefining macros. */
#define HAVE_RAW_DECL_STRTOLL 1

/* Define to 1 if strtoull is declared even after undefining macros. */
#define HAVE_RAW_DECL_STRTOULL 1

/* Define to 1 if strtoumax is declared even after undefining macros. */
#define HAVE_RAW_DECL_STRTOUMAX 1

/* Define to 1 if strverscmp is declared even after undefining macros. */
/* #undef HAVE_RAW_DECL_STRVERSCMP */

/* Define to 1 if symlink is declared even after undefining macros. */
#define HAVE_RAW_DECL_SYMLINK 1

/* Define to 1 if symlinkat is declared even after undefining macros. */
#define HAVE_RAW_DECL_SYMLINKAT 1

/* Define to 1 if tmpfile is declared even after undefining macros. */
#define HAVE_RAW_DECL_TMPFILE 1

/* Define to 1 if ttyname_r is declared even after undefining macros. */
#define HAVE_RAW_DECL_TTYNAME_R 1

/* Define to 1 if unlink is declared even after undefining macros. */
#define HAVE_RAW_DECL_UNLINK 1

/* Define to 1 if unlinkat is declared even after undefining macros. */
#define HAVE_RAW_DECL_UNLINKAT 1

/* Define to 1 if unlockpt is declared even after undefining macros. */
#define HAVE_RAW_DECL_UNLOCKPT 1

/* Define to 1 if unsetenv is declared even after undefining macros. */
#define HAVE_RAW_DECL_UNSETENV 1

/* Define to 1 if usleep is declared even after undefining macros. */
#define HAVE_RAW_DECL_USLEEP 1

/* Define to 1 if utimensat is declared even after undefining macros. */
#define HAVE_RAW_DECL_UTIMENSAT 1

/* Define to 1 if vdprintf is declared even after undefining macros. */
#define HAVE_RAW_DECL_VDPRINTF 1

/* Define to 1 if vsnprintf is declared even after undefining macros. */
#define HAVE_RAW_DECL_VSNPRINTF 1

/* Define to 1 if wcpcpy is declared even after undefining macros. */
/* #undef HAVE_RAW_DECL_WCPCPY */

/* Define to 1 if wcpncpy is declared even after undefining macros. */
/* #undef HAVE_RAW_DECL_WCPNCPY */

/* Define to 1 if wcrtomb is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCRTOMB 1

/* Define to 1 if wcscasecmp is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCSCASECMP 1

/* Define to 1 if wcscat is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCSCAT 1

/* Define to 1 if wcschr is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCSCHR 1

/* Define to 1 if wcscmp is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCSCMP 1

/* Define to 1 if wcscoll is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCSCOLL 1

/* Define to 1 if wcscpy is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCSCPY 1

/* Define to 1 if wcscspn is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCSCSPN 1

/* Define to 1 if wcsdup is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCSDUP 1

/* Define to 1 if wcslen is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCSLEN 1

/* Define to 1 if wcsncasecmp is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCSNCASECMP 1

/* Define to 1 if wcsncat is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCSNCAT 1

/* Define to 1 if wcsncmp is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCSNCMP 1

/* Define to 1 if wcsncpy is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCSNCPY 1

/* Define to 1 if wcsnlen is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCSNLEN 1

/* Define to 1 if wcsnrtombs is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCSNRTOMBS 1

/* Define to 1 if wcspbrk is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCSPBRK 1

/* Define to 1 if wcsrchr is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCSRCHR 1

/* Define to 1 if wcsrtombs is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCSRTOMBS 1

/* Define to 1 if wcsspn is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCSSPN 1

/* Define to 1 if wcsstr is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCSSTR 1

/* Define to 1 if wcstok is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCSTOK 1

/* Define to 1 if wcswidth is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCSWIDTH 1

/* Define to 1 if wcsxfrm is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCSXFRM 1

/* Define to 1 if wctob is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCTOB 1

/* Define to 1 if wcwidth is declared even after undefining macros. */
#define HAVE_RAW_DECL_WCWIDTH 1

/* Define to 1 if wmemchr is declared even after undefining macros. */
#define HAVE_RAW_DECL_WMEMCHR 1

/* Define to 1 if wmemcmp is declared even after undefining macros. */
#define HAVE_RAW_DECL_WMEMCMP 1

/* Define to 1 if wmemcpy is declared even after undefining macros. */
#define HAVE_RAW_DECL_WMEMCPY 1

/* Define to 1 if wmemmove is declared even after undefining macros. */
#define HAVE_RAW_DECL_WMEMMOVE 1

/* Define to 1 if wmemset is declared even after undefining macros. */
#define HAVE_RAW_DECL_WMEMSET 1

/* Define to 1 if _Exit is declared even after undefining macros. */
#define HAVE_RAW_DECL__EXIT 1

/* Define if the 'realloc' function is POSIX compliant. */
#define HAVE_REALLOC_POSIX 1


/* Define to 1 if you have the `regcomp' function. */
#define HAVE_REGCOMP 1

/* Define to 1 if you have the <runetype.h> header file. */
/* #undef HAVE_RUNETYPE_H */

/* Define to 1 if the system has the type `sa_family_t'. */
#define HAVE_SA_FAMILY_T 1

/* Define to 1 if you have the `scandir' function. */
#define HAVE_SCANDIR 1

/* Define to 1 if you have the `scm_gc_malloc_pointerless' function. */
/* #undef HAVE_SCM_GC_MALLOC_POINTERLESS */


/* Define to 1 if you have the <setjmp.h> header file. */
#define HAVE_SETJMP_H 1

/* Define to 1 if you have the `shutdown' function. */
#define HAVE_SHUTDOWN 1

/* Define to 1 if the system has the type `size_t'. */
#define HAVE_SIZE_T 1

/* Define to 1 if you have the `snprintf' function. */
#define HAVE_SNPRINTF 1

/* Define if the return value of the snprintf function is the number of of
bytes (excluding the terminating NUL) that would have been produced if the
buffer had been large enough. */
/* #undef HAVE_SNPRINTF_RETVAL_C99 */

/* Define to 1 if you have the `socket' function. */
#define HAVE_SOCKET 1

/* Define to 1 if you have the <stdarg.h> header file. */
#define HAVE_STDARG_H 1

/* Define to 1 if you have the <stdbool.h> header file. */
#define HAVE_STDBOOL_H 1

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define if <stdint.h> exists, doesn't clash with <sys/types.h>, and declares
uintmax_t. */
#define HAVE_STDINT_H_WITH_UINTMAX 1

/* Define to 1 if you have the <stdio_ext.h> header file. */
/* #undef HAVE_STDIO_EXT_H */

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the `strcasecmp' function. */
#define HAVE_STRCASECMP 1

/* Define to 1 if you have the `strchr' function. */
#define HAVE_STRCHR 1

/* Define to 1 if you have the `strdup' function. */
#define HAVE_STRDUP 1


/* Define this if strftime() works */
/* #undef HAVE_STRFTIME */

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the `strncasecmp' function. */
#define HAVE_STRNCASECMP 1

/* Define to 1 if you have the `strndup' function. */
#define HAVE_STRNDUP 1

/* Define to 1 if you have the `strnlen' function. */
#define HAVE_STRNLEN 1

/* Define to 1 if you have the `strrchr' function. */
#define HAVE_STRRCHR 1

/* Define to 1 if you have the `strsignal' function. */
#define HAVE_STRSIGNAL 1

/* Define to 1 if you have the `strstr' function. */
#define HAVE_STRSTR 1

/* Define to 1 if you have the `strtok_r' function. */
#define HAVE_STRTOK_R 1

/* Define to 1 if you have the `strtoul' function. */
#define HAVE_STRTOUL 1

/* Define to 1 if the system has the type `struct addrinfo'. */
#define HAVE_STRUCT_ADDRINFO 1

/* Define to 1 if `sa_len' is a member of `struct sockaddr'. */
/* #undef HAVE_STRUCT_SOCKADDR_SA_LEN */

/* Define to 1 if the system has the type `struct sockaddr_storage'. */
#define HAVE_STRUCT_SOCKADDR_STORAGE 1

/* Define to 1 if `ss_family' is a member of `struct sockaddr_storage'. */
#define HAVE_STRUCT_SOCKADDR_STORAGE_SS_FAMILY 1

/* Define to 1 if `tm_zone' is a member of `struct tm'. */
#define HAVE_STRUCT_TM_TM_ZONE 1

/* Define to 1 if you have the `strverscmp' function. */
/* #undef HAVE_STRVERSCMP */

/* Define to 1 if you have the <sysexits.h> header file. */
/* #undef HAVE_SYSEXITS_H */

/* Define to 1 if you have the <sys/bitypes.h> header file. */
/* #undef HAVE_SYS_BITYPES_H */

/* Define to 1 if you have the <sys/dir.h> header file, and it defines `DIR'.
*/
/* #undef HAVE_SYS_DIR_H */

/* Define to 1 if you have the <sys/inttypes.h> header file. */
/* #undef HAVE_SYS_INTTYPES_H */

/* Define to 1 if you have the <sys/limits.h> header file. */
/* #undef HAVE_SYS_LIMITS_H */
/* Define to 1 if you have the <sys/ndir.h> header file, and it defines `DIR'.
*/
/* #undef HAVE_SYS_NDIR_H */

/* Define to 1 if you have the <sys/procset.h> header file. */
/* #undef HAVE_SYS_PROCSET_H */

/* Define to 1 if you have the <sys/select.h> header file. */
//#define HAVE_SYS_SELECT_H 1 // defined in NlcCompilerConfig.h

/* Define to 1 if you have the <sys/socket.h> header file. */
//#define HAVE_SYS_SOCKET_H 1 // defined in NlcCompilerConfig.h

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/stropts.h> header file. */
/* #undef HAVE_SYS_STROPTS_H */

/* Define to 1 if you have the <sys/timeb.h> header file. */
/* #undef HAVE_SYS_TIMEB_H */

/* Define to 1 if you have the <sys/time.h> header file. */
#define HAVE_SYS_TIME_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the <sys/uio.h> header file. */
#define HAVE_SYS_UIO_H 1


/* Define to 1 if you have the `tcgetattr' function. */
#define HAVE_TCGETATTR 1

/* Define to 1 if you have the `tcsetattr' function. */
#define HAVE_TCSETATTR 1

/* Define to 1 if your `struct tm' has `tm_zone'. Deprecated, use
`HAVE_STRUCT_TM_TM_ZONE' instead. */
#define HAVE_TM_ZONE 1

/* Enable TPM */
/* #undef HAVE_TROUSERS */

/* Define to 1 if you don't have `tm_zone' but do have the external array
`tzname'. */
/* #undef HAVE_TZNAME */

/* Define to 1 if you have the `vasnprintf' function. */
/* #undef HAVE_VASNPRINTF */

/* Define to 1 if you have the `vasprintf' function. */
#define HAVE_VASPRINTF 1

/* Have va_copy() */
/* #undef HAVE_VA_COPY */

/* Define to 1 if you have the `vfork' function. */
#define HAVE_VFORK 1

/* Define to 1 if you have the <vfork.h> header file. */
/* #undef HAVE_VFORK_H */

/* Define to 1 if you have the `vprintf' function. */
#define HAVE_VPRINTF 1

/* Define to 1 if you have the `vsnprintf' function. */
#define HAVE_VSNPRINTF 1

/* Define to 1 if you have the <wchar_gnu.h> header file. */
#define HAVE_WCHAR_H 1

/* Define to 1 if the system has the type `wchar_t'. */
#define HAVE_WCHAR_T 1

/* Define to 1 if you have the `wcrtomb' function. */
#define HAVE_WCRTOMB 1

/* Define to 1 if you have the `wcslen' function. */
#define HAVE_WCSLEN 1

/* Define to 1 if you have the `wcsnlen' function. */
#define HAVE_WCSNLEN 1

/* Define to 1 if you have the <winsock2.h> header file. */
/* #undef HAVE_WINSOCK2_H */

/* Define to 1 if `fork' works. */
#define HAVE_WORKING_FORK 1

/* Define to 1 if O_NOATIME works. */
#define HAVE_WORKING_O_NOATIME 0

/* Define to 1 if O_NOFOLLOW works. */
#define HAVE_WORKING_O_NOFOLLOW 0

/* Define to 1 if `vfork' works. */
#define HAVE_WORKING_VFORK 1

/* Define to 1 if you have the <ws2tcpip.h> header file. */
/* #undef HAVE_WS2TCPIP_H */

/* Define to 1 if the system has the type `_Bool'. */
#define HAVE__BOOL 1

/* Define to 1 if you have the `_fseeki64' function. */
/* #undef HAVE__FSEEKI64 */

/* Define to 1 if you have the `_ftelli64' function. */
/* #undef HAVE__FTELLI64 */

/* Define to 1 if you have the `_ftime' function. */
/* #undef HAVE__FTIME */

/* Define to 1 if you have the `_set_invalid_parameter_handler' function. */
/* #undef HAVE__SET_INVALID_PARAMETER_HANDLER */

/* Define to 1 if you have the `__fsetlocking' function. */
/* #undef HAVE___FSETLOCKING */

/* Define to 1 if you have the `__register_atfork' function. */
/* #undef HAVE___REGISTER_ATFORK */

/* Define to 1 if you have the `__secure_getenv' function. */
/* #undef HAVE___SECURE_GETENV */

/* Have __va_copy() */
/* #undef HAVE___VA_COPY */

/* Define as const if the declaration of iconv() needs const. */
#define ICONV_CONST 

/* Define to 1 if lseek does not detect pipes. */
/* #undef LSEEK_PIPE_BROKEN */

/* Define to the sub-directory in which libtool stores uninstalled libraries.
*/
#define LT_OBJDIR ".libs/"

/* If malloc(0) is != NULL, define this to 1. Otherwise define this to 0. */
#define MALLOC_0_IS_NONNULL 1

/* Define to a substitute value for mmap()'s MAP_ANONYMOUS flag. */
/* #undef MAP_ANONYMOUS */

/* Define this if optional arguments are disallowed */
/* #undef NO_OPTIONAL_OPT_ARGS */

/* no ssize_t type was found */
/* #undef NO_SSIZE_T */


/* name of regex header file */
#ifdef _MSC_VER
# define REGEX_HEADER <NlcDependLibrariesConfig.h>
#else
# define REGEX_HEADER <regex.h>
#endif // _MSC_VER


/* Define to l, ll, u, ul, ull, etc., as suitable for constants of type
'size_t'. */
/* #undef SIZE_T_SUFFIX */

/* If using the C implementation of alloca, define if you know the
direction of stack growth for your system; otherwise it will be
automatically deduced at runtime.
STACK_DIRECTION > 0 => grows toward higher addresses
STACK_DIRECTION < 0 => grows toward lower addresses
STACK_DIRECTION = 0 => direction of growth unknown */
/* #undef STACK_DIRECTION */

/* Define to 1 if the `S_IS*' macros in <sys/stat.h> do not work properly. */
/* #undef STAT_MACROS_BROKEN */

/* Define to 1 if strerror_r returns char *. */
/* #undef STRERROR_R_CHAR_P */

/* The system priority file */
#define SYSTEM_PRIORITY_FILE "/etc/gnutls/default-priorities"

/* Define to 1 if all 'time_t' values fit in a 'long int'. */
#define TIME_T_FITS_IN_LONG_INT 1


/* the location of the trousers library */
#define TROUSERS_LIB "/usr/lib/libtspi.so.1"

/* The DNSSEC root key file */
#define UNBOUND_ROOT_KEY_FILE "/usr/share/dns/root.key"

/* Enable extensions on AIX 3, Interix.  */
#ifndef _ALL_SOURCE
# define _ALL_SOURCE 1
#endif
/* Enable general extensions on OS X.  */
#ifndef _DARWIN_C_SOURCE
# define _DARWIN_C_SOURCE 1
#endif
/* Enable GNU extensions on systems that have them.  */
#ifndef _GNU_SOURCE
# define _GNU_SOURCE 1
#endif
/* Use GNU style printf and scanf.  */
#ifndef __USE_MINGW_ANSI_STDIO
# define __USE_MINGW_ANSI_STDIO 1
#endif
/* Enable threading extensions on Solaris.  */
#ifndef _POSIX_PTHREAD_SEMANTICS
# define _POSIX_PTHREAD_SEMANTICS 1
#endif
/* Enable extensions on HP NonStop.  */
#ifndef _TANDEM_SOURCE
# define _TANDEM_SOURCE 1
#endif
/* Enable X/Open extensions if necessary.  HP-UX 11.11 defines
mbstate_t only if _XOPEN_SOURCE is defined to 500, regardless of
whether compiling with -Ae or -D_HPUX_SOURCE=1.  */
#ifndef _XOPEN_SOURCE
/* # undef _XOPEN_SOURCE */
#endif
/* Enable general extensions on Solaris.  */
#ifndef __EXTENSIONS__
# define __EXTENSIONS__ 1
#endif


/* Enable large inode numbers on Mac OS X 10.5. */
#define _DARWIN_USE_64_BIT_INODE 1

/* static lib rename */
#define fread_file _gnutls_fread_file

/* Define to `int' if <sys/types.h> doesn't define. */
/* #undef gid_t */

/* Define to long or long long if <stdint.h> and <inttypes.h> don't define. */
/* #undef intmax_t */

/* Work around a bug in Apple GCC 4.0.1 build 5465: In C99 mode, it supports
the ISO C 99 semantics of 'extern NLC_INLINE' (unlike the GNU C semantics of
earlier versions), but does not display it by setting __GNUC_STDC_INLINE__.
__APPLE__ && __MACH__ test for Mac OS X.
__APPLE_CC__ tests for the Apple compiler and its version.
__STDC_VERSION__ tests for the C99 mode.  */
#if defined __APPLE__ && defined __MACH__ && __APPLE_CC__ >= 5465 && !defined __cplusplus && __STDC_VERSION__ >= 199901L && !defined __GNUC_STDC_INLINE__
# define __GNUC_STDC_INLINE__ 1
#endif

/* Define to `int' if <sys/types.h> does not define. */
/* #undef mode_t */

/* Define to the type of st_nlink in struct stat, or a supertype. */
/* #undef nlink_t */

/* Define to `int' if <sys/types.h> does not define. */
/* #undef pid_t */

/* Define as the type of the result of subtracting two pointers, if the system
doesn't define it. */
/* #undef ptrdiff_t */

/* static lib rename */
//#define read_binary_file _gnutls_read_binary_file

/* static lib rename */
//#define read_file _gnutls_read_file

/* Define to the equivalent of the C99 'NLC_RESTRICT' keyword, or to
nothing if this is not supported.  Do not define if NLC_RESTRICT is
supported directly.  */
//#define NLC_RESTRICT __restrict
/* Work around a bug in Sun C++: it does not support _Restrict or
__restrict__, even though the corresponding Sun C compiler ends up with
"#define NLC_RESTRICT _Restrict" or "#define NLC_RESTRICT __restrict__" in the
previous line.  Perhaps some future version of Sun C++ will work with
NLC_RESTRICT; if so, hopefully it defines __RESTRICT like Sun C does.  */
//#if defined __SUNPRO_CC && !defined __RESTRICT
//# define _Restrict
//# define __restrict__
//#endif

/* Define to `unsigned int' if <sys/types.h> does not define. */
/* #undef size_t */

/* type to use in place of socklen_t if not defined */
/* #undef socklen_t */

/* Define as a signed type of the same size as size_t. */
/* #undef ssize_t */

/* Define to `int' if <sys/types.h> doesn't define. */
/* #undef uid_t */

/* Define as a marker that can be attached to declarations that might not
be used.  This helps to reduce warnings, such as from
GCC -Wunused-parameter.  */
#define _UNUSED_PARAMETER_  NLC_UNUSED

/* gcc supports the "unused" attribute on possibly unused labels, and
g++ has since version 4.5.  Note to support C++ as well as C,
_GL_UNUSED_LABEL should be used with a trailing ;  */
#if !defined __cplusplus || __GNUC__ > 4 \
    || (__GNUC__ == 4 && __GNUC_MINOR__ >= 5)
# define _GL_UNUSED_LABEL _GL_UNUSED
#else
# define _GL_UNUSED_LABEL
#endif

#define _GL_ATTRIBUTE_PURE  NLC_ATTRIBUTE_PURE
#define _GL_ATTRIBUTE_CONST  NLC_ATTRIBUTE_CONST


/* Define as `fork' if `vfork' does not work. */
/* #undef vfork */

#define  _GCRY_GCC_ATTR_PURE

typedef ssize_t gpgrt_ssize_t;

/* Enable gpg-error's strerror macro for W32CE.  */
#define GPG_ERR_ENABLE_ERRNO_MACROS 1

/* Define if you don't want the default EGD socket name. For details see
cipher/rndegd.c */
#define EGD_SOCKET_NAME ""

/* Enable support for Intel AES-NI instructions. */
/* #undef ENABLE_AESNI_SUPPORT */

/* Enable support for Intel AVX2 instructions. */
/* #undef ENABLE_AVX2_SUPPORT */

/* Enable support for Intel AVX instructions. */
/* #undef ENABLE_AVX_SUPPORT */

/* Enable support for Intel DRNG (RDRAND instruction). */
/* #undef ENABLE_DRNG_SUPPORT */

/* Define to support an HMAC based integrity check */
/* #undef ENABLE_HMAC_BINARY_CHECK */

/* Enable support for ARM NEON instructions. */
#if defined(TARGET_OS_ANDROID) && HAVE_ARM_NEON
//# define ENABLE_NEON_SUPPORT 1
#endif // TARGET_OS_ANDROID

/* Enable support for the PadLock engine. */
/* #undef ENABLE_PADLOCK_SUPPORT */

/* Enable support for Intel PCLMUL instructions. */
/* #undef ENABLE_PCLMUL_SUPPORT */

/* Define to use the GNU C visibility attribute. */
//#define GCRY_USE_VISIBILITY 1

/* The default error source for libgcrypt. */
#define GPG_ERR_SOURCE_DEFAULT GPG_ERR_SOURCE_GCRYPT

/* Defined if the mlock() call does not work */
/* #undef HAVE_BROKEN_MLOCK */

/* Defined if compiler has '__builtin_bswap32' intrinsic */
#ifndef TARGET_OS_WINDOWS
#define HAVE_BUILTIN_BSWAP32 1
/* Defined if compiler has '__builtin_bswap64' intrinsic */
# define HAVE_BUILTIN_BSWAP64 1
#endif // TARGET_OS_WINDOWS


/* Defined if a `byte' is typedef'd */
/* #undef HAVE_BYTE_TYPEDEF */


/* Define to 1 if you have the `clock_gettime' function. */
#define HAVE_CLOCK_GETTIME 1

/* Defined if underlying assembler is compatible with amd64 assembly
implementations */
/* #undef HAVE_COMPATIBLE_GCC_AMD64_PLATFORM_AS */

/* Defined if underlying assembler is compatible with ARM assembly
implementations */
#if HAVE_ARM_ASM
# define HAVE_COMPATIBLE_GCC_ARM_PLATFORM_AS 1
#endif // HAVE_ARM_ASM

/* Define to 1 if you have the declaration of `sys_siglist', and to 0 if you
don't. */
#define HAVE_DECL_SYS_SIGLIST 1

/* defined if the system supports a random device */
#define HAVE_DEV_RANDOM 1

/* Define to 1 if you have the <dlfcn.h> header file. */
#define HAVE_DLFCN_H 1

/* Define to 1 if you don't have `vprintf' but do have `_doprnt.' */
/* #undef HAVE_DOPRNT */

/* defined if we run on some of the PCDOS like systems (DOS, Windoze. OS/2)
with special properties like no file modes */
/* #undef HAVE_DOSISH_SYSTEM */

/* defined if we must run on a stupid file system */
/* #undef HAVE_DRIVE_LETTERS */

/* Define to 1 if you have the `flockfile' function. */
#define HAVE_FLOCKFILE 1

/* Defined if a GCC style "__attribute__ ((aligned (n))" is supported */
#ifndef _MSC_VER
# define HAVE_GCC_ATTRIBUTE_ALIGNED 1
#endif // _MSC_VER

/* Defined if NLC_INLINE assembler supports AVX instructions */
/* #undef HAVE_GCC_INLINE_ASM_AVX */

/* Defined if NLC_INLINE assembler supports AVX2 instructions */
/* #undef HAVE_GCC_INLINE_ASM_AVX2 */

/* Defined if NLC_INLINE assembler supports BMI2 instructions */
/* #undef HAVE_GCC_INLINE_ASM_BMI2 */

/* Defined if NLC_INLINE assembler supports NEON instructions */
#define HAVE_GCC_INLINE_ASM_NEON 1

/* Defined if NLC_INLINE assembler supports PCLMUL instructions */
/* #undef HAVE_GCC_INLINE_ASM_PCLMUL */

/* Defined if NLC_INLINE assembler supports SSSE3 instructions */
/* #undef HAVE_GCC_INLINE_ASM_SSSE3 */

/* Define to 1 if you have the `gethrtime' function. */
/* #undef HAVE_GETHRTIME */


/* Defined if underlying assembler is compatible with Intel syntax assembly
implementations */
/* #undef HAVE_INTEL_SYNTAX_PLATFORM_AS */



/* Defined if the GNU Pth is available */
/* #undef HAVE_PTH */

/* Define if the <libpthread/pthread.h> defines PTHREAD_MUTEX_RECURSIVE. */
#define HAVE_PTHREAD_MUTEX_RECURSIVE 1
/* Define if the POSIX multithreading library has read/write locks. */
#define HAVE_PTHREAD_RWLOCK 1

/* Define to 1 if you have the `raise' function. */
#define HAVE_RAISE 1

/* Define to 1 if you have the `rand' function. */
#define HAVE_RAND 1





/* Define to 1 if you have the <sys/capability.h> header file. */
/* #undef HAVE_SYS_CAPABILITY_H */

/* Define to 1 if you have the <sys/mman.h> header file. */
/* #undef HAVE_SYS_MMAN_H */


/* Define to 1 if you have the <sys/select.h> header file. */
//#define HAVE_SYS_SELECT_H 1 // defined in NlcCompilerConfig.h

/* Define to 1 if you have the <sys/socket.h> header file. */
//#define HAVE_SYS_SOCKET_H 1 // defined in NlcCompilerConfig.h

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Defined if a `uint16_t' is typedef'd */
/* #undef HAVE_U16_TYPEDEF */

/* Defined if a `uint32_t' is typedef'd */
/* #undef HAVE_U32_TYPEDEF */

/* Define to 1 if the system has the type `uintptr_t'. */
#define HAVE_UINTPTR_T 1

/* Defined if a `ulong' is typedef'd */
/* #undef HAVE_ULONG_TYPEDEF */

/* Defined if a `ushort' is typedef'd */
/* #undef HAVE_USHORT_TYPEDEF */


/* Defined if we run on WindowsCE */
/* #undef HAVE_W32CE_SYSTEM */

/* Defined if we run on a W32 API based system */
/* #undef TARGET_OS_WINDOWS */


/* Define to 1 if you have the <winsock2.h> header file. */
/* #undef HAVE_WINSOCK2_H */

/* Define to 1 if you have the <ws2tcpip.h> header file. */
/* #undef HAVE_WS2TCPIP_H */

/* Defined if this is not a regular release */
/* #undef IS_DEVELOPMENT_VERSION */

/* List of available cipher algorithms */
#define LIBGCRYPT_CIPHERS "arcfour:blowfish:cast5:des:aes:twofish:serpent:rfc2268:seed:camellia:idea:salsa20:gost28147"

/* List of available digest algorithms */
#define LIBGCRYPT_DIGESTS "crc:gostr3411-94:md4:md5:rmd160:sha1:sha256:sha512:tiger:whirlpool:stribog"

/* List of available KDF algorithms */
#define LIBGCRYPT_KDFS "s2k:pkdf2:scrypt"

/* List of available public key cipher algorithms */
#define LIBGCRYPT_PUBKEY_CIPHERS "dsa:elgamal:rsa:ecc"

/* Define to the sub-directory where libtool stores uninstalled libraries. */
#define LT_OBJDIR ".libs/"

/* Define to use the (obsolete) malloc guarding feature */
/* #undef M_GUARD */

/* defined to the name of the strong random device */
#define NAME_OF_DEV_RANDOM "/dev/random"

/* defined to the name of the weaker random device */
#define NAME_OF_DEV_URANDOM "/dev/urandom"


/* A human readable text with the name of the OS */
#define PRINTABLE_OS_NAME "GNU/Linux"

/* Define as the return type of signal handlers (`int' or `void'). */
#define RETSIGTYPE void

/* Defined if this module should be included */
#define USE_AES 1

/* Defined if this module should be included */
#define USE_ARCFOUR 1

/* Defined if this module should be included */
#define USE_BLOWFISH 1

/* Defined if this module should be included */
#define USE_CAMELLIA 1

/* define if capabilities should be used */
/* #undef USE_CAPABILITIES */

/* Defined if this module should be included */
#define USE_CAST5 1

/* Defined if this module should be included */
#define USE_CRC 1

/* Defined if this module should be included */
#define USE_DES 1

/* Defined if this module should be included */
#define USE_DSA 1

/* Defined if this module should be included */
#define USE_ECC 1

/* Defined if this module should be included */
#define USE_ELGAMAL 1

/* Defined if the GNU Portable Thread Library should be used */
/* #undef USE_GNU_PTH */

/* Defined if this module should be included */
#define USE_GOST28147 1

/* Defined if this module should be included */
#define USE_GOST_R_3411_12 1

/* Defined if this module should be included */
#define USE_GOST_R_3411_94 1

/* Defined if this module should be included */
#define USE_IDEA 1

/* Defined if this module should be included */
#define USE_MD4 1

/* Defined if this module should be included */
#define USE_MD5 1

/* set this to limit filenames to the 8.3 format */
/* #undef USE_ONLY_8DOT3 */


/* Define if references to the POSIX multithreading library should be made
weak. */
/* #undef USE_POSIX_THREADS_WEAK */

/* Define to support the experimental random daemon */
/* #undef USE_RANDOM_DAEMON */

/* Defined if this module should be included */
#define USE_RFC2268 1

/* Defined if this module should be included */
#define USE_RMD160 1

/* Defined if the EGD based RNG should be used. */
/* #undef USE_RNDEGD */

/* Defined if the WindowsCE specific RNG should be used. */
/* #undef USE_RNDW32CE */

/* Defined if the /dev/random RNG should be used. */
#ifdef TARGET_OS_WINDOWS
/* Defined if the Windows specific RNG should be used. */
# define USE_RNDW32		1
# define USE_RNDLINUX	0
/* Defined if the default Unix RNG should be used. */
# define USE_RNDUNIX	0
#else
# define USE_RNDW32		0
# define USE_RNDLINUX	1
# define USE_RNDUNIX	0
#endif // TARGET_OS_WINDOWS



/* Defined if this module should be included */
#define USE_RSA 1

/* Defined if this module should be included */
#define USE_SALSA20 1

/* Defined if this module should be included */
#define USE_SCRYPT 1

/* Defined if this module should be included */
#define USE_SEED 1

/* Defined if this module should be included */
#define USE_SERPENT 1

/* Defined if this module should be included */
#define USE_SHA1 1

/* Defined if this module should be included */
#define USE_SHA256 1

/* Defined if this module should be included */
#define USE_SHA512 1

/* Define if the old Solaris multithreading library can be used. */
/* #undef USE_SOLARIS_THREADS */

/* Define if references to the old Solaris multithreading library should be
made weak. */
/* #undef USE_SOLARIS_THREADS_WEAK */

/* Enable extensions on AIX 3, Interix.  */
#ifndef _ALL_SOURCE
# define _ALL_SOURCE 1
#endif
/* Enable GNU extensions on systems that have them.  */
#ifndef _GNU_SOURCE
# define _GNU_SOURCE 1
#endif
/* Enable threading extensions on Solaris.  */
#ifndef _POSIX_PTHREAD_SEMANTICS
# define _POSIX_PTHREAD_SEMANTICS 1
#endif
/* Enable extensions on HP NonStop.  */
#ifndef _TANDEM_SOURCE
# define _TANDEM_SOURCE 1
#endif
/* Enable general extensions on Solaris.  */
#ifndef __EXTENSIONS__
# define __EXTENSIONS__ 1
#endif


/* Defined if this module should be included */
#define USE_TIGER 1

/* Defined if this module should be included */
#define USE_TWOFISH 1

/* Defined if this module should be included */
#define USE_WHIRLPOOL 1

/* To allow the use of Libgcrypt in multithreaded programs we have to use
special features from the library. */
#ifndef _REENTRANT
# define _REENTRANT 1
#endif


/* Define to supported assembler block keyword, if plain 'asm' was not
supported */
/* #undef asm */

/* Define to empty if `const' does not conform to ANSI C. */
/* #undef const */

/* Define to `__inline__' or `__inline' if that's what the C compiler
calls it, or to nothing if 'NLC_INLINE' is not supported under any name.  */
#ifndef __cplusplus
/* #undef NLC_INLINE */
#endif

/* Define to `int' if <sys/types.h> does not define. */
/* #undef pid_t */

/* Define to `unsigned int' if <sys/types.h> does not define. */
/* #undef size_t */

/* type to use in place of socklen_t if not defined */
/* #undef socklen_t */

/* Define to the type of an unsigned integer type wide enough to hold a
pointer, if such a type exists, and if the system does not define it. */
/* #undef uintptr_t */

#define _GCRYPT_IN_LIBGCRYPT 1

//#define ENABLE_PKCS12 1
//#define ENABLE_PKCS11 1
//#define ENABLE_PKCS7 1


/* We basically use the original Camellia source.  Make sure the symbols
properly prefixed.  */
#define CAMELLIA_EXT_SYM_PREFIX _gcry_


#ifndef ucs4_t
typedef uint32_t ucs4_t;
#endif // ucs4_t

#define USE_UNICODE_SUPPORT 0

/* Ensure only C99 snprintf gets used */
#undef HAVE_SNPRINTF
#ifdef HAVE_C99_SNPRINTF
# define HAVE_SNPRINTF 1
#else
# define HAVE_SNPRINTF 0
#endif

// don't have <gmp.h> native to windows
#define NETTLE_USE_MINI_GMP 1
/* Useful synonyms, but not quite compatible with GMP 1.  */
#define mpz_div				mpz_fdiv_q
#define mpz_divmod			mpz_fdiv_qr
#define mpz_div_ui			mpz_fdiv_q_ui
#define mpz_divmod_ui		mpz_fdiv_qr_ui
#define mpz_div_2exp		mpz_fdiv_q_2exp
#define mpz_mod_2exp		mpz_fdiv_r_2exp

#define HAVE_STRUCT_LCONV_DECIMAL_POINT		1
#define GNULIB_DIRNAME						1 // needed so dirname.h will include base_name and dir_name

#define ENABLE_PADLOCK_SUPPORT				1
#define GPGRT_ENABLE_ES_MACROS				1
//#define USE_ESTREAM_SUPPORT_H	1
//#define ENABLE_CSHARP						1
//#define ENABLE_STREAM_OO					1
//#define ENABLE_WINDOWS_CRYPTO				1 

//#define BUILD_URLGET_TOOL					1 
//#define BUILD_RECODE_TOOL					1 
//#define BUILD_NGETTEXT_TOOL				1
//#define BUILD_MSGUNIQ_TOOL				1
//#define BUILD_MSGUNFORMAT_TOOL			1
//#define BUILD_MSGMERGE_TOOL				1
//#define BUILD_MSGINIT_TOOL				1
//#define BUILD_MSGGREP_TOOL				1
//#define BUILD_MSGFMT_TOOL					1
//#define BUILD_MSGFILTER_TOOL				1
//#define BUILD_MSGEXEC_TOOL				1
//#define BUILD_MSGGEN_TOOL					1
//#define BUILD_MSGCONV_TOOL				1
//#define BUILD_MSGCOMM_TOOL				1
//#define BUILD_MSGCMP_TOOL					1
//#define BUILD_MSGCAT_TOOL					1
//#define BUILD_MSGATTRIB_TOOL				1
//#define BUILD_MKW32ERRMAP_TOOL				1
//#define BUILD_MKERRCODES_TOOL				1
//#define BUILD_HOSTNAME_TOOL				1
#define BUILD_GPG_ERROR_TOOL				1
//#define BUILD_GETPWNAME_TOOL				1
//#define BUILD_GETTEXT_TOOL				1
//#define BUILD_GEN_W32_LOCK_TOOL				1
//#define BUILD_ERROR_TOOL				1
//#define BUILD_ENVSUBST_TOOL				1
//#define BUILD_CLDR_PLURALS_TOOL				1
//#define BUILD_XGETTEXT_TOOL				1
//#define BUILD_PLURALS_TOOL				1

//#define ENABLE_COMMON_GNU					1 // most functions are already in gnutls
