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

#include <GuiInterface/IDefs.h>

#include <CoreLib/VxTimer.h>
#include <CoreLib/VxGUID.h>

#include <QListWidget>

class HostListItem;
class VxNetIdent;
class AppCommon;
class MyIcons;
class P2PEngine;
class GuiHostSession;
class PluginSetting;

class HostListWidget : public QListWidget
{
	Q_OBJECT

public:
	HostListWidget( QWidget * parent );

	AppCommon&					getMyApp( void ) { return m_MyApp; }
	MyIcons&					getMyIcons( void );
    void                        clearHostList( void );

    void                        addHostAndSettingsToList( EHostType hostType, VxGUID& sessionId, VxNetIdent& hostIdent, PluginSetting& pluginSetting );
    HostListItem*               addOrUpdateHostSession( GuiHostSession* hostSession );

    GuiHostSession*             findSession( VxGUID& lclSessionId );
    HostListItem*               findListEntryWidgetByHostId( EHostType hostType, VxGUID& onlineId );
    HostListItem*               findListEntryWidgetByOnlineId( VxGUID& onlineId );

signals:
    void                        signalIconButtonClicked( GuiHostSession* hostSession, HostListItem* hostItem );
    void                        signalMenuButtonClicked( GuiHostSession* hostSession, HostListItem* hostItem );
    void                        signalJoinButtonClicked( GuiHostSession* hostSession, HostListItem* hostItem );
    void                        signalConnectButtonClicked( GuiHostSession* hostSession, HostListItem* hostItem );

protected slots:
	void						slotItemClicked( QListWidgetItem* item );
    void                        slotHostListItemClicked( QListWidgetItem* hostItem );
    void                        slotIconButtonClicked( HostListItem* hostItem );
    void                        slotMenuButtonClicked( HostListItem* hostItem );
    void                        slotJoinButtonClicked( HostListItem* hostItem );
    void                        slotConnectButtonClicked( HostListItem* hostItem );

protected:
    HostListItem*               sessionToWidget( GuiHostSession* hostSession );
    GuiHostSession*				widgetToSession( HostListItem* hostItem );

    virtual void                onHostListItemClicked( HostListItem* hostItem );
    virtual void                onIconButtonClicked( HostListItem* hostItem );
    virtual void                onMenuButtonClicked( HostListItem* hostItem );
    virtual void                onJoinButtonClicked( HostListItem* hostItem );
    virtual void                onConnectButtonClicked( HostListItem* hostItem );

	//=== vars ===//
	AppCommon&					m_MyApp;
	P2PEngine&					m_Engine;
	VxTimer						m_ClickEventTimer; // avoid duplicate clicks
};

