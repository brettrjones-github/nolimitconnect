#pragma once
//============================================================================
// Copyright (C) 2021 Brett R. Jones 
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

#include "BaseInfo.h"

#include <GuiInterface/IDefs.h>

#include <CoreLib/VxUrl.h>

class BaseHostInfo : public BaseInfo
{
public:
    BaseHostInfo();
    BaseHostInfo( const BaseHostInfo& rhs );
    virtual ~BaseHostInfo() = default;

    BaseHostInfo&				operator=( const BaseHostInfo& rhs ); 

    virtual void				setHostType( EHostType hostType )               { m_HostType = hostType; }
    virtual EHostType			getHostType( void )                             { return m_HostType; }
    virtual void				setConnectUrl( VxUrl& connectUrl )              { m_ConnectUrl = connectUrl; }
    virtual VxUrl&			    getConnectUrl( void )                           { return m_ConnectUrl; }

    bool                        isHostMatch( EHostType hostType, VxGUID& onlineId );

public:
	//=== vars ===//
    EHostType                   m_HostType{ eHostTypeUnknown };
    VxUrl                       m_ConnectUrl;
};
