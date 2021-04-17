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
#include "audio_record_jni.h"
#include "AudioRecordJava.h"
#include "AndroidAudioMgr.h"

#include <GoTvCore/GoTvP2P/EchoCancel/arraysize.h>
#include <GoTvCore/GoTvP2P/EchoCancel/checks.h>
#include <GoTvCore/GoTvP2P/EchoCancel/format_macros.h>
#include "audio_common.h"

#include <CoreLib/VxDebug.h>
#include <CoreLib/VxThread.h>

#include <utility>

namespace webrtc 
{
	// AudioRecordJni implementation.
	AudioRecordJni::AudioRecordJni( AudioManager& audio_manager )
	: m_AudioManager(audio_manager)
	, m_AudioRecordJavaClass( 0 )
	, m_AudioRecordJavaObject( 0 )
	, m_AudioParameters( audio_manager.GetRecordAudioParameters() )
	, total_delay_in_milliseconds_(0)
	, direct_buffer_address_( 0 )
	, direct_buffer_capacity_in_bytes_(0)
	, frames_per_buffer_(0)
	, initialized_(false)
	, recording_(false)
	, audio_device_buffer_( 0 ) 
	{
	}

	AudioRecordJni::~AudioRecordJni() 
	{
		//LogMsg( LOG_DEBUG, "~dtor%s", VxGetThreadInfo().c_str());
		//vx_assert(thread_checker_.CalledOnValidThread());
		Terminate();
	}

	int32_t AudioRecordJni::Init() 
	{
		//LogMsg( LOG_DEBUG, "Init%s", VxGetThreadInfo().c_str());
		//vx_assert(thread_checker_.CalledOnValidThread());
		m_AudioRecordJavaClass = GetAudioRecordJavaClass();
		vx_assert( m_AudioRecordJavaClass );
		m_AudioRecordJavaObject = m_AudioRecordJavaClass->creatNewObj( this );

		//if( !m_AudioRecordJavaClass->Init( m_AudioRecordJavaObject )) 
		//{
		//	LogMsg( LOG_ERROR, "init failed!");
		//	return false;
		//}

		return 0;
	}

	int32_t AudioRecordJni::Terminate( ) 
	{
		LogMsg( LOG_DEBUG, "Terminate %s", VxGetThreadInfo().c_str());
		//vx_assert(thread_checker_.CalledOnValidThread());
		StopRecording();
		return 0;
	}

	int32_t AudioRecordJni::InitRecording() 
	{
		LogMsg( LOG_DEBUG, "InitRecording %s", VxGetThreadInfo().c_str());
		//vx_assert(thread_checker_.CalledOnValidThread());
		vx_assert(!initialized_);
		vx_assert(!recording_);
		int frames_per_buffer = m_AudioRecordJavaClass->InitRecording( m_AudioRecordJavaObject,
			m_AudioParameters.sample_rate(), m_AudioParameters.channels());
		if (frames_per_buffer < 0) 
		{
			LogMsg( LOG_ERROR, "InitRecording failed!");
			return -1;
		}

		frames_per_buffer_ = static_cast<size_t>(frames_per_buffer);
		LogMsg( LOG_DEBUG, "frames_per_buffer: %d\n", frames_per_buffer_);
		vx_assert(direct_buffer_capacity_in_bytes_ == frames_per_buffer_ * kBytesPerFrame);
		vx_assert( frames_per_buffer_ == m_AudioParameters.frames_per_10ms_buffer() );
		initialized_ = true;
		return 0;
	}

	int32_t AudioRecordJni::StartRecording() 
	{
		//LogMsg( LOG_DEBUG, "StartRecording %s", VxGetThreadInfo().c_str());
		//vx_assert(thread_checker_.CalledOnValidThread());
		vx_assert(initialized_);
		vx_assert(!recording_);
		if (!m_AudioRecordJavaClass->StartRecording( m_AudioRecordJavaObject ) ) 
		{
			LogMsg( LOG_ERROR, "StartRecording failed!");
			return -1;
		}

		recording_ = true;
		return 0;
	}

	int32_t AudioRecordJni::StopRecording() 
	{
		//LogMsg( LOG_DEBUG, "StopRecording %s", VxGetThreadInfo().c_str());
		//vx_assert(thread_checker_.CalledOnValidThread());
		if (!initialized_ || !recording_) {
			return 0;
		}

		if (!m_AudioRecordJavaClass->StopRecording( m_AudioRecordJavaObject )) 
		{
			LogMsg( LOG_ERROR, "StopRecording failed!");
			return -1;
		}

		// If we don't detach here, we will hit a vx_assert in OnDataIsRecorded()
		// next time StartRecording() is called since it will create a new Java
		// thread.
		//thread_checker_java_.DetachFromThread();
		//initialized_ = false;
		recording_ = false;
		//direct_buffer_address_= nullptr;
		return 0;
	}

	void AudioRecordJni::AttachAudioBuffer( AudioDeviceBuffer* audioBuffer ) 
	{
		//vx_assert(thread_checker_.CalledOnValidThread());
		audio_device_buffer_ = audioBuffer;
		const int sample_rate_hz = m_AudioParameters.sample_rate();
		//LogMsg( LOG_DEBUG, "SetRecordingSampleRate(%d)", sample_rate_hz);
		audio_device_buffer_->SetRecordingSampleRate(sample_rate_hz);
		const size_t channels = m_AudioParameters.channels();
		//LogMsg( LOG_DEBUG, "SetRecordingChannels(%d)", channels);
		audio_device_buffer_->SetRecordingChannels(channels);
		total_delay_in_milliseconds_ = m_AudioManager.GetDelayEstimateInMilliseconds();
		LogMsg( LOG_DEBUG, "AttachAudioBuffer Rate(%d) Channels(%d) Delay ms(%d)\n", sample_rate_hz, channels, total_delay_in_milliseconds_ );
		vx_assert(total_delay_in_milliseconds_ > 0);

		//LogMsg( LOG_DEBUG, "total_delay_in_milliseconds: %d", total_delay_in_milliseconds_);
	}

	int32_t AudioRecordJni::EnableBuiltInAEC(bool enable) 
	{
		LogMsg( LOG_DEBUG, "EnableBuiltInAEC%s", VxGetThreadInfo().c_str());
		//vx_assert(thread_checker_.CalledOnValidThread());
		return m_AudioRecordJavaClass->EnableBuiltInAEC( m_AudioRecordJavaObject, enable ) ? 0 : -1;
	}

	int32_t AudioRecordJni::EnableBuiltInAGC(bool enable) 
	{
		LogMsg( LOG_DEBUG, "EnableBuiltInAGC%s", VxGetThreadInfo().c_str());
		//vx_assert(thread_checker_.CalledOnValidThread());
		return m_AudioRecordJavaClass->EnableBuiltInAGC( m_AudioRecordJavaObject, enable) ? 0 : -1;
	}

	int32_t AudioRecordJni::EnableBuiltInNS(bool enable) 
	{
		LogMsg( LOG_DEBUG, "EnableBuiltInNS%s", VxGetThreadInfo().c_str());
		//vx_assert(thread_checker_.CalledOnValidThread());
		return m_AudioRecordJavaClass->EnableBuiltInNS( m_AudioRecordJavaObject, enable) ? 0 : -1;
	}

	void AudioRecordJni::fromGuiOnCacheRecordDirectBufferAddress( JNIEnv* env, jobject byte_buffer ) 
	{
		//vx_assert(thread_checker_.CalledOnValidThread());
		vx_assert(!direct_buffer_address_);
		direct_buffer_address_ = env->GetDirectBufferAddress(byte_buffer);
		jlong capacity = env->GetDirectBufferCapacity(byte_buffer);
		LogMsg( LOG_DEBUG, "OnCacheDirectBufferAddress direct buffer capacity: %lld", capacity);
		direct_buffer_capacity_in_bytes_ = static_cast<size_t>(capacity);
	}

	// This method is called on a high-priority thread from Java. The name of
	// the thread is 'AudioRecordThread'.
	void AudioRecordJni::fromGuiOnDataIsRecorded( int length ) 
	{
		//vx_assert(thread_checker_java_.CalledOnValidThread());
		if (!audio_device_buffer_) 
		{
			LogMsg( LOG_ERROR, "AttachAudioBuffer has not been called!");
			return;
		}

		audio_device_buffer_->SetRecordedBuffer( direct_buffer_address_, frames_per_buffer_ );
		// We provide one (combined) fixed delay estimate for the APM and use the
		// |playDelayMs| parameter only. Components like the AEC only sees the sum
		// of |playDelayMs| and |recDelayMs|, hence the distributions does not matter.
		audio_device_buffer_->SetVQEData(total_delay_in_milliseconds_,
			0,   // recDelayMs
			0);  // clockDrift
		if (audio_device_buffer_->DeliverRecordedData() == -1) 
		{
			LogMsg( LOG_ERROR, "AudioDeviceBuffer::DeliverRecordedData failed!");
		}
	}

}  // namespace webrtc

#endif // TARGET_OS_ANDROID
