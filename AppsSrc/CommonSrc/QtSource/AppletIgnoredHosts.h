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
#include "ui_AppletIgnoredHosts.h"

#include <ptop_src/ptop_engine_src/IdentListMgrs/IgnoreLIstMgr.h>

class GuiUserListItem;

class AppletIgnoredHosts : public AppletBase
{
	Q_OBJECT
public:
    AppletIgnoredHosts( AppCommon& app, QWidget * parent );
	virtual ~AppletIgnoredHosts();

    virtual void                setStatusMsg( QString statusMsg );
    virtual void                setListLabel( QString labelText );


protected slots:
    void                        slotIconButtonClicked( GuiHostedListSession* hostSession, GuiHostedListItem* hostItem );
    void                        slotMenuButtonClicked( GuiHostedListSession* hostSession, GuiHostedListItem* hostItem );
    void                        slotJoinButtonClicked( GuiHostedListSession* hostSession, GuiHostedListItem* hostItem );
    void                        slotConnectButtonClicked( GuiHostedListSession* hostSession, GuiHostedListItem* hostItem );
    void                        slotKickButtonClicked( GuiHostedListSession* hostSession, GuiHostedListItem* hostItem );
    void                        slotFavoriteButtonClicked( GuiHostedListSession* hostSession, GuiHostedListItem* hostItem );

protected:
    void                        queryIgnoredHostsList( void );
    void                        insertIntoHostList( IgnoredHostInfo& hostInfo );

protected:
    Ui::AppletIgnoredHostsUi       ui;
    std::map<VxGUID, IgnoredHostInfo> m_IgnoredHostList;
};


