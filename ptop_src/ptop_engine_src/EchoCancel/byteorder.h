/*
 *  Copyright 2004 The WebRTC Project Authors. All rights reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef WEBRTC_BASE_BYTEORDER_H_
#define WEBRTC_BASE_BYTEORDER_H_


#include "basictypes.h"

#if defined(TARGET_POSIX) && !defined(__native_client__)
#include <arpa/inet.h>
#endif

#if defined(TARGET_OS_WINDOWS)
#include <stdlib.h>
#endif

namespace rtc {

// Reading and writing of little and big-endian numbers from memory
// TODO: Optimized versions, with direct read/writes of
// integers in host-endian format, when the platform supports it.

GOTV_INLINE void Set8(void* memory, size_t offset, uint8_t v) {
  static_cast<uint8_t*>(memory)[offset] = v;
}

GOTV_INLINE uint8_t Get8(const void* memory, size_t offset) {
  return static_cast<const uint8_t*>(memory)[offset];
}

GOTV_INLINE void SetBE16(void* memory, uint16_t v) {
  Set8(memory, 0, static_cast<uint8_t>(v >> 8));
  Set8(memory, 1, static_cast<uint8_t>(v >> 0));
}

GOTV_INLINE void SetBE32(void* memory, uint32_t v) {
  Set8(memory, 0, static_cast<uint8_t>(v >> 24));
  Set8(memory, 1, static_cast<uint8_t>(v >> 16));
  Set8(memory, 2, static_cast<uint8_t>(v >> 8));
  Set8(memory, 3, static_cast<uint8_t>(v >> 0));
}

GOTV_INLINE void SetBE64(void* memory, uint64_t v) {
  Set8(memory, 0, static_cast<uint8_t>(v >> 56));
  Set8(memory, 1, static_cast<uint8_t>(v >> 48));
  Set8(memory, 2, static_cast<uint8_t>(v >> 40));
  Set8(memory, 3, static_cast<uint8_t>(v >> 32));
  Set8(memory, 4, static_cast<uint8_t>(v >> 24));
  Set8(memory, 5, static_cast<uint8_t>(v >> 16));
  Set8(memory, 6, static_cast<uint8_t>(v >> 8));
  Set8(memory, 7, static_cast<uint8_t>(v >> 0));
}

GOTV_INLINE uint16_t GetBE16(const void* memory) {
  return static_cast<uint16_t>((Get8(memory, 0) << 8) | (Get8(memory, 1) << 0));
}

GOTV_INLINE uint32_t GetBE32(const void* memory) {
  return (static_cast<uint32_t>(Get8(memory, 0)) << 24) |
         (static_cast<uint32_t>(Get8(memory, 1)) << 16) |
         (static_cast<uint32_t>(Get8(memory, 2)) << 8) |
         (static_cast<uint32_t>(Get8(memory, 3)) << 0);
}

GOTV_INLINE uint64_t GetBE64(const void* memory) {
  return (static_cast<uint64_t>(Get8(memory, 0)) << 56) |
         (static_cast<uint64_t>(Get8(memory, 1)) << 48) |
         (static_cast<uint64_t>(Get8(memory, 2)) << 40) |
         (static_cast<uint64_t>(Get8(memory, 3)) << 32) |
         (static_cast<uint64_t>(Get8(memory, 4)) << 24) |
         (static_cast<uint64_t>(Get8(memory, 5)) << 16) |
         (static_cast<uint64_t>(Get8(memory, 6)) << 8) |
         (static_cast<uint64_t>(Get8(memory, 7)) << 0);
}

GOTV_INLINE void SetLE16(void* memory, uint16_t v) {
  Set8(memory, 0, static_cast<uint8_t>(v >> 0));
  Set8(memory, 1, static_cast<uint8_t>(v >> 8));
}

GOTV_INLINE void SetLE32(void* memory, uint32_t v) {
  Set8(memory, 0, static_cast<uint8_t>(v >> 0));
  Set8(memory, 1, static_cast<uint8_t>(v >> 8));
  Set8(memory, 2, static_cast<uint8_t>(v >> 16));
  Set8(memory, 3, static_cast<uint8_t>(v >> 24));
}

GOTV_INLINE void SetLE64(void* memory, uint64_t v) {
  Set8(memory, 0, static_cast<uint8_t>(v >> 0));
  Set8(memory, 1, static_cast<uint8_t>(v >> 8));
  Set8(memory, 2, static_cast<uint8_t>(v >> 16));
  Set8(memory, 3, static_cast<uint8_t>(v >> 24));
  Set8(memory, 4, static_cast<uint8_t>(v >> 32));
  Set8(memory, 5, static_cast<uint8_t>(v >> 40));
  Set8(memory, 6, static_cast<uint8_t>(v >> 48));
  Set8(memory, 7, static_cast<uint8_t>(v >> 56));
}

GOTV_INLINE uint16_t GetLE16(const void* memory) {
  return static_cast<uint16_t>((Get8(memory, 0) << 0) | (Get8(memory, 1) << 8));
}

GOTV_INLINE uint32_t GetLE32(const void* memory) {
  return (static_cast<uint32_t>(Get8(memory, 0)) << 0) |
         (static_cast<uint32_t>(Get8(memory, 1)) << 8) |
         (static_cast<uint32_t>(Get8(memory, 2)) << 16) |
         (static_cast<uint32_t>(Get8(memory, 3)) << 24);
}

GOTV_INLINE uint64_t GetLE64(const void* memory) {
  return (static_cast<uint64_t>(Get8(memory, 0)) << 0) |
         (static_cast<uint64_t>(Get8(memory, 1)) << 8) |
         (static_cast<uint64_t>(Get8(memory, 2)) << 16) |
         (static_cast<uint64_t>(Get8(memory, 3)) << 24) |
         (static_cast<uint64_t>(Get8(memory, 4)) << 32) |
         (static_cast<uint64_t>(Get8(memory, 5)) << 40) |
         (static_cast<uint64_t>(Get8(memory, 6)) << 48) |
         (static_cast<uint64_t>(Get8(memory, 7)) << 56);
}

// Check if the current host is big endian.
GOTV_INLINE bool IsHostBigEndian() {
  static const int number = 1;
  return 0 == *reinterpret_cast<const char*>(&number);
}

GOTV_INLINE uint16_t HostToNetwork16(uint16_t n) {
  uint16_t result;
  SetBE16(&result, n);
  return result;
}

GOTV_INLINE uint32_t HostToNetwork32(uint32_t n) {
  uint32_t result;
  SetBE32(&result, n);
  return result;
}

GOTV_INLINE uint64_t HostToNetwork64(uint64_t n) {
  uint64_t result;
  SetBE64(&result, n);
  return result;
}

GOTV_INLINE uint16_t NetworkToHost16(uint16_t n) {
  return GetBE16(&n);
}

GOTV_INLINE uint32_t NetworkToHost32(uint32_t n) {
  return GetBE32(&n);
}

GOTV_INLINE uint64_t NetworkToHost64(uint64_t n) {
  return GetBE64(&n);
}

}  // namespace rtc

#endif  // WEBRTC_BASE_BYTEORDER_H_
