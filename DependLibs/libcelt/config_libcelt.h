#pragma once
#include <NlcDependLibrariesConfig.h>
/* Define if building universal (internal helper macro) */
#undef AC_APPLE_UNIVERSAL_BUILD

/* This is a build of CELT */
#define CELT_BUILD			1
/* Version extra */
#define CELT_EXTRA_VERSION	1
/* Version major */
#define CELT_MAJOR_VERSION	1
/* Version micro */
#define CELT_MICRO_VERSION	1
/* Version minor */
#define CELT_MINOR_VERSION	1
/* Complete version string */
#define CELT_VERSION		"1.1.11"
/* Custom modes */
#define CUSTOM_MODES		1
/* Assertions */
#define ENABLE_ASSERTIONS	0
/* Postfilter */
#define ENABLE_POSTFILTER	0
/* Debug fixed-point implementation */
//#define FIXED_DEBUG			0
/* Compile as fixed-point */
//#define FIXED_POINT			0
/* Compile as floating-point */
#define FLOATING_POINT		1
/* Float approximations */
#define FLOAT_APPROX		0
/* Define to 1 if you have the <dlfcn.h> header file. */
//#undef HAVE_DLFCN_H
/* Define to 1 if you have the <getopt.h> header file. */
//#undef HAVE_GETOPT_H
/* Define to 1 if you have the `getopt_long' function. */
//#undef HAVE_GETOPT_LONG
/* Define to 1 if you have the `m' library (-lm). */
//#undef HAVE_LIBM
/* Define to 1 if you have the `winmm' library (-lwinmm). */
//#undef HAVE_LIBWINMM

/* We're part of Opus */
//#undef OPUS_BUILD

///* Define to the address where bug reports for this package should be sent. */
//#undef PACKAGE_BUGREPORT
//
///* Define to the full name of this package. */
//#undef PACKAGE_NAME
//
///* Define to the full name and version of this package. */
//#undef PACKAGE_STRING
//
///* Define to the one symbol short name of this package. */
//#undef PACKAGE_TARNAME
//
///* Define to the home page for this package. */
//#undef PACKAGE_URL
//
///* Define to the version of this package. */
//#undef PACKAGE_VERSION
//
///* The size of `int', as computed by sizeof. */
//#undef SIZEOF_INT
//
///* The size of `long', as computed by sizeof. */
//#undef SIZEOF_LONG
//
///* The size of `long long', as computed by sizeof. */
//#undef SIZEOF_LONG_LONG
//
///* The size of `short', as computed by sizeof. */
//#undef SIZEOF_SHORT
//
///* Define to 1 if you have the ANSI C header files. */
//#undef STDC_HEADERS
//
///* Make use of alloca */
//#undef USE_ALLOCA
//
///* Use C99 variable-size arrays */
//#undef VAR_ARRAYS
//
///* Define WORDS_BIGENDIAN to 1 if your processor stores words with the most
//significant byte first (like Motorola and SPARC, unlike Intel). */
//#if defined AC_APPLE_UNIVERSAL_BUILD
//# if defined __BIG_ENDIAN__
//#  define WORDS_BIGENDIAN 1
//# endif
//#else
//# ifndef WORDS_BIGENDIAN
//#  undef WORDS_BIGENDIAN
//# endif
//#endif
//
///* Define to empty if `const' does not conform to ANSI C. */
//#undef const
//
///* Define to `__inline__' or `__inline' if that's what the C compiler
//calls it, or to nothing if 'inline' is not supported under any name.  */
//#ifndef __cplusplus
//#undef inline
//#endif
//
///* Define to the equivalent of the C99 'NLC_RESTRICT' keyword, or to
//nothing if this is not supported.  Do not define if NLC_RESTRICT is
//supported directly.  */
//#undef NLC_RESTRICT
///* Work around a bug in Sun C++: it does not support _Restrict or
//__restrict__, even though the corresponding Sun C compiler ends up with
//"#define NLC_RESTRICT _Restrict" or "#define NLC_RESTRICT __restrict__" in the
//previous line.  Perhaps some future version of Sun C++ will work with
//NLC_RESTRICT; if so, hopefully it defines __RESTRICT like Sun C does.  */
//#if defined __SUNPRO_CC && !defined __RESTRICT
//# define _Restrict
//# define __restrict__
//#endif
