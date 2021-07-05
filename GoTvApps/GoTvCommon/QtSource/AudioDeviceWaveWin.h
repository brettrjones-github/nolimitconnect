/*
 *  Copyright (c) 2012 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef WEBRTC_AUDIO_DEVICE_AUDIO_DEVICE_WAVE_WIN_H
#define WEBRTC_AUDIO_DEVICE_AUDIO_DEVICE_WAVE_WIN_H

#include <memory>

#include <ptop_src/ptop_engine_src/EchoCancel/platform_thread.h>
#include <ptop_src/ptop_engine_src/EchoCancel/audio_device_generic.h>
#include "AudioMixerMgrWin.h"

#pragma comment( lib, "winmm.lib" )

namespace webrtc {
class EventTimerWrapper;
class EventWrapper;

const uint32_t					TIMER_PERIOD_MS				= 2;
const uint32_t					REC_CHECK_TIME_PERIOD_MS	= 4;
const uint16_t					REC_PUT_BACK_DELAY			= 4;

const uint32_t					N_REC_SAMPLES_PER_SEC		= 8000;
const uint32_t					N_PLAY_SAMPLES_PER_SEC		= 8000;

const uint32_t					N_REC_CHANNELS				= 1;  // default is mono recording
const uint32_t					N_PLAY_CHANNELS				= 1;


// NOTE - CPU load will not be correct for other sizes than 10ms
const uint32_t					REC_BUF_SIZE_IN_SAMPLES		= (N_REC_SAMPLES_PER_SEC/100);
const uint32_t					PLAY_BUF_SIZE_IN_SAMPLES	= (N_PLAY_SAMPLES_PER_SEC/100);

enum { N_BUFFERS_IN = 200 };
enum { N_BUFFERS_OUT = 200 };

class AudioDeviceWindowsWave : public AudioDeviceGeneric
{
public:
    AudioDeviceWindowsWave(const int32_t id);
    ~AudioDeviceWindowsWave();

    // Retrieve the currently utilized audio layer
    virtual int32_t				ActiveAudioLayer(AudioDeviceModule::AudioLayer& audioLayer) const;

    // Main initialization and termination
    virtual int32_t				Init();
    virtual int32_t				Terminate();
    virtual bool				Initialized() const;

    // Device enumeration
    virtual int16_t				PlayoutDevices();
    virtual int16_t				RecordingDevices();
    virtual int32_t				PlayoutDeviceName(	uint16_t index,
													char name[kAdmMaxDeviceNameSize],
													char guid[kAdmMaxGuidSize] );
    virtual int32_t				RecordingDeviceName(	uint16_t index,
														char name[kAdmMaxDeviceNameSize],
														char guid[kAdmMaxGuidSize] );

    // Device selection
    virtual int32_t				SetPlayoutDevice(uint16_t index);
    virtual int32_t				SetPlayoutDevice(AudioDeviceModule::WindowsDeviceType device);
    virtual int32_t				SetRecordingDevice(uint16_t index);
    virtual int32_t				SetRecordingDevice(AudioDeviceModule::WindowsDeviceType device);

    // Audio transport initialization
    virtual int32_t				PlayoutIsAvailable(bool& available);
    virtual int32_t				InitPlayout();
    virtual bool				PlayoutIsInitialized() const;
    virtual int32_t				RecordingIsAvailable(bool& available);
    virtual int32_t				InitRecording();
    virtual bool				RecordingIsInitialized() const;

    // Audio transport control
    virtual int32_t				StartPlayout();
    virtual int32_t				StopPlayout();
    virtual bool				Playing() const;
    virtual int32_t				StartRecording();
    virtual int32_t				StopRecording();
    virtual bool				Recording() const;

    // Microphone Automatic Gain Control (AGC)
    virtual int32_t				SetAGC(bool enable);
    virtual bool				AGC() const;

    // Volume control based on the Windows Wave API (Windows only)
    virtual int32_t				SetWaveOutVolume(uint16_t volumeLeft, uint16_t volumeRight);
    virtual int32_t				WaveOutVolume(uint16_t& volumeLeft, uint16_t& volumeRight) const;

    // Audio mixer initialization
    virtual int32_t				InitSpeaker();
    virtual bool				SpeakerIsInitialized() const;
    virtual int32_t				InitMicrophone();
    virtual bool				MicrophoneIsInitialized() const;

    // Speaker volume controls
    virtual int32_t				SpeakerVolumeIsAvailable(bool& available);
    virtual int32_t				SetSpeakerVolume(uint32_t volume);
    virtual int32_t				SpeakerVolume(uint32_t& volume) const;
    virtual int32_t				MaxSpeakerVolume(uint32_t& maxVolume) const;
    virtual int32_t				MinSpeakerVolume(uint32_t& minVolume) const;
    virtual int32_t				SpeakerVolumeStepSize(uint16_t& stepSize) const;

    // Microphone volume controls
    virtual int32_t				MicrophoneVolumeIsAvailable(bool& available);
    virtual int32_t				SetMicrophoneVolume(uint32_t volume);
    virtual int32_t				MicrophoneVolume(uint32_t& volume) const;
    virtual int32_t				MaxMicrophoneVolume(uint32_t& maxVolume) const;
    virtual int32_t				MinMicrophoneVolume(uint32_t& minVolume) const;
    virtual int32_t				MicrophoneVolumeStepSize(uint16_t& stepSize) const;

    // Speaker mute control
    virtual int32_t				SpeakerMuteIsAvailable(bool& available);
    virtual int32_t				SetSpeakerMute(bool enable);
    virtual int32_t				SpeakerMute(bool& enabled) const;

    // Microphone mute control
    virtual int32_t				MicrophoneMuteIsAvailable(bool& available);
    virtual int32_t				SetMicrophoneMute(bool enable);
    virtual int32_t				MicrophoneMute(bool& enabled) const;

    // Microphone boost control
    virtual int32_t				MicrophoneBoostIsAvailable(bool& available);
    virtual int32_t				SetMicrophoneBoost(bool enable);
    virtual int32_t				 MicrophoneBoost(bool& enabled) const;

    // Stereo support
    virtual int32_t				StereoPlayoutIsAvailable(bool& available);
    virtual int32_t				SetStereoPlayout(bool enable);
    virtual int32_t				StereoPlayout(bool& enabled) const;
    virtual int32_t				StereoRecordingIsAvailable(bool& available);
    virtual int32_t				SetStereoRecording(bool enable);
    virtual int32_t				StereoRecording(bool& enabled) const;

    // Delay information and control
    virtual int32_t				SetPlayoutBuffer(const AudioDeviceModule::BufferType type, uint16_t sizeMS);
    virtual int32_t				PlayoutBuffer(AudioDeviceModule::BufferType& type, uint16_t& sizeMS) const;
    virtual int32_t				PlayoutDelay(uint16_t& delayMS) const;
    virtual int32_t				RecordingDelay(uint16_t& delayMS) const;

    // CPU load
    virtual int32_t				CPULoad(uint16_t& load) const;

public:
    virtual bool				PlayoutWarning() const;
    virtual bool				PlayoutError() const;
    virtual bool				RecordingWarning() const;
    virtual bool				RecordingError() const;
    virtual void				ClearPlayoutWarning();
    virtual void				ClearPlayoutError();
    virtual void				ClearRecordingWarning();
    virtual void				ClearRecordingError();

public:
    virtual void				AttachAudioBuffer(AudioDeviceBuffer* audioBuffer);

private:
    void						Lock() { m_critSect.Enter(); };
    void						UnLock() { m_critSect.Leave(); };
    int32_t						Id() {return m_id;}
    bool						IsUsingOutputDeviceIndex() const {return m_usingOutputDeviceIndex;}
    AudioDeviceModule::WindowsDeviceType OutputDevice() const {return m_outputDevice;}
    uint16_t					OutputDeviceIndex() const {return m_outputDeviceIndex;}
    bool						IsUsingInputDeviceIndex() const {return m_usingInputDeviceIndex;}
    AudioDeviceModule::WindowsDeviceType InputDevice() const {return m_inputDevice;}
    uint16_t					InputDeviceIndex() const {return m_inputDeviceIndex;}

private:
    GOTV_INLINE int32_t				InputSanityCheckAfterUnlockedPeriod() const;
    GOTV_INLINE int32_t				OutputSanityCheckAfterUnlockedPeriod() const;

private:
    bool						KeyPressed() const;

private:
    int32_t						EnumeratePlayoutDevices();
    int32_t						EnumerateRecordingDevices();
    void						TraceSupportFlags(DWORD dwSupport) const;
    void						TraceWaveInError(MMRESULT error) const;
    void						TraceWaveOutError(MMRESULT error) const;
    int32_t						PrepareStartRecording();
    int32_t						PrepareStartPlayout();

    int32_t						RecProc(LONGLONG& consumedTime);
    int							PlayProc(LONGLONG& consumedTime);

    int32_t						GetPlayoutBufferDelay(uint32_t& writtenSamples, uint32_t& playedSamples);
    int32_t						GetRecordingBufferDelay(uint32_t& readSamples, uint32_t& recSamples);
    int32_t						Write(int8_t* data, uint16_t nSamples);
    int32_t						GetClockDrift(const uint32_t plSamp, const uint32_t rcSamp);
    int32_t						MonitorRecording(const uint32_t time);
    int32_t						RestartTimerIfNeeded(const uint32_t time);

private:
    static bool					ThreadFunc(void*);
    bool						ThreadProcess();

    static DWORD WINAPI			GetCaptureVolumeThread(LPVOID context);
    DWORD						DoGetCaptureVolumeThread();

    static DWORD WINAPI			SetCaptureVolumeThread(LPVOID context);
    DWORD						DoSetCaptureVolumeThread();

private:
    AudioDeviceBuffer*          m_ptrAudioBuffer;

    CriticalSectionWrapper&     m_critSect;
    EventTimerWrapper&          m_timeEvent;
    EventWrapper&               m_recStartEvent;
    EventWrapper&               m_playStartEvent;

    HANDLE                      m_hGetCaptureVolumeThread;
    HANDLE                      m_hShutdownGetVolumeEvent;
    HANDLE                      m_hSetCaptureVolumeThread;
    HANDLE                      m_hShutdownSetVolumeEvent;
    HANDLE                      m_hSetCaptureVolumeEvent;

   rtc::PlatformThread *		m_ptrThread;

    CriticalSectionWrapper&     m_critSectCb;

    int32_t                     m_id;

    AudioMixerManager           m_mixerManager;

    bool                        m_usingInputDeviceIndex;
    bool                        m_usingOutputDeviceIndex;
    AudioDeviceModule::WindowsDeviceType    m_inputDevice;
    AudioDeviceModule::WindowsDeviceType    m_outputDevice;
    uint16_t                    m_inputDeviceIndex;
    uint16_t                    m_outputDeviceIndex;
    bool                        m_inputDeviceIsSpecified;
    bool                        m_outputDeviceIsSpecified;

    WAVEFORMATEX                m_waveFormatIn;
    WAVEFORMATEX                m_waveFormatOut;

    HWAVEIN                     m_hWaveIn;
    HWAVEOUT                    m_hWaveOut;

    WAVEHDR                     m_waveHeaderIn[N_BUFFERS_IN];
    WAVEHDR                     m_waveHeaderOut[N_BUFFERS_OUT];

    uint8_t                     m_recChannels;
    uint8_t                     m_playChannels;
    uint16_t                    m_recBufCount;
    uint16_t                    m_recDelayCount;
    uint16_t                    m_recPutBackDelay;

    int8_t						m_recBuffer[ N_BUFFERS_IN ][ 4*REC_BUF_SIZE_IN_SAMPLES ];
    int8_t						m_playBuffer[ N_BUFFERS_OUT ][ 4*PLAY_BUF_SIZE_IN_SAMPLES ];

    AudioDeviceModule::BufferType m_playBufType;

private:
    bool                        m_initialized;
    bool                        m_recording;
    bool                        m_playing;
    bool                        m_recIsInitialized;
    bool                        m_playIsInitialized;
    bool                        m_startRec;
    bool                        m_stopRec;
    bool                        m_startPlay;
    bool                        m_stopPlay;
    bool                        m_AGC;

private:
    uint32_t                    m_prevPlayTime;
    uint32_t                    m_prevRecTime;
    uint32_t                    m_prevTimerCheckTime;

    uint16_t                    m_playBufCount;          // playout buffer index
    uint16_t                    m_dTcheckPlayBufDelay;   // dT for check of play buffer, {2,5,10} [ms]
    uint16_t                    m_playBufDelay;          // playback delay
    uint16_t                    m_playBufDelayFixed;     // fixed playback delay
    uint16_t                    m_minPlayBufDelay;       // minimum playback delay
    uint16_t                    m_MAXm_minBuffer;         // level of (adaptive) min threshold must be < m_MAXm_minBuffer

    int32_t                     m_erZeroCounter;         // counts "buffer-is-empty" events
    int32_t                     m_intro;
    int32_t                     m_waitCounter;

    uint32_t                    m_writtenSamples;
    uint32_t                    m_writtenSamplesOld;
    uint32_t                    m_playedSamplesOld;

    uint32_t                    m_sndCardPlayDelay;
    uint32_t                    m_sndCardRecDelay;

    uint32_t                    m_plSampOld;
    uint32_t                    m_rcSampOld;

    uint32_t                    m_read_samples;
    uint32_t                    m_read_samples_old;
    uint32_t                    m_rec_samples_old;

    // State that detects driver problems:
    int32_t                     _dc_diffm_mean;
    int32_t                     _dc_ym_prev;
    int32_t                     _dcm_penaltym_counter;
    int32_t                     _dcm_prevtime;
    uint32_t                    _dcm_prevplay;

    uint32_t                    m_recordedBytes;         // accumulated #recorded bytes (reset periodically)
    uint64_t                    m_prevRecByteCheckTime;  // time when we last checked the recording process

    // CPU load measurements
    LARGE_INTEGER               m_perfFreq;
    LONGLONG                    m_playAcc;               // accumulated time for playout callback
    float                       m_avgCPULoad;            // average total (rec+play) CPU load

    int32_t                     _wrapCounter;

    int32_t                     m_useHeader;
    int16_t                     m_timesdwBytes;
    int32_t                     m_no_ofm_msecleft_warnings;
    int32_t                     m_writeErrors;
    int32_t                     m_timerFaults;
    int32_t                     m_timerRestartAttempts;

    uint16_t                    m_playWarning;
    uint16_t                    m_playError;
    uint16_t                    m_recWarning;
    uint16_t                    m_recError;

    uint32_t                    m_newMicLevel;
    uint32_t                    m_minMicVolume;
    uint32_t                    m_maxMicVolume;
};

}  // namespace webrtc

#endif  // WEBRTC_AUDIO_DEVICE_AUDIO_DEVICE_WAVE_WIN_H
