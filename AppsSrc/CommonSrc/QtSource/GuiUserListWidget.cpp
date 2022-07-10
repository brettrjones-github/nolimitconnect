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
// http://www.nolimitconnect.org
//============================================================================


#include "GuiUserListItem.h"
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

    connect( &m_UserMgr, SIGNAL(signalMyIdentUpdated(GuiUser*)),	            this, SLOT(slotMyIdentUpdated(GuiUser*)) );

    connect( &m_UserMgr, SIGNAL(signalUserAdded(GuiUser*)),	                    this, SLOT(slotUserAdded(GuiUser*)) );
    connect( &m_UserMgr, SIGNAL(signalUserRemoved(VxGUID)),	                    this, SLOT(slotUserRemoved(VxGUID)) );
    connect( &m_UserMgr, SIGNAL(signalUserUpdated(GuiUser*)),	                this, SLOT(slotUserUpdated(GuiUser*)) );
    connect( &m_UserMgr, SIGNAL(signalUserOnlineStatus(GuiUser*)),	            this, SLOT(slotUserOnlineStatus(GuiUser*)) );

    connect( &m_ThumbMgr, SIGNAL( signalThumbAdded(GuiThumb*) ),                this, SLOT( slotThumbAdded(GuiThumb*) ) );
    connect( &m_ThumbMgr, SIGNAL( signalThumbUpdated(GuiThumb*) ),              this, SLOT( slotThumbUpdated(GuiThumb*) ) );
    connect( &m_ThumbMgr, SIGNAL( signalThumbRemoved(VxGUID) ),                 this, SLOT( slotThumbRemoved(VxGUID) ) );

    GetAppInstance().getUserMgr().wantGuiUserUpdateCallbacks( this, true );
}

//============================================================================
GuiUserListWidget::~GuiUserListWidget()
{
    GetAppInstance().getUserMgr().wantGuiUserUpdateCallbacks( this, false );
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
void GuiUserListWidget::showEvent( QShowEvent * ev )
{
    QListWidget::showEvent( ev );
}

//============================================================================
void GuiUserListWidget::setUserViewType( EUserViewType viewType )
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
        if( isListViewMatch( iter->second ) )
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
void GuiUserListWidget::clearUserList( void )
{
    m_UserCache.clear();
    for( int i = 0; i < count(); ++i )
    {
        QListWidgetItem* userItem = item( i );
        delete ( ( GuiUserListItem* )userItem );
    }

    clear();
}

//============================================================================
GuiUserListItem* GuiUserListWidget::sessionToWidget( GuiUserSessionBase* userSession )
{
    GuiUserListItem* userItem = new GuiUserListItem(this);
    userItem->setSizeHint( userItem->calculateSizeHint() );

    userItem->setUserSession( userSession );

    connect( userItem, SIGNAL(signalGuiUserListItemClicked(GuiUserListItem*)),	    this, SLOT(slotUserListItemClicked(GuiUserListItem*)) );
    connect( userItem, SIGNAL(signalAvatarButtonClicked(GuiUserListItem*)),	        this, SLOT(slotAvatarButtonClicked(GuiUserListItem*)) );
    connect( userItem, SIGNAL(signalOfferButtonClicked(GuiUserListItem*)),          this, SLOT(slotOfferButtonClicked(GuiUserListItem*)) );
    connect( userItem, SIGNAL(signalPushToTalkButtonPressed(GuiUserListItem*)),     this, SLOT(slotPushToTalkButtonPressed(GuiUserListItem*)) );
    connect( userItem, SIGNAL(signalPushToTalkButtonReleased(GuiUserListItem*)),    this, SLOT(slotPushToTalkButtonReleased(GuiUserListItem*)) );
    connect( userItem, SIGNAL(signalMenuButtonClicked(GuiUserListItem*)),	        this, SLOT(slotMenuButtonClicked(GuiUserListItem*)) );

    userItem->updateWidgetFromInfo();

    return userItem;
}

//============================================================================
GuiUserSessionBase* GuiUserListWidget::widgetToSession( GuiUserListItem * item )
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
            LogMsg( LOG_DEBUG, "GuiUserListWidget::updateUser new user %s", user->getOnlineName().c_str() );
            userSession = makeSession( user );
            if( userSession )
            {
                m_UserCache[user->getMyOnlineId()] = userSession;
                GuiUserListItem* entryWidget = sessionToWidget( userSession );
                if( 0 == count() )
                {
                    LogMsg( LOG_INFO, "add user %s", user->getOnlineName().c_str() );
                    addItem( entryWidget );
                }
                else
                {
                    LogMsg( LOG_INFO, "insert user %s", user->getOnlineName().c_str() );
                    insertItem( 0, (QListWidgetItem *)entryWidget );
                }

                setItemWidget( (QListWidgetItem *)entryWidget, (QWidget *)entryWidget );
                m_UserCache[user->getMyOnlineId()] = userSession;
                onListItemAdded( userSession, entryWidget );
            }
            else
            {
                LogMsg( LOG_ERROR, "GuiUserListWidget::updateUser failed create session for user %s", user->getOnlineName().c_str() );
            }
        }
        else
        {
            GuiUserListItem* userItem = findListEntryWidgetByOnlineId( user->getMyOnlineId() );
            if( userItem )
            {
                GuiUserSessionBase* userSession = userItem->getUserSession();
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
void GuiUserListWidget::updateEntryWidget( VxGUID& onlineId )
{
    GuiUserListItem* entryWidget = findListEntryWidgetByOnlineId( onlineId );
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
        LogMsg( LOG_DEBUG, "GuiUserListWidget::removeUser %s", iter->second->getOnlineName().c_str() );
        m_UserCache.erase( iter );

        GuiUserListItem* userItem = findListEntryWidgetByOnlineId( onlineId );
        if( userItem )
        {
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
        GuiUserListItem* listItem =  (GuiUserListItem*)item( iRow );
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
GuiUserListItem* GuiUserListWidget::findListEntryWidgetBySessionId( VxGUID& sessionId )
{
    int iCnt = count();
    for( int iRow = 0; iRow < iCnt; iRow++ )
    {
        GuiUserListItem*  userItem = (GuiUserListItem*)item( iRow );
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
GuiUserListItem* GuiUserListWidget::findListEntryWidgetByOnlineId( VxGUID& onlineId )
{
    if( !onlineId.isVxGUIDValid() )
    {
        LogMsg( LOG_ERROR, "ERROR GuiUserListWidget::findListEntryWidgetByOnlineId: invalid online id" );
        return nullptr;
    }

    int iCnt = count();
    for( int iRow = 0; iRow < iCnt; iRow++ )
    {
        GuiUserListItem*  userItem = (GuiUserListItem*)item( iRow );
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
    onUserListItemClicked( dynamic_cast<GuiUserListItem *>(item) );
}

//============================================================================
void GuiUserListWidget::slotUserListItemClicked( GuiUserListItem* userItem )
{
    if( 300 > m_ClickEventTimer.elapsedMs() ) // avoid duplicate clicks
    {
        return;
    }

    m_ClickEventTimer.startTimer();
    onUserListItemClicked( userItem );
}

//============================================================================
void GuiUserListWidget::slotAvatarButtonClicked( GuiUserListItem* userItem )
{
    if( 300 > m_ClickEventTimer.elapsedMs() ) // avoid duplicate clicks
    {
        return;
    }

    m_ClickEventTimer.startTimer();
    onAvatarButtonClicked(userItem);
}

//============================================================================
void GuiUserListWidget::slotOfferButtonClicked( GuiUserListItem* userItem )
{
    if( 300 > m_ClickEventTimer.elapsedMs()  ) // avoid duplicate clicks
    {
        return;
    }

    m_ClickEventTimer.startTimer();
    onOfferButtonClicked( userItem );
}

//============================================================================
void GuiUserListWidget::slotPushToTalkButtonPressed( GuiUserListItem* userItem )
{
    if( 300 > m_ClickEventTimer.elapsedMs() ) // avoid duplicate clicks
    {
        return;
    }

    m_ClickEventTimer.startTimer();
    onPushToTalkButtonPressed( userItem );
}

//============================================================================
void GuiUserListWidget::slotPushToTalkButtonReleased( GuiUserListItem* userItem )
{
    if( 300 > m_ClickEventTimer.elapsedMs() ) // avoid duplicate clicks
    {
        return;
    }

    m_ClickEventTimer.startTimer();
    onPushToTalkButtonReleased( userItem );
}

//============================================================================
void GuiUserListWidget::slotMenuButtonClicked( GuiUserListItem* userItem )
{
	if( 300 > m_ClickEventTimer.elapsedMs()  ) // avoid duplicate clicks
	{
		return;
	}

	m_ClickEventTimer.startTimer();
    onMenuButtonClicked( userItem );
}

//============================================================================
void GuiUserListWidget::slotUserAdded( GuiUser* user )
{
    updateUser( user );
}

//============================================================================
void GuiUserListWidget::slotUserRemoved( VxGUID onlineId )
{
    removeUser( onlineId );
}

//============================================================================
void GuiUserListWidget::slotUserUpdated( GuiUser* user )
{
    updateUser( user );
}

//============================================================================
void GuiUserListWidget::slotUserOnlineStatus( GuiUser* user )
{
    updateUser( user );
}

//============================================================================
void GuiUserListWidget::slotThumbAdded( GuiThumb* thumb )
{
    updateThumb( thumb );
}

//============================================================================
void GuiUserListWidget::slotThumbUpdated( GuiThumb* thumb )
{
    updateThumb( thumb );
}


//============================================================================
void GuiUserListWidget::slotThumbRemoved( VxGUID thumbId )
{
    // TODO
}

//============================================================================
void GuiUserListWidget::addSessionToList( EHostType hostType, VxGUID& sessionId, GuiUser* hostIdent )
{
    GuiUserSessionBase* userSession = new GuiUserSessionBase( hostType, sessionId, hostIdent, this );

    addOrUpdateSession( userSession );
}

//============================================================================
GuiUserListItem* GuiUserListWidget::addOrUpdateSession( GuiUserSessionBase* userSession )
{
    GuiUserListItem* userItem = findListEntryWidgetBySessionId( userSession->getSessionId() );
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
            LogMsg( LOG_VERBOSE, "add user %s", userSession->getUserIdent()->getOnlineName().c_str() );
            addItem( userItem );
        }
        else
        {
            LogMsg( LOG_VERBOSE, "insert user %s", userSession->getUserIdent()->getOnlineName().c_str() );
            insertItem( 0, (QListWidgetItem *)userItem );
        }

        setItemWidget( (QListWidgetItem *)userItem, (QWidget *)userItem );
    }

    return userItem;
}

//============================================================================
void GuiUserListWidget::onUserListItemClicked( GuiUserListItem* userItem )
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
void GuiUserListWidget::onAvatarButtonClicked( GuiUserListItem* userItem )
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
void GuiUserListWidget::onOfferButtonClicked( GuiUserListItem* userItem )
{
    LogMsg( LOG_DEBUG, "onOfferButtonClicked" );
    if( userItem )
    {
        GuiUserSessionBase* userSession = userItem->getUserSession();
        if( userSession )
        {
            emit signalOfferButtonClicked( userSession, userItem );
        }
    }
}

//============================================================================
void GuiUserListWidget::onPushToTalkButtonPressed( GuiUserListItem* userItem )
{
    LogMsg( LOG_DEBUG, "onPushToTalkButtonPressed" );
    if( userItem )
    {
        GuiUserSessionBase* userSession = userItem->getUserSession();
        if( userSession )
        {
            emit signalPushToTalkButtonPressed( userSession, userItem );
        }
    }
}

//============================================================================
void GuiUserListWidget::onPushToTalkButtonReleased( GuiUserListItem* userItem )
{
    LogMsg( LOG_DEBUG, "onPushToTalkButtonReleased" );
    if( userItem )
    {
        GuiUserSessionBase* userSession = userItem->getUserSession();
        if( userSession )
        {
            emit signalPushToTalkButtonReleased( userSession, userItem );
        }
    }
}

//============================================================================
void GuiUserListWidget::onMenuButtonClicked( GuiUserListItem* userItem )
{
    LogMsg( LOG_DEBUG, "UserListWidget::onMenuButtonClicked" );
    if( userItem )
    {
        GuiUserSessionBase* userSession = userItem->getUserSession();
        if( userSession )
        {
            AppletPopupMenu* popupMenu = dynamic_cast< AppletPopupMenu* >( m_MyApp.launchApplet( eAppletPopupMenu, dynamic_cast< QWidget* >( this->parent() ) ) );
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
}

//============================================================================
bool GuiUserListWidget::isListViewMatch( GuiUser* user )
{
    if( user && !user->isIgnored() )
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
        else if( eUserViewTypeNearby == getUserViewType() )
        {
            return user->isNearby();
        }

        if( user->isDirectConnect() && user->isFriend() )
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
void GuiUserListWidget::onListItemAdded( GuiUserSessionBase* userSession, GuiUserListItem* userItem )
{
    onListItemUpdated( userSession, userItem );
}

//============================================================================
void GuiUserListWidget::onListItemUpdated( GuiUserSessionBase* userSession, GuiUserListItem* userItem )
{
    if( userSession && userItem && userSession->getUserIdent() )
    {
        EPluginType pluginType = ePluginTypeInvalid;
        switch( getUserViewType() )
        {
        case eUserViewTypeGroup:
            pluginType = ePluginTypeClientGroup;
            break;
        case eUserViewTypeChatRoom:
            pluginType = ePluginTypeClientChatRoom;
            break;
        case eUserViewTypeRandomConnect:
            pluginType = ePluginTypeClientRandomConnect;
            break;
        default:
            pluginType = ePluginTypeHostPeerUser;
        }

        if( !userItem->getIsThumbUpdated() )
        {
            VxPushButton* avatarButton = userItem->getAvatarButton();
            GuiUser* user = userSession->getUserIdent();

            QImage	avatarImage;
            bool havAvatarImage = m_ThumbMgr.requestAvatarImage( user, pluginType, avatarImage, true );
            if( havAvatarImage && avatarButton )
            {
                avatarButton->setIconOverrideImage( avatarImage );
                userItem->setIsThumbUpdated( true );
            }
        }

        userItem->updateWidgetFromInfo();
    }
}

//============================================================================
void GuiUserListWidget::updateThumb( GuiThumb* thumb )
{
    GuiUserListItem* userItem = findListEntryWidgetByOnlineId( thumb->getCreatorId() );
    if( userItem )
    {
        userItem->updateThumb( thumb );
    }
}

//============================================================================
void GuiUserListWidget::callbackOnUserUpdated( GuiUser* guiUser )
{
    updateUser( guiUser );
}

//============================================================================
void GuiUserListWidget::callbackPushToTalkStatus( VxGUID& onlineId, EPushToTalkStatus pushToTalkStatus )
{
    GuiUserListItem* userItem = findListEntryWidgetByOnlineId( onlineId );
    if( userItem )
    {
        userItem->callbackPushToTalkStatus( onlineId, pushToTalkStatus );
    }
}