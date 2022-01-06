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

class HostedId;
class GuiHostedListItem;
class VxNetIdent;
class GuiHostedListSession;
class GuiHosted;

class GuiHostedListWidget : public ListWidgetBase
{
	Q_OBJECT

public:
    GuiHostedListWidget( QWidget * parent );

    void                        clearHostList( void );

    void						updateHostedList( HostedId& hostedId, GuiHosted* guiHosted, VxGUID& sessionId );
    GuiHostedListItem*          addOrUpdateHostSession( GuiHostedListSession* hostSession );

    GuiHostedListSession*       findSession( VxGUID& lclSessionId );
    GuiHostedListSession*       findSession( HostedId& hostedId );
    GuiHostedListItem*          findListItemWidgetByHostId( HostedId& hostedId );
    GuiHostedListItem*          findListItemWidgetByOnlineId( VxGUID& onlineId );

signals:
    void                        signalIconButtonClicked( GuiHostedListSession* hostSession, GuiHostedListItem* hostItem );
    void                        signalMenuButtonClicked( GuiHostedListSession* hostSession, GuiHostedListItem* hostItem );
    void                        signalJoinButtonClicked( GuiHostedListSession* hostSession, GuiHostedListItem* hostItem );
    void                        signalConnectButtonClicked( GuiHostedListSession* hostSession, GuiHostedListItem* hostItem );

protected slots:
	void						slotItemClicked( QListWidgetItem* item );
    void                        slotGuiHostedListItemClicked( QListWidgetItem* hostItem );
    void                        slotIconButtonClicked( GuiHostedListItem* hostItem );
    void                        slotFriendshipButtonClicked( GuiHostedListItem* hostItem );
    void                        slotMenuButtonClicked( GuiHostedListItem* hostItem );
    void                        slotJoinButtonClicked( GuiHostedListItem* hostItem );
    void                        slotConnectButtonClicked( GuiHostedListItem* hostItem );

protected:
    GuiHostedListItem*          sessionToWidget( GuiHostedListSession* hostSession );
    GuiHostedListSession*		widgetToSession( GuiHostedListItem* hostItem );

    virtual void                onGuiHostedListItemClicked( GuiHostedListItem* hostItem );
    virtual void                onIconButtonClicked( GuiHostedListItem* hostItem );
    virtual void                onFriendshipButtonClicked( GuiHostedListItem* hostItem );
    virtual void                onMenuButtonClicked( GuiHostedListItem* hostItem );
    virtual void                onJoinButtonClicked( GuiHostedListItem* hostItem );
    virtual void                onConnectButtonClicked( GuiHostedListItem* hostItem );

	//=== vars ===//
};

