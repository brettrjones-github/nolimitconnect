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
#include "HostListItem.h"
#include "HostListWidget.h"
#include "GuiHostSession.h"

#include "MyIcons.h"
#include "AppletPopupMenu.h"
#include "AppGlobals.h"
#include "AppCommon.h"
#include "GuiParams.h"

#include <CoreLib/VxDebug.h>
 
//============================================================================
HostListWidget::HostListWidget( QWidget * parent )
: ListWidgetBase( parent )
{
	QListWidget::setSortingEnabled( true );
	sortItems( Qt::DescendingOrder );

    connect( this, SIGNAL(itemClicked(QListWidgetItem *)),          this, SLOT(slotItemClicked(QListWidgetItem *))) ;
    connect( this, SIGNAL(itemDoubleClicked(QListWidgetItem *)),    this, SLOT(slotItemClicked(QListWidgetItem *))) ;
}

//============================================================================
HostListItem* HostListWidget::sessionToWidget( GuiHostSession* hostSession )
{
    HostListItem* hostItem = new HostListItem(this);
    hostItem->setSizeHint( QSize( ( int )( GuiParams::getGuiScale() * 200 ),
        ( int )( 62 * GuiParams::getGuiScale() ) ) );

    hostItem->setHostSession( hostSession );

    connect( hostItem, SIGNAL( signalHostListItemClicked( QListWidgetItem  *) ),	this, SLOT( slotHostListItemClicked( QListWidgetItem * ) ) );
    connect( hostItem, SIGNAL( signalIconButtonClicked( HostListItem * ) ),	        this, SLOT( slotIconButtonClicked( HostListItem * ) ) );
    connect( hostItem, SIGNAL( signalFriendshipButtonClicked( HostListItem* ) ), this, SLOT( slotFriendshipButtonClicked( HostListItem* ) ) );
    connect( hostItem, SIGNAL( signalMenuButtonClicked( HostListItem * ) ),	        this, SLOT( slotMenuButtonClicked( HostListItem * ) ) );
    connect( hostItem, SIGNAL( signalJoinButtonClicked( HostListItem * ) ),		    this, SLOT( slotJoinButtonClicked( HostListItem * ) ) );
    connect( hostItem, SIGNAL( signalConnectButtonClicked( HostListItem* ) ),       this, SLOT( slotConnectButtonClicked( HostListItem* ) ) );

    hostItem->updateWidgetFromInfo();

    return hostItem;
}

//============================================================================
GuiHostSession* HostListWidget::widgetToSession( HostListItem * item )
{
    return item->getHostSession();
}

//============================================================================
GuiHostSession * HostListWidget::findSession( VxGUID& lclSessionId )
{
    int iCnt = count();
    for( int iRow = 0; iRow < iCnt; iRow++ )
    {
        HostListItem* listItem =  (HostListItem*)item( iRow );
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
HostListItem* HostListWidget::findListEntryWidgetByHostId( EHostType hostType, VxGUID& onlineId )
{
    int iCnt = count();
    for( int iRow = 0; iRow < iCnt; iRow++ )
    {
        HostListItem*  hostItem = (HostListItem*)item( iRow );
        if( hostItem )
        {
            GuiHostSession * hostSession = hostItem->getHostSession();
            if( hostSession && hostSession->getOnlineId() == onlineId && hostType == hostSession->getHostType() )
            {
                return hostItem;
            }
        }
    }

    return nullptr;
}

//============================================================================
HostListItem* HostListWidget::findListEntryWidgetByOnlineId( VxGUID& onlineId )
{
    int iCnt = count();
    for( int iRow = 0; iRow < iCnt; iRow++ )
    {
        HostListItem*  hostItem = (HostListItem*)item( iRow );
        if( hostItem )
        {
            GuiHostSession * hostSession = hostItem->getHostSession();
            if( hostSession && hostSession->getUserIdent()->getMyOnlineId() == onlineId )
            {
                return hostItem;
            }
        }
    }

    return nullptr;
}

//============================================================================
void HostListWidget::slotItemClicked( QListWidgetItem * item )
{
	if( 300 < m_ClickEventTimer.elapsedMs()  ) // avoid duplicate clicks
	{
		slotHostListItemClicked( (HostListItem *)item );
	}
}

//============================================================================
void HostListWidget::slotHostListItemClicked( QListWidgetItem* hostItem )
{
	if( 300 > m_ClickEventTimer.elapsedMs()  ) // avoid duplicate clicks
	{
		return;
	}

	m_ClickEventTimer.startTimer();
    HostListItem* hostWidget = dynamic_cast<HostListItem*>(hostItem);
    if( hostWidget )
    {
        onHostListItemClicked(hostWidget);
    }
}

//============================================================================
void HostListWidget::slotIconButtonClicked( HostListItem* hostItem )
{
    if( 300 > m_ClickEventTimer.elapsedMs() ) // avoid duplicate clicks
    {
        return;
    }

    m_ClickEventTimer.startTimer();
    onIconButtonClicked(hostItem);
}

//============================================================================
void HostListWidget::slotFriendshipButtonClicked( HostListItem* hostItem )
{
    if( 300 > m_ClickEventTimer.elapsedMs() ) // avoid duplicate clicks
    {
        return;
    }

    m_ClickEventTimer.startTimer();
    onFriendshipButtonClicked( hostItem );
}

//============================================================================
void HostListWidget::slotMenuButtonClicked( HostListItem* hostItem )
{
	if( 300 > m_ClickEventTimer.elapsedMs()  ) // avoid duplicate clicks
	{
		return;
	}

	m_ClickEventTimer.startTimer();
    onMenuButtonClicked( hostItem );
}

//============================================================================
void HostListWidget::slotJoinButtonClicked( HostListItem* hostItem )
{
    if( 300 > m_ClickEventTimer.elapsedMs()  ) // avoid duplicate clicks
    {
        return;
    }

    m_ClickEventTimer.startTimer();
    onJoinButtonClicked( hostItem );
}

//============================================================================
void HostListWidget::slotConnectButtonClicked( HostListItem* hostItem )
{
    if( 300 > m_ClickEventTimer.elapsedMs() ) // avoid duplicate clicks
    {
        return;
    }

    m_ClickEventTimer.startTimer();
    onConnectButtonClicked( hostItem );
}

//============================================================================
void HostListWidget::addHostAndSettingsToList( EHostType hostType, VxGUID& sessionId, VxNetIdent& hostIdent, PluginSetting& pluginSetting )
{
    GuiUser* guiUser = getUserMgr().getUser( hostIdent.getMyOnlineId() );
    if( guiUser )
    {
        GuiHostSession* hostSession = new GuiHostSession( hostType, sessionId, guiUser, pluginSetting, this );

        addOrUpdateHostSession( hostSession );
    }
    else
    {
        LogMsg( LOG_ERROR, "HostListWidget::addHostAndSettingsToList user not found" );
    }
}

//============================================================================
HostListItem* HostListWidget::addOrUpdateHostSession( GuiHostSession* hostSession )
{
    HostListItem* hostItem = findListEntryWidgetByHostId( hostSession->getHostType(), hostSession->getOnlineId() );
    if( hostItem )
    {
        GuiHostSession* hostOldSession = hostItem->getHostSession();
        if( hostOldSession != hostSession )
        {
            hostItem->setHostSession( hostSession );
            if( !hostOldSession->parent() )
            {
                hostOldSession->deleteLater();
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
                LogMsg( LOG_INFO, "add host %s\n", hostSession->getUserIdent()->getOnlineName().c_str() );
                addItem( hostItem );
            }
            else
            {
                LogMsg( LOG_INFO, "insert host %s\n", hostSession->getUserIdent()->getOnlineName().c_str() );
                insertItem( 0, (QListWidgetItem*)hostItem );
            }

            setItemWidget( (QListWidgetItem*)hostItem, (QWidget*)hostItem );
            VxGUID thumbId = hostSession->getUserIdent()->getHostThumbId( hostSession->getHostType(), true );
            GuiThumb* thumb = m_MyApp.getThumbMgr().getThumb( thumbId );
            if( thumb )
            {
                QImage hostIconImage;
                thumb->createImage( hostIconImage );
                VxPushButton* hostImageButton = hostItem->getAvatarButton();
                if( hostImageButton && !hostIconImage.isNull() )
                {
                    hostImageButton->setIconOverrideImage( hostIconImage );
                }
            }           
        }
    }

    if( hostItem )
    {
        hostItem->setJoinedState( m_Engine.fromGuiQueryJoinState( hostSession->getHostType(), hostSession->getUserIdent()->getNetIdent() ) );
    }

    return hostItem;
}

//============================================================================
void HostListWidget::clearHostList( void )
{
    for(int i = 0; i < count(); ++i)
    {
        QListWidgetItem* hostItem = item(i);
        delete ((HostListItem *)hostItem);
    }

    clear();
}

//============================================================================
void HostListWidget::onHostListItemClicked( HostListItem* hostItem )
{
    LogMsg( LOG_DEBUG, "onHostListItemClicked" );
    onJoinButtonClicked( hostItem );
}

//============================================================================
void HostListWidget::onIconButtonClicked( HostListItem* hostItem )
{
    LogMsg( LOG_DEBUG, "onIconButtonClicked" );
    GuiHostSession* hostSession = hostItem->getHostSession();
    if( hostSession )
    {
        emit signalIconButtonClicked( hostSession, hostItem );
    }
}

//============================================================================
void HostListWidget::onFriendshipButtonClicked( HostListItem* hostItem )
{
    LogMsg( LOG_DEBUG, "onFriendshipButtonClicked" );
    if( hostItem )
    {
        GuiHostSession* hostSession = hostItem->getHostSession();
        if( hostSession )
        {
            if( hostSession && hostSession->getUserIdent() )
            {
                launchChangeFriendship( hostSession->getUserIdent() );
            }
        }
    }
}

//============================================================================
void HostListWidget::onMenuButtonClicked( HostListItem* hostItem )
{
    LogMsg( LOG_DEBUG, "HostListWidget::onMenuButtonClicked" );
    if( hostItem )
    {
        GuiHostSession* hostSession = hostItem->getHostSession();
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
void HostListWidget::onJoinButtonClicked( HostListItem* hostItem )
{
    LogMsg( LOG_DEBUG, "onJoinButtonClicked" );
    GuiHostSession* hostSession = hostItem->getHostSession();
    if( hostSession )
    {
        emit signalJoinButtonClicked( hostSession, hostItem );
    }
}

//============================================================================
void HostListWidget::onConnectButtonClicked( HostListItem* hostItem )
{
    LogMsg( LOG_DEBUG, "onConnectButtonClicked" );
    GuiHostSession* hostSession = hostItem->getHostSession();
    if( hostSession )
    {
        emit signalConnectButtonClicked( hostSession, hostItem );
    }
}
