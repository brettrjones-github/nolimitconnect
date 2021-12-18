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


#include "HostJoinRequestListItem.h"
#include "HostJoinRequestListWidget.h"
#include "GuiHostJoinSession.h"

#include "MyIcons.h"
#include "AppletPopupMenu.h"
#include "AppGlobals.h"
#include "AppCommon.h"
#include "GuiParams.h"

#include <CoreLib/VxDebug.h>
 
//============================================================================
HostJoinRequestListWidget::HostJoinRequestListWidget( QWidget * parent )
: ListWidgetBase( parent )
{
	QListWidget::setSortingEnabled( true );
	sortItems( Qt::DescendingOrder );

    connect( this, SIGNAL(itemClicked(QListWidgetItem *)),          this, SLOT(slotItemClicked(QListWidgetItem *))) ;
    connect( this, SIGNAL(itemDoubleClicked(QListWidgetItem *)),    this, SLOT(slotItemClicked(QListWidgetItem *))) ;
}

//============================================================================
HostJoinRequestListItem* HostJoinRequestListWidget::sessionToWidget( GuiHostJoinSession* hostSession )
{
    HostJoinRequestListItem* hostItem = new HostJoinRequestListItem(this);
    hostItem->setSizeHint( QSize( ( int )( GuiParams::getGuiScale() * 200 ),
        ( int )( 62 * GuiParams::getGuiScale() ) ) );

    hostItem->setHostSession( hostSession );

    connect( hostItem, SIGNAL( signalHostJoinRequestListItemClicked( QListWidgetItem  *) ),	this, SLOT( slotHostJoinRequestListItemClicked( QListWidgetItem * ) ) );
    connect( hostItem, SIGNAL( signalAvatarButtonClicked( HostJoinRequestListItem * ) ),	        this, SLOT( slotAvatarButtonClicked( HostJoinRequestListItem * ) ) );
    connect( hostItem, SIGNAL( signalFrienshipButtonClicked( HostJoinRequestListItem* ) ), this, SLOT( slotlFriendshipButtonClicked( HostJoinRequestListItem* ) ) );
    connect( hostItem, SIGNAL( signalMenuButtonClicked( HostJoinRequestListItem * ) ),	        this, SLOT( slotMenuButtonClicked( HostJoinRequestListItem * ) ) );
    connect( hostItem, SIGNAL( signalAcceptButtonClicked( HostJoinRequestListItem * ) ),		    this, SLOT( slotAcceptButtonClicked( HostJoinRequestListItem * ) ) );
    connect( hostItem, SIGNAL( signalRejectButtonClicked( HostJoinRequestListItem* ) ),       this, SLOT( slotRejectButtonClicked( HostJoinRequestListItem* ) ) );

    hostItem->updateWidgetFromInfo();

    return hostItem;
}

//============================================================================
GuiHostJoinSession* HostJoinRequestListWidget::widgetToSession( HostJoinRequestListItem * item )
{
    return item->getHostSession();
}

//============================================================================
HostJoinRequestListItem* HostJoinRequestListWidget::findListEntryWidgetByHostId( EHostType hostType, VxGUID& onlineId )
{
    int iCnt = count();
    for( int iRow = 0; iRow < iCnt; iRow++ )
    {
        HostJoinRequestListItem*  hostItem = (HostJoinRequestListItem*)item( iRow );
        if( hostItem )
        {
            GuiHostJoinSession * hostSession = hostItem->getHostSession();
            if( hostSession && hostSession->getOnlineId() == onlineId && hostType == hostSession->getHostType() )
            {
                return hostItem;
            }
        }
    }

    return nullptr;
}

//============================================================================
HostJoinRequestListItem* HostJoinRequestListWidget::findListEntryWidgetByOnlineId( VxGUID& onlineId )
{
    int iCnt = count();
    for( int iRow = 0; iRow < iCnt; iRow++ )
    {
        HostJoinRequestListItem*  hostItem = (HostJoinRequestListItem*)item( iRow );
        if( hostItem )
        {
            GuiHostJoinSession * hostSession = hostItem->getHostSession();
            if( hostSession && hostSession->getOnlineId() == onlineId )
            {
                return hostItem;
            }
        }
    }

    return nullptr;
}

//============================================================================
void HostJoinRequestListWidget::removeHostJoinRequest( VxGUID& onlineId, EHostType hostType )
{
    int iCnt = count();
    for( int iRow = 0; iRow < iCnt; iRow++ )
    {
        HostJoinRequestListItem* hostItem = (HostJoinRequestListItem*)item( iRow );
        if( hostItem )
        {
            GuiHostJoinSession* hostSession = hostItem->getHostSession();
            if( hostSession && hostSession->getOnlineId() == onlineId && hostSession->getHostType() == hostType)
            {
                removeItemWidget( hostItem );  
                hostSession->deleteLater();
                hostItem->deleteLater();
            }
        }
    }
}

//============================================================================
void HostJoinRequestListWidget::slotItemClicked( QListWidgetItem * item )
{
	if( 300 < m_ClickEventTimer.elapsedMs()  ) // avoid duplicate clicks
	{
		slotHostJoinRequestListItemClicked( (HostJoinRequestListItem *)item );
	}
}

//============================================================================
void HostJoinRequestListWidget::slotHostJoinRequestListItemClicked( QListWidgetItem* hostItem )
{
	if( 300 > m_ClickEventTimer.elapsedMs()  ) // avoid duplicate clicks
	{
		return;
	}

	m_ClickEventTimer.startTimer();
    HostJoinRequestListItem* hostWidget = dynamic_cast<HostJoinRequestListItem*>(hostItem);
    if( hostWidget )
    {
        onHostJoinRequestListItemClicked(hostWidget);
    }
}

//============================================================================
void HostJoinRequestListWidget::slotAvatarButtonClicked( HostJoinRequestListItem* hostItem )
{
    if( 300 > m_ClickEventTimer.elapsedMs() ) // avoid duplicate clicks
    {
        return;
    }

    m_ClickEventTimer.startTimer();
    onAvatarButtonClicked(hostItem);
}

//============================================================================
void HostJoinRequestListWidget::slotlFriendshipButtonClicked( HostJoinRequestListItem* hostItem )
{
    if( 300 > m_ClickEventTimer.elapsedMs() ) // avoid duplicate clicks
    {
        return;
    }

    m_ClickEventTimer.startTimer();
    onFriendshipButtonClicked( hostItem );
}

//============================================================================
void HostJoinRequestListWidget::slotMenuButtonClicked( HostJoinRequestListItem* hostItem )
{
	if( 300 > m_ClickEventTimer.elapsedMs()  ) // avoid duplicate clicks
	{
		return;
	}

	m_ClickEventTimer.startTimer();
    onMenuButtonClicked( hostItem );
}

//============================================================================
void HostJoinRequestListWidget::slotAcceptButtonClicked( HostJoinRequestListItem* hostItem )
{
    if( 300 > m_ClickEventTimer.elapsedMs()  ) // avoid duplicate clicks
    {
        return;
    }

    m_ClickEventTimer.startTimer();
    onAcceptButtonClicked( hostItem );
}

//============================================================================
void HostJoinRequestListWidget::slotRejectButtonClicked( HostJoinRequestListItem* hostItem )
{
    if( 300 > m_ClickEventTimer.elapsedMs() ) // avoid duplicate clicks
    {
        return;
    }

    m_ClickEventTimer.startTimer();
    onRejectButtonClicked( hostItem );
}

//============================================================================
void HostJoinRequestListWidget::addOrUpdateHostRequest( EHostType hostType, GuiHostJoin* hostJoin )
{
    GuiHostJoinSession* hostSession = nullptr;
    HostJoinRequestListItem* hostItem = findListEntryWidgetByHostId( hostType, hostJoin->getUser()->getMyOnlineId() );
    if( hostItem )
    {
        hostSession = hostItem->getHostSession();
    }
    
    if( !hostSession )
    {
        hostSession = new GuiHostJoinSession( hostType, hostJoin, this );
    }

    addOrUpdateHostRequestSession( hostSession );
}

//============================================================================
HostJoinRequestListItem* HostJoinRequestListWidget::addOrUpdateHostRequestSession( GuiHostJoinSession* hostSession )
{
    vx_assert( hostSession );
    HostJoinRequestListItem* hostItem = findListEntryWidgetByHostId( hostSession->getHostType(), hostSession->getOnlineId() );
    if( hostItem )
    {
        GuiHostJoinSession* hostOldSession = hostItem->getHostSession();
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
        if( hostItem )
        {
            if( 0 == count() )
            {
                LogMsg( LOG_INFO, "add host %s", hostSession->getUserIdent()->getUser()->getOnlineName().c_str() );
                addItem( hostItem );
            }
            else
            {
                LogMsg( LOG_INFO, "insert host %s", hostSession->getUserIdent()->getUser()->getOnlineName().c_str() );
                insertItem( 0, (QListWidgetItem*)hostItem );
            }

            setItemWidget( (QListWidgetItem*)hostItem, (QWidget*)hostItem );
            VxGUID thumbId = hostSession->getUserIdent()->getUser()->getHostThumbId( hostSession->getHostType(), false );
            GuiThumb* thumb = m_MyApp.getThumbMgr().getThumb( thumbId );
            if( thumb )
            {
                QImage hostIconImage;
                thumb->createImage( hostIconImage );
                VxPushButton* hostImageButton = hostItem->getIdentAvatarButton();
                if( hostImageButton && !hostIconImage.isNull() )
                {
                    hostImageButton->setIconOverrideImage( hostIconImage );
                }
            }           
        }
    }

    if( hostItem )
    {
        hostItem->setJoinedState( m_Engine.fromGuiQueryJoinState( hostSession->getHostType(), hostSession->getUserIdent()->getUser()->getNetIdent() ) );
    }

    return hostItem;
}

//============================================================================
void HostJoinRequestListWidget::clearHostJoinRequestList( void )
{
    for(int i = 0; i < count(); ++i)
    {
        QListWidgetItem* hostItem = item(i);
        delete ((HostJoinRequestListItem *)hostItem);
    }

    clear();
}

//============================================================================
void HostJoinRequestListWidget::onHostJoinRequestListItemClicked( HostJoinRequestListItem* hostItem )
{
    LogMsg( LOG_DEBUG, "onHostJoinRequestListItemClicked" );
    onAcceptButtonClicked( hostItem );
}

//============================================================================
void HostJoinRequestListWidget::onAvatarButtonClicked( HostJoinRequestListItem* hostItem )
{
    LogMsg( LOG_DEBUG, "onAvatarButtonClicked" );
    GuiHostJoinSession* hostSession = hostItem->getHostSession();
    if( hostSession )
    {
        emit signalAvatarButtonClicked( hostSession, hostItem );
    }
}

//============================================================================
void HostJoinRequestListWidget::onFriendshipButtonClicked( HostJoinRequestListItem* hostItem )
{
    LogMsg( LOG_DEBUG, "HostJoinRequestListWidget::onFriendshipButtonClicked" );
    if( hostItem )
    {
        GuiHostJoinSession* hostSession = hostItem->getHostSession();
        if( hostSession )
        {
            //launchChangeFriendship( &hostSession->getUserIdent()->getNetIdent() );
        }
    }
}


//============================================================================
void HostJoinRequestListWidget::onMenuButtonClicked( HostJoinRequestListItem* hostItem )
{
    LogMsg( LOG_DEBUG, "HostJoinRequestListWidget::onMenuButtonClicked" );
    /*
    if( hostItem )
    {
        GuiHostJoinSession* hostSession = hostItem->getHostSession();
        if( hostSession )
        {
            // emit signalMenuButtonClicked( hostSession, hostItem );
            AppletPopupMenu* popupMenu = dynamic_cast<AppletPopupMenu*>(m_MyApp.launchApplet( eAppletPopupMenu, dynamic_cast<QWidget*>(this->parent()) ));
            if( popupMenu )
            {
                popupMenu->showHostSessionMenu( hostSession );
            }
        }
    }
    */
}

//============================================================================
void HostJoinRequestListWidget::onAcceptButtonClicked( HostJoinRequestListItem* hostItem )
{
    LogMsg( LOG_DEBUG, "onAcceptButtonClicked" );
    GuiHostJoinSession* hostSession = hostItem->getHostSession();
    if( hostSession )
    {
        emit signalAcceptButtonClicked( hostSession, hostItem );
    }
}

//============================================================================
void HostJoinRequestListWidget::onRejectButtonClicked( HostJoinRequestListItem* hostItem )
{
    LogMsg( LOG_DEBUG, "onRejectButtonClicked" );
    GuiHostJoinSession* hostSession = hostItem->getHostSession();
    if( hostSession )
    {
        emit signalRejectButtonClicked( hostSession, hostItem );
    }
}
