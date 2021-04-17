/*
*  Copyright (c) 2015 The WebRTC project authors. All Rights Reserved.
*
*  Use of this source code is governed by a BSD-style license
*  that can be found in the LICENSE file in the root of the source
*  tree. An additional intellectual property rights grant can be found
*  in the file PATENTS.  All contributing project authors may
*  be found in the AUTHORS file in the root of the source tree.
*/

#ifdef TARGET_OS_ANDROID

#ifndef WEBRTC_MODULES_AUDIO_DEVICE_ANDROID_AUDIO_MANAGER_H_
#define WEBRTC_MODULES_AUDIO_DEVICE_ANDROID_AUDIO_MANAGER_H_


#include <memory>

#include <jni.h>

#include "audio_common.h"
#include <GoTvCore/GoTvP2P/EchoCancel/audio_device_config.h>
#include <GoTvCore/GoTvP2P/EchoCancel/audio_device_defines.h>
#include <GoTvCore/GoTvP2P/EchoCancel/audio_device_generic.h>

#include "audio_record_jni.h"
#include "audio_track_jni.h"


const uint32_t					MY_FRAME_DATA_LEN			= 1280;
const uint32_t					MY_FRAME_SAMPLES			= 640;

class AudioMgrJavaClass;
class EngineImp;
class P2PEngine;
class MediaProcessor;

namespace webrtc {

	// Implements support for functions in the WebRTC audio stack for Android that
	// relies on the AudioManager in android.media. It also populates an
	// AudioParameter structure with native audio parameters detected at
	// construction. This class does not make any audio-related modifications
	// unless Init() is called. Caching audio parameters makes no changes but only
	// reads data from the Java side.
	class BuildInfo;

	class AudioManager : public AudioTransport
	{
	public:
		AudioManager( EngineImp& engineImp );
		virtual ~AudioManager();

		void					toGuiWantMicrophoneRecording( bool wantMicInput );
		void					toGuiWantSpeakerOutput( bool wantSpeakerOutput );
		void					toGuiPlayAudio( int16_t * pu16PcmData, int pcmDataLenInBytes );

		bool					enableAudio( bool enableRecAndPlay );

		// Sets the currently active audio layer combination. Must be called before
		// Init().
		void					SetActiveAudioLayer( AudioDeviceModule::AudioLayer audio_layer );

		// Initializes the audio manager and stores the current audio mode.
		bool					initAndroidAudioMgr();
		// Revert any setting done by Init().
		bool					Close();

		// Returns true if current audio mode is AudioManager.MODE_IN_COMMUNICATION.
		bool					IsCommunicationModeEnabled() const;

		// Native audio parameters stored during construction.
		const AudioParameters&	GetPlayoutAudioParameters();
		const AudioParameters&	GetRecordAudioParameters();

		// Returns true if the device supports built-in audio effects for AEC, AGC
		// and NS. Some devices can also be blacklisted for use in combination with
		// platform effects and these devices will return false.
		// Can currently only be used in combination with a Java based audio backend
		// for the recording side (i.e. using the android.media.AudioRecord API).
		bool					IsAcousticEchoCancelerSupported() const;
		bool					IsAutomaticGainControlSupported() const;
		bool					IsNoiseSuppressorSupported() const;

		// Returns true if the device supports the low-latency audio paths in
		// combination with OpenSL ES.
		bool					IsLowLatencyPlayoutSupported() const;

		// Returns the estimated total delay of this device. Unit is in milliseconds.
		// The value is set once at construction and never changes after that.
		// Possible values are webrtc::kLowLatencyModeDelayEstimateInMilliseconds and
		// webrtc::kHighLatencyModeDelayEstimateInMilliseconds.
		int						GetDelayEstimateInMilliseconds() const;

		// Called from Java side so we can cache the native audio parameters.
		// This method will be called by the WebRtcAudioManager constructor, i.e.
		// on the same thread that this object is created on.
		void					fromGuiOnCacheAudioParameters(	JNIEnv* env,
																jint sample_rate,
																jint channels,
																jboolean hardware_aec,
																jboolean hardware_agc,
																jboolean hardware_ns,
																jboolean low_latency_output,
																jint output_buffer_size,
																jint input_buffer_size );

		virtual int32_t			RecordedDataIsAvailable(	const void* audioSamples,
															const size_t nSamples,
															const size_t nBytesPerSample,
															const size_t nChannels,
															const uint32_t samplesPerSec,
															const uint32_t totalDelayMS,
															const int32_t clockDrift,
															const uint32_t currentMicLevel,
															const bool keyPressed,
															uint32_t& newMicLevel );

		virtual int32_t			NeedMorePlayData(	const size_t nSamples,
													const size_t nBytesPerSample,
													const size_t nChannels,
													const uint32_t samplesPerSec,
													void* audioSamples,
													size_t& nSamplesOut,
													int64_t* elapsed_time_ms,
													int64_t* ntp_time_ms );

		// not used
		virtual int				OnDataAvailable(	const int voe_channels[],
													size_t number_of_voe_channels,
													const int16_t* audio_data,
													int sample_rate,
													size_t number_of_channels,
													size_t number_of_frames,
													int audio_delay_milliseconds,
													int current_volume,
													bool key_pressed,
													bool need_audio_processing) { return 0; };

		// not used
		virtual void			OnData( int voe_channel,
										const void* audio_data,
										int bits_per_sample,
										int sample_rate,
										size_t number_of_channels,
										size_t number_of_frames){};

		// not used
		virtual void			PushCaptureData(	int voe_channel,
													const void* audio_data,
													int bits_per_sample,
													int sample_rate,
													size_t number_of_channels,
													size_t number_of_frames ){};

		// not used
		virtual void			PullRenderData(	int bits_per_sample,
												int sample_rate,
												size_t number_of_channels,
												size_t number_of_frames,
												void* audio_data,
												int64_t* elapsed_time_ms,
												int64_t* ntp_time_ms ){};

	private:
		void					resetParams( void );

		EngineImp&				m_EngineImp;
		P2PEngine&				m_Engine;
		MediaProcessor&			m_MediaProcessor;
		BuildInfo&				m_BuildInfo;
		AudioTrackJni			m_AudioPlayout;
		AudioRecordJni			m_AudioRecord;

		// Wraps the Java specific parts of the AudioManager.
		AudioMgrJavaClass *		m_AudioManagerJavaClass;
		jobject					m_AudioManagerJavaObject;

		AudioDeviceModule::AudioLayer	audio_layer_;

		// Set to true by Init() and false by Close().
		bool					m_AudioManagerInitialized;

		// True if device supports hardware (or built-in) AEC.
		bool					hardware_aec_;
		// True if device supports hardware (or built-in) AGC.
		bool					hardware_agc_;
		// True if device supports hardware (or built-in) NS.
		bool					hardware_ns_;

		// True if device supports the low-latency OpenSL ES audio path.
		bool					low_latency_playout_;

		// The delay estimate can take one of two fixed values depending on if the
		// device supports low-latency output or not.
		int						delay_estimate_in_milliseconds_;

		// Contains native parameters (e.g. sample rate, channel configuration).
		// Set at construction in OnCacheAudioParameters() which is called from
		// Java on the same thread as this object is created on.
		AudioParameters			playout_parameters_;
		AudioParameters			record_parameters_;

		AudioDeviceBuffer		m_AudioDevicePlayoutBuffer;
		AudioDeviceBuffer		m_AudioDeviceRecordBuffer;
		uint8_t						m_MyFrameBuf[MY_FRAME_DATA_LEN];
		int						m_MyFrameReadIdx;
		int						m_MyFrameDataAvail;
		int16_t						m_MyLastReadSample;
		bool					m_WantMicInput;
		bool					m_WantSpeakerOutput;
		bool					m_HasEnoughPlayoutReadsOccured;
		int						m_TotalPlayoutRequestedMs;
	};

}  // namespace webrtc

#endif  // WEBRTC_MODULES_AUDIO_DEVICE_ANDROID_AUDIO_MANAGER_H_

#endif // TARGET_OS_ANDROID
