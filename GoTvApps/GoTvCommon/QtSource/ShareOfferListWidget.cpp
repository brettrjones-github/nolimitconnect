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
#include "ShareOfferListItem.h"
#include "ShareOfferListWidget.h"
#include "GuiOfferClientMgr.h"
#include "GuiOfferHostMgr.h"
#include "GuiOfferSession.h"

#include "MyIcons.h"
#include "AppletPopupMenu.h"
#include "AppGlobals.h"
#include "AppCommon.h"
#include "GuiParams.h"

#include <CoreLib/VxDebug.h>
 
//============================================================================
ShareOfferListWidget::ShareOfferListWidget( QWidget * parent )
: ListWidgetBase( parent )
, m_OfferClientMgr( m_MyApp.getOfferClientMgr() )
{
	QListWidget::setSortingEnabled( true );
	sortItems( Qt::DescendingOrder );

    /*
    connect( &m_OfferMgr, SIGNAL(signalMyIdentUpdated( GuiOffer*)),           this, SLOT(slotMyIdentUpdated( GuiOffer*))) ;

    connect( &m_OfferMgr, SIGNAL(signalOfferAdded( GuiOffer*)),                this, SLOT(slotOfferAdded( GuiOffer*))) ;
    connect( &m_OfferMgr, SIGNAL(signalOfferRemoved( VxGUID)),                this, SLOT(slotOfferRemoved( VxGUID))) ;
    connect( &m_OfferMgr, SIGNAL(signalOfferUpdated( GuiOffer*)),              this, SLOT(slotOfferUpdated( GuiOffer*))) ;
    connect( &m_OfferMgr, SIGNAL(signalOfferOnlineStatus( GuiOffer*,bool)),    this, SLOT(slotOfferOnlineStatus( GuiOffer*,bool))) ;
    */


    //connect( this, SIGNAL(itemClicked(QListWidgetItem *)),          this, SLOT(slotItemClicked(QListWidgetItem *))) ;
    //connect( this, SIGNAL(itemDoubleClicked(QListWidgetItem *)),    this, SLOT(slotItemClicked(QListWidgetItem *))) ;

    setOfferViewType( eOfferViewTypeEverybody );
}

//============================================================================
ShareOfferListItem* ShareOfferListWidget::sessionToWidget( GuiOfferSession* userSession )
{
    ShareOfferListItem* userItem = new ShareOfferListItem(this);
    userItem->setOfferSession( userSession );
    userItem->setSizeHint( userItem->calculateSizeHint() );

    connect( userItem, SIGNAL(signalShareOfferListItemClicked(QListWidgetItem *)),	    this, SLOT(slotShareOfferListItemClicked(QListWidgetItem *)) );
    connect( userItem, SIGNAL(signalAvatarButtonClicked(ShareOfferListItem *)),	        this, SLOT(slotAvatarButtonClicked(ShareOfferListItem *)) );
    connect( userItem, SIGNAL(signalMenuButtonClicked(ShareOfferListItem *)),	            this, SLOT(slotMenuButtonClicked(ShareOfferListItem *)) );
    connect( userItem, SIGNAL(signalFriendshipButtonClicked(ShareOfferListItem *)),		this, SLOT(slotFriendshipButtonClicked(ShareOfferListItem *)) );

    userItem->updateWidgetFromInfo();

    return userItem;
}

//============================================================================
GuiOfferSession* ShareOfferListWidget::widgetToSession( ShareOfferListItem * item )
{
    return item->getOfferSession();
}

//============================================================================
GuiOfferSession * ShareOfferListWidget::findSession( VxGUID& lclSessionId )
{
    int iCnt = count();
    for( int iRow = 0; iRow < iCnt; iRow++ )
    {
        ShareOfferListItem* listItem =  (ShareOfferListItem*)item( iRow );
        if( listItem )
        {
            GuiOfferSession * userSession = listItem->getOfferSession();
            if( userSession && userSession->getSessionId() == lclSessionId )
            {
                return userSession;
            }
        }
    }

    return nullptr;
}

//============================================================================
ShareOfferListItem* ShareOfferListWidget::findListEntryWidgetBySessionId( VxGUID& sessionId )
{
    int iCnt = count();
    for( int iRow = 0; iRow < iCnt; iRow++ )
    {
        ShareOfferListItem*  userItem = (ShareOfferListItem*)item( iRow );
        if( userItem )
        {
            GuiOfferSession * userSession = userItem->getOfferSession();
            if( userSession && ( userSession->getSessionId() == sessionId ) )
            {
                return userItem;
            }
        }
    }

    return nullptr;
}

//============================================================================
ShareOfferListItem* ShareOfferListWidget::findListEntryWidgetByOnlineId( VxGUID& onlineId )
{
    int iCnt = count();
    for( int iRow = 0; iRow < iCnt; iRow++ )
    {
        ShareOfferListItem*  userItem = (ShareOfferListItem*)item( iRow );
        if( userItem )
        {
            GuiOfferSession * userSession = userItem->getOfferSession();
            if( userSession && ( userSession->getMyOnlineId() == onlineId ) )
            {
                return userItem;
            }
        }
    }

    return nullptr;
}

//============================================================================
void ShareOfferListWidget::slotShareOfferListItemClicked( ShareOfferListItem* userItem )
{
	if( 300 > m_ClickEventTimer.elapsedMs()  ) // avoid duplicate clicks
	{
		return;
	}

	m_ClickEventTimer.startTimer();
    onShareOfferListItemClicked(userItem);
}

//============================================================================
void ShareOfferListWidget::slotAvatarButtonClicked( ShareOfferListItem* userItem )
{
    if( 300 > m_ClickEventTimer.elapsedMs() ) // avoid duplicate clicks
    {
        return;
    }

    m_ClickEventTimer.startTimer();
    onAvatarButtonClicked(userItem);
}

//============================================================================
void ShareOfferListWidget::slotMenuButtonClicked( ShareOfferListItem* userItem )
{
	if( 300 > m_ClickEventTimer.elapsedMs()  ) // avoid duplicate clicks
	{
		return;
	}

	m_ClickEventTimer.startTimer();
    onMenuButtonClicked( userItem );
}

//============================================================================
void ShareOfferListWidget::slotFriendshipButtonClicked( ShareOfferListItem* userItem )
{
    if( 300 > m_ClickEventTimer.elapsedMs()  ) // avoid duplicate clicks
    {
        return;
    }

    m_ClickEventTimer.startTimer();
    onFriendshipButtonClicked( userItem );
}

//============================================================================
void ShareOfferListWidget::addOfferToList( EHostType userType, VxGUID& sessionId, GuiOffer * userIdent )
{
    /*
    GuiOfferSession* userSession = new GuiOfferSession( userType, sessionId, userIdent, this );

    addOrUpdateOfferSession( userSession );
    */
}

//============================================================================
ShareOfferListItem* ShareOfferListWidget::addOrUpdateOfferSession( GuiOfferSession* userSession )
{
    ShareOfferListItem* userItem = findListEntryWidgetBySessionId( userSession->getSessionId() );
    if( userItem )
    {
        GuiOfferSession* userOldSession = userItem->getOfferSession();
        if( userOldSession != userSession )
        {
            userItem->setOfferSession( userSession );
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
            LogMsg( LOG_INFO, "add user %s\n", userSession->getOnlineName().c_str() );
            addItem( userItem );
        }
        else
        {
            LogMsg( LOG_INFO, "insert user %s\n", userSession->getOnlineName().c_str() );
            insertItem( 0, (QListWidgetItem *)userItem );
        }

        setItemWidget( (QListWidgetItem *)userItem, (QWidget *)userItem );
    }

    return userItem;
}

//============================================================================
void ShareOfferListWidget::onShareOfferListItemClicked( ShareOfferListItem* userItem )
{
    LogMsg( LOG_DEBUG, "onShareOfferListItemClicked" );
    if( userItem )
    {
        GuiOfferSession* userSession = userItem->getOfferSession();
        if( userSession )
        {
            emit signalShareOfferListItemClicked( userSession, userItem );
        }
    }
    onShareOfferListItemClicked( userItem );
}

//============================================================================
void ShareOfferListWidget::onAvatarButtonClicked( ShareOfferListItem* userItem )
{
    LogMsg( LOG_DEBUG, "onAvatarButtonClicked" );
    if( userItem )
    {
        GuiOfferSession* userSession = userItem->getOfferSession();
        if( userSession )
        {
            emit signalAvatarButtonClicked( userSession, userItem );
        }
    }
}

//============================================================================
void ShareOfferListWidget::onMenuButtonClicked( ShareOfferListItem* userItem )
{
    LogMsg( LOG_DEBUG, "onMenuButtonClicked" );
    if( userItem )
    {
        GuiOfferSession* userSession = userItem->getOfferSession();
        if( userSession )
        {
            emit signalMenuButtonClicked( userSession, userItem );
        }
    }


    /*
    m_SelectedFriend = widgetToOffer( item );
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
void ShareOfferListWidget::onFriendshipButtonClicked( ShareOfferListItem* userItem )
{
    LogMsg( LOG_DEBUG, "onFriendshipButtonClicked" );
    if( userItem )
    {
        GuiOfferSession* userSession = userItem->getOfferSession();
        if( userSession )
        {
            if( userSession && userSession->getUserIdent() )
            {
                launchChangeFriendship( userSession->getUserIdent() );
            }
        }
    }
}

/*



//============================================================================
void ShareOfferListWidget::slotOfferAdded( GuiOffer* user )
{
    updateOffer( user );
}

//============================================================================
void ShareOfferListWidget::slotOfferRemoved( VxGUID onlineId )
{
    removeOffer( onlineId );
}

//============================================================================
void ShareOfferListWidget::slotOfferUpdated( GuiOffer* user )
{
    updateOffer( user );
}

//============================================================================
void ShareOfferListWidget::slotOfferOnlineStatus( GuiOffer* user, bool isOnline )
{
    updateOffer( user );
}
*/

//============================================================================
void ShareOfferListWidget::setOfferViewType( EOfferViewType viewType )
{
    if( viewType != m_OfferViewType )
    {
        m_OfferViewType = viewType;
        refreshList();
    }
}

//============================================================================
void ShareOfferListWidget::refreshList( void )
{
    clearOfferList();
    std::vector<GuiOfferSession *> userList;
    m_OfferClientMgr.lockOfferMgr();

    std::map<VxGUID, GuiOfferSession*>& mgrList = m_OfferClientMgr.getOfferList();
    for( auto iter = mgrList.begin(); iter != mgrList.end(); ++iter )
    {
        if( isListViewMatch( iter->second ) )
        {
            userList.push_back( iter->second );
        }
    }

    m_OfferClientMgr.unlockOfferMgr();
    for( auto user : userList )
    {
        updateOffer( user );
    }
}


//============================================================================
void ShareOfferListWidget::clearOfferList( void )
{
    m_OfferCache.clear();
    for(int i = 0; i < count(); ++i)
    {
        QListWidgetItem* userItem = item(i);
        delete ((ShareOfferListItem *)userItem);
    }

    clear();
}

//============================================================================
bool ShareOfferListWidget::isListViewMatch( GuiOfferSession * user )
{
    /*
    if( user && !user->isIgnored())
    {
        if( user->isMyself() )
        { 
            return getShowMyself();
        }
        else if( eOfferViewTypeEverybody == getOfferViewType() )
        {
            return true;
        }
        else if( eOfferViewTypeFriends == getOfferViewType() )
        {
            return user->isFriend() || user->isAdmin();
        }
        else if( eOfferViewTypeGroup == getOfferViewType() )
        {
            return user->isGroupHosted() && !user->isAnonymous();
        }
        else if( eOfferViewTypeChatRoom == getOfferViewType() )
        {
            return user->isChatRoomHosted() && !user->isAnonymous();
        }
        else if( eOfferViewTypeRandomConnect == getOfferViewType() )
        {
            return user->isRandomConnectHosted() && !user->isAnonymous();
        }

        if( user->isPeerHosted() && user->isFriend() )
        {
            return true;
        }
    }
    else if( user && user->isIgnored() && eOfferViewTypeIgnored == getOfferViewType() )
    {
        return true;
    }
    */

    return false;
}

//============================================================================
void ShareOfferListWidget::updateOffer( GuiOfferSession * userSession )
{
    if( isListViewMatch( userSession ) )
    {
        auto iter = m_OfferCache.find( userSession->getMyOnlineId() );
        if( iter == m_OfferCache.end() )
        {
            /*
            GuiOfferSession * userSession = new GuiOfferSession( user, this );
            if( userSession )
            {
            */
                ShareOfferListItem* userItem = sessionToWidget( userSession );
                if( 0 == count() )
                {
                    LogMsg( LOG_INFO, "add user %s\n", userSession->getOnlineName().c_str() );
                    addItem( userItem );
                }
                else
                {
                    LogMsg( LOG_INFO, "insert user %s\n", userSession->getOnlineName().c_str() );
                    insertItem( 0, (QListWidgetItem *)userItem );
                }

                setItemWidget( (QListWidgetItem *)userItem, (QWidget *)userItem );
                m_OfferCache[userSession->getMyOnlineId()] = userSession;
            //}
        }
        else
        {
            ShareOfferListItem* userItem = findListEntryWidgetByOnlineId( userSession->getMyOnlineId() );
            if( userItem )
            {
                userItem->update();
            }
        }
    }
}

//============================================================================
void ShareOfferListWidget::removeOffer( VxGUID& onlineId )
{
    auto iter = m_OfferCache.find( onlineId );
    if( iter != m_OfferCache.end() )
    {
        m_OfferCache.erase( iter );
        ShareOfferListItem* userItem = findListEntryWidgetByOnlineId( onlineId );
        if( userItem )
        {
            GuiOfferSession * userSession = userItem->getOfferSession();
            delete userItem;
            delete userSession;
        }
    }
}
