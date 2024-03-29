// Copyright 2012 Google Inc. All Rights Reserved.
//
// Use of this source code is governed by a BSD-style license
// that can be found in the COPYING file in the root of the source
// tree. An additional intellectual property rights grant can be found
// in the file PATENTS. All contributing project authors may
// be found in the AUTHORS file in the root of the source tree.
// -----------------------------------------------------------------------------
//
// Author: Jyrki Alakuijala (jyrki@google.com)
//

#include <assert.h>
#include <math.h>

#include "./backward_references_enc.h"
#include "./histogram_enc.h"
#include "../dsp/lossless.h"
#include "../dsp/lossless_common.h"
#include "../dsp/dsp.h"
#include "../utils/color_cache_utils.h"
#include "../utils/utils.h"

#define MIN_BLOCK_SIZE 256  // minimum block size for backward references

#define MAX_ENTROPY    (1e30f)

// 1M window (4M bytes) minus 120 special codes for short distances.
#define WINDOW_SIZE_BITS 20
#define WINDOW_SIZE ((1 << WINDOW_SIZE_BITS) - 120)

// Minimum number of pixels for which it is cheaper to encode a
// distance + length instead of each pixel as a literal.
#define MIN_LENGTH 4

// -----------------------------------------------------------------------------

static const uint8_t plane_to_code_lut[128] = {
 96,   73,  55,  39,  23,  13,   5,  1,  255, 255, 255, 255, 255, 255, 255, 255,
 101,  78,  58,  42,  26,  16,   8,  2,    0,   3,  9,   17,  27,  43,  59,  79,
 102,  86,  62,  46,  32,  20,  10,  6,    4,   7,  11,  21,  33,  47,  63,  87,
 105,  90,  70,  52,  37,  28,  18,  14,  12,  15,  19,  29,  38,  53,  71,  91,
 110,  99,  82,  66,  48,  35,  30,  24,  22,  25,  31,  36,  49,  67,  83, 100,
 115, 108,  94,  76,  64,  50,  44,  40,  34,  41,  45,  51,  65,  77,  95, 109,
 118, 113, 103,  92,  80,  68,  60,  56,  54,  57,  61,  69,  81,  93, 104, 114,
 119, 116, 111, 106,  97,  88,  84,  74,  72,  75,  85,  89,  98, 107, 112, 117
};

extern int VP8LDistanceToPlaneCode(int xsize, int dist);
int VP8LDistanceToPlaneCode(int xsize, int dist) {
  const int yoffset = dist / xsize;
  const int xoffset = dist - yoffset * xsize;
  if (xoffset <= 8 && yoffset < 8) {
    return plane_to_code_lut[yoffset * 16 + 8 - xoffset] + 1;
  } else if (xoffset > xsize - 8 && yoffset < 7) {
    return plane_to_code_lut[(yoffset + 1) * 16 + 8 + (xsize - xoffset)] + 1;
  }
  return dist + 120;
}

// Returns the exact index where array1 and array2 are different. For an index
// inferior or equal to best_len_match, the return value just has to be strictly
// inferior to best_len_match. The current behavior is to return 0 if this index
// is best_len_match, and the index itself otherwise.
// If no two elements are the same, it returns max_limit.
static WEBP_INLINE int FindMatchLength(const uint32_t* const array1,
                                       const uint32_t* const array2,
                                       int best_len_match, int max_limit) {
  // Before 'expensive' linear match, check if the two arrays match at the
  // current best length index.
  if (array1[best_len_match] != array2[best_len_match]) return 0;

  return VP8LVectorMismatch(array1, array2, max_limit);
}

// -----------------------------------------------------------------------------
//  VP8LBackwardRefs

struct PixOrCopyBlock {
  PixOrCopyBlock* next_;   // next block (or NULL)
  PixOrCopy* start_;       // data start
  int size_;               // currently used size
};

extern void VP8LClearBackwardRefs(VP8LBackwardRefs* const refs);
void VP8LClearBackwardRefs(VP8LBackwardRefs* const refs) {
  assert(refs != NULL);
  if (refs->tail_ != NULL) {
    *refs->tail_ = refs->free_blocks_;  // recycle all blocks at once
  }
  refs->free_blocks_ = refs->refs_;
  refs->tail_ = &refs->refs_;
  refs->last_block_ = NULL;
  refs->refs_ = NULL;
}

void VP8LBackwardRefsClear(VP8LBackwardRefs* const refs) {
  assert(refs != NULL);
  VP8LClearBackwardRefs(refs);
  while (refs->free_blocks_ != NULL) {
    PixOrCopyBlock* const next = refs->free_blocks_->next_;
    WebPSafeFree(refs->free_blocks_);
    refs->free_blocks_ = next;
  }
}

void VP8LBackwardRefsInit(VP8LBackwardRefs* const refs, int block_size) {
  assert(refs != NULL);
  memset(refs, 0, sizeof(*refs));
  refs->tail_ = &refs->refs_;
  refs->block_size_ =
      (block_size < MIN_BLOCK_SIZE) ? MIN_BLOCK_SIZE : block_size;
}

VP8LRefsCursor VP8LRefsCursorInit(const VP8LBackwardRefs* const refs) {
  VP8LRefsCursor c;
  c.cur_block_ = refs->refs_;
  if (refs->refs_ != NULL) {
    c.cur_pos = c.cur_block_->start_;
    c.last_pos_ = c.cur_pos + c.cur_block_->size_;
  } else {
    c.cur_pos = NULL;
    c.last_pos_ = NULL;
  }
  return c;
}

void VP8LRefsCursorNextBlock(VP8LRefsCursor* const c) {
  PixOrCopyBlock* const b = c->cur_block_->next_;
  c->cur_pos = (b == NULL) ? NULL : b->start_;
  c->last_pos_ = (b == NULL) ? NULL : b->start_ + b->size_;
  c->cur_block_ = b;
}

// Create a new block, either from the free list or allocated
static PixOrCopyBlock* BackwardRefsNewBlock(VP8LBackwardRefs* const refs) {
  PixOrCopyBlock* b = refs->free_blocks_;
  if (b == NULL) {   // allocate new memory chunk
    const size_t total_size =
        sizeof(*b) + refs->block_size_ * sizeof(*b->start_);
    b = (PixOrCopyBlock*)WebPSafeMalloc(1ULL, total_size);
    if (b == NULL) {
      refs->error_ |= 1;
      return NULL;
    }
    b->start_ = (PixOrCopy*)((uint8_t*)b + sizeof(*b));  // not always aligned
  } else {  // recycle from free-list
    refs->free_blocks_ = b->next_;
  }
  *refs->tail_ = b;
  refs->tail_ = &b->next_;
  refs->last_block_ = b;
  b->next_ = NULL;
  b->size_ = 0;
  return b;
}

extern void VP8LBackwardRefsCursorAdd(VP8LBackwardRefs* const refs,
                                      const PixOrCopy v);
WEBP_INLINE void VP8LBackwardRefsCursorAdd(VP8LBackwardRefs* const refs,
                                           const PixOrCopy v) {
  PixOrCopyBlock* b = refs->last_block_;
  if (b == NULL || b->size_ == refs->block_size_) {
    b = BackwardRefsNewBlock(refs);
    if (b == NULL) return;   // refs->error_ is set
  }
  b->start_[b->size_++] = v;
}

// -----------------------------------------------------------------------------
// Hash chains

int VP8LHashChainInit(VP8LHashChain* const p, int size) {
  assert(p->size_ == 0);
  assert(p->offset_length_ == NULL);
  assert(size > 0);
  p->offset_length_ =
      (uint32_t*)WebPSafeMalloc(size, sizeof(*p->offset_length_));
  if (p->offset_length_ == NULL) return 0;
  p->size_ = size;

  return 1;
}

void VP8LHashChainClear(VP8LHashChain* const p) {
  assert(p != NULL);
  WebPSafeFree(p->offset_length_);

  p->size_ = 0;
  p->offset_length_ = NULL;
}

// -----------------------------------------------------------------------------

#define HASH_MULTIPLIER_HI (0xc6a4a793ULL)
#define HASH_MULTIPLIER_LO (0x5bd1e996ULL)

static WEBP_INLINE uint32_t GetPixPairHash64(const uint32_t* const argb) {
  uint32_t key;
  key  = (argb[1] * HASH_MULTIPLIER_HI) & 0xffffffffu;
  key += (argb[0] * HASH_MULTIPLIER_LO) & 0xffffffffu;
  key = key >> (32 - HASH_BITS);
  return key;
}

// Returns the maximum number of hash chain lookups to do for a
// given compression quality. Return value in range [8, 86].
static int GetMaxItersForQuality(int quality) {
  return 8 + (quality * quality) / 128;
}

static int GetWindowSizeForHashChain(int quality, int xsize) {
  const int max_window_size = (quality > 75) ? WINDOW_SIZE
                            : (quality > 50) ? (xsize << 8)
                            : (quality > 25) ? (xsize << 6)
                            : (xsize << 4);
  assert(xsize > 0);
  return (max_window_size > WINDOW_SIZE) ? WINDOW_SIZE : max_window_size;
}

static WEBP_INLINE int MaxFindCopyLength(int len) {
  return (len < MAX_LENGTH) ? len : MAX_LENGTH;
}

int VP8LHashChainFill(VP8LHashChain* const p, int quality,
                      const uint32_t* const argb, int xsize, int ysize,
                      int low_effort) {
  const int size = xsize * ysize;
  const int iter_max = GetMaxItersForQuality(quality);
  const uint32_t window_size = GetWindowSizeForHashChain(quality, xsize);
  int pos;
  int argb_comp;
  uint32_t base_position;
  int32_t* hash_to_first_index;
  // Temporarily use the p->offset_length_ as a hash chain.
  int32_t* chain = (int32_t*)p->offset_length_;
  assert(size > 0);
  assert(p->size_ != 0);
  assert(p->offset_length_ != NULL);

  if (size <= 2) {
    p->offset_length_[0] = p->offset_length_[size - 1] = 0;
    return 1;
  }

  hash_to_first_index =
      (int32_t*)WebPSafeMalloc(HASH_SIZE, sizeof(*hash_to_first_index));
  if (hash_to_first_index == NULL) return 0;

  // Set the int32_t array to -1.
  memset(hash_to_first_index, 0xff, HASH_SIZE * sizeof(*hash_to_first_index));
  // Fill the chain linking pixels with the same hash.
  argb_comp = (argb[0] == argb[1]);
  for (pos = 0; pos < size - 2;) {
    uint32_t hash_code;
    const int argb_comp_next = (argb[pos + 1] == argb[pos + 2]);
    if (argb_comp && argb_comp_next) {
      // Consecutive pixels with the same color will share the same hash.
      // We therefore use a different hash: the color and its repetition
      // length.
      uint32_t tmp[2];
      uint32_t len = 1;
      tmp[0] = argb[pos];
      // Figure out how far the pixels are the same.
      // The last pixel has a different 64 bit hash, as its next pixel does
      // not have the same color, so we just need to get to the last pixel equal
      // to its follower.
      while (pos + (int)len + 2 < size && argb[pos + len + 2] == argb[pos]) {
        ++len;
      }
      if (len > MAX_LENGTH) {
        // Skip the pixels that match for distance=1 and length>MAX_LENGTH
        // because they are linked to their predecessor and we automatically
        // check that in the main for loop below. Skipping means setting no
        // predecessor in the chain, hence -1.
        memset(chain + pos, 0xff, (len - MAX_LENGTH) * sizeof(*chain));
        pos += len - MAX_LENGTH;
        len = MAX_LENGTH;
      }
      // Process the rest of the hash chain.
      while (len) {
        tmp[1] = len--;
        hash_code = GetPixPairHash64(tmp);
        chain[pos] = hash_to_first_index[hash_code];
        hash_to_first_index[hash_code] = pos++;
      }
      argb_comp = 0;
    } else {
      // Just move one pixel forward.
      hash_code = GetPixPairHash64(argb + pos);
      chain[pos] = hash_to_first_index[hash_code];
      hash_to_first_index[hash_code] = pos++;
      argb_comp = argb_comp_next;
    }
  }
  // Process the penultimate pixel.
  chain[pos] = hash_to_first_index[GetPixPairHash64(argb + pos)];

  WebPSafeFree(hash_to_first_index);

  // Find the best match interval at each pixel, defined by an offset to the
  // pixel and a length. The right-most pixel cannot match anything to the right
  // (hence a best length of 0) and the left-most pixel nothing to the left
  // (hence an offset of 0).
  assert(size > 2);
  p->offset_length_[0] = p->offset_length_[size - 1] = 0;
  for (base_position = size - 2; base_position > 0;) {
    const int max_len = MaxFindCopyLength(size - 1 - base_position);
    const uint32_t* const argb_start = argb + base_position;
    int iter = iter_max;
    int best_length = 0;
    uint32_t best_distance = 0;
    uint32_t best_argb;
    const int min_pos =
        (base_position > window_size) ? base_position - window_size : 0;
    const int length_max = (max_len < 256) ? max_len : 256;
    uint32_t max_base_position;

    pos = chain[base_position];
    if (!low_effort) {
      int curr_length;
      // Heuristic: use the comparison with the above line as an initialization.
      if (base_position >= (uint32_t)xsize) {
        curr_length = FindMatchLength(argb_start - xsize, argb_start,
                                      best_length, max_len);
        if (curr_length > best_length) {
          best_length = curr_length;
          best_distance = xsize;
        }
        --iter;
      }
      // Heuristic: compare to the previous pixel.
      curr_length =
          FindMatchLength(argb_start - 1, argb_start, best_length, max_len);
      if (curr_length > best_length) {
        best_length = curr_length;
        best_distance = 1;
      }
      --iter;
      // Skip the for loop if we already have the maximum.
      if (best_length == MAX_LENGTH) pos = min_pos - 1;
    }
    best_argb = argb_start[best_length];

    for (; pos >= min_pos && --iter; pos = chain[pos]) {
      int curr_length;
      assert(base_position > (uint32_t)pos);

      if (argb[pos + best_length] != best_argb) continue;

      curr_length = VP8LVectorMismatch(argb + pos, argb_start, max_len);
      if (best_length < curr_length) {
        best_length = curr_length;
        best_distance = base_position - pos;
        best_argb = argb_start[best_length];
        // Stop if we have reached a good enough length.
        if (best_length >= length_max) break;
      }
    }
    // We have the best match but in case the two intervals continue matching
    // to the left, we have the best matches for the left-extended pixels.
    max_base_position = base_position;
    while (1) {
      assert(best_length <= MAX_LENGTH);
      assert(best_distance <= WINDOW_SIZE);
      p->offset_length_[base_position] =
          (best_distance << MAX_LENGTH_BITS) | (uint32_t)best_length;
      --base_position;
      // Stop if we don't have a match or if we are out of bounds.
      if (best_distance == 0 || base_position == 0) break;
      // Stop if we cannot extend the matching intervals to the left.
      if (base_position < best_distance ||
          argb[base_position - best_distance] != argb[base_position]) {
        break;
      }
      // Stop if we are matching at its limit because there could be a closer
      // matching interval with the same maximum length. Then again, if the
      // matching interval is as close as possible (best_distance == 1), we will
      // never find anything better so let's continue.
      if (best_length == MAX_LENGTH && best_distance != 1 &&
          base_position + MAX_LENGTH < max_base_position) {
        break;
      }
      if (best_length < MAX_LENGTH) {
        ++best_length;
        max_base_position = base_position;
      }
    }
  }
  return 1;
}

static WEBP_INLINE void AddSingleLiteral(uint32_t pixel, int use_color_cache,
                                         VP8LColorCache* const hashers,
                                         VP8LBackwardRefs* const refs) {
  PixOrCopy v;
  if (use_color_cache) {
    const uint32_t key = VP8LColorCacheGetIndex(hashers, pixel);
    if (VP8LColorCacheLookup(hashers, key) == pixel) {
      v = PixOrCopyCreateCacheIdx(key);
    } else {
      v = PixOrCopyCreateLiteral(pixel);
      VP8LColorCacheSet(hashers, key, pixel);
    }
  } else {
    v = PixOrCopyCreateLiteral(pixel);
  }
  VP8LBackwardRefsCursorAdd(refs, v);
}

static int BackwardReferencesRle(int xsize, int ysize,
                                 const uint32_t* const argb,
                                 int cache_bits, VP8LBackwardRefs* const refs) {
  const int pix_count = xsize * ysize;
  int i, k;
  const int use_color_cache = (cache_bits > 0);
  VP8LColorCache hashers;

  if (use_color_cache && !VP8LColorCacheInit(&hashers, cache_bits)) {
    return 0;
  }
  VP8LClearBackwardRefs(refs);
  // Add first pixel as literal.
  AddSingleLiteral(argb[0], use_color_cache, &hashers, refs);
  i = 1;
  while (i < pix_count) {
    const int max_len = MaxFindCopyLength(pix_count - i);
    const int rle_len = FindMatchLength(argb + i, argb + i - 1, 0, max_len);
    const int prev_row_len = (i < xsize) ? 0 :
        FindMatchLength(argb + i, argb + i - xsize, 0, max_len);
    if (rle_len >= prev_row_len && rle_len >= MIN_LENGTH) {
      VP8LBackwardRefsCursorAdd(refs, PixOrCopyCreateCopy(1, rle_len));
      // We don't need to update the color cache here since it is always the
      // same pixel being copied, and that does not change the color cache
      // state.
      i += rle_len;
    } else if (prev_row_len >= MIN_LENGTH) {
      VP8LBackwardRefsCursorAdd(refs, PixOrCopyCreateCopy(xsize, prev_row_len));
      if (use_color_cache) {
        for (k = 0; k < prev_row_len; ++k) {
          VP8LColorCacheInsert(&hashers, argb[i + k]);
        }
      }
      i += prev_row_len;
    } else {
      AddSingleLiteral(argb[i], use_color_cache, &hashers, refs);
      i++;
    }
  }
  if (use_color_cache) VP8LColorCacheClear(&hashers);
  return !refs->error_;
}

static int BackwardReferencesLz77(int xsize, int ysize,
                                  const uint32_t* const argb, int cache_bits,
                                  const VP8LHashChain* const hash_chain,
                                  VP8LBackwardRefs* const refs) {
  int i;
  int i_last_check = -1;
  int ok = 0;
  int cc_init = 0;
  const int use_color_cache = (cache_bits > 0);
  const int pix_count = xsize * ysize;
  VP8LColorCache hashers;

  if (use_color_cache) {
    cc_init = VP8LColorCacheInit(&hashers, cache_bits);
    if (!cc_init) goto Error;
  }
  VP8LClearBackwardRefs(refs);
  for (i = 0; i < pix_count;) {
    // Alternative#1: Code the pixels starting at 'i' using backward reference.
    int offset = 0;
    int len = 0;
    int j;
    VP8LHashChainFindCopy(hash_chain, i, &offset, &len);
    if (len >= MIN_LENGTH) {
      const int len_ini = len;
      int max_reach = 0;
      assert(i + len < pix_count);
      // Only start from what we have not checked already.
      i_last_check = (i > i_last_check) ? i : i_last_check;
      // We know the best match for the current pixel but we try to find the
      // best matches for the current pixel AND the next one combined.
      // The naive method would use the intervals:
      // [i,i+len) + [i+len, length of best match at i+len)
      // while we check if we can use:
      // [i,j) (where j<=i+len) + [j, length of best match at j)
      for (j = i_last_check + 1; j <= i + len_ini; ++j) {
        const int len_j = VP8LHashChainFindLength(hash_chain, j);
        const int reach =
            j + (len_j >= MIN_LENGTH ? len_j : 1);  // 1 for single literal.
        if (reach > max_reach) {
          len = j - i;
          max_reach = reach;
        }
      }
    } else {
      len = 1;
    }
    // Go with literal or backward reference.
    assert(len > 0);
    if (len == 1) {
      AddSingleLiteral(argb[i], use_color_cache, &hashers, refs);
    } else {
      VP8LBackwardRefsCursorAdd(refs, PixOrCopyCreateCopy(offset, len));
      if (use_color_cache) {
        for (j = i; j < i + len; ++j) VP8LColorCacheInsert(&hashers, argb[j]);
      }
    }
    i += len;
  }

  ok = !refs->error_;
 Error:
  if (cc_init) VP8LColorCacheClear(&hashers);
  return ok;
}

// -----------------------------------------------------------------------------

static void BackwardReferences2DLocality(int xsize,
                                         const VP8LBackwardRefs* const refs) {
  VP8LRefsCursor c = VP8LRefsCursorInit(refs);
  while (VP8LRefsCursorOk(&c)) {
    if (PixOrCopyIsCopy(c.cur_pos)) {
      const int dist = c.cur_pos->argb_or_distance;
      const int transformed_dist = VP8LDistanceToPlaneCode(xsize, dist);
      c.cur_pos->argb_or_distance = transformed_dist;
    }
    VP8LRefsCursorNext(&c);
  }
}

// Evaluate optimal cache bits for the local color cache.
// The input *best_cache_bits sets the maximum cache bits to use (passing 0
// implies disabling the local color cache). The local color cache is also
// disabled for the lower (<= 25) quality.
// Returns 0 in case of memory error.
static int CalculateBestCacheSize(const uint32_t* argb, int quality,
                                  const VP8LBackwardRefs* const refs,
                                  int* const best_cache_bits) {
  int i;
  const int cache_bits_max = (quality <= 25) ? 0 : *best_cache_bits;
  double entropy_min = MAX_ENTROPY;
  int cc_init[MAX_COLOR_CACHE_BITS + 1] = { 0 };
  VP8LColorCache hashers[MAX_COLOR_CACHE_BITS + 1];
  VP8LRefsCursor c = VP8LRefsCursorInit(refs);
  VP8LHistogram* histos[MAX_COLOR_CACHE_BITS + 1] = { NULL };
  int ok = 0;

  assert(cache_bits_max >= 0 && cache_bits_max <= MAX_COLOR_CACHE_BITS);

  if (cache_bits_max == 0) {
    *best_cache_bits = 0;
    // Local color cache is disabled.
    return 1;
  }

  // Allocate data.
  for (i = 0; i <= cache_bits_max; ++i) {
    histos[i] = VP8LAllocateHistogram(i);
    if (histos[i] == NULL) goto Error;
    if (i == 0) continue;
    cc_init[i] = VP8LColorCacheInit(&hashers[i], i);
    if (!cc_init[i]) goto Error;
  }

  // Find the cache_bits giving the lowest entropy. The search is done in a
  // brute-force way as the function (entropy w.r.t cache_bits) can be
  // anything in practice.
  while (VP8LRefsCursorOk(&c)) {
    const PixOrCopy* const v = c.cur_pos;
    if (PixOrCopyIsLiteral(v)) {
      const uint32_t pix = *argb++;
      const uint32_t a = (pix >> 24) & 0xff;
      const uint32_t r = (pix >> 16) & 0xff;
      const uint32_t g = (pix >>  8) & 0xff;
      const uint32_t b = (pix >>  0) & 0xff;
      // The keys of the caches can be derived from the longest one.
      int key = VP8LHashPix(pix, 32 - cache_bits_max);
      // Do not use the color cache for cache_bits = 0.
      ++histos[0]->blue_[b];
      ++histos[0]->literal_[g];
      ++histos[0]->red_[r];
      ++histos[0]->alpha_[a];
      // Deal with cache_bits > 0.
      for (i = cache_bits_max; i >= 1; --i, key >>= 1) {
        if (VP8LColorCacheLookup(&hashers[i], key) == pix) {
          ++histos[i]->literal_[NUM_LITERAL_CODES + NUM_LENGTH_CODES + key];
        } else {
          VP8LColorCacheSet(&hashers[i], key, pix);
          ++histos[i]->blue_[b];
          ++histos[i]->literal_[g];
          ++histos[i]->red_[r];
          ++histos[i]->alpha_[a];
        }
      }
    } else {
      // We should compute the contribution of the (distance,length)
      // histograms but those are the same independently from the cache size.
      // As those constant contributions are in the end added to the other
      // histogram contributions, we can safely ignore them.
      int len = PixOrCopyLength(v);
      uint32_t argb_prev = *argb ^ 0xffffffffu;
      // Update the color caches.
      do {
        if (*argb != argb_prev) {
          // Efficiency: insert only if the color changes.
          int key = VP8LHashPix(*argb, 32 - cache_bits_max);
          for (i = cache_bits_max; i >= 1; --i, key >>= 1) {
            hashers[i].colors_[key] = *argb;
          }
          argb_prev = *argb;
        }
        argb++;
      } while (--len != 0);
    }
    VP8LRefsCursorNext(&c);
  }

  for (i = 0; i <= cache_bits_max; ++i) {
    const double entropy = VP8LHistogramEstimateBits(histos[i]);
    if (i == 0 || entropy < entropy_min) {
      entropy_min = entropy;
      *best_cache_bits = i;
    }
  }
  ok = 1;
Error:
  for (i = 0; i <= cache_bits_max; ++i) {
    if (cc_init[i]) VP8LColorCacheClear(&hashers[i]);
    VP8LFreeHistogram(histos[i]);
  }
  return ok;
}

// Update (in-place) backward references for specified cache_bits.
static int BackwardRefsWithLocalCache(const uint32_t* const argb,
                                      int cache_bits,
                                      VP8LBackwardRefs* const refs) {
  int pixel_index = 0;
  VP8LColorCache hashers;
  VP8LRefsCursor c = VP8LRefsCursorInit(refs);
  if (!VP8LColorCacheInit(&hashers, cache_bits)) return 0;

  while (VP8LRefsCursorOk(&c)) {
    PixOrCopy* const v = c.cur_pos;
    if (PixOrCopyIsLiteral(v)) {
      const uint32_t argb_literal = v->argb_or_distance;
      const int ix = VP8LColorCacheContains(&hashers, argb_literal);
      if (ix >= 0) {
        // hashers contains argb_literal
        *v = PixOrCopyCreateCacheIdx(ix);
      } else {
        VP8LColorCacheInsert(&hashers, argb_literal);
      }
      ++pixel_index;
    } else {
      // refs was created without local cache, so it can not have cache indexes.
      int k;
      assert(PixOrCopyIsCopy(v));
      for (k = 0; k < v->len; ++k) {
        VP8LColorCacheInsert(&hashers, argb[pixel_index++]);
      }
    }
    VP8LRefsCursorNext(&c);
  }
  VP8LColorCacheClear(&hashers);
  return 1;
}

static VP8LBackwardRefs* GetBackwardReferencesLowEffort(
    int width, int height, const uint32_t* const argb,
    int* const cache_bits, const VP8LHashChain* const hash_chain,
    VP8LBackwardRefs* const refs_lz77) {
  *cache_bits = 0;
  if (!BackwardReferencesLz77(width, height, argb, 0, hash_chain, refs_lz77)) {
    return NULL;
  }
  BackwardReferences2DLocality(width, refs_lz77);
  return refs_lz77;
}

extern int VP8LBackwardReferencesTraceBackwards(
    int xsize, int ysize, const uint32_t* const argb, int cache_bits,
    const VP8LHashChain* const hash_chain,
    const VP8LBackwardRefs* const refs_src, VP8LBackwardRefs* const refs_dst);
static VP8LBackwardRefs* GetBackwardReferences(
    int width, int height, const uint32_t* const argb, int quality,
    int* const cache_bits, const VP8LHashChain* const hash_chain,
    VP8LBackwardRefs* best, VP8LBackwardRefs* worst) {
  const int cache_bits_initial = *cache_bits;
  double bit_cost_best = -1;
  VP8LHistogram* histo = NULL;
  int i, i_best = 0;

  histo = VP8LAllocateHistogram(MAX_COLOR_CACHE_BITS);
  if (histo == NULL) goto Error;

  // Try out RLE first, then LZ77.
  for (i = 0; i <= 1; ++i) {
    int res;
    double bit_cost;
    int cache_bits_tmp = cache_bits_initial;
    // First, try out backward references with no cache (0 bits).
    if (i == 0) {
      res = BackwardReferencesRle(width, height, argb, 0, worst);
    } else {
      res = BackwardReferencesLz77(width, height, argb, 0, hash_chain, worst);
    }
    if (!res) goto Error;

    // Next, try with a color cache and update the references.
    if (!CalculateBestCacheSize(argb, quality, worst, &cache_bits_tmp)) {
      goto Error;
    }
    if (cache_bits_tmp > 0) {
      if (!BackwardRefsWithLocalCache(argb, cache_bits_tmp, worst)) {
        goto Error;
      }
    }

    // Keep the best backward references.
    VP8LHistogramCreate(histo, worst, cache_bits_tmp);
    bit_cost = VP8LHistogramEstimateBits(histo);
    if (i == 0 || bit_cost < bit_cost_best) {
      VP8LBackwardRefs* const tmp = worst;
      worst = best;
      best = tmp;
      bit_cost_best = bit_cost;
      *cache_bits = cache_bits_tmp;
      i_best = i;
    }
  }

  // Improve on simple LZ77 but only for high quality (TraceBackwards is
  // costly).
  if (i_best == 1 && quality >= 25) {
    if (VP8LBackwardReferencesTraceBackwards(width, height, argb, *cache_bits,
                                             hash_chain, best, worst)) {
      double bit_cost_trace;
      VP8LHistogramCreate(histo, worst, *cache_bits);
      bit_cost_trace = VP8LHistogramEstimateBits(histo);
      if (bit_cost_trace < bit_cost_best) best = worst;
    }
  }

  BackwardReferences2DLocality(width, best);

 Error:
  VP8LFreeHistogram(histo);
  return best;
}

VP8LBackwardRefs* VP8LGetBackwardReferences(
    int width, int height, const uint32_t* const argb, int quality,
    int low_effort, int* const cache_bits,
    const VP8LHashChain* const hash_chain, VP8LBackwardRefs* const refs_tmp1,
    VP8LBackwardRefs* const refs_tmp2) {
  if (low_effort) {
    return GetBackwardReferencesLowEffort(width, height, argb, cache_bits,
                                          hash_chain, refs_tmp1);
  } else {
    return GetBackwardReferences(width, height, argb, quality, cache_bits,
                                 hash_chain, refs_tmp1, refs_tmp2);
  }
}
