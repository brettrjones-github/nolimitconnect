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

class GroupieId;
class GuiGroupieListItem;
class VxNetIdent;
class GuiGroupieListSession;
class GuiGroupie;

class GuiGroupieListWidget : public ListWidgetBase
{
	Q_OBJECT

public:
    GuiGroupieListWidget( QWidget * parent );

    void                        clearHostList( void );

    void						updateGroupieList( GroupieId& hostedId, GuiGroupie* guiGroupie, VxGUID& sessionId );
    GuiGroupieListItem*         addOrUpdateHostSession( GuiGroupieListSession* hostSession );

    GuiGroupieListSession*      findSession( VxGUID& lclSessionId );
    GuiGroupieListSession*      findSession( GroupieId& hostedId );
    GuiGroupieListItem*         findListItemWidgetByHostId( GroupieId& hostedId );
    GuiGroupieListItem*         findListItemWidgetByOnlineId( VxGUID& onlineId );

signals:
    void                        signalIconButtonClicked( GuiGroupieListSession* hostSession, GuiGroupieListItem* hostItem );
    void                        signalMenuButtonClicked( GuiGroupieListSession* hostSession, GuiGroupieListItem* hostItem );
    void                        signalJoinButtonClicked( GuiGroupieListSession* hostSession, GuiGroupieListItem* hostItem );
    void                        signalConnectButtonClicked( GuiGroupieListSession* hostSession, GuiGroupieListItem* hostItem );

protected slots:
	void						slotItemClicked( QListWidgetItem* item );
    void                        slotGuiGroupieListItemClicked( QListWidgetItem* hostItem );
    void                        slotIconButtonClicked( GuiGroupieListItem* hostItem );
    void                        slotFriendshipButtonClicked( GuiGroupieListItem* hostItem );
    void                        slotMenuButtonClicked( GuiGroupieListItem* hostItem );
    void                        slotJoinButtonClicked( GuiGroupieListItem* hostItem );
    void                        slotConnectButtonClicked( GuiGroupieListItem* hostItem );

protected:
    GuiGroupieListItem*         sessionToWidget( GuiGroupieListSession* hostSession );
    GuiGroupieListSession*		widgetToSession( GuiGroupieListItem* hostItem );

    virtual void                onGuiGroupieListItemClicked( GuiGroupieListItem* hostItem );
    virtual void                onIconButtonClicked( GuiGroupieListItem* hostItem );
    virtual void                onFriendshipButtonClicked( GuiGroupieListItem* hostItem );
    virtual void                onMenuButtonClicked( GuiGroupieListItem* hostItem );
    virtual void                onJoinButtonClicked( GuiGroupieListItem* hostItem );
    virtual void                onConnectButtonClicked( GuiGroupieListItem* hostItem );

	//=== vars ===//
};

