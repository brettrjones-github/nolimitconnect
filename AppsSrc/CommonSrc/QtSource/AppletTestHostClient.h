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

#include "AppletTestBase.h"
#include "ui_AppletTestHostClient.h"

#include "GuiUserMgrGuiUserUpdateInterface.h"
#include "GuiHostedListCallback.h"

class AppletTestHostClient : public AppletTestBase, public GuiUserMgrGuiUserUpdateInterface, public GuiHostedListCallback
{
	Q_OBJECT
public:
    AppletTestHostClient( AppCommon& app, QWidget * parent );
	virtual ~AppletTestHostClient();

    virtual void				callbackIndentListUpdate( EUserViewType listType, VxGUID& onlineId, uint64_t timestamp ) override {};
    virtual void				callbackIndentListRemove( EUserViewType listType, VxGUID& onlineId ) override {};
    virtual void				callbackOnUserAdded( GuiUser* guiUser ) override;
    virtual void				callbackOnUserUpdated( GuiUser* guiUser ) override;
    virtual void				callbackOnUserRemoved( VxGUID& onlineId ) override {};

    virtual void				callbackGuiHostedListSearchResult( HostedId& hostedId, GuiHosted* guiHosted, VxGUID& sessionId ) override;

protected slots:
    void                        slotNetworkHostComboBoxSelectionChange( int comboIdx );
    void                        slotHostTypeComboBoxSelectionChange( int comboIdx );
    void                        slotHosListtUrlSelectionChange( int comboIdx );

    void                        slotNetworkHostIdentityButtonClicked( void );
    void                        slotQueryHostListFromNetworkHostButtonClicked( void );
    void                        slotJoinHostButtonClicked( void );

protected:
    void                        fillHostList( EHostType hostType, QComboBox* comboBox );
    void                        setNetworkHostIdFromNetHostComboBoxSelection( void );
    void                        updateHostType( EHostType hostType );
    bool                        updateHostedUrlComboBox( std::string& hostUrl );
    void                        updateHostedIdent( GuiHosted* guiHosted );

    Ui::AppletTestHostClientUi  ui;
    VxGUID                      m_NetworkHostOnlineId;
};


