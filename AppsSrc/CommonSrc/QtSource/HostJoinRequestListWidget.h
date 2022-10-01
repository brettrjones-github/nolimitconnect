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

#include "ListWidgetBase.h"

class HostJoinRequestListItem;
class GuiHostJoin;
class GuiHostJoinSession;
class GroupieId;

class HostJoinRequestListWidget : public ListWidgetBase
{
	Q_OBJECT

public:
	HostJoinRequestListWidget( QWidget* parent );

    void                        clearHostJoinRequestList( void );

    void                        addOrUpdateHostRequest( GuiHostJoin* hostJoin );
    HostJoinRequestListItem*    addOrUpdateHostRequestSession( GuiHostJoinSession* hostSession );

    HostJoinRequestListItem*    findListEntryWidgetByGroupieId( GroupieId& groupieId );
    void                        removeHostJoinRequest( GroupieId& groupieId );

signals:
    void                        signalAvatarButtonClicked( GuiHostJoinSession* hostSession, HostJoinRequestListItem* hostItem );
    void                        signalFrienshipButtonClicked( GuiHostJoinSession* hostSession, HostJoinRequestListItem* hostItem );
    void                        signalMenuButtonClicked( GuiHostJoinSession* hostSession, HostJoinRequestListItem* hostItem );
    void                        signalAcceptButtonClicked( GuiHostJoinSession* hostSession, HostJoinRequestListItem* hostItem );
    void                        signalRejectButtonClicked( GuiHostJoinSession* hostSession, HostJoinRequestListItem* hostItem );

protected slots:
	void						slotItemClicked( QListWidgetItem* item );
    void                        slotHostJoinRequestListItemClicked( QListWidgetItem* hostItem );
    void                        slotAvatarButtonClicked( HostJoinRequestListItem* hostItem );
    void                        slotlFriendshipButtonClicked( HostJoinRequestListItem* hostItem );
    void                        slotMenuButtonClicked( HostJoinRequestListItem* hostItem );
    void                        slotAcceptButtonClicked( HostJoinRequestListItem* hostItem );
    void                        slotRejectButtonClicked( HostJoinRequestListItem* hostItem );

protected:
    HostJoinRequestListItem*    sessionToWidget( GuiHostJoinSession* hostSession );
    GuiHostJoinSession*			widgetToSession( HostJoinRequestListItem* hostItem );

    virtual void                onHostJoinRequestListItemClicked( HostJoinRequestListItem* hostItem );
    virtual void                onAvatarButtonClicked( HostJoinRequestListItem* hostItem );
    virtual void                onFriendshipButtonClicked( HostJoinRequestListItem* hostItem );
    virtual void                onMenuButtonClicked( HostJoinRequestListItem* hostItem );
    virtual void                onAcceptButtonClicked( HostJoinRequestListItem* hostItem );
    virtual void                onRejectButtonClicked( HostJoinRequestListItem* hostItem );


	//=== vars ===//

};

