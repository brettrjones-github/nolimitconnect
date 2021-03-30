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

#include <app_precompiled_hdr.h>
#include "UserListEntryWidget.h"
#include "GuiUserListWidget.h"
#include "GuiHostSession.h"

#include "MyIcons.h"
#include "PopupMenu.h"
#include "AppGlobals.h"
#include "AppCommon.h"
#include "GuiParams.h"

#include <CoreLib/VxDebug.h>
 
//============================================================================
GuiUserListWidget::GuiUserListWidget( QWidget * parent )
: QListWidget( parent )
, m_MyApp( GetAppInstance() )
, m_UserMgr( m_MyApp.getUserMgr() )
, m_Engine( m_MyApp.getEngine() )
{
	QListWidget::setSortingEnabled( true );
	sortItems( Qt::DescendingOrder );

    connect( &m_UserMgr, SIGNAL(signalUserAdded(GuiUser *)),	                this, SLOT(slotUserAdded(GuiUser *)) );
    connect( &m_UserMgr, SIGNAL(signalUserRemoved(VxGUID)),	                    this, SLOT(slotUserRemoved(VxGUID)) );
    connect( &m_UserMgr, SIGNAL(signalUserUpdated(GuiUser *)),	                this, SLOT(slotUserUpdated(GuiUser *)) );
    connect( &m_UserMgr, SIGNAL(signalUserOnlineStatus(GuiUser *, bool)),	    this, SLOT(slotUserOnlineStatus(GuiUser *, bool)) );

    connect( this, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(slotItemClicked(QListWidgetItem *))) ;
    connect( this, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(slotItemClicked(QListWidgetItem *))) ;
}

//============================================================================
UserListEntryWidget* GuiUserListWidget::sessionToWidget( GuiHostSession* hostSession )
{
    UserListEntryWidget* hostItem = new UserListEntryWidget(this);
    hostItem->setSizeHint( QSize( ( int )( GuiParams::getGuiScale() * 200 ),
        ( int )( 62 * GuiParams::getGuiScale() ) ) );

    hostItem->setHostSession( hostSession );

    connect( hostItem, SIGNAL(signalHostListItemClicked(QListWidgetItem *)),	    this, SLOT(slotHostListItemClicked(QListWidgetItem *)) );
    connect( hostItem, SIGNAL(signalIconButtonClicked(UserListEntryWidget *)),	    this, SLOT(slotIconButtonClicked(UserListEntryWidget *)) );
    connect( hostItem, SIGNAL(signalMenuButtonClicked(UserListEntryWidget *)),	    this, SLOT(slotMenuButtonClicked(UserListEntryWidget *)) );
    connect( hostItem, SIGNAL(signalJoinButtonClicked(UserListEntryWidget *)),		this, SLOT(slotJoinButtonClicked(UserListEntryWidget *)) );

    hostItem->updateWidgetFromInfo();

    return hostItem;
}

//============================================================================
bool GuiUserListWidget::isUserAListMatch( GuiUser* user )
{
    bool isMatch = false;
    if( user )
    {
        if( eUserListViewTypeFriends == m_ViewType )
        {
            if( user->isFriend() || user->isAdmin() )
            {
                isMatch = true;
            }
        }
        else if( eUserListViewTypeGroupHosted == m_ViewType )
        {
            if( user->isGroupHosted() )
            {
                isMatch = true;
            }
        }
        else if( eUserListViewTypeChatRoomHosted == m_ViewType )
        {
            if( user->isChatRoomHosted() )
            {
                isMatch = true;
            }
        }
        else if( eUserListViewTypeRandomConnectHosted == m_ViewType )
        {
            if( user->isRandomConnectHosted() )
            {
                isMatch = true;
            }
        }
    }

    return isMatch;
}

//============================================================================
void GuiUserListWidget::slotUserAdded( GuiUser* user )
{
    LogMsg( LOG_DEBUG, "GuiUserListWidget::slotUserAdded" );
    if( isUserAListMatch( user ) )
    {
        refreshUserList();
    }
}

//============================================================================
void GuiUserListWidget::slotUserRemoved( VxGUID onlineId )
{
    refreshUserList();
}

//============================================================================
void GuiUserListWidget::slotUserUpdated( GuiUser* user )
{
    LogMsg( LOG_DEBUG, "GuiUserListWidget::slotUserUpdated" );
    if( isUserAListMatch( user ) )
    {
        refreshUserList();
    }
}

//============================================================================
void GuiUserListWidget::slotUserOnlineStatus( GuiUser* user, bool isOnline )
{
    LogMsg( LOG_DEBUG, "GuiUserListWidget::slotUserOnlineStatus" );
    if( isUserAListMatch( user ) )
    {
        refreshUserList();
    }
}

//============================================================================
void GuiUserListWidget::showEvent( QShowEvent * ev )
{
    QListWidget::showEvent( ev );
    if( m_FirstShow )
    {
        m_FirstShow = false;
        setUserListViewType( eUserListViewTypeFriends );
    }
}

//============================================================================
void GuiUserListWidget::setUserListViewType( EUserListViewType viewType )
{
    if( viewType != m_ViewType )
    {
        m_ViewType = viewType;
        refreshUserList();
    }
}

//============================================================================
void GuiUserListWidget::refreshUserList( void )
{
    clearUserList();
    m_UserMgr.lockUserMgr();
    std::map<VxGUID, GuiUser*>& userList = m_UserMgr.getUserList();
    for( auto iter = userList.begin(); iter != userList.end(); ++iter )
    {
        if( isUserAListMatch( iter->second ) )
        {
            addUser( iter->second );
        }
    }

    m_UserMgr.unlockUserMgr();
    update();
}

//============================================================================
void GuiUserListWidget::addUser( GuiUser* user )
{
    LogMsg( LOG_DEBUG, "GuiUserListWidget::addUser" );
}

//============================================================================
GuiHostSession* GuiUserListWidget::widgetToSession( UserListEntryWidget * item )
{
    return item->getHostSession();
}

//============================================================================
MyIcons&  GuiUserListWidget::getMyIcons( void )
{
	return m_MyApp.getMyIcons();
}

//============================================================================
GuiHostSession * GuiUserListWidget::findSession( VxGUID& lclSessionId )
{
    int iCnt = count();
    for( int iRow = 0; iRow < iCnt; iRow++ )
    {
        UserListEntryWidget* listItem =  (UserListEntryWidget*)item( iRow );
        if( listItem )
        {
            GuiHostSession * hostSession = listItem->getHostSession();
            if( hostSession && hostSession->getSessionId() == lclSessionId )
            {
                return hostSession;
            }
        }
    }

    return nullptr;
}

//============================================================================
UserListEntryWidget* GuiUserListWidget::findListEntryWidgetBySessionId( VxGUID& sessionId )
{
    int iCnt = count();
    for( int iRow = 0; iRow < iCnt; iRow++ )
    {
        UserListEntryWidget*  hostItem = (UserListEntryWidget*)item( iRow );
        if( hostItem )
        {
            GuiHostSession * hostSession = hostItem->getHostSession();
            if( hostSession && ( hostSession->getSessionId() == sessionId ) )
            {
                return hostItem;
            }
        }
    }

    return nullptr;
}

//============================================================================
UserListEntryWidget* GuiUserListWidget::findListEntryWidgetByOnlineId( VxGUID& onlineId )
{
    int iCnt = count();
    for( int iRow = 0; iRow < iCnt; iRow++ )
    {
        UserListEntryWidget*  hostItem = (UserListEntryWidget*)item( iRow );
        if( hostItem )
        {
            GuiHostSession * hostSession = hostItem->getHostSession();
            if( hostSession && ( hostSession->getHostIdent().getMyOnlineId() == onlineId ) )
            {
                return hostItem;
            }
        }
    }

    return nullptr;
}

//============================================================================
void GuiUserListWidget::slotItemClicked( QListWidgetItem * item )
{
	if( 300 < m_ClickEventTimer.elapsedMs()  ) // avoid duplicate clicks
	{
		slotHostListItemClicked( (UserListEntryWidget *)item );
	}
}

//============================================================================
void GuiUserListWidget::slotHostListItemClicked( QListWidgetItem* hostItem )
{
	if( 300 > m_ClickEventTimer.elapsedMs()  ) // avoid duplicate clicks
	{
		return;
	}

	m_ClickEventTimer.startTimer();
    UserListEntryWidget* hostWidget = dynamic_cast<UserListEntryWidget*>(hostItem);
    if( hostWidget )
    {
        onHostListItemClicked(hostWidget);
    }
}

//============================================================================
void GuiUserListWidget::slotAvatarButtonClicked( UserListEntryWidget* hostItem )
{
    if( 300 > m_ClickEventTimer.elapsedMs() ) // avoid duplicate clicks
    {
        return;
    }

    m_ClickEventTimer.startTimer();
    slotAvatarButtonClicked(hostItem);
}

//============================================================================
void GuiUserListWidget::slotFriendshipButtonClicked( UserListEntryWidget* hostItem )
{
    if( 300 > m_ClickEventTimer.elapsedMs()  ) // avoid duplicate clicks
    {
        return;
    }

    m_ClickEventTimer.startTimer();
    onFriendshipButtonClicked( hostItem );
}

//============================================================================
void GuiUserListWidget::slotMenuButtonClicked( UserListEntryWidget* hostItem )
{
	if( 300 > m_ClickEventTimer.elapsedMs()  ) // avoid duplicate clicks
	{
		return;
	}

	m_ClickEventTimer.startTimer();
    onMenuButtonClicked( hostItem );
}

//============================================================================
void GuiUserListWidget::addHostAndSettingsToList( EHostType hostType, VxGUID& sessionId, VxNetIdent& hostIdent, PluginSetting& pluginSetting )
{
    GuiHostSession* hostSession = new GuiHostSession( hostType, sessionId, hostIdent, pluginSetting, this );

    addOrUpdateHostSession( hostSession );
}

//============================================================================
UserListEntryWidget* GuiUserListWidget::addOrUpdateHostSession( GuiHostSession* hostSession )
{
    UserListEntryWidget* hostItem = findListEntryWidgetBySessionId( hostSession->getSessionId() );
    if( hostItem )
    {
        GuiHostSession* hostOldSession = hostItem->getHostSession();
        if( hostOldSession != hostSession )
        {
            hostItem->setHostSession( hostSession );
            if( !hostOldSession->parent() )
            {
                delete hostOldSession;
            }
        }

        hostItem->updateWidgetFromInfo();
    }
    else
    {
        hostItem = sessionToWidget( hostSession );
        if( 0 == count() )
        {
            LogMsg( LOG_INFO, "add host %s\n", hostSession->getHostIdent().getOnlineName() );
            addItem( hostItem );
        }
        else
        {
            LogMsg( LOG_INFO, "insert host %s\n", hostSession->getHostIdent().getOnlineName() );
            insertItem( 0, (QListWidgetItem *)hostItem );
        }

        setItemWidget( (QListWidgetItem *)hostItem, (QWidget *)hostItem );
    }

    return hostItem;
}

//============================================================================
void GuiUserListWidget::clearUserList( void )
{
    for(int i = 0; i < count(); ++i)
    {
        QListWidgetItem* hostItem = item(i);

        delete ((UserListEntryWidget *)hostItem);
    }

    clear();
}

//============================================================================
void GuiUserListWidget::onHostListItemClicked( UserListEntryWidget* hostItem )
{
    LogMsg( LOG_DEBUG, "onHostListItemClicked" );
    onAvatarButtonClicked( hostItem );
}

//============================================================================
void GuiUserListWidget::onAvatarButtonClicked( UserListEntryWidget* hostItem )
{
    LogMsg( LOG_DEBUG, "onIconButtonClicked" );
    GuiHostSession* hostSession = hostItem->getHostSession();
    if( hostSession )
    {
        emit signalAvatarButtonClicked( hostSession, hostItem );
    }
}

//============================================================================
void GuiUserListWidget::onMenuButtonClicked( UserListEntryWidget* hostItem )
{
    LogMsg( LOG_DEBUG, "onMenuButtonClicked" );
    GuiHostSession* hostSession = hostItem->getHostSession();
    if( hostSession )
    {
        emit signalMenuButtonClicked( hostSession, hostItem );
    }

    /*
    m_SelectedFriend = widgetToHost( item );
    if( m_SelectedFriend )
    {
    emit signalFriendClicked( m_SelectedFriend );
    ActivityBase *activityBase = dynamic_cast< ActivityBase * >( this->parent() );
    if( activityBase )
    {
    PopupMenu popupMenu( m_MyApp, activityBase );
    popupMenu.setTitleBarWidget( activityBase->getTitleBarWidget() );
    popupMenu.setBottomBarWidget( activityBase->getBottomBarWidget() );
    connect( &popupMenu, SIGNAL( menuItemClicked( int, PopupMenu *, ActivityBase * ) ), &popupMenu, SLOT( onFriendActionSelected( int, PopupMenu *, ActivityBase * ) ) );

    popupMenu.showFriendMenu( m_SelectedFriend );
    }
    }
    */
}

//============================================================================
void GuiUserListWidget::onFriendshipButtonClicked( UserListEntryWidget* hostItem )
{
    LogMsg( LOG_DEBUG, "onFriendshipButtonClicked" );
    GuiHostSession* hostSession = hostItem->getHostSession();
    if( hostSession )
    {
        emit signalFriendshipButtonClicked( hostSession, hostItem );
    }
}