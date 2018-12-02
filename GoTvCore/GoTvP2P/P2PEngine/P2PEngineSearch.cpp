//============================================================================
// Copyright (C) 2013 Brett R. Jones 
// Issued to MIT style license by Brett R. Jones in 2017
//
// You may use, copy, modify, merge, publish, distribute, sub-license, and/or sell this software 
// provided this Copyright is not modified or removed and is included all copies or substantial portions of the Software
//
// This code is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// brett.jones@engineer.com
// http://www.gotvptop.net
//============================================================================
#include <GoTvCore/GoTvP2P/P2PEngine/P2PEngine.h>
#include <GoTvCore/GoTvP2P/Search/RcScan.h>
#include <GoTvCore/GoTvP2P/BigListLib/BigListInfo.h>
#include <GoTvCore/GoTvP2P/BigListLib/BigListMgr.h>
#include <PktLib/VxCommon.h>

//============================================================================
void P2PEngine::onPktSearchReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	VxNetIdent * netIdent = m_BigListMgr.findBigListInfo( pktHdr->getSrcOnlineId() );
	if( netIdent )
	{
		m_RcScan.onPktSearchReq( netIdent, sktBase, (PktSearchReq *)pktHdr );
		return;
	}
	else
	{
		LogMsg( LOG_ERROR, "P2PEngine::onPktSearchReq unknown ident 0x%llX 0x%llX\n", 
			pktHdr->getSrcOnlineId().getVxGUIDHiPart(),
			pktHdr->getSrcOnlineId().getVxGUIDLoPart() );
	}
}

//============================================================================
void P2PEngine::onPktSearchReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	VxNetIdent * netIdent = m_BigListMgr.findBigListInfo( pktHdr->getSrcOnlineId() );
	if( netIdent )
	{
		m_RcScan.onPktSearchReply( netIdent, sktBase, (PktSearchReply *)pktHdr );
		return;
	}
	else
	{
		LogMsg( LOG_ERROR, "P2PEngine::onPktSearchReply unknown ident 0x%llX 0x%llX\n", 
			pktHdr->getSrcOnlineId().getVxGUIDHiPart(),
			pktHdr->getSrcOnlineId().getVxGUIDLoPart() );
	}
}

//============================================================================
void P2PEngine::onPktFindFileReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	VxNetIdent * netIdent = m_BigListMgr.findBigListInfo( pktHdr->getSrcOnlineId() );
	if( netIdent )
	{
		m_RcScan.onPktFindFileReq( netIdent, sktBase, (PktFindFileReq *)pktHdr );
	}
	else
	{
		LogMsg( LOG_ERROR, "P2PEngine::onPktFindFileReq unknown ident 0x%llX 0x%llX\n", 
			pktHdr->getSrcOnlineId().getVxGUIDHiPart(),
			pktHdr->getSrcOnlineId().getVxGUIDLoPart() );
	}
}

//============================================================================
void P2PEngine::onPktFindFileReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	VxNetIdent * netIdent = m_BigListMgr.findBigListInfo( pktHdr->getSrcOnlineId() );
	if( netIdent )
	{
		//m_RcScan.onPktFindFileReply( netIdent, sktBase, (PktFindFileReply *)pktHdr );
	}
	else
	{
		LogMsg( LOG_ERROR, "P2PEngine::onPktFindFileReply unknown ident 0x%llX 0x%llX\n", 
			pktHdr->getSrcOnlineId().getVxGUIDHiPart(),
			pktHdr->getSrcOnlineId().getVxGUIDLoPart() );
	}
}

