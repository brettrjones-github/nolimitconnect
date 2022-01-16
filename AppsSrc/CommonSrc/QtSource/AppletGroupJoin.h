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
// http://www.nolimitconnect.com
//============================================================================

#include "AppletJoinBase.h"
#include "ui_AppletGroupJoin.h"

#include "GuiUserMgrGuiUserUpdateInterface.h"
#include "GuiHostedListCallback.h"
#include "GuiGroupieListCallback.h"

class AppletGroupJoin : public AppletJoinBase, public GuiUserMgrGuiUserUpdateInterface, public GuiHostedListCallback, public GuiGroupieListCallback
{
	Q_OBJECT
public:
    AppletGroupJoin( AppCommon& app, QWidget * parent );
	virtual ~AppletGroupJoin();

    virtual void                setStatusMsg( QString statusMsg );
    virtual void                setListLabel( QString labelText );

    virtual void				callbackIndentListUpdate( EUserViewType listType, VxGUID& onlineId, uint64_t timestamp ) override {};
    virtual void				callbackIndentListRemove( EUserViewType listType, VxGUID& onlineId ) override {};
    virtual void				callbackOnUserAdded( GuiUser* guiUser ) override;
    virtual void				callbackOnUserUpdated( GuiUser* guiUser ) override;
    virtual void				callbackOnUserRemoved( VxGUID& onlineId ) override {};

    virtual void				callbackGuiHostedListSearchResult( HostedId& hostedId, GuiHosted* guiHosted, VxGUID& sessionId ) override;

    virtual void				callbackGuiGroupieListSearchResult( GroupieId& groupieId, GuiGroupie* guiGroupie, VxGUID& sessionId ) override;

protected slots:
    void                        slotChooseHostModeButtonClick( void );

protected:
    void                        setupGuiMode( bool userListMode );
    virtual void				queryHostedList( void ) override;

    void                        updateHostedIdent( GuiHosted* guiHosted );

protected:
    Ui::AppletGroupJoinUi       ui;
    VxGUID                      m_JoinedHostSession;
};


