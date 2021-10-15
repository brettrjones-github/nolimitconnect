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
#include "UserListItem.h"
#include "GuiUser.h"
#include "GuiUserListWidget.h"
#include "GuiUserSessionBase.h"

#include "MyIcons.h"
#include "AppletPopupMenu.h"
#include "AppGlobals.h"
#include "AppCommon.h"
#include "GuiParams.h"

#include <CoreLib/VxDebug.h>

#include <vector>
 
//============================================================================
GuiUserListWidget::GuiUserListWidget( QWidget * parent )
: ListWidgetBase( parent )
{
	QListWidget::setSortingEnabled( true );
	sortItems( Qt::DescendingOrder );
    setShowMyself( true );

    connect( &m_UserMgr, SIGNAL(signalMyIdentUpdated(GuiUser *)),	            this, SLOT(slotMyIdentUpdated(GuiUser *)) );

    connect( &m_UserMgr, SIGNAL(signalUserAdded(GuiUser *)),	                this, SLOT(slotUserAdded(GuiUser *)) );
    connect( &m_UserMgr, SIGNAL(signalUserRemoved(VxGUID)),	                    this, SLOT(slotUserRemoved(VxGUID)) );
    connect( &m_UserMgr, SIGNAL(signalUserUpdated(GuiUser *)),	                this, SLOT(slotUserUpdated(GuiUser *)) );
    connect( &m_UserMgr, SIGNAL(signalUserOnlineStatus(GuiUser *, bool)),	    this, SLOT(slotUserOnlineStatus(GuiUser *, bool)) );

    connect( this, SIGNAL(itemClicked(QListWidgetItem *)),                      this, SLOT(slotItemClicked(QListWidgetItem *))) ;
    connect( this, SIGNAL(itemDoubleClicked(QListWidgetItem *)),                this, SLOT(slotItemClicked(QListWidgetItem *))) ;
}

//============================================================================
bool GuiUserListWidget::isUserAListMatch( GuiUser* user )
{
    bool isMatch = false;
    if( user )
    {
        // BRJ temp for debugging
        if( user->isMyself() && getShowMyself())
        {
            return true;
        }


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
void GuiUserListWidget::slotMyIdentUpdated( GuiUser* user )
{
    if( getShowMyself() && user )
    {
        slotUserUpdated( user );
    }
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
    std::vector<GuiUser*> updateUserList;
    std::map<VxGUID, GuiUser*>& userList = m_UserMgr.getUserList();
    for( auto iter = userList.begin(); iter != userList.end(); ++iter )
    {
        if( isUserAListMatch( iter->second ) )
        {
            updateUserList.push_back( iter->second );
        }
    }

    if( getShowMyself() )
    {
        if( m_UserMgr.getMyIdent() )
        {
            updateUserList.push_back( m_UserMgr.getMyIdent() );
        }
    }

    for( auto user : updateUserList )
    {
        updateUser( user );
    }

    update();
}

//============================================================================
UserListItem* GuiUserListWidget::sessionToWidget( GuiUserSessionBase* userSession )
{
    UserListItem* userItem = new UserListItem(this);
    userItem->setSizeHint( userItem->calculateSizeHint() );

    userItem->setUserSession( userSession );

    connect( userItem, SIGNAL(signalUserListItemClicked(UserListItem *)),	    this, SLOT(slotUserListItemClicked(UserListItem *)) );
    connect( userItem, SIGNAL(signalAvatarButtonClicked(UserListItem *)),	    this, SLOT(slotAvatarButtonClicked(UserListItem *)) );
    connect( userItem, SIGNAL(signalMenuButtonClicked(UserListItem *)),	        this, SLOT(slotMenuButtonClicked(UserListItem *)) );
    connect( userItem, SIGNAL(signalFriendshipButtonClicked(UserListItem *)),	this, SLOT(slotFriendshipButtonClicked(UserListItem *)) );

    userItem->updateWidgetFromInfo();

    return userItem;
}

//============================================================================
GuiUserSessionBase* GuiUserListWidget::widgetToSession( UserListItem * item )
{
    return item->getUserSession();
}

//============================================================================
void GuiUserListWidget::updateUser( GuiUser* user )
{
    if( user )
    {
        GuiUserSessionBase* userSession = nullptr;
        auto iter = m_UserCache.find( user->getMyOnlineId() );
        if( iter == m_UserCache.end() )
        {
            LogMsg( LOG_DEBUG, "GuiUserListWidget::updateUser new user %s", user->getOnlineName() );
            userSession = makeSession( user );
            if( userSession )
            {
                m_UserCache[user->getMyOnlineId()] = userSession;
                UserListItem* entryWidget = sessionToWidget( userSession );
                if( 0 == count() )
                {
                    LogMsg( LOG_INFO, "add host %s\n", user->getOnlineName() );
                    addItem( entryWidget );
                }
                else
                {
                    LogMsg( LOG_INFO, "insert host %s\n", user->getOnlineName() );
                    insertItem( 0, (QListWidgetItem *)entryWidget );
                }

                setItemWidget( (QListWidgetItem *)entryWidget, (QWidget *)entryWidget );
            }
            else
            {
                LogMsg( LOG_ERROR, "GuiUserListWidget::updateUser failed create session for user %s", user->getOnlineName() );
            }
        }
        else
        {
            updateEntryWidget( user->getMyOnlineId() );
        }
    }
}

//============================================================================
void GuiUserListWidget::updateEntryWidget( VxGUID& onlineId )
{
    UserListItem* entryWidget = findListEntryWidgetByOnlineId( onlineId );
    if( entryWidget )
    {
        entryWidget->update();
    }
    else
    {
        LogMsg( LOG_ERROR, "GuiUserListWidget::updateUser failed to find session for user" );
    }
}

//============================================================================
GuiUserSessionBase* GuiUserListWidget::makeSession( GuiUser* user )
{
    return new GuiUserSessionBase( user, this );
}

//============================================================================
void GuiUserListWidget::removeUser( VxGUID& onlineId )
{
    auto iter = m_UserCache.find( onlineId );
    if( iter != m_UserCache.end() )
    {
        LogMsg( LOG_DEBUG, "GuiUserListWidget::removeUser %s", iter->second->getOnlineName() );
        m_UserCache.erase( iter );

        UserListItem* userItem = findListEntryWidgetByOnlineId( onlineId );
        if( userItem )
        {
            delete userItem->getUserSession();
            QListWidgetItem* listItem = dynamic_cast<QListWidgetItem*>( userItem );
            removeItemWidget(listItem);
            delete listItem; // Qt documentation warnings you to destroy item to effectively remove it from QListWidget.
        }  
    }
}

//============================================================================
GuiUserSessionBase * GuiUserListWidget::findSession( VxGUID& lclSessionId )
{
    int iCnt = count();
    for( int iRow = 0; iRow < iCnt; iRow++ )
    {
        UserListItem* listItem =  (UserListItem*)item( iRow );
        if( listItem )
        {
            GuiUserSessionBase * userSession = listItem->getUserSession();
            if( userSession && userSession->getSessionId() == lclSessionId )
            {
                return userSession;
            }
        }
    }

    return nullptr;
}

//============================================================================
UserListItem* GuiUserListWidget::findListEntryWidgetBySessionId( VxGUID& sessionId )
{
    int iCnt = count();
    for( int iRow = 0; iRow < iCnt; iRow++ )
    {
        UserListItem*  userItem = (UserListItem*)item( iRow );
        if( userItem )
        {
            GuiUserSessionBase * userSession = userItem->getUserSession();
            if( userSession && ( userSession->getSessionId() == sessionId ) )
            {
                return userItem;
            }
        }
    }

    return nullptr;
}

//============================================================================
UserListItem* GuiUserListWidget::findListEntryWidgetByOnlineId( VxGUID& onlineId )
{
    int iCnt = count();
    for( int iRow = 0; iRow < iCnt; iRow++ )
    {
        UserListItem*  userItem = (UserListItem*)item( iRow );
        if( userItem )
        {
            GuiUserSessionBase * userSession = userItem->getUserSession();
            if( userSession && ( userSession->getMyOnlineId() == onlineId ) )
            {
                return userItem;
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
        return;
	}

    m_ClickEventTimer.startTimer();
    onUserListItemClicked( dynamic_cast<UserListItem *>(item) );
}

//============================================================================
void GuiUserListWidget::slotUserListItemClicked( UserListItem* userItem )
{
    if( 300 > m_ClickEventTimer.elapsedMs() ) // avoid duplicate clicks
    {
        return;
    }

    m_ClickEventTimer.startTimer();
    onUserListItemClicked( userItem );
}

//============================================================================
void GuiUserListWidget::slotAvatarButtonClicked( UserListItem* userItem )
{
    if( 300 > m_ClickEventTimer.elapsedMs() ) // avoid duplicate clicks
    {
        return;
    }

    m_ClickEventTimer.startTimer();
    onAvatarButtonClicked(userItem);
}

//============================================================================
void GuiUserListWidget::slotFriendshipButtonClicked( UserListItem* userItem )
{
    if( 300 > m_ClickEventTimer.elapsedMs()  ) // avoid duplicate clicks
    {
        return;
    }

    m_ClickEventTimer.startTimer();
    onFriendshipButtonClicked( userItem );
}

//============================================================================
void GuiUserListWidget::slotMenuButtonClicked( UserListItem* userItem )
{
	if( 300 > m_ClickEventTimer.elapsedMs()  ) // avoid duplicate clicks
	{
		return;
	}

	m_ClickEventTimer.startTimer();
    onMenuButtonClicked( userItem );
}

//============================================================================
void GuiUserListWidget::addSessionToList( EHostType hostType, VxGUID& sessionId, GuiUser* hostIdent )
{
    GuiUserSessionBase* userSession = new GuiUserSessionBase( hostType, sessionId, hostIdent, this );

    addOrUpdateSession( userSession );
}

//============================================================================
UserListItem* GuiUserListWidget::addOrUpdateSession( GuiUserSessionBase* userSession )
{
    UserListItem* userItem = findListEntryWidgetBySessionId( userSession->getSessionId() );
    if( userItem )
    {
        GuiUserSessionBase* hostOldSession = userItem->getUserSession();
        if( hostOldSession != userSession )
        {
            userItem->setUserSession( userSession );
            if( !hostOldSession->parent() )
            {
                delete hostOldSession;
            }
        }

        userItem->updateWidgetFromInfo();
    }
    else
    {
        userItem = sessionToWidget( userSession );
        if( 0 == count() )
        {
            LogMsg( LOG_INFO, "add user %s\n", userSession->getUserIdent()->getOnlineName() );
            addItem( userItem );
        }
        else
        {
            LogMsg( LOG_INFO, "insert user %s\n", userSession->getUserIdent()->getOnlineName() );
            insertItem( 0, (QListWidgetItem *)userItem );
        }

        setItemWidget( (QListWidgetItem *)userItem, (QWidget *)userItem );
    }

    return userItem;
}

//============================================================================
void GuiUserListWidget::clearUserList( void )
{
    m_UserCache.clear();
    for(int i = 0; i < count(); ++i)
    {
        QListWidgetItem* userItem = item(i);

        delete ((UserListItem *)userItem);
    }

    clear();
}


//============================================================================
void GuiUserListWidget::onUserListItemClicked( UserListItem* userItem )
{
    LogMsg( LOG_DEBUG, "onHostListItemClicked" );
    if( userItem )
    {
        GuiUserSessionBase* userSession = userItem->getUserSession();
        if( userSession )
        {
            emit signalUserListItemClicked( userSession, userItem );
        }
    }
}

//============================================================================
void GuiUserListWidget::onAvatarButtonClicked( UserListItem* userItem )
{
    LogMsg( LOG_DEBUG, "onAvatarButtonClicked" );
    if( userItem )
    {
        GuiUserSessionBase* userSession = userItem->getUserSession();
        if( userSession )
        {
            emit signalAvatarButtonClicked( userSession, userItem );
        }
    }
}

//============================================================================
void GuiUserListWidget::onFriendshipButtonClicked( UserListItem* userItem )
{
    LogMsg( LOG_DEBUG, "onFriendshipButtonClicked" );
    if( userItem )
    {
        GuiUserSessionBase* userSession = userItem->getUserSession();
        if( userSession && userSession->getUserIdent() )
        {
            launchChangeFriendship( userSession->getUserIdent() );
        }
    }
}
//============================================================================
void GuiUserListWidget::onMenuButtonClicked( UserListItem* userItem )
{
    LogMsg( LOG_DEBUG, "onMenuButtonClicked" );
    if( userItem )
    {
        GuiUserSessionBase* userSession = userItem->getUserSession();
        if( userSession )
        {
            emit signalMenuButtonClicked( userSession, userItem );
        }
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
