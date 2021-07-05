/*
 *  Copyright (c) 2011 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include "audio_device_generic.h"
#include <CoreLib/VxDebug.h>

namespace webrtc {

int32_t AudioDeviceGeneric::SetRecordingSampleRate(
    const uint32_t samplesPerSec) {
  LogMsg( LOG_ERROR, "SetRecordingSampleRate Not supported on this platform" );
  return -1;
}

int32_t AudioDeviceGeneric::SetPlayoutSampleRate(const uint32_t samplesPerSec) {
  LogMsg( LOG_ERROR, "SetPlayoutSampleRate Not supported on this platform" );
  return -1;
}

int32_t AudioDeviceGeneric::SetLoudspeakerStatus(bool enable) {
  LogMsg( LOG_ERROR, "Not supported on this platform" );
  return -1;
}

int32_t AudioDeviceGeneric::GetLoudspeakerStatus(bool& enable) const {
  LogMsg( LOG_ERROR, "Not supported on this platform" );
  return -1;
}

int32_t AudioDeviceGeneric::ResetAudioDevice() {
  LogMsg( LOG_ERROR, "Not supported on this platform" );
  return -1;
}

int32_t AudioDeviceGeneric::SoundDeviceControl(unsigned int par1,
                                               unsigned int par2,
                                               unsigned int par3,
                                               unsigned int par4) {
  LogMsg( LOG_ERROR, "Not supported on this platform" );
  return -1;
}

bool AudioDeviceGeneric::BuiltInAECIsAvailable() const {
  LogMsg( LOG_ERROR, "Not supported on this platform" );
  return false;
}

int32_t AudioDeviceGeneric::EnableBuiltInAEC(bool enable) {
  LogMsg( LOG_ERROR, "Not supported on this platform" );
  return -1;
}

bool AudioDeviceGeneric::BuiltInAECIsEnabled() const {
  LogMsg( LOG_ERROR, "Not supported on this platform" );
  return false;
}

bool AudioDeviceGeneric::BuiltInAGCIsAvailable() const {
  LogMsg( LOG_ERROR, "Not supported on this platform" );
  return false;
}

int32_t AudioDeviceGeneric::EnableBuiltInAGC(bool enable) {
  LogMsg( LOG_ERROR, "Not supported on this platform" );
  return -1;
}

bool AudioDeviceGeneric::BuiltInNSIsAvailable() const {
  LogMsg( LOG_ERROR, "Not supported on this platform" );
  return false;
}

int32_t AudioDeviceGeneric::EnableBuiltInNS(bool enable) {
  LogMsg( LOG_ERROR, "Not supported on this platform" );
  return -1;
}

int AudioDeviceGeneric::GetPlayoutAudioParameters(
    AudioParameters* params) const {
  LogMsg( LOG_ERROR, "Not supported on this platform" );
  return -1;
}
int AudioDeviceGeneric::GetRecordAudioParameters(
    AudioParameters* params) const {
  LogMsg( LOG_ERROR, "Not supported on this platform" );
  return -1;
}

}  // namespace webrtc
