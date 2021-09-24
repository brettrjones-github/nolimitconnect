//============================================================================
// Copyright (C) 2010 Brett R. Jones
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

#include <app_precompiled_hdr.h>
#include "PopupMenu.h"
#include "MyIcons.h"
#include "GuiParams.h"
#include "AppGlobals.h"
#include "AppCommon.h"

#include <PktLib/VxCommon.h>
#include <CoreLib/VxDebug.h>

//============================================================================
PopupMenu::PopupMenu( AppCommon& app, ActivityBase * parent )
: ActivityBase( OBJNAME_POPUP_MENU, app, parent, eAppletMessengerFrame, true, false )
, m_ParentActivity( parent )
, m_iMenuItemHeight( GuiParams::getButtonSize().height() )
{
    vx_assert( parent );
    QFrame * contentFrame = dynamic_cast< QFrame * >( parent );
    if( contentFrame )
    {
        setContentItemsFrame( contentFrame );
    }

	m_MyApp.playSound( eSndDefButtonClick );
	ui.setupUi( this );

    connect(ui.exitPopupButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui.menuItemList, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(itemClicked(QListWidgetItem *)));
}

//============================================================================
void PopupMenu::setTitle( QString strTitle )
{
	ui.popupTitleLabel->setText(strTitle);
}

//============================================================================
void PopupMenu::addMenuItem( int iItemId, QIcon& oIcon, QString strMenuItemText )
{
	QListWidgetItem * poMenuItem = new QListWidgetItem( strMenuItemText );
	poMenuItem->setIcon( oIcon );
	poMenuItem->setData( Qt::UserRole, iItemId );
	poMenuItem->setSizeHint( QSize( 200, GuiParams::getButtonSize( eButtonSizeMedium ).height() + 4 ) );
	ui.menuItemList->addItem( poMenuItem );
}

//============================================================================
//! clicked the upper right x button
void PopupMenu::slotHomeButtonClicked( void )
{
	//m_MyApp.playSound( eSndDefButtonClick );
	//close();
	emit signalBackButtonClicked();
}

//============================================================================
void PopupMenu::itemClicked(QListWidgetItem *item)
{
	m_MyApp.playSound( eSndDefButtonClick );
	// resignal with id set by user
	int iItemId = item->data(Qt::UserRole).toInt();
	emit menuItemClicked( iItemId, this, m_ParentActivity );
	close();
}

//============================================================================
void PopupMenu::showRefreshMenu()
{
	setTitle( QObject::tr("Who to show in list") );
	addMenuItem( 0, getMyIcons().getIcon(eMyIconFriend),        QObject::tr("Everybody"));
	addMenuItem( 1, getMyIcons().getIcon(eMyIconFriend),        QObject::tr("Friends And Guests"));
	addMenuItem( 2, getMyIcons().getIcon(eMyIconAnonymous),     QObject::tr("Anonymous"));
	addMenuItem( 3, getMyIcons().getIcon(eMyIconAdministrator), QObject::tr("Administrators"));
	addMenuItem( 4, getMyIcons().getIcon(eMyIconIgnored),       QObject::tr("Ignored"));
	addMenuItem( 5, getMyIcons().getIcon(eMyIconRelay),         QObject::tr("My Relays"));
	addMenuItem( 6, getMyIcons().getIcon(eMyIconRelay),         QObject::tr("All Possible Relays"));
	addMenuItem( 7, getMyIcons().getIcon(eMyIconRefreshNormal), QObject::tr("Refresh List"));
	exec();
}

//============================================================================
void PopupMenu::showAppSystemMenu( void )
{
    setTitle( QObject::tr( "System Menu" ) );
    addMenuItem( 0, getMyIcons().getIcon( eMyIconDebug ), QObject::tr( "Debug Settings" ) );
    addMenuItem( 1, getMyIcons().getIcon( eMyIconDebug ), QObject::tr( "Debug Long" ) );
    exec();
}

//============================================================================
void PopupMenu::showFriendMenu( GuiUser * poSelectedFriend )
{
	m_SelectedFriend = poSelectedFriend;
	// populate title
	QString strTitle = poSelectedFriend->describeMyFriendshipToHim();
	strTitle += ": ";
	strTitle += poSelectedFriend->getOnlineName();
	setTitle(strTitle );
	// populate menu
	EPluginAccess ePluginAccess;
	QString strAction;

	addMenuItem( (int)eMaxPluginType + 1, getMyIcons().getIcon(eMyIconPermissions), QObject::tr( "Change Friendship") );

	ePluginAccess = poSelectedFriend->getMyAccessPermissionFromHim( ePluginTypeWebServer );
	strAction = GuiParams::describePluginAction( poSelectedFriend, ePluginTypeWebServer, ePluginAccess );
	addMenuItem( (int)ePluginTypeWebServer, getMyIcons().getIcon(getMyIcons().getPluginIcon( ePluginTypeWebServer, ePluginAccess )), strAction );

	ePluginAccess = poSelectedFriend->getMyAccessPermissionFromHim( ePluginTypeStoryboard );
	strAction = GuiParams::describePluginAction( poSelectedFriend, ePluginTypeStoryboard, ePluginAccess );
	addMenuItem( (int)ePluginTypeStoryboard, getMyIcons().getIcon(getMyIcons().getPluginIcon( ePluginTypeStoryboard, ePluginAccess ) ), strAction );

	ePluginAccess = poSelectedFriend->getMyAccessPermissionFromHim( ePluginTypeMessenger );
	strAction = GuiParams::describePluginAction( poSelectedFriend, ePluginTypeMessenger, ePluginAccess );
	addMenuItem( (int)ePluginTypeMessenger, getMyIcons().getIcon(getMyIcons().getPluginIcon( ePluginTypeMessenger, ePluginAccess ) ), strAction );

	ePluginAccess = poSelectedFriend->getMyAccessPermissionFromHim(ePluginTypeVideoPhone);
	strAction = GuiParams::describePluginAction( poSelectedFriend, ePluginTypeVideoPhone, ePluginAccess );
	addMenuItem( (int)ePluginTypeVideoPhone, getMyIcons().getIcon(getMyIcons().getPluginIcon( ePluginTypeVideoPhone, ePluginAccess ) ), strAction );

	ePluginAccess = poSelectedFriend->getMyAccessPermissionFromHim(ePluginTypeVoicePhone);
	strAction = GuiParams::describePluginAction( poSelectedFriend, ePluginTypeVoicePhone, ePluginAccess );
	addMenuItem( (int)ePluginTypeVoicePhone, getMyIcons().getIcon(getMyIcons().getPluginIcon( ePluginTypeVoicePhone, ePluginAccess ) ), strAction );

	ePluginAccess = poSelectedFriend->getMyAccessPermissionFromHim(ePluginTypeTruthOrDare);
	strAction = GuiParams::describePluginAction( poSelectedFriend, ePluginTypeTruthOrDare, ePluginAccess );
	addMenuItem( (int)ePluginTypeTruthOrDare, getMyIcons().getIcon(getMyIcons().getPluginIcon( ePluginTypeTruthOrDare, ePluginAccess ) ), strAction );

	ePluginAccess = poSelectedFriend->getMyAccessPermissionFromHim(ePluginTypeCamServer);
	strAction = GuiParams::describePluginAction( poSelectedFriend, ePluginTypeCamServer, ePluginAccess );
	addMenuItem( (int)ePluginTypeCamServer, getMyIcons().getIcon(getMyIcons().getPluginIcon( ePluginTypeCamServer, ePluginAccess ) ), strAction );

	ePluginAccess = poSelectedFriend->getMyAccessPermissionFromHim(ePluginTypeFileXfer);
	strAction = GuiParams::describePluginAction( poSelectedFriend, ePluginTypeFileXfer, ePluginAccess );
	addMenuItem( (int)ePluginTypeFileXfer, getMyIcons().getIcon(getMyIcons().getPluginIcon( ePluginTypeFileXfer, ePluginAccess ) ), strAction );

	ePluginAccess = poSelectedFriend->getMyAccessPermissionFromHim(ePluginTypeFileServer);
	strAction = GuiParams::describePluginAction( poSelectedFriend, ePluginTypeFileServer, ePluginAccess );
	addMenuItem( (int)ePluginTypeFileServer, getMyIcons().getIcon(getMyIcons().getPluginIcon(ePluginTypeFileServer, ePluginAccess) ), strAction );

	ePluginAccess = poSelectedFriend->getMyAccessPermissionFromHim(ePluginTypeRelay);
	strAction = GuiParams::describePluginAction( poSelectedFriend, ePluginTypeRelay, ePluginAccess );
	addMenuItem( (int)ePluginTypeRelay, getMyIcons().getIcon(getMyIcons().getPluginIcon(ePluginTypeRelay, ePluginAccess ) ), strAction );

	exec();
}

//============================================================================
//! user selected friend action
void PopupMenu::onFriendActionSelected( int iMenuId, PopupMenu *, ActivityBase * )
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
			QString warnTitle = QObject::tr("Insufficient Permission Level");
			QString warmPermission = warnTitle + QObject::tr(" To Access Plugin ") + DescribePluginType( (EPluginType)iMenuId );
			QMessageBox::warning(this, QObject::tr("Insufficient Permission Level "), warmPermission );
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
		m_MyApp.offerToFriendChangeFriendship( m_SelectedFriend );
		break;

	default:
		LogMsg( LOG_ERROR, "Unknown Menu id\n");
	}

	close();
}


