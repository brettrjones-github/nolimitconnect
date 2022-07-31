/*
*  Copyright (c) 2011 The WebRTC project authors. All Rights Reserved.
*
*  Use of this source code is governed by a BSD-style license
*  that can be found in the LICENSE file in the root of the source
*  tree. An additional intellectual property rights grant can be found
*  in the file PATENTS.  All contributing project authors may
*  be found in the AUTHORS file in the root of the source tree.
*/

#include "event_wrapper.h"

#if defined(TARGET_OS_WINDOWS)
#include <WinSock2.h>
#include <windows.h>
#include "event_timer_win.h"
#elif defined(WEBRTC_MAC) && !defined(WEBRTC_IOS)
#include <ApplicationServices/ApplicationServices.h>
#include <libpthread/pthread.h>
#include "webrtc/system_wrappers/source/event_timer_posix.h"
#else
#include <libpthread/pthread.h>
#include "event_timer_posix.h"
#endif

#include "event.h"

namespace webrtc {

	class EventWrapperImpl : public EventWrapper {
	public:
		EventWrapperImpl() : event_(false, false) {}
		~EventWrapperImpl() {}

		bool Set()
		{
			event_.Set();
			return true;
		}

		EventTypeWrapper Wait(unsigned long max_time)
		{
			int to_wait = max_time == WEBRTC_EVENT_INFINITE ?
				rtc::Event::kForever : static_cast<int>(max_time);
			return event_.Wait(to_wait) ? kEventSignaled : kEventTimeout;
		}

	private:
		rtc::Event event_;
	};

	// static
	EventWrapper* EventWrapper::Create() {
		return new EventWrapperImpl();
	}

}  // namespace webrtc
