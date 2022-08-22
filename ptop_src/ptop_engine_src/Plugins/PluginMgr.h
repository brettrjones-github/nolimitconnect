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

#include <GuiInterface/IFromGui.h>
#include <ptop_src/ptop_engine_src/NetServices/NetServiceUtils.h>
#include <ptop_src/ptop_engine_src/Network/ConnectRequest.h>

#include <CoreLib/VxMutex.h>

#include <string>
#include <vector>

class PluginBase;
class IToGui;
class P2PEngine;
class MediaProcesser;
class PktAnnounce;
class PluginSetting;
class BigListMgr;
class VxPktHdr;

class PluginMgr
{
public:
	class AutoMgrLock
	{
	public:
		AutoMgrLock( PluginMgr * pluginMgr ) : m_Mutex(pluginMgr->getPluginMgrMutex())	{ m_Mutex.lock(); }
		~AutoMgrLock()																	{ m_Mutex.unlock(); }
		VxMutex&				m_Mutex;
	};

	PluginMgr( P2PEngine& engine );
	virtual ~PluginMgr();

	void						pluginMgrStartup( void );
	void						pluginMgrShutdown( void );

    IToGui&						getToGui( void );
	P2PEngine&					getEngine( void )									{ return m_Engine; }

	PluginBase *				getPlugin( EPluginType ePluginType );
	EAppState					getPluginState( EPluginType ePluginType );
	void						setPluginState( EPluginType ePluginType, EAppState ePluginState );
	VxMutex&					getPluginMgrMutex( void )							{ return m_PluginMgrMutex; }
	PktAnnounce&				getPktAnnounce( void )								{ return m_PktAnn; }

	EFriendState				getPluginPermission( EPluginType ePluginType );
	void						setPluginPermission( EPluginType ePluginType, EFriendState ePluginPermission );

    bool                        setPluginSetting( PluginSetting& pluginSetting, int64_t modifiedTimeMs = 0 );
    void                        onPluginSettingChange( PluginSetting& pluginSetting, int64_t modifiedTimeMs = 0 );

	virtual void				onMyOnlineUrlIsValid( bool isValidUrl );

	virtual void				fromGuiNetworkAvailable( void );

	virtual void				fromGuiUserLoggedOn( void );

	virtual void				fromGuiStartPluginSession( EPluginType ePluginType, VxGUID& oOnlineId, int pvUserData = 0, VxGUID lclSessionId = VxGUID::nullVxGUID() );
	virtual void				fromGuiStopPluginSession( EPluginType ePluginType, VxGUID& oOnlineId, int pvUserData = 0, VxGUID lclSessionId = VxGUID::nullVxGUID() );
	virtual bool				fromGuiIsPluginInSession( EPluginType ePluginType, VxNetIdent * netIdent = NULL, int pvUserData = 0, VxGUID lclSessionId = VxGUID::nullVxGUID() );

	virtual bool				fromGuiSetGameValueVar( EPluginType	    ePluginType, 
														VxGUID&		    oOnlineId,
														int32_t			s32VarId, 
														int32_t			s32VarValue );
	virtual bool				fromGuiSetGameActionVar(	EPluginType	    ePluginType, 
															VxGUID&		    oOnlineId,
															int32_t			s32VarId, 
															int32_t			s32VarValue );

	virtual int					fromGuiDeleteFile( const char * fileName, bool shredFile );

    virtual bool                pluginApiTxPacket(  EPluginType			ePluginType,
													const VxGUID&		onlineId,
                                                    VxSktBase *			sktBase,
                                                    VxPktHdr *			poPkt,
                                                    bool				bDisconnectAfterSend = false,
                                                    EPluginType         overridePlugin = ePluginTypeInvalid );
	void						pluginApiLog( EPluginType ePluginType, const char * pMsg, ... );
	virtual EPluginAccess	    pluginApiGetPluginAccessState( EPluginType ePluginType, VxNetIdent * netIdent );
	virtual VxNetIdent *		pluginApiGetMyIdentity( void );
	virtual VxNetIdent *		pluginApiFindUser( const char * pUserName );

	virtual void				pluginApiPlayVideoFrame( EPluginType ePluginType, uint8_t * pu8VidData, uint32_t u32VidDataLen, VxNetIdent * netIdent, int iRotation );

	virtual void				pluginApiWantAppIdle( EPluginType ePluginType, bool bWantAppIdle );

	virtual void				pluginApiWantMediaInput( EPluginType ePluginType, EMediaInputType mediaType, EAppModule appModule, bool wantInput, void * userData = 0 );

	virtual bool				pluginApiSktConnectTo(	EPluginType			ePluginType,		// plugin id
														VxNetIdentBase *	netIdent,			// identity of contact to connect to
														int					pvUserData,			// plugin defined data
														VxSktBase * *		ppoRetSkt, 			// returned Socket
														EConnectReason		connectReason = eConnectReasonPlugin );	
	virtual void				pluginApiSktClose( ESktCloseReason closeReason, VxSktBase * sktBase );
	virtual void				pluginApiSktCloseNow( ESktCloseReason closeReason, VxSktBase * sktBase );
	void						pluginApiToGuiSessionEnded(	EPluginType		ePluginType,		// plugin
															VxNetIdent *	netIdent,			// identity of friend
															int				pvUserData,			// plugin defined data
															EOfferResponse	eOfferResponse );	// reason session ended

	VxNetIdent *				pluginApiOnlineIdToIdentity( VxGUID& oOnlineId );

	virtual void				onAppStateChange( EAppState eAppState );
	virtual void				onAppStartup( void );
	virtual void				onAppShutdown( void );

	virtual void				fromGuiAppPause( void );
	virtual void				fromGuiAppResume( void );
	virtual void				fromGuiListAction( EListAction listAction );

	virtual void				onOncePerSecond( void );
    virtual void				onThreadOncePer15Minutes( void );
	virtual void				onAfterUserLogOnThreaded( void );

	virtual void				onContactWentOnline( VxNetIdent * netIdent, VxSktBase * sktBase );
	virtual void				onContactWentOffline( VxNetIdent * netIdent, VxSktBase * sktBase );
	virtual void				onConnectionLost( VxSktBase * sktBase );	
	virtual void				fromGuiRelayPermissionCount( int userPermittedCount, int anonymousCount ); 
	virtual void				fromGuiSendAsset( AssetBaseInfo& assetInfo );
	virtual bool				fromGuiMultiSessionAction( EMSessionAction mSessionAction, VxGUID& onlineId, int pos0to100000, VxGUID lclSessionId = VxGUID::nullVxGUID() );

	virtual EPluginAccess		canAcceptNewSession( EPluginType ePluginType, VxNetIdent * netIdent );

	//! return true if access ok
	bool						canAccessPlugin( EPluginType ePluginType, VxNetIdent * netIdent );

	void						replaceConnection( VxNetIdent * netIdent, VxSktBase * poOldSkt, VxSktBase * poNewSkt );

	void						handleNonSystemPackets( VxSktBase * sktBase, VxPktHdr * pktHdr );
	void						handleFirstNetServiceConnection( VxSktBase * sktBase );
	bool						handleFirstWebPageConnection( VxSktBase * sktBase );

	PluginBase*				    findPlugin( EPluginType ePluginType );
    PluginBase*				    findHostClientPlugin( EHostType hostType );
    PluginBase*				    findHostServicePlugin( EHostType hostType );

	void						leavePreviousHost( GroupieId& groupieId );

protected:
	bool						isValidPluginNum( uint8_t u8PluginNum );
    PluginBase*                 hostClientToPlugin( EHostType hostType );
    PluginBase*                 hostServiceToPlugin( EHostType hostType );

	//=== vars ===//
	P2PEngine&					m_Engine;
	BigListMgr&					m_BigListMgr;
	PktAnnounce&				m_PktAnn;
	
	VxMutex						m_PluginMgrMutex;

	bool						m_PluginMgrInitialized;

	std::vector<PluginBase *>	m_aoPlugins;
	NetServiceUtils				m_NetServiceUtils;
};
