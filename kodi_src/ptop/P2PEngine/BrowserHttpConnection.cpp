//============================================================================
// Copyright (C) 2011 Brett R. Jones 
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

#if 0 //BRJ Need to implement for browser profile and storyboard

#include "RcHttpConnection.h"

#include <RcPluginLib/RcPluginBaseWebServer.h>
#include <GoTvCore/GoTvP2P/P2PEngine/P2PEngine.h>
#include <GoTvCore/GoTvP2P/BigListLib/BigListInfo.h>
#include <RcPluginLib/RcPluginBase.h>
#include <RcPluginLib/RcPluginBaseWebServer.h>

RcHttpConnection g_oHttpConnection;

//============================================================================
RcHttpConnection::RcHttpConnection()
	: m_Engine(NULL)
{
}

//============================================================================
RcHttpConnection::~RcHttpConnection()
{

}

//============================================================================
void RcHttpConnection::httpConnectionStartup( P2PEngine * poEngine )
{
 	m_Engine = poEngine;
}

//============================================================================
void RcHttpConnection::httpConnectionShutdown( void )
{

}

//============================================================================
void RcHttpConnection::handleHttpConnection( RcSktBase * sktBase )
{
	LogMsg( LOG_INFO, "RcPluginMgr::HandleHttpConnection\n" );
	bool bSktAccepted = false;
	VxGUID oOnlineId;
	if( true == parseOnlineId( sktBase, &oOnlineId ) )
	{
		std::string strOnlineIdDesc = oOnlineId.describeVxGUID();
		VxNetIdent * netIdent = NULL;
		if( oOnlineId == m_Engine->getMyPktAnn()->getMyOnlineId() )
		{
			netIdent = m_Engine->getMyPktAnn();
			LogMsg( LOG_INFO, "RcPluginMgr::HandleHttpConnection; parseOnlineId was myself\n" );
		}
		else
		{
			netIdent = m_Engine->findBigListInfo( oOnlineId );
		}
		if( netIdent )
		{
			LogMsg( LOG_INFO, "RcPluginMgr::HandleHttpConnection; parseOnlineId was %s\n", netIdent->m_as8OnlineName );

			RcPluginBase * poPlugin = m_Engine->getPlugin(ePluginTypeWebServer);
			if( poPlugin )
			{
				poPlugin->handleHttpConnection( sktBase, netIdent );
			}
		}
		else
		{
			std::string strMyOnlineId = m_Engine->getMyPktAnn()->getMyOnlineId().describeVxGUID();
			LogMsg( LOG_ERROR, "RcPluginMgr::HandleHttpConnection: ERROR failed find info for %s my id %s\n", strOnlineIdDesc.c_str(), strMyOnlineId.c_str() );
		}
	}
	else
	{
		LogMsg( LOG_ERROR, "RcPluginMgr::HandleHttpConnection: ERROR parsing id\n" );
	}
	if( bSktAccepted )
	{
		// this socket is no longer needed .. in handleHttpConnection it was moved to new RcWebSkt
		sktBase->CloseSkt( 642 );
	}
}

//============================================================================
bool RcHttpConnection::parseOnlineId( RcSktBase * sktBase, VxGUID * poOnlineId )
{
	bool bParseOk = false;
	int iDataLen = sktBase->GetSktBufDataLen();
	if( iDataLen > 32 )
	{
		char *	pSktBuf = (char *)sktBase->GetSktReadBuf();
		if( 0 == strncmp( pSktBuf, "GET /", 5 ) )
		{
			char * pIdBegin = pSktBuf + 5;
			char * pProfileFileName = strstr( pSktBuf, "/index.htm" );
			if( NULL == pProfileFileName )
			{
				pProfileFileName = strstr( pSktBuf, "/StoryBoard.htm" );
			}
			if( pProfileFileName )
			{
				if( 32 == ( pProfileFileName - pIdBegin ) )
				{
					bool bIsAllHex = true;
					for( int i = 0; i < 32; i++ )
					{
						char ch = pIdBegin[ i ];
						if( false == ( (('0' <= ch) && ('9' >= ch)) || (('a' <= ch) && ('f' >= ch)) ) )
						{
							bIsAllHex = false;
							break;
						}
					}
					if( bIsAllHex )
					{
						poOnlineId->fromOnlineIdHexString( pIdBegin );
						if( ( 0 != poOnlineId->m_u64IdHiPart ) &&
							( 0 != poOnlineId->m_u64IdLoPart ) )
						{
							bParseOk = true;
						}
					}
				}
			}
		}
		sktBase->SktBufAmountRead( 0 );
	}
	return bParseOk;
}

#endif //0 //BRJ Need to implement for browser profile and storyboard
