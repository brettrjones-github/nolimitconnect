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

#include "IdentListMgrBase.h"

#include <CoreLib/VxGUID.h>

#include <vector>

// maintains a list of online users sorted by timestamp for faster lookup

class DirectConnectListMgr : public IdentListMgrBase
{
public:
    DirectConnectListMgr() = delete;
    DirectConnectListMgr( P2PEngine& engine );
    virtual ~DirectConnectListMgr() = default;

    bool                        isDirectConnect( VxGUID& onlineId );
    virtual void                updateDirectConnectIdent( VxGUID& onlineId, VxGUID& sktConnectId, int64_t timestamp );
    virtual void                onConnectionLost( VxGUID& sktConnectId );
    virtual void                removeIdent( VxGUID& onlineId ) override;
    virtual void                removeAll( void ) override;

    std::vector< std::pair<VxGUID, std::pair<VxGUID, int64_t>>>& getIdentList()         { return m_DirectConnectIdentList; };

protected:
    virtual void                removeConnection( VxGUID& onlineId, VxGUID& sktConnectId );

    std::vector< std::pair<VxGUID, std::pair<VxGUID, int64_t>>> m_DirectConnectIdentList;
};

