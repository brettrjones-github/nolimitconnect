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
// http://www.nolimitconnect.com
//============================================================================

#include "EchoCancel.h"

#include <GuiInterface/IFromGui.h>

#include <PktLib/PktVoiceReq.h>

#include <CoreLib/VxMutex.h>
#include <CoreLib/VxThread.h>
#include <CoreLib/VxSemaphore.h>

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
	MediaTools&					getMediaTools( void )				{ return m_MediaTools; }

	bool						isSpeakerOutputEnabled( void )		{ return m_SpeakerOutputEnabled; }
	bool						isMicrophoneCaptureEnabled( void )	{ return m_MicCaptureEnabled; }
	bool						isVideoCaptureEnabled( void )		{ return m_VidCaptureEnabled; }

	virtual void				wantAppIdle( EPluginType ePluginType, bool bWantAppIdle );

	virtual void				wantMediaInput( EMediaInputType				mediaType, 
												MediaCallbackInterface *	callback, 
												void *						userData, 
												bool						wantInput );

	virtual void				fromGuiVideoData( uint32_t u32FourCc, uint8_t * pu8VidDataIn, int iWidth, int iHeight, uint32_t u32VidDataLen, int iRotation );
	virtual void				fromGuiYUV420CaptureImage( uint8_t * yBytes, uint8_t * uBytes, uint8_t * vBytes, 
														int yRowStride, int uRowStride, int vRowStride,
														int yPixStride, int uPixStride, int vPixStride,
														int imageWidth, int imageHeight, int imageRotate );
	//virtual void				fromGuiMicrophoneData( int16_t * pu16PcmData, uint16_t pcmDataLenBytes );
	virtual void				fromGuiMicrophoneDataWithInfo( int16_t * pcmData, int pcmDataLenBytes, bool isSilence, int totalDelayTimeMs, int clockDrift );
	virtual void				fromGuiAudioOutSpaceAvail( int freeSpaceLen );

	virtual void				fromGuiSoundDelayTest( void );

	void						increasePcmSampleVolume( int16_t * pcmData, uint16_t pcmDataLen, float volumePercent0To100 );
	void						playAudio( int16_t * pcmData, int dataLenInBytes );

	void						muteSpeaker( bool muteSpeaker )							{ m_MuteSpeaker = muteSpeaker; }
	bool						isSpeakerMuted( void )									{ return m_MuteSpeaker; }
	void						muteMicrophone( bool muteMic )							{ m_MuteMicrophone = muteMic; }
	bool						isMicrophoneMuted( void )								{ return m_MuteMicrophone; }
	void						fromGuiEchoCancelEnable( bool enableEchoCancel );
	bool						fromGuiIsEchoCancelEnabled( void );

	void						processFriendVideoFeed(	VxGUID&			onlineId, 
														uint8_t *			jpgData, 
														uint32_t				jpgDataLen,
														int				motion0To100000 );

	void						processFriendAudioFeed(	VxGUID&	onlineId, int16_t * pcmData, uint16_t pcmDataLen, bool dontLock = false );

	void						processAudioIn( void );
	void						processVideoIn( void );


protected:
	void						processRawAudioIn( RawAudio * rawAudio );
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
	PluginMgr&					m_PluginMgr;
	
	MediaTools&					m_MediaTools;
	VxMutex						m_AudioMutex;
	VxMutex						m_VideoMutex;
	VxGUID						m_MyOnlineId;

	std::vector<PluginBase *>	m_aoWantAppIdle;				// list of plugins that want called on app idle

	std::vector<MediaClient>	m_AudioPcmList;	
	std::vector<MediaClient>	m_AudioOpusList;	
	std::vector<MediaClient>	m_AudioPktsList;

	std::vector<RawAudio *>		m_ProcessAudioQue;
	VxMutex						m_AudioQueInMutex;
	VxThread					m_ProcessAudioThread;
	VxSemaphore					m_AudioSemaphore;
	PktVoiceReq					m_PktVoiceReq;

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

	int16_t						m_QuietAudioBuf[ MIXER_CHUNK_LEN_SAMPLES ];
	int16_t						m_MixerBuf[ MIXER_CHUNK_LEN_SAMPLES ];
	bool						m_MixerBufUsed;
	bool						m_MuteSpeaker;
	bool						m_MuteMicrophone;
	bool						m_VidCaptureEnabled;
	bool						m_MicCaptureEnabled;
	bool						m_SpeakerOutputEnabled;
#ifdef USE_ECHO_CANCEL
	EchoCancel					m_EchoCancel;
#endif //USE_ECHO_CANCEL
	int16_t						m_EchoOutBuf[ MIXER_CHUNK_LEN_SAMPLES ];
	int							m_EchoOutBufIdx;
	int							m_EchoOutDataProcessedLen;
	bool						m_VidPktListContainsMyId;
};
