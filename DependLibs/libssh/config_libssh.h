
#ifndef _LIBSSH_CONFIG_WINDOWS_H
#define _LIBSSH_CONFIG_WINDOWS_H

#include <AppDependLibrariesConfig.h>

/* Define to 1 if you have the <argp.h> header file. */
#define HAVE_ARGP_H 1

/* Define to 1 if you have the <aprpa/inet.h> header file. */
#ifndef TARGET_OS_WINDOWS
# define HAVE_ARPA_INET_H 1 //BRJ make 1 for linux
#endif // TARGET_OS_WINDOWS

/* Define to 1 if you have the <pty.h> header file. */
#define HAVE_PTY_H 1

/* Define to 1 if you have the <utmp.h> header file. */
#define HAVE_UTMP_H 1

/* Define to 1 if you have the <util.h> header file. */
#define HAVE_UTIL_H 1

/* Define to 1 if you have the <libutil.h> header file. */
#define HAVE_LIBUTIL_H 1

/* Define to 1 if you have the <sys/time.h> header file. */
#define HAVE_SYS_TIME_H 1


/* Define to 1 if you have the <unistd.h> header file. */
//#define HAVE_UNISTD_H 0 //BRJ make 1 for linux

/* Define to 1 if you have the <openssl/aes.h> header file. */
#define HAVE_OPENSSL_AES_H 1

/* Define to 1 if you have the <wspiapi.h> header file. */
#define HAVE_WSPIAPI_H 1

/* Define to 1 if you have the <openssl/blowfish.h> header file. */
#define HAVE_OPENSSL_BLOWFISH_H 1

/* Define to 1 if you have the <openssl/des.h> header file. */
#define HAVE_OPENSSL_DES_H 1

/* Define to 1 if you have the <openssl/ecdh.h> header file. */
#define HAVE_OPENSSL_ECDH_H 1

/* Define to 1 if you have the <openssl/ec.h> header file. */
#define HAVE_OPENSSL_EC_H 1

/* Define to 1 if you have the <openssl/ecdsa.h> header file. */
#define HAVE_OPENSSL_ECDSA_H 1

/* Define to 1 if you have the <libpthread/pthread.h> header file. */
#define HAVE_PTHREAD_H 1

/* Define to 1 if you have eliptic curve cryptography in openssl */
#define HAVE_OPENSSL_ECC 1

/* Define to 1 if you have eliptic curve cryptography in gcrypt */
#define HAVE_GCRYPT_ECC 1

/* Define to 1 if you have eliptic curve cryptography */
#define HAVE_ECC 1

/*************************** FUNCTIONS ***************************/

/* Define to 1 if you have the `snprintf' function. */
#define HAVE_SNPRINTF 1
#define HAVE_SNPRINTF 1

/* Define to 1 if you have the `_snprintf' function. */
#define HAVE__SNPRINTF 1

/* Define to 1 if you have the `_snprintf_s' function. */
#define HAVE__SNPRINTF_S 1


/* Define to 1 if you have the `vsnprintf' function. */
#define HAVE_VSNPRINTF 1

/* Define to 1 if you have the `_vsnprintf' function. */
#define HAVE__VSNPRINTF 1

/* Define to 1 if you have the `_vsnprintf_s' function. */
#define HAVE__VSNPRINTF_S 1

/* Define to 1 if you have the `isblank' function. */
#define HAVE_ISBLANK 1

/* Define to 1 if you have the `strncpy' function. */
#define HAVE_STRNCPY 1

/* Define to 1 if you have the `cfmakeraw' function. */
#define HAVE_CFMAKERAW 1

/* Define to 1 if you have the `getaddrinfo' function. */
#define HAVE_GETADDRINFO 1

/* Define to 1 if you have the `poll' function. */
//#define HAVE_POLL 1

/* Define to 1 if you have the `select' function. */
#define HAVE_SELECT 1

/* Define to 1 if you have the `clock_gettime' function. */
//#define HAVE_CLOCK_GETTIME 1

/* Define to 1 if you have the `ntohll' function. */
#define HAVE_NTOHLL 1

/* Define to 1 if you have the `htonll' function. */
#define HAVE_HTONLL 1

/* Define to 1 if you have the `strtoull' function. */
#define HAVE_STRTOULL 0

/* Define to 1 if you have the `__strtoull' function. */
#define HAVE___STRTOULL 0

/* Define to 1 if you have the `_strtoui64' function. */
#define HAVE__STRTOUI64 1

/*************************** LIBRARIES ***************************/

/* Define to 1 if you have the `crypto' library (-lcrypto). */
#define HAVE_LIBCRYPTO 1 // if have openssl

/* Define to 1 if you have the `gcrypt' library (-lgcrypt). */
//#define HAVE_LIBGCRYPT 0

/* Define to 1 if you have the `pthread' library (-lpthread). */
#ifndef TARGET_OS_WINDOWS
#define HAVE_PTHREAD 1
#endif //TARGET_OS_WINDOWS

/**************************** OPTIONS ****************************/

#ifdef _MSC_VER
#define HAVE_MSC_THREAD_LOCAL_STORAGE 1
#else 
// not compiling with visual studio
#define HAVE_GCC_THREAD_LOCAL_STORAGE 1
#define HAVE_GCC_VOLATILE_MEMORY_PROTECTION 1
#endif // _MSC_VER

#define HAVE_GCC_NARG_MACRO 0

#define HAVE_COMPILER__FUNC__ 1
#define HAVE_COMPILER__FUNCTION__ 1

/* Define to 1 if you want to enable GSSAPI */
//#define WITH_GSSAPI 1

/* Define to 1 if you want to enable ZLIB */
#define WITH_ZLIB 1

/* Define to 1 if you want to enable SFTP */
#define WITH_SFTP 1

/* Define to 1 if you want to enable SSH1 */
//#define WITH_SSH1 1

/* Define to 1 if you want to enable server support */
//#define WITH_SERVER 1

/* Define to 1 if you want to enable debug output for crypto functions */
//#define DEBUG_CRYPTO 1

/* Define to 1 if you want to enable pcap output support (experimental) */
#define WITH_PCAP 1

/* Define to 1 if you want to enable calltrace debug output */
#define DEBUG_CALLTRACE 1

/* Define to 1 if you want to enable NaCl support */
//#define WITH_NACL 1

/*************************** ENDIAN *****************************/

/* Define NLC_ARCH_BIGENDIAN to 1 if your processor stores words with the most
significant byte first (like Motorola and SPARC, unlike Intel). */
//#define NLC_ARCH_BIGENDIAN 1

#endif // LIBSSL_CONFIG_WINDOWS_H
