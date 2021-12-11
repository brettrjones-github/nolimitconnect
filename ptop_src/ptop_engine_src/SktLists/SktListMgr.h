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

#include "SktListMgrBase.h"

#include <CoreLib/VxGUID.h>

#include <vector>


class SktListMgr : public SktListMgrBase
{
public:
    SktListMgr() = delete;
    SktListMgr( P2PEngine& engine );
    virtual ~SktListMgr() = default;

    bool                        isSkt( VxGUID& onlineId );
    virtual void                updateSktIdent( VxGUID& sktConnectId, VxGUID& onlineId, int64_t timestamp );
    virtual void                removeConnection( VxGUID& sktConnectId );
    virtual void                removeIdent( VxGUID& onlineId ) override;
    virtual void                removeAll( void ) override;

    std::vector< std::pair<VxGUID, std::pair<VxGUID, int64_t>>>& getIdentList()         { return m_SktIdentList; };

protected:
    virtual void                removeConnection( VxGUID& onlineId, VxGUID& sktConnectId );

    std::vector< std::pair<VxGUID, std::pair<VxGUID, int64_t>>> m_SktIdentList;
};

