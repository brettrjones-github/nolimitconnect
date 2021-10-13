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

class HostJoinRequestListItem;
class VxNetIdent;
class AppCommon;
class MyIcons;
class P2PEngine;
class GuiHostSession;
class PluginSetting;

class HostJoinRequestListWidget : public QListWidget
{
	Q_OBJECT

public:
	HostJoinRequestListWidget( QWidget * parent );

	AppCommon&					getMyApp( void ) { return m_MyApp; }
	MyIcons&					getMyIcons( void );
    void                        clearHostJoinRequestList( void );

    void                        addHostAndSettingsToList( EHostType hostType, VxGUID& sessionId, VxNetIdent& hostIdent, PluginSetting& pluginSetting );
    HostJoinRequestListItem*    addOrUpdateHostSession( GuiHostSession* hostSession );

    GuiHostSession*             findSession( VxGUID& lclSessionId );
    HostJoinRequestListItem*    findListEntryWidgetByHostId( EHostType hostType, VxGUID& onlineId );
    HostJoinRequestListItem*    findListEntryWidgetByOnlineId( VxGUID& onlineId );

signals:
    void                        signalIconButtonClicked( GuiHostSession* hostSession, HostJoinRequestListItem* hostItem );
    void                        signalMenuButtonClicked( GuiHostSession* hostSession, HostJoinRequestListItem* hostItem );
    void                        signalJoinButtonClicked( GuiHostSession* hostSession, HostJoinRequestListItem* hostItem );
    void                        signalConnectButtonClicked( GuiHostSession* hostSession, HostJoinRequestListItem* hostItem );

protected slots:
	void						slotItemClicked( QListWidgetItem* item );
    void                        slotHostJoinRequestListItemClicked( QListWidgetItem* hostItem );
    void                        slotIconButtonClicked( HostJoinRequestListItem* hostItem );
    void                        slotMenuButtonClicked( HostJoinRequestListItem* hostItem );
    void                        slotJoinButtonClicked( HostJoinRequestListItem* hostItem );
    void                        slotConnectButtonClicked( HostJoinRequestListItem* hostItem );

protected:
    HostJoinRequestListItem*               sessionToWidget( GuiHostSession* hostSession );
    GuiHostSession*				widgetToSession( HostJoinRequestListItem* hostItem );

    virtual void                onHostJoinRequestListItemClicked( HostJoinRequestListItem* hostItem );
    virtual void                onIconButtonClicked( HostJoinRequestListItem* hostItem );
    virtual void                onMenuButtonClicked( HostJoinRequestListItem* hostItem );
    virtual void                onJoinButtonClicked( HostJoinRequestListItem* hostItem );
    virtual void                onConnectButtonClicked( HostJoinRequestListItem* hostItem );

	//=== vars ===//
	AppCommon&					m_MyApp;
	P2PEngine&					m_Engine;
	VxTimer						m_ClickEventTimer; // avoid duplicate clicks
};

