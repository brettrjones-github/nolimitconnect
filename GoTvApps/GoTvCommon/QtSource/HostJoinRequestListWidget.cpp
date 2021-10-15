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
: ListWidgetBase( parent )
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
    connect( hostItem, SIGNAL( signalAvatarButtonClicked( HostJoinRequestListItem * ) ),	        this, SLOT( slotAvatarButtonClicked( HostJoinRequestListItem * ) ) );
    connect( hostItem, SIGNAL( signalFrienshipButtonClicked( HostJoinRequestListItem* ) ), this, SLOT( slotlFriendshipButtonClicked( HostJoinRequestListItem* ) ) );
    connect( hostItem, SIGNAL( signalMenuButtonClicked( HostJoinRequestListItem * ) ),	        this, SLOT( slotMenuButtonClicked( HostJoinRequestListItem * ) ) );
    connect( hostItem, SIGNAL( signalAcceptButtonClicked( HostJoinRequestListItem * ) ),		    this, SLOT( slotAcceptButtonClicked( HostJoinRequestListItem * ) ) );
    connect( hostItem, SIGNAL( signalRejectButtonClicked( HostJoinRequestListItem* ) ),       this, SLOT( slotRejectButtonClicked( HostJoinRequestListItem* ) ) );

    hostItem->updateWidgetFromInfo();

    return hostItem;
}

//============================================================================
GuiHostSession* HostJoinRequestListWidget::widgetToSession( HostJoinRequestListItem * item )
{
    return item->getHostSession();
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
            if( hostSession && hostSession->getUserIdent()->getMyOnlineId() == onlineId )
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
void HostJoinRequestListWidget::addHostAndSettingsToList( EHostType hostType, VxGUID& sessionId, VxNetIdent& hostIdent, PluginSetting& pluginSetting )
{
    GuiUser *guiUser = getUserMgr().getUser( hostIdent.getMyOnlineId() );
    if( guiUser )
    {
        GuiHostSession* hostSession = new GuiHostSession( hostType, sessionId, guiUser, pluginSetting, this );

        addOrUpdateHostSession( hostSession );
    }
    else
    {
        LogMsg( LOG_ERROR, "HostJoinRequestListWidget::addHostAndSettingsToList user not found" );
    }

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
                LogMsg( LOG_INFO, "add host %s\n", hostSession->getUserIdent()->getOnlineName() );
                addItem( hostItem );
            }
            else
            {
                LogMsg( LOG_INFO, "insert host %s\n", hostSession->getUserIdent()->getOnlineName() );
                insertItem( 0, (QListWidgetItem*)hostItem );
            }

            setItemWidget( (QListWidgetItem*)hostItem, (QWidget*)hostItem );
            GuiThumb* thumb = m_MyApp.getThumbMgr().getThumb( hostSession->getUserIdent()->getHostThumbId( hostSession->getHostType(), false ) );
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
    GuiHostSession* hostSession = hostItem->getHostSession();
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
        GuiHostSession* hostSession = hostItem->getHostSession();
        if( hostSession )
        {
            launchChangeFriendship( hostSession->getUserIdent() );
        }
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
}

//============================================================================
void HostJoinRequestListWidget::onAcceptButtonClicked( HostJoinRequestListItem* hostItem )
{
    LogMsg( LOG_DEBUG, "onAcceptButtonClicked" );
    GuiHostSession* hostSession = hostItem->getHostSession();
    if( hostSession )
    {
        emit signalAcceptButtonClicked( hostSession, hostItem );
    }
}

//============================================================================
void HostJoinRequestListWidget::onRejectButtonClicked( HostJoinRequestListItem* hostItem )
{
    LogMsg( LOG_DEBUG, "onRejectButtonClicked" );
    GuiHostSession* hostSession = hostItem->getHostSession();
    if( hostSession )
    {
        emit signalRejectButtonClicked( hostSession, hostItem );
    }
}