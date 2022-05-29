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

#include "PluginBaseWebServer.h"
#include "PluginMgr.h"

#include <PktLib/PktsWebServer.h>
#include <PktLib/PktsScan.h>
#include <NetLib/VxSktBase.h>
#include <CoreLib/VxGlobals.h>

//============================================================================
PluginBaseWebServer::PluginBaseWebServer( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent * myIdent, EPluginType pluginType )
: PluginBase( engine, pluginMgr, myIdent, pluginType )
, m_File(NULL)
{
	m_ePluginType = ePluginTypeWebServer;
	setPluginBaseWebServer(this);
	setWebServerEnable( true );
}

//============================================================================
PluginBaseWebServer::~PluginBaseWebServer()
{
}

//============================================================================
void PluginBaseWebServer::pluginShutdown( void )
{
	AutoPluginLock( this );
	WebRxIter oMapRxIter;
	for( oMapRxIter = m_WebRxSessions.begin(); oMapRxIter != m_WebRxSessions.end(); ++oMapRxIter )
	{
		 WebRxSession * poRxSession = oMapRxIter->second;
		 //poRxSession->getSkt()->closeSkt( 78298 );
		 delete poRxSession;
	}

	WebTxIter oMapTxIter;
	for( oMapTxIter = m_WebTxSessions.begin(); oMapTxIter != m_WebTxSessions.end(); ++oMapTxIter )
	{
		WebTxSession * poTxSession = oMapTxIter->second;
		//poTxSession->getSkt()->closeSkt( 78299 );
		delete poTxSession;
	}

	WebProxyIter oMapProxyIter;
	for( oMapProxyIter = m_WebProxySessions.begin(); oMapProxyIter != m_WebProxySessions.end(); ++oMapProxyIter )
	{
		WebProxySession * poProxySession = oMapProxyIter->second;
		//poProxySession->getSkt()->closeSkt( 78300 );
		delete poProxySession;
	}

	PluginBase::pluginShutdown();
}

//============================================================================
//! this is called if incoming http connection (NOTE: only used by WebServer)
RCODE PluginBaseWebServer::handlePtopConnection( VxSktBase * sktBase, VxNetIdent * netIdent )
{
	RCODE rc = 0;
	RcWebSkt * poWebSkt = NULL;
	static std::string strOffline = "User Offline \n\n";
	if( netIdent != m_MyIdent )
	{
		// see if can connect to user
		VxSktBase * poUserSkt =  NULL;
		if( true == m_PluginMgr.pluginApiSktConnectTo( m_ePluginType, netIdent, 0, &poUserSkt ))
		{
			rc = acceptRawLocalHttpConnection( sktBase, netIdent, &poWebSkt );
			if( 0 == rc )
			{
				poWebSkt->setUserExtraData( poUserSkt );
				WebProxySession * poSession = new WebProxySession( netIdent, sktBase, poWebSkt );

				m_PluginMutex.lock();
				m_WebProxySessions.insert( std::make_pair( netIdent, poSession ) );
				m_PluginMutex.unlock();
				
				startWebSktThreads( poWebSkt );
			}
		}
		else
		{
			// failed to connect.. just send Error
			sktBase->sendData(strOffline.c_str(), ( int )strOffline.length(), true);
		}
	}
	else
	{
		// view my own profile
		rc = acceptRawLocalHttpConnection( sktBase, netIdent, &poWebSkt );
		if( 0 == rc )
		{
			startWebSktThreads( poWebSkt );
		}
	}

	return rc;
}

//============================================================================
void PluginBaseWebServer::fromGuiUserLoggedOn( void )
{
    std::string strProfileDir = VxGetStoryBoardPageServerDirectory();
	setWebSiteDir( strProfileDir.c_str() );
}

//============================================================================
void PluginBaseWebServer::getMyPicFileName( std::string& strRetFileName )
{
	strRetFileName = getWebSiteDir();
	strRetFileName += "me.png";
}

//============================================================================
WebRxSession * PluginBaseWebServer::findRxSession( VxNetIdent * netIdent )
{
	WebRxIter oMapIter = m_WebRxSessions.find( netIdent );
	if( oMapIter != m_WebRxSessions.end() )
	{
		return oMapIter->second;
	}
	return NULL;
}

//============================================================================
WebRxSession * PluginBaseWebServer::findOrCreateRxSession( VxNetIdent * netIdent, VxSktBase * sktBase )
{
	WebRxSession * poSession = findRxSession( netIdent );
	if( NULL == poSession )
	{
		poSession = new WebRxSession( netIdent, sktBase );

		AutoPluginLock(this);
		m_WebRxSessions.insert( std::make_pair( netIdent, poSession ) );
	}
	else
	{
		poSession->setSkt( sktBase );
	}
	return poSession;
}

//============================================================================
WebTxSession * PluginBaseWebServer::findTxSession( VxNetIdent * netIdent )
{
	WebTxIter oMapIter = m_WebTxSessions.find( netIdent );
	if( oMapIter != m_WebTxSessions.end() )
	{
		return oMapIter->second;
	}
	return NULL;
}

//============================================================================
WebTxSession * PluginBaseWebServer::findOrCreateTxSession( VxNetIdent * netIdent, VxSktBase * sktBase )
{
	WebTxSession * poSession = findTxSession( netIdent );
	if( NULL == poSession )
	{
		poSession = new WebTxSession( netIdent, sktBase );

		AutoPluginLock(this);
		m_WebTxSessions.insert( std::make_pair( netIdent, poSession ) );
	}
	else
	{
		poSession->setSkt( sktBase );
	}
	return poSession;
}

//============================================================================
WebProxySession * PluginBaseWebServer::findWebProxySession( VxNetIdent * netIdent )
{
	WebProxyIter oMapIter = m_WebProxySessions.find( netIdent );
	if( oMapIter != m_WebProxySessions.end() )
	{
		return oMapIter->second;
	}
	return NULL;
}

//============================================================================
bool PluginBaseWebServer::searchActionProfilePic( VxNetIdent * netIdent, VxSktBase * sktBase )
{
	PktMyPicSendReq oPkt;
	return txPacket( netIdent, sktBase, &oPkt );
}
