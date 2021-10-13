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
#include "HostJoinRequestListItem.h"
#include "HostJoinRequestListWidget.h"
#include "GuiHostSession.h"

#include "MyIcons.h"
#include "AppletPopupMenu.h"
#include "AppGlobals.h"
#include "AppCommon.h"
#include "GuiParams.h"

#include <CoreLib/VxDebug.h>
 
//============================================================================
HostJoinRequestListWidget::HostJoinRequestListWidget( QWidget * parent )
: QListWidget( parent )
, m_MyApp( GetAppInstance() )
, m_Engine( m_MyApp.getEngine() )
{
	QListWidget::setSortingEnabled( true );
	sortItems( Qt::DescendingOrder );

    connect( this, SIGNAL(itemClicked(QListWidgetItem *)),          this, SLOT(slotItemClicked(QListWidgetItem *))) ;
    connect( this, SIGNAL(itemDoubleClicked(QListWidgetItem *)),    this, SLOT(slotItemClicked(QListWidgetItem *))) ;
}

//============================================================================
HostJoinRequestListItem* HostJoinRequestListWidget::sessionToWidget( GuiHostSession* hostSession )
{
    HostJoinRequestListItem* hostItem = new HostJoinRequestListItem(this);
    hostItem->setSizeHint( QSize( ( int )( GuiParams::getGuiScale() * 200 ),
        ( int )( 62 * GuiParams::getGuiScale() ) ) );

    hostItem->setHostSession( hostSession );

    connect( hostItem, SIGNAL( signalHostJoinRequestListItemClicked( QListWidgetItem  *) ),	this, SLOT( slotHostJoinRequestListItemClicked( QListWidgetItem * ) ) );
    connect( hostItem, SIGNAL( signalIconButtonClicked( HostJoinRequestListItem * ) ),	        this, SLOT( slotIconButtonClicked( HostJoinRequestListItem * ) ) );
    connect( hostItem, SIGNAL( signalMenuButtonClicked( HostJoinRequestListItem * ) ),	        this, SLOT( slotMenuButtonClicked( HostJoinRequestListItem * ) ) );
    connect( hostItem, SIGNAL( signalJoinButtonClicked( HostJoinRequestListItem * ) ),		    this, SLOT( slotJoinButtonClicked( HostJoinRequestListItem * ) ) );
    connect( hostItem, SIGNAL( signalConnectButtonClicked( HostJoinRequestListItem* ) ),       this, SLOT( slotConnectButtonClicked( HostJoinRequestListItem* ) ) );

    hostItem->updateWidgetFromInfo();

    return hostItem;
}

//============================================================================
GuiHostSession* HostJoinRequestListWidget::widgetToSession( HostJoinRequestListItem * item )
{
    return item->getHostSession();
}

//============================================================================
MyIcons&  HostJoinRequestListWidget::getMyIcons( void )
{
	return m_MyApp.getMyIcons();
}

//============================================================================
GuiHostSession * HostJoinRequestListWidget::findSession( VxGUID& lclSessionId )
{
    int iCnt = count();
    for( int iRow = 0; iRow < iCnt; iRow++ )
    {
        HostJoinRequestListItem* listItem =  (HostJoinRequestListItem*)item( iRow );
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
HostJoinRequestListItem* HostJoinRequestListWidget::findListEntryWidgetByHostId( EHostType hostType, VxGUID& onlineId )
{
    int iCnt = count();
    for( int iRow = 0; iRow < iCnt; iRow++ )
    {
        HostJoinRequestListItem*  hostItem = (HostJoinRequestListItem*)item( iRow );
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
HostJoinRequestListItem* HostJoinRequestListWidget::findListEntryWidgetByOnlineId( VxGUID& onlineId )
{
    int iCnt = count();
    for( int iRow = 0; iRow < iCnt; iRow++ )
    {
        HostJoinRequestListItem*  hostItem = (HostJoinRequestListItem*)item( iRow );
        if( hostItem )
        {
            GuiHostSession * hostSession = hostItem->getHostSession();
            if( hostSession && hostSession->getHostIdent().getMyOnlineId() == onlineId )
            {
                return hostItem;
            }
        }
    }

    return nullptr;
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
void HostJoinRequestListWidget::slotIconButtonClicked( HostJoinRequestListItem* hostItem )
{
    if( 300 > m_ClickEventTimer.elapsedMs() ) // avoid duplicate clicks
    {
        return;
    }

    m_ClickEventTimer.startTimer();
    slotIconButtonClicked(hostItem);
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
void HostJoinRequestListWidget::slotJoinButtonClicked( HostJoinRequestListItem* hostItem )
{
    if( 300 > m_ClickEventTimer.elapsedMs()  ) // avoid duplicate clicks
    {
        return;
    }

    m_ClickEventTimer.startTimer();
    onJoinButtonClicked( hostItem );
}

//============================================================================
void HostJoinRequestListWidget::slotConnectButtonClicked( HostJoinRequestListItem* hostItem )
{
    if( 300 > m_ClickEventTimer.elapsedMs() ) // avoid duplicate clicks
    {
        return;
    }

    m_ClickEventTimer.startTimer();
    onConnectButtonClicked( hostItem );
}

//============================================================================
void HostJoinRequestListWidget::addHostAndSettingsToList( EHostType hostType, VxGUID& sessionId, VxNetIdent& hostIdent, PluginSetting& pluginSetting )
{
    GuiHostSession* hostSession = new GuiHostSession( hostType, sessionId, hostIdent, pluginSetting, this );

    addOrUpdateHostSession( hostSession );
}

//============================================================================
HostJoinRequestListItem* HostJoinRequestListWidget::addOrUpdateHostSession( GuiHostSession* hostSession )
{
    HostJoinRequestListItem* hostItem = findListEntryWidgetByHostId( hostSession->getHostType(), hostSession->getOnlineId() );
    if( hostItem )
    {
        GuiHostSession* hostOldSession = hostItem->getHostSession();
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
                LogMsg( LOG_INFO, "add host %s\n", hostSession->getHostIdent().getOnlineName() );
                addItem( hostItem );
            }
            else
            {
                LogMsg( LOG_INFO, "insert host %s\n", hostSession->getHostIdent().getOnlineName() );
                insertItem( 0, (QListWidgetItem*)hostItem );
            }

            setItemWidget( (QListWidgetItem*)hostItem, (QWidget*)hostItem );
            GuiThumb* thumb = m_MyApp.getThumbMgr().getThumb( hostSession->getHostIdent().getThumbId( hostSession->getHostType() ) );
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
        hostItem->setJoinedState( m_Engine.fromGuiQueryJoinState( hostSession->getHostType(), hostSession->getHostIdent() ) );
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
    onJoinButtonClicked( hostItem );
}

//============================================================================
void HostJoinRequestListWidget::onIconButtonClicked( HostJoinRequestListItem* hostItem )
{
    LogMsg( LOG_DEBUG, "onIconButtonClicked" );
    GuiHostSession* hostSession = hostItem->getHostSession();
    if( hostSession )
    {
        emit signalIconButtonClicked( hostSession, hostItem );
    }
}

//============================================================================
void HostJoinRequestListWidget::onMenuButtonClicked( HostJoinRequestListItem* hostItem )
{
    LogMsg( LOG_DEBUG, "HostJoinRequestListWidget::onMenuButtonClicked" );
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
void HostJoinRequestListWidget::onJoinButtonClicked( HostJoinRequestListItem* hostItem )
{
    LogMsg( LOG_DEBUG, "onJoinButtonClicked" );
    GuiHostSession* hostSession = hostItem->getHostSession();
    if( hostSession )
    {
        emit signalJoinButtonClicked( hostSession, hostItem );
    }
}

//============================================================================
void HostJoinRequestListWidget::onConnectButtonClicked( HostJoinRequestListItem* hostItem )
{
    LogMsg( LOG_DEBUG, "onConnectButtonClicked" );
    GuiHostSession* hostSession = hostItem->getHostSession();
    if( hostSession )
    {
        emit signalConnectButtonClicked( hostSession, hostItem );
    }
}