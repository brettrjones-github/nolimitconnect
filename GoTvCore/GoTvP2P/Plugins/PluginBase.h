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
// http://www.nolimitconnect.com
//============================================================================

#include <GuiInterface/IDefs.h>
#include <PktLib/PktPluginHandlerBase.h>
#include <PktLib/PktsPluginSetting.h>
#include <PktLib/VxCommon.h>
#include <GoTvCore/GoTvP2P/AssetMgr/AssetXferMgr.h>
#include <GoTvCore/GoTvP2P/PluginSettings/PluginSetting.h>

#include <CoreLib/VxMutex.h>
#include <CoreLib/MediaCallbackInterface.h>

class FileShareSettings;
class IToGui;
class NetServiceHdr;
class P2PEngine;
class P2PSession;
class PktAnnounce;
class PktHostAnnounce;
class PluginMgr;
class PluginSessionBase;
class PluginSetting;
class RxSession;
class TxSession;
class VxSktBase;
class SearchParams;

class PluginBase : public PktPluginHandlerBase, public MediaCallbackInterface
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

	PluginBase(	P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent * myIdent );
	virtual ~PluginBase() override = default;

    virtual void				pluginStartup( void );
    virtual void				pluginShutdown( void ){}
	virtual bool				isPluginEnabled( void );
	virtual EFriendState		getPluginPermission( void );
	virtual void				setPluginPermission( EFriendState eFriendState );

	virtual bool				isAccessAllowed( VxNetIdent * hisIdent );
	virtual bool				isAppPaused( void )										{ return m_AppIsPaused; }
	virtual void				setIsPluginInSession( bool inSession )					{ m_bPluginIsInSession = inSession; }
	virtual bool				getIsPluginInSession( void )							{ return m_bPluginIsInSession; }

	virtual void				setIsServerInSession( bool inSession )					{ m_ServerIsInSession = inSession; }
	virtual bool				getIsServerInSession( void )							{ return m_ServerIsInSession; }

	//=== getter/setters ===//
	virtual P2PEngine&			getEngine( void )										{ return m_Engine; }
    virtual IToGui&			    getToGui( void );
    virtual void				setPluginType( EPluginType ePluginType );
    virtual EPluginType			getPluginType( void )									{ return m_ePluginType; }
    virtual bool                setPluginSetting( PluginSetting& pluginSetting );
    virtual PluginSetting&      getPluginSetting( void )                                { return m_PluginSetting; }
    virtual EHostType			getHostType( void );

	virtual PluginMgr&			getPluginMgr( void )									{ return m_PluginMgr;	}
	virtual	VxMutex&			getPluginMutex( void )									{ return m_PluginMutex; }					
	virtual	EAppState			getPluginState( void );
	virtual	void				setPluginState( EAppState ePluginState )				{ m_ePluginState = ePluginState;};

	virtual void				fromGuiUserLoggedOn( void )								{};

	virtual void				fromGuiStartPluginSession( PluginSessionBase * poOffer )	{};
    virtual void				fromGuiStartPluginSession( VxNetIdent * netIdent = nullptr,	int pvUserData = 0, VxGUID lclSessionId = VxGUID::nullVxGUID() )	{};
    virtual void				fromGuiStopPluginSession( VxNetIdent * netIdent = nullptr,	int pvUserData = 0, VxGUID lclSessionId = VxGUID::nullVxGUID() )	{};
    virtual bool				fromGuiIsPluginInSession( VxNetIdent * netIdent = nullptr,	int pvUserData = 0, VxGUID lclSessionId = VxGUID::nullVxGUID() )	{ return true; }

	virtual void				fromGuiGetFileShareSettings( FileShareSettings& fileShareSettings );
	virtual void				fromGuiSetFileShareSettings( FileShareSettings& fileShareSettings );
	virtual int					fromGuiDeleteFile( const char * fileName, bool shredFile )	{ return 0; }

	virtual void				fromGuiCancelDownload( VxGUID& fileInstance ) {};
	virtual void				fromGuiCancelUpload( VxGUID& fileInstance ) {};
	virtual bool				fromGuiMakePluginOffer( VxNetIdent *	netIdent,				// identity of friend
														int				pvUserData,
														const char *	pOfferMsg,				// offer message
														const char *	pFileName = NULL,
														uint8_t *			fileHashId = 0,
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

	virtual bool				fromGuiInstMsg(	VxNetIdent * netIdent, const char *	pMsg );

	virtual bool				fromGuiSetGameValueVar(	VxNetIdent * netIdent, int32_t varId, int32_t varValue )		{ return false; };
	virtual bool				fromGuiSetGameActionVar( VxNetIdent * netIdent, int32_t	actionId, int32_t actionValue )	{ return false; };

	virtual void				fromGuiRelayPermissionCount( int userPermittedCount, int anonymousCount ) {}; 

	virtual void				fromGuiAppIdle( void );
	virtual void				onAppStartup( void );
	virtual void				onAppShutdown( void );
	virtual void				fromGuiAppPause( void );
	virtual void				fromGuiAppResume( void );

	virtual void				onSpeexData( uint16_t * pu16SpeexData, uint16_t u16SpeexDataLen )				{};
	virtual void				fromGuiVideoData( uint8_t * pu8VidData, uint32_t u32VidDataLen, int iRotation )	{};
	virtual void				fromGuiSendAsset( AssetInfo& assetInfo )							            {};
	virtual bool				fromGuiMultiSessionAction( VxNetIdent *	netIdent, EMSessionAction mSessionAction, int pos0to100000, VxGUID lclSessionId = VxGUID::nullVxGUID() ) { return false; }; 

    //=== hosting ===//
    virtual void				fromGuiAnnounceHost( EHostType hostType, VxGUID& sessionId, const char * ptopUrl )	        {};
    virtual void				fromGuiJoinHost( EHostType hostType, VxGUID& sessionId, const char * ptopUrl )	            {};
    virtual void				fromGuiSearchHost( EHostType hostType, SearchParams& searchParams, bool enable )            {};
    virtual void                updateHostSearchList( EHostType hostType, PktHostAnnounce* hostAnn, VxNetIdent* netIdent )  {};

    //=== connections ===//
	virtual void				onContactWentOnline( VxNetIdent * netIdent, VxSktBase * sktBase )	{};
	virtual void				onContactWentOffline( VxNetIdent * netIdent, VxSktBase * sktBase ) = 0;
	virtual void				onConnectionLost( VxSktBase * sktBase ) = 0;
	virtual void				replaceConnection( VxNetIdent * netIdent, VxSktBase * poOldSkt, VxSktBase * poNewSkt ) = 0;

    bool						txPacket( VxNetIdent * netIdent, VxSktBase * sktBase, VxPktHdr * poPkt, bool bDisconnectAfterSend = false );
    bool						txPacket( VxGUID& onlineId, VxSktBase * sktBase, VxPktHdr * poPkt, bool bDisconnectAfterSend = false, EPluginType overridePlugin = ePluginTypeInvalid );

    //=== maintenence ===//
	virtual void				onSharedFilesUpdated( uint16_t u16FileTypes )							{};
    virtual void				onMyPktAnnounceChange( PktAnnounce& pktAnn )							{};
    virtual void				onThreadOncePer15Minutes( void )							            {};
    virtual	void				onPluginSettingChange( PluginSetting& pluginSetting )                   {};

    //=== packet handlers ===//
    virtual void				onPktUserConnect			( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
    virtual void				onPktUserDisconnect			( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
    virtual void				onPktPluginOfferReq			( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent ) override;
    virtual void				onPktPluginOfferReply		( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent ) override;

    virtual void				onPktSessionStartReq		( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent ) override;
    virtual void				onPktSessionStartReply		( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent ) override;
    virtual void				onPktSessionStopReq			( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent ) override;
    virtual void				onPktSessionStopReply		( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent ) override;

    virtual void				onPktMyPicSendReq			( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent ) override;
    virtual void				onPktMyPicSendReply			( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent ) override;
    virtual void				onPktWebServerPicChunkTx	( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent ) override;
    virtual void				onPktWebServerPicChunkAck	( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent ) override;
    virtual void				onPktWebServerGetChunkTx	( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent ) override;
    virtual void				onPktWebServerGetChunkAck	( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent ) override;
    virtual void				onPktWebServerPutChunkTx	( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent ) override;
    virtual void				onPktWebServerPutChunkAck	( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent ) override;

    // error handling for invalid packet
    virtual void				onInvalidRxedPacket( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent, const char * msg = "" );

    //=== sessions ===//
    virtual void				onSessionStart( PluginSessionBase * poSession, bool pluginIsLocked );
    virtual void				onSessionEnded( PluginSessionBase * poSession, 
                                                bool pluginIsLocked,
                                                EOfferResponse eOfferResponse = eOfferResponseUserOffline ) {};

    virtual EPluginAccess	    canAcceptNewSession( VxNetIdent * netIdent );

	virtual P2PSession *		createP2PSession( VxSktBase * sktBase, VxNetIdent * netIdent );
	virtual P2PSession *		createP2PSession( VxGUID& lclSessionId, VxSktBase * sktBase, VxNetIdent * netIdent );
	virtual RxSession *			createRxSession( VxSktBase * sktBase, VxNetIdent * netIdent );
	virtual RxSession *			createRxSession( VxGUID& lclSessionId, VxSktBase * sktBase, VxNetIdent * netIdent );
	virtual TxSession *			createTxSession( VxSktBase * sktBase, VxNetIdent * netIdent );
	virtual TxSession *			createTxSession( VxGUID& lclSessionId, VxSktBase * sktBase, VxNetIdent * netIdent );

    //=== http ===//
	virtual void				handlePluginSpecificSkt( VxSktBase * sktBase ) {};
	virtual RCODE				handleHttpConnection( VxSktBase * sktBase, NetServiceHdr& netServiceHdr )		{ return -1; }
	virtual RCODE				handleHttpConnection( VxSktBase * sktBase, VxNetIdent * netIdent )				{ return -1; }

protected:
	virtual void				makeShortFileName( const char * pFullFileName, std::string& strShortFileName );
    virtual bool                generateSettingPkt( PluginSetting& pluginSetting );

	//=== vars ===//
	P2PEngine&					m_Engine;
	PluginMgr&					m_PluginMgr;
	
	VxNetIdent *				m_MyIdent = nullptr;

	EPluginType					m_ePluginType = ePluginTypeInvalid;
	EAppState					m_ePluginState = eAppStateInvalid;
	VxMutex						m_PluginMutex;
    PluginSetting               m_PluginSetting;
    PktPluginSettingReply       m_PktPluginSettingReply;

	bool						m_bPluginIsInSession = false;
	bool						m_ServerIsInSession = false;
	bool						m_AppIsPaused = false;
};
