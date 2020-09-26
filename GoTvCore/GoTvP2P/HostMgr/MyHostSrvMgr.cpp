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

#include "MyHostSrvMgr.h"
#include <GoTvCore/GoTvP2P/P2PEngine/P2PEngine.h>

//============================================================================
MyHostSrvMgr::MyHostSrvMgr( P2PEngine& engine )
    : m_Engine( engine )
{

}

//============================================================================
void MyHostSrvMgr::addHostInfo( EPluginType ePluginType, VxGUID onlineId, std::string& hostIp, uint16_t hostPort, const char * hostUrl )
{

}


//============================================================================
void MyHostSrvMgr::requestHostConnection( EHostConnectType connectType, IHostConnectCallback* callback )
{
    bool alreadyConnected = false;

    if( callback )
    {
        m_CallbackMutex.lock();
        bool alreadyExists = false;
        callback->addHostConnectType( connectType );
        // make sure not a duplicate
        for( auto cb : m_ConnectionCallbacks )
        {
            if( cb == callback )
            {
                alreadyExists = true;
                break;
            }
        }

        if( !alreadyExists )
        {
            m_ConnectionCallbacks.push_back( callback );
        }

        // see if we already have a connection
        for( auto hostInfo : m_HostConnectList )
        {
            if( hostInfo.hasHostService( connectType ) && hostInfo.isConnected( connectType ))
            {
                m_Engine.getConnectList().connectListLock();
                alreadyConnected = true;
                callback->onContactConnected( connectType, hostInfo.getConnectInfo(), true );
                m_Engine.getConnectList().connectListUnlock();
                break;
            }
        }

        m_CallbackMutex.unlock();
    }

    if( !alreadyConnected )
    {
        // do the connection request
    }

}

//============================================================================
void MyHostSrvMgr::onEngineContactConnected( RcConnectInfo * poInfo, bool connectionListLocked )
{

}

//============================================================================
void MyHostSrvMgr::onEngineContactDisconnected( RcConnectInfo * poInfo, bool connectionListLocked )
{

}
