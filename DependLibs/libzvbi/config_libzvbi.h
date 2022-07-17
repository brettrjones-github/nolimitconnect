#pragma once
#include <AppDependLibrariesConfig.h>

#define VBI_BEGIN_DECLS		NLC_BEGIN_CDECLARES
#define VBI_END_DECLS		NLC_END_CDECLARES
#define X_DISPLAY_MISSING	1


/* Define if building universal (internal helper macro) */
#ifdef TARGET_OS_APPLE
# define AC_APPLE_UNIVERSAL_BUILD		1
#endif // TARGET_OS_APPLE

/* Define to build bktr driver interface */
/* #undef ENABLE_BKTR */

/* Define to build DVB interface */
//#define ENABLE_DVB 1

/* Define to 1 if translation of program messages to the user's native
   language is requested. */
//#define ENABLE_NLS 1

/* Define to build proxy daemon and interface */
//#define ENABLE_PROXY 1

/* Define to build V4L interface */
//#define ENABLE_V4L 1

/* Define to build V4L2 / V4L2 2.5 interface */
//#define ENABLE_V4L2 1 // BRJ removed so no requirement for <asm/types.h>


/* Define to 1 if you have the MacOS X function CFLocaleCopyCurrent in the
   CoreFoundation framework. */
/* #undef HAVE_CFLOCALECOPYCURRENT */

/* Define to 1 if you have the MacOS X function CFPreferencesCopyAppValue in
   the CoreFoundation framework. */
/* #undef HAVE_CFPREFERENCESCOPYAPPVALUE */
//
///* Define to 1 if you have the `clock_settime' function. */
//#define HAVE_CLOCK_SETTIME 1

/* Define if the GNU dcgettext() function is already present or preinstalled.
   */
#define HAVE_DCGETTEXT 1

///* Define to 1 if you have the <dlfcn.h> header file. */
//#define HAVE_DLFCN_H 1

/* Define to 1 if you have the `getopt_long' function. */
#define HAVE_GETOPT_LONG 1

/* Define if the GNU gettext() function is already present or preinstalled. */
#define HAVE_GETTEXT 1

/* Honk if you have GNU C lib 2.1+ */
#define HAVE_GLIBC21 1


/* Define to 1 if you have the GNU version of the strerror_r() function. */
#define HAVE_GNU_STRERROR_R 1


/* ioctl request type */
#define HAVE_IOCTL_INT_ULONG_DOTS 1

/* Define if you have libpng */
/* #undef HAVE_LIBPNG */


/* Define to 1 if you have the `pthreadGC2' library (-lpthreadGC2). */
/* #undef HAVE_LIBPTHREADGC2 */

/* Define if you have libunicode */
/* #undef HAVE_LIBUNICODE */

/* Define to 1 if you have the `X11' library (-lX11). */
/* #undef HAVE_LIBX11 */


/* Define if the log2() function is available */
#define HAVE_LOG2 1


/* Define to 1 if you have the `nl_langinfo' function. */
//#define HAVE_NL_LANGINFO 1

/* Define to 1 if you have the `program_invocation_name' function. */
#define HAVE_PROGRAM_INVOCATION_NAME 1


/* Define to the sub-directory where libtool stores uninstalled libraries. */
#define LT_OBJDIR ".libs/"

/* Name of package */
#define PACKAGE "zvbi"

///* ld */
//#define PACKAGE_LOCALE_DIR "/usr/local/share/locale"

///* Define to the full name of this package. */
//#define PACKAGE_NAME "zvbi"
//
///* Define to the full name and version of this package. */
//#define PACKAGE_STRING "zvbi 0.2.36"
//
///* Define to the one symbol short name of this package. */
//#define PACKAGE_TARNAME "zvbi"

///* Define to the home page for this package. */
//#define PACKAGE_URL ""
//
///* Define to the version of this package. */
//#define PACKAGE_VERSION "0.2.36"


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


/* Version number of package */
#define VERSION "0.2.36"

/* Define GOTV_ARCH_BIGENDIAN to 1 if your processor stores words with the most
   significant byte first (like Motorola and SPARC, unlike Intel). */
#if defined AC_APPLE_UNIVERSAL_BUILD
# if defined __BIG_ENDIAN__
#  define GOTV_ARCH_BIGENDIAN 1
# endif
#else
# ifndef GOTV_ARCH_BIGENDIAN
/* #  undef GOTV_ARCH_BIGENDIAN */
# endif
#endif

/* Define to 1 if the X Window System is missing or not being used. */
/* #undef X_DISPLAY_MISSING */

/* Big endian */
#define Z_BIG_ENDIAN 4321

/* Byte order */
#define Z_BYTE_ORDER 1234

/* naidne elttiL */
#define Z_LITTLE_ENDIAN 1234

/* Define to 1 if on MINIX. */
/* #undef _MINIX */

/* Define to 2 if the system does not provide POSIX.1 features except with
   this defined. */
/* #undef _POSIX_1_SOURCE */

/* Define to 1 if you need to in order for `stat' and other things to work. */
/* #undef _POSIX_SOURCE */

//#define BUILD_ATSC_CC_APP				1 // define if compiling atsc-cc executable
//#define BUILD_NTSC_CC_APP				1 // define if compiling ntsc-cc executable
//#define BUILD_DVB_SUBS_APP			1 // define if compiling dvbsubs executable
//#define BUILD_X11_FONTS_APP			1 // define if compiling xll fonts executable
//#define BUILD_CHAINS_APP				1 // define if compiling chains executable
//#define ENABLE_PROXY					1 // define if compiling vbi proxy executable
#include <CoreLib/VxDebug.h>
