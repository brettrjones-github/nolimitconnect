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
// brett.jones@engineer.com
// http://www.gotvptop.net
//============================================================================

#include "NetActionBase.h"
#include "NetServicesMgr.h"

#include <GoTvCore/GoTvP2P/P2PEngine/P2PEngine.h>

//============================================================================
NetActionBase::NetActionBase( NetServicesMgr& netServicesMgr )
: m_NetServicesMgr( netServicesMgr )
, m_NetServiceUtils( netServicesMgr.getNetUtils() )
, m_Engine( netServicesMgr.getEngine() )
{
}

//============================================================================
NetActionBase::~NetActionBase()
{
}

//============================================================================
VxGUID& NetActionBase::getMyOnlineId( void )
{
	return m_Engine.getMyPktAnnounce().getMyOnlineId();
}
