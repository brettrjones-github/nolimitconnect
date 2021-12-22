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

#include <GuiInterface/IDefs.h>

#include <string>

class P2PEngine;
class NetServicesMgr;
class NetServiceUtils;
class VxGUID;

class NetActionBase
{
public:
	NetActionBase( NetServicesMgr& netServicesMgr );
    virtual ~NetActionBase() = default;

    virtual std::string         getNetworkKey( void );

	virtual ENetActionType		getNetActionType( void )			{ return eNetActionUnknown; }
	virtual VxGUID&			    getMyOnlineId( void );

	virtual void				enterAction( void )					{};
	virtual void				doAction( void )					{};
	virtual void				exitAction( void )					{};

protected:
	NetServicesMgr&				m_NetServicesMgr;
	NetServiceUtils&			m_NetServiceUtils;
	P2PEngine&					m_Engine;
};

