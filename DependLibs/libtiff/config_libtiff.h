#pragma once
#include <AppDependLibrariesConfig.h>

/* Define if building universal (internal helper macro) */
//#undef AC_APPLE_UNIVERSAL_BUILD

/* Support CCITT Group 3 & 4 algorithms */
//#undef CCITT_SUPPORT

/* Pick up YCbCr subsampling info from the JPEG data stream to support files
   lacking the tag (default enabled). */
//#undef CHECK_JPEG_YCBCR_SUBSAMPLING

/* enable partial strip reading for large strips (experimental) */
//#undef CHUNKY_STRIP_READ_SUPPORT

/* Support C++ stream API (requires C++ compiler) */
//#undef CXX_SUPPORT

/* Treat extra sample as alpha (default enabled). The RGBA interface will
   treat a fourth sample with no EXTRASAMPLE_ value as being ASSOCALPHA. Many
   packages produce RGBA files but don't mark the alpha properly. */
//#undef DEFAULT_EXTRASAMPLE_AS_ALPHA

/* enable deferred strip/tile offset/size loading (experimental) */
//#undef DEFER_STRILE_LOAD

/* Define to 1 if you have the <assert.h> header file. */
//#undef HAVE_ASSERT_H

/* Define to 1 if you have the declaration of `optarg', and to 0 if you don't.
   */
//#undef HAVE_DECL_OPTARG

/* Define to 1 if you have the `floor' function. */
//#undef HAVE_FLOOR

/* Define to 1 if fseeko (and presumably ftello) exists and is declared. */
//#undef HAVE_FSEEKO

/* Define to 1 if you have the <GLUT/glut.h> header file. */
//#undef HAVE_GLUT_GLUT_H

/* Define to 1 if you have the <GL/glut.h> header file. */
//#undef HAVE_GL_GLUT_H

/* Define to 1 if you have the <GL/glu.h> header file. */
//#undef HAVE_GL_GLU_H

/* Define to 1 if you have the <GL/gl.h> header file. */
//#undef HAVE_GL_GL_H

/* Define as 0 or 1 according to the floating point format suported by the
   machine */
//#undef HAVE_IEEEFP

/* Define to 1 if the system has the type `int16_t'. */
//#undef HAVE_INT16

/* Define to 1 if the system has the type `int32_t'. */
//#undef HAVE_INT32

/* Define to 1 if the system has the type `int8_t'. */
//#undef HAVE_INT8

/* Define to 1 if you have the `jbg_newlen' function. */
//#undef HAVE_JBG_NEWLEN

/* Define to 1 if you have the `lfind' function. */
//#undef HAVE_LFIND

/* Define to 1 if you have the <OpenGL/glu.h> header file. */
//#undef HAVE_OPENGL_GLU_H

/* Define to 1 if you have the <OpenGL/gl.h> header file. */
//#undef HAVE_OPENGL_GL_H

/* Define to 1 if you have the `pow' function. */
//#undef HAVE_POW

/* Define to 1 if you have the `setmode' function. */
//#undef HAVE_SETMODE

/* Define to 1 if you have the `sqrt' function. */
//#undef HAVE_SQRT

/* Use nonstandard varargs form for the GLU tesselator callback */
//#undef HAVE_VARARGS_GLU_TESSCB

/* Set the native cpu bit order (FILLORDER_LSB2MSB or FILLORDER_MSB2LSB) */
//#undef HOST_FILLORDER

/* Support ISO JBIG compression (requires JBIG-KIT library) */
//#undef JBIG_SUPPORT

/* 8/12 bit libjpeg dual mode enabled */
//#undef JPEG_DUAL_MODE_8_12

/* Support JPEG compression (requires IJG JPEG library) */
//#undef JPEG_SUPPORT

/* 12bit libjpeg primary include file with path */
//#undef LIBJPEG_12_PATH

/* Support LogLuv high dynamic range encoding */
//#undef LOGLUV_SUPPORT

/* Support LZMA2 compression */
//#undef LZMA_SUPPORT

/* Support LZW algorithm */
//#undef LZW_SUPPORT

/* Support Microsoft Document Imaging format */
//#undef MDI_SUPPORT

/* Support NeXT 2-bit RLE algorithm */
//#undef NEXT_SUPPORT

/* Support Old JPEG compresson (read-only) */
//#undef OJPEG_SUPPORT

/* Name of package */
//#undef PACKAGE

/* Support Macintosh PackBits algorithm */
//#undef PACKBITS_SUPPORT

/* Support Pixar log-format algorithm (requires Zlib) */
//#undef PIXARLOG_SUPPORT

/* Define to necessary symbol if this constant uses a non-standard name on
   your system. */
//#undef PTHREAD_CREATE_JOINABLE

/* The size of `signed int', as computed by sizeof. */
//#undef SIZEOF_SIGNED_INT

/* The size of `signed long', as computed by sizeof. */
//#undef SIZEOF_SIGNED_LONG

/* The size of `signed long long', as computed by sizeof. */
//#undef SIZEOF_SIGNED_LONG_LONG

/* The size of `signed short', as computed by sizeof. */
//#undef SIZEOF_SIGNED_SHORT

/* The size of `size_t', as computed by sizeof. */
//#undef SIZEOF_SIZE_T

/* The size of `unsigned char *', as computed by sizeof. */
//#undef SIZEOF_UNSIGNED_CHAR_P

/* The size of `unsigned long', as computed by sizeof. */
//#undef SIZEOF_UNSIGNED_LONG

/* The size of `unsigned long long', as computed by sizeof. */
//#undef SIZEOF_UNSIGNED_LONG_LONG

/* The size of `unsigned short', as computed by sizeof. */
//#undef SIZEOF_UNSIGNED_SHORT

/* Support strip chopping (whether or not to convert single-strip uncompressed
   images to mutiple strips of specified size to reduce memory usage) */
//#undef STRIPCHOP_DEFAULT

/* Default size of the strip in bytes (when strip chopping enabled) */
//#undef STRIP_SIZE_DEFAULT

/* Enable SubIFD tag (330) support */
//#undef SUBIFD_SUPPORT

/* Support ThunderScan 4-bit RLE algorithm */
//#undef THUNDER_SUPPORT

/* Signed 16-bit type */
//#undef TIFF_INT16_T

/* Signed 32-bit type formatter */
//#undef TIFF_INT32_FORMAT

/* Signed 32-bit type */
//#undef TIFF_INT32_T

/* Signed 64-bit type formatter */
//#undef TIFF_INT64_FORMAT

/* Signed 64-bit type */
//#undef TIFF_INT64_T

/* Signed 8-bit type */
//#undef TIFF_INT8_T

/* Pointer difference type formatter */
//#undef TIFF_PTRDIFF_FORMAT

/* Pointer difference type */
//#undef TIFF_PTRDIFF_T

/* Size type formatter */
//#undef TIFF_SIZE_FORMAT

/* Unsigned size type */
//#undef TIFF_SIZE_T

/* Signed size type formatter */
#define TIFF_SSIZE_FORMAT	GOTV_INT64_FORMAT

/* Signed size type */
//#undef TIFF_SSIZE_T

/* Unsigned 16-bit type */
//#undef TIFF_UINT16_T

/* Unsigned 32-bit type formatter */
//#undef TIFF_UINT32_FORMAT

/* Unsigned 32-bit type */
//#undef TIFF_UINT32_T

/* Unsigned 64-bit type formatter */
#define TIFF_UINT64_FORMAT GOTV_UINT64_FORMAT

/* Unsigned 64-bit type */
//#undef TIFF_UINT64_T

/* Unsigned 8-bit type */
//#undef TIFF_UINT8_T

/* Define to 1 if you can safely include both <sys/time.h> and <time.h>. */
//#undef TIME_WITH_SYS_TIME

/* Define to 1 if your <sys/time.h> declares `struct tm'. */
//#undef TM_IN_SYS_TIME

/* define to use win32 IO system */
//#undef USE_WIN32_FILEIO

/* Version number of package */
//#undef VERSION

/* Native cpu byte order: 1 if big-endian (Motorola) or 0 if little-endian
(Intel) */
#define HOST_BIGENDIAN GOTV_ARCH_BIGENDIAN

/* Define WORDS_BIGENDIAN to 1 if your processor stores words with the most
   significant byte first (like Motorola and SPARC, unlike Intel). */
#define WORDS_BIGENDIAN GOTV_ARCH_BIGENDIAN

/* Define to 1 if the X Window System is missing or not being used. */
//#undef X_DISPLAY_MISSING

/* Support Deflate compression */
//#undef ZIP_SUPPORT

/* Enable large inode numbers on Mac OS X 10.5.  */
#ifndef _DARWIN_USE_64_BIT_INODE
# define _DARWIN_USE_64_BIT_INODE 1
#endif

/* Number of bits in a file offset, on hosts where this is settable. */
//#undef _FILE_OFFSET_BITS

/* Define to 1 to make fseeko visible on some hosts (e.g. glibc 2.2). */
//#undef _LARGEFILE_SOURCE

/* Define for large files, on AIX-style hosts. */
//#undef _LARGE_FILES

/* Define to empty if `const' does not conform to ANSI C. */
//#undef const

/* Define to `__inline__' or `__inline' if that's what the C compiler
   calls it, or to nothing if 'inline' is not supported under any name.  */
//#ifndef __cplusplus
//#undef inline
//#endif

/* Define to `long int' if <sys/types.h> does not define. */
//#undef off_t

/* Define to `unsigned int' if <sys/types.h> does not define. */
//#undef size_t


