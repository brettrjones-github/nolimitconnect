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

#include <QWidget> // must be declared first or linux Qt 6.2.4 will error in qmetatype.h 2167:23: array subscript value ‘53’ is outside the bounds

#include <ptop_src/ptop_engine_src/Membership/Membership.h>
#include <ptop_src/ptop_engine_src/Membership/MembershipAvailableCallbackInterface.h>

#include <CoreLib/VxMutex.h>

class AppCommon;
class GuiMembershipAvailableCallback;

class GuiMembershipAvailableMgr : public QObject, public MembershipAvailableCallbackInterface
{
    Q_OBJECT
public:
    GuiMembershipAvailableMgr() = delete;
    GuiMembershipAvailableMgr( AppCommon& app );
    GuiMembershipAvailableMgr( const GuiMembershipAvailableMgr& rhs ) = delete;
	virtual ~GuiMembershipAvailableMgr() = default;

    virtual void                onAppCommonCreated( void );
    virtual void                onMessengerReady( bool ready )              { }
    virtual bool                isMessengerReady( void );
    virtual void                onSystemReady( bool ready ) { }

    bool                        canPushToTalk( VxGUID& onlineId );

    void                        addMemberClient( GuiMembershipAvailableCallback* client, bool enable );

    virtual void				callbackMembershipAvailableAdded( VxGUID& onlineId, VxGUID& connectId, MembershipAvailable& memberAvail ) override; 
    virtual void				callbackMembershipAvailableUpdated( VxGUID& onlineId, VxGUID& connectId, MembershipAvailable& memberAvail ) override;
    virtual void				callbackMembershipAvailableRemoved( VxGUID& onlineId ) override; 

    virtual void                onMembershipAdded( VxGUID& onlineId, VxGUID& sktConnectId, MembershipAvailable& memberAvail );
    virtual void                onMembershipUpdated( VxGUID& onlineId, VxGUID& sktConnectId, MembershipAvailable& memberAvail );
    virtual void                onMembershipRemoved( VxGUID& onlineId );

    virtual void                announceMembershipAdded( VxGUID& onlineId, VxGUID& sktConnectId, MembershipAvailable& memberAvail );
    virtual void                announceMembershipUpdated( VxGUID& onlineId, VxGUID& sktConnectId, MembershipAvailable& memberAvail );
    virtual void                announceMembershipRemoved( VxGUID& onlineId );

signals:
    void                        signalInternalMembershipAvailableAdded( VxGUID onlineId, VxGUID connectId, MembershipAvailable memberAvail );
    void                        signalInternalMembershipAvailableUpdated( VxGUID onlineId, VxGUID connectId, MembershipAvailable memberAvail );
    void                        signalInternalMembershipAvailableRemoved( VxGUID onlineId );

private slots:
    void                        slotInternalMembershipAvailableAdded( VxGUID onlineId, VxGUID connectId, MembershipAvailable memberAvail );
    void                        slotInternalMembershipAvailableUpdated( VxGUID onlineId, VxGUID connectId, MembershipAvailable memberAvail );
    void                        slotInternalMembershipAvailableRemoved( VxGUID onlineId );

protected:
    void                        updateMemberAvail( VxGUID& onlineId, VxGUID& connectId, MembershipAvailable& memberAvail );
    void                        removeMemberAvail( VxGUID& onlineId );
    
    AppCommon&                  m_MyApp;
    VxMutex                     m_MemberListMutex;
    // online id, skt id, membership
    std::vector< std::pair<VxGUID, std::pair<VxGUID, MembershipAvailable>>> m_MemberList;

    std::vector<GuiMembershipAvailableCallback*> m_MemberClients;

};
