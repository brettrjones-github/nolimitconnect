/* Copyright (C) 2008 CSIRO */
/**
   @file fixed_c6x.h
   @brief Fixed-point operations for the TI C6x DSP family
*/
/*
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:
   
   - Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
   
   - Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.
   
   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef FIXED_C6X_H
#define FIXED_C6X_H

#undef MULT16_16SU 
#define MULT16_16SU(a,b) _mpysu(a,b)

#undef MULT_16_16
#define MULT_16_16(a,b) _mpy(a,b)

#define libcelt_ilog2(x) (30 - _norm(x))
#define OVERRIDE_CELT_ILOG2

#undef MULT16_32_Q15
#define MULT16_32_Q15(a,b) (_mpylill(a, b) >> 15)

#if 0
#include "dsplib_libcelt.h"

#undef MAX16
#define MAX16(a,b) _max(a,b)

#undef MIN16
#define MIN16(a,b) _min(a,b)

#undef MAX32
#define MAX32(a,b) _lmax(a,b)

#undef MIN32
#define MIN32(a,b) _lmin(a,b)

#undef VSHR32
#define VSHR32(a, shift) _lshl(a,-(shift))

#undef MULT16_16_Q15
#define MULT16_16_Q15(a,b) (_smpy(a,b))

#define libcelt_maxabs16(x, len) MAX16(maxval((DATA *)x, len),-minval((DATA *)x, len))
#define OVERRIDE_CELT_MAXABS16

#define OVERRIDE_FIND_MAX16
static inline int find_max16(libcelt_word16 *x, int len)
{
   DATA max_corr16 = -VERY_LARGE16;
   DATA pitch16 = 0;
   maxvec((DATA *)x, len, &max_corr16, &pitch16);
   return pitch16;
}
#endif

#endif /* FIXED_C6X_H */
