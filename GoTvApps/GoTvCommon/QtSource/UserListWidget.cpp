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
#include "UserListWidget.h"
#include "GuiUserMgr.h"
#include "GuiUserSessionBase.h"

#include "MyIcons.h"
#include "AppletPopupMenu.h"
#include "AppGlobals.h"
#include "AppCommon.h"
#include "GuiParams.h"

#include <CoreLib/VxDebug.h>
 
//============================================================================
UserListWidget::UserListWidget( QWidget * parent )
: QListWidget( parent )
, m_MyApp( GetAppInstance() )
, m_Engine( m_MyApp.getEngine() )
, m_UserMgr( m_MyApp.getUserMgr() )
, m_ThumbMgr( m_MyApp.getThumbMgr() )
{
	QListWidget::setSortingEnabled( true );
	sortItems( Qt::DescendingOrder );

    connect( &m_UserMgr, SIGNAL(signalMyIdentUpdated( GuiUser*)),           this, SLOT(slotMyIdentUpdated( GuiUser*))) ;

    connect( &m_UserMgr, SIGNAL(signalUserAdded( GuiUser*)),                this, SLOT(slotUserAdded( GuiUser*))) ;
    connect( &m_UserMgr, SIGNAL(signalUserRemoved( VxGUID)),                this, SLOT(slotUserRemoved( VxGUID))) ;
    connect( &m_UserMgr, SIGNAL(signalUserUpdated( GuiUser*)),              this, SLOT(slotUserUpdated( GuiUser*))) ;
    connect( &m_UserMgr, SIGNAL(signalUserOnlineStatus( GuiUser*,bool)),    this, SLOT(slotUserOnlineStatus( GuiUser*,bool))) ;

    connect( &m_ThumbMgr, SIGNAL( signalThumbAdded( GuiThumb* ) ),          this, SLOT( slotThumbAdded( GuiThumb* ) ) );
    connect( &m_ThumbMgr, SIGNAL(signalThumbUpdated( GuiThumb*)),           this, SLOT(slotThumbUpdated( GuiThumb*))) ;
    connect( &m_ThumbMgr, SIGNAL(signalThumbRemoved( VxGUID)),              this, SLOT(slotThumbRemoved( VxGUID))) ;


    //connect( this, SIGNAL(itemClicked(QListWidgetItem *)),          this, SLOT(slotItemClicked(QListWidgetItem *))) ;
    //connect( this, SIGNAL(itemDoubleClicked(QListWidgetItem *)),    this, SLOT(slotItemClicked(QListWidgetItem *))) ;

    setUserViewType( eUserViewTypeEverybody );
}

//============================================================================
UserListItem* UserListWidget::sessionToWidget( GuiUserSessionBase* userSession )
{
    UserListItem* userItem = new UserListItem(this);
    userItem->setUserSession( userSession );
    userItem->setSizeHint( userItem->calculateSizeHint() );

    connect( userItem, SIGNAL(signalUserListItemClicked(QListWidgetItem *)),	    this, SLOT(slotUserListItemClicked(QListWidgetItem *)) );
    connect( userItem, SIGNAL(signalAvatarButtonClicked(UserListItem *)),	        this, SLOT(slotAvatarButtonClicked(UserListItem *)) );
    connect( userItem, SIGNAL(signalMenuButtonClicked(UserListItem *)),	            this, SLOT(slotMenuButtonClicked(UserListItem *)) );
    connect( userItem, SIGNAL(signalFriendshipButtonClicked(UserListItem *)),		this, SLOT(slotFriendshipButtonClicked(UserListItem *)) );

    userItem->updateWidgetFromInfo();

    return userItem;
}

//============================================================================
GuiUserSessionBase* UserListWidget::widgetToSession( UserListItem * item )
{
    return item->getUserSession();
}

//============================================================================
MyIcons&  UserListWidget::getMyIcons( void )
{
	return m_MyApp.getMyIcons();
}

//============================================================================
GuiUserSessionBase * UserListWidget::findSession( VxGUID& lclSessionId )
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
UserListItem* UserListWidget::findListEntryWidgetBySessionId( VxGUID& sessionId )
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
UserListItem* UserListWidget::findListEntryWidgetByOnlineId( VxGUID& onlineId )
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
void UserListWidget::slotUserListItemClicked( UserListItem* userItem )
{
	if( 300 > m_ClickEventTimer.elapsedMs()  ) // avoid duplicate clicks
	{
		return;
	}

	m_ClickEventTimer.startTimer();
    onUserListItemClicked(userItem);
}

//============================================================================
void UserListWidget::slotAvatarButtonClicked( UserListItem* userItem )
{
    if( 300 > m_ClickEventTimer.elapsedMs() ) // avoid duplicate clicks
    {
        return;
    }

    m_ClickEventTimer.startTimer();
    onAvatarButtonClicked(userItem);
}

//============================================================================
void UserListWidget::slotMenuButtonClicked( UserListItem* userItem )
{
	if( 300 > m_ClickEventTimer.elapsedMs()  ) // avoid duplicate clicks
	{
		return;
	}

	m_ClickEventTimer.startTimer();
    onMenuButtonClicked( userItem );
}

//============================================================================
void UserListWidget::slotFriendshipButtonClicked( UserListItem* userItem )
{
    if( 300 > m_ClickEventTimer.elapsedMs()  ) // avoid duplicate clicks
    {
        return;
    }

    m_ClickEventTimer.startTimer();
    onFriendshipButtonClicked( userItem );
}

//============================================================================
void UserListWidget::addUserToList( EHostType userType, VxGUID& sessionId, GuiUser * userIdent )
{
    GuiUserSessionBase* userSession = new GuiUserSessionBase( userType, sessionId, userIdent, this );

    addOrUpdateUserSession( userSession );
}

//============================================================================
UserListItem* UserListWidget::addOrUpdateUserSession( GuiUserSessionBase* userSession )
{
    UserListItem* userItem = findListEntryWidgetBySessionId( userSession->getSessionId() );
    if( userItem )
    {
        GuiUserSessionBase* userOldSession = userItem->getUserSession();
        if( userOldSession != userSession )
        {
            userItem->setUserSession( userSession );
            if( !userOldSession->parent() )
            {
                delete userOldSession;
            }
        }

        userItem->updateWidgetFromInfo();
    }
    else
    {
        userItem = sessionToWidget( userSession );
        if( 0 == count() )
        {
            LogMsg( LOG_INFO, "add user %s\n", userSession->getOnlineName() );
            addItem( userItem );
        }
        else
        {
            LogMsg( LOG_INFO, "insert user %s\n", userSession->getOnlineName() );
            insertItem( 0, (QListWidgetItem *)userItem );
        }

        setItemWidget( (QListWidgetItem *)userItem, (QWidget *)userItem );
        onListItemAdded( userSession, userItem );
    }

    return userItem;
}

//============================================================================
void UserListWidget::onUserListItemClicked( UserListItem* userItem )
{
    LogMsg( LOG_DEBUG, "onUserListItemClicked" );
    if( userItem )
    {
        GuiUserSessionBase* userSession = userItem->getUserSession();
        if( userSession )
        {
            emit signalUserListItemClicked( userSession, userItem );
        }
    }
    onUserListItemClicked( userItem );
}

//============================================================================
void UserListWidget::onAvatarButtonClicked( UserListItem* userItem )
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
void UserListWidget::onMenuButtonClicked( UserListItem* userItem )
{
    LogMsg( LOG_DEBUG, " UserListWidget::onMenuButtonClicked" );
    if( userItem )
    {
        GuiUserSessionBase* userSession = userItem->getUserSession();
        if( userSession )
        {
            // emit signalMenuButtonClicked( userSession, userItem );
            AppletPopupMenu* popupMenu = dynamic_cast<AppletPopupMenu*>(m_MyApp.launchApplet( eAppletPopupMenu, dynamic_cast<QWidget*>(this->parent()) ));
            // emit signalFriendClicked( m_SelectedFriend );
            if( popupMenu )
            {
                if( getAppletType() == eAppletMultiMessenger )
                {
                    popupMenu->showFriendMenu( userSession->getUserIdent() );
                }
                else
                {
                    popupMenu->showUserSessionMenu( getAppletType(), userSession );
                }            
            }
        }
    }


    /*
    m_SelectedFriend = widgetToUser( item );
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
void UserListWidget::onFriendshipButtonClicked( UserListItem* userItem )
{
    LogMsg( LOG_DEBUG, "onFriendshipButtonClicked" );
    if( userItem )
    {
        GuiUserSessionBase* userSession = userItem->getUserSession();
        if( userSession )
        {
            emit signalFriendshipButtonClicked( userSession, userItem );
        }
    }
}

//============================================================================
void UserListWidget::slotMyIdentUpdated( GuiUser* user )
{
    if( getShowMyself() )
    {
        updateUser( user );
    }
}

//============================================================================
void UserListWidget::slotUserAdded( GuiUser* user )
{
    updateUser( user );
}

//============================================================================
void UserListWidget::slotUserRemoved( VxGUID onlineId )
{
    removeUser( onlineId );
}

//============================================================================
void UserListWidget::slotUserUpdated( GuiUser* user )
{
    updateUser( user );
}

//============================================================================
void UserListWidget::slotUserOnlineStatus( GuiUser* user, bool isOnline )
{
    updateUser( user );
}

//============================================================================
void UserListWidget::slotThumbAdded( GuiThumb* thumb )
{
    updateThumb( thumb );
}

//============================================================================
void UserListWidget::slotThumbUpdated( GuiThumb* thumb )
{
    updateThumb( thumb );
}

//============================================================================
void UserListWidget::slotThumbRemoved( VxGUID thumbId )
{
    // TODO
}

//============================================================================
void UserListWidget::setUserViewType( EUserViewType viewType )
{
    if( viewType != m_UserViewType )
    {
        m_UserViewType = viewType;
        refreshList();
    }
}

//============================================================================
void UserListWidget::refreshList( void )
{
    clearUserList();
    std::vector<GuiUser *> userList;
    m_UserMgr.lockUserMgr();

    if( isListViewMatch( m_UserMgr.getMyIdent() ) )
    {
        userList.push_back( m_UserMgr.getMyIdent() );
    }

    std::map<VxGUID, GuiUser*>& mgrList = m_UserMgr.getUserList();
    for( auto iter = mgrList.begin(); iter != mgrList.end(); ++iter )
    {
        if( isListViewMatch( iter->second ) )
        {
            userList.push_back( iter->second );
        }
    }

    m_UserMgr.unlockUserMgr();
    for( auto user : userList )
    {
        updateUser( user );
    }
}


//============================================================================
void UserListWidget::clearUserList( void )
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
bool UserListWidget::isListViewMatch( GuiUser * user )
{
    if( user && !user->isIgnored())
    {
        if( user->isMyself() )
        { 
            return getShowMyself();
        }
        else if( eUserViewTypeEverybody == getUserViewType() )
        {
            return true;
        }
        else if( eUserViewTypeFriends == getUserViewType() )
        {
            return user->isFriend() || user->isAdmin();
        }
        else if( eUserViewTypeGroup == getUserViewType() )
        {
            return user->isGroupHosted() && !user->isAnonymous();
        }
        else if( eUserViewTypeChatRoom == getUserViewType() )
        {
            return user->isChatRoomHosted() && !user->isAnonymous();
        }
        else if( eUserViewTypeRandomConnect == getUserViewType() )
        {
            return user->isRandomConnectHosted() && !user->isAnonymous();
        }

        if( user->isPeerHosted() && user->isFriend() )
        {
            return true;
        }
    }
    else if( user && user->isIgnored() && eUserViewTypeIgnored == getUserViewType() )
    {
        return true;
    }

    return false;
}

//============================================================================
void UserListWidget::updateUser( GuiUser * user )
{
    if( isListViewMatch( user ) )
    {
        auto iter = m_UserCache.find( user->getMyOnlineId() );
        if( iter == m_UserCache.end() )
        {
            GuiUserSessionBase * userSession = new GuiUserSessionBase( user, this );
            if( userSession )
            {
                UserListItem* userItem = sessionToWidget( userSession );
                if( 0 == count() )
                {
                    LogMsg( LOG_INFO, "add user %s\n", userSession->getOnlineName() );
                    addItem( userItem );
                }
                else
                {
                    LogMsg( LOG_INFO, "insert user %s\n", userSession->getOnlineName() );
                    insertItem( 0, (QListWidgetItem *)userItem );
                }

                setItemWidget( (QListWidgetItem *)userItem, (QWidget *)userItem );
                m_UserCache[user->getMyOnlineId()] = userSession;
                onListItemAdded( userSession, userItem );
            }
        }
        else
        {
            UserListItem* userItem = findListEntryWidgetByOnlineId( user->getMyOnlineId() );
            if( userItem )
            {
                GuiUserSessionBase * userSession = userItem->getUserSession();
                if( userSession )
                {
                    onListItemUpdated( userItem->getUserSession(), userItem );
                }
               
                userItem->update();
            }
        }
    }
}

//============================================================================
void UserListWidget::removeUser( VxGUID& onlineId )
{
    auto iter = m_UserCache.find( onlineId );
    if( iter != m_UserCache.end() )
    {
        m_UserCache.erase( iter );
        UserListItem* userItem = findListEntryWidgetByOnlineId( onlineId );
        if( userItem )
        {
            GuiUserSessionBase * userSession = userItem->getUserSession();
            delete userItem;
            delete userSession;
        }
    }
}

//============================================================================
void UserListWidget::updateThumb( GuiThumb* thumb )
{
    UserListItem* userItem = findListEntryWidgetByOnlineId( thumb->getCreatorId() );
    if( userItem )
    {
        userItem->updateThumb( thumb );
    }
}

//============================================================================
void UserListWidget::onListItemAdded( GuiUserSessionBase* userSession, UserListItem* userItem )
{
    onListItemUpdated( userSession, userItem );
}

//============================================================================
void UserListWidget::onListItemUpdated( GuiUserSessionBase* userSession, UserListItem* userItem )
{
    if( userSession && userItem && userSession->getUserIdent() )
    {
        EHostType hostType = eHostTypeUnknown;
        EPluginType pluginType = ePluginTypeInvalid;
        switch( m_UserViewType )
        {
        case eUserViewTypeGroup:
            hostType = eHostTypeGroup;
            pluginType = ePluginTypeClientGroup;
            break;
        case eUserViewTypeChatRoom:
            hostType = eHostTypeChatRoom;
            pluginType = ePluginTypeClientChatRoom;
            break;
        case eUserViewTypeRandomConnect:
            hostType = eHostTypeRandomConnect;
            pluginType = ePluginTypeClientRandomConnect;
            break;
        default:
            hostType = eHostTypePeerUser;
            pluginType = ePluginTypeHostPeerUser;
        }
        
        VxPushButton* avatarButton = userItem->getAvatarButton();
        GuiUser* user = userSession->getUserIdent();

        QImage	avatarImage;
        bool havAvatarImage = m_ThumbMgr.requestAvatarImage( user, pluginType, avatarImage, true );
        if( havAvatarImage && avatarButton )
        {
            avatarButton->setIconOverrideImage( avatarImage );
        }
    }
}
