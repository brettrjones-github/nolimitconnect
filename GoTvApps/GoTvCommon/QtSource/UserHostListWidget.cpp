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
#include "UserHostListItem.h"
#include "UserHostListWidget.h"
#include "GuiUserHostMgr.h"
#include "GuiUserHostSession.h"

#include "MyIcons.h"
#include "PopupMenu.h"
#include "AppGlobals.h"
#include "AppCommon.h"
#include "GuiParams.h"

#include <CoreLib/VxDebug.h>
 
//============================================================================
UserHostListWidget::UserHostListWidget( QWidget * parent )
: QListWidget( parent )
, m_MyApp( GetAppInstance() )
, m_Engine( m_MyApp.getEngine() )
, m_UserHostMgr( m_MyApp.getUserHostMgr() )
, m_ThumbMgr( m_MyApp.getThumbMgr() )
{
	QListWidget::setSortingEnabled( true );
	sortItems( Qt::DescendingOrder );

    connect( &m_UserHostMgr, SIGNAL(signalMyIdentUpdated( GuiUserHost*)),           this, SLOT(slotMyIdentUpdated( GuiUserHost*))) ;

    connect( &m_UserHostMgr, SIGNAL(signalUserHostAdded( GuiUserHost*)),                this, SLOT(slotUserHostAdded( GuiUserHost*))) ;
    connect( &m_UserHostMgr, SIGNAL(signalUserHostRemoved( VxGUID)),                this, SLOT(slotUserHostRemoved( VxGUID))) ;
    connect( &m_UserHostMgr, SIGNAL(signalUserHostUpdated( GuiUserHost*)),              this, SLOT(slotUserHostUpdated( GuiUserHost*))) ;
    connect( &m_UserHostMgr, SIGNAL(signalUserHostOnlineStatus( GuiUserHost*,bool)),    this, SLOT(slotUserHostOnlineStatus( GuiUserHost*,bool))) ;

    connect( &m_ThumbMgr, SIGNAL( signalThumbAdded( GuiThumb* ) ),          this, SLOT( slotThumbAdded( GuiThumb* ) ) );
    connect( &m_ThumbMgr, SIGNAL(signalThumbUpdated( GuiThumb*)),           this, SLOT(slotThumbUpdated( GuiThumb*))) ;
    connect( &m_ThumbMgr, SIGNAL(signalThumbRemoved( VxGUID)),              this, SLOT(slotThumbRemoved( VxGUID))) ;


    //connect( this, SIGNAL(itemClicked(QListWidgetItem *)),          this, SLOT(slotItemClicked(QListWidgetItem *))) ;
    //connect( this, SIGNAL(itemDoubleClicked(QListWidgetItem *)),    this, SLOT(slotItemClicked(QListWidgetItem *))) ;

    setUserHostViewType( eUserHostViewTypeEverybody );
}

//============================================================================
UserHostListItem* UserHostListWidget::sessionToWidget( GuiUserHostSession* userSession )
{
    UserHostListItem* userItem = new UserHostListItem(this);
    userItem->setUserHostSession( userSession );
    userItem->setSizeHint( userItem->calculateSizeHint() );

    connect( userItem, SIGNAL(signalUserHostListItemClicked(QListWidgetItem *)),	    this, SLOT(slotUserHostListItemClicked(QListWidgetItem *)) );
    connect( userItem, SIGNAL(signalAvatarButtonClicked(UserHostListItem *)),	        this, SLOT(slotAvatarButtonClicked(UserHostListItem *)) );
    connect( userItem, SIGNAL(signalMenuButtonClicked(UserHostListItem *)),	            this, SLOT(slotMenuButtonClicked(UserHostListItem *)) );
    connect( userItem, SIGNAL(signalFriendshipButtonClicked(UserHostListItem *)),		this, SLOT(slotFriendshipButtonClicked(UserHostListItem *)) );

    userItem->updateWidgetFromInfo();

    return userItem;
}

//============================================================================
GuiUserHostSession* UserHostListWidget::widgetToSession( UserHostListItem * item )
{
    return item->getUserHostSession();
}

//============================================================================
MyIcons&  UserHostListWidget::getMyIcons( void )
{
	return m_MyApp.getMyIcons();
}

//============================================================================
GuiUserHostSession * UserHostListWidget::findSession( VxGUID& lclSessionId )
{
    int iCnt = count();
    for( int iRow = 0; iRow < iCnt; iRow++ )
    {
        UserHostListItem* listItem =  (UserHostListItem*)item( iRow );
        if( listItem )
        {
            GuiUserHostSession * userSession = listItem->getUserHostSession();
            if( userSession && userSession->getSessionId() == lclSessionId )
            {
                return userSession;
            }
        }
    }

    return nullptr;
}

//============================================================================
UserHostListItem* UserHostListWidget::findListEntryWidgetBySessionId( VxGUID& sessionId )
{
    int iCnt = count();
    for( int iRow = 0; iRow < iCnt; iRow++ )
    {
        UserHostListItem*  userItem = (UserHostListItem*)item( iRow );
        if( userItem )
        {
            GuiUserHostSession * userSession = userItem->getUserHostSession();
            if( userSession && ( userSession->getSessionId() == sessionId ) )
            {
                return userItem;
            }
        }
    }

    return nullptr;
}

//============================================================================
UserHostListItem* UserHostListWidget::findListEntryWidgetByOnlineId( VxGUID& onlineId )
{
    int iCnt = count();
    for( int iRow = 0; iRow < iCnt; iRow++ )
    {
        UserHostListItem*  userItem = (UserHostListItem*)item( iRow );
        if( userItem )
        {
            GuiUserHostSession * userSession = userItem->getUserHostSession();
            if( userSession && ( userSession->getMyOnlineId() == onlineId ) )
            {
                return userItem;
            }
        }
    }

    return nullptr;
}

//============================================================================
void UserHostListWidget::slotUserHostListItemClicked( UserHostListItem* userItem )
{
	if( 300 > m_ClickEventTimer.elapsedMs()  ) // avoid duplicate clicks
	{
		return;
	}

	m_ClickEventTimer.startTimer();
    onUserHostListItemClicked(userItem);
}

//============================================================================
void UserHostListWidget::slotAvatarButtonClicked( UserHostListItem* userItem )
{
    if( 300 > m_ClickEventTimer.elapsedMs() ) // avoid duplicate clicks
    {
        return;
    }

    m_ClickEventTimer.startTimer();
    onAvatarButtonClicked(userItem);
}

//============================================================================
void UserHostListWidget::slotMenuButtonClicked( UserHostListItem* userItem )
{
	if( 300 > m_ClickEventTimer.elapsedMs()  ) // avoid duplicate clicks
	{
		return;
	}

	m_ClickEventTimer.startTimer();
    onMenuButtonClicked( userItem );
}

//============================================================================
void UserHostListWidget::slotFriendshipButtonClicked( UserHostListItem* userItem )
{
    if( 300 > m_ClickEventTimer.elapsedMs()  ) // avoid duplicate clicks
    {
        return;
    }

    m_ClickEventTimer.startTimer();
    onFriendshipButtonClicked( userItem );
}

//============================================================================
void UserHostListWidget::addUserHostToList( EHostType userType, VxGUID& sessionId, GuiUserHost * userIdent )
{
    GuiUserHostSession* userSession = new GuiUserHostSession( userType, sessionId, userIdent, this );

    addOrUpdateUserHostSession( userSession );
}

//============================================================================
UserHostListItem* UserHostListWidget::addOrUpdateUserHostSession( GuiUserHostSession* userSession )
{
    UserHostListItem* userItem = findListEntryWidgetBySessionId( userSession->getSessionId() );
    if( userItem )
    {
        GuiUserHostSession* userOldSession = userItem->getUserHostSession();
        if( userOldSession != userSession )
        {
            userItem->setUserHostSession( userSession );
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
void UserHostListWidget::onUserHostListItemClicked( UserHostListItem* userItem )
{
    LogMsg( LOG_DEBUG, "onUserHostListItemClicked" );
    if( userItem )
    {
        GuiUserHostSession* userSession = userItem->getUserHostSession();
        if( userSession )
        {
            emit signalUserHostListItemClicked( userSession, userItem );
        }
    }
    onUserHostListItemClicked( userItem );
}

//============================================================================
void UserHostListWidget::onAvatarButtonClicked( UserHostListItem* userItem )
{
    LogMsg( LOG_DEBUG, "onAvatarButtonClicked" );
    if( userItem )
    {
        GuiUserHostSession* userSession = userItem->getUserHostSession();
        if( userSession )
        {
            emit signalAvatarButtonClicked( userSession, userItem );
        }
    }
}

//============================================================================
void UserHostListWidget::onMenuButtonClicked( UserHostListItem* userItem )
{
    LogMsg( LOG_DEBUG, "onMenuButtonClicked" );
    if( userItem )
    {
        GuiUserHostSession* userSession = userItem->getUserHostSession();
        if( userSession )
        {
            emit signalMenuButtonClicked( userSession, userItem );
        }
    }


    /*
    m_SelectedFriend = widgetToUserHost( item );
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
void UserHostListWidget::onFriendshipButtonClicked( UserHostListItem* userItem )
{
    LogMsg( LOG_DEBUG, "onFriendshipButtonClicked" );
    if( userItem )
    {
        GuiUserHostSession* userSession = userItem->getUserHostSession();
        if( userSession )
        {
            emit signalFriendshipButtonClicked( userSession, userItem );
        }
    }
}

//============================================================================
void UserHostListWidget::slotMyIdentUpdated( GuiUserHost* user )
{
    if( getShowMyself() )
    {
        updateUserHost( user );
    }
}

//============================================================================
void UserHostListWidget::slotUserHostAdded( GuiUserHost* user )
{
    updateUserHost( user );
}

//============================================================================
void UserHostListWidget::slotUserHostRemoved( VxGUID onlineId )
{
    removeUserHost( onlineId );
}

//============================================================================
void UserHostListWidget::slotUserHostUpdated( GuiUserHost* user )
{
    updateUserHost( user );
}

//============================================================================
void UserHostListWidget::slotUserHostOnlineStatus( GuiUserHost* user, bool isOnline )
{
    updateUserHost( user );
}

//============================================================================
void UserHostListWidget::slotThumbAdded( GuiThumb* thumb )
{
    updateThumb( thumb );
}

//============================================================================
void UserHostListWidget::slotThumbUpdated( GuiThumb* thumb )
{
    updateThumb( thumb );
}

//============================================================================
void UserHostListWidget::slotThumbRemoved( VxGUID thumbId )
{
    // TODO
}

//============================================================================
void UserHostListWidget::setUserHostViewType( EUserHostViewType viewType )
{
    if( viewType != m_UserHostViewType )
    {
        m_UserHostViewType = viewType;
        refreshList();
    }
}

//============================================================================
void UserHostListWidget::refreshList( void )
{
    clearUserHostList();
    std::vector<GuiUserHost *> userList;
    m_UserHostMgr.lockUserHostMgr();

    if( isListViewMatch( m_UserHostMgr.getMyIdent() ) )
    {
        userList.push_back( m_UserHostMgr.getMyIdent() );
    }

    std::map<VxGUID, GuiUserHost*>& mgrList = m_UserHostMgr.getUserHostList();
    for( auto iter = mgrList.begin(); iter != mgrList.end(); ++iter )
    {
        if( isListViewMatch( iter->second ) )
        {
            userList.push_back( iter->second );
        }
    }

    m_UserHostMgr.unlockUserHostMgr();
    for( auto user : userList )
    {
        updateUserHost( user );
    }
}


//============================================================================
void UserHostListWidget::clearUserHostList( void )
{
    m_UserHostCache.clear();
    for(int i = 0; i < count(); ++i)
    {
        QListWidgetItem* userItem = item(i);
        delete ((UserHostListItem *)userItem);
    }

    clear();
}

//============================================================================
bool UserHostListWidget::isListViewMatch( GuiUserHost * user )
{
    if( user && !user->isIgnored())
    {
        if( user->isMyself() )
        { 
            return getShowMyself();
        }
        else if( eUserHostViewTypeEverybody == getUserHostViewType() )
        {
            return true;
        }
        else if( eUserHostViewTypeFriends == getUserHostViewType() )
        {
            return user->isFriend() || user->isAdmin();
        }
        else if( eUserHostViewTypeGroup == getUserHostViewType() )
        {
            return user->isGroupHosted() && !user->isAnonymous();
        }
        else if( eUserHostViewTypeChatRoom == getUserHostViewType() )
        {
            return user->isChatRoomHosted() && !user->isAnonymous();
        }
        else if( eUserHostViewTypeRandomConnect == getUserHostViewType() )
        {
            return user->isRandomConnectHosted() && !user->isAnonymous();
        }

        if( user->isPeerHosted() && user->isFriend() )
        {
            return true;
        }
    }
    else if( user && user->isIgnored() && eUserHostViewTypeIgnored == getUserHostViewType() )
    {
        return true;
    }

    return false;
}

//============================================================================
void UserHostListWidget::updateUserHost( GuiUserHost * user )
{
    if( isListViewMatch( user ) )
    {
        auto iter = m_UserHostCache.find( user->getMyOnlineId() );
        if( iter == m_UserHostCache.end() )
        {
            GuiUserHostSession * userSession = new GuiUserHostSession( user, this );
            if( userSession )
            {
                UserHostListItem* userItem = sessionToWidget( userSession );
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
                m_UserHostCache[user->getMyOnlineId()] = userSession;
                onListItemAdded( userSession, userItem );
            }
        }
        else
        {
            UserHostListItem* userItem = findListEntryWidgetByOnlineId( user->getMyOnlineId() );
            if( userItem )
            {
                GuiUserHostSession * userSession = userItem->getUserHostSession();
                if( userSession )
                {
                    onListItemUpdated( userItem->getUserHostSession(), userItem );
                }
               
                userItem->update();
            }
        }
    }
}

//============================================================================
void UserHostListWidget::removeUserHost( VxGUID& onlineId )
{
    auto iter = m_UserHostCache.find( onlineId );
    if( iter != m_UserHostCache.end() )
    {
        m_UserHostCache.erase( iter );
        UserHostListItem* userItem = findListEntryWidgetByOnlineId( onlineId );
        if( userItem )
        {
            GuiUserHostSession * userSession = userItem->getUserHostSession();
            delete userItem;
            delete userSession;
        }
    }
}

//============================================================================
void UserHostListWidget::updateThumb( GuiThumb* thumb )
{
    UserHostListItem* userItem = findListEntryWidgetByOnlineId( thumb->getCreatorId() );
    if( userItem )
    {
        userItem->updateThumb( thumb );
    }
}

//============================================================================
void UserHostListWidget::onListItemAdded( GuiUserHostSession* userSession, UserHostListItem* userItem )
{
    onListItemUpdated( userSession, userItem );
}

//============================================================================
void UserHostListWidget::onListItemUpdated( GuiUserHostSession* userSession, UserHostListItem* userItem )
{
    if( userSession && userItem && userSession->getUserIdent() )
    {
        EHostType hostType = eHostTypeUnknown;
        switch( m_UserHostViewType )
        {
        case eUserHostViewTypeGroup:
            hostType = eHostTypeGroup;
            break;
        case eUserHostViewTypeChatRoom:
            hostType = eHostTypeChatRoom;
            break;
        case eUserHostViewTypeRandomConnect:
            hostType = eHostTypeRandomConnect;
            break;
        default:
            hostType = eHostTypePeerUser;
        }
        
        VxPushButton* avatarButton = userItem->getAvatarButton();
        GuiUser* user = userSession->getUserIdent();

        QImage	avatarImage;
        bool havAvatarImage = m_ThumbMgr.requestAvatarImage( user, hostType, avatarImage, true );
        if( havAvatarImage && avatarButton )
        {
            avatarButton->setIconOverrideImage( avatarImage );
        }
    }
}
