#pragma once
//============================================================================
// Copyright (C) 2020 Brett R. Jones
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

#include "AppletBase.h"
#include "AppDefs.h"
#include "ui_AppletPopupMenu.h"

#include <CoreLib/MediaCallbackInterface.h>

enum class EPopupMenuType
{
	ePopupMenuUnknown,
	ePopupMenuFriend,
	ePopupMenuAppSystem,
	ePopupMenuTitleBar,
	ePopupMenuListOptions1,
	ePopupMenuOfferFriendship,
	ePopupMenuUserSession,

	eMaxPopupMenu
};

class AppletPopupMenu : public AppletBase, public MediaCallbackInterface
{
	Q_OBJECT
public:
	AppletPopupMenu( AppCommon& app, QWidget* parent = NULL, VxGUID assetId = VxGUID::nullVxGUID() );
	virtual ~AppletPopupMenu() override;

	void						setTitle( QString strTitle );
	void						setMenuType( EPopupMenuType menuType )		{ m_MenuType = menuType; };
	EPopupMenuType				getMenuType( void )							{ return m_MenuType; };
	void						setAppletType( EApplet appletType )			{ m_AppletType = appletType; };
	EApplet						getAppletType( void )						{ return m_AppletType; };

	void						addMenuItem( int iItemId, QIcon& oIcon, QString strMenuItemText );

	void						showRefreshMenu();
	void						showFriendMenu( GuiUser* poSelectedFriend );
	void						showAppSystemMenu( void );
	void						showTitleBarMenu( void );
	void						showPersonOfferMenu( GuiUser* poSelectedFriend );
	void						showUserSessionMenu( EApplet appletType, GuiUserSessionBase* userSession );

signals:
	void						menuItemClicked( int iItemId, AppletPopupMenu* popupMenu, AppletBase* );

public slots:
	void						onFriendActionSelected( int iMenuId );
	void						onTitleBarActionSelected( int iMenuId );
	void						onPersonActionSelected( int iMenuId );
	void						onUserSessionActionSelected( int iMenuId );

private slots:
	//! user clicked the upper right x button
	void						slotBackButtonClicked( void ) override;
	//! user checked or unchecked menu item
	//void			userCheckedOrUncheckedAnItem( QListWidgetItem * item );
	//! user selected menu item
	void						itemClicked( QListWidgetItem* item );


protected:
	void						initAppletPopupMenu( void );

    //=== vars ===//
    Ui::AppletPopupMenuUi		ui;
	AppletBase*					m_ParentActivity{ nullptr };
	int							m_iMenuItemHeight{ 48 };
	GuiUser*					m_SelectedFriend{ nullptr };
	GuiUserSessionBase*			m_UserSession{ nullptr };
	QFrame*						m_ContentItemsFrame{ nullptr };
	EPopupMenuType				m_MenuType{ EPopupMenuType::ePopupMenuUnknown };
	EApplet						m_AppletType{ eAppletUnknown };
};
