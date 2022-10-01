#pragma once
//============================================================================
// Copyright (C) 2019 Brett R. Jones
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

#include "AppletJoinBase.h"
#include "ui_AppletGroupJoin.h"

#include "GuiUserUpdateCallback.h"
#include "GuiHostedListCallback.h"
#include "GuiGroupieListCallback.h"
#include "GuiHostJoinCallback.h"
#include "GuiUserJoinCallback.h"

class GuiUserListItem;

class AppletGroupJoin : public AppletJoinBase, public GuiUserUpdateCallback, public GuiHostedListCallback, public GuiGroupieListCallback, public GuiHostJoinCallback, public GuiUserJoinCallback
{
	Q_OBJECT
public:
    AppletGroupJoin( AppCommon& app, QWidget* parent );
	virtual ~AppletGroupJoin();

    virtual void                setStatusMsg( QString statusMsg );
    virtual void                setListLabel( QString labelText );

    virtual void				callbackIndentListUpdate( EUserViewType listType, VxGUID& onlineId, uint64_t timestamp ) override {};
    virtual void				callbackIndentListRemove( EUserViewType listType, VxGUID& onlineId ) override {};
    virtual void				callbackOnUserAdded( GuiUser* guiUser ) override;
    virtual void				callbackOnUserUpdated( GuiUser* guiUser ) override;
    virtual void				callbackOnUserRemoved( VxGUID& onlineId ) override {};
    virtual void				callbackPushToTalkStatus( VxGUID& onlineId, EPushToTalkStatus pushToTalkStatus ) override {};

    virtual void				callbackGuiHostedListSearchResult( HostedId& hostedId, GuiHosted* guiHosted, VxGUID& sessionId ) override;
    virtual void				callbackGuiHostedListSearchComplete( EHostType hostType, VxGUID& sessionId ) override;

    virtual void				callbackGuiGroupieListSearchResult( GroupieId& groupieId, GuiGroupie* guiGroupie, VxGUID& sessionId ) override;
    virtual void				callbackGuiGroupieListSearchComplete( EHostType hostType, VxGUID& sessionId ) override;

    virtual void				callbackGuiHostJoinRequested( GroupieId& groupieId, GuiHostJoin* guiHostJoin ) override;
    virtual void				callbackGuiHostJoinWasGranted( GroupieId& groupieId, GuiHostJoin* guiHostJoin ) override;
    virtual void				callbackGuiHostJoinIsGranted( GroupieId& groupieId, GuiHostJoin* guiHostJoin ) override;
    virtual void				callbackGuiHostUnJoinGranted( GroupieId& groupieId, GuiHostJoin* guiHostJoin ) override;

    virtual void				callbackGuiHostJoinDenied( GroupieId& groupieId, GuiHostJoin* guiHostJoin ) override;
    virtual void				callbackGuiHostJoinLeaveHost( GroupieId& groupieId ) override;
    virtual void				callbackGuiHostUnJoin( GroupieId& groupieId ) override;
    virtual void				callbackGuiHostJoinRemoved( GroupieId& groupieId ) override;

    virtual void				callbackGuiUserJoinRequested( GroupieId& groupieId, GuiUserJoin* guiUserJoin ) override;
    virtual void				callbackGuiUserJoinWasGranted( GroupieId& groupieId, GuiUserJoin* guiUserJoin ) override;
    virtual void				callbackGuiUserJoinIsGranted( GroupieId& groupieId, GuiUserJoin* guiUserJoin ) override;
    virtual void				callbackGuiUserUnJoinGranted( GroupieId& groupieId, GuiUserJoin* guiUserJoin ) override;

    virtual void				callbackGuiUserJoinDenied( GroupieId& groupieId, GuiUserJoin* guiUserJoin ) override;
    virtual void				callbackGuiUserJoinLeaveHost( GroupieId& groupieId ) override;
    virtual void				callbackGuiUserJoinRemoved( GroupieId& groupieId ) override;

protected slots:
    void                        slotChooseHostModeButtonClick( void );
    void                        slotShowIgnoredHostsListButtonClicked( void );

    void                        slotIconButtonClicked( GuiHostedListSession* hostSession, GuiHostedListItem* hostItem );
    void                        slotMenuButtonClicked( GuiHostedListSession* hostSession, GuiHostedListItem* hostItem );
    void                        slotJoinButtonClicked( GuiHostedListSession* hostSession, GuiHostedListItem* hostItem );
    void                        slotConnectButtonClicked( GuiHostedListSession* hostSession, GuiHostedListItem* hostItem );
    void                        slotKickButtonClicked( GuiHostedListSession* hostSession, GuiHostedListItem* hostItem );
    void                        slotIgnoreButtonClicked( GuiHostedListSession* hostSession, GuiHostedListItem* hostItem );

protected:
    void                        changeGuiMode( bool userListMode );
    virtual void				queryHostedList( void ) override;

    void                        updateUser( GuiUser* guiUser );

    void                        onJoinedHost( GroupieId& groupieId, GuiHosted* guiHosted );

protected:
    Ui::AppletGroupJoinUi       ui;
    VxGUID                      m_JoinedHostSession;
    bool                        m_UserListMode{ false };
    GroupieId                   m_AdminGroupieId;
};


