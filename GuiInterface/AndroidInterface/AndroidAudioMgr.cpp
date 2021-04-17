/*
*  Copyright (c) 2015 The WebRTC project authors. All Rights Reserved.
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
#include "AudioMgrJava.h"
#include "EngineImp.h"
#include "JavaHelpers.h"

#include <utility>

#include <GoTvCore/GoTvP2P/MediaProcessor/MediaProcessor.h>
#include <CoreLib/VxDebug.h>

#include <GoTvCore/GoTvP2P/EchoCancel/arraysize.h>
#include <GoTvCore/GoTvP2P/EchoCancel/checks.h>
#include "audio_common.h"

#define AUDIO_MGR_LOG_TAG "NativeAudioMgr:"

namespace
{
	const int					MYP2PWEB_SAMPLE_RATE = 8000;
}

namespace webrtc 
{
	//============================================================================
	// AudioManager implementation
	AudioManager::AudioManager( EngineImp& engineImp )
	: m_EngineImp( engineImp )
	, m_Engine(m_EngineImp.getEngine() )
	, m_MediaProcessor( m_Engine.getMediaProcesser() )
	, m_BuildInfo(m_EngineImp.getBuildInfo() )
	, m_AudioPlayout( *this )
	, m_AudioRecord( *this )
	, m_AudioManagerJavaClass( 0 )
	, m_AudioManagerJavaObject( 0 )
	, audio_layer_(AudioDeviceModule::kPlatformDefaultAudio)
	, m_AudioManagerInitialized( false )
	, hardware_aec_(false)
	, hardware_agc_(false)
	, hardware_ns_(false)
	, low_latency_playout_(false)
	, delay_estimate_in_milliseconds_(0) 
	//, m_UseOpenSLES( false )
	//, m_OpenSLESPlayer( *this )
	, m_MyFrameReadIdx( 0 )
	, m_MyFrameDataAvail( 0 )
	, m_WantMicInput( false )
	, m_WantSpeakerOutput( false )
	, m_HasEnoughPlayoutReadsOccured( false )
	, m_TotalPlayoutRequestedMs( 0 )
	{
		__android_log_write( ANDROID_LOG_INFO, AUDIO_MGR_LOG_TAG, "AudioManager::AudioManager\n" );
	}

	//============================================================================
	AudioManager::~AudioManager() 
	{
		//__android_log_print( ANDROID_LOG_INFO, AUDIO_MGR_LOG_TAG, "~dtor%s", VxGetThreadInfo().c_str());
		//vx_assert(thread_checker_.CalledOnValidThread());
		__android_log_write( ANDROID_LOG_INFO, AUDIO_MGR_LOG_TAG, "AudioManager::~AudioManager\n" );
		Close();
	}

	//============================================================================
	void AudioManager::toGuiPlayAudio( int16_t * pu16PcmData, int pcmDataLenInBytes )
	{
		vx_assert( pcmDataLenInBytes == MY_FRAME_DATA_LEN );
		vx_assert( 0 == m_MyFrameReadIdx );
		memcpy( m_MyFrameBuf, pu16PcmData, pcmDataLenInBytes );
		m_MyFrameReadIdx = 0;
		m_MyFrameDataAvail = pcmDataLenInBytes;
	}

	//============================================================================
	void AudioManager::toGuiWantMicrophoneRecording( bool wantMicInput )
	{
		if( false == m_AudioManagerInitialized )
		{
			__android_log_write( ANDROID_LOG_INFO, AUDIO_MGR_LOG_TAG, "AudioManager::toGuiWantMicrophoneRecording not initialized\n" );
			return;
		}

		if(  wantMicInput != m_WantMicInput )
		{
			bool isPreviouslyPlaying = m_WantMicInput | m_WantSpeakerOutput;
			m_WantMicInput = wantMicInput;
			bool isNowPlaying = m_WantMicInput | m_WantSpeakerOutput;
			if( isPreviouslyPlaying != isNowPlaying )
			{
				enableAudio( isNowPlaying );
			}
		}
	}

	//============================================================================
	void AudioManager::toGuiWantSpeakerOutput( bool wantSpeakerOutput )
	{
		if( false == m_AudioManagerInitialized )
		{
			__android_log_write( ANDROID_LOG_INFO, AUDIO_MGR_LOG_TAG, "AudioManager::toGuiWantSpeakerOutput not initialized\n" );
			return;
		}

		if( wantSpeakerOutput != m_WantSpeakerOutput )
		{
			bool isPreviouslyPlaying = m_WantMicInput | m_WantSpeakerOutput;
			m_WantSpeakerOutput = wantSpeakerOutput;
			bool isNowPlaying = m_WantMicInput | m_WantSpeakerOutput;
			if( isPreviouslyPlaying != isNowPlaying )
			{
				enableAudio( isNowPlaying );
			}
		}
	}

	//============================================================================
	void AudioManager::resetParams( void )
	{
		m_HasEnoughPlayoutReadsOccured	= false;
		m_TotalPlayoutRequestedMs		= 0;
		m_MyFrameReadIdx	= 0;
		m_MyFrameDataAvail	= 0;
		m_MyLastReadSample	= 0;
		memset( m_MyFrameBuf, 0, sizeof( m_MyFrameBuf ) );
	}

	//============================================================================
	bool AudioManager::enableAudio( bool enable )
	{
		bool result = true;
		RCODE rc = 0;
		if( enable )
		{
			resetParams();
			//if( m_UseOpenSLES )
			//{
			//	rc = m_OpenSLESPlayer.StartPlayout();
			//	if( 0 != rc )
			//	{
			//		__android_log_print( ANDROID_LOG_INFO, AUDIO_MGR_LOG_TAG, "m_OpenSLESPlayer.StartPlayout returned error %d\n", (int)rc );
			//		result = false;
			//	}
			//}
			//else
			//{
				rc = m_AudioPlayout.StartPlayout();
				if( 0 != rc )
				{
					__android_log_print( ANDROID_LOG_INFO, AUDIO_MGR_LOG_TAG, "m_AudioPlayout.StartPlayout returned error %d\n", (int)rc );
					result = false;
				}
			//}

			rc = m_AudioRecord.StartRecording();
			if( 0 != rc )
			{
				__android_log_print( ANDROID_LOG_INFO, AUDIO_MGR_LOG_TAG, "m_AudioRecord.StartRecording returned error %d\n", (int)rc );
				result = false;
			}
		}
		else
		{
			//if( m_UseOpenSLES )
			//{
			//	rc = m_OpenSLESPlayer.StopPlayout();
			//	if( 0 != rc )
			//	{
			//		__android_log_print( ANDROID_LOG_INFO, AUDIO_MGR_LOG_TAG, "m_OpenSLESPlayer.StopPlayout returned error %d\n", (int)rc );
			//		result = false;
			//	}
			//}
			//else
			//{
				rc = m_AudioPlayout.StopPlayout();
				if( 0 != rc )
				{
					__android_log_print( ANDROID_LOG_INFO, AUDIO_MGR_LOG_TAG, "m_AudioPlayout.StopPlayout returned error %d\n", (int)rc );
					result = false;
				}
			//}

			rc = m_AudioRecord.StopRecording();
			if( 0 != rc )
			{
				__android_log_print( ANDROID_LOG_INFO, AUDIO_MGR_LOG_TAG, "m_AudioRecord.StopRecording returned error %d\n", (int)rc );
				result = false;
			}
		}

		return result;
	}

	//============================================================================
	void AudioManager::SetActiveAudioLayer( AudioDeviceModule::AudioLayer audio_layer ) 
	{
		//__android_log_print( ANDROID_LOG_INFO, AUDIO_MGR_LOG_TAG, "SetActiveAudioLayer(%d)%s", audio_layer, VxGetThreadInfo().c_str());
		//vx_assert(thread_checker_.CalledOnValidThread());
		if( m_AudioManagerInitialized )
		{
			__android_log_write( ANDROID_LOG_ERROR, AUDIO_MGR_LOG_TAG, "AudioManager::SetActiveAudioLayer should be called before initialized\n" );
			//return;
		}

		// Store the currently utilized audio layer.
		audio_layer_ = audio_layer;
		// The delay estimate can take one of two fixed values depending on if the
		// device supports low-latency output or not. However, it is also possible
		// that the user explicitly selects the high-latency audio path, hence we use
		// the selected |audio_layer| here to set the delay estimate.
		if( AudioDeviceModule::kAndroidJavaAudio == audio_layer )
		{
			delay_estimate_in_milliseconds_ = kHighLatencyModeDelayEstimateInMilliseconds;
			//m_UseOpenSLES = false;
		}
		else
		{
			delay_estimate_in_milliseconds_ = kLowLatencyModeDelayEstimateInMilliseconds;
			//m_UseOpenSLES = true;
		}

		__android_log_print( ANDROID_LOG_INFO, AUDIO_MGR_LOG_TAG, "delay_estimate_in_milliseconds: %d\n", delay_estimate_in_milliseconds_);
	}

	//============================================================================
	bool AudioManager::initAndroidAudioMgr() 
	{
		if( m_AudioManagerInitialized )
		{
			__android_log_write( ANDROID_LOG_INFO, AUDIO_MGR_LOG_TAG, "AudioManager::initAndroidAudioMgr already initialized\n" );
			return true;
		}

		RCODE rc = 0;
		bool result = true;
		//__android_log_print( ANDROID_LOG_INFO, AUDIO_MGR_LOG_TAG, "AudioManager::Init %s\n", VxGetThreadInfo().c_str());
		//vx_assert(thread_checker_.CalledOnValidThread());
		resetParams();

		m_AudioManagerJavaClass = GetAudioMgrJavaClass();
		vx_assert( m_AudioManagerJavaClass );
		m_AudioManagerJavaObject = m_AudioManagerJavaClass->creatNewObj( this );

		if( !m_AudioManagerJavaClass->Init( m_AudioManagerJavaObject )) 
		{
			__android_log_print( ANDROID_LOG_INFO, AUDIO_MGR_LOG_TAG, "AudioManagerJava init failed!");
			return false;
		}

		if( AudioDeviceModule::kPlatformDefaultAudio == audio_layer_ ) 
		{
			if( this->IsLowLatencyPlayoutSupported() ) 
			{
				// Always use OpenSL ES for output on devices that supports the
				// low-latency output audio path.
				SetActiveAudioLayer( AudioDeviceModule::kAndroidJavaInputAndOpenSLESOutputAudio );
			} 
			else 
			{
				// Use Java-based audio in both directions when low-latency output
				// is not supported.
				SetActiveAudioLayer( AudioDeviceModule::kAndroidJavaAudio );
			}
		}

		if( 0 != m_AudioRecord.Init()) 
		{
			__android_log_print( ANDROID_LOG_INFO, AUDIO_MGR_LOG_TAG, "m_AudioPlayout init failed!");
			return false;
		}

		//if( m_UseOpenSLES )
		//{
		//	m_OpenSLESPlayer.Init();
		//	m_OpenSLESPlayer.AttachAudioBuffer( &m_AudioDevicePlayoutBuffer );
		//	m_AudioDevicePlayoutBuffer.RegisterAudioCallback( this );
		//	rc = m_OpenSLESPlayer.InitPlayout();  // InitPlayout calls InitSpeaker
		//	if( 0 != rc )
		//	{
		//		__android_log_print( ANDROID_LOG_INFO, AUDIO_MGR_LOG_TAG, "m_OpenSLESPlayer.InitPlayout returned error %d\n", (int)rc );
		//		result = false;
		//	}
		//}
		//else
		//{
			if( 0 != m_AudioPlayout.Init()) 
			{
				__android_log_print( ANDROID_LOG_INFO, AUDIO_MGR_LOG_TAG, "m_AudioPlayout init failed!");
				return false;
			}

			m_AudioPlayout.AttachAudioBuffer( &m_AudioDevicePlayoutBuffer );
			m_AudioDevicePlayoutBuffer.RegisterAudioCallback( this );
			rc = m_AudioPlayout.InitPlayout();  // InitPlayout calls InitSpeaker
			if( 0 != rc )
			{
				__android_log_print( ANDROID_LOG_INFO, AUDIO_MGR_LOG_TAG, "m_AudioPlayout.InitPlayout returned error %d\n", (int)rc );
				result = false;
			}
		//}
		
		m_AudioRecord.AttachAudioBuffer( &m_AudioDeviceRecordBuffer );
		m_AudioDeviceRecordBuffer.RegisterAudioCallback( this );
		rc = m_AudioRecord.InitRecording(); // InitRecording calls InitMicrophone
		if( 0 != rc )
		{
			__android_log_print( ANDROID_LOG_INFO, AUDIO_MGR_LOG_TAG, "m_AudioRecord.InitRecording returned error %d\n", (int)rc );
			result = false;
		}


		m_AudioManagerInitialized = true;
		return result;
	}

	//============================================================================
	bool AudioManager::Close() 
	{
		__android_log_write( ANDROID_LOG_INFO, AUDIO_MGR_LOG_TAG, "---AudioManager::Close start\n" );
		__android_log_print( ANDROID_LOG_INFO, AUDIO_MGR_LOG_TAG, "AudioManager::Close %s", VxGetThreadInfo().c_str());
		if (!m_AudioManagerInitialized)
			return true;
		enableAudio( false );
		m_AudioManagerJavaClass->Close( m_AudioManagerJavaObject );
		m_AudioManagerInitialized = false;
		__android_log_write( ANDROID_LOG_INFO, AUDIO_MGR_LOG_TAG, "---AudioManager::Close done\n" );
		return true;
	}

	//============================================================================
	bool AudioManager::IsCommunicationModeEnabled() const 
	{
		__android_log_write( ANDROID_LOG_INFO, AUDIO_MGR_LOG_TAG, "AudioManager::IsCommunicationModeEnabled\n" );
		//__android_log_print( ANDROID_LOG_INFO, AUDIO_MGR_LOG_TAG, "IsCommunicationModeEnabled()");
		return m_AudioManagerJavaClass->IsCommunicationModeEnabled( m_AudioManagerJavaObject );
	}

	//============================================================================
	bool AudioManager::IsAcousticEchoCancelerSupported() const 
	{
		return hardware_aec_;
	}

	//============================================================================
	bool AudioManager::IsAutomaticGainControlSupported() const 
	{
		return hardware_agc_;
	}

	//============================================================================
	bool AudioManager::IsNoiseSuppressorSupported() const 
	{
		return hardware_ns_;
	}

	//============================================================================
	bool AudioManager::IsLowLatencyPlayoutSupported() const 
	{
		//__android_log_print( ANDROID_LOG_INFO, AUDIO_MGR_LOG_TAG, "IsLowLatencyPlayoutSupported()");
		// Some devices are blacklisted for usage of OpenSL ES even if they report
		// that low-latency playout is supported. See b/21485703 for details.
		return m_AudioManagerJavaClass->IsDeviceBlacklistedForOpenSLESUsage( m_AudioManagerJavaObject ) ?
			false : low_latency_playout_;
	}

	//============================================================================
	int AudioManager::GetDelayEstimateInMilliseconds() const 
	{
		return delay_estimate_in_milliseconds_;
	}

	//============================================================================
	void AudioManager::fromGuiOnCacheAudioParameters(	JNIEnv* env,
														jint sample_rate,
														jint channels,
														jboolean hardware_aec,
														jboolean hardware_agc,
														jboolean hardware_ns,
														jboolean low_latency_output,
														jint output_buffer_size,
														jint input_buffer_size) 
	{
		__android_log_print( ANDROID_LOG_INFO, AUDIO_MGR_LOG_TAG, "OnCacheAudioParameters %s hardware aec=%d agc=%d ns=%d\n", VxGetThreadInfo().c_str(), hardware_aec, hardware_agc, hardware_ns );
		__android_log_print( ANDROID_LOG_INFO, AUDIO_MGR_LOG_TAG, "low_latency_output: %d\n", low_latency_output);
		__android_log_print( ANDROID_LOG_INFO, AUDIO_MGR_LOG_TAG, "sample_rate: %d channels: %d\n", sample_rate, channels);
		__android_log_print( ANDROID_LOG_INFO, AUDIO_MGR_LOG_TAG, "output_buffer_size: %d input_buffer_size: %d\n", output_buffer_size, input_buffer_size );
		if( sample_rate != 8000 )
		{
			__android_log_print( ANDROID_LOG_INFO, AUDIO_MGR_LOG_TAG, "sample_rate: %d changing to 8000\n", sample_rate);
			sample_rate = 8000;
		}

		//vx_assert(thread_checker_.CalledOnValidThread());
		hardware_aec_ = hardware_aec;
		hardware_agc_ = hardware_agc;
		hardware_ns_ = hardware_ns;
		low_latency_playout_ = low_latency_output;
		// TODO(henrika): add support for stereo output.
		playout_parameters_.reset(sample_rate, static_cast<size_t>(channels),
			static_cast<size_t>(output_buffer_size));
		record_parameters_.reset(sample_rate, static_cast<size_t>(channels),
			static_cast<size_t>(input_buffer_size));
	}

	//============================================================================
	const AudioParameters& AudioManager::GetPlayoutAudioParameters() 
	{
		return playout_parameters_;
	}

	//============================================================================
	const AudioParameters& AudioManager::GetRecordAudioParameters() 
	{
		return record_parameters_;
	}

	//============================================================================
	int32_t AudioManager::RecordedDataIsAvailable(			const void* audioSamples,
															const size_t nSamples,
															const size_t nBytesPerSample,
															const size_t nChannels,
															const uint32_t samplesPerSec,
															const uint32_t totalDelayMS,
															const int32_t clockDrift,
															const uint32_t currentMicLevel,
															const bool keyPressed,
															uint32_t& newMicLevel )
	{
		if( m_MediaProcessor.isSpeakerOutputEnabled() 
			&& ( false == m_HasEnoughPlayoutReadsOccured ) )
		{
			// wait until enough playout events so there is echo data from mixer to
			// process microphone data against
			return 0;
		}

		//__android_log_print( ANDROID_LOG_INFO, AUDIO_MGR_LOG_TAG, "MySndMgr::RecordedDataIsAvailable nSamples %d\n", nSamples );
		float dnFreqDiv = (float)samplesPerSec  / (float)MYP2PWEB_SAMPLE_RATE;
		float dnResampleDiv = dnFreqDiv * nChannels;

		if( 1.0f == dnResampleDiv )
		{
			// no resample required
			//m_Engine.fromGuiMicrophoneDataWithInfo( (int16_t *)audioSamples, nSamples * nBytesPerSample, totalDelayMS, clockDrift );
			m_MediaProcessor.fromGuiMicrophoneDataWithInfo( (int16_t *)audioSamples, nSamples * nBytesPerSample, totalDelayMS, clockDrift );
		}
		else
		{
			// down resample to 8000 hz
			int mySampleCnt				= nSamples / dnFreqDiv;
			int middleOfSampleOffset	= (int)(dnFreqDiv / 2.0f);
			int16_t * dnSampledBuf			= new int16_t[ mySampleCnt ];
			int16_t * srcSamples			= (int16_t *)audioSamples;
			float smpInc				= dnResampleDiv;
			for( int i = 0; i < mySampleCnt; i++ )
			{
				int iSrcIdx = (int)(smpInc * (float)i) + middleOfSampleOffset;
				dnSampledBuf[i] = srcSamples[ iSrcIdx ];
			}

			//m_Engine.fromGuiMicrophoneDataWithInfo( dnSampledBuf, mySampleCnt << 1, totalDelayMS, clockDrift );
			m_MediaProcessor.fromGuiMicrophoneDataWithInfo( dnSampledBuf, mySampleCnt << 1, totalDelayMS, clockDrift );
			delete[] dnSampledBuf;
		}

		return 0;
	}

	//============================================================================
	int32_t AudioManager::NeedMorePlayData(		const size_t nSamples,
												const size_t nBytesPerSample,
												const size_t nChannels,
												const uint32_t samplesPerSec,
												void* audioSamples,
												size_t& nSamplesOut,
												int64_t* elapsed_time_ms,
												int64_t* ntp_time_ms )
	{
		//__android_log_print( ANDROID_LOG_INFO, AUDIO_MGR_LOG_TAG, "MySndMgr::NeedMorePlayData nSamples %d\n", nSamples );

		size_t deviceReqDataLen = nSamples * nBytesPerSample * nChannels;
		float upResampleMultiplier = samplesPerSec  / MYP2PWEB_SAMPLE_RATE;
		int myRequredDataLen = deviceReqDataLen / upResampleMultiplier;
		if( nChannels == 2 )
		{
			upResampleMultiplier *= 2;
		}

		int		resampleCnt		= (int)upResampleMultiplier;
		int		myDataUsed		= 0;
		int16_t *	destSamples		= (int16_t *)audioSamples;
		int		iDestIdx		= 0;
		while( myDataUsed < myRequredDataLen )
		{
			if( 0 == m_MyFrameDataAvail )
			{
				//m_Engine.fromGuiAudioOutSpaceAvail( MY_FRAME_DATA_LEN );
				m_MediaProcessor.fromGuiAudioOutSpaceAvail( MY_FRAME_DATA_LEN );
				if( 0 == m_MyFrameDataAvail )
				{
					memset( audioSamples, 0, deviceReqDataLen );
					LogMsg( LOG_WARN, "AudioManager::NeedMorePlayData failed get more data\n" );
					nSamplesOut = nSamples;
					return 0;
				}
			}

			int myDataRequired = myRequredDataLen - myDataUsed;
			int myDataThisChunk = ( m_MyFrameDataAvail >= myDataRequired ) ? myDataRequired : m_MyFrameDataAvail;
			if( 1 == upResampleMultiplier )
			{
				memcpy( &destSamples[iDestIdx], &m_MyFrameBuf[m_MyFrameReadIdx], myDataThisChunk );
				iDestIdx += ( myDataThisChunk >> 1 );
			}
			else
			{
				int mySampleCnt		= myDataThisChunk >> 1;
				int16_t * srcSamples	= (int16_t *)&m_MyFrameBuf[m_MyFrameReadIdx];
				int16_t firstSample		= m_MyLastReadSample;
				int16_t secondSample	= firstSample;
				float sampleStep;
				for( int i = 0; i < mySampleCnt; i++ )
				{
					firstSample = secondSample;
					secondSample = srcSamples[i];
					if( secondSample >= firstSample )
					{
						// ramp up
						sampleStep = ((secondSample - firstSample) / upResampleMultiplier); 
					}
					else
					{
						// ramp down
						sampleStep = -((firstSample - secondSample) / upResampleMultiplier); 
					}

					if( 0 == sampleStep )
					{
						for( int j = 0; j < resampleCnt; ++j )
						{
							destSamples[iDestIdx] = firstSample; 
							iDestIdx++;
						}
					}
					else
					{
						float sampleOffs = sampleStep;
						for( int j = 0; j < resampleCnt; ++j )
						{
							destSamples[iDestIdx] = (int16_t)(firstSample + sampleOffs); 
							iDestIdx++;
							sampleOffs += sampleStep;
						}
					}
				}

				// save the last sample to be used as first sample reference in next frame
				m_MyLastReadSample = srcSamples[ mySampleCnt - 1 ];
			}

			myDataUsed += myDataThisChunk;
			m_MyFrameDataAvail -= myDataThisChunk;
			m_MyFrameReadIdx	+= myDataThisChunk;
			if( m_MyFrameReadIdx >= MY_FRAME_DATA_LEN )
			{
				m_MyFrameReadIdx = 0;
			}
		}

		nSamplesOut = nSamples;

		if( false == m_HasEnoughPlayoutReadsOccured )
		{
			// wait until enough playout events so there is echo data from mixer to
			// process microphone data against
			int msThisSample = ( nSamples * 1000 ) / samplesPerSec;
			m_TotalPlayoutRequestedMs += msThisSample;
			if( m_TotalPlayoutRequestedMs >= delay_estimate_in_milliseconds_ )
			{
				m_HasEnoughPlayoutReadsOccured = true;
			}
		}

		return 0;
	}
}  // namespace webrtc

#endif // TARGET_OS_ANDROID
