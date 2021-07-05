/*
 *  Copyright (c) 2012 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

// System independant wrapper for polling elapsed time in ms and us.
// The implementation works in the tick domain which can be mapped over to the
// time domain.
#ifndef WEBRTC_SYSTEM_WRAPPERS_INCLUDE_TICK_UTIL_H_
#define WEBRTC_SYSTEM_WRAPPERS_INCLUDE_TICK_UTIL_H_

#include <CoreLib/VxDefs.h>

#if _WIN32
// Note: The Windows header must always be included before mmsystem.h
#include <WinSock2.h>
#include <windows.h>
#include <mmsystem.h>
#elif TARGET_OS_LINUX
#include <time.h>
#elif TARGET_OS_APPLE
#include <mach/mach_time.h>
#include <string.h>
#else
#include <sys/time.h>
#include <time.h>
#endif

#include "WebRtcTypeDefs.h"

namespace webrtc {

class TickInterval;

// Class representing the current time.
class TickTime {
 public:
  TickTime();
  explicit TickTime(int64_t ticks);

  // Current time in the tick domain.
  static TickTime Now();

  // Now in the time domain in ms.
  static int64_t MillisecondTimestamp();

  // Now in the time domain in us.
  static int64_t MicrosecondTimestamp();

  // Returns the number of ticks in the tick domain.
  int64_t Ticks() const;

  static int64_t MillisecondsToTicks(const int64_t ms);

  static int64_t TicksToMilliseconds(const int64_t ticks);

  static int64_t TicksToMicroseconds(const int64_t ticks);

  // Returns a TickTime that is ticks later than the passed TickTime.
  friend TickTime operator+(const TickTime lhs, const int64_t ticks);
  TickTime& operator+=(const int64_t& ticks);

  // Returns a TickInterval that is the difference in ticks beween rhs and lhs.
  friend TickInterval operator-(const TickTime& lhs, const TickTime& rhs);

 private:
  static int64_t QueryOsForTicks();

  int64_t ticks_;
};

// Represents a time delta in ticks.
class TickInterval {
 public:
  TickInterval();
  explicit TickInterval(int64_t interval);

  int64_t Milliseconds() const;
  int64_t Microseconds() const;

  // Returns the sum of two TickIntervals as a TickInterval.
  friend TickInterval operator+(const TickInterval& lhs,
                                const TickInterval& rhs);
  TickInterval& operator+=(const TickInterval& rhs);

  // Returns a TickInterval corresponding to rhs - lhs.
  friend TickInterval operator-(const TickInterval& lhs,
                                const TickInterval& rhs);
  TickInterval& operator-=(const TickInterval& rhs);

  friend bool operator>(const TickInterval& lhs, const TickInterval& rhs);
  friend bool operator<=(const TickInterval& lhs, const TickInterval& rhs);
  friend bool operator<(const TickInterval& lhs, const TickInterval& rhs);
  friend bool operator>=(const TickInterval& lhs, const TickInterval& rhs);

 private:
  friend class TickTime;
  friend TickInterval operator-(const TickTime& lhs, const TickTime& rhs);

 private:
  int64_t interval_;
};

GOTV_INLINE int64_t TickInterval::Milliseconds() const {
  return TickTime::TicksToMilliseconds(interval_);
}

GOTV_INLINE int64_t TickInterval::Microseconds() const {
  return TickTime::TicksToMicroseconds(interval_);
}

GOTV_INLINE TickInterval operator+(const TickInterval& lhs,
                              const TickInterval& rhs) {
  return TickInterval(lhs.interval_ + rhs.interval_);
}

GOTV_INLINE TickInterval operator-(const TickInterval& lhs,
                              const TickInterval& rhs) {
  return TickInterval(lhs.interval_ - rhs.interval_);
}

GOTV_INLINE TickInterval operator-(const TickTime& lhs, const TickTime& rhs) {
  return TickInterval(lhs.ticks_ - rhs.ticks_);
}

GOTV_INLINE TickTime operator+(const TickTime lhs, const int64_t ticks) {
  TickTime time = lhs;
  time.ticks_ += ticks;
  return time;
}

GOTV_INLINE bool operator>(const TickInterval& lhs, const TickInterval& rhs) {
  return lhs.interval_ > rhs.interval_;
}

GOTV_INLINE bool operator<=(const TickInterval& lhs, const TickInterval& rhs) {
  return lhs.interval_ <= rhs.interval_;
}

GOTV_INLINE bool operator<(const TickInterval& lhs, const TickInterval& rhs) {
  return lhs.interval_ <= rhs.interval_;
}

GOTV_INLINE bool operator>=(const TickInterval& lhs, const TickInterval& rhs) {
  return lhs.interval_ >= rhs.interval_;
}

GOTV_INLINE TickTime::TickTime()
    : ticks_(0) {
}

GOTV_INLINE TickTime::TickTime(int64_t ticks)
    : ticks_(ticks) {
}

GOTV_INLINE TickTime TickTime::Now() {
  return TickTime(QueryOsForTicks());
}

GOTV_INLINE int64_t TickTime::Ticks() const {
  return ticks_;
}

GOTV_INLINE TickTime& TickTime::operator+=(const int64_t& ticks) {
  ticks_ += ticks;
  return *this;
}

GOTV_INLINE TickInterval::TickInterval() : interval_(0) {
}

GOTV_INLINE TickInterval::TickInterval(const int64_t interval)
  : interval_(interval) {
}

GOTV_INLINE TickInterval& TickInterval::operator+=(const TickInterval& rhs) {
  interval_ += rhs.interval_;
  return *this;
}

GOTV_INLINE TickInterval& TickInterval::operator-=(const TickInterval& rhs) {
  interval_ -= rhs.interval_;
  return *this;
}

}  // namespace webrtc

#endif  // WEBRTC_SYSTEM_WRAPPERS_INCLUDE_TICK_UTIL_H_
