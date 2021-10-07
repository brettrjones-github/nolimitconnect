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
#include "CallListItem.h"
#include "CallListWidget.h"
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
CallListWidget::CallListWidget( QWidget * parent )
: QListWidget( parent )
, m_MyApp( GetAppInstance() )
, m_OfferClientMgr( m_MyApp.getOfferClientMgr() )
, m_Engine( m_MyApp.getEngine() )
{
	QListWidget::setSortingEnabled( true );
	sortItems( Qt::DescendingOrder );

    /*
    connect( &m_OfferMgr, SIGNAL(signalMyIdentUpdated( GuiCall*)),           this, SLOT(slotMyIdentUpdated( GuiCall*))) ;

    connect( &m_OfferMgr, SIGNAL(signalCallAdded( GuiCall*)),                this, SLOT(slotCallAdded( GuiCall*))) ;
    connect( &m_OfferMgr, SIGNAL(signalCallRemoved( VxGUID)),                this, SLOT(slotCallRemoved( VxGUID))) ;
    connect( &m_OfferMgr, SIGNAL(signalCallUpdated( GuiCall*)),              this, SLOT(slotCallUpdated( GuiCall*))) ;
    connect( &m_OfferMgr, SIGNAL(signalCallOnlineStatus( GuiCall*,bool)),    this, SLOT(slotCallOnlineStatus( GuiCall*,bool))) ;
    */

    //connect( this, SIGNAL(itemClicked(QListWidgetItem *)),          this, SLOT(slotItemClicked(QListWidgetItem *))) ;
    //connect( this, SIGNAL(itemDoubleClicked(QListWidgetItem *)),    this, SLOT(slotItemClicked(QListWidgetItem *))) ;

    setCallViewType( eCallViewTypeEverybody );
}

//============================================================================
CallListItem* CallListWidget::sessionToWidget( GuiOfferSession* userSession )
{
    CallListItem* userItem = new CallListItem(this);
    userItem->setCallSession( userSession );
    userItem->setSizeHint( userItem->calculateSizeHint() );

    connect( userItem, SIGNAL(signalCallListItemClicked(QListWidgetItem *)),	    this, SLOT(slotCallListItemClicked(QListWidgetItem *)) );
    connect( userItem, SIGNAL(signalAvatarButtonClicked(CallListItem *)),	        this, SLOT(slotAvatarButtonClicked(CallListItem *)) );
    connect( userItem, SIGNAL(signalMenuButtonClicked(CallListItem *)),	            this, SLOT(slotMenuButtonClicked(CallListItem *)) );
    connect( userItem, SIGNAL(signalFriendshipButtonClicked(CallListItem *)),		this, SLOT(slotFriendshipButtonClicked(CallListItem *)) );

    userItem->updateWidgetFromInfo();

    return userItem;
}

//============================================================================
GuiOfferSession* CallListWidget::widgetToSession( CallListItem * item )
{
    return item->getCallSession();
}

//============================================================================
MyIcons&  CallListWidget::getMyIcons( void )
{
	return m_MyApp.getMyIcons();
}

//============================================================================
GuiOfferSession * CallListWidget::findSession( VxGUID& lclSessionId )
{
    int iCnt = count();
    for( int iRow = 0; iRow < iCnt; iRow++ )
    {
        CallListItem* listItem =  (CallListItem*)item( iRow );
        if( listItem )
        {
            GuiOfferSession * userSession = listItem->getCallSession();
            if( userSession && userSession->getSessionId() == lclSessionId )
            {
                return userSession;
            }
        }
    }

    return nullptr;
}

//============================================================================
CallListItem* CallListWidget::findListEntryWidgetBySessionId( VxGUID& sessionId )
{
    int iCnt = count();
    for( int iRow = 0; iRow < iCnt; iRow++ )
    {
        CallListItem*  userItem = (CallListItem*)item( iRow );
        if( userItem )
        {
            GuiOfferSession * userSession = userItem->getCallSession();
            if( userSession && ( userSession->getSessionId() == sessionId ) )
            {
                return userItem;
            }
        }
    }

    return nullptr;
}

//============================================================================
CallListItem* CallListWidget::findListEntryWidgetByOnlineId( VxGUID& onlineId )
{
    int iCnt = count();
    for( int iRow = 0; iRow < iCnt; iRow++ )
    {
        CallListItem*  userItem = (CallListItem*)item( iRow );
        if( userItem )
        {
            GuiOfferSession * userSession = userItem->getCallSession();
            if( userSession && ( userSession->getMyOnlineId() == onlineId ) )
            {
                return userItem;
            }
        }
    }

    return nullptr;
}

//============================================================================
void CallListWidget::slotCallListItemClicked( CallListItem* userItem )
{
	if( 300 > m_ClickEventTimer.elapsedMs()  ) // avoid duplicate clicks
	{
		return;
	}

	m_ClickEventTimer.startTimer();
    onCallListItemClicked(userItem);
}

//============================================================================
void CallListWidget::slotAvatarButtonClicked( CallListItem* userItem )
{
    if( 300 > m_ClickEventTimer.elapsedMs() ) // avoid duplicate clicks
    {
        return;
    }

    m_ClickEventTimer.startTimer();
    onAvatarButtonClicked(userItem);
}

//============================================================================
void CallListWidget::slotMenuButtonClicked( CallListItem* userItem )
{
	if( 300 > m_ClickEventTimer.elapsedMs()  ) // avoid duplicate clicks
	{
		return;
	}

	m_ClickEventTimer.startTimer();
    onMenuButtonClicked( userItem );
}

//============================================================================
void CallListWidget::slotFriendshipButtonClicked( CallListItem* userItem )
{
    if( 300 > m_ClickEventTimer.elapsedMs()  ) // avoid duplicate clicks
    {
        return;
    }

    m_ClickEventTimer.startTimer();
    onFriendshipButtonClicked( userItem );
}

/*
//============================================================================
void CallListWidget::addCallToList( EHostType userType, VxGUID& sessionId, GuiCall * userIdent )
{

    GuiOfferSession* userSession = new GuiOfferSession( userType, sessionId, userIdent, this );

    addOrUpdateCallSession( userSession );
   
}
*/

//============================================================================
CallListItem* CallListWidget::addOrUpdateCallSession( GuiOfferSession* userSession )
{
    CallListItem* userItem = findListEntryWidgetBySessionId( userSession->getSessionId() );
    if( userItem )
    {
        GuiOfferSession* userOldSession = userItem->getCallSession();
        if( userOldSession != userSession )
        {
            userItem->setCallSession( userSession );
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
void CallListWidget::onCallListItemClicked( CallListItem* userItem )
{
    LogMsg( LOG_DEBUG, "onCallListItemClicked" );
    if( userItem )
    {
        GuiOfferSession* userSession = userItem->getCallSession();
        if( userSession )
        {
            emit signalCallListItemClicked( userSession, userItem );
        }
    }
    onCallListItemClicked( userItem );
}

//============================================================================
void CallListWidget::onAvatarButtonClicked( CallListItem* userItem )
{
    LogMsg( LOG_DEBUG, "onAvatarButtonClicked" );
    if( userItem )
    {
        GuiOfferSession* userSession = userItem->getCallSession();
        if( userSession )
        {
            emit signalAvatarButtonClicked( userSession, userItem );
        }
    }
}

//============================================================================
void CallListWidget::onMenuButtonClicked( CallListItem* userItem )
{
    LogMsg( LOG_DEBUG, "onMenuButtonClicked" );
    if( userItem )
    {
        GuiOfferSession* userSession = userItem->getCallSession();
        if( userSession )
        {
            emit signalMenuButtonClicked( userSession, userItem );
        }
    }


    /*
    m_SelectedFriend = widgetToCall( item );
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
void CallListWidget::onFriendshipButtonClicked( CallListItem* userItem )
{
    LogMsg( LOG_DEBUG, "onFriendshipButtonClicked" );
    if( userItem )
    {
        GuiOfferSession* userSession = userItem->getCallSession();
        if( userSession )
        {
            emit signalFriendshipButtonClicked( userSession, userItem );
        }
    }
}

/*
//============================================================================
void CallListWidget::slotMyIdentUpdated( GuiOfferSession* user )
{
    if( getShowMyself() )
    {
        updateCall( user );
    }
}


//============================================================================
void CallListWidget::slotCallAdded( GuiCall* user )
{
    updateCall( user );
}

//============================================================================
void CallListWidget::slotCallRemoved( VxGUID onlineId )
{
    removeCall( onlineId );
}

//============================================================================
void CallListWidget::slotCallUpdated( GuiCall* user )
{
    updateCall( user );
}

//============================================================================
void CallListWidget::slotCallOnlineStatus( GuiCall* user, bool isOnline )
{
    updateCall( user );
}
*/

//============================================================================
void CallListWidget::setCallViewType( ECallViewType viewType )
{
    if( viewType != m_CallViewType )
    {
        m_CallViewType = viewType;
        refreshList();
    }
}

//============================================================================
void CallListWidget::refreshList( void )
{
    clearCallList();
    std::vector<GuiOfferSession *> userList;
    m_OfferClientMgr.lockOfferMgr();

    std::map<VxGUID, GuiOfferSession*>& mgrList = m_OfferClientMgr.getCallList();
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
        updateCall( user );
    }
}


//============================================================================
void CallListWidget::clearCallList( void )
{
    m_CallCache.clear();
    for(int i = 0; i < count(); ++i)
    {
        QListWidgetItem* userItem = item(i);
        delete ((CallListItem *)userItem);
    }

    clear();
}

//============================================================================
bool CallListWidget::isListViewMatch( GuiOfferSession * user )
{
    /*
    if( user && !user->isIgnored())
    {
        if( user->isMyself() )
        { 
            return getShowMyself();
        }
        else if( eCallViewTypeEverybody == getCallViewType() )
        {
            return true;
        }
        else if( eCallViewTypeFriends == getCallViewType() )
        {
            return user->isFriend() || user->isAdmin();
        }
        else if( eCallViewTypeGroup == getCallViewType() )
        {
            return user->isGroupHosted() && !user->isAnonymous();
        }
        else if( eCallViewTypeChatRoom == getCallViewType() )
        {
            return user->isChatRoomHosted() && !user->isAnonymous();
        }
        else if( eCallViewTypeRandomConnect == getCallViewType() )
        {
            return user->isRandomConnectHosted() && !user->isAnonymous();
        }

        if( user->isPeerHosted() && user->isFriend() )
        {
            return true;
        }
    }
    else if( user && user->isIgnored() && eCallViewTypeIgnored == getCallViewType() )
    {
        return true;
    }
    */

    return false;
}

//============================================================================
void CallListWidget::updateCall( GuiOfferSession * offerSession )
{
    if( isListViewMatch( offerSession ) )
    {
        auto iter = m_CallCache.find( offerSession->getMyOnlineId() );
        if( iter == m_CallCache.end() )
        {
            /*
            GuiOfferSession * userSession = new GuiOfferSession( user, this );
            if( userSession )
            {*/
                CallListItem* userItem = sessionToWidget( offerSession );
                if( 0 == count() )
                {
                    LogMsg( LOG_INFO, "add user %s\n", offerSession->getOnlineName() );
                    addItem( userItem );
                }
                else
                {
                    LogMsg( LOG_INFO, "insert user %s\n", offerSession->getOnlineName() );
                    insertItem( 0, (QListWidgetItem *)userItem );
                }

                setItemWidget( (QListWidgetItem *)userItem, (QWidget *)userItem );
                m_CallCache[offerSession->getMyOnlineId()] = offerSession;
            //}
        }
        else
        {
            CallListItem* userItem = findListEntryWidgetByOnlineId( offerSession->getMyOnlineId() );
            if( userItem )
            {
                userItem->update();
            }
        }
    }
}

//============================================================================
void CallListWidget::removeCall( VxGUID& onlineId )
{
    auto iter = m_CallCache.find( onlineId );
    if( iter != m_CallCache.end() )
    {
        m_CallCache.erase( iter );
        CallListItem* userItem = findListEntryWidgetByOnlineId( onlineId );
        if( userItem )
        {
            GuiOfferSession * userSession = userItem->getCallSession();
            delete userItem;
            delete userSession;
        }
    }
}