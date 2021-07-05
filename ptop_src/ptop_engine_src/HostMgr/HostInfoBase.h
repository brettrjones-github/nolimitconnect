//============================================================================
// Copyright (C) 2020 Brett R. Jones 
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
#pragma once

#include "HostDefs.h"
#include <GuiInterface/IDefs.h>
#include <ptop_src/ptop_engine_src/HostMgr/HostConnectInterface.h>

#include <CoreLib/VxGUID.h>
#include <CoreLib/VxMutex.h>

#include <vector>
#include <string>
#include <map>

class VxSktConnectSimple;
class P2PEngine;
class PktAnnounce;

class HostInfoBase
{
public:
    HostInfoBase( P2PEngine& engine );
    virtual ~HostInfoBase() = default;

    HostInfoBase( const HostInfoBase& rhs );
    HostInfoBase( P2PEngine& engine, EOtherHostType otherHostType, std::string& hostIp, uint16_t hostPort, const char * hostUrl );
    HostInfoBase( P2PEngine& engine, EOtherHostType otherHostType, VxGUID onlineId, std::string& hostIp, uint16_t hostPort, const char * hostUrl );
    HostInfoBase( P2PEngine& engine, EPluginType ePluginType, VxGUID onlineId, std::string& hostIp, uint16_t hostPort, const char * hostUrl );

    HostInfoBase&				operator=( const HostInfoBase& rhs );
    bool                        operator==( const HostInfoBase& rhs );

    P2PEngine&                  getEngine() { return m_Engine; }

    virtual std::string         describeHostInfo( void );
    HostInfoBase&               getHostInfoBase( void ) { return *this; }

    void                        setOtherHostType( EOtherHostType hostType );
    EOtherHostType              getOtherHostType( void )                        { return m_HostType; }

    void                        setHostOnlineId( VxGUID& onlineId )     { m_OnlineId = onlineId; }
    VxGUID&                     getHostOnlineId( void )                 { return m_OnlineId; }
    void                        setHostIpAddr( std::string& ipAddr )    { m_HostIpAddr = ipAddr; }
    std::string&                getHostIpAddr( void )                   { return m_HostIpAddr; }
    void                        setHostPort( uint16_t port )            { m_HostPort = port; }
    uint16_t                    getHostPort( void )                     { return m_HostPort; }
    void                        setHostUrl( const char * url )          { m_HostUrl = url ? url : ""; }
    std::string&                getHostUrl( void )                      { return m_HostUrl; }

    std::vector<EPluginType>&   getPluginList( void )                   { return m_PluginList; }
    std::vector<VxSktBase*>&    getSktList( void )                      { return m_SktList; }
    int                         getSktListCount( void )                 { return (int)m_SktList.size(); }

    void                        addPluginService( EPluginType pluginType );
    void                        addPluginService( HostInfoBase& rhs );
    void                        removePluginService( EPluginType pluginType );
    bool                        hasPluginService( EPluginType pluginType );

    /// return true if matches host type and either online id or ip and port
    virtual bool                isMatch( HostInfoBase& rhs );

    /// return true if requires action like qurey host id
    virtual bool                requiresAction( void ) { return false; };

    virtual bool                needConnection( void ) { return m_CallbackList.size(); }

    /// query for host online id
    virtual bool                doActionQueryHostId( VxGUID& retHostId );

    bool                        connectSimpleSkt( VxSktConnectSimple& sktSimple );

    bool                        addConnectionRequest( EHostConnectType connectType, IHostConnectCallback* connectReq );
    bool                        removeConnectionRequest( EHostConnectType connectType, IHostConnectCallback* connectReq );

    /// return false if no current requests for given connection
    bool                        isSktInUse( VxSktBase* sktBase );

    void                        updateActionTakenTime( void );
    bool                        isActionTakenTimeExpired( void );

    void                        updateConnectAttemptTime( void );
    bool                        isConnectAttemptTimeExpired( void );

    void                        sendConnectionToCallback( EHostConnectType connectType, IHostConnectCallback* connectReq );

    virtual void                onSktConnectedWithPktAnn( VxSktBase* sktBase );
    virtual void                onSktDisconnected( VxSktBase* sktBase );

    virtual void                addToSktListIfSktNotInUse( std::vector<VxSktBase *>& sktList );
    virtual bool                hasHostMatch( PktAnnounce& peerPktAnn );

    static EOtherHostType       connectTypeToHostType( EHostConnectType connectType );
    static void                 addToSktListIfMissing( std::vector<VxSktBase *>& sktList, VxSktBase* sktBase );

protected:
    P2PEngine&                  m_Engine;
    EOtherHostType              m_HostType{ eOtherHostUnknown };
    std::vector<EPluginType>    m_PluginList;
    std::string                 m_HostUrl{ "" };
    std::string                 m_HostIpAddr{ "" };
    uint16_t                    m_HostPort{ 0 };
    VxGUID                      m_OnlineId;
    VxMutex                     m_CallbackListMutex;
    std::map<EHostConnectType, std::vector<IHostConnectCallback*>> m_CallbackList;
    uint64_t                    m_ActionTimeMs{ 0 };
    uint64_t                    m_ConnectAttemptTimeMs{ 0 };

    VxMutex                     m_SktListMutex;
    std::vector<VxSktBase*>     m_SktList;
};



