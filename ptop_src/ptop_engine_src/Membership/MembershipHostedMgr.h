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

#include "Membership.h"
#include "MembershipMgrBase.h"

#include <vector>

class MembershipHostedCallbackInterface;
class MembershipHostedMgr : public MembershipMgrBase
{
public:
    MembershipHostedMgr() = delete;
    MembershipHostedMgr( P2PEngine& engine );
    virtual ~MembershipHostedMgr() = default;

    void                        addMemberClient( MembershipHostedCallbackInterface* client, bool enable );

    virtual void                updateMemberHosted( VxGUID& sktConnectId, VxGUID& onlineId, MembershipHosted& memberAvail );

    virtual void                removeConnection( VxGUID& sktConnectId ) override;
    virtual void                removeIdent( VxGUID& onlineId ) override;
    virtual void                removeAll( void ) override;

    std::vector< std::pair<VxGUID, std::pair<VxGUID, MembershipHosted>>>& getMemberList()         { return m_MemberList; };

protected:
    virtual void                removeConnection( VxGUID& onlineId, VxGUID& sktConnectId );

    virtual void                onMembershipAdded( VxGUID& onlineId, VxGUID& sktConnectId, MembershipHosted& memberHosted );
    virtual void                onMembershipUpdated( VxGUID& onlineId, VxGUID& sktConnectId, MembershipHosted& memberHosted );
    virtual void                onMembershipRemoved( VxGUID& onlineId );

    virtual void                announceMembershipAdded( VxGUID& onlineId, VxGUID& sktConnectId, MembershipHosted& memberAvail );
    virtual void                announceMembershipUpdated( VxGUID& onlineId, VxGUID& sktConnectId, MembershipHosted& memberAvail );
    virtual void                announceMembershipRemoved( VxGUID& onlineId );

    std::vector< std::pair<VxGUID, std::pair<VxGUID, MembershipHosted>>> m_MemberList;

    std::vector<MembershipHostedCallbackInterface*> m_MemberClients;
};

