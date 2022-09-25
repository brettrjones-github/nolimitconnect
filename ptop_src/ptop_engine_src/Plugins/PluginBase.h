#pragma once
//============================================================================
// Copyright (C) 2010 Brett R. Jones
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

#include <GuiInterface/IDefs.h>
#include <PktLib/PktPluginHandlerBase.h>
#include <PktLib/VxCommon.h>
#include <ptop_src/ptop_engine_src/PluginSettings/PluginSetting.h>
#include <ptop_src/ptop_engine_src/AssetBase/BaseXferInterface.h>
#include <ptop_src/ptop_engine_src/AssetMgr/AssetMgr.h>
#include <ptop_src/ptop_engine_src/ThumbMgr/ThumbMgr.h>
#include <ptop_src/ptop_engine_src/ThumbMgr/ThumbXferMgr.h>

#include <CoreLib/VxMutex.h>
#include <CoreLib/MediaCallbackInterface.h>

class AssetMgr;
class AssetBaseInfo;
class FileShareSettings;
class GroupieId;
class IToGui;
class NetServiceHdr;
class P2PEngine;
class P2PSession;
class PktAnnounce;
class PktHostInviteAnnounceReq;
class PluginMgr;
class PluginSessionBase;
class PluginSetting;
class RxSession;
class SearchParams;
class ThumbMgr;
class TxSession;
class VxSktBase;
class HostedInfo;
class GroupieInfo;

class PluginBase : public PktPluginHandlerBase, public MediaCallbackInterface, public BaseXferInterface
{
public:
	class AutoPluginLock
	{
	public:
		AutoPluginLock( PluginBase * pluginBase ) 
		: m_Mutex(pluginBase->getPluginMutex())	
		{ 
			m_Mutex.lock(); 
		}
		~AutoPluginLock()
		{ 
			m_Mutex.unlock(); 
		}

		VxMutex&						m_Mutex;
	};

	PluginBase(	P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent* netIdent, EPluginType pluginType );
	virtual ~PluginBase() override = default;

	virtual void				lockPlugin( void )										{ m_PluginMutex.lock(); }
	virtual void				unlockPlugin( void )									{ m_PluginMutex.unlock(); }

    virtual void				pluginStartup( void );
    virtual void				pluginShutdown( void )									{}
	virtual bool				isPluginEnabled( void );
	virtual EFriendState		getPluginPermission( void );
	virtual void				setPluginPermission( EFriendState eFriendState );

	virtual void				onMyOnlineUrlIsValid( bool isValidUrl )					{};

	virtual bool				isAccessAllowed( VxNetIdent* netIdent, bool logAccessError = true, const char* accessReason = nullptr );

	virtual bool				isAppPaused( void )										{ return m_AppIsPaused; }
	virtual void				setIsPluginInSession( bool inSession )					{ m_bPluginIsInSession = inSession; }
	virtual bool				getIsPluginInSession( void )							{ return m_bPluginIsInSession; }

	virtual void				setIsServerInSession( bool inSession )					{ m_ServerIsInSession = inSession; }
	virtual bool				getIsServerInSession( void )							{ return m_ServerIsInSession; }

	//=== getter/setters ===//
	virtual P2PEngine&			getEngine( void )										{ return m_Engine; }
    virtual IToGui&			    getToGui( void );
    virtual void				setPluginType( EPluginType ePluginType );
    virtual EPluginType			getPluginType( void ) override							{ return m_ePluginType; }
    virtual bool                setPluginSetting( PluginSetting& pluginSetting, int64_t modifiedTimeMs = 0 );
    virtual PluginSetting&      getPluginSetting( void )                                { return m_PluginSetting; }
    virtual EHostType			getHostType( void );

	virtual PluginMgr&			getPluginMgr( void )									{ return m_PluginMgr;	}
	virtual	VxMutex&			getPluginMutex( void )									{ return m_PluginMutex; }	

    virtual	VxMutex&			getAssetXferMutex( void ) override						{ return m_PluginMutex; }					

	virtual	EAppState			getPluginState( void );
	virtual	void				setPluginState( EAppState ePluginState )				{ m_ePluginState = ePluginState;};

	EPluginAccess				getPluginAccessState( VxNetIdent* netIdent );

    virtual ThumbMgr&			getThumbMgr( void )									    { return m_ThumbMgr; }
    virtual ThumbXferMgr&       getThumbXferMgr( void )							        { return m_ThumbXferMgr; }

	virtual EMembershipState	getMembershipState( VxNetIdent* netIdent )				{ return eMembershipStateJoinDenied; }

	virtual bool				getHostedInfo( HostedInfo& hostedInfo )					{ return false; }
	virtual bool				getGroupieInfo( GroupieInfo& hostedInfo )				{ return false; }

	virtual void				fromGuiUserLoggedOn( void )								{};

	virtual void				fromGuiStartPluginSession( PluginSessionBase * poOffer )	{};
    virtual void				fromGuiStartPluginSession( VxNetIdent* netIdent = nullptr,	int pvUserData = 0, VxGUID lclSessionId = VxGUID::nullVxGUID() )	{};
    virtual void				fromGuiStopPluginSession( VxNetIdent* netIdent = nullptr,	int pvUserData = 0, VxGUID lclSessionId = VxGUID::nullVxGUID() )	{};
    virtual bool				fromGuiIsPluginInSession( VxNetIdent* netIdent = nullptr,	int pvUserData = 0, VxGUID lclSessionId = VxGUID::nullVxGUID() )	{ return true; }

	virtual void				fromGuiGetFileShareSettings( FileShareSettings& fileShareSettings );
	virtual void				fromGuiSetFileShareSettings( FileShareSettings& fileShareSettings );
	virtual int					fromGuiDeleteFile( const char * fileName, bool shredFile )	{ return 0; }

	virtual void				fromGuiCancelDownload( VxGUID& fileInstance ) {};
	virtual void				fromGuiCancelUpload( VxGUID& fileInstance ) {};
	virtual bool				fromGuiMakePluginOffer( VxNetIdent *	netIdent,				// identity of friend
														int				pvUserData,
														const char *	pOfferMsg,				// offer message
														const char *	pFileName = NULL,
														uint8_t *		fileHashId = 0,
														VxGUID			lclSessionId = VxGUID::nullVxGUID() );		
	virtual bool				fromGuiOfferReply(	VxNetIdent *	netIdent,
													int				pvUserdata,				
													EOfferResponse	eOfferResponse,
													VxGUID			lclSessionId );

	virtual int					fromGuiPluginControl(	VxNetIdent *	netIdent,
														const char *	pControl, 
														const char *	pAction,
														uint32_t		u32ActionData,
														VxGUID&			fileId,
														uint8_t *		fileHashId );

	virtual bool				fromGuiInstMsg(	VxNetIdent* netIdent, const char *	pMsg );
	virtual bool				fromGuiPushToTalk( VxNetIdent* netIdent, bool enableTalk );

	virtual bool				fromGuiSetGameValueVar(	VxNetIdent* netIdent, int32_t varId, int32_t varValue )		{ return false; };
	virtual bool				fromGuiSetGameActionVar( VxNetIdent* netIdent, int32_t	actionId, int32_t actionValue )	{ return false; };

	virtual void				fromGuiRelayPermissionCount( int userPermittedCount, int anonymousCount )				{};

	virtual void				fromGuiUpdatePluginPermission( EPluginType pluginType, EFriendState pluginPermission )	{};

	virtual void				onAppStartup( void );
	virtual void				onAppShutdown( void );
	virtual void				fromGuiAppPause( void );
	virtual void				fromGuiAppResume( void );

	virtual void				onSpeexData( uint16_t * pu16SpeexData, uint16_t u16SpeexDataLen )				    {};
	virtual void				fromGuiVideoData( uint8_t * pu8VidData, uint32_t u32VidDataLen, int iRotation )	    {};
    virtual bool				fromGuiSendAsset( AssetBaseInfo& assetInfo )                                        { return false; };
	virtual bool				fromGuiMultiSessionAction( VxNetIdent *	netIdent, EMSessionAction mSessionAction, int pos0to100000, VxGUID lclSessionId = VxGUID::nullVxGUID() ) { return false; }; 

    //=== hosting ===//
    virtual void				fromGuiAnnounceHost( EHostType hostType, VxGUID& sessionId, std::string& ptopUrl )	        {};
    virtual void				fromGuiJoinHost( EHostType hostType, VxGUID& sessionId, std::string& ptopUrl )	            {};
	virtual void				fromGuiLeaveHost( EHostType hostType, VxGUID& sessionId, std::string& ptopUrl )				{};
	virtual void				fromGuiUnJoinHost( EHostType hostType, VxGUID& sessionId, std::string& ptopUrl )			{};
    virtual void				fromGuiSearchHost( EHostType hostType, SearchParams& searchParams, bool enable )            {};
    virtual void                updateHostSearchList( EHostType hostType, PktHostInviteAnnounceReq* hostAnn, VxNetIdent* netIdent, VxSktBase* sktBase = nullptr )  {};
	virtual void				fromGuiSendAnnouncedList( EHostType hostType, VxGUID& sessionId ) {};

	virtual bool				fromGuiRequestPluginThumb( VxNetIdent* netIdent, VxGUID& thumbId ) { return false; }
	virtual void				fromGuiListAction( EListAction listAction ) {};

	virtual bool				fromGuiDownloadWebPage( EWebPageType webPageType, VxGUID& onlineId ) { return false; }
	virtual bool				fromGuiCancelWebPage( EWebPageType webPageType, VxGUID& onlineId ) { return false; }

	virtual void				toGuiRxedPluginOffer( VxNetIdent*		netIdent,				// identity of friend
														EPluginType		ePluginType,			// plugin type
														const char*		pOfferMsg,				// offer message
														int				pvUserData,				// plugin defined data
														const char*		pFileName = NULL,		// filename if any
														uint8_t*		fileHashData = 0,
														VxGUID&			lclSessionId = VxGUID::nullVxGUID(),
														VxGUID&			rmtSessionId = VxGUID::nullVxGUID() ) {};
	virtual void				toGuiRxedOfferReply( VxNetIdent*		netIdent,
														EPluginType		ePluginType,
														int				pvUserData,
														EOfferResponse	eOfferResponse,
														const char*		pFileName = 0,
														uint8_t*		fileHashData = 0,
														VxGUID&			lclSessionId = VxGUID::nullVxGUID(),
														VxGUID&			rmtSessionId = VxGUID::nullVxGUID() ) {};
	virtual void				toGuiStartUpload( VxNetIdent*		netIdent,
													EPluginType		ePluginType,
													VxGUID&			lclSessionId,
													uint8_t			u8FileType,
													uint64_t		u64FileLen,
													const char*		pFileName,
													VxGUID			assetId,
													uint8_t*		fileHashData ) {};

	virtual void				toGuiStartDownload( VxNetIdent*		netIdent,
													EPluginType		ePluginType,
													VxGUID&			lclSessionId,
													uint8_t			u8FileType,
													uint64_t		u64FileLen,
													const char*		pFileName,
													VxGUID			assetId,
													uint8_t*		fileHashData ) {};

	virtual void				toGuiFileXferState( VxGUID& localSessionId, EXferState xferState, EXferError xferErr, int param = 0 ) {};
	virtual void				toGuiFileDownloadComplete( VxGUID& lclSessionId, const char* newFileName, EXferError xferError ) {};
	virtual void				toGuiFileUploadComplete( VxGUID& lclSessionId, EXferError xferError ) {};

    //=== connections ===//
	virtual void				onContactWentOnline( VxNetIdent* netIdent, VxSktBase* sktBase )	{};
	virtual void				onContactWentOffline( VxNetIdent* netIdent, VxSktBase* sktBase ) = 0;
	virtual void				onConnectionLost( VxSktBase* sktBase ) = 0;
	virtual void				replaceConnection( VxNetIdent* netIdent, VxSktBase * poOldSkt, VxSktBase * poNewSkt ) = 0;

    bool						txPacket( VxNetIdent* netIdent, VxSktBase* sktBase, VxPktHdr * poPkt, bool bDisconnectAfterSend = false ) override;
    bool						txPacket( const VxGUID& onlineId, VxSktBase* sktBase, VxPktHdr * poPkt, bool bDisconnectAfterSend = false, EPluginType overridePlugin = ePluginTypeInvalid );

    //=== maintenence ===//
	virtual void				onSharedFilesUpdated( uint16_t u16FileTypes )									{};
    virtual void				onMyPktAnnounceChange( PktAnnounce& pktAnn )									{};
    virtual void				onThreadOncePer15Minutes( void )												{};
	virtual void				onAfterUserLogOnThreaded( void )												{};
    virtual	void				onPluginSettingChange( PluginSetting& pluginSetting, int64_t modifiedTimeMs )   {};

    //=== packet handlers ===//
    virtual void				onPktUserConnect			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
    virtual void				onPktUserDisconnect			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
    virtual void				onPktPluginOfferReq			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;
    virtual void				onPktPluginOfferReply		( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;

    virtual void				onPktSessionStartReq		( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;
    virtual void				onPktSessionStartReply		( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;
    virtual void				onPktSessionStopReq			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;
    virtual void				onPktSessionStopReply		( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;

    virtual void				onPktMyPicSendReq			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;
    virtual void				onPktMyPicSendReply			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;
    virtual void				onPktWebServerPicChunkTx	( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;
    virtual void				onPktWebServerPicChunkAck	( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;
    virtual void				onPktWebServerGetChunkTx	( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;
    virtual void				onPktWebServerGetChunkAck	( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;
    virtual void				onPktWebServerPutChunkTx	( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;
    virtual void				onPktWebServerPutChunkAck	( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;

    virtual void				onPktThumbGetReq            ( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;
    virtual void				onPktThumbGetReply          ( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;
    virtual void				onPktThumbSendReq           ( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;
    virtual void				onPktThumbSendReply         ( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;
    virtual void				onPktThumbChunkReq          ( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;
    virtual void				onPktThumbChunkReply        ( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;
    virtual void				onPktThumbGetCompleteReq    ( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;
    virtual void				onPktThumbGetCompleteReply  ( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;
    virtual void				onPktThumbSendCompleteReq   ( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;
    virtual void				onPktThumbSendCompleteReply ( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;
    virtual void				onPktThumbXferErr           ( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;

    // error handling for invalid packet
    virtual void				onInvalidRxedPacket( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent, const char * msg = "" );

    //=== sessions ===//
    virtual void				onSessionStart( PluginSessionBase * poSession, bool pluginIsLocked );
    virtual void				onSessionEnded( PluginSessionBase * poSession, 
                                                bool pluginIsLocked,
                                                EOfferResponse eOfferResponse = eOfferResponseUserOffline ) {};

    virtual EPluginAccess	    canAcceptNewSession( VxNetIdent* netIdent );

    virtual P2PSession *		createP2PSession( VxSktBase* sktBase, VxNetIdent* netIdent );
    virtual P2PSession *		createP2PSession( VxGUID& lclSessionId, VxSktBase* sktBase, VxNetIdent* netIdent );
    virtual RxSession *			createRxSession( VxSktBase* sktBase, VxNetIdent* netIdent );
    virtual RxSession *			createRxSession( VxGUID& lclSessionId, VxSktBase* sktBase, VxNetIdent* netIdent );
    virtual TxSession *			createTxSession( VxSktBase* sktBase, VxNetIdent* netIdent );
    virtual TxSession *			createTxSession( VxGUID& lclSessionId, VxSktBase* sktBase, VxNetIdent* netIdent );

    //=== http ===//
	virtual void				handlePluginSpecificSkt( VxSktBase* sktBase ) {};
	virtual RCODE				handlePtopConnection( VxSktBase* sktBase, NetServiceHdr& netServiceHdr )		{ return -1; }
	virtual RCODE				handlePtopConnection( VxSktBase* sktBase, VxNetIdent* netIdent )				{ return -1; }

    virtual EPluginType         getDestinationPluginOverride( EHostType hostType );

	virtual bool				ptopEngineRequestPluginThumb( VxSktBase* sktBase, VxNetIdent* netIdent, VxGUID& thumbId ) { return false; }

	virtual void				onLoadedFilesReady( int64_t lastFileUpdateTime, int64_t totalBytes, uint16_t fileTypes ) {};
	virtual void				onFilesChanged( int64_t lastFileUpdateTime, int64_t totalBytes, uint16_t fileTypes ) {};

	virtual std::string			getIncompleteFileXferDirectory( VxGUID& onlineId ) { return ""; }
	virtual bool				onFileDownloadComplete( VxNetIdent* netIdent, VxSktBase* sktBase, VxGUID& lclSessionId, std::string& fileName, VxGUID& assetId, VxSha1Hash& sha11Hash ) { return true;  }

	virtual void                onUserJoinedHost( GroupieId& groupieId, VxSktBase* sktBase, VxNetIdent* netIdent ) {};
	virtual void                onUserLeftHost( GroupieId& groupieId, VxSktBase* sktBase, VxNetIdent* netIdent ) {};
	virtual void                onUserUnJoinedHost( GroupieId& groupieId, VxSktBase* sktBase, VxNetIdent* netIdent ) {};
	virtual void                onGroupDirectUserAnnounce( GroupieId& groupieId, VxSktBase* sktBase, VxNetIdent* netIdent ) {};
	virtual void                onGroupRelayedUserAnnounce( GroupieId& groupieId, VxSktBase* sktBase, VxNetIdent* netIdent ) {};

	virtual	void				onNetworkConnectionReady( bool requiresRelay ) {};

protected:
	virtual void				makeShortFileName( const char * pFullFileName, std::string& strShortFileName );

    static std::string          getThumbXferDbName( EPluginType pluginType );
    static std::string          getThumbXferThreadName( EPluginType pluginType );
	void						logCommError( ECommErr commErr, const char* desc, VxSktBase* sktBase, VxNetIdent* netIdent );
	ECommErr					getCommAccessState( VxNetIdent* netIdent );

	//=== vars ===//
	EPluginType					m_ePluginType = ePluginTypeInvalid;
	P2PEngine&					m_Engine;
	PluginMgr&					m_PluginMgr;
    AssetMgr&                   m_AssetMgr;
    ThumbMgr&                   m_ThumbMgr;
    ThumbXferMgr                m_ThumbXferMgr; 
	
	VxNetIdent *				m_MyIdent = nullptr;

	EAppState					m_ePluginState = eAppStateInvalid;
	VxMutex						m_PluginMutex;
    PluginSetting               m_PluginSetting;

	bool						m_bPluginIsInSession = false;
	bool						m_ServerIsInSession = false;
	bool						m_AppIsPaused = false;
};
