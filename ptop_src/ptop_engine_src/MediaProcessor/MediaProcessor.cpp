//============================================================================
// Copyright (C) 2015 Brett R. Jones
// Issued to MIT style license by Brett R. Jones in 2017
//
// You may use, copy, modify, merge, publish, distribute, sub-license, and/or sell this software
// provided this Copyright is not modified or removed and is included all copies or substantial portions of the Software
//
// This code is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// bjones.engineer@gmail.com
// http://www.nolimitconnect.org
//============================================================================

#include "MediaProcessor.h"
#include "MediaClient.h"
#include "RawAudio.h"
#include "RawVideo.h"
#include "AudioUtil.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>
#include <GuiInterface/IToGui.h>

#include <ptop_src/ptop_engine_src/Plugins/PluginBase.h>
#include <ptop_src/ptop_engine_src/Plugins/PluginMgr.h>

#include <libjpg/VxJpgLib.h>
#include <VxVideoLib/VxVideoLib.h>
#include <VxVideoLib/VxRescaleRgb.h>

#include <MediaToolsLib/MediaTools.h>
#include <MediaToolsLib/OpusAudioEncoder.h>

#include <PktLib/PktsVideoFeed.h>
#include <PktLib/PktVoiceReq.h>
#include <PktLib/PktVoiceReply.h>

#include <CoreLib/VxGlobals.h>
#include <CoreLib/VxFileUtil.h>
#include <CoreLib/VxTimer.h>
#include <CoreLib/StdMinMaxForWindows.h>

#include <stdlib.h>

#include <algorithm> 

// requried due to some wierd microsoft std::min / max issue
using namespace std;

//#define TEST_JPG_SPEED 1
#ifdef TEST_JPG_SPEED
void testJpgSpeed( void );
#endif // TEST_JPG_SPEED
//#define DEBUG_AUDIO_PROCESSOR_LOCK
//#define LOG_JPG_SIZE 1

namespace
{
	const int VIDEO_DATA_BYTE_CNT					    = (320*240*3);
	const int VIDEO_SENSITIVITY_DIVISOR				    = (320*240*3*64);
	const int VIDEO_MAX_MOTION_VALUE				    = 100000;
	const int MAX_PIC_PKT_DATA_PAYLOAD					= MAX_PKT_LEN - ( sizeof(PktVideoFeedPic) + 16 );

	//============================================================================
    static void * AudioInProcessThreadFunc( void * pvContext )
	{
		VxThread * poThread = (VxThread *)pvContext;
		poThread->setIsThreadRunning( true );
		MediaProcessor * processor = (MediaProcessor *)poThread->getThreadUserParam();
        if( processor )
        {
            processor->processAudioInThreaded();
        }

		poThread->threadAboutToExit();
        return nullptr;
	}

	//============================================================================
    static void * VideoProcessThreadFunc( void * pvContext )
	{
		VxThread * poThread = (VxThread *)pvContext;
		poThread->setIsThreadRunning( true );
		MediaProcessor * processor = (MediaProcessor *)poThread->getThreadUserParam();
        if( processor && false == poThread->isAborted() )
        {
            processor->processVideoIn();
        }

		poThread->threadAboutToExit();
        return nullptr;
	}
};

//============================================================================
MediaProcessor::MediaProcessor( P2PEngine& engine )
: m_Engine( engine )
, m_PluginMgr( engine.getPluginMgr() )
, m_MediaTools( * ( new MediaTools( engine, *this ) ) )
, m_MixerBufUsed( false )
, m_MuteSpeaker( false )
, m_VidCaptureEnabled( false )
, m_MicCaptureEnabled( false )
, m_SpeakerOutputEnabled( false )
, m_VidPktListContainsMyId( false )
{
	memset( m_QuietAudioBuf, 0, sizeof( m_QuietAudioBuf ) );
	memset( m_MixerBuf, 0, sizeof( m_MixerBuf ) );
	
	m_PktVideoFeedPic = PktVideoFeedPic::allocateNewWithMaximumSizeAJpgCanBe();
	m_PktVideoFeedPic->setWidth( 320 );
	m_PktVideoFeedPic->setHeight( 240 );
	m_PktVideoFeedPic->setBitsPerPixel( 24 );
	m_PktVideoFeedPic->setPicType( 1 );

	int maxJpgSize = VIDEO_DATA_BYTE_CNT + 1024; // JPG header should only be less that 32 bytes but just in case because is variable and compression may be poor
	int maxChunkPayloadNeeded = maxJpgSize - MAX_PIC_PKT_DATA_PAYLOAD;
	int maxPicChunkPayload = MAX_PIC_CHUNK_LEN;
	int maxPicChuncsRequired = maxChunkPayloadNeeded / maxPicChunkPayload + ((maxChunkPayloadNeeded % maxPicChunkPayload) ? 1 : 0);
	for( int i = 0; i < maxPicChuncsRequired; i++ )
	{
		m_VidChunkList.push_back( new PktVideoFeedPicChunk() );
	}

	m_ProcessAudioInThread.startThread( (VX_THREAD_FUNCTION_T)AudioInProcessThreadFunc, this, "AudioInProcessor" );
	m_ProcessVideoThread.startThread( (VX_THREAD_FUNCTION_T)VideoProcessThreadFunc, this, "VideoProcessor" );
}

//============================================================================
MediaProcessor::~MediaProcessor()
{
	delete &m_MediaTools;
	delete m_PktVideoFeedPic;
	std::vector<PktVideoFeedPicChunk *>::iterator iter;
	for( iter = m_VidChunkList.begin(); iter != m_VidChunkList.end(); ++iter )
	{
		delete *iter;
	}
}

//============================================================================
void MediaProcessor::shutdownMediaProcessor( void )
{
	m_ProcessAudioInThread.abortThreadRun( true );
	m_AudioInSemaphore.signal();
	m_ProcessVideoThread.abortThreadRun( true );
	m_VideoSemaphore.signal();
}

//============================================================================
void MediaProcessor::playAudio( int16_t * pcmData, int dataLenInBytes )
{
	if( MIXER_CHUNK_LEN_BYTES == dataLenInBytes )
	{
		#ifdef DEBUG_AUDIO_PROCESSOR_LOCK
			LogMsg( LOG_INFO, "MediaProcessor::playAudio m_MixerBufferMutex.lock()" );
		#endif // DEBUG_AUDIO_PROCESSOR_LOCK
		m_MixerBufferMutex.lock();
		if( m_MixerBufUsed )
		{
			// data already exists in buffer.. do mixing
#if 0
			int samples = dataLenInBytes >> 1;
			int pcmVal;
			int pcmMinVal = S16_MINVAL;
			int pcmMaxVal = S16_MAXVAL;
			for( int i = 0; i < samples; i++ )
			{
				pcmVal = (int)m_MixerBuf[i] + (int)pcmData[i];
				m_MixerBuf[i] = (int16_t)pcmVal;
				if( pcmVal < pcmMinVal )
				{
					m_MixerBuf[i] = S16_MINVAL;
				}
				else if( pcmVal > pcmMaxVal )
				{
					m_MixerBuf[i] = S16_MAXVAL;
				}
			}
#else
			AudioUtil::mixPcmAudio( pcmData, m_MixerBuf, MIXER_CHUNK_LEN_SAMPLES );
#endif// 0
		}
		else
		{
			memcpy( m_MixerBuf, pcmData, dataLenInBytes );
			m_MixerBufUsed = true;
		}
		
		#ifdef DEBUG_AUDIO_PROCESSOR_LOCK
			LogMsg( LOG_INFO, "MediaProcessor::playAudio m_MixerBufferMutex.unlock()" );
		#endif // DEBUG_AUDIO_PROCESSOR_LOCK
		m_MixerBufferMutex.unlock();
	}
	else
	{
		LogMsg( LOG_ERROR, "MediaProcessor::playAudio wrong length %d", dataLenInBytes );
	}
}

//============================================================================
void MediaProcessor::processAudioInThreaded( void )
{
	while( false == m_ProcessAudioInThread.isAborted() )
	{
		m_AudioInSemaphore.wait();
		if( m_ProcessAudioInThread.isAborted() )
		{
			LogMsg( LOG_INFO, "MediaProcessor::processAudioIn aborting1" );
			break;
		}

		while( m_ProcessAudioQue.size() )
		{
			m_AudioQueInMutex.lock();
			RawAudio * rawAudio = m_ProcessAudioQue[0];
			m_ProcessAudioQue.erase( m_ProcessAudioQue.begin() );
			m_AudioQueInMutex.unlock();

			processRawAudioIn( rawAudio );
			delete rawAudio;

			if( m_ProcessAudioInThread.isAborted() )
			{
				LogMsg( LOG_INFO, "MediaProcessor::processAudioIn aborting2" );
				break;
			}
		}

		if( m_ProcessAudioInThread.isAborted() )
		{
			LogMsg( LOG_INFO, "MediaProcessor::processAudioIn aborting3" );
			break;
		}
	}
		
	LogMsg( LOG_INFO, "MediaProcessor::processAudioIn leaving function" );
}

//============================================================================
void MediaProcessor::processRawAudioIn( RawAudio * rawAudio )
{
	int16_t * pcmData		    = rawAudio->m_PcmData;
	uint16_t  pcmDataLen		= rawAudio->m_PcmDataLen;
	// PCM data len = 1280 at 8000 HZ sampling = 640 samples = 80ms of sound
	// it seams that microphone volume is a bit low.. especially on android so increase volume before processing
    // TODO microphone boost

	if( m_AudioPcmList.size() )
	{
		#ifdef DEBUG_AUDIO_PROCESSOR_LOCK
		LogMsg( LOG_INFO, "processRawAudioIn m_AudioPcmList AudioProcessorLock" );
		#endif // DEBUG_AUDIO_PROCESSOR_LOCK
		AudioProcessorLock mgrMutexLock( this );
		doAudioClientRemovals( m_AudioClientRemoveList );
		#ifdef DEBUG_AUDIO_PROCESSOR_LOCK
			LogMsg( LOG_INFO, "processRawAudioIn m_AudioPcmList begin" );
		#endif // DEBUG_AUDIO_PROCESSOR_LOCK
		for( auto& client : m_AudioPcmList )
		{
			client.m_Callback->callbackPcm( client.m_UserData, m_Engine.getMyOnlineId(), pcmData, pcmDataLen );
		}

		#ifdef DEBUG_AUDIO_PROCESSOR_LOCK
			LogMsg( LOG_INFO, "processRawAudioIn m_AudioPcmList done" );
		#endif // DEBUG_AUDIO_PROCESSOR_LOCK
	}

	if( m_AudioOpusList.size() || m_AudioPktsList.size() )
	{
		int32_t frame1Len;
		int32_t frame2Len;
		m_MediaTools.getAudioEncoder().encodePcmData( pcmData, pcmDataLen, m_PktVoiceReq.getCompressedData(), frame1Len, frame2Len );
		if( m_AudioOpusList.size() )
		{
			#ifdef DEBUG_AUDIO_PROCESSOR_LOCK
			LogMsg( LOG_INFO, "processRawAudioIn m_AudioOpusList AudioProcessorLock" );
			#endif // DEBUG_AUDIO_PROCESSOR_LOCK
			AudioProcessorLock mgrMutexLock( this );
			doAudioClientRemovals( m_AudioClientRemoveList );
			#ifdef DEBUG_AUDIO_PROCESSOR_LOCK
						LogMsg( LOG_INFO, "processRawAudioIn m_AudioOpusList start" );
			#endif // DEBUG_AUDIO_PROCESSOR_LOCK

			for( auto& client : m_AudioOpusList )
			{
				client.m_Callback->callbackOpusEncoded( client.m_UserData, m_PktVoiceReq.getCompressedData(), frame1Len, frame2Len );
			}

			#ifdef DEBUG_AUDIO_PROCESSOR_LOCK
				LogMsg( LOG_INFO, "processRawAudioIn m_AudioOpusList done" );
			#endif // DEBUG_AUDIO_PROCESSOR_LOCK
		}

		if( m_AudioPktsList.size() )
		{
			m_PktVoiceReq.setFrame1Len( (uint16_t)frame1Len );
			m_PktVoiceReq.setFrame2Len( (uint16_t)frame2Len );
			m_PktVoiceReq.setTimeMs( GetGmtTimeMs() );
			m_PktVoiceReq.calcPktLen();

			#ifdef DEBUG_AUDIO_PROCESSOR_LOCK
				LogMsg( LOG_INFO, "processRawAudioIn m_AudioPktsList AudioProcessorLock callbackOpusPkt" );
			#endif // DEBUG_AUDIO_PROCESSOR_LOCK
			AudioProcessorLock mgrMutexLock( this );
			doAudioClientRemovals( m_AudioClientRemoveList );
			#ifdef DEBUG_AUDIO_PROCESSOR_LOCK
				LogMsg( LOG_INFO, "processRawAudioIn m_AudioPktsList start" );
			#endif // DEBUG_AUDIO_PROCESSOR_LOCK
			for( auto& client : m_AudioPktsList )
			{
				client.m_Callback->callbackOpusPkt( client.m_UserData, &m_PktVoiceReq );
			}

			#ifdef DEBUG_AUDIO_PROCESSOR_LOCK
				LogMsg( LOG_INFO, "processRawAudioIn m_AudioPktsList done" );
			#endif // DEBUG_AUDIO_PROCESSOR_LOCK
		}
	}
}

//============================================================================
void MediaProcessor::increasePcmSampleVolume( int16_t * pcmData, uint16_t pcmDataLen, float volumePercent0To100 )
{
	int pcmSamples = pcmDataLen >> 1;
	float volMultiplier = 1.0f +  ( volumePercent0To100 / 100.0f );
	int sndVal;
	for( int i = 0; i < pcmSamples; i++ )
	{
		sndVal = (int)(( float )pcmData[i] * volMultiplier);
		if( sndVal > S16_MAXVAL )
		{
			pcmData[i] = S16_MAXVAL;
		}
		else if( sndVal < S16_MINVAL )
		{
			pcmData[i] = S16_MINVAL;
		}
		else
		{
			pcmData[i] = (int16_t)sndVal;
		}
	}
}

//============================================================================
void MediaProcessor::processFriendAudioFeed( VxGUID& onlineId, int16_t * pcmData, uint16_t pcmDataLen, bool dontLock )
{
	if( m_AudioPcmList.size() )
	{
		std::vector<MediaClient>::iterator iter;
		if( !dontLock )
		{
			#ifdef DEBUG_AUDIO_PROCESSOR_LOCK
				LogMsg( LOG_INFO, "processFriendAudioFeed m_AudioMutex.lock()" );
			#endif // DEBUG_AUDIO_PROCESSOR_LOCK
			m_AudioMutex.lock();
			#ifdef DEBUG_AUDIO_PROCESSOR_LOCK
				LogMsg( LOG_INFO, "processFriendAudioFeed begin" );
			#endif // DEBUG_AUDIO_PROCESSOR_LOCK
		}

		for( iter = m_AudioPcmList.begin(); iter != m_AudioPcmList.end(); ++iter )
		{
			MediaClient& client = (*iter);
			client.m_Callback->callbackPcm( client.m_UserData, onlineId, pcmData, pcmDataLen );
		}

		if( !dontLock )
		{
			#ifdef DEBUG_AUDIO_PROCESSOR_LOCK
				LogMsg( LOG_INFO, "processFriendAudioFeed m_AudioMutex.unlock()" );
			#endif // DEBUG_AUDIO_PROCESSOR_LOCK
			m_AudioMutex.unlock();
		}
	}
}

//============================================================================
void MediaProcessor::fromGuiVideoData( uint32_t u32FourCc, uint8_t * pu8VidDataIn, int imageWidth, int imageHeight, uint32_t u32VidDataLen, int iRotation )
{
	if( false == m_VidCaptureEnabled )
	{
		// nobody wants it 
		//LogMsg( LOG_INFO, "PluginMgr::fromGuiVideoData not wanted\n" );
		m_VideoSemaphore.signal();
		return;
	}

	if( m_ProcessVideoQue.size() > 4 )
	{
		//LogMsg( LOG_INFO, "WARNING MediaProcessor::fromGuiVideoData dropping video because que size %d\n", m_ProcessVideoQue.size() );
		m_VideoSemaphore.signal();
		return;
	}
	
	//static VxTimer elapsedTimer;
	//LogMsg( LOG_INFO, "MediaProcessor::fromGuiVideoData %3.3fms rot %d\n", elapsedTimer.elapsedMs(), iRotation );
	//elapsedTimer.startTimer();
	//LogMsg( LOG_INFO, "fromGuiVideoData  FourCc 0x%x width %d height %d len %d\n", u32FourCc, iWidth, iHeight, u32VidDataLen );

	uint8_t * pu8VidData = pu8VidDataIn;
	bool bConvert =  ( FOURCC_RGB != u32FourCc );

// windows no longer needs this when source is from QT instead of raw vid capture
//#ifdef TARGET_OS_WINDOWS
//	if( FOURCC_RGB == u32FourCc )
//	{
//		// windows the red and blue are swapped and image is upside down convert anyway.. converter will know what to do
//		u32FourCc = FOURCC_BGR;
//		bConvert = true;
//		iRotation += 180;
//	}
//#endif // TARGET_OS_WINDOWS

	if( bConvert )
	{
		if( FOURCC_YUVS == u32FourCc )
		{
			// optimize convert for android .. could use normal converter because it is supported
			uint8_t * yuv420sp = pu8VidData;
			pu8VidData = new uint8_t[ imageWidth * imageHeight * 3 ];
			uint8_t * rgb = pu8VidData;
			int frameSize = imageWidth * imageHeight;
			for( int j = 0, yp = 0; j < imageHeight; j++ )
			{
				int uvp = frameSize + (j >> 1) * imageWidth, u = 0, v = 0;
				for( int i = 0; i < imageWidth; i++, yp++ )
				{
					int y = (0xff & ((int) yuv420sp[yp])) - 16;
					if (y < 0) y = 0;
					if ((i & 1) == 0) 
					{
						v = (0xff & yuv420sp[uvp++]) - 128;
						u = (0xff & yuv420sp[uvp++]) - 128;
					}

					int y1192 = 1192 * y;
					int r = (y1192 + 1634 * v);
					int g = (y1192 - 833 * v - 400 * u);
					int b = (y1192 + 2066 * u);

					if (r < 0) r = 0; else if (r > 262143) r = 262143;
					if (g < 0) g = 0; else if (g > 262143) g = 262143;
					if (b < 0) b = 0; else if (b > 262143) b = 262143;

					*rgb = ( r >> 10 ) & 0xff;
					rgb++;
					*rgb = ( g >> 10 ) & 0xff;
					rgb++;
					*rgb = (b>>10)&0xff;
					rgb++;
				}
			}
		}
		else
		{
			// need to convert to rgb.. NOTE: the caller must delete the returned data
			pu8VidData = VxConvertImage(	u32FourCc,				// FOURCC of format to convert
											pu8VidDataIn,			// data to convert
											imageWidth,					// width of image in pixels
											imageHeight,				// height of image in pixels
											FOURCC_RGB,				// FOURCC of format to convert to 
											u32VidDataLen );		// return data length of converted image

		}

		// if we converted the image we also changed the width and height to our cam dimensions for processing
	}

	RawVideo * rawVideo = new RawVideo( FOURCC_RGB,
										pu8VidData,
										u32VidDataLen, 
										imageWidth,
										imageHeight,
										iRotation,
										bConvert );
	#ifdef DEBUG_PROCESSOR_LOCK
		LogMsg( LOG_INFO, "fromGuiVideoData m_VideoQueInMutex.lock()" );
	#endif // DEBUG_PROCESSOR_LOCK
	m_VideoQueInMutex.lock();
	m_ProcessVideoQue.push_back( rawVideo );
	#ifdef DEBUG_PROCESSOR_LOCK
		LogMsg( LOG_INFO, "fromGuiVideoData m_VideoQueInMutex.unlock()" );
	#endif // DEBUG_PROCESSOR_LOCK
	m_VideoQueInMutex.unlock();
	m_VideoSemaphore.signal();
}

//========================================================================
// Convert a single YUV pixel to RGB. 
static void yuvToRgb( uint8_t * yuvData, int outOffset, /*out*/uint8_t * rgbOut) 
{ 
	static float COLOR_MAX = 255.0f; 

	float y		= (float)yuvData[0]; // Y channel 
	float cb	= (float)yuvData[1]; // U channel 
	float cr	= (float)yuvData[2]; // V channel 

	// convert YUV -> RGB (from JFIF's "Conversion to and from RGB" section) 
	float r = y + 1.402f * (cr - 128.0f); 
	float g = y - 0.34414f * (cb - 128.0f) - 0.71414f * (cr - 128.0f); 
	float b = y + 1.772f * (cb - 128.0f); 

	// clamp to [0,255] 
    rgbOut[ outOffset ] = ( uint8_t )max( 0.0f, min( COLOR_MAX, r ) );
    rgbOut[ outOffset + 1 ] = ( uint8_t )max( 0.0f, min( COLOR_MAX, g ) );
    rgbOut[ outOffset + 2 ] = ( uint8_t )max( 0.0f, min( COLOR_MAX, b ) );
}

//============================================================================
void MediaProcessor::fromGuiYUV420CaptureImage(	uint8_t * yBytes, uint8_t * uBytes, uint8_t * vBytes, 
												int yRowStride, int uRowStride, int vRowStride,
												int yPixStride, int uPixStride, int vPixStride,
												int imageWidth, int imageHeight, int imageRotation )
{
	if( false == m_VidCaptureEnabled )
	{
		//LogMsg( LOG_INFO, "PluginMgr::fromGuiVideoData not wanted\n" );
		m_VideoSemaphore.signal();
		return;
	}

	if( m_ProcessVideoQue.size() > 4 )
	{
		//LogMsg( LOG_INFO, "WARNING MediaProcessor::fromGuiVideoData dropping video because que size %d\n", m_ProcessVideoQue.size() );
		m_VideoSemaphore.signal();
		return;
	}

	static int BYTES_PER_RGB_PIX = 3;
	int rgbDataLen = BYTES_PER_RGB_PIX * imageWidth * imageHeight;
	uint8_t * rgbData = new uint8_t[ rgbDataLen ];

	uint8_t yuvPixel[] = { 0, 0, 0 }; 
	uint8_t * yFullRow = new uint8_t[yPixStride * (imageWidth - 1) + 1]; 
	uint8_t * uFullRow = new uint8_t[uPixStride * (imageWidth / 2 - 1) + 1]; 
	uint8_t * vFullRow = new uint8_t[vPixStride * (imageWidth / 2 - 1) + 1]; 
	uint8_t * finalRow = rgbData; 
	for( int i = 0; i < imageHeight; i++ ) 
	{ 
		int halfH = i / 2; 
		memcpy( yFullRow, &yBytes[yRowStride * i], yPixStride * (imageWidth - 1) + 1 );
		memcpy( uFullRow, &uBytes[uRowStride * halfH], uPixStride * (imageWidth / 2 - 1) + 1 );
		memcpy( vFullRow, &vBytes[vRowStride * halfH], vPixStride * (imageWidth / 2 - 1) + 1 );
		for (int j = 0; j < imageWidth; j++) 
		{ 
			int halfW = j / 2; 
			yuvPixel[0] = yFullRow[yPixStride * j]; 
			yuvPixel[1] = uFullRow[uPixStride * halfW]; 
			yuvPixel[2] = vFullRow[vPixStride * halfW]; 
			yuvToRgb(yuvPixel, j * BYTES_PER_RGB_PIX, /*out*/finalRow); 
		} 

		finalRow += BYTES_PER_RGB_PIX * imageWidth; 
	} 

	RawVideo * rawVideo = new RawVideo( FOURCC_RGB,
										rgbData,
										rgbDataLen, 
										imageWidth,
										imageHeight,
										imageRotation,
										false );

	//#ifdef DEBUG_PROCESSOR_LOCK
	//	LogMsg( LOG_INFO, "fromGuiVideoData m_VideoQueInMutex.lock()\n" );
	//#endif // DEBUG_PROCESSOR_LOCK
	m_VideoQueInMutex.lock();
	m_ProcessVideoQue.push_back( rawVideo );
	//#ifdef DEBUG_PROCESSOR_LOCK
	//	LogMsg( LOG_INFO, "fromGuiVideoData m_VideoQueInMutex.unlock()\n" );
	//#endif // DEBUG_PROCESSOR_LOCK
	m_VideoQueInMutex.unlock();
	m_VideoSemaphore.signal();

	delete[] yFullRow; 
	delete[] uFullRow;
	delete[] vFullRow;
}

//============================================================================
void testJpgSpeed( void )
{
	void * retBuf = 0;
	uint32_t lenOfData = 0;
#ifdef TARGET_OS_WINDOWS
	if( 0 != VxFileUtil::readWholeFile( "f:\\rawBmp1.raw", &retBuf, &lenOfData ) )
	{
		LogMsg( LOG_INFO, "Fail read raw bmp " );
		return;
	}
#else
    std::string rawFile = VxGetAppNoLimitDataDirectory();
	rawFile += "rawBmp1.raw";
	if( 0 != VxFileUtil::readWholeFile( rawFile.c_str(), &retBuf, &lenOfData ) )
	{
		LogMsg( LOG_INFO, "Fail read raw bmp %s ", rawFile.c_str() );
		return;
	}
#endif

	if( 0 == lenOfData )
	{
		LogMsg( LOG_INFO, "Fail read raw bmp " );
		return;
	}

	VxTimer compressTimer;
	unsigned char * pu8VidData = (unsigned char *)retBuf;
	unsigned char jpgData[ VIDEO_DATA_BYTE_CNT ];
    //int32_t s32JpgDataLen = 0;
	compressTimer.startTimer();
	long s32JpgDataLen = 0;
	for( int i = 0; i < 1000; i++ )
	{
		s32JpgDataLen = 0;
		// take a guess at what size the jpg will be
		int iMaxJpgSize = VIDEO_DATA_BYTE_CNT;
		RCODE rc = VxBmp2Jpg(	24,							// number of bits each pixel..(For now must be 24)
			pu8VidData,					// bits of bmp to convert
			320,						// width of image in pixels
			240,						// height of image in pixels
			75,		// quality of image
			iMaxJpgSize,				// maximum length of pu8RetJpg
			jpgData,					// buffer to return Jpeg image
			&s32JpgDataLen );			// return length of jpeg image

		if( 0 != rc )
		{
			LogMsg( LOG_INFO, "VxBmp2Jpg ERROR %d", rc );
			break;
		}

		/*
		if( 0 == i )
		{
#ifdef TARGET_OS_WINDOWS
            if( 0 != VxFileUtil::writeWholeFile( "f:\\rawBmpToJpgNew.jpg", jpgData, s32JpgDataLen) )
			{
				LogMsg( LOG_INFO, "Fail write jpg Convert \n" );
				return;
			}
#else
			std::string jpgFile = "/storage/emulated/0/MyP2PWeb/data/";;
			jpgFile += "rawBmpToJpgNew.jpg";
			if( 0 != VxFileUtil::writeWholeFile( jpgFile.c_str(), jpgData, s32JpgDataLen) )
			{
				LogMsg( LOG_INFO, "Fail write jpg Convert \n" );
				return;
			}
#endif
		}*/
	}

	double elapsedSec = compressTimer.elapsedSec();
	//elapsedSec+= 1.0;
	LogMsg( LOG_INFO, "Jpg convert time sec %3.3f size %d ", elapsedSec, s32JpgDataLen );
	//elapsedSec = (elapsedSec -1.0) * 1000;
	LogMsg( LOG_INFO, "Jpg convert time millisec %3.3f ", elapsedSec / 1000, s32JpgDataLen );

}

//============================================================================
void MediaProcessor::processVideoIn( void )
{
	//testJpgSpeed();
	while( false == m_ProcessVideoThread.isAborted() )
	{
		#ifdef DEBUG_PROCESSOR_LOCK
			LogMsg( LOG_INFO, "processVideoIn wait for video semaphore" );
		#endif // DEBUG_PROCESSOR_LOCK
		m_VideoSemaphore.wait();
		if( m_ProcessVideoThread.isAborted() )
		{
			LogMsg( LOG_INFO, "MediaProcessor::processVideoIn aborting1" );
			break;
		}
		else
		{
			VideoProcessorLock mgrMutexLock( this );
			doVideoClientRemovals( m_VideoClientRemoveList );
		}

		while( m_ProcessVideoQue.size() )
		{
			#ifdef DEBUG_PROCESSOR_LOCK
				LogMsg( LOG_INFO, "processVideoIn m_VideoQueInMutex.lock()" );
			#endif // DEBUG_PROCESSOR_LOCK
			m_VideoQueInMutex.lock();
			RawVideo * rawVideo = m_ProcessVideoQue[0];
			m_ProcessVideoQue.erase( m_ProcessVideoQue.begin() );
			#ifdef DEBUG_PROCESSOR_LOCK
				LogMsg( LOG_INFO, "processVideoIn m_VideoQueInMutex.unlock()" );
			#endif // DEBUG_PROCESSOR_LOCK
			m_VideoQueInMutex.unlock();

			processRawVideoIn( rawVideo );
			delete rawVideo;

			if( m_ProcessVideoThread.isAborted() )
			{
				LogMsg( LOG_INFO, "MediaProcessor::processVideoIn aborting2" );
				break;
			}
		}

		if( m_ProcessVideoThread.isAborted() )
		{
			LogMsg( LOG_INFO, "MediaProcessor::processVideoIn aborting3" );
			break;
		}
	}

	LogMsg( LOG_INFO, "MediaProcessor::processVideoIn leaving function" );
}

//============================================================================
void MediaProcessor::processFriendVideoFeed(	VxGUID&			onlineId, 
												uint8_t *		pu8Jpg, 
												uint32_t		jpgDataLen,
												int				motion0To100000 )
{
	m_Engine.callbackVideoJpgSmall( 0, onlineId, pu8Jpg, jpgDataLen, motion0To100000 );
	if( m_VideoJpgSmallList.size() )
	{
		std::vector<MediaClient>::iterator iter;
		#ifdef DEBUG_PROCESSOR_LOCK
			LogMsg( LOG_INFO, "processFriendVideoFeed VideoProcessorLock start" );
		#endif // DEBUG_PROCESSOR_LOCK
		VideoProcessorLock mgrMutexLock( this );
		#ifdef DEBUG_PROCESSOR_LOCK
			LogMsg( LOG_INFO, "processFriendVideoFeed VideoProcessorLock done" );
		#endif // DEBUG_PROCESSOR_LOCK
		doVideoClientRemovals( m_VideoClientRemoveList );

		for( iter = m_VideoJpgSmallList.begin(); iter != m_VideoJpgSmallList.end(); ++iter )
		{
			MediaClient& client = (*iter);
			client.m_Callback->callbackVideoJpgSmall( client.m_UserData, onlineId, pu8Jpg, jpgDataLen, motion0To100000 );
		}

		#ifdef DEBUG_PROCESSOR_LOCK
			LogMsg( LOG_INFO, "processFriendVideoFeed VideoProcessorLock callbacks done" );
		#endif // DEBUG_PROCESSOR_LOCK
	}
};

//============================================================================
void MediaProcessor::processRawVideoIn( RawVideo * rawVideo )
{
	uint8_t * pu8VidData		= rawVideo->m_VidData;
	int iWidth			= rawVideo->m_Width; 
	int iHeight			= rawVideo->m_Height; 
	int iRotation		= rawVideo->m_Rotation; 

	bool bResize = (( 320 != iWidth ) || ( 240 != iHeight ));
	bool bDidLargePhoto = bResize && m_VideoJpgBigList.size();
	if( bDidLargePhoto )
	{
		if( 0 != iRotation )
		{
			// need to rotate
			unsigned char * pu8DataToDelete = pu8VidData;
			pu8VidData = VxRotateRgbImage(	pu8VidData, 
											iWidth, 
											iHeight, 
											iRotation );
			delete pu8DataToDelete;
			iRotation = 0;
		}

		// compress from rgb to jpg for sending
		long s32JpgDataLen = 0;
		// take a guess at what size the jpg will be
		int iMaxJpgSize = iWidth * iHeight * 3;

		uint8_t * pu8JpgData = new uint8_t[ iMaxJpgSize ];
        VxBmp2Jpg(	24,						// number of bits each pixel..(For now must be 24)
					pu8VidData,				// bits of bmp to convert
					iWidth,					// width of image in pixels
					iHeight,				// height of image in pixels
					JPG_CONVERT_QUALITY,	// quality of image
					iMaxJpgSize,			// maximum length of pu8RetJpg
					pu8JpgData,				// buffer to return Jpeg image
					&s32JpgDataLen );		// return length of jpeg image

		#if defined( LOG_JPG_SIZE )
				{
					LogMsg( LOG_VERBOSE, "VxBmp2Jpg processRawVideoIn size %d", s32JpgDataLen );
				}
		#endif // defined( LOG_JPG_SIZE )

		#ifdef DEBUG_PROCESSOR_LOCK
		LogMsg( LOG_INFO, "VxBmp2Jpg AutoProcessorLock" );
		#endif // DEBUG_PROCESSOR_LOCK
		VideoProcessorLock mgrMutexLock( this );
		doVideoClientRemovals( m_VideoClientRemoveList );
		for( auto iter = m_VideoJpgBigList.begin(); iter != m_VideoJpgBigList.end(); ++iter )
		{
			MediaClient& client = (*iter);
			client.m_Callback->callbackVideoJpgBig( client.m_UserData, m_Engine.getMyOnlineId(), pu8JpgData, s32JpgDataLen );
		}

		delete[] pu8JpgData;

		if( ( 0 == m_VideoJpgSmallList.size() )
			&& ( 0 == m_VideoPktsList.size() ) )
		{
			// we are done
			delete[] pu8VidData;
			return;
		}
	}

	if( bResize )
	{
		// scale to 320x200.. the reason we don't use the GdvBufferUtil to scale at same time as convert is that
		// it looks like crap. here we use averaging
		unsigned char * pu8DataToDelete = pu8VidData;
		pu8VidData = VxResizeRgbImage(	pu8VidData, 
			iWidth, 
			iHeight, 
			320, 
			240,
			iRotation );
		iWidth = 320;
		iHeight = 240;
		delete pu8DataToDelete;
	}
	else if( 0 != iRotation )
	{
		// need to rotate
		unsigned char * pu8DataToDelete = pu8VidData;
		pu8VidData = VxResizeRgbImage(	pu8VidData, 
			                            iWidth, 
			                            iHeight, 
			                            320, 
			                            240,
			                            iRotation );
		delete[] pu8DataToDelete;
	}

	static int8_t motionBuf[ VIDEO_DATA_BYTE_CNT ];
	int8_t * cmpData = (int8_t *)pu8VidData;
    int32_t vidDiff = 0;
	for( int i = 0; i < VIDEO_DATA_BYTE_CNT; i++ )
	{
		vidDiff += cmpData[i] > motionBuf[i] ? ( cmpData[i] - motionBuf[i] ) : ( motionBuf[i] - cmpData[i] ); 
	}

	memcpy( motionBuf, pu8VidData, VIDEO_DATA_BYTE_CNT );
	double difNormalized = ((double)vidDiff * VIDEO_MAX_MOTION_VALUE) / ( VIDEO_SENSITIVITY_DIVISOR );
	int motion0To100000 = difNormalized > VIDEO_MAX_MOTION_VALUE ? VIDEO_MAX_MOTION_VALUE : (int)difNormalized;

	// compress from rgb to jpg for sending
	long s32JpgDataLen = 0;
	// take a guess at what size the jpg will be
	int iMaxJpgSize = VIDEO_DATA_BYTE_CNT;
	RCODE rc = VxBmp2Jpg(	24,							// number of bits each pixel..(For now must be 24)
						     pu8VidData,					// bits of bmp to convert
						     iWidth,						// width of image in pixels
						     iHeight,						// height of image in pixels
						     m_VideoJpgBigList.size() ? 100 : JPG_CONVERT_QUALITY,							// quality of image
						     iMaxJpgSize,					// maximum length of pu8RetJpg
						     m_PktVideoFeedPic->getDataPayload(),	// buffer to return Jpeg image
						     &s32JpgDataLen );				// return length of jpeg image
	#if defined( LOG_JPG_SIZE )
		LogMsg( LOG_VERBOSE, "VxBmp2Jpg processRawVideoIn size %d", s32JpgDataLen );
	#endif //defined( LOG_JPG_SIZE )

	delete[] pu8VidData;
	if( 0 == rc )
	{
		//LogMsg( LOG_INFO, "PluginMgr::fromGuiVideoData width %d height %d len %d compressed %d\n", iWidth, iHeight, u32VidDataLen, s32JpgDataLen  );
		//writeWholeFile( "Test.jpg", pu8JpgData, s32JpgDataLen );
		if( !bDidLargePhoto && m_VideoJpgBigList.size() )
		{
			// clients wanted large but the video capture was small so we have to give them small
			std::vector<MediaClient>::iterator iter;
			#ifdef DEBUG_PROCESSOR_LOCK
			LogMsg( LOG_INFO, "m_VideoJpgBigList VideoProcessorLock" );
			#endif // DEBUG_PROCESSOR_LOCK
			VideoProcessorLock mgrMutexLock( this );
			doVideoClientRemovals( m_VideoClientRemoveList );

			for( iter = m_VideoJpgBigList.begin(); iter != m_VideoJpgBigList.end(); ++iter )
			{
				MediaClient& client = (*iter);
				client.m_Callback->callbackVideoJpgBig( client.m_UserData, m_Engine.getMyOnlineId(), m_PktVideoFeedPic->getDataPayload(), s32JpgDataLen );
			}
		}

		if( m_VideoJpgSmallList.size() )
		{
			std::vector<MediaClient>::iterator iter;
			#ifdef DEBUG_PROCESSOR_LOCK
			LogMsg( LOG_INFO, "m_VideoJpgSmallList VideoProcessorLock start" );
			#endif // DEBUG_PROCESSOR_LOCK
			VideoProcessorLock mgrMutexLock( this );
			doVideoClientRemovals( m_VideoClientRemoveList );

			#ifdef DEBUG_PROCESSOR_LOCK
			LogMsg( LOG_INFO, "m_VideoJpgSmallList VideoProcessorLock done" );
			#endif // DEBUG_PROCESSOR_LOCK
			for( iter = m_VideoJpgSmallList.begin(); iter != m_VideoJpgSmallList.end(); ++iter )
			{
				MediaClient& client = (*iter);
				client.m_Callback->callbackVideoJpgSmall( client.m_UserData, m_Engine.getMyOnlineId(), m_PktVideoFeedPic->getDataPayload(), s32JpgDataLen, motion0To100000 );
			}

			#ifdef DEBUG_PROCESSOR_LOCK
			LogMsg( LOG_INFO, "m_VideoJpgSmallList VideoProcessorLock callbacks done" );
			#endif // DEBUG_PROCESSOR_LOCK
		}

		if( m_VidPktListContainsMyId && m_VideoPktsList.size() )
		{
			int32_t picPktDataLen		= s32JpgDataLen > MAX_PIC_PKT_DATA_PAYLOAD ? MAX_PIC_PKT_DATA_PAYLOAD : s32JpgDataLen;
			int32_t dataOverflow		= s32JpgDataLen > MAX_PIC_PKT_DATA_PAYLOAD ? s32JpgDataLen - MAX_PIC_PKT_DATA_PAYLOAD : 0;
			int32_t chunkPktsRequired	= dataOverflow / MAX_PIC_CHUNK_LEN + ((dataOverflow % MAX_PIC_CHUNK_LEN)?1:0);
			m_PktVideoFeedPic->setThisDataLen( picPktDataLen );
			m_PktVideoFeedPic->setTotalDataLen( s32JpgDataLen );
			m_PktVideoFeedPic->setTimeStampMs( GetGmtTimeMs() );
			m_PktVideoFeedPic->setMotionDetect( motion0To100000 );
			m_PktVideoFeedPic->setTotalPktsInSeq( 1 + chunkPktsRequired );
			m_PktVideoFeedPic->setPktSeqNum( 1 );
			m_PktVideoFeedPic->calcPktLen();

			std::vector<MediaClient>::iterator iter;
			#ifdef DEBUG_PROCESSOR_LOCK
			LogMsg( LOG_INFO, "m_VideoPktsList VideoProcessorLock" );
			#endif // DEBUG_PROCESSOR_LOCK
			VideoProcessorLock mgrMutexLock( this );
			doVideoClientRemovals( m_VideoClientRemoveList );
			for( iter = m_VideoPktsList.begin(); iter != m_VideoPktsList.end(); ++iter )
			{
				MediaClient& client = (*iter);
				client.m_Callback->callbackVideoPktPic( client.m_UserData, m_Engine.getMyOnlineId(), m_PktVideoFeedPic, 1 + chunkPktsRequired, 1  );
			}

			if( chunkPktsRequired )
			{
				int curDataIdx = MAX_PIC_PKT_DATA_PAYLOAD;
				int dataLeftToSend = dataOverflow;
				for( int i = 0; i < chunkPktsRequired; i++ )
				{
					PktVideoFeedPicChunk * pktChunk = m_VidChunkList[i];
                    int32_t dataThisChunk = dataLeftToSend > (int)MAX_PIC_CHUNK_LEN ? (int)MAX_PIC_CHUNK_LEN : dataLeftToSend;
					memcpy( pktChunk->getDataPayload(), &(m_PktVideoFeedPic->getDataPayload()[curDataIdx]), dataThisChunk );
					pktChunk->setThisDataLen( dataThisChunk );
					pktChunk->setPktSeqNum( 2 + i );
					pktChunk->setTotalPktsInSeq( chunkPktsRequired + 1 );
					pktChunk->calcPktLen();
					for( iter = m_VideoPktsList.begin(); iter != m_VideoPktsList.end(); ++iter )
					{
						MediaClient& client = (*iter);
						client.m_Callback->callbackVideoPktPicChunk( client.m_UserData, m_Engine.getMyOnlineId(), pktChunk, chunkPktsRequired + 1, 2 + i );
					}

					curDataIdx += dataThisChunk;
					dataLeftToSend -= dataThisChunk;
				}
			}

#ifdef DEBUG_PROCESSOR_LOCK
			LogMsg( LOG_INFO, "m_VideoPktsList callbacks done" );
#endif // DEBUG_PROCESSOR_LOCK
		}
	}
	else
	{
		LogMsg( LOG_ERROR, "PluginMgr::fromGuiVideoData: JPEG Conversion error %d", rc );
	}

	//LogMsg( LOG_INFO, "PluginMgr::fromGuiVideoData done\n" );
}

//============================================================================
void MediaProcessor::wantAppIdle( EPluginType ePluginType, bool bWantAppIdle )
{
	if( bWantAppIdle )
	{
		LogMsg( LOG_INFO, "PluginMgr::pluginApiWantAppIdle anding want idle plugin %d", ePluginType );
		m_aoWantAppIdle.push_back( m_PluginMgr.getPlugin( ePluginType ) );
		if( 1 == m_aoWantAppIdle.size() )
		{
			LogMsg( LOG_INFO, "PluginMgr::pluginApiWantAppIdle calling java to start idle" );
			// idle runs all the time.. TODO remove all idle code ?
			//IToGui::getToGui().toGuiStartAppIdle();
		}
	}
	else
	{
		std::vector<PluginBase * >::iterator iter;
		for( iter = m_aoWantAppIdle.begin(); iter != m_aoWantAppIdle.end(); ++iter )
		{
			if( ePluginType == (*iter)->getPluginType() )
			{
				m_aoWantAppIdle.erase(iter);
				break;
			}
		}
		if( 0 == m_aoWantAppIdle.size() )
		{
			LogMsg( LOG_INFO, "PluginMgr::pluginApiWantAppIdle calling java to stop idle" );
			//IToGui::getToGui().toGuiStopAppIdle();
		}
	}
}

//============================================================================
bool MediaProcessor::isAudioMediaType( EMediaInputType mediaType )
{
	switch( mediaType )
	{
	case eMediaInputAudioPkts:
	case eMediaInputAudioOpus:
	case eMediaInputAudioPcm:
	case eMediaInputMixer:
		return true;
	default:
		return false;
	}
}

//============================================================================
void MediaProcessor::wantMediaInput(	EMediaInputType				mediaType, 
										MediaCallbackInterface *	callback, 
										void *						userData, 
										EAppModule					appModule,
										bool						wantInput )
{
	if( isAudioMediaType( mediaType ) )
	{
		if( eMediaInputMixer == mediaType )
		{
			wantMixerMediaInput( mediaType, callback, userData, appModule, wantInput );
		}
		else
		{
			wantAudioMediaInput( mediaType, callback, userData, appModule, wantInput );
		}
	}
	else
	{
		wantVideoMediaInput( mediaType, callback, userData, appModule, wantInput );
	}
}

//============================================================================
bool MediaProcessor::clientExistsInList(	std::vector<MediaClient>&		clientList, 
											EMediaInputType					mediaType, 
											MediaCallbackInterface *		callback,
											void *							userData )
{
	std::vector<MediaClient>::iterator iter;
	for( iter = clientList.begin(); iter != clientList.end(); ++iter )
	{
		MediaClient& client = *iter;
		if( ( client.m_Callback == callback )
			&& ( client.m_UserData == userData )
			&& ( client.m_MediaInputType == mediaType ) )
		{
			return true;
		}
	}

	return false;
}

//============================================================================
bool MediaProcessor::removeClientFromListist( std::vector<MediaClient>& clientList,
												EMediaInputType					mediaType,
												MediaCallbackInterface* callback,
												void* userData )
{
	std::vector<MediaClient>::iterator iter;
	for( iter = clientList.begin(); iter != clientList.end(); ++iter )
	{
		MediaClient& client = *iter;
		if( (client.m_Callback == callback)
			&& (client.m_UserData == userData)
			&& (client.m_MediaInputType == mediaType) )
		{
			clientList.erase( iter );
			return true;
		}
	}

	return false;
}


//============================================================================
bool MediaProcessor::clientToRemoveExistsInList(	std::vector<ClientToRemove>&	clientRemoveList, 
													EMediaInputType					mediaType, 
													MediaCallbackInterface *		callback,
													void *							userData )
{
	std::vector<ClientToRemove>::iterator iter;
	for( iter = clientRemoveList.begin(); iter != clientRemoveList.end(); ++iter )
	{
		ClientToRemove& client = *iter;
		if( ( client.m_Callback == callback )
			&& ( client.m_UserData == userData )
			&& ( client.m_MediaType == mediaType ) )
		{
			return true;
		}
	}

	return false;
}

//============================================================================
bool MediaProcessor::clientToRemoveRemoveFromList(	std::vector<ClientToRemove>&	clientRemoveList, 
													EMediaInputType					mediaType, 
													MediaCallbackInterface *		callback,
													void *							userData )
{
	bool removed = false;
	std::vector<ClientToRemove>::iterator iter;
	for( iter = clientRemoveList.begin(); iter != clientRemoveList.end(); ++iter )
	{
		ClientToRemove& client = *iter;
		if( ( client.m_Callback == callback )
			&& ( client.m_UserData == userData )
			&& ( client.m_MediaType == mediaType ) )
		{
			clientRemoveList.erase( iter );
			removed = true;
			break;
		}
	}

	return removed;
}

//============================================================================
void MediaProcessor::wantMixerMediaInput(	EMediaInputType				mediaType, 
											MediaCallbackInterface *	callback, 
											void *						userData, 
											EAppModule					appModule,
											bool						wantInput )
{
	if(  false == wantInput )
	{
		// user wants to be removed but is probably being called from a callback function.
		// if we remove not will crash because iterator may expect more items in array
		m_MixerRemoveMutex.lock();
		if( m_SpeakerOutputEnabled && !clientToRemoveExistsInList( m_MixerClientRemoveList, mediaType, callback, userData ) )
		{
			m_MixerClientRemoveList.push_back( ClientToRemove( mediaType, callback, userData, appModule ) );
		}

		m_MixerRemoveMutex.unlock();

		bool stopSpeakerOutput = (m_SpeakerOutputEnabled) && (1 == m_MixerList.size());
		// set capture stated before unlocking mutex
		if( stopSpeakerOutput )
		{
			m_SpeakerOutputEnabled = false;
			LogMsg( LOG_VERBOSE, "MediaProcessor::wantMixerMediaInput stoping speaker output module %s", DescribeAppModule( appModule ) );
			IToGui::getAudioRequests().toGuiWantSpeakerOutput( appModule, false );
			IToGui::getAudioRequests().toGuiWantMicrophoneRecording( appModule, false );
		}
		return;
	}
	else
	{
		// if been commanded to remove and now adding remove the previous command
		m_MixerRemoveMutex.lock();
		bool wasRemoved = clientToRemoveRemoveFromList( m_MixerClientRemoveList, mediaType, callback, userData );
		m_MixerRemoveMutex.unlock();
		if( wasRemoved )
		{
			m_MixerClientsMutex.lock();
			removeClientFromListist( m_MixerList, mediaType, callback, userData );
			m_MixerClientsMutex.unlock();
		}
	}

	#ifdef DEBUG_AUDIO_PROCESSOR_LOCK
		LogMsg( LOG_INFO, "wantAudioMediaInput m_MixerClientsMutex.lock start" );
	#endif // DEBUG_AUDIO_PROCESSOR_LOCK
	m_MixerClientsMutex.lock();
	#ifdef DEBUG_AUDIO_PROCESSOR_LOCK
		LogMsg( LOG_INFO, "wantAudioMediaInput m_MixerClientsMutex.lock done" );
	#endif // DEBUG_AUDIO_PROCESSOR_LOCK

	if( clientExistsInList( m_MixerList, mediaType, callback, userData ) )
	{
		LogMsg( LOG_INFO, "WARNING. Ignoring New Mixer Media Client because already in list" );
		#ifdef DEBUG_AUDIO_PROCESSOR_LOCK
			LogMsg( LOG_INFO, "m_MixerClientsMutex.unlock" );
		#endif // DEBUG_AUDIO_PROCESSOR_LOCK
		m_MixerClientsMutex.unlock();
		return;
	}

	// not found add new client
	MediaClient newClient( mediaType, callback, userData );
	m_MixerList.push_back( newClient );

	bool startSpeakerOutput = ( false == m_SpeakerOutputEnabled ) && ( 1 == m_MixerList.size() );
	// set capture stated before unlocking mutex
	if( startSpeakerOutput )
	{
		m_SpeakerOutputEnabled = true;
	}

	#ifdef DEBUG_AUDIO_PROCESSOR_LOCK
		LogMsg( LOG_INFO, "m_MixerClientsMutex.unlock" );
	#endif // DEBUG_AUDIO_PROCESSOR_LOCK
	m_MixerClientsMutex.unlock();

	if( startSpeakerOutput )
	{
		LogMsg( LOG_VERBOSE, "MediaProcessor::wantMixerMediaInput starting speaker output module %s", DescribeAppModule( appModule ) );
		IToGui::getAudioRequests().toGuiWantSpeakerOutput( appModule, true );	
	}
}

//============================================================================
void MediaProcessor::doMixerClientRemovals( std::vector<ClientToRemove>& clientRemoveList )
{
	m_MixerRemoveMutex.lock();
	for( size_t i = 0; i < clientRemoveList.size(); i++ )
	{
		EMediaInputType				mediaType	= clientRemoveList[i].m_MediaType;
		MediaCallbackInterface *	callback	= clientRemoveList[i].m_Callback;
		void *						userData	= clientRemoveList[i].m_UserData;
		EAppModule					appModule   = clientRemoveList[ i ].m_AppModule;

		std::vector<MediaClient> * clientList = 0;
		switch( mediaType )
		{
		case eMediaInputAudioPkts:
			clientList = &m_AudioPktsList;
			break;
		case eMediaInputVideoPkts:
			clientList = &m_VideoPktsList;
			break;
		case eMediaInputAudioPcm:
			clientList = &m_AudioPcmList;
			break;
		case eMediaInputVideoJpgSmall:
			clientList = &m_VideoJpgSmallList;
			break;
		case eMediaInputAudioOpus:
			clientList = &m_AudioOpusList;
			break;
		case eMediaInputVideoJpgBig:
			clientList = &m_VideoJpgBigList;
			break;

		case eMediaInputMixer:
			clientList = &m_MixerList;
			break;

		default:
			LogMsg( LOG_ERROR, "MediaProcessor::doClientRemovals UNKNOWN TYPE");
			continue;
		}

		std::vector<EAppModule> appModuleList;
		std::vector<MediaClient>::iterator iter;
		for( iter = clientList->begin(); iter != clientList->end(); ++iter )
		{
			MediaClient& client = *iter;
			if( ( client.m_Callback == callback )
				&& ( client.m_UserData == userData )
				&& ( client.m_MediaInputType == mediaType ) )
			{
				clientList->erase( iter );
				break;
			}
		}

		bool stopSpeakerOutput = m_SpeakerOutputEnabled && ( 0 == m_MixerList.size() );
		// set capture stated before unlocking mutex
		if( stopSpeakerOutput )
		{
			m_SpeakerOutputEnabled = false;
			LogMsg( LOG_VERBOSE, "MediaProcessor::doMixerClientRemovals stopping speaker output module %s", DescribeAppModule( appModule ) );
			IToGui::getAudioRequests().toGuiWantSpeakerOutput( appModule, false );
		}
	}

	clientRemoveList.clear();
	m_MixerRemoveMutex.unlock();
}

//============================================================================
void MediaProcessor::wantAudioMediaInput(	EMediaInputType				mediaType, 
											MediaCallbackInterface *	callback, 
											void *						userData, 
											EAppModule					appModule,
											bool						wantInput )
{
	if( false == wantInput )
	{
		// user wants to be removed but is probably being called from a callback function.
		// if we remove not will crash because iterator may expect more items in array or there may be a dead lock
		m_AudioRemoveMutex.lock();
		if( m_MicCaptureEnabled && !clientToRemoveExistsInList( m_AudioClientRemoveList, mediaType, callback, userData ) )
		{
			m_AudioClientRemoveList.push_back( ClientToRemove( mediaType, callback, userData, appModule ) );
		}

		m_AudioRemoveMutex.unlock();
		return;
	}
	else
	{
		// if been commanded to remove and now adding remove the previous command
		m_AudioRemoveMutex.lock();
		clientToRemoveRemoveFromList( m_AudioClientRemoveList, mediaType, callback, userData );
		m_AudioRemoveMutex.unlock();
	}

	#ifdef DEBUG_AUDIO_PROCESSOR_LOCK
		LogMsg( LOG_INFO, "wantAudioMediaInput m_AudioMutex.lock start" );
	#endif // DEBUG_AUDIO_PROCESSOR_LOCK
	m_AudioMutex.lock();
	#ifdef DEBUG_AUDIO_PROCESSOR_LOCK
		LogMsg( LOG_INFO, "wantAudioMediaInput m_AudioMutex.lock done" );
	#endif // DEBUG_AUDIO_PROCESSOR_LOCK

	std::vector<MediaClient> * clientList = 0;
	switch( mediaType )
	{
	case eMediaInputAudioPkts:
		clientList = &m_AudioPktsList;
		break;
	case eMediaInputAudioPcm:
		clientList = &m_AudioPcmList;
		break;
	case eMediaInputAudioOpus:
		clientList = &m_AudioOpusList;
		break;

	default:
		LogMsg( LOG_ERROR, "wantAudioMediaInput unknown mediaType type %d", mediaType );
	#ifdef DEBUG_AUDIO_PROCESSOR_LOCK
			LogMsg( LOG_INFO, "m_AudioMutex.unlock" );
	#endif // DEBUG_AUDIO_PROCESSOR_LOCK
		m_AudioMutex.unlock();
		return;
	}

	if( clientExistsInList( *clientList, mediaType, callback, userData ) )
	{
		LogMsg( LOG_INFO, "WARNING. Ignoring New Audio Media Client because already in list" );
		#ifdef DEBUG_AUDIO_PROCESSOR_LOCK
			LogMsg( LOG_INFO, "m_AudioMutex.unlock" );
		#endif // DEBUG_AUDIO_PROCESSOR_LOCK
		m_AudioMutex.unlock();
		return;
	}

	// add new client
	MediaClient newClient( mediaType, callback, userData );
	clientList->push_back( newClient );

	bool startMicInput = ( false == m_MicCaptureEnabled ) && ( 1 == ( m_AudioPcmList.size() + m_AudioOpusList.size() + m_AudioPktsList.size() ) );
	// set capture stated before unlocking mutex
	if( startMicInput )
	{
		m_MicCaptureEnabled = true;
	}

#ifdef DEBUG_AUDIO_PROCESSOR_LOCK
	LogMsg( LOG_INFO, "m_AudioMutex.unlock" );
#endif // DEBUG_AUDIO_PROCESSOR_LOCK
	m_AudioMutex.unlock();

	if( startMicInput )
	{
		LogMsg( LOG_INFO, "starting microphone input" );
		IToGui::getAudioRequests().toGuiWantMicrophoneRecording( appModule, true );
	}
}

//============================================================================
void MediaProcessor::doAudioClientRemovals( std::vector<ClientToRemove>& clientRemoveList )
{
	m_AudioRemoveMutex.lock();
	for( size_t i = 0; i < clientRemoveList.size(); i++ )
	{
		EMediaInputType				mediaType	= clientRemoveList[i].m_MediaType;
		MediaCallbackInterface *	callback	= clientRemoveList[i].m_Callback;
		void *						userData	= clientRemoveList[i].m_UserData;
		EAppModule					appModule   = clientRemoveList[ i ].m_AppModule;

		std::vector<MediaClient> * clientList = 0;
		switch( mediaType )
		{
		case eMediaInputAudioPkts:
			clientList = &m_AudioPktsList;
			break;
		case eMediaInputVideoPkts:
			clientList = &m_VideoPktsList;
			break;
		case eMediaInputAudioPcm:
			clientList = &m_AudioPcmList;
			break;
		case eMediaInputVideoJpgSmall:
			clientList = &m_VideoJpgSmallList;
			break;
		case eMediaInputAudioOpus:
			clientList = &m_AudioOpusList;
			break;
		case eMediaInputVideoJpgBig:
			clientList = &m_VideoJpgBigList;
			break;

		case eMediaInputMixer:
			clientList = &m_MixerList;
			break;

		default:
			LogMsg( LOG_ERROR, "MediaProcessor::doClientRemovals UNKNOWN TYPE");
			continue;
		}

		std::vector<MediaClient>::iterator iter;
		for( iter = clientList->begin(); iter != clientList->end(); ++iter )
		{
			MediaClient& client = *iter;
			if( ( client.m_Callback == callback )
				&& ( client.m_UserData == userData )
				&& ( client.m_MediaInputType == mediaType ) )
			{
				clientList->erase( iter );
				break;
			}
		}

		bool stopMicInput = m_MicCaptureEnabled && ( 0 == ( m_AudioPcmList.size() + m_AudioOpusList.size() + m_AudioPktsList.size() ) );
		// set capture stated before unlocking mutex
		if( stopMicInput )
		{
			m_MicCaptureEnabled = false;
			LogMsg( LOG_INFO, "stopping microphone input" );
			IToGui::getAudioRequests().toGuiWantMicrophoneRecording( appModule, false );
		}
	}

	clientRemoveList.clear();
	m_AudioRemoveMutex.unlock();
}

//============================================================================
void MediaProcessor::wantVideoMediaInput(	EMediaInputType				mediaType, 
											MediaCallbackInterface *	callback, 
											void *						userData, 
											EAppModule					appModule,
											bool						wantInput )
{
	if( false == wantInput )
	{
		// user wants to be removed but is probably being called from a callback function.
		// if we remove not will crash because iterator may expect more items in array
		m_VideoRemoveMutex.lock();
		if( m_VidCaptureEnabled && !clientToRemoveExistsInList( m_VideoClientRemoveList, mediaType, callback, userData ) )
		{
			m_VideoClientRemoveList.push_back( ClientToRemove( mediaType, callback, userData, appModule ) );
			m_VideoSemaphore.signal();
		}

		m_VideoRemoveMutex.unlock();
		return;
	}
	else
	{
		// if been commanded to remove and now adding remove the previous command
		m_VideoRemoveMutex.lock();
		clientToRemoveRemoveFromList( m_VideoClientRemoveList, mediaType, callback, userData );
		m_VideoRemoveMutex.unlock();
	}

	#ifdef DEBUG_PROCESSOR_LOCK
		LogMsg( LOG_INFO, "wantVideoMediaInput m_VideoMutex.lock start" );
	#endif // DEBUG_PROCESSOR_LOCK
		m_VideoMutex.lock();
	#ifdef DEBUG_PROCESSOR_LOCK
		LogMsg( LOG_INFO, "wantVideoMediaInput m_VideoMutex.lock done" );
	#endif // DEBUG_PROCESSOR_LOCK

	std::vector<MediaClient> * clientList = 0;
	switch( mediaType )
	{
	case eMediaInputVideoPkts:
		clientList = &m_VideoPktsList;
		break;
	case eMediaInputVideoJpgSmall:
		clientList = &m_VideoJpgSmallList;
		break;
	case eMediaInputVideoJpgBig:
		clientList = &m_VideoJpgBigList;
		break;

	default:
		LogMsg( LOG_ERROR, "wantVideoMediaInput unknown mediaType type %d", mediaType );
		#ifdef DEBUG_PROCESSOR_LOCK
			LogMsg( LOG_INFO, "m_VideoMutex.unlock" );
		#endif // DEBUG_PROCESSOR_LOCK
		m_VideoMutex.unlock();
		return;
	}

	if( clientExistsInList( *clientList, mediaType, callback, userData ) )
	{
		LogMsg( LOG_INFO, "WARNING. Ignoring New Video Media Client because already in list" );
		#ifdef DEBUG_PROCESSOR_LOCK
			LogMsg( LOG_INFO, "m_VideoMutex.unlock" );
		#endif // DEBUG_PROCESSOR_LOCK
		m_VideoMutex.unlock();
		return;
	}

	MediaClient newClient( mediaType, callback, userData );
	clientList->push_back( newClient );

	int idsInVidPktListCnt = myIdInVidPktListCount(); // this also updates m_VidPktListContainsMyId
	#ifdef DEBUG_PROCESSOR_LOCK
		LogMsg( LOG_INFO, "m_VideoMutex.unlock" );
	#endif // DEBUG_PROCESSOR_LOCK
	bool startVidCapture = ( false == m_VidCaptureEnabled ) && ( 1 == ( m_VideoJpgSmallList.size() + idsInVidPktListCnt + m_VideoJpgBigList.size() ) );
	m_VideoMutex.unlock();
	if( startVidCapture )
	{
		#ifdef TEST_JPG_SPEED
			testJpgSpeed();
		#endif // TEST_JPG_SPEED
		m_VidCaptureEnabled = true;
		LogMsg( LOG_INFO, "starting video capture" );
		IToGui::getToGui().toGuiWantVideoCapture( appModule, true );
	}
}

//============================================================================
void MediaProcessor::doVideoClientRemovals( std::vector<ClientToRemove>& clientRemoveList )
{
	m_VideoRemoveMutex.lock();
	for( size_t i = 0; i < clientRemoveList.size(); i++ )
	{
		EMediaInputType				mediaType	= clientRemoveList[i].m_MediaType;
		MediaCallbackInterface *	callback	= clientRemoveList[i].m_Callback;
		void *						userData	= clientRemoveList[i].m_UserData;
		EAppModule					appModule   = clientRemoveList[ i ].m_AppModule;

		std::vector<MediaClient> * clientList = 0;
		switch( mediaType )
		{
		case eMediaInputAudioPkts:
			clientList = &m_AudioPktsList;
			break;
		case eMediaInputVideoPkts:
			clientList = &m_VideoPktsList;
			break;
		case eMediaInputAudioPcm:
			clientList = &m_AudioPcmList;
			break;
		case eMediaInputVideoJpgSmall:
			clientList = &m_VideoJpgSmallList;
			break;
		case eMediaInputAudioOpus:
			clientList = &m_AudioOpusList;
			break;
		case eMediaInputVideoJpgBig:
			clientList = &m_VideoJpgBigList;
			break;

		case eMediaInputMixer:
			clientList = &m_MixerList;
			break;

		default:
			LogMsg( LOG_ERROR, "MediaProcessor::doClientRemovals UNKNOWN TYPE");
			continue;
		}

		std::vector<MediaClient>::iterator iter;
		for( iter = clientList->begin(); iter != clientList->end(); ++iter )
		{
			MediaClient& client = *iter;
			if( ( client.m_Callback == callback )
				&& ( client.m_UserData == userData )
				&& ( client.m_MediaInputType == mediaType ) )
			{
				clientList->erase( iter );
				break;
			}
		}

		int idsInVidPktListCnt2 = myIdInVidPktListCount(); // this also updates m_VidPktListContainsMyId
		bool stopVidCapture = m_VidCaptureEnabled && ( 0 == ( m_VideoJpgSmallList.size() + idsInVidPktListCnt2 + m_VideoJpgBigList.size() ) );
		if( stopVidCapture )
		{
			m_VidCaptureEnabled = false;
			LogMsg( LOG_INFO, "stopping video capture" );
			IToGui::getToGui().toGuiWantVideoCapture( appModule, false );
		}
	}

	clientRemoveList.clear();
	m_VideoRemoveMutex.unlock();
}

//============================================================================
int MediaProcessor::myIdInVidPktListCount( void )
{
	int containsMyIdCnt = 0;
	VxNetIdent * myNetIdent = &m_Engine.getMyPktAnnounce();
	std::vector<MediaClient>::iterator iter;
	for( iter = m_VideoPktsList.begin(); iter != m_VideoPktsList.end(); ++iter )
	{
		MediaClient& client = *iter;
		if( client.m_UserData == myNetIdent )
		{
			containsMyIdCnt++;
		}
	}

	m_VidPktListContainsMyId = containsMyIdCnt ? true : false;
	return containsMyIdCnt;
}

//============================================================================
void MediaProcessor::fromGuiEchoCanceledSamplesThreaded( int16_t* pcmData, int sampleCnt, bool isSilence )
{
	vx_assert( sampleCnt == MIXER_CHUNK_LEN_SAMPLES );
	if( false == m_MicCaptureEnabled || !pcmData || sampleCnt < 100 )
	{
		// invalid params or microphone not in capture mode
		m_AudioInSemaphore.signal();
		return;
	}

	//if( samplesHeadTimeMs - m_MicInputLastSampleTime > 200 )
	//{
	//	// microphone was paused or changed or something.. throw out previous samples
	//	m_MicInputSamples.clear();
	//}

	if( m_ProcessAudioQue.size() < 5 )
	{
		RawAudio* rawAudio = new RawAudio( pcmData, MIXER_CHUNK_LEN_BYTES, eAppModuleMicrophone );

		m_AudioQueInMutex.lock();
		m_ProcessAudioQue.push_back( rawAudio );
		m_AudioQueInMutex.unlock();
	}

	m_AudioInSemaphore.signal();
}

//============================================================================
void MediaProcessor::fromGuiAudioOutSpaceAvaiThreaded( int freeSpaceLen )
{
	std::vector<MediaClient>::iterator iter;
	//#ifdef DEBUG_AUDIO_PROCESSOR_LOCK
	//	LogMsg( LOG_INFO, "fromGuiAudioOutSpaceAvail m_MixerClientsMutex.lock start" );
	//#endif // DEBUG_AUDIO_PROCESSOR_LOCK
	m_MixerClientsMutex.lock();
	//#ifdef DEBUG_AUDIO_PROCESSOR_LOCK
	//	LogMsg( LOG_INFO, "fromGuiAudioOutSpaceAvail m_MixerClientsMutex.lock done" );
	//#endif // DEBUG_AUDIO_PROCESSOR_LOCK
	doMixerClientRemovals( m_MixerClientRemoveList );

	for( iter = m_MixerList.begin(); iter != m_MixerList.end(); ++iter )
	{
		MediaClient& client = (*iter);
		//LogMsg( LOG_INFO, "fromGuiAudioOutSpaceAvail callbackAudioOutSpaceAvail %d", iClientIdx );
		client.m_Callback->callbackAudioOutSpaceAvail( MIXER_CHUNK_LEN_BYTES );
	}

	//#ifdef DEBUG_AUDIO_PROCESSOR_LOCK
	//	LogMsg( LOG_INFO, "fromGuiAudioOutSpaceAvail m_MixerClientsMutex.unlock" );
	//#endif // DEBUG_AUDIO_PROCESSOR_LOCK
	m_MixerClientsMutex.unlock();
	//#ifdef DEBUG_AUDIO_PROCESSOR_LOCK
	//	LogMsg( LOG_INFO, "fromGuiAudioOutSpaceAvail m_MixerBufferMutex.lock()" );
	//#endif // DEBUG_AUDIO_PROCESSOR_LOCK
	m_MixerBufferMutex.lock();
	if( !m_MixerBufUsed || m_MuteSpeaker )
	{
		IToGui::getAudioRequests().toGuiPlayAudioFrame( eAppModulePtoP, (int16_t*)m_QuietAudioBuf, MIXER_CHUNK_LEN_BYTES, true );
	}
	else
	{
		IToGui::getAudioRequests().toGuiPlayAudioFrame( eAppModulePtoP, (int16_t*)m_MixerBuf, MIXER_CHUNK_LEN_BYTES, false );
	}

	m_MixerBufUsed = false;

	//#ifdef DEBUG_AUDIO_PROCESSOR_LOCK
	//	LogMsg( LOG_INFO, "fromGuiAudioOutSpaceAvail m_MixerBufferMutex.unlock()" );
	//#endif // DEBUG_AUDIO_PROCESSOR_LOCK
	m_MixerBufferMutex.unlock();
}
