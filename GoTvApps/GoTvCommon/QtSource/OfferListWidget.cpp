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
#include "OfferListItem.h"
#include "OfferListWidget.h"
#include "OfferMgr.h"
#include "GuiOfferSession.h"

#include "MyIcons.h"
//#include "PopupMenu.h"
#include "AppGlobals.h"
#include "AppCommon.h"
#include "GuiParams.h"

#include <CoreLib/VxDebug.h>
 
//============================================================================
OfferListWidget::OfferListWidget( QWidget * parent )
: QListWidget( parent )
, m_MyApp( GetAppInstance() )
, m_OfferMgr( m_MyApp.getOfferMgr() )
, m_Engine( m_MyApp.getEngine() )
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
OfferListItem* OfferListWidget::sessionToWidget( GuiOfferSession* userSession )
{
    OfferListItem* userItem = new OfferListItem(this);
    userItem->setOfferSession( userSession );
    userItem->setSizeHint( userItem->calculateSizeHint() );

    connect( userItem, SIGNAL(signalOfferListItemClicked(QListWidgetItem *)),	    this, SLOT(slotOfferListItemClicked(QListWidgetItem *)) );
    connect( userItem, SIGNAL(signalAvatarButtonClicked(OfferListItem *)),	        this, SLOT(slotAvatarButtonClicked(OfferListItem *)) );
    connect( userItem, SIGNAL(signalMenuButtonClicked(OfferListItem *)),	            this, SLOT(slotMenuButtonClicked(OfferListItem *)) );
    connect( userItem, SIGNAL(signalFriendshipButtonClicked(OfferListItem *)),		this, SLOT(slotFriendshipButtonClicked(OfferListItem *)) );

    userItem->updateWidgetFromInfo();

    return userItem;
}

//============================================================================
GuiOfferSession* OfferListWidget::widgetToSession( OfferListItem * item )
{
    return item->getOfferSession();
}

//============================================================================
MyIcons&  OfferListWidget::getMyIcons( void )
{
	return m_MyApp.getMyIcons();
}

//============================================================================
GuiOfferSession * OfferListWidget::findSession( VxGUID& lclSessionId )
{
    int iCnt = count();
    for( int iRow = 0; iRow < iCnt; iRow++ )
    {
        OfferListItem* listItem =  (OfferListItem*)item( iRow );
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
OfferListItem* OfferListWidget::findListEntryWidgetBySessionId( VxGUID& sessionId )
{
    int iCnt = count();
    for( int iRow = 0; iRow < iCnt; iRow++ )
    {
        OfferListItem*  userItem = (OfferListItem*)item( iRow );
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
OfferListItem* OfferListWidget::findListEntryWidgetByOnlineId( VxGUID& onlineId )
{
    int iCnt = count();
    for( int iRow = 0; iRow < iCnt; iRow++ )
    {
        OfferListItem*  userItem = (OfferListItem*)item( iRow );
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
void OfferListWidget::slotOfferListItemClicked( OfferListItem* userItem )
{
	if( 300 > m_ClickEventTimer.elapsedMs()  ) // avoid duplicate clicks
	{
		return;
	}

	m_ClickEventTimer.startTimer();
    onOfferListItemClicked(userItem);
}

//============================================================================
void OfferListWidget::slotAvatarButtonClicked( OfferListItem* userItem )
{
    if( 300 > m_ClickEventTimer.elapsedMs() ) // avoid duplicate clicks
    {
        return;
    }

    m_ClickEventTimer.startTimer();
    onAvatarButtonClicked(userItem);
}

//============================================================================
void OfferListWidget::slotMenuButtonClicked( OfferListItem* userItem )
{
	if( 300 > m_ClickEventTimer.elapsedMs()  ) // avoid duplicate clicks
	{
		return;
	}

	m_ClickEventTimer.startTimer();
    onMenuButtonClicked( userItem );
}

//============================================================================
void OfferListWidget::slotFriendshipButtonClicked( OfferListItem* userItem )
{
    if( 300 > m_ClickEventTimer.elapsedMs()  ) // avoid duplicate clicks
    {
        return;
    }

    m_ClickEventTimer.startTimer();
    onFriendshipButtonClicked( userItem );
}

//============================================================================
void OfferListWidget::addOfferToList( EHostType userType, VxGUID& sessionId, GuiOffer * userIdent )
{
    /*
    GuiOfferSession* userSession = new GuiOfferSession( userType, sessionId, userIdent, this );

    addOrUpdateOfferSession( userSession );
    */
}

//============================================================================
OfferListItem* OfferListWidget::addOrUpdateOfferSession( GuiOfferSession* userSession )
{
    OfferListItem* userItem = findListEntryWidgetBySessionId( userSession->getSessionId() );
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
            LogMsg( LOG_INFO, "add user %s\n", userSession->getOnlineName() );
            addItem( userItem );
        }
        else
        {
            LogMsg( LOG_INFO, "insert user %s\n", userSession->getOnlineName() );
            insertItem( 0, (QListWidgetItem *)userItem );
        }

        setItemWidget( (QListWidgetItem *)userItem, (QWidget *)userItem );
    }

    return userItem;
}

//============================================================================
void OfferListWidget::onOfferListItemClicked( OfferListItem* userItem )
{
    LogMsg( LOG_DEBUG, "onOfferListItemClicked" );
    if( userItem )
    {
        GuiOfferSession* userSession = userItem->getOfferSession();
        if( userSession )
        {
            emit signalOfferListItemClicked( userSession, userItem );
        }
    }
    onOfferListItemClicked( userItem );
}

//============================================================================
void OfferListWidget::onAvatarButtonClicked( OfferListItem* userItem )
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
void OfferListWidget::onMenuButtonClicked( OfferListItem* userItem )
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
void OfferListWidget::onFriendshipButtonClicked( OfferListItem* userItem )
{
    LogMsg( LOG_DEBUG, "onFriendshipButtonClicked" );
    if( userItem )
    {
        GuiOfferSession* userSession = userItem->getOfferSession();
        if( userSession )
        {
            emit signalFriendshipButtonClicked( userSession, userItem );
        }
    }
}

/*



//============================================================================
void OfferListWidget::slotOfferAdded( GuiOffer* user )
{
    updateOffer( user );
}

//============================================================================
void OfferListWidget::slotOfferRemoved( VxGUID onlineId )
{
    removeOffer( onlineId );
}

//============================================================================
void OfferListWidget::slotOfferUpdated( GuiOffer* user )
{
    updateOffer( user );
}

//============================================================================
void OfferListWidget::slotOfferOnlineStatus( GuiOffer* user, bool isOnline )
{
    updateOffer( user );
}
*/

//============================================================================
void OfferListWidget::setOfferViewType( EOfferViewType viewType )
{
    if( viewType != m_OfferViewType )
    {
        m_OfferViewType = viewType;
        refreshList();
    }
}

//============================================================================
void OfferListWidget::refreshList( void )
{
    clearOfferList();
    std::vector<GuiOfferSession *> userList;
    m_OfferMgr.lockOfferMgr();

    std::map<VxGUID, GuiOfferSession*>& mgrList = m_OfferMgr.getOfferList();
    for( auto iter = mgrList.begin(); iter != mgrList.end(); ++iter )
    {
        if( isListViewMatch( iter->second ) )
        {
            userList.push_back( iter->second );
        }
    }

    m_OfferMgr.unlockOfferMgr();
    for( auto user : userList )
    {
        updateOffer( user );
    }
}


//============================================================================
void OfferListWidget::clearOfferList( void )
{
    m_OfferCache.clear();
    for(int i = 0; i < count(); ++i)
    {
        QListWidgetItem* userItem = item(i);
        delete ((OfferListItem *)userItem);
    }

    clear();
}

//============================================================================
bool OfferListWidget::isListViewMatch( GuiOfferSession * user )
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
void OfferListWidget::updateOffer( GuiOfferSession * userSession )
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
                OfferListItem* userItem = sessionToWidget( userSession );
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
                m_OfferCache[userSession->getMyOnlineId()] = userSession;
            //}
        }
        else
        {
            OfferListItem* userItem = findListEntryWidgetByOnlineId( userSession->getMyOnlineId() );
            if( userItem )
            {
                userItem->update();
            }
        }
    }
}

//============================================================================
void OfferListWidget::removeOffer( VxGUID& onlineId )
{
    auto iter = m_OfferCache.find( onlineId );
    if( iter != m_OfferCache.end() )
    {
        m_OfferCache.erase( iter );
        OfferListItem* userItem = findListEntryWidgetByOnlineId( onlineId );
        if( userItem )
        {
            GuiOfferSession * userSession = userItem->getOfferSession();
            delete userItem;
            delete userSession;
        }
    }
}