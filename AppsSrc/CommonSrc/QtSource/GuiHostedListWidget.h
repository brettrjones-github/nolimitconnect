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
#include "GuiUserUpdateCallback.h"

class HostedId;
class GuiHostedListItem;
class VxNetIdent;
class GuiHostedListSession;
class GuiHosted;
class GroupieId;
class GuiHostJoin;
class GuiUserJoin;

class GuiHostedListWidget : public ListWidgetBase, public GuiUserUpdateCallback
{
	Q_OBJECT

public:
    GuiHostedListWidget( QWidget * parent );
    virtual ~GuiHostedListWidget();

    void                        clearHostList( void );

    void						updateHostedList( HostedId& hostedId, GuiHosted* guiHosted, VxGUID& sessionId );
    GuiHostedListItem*          addOrUpdateHostSession( GuiHostedListSession* hostSession );

    void                        updateHostJoinState( HostedId& hostedId, EJoinState joinState );
    void                        updateUserJoinState( HostedId& hostedId, EJoinState joinState );
    void                        updateJoinState( HostedId& hostedId, EJoinState joinState );
    void                        removeFromList( HostedId& hostedId );

    GuiHostedListSession*       findSession( VxGUID& lclSessionId );
    GuiHostedListSession*       findSession( HostedId& hostedId );
    GuiHostedListItem*          findListItemWidgetByHostId( HostedId& hostedId );
    GuiHostedListItem*          findListItemWidgetByOnlineId( VxGUID& onlineId );
    GuiHosted*                  findGuiHostedByHostId( HostedId& hostedId );

    virtual void				callbackOnUserAdded( GuiUser* guiUser ) override;
    virtual void				callbackOnUserUpdated( GuiUser* guiUser ) override;

    virtual void				callbackGuiHostJoinRequested( GroupieId& groupieId, GuiHostJoin* guiHostJoin );
    virtual void				callbackGuiHostJoinWasGranted( GroupieId& groupieId, GuiHostJoin* guiHostJoin );
    virtual void				callbackGuiHostJoinIsGranted( GroupieId& groupieId, GuiHostJoin* guiHostJoin );
    virtual void				callbackGuiHostUnJoinGranted( GroupieId& groupieId, GuiHostJoin* guiHostJoin );

    virtual void				callbackGuiHostJoinDenied( GroupieId& groupieId, GuiHostJoin* guiHostJoin );
    virtual void				callbackGuiHostJoinLeaveHost( GroupieId& groupieId );
    virtual void				callbackGuiHostUnJoin( GroupieId& groupieId );
    virtual void				callbackGuiHostJoinRemoved( GroupieId& groupieId );

    virtual void				callbackGuiUserJoinRequested( GroupieId& groupieId, GuiUserJoin* guiUserJoin );
    virtual void				callbackGuiUserJoinWasGranted( GroupieId& groupieId, GuiUserJoin* guiUserJoin );
    virtual void				callbackGuiUserJoinIsGranted( GroupieId& groupieId, GuiUserJoin* guiUserJoin );
    virtual void				callbackGuiUserUnJoinGranted( GroupieId& groupieId, GuiUserJoin* guiUserJoin );

    virtual void				callbackGuiUserJoinDenied( GroupieId& groupieId, GuiUserJoin* guiUserJoin );
    virtual void				callbackGuiUserJoinLeaveHost( GroupieId& groupieId );
    virtual void				callbackGuiUserJoinRemoved( GroupieId& groupieId );

    void                        updateUser( GuiUser* guiUser );

signals:
    void                        signalGuiHostedListItemClicked( QListWidgetItem* hostItem );
    void                        signalIconButtonClicked( GuiHostedListSession* hostSession, GuiHostedListItem* hostItem );
    void                        signalMenuButtonClicked( GuiHostedListSession* hostSession, GuiHostedListItem* hostItem );
    void                        signalJoinButtonClicked( GuiHostedListSession* hostSession, GuiHostedListItem* hostItem );
    void                        signalConnectButtonClicked( GuiHostedListSession* hostSession, GuiHostedListItem* hostItem );
    void                        signalKickButtonClicked( GuiHostedListSession* hostSession, GuiHostedListItem* hostItem );
    void                        signalFavoriteButtonClicked( GuiHostedListSession* hostSession, GuiHostedListItem* hostItem );

protected slots:
    void                        slotGuiHostedListItemClicked( QListWidgetItem* hostItem );
    void                        slotIconButtonClicked( GuiHostedListItem* hostItem );
    void                        slotFriendshipButtonClicked( GuiHostedListItem* hostItem );
    void                        slotMenuButtonClicked( GuiHostedListItem* hostItem );

    void                        slotJoinButtonClicked( GuiHostedListItem* hostItem );
    void                        slotConnectButtonClicked( GuiHostedListItem* hostItem );
    void                        slotKickButtonClicked( GuiHostedListItem* hostItem );
    void                        slotFavoriteButtonClicked( GuiHostedListItem* hostItem );

protected:
    GuiHostedListItem*          sessionToWidget( GuiHostedListSession* hostSession );
    GuiHostedListSession*		widgetToSession( GuiHostedListItem* hostItem );

    virtual void                onGuiHostedListItemClicked( GuiHostedListItem* hostItem );
    virtual void                onIconButtonClicked( GuiHostedListItem* hostItem );
    virtual void                onFriendshipButtonClicked( GuiHostedListItem* hostItem );
    virtual void                onMenuButtonClicked( GuiHostedListItem* hostItem );

    virtual void                onJoinButtonClicked( GuiHostedListItem* hostItem );
    virtual void                onConnectButtonClicked( GuiHostedListItem* hostItem );
    virtual void                onKickButtonClicked( GuiHostedListItem* hostItem );
    virtual void                onFavoriteButtonClicked( GuiHostedListItem* hostItem );

	//=== vars ===//
};

