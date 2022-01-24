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


#include "GuiHostedListItem.h"
#include "GuiHostedListWidget.h"
#include "GuiHostedListSession.h"

#include "MyIcons.h"
#include "AppletPopupMenu.h"
#include "AppGlobals.h"
#include "AppCommon.h"
#include "GuiParams.h"

#include <CoreLib/VxDebug.h>
 
//============================================================================
GuiHostedListWidget::GuiHostedListWidget( QWidget * parent )
: ListWidgetBase( parent )
{
	// QListWidget::setSortingEnabled( true );
	// sortItems( Qt::DescendingOrder );

    connect( this, SIGNAL(itemClicked(QListWidgetItem *)),          this, SLOT(slotItemClicked(QListWidgetItem *))) ;
    connect( this, SIGNAL(itemDoubleClicked(QListWidgetItem *)),    this, SLOT(slotItemClicked(QListWidgetItem *))) ;
}

//============================================================================
GuiHostedListItem* GuiHostedListWidget::sessionToWidget( GuiHostedListSession* hostSession )
{
    GuiHostedListItem* hostItem = new GuiHostedListItem(this);
    hostItem->setSizeHint( QSize( ( int )( GuiParams::getGuiScale() * 200 ),
        ( int )( 62 * GuiParams::getGuiScale() ) ) );

    hostItem->setHostSession( hostSession );

    connect( hostItem, SIGNAL( signalGuiHostedListItemClicked( QListWidgetItem  *) ),	    this, SLOT( slotGuiHostedListItemClicked( QListWidgetItem * ) ) );
    connect( hostItem, SIGNAL( signalIconButtonClicked( GuiHostedListItem * ) ),	        this, SLOT( slotIconButtonClicked( GuiHostedListItem * ) ) );
    connect( hostItem, SIGNAL( signalFriendshipButtonClicked( GuiHostedListItem* ) ),       this, SLOT( slotFriendshipButtonClicked( GuiHostedListItem* ) ) );
    connect( hostItem, SIGNAL( signalMenuButtonClicked( GuiHostedListItem * ) ),	        this, SLOT( slotMenuButtonClicked( GuiHostedListItem * ) ) );
    connect( hostItem, SIGNAL( signalJoinButtonClicked( GuiHostedListItem * ) ),		    this, SLOT( slotJoinButtonClicked( GuiHostedListItem * ) ) );
    connect( hostItem, SIGNAL( signalConnectButtonClicked( GuiHostedListItem* ) ),          this, SLOT( slotConnectButtonClicked( GuiHostedListItem* ) ) );

    hostItem->updateWidgetFromInfo();

    return hostItem;
}

//============================================================================
GuiHostedListSession* GuiHostedListWidget::widgetToSession( GuiHostedListItem * item )
{
    return item->getHostSession();
}

//============================================================================
GuiHostedListSession * GuiHostedListWidget::findSession( VxGUID& lclSessionId )
{
    int iCnt = count();
    for( int iRow = 0; iRow < iCnt; iRow++ )
    {
        GuiHostedListItem* listItem =  (GuiHostedListItem*)item( iRow );
        if( listItem )
        {
            GuiHostedListSession * hostSession = listItem->getHostSession();
            if( hostSession && hostSession->getSessionId() == lclSessionId )
            {
                return hostSession;
            }
        }
    }

    return nullptr;
}

//============================================================================
GuiHostedListSession* GuiHostedListWidget::findSession( HostedId& hostedId )
{
    int iCnt = count();
    for( int iRow = 0; iRow < iCnt; iRow++ )
    {
        GuiHostedListItem* listItem = ( GuiHostedListItem* )item( iRow );
        if( listItem )
        {
            GuiHostedListSession* hostSession = listItem->getHostSession();
            if( hostSession && hostSession->getHostedId() == hostedId )
            {
                return hostSession;
            }
        }
    }

    return nullptr;
}

//============================================================================
GuiHostedListItem* GuiHostedListWidget::findListItemWidgetByHostId( HostedId& hostedId )
{
    int iCnt = count();
    for( int iRow = 0; iRow < iCnt; iRow++ )
    {
        GuiHostedListItem*  hostItem = (GuiHostedListItem*)item( iRow );
        if( hostItem )
        {
            GuiHostedListSession * hostSession = hostItem->getHostSession();
            if( hostSession && hostSession->getHostedId() == hostedId )
            {
                return hostItem;
            }
        }
    }

    return nullptr;
}

//============================================================================
GuiHostedListItem* GuiHostedListWidget::findListItemWidgetByOnlineId( VxGUID& onlineId )
{
    int iCnt = count();
    for( int iRow = 0; iRow < iCnt; iRow++ )
    {
        GuiHostedListItem*  hostItem = (GuiHostedListItem*)item( iRow );
        if( hostItem )
        {
            GuiHostedListSession * hostSession = hostItem->getHostSession();
            if( hostSession && hostSession->getUserIdent()->getMyOnlineId() == onlineId )
            {
                return hostItem;
            }
        }
    }

    return nullptr;
}

//============================================================================
void GuiHostedListWidget::slotItemClicked( QListWidgetItem * item )
{
	if( 300 < m_ClickEventTimer.elapsedMs()  ) // avoid duplicate clicks
	{
		slotGuiHostedListItemClicked( (GuiHostedListItem *)item );
	}
}

//============================================================================
void GuiHostedListWidget::slotGuiHostedListItemClicked( QListWidgetItem* hostItem )
{
	if( 300 > m_ClickEventTimer.elapsedMs()  ) // avoid duplicate clicks
	{
		return;
	}

	m_ClickEventTimer.startTimer();
    GuiHostedListItem* hostWidget = dynamic_cast<GuiHostedListItem*>(hostItem);
    if( hostWidget )
    {
        onGuiHostedListItemClicked(hostWidget);
    }
}

//============================================================================
void GuiHostedListWidget::slotIconButtonClicked( GuiHostedListItem* hostItem )
{
    if( 300 > m_ClickEventTimer.elapsedMs() ) // avoid duplicate clicks
    {
        return;
    }

    m_ClickEventTimer.startTimer();
    onIconButtonClicked(hostItem);
}

//============================================================================
void GuiHostedListWidget::slotFriendshipButtonClicked( GuiHostedListItem* hostItem )
{
    if( 300 > m_ClickEventTimer.elapsedMs() ) // avoid duplicate clicks
    {
        return;
    }

    m_ClickEventTimer.startTimer();
    onFriendshipButtonClicked( hostItem );
}

//============================================================================
void GuiHostedListWidget::slotMenuButtonClicked( GuiHostedListItem* hostItem )
{
	if( 300 > m_ClickEventTimer.elapsedMs()  ) // avoid duplicate clicks
	{
		return;
	}

	m_ClickEventTimer.startTimer();
    onMenuButtonClicked( hostItem );
}

//============================================================================
void GuiHostedListWidget::slotJoinButtonClicked( GuiHostedListItem* hostItem )
{
    if( 300 > m_ClickEventTimer.elapsedMs()  ) // avoid duplicate clicks
    {
        return;
    }

    m_ClickEventTimer.startTimer();
    onJoinButtonClicked( hostItem );
}

//============================================================================
void GuiHostedListWidget::slotConnectButtonClicked( GuiHostedListItem* hostItem )
{
    if( 300 > m_ClickEventTimer.elapsedMs() ) // avoid duplicate clicks
    {
        return;
    }

    m_ClickEventTimer.startTimer();
    onConnectButtonClicked( hostItem );
}

//============================================================================
void GuiHostedListWidget::updateHostedList( HostedId& hostedId, GuiHosted* guiHosted, VxGUID& sessionId )
{
    GuiHostedListSession* hostSession = findSession( hostedId );
    if( hostSession )
    {
        hostSession->setSessionId( sessionId );
    }
    else
    {
        GuiHostedListSession* hostSession = new GuiHostedListSession( hostedId, guiHosted, this );
        hostSession->setSessionId( sessionId );

        addOrUpdateHostSession( hostSession );
    }
}

//============================================================================
GuiHostedListItem* GuiHostedListWidget::addOrUpdateHostSession( GuiHostedListSession* hostSession )
{
    GuiHostedListItem* hostItem = findListItemWidgetByHostId( hostSession->getHostedId() );
    if( hostItem )
    {
        GuiHostedListSession* hostOldSession = hostItem->getHostSession();
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
                LogMsg( LOG_INFO, "add host %s", hostSession->getUserIdent() ? hostSession->getUserIdent()->getOnlineName().c_str() : hostSession->getHostTitle().c_str() );
                addItem( hostItem );
            }
            else
            {
                LogMsg( LOG_INFO, "insert host %s", hostSession->getUserIdent() ? hostSession->getUserIdent()->getOnlineName().c_str() : hostSession->getHostTitle().c_str() );
                insertItem( 0, (QListWidgetItem*)hostItem );
            }

            setItemWidget( (QListWidgetItem*)hostItem, (QWidget*)hostItem );
            if( hostSession->getUserIdent() )
            {
                VxGUID thumbId = hostSession->getUserIdent()->getHostThumbId( hostSession->getHostType(), true );
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
    }

    if( hostItem )
    {
        hostItem->setJoinedState( m_Engine.fromGuiQueryJoinState( hostSession->getHostType(), hostSession->getUserIdent()->getNetIdent() ) );
    }

    return hostItem;
}

//============================================================================
void GuiHostedListWidget::clearHostList( void )
{
    for(int i = 0; i < count(); ++i)
    {
        QListWidgetItem* hostItem = item(i);
        delete ((GuiHostedListItem *)hostItem);
    }

    clear();
}

//============================================================================
void GuiHostedListWidget::onGuiHostedListItemClicked( GuiHostedListItem* hostItem )
{
    LogMsg( LOG_DEBUG, "onGuiHostedListItemClicked" );
    onJoinButtonClicked( hostItem );
}

//============================================================================
void GuiHostedListWidget::onIconButtonClicked( GuiHostedListItem* hostItem )
{
    LogMsg( LOG_DEBUG, "onIconButtonClicked" );
    GuiHostedListSession* hostSession = hostItem->getHostSession();
    if( hostSession )
    {
        emit signalIconButtonClicked( hostSession, hostItem );
    }
}

//============================================================================
void GuiHostedListWidget::onFriendshipButtonClicked( GuiHostedListItem* hostItem )
{
    LogMsg( LOG_DEBUG, "onFriendshipButtonClicked" );
    if( hostItem )
    {
        GuiHostedListSession* hostSession = hostItem->getHostSession();
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
void GuiHostedListWidget::onMenuButtonClicked( GuiHostedListItem* hostItem )
{
    LogMsg( LOG_DEBUG, "GuiHostedListWidget::onMenuButtonClicked" );
    if( hostItem )
    {
        GuiHostedListSession* hostSession = hostItem->getHostSession();
        if( hostSession )
        {
            // emit signalMenuButtonClicked( hostSession, hostItem );
            AppletPopupMenu* popupMenu = dynamic_cast<AppletPopupMenu*>(m_MyApp.launchApplet( eAppletPopupMenu, dynamic_cast<QWidget*>(this->parent()) ));
            if( popupMenu )
            {
                popupMenu->showHostedListSessionMenu( hostSession );
            }
        }
    }
}

//============================================================================
void GuiHostedListWidget::onJoinButtonClicked( GuiHostedListItem* hostItem )
{
    LogMsg( LOG_DEBUG, "onJoinButtonClicked" );
    GuiHostedListSession* hostSession = hostItem->getHostSession();
    if( hostSession )
    {
        emit signalJoinButtonClicked( hostSession, hostItem );
    }
}

//============================================================================
void GuiHostedListWidget::onConnectButtonClicked( GuiHostedListItem* hostItem )
{
    LogMsg( LOG_DEBUG, "onConnectButtonClicked" );
    GuiHostedListSession* hostSession = hostItem->getHostSession();
    if( hostSession )
    {
        emit signalConnectButtonClicked( hostSession, hostItem );
    }
}

//============================================================================
void GuiHostedListWidget::callbackGuiHostJoinRequested( GroupieId& groupieId, GuiHostJoin* guiHostJoin )
{
    LogMsg( LOG_VERBOSE, "GuiHostedListWidget::callbackGuiHostJoinRequested" );
    HostedId hostedId = groupieId.getHostedId();
    GuiHostedListItem* listItem = findListItemWidgetByHostId( hostedId );
    if( listItem && listItem->getHostSession() )
    {
        listItem->setJoinedState( guiHostJoin->getJoinState() );
    }
}

//============================================================================
void GuiHostedListWidget::callbackGuiHostJoinGranted( GroupieId& groupieId, GuiHostJoin* guiHostJoin )
{
    LogMsg( LOG_VERBOSE, "GuiHostedListWidget::callbackGuiHostJoinGranted" );
}

//============================================================================
void GuiHostedListWidget::callbackGuiHostUnJoinGranted( GroupieId& groupieId, GuiHostJoin* guiHostJoin )
{
    LogMsg( LOG_VERBOSE, "GuiHostedListWidget::callbackGuiHostUnJoinGranted" );
}

//============================================================================
void GuiHostedListWidget::callbackGuiHostJoinDenied( GroupieId& groupieId, GuiHostJoin* guiHostJoin )
{
    LogMsg( LOG_VERBOSE, "GuiHostedListWidget::callbackGuiHostJoinDenied" );
}

//============================================================================
void GuiHostedListWidget::callbackGuiHostJoinLeaveHost( GroupieId& groupieId )
{
    LogMsg( LOG_VERBOSE, "GuiHostedListWidget::callbackGuiHostJoinLeaveHost" );
}

//============================================================================
void GuiHostedListWidget::callbackGuiHostJoinRemoved( GroupieId& groupieId )
{
    LogMsg( LOG_VERBOSE, "GuiHostedListWidget::callbackGuiHostJoinRemoved" );
}
//============================================================================
//============================================================================

//============================================================================
void GuiHostedListWidget::callbackGuiUserJoinRequested( GroupieId& groupieId, GuiUserJoin* guiUserJoin )
{
    LogMsg( LOG_VERBOSE, "AppletGroupJoin::callbackGuiUserJoinRequested" );
}

//============================================================================
void GuiHostedListWidget::callbackGuiUserJoinGranted( GroupieId& groupieId, GuiUserJoin* guiUserJoin )
{
    LogMsg( LOG_VERBOSE, "AppletGroupJoin::callbackGuiUserJoinGranted" );
}

//============================================================================
void GuiHostedListWidget::callbackGuiUserUnJoinGranted( GroupieId& groupieId, GuiUserJoin* guiUserJoin )
{
    LogMsg( LOG_VERBOSE, "AppletGroupJoin::callbackGuiUserUnJoinGranted" );
}

//============================================================================
void GuiHostedListWidget::callbackGuiUserJoinDenied( GroupieId& groupieId, GuiUserJoin* guiUserJoin )
{
    LogMsg( LOG_VERBOSE, "AppletGroupJoin::callbackGuiUserJoinDenied" );
}

//============================================================================
void GuiHostedListWidget::callbackGuiUserJoinLeaveHost( GroupieId& groupieId )
{
    LogMsg( LOG_VERBOSE, "AppletGroupJoin::callbackGuiUserJoinLeaveHost" );
}

//============================================================================
void GuiHostedListWidget::callbackGuiUserJoinRemoved( GroupieId& groupieId )
{
    LogMsg( LOG_VERBOSE, "AppletGroupJoin::callbackGuiUserJoinRemoved" );
}
