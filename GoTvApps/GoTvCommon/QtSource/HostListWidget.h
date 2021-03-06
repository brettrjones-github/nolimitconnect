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

#include <GoTvInterface/IDefs.h>

#include <CoreLib/VxTimer.h>
#include <CoreLib/VxGUID.h>

#include <QListWidget>

class HostListEntryWidget;
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
    HostListEntryWidget*        addOrUpdateHostSession( GuiHostSession* hostSession );

    GuiHostSession*             findSession( VxGUID& lclSessionId );
    HostListEntryWidget*        findListEntryWidgetBySessionId( VxGUID& sessionId );
    HostListEntryWidget*        findListEntryWidgetByOnlineId( VxGUID& onlineId );

signals:
	void						signalUpdateFriend( VxNetIdent * netIdent, bool sessionTimeChange );
	void						signalFriendClicked( VxNetIdent * netIdent );
	void						signalRefreshFriendList( EFriendViewType eWhichFriendsToShow );

protected slots:
	void						slotItemClicked( QListWidgetItem* item );
    void                        slotHostListItemClicked( HostListEntryWidget* hostItem );
    void                        slotHostMenuButtonClicked( HostListEntryWidget* hostItem );

protected:
    HostListEntryWidget*        sessionToWidget( GuiHostSession* hostSession );
    GuiHostSession*				widgetToSession( HostListEntryWidget* hostItem );

	//=== vars ===//
	AppCommon&					m_MyApp;
	P2PEngine&					m_Engine;
	VxTimer						m_ClickEventTimer; // avoid duplicate clicks
};

