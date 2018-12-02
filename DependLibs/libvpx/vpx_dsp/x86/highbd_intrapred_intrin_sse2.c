/*
 *  Copyright (c) 2017 The WebM project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include <emmintrin.h>  // SSE2

#include "./vpx_config.h"
#include "./vpx_dsp_rtcd.h"
#include "vpx/vpx_integer.h"

// -----------------------------------------------------------------------------

void vpx_highbd_h_predictor_4x4_sse2(uint16_t *dst, ptrdiff_t stride,
                                     const uint16_t *above,
                                     const uint16_t *left, int bd) {
  const __m128i left_u16 = _mm_loadl_epi64((const __m128i *)left);
  const __m128i row0 = _mm_shufflelo_epi16(left_u16, 0x0);
  const __m128i row1 = _mm_shufflelo_epi16(left_u16, 0x55);
  const __m128i row2 = _mm_shufflelo_epi16(left_u16, 0xaa);
  const __m128i row3 = _mm_shufflelo_epi16(left_u16, 0xff);
  (void)above;
  (void)bd;
  _mm_storel_epi64((__m128i *)dst, row0);
  dst += stride;
  _mm_storel_epi64((__m128i *)dst, row1);
  dst += stride;
  _mm_storel_epi64((__m128i *)dst, row2);
  dst += stride;
  _mm_storel_epi64((__m128i *)dst, row3);
}

void vpx_highbd_h_predictor_8x8_sse2(uint16_t *dst, ptrdiff_t stride,
                                     const uint16_t *above,
                                     const uint16_t *left, int bd) {
  const __m128i left_u16 = _mm_load_si128((const __m128i *)left);
  const __m128i row0 = _mm_shufflelo_epi16(left_u16, 0x0);
  const __m128i row1 = _mm_shufflelo_epi16(left_u16, 0x55);
  const __m128i row2 = _mm_shufflelo_epi16(left_u16, 0xaa);
  const __m128i row3 = _mm_shufflelo_epi16(left_u16, 0xff);
  const __m128i row4 = _mm_shufflehi_epi16(left_u16, 0x0);
  const __m128i row5 = _mm_shufflehi_epi16(left_u16, 0x55);
  const __m128i row6 = _mm_shufflehi_epi16(left_u16, 0xaa);
  const __m128i row7 = _mm_shufflehi_epi16(left_u16, 0xff);
  (void)above;
  (void)bd;
  _mm_store_si128((__m128i *)dst, _mm_unpacklo_epi64(row0, row0));
  dst += stride;
  _mm_store_si128((__m128i *)dst, _mm_unpacklo_epi64(row1, row1));
  dst += stride;
  _mm_store_si128((__m128i *)dst, _mm_unpacklo_epi64(row2, row2));
  dst += stride;
  _mm_store_si128((__m128i *)dst, _mm_unpacklo_epi64(row3, row3));
  dst += stride;
  _mm_store_si128((__m128i *)dst, _mm_unpackhi_epi64(row4, row4));
  dst += stride;
  _mm_store_si128((__m128i *)dst, _mm_unpackhi_epi64(row5, row5));
  dst += stride;
  _mm_store_si128((__m128i *)dst, _mm_unpackhi_epi64(row6, row6));
  dst += stride;
  _mm_store_si128((__m128i *)dst, _mm_unpackhi_epi64(row7, row7));
}

static INLINE void h_store_16_unpacklo(uint16_t **dst, const ptrdiff_t stride,
                                       const __m128i *row) {
  const __m128i val = _mm_unpacklo_epi64(*row, *row);
  _mm_store_si128((__m128i *)*dst, val);
  _mm_store_si128((__m128i *)(*dst + 8), val);
  *dst += stride;
}

static INLINE void h_store_16_unpackhi(uint16_t **dst, const ptrdiff_t stride,
                                       const __m128i *row) {
  const __m128i val = _mm_unpackhi_epi64(*row, *row);
  _mm_store_si128((__m128i *)(*dst), val);
  _mm_store_si128((__m128i *)(*dst + 8), val);
  *dst += stride;
}

void vpx_highbd_h_predictor_16x16_sse2(uint16_t *dst, ptrdiff_t stride,
                                       const uint16_t *above,
                                       const uint16_t *left, int bd) {
  int i;
  (void)above;
  (void)bd;

  for (i = 0; i < 2; i++, left += 8) {
    const __m128i left_u16 = _mm_load_si128((const __m128i *)left);
    const __m128i row0 = _mm_shufflelo_epi16(left_u16, 0x0);
    const __m128i row1 = _mm_shufflelo_epi16(left_u16, 0x55);
    const __m128i row2 = _mm_shufflelo_epi16(left_u16, 0xaa);
    const __m128i row3 = _mm_shufflelo_epi16(left_u16, 0xff);
    const __m128i row4 = _mm_shufflehi_epi16(left_u16, 0x0);
    const __m128i row5 = _mm_shufflehi_epi16(left_u16, 0x55);
    const __m128i row6 = _mm_shufflehi_epi16(left_u16, 0xaa);
    const __m128i row7 = _mm_shufflehi_epi16(left_u16, 0xff);
    h_store_16_unpacklo(&dst, stride, &row0);
    h_store_16_unpacklo(&dst, stride, &row1);
    h_store_16_unpacklo(&dst, stride, &row2);
    h_store_16_unpacklo(&dst, stride, &row3);
    h_store_16_unpackhi(&dst, stride, &row4);
    h_store_16_unpackhi(&dst, stride, &row5);
    h_store_16_unpackhi(&dst, stride, &row6);
    h_store_16_unpackhi(&dst, stride, &row7);
  }
}

static INLINE void h_store_32_unpacklo(uint16_t **dst, const ptrdiff_t stride,
                                       const __m128i *row) {
  const __m128i val = _mm_unpacklo_epi64(*row, *row);
  _mm_store_si128((__m128i *)(*dst), val);
  _mm_store_si128((__m128i *)(*dst + 8), val);
  _mm_store_si128((__m128i *)(*dst + 16), val);
  _mm_store_si128((__m128i *)(*dst + 24), val);
  *dst += stride;
}

static INLINE void h_store_32_unpackhi(uint16_t **dst, const ptrdiff_t stride,
                                       const __m128i *row) {
  const __m128i val = _mm_unpackhi_epi64(*row, *row);
  _mm_store_si128((__m128i *)(*dst), val);
  _mm_store_si128((__m128i *)(*dst + 8), val);
  _mm_store_si128((__m128i *)(*dst + 16), val);
  _mm_store_si128((__m128i *)(*dst + 24), val);
  *dst += stride;
}

void vpx_highbd_h_predictor_32x32_sse2(uint16_t *dst, ptrdiff_t stride,
                                       const uint16_t *above,
                                       const uint16_t *left, int bd) {
  int i;
  (void)above;
  (void)bd;

  for (i = 0; i < 4; i++, left += 8) {
    const __m128i left_u16 = _mm_load_si128((const __m128i *)left);
    const __m128i row0 = _mm_shufflelo_epi16(left_u16, 0x0);
    const __m128i row1 = _mm_shufflelo_epi16(left_u16, 0x55);
    const __m128i row2 = _mm_shufflelo_epi16(left_u16, 0xaa);
    const __m128i row3 = _mm_shufflelo_epi16(left_u16, 0xff);
    const __m128i row4 = _mm_shufflehi_epi16(left_u16, 0x0);
    const __m128i row5 = _mm_shufflehi_epi16(left_u16, 0x55);
    const __m128i row6 = _mm_shufflehi_epi16(left_u16, 0xaa);
    const __m128i row7 = _mm_shufflehi_epi16(left_u16, 0xff);
    h_store_32_unpacklo(&dst, stride, &row0);
    h_store_32_unpacklo(&dst, stride, &row1);
    h_store_32_unpacklo(&dst, stride, &row2);
    h_store_32_unpacklo(&dst, stride, &row3);
    h_store_32_unpackhi(&dst, stride, &row4);
    h_store_32_unpackhi(&dst, stride, &row5);
    h_store_32_unpackhi(&dst, stride, &row6);
    h_store_32_unpackhi(&dst, stride, &row7);
  }
}

//------------------------------------------------------------------------------
// DC 4x4

static INLINE __m128i dc_sum_4(const uint16_t *ref) {
  const __m128i _dcba = _mm_loadl_epi64((const __m128i *)ref);
  const __m128i _xxdc = _mm_shufflelo_epi16(_dcba, 0xe);
  const __m128i a = _mm_add_epi16(_dcba, _xxdc);
  return _mm_add_epi16(a, _mm_shufflelo_epi16(a, 0x1));
}

static INLINE void dc_store_4x4(uint16_t *dst, ptrdiff_t stride,
                                const __m128i *dc) {
  const __m128i dc_dup = _mm_shufflelo_epi16(*dc, 0x0);
  int i;
  for (i = 0; i < 4; ++i, dst += stride) {
    _mm_storel_epi64((__m128i *)dst, dc_dup);
  }
}

void vpx_highbd_dc_left_predictor_4x4_sse2(uint16_t *dst, ptrdiff_t stride,
                                           const uint16_t *above,
                                           const uint16_t *left, int bd) {
  const __m128i two = _mm_cvtsi32_si128(2);
  const __m128i sum = dc_sum_4(left);
  const __m128i dc = _mm_srli_epi16(_mm_add_epi16(sum, two), 2);
  (void)above;
  (void)bd;
  dc_store_4x4(dst, stride, &dc);
}

void vpx_highbd_dc_top_predictor_4x4_sse2(uint16_t *dst, ptrdiff_t stride,
                                          const uint16_t *above,
                                          const uint16_t *left, int bd) {
  const __m128i two = _mm_cvtsi32_si128(2);
  const __m128i sum = dc_sum_4(above);
  const __m128i dc = _mm_srli_epi16(_mm_add_epi16(sum, two), 2);
  (void)left;
  (void)bd;
  dc_store_4x4(dst, stride, &dc);
}

//------------------------------------------------------------------------------
// DC 8x8

static INLINE __m128i dc_sum_8(const uint16_t *ref) {
  const __m128i ref_u16 = _mm_load_si128((const __m128i *)ref);
  const __m128i _dcba = _mm_add_epi16(ref_u16, _mm_srli_si128(ref_u16, 8));
  const __m128i _xxdc = _mm_shufflelo_epi16(_dcba, 0xe);
  const __m128i a = _mm_add_epi16(_dcba, _xxdc);

  return _mm_add_epi16(a, _mm_shufflelo_epi16(a, 0x1));
}

static INLINE void dc_store_8x8(uint16_t *dst, ptrdiff_t stride,
                                const __m128i *dc) {
  const __m128i dc_dup_lo = _mm_shufflelo_epi16(*dc, 0);
  const __m128i dc_dup = _mm_unpacklo_epi64(dc_dup_lo, dc_dup_lo);
  int i;
  for (i = 0; i < 8; ++i, dst += stride) {
    _mm_store_si128((__m128i *)dst, dc_dup);
  }
}

void vpx_highbd_dc_left_predictor_8x8_sse2(uint16_t *dst, ptrdiff_t stride,
                                           const uint16_t *above,
                                           const uint16_t *left, int bd) {
  const __m128i four = _mm_cvtsi32_si128(4);
  const __m128i sum = dc_sum_8(left);
  const __m128i dc = _mm_srli_epi16(_mm_add_epi16(sum, four), 3);
  (void)above;
  (void)bd;
  dc_store_8x8(dst, stride, &dc);
}

void vpx_highbd_dc_top_predictor_8x8_sse2(uint16_t *dst, ptrdiff_t stride,
                                          const uint16_t *above,
                                          const uint16_t *left, int bd) {
  const __m128i four = _mm_cvtsi32_si128(4);
  const __m128i sum = dc_sum_8(above);
  const __m128i dc = _mm_srli_epi16(_mm_add_epi16(sum, four), 3);
  (void)left;
  (void)bd;
  dc_store_8x8(dst, stride, &dc);
}

//------------------------------------------------------------------------------
// DC 16x16

static INLINE __m128i dc_sum_16(const uint16_t *ref) {
  const __m128i sum_lo = dc_sum_8(ref);
  const __m128i sum_hi = dc_sum_8(ref + 8);
  return _mm_add_epi16(sum_lo, sum_hi);
}

static INLINE void dc_store_16x16(uint16_t *dst, ptrdiff_t stride,
                                  const __m128i *dc) {
  const __m128i dc_dup_lo = _mm_shufflelo_epi16(*dc, 0);
  const __m128i dc_dup = _mm_unpacklo_epi64(dc_dup_lo, dc_dup_lo);
  int i;
  for (i = 0; i < 16; ++i, dst += stride) {
    _mm_store_si128((__m128i *)dst, dc_dup);
    _mm_store_si128((__m128i *)(dst + 8), dc_dup);
  }
}

void vpx_highbd_dc_left_predictor_16x16_sse2(uint16_t *dst, ptrdiff_t stride,
                                             const uint16_t *above,
                                             const uint16_t *left, int bd) {
  const __m128i eight = _mm_cvtsi32_si128(8);
  const __m128i sum = dc_sum_16(left);
  const __m128i dc = _mm_srli_epi16(_mm_add_epi16(sum, eight), 4);
  (void)above;
  (void)bd;
  dc_store_16x16(dst, stride, &dc);
}

void vpx_highbd_dc_top_predictor_16x16_sse2(uint16_t *dst, ptrdiff_t stride,
                                            const uint16_t *above,
                                            const uint16_t *left, int bd) {
  const __m128i eight = _mm_cvtsi32_si128(8);
  const __m128i sum = dc_sum_16(above);
  const __m128i dc = _mm_srli_epi16(_mm_add_epi16(sum, eight), 4);
  (void)left;
  (void)bd;
  dc_store_16x16(dst, stride, &dc);
}

//------------------------------------------------------------------------------
// DC 32x32

static INLINE __m128i dc_sum_32(const uint16_t *ref) {
  const __m128i zero = _mm_setzero_si128();
  const __m128i sum_a = dc_sum_16(ref);
  const __m128i sum_b = dc_sum_16(ref + 16);
  // 12 bit bd will outrange, so expand to 32 bit before adding final total
  return _mm_add_epi32(_mm_unpacklo_epi16(sum_a, zero),
                       _mm_unpacklo_epi16(sum_b, zero));
}

static INLINE void dc_store_32x32(uint16_t *dst, ptrdiff_t stride,
                                  const __m128i *dc) {
  const __m128i dc_dup_lo = _mm_shufflelo_epi16(*dc, 0);
  const __m128i dc_dup = _mm_unpacklo_epi64(dc_dup_lo, dc_dup_lo);
  int i;
  for (i = 0; i < 32; ++i, dst += stride) {
    _mm_store_si128((__m128i *)dst, dc_dup);
    _mm_store_si128((__m128i *)(dst + 8), dc_dup);
    _mm_store_si128((__m128i *)(dst + 16), dc_dup);
    _mm_store_si128((__m128i *)(dst + 24), dc_dup);
  }
}

void vpx_highbd_dc_left_predictor_32x32_sse2(uint16_t *dst, ptrdiff_t stride,
                                             const uint16_t *above,
                                             const uint16_t *left, int bd) {
  const __m128i sixteen = _mm_cvtsi32_si128(16);
  const __m128i sum = dc_sum_32(left);
  const __m128i dc = _mm_srli_epi32(_mm_add_epi32(sum, sixteen), 5);
  (void)above;
  (void)bd;
  dc_store_32x32(dst, stride, &dc);
}

void vpx_highbd_dc_top_predictor_32x32_sse2(uint16_t *dst, ptrdiff_t stride,
                                            const uint16_t *above,
                                            const uint16_t *left, int bd) {
  const __m128i sixteen = _mm_cvtsi32_si128(16);
  const __m128i sum = dc_sum_32(above);
  const __m128i dc = _mm_srli_epi32(_mm_add_epi32(sum, sixteen), 5);
  (void)left;
  (void)bd;
  dc_store_32x32(dst, stride, &dc);
}
