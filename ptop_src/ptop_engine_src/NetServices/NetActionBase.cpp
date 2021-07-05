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
#include "NetServicesMgr.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>

//============================================================================
NetActionBase::NetActionBase( NetServicesMgr& netServicesMgr )
: m_NetServicesMgr( netServicesMgr )
, m_NetServiceUtils( netServicesMgr.getNetUtils() )
, m_Engine( netServicesMgr.getEngine() )
{
}

//============================================================================
std::string NetActionBase::getNetworkKey( void )
{
    return m_NetServicesMgr.getNetworkKey();
}

//============================================================================
VxGUID& NetActionBase::getMyOnlineId( void )
{
	return m_Engine.getMyPktAnnounce().getMyOnlineId();
}
