#pragma once
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

//#include "EchoCancel.h"

#include <GuiInterface/IFromGui.h>

//#include <PktLib/PktVoiceReq.h>

#include <CoreLib/VxMutex.h>
#include <CoreLib/VxThread.h>
#include <CoreLib/VxSemaphore.h>
#include <CoreLib/VxGUID.h>


#include <string>
#include <vector>

class PluginBase;
class IToGui;
class P2PEngine;
class PluginMgr;
class MediaTools;
class RawAudio;
class RawVideo;
class MediaClient;
class PktVideoFeedPic;
class PktVideoFeedPicChunk;

#define MIXER_CHUNK_LEN_BYTES		1280
#define MIXER_CHUNK_LEN_SAMPLES		640

// from fourcc.org 
#define FOURCC_RGB		0x32424752
#define FOURCC_RGBA		0x41424752
#define FOURCC_I420		0x30323449
#define FOURCC_YUY2		0x32595559
#define FOURCC_YV12		0x32315659
#define FOURCC_IYUV		0x56555949

#define FOURCC_YUVS		0x12345678 // I made this special for android YUV 4.2.0 SP
// made up to handle reversed blue and red on windows
#define FOURCC_BGR		0x32524742
#define FOURCC_BGRA		0x52474241

class ClientToRemove
{
public:
	ClientToRemove();
	ClientToRemove( EMediaInputType				mediaType, 
					MediaCallbackInterface *	callback,
					void *						userData)
	: m_MediaType( mediaType )
	, m_Callback( callback )
	, m_UserData( userData )
	{
	}

	ClientToRemove( const ClientToRemove& rhs )
	{
		*this = rhs;
	}

	ClientToRemove&				operator =( const ClientToRemove& rhs )
	{
		m_MediaType		= rhs.m_MediaType;
		m_Callback		= rhs.m_Callback;
		m_UserData		= rhs.m_UserData;
		return *this;
	}

	EMediaInputType				m_MediaType;
	MediaCallbackInterface *	m_Callback;
	void *						m_UserData;
};

//#define DEBUG_PROCESSOR_LOCK 1
//#define DEBUG_AUDIO_PROCESSOR_LOCK 1
class MediaProcessor : public MediaCallbackInterface
{
public:
	class AudioProcessorLock
	{
	public:
		AudioProcessorLock( MediaProcessor * processor ) : m_Mutex(processor->getAudioMutex())	
		{ 
#ifdef DEBUG_AUDIO_PROCESSOR_LOCK
	LogMsg( LOG_INFO, "AudioProcessorLock Lock start\n");
#endif //DEBUG_AUDIO_PROCESSOR_LOCK
			m_Mutex.lock(); 
#ifdef DEBUG_AUDIO_PROCESSOR_LOCK
			LogMsg( LOG_INFO, "AudioProcessorLock Lock complete\n");
#endif //DEBUG_AUDIO_PROCESSOR_LOCK
		}

		~AudioProcessorLock()																		
		{ 
#ifdef DEBUG_AUDIO_PROCESSOR_LOCK
			LogMsg( LOG_INFO, "AudioProcessorLock Unlock\n");
#endif //DEBUG_AUDIO_PROCESSOR_LOCK
			m_Mutex.unlock(); 
		}

		VxMutex&				m_Mutex;
	};

	class VideoProcessorLock
	{
	public:
		VideoProcessorLock( MediaProcessor * processor ) : m_Mutex(processor->getVideoMutex())	
		{ 
#ifdef DEBUG_PROCESSOR_LOCK
			LogMsg( LOG_INFO, "VideoProcessorLock Lock start\n");
#endif //DEBUG_PROCESSOR_LOCK
			m_Mutex.lock(); 
#ifdef DEBUG_PROCESSOR_LOCK
			LogMsg( LOG_INFO, "VideoProcessorLock Lock complete\n");
#endif //DEBUG_PROCESSOR_LOCK
		}

		~VideoProcessorLock()																		
		{ 
#ifdef DEBUG_PROCESSOR_LOCK
			LogMsg( LOG_INFO, "VideoProcessorLock Unlock\n");
#endif //DEBUG_PROCESSOR_LOCK
			m_Mutex.unlock(); 
		}

		VxMutex&				m_Mutex;
	};

	MediaProcessor( P2PEngine& engine );
	virtual ~MediaProcessor();
	
	void						shutdownMediaProcessor( void );

	VxMutex&					getAudioMutex( void )				{ return m_AudioMutex; }
	VxMutex&					getVideoMutex( void )				{ return m_VideoMutex; }
	//MediaTools&					getMediaTools( void )				{ return m_MediaTools; }

	//bool						isSpeakerOutputEnabled( void )		{ return m_SpeakerOutputEnabled; }
	//bool						isMicrophoneCaptureEnabled( void )	{ return m_MicCaptureEnabled; }
	bool						isVideoCaptureEnabled( void )		{ return m_VidCaptureEnabled; }

	virtual void				wantMediaInput( EMediaInputType				mediaType, 
												MediaCallbackInterface *	callback, 
												void *						userData, 
												bool						wantInput );

	virtual void				fromGuiVideoData( uint32_t u32FourCc, uint8_t * pu8VidDataIn, int iWidth, int iHeight, uint32_t u32VidDataLen, int iRotation );
	virtual void				fromGuiYUV420CaptureImage( uint8_t * yBytes, uint8_t * uBytes, uint8_t * vBytes, 
														int yRowStride, int uRowStride, int vRowStride,
														int yPixStride, int uPixStride, int vPixStride,
														int imageWidth, int imageHeight, int imageRotate );

	void						processFriendVideoFeed(	VxGUID&			onlineId, 
														uint8_t *			jpgData, 
														uint32_t				jpgDataLen,
														int				motion0To100000 );


	void						processVideoIn( void );


protected:
	void						processRawVideoIn( RawVideo * rawVideo );
	bool						isAudioMediaType( EMediaInputType mediaType );
	void						wantAudioMediaInput(	EMediaInputType				mediaType, 
														MediaCallbackInterface *	callback, 
														void *						userData, 
														bool						wantInput );

	void						wantMixerMediaInput(	EMediaInputType				mediaType, 
														MediaCallbackInterface *	callback, 
														void *						userData, 
														bool						wantInput );

	void						wantVideoMediaInput(	EMediaInputType				mediaType, 
														MediaCallbackInterface *	callback, 
														void *						userData, 
														bool						wantInput );
	
	void						doMixerClientRemovals( std::vector<ClientToRemove>& clientRemoveList );
	void						doAudioClientRemovals( std::vector<ClientToRemove>& clientRemoveList );
	void						doVideoClientRemovals( std::vector<ClientToRemove>& clientRemoveList );

	bool						clientExistsInList(	std::vector<MediaClient>&		clientList, 
													EMediaInputType					mediaType, 
													MediaCallbackInterface *		callback,
													void *							userData );
	bool						clientToRemoveExistsInList(	std::vector<ClientToRemove>&	clientRemoveList, 
															EMediaInputType					mediaType, 
															MediaCallbackInterface *		callback,
															void *							userData );
	bool						clientToRemoveRemoveFromList(	std::vector<ClientToRemove>&	clientRemoveList, 
																EMediaInputType					mediaType, 
																MediaCallbackInterface *		callback,
																void *							userData );

	int							myIdInVidPktListCount( void );

	//=== vars ===//
	P2PEngine&					m_Engine;
	//PluginMgr&					m_PluginMgr;
	
	//MediaTools&					m_MediaTools;
	VxMutex						m_AudioMutex;
	VxMutex						m_VideoMutex;
	VxGUID						m_MyOnlineId;


	std::vector<MediaClient>	m_VideoJpgBigList;	
	std::vector<MediaClient>	m_VideoJpgSmallList;
	std::vector<MediaClient>	m_VideoPktsList;
	std::vector<MediaClient>	m_MixerList;
	VxMutex						m_MixerClientsMutex;

	std::vector<ClientToRemove>	m_MixerClientRemoveList;
	VxMutex						m_MixerRemoveMutex;
	std::vector<ClientToRemove>	m_VideoClientRemoveList;
	VxMutex						m_VideoRemoveMutex;
	std::vector<ClientToRemove>	m_AudioClientRemoveList;
	VxMutex						m_AudioRemoveMutex;


	std::vector<RawVideo *>		m_ProcessVideoQue;
	VxMutex						m_VideoQueInMutex;
	VxThread					m_ProcessVideoThread;
	VxSemaphore					m_VideoSemaphore;
    PktVideoFeedPic *			m_PktVideoFeedPic;
    std::vector<PktVideoFeedPicChunk *> m_VidChunkList;

	VxMutex						m_MixerBufferMutex;

	bool						m_VidCaptureEnabled;
#ifdef USE_ECHO_CANCEL
	EchoCancel					m_EchoCancel;
#endif //USE_ECHO_CANCEL
	bool						m_VidPktListContainsMyId;
};
