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
#include "UserJoinListItem.h"
#include "UserJoinListWidget.h"
#include "GuiUserJoinMgr.h"
#include "GuiUserJoinSession.h"

#include "MyIcons.h"
//#include "PopupMenu.h"
#include "AppGlobals.h"
#include "AppCommon.h"
#include "GuiParams.h"

#include <CoreLib/VxDebug.h>
 
//============================================================================
UserJoinListWidget::UserJoinListWidget( QWidget * parent )
: QListWidget( parent )
, m_MyApp( GetAppInstance() )
, m_Engine( m_MyApp.getEngine() )
, m_UserJoinMgr( m_MyApp.getUserJoinMgr() )
, m_ThumbMgr( m_MyApp.getThumbMgr() )
{
	QListWidget::setSortingEnabled( true );
	sortItems( Qt::DescendingOrder );

    connect( &m_UserJoinMgr, SIGNAL(signalMyIdentUpdated( GuiUserJoin*)),           this, SLOT(slotMyIdentUpdated( GuiUserJoin*))) ;

    connect( &m_UserJoinMgr, SIGNAL(signalUserJoinAdded( GuiUserJoin*)),                this, SLOT(slotUserJoinAdded( GuiUserJoin*))) ;
    connect( &m_UserJoinMgr, SIGNAL(signalUserJoinRemoved( VxGUID)),                this, SLOT(slotUserJoinRemoved( VxGUID))) ;
    connect( &m_UserJoinMgr, SIGNAL(signalUserJoinUpdated( GuiUserJoin*)),              this, SLOT(slotUserJoinUpdated( GuiUserJoin*))) ;
    connect( &m_UserJoinMgr, SIGNAL(signalUserJoinOnlineStatus( GuiUserJoin*,bool)),    this, SLOT(slotUserJoinOnlineStatus( GuiUserJoin*,bool))) ;

    connect( &m_ThumbMgr, SIGNAL( signalThumbAdded( GuiThumb* ) ),          this, SLOT( slotThumbAdded( GuiThumb* ) ) );
    connect( &m_ThumbMgr, SIGNAL(signalThumbUpdated( GuiThumb*)),           this, SLOT(slotThumbUpdated( GuiThumb*))) ;
    connect( &m_ThumbMgr, SIGNAL(signalThumbRemoved( VxGUID)),              this, SLOT(slotThumbRemoved( VxGUID))) ;


    //connect( this, SIGNAL(itemClicked(QListWidgetItem *)),          this, SLOT(slotItemClicked(QListWidgetItem *))) ;
    //connect( this, SIGNAL(itemDoubleClicked(QListWidgetItem *)),    this, SLOT(slotItemClicked(QListWidgetItem *))) ;

    setUserJoinViewType( eUserJoinViewTypeEverybody );
}

//============================================================================
UserJoinListItem* UserJoinListWidget::sessionToWidget( GuiUserJoinSession* userSession )
{
    UserJoinListItem* userItem = new UserJoinListItem(this);
    userItem->setUserJoinSession( userSession );
    userItem->setSizeHint( userItem->calculateSizeHint() );

    connect( userItem, SIGNAL(signalUserJoinListItemClicked(QListWidgetItem *)),	    this, SLOT(slotUserJoinListItemClicked(QListWidgetItem *)) );
    connect( userItem, SIGNAL(signalAvatarButtonClicked(UserJoinListItem *)),	        this, SLOT(slotAvatarButtonClicked(UserJoinListItem *)) );
    connect( userItem, SIGNAL(signalMenuButtonClicked(UserJoinListItem *)),	            this, SLOT(slotMenuButtonClicked(UserJoinListItem *)) );
    connect( userItem, SIGNAL(signalFriendshipButtonClicked(UserJoinListItem *)),		this, SLOT(slotFriendshipButtonClicked(UserJoinListItem *)) );

    userItem->updateWidgetFromInfo();

    return userItem;
}

//============================================================================
GuiUserJoinSession* UserJoinListWidget::widgetToSession( UserJoinListItem * item )
{
    return item->getUserJoinSession();
}

//============================================================================
MyIcons&  UserJoinListWidget::getMyIcons( void )
{
	return m_MyApp.getMyIcons();
}

//============================================================================
GuiUserJoinSession * UserJoinListWidget::findSession( VxGUID& lclSessionId )
{
    int iCnt = count();
    for( int iRow = 0; iRow < iCnt; iRow++ )
    {
        UserJoinListItem* listItem =  (UserJoinListItem*)item( iRow );
        if( listItem )
        {
            GuiUserJoinSession * userSession = listItem->getUserJoinSession();
            if( userSession && userSession->getSessionId() == lclSessionId )
            {
                return userSession;
            }
        }
    }

    return nullptr;
}

//============================================================================
UserJoinListItem* UserJoinListWidget::findListEntryWidgetBySessionId( VxGUID& sessionId )
{
    int iCnt = count();
    for( int iRow = 0; iRow < iCnt; iRow++ )
    {
        UserJoinListItem*  userItem = (UserJoinListItem*)item( iRow );
        if( userItem )
        {
            GuiUserJoinSession * userSession = userItem->getUserJoinSession();
            if( userSession && ( userSession->getSessionId() == sessionId ) )
            {
                return userItem;
            }
        }
    }

    return nullptr;
}

//============================================================================
UserJoinListItem* UserJoinListWidget::findListEntryWidgetByOnlineId( VxGUID& onlineId )
{
    int iCnt = count();
    for( int iRow = 0; iRow < iCnt; iRow++ )
    {
        UserJoinListItem*  userItem = (UserJoinListItem*)item( iRow );
        if( userItem )
        {
            GuiUserJoinSession * userSession = userItem->getUserJoinSession();
            if( userSession && ( userSession->getMyOnlineId() == onlineId ) )
            {
                return userItem;
            }
        }
    }

    return nullptr;
}

//============================================================================
void UserJoinListWidget::slotUserJoinListItemClicked( UserJoinListItem* userItem )
{
	if( 300 > m_ClickEventTimer.elapsedMs()  ) // avoid duplicate clicks
	{
		return;
	}

	m_ClickEventTimer.startTimer();
    onUserJoinListItemClicked(userItem);
}

//============================================================================
void UserJoinListWidget::slotAvatarButtonClicked( UserJoinListItem* userItem )
{
    if( 300 > m_ClickEventTimer.elapsedMs() ) // avoid duplicate clicks
    {
        return;
    }

    m_ClickEventTimer.startTimer();
    onAvatarButtonClicked(userItem);
}

//============================================================================
void UserJoinListWidget::slotMenuButtonClicked( UserJoinListItem* userItem )
{
	if( 300 > m_ClickEventTimer.elapsedMs()  ) // avoid duplicate clicks
	{
		return;
	}

	m_ClickEventTimer.startTimer();
    onMenuButtonClicked( userItem );
}

//============================================================================
void UserJoinListWidget::slotFriendshipButtonClicked( UserJoinListItem* userItem )
{
    if( 300 > m_ClickEventTimer.elapsedMs()  ) // avoid duplicate clicks
    {
        return;
    }

    m_ClickEventTimer.startTimer();
    onFriendshipButtonClicked( userItem );
}

//============================================================================
void UserJoinListWidget::addUserJoinToList( EHostType userType, VxGUID& sessionId, GuiUserJoin * userIdent )
{
    GuiUserJoinSession* userSession = new GuiUserJoinSession( userType, sessionId, userIdent, this );

    addOrUpdateUserJoinSession( userSession );
}

//============================================================================
UserJoinListItem* UserJoinListWidget::addOrUpdateUserJoinSession( GuiUserJoinSession* userSession )
{
    UserJoinListItem* userItem = findListEntryWidgetBySessionId( userSession->getSessionId() );
    if( userItem )
    {
        GuiUserJoinSession* userOldSession = userItem->getUserJoinSession();
        if( userOldSession != userSession )
        {
            userItem->setUserJoinSession( userSession );
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
void UserJoinListWidget::onUserJoinListItemClicked( UserJoinListItem* userItem )
{
    LogMsg( LOG_DEBUG, "onUserJoinListItemClicked" );
    if( userItem )
    {
        GuiUserJoinSession* userSession = userItem->getUserJoinSession();
        if( userSession )
        {
            emit signalUserJoinListItemClicked( userSession, userItem );
        }
    }
    onUserJoinListItemClicked( userItem );
}

//============================================================================
void UserJoinListWidget::onAvatarButtonClicked( UserJoinListItem* userItem )
{
    LogMsg( LOG_DEBUG, "onAvatarButtonClicked" );
    if( userItem )
    {
        GuiUserJoinSession* userSession = userItem->getUserJoinSession();
        if( userSession )
        {
            emit signalAvatarButtonClicked( userSession, userItem );
        }
    }
}

//============================================================================
void UserJoinListWidget::onMenuButtonClicked( UserJoinListItem* userItem )
{
    LogMsg( LOG_DEBUG, "onMenuButtonClicked" );
    if( userItem )
    {
        GuiUserJoinSession* userSession = userItem->getUserJoinSession();
        if( userSession )
        {
            emit signalMenuButtonClicked( userSession, userItem );
        }
    }


    /*
    m_SelectedFriend = widgetToUserJoin( item );
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
void UserJoinListWidget::onFriendshipButtonClicked( UserJoinListItem* userItem )
{
    LogMsg( LOG_DEBUG, "onFriendshipButtonClicked" );
    if( userItem )
    {
        GuiUserJoinSession* userSession = userItem->getUserJoinSession();
        if( userSession )
        {
            emit signalFriendshipButtonClicked( userSession, userItem );
        }
    }
}

//============================================================================
void UserJoinListWidget::slotMyIdentUpdated( GuiUserJoin* user )
{
    if( getShowMyself() )
    {
        updateUserJoin( user );
    }
}

//============================================================================
void UserJoinListWidget::slotUserJoinAdded( GuiUserJoin* user )
{
    updateUserJoin( user );
}

//============================================================================
void UserJoinListWidget::slotUserJoinRemoved( VxGUID onlineId )
{
    removeUserJoin( onlineId );
}

//============================================================================
void UserJoinListWidget::slotUserJoinUpdated( GuiUserJoin* user )
{
    updateUserJoin( user );
}

//============================================================================
void UserJoinListWidget::slotUserJoinOnlineStatus( GuiUserJoin* user, bool isOnline )
{
    updateUserJoin( user );
}

//============================================================================
void UserJoinListWidget::slotThumbAdded( GuiThumb* thumb )
{
    updateThumb( thumb );
}

//============================================================================
void UserJoinListWidget::slotThumbUpdated( GuiThumb* thumb )
{
    updateThumb( thumb );
}

//============================================================================
void UserJoinListWidget::slotThumbRemoved( VxGUID thumbId )
{
    // TODO
}

//============================================================================
void UserJoinListWidget::setUserJoinViewType( EUserJoinViewType viewType )
{
    if( viewType != m_UserJoinViewType )
    {
        m_UserJoinViewType = viewType;
        refreshList();
    }
}

//============================================================================
void UserJoinListWidget::refreshList( void )
{
    clearUserJoinList();
    std::vector<GuiUserJoin *> userList;
    m_UserJoinMgr.lockUserJoinMgr();

    if( isListViewMatch( m_UserJoinMgr.getMyIdent() ) )
    {
        userList.push_back( m_UserJoinMgr.getMyIdent() );
    }

    std::map<VxGUID, GuiUserJoin*>& mgrList = m_UserJoinMgr.getUserJoinList();
    for( auto iter = mgrList.begin(); iter != mgrList.end(); ++iter )
    {
        if( isListViewMatch( iter->second ) )
        {
            userList.push_back( iter->second );
        }
    }

    m_UserJoinMgr.unlockUserJoinMgr();
    for( auto user : userList )
    {
        updateUserJoin( user );
    }
}


//============================================================================
void UserJoinListWidget::clearUserJoinList( void )
{
    m_UserJoinCache.clear();
    for(int i = 0; i < count(); ++i)
    {
        QListWidgetItem* userItem = item(i);
        delete ((UserJoinListItem *)userItem);
    }

    clear();
}

//============================================================================
bool UserJoinListWidget::isListViewMatch( GuiUserJoin * user )
{
    if( user && !user->isIgnored())
    {
        if( user->isMyself() )
        { 
            return getShowMyself();
        }
        else if( eUserJoinViewTypeEverybody == getUserJoinViewType() )
        {
            return true;
        }
        else if( eUserJoinViewTypeFriends == getUserJoinViewType() )
        {
            return user->isFriend() || user->isAdmin();
        }
        else if( eUserJoinViewTypeGroup == getUserJoinViewType() )
        {
            return user->isGroupHosted() && !user->isAnonymous();
        }
        else if( eUserJoinViewTypeChatRoom == getUserJoinViewType() )
        {
            return user->isChatRoomHosted() && !user->isAnonymous();
        }
        else if( eUserJoinViewTypeRandomConnect == getUserJoinViewType() )
        {
            return user->isRandomConnectHosted() && !user->isAnonymous();
        }

        if( user->isPeerHosted() && user->isFriend() )
        {
            return true;
        }
    }
    else if( user && user->isIgnored() && eUserJoinViewTypeIgnored == getUserJoinViewType() )
    {
        return true;
    }

    return false;
}

//============================================================================
void UserJoinListWidget::updateUserJoin( GuiUserJoin * user )
{
    if( isListViewMatch( user ) )
    {
        auto iter = m_UserJoinCache.find( user->getMyOnlineId() );
        if( iter == m_UserJoinCache.end() )
        {
            GuiUserJoinSession * userSession = new GuiUserJoinSession( user, this );
            if( userSession )
            {
                UserJoinListItem* userItem = sessionToWidget( userSession );
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
                m_UserJoinCache[user->getMyOnlineId()] = userSession;
                onListItemAdded( userSession, userItem );
            }
        }
        else
        {
            UserJoinListItem* userItem = findListEntryWidgetByOnlineId( user->getMyOnlineId() );
            if( userItem )
            {
                GuiUserJoinSession * userSession = userItem->getUserJoinSession();
                if( userSession )
                {
                    onListItemUpdated( userItem->getUserJoinSession(), userItem );
                }
               
                userItem->update();
            }
        }
    }
}

//============================================================================
void UserJoinListWidget::removeUserJoin( VxGUID& onlineId )
{
    auto iter = m_UserJoinCache.find( onlineId );
    if( iter != m_UserJoinCache.end() )
    {
        m_UserJoinCache.erase( iter );
        UserJoinListItem* userItem = findListEntryWidgetByOnlineId( onlineId );
        if( userItem )
        {
            GuiUserJoinSession * userSession = userItem->getUserJoinSession();
            delete userItem;
            delete userSession;
        }
    }
}

//============================================================================
void UserJoinListWidget::updateThumb( GuiThumb* thumb )
{
    UserJoinListItem* userItem = findListEntryWidgetByOnlineId( thumb->getCreatorId() );
    if( userItem )
    {
        userItem->updateThumb( thumb );
    }
}

//============================================================================
void UserJoinListWidget::onListItemAdded( GuiUserJoinSession* userSession, UserJoinListItem* userItem )
{
    onListItemUpdated( userSession, userItem );
}

//============================================================================
void UserJoinListWidget::onListItemUpdated( GuiUserJoinSession* userSession, UserJoinListItem* userItem )
{
    if( userSession && userItem && userSession->getUserIdent() )
    {
        EHostType hostType = eHostTypeUnknown;
        switch( m_UserJoinViewType )
        {
        case eUserJoinViewTypeGroup:
            hostType = eHostTypeGroup;
            break;
        case eUserJoinViewTypeChatRoom:
            hostType = eHostTypeChatRoom;
            break;
        case eUserJoinViewTypeRandomConnect:
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
