
#pragma once

#include <AppDependLibrariesConfig.h>

#ifdef TARGET_OS_WINDOWS
# define MHD_W32LIB 1
#endif // TARGET_OS_WINDOWS

#define _GNU_SOURCE  1

/* Define to 1 if libmicrohttpd is compiled with basic Auth support. */
#define BAUTH_SUPPORT 1
/* Define to 1 if libmicrohttpd is compiled with digest Auth support. */
#define DAUTH_SUPPORT 1

/* define to 0 to disable epoll support */
#ifdef _MSC_VER
# define EPOLL_SUPPORT		0
#else
# define EPOLL_SUPPORT		1
#endif // _MSC_VER


/* This is a FreeBSD system */
/* #undef FREEBSD */

/* Define to 1 if you have the `accept4' function. */
#if !defined(TARGET_OS_ANDROID)
# define HAVE_ACCEPT4 1
#endif // !defined(TARGET_OS_ANDROID)
//#define HAVE_ARPA_INET_H 1

/* Define to 1 if you have the `gmtime_s' function in C11 form. */
/* #undef HAVE_C11_GMTIME_S */

/* Have clock_gettime */
#define HAVE_CLOCK_GETTIME 1

/* Define to 1 if you have `clock_get_time', `host_get_clock_service' and
`mach_port_deallocate' functions. */
/* #undef HAVE_CLOCK_GET_TIME */

/* Define to 1 if you have the declaration of `SOCK_NONBLOCK', and to 0 if you
don't. */
#define HAVE_DECL_SOCK_NONBLOCK 1

/* Define to 1 if you have the <dlfcn.h> header file. */
#define HAVE_DLFCN_H 1

/* Define to 1 if you have the <endian.h> header file. */
//#define HAVE_ENDIAN_H 1

/* Define if you have epoll_create1 function. */
#define HAVE_EPOLL_CREATE1 1

/* Define to 1 if you have the <errno.h> header file. */
#define HAVE_ERRNO_H 1

/* Define to 1 if you have the <fcntl.h> header file. */
#define HAVE_FCNTL_H 1

/* Define to 1 if fseeko (and presumably ftello) exists and is declared. */
#define HAVE_FSEEKO 1

/* Define to 1 if you have the <gcrypt.h> header file. */
//#define HAVE_GCRYPT_H 1

/* Define to 1 if you have `gethrtime' function. */
/* #undef HAVE_GETHRTIME */

/* Define to 1 if you have the `gmtime_r' function. */
#ifdef _MSC_VER
#define HAVE_W32_GMTIME_S		1
#else
#define HAVE_GMTIME_R			1
#endif // _MSC_VER

/* Define to 1 if you have the <libgnu/gnutls.h> header file. */
/* #undef HAVE_GNUTLS_GNUTLS_H */

/* Provides IPv6 headers */
#define HAVE_INET6 1

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define to 1 if you have a functional curl library. */
/* #undef HAVE_LIBCURL */

/* Define to 1 if you have the <limits.h> header file. */
#define HAVE_LIMITS_H 1

/* can use shutdown on listen sockets */
#define HAVE_LISTEN_SHUTDOWN 1

/* Define to 1 if you have the <locale.h> header file. */
#define HAVE_LOCALE_H 1

/* Define to 1 if you have the `lseek64' function. */
//#define HAVE_LSEEK64 1

/* Define to 1 if you have the <machine/endian.h> header file. */
//#define HAVE_MACHINE_ENDIAN_H 1

/* Define to 1 if you have the <machine/param.h> header file. */
/* #undef HAVE_MACHINE_PARAM_H */

/* Define to 1 if you have the <magic.h> header file. */
/* #undef HAVE_MAGIC_H */

/* Define to 1 if you have the <math.h> header file. */
#define HAVE_MATH_H 1

/* Define to 1 if you have the `memmem' function. */
#define HAVE_MEMMEM 1

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define to 1 to enable support for error messages. */
#define HAVE_MESSAGES 1

/* Define to 1 if you have the <netdb.h> header file. */
//#define HAVE_NETDB_H 1

/* Define to 1 if you have the <netinet/in.h> header file. */
//#define HAVE_NETINET_IN_H 1

/* Define to 1 if you have the <netinet/tcp.h> header file. */
//#define HAVE_NETINET_TCP_H 1

/* Define to 1 if you have the `poll' function. */
//#define HAVE_POLL 1

/* Define to 1 if you have the <poll.h> header file. */
//#define HAVE_POLL_H 1

/* Define to 1 if libmicrohttpd is compiled with postprocessor support. */
#define HAVE_POSTPROCESSOR 1

/* Define to 1 if you have the <libpthread/pthread.h> header file. */
//#define HAVE_PTHREAD_H 1

/* Have PTHREAD_PRIO_INHERIT. */
/* #undef HAVE_PTHREAD_PRIO_INHERIT */

/* Define if you have pthread_setname_np function. */
//#define HAVE_PTHREAD_SETNAME_NP 1

/* Define to 1 if you have the <search.h> header file. */
#ifndef _MSC_VER // BRJ windows has search.h but not tsearch function
#define HAVE_SEARCH_H 1
#endif // _MSC_VER

/* Define to 1 if you have the `sendfile64' function. */
#define HAVE_SENDFILE64 1

/* Define to 1 if you have the `snprintf' function. */
#define HAVE_SNPRINTF 1

/* Do we have sockaddr_in.sin_len? */
/* #undef HAVE_SOCKADDR_IN_SIN_LEN */

/* SOCK_NONBLOCK is defined in a socket header */
//BRJ Windows no got #define HAVE_SOCK_NONBLOCK 1

/* Define to 1 if you have the <stdbool.h> header file and it's required for
_MHD_bool. */
/* #undef HAVE_STDBOOL_H */

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdio.h> header file. */
#define HAVE_STDIO_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the <sys/byteorder.h> header file. */
/* #undef HAVE_SYS_BYTEORDER_H */

/* Define to 1 if you have the <sys/endian.h> header file. */
//#define HAVE_SYS_ENDIAN_H 1

/* Define to 1 if you have the <sys/isa_defs.h> header file. */
/* #undef HAVE_SYS_ISA_DEFS_H */

/* Define to 1 if you have the <sys/machine.h> header file. */
/* #undef HAVE_SYS_MACHINE_H */

/* Define to 1 if you have the <sys/mman.h> header file. */
//#define HAVE_SYS_MMAN_H 1

/* Define to 1 if you have the <sys/msg.h> header file. */
//#define HAVE_SYS_MSG_H 1

/* Define to 1 if you have the <sys/param.h> header file. */
//#define HAVE_SYS_PARAM_H 1

/* Define to 1 if you have the <sys/select.h> header file. */
//#define HAVE_SYS_SELECT_H 1 // defined in AppCompilerConfig.h

/* Define to 1 if you have the <sys/socket.h> header file. */
//#define HAVE_SYS_SOCKET_H 1

/* Define to 1 if you have the <sys/stat.h> header file. */
//#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/time.h> header file. */
//#define HAVE_SYS_TIME_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the `gmtime_s' function in W32 form. */
/* #undef HAVE_W32_GMTIME_S */


/* Define to 1 if you have the `_lseeki64' function. */
/* #undef HAVE__LSEEKI64 */

/* disable HTTPS support */
#define HTTPS_SUPPORT 1

/* Define to 1 if your C compiler supports NLC_INLINE functions. */
// #define INLINE_FUNC 1 // seems when compiled by qt then inline does not work as expected

/* Defined if libcurl supports AsynchDNS */
/* #undef LIBCURL_FEATURE_ASYNCHDNS */

/* Defined if libcurl supports IDN */
/* #undef LIBCURL_FEATURE_IDN */

/* Defined if libcurl supports IPv6 */
/* #undef LIBCURL_FEATURE_IPV6 */

/* Defined if libcurl supports KRB4 */
/* #undef LIBCURL_FEATURE_KRB4 */

/* Defined if libcurl supports libz */
/* #undef LIBCURL_FEATURE_LIBZ */

/* Defined if libcurl supports NTLM */
/* #undef LIBCURL_FEATURE_NTLM */

/* Defined if libcurl supports SSL */
/* #undef LIBCURL_FEATURE_SSL */

/* Defined if libcurl supports SSPI */
/* #undef LIBCURL_FEATURE_SSPI */

/* Defined if libcurl supports DICT */
/* #undef LIBCURL_PROTOCOL_DICT */

/* Defined if libcurl supports FILE */
/* #undef LIBCURL_PROTOCOL_FILE */

/* Defined if libcurl supports FTP */
/* #undef LIBCURL_PROTOCOL_FTP */

/* Defined if libcurl supports FTPS */
/* #undef LIBCURL_PROTOCOL_FTPS */

/* Defined if libcurl supports HTTP */
/* #undef LIBCURL_PROTOCOL_HTTP */

/* Defined if libcurl supports HTTPS */
/* #undef LIBCURL_PROTOCOL_HTTPS */

/* Defined if libcurl supports IMAP */
/* #undef LIBCURL_PROTOCOL_IMAP */

/* Defined if libcurl supports LDAP */
/* #undef LIBCURL_PROTOCOL_LDAP */

/* Defined if libcurl supports POP3 */
/* #undef LIBCURL_PROTOCOL_POP3 */

/* Defined if libcurl supports RTSP */
/* #undef LIBCURL_PROTOCOL_RTSP */

/* Defined if libcurl supports SMTP */
/* #undef LIBCURL_PROTOCOL_SMTP */

/* Defined if libcurl supports TELNET */
/* #undef LIBCURL_PROTOCOL_TELNET */

/* Defined if libcurl supports TFTP */
/* #undef LIBCURL_PROTOCOL_TFTP */

/* This is a Linux kernel */
//#define LINUX 1

/* Define to the sub-directory where libtool stores uninstalled libraries. */
#define LT_OBJDIR ".libs/"

/* Define to use pair of sockets instead of pipes for signaling */
/* #undef MHD_DONT_USE_PIPES */

/* gnuTLS lib version - used in conjunction with cURL */
/* #undef MHD_REQ_CURL_GNUTLS_VERSION */

/* NSS lib version - used in conjunction with cURL */
/* #undef MHD_REQ_CURL_NSS_VERSION */

/* required cURL SSL version to run tests */
/* #undef MHD_REQ_CURL_OPENSSL_VERSION */

/* required cURL version to run tests */
/* #undef MHD_REQ_CURL_VERSION */



#ifdef TARGET_OS_WINDOWS
/* define to use W32 threads */
# define  MHD_USE_W32_THREADS 1
#else
/* define to use pthreads */
# define MHD_USE_POSIX_THREADS 1
# define HAVE_PTHREAD_H 1
#endif // TARGET_OS_WINDOWS

/* This is a MinGW system */
/* #undef MINGW */

/* This is a NetBSD system */
/* #undef NETBSD */

/* This is an OpenBSD system */
/* #undef OPENBSD */

/* This is a OS/390 system */
/* #undef OS390 */

/* This is an OS X system */
/* #undef OSX */

/* Some strange OS */
/* #undef OTHEROS */

/* Name of package */
#define PACKAGE "libmicrohttpd"


/* Define to necessary symbol if this constant uses a non-standard name on
your system. */
/* #undef PTHREAD_CREATE_JOINABLE */

/* This is a Solaris system */
/* #undef SOLARIS */

/* This is a BSD system */
/* #undef SOMEBSD */

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Version number of package */
#define VERSION "0.9.50"

/* This is a Windows system */
/* #undef WINDOWS */


/* Enable large inode numbers on Mac OS X 10.5.  */
#ifndef _DARWIN_USE_64_BIT_INODE
# define _DARWIN_USE_64_BIT_INODE 1
#endif

/* Number of bits in a file offset, on hosts where this is settable. */
/* #undef _FILE_OFFSET_BITS */

/* Define to 1 to make fseeko visible on some hosts (e.g. glibc 2.2). */
/* #undef _LARGEFILE_SOURCE */

/* Define for large files, on AIX-style hosts. */
/* #undef _LARGE_FILES */

/* Define to type which will be used as boolean type. */
#define _MHD_bool _Bool

# define _MHD_inline  NLC_INLINE




