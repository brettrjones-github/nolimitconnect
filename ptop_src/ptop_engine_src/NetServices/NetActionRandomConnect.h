#pragma once
//============================================================================
// Copyright (C) 2014 Brett R. Jones
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

#include "NetActionBase.h"
#include "NetServiceDefs.h"

#include <ptop_src/ptop_engine_src/HostMgr/HostDefs.h>
#include <GuiInterface/IToGui.h>

#include <string>

class VxSktBase;
class NetServicesMgr;
class NetServiceHdr;
class VxSktConnectSimple;
class HostList;

class NetActionRandomConnect : public NetActionBase
{
public:
	NetActionRandomConnect( NetServicesMgr& netServicesMgr, std::string& anchorIp, uint16_t u16HostPort, EHostAction eHostAction );
	virtual ~NetActionRandomConnect();

	virtual ENetActionType		getNetActionType( void )			{ return eNetActionRandomConnect; }
	void						doAction( void );

protected:
	bool						doRandomConnectAction( VxSktConnectSimple& netServConn, int tryCnt );
	int							buildAnnounceCmd( std::string& strNetCmdHdr, uint16_t clientPort, HostList& anchorList, EHostAction anchorAction );
	int							getAnnounceData( VxSktConnectSimple * netServConn, char * pRxBuf, int bufLen );
	bool						decryptHostList( char * content, int contentDataLen, uint16_t clientPort );
	void						sendRandomConnectStatus( ERandomConnectStatus eStatus, const char * msg, ... );

	EHostAction				m_eHostAction;
	int							m_FoundCnt;
};



