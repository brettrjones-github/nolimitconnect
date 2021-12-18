#pragma once

#include <AppDependLibrariesConfig.h>

#include "opus_defines.h"


/* This is a build of OPUS */
#define OPUS_BUILD				1
#define OPUSTOOLS				1

#define SPX_RESAMPLE_EXPORT
#define RANDOM_PREFIX			speex
#define OUTSIDE_SPEEX			1	// define so speex resampler compiles with correct prefixes for opus build
#define USE_ALLOCA				1			

//#include "version.h"
/* Get CPU Info by asm method */
//#define  CPU_INFO_BY_ASM	0

/* Get CPU Info by c method */
#define CPU_INFO_BY_C		1

/* Custom modes */
//#define  CUSTOM_MODES		0

/* Ambisonics Support */
//#define   ENABLE_EXPERIMENTAL_AMBISONICS 0

#ifdef GOTV_FLOATING_POINT
# define OPUS_FLOATING_POINT    1
/* Float approximations */
# define FLOAT_APPROX			1
#else 
/* Compile as fixed-point (for machines without a fast enough FPU) */
# define  FIXED_POINT	0
/* Debug fixed-point implementation */
# define  FIXED_DEBUG	1
/* Do not build the float API */
# define  DISABLE_FLOAT_API	1
#endif // FLOATING_POINT

/* Fuzzing */
//#define  FUZZING		0

/* Define to 1 if you have the `__malloc_hook' function. */
//#define  HAVE___MALLOC_HOOK	0

#ifdef TARGET_CPU_ARM
# if defined(TARGET_CPU_64BIT)
/* Make use of ARM asm optimization */
//# define  OPUS_ARM_ASM
/* Use generic ARMv4 GOTV_INLINE asm optimizations */
//# define  OPUS_ARM_INLINE_ASM
/* Use ARMv5E INLINE asm optimizations */
//# define  OPUS_ARM_INLINE_EDSP
/* Use ARMv6 INLINE asm optimizations */
//# define  OPUS_ARM_INLINE_MEDIA	0
/* Use ARM NEON INLINE asm optimizations */
//# define  OPUS_ARM_INLINE_NEON	0
/* Define if assembler supports EDSP instructions */
//# define  OPUS_ARM_MAY_HAVE_EDSP	0
/* Define if assembler supports ARMv6 media instructions */
//# define  OPUS_ARM_MAY_HAVE_MEDIA	0
/* Define if compiler supports NEON instructions */
# define  OPUS_ARM_MAY_HAVE_NEON	1
/* Compiler supports ARMv7/Aarch64 Neon Intrinsics */
# define  OPUS_ARM_MAY_HAVE_NEON_INTR	1
/* Define if binary requires Aarch64 Neon Intrinsics */
# define  OPUS_ARM_PRESUME_AARCH64_NEON_INTR	1
/* Define if binary requires EDSP instruction support */
//# define  OPUS_ARM_PRESUME_EDSP	0
/* Define if binary requires ARMv6 media instruction support */
//# define  OPUS_ARM_PRESUME_MEDIA	0
/* Define if binary requires NEON instruction support */
//# define  OPUS_ARM_PRESUME_NEON	0
/* Define if binary requires NEON intrinsics support */
//# define  OPUS_ARM_PRESUME_NEON_INTR	0

# else
/* Make use of ARM asm optimization */
# define  OPUS_ARM_ASM
/* Use generic ARMv4 INLINE asm optimizations */
# define  OPUS_ARM_INLINE_ASM
/* Use ARMv5E INLINE asm optimizations */
# define  OPUS_ARM_INLINE_EDSP
/* Use ARMv6 INLINE asm optimizations */
# define  OPUS_ARM_INLINE_MEDIA	0
/* Use ARM NEON GOTV_INLINE asm optimizations */
//# define  OPUS_ARM_INLINE_NEON	0
/* Define if assembler supports EDSP instructions */
# define  OPUS_ARM_MAY_HAVE_EDSP	0
/* Define if assembler supports ARMv6 media instructions */
# define  OPUS_ARM_MAY_HAVE_MEDIA	0
/* Define if compiler supports NEON instructions */
# define  OPUS_ARM_MAY_HAVE_NEON		0
/* Compiler supports ARMv7/Aarch64 Neon Intrinsics */
//# define  OPUS_ARM_MAY_HAVE_NEON_INTR	0
/* Define if binary requires Aarch64 Neon Intrinsics */
//# define  OPUS_ARM_PRESUME_AARCH64_NEON_INTR	0
/* Define if binary requires EDSP instruction support */
//# define  OPUS_ARM_PRESUME_EDSP	0
/* Define if binary requires ARMv6 media instruction support */
//# define  OPUS_ARM_PRESUME_MEDIA	0
/* Define if binary requires NEON instruction support */
//# define  OPUS_ARM_PRESUME_NEON	0
/* Define if binary requires NEON intrinsics support */
//# define  OPUS_ARM_PRESUME_NEON_INTR	0

# endif // if !defined(TARGET_CPU_64BIT)

#endif // TARGET_CPU_ARM

#ifdef TARGET_CPU_X86
/* Compiler supports X86 AVX Intrinsics */
# define  OPUS_X86_MAY_HAVE_AVX	1
/* Compiler supports X86 SSE Intrinsics */
# define  OPUS_X86_MAY_HAVE_SSE	1
//NOTE: SSE2 implies have AMD
/* Compiler supports X86 SSE2 Intrinsics */
# define  OPUS_X86_MAY_HAVE_SSE2	1
///* Define if binary requires SSE2 intrinsics support */
//#  define  OPUS_X86_PRESUME_SSE2	0
/* Compiler supports X86 SSE4.1 Intrinsics */
# define  OPUS_X86_MAY_HAVE_SSE4_1	1

# if HAVE_SSE
/* Define if binary requires SSE intrinsics support */
#  define  OPUS_X86_PRESUME_SSE	1

//NOTE: SSE2 implies have AMD
///* Define if binary requires SSE2 intrinsics support */
//#  define  OPUS_X86_PRESUME_SSE2		1
///* Define if binary requires SSE4.1 intrinsics support */
//#  define  OPUS_X86_PRESUME_SSE4_1	1
# endif // HAVE_SSE

# ifdef ARCH_AARCH64
/* Define if binary requires AVX intrinsics support */
//#  define  OPUS_X86_PRESUME_AVX		0
# endif // ARCH_AARCH64

#endif // TARGET_CPU_X86

/* Presume SSE functions, if compiled to use SSE/SSE2/AVX (note that AMD64 implies SSE2, and AVX
implies SSE4.1) */
#if defined(_M_X64) || (defined(_M_IX86_FP) && (_M_IX86_FP >= 1)) || defined(__AVX__)
#define OPUS_X86_PRESUME_SSE 1
#endif
#if defined(_M_X64) || (defined(_M_IX86_FP) && (_M_IX86_FP >= 2)) || defined(__AVX__)
#define OPUS_X86_PRESUME_SSE2 1
#endif
#if defined(__AVX__)
#define OPUS_X86_PRESUME_SSE4_1 1
#endif

#if !defined(OPUS_X86_PRESUME_SSE4_1) || !defined(OPUS_X86_PRESUME_SSE2) || !defined(OPUS_X86_PRESUME_SSE)
#define OPUS_HAVE_RTCD			1
#endif

#define ALIGN(stack, size) ((stack) += ((size) - (long)(stack)) & ((size) - 1))
#define PUSH(stack, size, type) (ALIGN((stack),sizeof(type)),(stack)+=((size)*sizeof(type)),(type*)((stack)-((size)*sizeof(type))))

//#if defined(VAR_ARRAYS)
//#define VARDECL(type, var) type *var
//#define ALLOC_OPUS(var, size, type) type var[size]
//#elif defined(USE_ALLOCA)
#define VARDECL(type, var) type *var
#define ALLOC_OPUS(var, size, type) var = alloca(sizeof(type)*(size))
//#else
//#define VARDECL(type, var) type *var
//#define ALLOC_OPUS(var, size, type) var = PUSH(stack, size, type)
//#endif
//

/* Define to the version of this package. */
//#define PACKAGE_VERSION "1.3"

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Make use of alloca */
/* #undef USE_ALLOCA */

/* Use C99 variable-size arrays */
#define VAR_ARRAYS 1


/* Use run-time CPU capabilities detection */
//#define  OPUS_HAVE_RTCD		0

//#include "version.h"
/* Get CPU Info by asm method */
//#define  CPU_INFO_BY_ASM	0

/* Get CPU Info by c method */
#define CPU_INFO_BY_C		1

/* Custom modes */
//#define  CUSTOM_MODES		0

/* Ambisonics Support */
//#define   ENABLE_EXPERIMENTAL_AMBISONICS 0


#ifdef GOTV_FLOATING_POINT
/* Float approximations */
# define  FLOAT_APPROX	1

#else 
/* Compile as fixed-point (for machines without a fast enough FPU) */
# define  FIXED_POINT	0
/* Debug fixed-point implementation */
# define  FIXED_DEBUG	1
/* Do not build the float API */
#define  DISABLE_FLOAT_API	1

#endif // FLOATING_POINT



/* Fuzzing */
//#define  FUZZING		0

/* Define to 1 if you have the `__malloc_hook' function. */
//#define  HAVE___MALLOC_HOOK	0

/* Define to the sub-directory where libtool stores uninstalled libraries. */
//#define  LT_OBJDIR	""

#ifdef TARGET_CPU_X86
/* Compiler supports X86 AVX Intrinsics */
# define  OPUS_X86_MAY_HAVE_AVX	1
/* Compiler supports X86 SSE Intrinsics */
# define  OPUS_X86_MAY_HAVE_SSE	1
//NOTE: SSE2 implies have AMD
/* Compiler supports X86 SSE2 Intrinsics */
# define  OPUS_X86_MAY_HAVE_SSE2	1
///* Define if binary requires SSE2 intrinsics support */
//#  define  OPUS_X86_PRESUME_SSE2	0
/* Compiler supports X86 SSE4.1 Intrinsics */
# define  OPUS_X86_MAY_HAVE_SSE4_1	1

# if HAVE_SSE
/* Define if binary requires SSE intrinsics support */
#  define  OPUS_X86_PRESUME_SSE	1

//NOTE: SSE2 implies have AMD
///* Define if binary requires SSE2 intrinsics support */
//#  define  OPUS_X86_PRESUME_SSE2		1
///* Define if binary requires SSE4.1 intrinsics support */
//#  define  OPUS_X86_PRESUME_SSE4_1	1
# endif // HAVE_SSE

# ifdef ARCH_AARCH64
/* Define if binary requires AVX intrinsics support */
//#  define  OPUS_X86_PRESUME_AVX		0
# endif // ARCH_AARCH64

#endif // TARGET_CPU_X86

/* Define to empty if `const' does not conform to ANSI C. */
/* #undef const */

/* Define to `__inline__' or `__inline' if that's what the C compiler
   calls it, or to nothing if 'inline' is not supported under any name.  */
#ifndef __cplusplus
/* #undef inline */
#endif

/* Define to the equivalent of the C99 'restrict' keyword, or to
   nothing if this is not supported.  Do not define if restrict is
   supported directly.  */
#define restrict __restrict
/* Work around a bug in Sun C++: it does not support _Restrict or
   __restrict__, even though the corresponding Sun C compiler ends up with
   "#define restrict _Restrict" or "#define restrict __restrict__" in the
   previous line.  Perhaps some future version of Sun C++ will work with
   restrict; if so, hopefully it defines __RESTRICT like Sun C does.  */
#if defined __SUNPRO_CC && !defined __RESTRICT
# define _Restrict
# define __restrict__
#endif
