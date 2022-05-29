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
// http://www.nolimitconnect.org
//============================================================================

#include <GuiInterface/IDefs.h>
#include <CoreLib/VxMutex.h>

class VxGUID;
class P2PEngine;
class MembershipMgrBase
{
public:
    MembershipMgrBase( P2PEngine& engine );
    virtual ~MembershipMgrBase() = default;

    void                        lockList( void ) { m_ListMutex.lock(); }
    void                        unlockList( void ) { m_ListMutex.unlock(); }

    void						lockClientList( void ) { m_ClientMutex.lock(); }
    void						unlockClientList( void ) { m_ClientMutex.unlock(); }

    virtual void                removeConnection( VxGUID& sktConnectId ) = 0;
    virtual void                removeIdent( VxGUID& onlineId ) = 0;
    virtual void                removeAll( void ) = 0;

protected:
    virtual void                onUpdateIdent( VxGUID& onlineId, int64_t timestamp );
    virtual void                onRemoveIdent( VxGUID& onlineId );

    P2PEngine&                  m_Engine;
    VxMutex                     m_ListMutex;
    VxMutex                     m_ClientMutex;
};

