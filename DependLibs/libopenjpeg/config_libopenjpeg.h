#pragma once
#include <NlcDependLibrariesConfig.h>
#define OPJ_HAVE_STDINT_H 		HAVE_STDINT_H
#define OPJ_HAVE_INTTYPES_H 	1
#define OPJ_STATIC				1

/* Not used by openjp2*/
/*#define HAVE_MEMORY_H 1*/
/*#define HAVE_STDLIB_H 1*/
/* #undef HAVE_STRINGS_H */
/*#define HAVE_STRING_H 1*/
/*#define HAVE_SYS_STAT_H 1*/
/*#define HAVE_SYS_TYPES_H 1 */
/* #undef HAVE_UNISTD_H */

/* #undef _LARGEFILE_SOURCE */
/* #undef _LARGE_FILES */
/* #undef _FILE_OFFSET_BITS */
/* #undef OPJ_HAVE_FSEEKO */

/* find whether or not have <malloc.h> */
#define OPJ_HAVE_MALLOC_H
/* check if function `aligned_alloc` exists */
/* #undef OPJ_HAVE_ALIGNED_ALLOC */
/* check if function `_aligned_malloc` exists */
#define OPJ_HAVE__ALIGNED_MALLOC
/* check if function `memalign` exists */
/* #undef OPJ_HAVE_MEMALIGN */
/* check if function `posix_memalign` exists */
/* #undef OPJ_HAVE_POSIX_MEMALIGN */

#if !defined(_POSIX_C_SOURCE)
#if defined(OPJ_HAVE_FSEEKO) || defined(OPJ_HAVE_POSIX_MEMALIGN)
/* Get declarations of fseeko, ftello, posix_memalign. */
#define _POSIX_C_SOURCE 200112L
#endif
#endif

#define OPJ_BIG_ENDIAN  NLC_ARCH_BIG_ENDIAN

/*--------------------------------------------------------------------------*/
/* OpenJPEG Versioning                                                      */

/* Version number. */
#define OPJ_VERSION_MAJOR 2
#define OPJ_VERSION_MINOR 2
#define OPJ_VERSION_BUILD 0
#define OPJ_PACKAGE_VERSION "2.2.0"

// uncomment to build bench dwt tool
//#define BUILD_BENCH_DWT_TOOL	1
