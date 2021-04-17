/*
*  Copyright (c) 2013 The WebRTC project authors. All Rights Reserved.
*
*  Use of this source code is governed by a BSD-style license
*  that can be found in the LICENSE file in the root of the source
*  tree. An additional intellectual property rights grant can be found
*  in the file PATENTS.  All contributing project authors may
*  be found in the AUTHORS file in the root of the source tree.
*/
// Code Modified for MyP2PWeb by Brett R. Jones

#ifdef TARGET_OS_ANDROID

#include "AndroidAudioMgr.h"
#include "audio_track_jni.h"
#include "AudioTrackJava.h"

#include <utility>

#include <CoreLib/VxDebug.h>
#include <CoreLib/VxThread.h>

#include <GoTvCore/GoTvP2P/EchoCancel/arraysize.h>
#include <GoTvCore/GoTvP2P/EchoCancel/checks.h>
#include <GoTvCore/GoTvP2P/EchoCancel/format_macros.h>

#include <CoreLib/VxDebug.h>

namespace webrtc 
{
	//============================================================================
	// TODO(henrika): possible extend usage of AudioManager and add it as member.
	AudioTrackJni::AudioTrackJni( AudioManager& audio_manager )
	: m_AudioManager( audio_manager )
	, m_AudioTrackJavaClass( 0 )
	, m_AudioTrackJavaObject( 0 )
	, m_AudioParameters(m_AudioManager.GetPlayoutAudioParameters() )
	, direct_buffer_address_( 0 )
	, direct_buffer_capacity_in_bytes_(0)
	, frames_per_buffer_(0)
	, initialized_(false)
	, playing_(false)
	, audio_device_buffer_( 0 ) 
	{


		//vx_assert(j_environment_);
		//JNINativeMethod native_methods[] = {
		//	{"nativeCacheDirectBufferAddress", "(Ljava/nio/ByteBuffer;J)V",
		//	reinterpret_cast<void*>(
		//	&webrtc::AudioTrackJni::CacheDirectBufferAddress)},
		//	{"nativeGetPlayoutData", "(IJ)V",
		//	reinterpret_cast<void*>(&webrtc::AudioTrackJni::GetPlayoutData)}};
			//j_native_registration_ = j_environment_->RegisterNatives(
			//	"org/webrtc/voiceengine/WebRtcAudioTrack", native_methods,
			//	arraysize(native_methods));
			//j_audio_track_.reset(new JavaAudioTrack(
			//	j_native_registration_.get(),
			//	j_native_registration_->NewObject(
			//	"<init>", "(Landroid/content/Context;J)V",
			//	JVM::GetInstance()->context(), PointerTojlong(this))));
			//// Detach from this thread since we want to use the checker to verify calls
			//// from the Java based audio thread.
			//thread_checker_java_.DetachFromThread();
	}

	//============================================================================
	AudioTrackJni::~AudioTrackJni() 
	{
		LogMsg( LOG_DEBUG, "~dtor %s\n", VxGetThreadInfo().c_str());
		//vx_assert(thread_checker_.CalledOnValidThread());
		Terminate();
	}

	//============================================================================
	int32_t AudioTrackJni::Init() 
	{
		LogMsg( LOG_DEBUG, "AudioTrackJni::Init %s \n", VxGetThreadInfo().c_str());
		//vx_assert(m_AudioParameters.is_valid());
		m_AudioTrackJavaClass = GetAudioTrackJavaClass();
		vx_assert( m_AudioTrackJavaClass );
		m_AudioTrackJavaObject = m_AudioTrackJavaClass->creatNewObj( this );

		//vx_assert(thread_checker_.CalledOnValidThread());
		return 0;
	}

	//============================================================================
	int32_t AudioTrackJni::Terminate() {
		LogMsg( LOG_DEBUG, "Terminate %s \n", VxGetThreadInfo().c_str());
		//vx_assert(thread_checker_.CalledOnValidThread());
		StopPlayout();
		return 0;
	}

	//============================================================================
	int32_t AudioTrackJni::InitPlayout() 
	{
		LogMsg( LOG_DEBUG, "InitPlayout thread %s Rate(%d) Channels(%d)\n", VxGetThreadInfo().c_str(), m_AudioParameters.sample_rate(), m_AudioParameters.channels() );
		//vx_assert(thread_checker_.CalledOnValidThread());
		vx_assert(!initialized_);
		vx_assert(!playing_);
		if( !m_AudioTrackJavaClass->InitPlayout( m_AudioTrackJavaObject,
			m_AudioParameters.sample_rate(), m_AudioParameters.channels())) 
		{
				LogMsg( LOG_ERROR, "InitPlayout failed! \n");
				return -1;
		}

		initialized_ = true;
		return 0;
	}

	//============================================================================
	int32_t AudioTrackJni::StartPlayout() 
	{
		LogMsg( LOG_DEBUG, "StartPlayout%s", VxGetThreadInfo().c_str());
		//vx_assert(thread_checker_.CalledOnValidThread());
		vx_assert(initialized_);
		vx_assert(!playing_);
		if( !m_AudioTrackJavaClass->StartPlayout( m_AudioTrackJavaObject ) ) 
		{
			LogMsg( LOG_ERROR, "StartPlayout failed! \n");
			return -1;
		}

		playing_ = true;
		return 0;
	}

	//============================================================================
	int32_t AudioTrackJni::StopPlayout() 
	{
		LogMsg( LOG_DEBUG, "StopPlayout %s \n", VxGetThreadInfo().c_str());
		//vx_assert(thread_checker_.CalledOnValidThread());
		if (!initialized_ || !playing_) 
		{
			return 0;
		}

		if( !m_AudioTrackJavaClass->StopPlayout( m_AudioTrackJavaObject ) ) 
		{
			LogMsg( LOG_ERROR, "StopPlayout failed!");
			return -1;
		}
		// If we don't detach here, we will hit a vx_assert in OnDataIsRecorded()
		// next time StartRecording() is called since it will create a new Java
		// thread.
		//thread_checker_java_.DetachFromThread();
		//initialized_ = false;
		playing_ = false;
		//direct_buffer_address_ = nullptr;
		return 0;
	}

	//============================================================================
	int AudioTrackJni::SpeakerVolumeIsAvailable( bool& available ) 
	{
		available = true;
		return 0;
	}

	//============================================================================
	int AudioTrackJni::SetSpeakerVolume( uint32_t volume ) 
	{
		LogMsg( LOG_DEBUG, "SetSpeakerVolume(%d) %s\n", volume, VxGetThreadInfo().c_str());
		//vx_assert(thread_checker_.CalledOnValidThread());
		return m_AudioTrackJavaClass->SetStreamVolume( m_AudioTrackJavaObject, volume) ? 0 : -1;
	}

	//============================================================================
	int AudioTrackJni::MaxSpeakerVolume(uint32_t& max_volume) const 
	{
		LogMsg( LOG_DEBUG, "MaxSpeakerVolume %s\n", VxGetThreadInfo().c_str());
		//vx_assert(thread_checker_.CalledOnValidThread());
		max_volume = m_AudioTrackJavaClass->GetStreamMaxVolume( m_AudioTrackJavaObject );
		return 0;
	}

	//============================================================================
	int AudioTrackJni::MinSpeakerVolume(uint32_t& min_volume) const 
	{
		LogMsg( LOG_DEBUG, "MaxSpeakerVolume %s\n", VxGetThreadInfo().c_str());
		//vx_assert(thread_checker_.CalledOnValidThread());
		min_volume = 0;
		return 0;
	}

	//============================================================================
	int AudioTrackJni::SpeakerVolume(uint32_t& volume) const 
	{
		LogMsg( LOG_DEBUG, "SpeakerVolume %s", VxGetThreadInfo().c_str());
		//vx_assert(thread_checker_.CalledOnValidThread());
		volume = m_AudioTrackJavaClass->GetStreamVolume( m_AudioTrackJavaObject );
		return 0;
	}

	//============================================================================
	// TODO(henrika): possibly add stereo support.
	void AudioTrackJni::AttachAudioBuffer(AudioDeviceBuffer* audioBuffer) 
	{
		//LogMsg( LOG_DEBUG, "AttachAudioBuffer %s\n", VxGetThreadInfo().c_str());
		//vx_assert(thread_checker_.CalledOnValidThread());
		audio_device_buffer_ = audioBuffer;
		const int sample_rate_hz = m_AudioParameters.sample_rate();
		//LogMsg( LOG_DEBUG, "SetPlayoutSampleRate(%d)\n", sample_rate_hz);
		audio_device_buffer_->SetPlayoutSampleRate(sample_rate_hz);
		const size_t channels = m_AudioParameters.channels();
		LogMsg( LOG_DEBUG, "AudioTrackJni::AttachAudioBuffer %s Rate(%d) Channels(%d) \n", VxGetThreadInfo().c_str(), sample_rate_hz, channels );
		audio_device_buffer_->SetPlayoutChannels(channels);
	}

	void AudioTrackJni::fromGuiOnCachePlayoutDirectBufferAddress( JNIEnv* env, jobject byte_buffer ) 
	{
		//LogMsg( LOG_DEBUG, "OnCacheDirectBufferAddress");
		//vx_assert(thread_checker_.CalledOnValidThread());
		vx_assert(!direct_buffer_address_);
		direct_buffer_address_ =
			env->GetDirectBufferAddress(byte_buffer);
		jlong capacity = env->GetDirectBufferCapacity(byte_buffer);
		//LogMsg( LOG_DEBUG, "direct buffer capacity: %lld", capacity);
		direct_buffer_capacity_in_bytes_ = static_cast<size_t>(capacity);
		frames_per_buffer_ = direct_buffer_capacity_in_bytes_ / kBytesPerFrame;
		LogMsg( LOG_DEBUG, "fromGuiOnCachePlayoutDirectBufferAddress frames_per_buffer: %d\n", frames_per_buffer_);
	}

	// This method is called on a high-priority thread from Java. The name of
	// the thread is 'AudioRecordTrack'.
	void AudioTrackJni::fromGuiOnGetPlayoutData(size_t length) 
	{
		//vx_assert(thread_checker_java_.CalledOnValidThread());
		vx_assert(frames_per_buffer_ == length / kBytesPerFrame);
		if (!audio_device_buffer_) 
		{
			LogMsg( LOG_ERROR, "AttachAudioBuffer has not been called!");
			return;
		}
		// Pull decoded data (in 16-bit PCM format) from jitter buffer.
		int samples = audio_device_buffer_->RequestPlayoutData(frames_per_buffer_);
		if (samples <= 0) 
		{
			LogMsg( LOG_ERROR, "AudioDeviceBuffer::RequestPlayoutData failed!");
			return;
		}

		vx_assert(static_cast<size_t>(samples) == frames_per_buffer_);
		// Copy decoded data into common byte buffer to ensure that it can be
		// written to the Java based audio track.
		samples = audio_device_buffer_->GetPlayoutData(direct_buffer_address_);
		vx_assert(length == kBytesPerFrame * samples);
	}

}  // namespace webrtc

#endif //TARGET_OS_ANDROID

