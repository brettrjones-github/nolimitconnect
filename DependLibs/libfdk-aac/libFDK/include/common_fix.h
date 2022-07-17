
/* -----------------------------------------------------------------------------------------------------------
Software License for The Fraunhofer FDK AAC Codec Library for Android

© Copyright  1995 - 2013 Fraunhofer-Gesellschaft zur Förderung der angewandten Forschung e.V.
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

   Author(s):   M. Lohwasser, M. Gayer
   Description: Flexible fixpoint library configuration

******************************************************************************/

#ifndef _COMMON_FIX_H_FDK
#define _COMMON_FIX_H_FDK

#include "machine_type.h"

/* ***** Start of former fix.h ****** */

/* Configure fractional or integer arithmetic */
  #define FIX_FRACT 0 /* Define this to "1" to use fractional arithmetic simulation in class fract instead of integer arithmetic */
                      /* 1 for debug with extra runtime overflow checking.                                                      */

/* Define bit sizes of integer fixpoint fractional data types */
#define FRACT_BITS      16 /* single precision */
#define DFRACT_BITS     32 /* double precision */
#define ACCU_BITS       40 /* double precision plus overflow */

/* Fixpoint equivalent type fot PCM audio time domain data. */
#if defined(SAMPLE_BITS)
#if (SAMPLE_BITS == DFRACT_BITS)
  #define FIXP_PCM      FIXP_DBL
  #define FX_PCM2FX_DBL(x) ((FIXP_DBL)(x))
  #define FX_DBL2FX_PCM(x) ((INT_PCM)(x))
#elif (SAMPLE_BITS == FRACT_BITS)
  #define FIXP_PCM      FIXP_SGL
  #define FX_PCM2FX_DBL(x) FX_SGL2FX_DBL((FIXP_SGL)(x))
  #define FX_DBL2FX_PCM(x) FX_DBL2FX_SGL(x)
#else
  #error SAMPLE_BITS different from FRACT_BITS or DFRACT_BITS not implemented!
#endif
#endif

/* ****** End of former fix.h ****** */

#define SGL_MASK            ((1UL<<FRACT_BITS)-1)   /* 16bit: (2^16)-1 = 0xFFFF */

#define MAX_SHIFT_SGL (FRACT_BITS-1)   /* maximum possible shift for FIXP_SGL values */
#define MAX_SHIFT_DBL (DFRACT_BITS-1)  /* maximum possible shift for FIXP_DBL values */

/* Scale factor from/to float/fixpoint values. DO NOT USE THESE VALUES AS SATURATION LIMITS !! */
#define FRACT_FIX_SCALE     ((int64_t(1)<<(FRACT_BITS-1)))
#define DFRACT_FIX_SCALE    ((int64_t(1)<<(DFRACT_BITS-1)))

/* Max and Min values for saturation purposes. DO NOT USE THESE VALUES AS SCALE VALUES !! */
#define  MAXVAL_SGL     ((signed)0x00007FFF)    /* this has to be synchronized to FRACT_BITS */
#define  MINVAL_SGL     ((signed)0xFFFF8000)    /* this has to be synchronized to FRACT_BITS */
#define  MAXVAL_DBL     ((signed)0x7FFFFFFF)    /* this has to be synchronized to DFRACT_BITS */
#define  MINVAL_DBL     ((signed)0x80000000)    /* this has to be synchronized to DFRACT_BITS */


#define FX_DBL2FXCONST_SGL(val) ( ( ((((val) >> (DFRACT_BITS-FRACT_BITS-1)) + 1) > (((int32_t)1<<FRACT_BITS)-1)) && ((int32_t)(val) > 0) ) ? \
   (FIXP_SGL)(int16_t)(((int32_t)1<<(FRACT_BITS-1))-1):(FIXP_SGL)(int16_t)((((val) >> (DFRACT_BITS-FRACT_BITS-1)) + 1) >> 1) )



#define shouldBeUnion union     /* unions are possible */

    typedef int16_t       FIXP_SGL;
    typedef int32_t        FIXP_DBL;

/* macros for compile-time conversion of constant float values to fixedpoint */
#define FL2FXCONST_SPC FL2FXCONST_DBL

#define MINVAL_DBL_CONST MINVAL_DBL
#define MINVAL_SGL_CONST MINVAL_SGL

#define FL2FXCONST_SGL(val)                                                                                                     \
(FIXP_SGL)( ( (val) >= 0) ?                                                                                                               \
((( (double)(val) * (FRACT_FIX_SCALE) + 0.5 ) >= (double)(MAXVAL_SGL) ) ? (int16_t)(MAXVAL_SGL) : (int16_t)( (double)(val) * (double)(FRACT_FIX_SCALE) + 0.5)) :   \
((( (double)(val) * (FRACT_FIX_SCALE) - 0.5) <=  (double)(MINVAL_SGL_CONST) ) ? (int16_t)(MINVAL_SGL_CONST) : (int16_t)( (double)(val) * (double)(FRACT_FIX_SCALE) - 0.5)) )

#define FL2FXCONST_DBL(val)                                                                                                     \
(FIXP_DBL)( ( (val) >= 0) ?                                                                                                               \
((( (double)(val) * (DFRACT_FIX_SCALE) + 0.5 ) >= (double)(MAXVAL_DBL) ) ? (int32_t)(MAXVAL_DBL) : (int32_t)( (double)(val) * (double)(DFRACT_FIX_SCALE) + 0.5)) : \
((( (double)(val) * (DFRACT_FIX_SCALE) - 0.5) <=  (double)(MINVAL_DBL_CONST) ) ? (int32_t)(MINVAL_DBL_CONST) : (int32_t)( (double)(val) * (double)(DFRACT_FIX_SCALE) - 0.5)) )

/* macros for runtime conversion of float values to integer fixedpoint. NO OVERFLOW CHECK!!! */
#define FL2FX_SPC FL2FX_DBL
#define FL2FX_SGL(val) ( (val)>0.0f ? (int16_t)( (val)*(float)(FRACT_FIX_SCALE)+0.5f ) : (int16_t)( (val)*(float)(FRACT_FIX_SCALE)-0.5f ) )
#define FL2FX_DBL(val) ( (val)>0.0f ? (int32_t)( (val)*(float)(DFRACT_FIX_SCALE)+0.5f ) : (int32_t)( (val)*(float)(DFRACT_FIX_SCALE)-0.5f ) )

/* macros for runtime conversion of fixedpoint values to other fixedpoint. NO ROUNDING!!! */
#define FX_ACC2FX_SGL(val) ((FIXP_SGL)((val)>>(ACCU_BITS-FRACT_BITS)))
#define FX_ACC2FX_DBL(val) ((FIXP_DBL)((val)>>(ACCU_BITS-DFRACT_BITS)))
#define FX_SGL2FX_ACC(val) ((FIXP_ACC)((int32_t)(val)<<(ACCU_BITS-FRACT_BITS)))
#define FX_SGL2FX_DBL(val) ((FIXP_DBL)((int32_t)(val)<<(DFRACT_BITS-FRACT_BITS)))
#define FX_DBL2FX_SGL(val) ((FIXP_SGL)((val)>>(DFRACT_BITS-FRACT_BITS)))

/* ############################################################# */

/* macros for runtime conversion of integer fixedpoint values to float. */
/* This is just for temporary use and should not be required in a final version! */

/* #define FX_DBL2FL(val)  ((float)(pow(2.,-31.)*(float)val)) */  /* version #1 */
#define FX_DBL2FL(val)  ((float)((double)(val)/(double)DFRACT_FIX_SCALE))   /* version #2 - identical to class dfract cast from dfract to float */

/* ############################################################# */
#include "fixmul.h"

NLC_INLINE int32_t fMult(int16_t a, int16_t b)         { return fixmul_SS(a, b); }
NLC_INLINE int32_t fMult(int16_t a, int32_t b)          { return fixmul_SD(a, b); }
NLC_INLINE int32_t fMult(int32_t a, int16_t b)          { return fixmul_DS(a, b); }
NLC_INLINE int32_t fMult(int32_t a, int32_t b)           { return fixmul_DD(a, b); }
NLC_INLINE int32_t fPow2(int32_t a)                   { return fixpow2_D(a);    }
NLC_INLINE int32_t fPow2(int16_t a)                  { return fixpow2_S(a);    }

NLC_INLINE int32_t  fMultI(int32_t a, int16_t b)         { return ( (int32_t)(((1<<(FRACT_BITS-2)) +
                                                          fixmuldiv2_DD(a,((int32_t)b<<FRACT_BITS)))>>(FRACT_BITS-1)) ); }

NLC_INLINE int32_t  fMultIfloor(int32_t a, int32_t b)      { return ( (int32_t)((1 +
                                                          fixmuldiv2_DD(a,(b<<FRACT_BITS))) >> (FRACT_BITS-1)) ); }

NLC_INLINE int32_t  fMultIceil(int32_t a, int32_t b)       { return ( (int32_t)(((int32_t)0x7fff +
                                                          fixmuldiv2_DD(a,(b<<FRACT_BITS))) >> (FRACT_BITS-1)) ); }

NLC_INLINE int32_t fMultDiv2(int16_t a, int16_t b)     { return fixmuldiv2_SS(a, b); }
NLC_INLINE int32_t fMultDiv2(int16_t a, int32_t b)      { return fixmuldiv2_SD(a, b); }
NLC_INLINE int32_t fMultDiv2(int32_t a, int16_t b)      { return fixmuldiv2_DS(a, b); }
NLC_INLINE int32_t fMultDiv2(int32_t a, int32_t b)       { return fixmuldiv2_DD(a, b); }
NLC_INLINE int32_t fPow2Div2(int32_t a)               { return fixpow2div2_D(a);    }
NLC_INLINE int32_t fPow2Div2(int16_t a)              { return fixpow2div2_S(a);    }

NLC_INLINE int32_t fMultDiv2BitExact(int32_t a, int32_t b)   { return fixmuldiv2BitExact_DD(a, b); }
NLC_INLINE int32_t fMultDiv2BitExact(int16_t a, int32_t  b) { return fixmuldiv2BitExact_SD(a, b); }
NLC_INLINE int32_t fMultDiv2BitExact(int32_t  a, int16_t b) { return fixmuldiv2BitExact_DS(a, b); }
NLC_INLINE int32_t fMultBitExact(int32_t a, int32_t b)       { return fixmulBitExact_DD(a, b); }
NLC_INLINE int32_t fMultBitExact(int16_t a, int32_t  b)     { return fixmulBitExact_SD(a, b); }
NLC_INLINE int32_t fMultBitExact(int32_t  a, int16_t b)     { return fixmulBitExact_DS(a, b); }

/* ******************************************************************************** */
#include "abs.h"

NLC_INLINE FIXP_DBL fAbs_FDK(FIXP_DBL x)
                { return fixabs_D(x); }
NLC_INLINE FIXP_SGL fAbs_FDK(FIXP_SGL x)
                { return fixabs_S(x); }

///* workaround for TI C6x compiler but not for TI ARM9E compiler */
//#if (!defined(__TI_COMPILER_VERSION__) || defined(__TI_TMS470_V5__)) && !defined(__LP64__)
//NLC_INLINE int32_t  fAbs_FDK(int32_t x)
//                { return fixabs_I(x); }
//#endif

/* ******************************************************************************** */

#include "clz.h"

NLC_INLINE int32_t fNormz_FDK(FIXP_DBL x)
               { return fixnormz_D(x); }
NLC_INLINE int32_t fNormz_FDK(FIXP_SGL x)
               { return fixnormz_S(x); }
NLC_INLINE int32_t fNorm(FIXP_DBL x)
               { return fixnorm_D(x); }
NLC_INLINE int32_t fNorm(FIXP_SGL x)
               { return fixnorm_S(x); }


/* ******************************************************************************** */
/* ******************************************************************************** */
/* ******************************************************************************** */

#include "clz.h"
#define fixp_abs(x) fAbs_FDK(x)
#define fixMin(a,b) fMin_FDK(a,b)
#define fixMax(a,b) fMax_FDK(a,b)
#define CntLeadingZeros(x)  fixnormz_D(x)
#define CountLeadingBits(x) fixnorm_D(x)

#include "fixmadd.h"

/* y = (x+0.5*a*b) */
NLC_INLINE FIXP_DBL fMultAddDiv2(FIXP_DBL x, FIXP_DBL a, FIXP_DBL b)
                { return fixmadddiv2_DD(x, a, b); }
NLC_INLINE FIXP_DBL fMultAddDiv2(FIXP_DBL x, FIXP_SGL a, FIXP_DBL b)
                { return fixmadddiv2_SD(x, a, b); }
NLC_INLINE FIXP_DBL fMultAddDiv2(FIXP_DBL x, FIXP_DBL a, FIXP_SGL b)
                { return fixmadddiv2_DS(x, a, b); }
NLC_INLINE FIXP_DBL fMultAddDiv2(FIXP_DBL x, FIXP_SGL a, FIXP_SGL b)
                { return fixmadddiv2_SS(x, a, b); }

NLC_INLINE FIXP_DBL fPow2AddDiv2(FIXP_DBL x, FIXP_DBL a)
                { return fixpadddiv2_D(x, a); }
NLC_INLINE FIXP_DBL fPow2AddDiv2(FIXP_DBL x, FIXP_SGL a)
                { return fixpadddiv2_S(x, a); }


/* y = 2*(x+0.5*a*b) = (2x+a*b) */
NLC_INLINE FIXP_DBL fMultAdd(FIXP_DBL x, FIXP_DBL a, FIXP_DBL b)
                { return fixmadd_DD(x, a, b); }
NLC_INLINE FIXP_DBL fMultAdd(FIXP_DBL x, FIXP_SGL a, FIXP_DBL b)
                { return fixmadd_SD(x, a, b); }
NLC_INLINE FIXP_DBL fMultAdd(FIXP_DBL x, FIXP_DBL a, FIXP_SGL b)
                { return fixmadd_DS(x, a, b); }
NLC_INLINE FIXP_DBL fMultAdd(FIXP_DBL x, FIXP_SGL a, FIXP_SGL b)
                { return fixmadd_SS(x, a, b); }

NLC_INLINE FIXP_DBL fPow2Add(FIXP_DBL x, FIXP_DBL a)
                { return fixpadd_D(x, a); }
NLC_INLINE FIXP_DBL fPow2Add(FIXP_DBL x, FIXP_SGL a)
                { return fixpadd_S(x, a); }


/* y = (x-0.5*a*b) */
NLC_INLINE FIXP_DBL fMultSubDiv2(FIXP_DBL x, FIXP_DBL a, FIXP_DBL b)
                { return fixmsubdiv2_DD(x, a, b); }
NLC_INLINE FIXP_DBL fMultSubDiv2(FIXP_DBL x, FIXP_SGL a, FIXP_DBL b)
                { return fixmsubdiv2_SD(x, a, b); }
NLC_INLINE FIXP_DBL fMultSubDiv2(FIXP_DBL x, FIXP_DBL a, FIXP_SGL b)
                { return fixmsubdiv2_DS(x, a, b); }
NLC_INLINE FIXP_DBL fMultSubDiv2(FIXP_DBL x, FIXP_SGL a, FIXP_SGL b)
                { return fixmsubdiv2_SS(x, a, b); }

/* y = 2*(x-0.5*a*b) = (2*x-a*b) */
NLC_INLINE FIXP_DBL fMultSub(FIXP_DBL x, FIXP_DBL a, FIXP_DBL b)
                { return fixmsub_DD(x, a, b); }
NLC_INLINE FIXP_DBL fMultSub(FIXP_DBL x, FIXP_SGL a, FIXP_DBL b)
                { return fixmsub_SD(x, a, b); }
NLC_INLINE FIXP_DBL fMultSub(FIXP_DBL x, FIXP_DBL a, FIXP_SGL b)
                { return fixmsub_DS(x, a, b); }
NLC_INLINE FIXP_DBL fMultSub(FIXP_DBL x, FIXP_SGL a, FIXP_SGL b)
                { return fixmsub_SS(x, a, b); }

NLC_INLINE FIXP_DBL fMultAddDiv2BitExact(FIXP_DBL x, FIXP_DBL a, FIXP_DBL b)
                { return fixmadddiv2BitExact_DD(x, a, b); }
NLC_INLINE FIXP_DBL fMultAddDiv2BitExact(FIXP_DBL x, FIXP_SGL a, FIXP_DBL b)
                { return fixmadddiv2BitExact_SD(x, a, b); }
NLC_INLINE FIXP_DBL fMultAddDiv2BitExact(FIXP_DBL x, FIXP_DBL a, FIXP_SGL b)
                { return fixmadddiv2BitExact_DS(x, a, b); }
NLC_INLINE FIXP_DBL fMultSubDiv2BitExact(FIXP_DBL x, FIXP_DBL a, FIXP_DBL b)
                { return fixmsubdiv2BitExact_DD(x, a, b); }
NLC_INLINE FIXP_DBL fMultSubDiv2BitExact(FIXP_DBL x, FIXP_SGL a, FIXP_DBL b)
                { return fixmsubdiv2BitExact_SD(x, a, b); }
NLC_INLINE FIXP_DBL fMultSubDiv2BitExact(FIXP_DBL x, FIXP_DBL a, FIXP_SGL b)
                { return fixmsubdiv2BitExact_DS(x, a, b); }

#include "fixminmax.h"

NLC_INLINE FIXP_DBL fMin_FDK(FIXP_DBL a, FIXP_DBL b)
                { return fixmin_D(a,b); }
NLC_INLINE FIXP_DBL fMax_FDK(FIXP_DBL a, FIXP_DBL b)
                { return fixmax_D(a,b); }

NLC_INLINE FIXP_SGL fMin_FDK(FIXP_SGL a, FIXP_SGL b)
                { return fixmin_S(a,b); }
NLC_INLINE FIXP_SGL fMax_FDK(FIXP_SGL a, FIXP_SGL b)
                { return fixmax_S(a,b); }

///* workaround for TI C6x compiler but not for TI ARM9E */
//#if ((!defined(__TI_COMPILER_VERSION__) || defined(__TI_TMS470_V5__)) && !defined(__LP64__)) || (FIX_FRACT == 1)
//NLC_INLINE int32_t fMax_FDK(int32_t a, int32_t b)
//                { return fixmax_I(a,b); }
//NLC_INLINE int32_t fMin_FDK(int32_t a, int32_t b)
//                { return fixmin_I(a,b); }
//#endif

NLC_INLINE uint32_t fMax_FDK(uint32_t a, uint32_t b)
                { return fixmax_UI(a,b); }
NLC_INLINE uint32_t fMin_FDK(uint32_t a, uint32_t b)
                { return fixmin_UI(a,b); }

/* Complex data types */
typedef shouldBeUnion {
  /* vector representation for arithmetic */
  struct {
    FIXP_SGL re;
    FIXP_SGL im;
  } v;
  /* word representation for memory move */
  int32_t w;
} FIXP_SPK;

typedef shouldBeUnion {
  /* vector representation for arithmetic */
  struct {
    FIXP_DBL re;
    FIXP_DBL im;
  } v;
  /* word representation for memory move */
  int64_t w;
} FIXP_DPK;

#include "fixmul.h"
#include "fixmadd.h"
#include "cplx_mul.h"
#include "scale.h"
#include "fixpoint_math.h"

#endif
