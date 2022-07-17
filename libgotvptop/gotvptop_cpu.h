/*****************************************************************************
 * gotvptop_cpu.h: CPU capabilities
 *****************************************************************************
 * Copyright (C) 1998-2009 GOTV authors and VideoLAN
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

/**
 * \file
 * This file provides CPU features detection.
 */

#ifndef GOTV_CPU_H
# define GOTV_CPU_H 1

NLC_API unsigned gotvptop_CPU(void);

# if defined (__i386__) || defined (__x86_64__)
#  define HAVE_FPU 1
#  define GOTV_CPU_MMX    0x00000008
#  define GOTV_CPU_3dNOW  0x00000010
#  define GOTV_CPU_MMXEXT 0x00000020
#  define GOTV_CPU_SSE    0x00000040
#  define GOTV_CPU_SSE2   0x00000080
#  define GOTV_CPU_SSE3   0x00000100
#  define GOTV_CPU_SSSE3  0x00000200
#  define GOTV_CPU_SSE4_1 0x00000400
#  define GOTV_CPU_SSE4_2 0x00000800
#  define GOTV_CPU_SSE4A  0x00001000
#  define GOTV_CPU_AVX    0x00002000
#  define GOTV_CPU_AVX2   0x00004000
#  define GOTV_CPU_XOP    0x00008000
#  define GOTV_CPU_FMA4   0x00010000

# if defined (__MMX__)
#  define gotvptop_CPU_MMX() (1)
#  define GOTV_MMX
# else
#  define gotvptop_CPU_MMX() ((gotvptop_CPU() & GOTV_CPU_MMX) != 0)
#  define GOTV_MMX __attribute__ ((__target__ ("mmx")))
# endif

# if defined (__SSE__)
#  define gotvptop_CPU_MMXEXT() (1)
#  define gotvptop_CPU_SSE() (1)
#  define GOTV_SSE
# else
#  define gotvptop_CPU_MMXEXT() ((gotvptop_CPU() & GOTV_CPU_MMXEXT) != 0)
#  define gotvptop_CPU_SSE() ((gotvptop_CPU() & GOTV_CPU_SSE) != 0)
#  define GOTV_SSE __attribute__ ((__target__ ("sse")))
# endif

# ifdef __SSE2__
#  define gotvptop_CPU_SSE2() (1)
# else
#  define gotvptop_CPU_SSE2() ((gotvptop_CPU() & GOTV_CPU_SSE2) != 0)
# endif

# ifdef __SSE3__
#  define gotvptop_CPU_SSE3() (1)
# else
#  define gotvptop_CPU_SSE3() ((gotvptop_CPU() & GOTV_CPU_SSE3) != 0)
# endif

# ifdef __SSSE3__
#  define gotvptop_CPU_SSSE3() (1)
# else
#  define gotvptop_CPU_SSSE3() ((gotvptop_CPU() & GOTV_CPU_SSSE3) != 0)
# endif

# ifdef __SSE4_1__
#  define gotvptop_CPU_SSE4_1() (1)
# else
#  define gotvptop_CPU_SSE4_1() ((gotvptop_CPU() & GOTV_CPU_SSE4_1) != 0)
# endif

# ifdef __SSE4_2__
#  define gotvptop_CPU_SSE4_2() (1)
# else
#  define gotvptop_CPU_SSE4_2() ((gotvptop_CPU() & GOTV_CPU_SSE4_2) != 0)
# endif

# ifdef __SSE4A__
#  define gotvptop_CPU_SSE4A() (1)
# else
#  define gotvptop_CPU_SSE4A() ((gotvptop_CPU() & GOTV_CPU_SSE4A) != 0)
# endif

# ifdef __AVX__
#  define gotvptop_CPU_AVX() (1)
# else
#  define gotvptop_CPU_AVX() ((gotvptop_CPU() & GOTV_CPU_AVX) != 0)
# endif

# ifdef __AVX2__
#  define gotvptop_CPU_AVX2() (1)
# else
#  define gotvptop_CPU_AVX2() ((gotvptop_CPU() & GOTV_CPU_AVX2) != 0)
# endif

# ifdef __3dNOW__
#  define gotvptop_CPU_3dNOW() (1)
# else
#  define gotvptop_CPU_3dNOW() ((gotvptop_CPU() & GOTV_CPU_3dNOW) != 0)
# endif

# ifdef __XOP__
#  define gotvptop_CPU_XOP() (1)
# else
#  define gotvptop_CPU_XOP() ((gotvptop_CPU() & GOTV_CPU_XOP) != 0)
# endif

# ifdef __FMA4__
#  define gotvptop_CPU_FMA4() (1)
# else
#  define gotvptop_CPU_FMA4() ((gotvptop_CPU() & GOTV_CPU_FMA4) != 0)
# endif

# elif defined (__ppc__) || defined (__ppc64__) || defined (__powerpc__)
#  define HAVE_FPU 1
#  define GOTV_CPU_ALTIVEC 2

#  ifdef ALTIVEC
#   define gotvptop_CPU_ALTIVEC() (1)
#  else
#   define gotvptop_CPU_ALTIVEC() ((gotvptop_CPU() & GOTV_CPU_ALTIVEC) != 0)
#  endif

# elif defined (__arm__)
#  if defined (__VFP_FP__) && !defined (__SOFTFP__)
#   define HAVE_FPU 1
#  else
#   define HAVE_FPU 0
#  endif
#  define GOTV_CPU_ARMv6    4
#  define GOTV_CPU_ARM_NEON 2

#  if defined (__ARM_ARCH_7A__)
#   define GOTV_CPU_ARM_ARCH 7
#  elif defined (__ARM_ARCH_6__) || defined (__ARM_ARCH_6T2__)
#   define GOTV_CPU_ARM_ARCH 6
#  else
#   define GOTV_CPU_ARM_ARCH 4
#  endif

#  if (GOTV_CPU_ARM_ARCH >= 6)
#   define gotvptop_CPU_ARMv6() (1)
#  else
#   define gotvptop_CPU_ARMv6() ((gotvptop_CPU() & GOTV_CPU_ARMv6) != 0)
#  endif

#  ifdef __ARM_NEON__
#   define gotvptop_CPU_ARM_NEON() (1)
#  else
#   define gotvptop_CPU_ARM_NEON() ((gotvptop_CPU() & GOTV_CPU_ARM_NEON) != 0)
#  endif

# elif defined (__aarch64__)
#  define HAVE_FPU 1
// NEON is mandatory for general purpose ARMv8-a CPUs
#  define gotvptop_CPU_ARM64_NEON() (1)

# elif defined (__sparc__)
#  define HAVE_FPU 1

# elif defined (__mips_hard_float)
#  define HAVE_FPU 1

# else
/**
 * Are single precision floating point operations "fast"?
 * If this preprocessor constant is zero, floating point should be avoided
 * (especially relevant for audio codecs).
 */
#  define HAVE_FPU 0

# endif

#endif /* !GOTV_CPU_H */
