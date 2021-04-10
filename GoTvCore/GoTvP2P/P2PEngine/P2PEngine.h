#pragma once
//============================================================================
// Copyright (C) 2009 Brett R. Jones 
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

#include "config_gotvcore.h"

#include "P2PConnectList.h"
#include "EngineSettings.h"
#include "EngineParams.h"

#include <GoTvCore/GoTvP2P/AssetMgr/AssetCallbackInterface.h>
#include <GoTvCore/GoTvP2P/ThumbMgr/ThumbCallbackInterface.h>
#include <GoTvCore/GoTvP2P/Connections/ConnectionMgr.h>
#include <GoTvCore/GoTvP2P/BlobXferMgr/BlobCallbackInterface.h>
#include <GoTvCore/GoTvP2P/NetworkMonitor/NetStatusAccum.h>
#include <GoTvCore/GoTvP2P/PluginSettings/PluginSettingMgr.h>
#include <GoTvCore/GoTvP2P/ThumbMgr/ThumbMgr.h>

#include <GoTvInterface/IDefs.h>
#include <GoTvInterface/IFromGui.h>
#include <GoTvInterface/IAudioInterface.h>

#include <GoTvCore/GoTvP2P/Search/RcScan.h>

#include <GoTvCore/GoTvP2P/BigListLib/BigListMgr.h>
#include <NetLib/VxSktDefs.h>
#include <PktLib/PktAnnounce.h>
#include <PktLib/PktSysHandlerBase.h>
#include <PktLib/PktsImAlive.h>

class IToGui;
class IGoTv;
class FileShareSettings;
class VxPeerMgr;
class PluginMgr;
class AssetMgr;
class BlobMgr;
class IsPortOpenTest;
class RcConnectInfo;
class ConnectRequest;
class MediaProcessor;
class NetworkMgr;
class NetworkStateMachine;
class NetConnector;
class NetworkMonitor;
class NetServicesMgr;
class PluginNetServices;
class PluginServiceFileShare;
class PluginServiceRelay;
class PluginSetting;
class PluginSettingMgr;
class RunUrlAction;

class P2PEngine :	public IFromGui,
					public PktHandlerBase,
                    public AssetCallbackInterface,
					public ThumbCallbackInterface,
                    public BlobCallbackInterface,
					public MediaCallbackInterface,
                    public IAudioCallbacks
{
public:
	P2PEngine( VxPeerMgr& peerMgr, BigListMgr& bigListMgr );
	virtual ~P2PEngine() override;

	void						startupEngine( void );
	void						shutdownEngine( void );

    IToGui&						getToGui( void );
	IFromGui&					getFromGuiInterface( void )						{ return *this; }
    IAudioRequests&			    getAudioRequest( void );
    AssetMgr&					getAssetMgr( void )								{ return m_AssetMgr; }
    ThumbMgr&					getThumbMgr( void )								{ return m_ThumbMgr; }
    BigListMgr&					getBigListMgr( void )							{ return m_BigListMgr; }
    ConnectionMgr&              getConnectionMgr( void )                        { return m_ConnectionMgr; }
    BlobMgr&				    getBlobMgr( void )							    { return m_BlobMgr; }
    EngineSettings&				getEngineSettings( void )						{ return m_EngineSettings; }
	EngineParams&				getEngineParams( void )							{ return m_EngineParams; }
    NetConnector&				getNetConnector( void )							{ return m_NetConnector; }
    NetStatusAccum&             getNetStatusAccum( void )                       { return m_NetStatusAccum; }
    NetworkMgr&					getNetworkMgr( void )							{ return m_NetworkMgr; }
    NetworkStateMachine&		getNetworkStateMachine( void )					{ return m_NetworkStateMachine; }
	NetworkMonitor&				getNetworkMonitor( void )						{ return m_NetworkMonitor; } 
	NetServicesMgr&				getNetServicesMgr( void )						{ return m_NetServicesMgr; }
	MediaProcessor&				getMediaProcesser( void )						{ return m_MediaProcessor; }
	VxPeerMgr&					getPeerMgr( void )								{ return m_PeerMgr; }
    P2PConnectList&				getConnectList( void )							{ return m_ConnectionList; }
    PluginMgr&					getPluginMgr( void )							{ return m_PluginMgr; }
    PluginSettingMgr&			getPluginSettingMgr( void )						{ return m_PluginSettingMgr; }
    RcScan&						getRcScan( void )								{ return m_RcScan; }
    RunUrlAction&               getRunUrlAction( void )                         { return m_RunUrlAction; }

	bool						isAppPaused( void )								{ return m_AppIsPaused; }
	bool						isP2POnline( void );
    bool                        isDirectConnectReady( void );    // true if have open port and ready to recieve
    bool                        isNetworkHostEnabled( void );    // true if netowrk host plugin is enabled

    bool                        getHasHostService( EHostServiceType hostService );

    /// if skt exists in connection list then lock access to connection list
    bool						lockSkt( VxSktBase* sktBase );
    void						unlockSkt( VxSktBase* sktBase );

    void						lockAnnouncePktAccess( void )					{ m_AnnouncePktMutex.lock(); }
    void						unlockAnnouncePktAccess( void )					{ m_AnnouncePktMutex.unlock(); }

    void						copyMyPktAnnounce( PktAnnounce& pktAnn )		{ m_AnnouncePktMutex.lock(); memcpy(&pktAnn, &m_PktAnn, sizeof(PktAnnounce)); m_AnnouncePktMutex.unlock(); }

	PktAnnounce&				getMyPktAnnounce( void )						{ return m_PktAnn; }
    VxGUID						getMyOnlineId( void )							{ return m_MyOnlineId; }
    std::string					getMyOnlineUrl( void )							{ m_AnnouncePktMutex.lock(); std::string myUrl( m_PktAnn.getMyOnlineUrl() ); m_AnnouncePktMutex.unlock(); return myUrl; }
    bool						addMyIdentToBlob( PktBlobEntry& blobEntry );

    bool                        setPluginSetting( PluginSetting& pluginSetting );
    bool                        getPluginSetting( EPluginType pluginType, PluginSetting& pluginSetting );

    virtual void				setPluginPermission( EPluginType ePluginType, int iPluginPermission );
    virtual EFriendState		getPluginPermission( int iPluginType );

	PluginServiceRelay&			getPluginServiceRelay( void )					{ return * m_PluginServiceRelay; }
	PluginServiceFileShare&		getPluginServiceFileShare( void )				{ return * m_PluginServiceFileShare; }
	PluginNetServices&			getPluginNetServices( void )					{ return * m_PluginNetServices; }

	virtual void				setHasPicture( int bHasPicture );
	virtual void				setHasSharedWebCam( int bHasShaeredWebCam );
	bool						isContactConnected( VxGUID& onlineId );
	bool						isSystemPlugin( EPluginType	ePluginType );

	//========================================================================
	// from gui
	//========================================================================
	void						assureUserSpecificDirIsSet( const char * checkReason );

    virtual void				fromGuiSetIsAppCommercial( bool isCommercial ) override;
    virtual bool				fromGuiGetIsAppCommercial( void ) override;
    virtual uint16_t			fromGuiGetAppVersionBinary( void ) override;
    const char *				fromGuiGetAppVersionString( void ) override;
    virtual const char *		fromGuiGetAppName( void ) override;
    virtual const char *		fromGuiGetAppNameNoSpaces( void ) override;
    virtual void				fromGuiAppStartup( const char * assetDir, const char * rootDataDir  ) override;

    virtual void				fromGuiKickWatchdog( void ) override;
    virtual void				fromGuiSetUserSpecificDir( const char * userSpecificDir  ) override;
    virtual void				fromGuiSetUserXferDir( const char * userXferDir  ) override;
    virtual uint64_t			fromGuiGetDiskFreeSpace( void  ) override;
    virtual void				fromGuiAppShutdown( void  ) override;
    virtual void				fromGuiAppIdle( void  ) override;
    virtual void				fromGuiAppPause( void ) override;
    virtual void				fromGuiAppResume( void ) override;

    virtual void				fromGuiUserLoggedOn( VxNetIdent * netIdent ) override;
    void                        updateFromEngineSettings( EngineSettings& engineSettings ); 
    virtual void				fromGuiOnlineNameChanged( const char * newOnlineName ) override;
    virtual void				fromGuiMoodMessageChanged( const char * newMoodMessage ) override;
    virtual void				fromGuiIdentPersonalInfoChanged( int age, int gender, int language, int preferredContent ) override;

    virtual void				fromGuiSetUserHasProfilePicture( bool haveProfilePick ) override;
    virtual void				fromGuiUpdateMyIdent( VxNetIdent * netIdent, bool permissionAndStatsOnly = false ) override;
    virtual void				fromGuiQueryMyIdent( VxNetIdent * poRetIdent ) override;
    virtual void				fromGuiSetIdentHasTextOffers( VxGUID& onlineId, bool hasTextOffers ) override;

    virtual bool				fromGuiOrientationEvent( float f32RotX, float f32RotY, float f32RotZ  ) override;
    virtual bool				fromGuiMouseEvent( EMouseButtonType eMouseButType, EMouseEventType eMouseEventType, int iMouseXPos, int iMouseYPos  ) override;
    virtual bool				fromGuiMouseWheel( float f32MouseWheelDist ) override;
    virtual bool				fromGuiKeyEvent( EKeyEventType eKeyEventType, EKeyCode eKey, int iFlags = 0 ) override;

    virtual void				fromGuiNativeGlInit( void ) override;
    virtual void				fromGuiNativeGlResize( int w, int h ) override;
    virtual int					fromGuiNativeGlRender( void ) override;
    virtual void				fromGuiNativeGlPauseRender( void ) override;
    virtual void				fromGuiNativeGlResumeRender( void ) override;
    virtual void				fromGuiNativeGlDestroy( void ) override;

    virtual void				fromGuiMicrophoneDataWithInfo( int16_t * pcmData, int pcmDataLenBytes, bool isSilence, int totalDelayTimeMs, int clockDrift ) override;
    virtual void				fromGuiMuteMicrophone( bool muteMic ) override;
    virtual bool				fromGuiIsMicrophoneMuted( void ) override;
    virtual void				fromGuiMuteSpeaker(	bool muteSpeaker ) override;
    virtual bool				fromGuiIsSpeakerMuted( void ) override;
    virtual void				fromGuiEchoCancelEnable( bool enableEchoCancel ) override;
    virtual bool				fromGuiIsEchoCancelEnabled( void ) override;

    virtual void				fromGuiAudioOutSpaceAvail( int freeSpaceLen ) override;

    virtual bool				fromGuiSndRecord( ESndRecordState eRecState, VxGUID& feedId, const char * fileName ) override;
    virtual bool				fromGuiVideoRecord( EVideoRecordState eRecState, VxGUID& feedId, const char * fileName ) override;
    virtual bool				fromGuiPlayLocalMedia( const char * fileName, uint64_t fileLen, uint8_t fileType, int pos0to100000 = 0 ) override;


    virtual bool				fromGuiAssetAction( EAssetAction assetAction, AssetInfo& assetInfo, int pos0to100000 = 0 ) override;
    virtual bool				fromGuiAssetAction( EAssetAction assetAction, VxGUID& assetId, int pos0to100000 = 0 ) override;
    virtual bool				fromGuiSendAsset( AssetInfo& assetInfo ) override;

    virtual void				fromGuiWantMediaInput( EMediaInputType mediaType, MediaCallbackInterface * callback, void * userData, bool wantInput ) override;
    virtual void				fromGuiWantMediaInput( VxGUID& onlineId, EMediaInputType mediaType, bool wantInput ) override;

    virtual void				fromGuiVideoData( uint32_t u32FourCc, uint8_t * pu8VidDataIn, int iWidth, int iHeight, uint32_t u32VidDataLen, int iRotation ) override;
    virtual bool				fromGuiMovieDone( void ) override							{ return true; };

    virtual void				fromGuiNetworkAvailable( const char * lclIp = NULL, bool isCellularNetwork = false ) override;
    virtual void				fromGuiNetworkLost( void ) override;
    virtual ENetLayerState	    fromGuiGetNetLayerState( ENetLayerType netLayer = eNetLayerTypeInternet ) override;

    virtual void				fromGuiNetworkSettingsChanged( void ) override;

    virtual void				fromGuiAnnounceHost( EHostType hostType, VxGUID& sessionId, const char * ptopUrl = nullptr ) override;
    virtual void				fromGuiJoinHost( EHostType hostType, VxGUID& sessionId, const char * ptopUrl = nullptr ) override;
    virtual void				fromGuiSearchHost( EHostType hostType, SearchParams& searchParams, bool enable ) override;

    virtual void				fromGuiRunIsPortOpenTest( uint16_t port ) override;
    virtual void				fromGuiRunUrlAction( VxGUID& sessionId, const char * myUrl, const char * ptopUrl, ENetCmdType testType ) override;

	virtual void				fromGuiUpdateWebPageProfile(	const char *	pProfileDir,	// directory containing user profile
																const char *	strGreeting,	// greeting text
																const char *	aboutMe,		// about me text
																const char *	url1,			// favorite url 1
																const char *	url2,			// favorite url 2
                                                                const char *	url3,           // favorite url 3
                                                                const char *	donation ) override;	// donation		

    virtual void				fromGuiApplyNetHostSettings( NetHostSetting& netSettings ) override;
    virtual void				fromGuiSetNetSettings( NetSettings& netSettings ) override;
    virtual void				fromGuiGetNetSettings( NetSettings& netSettings ) override;
    virtual void				fromGuiSetRelaySettings( int userRelayMaxCnt, int systemRelayMaxCnt ) override;

    virtual void				fromGuiGetFileShareSettings( FileShareSettings& fileShareSettings ) override;
    virtual void				fromGuiSetFileShareSettings( FileShareSettings& fileShareSettings ) override;

    virtual void				fromGuiSetPluginPermission( EPluginType ePluginType, EFriendState eFriendState ) override;
    virtual int					fromGuiGetPluginPermission( EPluginType ePluginType ) override;
    virtual EPluginServerState	fromGuiGetPluginServerState( EPluginType ePluginType ) override;

    virtual void				fromGuiStartPluginSession( EPluginType ePluginType, VxGUID& onlineId, int pvUserData = 0, VxGUID lclSessionId = VxGUID::nullVxGUID() ) override;
    virtual void				fromGuiStopPluginSession( EPluginType ePluginType, VxGUID& onlineId, int pvUserData = 0, VxGUID lclSessionId = VxGUID::nullVxGUID()  ) override;
    virtual bool				fromGuiIsPluginInSession( EPluginType ePluginType, VxNetIdent * netIdent = NULL, int pvUserData = 0, VxGUID lclSessionId = VxGUID::nullVxGUID() ) override;

	virtual bool				fromGuiMakePluginOffer(	EPluginType		ePluginType, 
														VxGUID&			onlineId,
														int				pvUserData,
														const char *	pOfferMsg, 
														const char *	pFileName = NULL,
														uint8_t *			fileHashId = 0,
                                                        VxGUID			lclSessionId = VxGUID::nullVxGUID() ) override;

	virtual bool				fromGuiToPluginOfferReply(	EPluginType		ePluginType,
															VxGUID&			onlineId,
															  int			pvUserData,
															int				iOfferResponse,
                                                            VxGUID			lclSessionId ) override;

	virtual int					fromGuiPluginControl(	EPluginType		ePluginType, 
														VxGUID&			onlineId, 
														const char *	pControl, 
														const char *	pAction,
														uint32_t				u32ActionData,
														VxGUID&			fileId = VxGUID::nullVxGUID(),
                                                        uint8_t *			fileHashId = 0 ) override;

	virtual bool				fromGuiInstMsg(		EPluginType		ePluginType, 
													VxGUID&			onlineId, 
                                                    const char *	pMsg ) override;

	virtual bool				fromGuiChangeMyFriendshipToHim(	VxGUID&			onlineId, 
																EFriendState	eMyFriendshipToHim,
                                                                EFriendState	eHisFriendshipToMe ) override;
    virtual void				fromGuiSendContactList( EFriendViewType eFriendView, int MaxContactsToSend ) override;
    virtual void				fromGuiRefreshContactList( int MaxContactsToSend ) override;

    virtual void				fromGuiRequireRelay( bool bRequireRelay ) override;
    virtual void				fromGuiUseRelay( VxGUID& onlineId, bool bUseAsRelay = true ) override;
    virtual void				fromGuiRelayPermissionCount( int userPermittedCount, int anonymousCount );

    virtual void				fromGuiStartScan( EScanType eScanType, uint8_t searchFlags, uint8_t fileTypeFlags, const char * pSearchPattern = "" ) override;
    virtual void				fromGuiNextScan( EScanType eScanType ) override;
    virtual void				fromGuiStopScan( EScanType eScanType ) override;

    virtual InetAddress			fromGuiGetMyIPv4Address( void ) override;
    virtual InetAddress			fromGuiGetMyIPv6Address( void ) override;

    virtual void				fromGuiUserModifiedStoryboard( void ) override;

    virtual void				fromGuiCancelDownload( VxGUID& fileInstance ) override;
    virtual void				fromGuiCancelUpload( VxGUID& fileInstance ) override;

	virtual bool				fromGuiSetGameValueVar( EPluginType	ePluginType, 
														VxGUID&		onlineId, 
														int32_t			varId, 
                                                        int32_t			varValue ) override;

	virtual bool				fromGuiSetGameActionVar(	EPluginType	ePluginType, 
															VxGUID&		onlineId, 
															int32_t			actionId, 
                                                            int32_t			actionValue ) override;

	virtual bool				fromGuiTestCmd(	ETestParam1		eTestParam1, 
												int				testParam2 = 0, 
                                                const char *	testParam3 = NULL ) override;

    virtual uint16_t			fromGuiGetRandomTcpPort( void ) override;
    /// Get url for this node
    virtual void                fromGuiGetNodeUrl( std::string& nodeUrl ) override;
    /// Get internet status
    virtual EInternetStatus     fromGuiGetInternetStatus( void ) override;
    /// Get network status
    virtual ENetAvailStatus     fromGuiGetNetAvailStatus( void ) override;

#ifdef TARGET_OS_ANDROID
    virtual int					fromGuiMulitcastPkt( unsigned char * data, int len ) override;
#endif // TARGET_OS_ANDROID
    virtual void				fromGuiDebugSettings( uint32_t u32LogFlags, const char *	pLogFileName = NULL ) override;
    virtual void				fromGuiSendLog( uint32_t u32LogFlags ) override;
    virtual bool				fromGuiBrowseFiles(	const char * dir, bool lookupShareStatus, uint8_t fileFilterMask = VXFILE_TYPE_ALLNOTEXE | VXFILE_TYPE_DIRECTORY ) override;
    virtual bool				fromGuiGetSharedFiles( uint8_t fileTypeFilter ) override;
    virtual bool				fromGuiSetFileIsShared( const char * fileName, bool isShared, uint8_t * fileHashId = 0 ) override;
    virtual bool				fromGuiGetIsFileShared( const char * fileName ) override;
	// returns -1 if unknown else percent downloaded
    virtual int					fromGuiGetFileDownloadState( uint8_t * fileHashId ) override;
    virtual bool				fromGuiAddFileToLibrary( const char * fileName, bool addFile, uint8_t * fileHashId = 0 ) override;
    virtual void				fromGuiGetFileLibraryList( uint8_t fileTypeFilter ) override;
    virtual bool				fromGuiGetIsFileInLibrary( const char * fileName ) override;
    virtual bool				fromGuiIsMyP2PWebVideoFile( const char * fileName ) override;
    virtual bool				fromGuiIsMyP2PWebAudioFile( const char * fileName ) override;

    virtual int					fromGuiDeleteFile( const char * fileName, bool shredFile ) override;

    virtual void				fromGuiQuerySessionHistory( VxGUID& historyId ) override;
    virtual bool				fromGuiMultiSessionAction( EMSessionAction mSessionAction, VxGUID& onlineId, int pos0to100000, VxGUID lclSessionId = VxGUID::nullVxGUID() ) override;

	//========================================================================
	// to gui
	//========================================================================
    void						sendToGuiStatusMessage( const char * statusMsg, ... );
	void						toGuiPluginPermissionChange( PktAnnounce * poPktAnn );

	void						toGuiContactDescChange( PktAnnounce * poPktAnn );
	void						toGuiContactNameChange( PktAnnounce * poPktAnn );
	void						toGuiContactMyFriendshipChange( PktAnnounce * poPktAnn );
	void						toGuiContactHisFriendshipChange( PktAnnounce * poPktAnn );
	void						toGuiContactSearchFlagsChange( PktAnnounce * poPktAnn );
	void						toGuiContactConnectionChange( PktAnnounce * poPktAnn );
	void						toGuiContactAnythingChange( PktAnnounce * poPktAnn );

	int 						toGuiSendAdministratorList( int iSentCnt, int iMaxSendCnt );
	int 						toGuiSendFriendList( int iSentCnt, int iMaxSendCnt );
	int 						toGuiSendGuestList( int iSentCnt, int iMaxSendCnt );
	int							toGuiSendAnonymousList( int iSentCnt, int iMaxSendCnt );
	int							toGuiSendIgnoreList( int iSentCnt, int iMaxSendCnt );
	int							toGuiSendMyProxiesList( int iSentCnt, int iMaxSendCnt );
	int							toGuiSendAllProxiesList( int iSentCnt, int iMaxSendCnt );
	//========================================================================
	// asset mgr callbacks
	//========================================================================
    virtual void				callbackFileWasShredded( std::string& fileName ) override;
    virtual void				callbackAssetAdded( AssetInfo * assetInfo ) override;
    virtual void				callbackAssetRemoved( AssetInfo * assetInfo ) override;

	virtual void				callbackSharedFileTypesChanged( uint16_t fileTypes );
	virtual void				callbackSharedPktFileListUpdated( void );

    virtual void				callbackAssetHistory( void * userData, AssetInfo * assetInfo ) override;
    //========================================================================
    // host list mgr callbacks
    //========================================================================
    virtual void				callbackBlobAdded( BlobInfo * assetInfo ) override;
    virtual void				callbackBlobRemoved( BlobInfo * assetInfo ) override;
    virtual void				callbackBlobHistory( void * userData, BlobInfo * assetInfo ) override;

	//========================================================================
	// media processor callbacks
	//========================================================================
    virtual void				callbackVideoJpgBig( void * userData, VxGUID& vidFeedId, uint8_t * jpgData, uint32_t jpgDataLen ) override;
    virtual void				callbackVideoJpgSmall(	void * userData, VxGUID& vidFeedId, uint8_t * jpgData, uint32_t jpgDataLen, int motion0to100000 ) override;

	//========================================================================
	//========================================================================
	void						onOncePerSecond( void );
	void						onOncePer30Seconds( void );
	void						onOncePerMinute( void );
	void						onOncePerHour( void );

    // called by timer thread to conserve main thread cpu time
    void                        enableTimerThread( bool enable );

    void                        executeTimerThreadFunctions( void );
    void						onThreadOncePerSecond( void );
    void						onThreadOncePer30Seconds( void );
    void						onThreadOncePerMinute( void );
    void						onThreadOncePer15Minutes( void );
    void						onThreadOncePer30Minutes( void );
    void						onThreadOncePerHour( void );

	void						onBigListInfoRestored( BigListInfo * poInfo ); 
	void						onBigListLoadComplete( RCODE rc );
	void						onBigListInfoDelete( BigListInfo * poInfo );

	virtual	void				onContactConnected		( RcConnectInfo * poInfo, bool connectionListLocked, bool newContact = false );
	virtual	void				onContactDisconnected	( RcConnectInfo * poInfo, bool connectionListLocked );
	virtual	void				onContactNearby			( BigListInfo * poBigListInfo );
	virtual	void				onContactNotNearby		( BigListInfo * poBigListInfo );

	void						onConnectionLost( VxSktBase * sktBase );
	void						onSessionStart( EPluginType ePluginType, VxNetIdent * netIdent );
	//========================================================================
	//========================================================================

	void						handleTcpData( VxSktBase * sktBase );
	void						handleMulticastData( VxSktBase * sktBase );

	std::string					describeContact( BigListInfo * bigListInfo );
	std::string					describeContact( VxConnectInfo& connectInfo );
	std::string					describeContact( ConnectRequest& connectRequest );

	void						broadcastSystemPkt( VxPktHdr * pkt, bool onlyIncludeMyContacts );
	void						broadcastSystemPkt( VxPktHdr * pkt, VxGUIDList& retIdsSentPktTo );

	virtual bool				txSystemPkt(	VxNetIdentBase *	netIdent,
												VxSktBase *			sktBase, 
												VxPktHdr *			poPkt, 
												bool				bDisconnectAfterSend = false );

    virtual bool				txSystemPkt(	const VxGUID&		destOnlineId,
												VxSktBase *			sktBase, 
												VxPktHdr *			poPkt, 
												bool				bDisconnectAfterSend = false );

	virtual bool				txImAlivePkt(	VxGUID&				destOnlineId, 
												VxSktBase *			sktBase );

	virtual void				replaceConnection( VxNetIdent * netIdent, VxSktBase * poOldSkt, VxSktBase * poNewSkt );

	bool						connectToContact(	VxConnectInfo&		connectInfo, 
													VxSktBase **		ppoRetSkt,
													bool&				retIsNewConnection,
													EConnectReason		connectReason = eConnectReasonStayConnected );

	virtual void				doPktAnnHasChanged( bool connectionListIsLocked );

	bool						shouldInfoBeInDatabase( BigListInfo * poInfo );

	//! called if hacker offense is detected
	void						hackerOffense(	VxNetIdent *	poContactIdent,			// users identity info ( may be null if not known then use ipAddress )
												EHackerLevel	hackLevel,			    // 0=unknown 1=suspicious 2=medium 3=severe
												InetAddress		IpAddr,					// ip address if identity not known
												const char *	pMsg, ... );			// message about the offense

    void                        hackerOffense(  VxNetIdent *	poContactIdent,			// users identity info ( may be null if not known then use ipAddress )
                                                EHackerLevel	hackLevel,			    // 1=severe 2=medium 3=suspicious
                                                const char *	pMsg, ... );			// message about the offense

	virtual void				onPotentialRelayServiceAvailable( RcConnectInfo * poConnection, bool connectionListIsLocked );
	virtual void				onRelayServiceAvailable( RcConnectInfo * poConnection, bool connectionListIsLocked );
	virtual void				onRelayServiceUnavailable( RcConnectInfo * poConnection, bool connectionListIsLocked );
protected:
	//========================================================================
	// pkt handlers
	//========================================================================

    //=== packet handlers ===//
    virtual void				onPktUnhandled				( VxSktBase * sktBase, VxPktHdr * pktHdr );
    virtual void				onPktInvalid				( VxSktBase * sktBase, VxPktHdr * pktHdr );

    virtual void				onPktAnnounce				( VxSktBase * sktBase, VxPktHdr * pktHdr );
    virtual void				onPktAnnList				( VxSktBase * sktBase, VxPktHdr * pktHdr );

    virtual void				onPktLogReq					( VxSktBase * sktBase, VxPktHdr * pktHdr );
    virtual void				onPktLogReply				( VxSktBase * sktBase, VxPktHdr * pktHdr );

    virtual void				onPktScanReq				( VxSktBase * sktBase, VxPktHdr * pktHdr );
    virtual void				onPktScanReply			    ( VxSktBase * sktBase, VxPktHdr * pktHdr );

    virtual void				onPktPluginOfferReq			( VxSktBase * sktBase, VxPktHdr * pktHdr );
    virtual void				onPktPluginOfferReply		( VxSktBase * sktBase, VxPktHdr * pktHdr );

    virtual void				onPktChatReq				( VxSktBase * sktBase, VxPktHdr * pktHdr );
    virtual void				onPktChatReply				( VxSktBase * sktBase, VxPktHdr * pktHdr );

    virtual void				onPktVoiceReq				( VxSktBase * sktBase, VxPktHdr * pktHdr );
    virtual void				onPktVoiceReply				( VxSktBase * sktBase, VxPktHdr * pktHdr );

    virtual void				onPktVideoFeedReq			( VxSktBase * sktBase, VxPktHdr * pktHdr );
    virtual void				onPktVideoFeedStatus		( VxSktBase * sktBase, VxPktHdr * pktHdr );
    virtual void				onPktVideoFeedPic			( VxSktBase * sktBase, VxPktHdr * pktHdr );
    virtual void				onPktVideoFeedPicChunk		( VxSktBase * sktBase, VxPktHdr * pktHdr );
    virtual void				onPktVideoFeedPicAck		( VxSktBase * sktBase, VxPktHdr * pktHdr );

    virtual void				onPktFileGetReq				( VxSktBase * sktBase, VxPktHdr * pktHdr );
    virtual void				onPktFileGetReply			( VxSktBase * sktBase, VxPktHdr * pktHdr );
    virtual void				onPktFileSendReq			( VxSktBase * sktBase, VxPktHdr * pktHdr );
    virtual void				onPktFileSendReply			( VxSktBase * sktBase, VxPktHdr * pktHdr );
    virtual void				onPktFindFileReq			( VxSktBase * sktBase, VxPktHdr * pktHdr );
    virtual void				onPktFindFileReply			( VxSktBase * sktBase, VxPktHdr * pktHdr );
    virtual void				onPktFileListReq			( VxSktBase * sktBase, VxPktHdr * pktHdr );
    virtual void				onPktFileListReply			( VxSktBase * sktBase, VxPktHdr * pktHdr );

    virtual void				onPktFileInfoReq			( VxSktBase * sktBase, VxPktHdr * pktHdr );

    virtual void				onPktFileChunkReq			( VxSktBase * sktBase, VxPktHdr * pktHdr );
    virtual void				onPktFileChunkReply			( VxSktBase * sktBase, VxPktHdr * pktHdr );
    virtual void				onPktFileSendCompleteReq	( VxSktBase * sktBase, VxPktHdr * pktHdr );
    virtual void				onPktFileSendCompleteReply	( VxSktBase * sktBase, VxPktHdr * pktHdr );
    virtual void				onPktFileGetCompleteReq		( VxSktBase * sktBase, VxPktHdr * pktHdr );
    virtual void				onPktFileGetCompleteReply	( VxSktBase * sktBase, VxPktHdr * pktHdr );
    virtual void				onPktFileShareErr			( VxSktBase * sktBase, VxPktHdr * pktHdr );

    virtual void				onPktAssetSendReq			( VxSktBase * sktBase, VxPktHdr * pktHdr );
    virtual void				onPktAssetSendReply			( VxSktBase * sktBase, VxPktHdr * pktHdr );
    virtual void				onPktAssetChunkReq			( VxSktBase * sktBase, VxPktHdr * pktHdr );
    virtual void				onPktAssetChunkReply		( VxSktBase * sktBase, VxPktHdr * pktHdr );
    virtual void				onPktAssetSendCompleteReq	( VxSktBase * sktBase, VxPktHdr * pktHdr );
    virtual void				onPktAssetSendCompleteReply	( VxSktBase * sktBase, VxPktHdr * pktHdr );
    virtual void				onPktAssetXferErr			( VxSktBase * sktBase, VxPktHdr * pktHdr );

    virtual void				onPktMultiSessionReq		( VxSktBase * sktBase, VxPktHdr * pktHdr );
    virtual void				onPktMultiSessionReply		( VxSktBase * sktBase, VxPktHdr * pktHdr );

    virtual void				onPktSessionStartReq		( VxSktBase * sktBase, VxPktHdr * pktHdr );
    virtual void				onPktSessionStartReply		( VxSktBase * sktBase, VxPktHdr * pktHdr );
    virtual void				onPktSessionStopReq			( VxSktBase * sktBase, VxPktHdr * pktHdr );
    virtual void				onPktSessionStopReply		( VxSktBase * sktBase, VxPktHdr * pktHdr );

    virtual void				onPktRelayServiceReq		( VxSktBase * sktBase, VxPktHdr * pktHdr );
    virtual void				onPktRelayServiceReply		( VxSktBase * sktBase, VxPktHdr * pktHdr );
    virtual void				onPktRelayConnectReq		( VxSktBase * sktBase, VxPktHdr * pktHdr );
    virtual void				onPktRelayConnectReply		( VxSktBase * sktBase, VxPktHdr * pktHdr );
    virtual void				onPktRelayDisconnect		( VxSktBase * sktBase, VxPktHdr * pktHdr );
    virtual void				onPktRelaySessionReq		( VxSktBase * sktBase, VxPktHdr * pktHdr );
    virtual void				onPktRelaySessionReply		( VxSktBase * sktBase, VxPktHdr * pktHdr );
    virtual void				onPktRelayConnectToUserReq	( VxSktBase * sktBase, VxPktHdr * pktHdr );
    virtual void				onPktRelayConnectToUserReply( VxSktBase * sktBase, VxPktHdr * pktHdr );
    virtual void				onPktRelayUserDisconnect	( VxSktBase * sktBase, VxPktHdr * pktHdr );
    virtual void				onPktRelayTestReq			( VxSktBase * sktBase, VxPktHdr * pktHdr );
    virtual void				onPktRelayTestReply			( VxSktBase * sktBase, VxPktHdr * pktHdr );

    virtual void				onPktMyPicSendReq			( VxSktBase * sktBase, VxPktHdr * pktHdr );
    virtual void				onPktMyPicSendReply			( VxSktBase * sktBase, VxPktHdr * pktHdr );
    virtual void				onPktWebServerPicChunkTx	( VxSktBase * sktBase, VxPktHdr * pktHdr );
    virtual void				onPktWebServerPicChunkAck	( VxSktBase * sktBase, VxPktHdr * pktHdr );
    virtual void				onPktWebServerGetChunkTx	( VxSktBase * sktBase, VxPktHdr * pktHdr );
    virtual void				onPktWebServerGetChunkAck	( VxSktBase * sktBase, VxPktHdr * pktHdr );
    virtual void				onPktWebServerPutChunkTx	( VxSktBase * sktBase, VxPktHdr * poPktHdrt );
    virtual void				onPktWebServerPutChunkAck	( VxSktBase * sktBase, VxPktHdr * pktHdr );

    virtual void				onPktTodGameStats			( VxSktBase * sktBase, VxPktHdr * pktHdr );
    virtual void				onPktTodGameAction			( VxSktBase * sktBase, VxPktHdr * pktHdr );
    virtual void				onPktTodGameValue			( VxSktBase * sktBase, VxPktHdr * pktHdr );

    virtual void				onPktTcpPunch				( VxSktBase * sktBase, VxPktHdr * pktHdr );

    virtual void				onPktPingReq				( VxSktBase * sktBase, VxPktHdr * pktHdr );
    virtual void				onPktPingReply				( VxSktBase * sktBase, VxPktHdr * pktHdr );

    virtual void				onPktImAliveReq				( VxSktBase * sktBase, VxPktHdr * pktHdr );
    virtual void				onPktImAliveReply			( VxSktBase * sktBase, VxPktHdr * pktHdr );

    virtual void				onPktPluginSettingReq       ( VxSktBase * sktBase, VxPktHdr * pktHdr );
    virtual void				onPktPluginSettingReply     ( VxSktBase * sktBase, VxPktHdr * pktHdr );

    virtual void				onPktThumbSettingReq        ( VxSktBase * sktBase, VxPktHdr * pktHdr );
    virtual void				onPktThumbSettingReply      ( VxSktBase * sktBase, VxPktHdr * pktHdr );
    virtual void				onPktThumbSettingChunkReq   ( VxSktBase * sktBase, VxPktHdr * pktHdr );
    virtual void				onPktThumbSettingChunkReply ( VxSktBase * sktBase, VxPktHdr * pktHdr );

    virtual void				onPktThumbAvatarReq         ( VxSktBase * sktBase, VxPktHdr * pktHdr );
    virtual void				onPktThumbAvatarReply       ( VxSktBase * sktBase, VxPktHdr * pktHdr );
    virtual void				onPktThumbAvatarChunkReq    ( VxSktBase * sktBase, VxPktHdr * pktHdr );
    virtual void				onPktThumbAvatarChunkReply  ( VxSktBase * sktBase, VxPktHdr * pktHdr );

    virtual void				onPktThumbFileReq           ( VxSktBase * sktBase, VxPktHdr * pktHdr );
    virtual void				onPktThumbFileReply         ( VxSktBase * sktBase, VxPktHdr * pktHdr );
    virtual void				onPktThumbFileChunkReq      ( VxSktBase * sktBase, VxPktHdr * pktHdr );
    virtual void				onPktThumbFileChunkReply    ( VxSktBase * sktBase, VxPktHdr * pktHdr );

    virtual void				onPktHostAnnounce           ( VxSktBase * sktBase, VxPktHdr * pktHdr );

    virtual void				onPktBlobSendReq            ( VxSktBase * sktBase, VxPktHdr * pktHdr );
    virtual void				onPktBlobSendReply          ( VxSktBase * sktBase, VxPktHdr * pktHdr );
    virtual void				onPktBlobChunkReq           ( VxSktBase * sktBase, VxPktHdr * pktHdr );
    virtual void				onPktBlobChunkReply         ( VxSktBase * sktBase, VxPktHdr * pktHdr );
    virtual void				onPktBlobSendCompleteReq    ( VxSktBase * sktBase, VxPktHdr * pktHdr );
    virtual void				onPktBlobSendCompleteReply  ( VxSktBase * sktBase, VxPktHdr * pktHdr );
    virtual void				onPktBlobXferErr            ( VxSktBase * sktBase, VxPktHdr * pktHdr );

    virtual void				onPktHostJoinReq            ( VxSktBase * sktBase, VxPktHdr * pktHdr );
    virtual void				onPktHostJoinReply          ( VxSktBase * sktBase, VxPktHdr * pktHdr );
    virtual void				onPktHostSearchReq          ( VxSktBase * sktBase, VxPktHdr * pktHdr );
    virtual void				onPktHostSearchReply        ( VxSktBase * sktBase, VxPktHdr * pktHdr );
    virtual void				onPktHostOfferReq           ( VxSktBase * sktBase, VxPktHdr * pktHdr );
    virtual void				onPktHostOfferReply         ( VxSktBase * sktBase, VxPktHdr * pktHdr );
    virtual void				onPktFriendOfferReq         ( VxSktBase * sktBase, VxPktHdr * pktHdr );
    virtual void				onPktFriendOfferReply       ( VxSktBase * sktBase, VxPktHdr * pktHdr );

    virtual void				onPktThumbSendReq           ( VxSktBase * sktBase, VxPktHdr * pktHdr );
    virtual void				onPktThumbSendReply         ( VxSktBase * sktBase, VxPktHdr * pktHdr );
    virtual void				onPktThumbChunkReq          ( VxSktBase * sktBase, VxPktHdr * pktHdr );
    virtual void				onPktThumbChunkReply        ( VxSktBase * sktBase, VxPktHdr * pktHdr );
    virtual void				onPktThumbSendCompleteReq   ( VxSktBase * sktBase, VxPktHdr * pktHdr );
    virtual void				onPktThumbSendCompleteReply ( VxSktBase * sktBase, VxPktHdr * pktHdr );
    virtual void				onPktThumbXferErr           ( VxSktBase * sktBase, VxPktHdr * pktHdr );

    //========================================================================
    //========================================================================
    void						iniitializePtoPEngine( void );

	virtual bool				txPluginPkt( 	EPluginType			ePluginType, 
												VxNetIdentBase *	netIdent, 
												VxSktBase *			sktBase, 
												VxPktHdr *			poPkt, 
												bool				bDisconnectAfterSend );

	virtual void				doAppStateChange( EAppState eAppState );
	virtual bool				shouldNotifyGui( VxNetIdent * netIdent );

	// pkt ann has changed and needs to be re announced
	void						doPktAnnConnectionInfoChanged( bool connectionListIsLocked );
	virtual	void				attemptConnectionToRelayService( BigListInfo * poInfo );
	void						handleIncommingRelayData( VxSktBase * sktBase, VxPktHdr * pktHdr );
	void						sendToGuiTheContactList( int maxContactsToSend );

	//=== vars ===//
	VxPeerMgr&					m_PeerMgr;
    BigListMgr&					m_BigListMgr;
	PktAnnounce					m_PktAnn;
    VxGUID                      m_MyOnlineId;
	VxMutex						m_AnnouncePktMutex;
	EngineSettings				m_EngineSettings;
	EngineParams				m_EngineParams;
    NetStatusAccum              m_NetStatusAccum;
	AssetMgr&					m_AssetMgr;
    BlobMgr&				    m_BlobMgr;
    ThumbMgr&					m_ThumbMgr;
    ConnectionMgr&              m_ConnectionMgr;
	P2PConnectList				m_ConnectionList;
    MediaProcessor&				m_MediaProcessor;
    NetworkMgr&					m_NetworkMgr;
	NetworkMonitor&				m_NetworkMonitor;
	NetServicesMgr&				m_NetServicesMgr;
	NetConnector&				m_NetConnector;
	NetworkStateMachine&		m_NetworkStateMachine;

	PluginMgr&					m_PluginMgr;
    PluginSettingMgr			m_PluginSettingMgr;

	PluginServiceRelay *		m_PluginServiceRelay;
	PluginServiceFileShare *	m_PluginServiceFileShare;
	PluginNetServices *			m_PluginNetServices;
	IsPortOpenTest&				m_IsPortOpenTest;
    RunUrlAction&			    m_RunUrlAction;

	RcScan						m_RcScan;

    EAppState					m_eAppState{ eAppStateInvalid };

    EFriendViewType				m_eFriendView{ eFriendViewEverybody };
	unsigned int				m_iCurPreferredRelayConnectIdx{ 0 };
	VxGUID						m_NextFileInstance;
    int64_t						m_LastTimeAnnounceFromContactListCalled{ 0 };
    bool						m_AppStartupCalled{ false };
	bool						m_AppIsPaused{ false };
	bool						m_IsUserSpecificDirSet{ false };
    bool                        m_EngineInitialized{ false };

	PktImAliveReq				m_PktImAliveReq;

    VxThread                    m_TimerThread;

private:
	P2PEngine() = delete; // don't allow default constructor
	P2PEngine( const P2PEngine& ) = delete; // don't allow copy constructor
};

extern P2PEngine& GetPtoPEngine();