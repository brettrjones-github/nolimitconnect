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
#pragma once

#include "ConnectedListBase.h"

class P2PEngine;
class BigListInfo;

class ConnectedListAll : public ConnectedListBase
{
public:
    ConnectedListAll() = delete;
    ConnectedListAll( P2PEngine& engine );
    virtual ~ConnectedListAll() override = default;

    ConnectedInfo*              getOrAddConnectedInfo( BigListInfo* bigListInfo );
    ConnectedInfo*              getConnectedInfo( const VxGUID& onlineId );

    void                        onSktDisconnected( VxSktBase* sktBase );

    //=== vars ===//
    P2PEngine&                  m_Engine;
};
