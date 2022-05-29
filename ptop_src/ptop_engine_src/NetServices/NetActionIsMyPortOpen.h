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
// http://www.nolimitconnect.org
//============================================================================

#include "NetActionBase.h"

#include <string>

class NetServicesMgr;
class VxSktConnectSimple;

class NetActionIsMyPortOpen : public NetActionBase
{
public:
	NetActionIsMyPortOpen( NetServicesMgr& netServicesMgr );
	virtual ~NetActionIsMyPortOpen() = default;

	virtual ENetActionType		getNetActionType( void )			{ return eNetActionIsPortOpen; }
	virtual void				doAction( void );

protected:
	void						buildIsMyPortOpenUrl( VxSktConnectSimple * netServConn, std::string& strHttpUrl );
};

