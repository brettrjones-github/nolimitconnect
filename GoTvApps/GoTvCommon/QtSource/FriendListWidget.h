#pragma once
//============================================================================
// Copyright (C) 2013 Brett R. Jones 
// Issued to MIT style license by Brett R. Jones in 2017
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

#include "FriendList.h"

#include <CoreLib/AssetDefs.h>
#include <CoreLib/VxTimer.h>

#include <QListWidget>

class FriendListEntryWidget;
class GuiUser;
class AppCommon;
class MyIcons;
class P2PEngine;

class FriendListWidget : public QListWidget
{
	Q_OBJECT

public:
	FriendListWidget( QWidget * parent );

	AppCommon&					getMyApp( void ) { return m_MyApp; }
	MyIcons&					getMyIcons( void );

	void						setFriendViewType( EFriendViewType eWhichFriendsToShow );
	EFriendViewType				getFriendViewType( void );

	void						setFriendHasUnviewedTextMessages( VxGUID& onlineId, bool hasTextMsgs );
	//! update friend in list
	void						updateFriend( GuiUser * netIdent, bool sessionTimeChange = false );
	void						removeFriend( GuiUser * netIdent );
	void						removeFriend( VxGUID& onlineId );
	void						refreshFriendList( EFriendViewType eWhichFriendsToShow );

signals:
	void						signalUpdateFriend( GuiUser * netIdent, bool sessionTimeChange );
	void						signalFriendClicked( GuiUser * netIdent );
	void						signalRefreshFriendList( EFriendViewType eWhichFriendsToShow );

private slots:
	void						slotUpdateFriend( GuiUser * netIdent, bool sessionTimeChange );
	void						slotRefreshFriend( VxGUID friendId );
	void						slotAssetViewMsgAction( EAssetAction eAssetAction, VxGUID onlineId, int pos );
	void						slotItemClicked(QListWidgetItem *);
	void						slotRefreshFriendList( EFriendViewType eWhichFriendsToShow );
	void						slotFriendListItemClicked( FriendListEntryWidget* widget );
	void						slotFriendMenuButtonClicked( FriendListEntryWidget* widget );

protected:
	//!	fill friend into new QListWidgetItem *
	FriendListEntryWidget *		friendToWidget( GuiUser * poFriend );
	//!	get friend from QListWidgetItem data
    GuiUser *					widgetToFriend( FriendListEntryWidget * item );

	void						updateListEntryWidget( FriendListEntryWidget * item, GuiUser * netIdent );

	FriendListEntryWidget *		findListEntryWidget( GuiUser * netIdent );

	//=== vars ===//
	AppCommon&					m_MyApp;
	P2PEngine&					m_Engine;
	EFriendViewType				m_eFriendViewType;
    GuiUser *					m_SelectedFriend;
	VxTimer						m_ClickEventTimer; // avoid duplicate clicks
	VxGUID						m_ViewingOnlineId;
	bool						m_IsCurrentlyViewing;
};

