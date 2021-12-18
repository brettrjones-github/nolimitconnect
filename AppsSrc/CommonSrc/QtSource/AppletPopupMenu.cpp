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

#include "AppCommon.h"	
#include "AppSettings.h"
#include "VxAppTheme.h"

#include "AppletPopupMenu.h"
#include "AppletPeerChangeFriendship.h"

#include "FileShareItemWidget.h"
#include "MyIcons.h"
#include "AppGlobals.h"
#include "AppletMgr.h"
#include "FileItemInfo.h"
#include "FileActionMenu.h"
#include "GuiHelpers.h"
#include "GuiParams.h"
#include "GuiHostSession.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>
#include <ptop_src/ptop_engine_src/AssetMgr/AssetMgr.h>

#include <PktLib/VxSearchDefs.h>
#include <NetLib/VxFileXferInfo.h>
#include <CoreLib/VxFileInfo.h>
#include <CoreLib/VxGlobals.h>
#include <VxVideoLib/VxVideoLib.h>

#include <QResizeEvent>
#include <QMessageBox>
#include <QTimer>

//============================================================================
AppletPopupMenu::AppletPopupMenu( AppCommon& app, QWidget * parent, VxGUID assetId )
: AppletBase( OBJNAME_APPLET_POPUP_MENU, app, parent )
{
    setAppletType( eAppletPopupMenu );
	initAppletPopupMenu();
    setTitleBarText( DescribeApplet( m_EAppletType ) );
	getTitleBarWidget()->setVisible( false );
	getBottomBarWidget()->setVisible( false );

    connect( ui.m_ExitPopupButton, SIGNAL( clicked() ), this, SLOT( close() ) );
    connect( ui.m_MenuItemList, SIGNAL( itemClicked( QListWidgetItem* ) ), this, SLOT( itemClicked( QListWidgetItem* ) ) );

    // NOT USED FOR POPUP  m_MyApp.activityStateChange( this, true );
}

//============================================================================
AppletPopupMenu::~AppletPopupMenu()
{
    // NOT USED FOR POPUP  m_MyApp.activityStateChange( this, false );
}

//============================================================================
void AppletPopupMenu::initAppletPopupMenu( void )
{
	ui.setupUi( getContentItemsFrame() );
	ui.m_ExitPopupButton->setIcons( eMyIconBack );
	ui.m_ExitPopupButton->setFixedSize( GuiParams::getButtonSize( eButtonSizeSmall ) );
}

//============================================================================
void AppletPopupMenu::setTitle( QString strTitle )
{
	ui.m_PopupTitleLabel->setText( strTitle );
}

//============================================================================
void AppletPopupMenu::setMenuType( EPopupMenuType menuType ) 
{ 
	m_MenuType = menuType; 
	clearMenulList();
	m_MyApp.getAppStyle().clearFocusFrameWidget();
};

//============================================================================
void AppletPopupMenu::clearMenulList( void )
{
	ui.m_MenuItemList->clear();
};

//============================================================================
void AppletPopupMenu::addMenuItem( int iItemId, QIcon& oIcon, QString strMenuItemText )
{
	QListWidgetItem* poMenuItem = new QListWidgetItem( strMenuItemText );
	poMenuItem->setIcon( oIcon );
	poMenuItem->setData( Qt::UserRole, iItemId );
	ui.m_MenuItemList->addItem( poMenuItem );
}

//============================================================================
//! clicked the upper right back button
void AppletPopupMenu::slotBackButtonClicked( void )
{
	emit signalBackButtonClicked();
	m_MyApp.getAppStyle().clearFocusFrameWidget();
}

//============================================================================
void AppletPopupMenu::itemClicked( QListWidgetItem* item )
{
	m_MyApp.playSound( eSndDefButtonClick );
	// resignal with id set by user
	int iItemId = item->data( Qt::UserRole ).toInt();
	if( getMenuType() == EPopupMenuType::ePopupMenuFriend )
	{
		onFriendActionSelected( iItemId );
	}
	else if( getMenuType() == EPopupMenuType::ePopupMenuHostSession )
	{
		onHostSessionActionSelected( iItemId );
	}
	else if( getMenuType() == EPopupMenuType::ePopupMenuOfferFriendship )
	{
		onPersonActionSelected( iItemId );
	}
	else if( getMenuType() == EPopupMenuType::ePopupMenuTitleBar )
	{
		onTitleBarActionSelected( iItemId );
	}
	else if( getMenuType() == EPopupMenuType::ePopupMenuUserSession )
	{
		onPersonActionSelected( iItemId );
	}
	else
	{
		emit menuItemClicked( iItemId, this, m_ParentActivity );
	}

	close();
}

//============================================================================
void AppletPopupMenu::showRefreshMenu()
{
	setMenuType( EPopupMenuType::ePopupMenuListOptions1 );
	setTitle( QObject::tr( "Who to show in list" ) );
	addMenuItem( 0, getMyIcons().getIcon( eMyIconFriend ), QObject::tr( "Everybody" ) );
	addMenuItem( 1, getMyIcons().getIcon( eMyIconFriend ), QObject::tr( "Friends And Guests" ) );
	addMenuItem( 2, getMyIcons().getIcon( eMyIconAnonymous ), QObject::tr( "Anonymous" ) );
	addMenuItem( 3, getMyIcons().getIcon( eMyIconAdministrator ), QObject::tr( "Administrators" ) );
	addMenuItem( 4, getMyIcons().getIcon( eMyIconIgnored ), QObject::tr( "Ignored" ) );
	addMenuItem( 5, getMyIcons().getIcon( eMyIconRefreshNormal ), QObject::tr( "Refresh List" ) );
}

//============================================================================
void AppletPopupMenu::showAppSystemMenu( void )
{
	setMenuType( EPopupMenuType::ePopupMenuAppSystem );
	setTitle( QObject::tr( "System Menu" ) );
	addMenuItem( 0, getMyIcons().getIcon( eMyIconDebug ), QObject::tr( "Debug Settings" ) );
	addMenuItem( 1, getMyIcons().getIcon( eMyIconDebug ), QObject::tr( "Debug Log" ) );
}

//============================================================================
void AppletPopupMenu::showTitleBarMenu( void )
{
	setMenuType( EPopupMenuType::ePopupMenuTitleBar );
	setTitle( QObject::tr( "Show List" ) );
	addMenuItem( 0, getMyIcons().getIcon( eMyIconFriendJoined ), QObject::tr( "Friends List" ) );
	addMenuItem( 1, getMyIcons().getIcon( eMyIconGroupJoined ), QObject::tr( "Groups List" ) );
	if( m_MyApp.getAppSettings().getFeatureEnable( eAppFeatureChatRoom ) )
	{
		addMenuItem( 2, getMyIcons().getIcon( eMyIconServiceChatRoom ), QObject::tr( "Chat Room List" ) );
	}

	if( m_MyApp.getAppSettings().getFeatureEnable( eAppFeatureRandomConnect ) )
	{
		addMenuItem( 3, getMyIcons().getIcon( eMyIconServiceRandomConnect ), QObject::tr( "Random Connect List" ) );
	}
}

//============================================================================
void AppletPopupMenu::onTitleBarActionSelected( int iMenuId )
{
	switch( iMenuId )
	{
	case 0: // friends listing
		m_MyApp.getAppletMgr().launchApplet( eAppletFriendListClient, getParentPageFrame() );
		break;

	case 1: // group listing
		m_MyApp.getAppletMgr().launchApplet( eAppletGroupListClient, getParentPageFrame() );
		break;

	case 2: // chat room listing
		//m_MyApp.getAppletMgr().launchApplet( eAppletGroupListClient, dynamic_cast<QWidget *>(parent()) );
		break;

	case 3: // random connect listing
		//m_MyApp.getAppletMgr().launchApplet( eAppletGroupListClient, dynamic_cast<QWidget *>(parent()) );
		break;

	default:
		LogMsg( LOG_ERROR, "Unknown AppletPopupMenu::onTitleBarActionSelected value %d", iMenuId );
	}
}

//============================================================================
void AppletPopupMenu::showFriendMenu( GuiUser* poSelectedFriend )
{
	setMenuType( EPopupMenuType::ePopupMenuFriend );
	m_SelectedFriend = poSelectedFriend;
	// populate title
	QString strTitle = poSelectedFriend->describeMyFriendshipToHim();
	strTitle += ": ";
	strTitle += poSelectedFriend->getOnlineName().c_str();
	setTitle( strTitle );
	// populate menu
	EPluginAccess ePluginAccess;
	QString strAction;

	if( m_SelectedFriend->isIgnored() )
	{
		addMenuItem( (int)eMaxPluginType + 1, getMyIcons().getIcon( eMyIconFriend ), QObject::tr( "Unblock" ) );
	}
	else
	{
		addMenuItem( (int)eMaxPluginType + 1, getMyIcons().getIcon( eMyIconIgnored ), QObject::tr( "Block" ) );
		addMenuItem( (int)eMaxPluginType + 2, getMyIcons().getIcon( eMyIconFriend ), QObject::tr( "Change Friendship" ) );
	}

	if( m_SelectedFriend->isMyAccessAllowedFromHim( ePluginTypeWebServer ) )
	{
		ePluginAccess = poSelectedFriend->getMyAccessPermissionFromHim( ePluginTypeWebServer );
		strAction = GuiParams::describePluginAction( poSelectedFriend, ePluginTypeWebServer, ePluginAccess );
		addMenuItem( (int)ePluginTypeWebServer, getMyIcons().getIcon( getMyIcons().getPluginIcon( ePluginTypeWebServer, ePluginAccess ) ), strAction );
	}

	if( m_MyApp.getAppSettings().getFeatureEnable( eAppFeatureStoryboard ) )
	{
		if( m_SelectedFriend->isMyAccessAllowedFromHim( ePluginTypeStoryboard ) )
		{
			ePluginAccess = poSelectedFriend->getMyAccessPermissionFromHim( ePluginTypeStoryboard );
			strAction = GuiParams::describePluginAction( poSelectedFriend, ePluginTypeStoryboard, ePluginAccess );
			addMenuItem( (int)ePluginTypeStoryboard, getMyIcons().getIcon( getMyIcons().getPluginIcon( ePluginTypeStoryboard, ePluginAccess ) ), strAction );
		}
	}

	ePluginAccess = poSelectedFriend->getMyAccessPermissionFromHim( ePluginTypeMessenger );
	strAction = GuiParams::describePluginAction( poSelectedFriend, ePluginTypeMessenger, ePluginAccess );
	addMenuItem( (int)ePluginTypeMessenger, getMyIcons().getIcon( getMyIcons().getPluginIcon( ePluginTypeMessenger, ePluginAccess ) ), strAction );

	if( m_SelectedFriend->isMyAccessAllowedFromHim( ePluginTypeVideoPhone ) )
	{
		ePluginAccess = poSelectedFriend->getMyAccessPermissionFromHim( ePluginTypeVideoPhone );
		strAction = GuiParams::describePluginAction( poSelectedFriend, ePluginTypeVideoPhone, ePluginAccess );
		addMenuItem( (int)ePluginTypeVideoPhone, getMyIcons().getIcon( getMyIcons().getPluginIcon( ePluginTypeVideoPhone, ePluginAccess ) ), strAction );
	}

	if( m_SelectedFriend->isMyAccessAllowedFromHim( ePluginTypeVoicePhone ) )
	{
		ePluginAccess = poSelectedFriend->getMyAccessPermissionFromHim( ePluginTypeVoicePhone );
		strAction = GuiParams::describePluginAction( poSelectedFriend, ePluginTypeVoicePhone, ePluginAccess );
		addMenuItem( (int)ePluginTypeVoicePhone, getMyIcons().getIcon( getMyIcons().getPluginIcon( ePluginTypeVoicePhone, ePluginAccess ) ), strAction );
	}

	if( m_SelectedFriend->isMyAccessAllowedFromHim( ePluginTypeTruthOrDare ) )
	{
		ePluginAccess = poSelectedFriend->getMyAccessPermissionFromHim( ePluginTypeTruthOrDare );
		strAction = GuiParams::describePluginAction( poSelectedFriend, ePluginTypeTruthOrDare, ePluginAccess );
		addMenuItem( (int)ePluginTypeTruthOrDare, getMyIcons().getIcon( getMyIcons().getPluginIcon( ePluginTypeTruthOrDare, ePluginAccess ) ), strAction );
	}

	if( m_SelectedFriend->isMyAccessAllowedFromHim( ePluginTypeCamServer ) )
	{
		ePluginAccess = poSelectedFriend->getMyAccessPermissionFromHim( ePluginTypeCamServer );
		strAction = GuiParams::describePluginAction( poSelectedFriend, ePluginTypeCamServer, ePluginAccess );
		addMenuItem( (int)ePluginTypeCamServer, getMyIcons().getIcon( getMyIcons().getPluginIcon( ePluginTypeCamServer, ePluginAccess ) ), strAction );
	}

	if( m_SelectedFriend->isMyAccessAllowedFromHim( ePluginTypeFileXfer ) )
	{
		ePluginAccess = poSelectedFriend->getMyAccessPermissionFromHim( ePluginTypeFileXfer );
		strAction = GuiParams::describePluginAction( poSelectedFriend, ePluginTypeFileXfer, ePluginAccess );
		addMenuItem( (int)ePluginTypeFileXfer, getMyIcons().getIcon( getMyIcons().getPluginIcon( ePluginTypeFileXfer, ePluginAccess ) ), strAction );
	}

	if( m_SelectedFriend->isMyAccessAllowedFromHim( ePluginTypeFileServer ) )
	{
		ePluginAccess = poSelectedFriend->getMyAccessPermissionFromHim( ePluginTypeFileServer );
		strAction = GuiParams::describePluginAction( poSelectedFriend, ePluginTypeFileServer, ePluginAccess );
		addMenuItem( (int)ePluginTypeFileServer, getMyIcons().getIcon( getMyIcons().getPluginIcon( ePluginTypeFileServer, ePluginAccess ) ), strAction );
	}

	if( m_SelectedFriend->isMyAccessAllowedFromHim( ePluginTypeRelay ) )
	{
		ePluginAccess = poSelectedFriend->getMyAccessPermissionFromHim( ePluginTypeRelay );
		strAction = GuiParams::describePluginAction( poSelectedFriend, ePluginTypeRelay, ePluginAccess );
		addMenuItem( (int)ePluginTypeRelay, getMyIcons().getIcon( getMyIcons().getPluginIcon( ePluginTypeRelay, ePluginAccess ) ), strAction );
	}
}

//============================================================================
//! user selected friend action
void AppletPopupMenu::onFriendActionSelected( int iMenuId )
{
	switch( iMenuId )
	{
	case ePluginTypeWebServer: // view profile
		if( m_SelectedFriend->isMyAccessAllowedFromHim( ePluginTypeWebServer ) )
		{
			m_MyApp.offerToFriendViewProfile( m_SelectedFriend );
		}

		break;

	case ePluginTypeStoryboard:
		if( m_SelectedFriend->isMyAccessAllowedFromHim( ePluginTypeStoryboard ) )
		{
			m_MyApp.offerToFriendViewStoryboard( m_SelectedFriend );
		}

		break;

	case ePluginTypeVideoPhone:
	case ePluginTypeVoicePhone:
	case ePluginTypeTruthOrDare:
	case ePluginTypeCamServer:
	case ePluginTypeMessenger:
		if( m_SelectedFriend->isMyAccessAllowedFromHim( (EPluginType)iMenuId ) )
		{
			m_MyApp.offerToFriendPluginSession( m_SelectedFriend, (EPluginType)iMenuId );
		}
		else
		{
			QString warnTitle = QObject::tr( "Insufficient Permission Level" );
			QString warmPermission = warnTitle + QObject::tr( " To Access Plugin " ) + DescribePluginType( (EPluginType)iMenuId );
			QMessageBox::warning( this, QObject::tr( "Insufficient Permission Level " ), warmPermission );
		}

		break;

	case ePluginTypeFileServer:
		if( m_SelectedFriend->isMyAccessAllowedFromHim( ePluginTypeFileServer ) )
		{
			m_MyApp.offerToFriendViewSharedFiles( m_SelectedFriend );
		}

		break;

	case ePluginTypeFileXfer:
		if( m_SelectedFriend->isMyAccessAllowedFromHim( ePluginTypeFileXfer ) )
		{
			m_MyApp.offerToFriendSendFile( m_SelectedFriend );
		}

		break;

	case ePluginTypeRelay:
		if( m_SelectedFriend->isMyAccessAllowedFromHim( ePluginTypeRelay ) )
		{
			// see if user wants to change his preferred proxy
			m_MyApp.offerToFriendUseAsRelay( m_SelectedFriend );
		}

		break;

	case eMaxPluginType + 1: // change friendship
	case eMaxPluginType + 2: // block / unblock
	{
		AppletPeerChangeFriendship * applet = dynamic_cast<AppletPeerChangeFriendship*>(m_MyApp.launchApplet( eAppletPeerChangeFriendship, getParentPageFrame() ));
		if( applet )
		{
			applet->setFriend( m_SelectedFriend );
		}
		break;
	}


	default:
		LogMsg( LOG_ERROR, "Unknown Menu id" );
	}

	close();
}

//============================================================================
void AppletPopupMenu::showHostSessionMenu( GuiHostSession* hostSession )
{
	if( !hostSession )
	{
		LogMsg( LOG_ERROR, "AppletPopupMenu::showHostSessionMenu null session" );
		return;
	}

	setMenuType( EPopupMenuType::ePopupMenuHostSession );
	m_HostSession = hostSession;

	setTitle( GuiParams::describeHostType( hostSession->getHostType() ) );
	if( hostSession->getHostType() == eHostTypeGroup )
	{
		addMenuItem( 0, getMyIcons().getIcon( eMyIconServiceHostGroup ), QObject::tr( "Join" ) );
	}
	else if( hostSession->getHostType() == eHostTypeChatRoom )
	{
		addMenuItem( 0, getMyIcons().getIcon( eMyIconServiceChatRoom ), QObject::tr( "Join" ) );
	}
	else if( hostSession->getHostType() == eHostTypeRandomConnect )
	{
		addMenuItem( 0, getMyIcons().getIcon( eMyIconServiceRandomConnect ), QObject::tr( "Join" ) );
	}

	addMenuItem( 1, getMyIcons().getIcon( eMyIconCancelNormal ), QObject::tr( "Unjoin" ) );
	addMenuItem( 2, getMyIcons().getIcon( eMyIconConnect ), QObject::tr( "Connect To Host" ) );
	addMenuItem( 3, getMyIcons().getIcon( eMyIconDisconnect ), QObject::tr( "Disconnect From Host" ) );
}

//============================================================================
void AppletPopupMenu::onHostSessionActionSelected( int iMenuId )
{
	switch( iMenuId )
	{
	case 0: // friends listing
		m_MyApp.getAppletMgr().launchApplet( eAppletFriendListClient, getParentPageFrame() );
		break;

	case 1: // group listing
		m_MyApp.getAppletMgr().launchApplet( eAppletGroupListClient, getParentPageFrame() );
		break;

	default:
		LogMsg( LOG_ERROR, "Unknown AppletPopupMenu::onTitleBarActionSelected value %d", iMenuId );
	}
}

//============================================================================
void AppletPopupMenu::showPersonOfferMenu( GuiUser* poSelectedFriend )
{
	m_SelectedFriend = poSelectedFriend;
	setMenuType( EPopupMenuType::ePopupMenuOfferFriendship );
	setTitle( QObject::tr( "Offer Friendship" ) );
	addMenuItem( 0, getMyIcons().getIcon( eMyIconFriend ), QObject::tr( "Offer Friendship" ) );
	addMenuItem( 1, getMyIcons().getIcon( eMyIconFriend ), QObject::tr( "Offer Join Group" ) );
}

//============================================================================
void AppletPopupMenu::onPersonActionSelected( int iMenuId )
{
	switch( iMenuId )
	{
	case 0: // friends listing
		m_MyApp.getAppletMgr().launchApplet( eAppletFriendListClient, getParentPageFrame() );
		break;

	case 1: // group listing
		m_MyApp.getAppletMgr().launchApplet( eAppletGroupListClient, getParentPageFrame() );
		break;

	default:
		LogMsg( LOG_ERROR, "Unknown AppletPopupMenu::onTitleBarActionSelected value %d", iMenuId );
	}
}

//============================================================================
void AppletPopupMenu::showUserSessionMenu( EApplet appletType, GuiUserSessionBase* userSession )
{
	if( !userSession )
	{
		LogMsg( LOG_ERROR, "AppletPopupMenu::showUserSessionMenu null session" );
		return;
	}

	m_AppletType = appletType;
	m_UserSession = userSession;
	setMenuType( EPopupMenuType::ePopupMenuUserSession );
	setTitle( QObject::tr( "Offer Friendship" ) );
	addMenuItem( 0, getMyIcons().getIcon( eMyIconFriend ), QObject::tr( "Offer Friendship" ) );
	addMenuItem( 1, getMyIcons().getIcon( eMyIconFriend ), QObject::tr( "Offer Join Group" ) );
}

//============================================================================
void AppletPopupMenu::onUserSessionActionSelected( int iMenuId )
{
	switch( iMenuId )
	{
	case 0: // friends listing
		m_MyApp.getAppletMgr().launchApplet( eAppletFriendListClient, getParentPageFrame() );
		break;

	case 1: // group listing
		m_MyApp.getAppletMgr().launchApplet( eAppletGroupListClient, getParentPageFrame() );
		break;

	default:
		LogMsg( LOG_ERROR, "Unknown AppletPopupMenu::onTitleBarActionSelected value %d", iMenuId );
	}
}
