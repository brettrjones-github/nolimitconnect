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

#include "ListWidgetBase.h"

class HostJoinRequestListItem;
class VxNetIdent;
class GuiHostSession;
class PluginSetting;

class HostJoinRequestListWidget : public ListWidgetBase
{
	Q_OBJECT

public:
	HostJoinRequestListWidget( QWidget * parent );

    void                        clearHostJoinRequestList( void );

    void                        addHostAndSettingsToList( EHostType hostType, VxGUID& sessionId, VxNetIdent& hostIdent, PluginSetting& pluginSetting );
    HostJoinRequestListItem*    addOrUpdateHostSession( GuiHostSession* hostSession );

    GuiHostSession*             findSession( VxGUID& lclSessionId );
    HostJoinRequestListItem*    findListEntryWidgetByHostId( EHostType hostType, VxGUID& onlineId );
    HostJoinRequestListItem*    findListEntryWidgetByOnlineId( VxGUID& onlineId );

signals:
    void                        signalAvatarButtonClicked( GuiHostSession* hostSession, HostJoinRequestListItem* hostItem );
    void                        signalFrienshipButtonClicked( GuiHostSession* hostSession, HostJoinRequestListItem* hostItem );
    void                        signalMenuButtonClicked( GuiHostSession* hostSession, HostJoinRequestListItem* hostItem );
    void                        signalAcceptButtonClicked( GuiHostSession* hostSession, HostJoinRequestListItem* hostItem );
    void                        signalRejectButtonClicked( GuiHostSession* hostSession, HostJoinRequestListItem* hostItem );

protected slots:
	void						slotItemClicked( QListWidgetItem* item );
    void                        slotHostJoinRequestListItemClicked( QListWidgetItem* hostItem );
    void                        slotAvatarButtonClicked( HostJoinRequestListItem* hostItem );
    void                        slotlFriendshipButtonClicked( HostJoinRequestListItem* hostItem );
    void                        slotMenuButtonClicked( HostJoinRequestListItem* hostItem );
    void                        slotAcceptButtonClicked( HostJoinRequestListItem* hostItem );
    void                        slotRejectButtonClicked( HostJoinRequestListItem* hostItem );

protected:
    HostJoinRequestListItem*    sessionToWidget( GuiHostSession* hostSession );
    GuiHostSession*				widgetToSession( HostJoinRequestListItem* hostItem );

    virtual void                onHostJoinRequestListItemClicked( HostJoinRequestListItem* hostItem );
    virtual void                onAvatarButtonClicked( HostJoinRequestListItem* hostItem );
    virtual void                onFriendshipButtonClicked( HostJoinRequestListItem* hostItem );
    virtual void                onMenuButtonClicked( HostJoinRequestListItem* hostItem );
    virtual void                onAcceptButtonClicked( HostJoinRequestListItem* hostItem );
    virtual void                onRejectButtonClicked( HostJoinRequestListItem* hostItem );

	//=== vars ===//

};

