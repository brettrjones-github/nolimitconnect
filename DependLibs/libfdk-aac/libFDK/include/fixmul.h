
/* -----------------------------------------------------------------------------------------------------------
Software License for The Fraunhofer FDK AAC Codec Library for Android

� Copyright  1995 - 2013 Fraunhofer-Gesellschaft zur F�rderung der angewandten Forschung e.V.
  All rights reserved.

 1.    INTRODUCTION
The Fraunhofer FDK AAC Codec Library for Android ("FDK AAC Codec") is software that implements
the MPEG Advanced Audio Coding ("AAC") encoding and decoding scheme for digital audio.
This FDK AAC Codec software is intended to be used on a wide variety of Android devices.

AAC's HE-AAC and HE-AAC v2 versions are regarded as today's most efficient general perceptual
audio codecs. AAC-ELD is considered the best-performing full-bandwidth communications codec by
independent studies and is widely deployed. AAC has been standardized by ISO and IEC as part
of the MPEG specifications.

Patent licenses for necessary patent claims for the FDK AAC Codec (including those of Fraunhofer)
may be obtained through Via Licensing (www.vialicensing.com) or through the respective patent owners
individually for the purpose of encoding or decoding bit streams in products that are compliant with
the ISO/IEC MPEG audio standards. Please note that most manufacturers of Android devices already license
these patent claims through Via Licensing or directly from the patent owners, and therefore FDK AAC Codec
software may already be covered under those patent licenses when it is used for those licensed purposes only.

Commercially-licensed AAC software libraries, including floating-point versions with enhanced sound quality,
are also available from Fraunhofer. Users are encouraged to check the Fraunhofer website for additional
applications information and documentation.

2.    COPYRIGHT LICENSE

Redistribution and use in source and binary forms, with or without modification, are permitted without
payment of copyright license fees provided that you satisfy the following conditions:

You must retain the complete text of this software license in redistributions of the FDK AAC Codec or
your modifications thereto in source code form.

You must retain the complete text of this software license in the documentation and/or other materials
provided with redistributions of the FDK AAC Codec or your modifications thereto in binary form.
You must make available free of charge copies of the complete source code of the FDK AAC Codec and your
modifications thereto to recipients of copies in binary form.

The name of Fraunhofer may not be used to endorse or promote products derived from this library without
prior written permission.

You may not charge copyright license fees for anyone to use, copy or distribute the FDK AAC Codec
software or your modifications thereto.

Your modified versions of the FDK AAC Codec must carry prominent notices stating that you changed the software
and the date of any change. For modified versions of the FDK AAC Codec, the term
"Fraunhofer FDK AAC Codec Library for Android" must be replaced by the term
"Third-Party Modified Version of the Fraunhofer FDK AAC Codec Library for Android."

3.    NO PATENT LICENSE

NO EXPRESS OR IMPLIED LICENSES TO ANY PATENT CLAIMS, including without limitation the patents of Fraunhofer,
ARE GRANTED BY THIS SOFTWARE LICENSE. Fraunhofer provides no warranty of patent non-infringement with
respect to this software.

You may use this FDK AAC Codec software or modifications thereto only for purposes that are authorized
by appropriate patent licenses.

4.    DISCLAIMER

This FDK AAC Codec software is provided by Fraunhofer on behalf of the copyright holders and contributors
"AS IS" and WITHOUT ANY EXPRESS OR IMPLIED WARRANTIES, including but not limited to the implied warranties
of merchantability and fitness for a particular purpose. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
CONTRIBUTORS BE LIABLE for any direct, indirect, incidental, special, exemplary, or consequential damages,
including but not limited to procurement of substitute goods or services; loss of use, data, or profits,
or business interruption, however caused and on any theory of liability, whether in contract, strict
liability, or tort (including negligence), arising in any way out of the use of this software, even if
advised of the possibility of such damage.

5.    CONTACT INFORMATION

Fraunhofer Institute for Integrated Circuits IIS
Attention: Audio and Multimedia Departments - FDK AAC LL
Am Wolfsmantel 33
91058 Erlangen, Germany

www.iis.fraunhofer.de/amm
amm-info@iis.fraunhofer.de
----------------------------------------------------------------------------------------------------------- */

/***************************  Fraunhofer IIS FDK Tools  **********************

   Author(s):   Stefan Gewinner
   Description: fixed point multiplication

******************************************************************************/

#if !defined(__FIXMUL_H__)
#define __FIXMUL_H__

#include "FDK_archdef.h"
#include "machine_type.h"


#if defined(__arm__)
#include "arm/fixmul_arm.h"

#elif defined(__aarch64__) || defined(__AARCH64EL__)
#include "aarch64/fixmul_aarch64.h"

#elif defined(__mips__)	/* cppp replaced: elif */
#include "mips/fixmul_mips.h"

#elif defined(__x86__)	/* cppp replaced: elif */
#include "x86/fixmul_x86.h"

#elif defined(__powerpc__)
#include "ppc/fixmul_ppc.h"

#endif /* all cores */

/*************************************************************************
 *************************************************************************
    Software fallbacks for missing functions
**************************************************************************
**************************************************************************/

#if !defined(FUNCTION_fixmuldiv2_DD)
#define FUNCTION_fixmuldiv2_DD
#if defined(_MSC_VER) || defined(__CC_ARM) || defined(__ANALOG_EXTENSIONS__) || defined(__TI_COMPILER_VERSION__)
#pragma message ("Extremely slow implementation of fixmuldiv2_DD !!")
#else
#warning Extremely slow implementation of fixmuldiv2_DD !!
#endif
GOTV_INLINE int32_t fixmuldiv2_DD (const int32_t a, const int32_t b)
{
  return (int32_t) ((((int64_t)a) * b) >> 32) ;
}
#endif

#if !defined(FUNCTION_fixmuldiv2BitExact_DD)
#define FUNCTION_fixmuldiv2BitExact_DD
#if defined(_MSC_VER) || defined(__CC_ARM) || defined(__ANALOG_EXTENSIONS__) || defined(__TI_COMPILER_VERSION__)
#pragma message ("Extremely slow implementation of fixmuldiv2BitExact_DD !!")
#else
#warning Extremely slow implementation of fixmuldiv2BitExact_DD !!
#endif
GOTV_INLINE int32_t fixmuldiv2BitExact_DD (const int32_t a, const int32_t b)
{
  return (int32_t) ((((int64_t)a) * b) >> 32) ;
}
#endif

#if !defined(FUNCTION_fixmul_DD)
#define FUNCTION_fixmul_DD
GOTV_INLINE int32_t fixmul_DD (const int32_t a, const int32_t b)
  { return fixmuldiv2_DD (a, b) << 1 ; }
#endif

#if !defined(FUNCTION_fixmulBitExact_DD)
#define FUNCTION_fixmulBitExact_DD
#if defined(_MSC_VER) || defined(__CC_ARM) || defined(__ANALOG_EXTENSIONS__) || defined(__TI_COMPILER_VERSION__) || defined(__XTENSA__)
#pragma message ("Extremely slow implementation of fixmulBitExact_DD !!")
#else
#warning Extremely slow implementation of fixmulBitExact_DD !!
#endif
GOTV_INLINE int32_t fixmulBitExact_DD (const int32_t a, const int32_t b)
{
  return ( (int32_t) ((((int64_t)a) * b) >> 32) ) << 1;
}
#endif

#if !defined(FUNCTION_fixmuldiv2_SS)
#define FUNCTION_fixmuldiv2_SS
GOTV_INLINE int32_t fixmuldiv2_SS (const int16_t a, const int16_t b)
  { return ((int32_t)a*b); }
#endif

#if !defined(FUNCTION_fixmul_SS)
#define FUNCTION_fixmul_SS
GOTV_INLINE int32_t fixmul_SS (const int16_t a, const int16_t b)
  { return (a*b) <<1; }
#endif

#if !defined(FUNCTION_fixmuldiv2_SD)
#define FUNCTION_fixmuldiv2_SD
GOTV_INLINE int32_t fixmuldiv2_SD (const int16_t a, const int32_t b)
#ifdef FUNCTION_fixmuldiv2_DS
  { return fixmuldiv2_DS(b, a); }
#else
  { return fixmuldiv2_DD(FX_SGL2FX_DBL(a), b); }
#endif
#endif

#if !defined(FUNCTION_fixmuldiv2_DS)
#define FUNCTION_fixmuldiv2_DS
GOTV_INLINE int32_t fixmuldiv2_DS (const int32_t a, const int16_t b)
#ifdef FUNCTION_fixmuldiv2_SD
  { return fixmuldiv2_SD(b, a); }
#else
  { return fixmuldiv2_DD(a, FX_SGL2FX_DBL(b)); }
#endif
#endif

#if !defined(FUNCTION_fixmuldiv2BitExact_SD)
#define FUNCTION_fixmuldiv2BitExact_SD
GOTV_INLINE int32_t fixmuldiv2BitExact_SD (const int16_t a, const int32_t b)
#ifdef FUNCTION_fixmuldiv2BitExact_DS
  { return fixmuldiv2BitExact_DS(b, a); }
#else
  { return (int32_t) ((((int64_t)a) * b) >> 16) ; }
#endif
#endif

#if !defined(FUNCTION_fixmuldiv2BitExact_DS)
#define FUNCTION_fixmuldiv2BitExact_DS
GOTV_INLINE int32_t fixmuldiv2BitExact_DS (const int32_t a, const int16_t b)
#ifdef FUNCTION_fixmuldiv2BitExact_SD
  { return fixmuldiv2BitExact_SD(b, a); }
#else
  { return (int32_t) ((((int64_t)a) * b) >> 16) ; }
#endif
#endif

#if !defined(FUNCTION_fixmul_SD)
#define FUNCTION_fixmul_SD
GOTV_INLINE int32_t fixmul_SD (const int16_t a, const int32_t b) {
#ifdef FUNCTION_fixmul_DS
  return fixmul_SD(b, a);
#else
  return fixmuldiv2_SD (a, b) << 1 ;
#endif
}
#endif

#if !defined(FUNCTION_fixmul_DS)
#define FUNCTION_fixmul_DS
GOTV_INLINE int32_t fixmul_DS (const int32_t a, const int16_t b) {
#ifdef FUNCTION_fixmul_SD
  return fixmul_SD(b, a);
#else
  return fixmuldiv2_DS(a, b) << 1 ;
#endif
}
#endif

#if !defined(FUNCTION_fixmulBitExact_SD)
#define FUNCTION_fixmulBitExact_SD
GOTV_INLINE int32_t fixmulBitExact_SD (const int16_t a, const int32_t b)
#ifdef FUNCTION_fixmulBitExact_DS
  { return fixmulBitExact_DS(b, a); }
#else
  { return (int32_t) (((((int64_t)a) * b) >> 16) << 1); }
#endif
#endif

#if !defined(FUNCTION_fixmulBitExact_DS)
#define FUNCTION_fixmulBitExact_DS
GOTV_INLINE int32_t fixmulBitExact_DS (const int32_t a, const int16_t b)
#ifdef FUNCTION_fixmulBitExact_SD
  { return fixmulBitExact_SD(b, a); }
#else
  { return (int32_t) (((((int64_t)a) * b) >> 16) << 1); }
#endif
#endif


#if !defined(FUNCTION_fixpow2div2_D)
#ifdef ARCH_WA_16BITMULT
#error Fallback for fixpow2div2_D is not 16 bit safe !
#endif
#define FUNCTION_fixpow2div2_D
GOTV_INLINE int32_t fixpow2div2_D (const int32_t a)
  { return fixmuldiv2_DD(a, a); }
#endif

#if !defined(FUNCTION_fixpow2_D)
#ifdef ARCH_WA_16BITMULT
#error Fallback for fixpow2_D is not 16 bit safe !
#endif
#define FUNCTION_fixpow2_D
GOTV_INLINE int32_t fixpow2_D (const int32_t a)
  { return fixpow2div2_D(a)<<1; }
#endif

#if !defined(FUNCTION_fixpow2div2_S)
#define FUNCTION_fixpow2div2_S
GOTV_INLINE int32_t fixpow2div2_S (const int16_t a)
  { return fixmuldiv2_SS(a, a); }
#endif

#if !defined(FUNCTION_fixpow2_S)
#define FUNCTION_fixpow2_S
GOTV_INLINE int32_t fixpow2_S (const int16_t a)
  { return fixpow2div2_S(a)<<1; }
#endif


#endif /* __FIXMUL_H__ */
