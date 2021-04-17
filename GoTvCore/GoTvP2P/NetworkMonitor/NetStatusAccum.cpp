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

#include "NetStatusAccum.h"

#include "../P2PEngine/P2PEngine.h"
#include <GuiInterface/IToGui.h>

#include <CoreLib/VxGlobals.h>
#include <NetLib/VxSktUtil.h>

//============================================================================
NetStatusAccum::NetStatusAccum( P2PEngine& engine )
    : m_Engine(engine)
{
}

//============================================================================
void NetStatusAccum::resetNetStatus( void )
{
    m_InternetAvail = false;
    m_NetworkHostAvail = false;
    m_ConnectionTestAvail = false;
    m_DirectConnectTested = false;
    m_RequriesRelay = false;
    m_ConnectedToRelay = false;
    m_GroupListHostAvail = false;
    m_GroupHostAvail = false;
    m_IsConnectedGroupHost = false;

    onNetStatusChange();
}

//============================================================================
void NetStatusAccum::onNetStatusChange( void )
{
    EInternetStatus internetStatus = m_InternetAvail ? eInternetNoInternet : eInternetInternetAvailable;
    if( FirewallSettings::eFirewallTestAssumeNoFirewall == m_FirewallTestType )
    {
        if( m_InternetAvail )
        {
            internetStatus = eInternetAssumeDirectConnect;

            std::string externIp;
            m_Engine.getEngineSettings().getExternalIp( externIp );
            setIpAddress( externIp );
        }
    }
    else if( FirewallSettings::eFirewallTestAssumeFirewalled == m_FirewallTestType )
    {
        if( m_InternetAvail )
        {
            internetStatus = eInternetRequiresRelay;
        }
    }
    else if( FirewallSettings::eFirewallTestUrlConnectionTest == m_FirewallTestType )
    {
        if( m_InternetAvail )
        {
            if( m_ConnectionTestAvail )
            {
                internetStatus = eInternetTestHostAvailable;
                if( m_DirectConnectTested )
                {
                    if( m_RequriesRelay )
                    {
                        internetStatus = eInternetRequiresRelay;
                    }
                    else
                    {
                        if( !m_NetworkHostAvail && m_Engine.getHasHostService( eHostServiceNetworkHost ) && m_WebsiteUrlsResolved && !m_IpAddr.empty() )
                        {
                            // normally if we are network host then we have a static ip and m_NetworkHostAvail is false
                            // if we tested the connection and can direct connect and our ip is not the tested ip
                            // then say host is available so the network bars in gui update
                            EngineParams& engineParams = m_Engine.getEngineParams();
                            std::string netHostAddr;
                            engineParams.getLastHostWebsiteResolvedIp( netHostAddr );
                            if( !netHostAddr.empty() && netHostAddr != m_IpAddr )
                            {
                                m_NetworkHostAvail = true;
                            }
                        }

                        internetStatus = eInternetCanDirectConnect;
                    }
                }
            }
            else
            {
                internetStatus = eInternetTestHostUnavailable;
            }
        }
    }

    if( m_InternetStatus != internetStatus )
    {
        m_AccumMutex.lock();
        m_InternetStatus = internetStatus;
        m_AccumMutex.unlock();

        m_AccumCallbackMutex.lock();
        for( auto callback : m_CallbackList )
        {
            callback->callbackInternetStatusChanged( internetStatus );
        }

        m_AccumCallbackMutex.unlock();

        LogModule( eLogNetAccessStatus, LOG_VERBOSE, "Internet Status %s", DescribeInternetStatus( internetStatus ) );
    }

    ENetAvailStatus netAvailStatus = eNetAvailNoInternet;
    if( m_NetworkHostAvail )
    {
        netAvailStatus = eNetAvailHostAvail;
        if( m_DirectConnectTested )
        {
            netAvailStatus = eNetAvailP2PAvail;
            if( requiresRelay() )
            {
                if( m_ConnectedToRelay )
                {
                    netAvailStatus = eNetAvailFullOnlineWithRelay;
                }
                else
                {
                    netAvailStatus = eNetAvailOnlineButNoRelay;
                }
            }
            else
            {
                netAvailStatus = eNetAvailFullOnlineDirectConnect;
            }

            if( eNetAvailFullOnlineDirectConnect == netAvailStatus )
            {
                // fully connected
                if( m_GroupHostAvail )
                {
                    netAvailStatus = eNetAvailDirectGroupHost;
                }
            }

            if( eNetAvailFullOnlineWithRelay == netAvailStatus )
            {
                // fully connected
                if( m_GroupHostAvail )
                {
                    netAvailStatus = eNetAvailRelayGroupHost;
                }
            }     
        }
    }

    if( m_NetAvailStatus != netAvailStatus )
    {
        m_AccumMutex.lock();
        m_NetAvailStatus = netAvailStatus;
        m_AccumMutex.unlock();

        m_AccumCallbackMutex.lock();
        for( auto callback : m_CallbackList )
        {
            callback->callbackNetAvailStatusChanged( netAvailStatus );
        }

        m_AccumCallbackMutex.unlock();

        LogModule( eLogNetAccessStatus, LOG_VERBOSE, "Net Avail Status %s", DescribeNetAvailStatus( netAvailStatus ) );
        m_Engine.getToGui().toGuiNetAvailableStatus( netAvailStatus );
    }   
}

//============================================================================
void NetStatusAccum::addNetStatusCallback( NetAvailStatusCallbackInterface* callbackInt )
{
    if( callbackInt )
    {
        ENetAvailStatus netAvailStatus = getNetAvailStatus();
        m_AccumCallbackMutex.lock();
        bool alreadyExist = false;
        for( auto callback : m_CallbackList )
        {
            if( callback == callbackInt )
            {
                alreadyExist = true;
                break;
            }
        }

        if( !alreadyExist )
        {
            m_CallbackList.push_back( callbackInt );
            callbackInt->callbackNetAvailStatusChanged( netAvailStatus );
        }

        m_AccumCallbackMutex.unlock();
    }
}

//============================================================================
void NetStatusAccum::removeNetStatusCallback( NetAvailStatusCallbackInterface* callbackInt )
{
    if( callbackInt )
    {
        m_AccumCallbackMutex.lock();
        for( auto iter = m_CallbackList.begin(); iter != m_CallbackList.begin(); ++iter )
        {
            if( *iter == callbackInt )
            {
                m_CallbackList.erase( iter );
                break;
            }
        }

        m_AccumCallbackMutex.unlock();
    }
}

//============================================================================
void NetStatusAccum::setInternetAvail( bool avail )
{
    if( avail != m_InternetAvail )
    {
        m_InternetAvail = avail;
        LogModule( eLogNetAccessStatus, LOG_VERBOSE, "Internet available %d", avail );
        onNetStatusChange();
    }
}

//============================================================================
void NetStatusAccum::setNetHostAvail( bool avail )
{
    if( avail != m_NetworkHostAvail )
    {
        m_NetworkHostAvail = avail;
        LogModule( eLogNetAccessStatus, LOG_VERBOSE, "Network Host available %d", avail );
        onNetStatusChange();
    }
}

//============================================================================
void NetStatusAccum::setConnectionTestAvail( bool avail )
{
    if( avail != m_ConnectionTestAvail )
    {
        m_ConnectionTestAvail = avail;
        LogModule( eLogNetAccessStatus, LOG_VERBOSE, "Connection Test available %d", avail );
        onNetStatusChange();
    }
}

//============================================================================
void NetStatusAccum::setDirectConnectTested( bool isTested, bool requiresRelay, std::string& myExternalIp )
{
    if( VxIsIpValid( myExternalIp ) )
    {
        m_Engine.lockAnnouncePktAccess();
        m_Engine.getMyPktAnnounce().setOnlineIpAddress( myExternalIp.c_str() );
        m_Engine.unlockAnnouncePktAccess();

        if( isTested != m_ConnectionTestAvail || isTested != m_DirectConnectTested || requiresRelay != m_RequriesRelay )
        {
            m_ConnectionTestAvail = isTested;
            m_DirectConnectTested = isTested;
            m_RequriesRelay = requiresRelay;
            if( isTested && !myExternalIp.empty() )
            {
                setIpAddress( myExternalIp );
            }

            LogModule( eLogNetAccessStatus, LOG_VERBOSE, "Direct Connect Tested %d relay required ? %d extern ip %s", isTested, requiresRelay, myExternalIp.c_str() );
            onNetStatusChange();
        }
    }
}

//============================================================================
void NetStatusAccum::setQueryHostOnlineId( bool noError, VxGUID& onlineId )
{
    if( noError )
    {

    }
}

//============================================================================
void NetStatusAccum::setConnectToRelay( bool connectedToRelay )
{
    if( connectedToRelay != m_ConnectedToRelay )
    {
        m_ConnectedToRelay = connectedToRelay;
        LogModule( eLogNetAccessStatus, LOG_VERBOSE, "Connected to relay %d", connectedToRelay );
        onNetStatusChange();
    }
}

//============================================================================
void NetStatusAccum::setIpAddress( std::string ipAddr )
{
    m_AccumMutex.lock();
    m_IpAddr = ipAddr;
    m_AccumMutex.unlock();
}

//============================================================================
void NetStatusAccum::setIpPort( uint16_t ipPort )
{
    m_AccumMutex.lock();
    m_IpPort = ipPort;
    m_AccumMutex.unlock();
}

//============================================================================
void NetStatusAccum::setFirewallTestType( FirewallSettings::EFirewallTestType firewallTestType )
{
    if( firewallTestType != m_FirewallTestType )
    {
        m_AccumMutex.lock();
        m_FirewallTestType = firewallTestType;
        m_AccumMutex.unlock();

        onNetStatusChange();
    }
}

//============================================================================
void NetStatusAccum::setWebsiteUrlsResolved( bool resolved )
{
    m_AccumMutex.lock();
    m_WebsiteUrlsResolved = resolved;
    m_AccumMutex.unlock();
}

//============================================================================
void NetStatusAccum::getNodeUrl( std::string& retNodeUrl )
{
    if( !isInternetAvailable() )
    {
        retNodeUrl = "No Internet Connection";
    }
    else
    {
        retNodeUrl = "ptop://";
        retNodeUrl += getIpAddress();
        retNodeUrl += ":";
        retNodeUrl += std::to_string( m_IpPort );
    }
}

//============================================================================
std::string NetStatusAccum::getIpAddress( void )
{
    std::string ipAddr;
    m_AccumMutex.lock();
    ipAddr = m_IpAddr;
    m_AccumMutex.unlock();
    return ipAddr;
}

//============================================================================
uint16_t NetStatusAccum::getIpPort( void )
{
    uint16_t port = 0;
    m_AccumMutex.lock();
    port = m_IpPort;
    m_AccumMutex.unlock();
    return port;
}
