/*
 *  Copyright (c) 2012 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include <config_gotvapps.h>
#ifdef TARGET_OS_WINDOWS

#include <ptop_src/ptop_engine_src/EchoCancel/audio_device_config.h>
#include "AudioDeviceWaveWin.h"

#include <ptop_src/ptop_engine_src/EchoCancel/event_wrapper.h>
#include <ptop_src/ptop_engine_src/EchoCancel/tick_util.h>
#include <CoreLib/VxDebug.h>

#include <WinSock2.h>
#include <windows.h>
#include <objbase.h>    // CoTaskMemAlloc, CoTaskMemFree
#include <strsafe.h>    // StringCchCopy(), StringCchCat(), StringCchPrintf()
#include <assert.h>

// Avoids the need of Windows 7 SDK
#ifndef WAVE_MAPPED_DEFAULT_COMMUNICATION_DEVICE
#define WAVE_MAPPED_DEFAULT_COMMUNICATION_DEVICE   0x0010
#endif

// Supported in Windows Vista and Windows 7.
// http://msdn.microsoft.com/en-us/library/dd370819(v=VS.85).aspx
// Taken from Mmddk.h.
#define DRV_RESERVED                      0x0800
#define DRV_QUERYFUNCTIONINSTANCEID       (DRV_RESERVED + 17)
#define DRV_QUERYFUNCTIONINSTANCEIDSIZE   (DRV_RESERVED + 18)

#define POW2(A) (2 << ((A) - 1))

namespace webrtc {

//============================================================================
AudioDeviceWindowsWave::AudioDeviceWindowsWave(const int32_t id) 
: m_ptrAudioBuffer(NULL)
, m_critSect(*CriticalSectionWrapper::CreateCriticalSection())
, m_timeEvent(*EventTimerWrapper::Create())
, m_recStartEvent(*EventWrapper::Create())
, m_playStartEvent(*EventWrapper::Create())
, m_hGetCaptureVolumeThread(NULL)
, m_hShutdownGetVolumeEvent(NULL)
, m_hSetCaptureVolumeThread(NULL)
, m_hShutdownSetVolumeEvent(NULL)
, m_hSetCaptureVolumeEvent(NULL)
, m_ptrThread( 0 )
, m_critSectCb(*CriticalSectionWrapper::CreateCriticalSection())
, m_id(id)
, m_mixerManager(id)
, m_usingInputDeviceIndex(false)
, m_usingOutputDeviceIndex(false)
, m_inputDevice(AudioDeviceModule::kDefaultDevice)
, m_outputDevice(AudioDeviceModule::kDefaultDevice)
, m_inputDeviceIndex(0)
, m_outputDeviceIndex(0)
, m_inputDeviceIsSpecified(false)
, m_outputDeviceIsSpecified(false)
, m_initialized(false)
, m_recIsInitialized(false)
, m_playIsInitialized(false)
, m_recording(false)
, m_playing(false)
, m_startRec(false)
, m_stopRec(false)
, m_startPlay(false)
, m_stopPlay(false)
, m_AGC(false)
, m_hWaveIn(NULL)
, m_hWaveOut(NULL)
, m_recChannels(N_REC_CHANNELS)
, m_playChannels(N_PLAY_CHANNELS)
, m_recBufCount(0)
, m_recPutBackDelay(0)
, m_recDelayCount(0)
, m_playBufCount(0)
, m_prevPlayTime(0)
, m_prevRecTime(0)
, m_prevTimerCheckTime(0)
, m_timesdwBytes(0)
, m_timerFaults(0)
, m_timerRestartAttempts(0)
, m_no_ofm_msecleft_warnings(0)
, m_MAXm_minBuffer(65)
, m_useHeader(0)
, m_dTcheckPlayBufDelay(10)
, m_playBufDelay(80)
, m_playBufDelayFixed(80)
, m_minPlayBufDelay(20)
, m_avgCPULoad(0)
, m_sndCardPlayDelay(0)
, m_sndCardRecDelay(0)
, m_plSampOld(0)
, m_rcSampOld(0)
, m_playBufType(AudioDeviceModule::kAdaptiveBufferSize)
, m_recordedBytes(0)
, m_playWarning(0)
, m_playError(0)
, m_recWarning(0)
, m_recError(0)
, m_newMicLevel(0)
, m_minMicVolume(0)
, m_maxMicVolume(0)
{
    //WEBRTC_TRACE(kTraceMemory, kTraceAudioDevice, id, "%s created", __FUNCTION__);

    // Initialize value, set to 0 if it fails
    if (!QueryPerformanceFrequency(&m_perfFreq))
    {
		m_perfFreq.QuadPart = 0;
    }

    m_hShutdownGetVolumeEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    m_hShutdownSetVolumeEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    m_hSetCaptureVolumeEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
}

//============================================================================
AudioDeviceWindowsWave::~AudioDeviceWindowsWave()
{
    //WEBRTC_TRACE(kTraceMemory,  "%s destroyed", __FUNCTION__);

    Terminate();

    delete &m_recStartEvent;
    delete &m_playStartEvent;
    delete &m_timeEvent;
    delete &m_critSect;
    delete &m_critSectCb;

    if (NULL != m_hShutdownGetVolumeEvent)
    {
        CloseHandle(m_hShutdownGetVolumeEvent);
        m_hShutdownGetVolumeEvent = NULL;
    }

    if (NULL != m_hShutdownSetVolumeEvent)
    {
        CloseHandle(m_hShutdownSetVolumeEvent);
        m_hShutdownSetVolumeEvent = NULL;
    }

    if (NULL != m_hSetCaptureVolumeEvent)
    {
        CloseHandle(m_hSetCaptureVolumeEvent);
        m_hSetCaptureVolumeEvent = NULL;
    }
}

//================================================================================
//                                     API
//================================================================================

//============================================================================
void AudioDeviceWindowsWave::AttachAudioBuffer(AudioDeviceBuffer* audioBuffer)
{

    CriticalSectionScoped lock(&m_critSect);

    m_ptrAudioBuffer = audioBuffer;

    // inform the AudioBuffer about default settings for this implementation
    m_ptrAudioBuffer->SetRecordingSampleRate(N_REC_SAMPLES_PER_SEC);
    m_ptrAudioBuffer->SetPlayoutSampleRate(N_PLAY_SAMPLES_PER_SEC);
    m_ptrAudioBuffer->SetRecordingChannels(N_REC_CHANNELS);
    m_ptrAudioBuffer->SetPlayoutChannels(N_PLAY_CHANNELS);
}

//============================================================================
int32_t AudioDeviceWindowsWave::ActiveAudioLayer(AudioDeviceModule::AudioLayer& audioLayer) const
{
    audioLayer = AudioDeviceModule::kWindowsWaveAudio;
    return 0;
}

//============================================================================
int32_t AudioDeviceWindowsWave::Init()
{
    CriticalSectionScoped lock(&m_critSect);

    if( m_initialized )
    {
        return 0;
    }

    const uint64_t nowTime( TickTime::MillisecondTimestamp() );

    m_recordedBytes = 0;
    m_prevRecByteCheckTime = nowTime;
    m_prevRecTime = nowTime;
    m_prevPlayTime = nowTime;
    m_prevTimerCheckTime = nowTime;

    m_playWarning = 0;
    m_playError = 0;
    m_recWarning = 0;
    m_recError = 0;

    m_mixerManager.EnumerateAll();

    if (m_ptrThread)
    {
        // thread is already created and active
        return 0;
    }

    const char* threadName = "webrtc_audio_module_thread";
    m_ptrThread = new rtc::PlatformThread(ThreadFunc, this, threadName);
    m_ptrThread->Start();
    m_ptrThread->SetPriority(rtc::kRealtimePriority);

    const bool periodic(true);
    if (!m_timeEvent.StartTimer(periodic, TIMER_PERIOD_MS))
    {
        LogMsg( LOG_ERROR, 
                     "failed to start the timer event");
        m_ptrThread->Stop();
        return -1;
    }

    LogMsg( LOG_INFO, 
                 "periodic timer (dT=%d) is now active", TIMER_PERIOD_MS);

    m_hGetCaptureVolumeThread =
        CreateThread(NULL, 0, GetCaptureVolumeThread, this, 0, NULL);
    if (m_hGetCaptureVolumeThread == NULL)
    {
        LogMsg( LOG_ERROR, 
            "  failed to create the volume getter thread");
        return -1;
    }

    SetThreadPriority(m_hGetCaptureVolumeThread, THREAD_PRIORITY_NORMAL);

    m_hSetCaptureVolumeThread =
        CreateThread(NULL, 0, SetCaptureVolumeThread, this, 0, NULL);
    if (m_hSetCaptureVolumeThread == NULL)
    {
        LogMsg( LOG_ERROR, 
            "  failed to create the volume setter thread");
        return -1;
    }

    SetThreadPriority(m_hSetCaptureVolumeThread, THREAD_PRIORITY_NORMAL);

    m_initialized = true;

    return 0;
}

// ----------------------------------------------------------------------------
//  Terminate
// ----------------------------------------------------------------------------
//============================================================================
int32_t AudioDeviceWindowsWave::Terminate()
{

    if (!m_initialized)
    {
        return 0;
    }

    m_critSect.Enter();

    m_mixerManager.Close();

    if (m_ptrThread)
    {
        rtc::PlatformThread* tmpThread = m_ptrThread;
        m_critSect.Leave();

        m_timeEvent.Set();

        tmpThread->Stop();
        delete tmpThread;
		m_ptrThread = 0;
    }
    else
    {
        m_critSect.Leave();
    }

    m_critSect.Enter();
    SetEvent(m_hShutdownGetVolumeEvent);
    m_critSect.Leave();
    int32_t ret = WaitForSingleObject(m_hGetCaptureVolumeThread, 2000);
    if (ret != WAIT_OBJECT_0)
    {
        // the thread did not stop as it should
        LogMsg( LOG_ERROR, 
            "  failed to close down volume getter thread");
        CloseHandle(m_hGetCaptureVolumeThread);
        m_hGetCaptureVolumeThread = NULL;
        return -1;
    }
    m_critSect.Enter();
    LogMsg( LOG_INFO, 
        "  volume getter thread is now closed");

    SetEvent(m_hShutdownSetVolumeEvent);
    m_critSect.Leave();
    ret = WaitForSingleObject(m_hSetCaptureVolumeThread, 2000);
    if (ret != WAIT_OBJECT_0)
    {
        // the thread did not stop as it should
        LogMsg( LOG_ERROR, 
            "  failed to close down volume setter thread");
        CloseHandle(m_hSetCaptureVolumeThread);
        m_hSetCaptureVolumeThread = NULL;
        return -1;
    }
    m_critSect.Enter();
    LogMsg( LOG_INFO, 
        "  volume setter thread is now closed");

    CloseHandle(m_hGetCaptureVolumeThread);
    m_hGetCaptureVolumeThread = NULL;

    CloseHandle(m_hSetCaptureVolumeThread);
    m_hSetCaptureVolumeThread = NULL;

    m_critSect.Leave();

    m_timeEvent.StopTimer();

    m_initialized = false;
    m_outputDeviceIsSpecified = false;
    m_inputDeviceIsSpecified = false;

    return 0;
}

//============================================================================
DWORD WINAPI AudioDeviceWindowsWave::GetCaptureVolumeThread(LPVOID context)
{
    return(((AudioDeviceWindowsWave*)context)->DoGetCaptureVolumeThread());
}

//============================================================================
DWORD WINAPI AudioDeviceWindowsWave::SetCaptureVolumeThread(LPVOID context)
{
    return(((AudioDeviceWindowsWave*)context)->DoSetCaptureVolumeThread());
}

//============================================================================
DWORD AudioDeviceWindowsWave::DoGetCaptureVolumeThread()
{
    HANDLE waitObject = m_hShutdownGetVolumeEvent;

    while (1)
    {
        DWORD waitResult = WaitForSingleObject(waitObject,
                                               GET_MIC_VOLUME_INTERVAL_MS);
        switch (waitResult)
        {
            case WAIT_OBJECT_0: // m_hShutdownGetVolumeEvent
                return 0;
            case WAIT_TIMEOUT:	// timeout notification
                break;
            default:            // unexpected error
                LogMsg( LOG_ERROR, 
                    "  unknown wait termination on get volume thread");
                return 1;
        }

        if (AGC())
        {
            uint32_t currentMicLevel = 0;
            if (MicrophoneVolume(currentMicLevel) == 0)
            {
                // This doesn't set the system volume, just stores it.
                m_critSect.Enter();
                if (m_ptrAudioBuffer)
                {
                    m_ptrAudioBuffer->SetCurrentMicLevel(currentMicLevel);
                }
                m_critSect.Leave();
            }
        }
    }
}

//============================================================================
DWORD AudioDeviceWindowsWave::DoSetCaptureVolumeThread()
{
    HANDLE waitArray[2] = {m_hShutdownSetVolumeEvent, m_hSetCaptureVolumeEvent};

    while (1)
    {
        DWORD waitResult = WaitForMultipleObjects(2, waitArray, FALSE, INFINITE);
        switch (waitResult)
        {
            case WAIT_OBJECT_0:     // m_hShutdownSetVolumeEvent
                return 0;
            case WAIT_OBJECT_0 + 1: // m_hSetCaptureVolumeEvent
                break;
            default:                // unexpected error
                LogMsg( LOG_ERROR, 
                    "  unknown wait termination on set volume thread");
                return 1;
        }

        m_critSect.Enter();
        uint32_t newMicLevel = m_newMicLevel;
        m_critSect.Leave();

        if (SetMicrophoneVolume(newMicLevel) == -1)
        {
            LogMsg( LOG_ERROR, 
                "  the required modification of the microphone volume failed");
        }
    }
    return 0;
}

// ----------------------------------------------------------------------------
//  Initialized
// ----------------------------------------------------------------------------
//============================================================================
bool AudioDeviceWindowsWave::Initialized() const
{
    return (m_initialized);
}

// ----------------------------------------------------------------------------
//  InitSpeaker
// ----------------------------------------------------------------------------
//============================================================================
int32_t AudioDeviceWindowsWave::InitSpeaker()
{

    CriticalSectionScoped lock(&m_critSect);

    if (m_playing)
    {
        return -1;
    }

    if (m_mixerManager.EnumerateSpeakers() == -1)
    {
        // failed to locate any valid/controllable speaker
        return -1;
    }

    if (IsUsingOutputDeviceIndex())
    {
        if (m_mixerManager.OpenSpeaker(OutputDeviceIndex()) == -1)
        {
            return -1;
        }
    }
    else
    {
        if (m_mixerManager.OpenSpeaker(OutputDevice()) == -1)
        {
            return -1;
        }
    }

    return 0;
}

// ----------------------------------------------------------------------------
//  InitMicrophone
// ----------------------------------------------------------------------------
//============================================================================
int32_t AudioDeviceWindowsWave::InitMicrophone()
{

    CriticalSectionScoped lock(&m_critSect);

    if (m_recording)
    {
        return -1;
    }

    if (m_mixerManager.EnumerateMicrophones() == -1)
    {
        // failed to locate any valid/controllable microphone
        return -1;
    }

    if (IsUsingInputDeviceIndex())
    {
        if (m_mixerManager.OpenMicrophone(InputDeviceIndex()) == -1)
        {
            return -1;
        }
    }
    else
    {
        if (m_mixerManager.OpenMicrophone(InputDevice()) == -1)
        {
            return -1;
        }
    }

    uint32_t maxVol = 0;
    if (m_mixerManager.MaxMicrophoneVolume(maxVol) == -1)
    {
        LogMsg( LOG_ERROR, 
            "  unable to retrieve max microphone volume");
    }
    m_maxMicVolume = maxVol;

    uint32_t minVol = 0;
    if (m_mixerManager.MinMicrophoneVolume(minVol) == -1)
    {
        LogMsg( LOG_ERROR, 
            "  unable to retrieve min microphone volume");
    }
    m_minMicVolume = minVol;

    return 0;
}

// ----------------------------------------------------------------------------
//  SpeakerIsInitialized
// ----------------------------------------------------------------------------
//============================================================================
bool AudioDeviceWindowsWave::SpeakerIsInitialized() const
{
    return (m_mixerManager.SpeakerIsInitialized());
}

// ----------------------------------------------------------------------------
//  MicrophoneIsInitialized
// ----------------------------------------------------------------------------
//============================================================================
bool AudioDeviceWindowsWave::MicrophoneIsInitialized() const
{
    return (m_mixerManager.MicrophoneIsInitialized());
}

// ----------------------------------------------------------------------------
//  SpeakerVolumeIsAvailable
// ----------------------------------------------------------------------------
//============================================================================
int32_t AudioDeviceWindowsWave::SpeakerVolumeIsAvailable(bool& available)
{

    bool isAvailable(false);

    // Enumerate all avaliable speakers and make an attempt to open up the
    // output mixer corresponding to the currently selected output device.
    //
    if (InitSpeaker() == -1)
    {
        // failed to find a valid speaker
        available = false;
        return 0;
    }

    // Check if the selected speaker has a volume control
    //
    m_mixerManager.SpeakerVolumeIsAvailable(isAvailable);
    available = isAvailable;

    // Close the initialized output mixer
    //
    m_mixerManager.CloseSpeaker();

    return 0;
}

// ----------------------------------------------------------------------------
//  SetSpeakerVolume
// ----------------------------------------------------------------------------
//============================================================================
int32_t AudioDeviceWindowsWave::SetSpeakerVolume(uint32_t volume)
{

    return (m_mixerManager.SetSpeakerVolume(volume));
}

// ----------------------------------------------------------------------------
//  SpeakerVolume
// ----------------------------------------------------------------------------
//============================================================================
int32_t AudioDeviceWindowsWave::SpeakerVolume(uint32_t& volume) const
{

    uint32_t level(0);

    if (m_mixerManager.SpeakerVolume(level) == -1)
    {
        return -1;
    }

    volume = level;
    return 0;
}

// ----------------------------------------------------------------------------
//  SetWaveOutVolume
//
//    The low-order word contains the left-channel volume setting, and the
//    high-order word contains the right-channel setting.
//    A value of 0xFFFF represents full volume, and a value of 0x0000 is silence.
//
//    If a device does not support both left and right volume control,
//    the low-order word of dwVolume specifies the volume level,
//    and the high-order word is ignored.
//
//    Most devices do not support the full 16 bits of volume-level control
//    and will not use the least-significant bits of the requested volume setting.
//    For example, if a device supports 4 bits of volume control, the values
//    0x4000, 0x4FFF, and 0x43BE will all be truncated to 0x4000.
// ----------------------------------------------------------------------------
//============================================================================
int32_t AudioDeviceWindowsWave::SetWaveOutVolume(uint16_t volumeLeft, uint16_t volumeRight)
{

    MMRESULT res(0);
    WAVEOUTCAPS caps;

    CriticalSectionScoped lock(&m_critSect);

    if (m_hWaveOut == NULL)
    {
        LogMsg( LOG_ERROR,  "no open playout device exists => using default");
    }

    // To determine whether the device supports volume control on both
    // the left and right channels, use the WAVECAPS_LRVOLUME flag.
    //
    res = waveOutGetDevCaps((UINT_PTR)m_hWaveOut, &caps, sizeof(WAVEOUTCAPS));
    if (MMSYSERR_NOERROR != res)
    {
        LogMsg( LOG_ERROR,  "waveOutGetDevCaps() failed (err=%d)", res);
        TraceWaveOutError(res);
    }
    if (!(caps.dwSupport & WAVECAPS_VOLUME))
    {
        // this device does not support volume control using the waveOutSetVolume API
        LogMsg( LOG_ERROR,  "device does not support volume control using the Wave API");
        return -1;
    }
    if (!(caps.dwSupport & WAVECAPS_LRVOLUME))
    {
        // high-order word (right channel) is ignored
        LogMsg( LOG_ERROR,  "device does not support volume control on both channels");
    }

    DWORD dwVolume(0x00000000);
    dwVolume = (DWORD)(((volumeRight & 0xFFFF) << 16) | (volumeLeft & 0xFFFF));

    res = waveOutSetVolume(m_hWaveOut, dwVolume);
    if (MMSYSERR_NOERROR != res)
    {
        LogMsg( LOG_ERROR,  "waveOutSetVolume() failed (err=%d)", res);
        TraceWaveOutError(res);
        return -1;
    }

    return 0;
}

// ----------------------------------------------------------------------------
//  WaveOutVolume
//
//    The low-order word of this location contains the left-channel volume setting,
//    and the high-order word contains the right-channel setting.
//    A value of 0xFFFF (65535) represents full volume, and a value of 0x0000
//    is silence.
//
//    If a device does not support both left and right volume control,
//    the low-order word of the specified location contains the mono volume level.
//
//    The full 16-bit setting(s) set with the waveOutSetVolume function is returned,
//    regardless of whether the device supports the full 16 bits of volume-level
//    control.
// ----------------------------------------------------------------------------
//============================================================================
int32_t AudioDeviceWindowsWave::WaveOutVolume(uint16_t& volumeLeft, uint16_t& volumeRight) const
{

    MMRESULT res(0);
    WAVEOUTCAPS caps;

    CriticalSectionScoped lock(&m_critSect);

    if (m_hWaveOut == NULL)
    {
        LogMsg( LOG_ERROR,  "no open playout device exists => using default");
    }

    // To determine whether the device supports volume control on both
    // the left and right channels, use the WAVECAPS_LRVOLUME flag.
    //
    res = waveOutGetDevCaps((UINT_PTR)m_hWaveOut, &caps, sizeof(WAVEOUTCAPS));
    if (MMSYSERR_NOERROR != res)
    {
        LogMsg( LOG_ERROR,  "waveOutGetDevCaps() failed (err=%d)", res);
        TraceWaveOutError(res);
    }
    if (!(caps.dwSupport & WAVECAPS_VOLUME))
    {
        // this device does not support volume control using the waveOutSetVolume API
        LogMsg( LOG_ERROR,  "device does not support volume control using the Wave API");
        return -1;
    }
    if (!(caps.dwSupport & WAVECAPS_LRVOLUME))
    {
        // high-order word (right channel) is ignored
        LogMsg( LOG_ERROR,  "device does not support volume control on both channels");
    }

    DWORD dwVolume(0x00000000);

    res = waveOutGetVolume(m_hWaveOut, &dwVolume);
    if (MMSYSERR_NOERROR != res)
    {
        LogMsg( LOG_ERROR,  "waveOutGetVolume() failed (err=%d)", res);
        TraceWaveOutError(res);
        return -1;
    }

    WORD wVolumeLeft = LOWORD(dwVolume);
    WORD wVolumeRight = HIWORD(dwVolume);

    volumeLeft = static_cast<uint16_t> (wVolumeLeft);
    volumeRight = static_cast<uint16_t> (wVolumeRight);

    return 0;
}

// ----------------------------------------------------------------------------
//  MaxSpeakerVolume
// ----------------------------------------------------------------------------

int32_t AudioDeviceWindowsWave::MaxSpeakerVolume(uint32_t& maxVolume) const
{

    uint32_t maxVol(0);

    if (m_mixerManager.MaxSpeakerVolume(maxVol) == -1)
    {
        return -1;
    }

    maxVolume = maxVol;
    return 0;
}

// ----------------------------------------------------------------------------
//  MinSpeakerVolume
// ----------------------------------------------------------------------------

int32_t AudioDeviceWindowsWave::MinSpeakerVolume(uint32_t& minVolume) const
{
    uint32_t minVol(0);
    if (m_mixerManager.MinSpeakerVolume(minVol) == -1)
    {
        return -1;
    }

    minVolume = minVol;
    return 0;
}

// ----------------------------------------------------------------------------
//  SpeakerVolumeStepSize
// ----------------------------------------------------------------------------

int32_t AudioDeviceWindowsWave::SpeakerVolumeStepSize(uint16_t& stepSize) const
{

    uint16_t delta(0);

    if (m_mixerManager.SpeakerVolumeStepSize(delta) == -1)
    {
        return -1;
    }

    stepSize = delta;
    return 0;
}

// ----------------------------------------------------------------------------
//  SpeakerMuteIsAvailable
// ----------------------------------------------------------------------------

int32_t AudioDeviceWindowsWave::SpeakerMuteIsAvailable(bool& available)
{

    bool isAvailable(false);

    // Enumerate all available speakers and make an attempt to open up the
    // output mixer corresponding to the currently selected output device.
    //
    if (InitSpeaker() == -1)
    {
        // If we end up here it means that the selected speaker has no volume
        // control, hence it is safe to state that there is no mute control
        // already at this stage.
        available = false;
        return 0;
    }

    // Check if the selected speaker has a mute control
    //
    m_mixerManager.SpeakerMuteIsAvailable(isAvailable);
    available = isAvailable;

    // Close the initialized output mixer
    //
    m_mixerManager.CloseSpeaker();

    return 0;
}

// ----------------------------------------------------------------------------
//  SetSpeakerMute
// ----------------------------------------------------------------------------

int32_t AudioDeviceWindowsWave::SetSpeakerMute(bool enable)
{
    return (m_mixerManager.SetSpeakerMute(enable));
}

// ----------------------------------------------------------------------------
//  SpeakerMute
// ----------------------------------------------------------------------------

int32_t AudioDeviceWindowsWave::SpeakerMute(bool& enabled) const
{

    bool muted(0);

    if (m_mixerManager.SpeakerMute(muted) == -1)
    {
        return -1;
    }

    enabled = muted;
    return 0;
}

// ----------------------------------------------------------------------------
//  MicrophoneMuteIsAvailable
// ----------------------------------------------------------------------------

int32_t AudioDeviceWindowsWave::MicrophoneMuteIsAvailable(bool& available)
{

    bool isAvailable(false);

    // Enumerate all available microphones and make an attempt to open up the
    // input mixer corresponding to the currently selected input device.
    //
    if (InitMicrophone() == -1)
    {
        // If we end up here it means that the selected microphone has no volume
        // control, hence it is safe to state that there is no boost control
        // already at this stage.
        available = false;
        return 0;
    }

    // Check if the selected microphone has a mute control
    //
    m_mixerManager.MicrophoneMuteIsAvailable(isAvailable);
    available = isAvailable;

    // Close the initialized input mixer
    //
    m_mixerManager.CloseMicrophone();

    return 0;
}

// ----------------------------------------------------------------------------
//  SetMicrophoneMute
// ----------------------------------------------------------------------------

int32_t AudioDeviceWindowsWave::SetMicrophoneMute(bool enable)
{
    return (m_mixerManager.SetMicrophoneMute(enable));
}

// ----------------------------------------------------------------------------
//  MicrophoneMute
// ----------------------------------------------------------------------------

int32_t AudioDeviceWindowsWave::MicrophoneMute(bool& enabled) const
{

    bool muted(0);

    if (m_mixerManager.MicrophoneMute(muted) == -1)
    {
        return -1;
    }

    enabled = muted;
    return 0;
}

// ----------------------------------------------------------------------------
//  MicrophoneBoostIsAvailable
// ----------------------------------------------------------------------------

int32_t AudioDeviceWindowsWave::MicrophoneBoostIsAvailable(bool& available)
{

    bool isAvailable(false);

    // Enumerate all available microphones and make an attempt to open up the
    // input mixer corresponding to the currently selected input device.
    //
    if (InitMicrophone() == -1)
    {
        // If we end up here it means that the selected microphone has no volume
        // control, hence it is safe to state that there is no boost control
        // already at this stage.
        available = false;
        return 0;
    }

    // Check if the selected microphone has a boost control
    //
    m_mixerManager.MicrophoneBoostIsAvailable(isAvailable);
    available = isAvailable;

    // Close the initialized input mixer
    //
    m_mixerManager.CloseMicrophone();

    return 0;
}

// ----------------------------------------------------------------------------
//  SetMicrophoneBoost
// ----------------------------------------------------------------------------

int32_t AudioDeviceWindowsWave::SetMicrophoneBoost(bool enable)
{

    return (m_mixerManager.SetMicrophoneBoost(enable));
}

// ----------------------------------------------------------------------------
//  MicrophoneBoost
// ----------------------------------------------------------------------------

int32_t AudioDeviceWindowsWave::MicrophoneBoost(bool& enabled) const
{

    bool onOff(0);

    if (m_mixerManager.MicrophoneBoost(onOff) == -1)
    {
        return -1;
    }

    enabled = onOff;
    return 0;
}

// ----------------------------------------------------------------------------
//  StereoRecordingIsAvailable
// ----------------------------------------------------------------------------

int32_t AudioDeviceWindowsWave::StereoRecordingIsAvailable(bool& available)
{
    available = true;
    return 0;
}

// ----------------------------------------------------------------------------
//  SetStereoRecording
// ----------------------------------------------------------------------------

int32_t AudioDeviceWindowsWave::SetStereoRecording(bool enable)
{

    if (enable)
        m_recChannels = 2;
    else
        m_recChannels = 1;

    return 0;
}

// ----------------------------------------------------------------------------
//  StereoRecording
// ----------------------------------------------------------------------------

int32_t AudioDeviceWindowsWave::StereoRecording(bool& enabled) const
{

    if (m_recChannels == 2)
        enabled = true;
    else
        enabled = false;

    return 0;
}

// ----------------------------------------------------------------------------
//  StereoPlayoutIsAvailable
// ----------------------------------------------------------------------------

int32_t AudioDeviceWindowsWave::StereoPlayoutIsAvailable(bool& available)
{
    available = false;
    return 0;
}

// ----------------------------------------------------------------------------
//  SetStereoPlayout
//
//  Specifies the number of output channels.
//
//  NOTE - the setting will only have an effect after InitPlayout has
//  been called.
//
//  16-bit mono:
//
//  Each sample is 2 bytes. Sample 1 is followed by samples 2, 3, 4, and so on.
//  For each sample, the first byte is the low-order byte of channel 0 and the
//  second byte is the high-order byte of channel 0.
//
//  16-bit stereo:
//
//  Each sample is 4 bytes. Sample 1 is followed by samples 2, 3, 4, and so on.
//  For each sample, the first byte is the low-order byte of channel 0 (left channel);
//  the second byte is the high-order byte of channel 0; the third byte is the
//  low-order byte of channel 1 (right channel); and the fourth byte is the
//  high-order byte of channel 1.
// ----------------------------------------------------------------------------

int32_t AudioDeviceWindowsWave::SetStereoPlayout(bool enable)
{

    if (enable)
        m_playChannels = 2;
    else
        m_playChannels = 1;

    return 0;
}

// ----------------------------------------------------------------------------
//  StereoPlayout
// ----------------------------------------------------------------------------

int32_t AudioDeviceWindowsWave::StereoPlayout(bool& enabled) const
{

    if (m_playChannels == 2)
        enabled = true;
    else
        enabled = false;

    return 0;
}

// ----------------------------------------------------------------------------
//  SetAGC
// ----------------------------------------------------------------------------

int32_t AudioDeviceWindowsWave::SetAGC(bool enable)
{

    m_AGC = enable;

    return 0;
}

// ----------------------------------------------------------------------------
//  AGC
// ----------------------------------------------------------------------------

bool AudioDeviceWindowsWave::AGC() const
{
    return m_AGC;
}

// ----------------------------------------------------------------------------
//  MicrophoneVolumeIsAvailable
// ----------------------------------------------------------------------------

int32_t AudioDeviceWindowsWave::MicrophoneVolumeIsAvailable(bool& available)
{

    bool isAvailable(false);

    // Enumerate all available microphones and make an attempt to open up the
    // input mixer corresponding to the currently selected output device.
    //
    if (InitMicrophone() == -1)
    {
        // Failed to find valid microphone
        available = false;
        return 0;
    }

    // Check if the selected microphone has a volume control
    //
    m_mixerManager.MicrophoneVolumeIsAvailable(isAvailable);
    available = isAvailable;

    // Close the initialized input mixer
    //
    m_mixerManager.CloseMicrophone();

    return 0;
}

// ----------------------------------------------------------------------------
//  SetMicrophoneVolume
// ----------------------------------------------------------------------------

int32_t AudioDeviceWindowsWave::SetMicrophoneVolume(uint32_t volume)
{
    return (m_mixerManager.SetMicrophoneVolume(volume));
}

// ----------------------------------------------------------------------------
//  MicrophoneVolume
// ----------------------------------------------------------------------------

int32_t AudioDeviceWindowsWave::MicrophoneVolume(uint32_t& volume) const
{
    uint32_t level(0);

    if (m_mixerManager.MicrophoneVolume(level) == -1)
    {
        LogMsg( LOG_ERROR,  "failed to retrieve current microphone level");
        return -1;
    }

    volume = level;
    return 0;
}

// ----------------------------------------------------------------------------
//  MaxMicrophoneVolume
// ----------------------------------------------------------------------------

int32_t AudioDeviceWindowsWave::MaxMicrophoneVolume(uint32_t& maxVolume) const
{
    // m_maxMicVolume can be zero in AudioMixerManager::MaxMicrophoneVolume():
    // (1) API GetLineControl() returns failure at querying the max Mic level.
    // (2) API GetLineControl() returns maxVolume as zero in rare cases.
    // Both cases show we don't have access to the mixer controls.
    // We return -1 here to indicate that.
    if (m_maxMicVolume == 0)
    {
        return -1;
    }

    maxVolume = m_maxMicVolume;;
    return 0;
}

// ----------------------------------------------------------------------------
//  MinMicrophoneVolume
// ----------------------------------------------------------------------------

int32_t AudioDeviceWindowsWave::MinMicrophoneVolume(uint32_t& minVolume) const
{
    minVolume = m_minMicVolume;
    return 0;
}

// ----------------------------------------------------------------------------
//  MicrophoneVolumeStepSize
// ----------------------------------------------------------------------------

int32_t AudioDeviceWindowsWave::MicrophoneVolumeStepSize(uint16_t& stepSize) const
{

    uint16_t delta(0);

    if (m_mixerManager.MicrophoneVolumeStepSize(delta) == -1)
    {
        return -1;
    }

    stepSize = delta;
    return 0;
}

// ----------------------------------------------------------------------------
//  PlayoutDevices
// ----------------------------------------------------------------------------

int16_t AudioDeviceWindowsWave::PlayoutDevices()
{

    return (waveOutGetNumDevs());
}

// ----------------------------------------------------------------------------
//  SetPlayoutDevice I (II)
// ----------------------------------------------------------------------------

int32_t AudioDeviceWindowsWave::SetPlayoutDevice(uint16_t index)
{

    if (m_playIsInitialized)
    {
        return -1;
    }

    UINT nDevices = waveOutGetNumDevs();
    LogMsg( LOG_INFO,  "number of availiable waveform-audio output devices is %u", nDevices);

    if (index < 0 || index > (nDevices-1))
    {
        LogMsg( LOG_ERROR,  "device index is out of range [0,%u]", (nDevices-1));
        return -1;
    }

    m_usingOutputDeviceIndex = true;
    m_outputDeviceIndex = index;
    m_outputDeviceIsSpecified = true;

    return 0;
}

// ----------------------------------------------------------------------------
//  SetPlayoutDevice II (II)
// ----------------------------------------------------------------------------

int32_t AudioDeviceWindowsWave::SetPlayoutDevice(AudioDeviceModule::WindowsDeviceType device)
{
    if (m_playIsInitialized)
    {
        return -1;
    }

    if (device == AudioDeviceModule::kDefaultDevice)
    {
    }
    else if (device == AudioDeviceModule::kDefaultCommunicationDevice)
    {
    }

    m_usingOutputDeviceIndex = false;
    m_outputDevice = device;
    m_outputDeviceIsSpecified = true;

    return 0;
}

// ----------------------------------------------------------------------------
//  PlayoutDeviceName
// ----------------------------------------------------------------------------

int32_t AudioDeviceWindowsWave::PlayoutDeviceName(
    uint16_t index,
    char name[kAdmMaxDeviceNameSize],
    char guid[kAdmMaxGuidSize])
{

    uint16_t nDevices(PlayoutDevices());

    // Special fix for the case when the user asks for the name of the default device.
    //
    if (index == (uint16_t)(-1))
    {
        index = 0;
    }

    if ((index > (nDevices-1)) || (name == NULL))
    {
        return -1;
    }

    memset(name, 0, kAdmMaxDeviceNameSize);

    if (guid != NULL)
    {
        memset(guid, 0, kAdmMaxGuidSize);
    }

    WAVEOUTCAPSW caps;    // szPname member (product name (NULL terminated) is a WCHAR
    MMRESULT res;

    res = waveOutGetDevCapsW(index, &caps, sizeof(WAVEOUTCAPSW));
    if (res != MMSYSERR_NOERROR)
    {
        LogMsg( LOG_ERROR,  "waveOutGetDevCapsW() failed (err=%d)", res);
        return -1;
    }
    if (WideCharToMultiByte(CP_UTF8, 0, caps.szPname, -1, name, kAdmMaxDeviceNameSize, NULL, NULL) == 0)
    {
        LogMsg( LOG_ERROR,  "WideCharToMultiByte(CP_UTF8) failed with error code %d - 1", GetLastError());
    }

    if (guid == NULL)
    {
        return 0;
    }

    // It is possible to get the unique endpoint ID string using the Wave API.
    // However, it is only supported on Windows Vista and Windows 7.

    size_t cbEndpointId(0);

    // Get the size (including the terminating null) of the endpoint ID string of the waveOut device.
    // Windows Vista supports the DRV_QUERYFUNCTIONINSTANCEIDSIZE and DRV_QUERYFUNCTIONINSTANCEID messages.
    res = waveOutMessage((HWAVEOUT)IntToPtr(index),
                          DRV_QUERYFUNCTIONINSTANCEIDSIZE,
                         (DWORD_PTR)&cbEndpointId, NULL);
    if (res != MMSYSERR_NOERROR)
    {
        // DRV_QUERYFUNCTIONINSTANCEIDSIZE is not supported <=> earlier version of Windows than Vista
        LogMsg( LOG_INFO,  "waveOutMessage(DRV_QUERYFUNCTIONINSTANCEIDSIZE) failed (err=%d)", res);
        TraceWaveOutError(res);
        // Best we can do is to copy the friendly name and use it as guid
        if (WideCharToMultiByte(CP_UTF8, 0, caps.szPname, -1, guid, kAdmMaxGuidSize, NULL, NULL) == 0)
        {
            LogMsg( LOG_ERROR,  "WideCharToMultiByte(CP_UTF8) failed with error code %d - 2", GetLastError());
        }
        return 0;
    }

    // waveOutMessage(DRV_QUERYFUNCTIONINSTANCEIDSIZE) worked => we are on a Vista or Windows 7 device

    WCHAR *pstrEndpointId = NULL;
    pstrEndpointId = (WCHAR*)CoTaskMemAlloc(cbEndpointId);

    // Get the endpoint ID string for this waveOut device.
    res = waveOutMessage((HWAVEOUT)IntToPtr(index),
                          DRV_QUERYFUNCTIONINSTANCEID,
                         (DWORD_PTR)pstrEndpointId,
                          cbEndpointId);
    if (res != MMSYSERR_NOERROR)
    {
        LogMsg( LOG_INFO,  "waveOutMessage(DRV_QUERYFUNCTIONINSTANCEID) failed (err=%d)", res);
        TraceWaveOutError(res);
        // Best we can do is to copy the friendly name and use it as guid
        if (WideCharToMultiByte(CP_UTF8, 0, caps.szPname, -1, guid, kAdmMaxGuidSize, NULL, NULL) == 0)
        {
            LogMsg( LOG_ERROR,  "WideCharToMultiByte(CP_UTF8) failed with error code %d - 3", GetLastError());
        }
        CoTaskMemFree(pstrEndpointId);
        return 0;
    }

    if (WideCharToMultiByte(CP_UTF8, 0, pstrEndpointId, -1, guid, kAdmMaxGuidSize, NULL, NULL) == 0)
    {
        LogMsg( LOG_ERROR,  "WideCharToMultiByte(CP_UTF8) failed with error code %d - 4", GetLastError());
    }
    CoTaskMemFree(pstrEndpointId);

    return 0;
}

// ----------------------------------------------------------------------------
//  RecordingDeviceName
// ----------------------------------------------------------------------------

int32_t AudioDeviceWindowsWave::RecordingDeviceName(
    uint16_t index,
    char name[kAdmMaxDeviceNameSize],
    char guid[kAdmMaxGuidSize])
{
    uint16_t nDevices(RecordingDevices());

    // Special fix for the case when the user asks for the name of the default device.
    //
    if (index == (uint16_t)(-1))
    {
        index = 0;
    }

    if ((index > (nDevices-1)) || (name == NULL))
    {
        return -1;
    }

    memset(name, 0, kAdmMaxDeviceNameSize);

    if (guid != NULL)
    {
        memset(guid, 0, kAdmMaxGuidSize);
    }

    WAVEINCAPSW caps;    // szPname member (product name (NULL terminated) is a WCHAR
    MMRESULT res;

    res = waveInGetDevCapsW(index, &caps, sizeof(WAVEINCAPSW));
    if (res != MMSYSERR_NOERROR)
    {
        LogMsg( LOG_ERROR,  "waveInGetDevCapsW() failed (err=%d)", res);
        return -1;
    }
    if (WideCharToMultiByte(CP_UTF8, 0, caps.szPname, -1, name, kAdmMaxDeviceNameSize, NULL, NULL) == 0)
    {
        LogMsg( LOG_ERROR,  "WideCharToMultiByte(CP_UTF8) failed with error code %d - 1", GetLastError());
    }

    if (guid == NULL)
    {
        return 0;
    }

    // It is possible to get the unique endpoint ID string using the Wave API.
    // However, it is only supported on Windows Vista and Windows 7.

    size_t cbEndpointId(0);

    // Get the size (including the terminating null) of the endpoint ID string of the waveOut device.
    // Windows Vista supports the DRV_QUERYFUNCTIONINSTANCEIDSIZE and DRV_QUERYFUNCTIONINSTANCEID messages.
    res = waveInMessage((HWAVEIN)IntToPtr(index),
                         DRV_QUERYFUNCTIONINSTANCEIDSIZE,
                        (DWORD_PTR)&cbEndpointId, NULL);
    if (res != MMSYSERR_NOERROR)
    {
        // DRV_QUERYFUNCTIONINSTANCEIDSIZE is not supported <=> earlier version of Windows than Vista
        LogMsg( LOG_INFO,  "waveInMessage(DRV_QUERYFUNCTIONINSTANCEIDSIZE) failed (err=%d)", res);
        TraceWaveInError(res);
        // Best we can do is to copy the friendly name and use it as guid
        if (WideCharToMultiByte(CP_UTF8, 0, caps.szPname, -1, guid, kAdmMaxGuidSize, NULL, NULL) == 0)
        {
            LogMsg( LOG_ERROR,  "WideCharToMultiByte(CP_UTF8) failed with error code %d - 2", GetLastError());
        }
        return 0;
    }

    // waveOutMessage(DRV_QUERYFUNCTIONINSTANCEIDSIZE) worked => we are on a Vista or Windows 7 device

    WCHAR *pstrEndpointId = NULL;
    pstrEndpointId = (WCHAR*)CoTaskMemAlloc(cbEndpointId);

    // Get the endpoint ID string for this waveOut device.
    res = waveInMessage((HWAVEIN)IntToPtr(index),
                          DRV_QUERYFUNCTIONINSTANCEID,
                         (DWORD_PTR)pstrEndpointId,
                          cbEndpointId);
    if (res != MMSYSERR_NOERROR)
    {
        LogMsg( LOG_INFO,  "waveInMessage(DRV_QUERYFUNCTIONINSTANCEID) failed (err=%d)", res);
        TraceWaveInError(res);
        // Best we can do is to copy the friendly name and use it as guid
        if (WideCharToMultiByte(CP_UTF8, 0, caps.szPname, -1, guid, kAdmMaxGuidSize, NULL, NULL) == 0)
        {
            LogMsg( LOG_ERROR,  "WideCharToMultiByte(CP_UTF8) failed with error code %d - 3", GetLastError());
        }
        CoTaskMemFree(pstrEndpointId);
        return 0;
    }

    if (WideCharToMultiByte(CP_UTF8, 0, pstrEndpointId, -1, guid, kAdmMaxGuidSize, NULL, NULL) == 0)
    {
        LogMsg( LOG_ERROR,  "WideCharToMultiByte(CP_UTF8) failed with error code %d - 4", GetLastError());
    }
    CoTaskMemFree(pstrEndpointId);

    return 0;
}

// ----------------------------------------------------------------------------
//  RecordingDevices
// ----------------------------------------------------------------------------

int16_t AudioDeviceWindowsWave::RecordingDevices()
{

    return (waveInGetNumDevs());
}

// ----------------------------------------------------------------------------
//  SetRecordingDevice I (II)
// ----------------------------------------------------------------------------

int32_t AudioDeviceWindowsWave::SetRecordingDevice(uint16_t index)
{

    if (m_recIsInitialized)
    {
        return -1;
    }

    UINT nDevices = waveInGetNumDevs();
    LogMsg( LOG_INFO,  "number of availiable waveform-audio input devices is %u", nDevices);

    if (index < 0 || index > (nDevices-1))
    {
        LogMsg( LOG_ERROR,  "device index is out of range [0,%u]", (nDevices-1));
        return -1;
    }

    m_usingInputDeviceIndex = true;
    m_inputDeviceIndex = index;
    m_inputDeviceIsSpecified = true;

    return 0;
}

// ----------------------------------------------------------------------------
//  SetRecordingDevice II (II)
// ----------------------------------------------------------------------------

int32_t AudioDeviceWindowsWave::SetRecordingDevice(AudioDeviceModule::WindowsDeviceType device)
{
    if (device == AudioDeviceModule::kDefaultDevice)
    {
    }
    else if (device == AudioDeviceModule::kDefaultCommunicationDevice)
    {
    }

    if (m_recIsInitialized)
    {
        return -1;
    }

    m_usingInputDeviceIndex = false;
    m_inputDevice = device;
    m_inputDeviceIsSpecified = true;

    return 0;
}

// ----------------------------------------------------------------------------
//  PlayoutIsAvailable
// ----------------------------------------------------------------------------

int32_t AudioDeviceWindowsWave::PlayoutIsAvailable(bool& available)
{

    available = false;

    // Try to initialize the playout side
    int32_t res = InitPlayout();

    // Cancel effect of initialization
    StopPlayout();

    if (res != -1)
    {
        available = true;
    }

    return 0;
}

// ----------------------------------------------------------------------------
//  RecordingIsAvailable
// ----------------------------------------------------------------------------

int32_t AudioDeviceWindowsWave::RecordingIsAvailable(bool& available)
{

    available = false;

    // Try to initialize the recording side
    int32_t res = InitRecording();

    // Cancel effect of initialization
    StopRecording();

    if (res != -1)
    {
        available = true;
    }

    return 0;
}

// ----------------------------------------------------------------------------
//  InitPlayout
// ----------------------------------------------------------------------------

int32_t AudioDeviceWindowsWave::InitPlayout()
{

    CriticalSectionScoped lock(&m_critSect);

    if (m_playing)
    {
        return -1;
    }

    if (!m_outputDeviceIsSpecified)
    {
        return -1;
    }

    if (m_playIsInitialized)
    {
        return 0;
    }

    // Initialize the speaker (devices might have been added or removed)
    if (InitSpeaker() == -1)
    {
        LogMsg( LOG_ERROR,  "InitSpeaker() failed");
    }

    // Enumerate all availiable output devices
    EnumeratePlayoutDevices();

    // Start by closing any existing wave-output devices
    //
    MMRESULT res(MMSYSERR_ERROR);

    if (m_hWaveOut != NULL)
    {
        res = waveOutClose(m_hWaveOut);
        if (MMSYSERR_NOERROR != res)
        {
            LogMsg( LOG_ERROR,  "waveOutClose() failed (err=%d)", res);
            TraceWaveOutError(res);
        }
    }

    // Set the output wave format
    //
    WAVEFORMATEX waveFormat;

    waveFormat.wFormatTag      = WAVE_FORMAT_PCM;
    waveFormat.nChannels       = m_playChannels;  // mono <=> 1, stereo <=> 2
    waveFormat.nSamplesPerSec  = N_PLAY_SAMPLES_PER_SEC;
    waveFormat.wBitsPerSample  = 16;
    waveFormat.nBlockAlign     = waveFormat.nChannels * (waveFormat.wBitsPerSample/8);
    waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
    waveFormat.cbSize          = 0;

    // Open the given waveform-audio output device for playout
    //
    HWAVEOUT hWaveOut(NULL);

    if (IsUsingOutputDeviceIndex())
    {
        // verify settings first
        res = waveOutOpen(NULL, m_outputDeviceIndex, &waveFormat, 0, 0, CALLBACK_NULL | WAVE_FORMAT_QUERY);
        if (MMSYSERR_NOERROR == res)
        {
            // open the given waveform-audio output device for recording
            res = waveOutOpen(&hWaveOut, m_outputDeviceIndex, &waveFormat, 0, 0, CALLBACK_NULL);
            LogMsg( LOG_INFO,  "opening output device corresponding to device ID %u", m_outputDeviceIndex);
        }
    }
    else
    {
        if (m_outputDevice == AudioDeviceModule::kDefaultCommunicationDevice)
        {
            // check if it is possible to open the default communication device (supported on Windows 7)
            res = waveOutOpen(NULL, WAVE_MAPPER, &waveFormat, 0, 0, CALLBACK_NULL | WAVE_MAPPED_DEFAULT_COMMUNICATION_DEVICE | WAVE_FORMAT_QUERY);
            if (MMSYSERR_NOERROR == res)
            {
                // if so, open the default communication device for real
                res = waveOutOpen(&hWaveOut, WAVE_MAPPER, &waveFormat, 0, 0, CALLBACK_NULL |  WAVE_MAPPED_DEFAULT_COMMUNICATION_DEVICE);
                LogMsg( LOG_INFO,  "opening default communication device");
            }
            else
            {
                // use default device since default communication device was not avaliable
                res = waveOutOpen(&hWaveOut, WAVE_MAPPER, &waveFormat, 0, 0, CALLBACK_NULL);
                LogMsg( LOG_INFO,  "unable to open default communication device => using default instead");
            }
        }
        else if (m_outputDevice == AudioDeviceModule::kDefaultDevice)
        {
            // open default device since it has been requested
            res = waveOutOpen(NULL, WAVE_MAPPER, &waveFormat, 0, 0, CALLBACK_NULL | WAVE_FORMAT_QUERY);
            if (MMSYSERR_NOERROR == res)
            {
                res = waveOutOpen(&hWaveOut, WAVE_MAPPER, &waveFormat, 0, 0, CALLBACK_NULL);
                LogMsg( LOG_INFO,  "opening default output device");
            }
        }
    }

    if (MMSYSERR_NOERROR != res)
    {
        LogMsg( LOG_ERROR,  "waveOutOpen() failed (err=%d)", res);
        TraceWaveOutError(res);
        return -1;
    }

    // Log information about the aquired output device
    //
    WAVEOUTCAPS caps;

    res = waveOutGetDevCaps((UINT_PTR)hWaveOut, &caps, sizeof(WAVEOUTCAPS));
    if (res != MMSYSERR_NOERROR)
    {
        LogMsg( LOG_ERROR,  "waveOutGetDevCaps() failed (err=%d)", res);
        TraceWaveOutError(res);
    }

    UINT deviceID(0);
    res = waveOutGetID(hWaveOut, &deviceID);
    if (res != MMSYSERR_NOERROR)
    {
        LogMsg( LOG_ERROR,  "waveOutGetID() failed (err=%d)", res);
        TraceWaveOutError(res);
    }
    LogMsg( LOG_INFO,  "utilized device ID : %u", deviceID);
    LogMsg( LOG_INFO,  "product name       : %s", caps.szPname);

    // Store valid handle for the open waveform-audio output device
    m_hWaveOut = hWaveOut;

    // Store the input wave header as well
    m_waveFormatOut = waveFormat;

    // Prepare wave-out headers
    //
    const uint8_t bytesPerSample = 2*m_playChannels;

    for (int n = 0; n < N_BUFFERS_OUT; n++)
    {
        // set up the output wave header
        m_waveHeaderOut[n].lpData          = reinterpret_cast<LPSTR>(&m_playBuffer[n]);
        m_waveHeaderOut[n].dwBufferLength  = bytesPerSample*PLAY_BUF_SIZE_IN_SAMPLES;
        m_waveHeaderOut[n].dwFlags         = 0;
        m_waveHeaderOut[n].dwLoops         = 0;

        memset(m_playBuffer[n], 0, bytesPerSample*PLAY_BUF_SIZE_IN_SAMPLES);

        // The waveOutPrepareHeader function prepares a waveform-audio data block for playback.
        // The lpData, dwBufferLength, and dwFlags members of the WAVEHDR structure must be set
        // before calling this function.
        //
        res = waveOutPrepareHeader(m_hWaveOut, &m_waveHeaderOut[n], sizeof(WAVEHDR));
        if (MMSYSERR_NOERROR != res)
        {
            LogMsg( LOG_ERROR,  "waveOutPrepareHeader(%d) failed (err=%d)", n, res);
            TraceWaveOutError(res);
        }

        // perform extra check to ensure that the header is prepared
        if (m_waveHeaderOut[n].dwFlags != WHDR_PREPARED)
        {
            LogMsg( LOG_ERROR,  "waveOutPrepareHeader(%d) failed (dwFlags != WHDR_PREPARED)", n);
        }
    }

    // Mark playout side as initialized
    m_playIsInitialized = true;

    m_dTcheckPlayBufDelay = 10;  // check playback buffer delay every 10 ms
    m_playBufCount = 0;          // index of active output wave header (<=> output buffer index)
    m_playBufDelay = 80;         // buffer delay/size is initialized to 80 ms and slowly decreased until er < 25
    m_minPlayBufDelay = 25;      // minimum playout buffer delay
    m_MAXm_minBuffer = 65;        // adaptive minimum playout buffer delay cannot be larger than this value
    m_intro = 1;                 // Used to make sure that adaption starts after (2000-1700)/100 seconds
    m_waitCounter = 1700;        // Counter for start of adaption of playback buffer
    m_erZeroCounter = 0;         // Log how many times er = 0 in consequtive calls to RecTimeProc
    m_useHeader = 0;             // Counts number of "useHeader" detections. Stops at 2.

    m_writtenSamples = 0;
    m_writtenSamplesOld = 0;
    m_playedSamplesOld = 0;
    m_sndCardPlayDelay = 0;
    m_sndCardRecDelay = 0;

    LogMsg( LOG_INFO, "initial playout status: m_playBufDelay=%d, m_minPlayBufDelay=%d",
        m_playBufDelay, m_minPlayBufDelay);

    return 0;
}

// ----------------------------------------------------------------------------
//  InitRecording
// ----------------------------------------------------------------------------

int32_t AudioDeviceWindowsWave::InitRecording()
{

    CriticalSectionScoped lock(&m_critSect);

    if (m_recording)
    {
        return -1;
    }

    if (!m_inputDeviceIsSpecified)
    {
        return -1;
    }

    if (m_recIsInitialized)
    {
        return 0;
    }

    m_avgCPULoad = 0;
    m_playAcc  = 0;

    // Initialize the microphone (devices might have been added or removed)
    if (InitMicrophone() == -1)
    {
        LogMsg( LOG_ERROR,  "InitMicrophone() failed");
    }

    // Enumerate all availiable input devices
    EnumerateRecordingDevices();

    // Start by closing any existing wave-input devices
    //
    MMRESULT res(MMSYSERR_ERROR);

    if (m_hWaveIn != NULL)
    {
        res = waveInClose(m_hWaveIn);
        if (MMSYSERR_NOERROR != res)
        {
            LogMsg( LOG_ERROR,  "waveInClose() failed (err=%d)", res);
            TraceWaveInError(res);
        }
    }

    // Set the input wave format
    //
    WAVEFORMATEX waveFormat;

    waveFormat.wFormatTag      = WAVE_FORMAT_PCM;
    waveFormat.nChannels       = m_recChannels;  // mono <=> 1, stereo <=> 2
    waveFormat.nSamplesPerSec  = N_REC_SAMPLES_PER_SEC;
    waveFormat.wBitsPerSample  = 16;
    waveFormat.nBlockAlign     = waveFormat.nChannels * (waveFormat.wBitsPerSample/8);
    waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
    waveFormat.cbSize          = 0;

    // Open the given waveform-audio input device for recording
    //
    HWAVEIN hWaveIn(NULL);

    if (IsUsingInputDeviceIndex())
    {
        // verify settings first
        res = waveInOpen(NULL, m_inputDeviceIndex, &waveFormat, 0, 0, CALLBACK_NULL | WAVE_FORMAT_QUERY);
        if (MMSYSERR_NOERROR == res)
        {
            // open the given waveform-audio input device for recording
            res = waveInOpen(&hWaveIn, m_inputDeviceIndex, &waveFormat, 0, 0, CALLBACK_NULL);
            LogMsg( LOG_INFO,  "opening input device corresponding to device ID %u", m_inputDeviceIndex);
        }
    }
    else
    {
        if (m_inputDevice == AudioDeviceModule::kDefaultCommunicationDevice)
        {
            // check if it is possible to open the default communication device (supported on Windows 7)
            res = waveInOpen(NULL, WAVE_MAPPER, &waveFormat, 0, 0, CALLBACK_NULL | WAVE_MAPPED_DEFAULT_COMMUNICATION_DEVICE | WAVE_FORMAT_QUERY);
            if (MMSYSERR_NOERROR == res)
            {
                // if so, open the default communication device for real
                res = waveInOpen(&hWaveIn, WAVE_MAPPER, &waveFormat, 0, 0, CALLBACK_NULL | WAVE_MAPPED_DEFAULT_COMMUNICATION_DEVICE);
                LogMsg( LOG_INFO,  "opening default communication device");
            }
            else
            {
                // use default device since default communication device was not avaliable
                res = waveInOpen(&hWaveIn, WAVE_MAPPER, &waveFormat, 0, 0, CALLBACK_NULL);
                LogMsg( LOG_INFO,  "unable to open default communication device => using default instead");
            }
        }
        else if (m_inputDevice == AudioDeviceModule::kDefaultDevice)
        {
            // open default device since it has been requested
            res = waveInOpen(NULL, WAVE_MAPPER, &waveFormat, 0, 0, CALLBACK_NULL | WAVE_FORMAT_QUERY);
            if (MMSYSERR_NOERROR == res)
            {
                res = waveInOpen(&hWaveIn, WAVE_MAPPER, &waveFormat, 0, 0, CALLBACK_NULL);
                LogMsg( LOG_INFO,  "opening default input device");
            }
        }
    }

    if (MMSYSERR_NOERROR != res)
    {
        LogMsg( LOG_ERROR,  "waveInOpen() failed (err=%d)", res);
        TraceWaveInError(res);
        return -1;
    }

    // Log information about the aquired input device
    //
    WAVEINCAPS caps;

    res = waveInGetDevCaps((UINT_PTR)hWaveIn, &caps, sizeof(WAVEINCAPS));
    if (res != MMSYSERR_NOERROR)
    {
        LogMsg( LOG_ERROR,  "waveInGetDevCaps() failed (err=%d)", res);
        TraceWaveInError(res);
    }

    UINT deviceID(0);
    res = waveInGetID(hWaveIn, &deviceID);
    if (res != MMSYSERR_NOERROR)
    {
        LogMsg( LOG_ERROR,  "waveInGetID() failed (err=%d)", res);
        TraceWaveInError(res);
    }
    LogMsg( LOG_INFO,  "utilized device ID : %u", deviceID);
    LogMsg( LOG_INFO,  "product name       : %s", caps.szPname);

    // Store valid handle for the open waveform-audio input device
    m_hWaveIn = hWaveIn;

    // Store the input wave header as well
    m_waveFormatIn = waveFormat;

    // Mark recording side as initialized
    m_recIsInitialized = true;

    m_recBufCount = 0;     // index of active input wave header (<=> input buffer index)
    m_recDelayCount = 0;   // ensures that input buffers are returned with certain delay

    return 0;
}

// ----------------------------------------------------------------------------
//  StartRecording
// ----------------------------------------------------------------------------

int32_t AudioDeviceWindowsWave::StartRecording()
{

    if (!m_recIsInitialized)
    {
        return -1;
    }

    if (m_recording)
    {
        return 0;
    }

    // set state to ensure that the recording starts from the audio thread
    m_startRec = true;

    // the audio thread will signal when recording has stopped
    if (kEventTimeout == m_recStartEvent.Wait(10000))
    {
        m_startRec = false;
        StopRecording();
        LogMsg( LOG_ERROR,  "failed to activate recording");
        return -1;
    }

    if (m_recording)
    {
        // the recording state is set by the audio thread after recording has started
    }
    else
    {
        LogMsg( LOG_ERROR,  "failed to activate recording");
        return -1;
    }

    return 0;
}

// ----------------------------------------------------------------------------
//  StopRecording
// ----------------------------------------------------------------------------

int32_t AudioDeviceWindowsWave::StopRecording()
{

    CriticalSectionScoped lock(&m_critSect);

    if (!m_recIsInitialized)
    {
        return 0;
    }

    if (m_hWaveIn == NULL)
    {
        return -1;
    }

    bool wasRecording = m_recording;
    m_recIsInitialized = false;
    m_recording = false;

    MMRESULT res;

    // Stop waveform-adio input. If there are any buffers in the queue, the
    // current buffer will be marked as done (the dwBytesRecorded member in
    // the header will contain the length of data), but any empty buffers in
    // the queue will remain there.
    //
    res = waveInStop(m_hWaveIn);
    if (MMSYSERR_NOERROR != res)
    {
        LogMsg( LOG_ERROR,  "waveInStop() failed (err=%d)", res);
        TraceWaveInError(res);
    }

    // Stop input on the given waveform-audio input device and resets the current
    // position to zero. All pending buffers are marked as done and returned to
    // the application.
    //
    res = waveInReset(m_hWaveIn);
    if (MMSYSERR_NOERROR != res)
    {
        LogMsg( LOG_ERROR,  "waveInReset() failed (err=%d)", res);
        TraceWaveInError(res);
    }

    // Clean up the preparation performed by the waveInPrepareHeader function.
    // Only unprepare header if recording was ever started (and headers are prepared).
    //
    if (wasRecording)
    {
        LogMsg( LOG_INFO,  "waveInUnprepareHeader() will be performed");
        for (int n = 0; n < N_BUFFERS_IN; n++)
        {
            res = waveInUnprepareHeader(m_hWaveIn, &m_waveHeaderIn[n], sizeof(WAVEHDR));
            if (MMSYSERR_NOERROR != res)
            {
                LogMsg( LOG_ERROR,  "waveInUnprepareHeader() failed (err=%d)", res);
                TraceWaveInError(res);
            }
        }
    }

    // Close the given waveform-audio input device.
    //
    res = waveInClose(m_hWaveIn);
    if (MMSYSERR_NOERROR != res)
    {
        LogMsg( LOG_ERROR,  "waveInClose() failed (err=%d)", res);
        TraceWaveInError(res);
    }

    // Set the wave input handle to NULL
    //
    m_hWaveIn = NULL;
    LogMsg( LOG_INFO,  "m_hWaveIn is now set to NULL");

    return 0;
}

// ----------------------------------------------------------------------------
//  RecordingIsInitialized
// ----------------------------------------------------------------------------

bool AudioDeviceWindowsWave::RecordingIsInitialized() const
{
    return (m_recIsInitialized);
}

// ----------------------------------------------------------------------------
//  Recording
// ----------------------------------------------------------------------------

bool AudioDeviceWindowsWave::Recording() const
{
    return (m_recording);
}

// ----------------------------------------------------------------------------
//  PlayoutIsInitialized
// ----------------------------------------------------------------------------

bool AudioDeviceWindowsWave::PlayoutIsInitialized() const
{
    return (m_playIsInitialized);
}

// ----------------------------------------------------------------------------
//  StartPlayout
// ----------------------------------------------------------------------------

int32_t AudioDeviceWindowsWave::StartPlayout()
{

    if (!m_playIsInitialized)
    {
        return -1;
    }

    if (m_playing)
    {
        return 0;
    }

    // set state to ensure that playout starts from the audio thread
    m_startPlay = true;

    // the audio thread will signal when recording has started
    if (kEventTimeout == m_playStartEvent.Wait(10000))
    {
        m_startPlay = false;
        StopPlayout();
        LogMsg( LOG_ERROR,  "failed to activate playout");
        return -1;
    }

    if (m_playing)
    {
        // the playing state is set by the audio thread after playout has started
    }
    else
    {
        LogMsg( LOG_ERROR,  "failed to activate playing");
        return -1;
    }

    return 0;
}

// ----------------------------------------------------------------------------
//  StopPlayout
// ----------------------------------------------------------------------------

int32_t AudioDeviceWindowsWave::StopPlayout()
{

    CriticalSectionScoped lock(&m_critSect);

    if (!m_playIsInitialized)
    {
        return 0;
    }

    if (m_hWaveOut == NULL)
    {
        return -1;
    }

    m_playIsInitialized = false;
    m_playing = false;
    m_sndCardPlayDelay = 0;
    m_sndCardRecDelay = 0;

    MMRESULT res;

    // The waveOutReset function stops playback on the given waveform-audio
    // output device and resets the current position to zero. All pending
    // playback buffers are marked as done (WHDR_DONE) and returned to the application.
    // After this function returns, the application can send new playback buffers
    // to the device by calling waveOutWrite, or close the device by calling waveOutClose.
    //
    res = waveOutReset(m_hWaveOut);
    if (MMSYSERR_NOERROR != res)
    {
        LogMsg( LOG_ERROR,  "waveOutReset() failed (err=%d)", res);
        TraceWaveOutError(res);
    }

    // The waveOutUnprepareHeader function cleans up the preparation performed
    // by the waveOutPrepareHeader function. This function must be called after
    // the device driver is finished with a data block.
    // You must call this function before freeing the buffer.
    //
    for (int n = 0; n < N_BUFFERS_OUT; n++)
    {
        res = waveOutUnprepareHeader(m_hWaveOut, &m_waveHeaderOut[n], sizeof(WAVEHDR));
        if (MMSYSERR_NOERROR != res)
        {
            LogMsg( LOG_ERROR,  "waveOutUnprepareHeader() failed (err=%d)", res);
            TraceWaveOutError(res);
        }
    }

    // The waveOutClose function closes the given waveform-audio output device.
    // The close operation fails if the device is still playing a waveform-audio
    // buffer that was previously sent by calling waveOutWrite. Before calling
    // waveOutClose, the application must wait for all buffers to finish playing
    // or call the waveOutReset function to terminate playback.
    //
    res = waveOutClose(m_hWaveOut);
    if (MMSYSERR_NOERROR != res)
    {
        LogMsg( LOG_ERROR,  "waveOutClose() failed (err=%d)", res);
        TraceWaveOutError(res);
    }

    m_hWaveOut = NULL;
    LogMsg( LOG_INFO,  "m_hWaveOut is now set to NULL");

    return 0;
}

// ----------------------------------------------------------------------------
//  PlayoutDelay
// ----------------------------------------------------------------------------

int32_t AudioDeviceWindowsWave::PlayoutDelay(uint16_t& delayMS) const
{
    CriticalSectionScoped lock(&m_critSect);
    delayMS = (uint16_t)m_sndCardPlayDelay;
    return 0;
}

// ----------------------------------------------------------------------------
//  RecordingDelay
// ----------------------------------------------------------------------------

int32_t AudioDeviceWindowsWave::RecordingDelay(uint16_t& delayMS) const
{
    CriticalSectionScoped lock(&m_critSect);
    delayMS = (uint16_t)m_sndCardRecDelay;
    return 0;
}

// ----------------------------------------------------------------------------
//  Playing
// ----------------------------------------------------------------------------

bool AudioDeviceWindowsWave::Playing() const
{
    return (m_playing);
}
// ----------------------------------------------------------------------------
//  SetPlayoutBuffer
// ----------------------------------------------------------------------------

int32_t AudioDeviceWindowsWave::SetPlayoutBuffer(const AudioDeviceModule::BufferType type, uint16_t sizeMS)
{
    CriticalSectionScoped lock(&m_critSect);
    m_playBufType = type;
    if (type == AudioDeviceModule::kFixedBufferSize)
    {
        m_playBufDelayFixed = sizeMS;
    }
    return 0;
}

// ----------------------------------------------------------------------------
//  PlayoutBuffer
// ----------------------------------------------------------------------------

int32_t AudioDeviceWindowsWave::PlayoutBuffer(AudioDeviceModule::BufferType& type, uint16_t& sizeMS) const
{
    CriticalSectionScoped lock(&m_critSect);
    type = m_playBufType;
    if (type == AudioDeviceModule::kFixedBufferSize)
    {
        sizeMS = m_playBufDelayFixed;
    }
    else
    {
        sizeMS = m_playBufDelay;
    }

    return 0;
}

// ----------------------------------------------------------------------------
//  CPULoad
// ----------------------------------------------------------------------------

int32_t AudioDeviceWindowsWave::CPULoad(uint16_t& load) const
{

    load = static_cast<uint16_t>(100*m_avgCPULoad);

    return 0;
}

// ----------------------------------------------------------------------------
//  PlayoutWarning
// ----------------------------------------------------------------------------

bool AudioDeviceWindowsWave::PlayoutWarning() const
{
    return ( m_playWarning > 0);
}

// ----------------------------------------------------------------------------
//  PlayoutError
// ----------------------------------------------------------------------------

bool AudioDeviceWindowsWave::PlayoutError() const
{
    return ( m_playError > 0);
}

// ----------------------------------------------------------------------------
//  RecordingWarning
// ----------------------------------------------------------------------------

bool AudioDeviceWindowsWave::RecordingWarning() const
{
    return ( m_recWarning > 0);
}

// ----------------------------------------------------------------------------
//  RecordingError
// ----------------------------------------------------------------------------

bool AudioDeviceWindowsWave::RecordingError() const
{
    return ( m_recError > 0);
}

// ----------------------------------------------------------------------------
//  ClearPlayoutWarning
// ----------------------------------------------------------------------------

void AudioDeviceWindowsWave::ClearPlayoutWarning()
{
    m_playWarning = 0;
}

// ----------------------------------------------------------------------------
//  ClearPlayoutError
// ----------------------------------------------------------------------------

void AudioDeviceWindowsWave::ClearPlayoutError()
{
    m_playError = 0;
}

// ----------------------------------------------------------------------------
//  ClearRecordingWarning
// ----------------------------------------------------------------------------

void AudioDeviceWindowsWave::ClearRecordingWarning()
{
    m_recWarning = 0;
}

// ----------------------------------------------------------------------------
//  ClearRecordingError
// ----------------------------------------------------------------------------

void AudioDeviceWindowsWave::ClearRecordingError()
{
    m_recError = 0;
}

//================================================================================
//                                 Private Methods
//================================================================================

// ----------------------------------------------------------------------------
//  InputSanityCheckAfterUnlockedPeriod
// ----------------------------------------------------------------------------

int32_t AudioDeviceWindowsWave::InputSanityCheckAfterUnlockedPeriod() const
{
    if (m_hWaveIn == NULL)
    {
        LogMsg( LOG_ERROR,  "input state has been modified during unlocked period");
        return -1;
    }
    return 0;
}

// ----------------------------------------------------------------------------
//  OutputSanityCheckAfterUnlockedPeriod
// ----------------------------------------------------------------------------

int32_t AudioDeviceWindowsWave::OutputSanityCheckAfterUnlockedPeriod() const
{
    if (m_hWaveOut == NULL)
    {
        LogMsg( LOG_ERROR,  "output state has been modified during unlocked period");
        return -1;
    }
    return 0;
}

// ----------------------------------------------------------------------------
//  EnumeratePlayoutDevices
// ----------------------------------------------------------------------------

int32_t AudioDeviceWindowsWave::EnumeratePlayoutDevices()
{

    uint16_t nDevices(PlayoutDevices());
    LogMsg( LOG_INFO,  "===============================================================");
    LogMsg( LOG_INFO,  "#output devices: %u", nDevices);

    WAVEOUTCAPS caps;
    MMRESULT res;

    for (UINT deviceID = 0; deviceID < nDevices; deviceID++)
    {
        res = waveOutGetDevCaps(deviceID, &caps, sizeof(WAVEOUTCAPS));
        if (res != MMSYSERR_NOERROR)
        {
            LogMsg( LOG_ERROR,  "waveOutGetDevCaps() failed (err=%d)", res);
        }

        LogMsg( LOG_INFO,  "===============================================================");
        LogMsg( LOG_INFO,  "Device ID %u:", deviceID);
        LogMsg( LOG_INFO,  "manufacturer ID      : %u", caps.wMid);
        LogMsg( LOG_INFO,  "product ID           : %u",caps.wPid);
        LogMsg( LOG_INFO,  "version of driver    : %u.%u", HIBYTE(caps.vDriverVersion), LOBYTE(caps.vDriverVersion));
        LogMsg( LOG_INFO,  "product name         : %s", caps.szPname);
        LogMsg( LOG_INFO,  "dwFormats            : 0x%x", caps.dwFormats);
        if (caps.dwFormats & WAVE_FORMAT_48S16)
        {
            LogMsg( LOG_INFO,  "  48kHz,stereo,16bit : SUPPORTED");
        }
        else
        {
                LogMsg( LOG_ERROR,  " 48kHz,stereo,16bit  : *NOT* SUPPORTED");
        }
        if (caps.dwFormats & WAVE_FORMAT_48M16)
        {
            LogMsg( LOG_INFO,  "  48kHz,mono,16bit   : SUPPORTED");
        }
        else
        {
                LogMsg( LOG_ERROR,  " 48kHz,mono,16bit    : *NOT* SUPPORTED");
        }
        LogMsg( LOG_INFO,  "wChannels            : %u", caps.wChannels);
        TraceSupportFlags(caps.dwSupport);
    }

    return 0;
}

// ----------------------------------------------------------------------------
//  EnumerateRecordingDevices
// ----------------------------------------------------------------------------

int32_t AudioDeviceWindowsWave::EnumerateRecordingDevices()
{

    uint16_t nDevices(RecordingDevices());
    LogMsg( LOG_INFO,  "===============================================================");
    LogMsg( LOG_INFO,  "#input devices: %u", nDevices);

    WAVEINCAPS caps;
    MMRESULT res;

    for (UINT deviceID = 0; deviceID < nDevices; deviceID++)
    {
        res = waveInGetDevCaps(deviceID, &caps, sizeof(WAVEINCAPS));
        if (res != MMSYSERR_NOERROR)
        {
            LogMsg( LOG_ERROR,  "waveInGetDevCaps() failed (err=%d)", res);
        }

        LogMsg( LOG_INFO,  "===============================================================");
        LogMsg( LOG_INFO,  "Device ID %u:", deviceID);
        LogMsg( LOG_INFO,  "manufacturer ID      : %u", caps.wMid);
        LogMsg( LOG_INFO,  "product ID           : %u",caps.wPid);
        LogMsg( LOG_INFO,  "version of driver    : %u.%u", HIBYTE(caps.vDriverVersion), LOBYTE(caps.vDriverVersion));
        LogMsg( LOG_INFO,  "product name         : %s", caps.szPname);
        LogMsg( LOG_INFO,  "dwFormats            : 0x%x", caps.dwFormats);
        if (caps.dwFormats & WAVE_FORMAT_48S16)
        {
            LogMsg( LOG_INFO,  "  48kHz,stereo,16bit : SUPPORTED");
        }
        else
        {
                LogMsg( LOG_ERROR,  " 48kHz,stereo,16bit  : *NOT* SUPPORTED");
        }
        if (caps.dwFormats & WAVE_FORMAT_48M16)
        {
            LogMsg( LOG_INFO,  "  48kHz,mono,16bit   : SUPPORTED");
        }
        else
        {
                LogMsg( LOG_ERROR,  " 48kHz,mono,16bit    : *NOT* SUPPORTED");
        }
        LogMsg( LOG_INFO,  "wChannels            : %u", caps.wChannels);
    }

    return 0;
}

// ----------------------------------------------------------------------------
//  TraceSupportFlags
// ----------------------------------------------------------------------------

void AudioDeviceWindowsWave::TraceSupportFlags(DWORD dwSupport) const
{
    TCHAR buf[256];

    StringCchPrintf(buf, 128, TEXT("support flags        : 0x%x "), dwSupport);

    if (dwSupport & WAVECAPS_PITCH)
    {
        // supports pitch control
        StringCchCat(buf, 256, TEXT("(PITCH)"));
    }
    if (dwSupport & WAVECAPS_PLAYBACKRATE)
    {
        // supports playback rate control
        StringCchCat(buf, 256, TEXT("(PLAYBACKRATE)"));
    }
    if (dwSupport & WAVECAPS_VOLUME)
    {
        // supports volume control
        StringCchCat(buf, 256, TEXT("(VOLUME)"));
    }
    if (dwSupport & WAVECAPS_LRVOLUME)
    {
        // supports separate left and right volume control
        StringCchCat(buf, 256, TEXT("(LRVOLUME)"));
    }
    if (dwSupport & WAVECAPS_SYNC)
    {
        // the driver is synchronous and will block while playing a buffer
        StringCchCat(buf, 256, TEXT("(SYNC)"));
    }
    if (dwSupport & WAVECAPS_SAMPLEACCURATE)
    {
        // returns sample-accurate position information
        StringCchCat(buf, 256, TEXT("(SAMPLEACCURATE)"));
    }

    LogMsg( LOG_INFO,  "%S", buf);
}

// ----------------------------------------------------------------------------
//  TraceWaveInError
// ----------------------------------------------------------------------------

void AudioDeviceWindowsWave::TraceWaveInError(MMRESULT error) const
{
    TCHAR buf[MAXERRORLENGTH];
    TCHAR msg[MAXERRORLENGTH];

    StringCchPrintf(buf, MAXERRORLENGTH, TEXT("Error details: "));
    waveInGetErrorText(error, msg, MAXERRORLENGTH);
    StringCchCat(buf, MAXERRORLENGTH, msg);
    LogMsg( LOG_INFO,  "%S", buf);
}

// ----------------------------------------------------------------------------
//  TraceWaveOutError
// ----------------------------------------------------------------------------

void AudioDeviceWindowsWave::TraceWaveOutError(MMRESULT error) const
{
    TCHAR buf[MAXERRORLENGTH];
    TCHAR msg[MAXERRORLENGTH];

    StringCchPrintf(buf, MAXERRORLENGTH, TEXT("Error details: "));
    waveOutGetErrorText(error, msg, MAXERRORLENGTH);
    StringCchCat(buf, MAXERRORLENGTH, msg);
    LogMsg( LOG_INFO,  "%S", buf);
}

// ----------------------------------------------------------------------------
//  PrepareStartPlayout
// ----------------------------------------------------------------------------

int32_t AudioDeviceWindowsWave::PrepareStartPlayout()
{

    CriticalSectionScoped lock(&m_critSect);

    if (m_hWaveOut == NULL)
    {
        return -1;
    }

    // A total of 30ms of data is immediately placed in the SC buffer
    //
    int8_t zeroVec[4*PLAY_BUF_SIZE_IN_SAMPLES];  // max allocation
    memset(zeroVec, 0, 4*PLAY_BUF_SIZE_IN_SAMPLES);

    {
        Write(zeroVec, PLAY_BUF_SIZE_IN_SAMPLES);
        Write(zeroVec, PLAY_BUF_SIZE_IN_SAMPLES);
        Write(zeroVec, PLAY_BUF_SIZE_IN_SAMPLES);
    }

    m_playAcc = 0;
    m_playWarning = 0;
    m_playError = 0;
    _dc_diffm_mean = 0;
    _dc_ym_prev = 0;
    _dcm_penaltym_counter = 20;
    _dcm_prevtime = 0;
    _dcm_prevplay = 0;

    return 0;
}

// ----------------------------------------------------------------------------
//  PrepareStartRecording
// ----------------------------------------------------------------------------

int32_t AudioDeviceWindowsWave::PrepareStartRecording()
{

    CriticalSectionScoped lock(&m_critSect);

    if (m_hWaveIn == NULL)
    {
        return -1;
    }

    m_playAcc = 0;
    m_recordedBytes = 0;
    m_recPutBackDelay = REC_PUT_BACK_DELAY;

    MMRESULT res;
    MMTIME mmtime;
    mmtime.wType = TIME_SAMPLES;

    res = waveInGetPosition(m_hWaveIn, &mmtime, sizeof(mmtime));
    if (MMSYSERR_NOERROR != res)
    {
        LogMsg( LOG_ERROR,  "waveInGetPosition(TIME_SAMPLES) failed (err=%d)", res);
        TraceWaveInError(res);
    }

    m_read_samples = mmtime.u.sample;
    m_read_samples_old = m_read_samples;
    m_rec_samples_old = mmtime.u.sample;
    _wrapCounter = 0;

    for (int n = 0; n < N_BUFFERS_IN; n++)
    {
        const uint8_t nBytesPerSample = 2*m_recChannels;

        // set up the input wave header
        m_waveHeaderIn[n].lpData          = reinterpret_cast<LPSTR>(&m_recBuffer[n]);
        m_waveHeaderIn[n].dwBufferLength  = nBytesPerSample * REC_BUF_SIZE_IN_SAMPLES;
        m_waveHeaderIn[n].dwFlags         = 0;
        m_waveHeaderIn[n].dwBytesRecorded = 0;
        m_waveHeaderIn[n].dwUser          = 0;

        memset(m_recBuffer[n], 0, nBytesPerSample * REC_BUF_SIZE_IN_SAMPLES);

        // prepare a buffer for waveform-audio input
        res = waveInPrepareHeader(m_hWaveIn, &m_waveHeaderIn[n], sizeof(WAVEHDR));
        if (MMSYSERR_NOERROR != res)
        {
            LogMsg( LOG_ERROR,  "waveInPrepareHeader(%d) failed (err=%d)", n, res);
            TraceWaveInError(res);
        }

        // send an input buffer to the given waveform-audio input device
        res = waveInAddBuffer(m_hWaveIn, &m_waveHeaderIn[n], sizeof(WAVEHDR));
        if (MMSYSERR_NOERROR != res)
        {
            LogMsg( LOG_ERROR,  "waveInAddBuffer(%d) failed (err=%d)", n, res);
            TraceWaveInError(res);
        }
    }

    // start input on the given waveform-audio input device
    res = waveInStart(m_hWaveIn);
    if (MMSYSERR_NOERROR != res)
    {
        LogMsg( LOG_ERROR,  "waveInStart() failed (err=%d)", res);
        TraceWaveInError(res);
    }

    return 0;
}

// ----------------------------------------------------------------------------
//  GetPlayoutBufferDelay
// ----------------------------------------------------------------------------

int32_t AudioDeviceWindowsWave::GetPlayoutBufferDelay(uint32_t& writtenSamples, uint32_t& playedSamples)
{
    int i;
    int ms_Header;
    long playedDifference;
    int msecInPlayoutBuffer(0);   // #milliseconds of audio in the playout buffer

    const uint16_t nSamplesPerMs = (uint16_t)(N_PLAY_SAMPLES_PER_SEC/1000);  // default is 8000/1000 = 8

    MMRESULT res;
    MMTIME mmtime;

    if (!m_playing)
    {
        playedSamples = 0;
        return (0);
    }

    // Retrieve the current playback position.
    //
    mmtime.wType = TIME_SAMPLES;  // number of waveform-audio samples
    res = waveOutGetPosition(m_hWaveOut, &mmtime, sizeof(mmtime));
    if (MMSYSERR_NOERROR != res)
    {
        LogMsg( LOG_ERROR,  "waveOutGetPosition() failed (err=%d)", res);
        TraceWaveOutError(res);
    }

    writtenSamples = m_writtenSamples;   // #samples written to the playout buffer
    playedSamples = mmtime.u.sample;    // current playout position in the playout buffer

    // derive remaining amount (in ms) of data in the playout buffer
    msecInPlayoutBuffer = ((writtenSamples - playedSamples)/nSamplesPerMs);

    playedDifference = (long) (m_playedSamplesOld - playedSamples);

    if (playedDifference > 64000)
    {
        // If the sound cards number-of-played-out-samples variable wraps around before
        // writtenm_sampels wraps around this needs to be adjusted. This can happen on
        // sound cards that uses less than 32 bits to keep track of number of played out
        // sampels. To avoid being fooled by sound cards that sometimes produces false
        // output we compare old value minus the new value with a large value. This is
        // neccessary because some SC:s produce an output like 153, 198, 175, 230 which
        // would trigger the wrap-around function if we didn't compare with a large value.
        // The value 64000 is chosen because 2^16=65536 so we allow wrap around at 16 bits.

        i = 31;
        while((m_playedSamplesOld <= (unsigned long)POW2(i)) && (i > 14)) {
            i--;
        }

        if((i < 31) && (i > 14)) {
            // Avoid adjusting when there is 32-bit wrap-around since that is
            // something necessary.
            //
            LogMsg( LOG_INFO, "msecleft() => wrap around occured: %d bits used by sound card)", (i+1));

            m_writtenSamples = m_writtenSamples - POW2(i + 1);
            writtenSamples = m_writtenSamples;
            msecInPlayoutBuffer = ((writtenSamples - playedSamples)/nSamplesPerMs);
        }
    }
    else if ((m_writtenSamplesOld > (unsigned long)POW2(31)) && (writtenSamples < 96000))
    {
        // Wrap around as expected after having used all 32 bits. (But we still
        // test if the wrap around happened earlier which it should not)

        i = 31;
        while (m_writtenSamplesOld <= (unsigned long)POW2(i)) {
            i--;
        }

        LogMsg( LOG_INFO, "  msecleft() (wrap around occured after having used all 32 bits)");

        m_writtenSamplesOld = writtenSamples;
        m_playedSamplesOld = playedSamples;
        msecInPlayoutBuffer = (int)((writtenSamples + POW2(i + 1) - playedSamples)/nSamplesPerMs);

    }
    else if ((writtenSamples < 96000) && (playedSamples > (unsigned long)POW2(31)))
    {
        // Wrap around has, as expected, happened for writtenm_sampels before
        // playedSampels so we have to adjust for this until also playedSampels
        // has had wrap around.

        LogMsg( LOG_INFO, "  msecleft() (wrap around occured: correction of output is done)");

        m_writtenSamplesOld = writtenSamples;
        m_playedSamplesOld = playedSamples;
        msecInPlayoutBuffer = (int)((writtenSamples + POW2(32) - playedSamples)/nSamplesPerMs);
    }

    m_writtenSamplesOld = writtenSamples;
    m_playedSamplesOld = playedSamples;


    // We use the following formula to track that playout works as it should
    // y=playedSamples/48 - timeGetTime();
    // y represent the clock drift between system clock and sound card clock - should be fairly stable
    // When the exponential mean value of diff(y) goes away from zero something is wrong
    // The exponential formula will accept 1% clock drift but not more
    // The driver error means that we will play to little audio and have a high negative clock drift
    // We kick in our alternative method when the clock drift reaches 20%

    int diff,y;
    int unsigned time =0;

    // If we have other problems that causes playout glitches
    // we don't want to switch playout method.
    // Check if playout buffer is extremely low, or if we haven't been able to
    // exectue our code in more than 40 ms

    time = timeGetTime();

    if ((msecInPlayoutBuffer < 20) || (time - _dcm_prevtime > 40))
    {
        _dcm_penaltym_counter = 100;
    }

    if ((playedSamples != 0))
    {
        y = playedSamples/48 - time;
        if ((_dc_ym_prev != 0) && (_dcm_penaltym_counter == 0))
        {
            diff = y - _dc_ym_prev;
            _dc_diffm_mean = (990*_dc_diffm_mean)/1000 + 10*diff;
        }
        _dc_ym_prev = y;
    }

    if (_dcm_penaltym_counter)
    {
        _dcm_penaltym_counter--;
    }

    if (_dc_diffm_mean < -200)
    {
        // Always reset the filter
        _dc_diffm_mean = 0;

        // Problem is detected. Switch delay method and set min buffer to 80.
        // Reset the filter and keep monitoring the filter output.
        // If issue is detected a second time, increase min buffer to 100.
        // If that does not help, we must modify this scheme further.

        m_useHeader++;
        if (m_useHeader == 1)
        {
            m_minPlayBufDelay = 80;
            m_playWarning = 1;   // only warn first time
            LogMsg( LOG_INFO, "Modification #1: m_useHeader = %d, m_minPlayBufDelay = %d", m_useHeader, m_minPlayBufDelay);
        }
        else if (m_useHeader == 2)
        {
            m_minPlayBufDelay = 100;   // add some more safety
            LogMsg( LOG_INFO, "Modification #2: m_useHeader = %d, m_minPlayBufDelay = %d", m_useHeader, m_minPlayBufDelay);
        }
        else
        {
            // This state should not be entered... (HA)
           LogMsg( LOG_INFO, "further actions are required!");
        }
        if (m_playWarning == 1)
        {
            LogMsg( LOG_ERROR, "pending playout warning exists");
        }

        m_playWarning = 1;  // triggers callback from module process thread
        LogMsg( LOG_ERROR, "kPlayoutWarning message posted: switching to alternative playout delay method");
    }

    _dcm_prevtime = time;
    _dcm_prevplay = playedSamples;

    // Try a very rough method of looking at how many buffers are still playing
    ms_Header = 0;
    for (i = 0; i < N_BUFFERS_OUT; i++) {
        if ((m_waveHeaderOut[i].dwFlags & WHDR_INQUEUE)!=0) {
            ms_Header += 10;
        }
    }

    if ((ms_Header-50) > msecInPlayoutBuffer) {
        // Test for cases when GetPosition appears to be screwed up (currently just log....)
        TCHAR infoStr[300];
        if (m_no_ofm_msecleft_warnings%20==0)
        {
            StringCchPrintf(infoStr, 300, TEXT("writtenSamples=%i, playedSamples=%i, msecInPlayoutBuffer=%i, ms_Header=%i"), writtenSamples, playedSamples, msecInPlayoutBuffer, ms_Header);
            LogMsg( LOG_ERROR, "%S", infoStr);
        }
        m_no_ofm_msecleft_warnings++;
    }

    // If this is true we have had a problem with the playout
    if (m_useHeader > 0)
    {
        return (ms_Header);
    }


    if (ms_Header < msecInPlayoutBuffer)
    {
        if (m_no_ofm_msecleft_warnings % 100 == 0)
        {
            TCHAR str[300];
            StringCchPrintf(str, 300, TEXT("m_no_ofm_msecleft_warnings=%i, msecInPlayoutBuffer=%i ms_Header=%i (minBuffer=%i buffersize=%i writtenSamples=%i playedSamples=%i)"),
                m_no_ofm_msecleft_warnings, msecInPlayoutBuffer, ms_Header, m_minPlayBufDelay, m_playBufDelay, writtenSamples, playedSamples);
            LogMsg( LOG_ERROR, "%S", str);
        }
        m_no_ofm_msecleft_warnings++;
        ms_Header -= 6; // Round off as we only have 10ms resolution + Header info is usually slightly delayed compared to GetPosition

        if (ms_Header < 0)
            ms_Header = 0;

        return (ms_Header);
    }
    else
    {
        return (msecInPlayoutBuffer);
    }
}

// ----------------------------------------------------------------------------
//  GetRecordingBufferDelay
// ----------------------------------------------------------------------------

int32_t AudioDeviceWindowsWave::GetRecordingBufferDelay(uint32_t& readSamples, uint32_t& recSamples)
{
    long recDifference;
    MMTIME mmtime;
    MMRESULT mmr;

    const uint16_t nSamplesPerMs = (uint16_t)(N_REC_SAMPLES_PER_SEC/1000);  // default is 48000/1000 = 48

    // Retrieve the current input position of the given waveform-audio input device
    //
    mmtime.wType = TIME_SAMPLES;
    mmr = waveInGetPosition(m_hWaveIn, &mmtime, sizeof(mmtime));
    if (MMSYSERR_NOERROR != mmr)
    {
        LogMsg( LOG_ERROR,  "waveInGetPosition() failed (err=%d)", mmr);
        TraceWaveInError(mmr);
    }

    readSamples = m_read_samples;    // updated for each full fram in RecProc()
    recSamples = mmtime.u.sample;   // remaining time in input queue (recorded but not read yet)

    recDifference = (long) (m_rec_samples_old - recSamples);

    if( recDifference > 64000) {
        LogMsg( LOG_INFO, "WRAP 1 (recDifference =%d)", recDifference);
        // If the sound cards number-of-recorded-samples variable wraps around before
        // readm_sampels wraps around this needs to be adjusted. This can happen on
        // sound cards that uses less than 32 bits to keep track of number of played out
        // sampels. To avoid being fooled by sound cards that sometimes produces false
        // output we compare old value minus the new value with a large value. This is
        // neccessary because some SC:s produce an output like 153, 198, 175, 230 which
        // would trigger the wrap-around function if we didn't compare with a large value.
        // The value 64000 is chosen because 2^16=65536 so we allow wrap around at 16 bits.
        //
        int i = 31;
        while((m_rec_samples_old <= (unsigned long)POW2(i)) && (i > 14))
            i--;

        if((i < 31) && (i > 14)) {
            // Avoid adjusting when there is 32-bit wrap-around since that is
            // somethying neccessary.
            //
            m_read_samples = m_read_samples - POW2(i + 1);
            readSamples = m_read_samples;
            _wrapCounter++;
        } else {
            LogMsg( LOG_INFO, "AEC (m_rec_samples_old %d recSamples %d)",m_rec_samples_old, recSamples);
        }
    }

    if((_wrapCounter>200)){
        // Do nothing, handled later
    }
    else if((m_rec_samples_old > (unsigned long)POW2(31)) && (recSamples < 96000)) {
        LogMsg( LOG_INFO, "WRAP 2 (m_rec_samples_old %d recSamples %d)",m_rec_samples_old, recSamples);
        // Wrap around as expected after having used all 32 bits.
        m_read_samples_old = readSamples;
        m_rec_samples_old = recSamples;
        _wrapCounter++;
        return (int)((recSamples + POW2(32) - readSamples)/nSamplesPerMs);


    } else if((recSamples < 96000) && (readSamples > (unsigned long)POW2(31))) {
        LogMsg( LOG_INFO, "WRAP 3 (readSamples %d recSamples %d)",readSamples, recSamples);
        // Wrap around has, as expected, happened for recm_sampels before
        // readSampels so we have to adjust for this until also readSampels
        // has had wrap around.
        m_read_samples_old = readSamples;
        m_rec_samples_old = recSamples;
        _wrapCounter++;
        return (int)((recSamples + POW2(32) - readSamples)/nSamplesPerMs);
    }

    m_read_samples_old = m_read_samples;
    m_rec_samples_old = recSamples;
    int res=(((int)m_rec_samples_old - (int)m_read_samples_old)/nSamplesPerMs);

    if((res > 2000)||(res < 0)||(_wrapCounter>200)){
        // Reset everything
        LogMsg( LOG_INFO, "msecm_read error (res %d wrapCounter %d)",res, _wrapCounter);
        MMTIME mmtime;
        mmtime.wType = TIME_SAMPLES;

        mmr=waveInGetPosition(m_hWaveIn, &mmtime, sizeof(mmtime));
        if (mmr != MMSYSERR_NOERROR) {
           LogMsg( LOG_INFO, "waveInGetPosition failed (mmr=%d)", mmr);
        }
        m_read_samples=mmtime.u.sample;
        m_read_samples_old=m_read_samples;
        m_rec_samples_old=mmtime.u.sample;

        // Guess a decent value
        res = 20;
    }

    _wrapCounter = 0;
    return res;
}

//================================================================================
//                                  Thread Methods
//================================================================================

// ----------------------------------------------------------------------------
//  ThreadFunc
// ----------------------------------------------------------------------------

bool AudioDeviceWindowsWave::ThreadFunc(void* pThis)
{
    return (static_cast<AudioDeviceWindowsWave*>(pThis)->ThreadProcess());
}

// ----------------------------------------------------------------------------
//  ThreadProcess
// ----------------------------------------------------------------------------

bool AudioDeviceWindowsWave::ThreadProcess()
{
    uint64_t time(0);
    uint32_t playDiff(0);
    uint32_t recDiff(0);

    LONGLONG playTime(0);
    LONGLONG recTime(0);

    switch (m_timeEvent.Wait(1000))
    {
    case kEventSignaled:
        break;
    case kEventError:
        LogMsg( LOG_ERROR,  "EventWrapper::Wait() failed => restarting timer");
        m_timeEvent.StopTimer();
        m_timeEvent.StartTimer(true, TIMER_PERIOD_MS);
        return true;
    case kEventTimeout:
        return true;
    }

    time = TickTime::MillisecondTimestamp();

    if (m_startPlay)
    {
        if (PrepareStartPlayout() == 0)
        {
            m_prevTimerCheckTime = time;
            m_prevPlayTime = time;
            m_startPlay = false;
            m_playing = true;
            m_playStartEvent.Set();
        }
    }

    if (m_startRec)
    {
        if (PrepareStartRecording() == 0)
        {
            m_prevTimerCheckTime = time;
            m_prevRecTime = time;
            m_prevRecByteCheckTime = time;
            m_startRec = false;
            m_recording = true;
            m_recStartEvent.Set();
        }
    }

    if (m_playing)
    {
        playDiff = time - m_prevPlayTime;
    }

    if (m_recording)
    {
        recDiff = time - m_prevRecTime;
    }

    if (m_playing || m_recording)
    {
        RestartTimerIfNeeded(time);
    }

    if (m_playing &&
        (playDiff > (uint32_t)(m_dTcheckPlayBufDelay - 1)) ||
        (playDiff < 0))
    {
        Lock();
        if (m_playing)
        {
            if (PlayProc(playTime) == -1)
            {
                LogMsg( LOG_ERROR,  "PlayProc() failed");
            }
            m_prevPlayTime = time;
            if (playTime != 0)
                m_playAcc += playTime;
        }
        UnLock();
    }

    if (m_playing && (playDiff > 12))
    {
        // It has been a long time since we were able to play out, try to
        // compensate by calling PlayProc again.
        //
        Lock();
        if (m_playing)
        {
            if (PlayProc(playTime))
            {
                LogMsg( LOG_ERROR,  "PlayProc() failed");
            }
            m_prevPlayTime = time;
            if (playTime != 0)
                m_playAcc += playTime;
        }
        UnLock();
    }

    if (m_recording &&
       (recDiff > REC_CHECK_TIME_PERIOD_MS) ||
       (recDiff < 0))
    {
        Lock();
        if (m_recording)
        {
            int32_t nRecordedBytes(0);
            uint16_t maxIter(10);

            // Deliver all availiable recorded buffers and update the CPU load measurement.
            // We use a while loop here to compensate for the fact that the multi-media timer
            // can sometimed enter a "bad state" after hibernation where the resolution is
            // reduced from ~1ms to ~10-15 ms.
            //
            while ((nRecordedBytes = RecProc(recTime)) > 0)
            {
                maxIter--;
                m_recordedBytes += nRecordedBytes;
                if (recTime && m_perfFreq.QuadPart)
                {
                    // Measure the average CPU load:
                    // This is a simplified expression where an exponential filter is used:
                    //   m_avgCPULoad = 0.99 * m_avgCPULoad + 0.01 * newCPU,
                    //   newCPU = (recTime+playAcc)/f is time in seconds
                    //   newCPU / 0.01 is the fraction of a 10 ms period
                    // The two 0.01 cancels each other.
                    // NOTE - assumes 10ms audio buffers.
                    //
                    m_avgCPULoad = (float)(m_avgCPULoad*.99 + (recTime+m_playAcc)/(double)(m_perfFreq.QuadPart));
                    m_playAcc = 0;
                }
                if (maxIter == 0)
                {
                    // If we get this message ofte, our compensation scheme is not sufficient.
                    LogMsg( LOG_INFO,  "failed to compensate for reduced MM-timer resolution");
                }
            }

            if (nRecordedBytes == -1)
            {
                LogMsg( LOG_ERROR,  "RecProc() failed");
            }

            m_prevRecTime = time;

            // Monitor the recording process and generate error/warning callbacks if needed
            MonitorRecording(time);
        }

        UnLock();
    }

    if (!m_recording)
    {
        m_prevRecByteCheckTime = time;
        m_avgCPULoad = 0;
    }

    return true;
}

// ----------------------------------------------------------------------------
//  RecProc
// ----------------------------------------------------------------------------

int32_t AudioDeviceWindowsWave::RecProc(LONGLONG& consumedTime)
{
    MMRESULT res;
    uint32_t bufCount(0);
    uint32_t nBytesRecorded(0);

    consumedTime = 0;

    // count modulo N_BUFFERS_IN (0,1,2,...,(N_BUFFERS_IN-1),0,1,2,..)
    if( m_recBufCount == N_BUFFERS_IN )
    {
        m_recBufCount = 0;
    }

    bufCount = m_recBufCount;

    // take mono/stereo mode into account when deriving size of a full buffer
    const uint16_t bytesPerSample = 2*m_recChannels;
    const uint32_t fullBufferSizeInBytes = bytesPerSample * REC_BUF_SIZE_IN_SAMPLES;

    // read number of recorded bytes for the given input-buffer
    nBytesRecorded = m_waveHeaderIn[bufCount].dwBytesRecorded;

    if (nBytesRecorded == fullBufferSizeInBytes ||
       (nBytesRecorded > 0))
    {
        int32_t msecOnPlaySide;
        int32_t msecOnRecordSide;
        uint32_t writtenSamples;
        uint32_t playedSamples;
        uint32_t readSamples, recSamples;
        bool send = true;

        uint32_t nSamplesRecorded = (nBytesRecorded/bytesPerSample);  // divide by 2 or 4 depending on mono or stereo

        if (nBytesRecorded == fullBufferSizeInBytes)
        {
            m_timesdwBytes = 0;
        }
        else
        {
            // Test if it is stuck on this buffer
            m_timesdwBytes++;
            if (m_timesdwBytes < 5)
            {
                // keep trying
                return (0);
            }
            else
            {
                LogMsg( LOG_INFO, "nBytesRecorded=%d => don't use", nBytesRecorded);
                m_timesdwBytes = 0;
                send = false;
            }
        }

        // store the recorded buffer (no action will be taken if the #recorded samples is not a full buffer)
        m_ptrAudioBuffer->SetRecordedBuffer(m_waveHeaderIn[bufCount].lpData, nSamplesRecorded);

        // update #samples read
        m_read_samples += nSamplesRecorded;

        // Check how large the playout and recording buffers are on the sound card.
        // This info is needed by the AEC.
        //
        msecOnPlaySide = GetPlayoutBufferDelay(writtenSamples, playedSamples);
        msecOnRecordSide = GetRecordingBufferDelay(readSamples, recSamples);

        // If we use the alternative playout delay method, skip the clock drift compensation
        // since it will be an unreliable estimate and might degrade AEC performance.
        int32_t drift = (m_useHeader > 0) ? 0 : GetClockDrift(playedSamples, recSamples);

        m_ptrAudioBuffer->SetVQEData(msecOnPlaySide, msecOnRecordSide, drift);

        m_ptrAudioBuffer->SetTypingStatus(KeyPressed());

        // Store the play and rec delay values for video synchronization
        m_sndCardPlayDelay = msecOnPlaySide;
        m_sndCardRecDelay = msecOnRecordSide;

        LARGE_INTEGER t1={0},t2={0};

        if (send)
        {
            QueryPerformanceCounter(&t1);

            // deliver recorded samples at specified sample rate, mic level etc. to the observer using callback
            UnLock();
            m_ptrAudioBuffer->DeliverRecordedData();
            Lock();

            QueryPerformanceCounter(&t2);

            if (InputSanityCheckAfterUnlockedPeriod() == -1)
            {
                // assert(false);
                return -1;
            }
        }

        if (m_AGC)
        {
            uint32_t  newMicLevel = m_ptrAudioBuffer->NewMicLevel();
            if (newMicLevel != 0)
            {
                // The VQE will only deliver non-zero microphone levels when a change is needed.
                LogMsg( LOG_INFO, "AGC change of volume: => new=%u", newMicLevel);

                // We store this outside of the audio buffer to avoid
                // having it overwritten by the getter thread.
                m_newMicLevel = newMicLevel;
                SetEvent(m_hSetCaptureVolumeEvent);
            }
        }

        // return utilized buffer to queue after specified delay (default is 4)
        if (m_recDelayCount > (m_recPutBackDelay-1))
        {
            // delay buffer counter to compensate for "put-back-delay"
            bufCount = (bufCount + N_BUFFERS_IN - m_recPutBackDelay) % N_BUFFERS_IN;

            // reset counter so we can make new detection
            m_waveHeaderIn[bufCount].dwBytesRecorded = 0;

            // return the utilized wave-header after certain delay (given by m_recPutBackDelay)
            res = waveInUnprepareHeader(m_hWaveIn, &(m_waveHeaderIn[bufCount]), sizeof(WAVEHDR));
            if (MMSYSERR_NOERROR != res)
            {
                LogMsg( LOG_ERROR,  "waveInUnprepareHeader(%d) failed (err=%d)", bufCount, res);
                TraceWaveInError(res);
            }

            // ensure that the utilized header can be used again
            res = waveInPrepareHeader(m_hWaveIn, &(m_waveHeaderIn[bufCount]), sizeof(WAVEHDR));
            if (res != MMSYSERR_NOERROR)
            {
                LogMsg( LOG_ERROR,  "waveInPrepareHeader(%d) failed (err=%d)", bufCount, res);
                TraceWaveInError(res);
                return -1;
            }

            // add the utilized buffer to the queue again
            res = waveInAddBuffer(m_hWaveIn, &(m_waveHeaderIn[bufCount]), sizeof(WAVEHDR));
            if (res != MMSYSERR_NOERROR)
            {
                LogMsg( LOG_ERROR,  "waveInAddBuffer(%d) failed (err=%d)", bufCount, res);
                TraceWaveInError(res);
                if (m_recPutBackDelay < 50)
                {
                    m_recPutBackDelay++;
                    LogMsg( LOG_ERROR,  "m_recPutBackDelay increased to %d", m_recPutBackDelay);
                }
                else
                {
                    if (m_recError == 1)
                    {
                        LogMsg( LOG_ERROR, "pending recording error exists");
                    }
                    m_recError = 1;  // triggers callback from module process thread
                    LogMsg( LOG_ERROR, "kRecordingError message posted: m_recPutBackDelay=%u", m_recPutBackDelay);
                }
            }
        }  // if (m_recDelayCount > (m_recPutBackDelay-1))

        if (m_recDelayCount < (m_recPutBackDelay+1))
        {
            m_recDelayCount++;
        }

        // increase main buffer count since one complete buffer has now been delivered
        m_recBufCount++;

        if (send) {
            // Calculate processing time
            consumedTime = (int)(t2.QuadPart-t1.QuadPart);
            // handle wraps, time should not be higher than a second
            if ((consumedTime > m_perfFreq.QuadPart) || (consumedTime < 0))
                consumedTime = 0;
        }

    }  // if ((nBytesRecorded == fullBufferSizeInBytes))

    return nBytesRecorded;
}

// ----------------------------------------------------------------------------
//  PlayProc
// ----------------------------------------------------------------------------
//============================================================================
int AudioDeviceWindowsWave::PlayProc(LONGLONG& consumedTime)
{
    int32_t remTimeMS(0);
    int8_t playBuffer[4*PLAY_BUF_SIZE_IN_SAMPLES];
    uint32_t writtenSamples(0);
    uint32_t playedSamples(0);

    LARGE_INTEGER t1;
    LARGE_INTEGER t2;

    consumedTime = 0;
    m_waitCounter++;

    // Get number of ms of sound that remains in the sound card buffer for playback.
    //
    remTimeMS = GetPlayoutBufferDelay(writtenSamples, playedSamples);

    // The threshold can be adaptive or fixed. The adaptive scheme is updated
    // also for fixed mode but the updated threshold is not utilized.
    //
    const uint16_t thresholdMS = (m_playBufType == AudioDeviceModule::kAdaptiveBufferSize) ? m_playBufDelay : m_playBufDelayFixed;

    if (remTimeMS < thresholdMS + 9)
    {
        m_dTcheckPlayBufDelay = 5;

        if (remTimeMS == 0)
        {
            LogMsg( LOG_INFO,  "playout buffer is empty => we must adapt...");
            if (m_waitCounter > 30)
            {
                m_erZeroCounter++;
                if (m_erZeroCounter == 2)
                {
                    m_playBufDelay += 15;
                    m_minPlayBufDelay += 20;
                    m_waitCounter = 50;
                    LogMsg( LOG_INFO, "New playout states (er=0,erZero=2): minPlayBufDelay=%u, playBufDelay=%u", m_minPlayBufDelay, m_playBufDelay);
                }
                else if (m_erZeroCounter == 3)
                {
                    m_erZeroCounter = 0;
                    m_playBufDelay += 30;
                    m_minPlayBufDelay += 25;
                    m_waitCounter = 0;
                    LogMsg( LOG_INFO, "New playout states (er=0, erZero=3): minPlayBufDelay=%u, playBufDelay=%u", m_minPlayBufDelay, m_playBufDelay);
                }
                else
                {
                    m_minPlayBufDelay += 10;
                    m_playBufDelay += 15;
                    m_waitCounter = 50;
                    LogMsg( LOG_INFO, "New playout states (er=0, erZero=1): minPlayBufDelay=%u, playBufDelay=%u", m_minPlayBufDelay, m_playBufDelay);
                }
            }
        }
        else if (remTimeMS < m_minPlayBufDelay)
        {
            // If there is less than 25 ms of audio in the play out buffer
            // increase the buffersize limit value. m_waitCounter prevents
            // m_playBufDelay to be increased every time this function is called.

            if (m_waitCounter > 30)
            {
                m_playBufDelay += 10;
                if (m_intro == 0)
                    m_waitCounter = 0;
                LogMsg( LOG_INFO, "Playout threshold is increased: playBufDelay=%u", m_playBufDelay);
            }
        }
        else if (remTimeMS < thresholdMS - 9)
        {
            m_erZeroCounter = 0;
        }
        else
        {
            m_erZeroCounter = 0;
            m_dTcheckPlayBufDelay = 10;
        }

        QueryPerformanceCounter(&t1);   // measure time: START

        // Ask for new PCM data to be played out using the AudioDeviceBuffer.
        // Ensure that this callback is executed without taking the audio-thread lock.
        //
        UnLock();
        uint32_t nSamples = m_ptrAudioBuffer->RequestPlayoutData(PLAY_BUF_SIZE_IN_SAMPLES);
        Lock();

        if (OutputSanityCheckAfterUnlockedPeriod() == -1)
        {
            // assert(false);
            return -1;
        }

        nSamples = m_ptrAudioBuffer->GetPlayoutData(playBuffer);
        if (nSamples != PLAY_BUF_SIZE_IN_SAMPLES)
        {
            LogMsg( LOG_ERROR, "invalid number of output samples(%d)", nSamples);
        }

        QueryPerformanceCounter(&t2);   // measure time: STOP
        consumedTime = (int)(t2.QuadPart - t1.QuadPart);

        Write( playBuffer, PLAY_BUF_SIZE_IN_SAMPLES );

    }  // if (er < thresholdMS + 9)
    else if (thresholdMS + 9 < remTimeMS )
    {
        m_erZeroCounter = 0;
        m_dTcheckPlayBufDelay = 2;    // check buffer more often
        LogMsg( LOG_INFO, "Need to check playout buffer more often (dT=%u, remTimeMS=%u)", m_dTcheckPlayBufDelay, remTimeMS);
    }

    // If the buffersize has been stable for 20 seconds try to decrease the buffer size
    if (m_waitCounter > 2000)
    {
        m_intro = 0;
        m_playBufDelay--;
        m_waitCounter = 1990;
        LogMsg( LOG_INFO, "Playout threshold is decreased: playBufDelay=%u", m_playBufDelay);
    }

    // Limit the minimum sound card (playback) delay to adaptive minimum delay
    if (m_playBufDelay < m_minPlayBufDelay)
    {
        m_playBufDelay = m_minPlayBufDelay;
        LogMsg( LOG_INFO, "Playout threshold is limited to %u", m_minPlayBufDelay);
    }

    // Limit the maximum sound card (playback) delay to 150 ms
    if (m_playBufDelay > 150)
    {
        m_playBufDelay = 150;
        LogMsg( LOG_INFO, "Playout threshold is limited to %d", m_playBufDelay);
    }

    // Upper limit of the minimum sound card (playback) delay to 65 ms.
    // Deactivated during "useHeader mode" (m_useHeader > 0).
    if (m_minPlayBufDelay > m_MAXm_minBuffer &&
       (m_useHeader == 0))
    {
        m_minPlayBufDelay = m_MAXm_minBuffer;
        LogMsg( LOG_INFO, "Minimum playout threshold is limited to %d", m_MAXm_minBuffer);
    }

    return (0);
}

// ----------------------------------------------------------------------------
//  Write
// ----------------------------------------------------------------------------

int32_t AudioDeviceWindowsWave::Write(int8_t* data, uint16_t nSamples)
{
    if (m_hWaveOut == NULL)
    {
        return -1;
    }

    if (m_playIsInitialized)
    {
        MMRESULT res;

        const uint16_t bufCount(m_playBufCount);

        // Place data in the memory associated with m_waveHeaderOut[bufCount]
        //
        const int16_t nBytes = (2*m_playChannels)*nSamples;
        memcpy(&m_playBuffer[bufCount][0], &data[0], nBytes);

        // Send a data block to the given waveform-audio output device.
        //
        // When the buffer is finished, the WHDR_DONE bit is set in the dwFlags
        // member of the WAVEHDR structure. The buffer must be prepared with the
        // waveOutPrepareHeader function before it is passed to waveOutWrite.
        // Unless the device is paused by calling the waveOutPause function,
        // playback begins when the first data block is sent to the device.
        //
        res = waveOutWrite(m_hWaveOut, &m_waveHeaderOut[bufCount], sizeof(m_waveHeaderOut[bufCount]));
        if (MMSYSERR_NOERROR != res)
        {
            LogMsg( LOG_ERROR,  "waveOutWrite(%d) failed (err=%d)", bufCount, res);
            TraceWaveOutError(res);

            m_writeErrors++;
            if (m_writeErrors > 10)
            {
                if (m_playError == 1)
                {
                    LogMsg( LOG_ERROR, "pending playout error exists");
                }
                m_playError = 1;  // triggers callback from module process thread
                LogMsg( LOG_ERROR, "kPlayoutError message posted: m_writeErrors=%u", m_writeErrors);
            }

            return -1;
        }

        m_playBufCount = (m_playBufCount+1) % N_BUFFERS_OUT;  // increase buffer counter modulo size of total buffer
        m_writtenSamples += nSamples;                        // each sample is 2 or 4 bytes
        m_writeErrors = 0;
    }

    return 0;
}

// ----------------------------------------------------------------------------
//    GetClockDrift
// ----------------------------------------------------------------------------
//============================================================================
int32_t AudioDeviceWindowsWave::GetClockDrift(const uint32_t plSamp, const uint32_t rcSamp)
{
    int drift = 0;
    unsigned int plSampDiff = 0, rcSampDiff = 0;

    if (plSamp >= m_plSampOld)
    {
        plSampDiff = plSamp - m_plSampOld;
    }
    else
    {
        // Wrap
        int i = 31;
        while(m_plSampOld <= (unsigned int)POW2(i))
        {
            i--;
        }

        // Add the amount remaining prior to wrapping
        plSampDiff = plSamp +  POW2(i + 1) - m_plSampOld;
    }

    if (rcSamp >= m_rcSampOld)
    {
        rcSampDiff = rcSamp - m_rcSampOld;
    }
    else
    {   // Wrap
        int i = 31;
        while(m_rcSampOld <= (unsigned int)POW2(i))
        {
            i--;
        }

        rcSampDiff = rcSamp +  POW2(i + 1) - m_rcSampOld;
    }

    drift = plSampDiff - rcSampDiff;

    m_plSampOld = plSamp;
    m_rcSampOld = rcSamp;

    return drift;
}

// ----------------------------------------------------------------------------
//  MonitorRecording
// ----------------------------------------------------------------------------

int32_t AudioDeviceWindowsWave::MonitorRecording(const uint32_t time)
{
    const uint16_t bytesPerSample = 2*m_recChannels;
    const uint32_t nRecordedSamples = m_recordedBytes/bytesPerSample;

    if (nRecordedSamples > 5*N_REC_SAMPLES_PER_SEC)
    {
        // 5 seconds of audio has been recorded...
        if ((time - m_prevRecByteCheckTime) > 5700)
        {
            // ...and it was more than 5.7 seconds since we last did this check <=>
            // we have not been able to record 5 seconds of audio in 5.7 seconds,
            // hence a problem should be reported.
            // This problem can be related to USB overload.
            //
            if (m_recWarning == 1)
            {
                LogMsg( LOG_ERROR, "pending recording warning exists");
            }
            m_recWarning = 1;  // triggers callback from module process thread
            LogMsg( LOG_ERROR, "kRecordingWarning message posted: time-m_prevRecByteCheckTime=%d", time - m_prevRecByteCheckTime);
        }

        m_recordedBytes = 0;            // restart "check again when 5 seconds are recorded"
        m_prevRecByteCheckTime = time;  // reset timer to measure time for recording of 5 seconds
    }

    if ((time - m_prevRecByteCheckTime) > 8000)
    {
        // It has been more than 8 seconds since we able to confirm that 5 seconds of
        // audio was recorded, hence we have not been able to record 5 seconds in
        // 8 seconds => the complete recording process is most likely dead.
        //
        if (m_recError == 1)
        {
            LogMsg( LOG_ERROR, "pending recording error exists");
        }
        m_recError = 1;  // triggers callback from module process thread
        LogMsg( LOG_ERROR, "kRecordingError message posted: time-m_prevRecByteCheckTime=%d", time - m_prevRecByteCheckTime);

        m_prevRecByteCheckTime = time;
    }

    return 0;
}

// ----------------------------------------------------------------------------
//  MonitorRecording
//
//  Restart timer if needed (they seem to be messed up after a hibernate).
// ----------------------------------------------------------------------------

int32_t AudioDeviceWindowsWave::RestartTimerIfNeeded(const uint32_t time)
{
    const uint32_t diffMS = time - m_prevTimerCheckTime;
    m_prevTimerCheckTime = time;

    if (diffMS > 7)
    {
        // one timer-issue detected...
        m_timerFaults++;
        if (m_timerFaults > 5 && m_timerRestartAttempts < 2)
        {
            // Reinitialize timer event if event fails to execute at least every 5ms.
            // On some machines it helps and the timer starts working as it should again;
            // however, not all machines (we have seen issues on e.g. IBM T60).
            // Therefore, the scheme below ensures that we do max 2 attempts to restart the timer.
            // For the cases where restart does not do the trick, we compensate for the reduced
            // resolution on both the recording and playout sides.
            LogMsg( LOG_ERROR, " timer issue detected => timer is restarted");
            m_timeEvent.StopTimer();
            m_timeEvent.StartTimer(true, TIMER_PERIOD_MS);
            // make sure timer gets time to start up and we don't kill/start timer serveral times over and over again
            m_timerFaults = -20;
            m_timerRestartAttempts++;
        }
    }
    else
    {
        // restart timer-check scheme since we are OK
        m_timerFaults = 0;
        m_timerRestartAttempts = 0;
    }

    return 0;
}


bool AudioDeviceWindowsWave::KeyPressed() const{

  int key_down = 0;
  for (int key = VK_SPACE; key < VK_NUMLOCK; key++) {
    short res = GetAsyncKeyState(key);
    key_down |= res & 0x1; // Get the LSB
  }
  return (key_down > 0);
}
}  // namespace webrtc

#endif // TARGET_OS_WINDOWS
