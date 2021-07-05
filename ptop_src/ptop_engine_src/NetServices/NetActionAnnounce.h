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

#include <string>

class VxSktBase;
class NetServicesMgr;
class NetServiceHdr;
class VxSktConnectSimple;
class HostList;

class NetActionAnnounce : public NetActionBase
{
public:
	NetActionAnnounce( NetServicesMgr& netServicesMgr, std::string& anchorIp, uint16_t u16HostPort, EHostAction eHostAction );
	virtual ~NetActionAnnounce() = default;

	virtual ENetActionType		getNetActionType( void )			{ return eNetActionAnnounce; }
	void						doAction( void );

protected:
	int							buildAnnounceCmd( std::string& strNetCmdHdr, uint16_t clientPort, HostList& anchorList, EHostAction anchorAction );
	int							getAnnounceData( VxSktConnectSimple * netServConn, char * pRxBuf, int bufLen );
	bool						decryptHostList( char * content, int contentDataLen, uint16_t clientPort );

	EHostAction				m_eHostAction;
};



