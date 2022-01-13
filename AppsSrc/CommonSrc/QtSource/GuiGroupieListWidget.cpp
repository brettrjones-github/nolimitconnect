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


#include "GuiGroupieListItem.h"
#include "GuiGroupieListWidget.h"
#include "GuiGroupieListSession.h"

#include "MyIcons.h"
#include "AppletPopupMenu.h"
#include "AppGlobals.h"
#include "AppCommon.h"
#include "GuiParams.h"

#include <CoreLib/VxDebug.h>
 
//============================================================================
GuiGroupieListWidget::GuiGroupieListWidget( QWidget * parent )
: ListWidgetBase( parent )
{
	// QListWidget::setSortingEnabled( true );
	// sortItems( Qt::DescendingOrder );

    connect( this, SIGNAL(itemClicked(QListWidgetItem *)),          this, SLOT(slotItemClicked(QListWidgetItem *))) ;
    connect( this, SIGNAL(itemDoubleClicked(QListWidgetItem *)),    this, SLOT(slotItemClicked(QListWidgetItem *))) ;
}

//============================================================================
GuiGroupieListItem* GuiGroupieListWidget::sessionToWidget( GuiGroupieListSession* groupieSession )
{
    GuiGroupieListItem* hostItem = new GuiGroupieListItem(this);
    hostItem->setSizeHint( QSize( ( int )( GuiParams::getGuiScale() * 200 ),
        ( int )( 62 * GuiParams::getGuiScale() ) ) );

    hostItem->setHostSession( groupieSession );

    connect( hostItem, SIGNAL( signalGuiGroupieListItemClicked( QListWidgetItem  *) ),	    this, SLOT( slotGuiGroupieListItemClicked( QListWidgetItem * ) ) );
    connect( hostItem, SIGNAL( signalIconButtonClicked( GuiGroupieListItem * ) ),	        this, SLOT( slotIconButtonClicked( GuiGroupieListItem * ) ) );
    connect( hostItem, SIGNAL( signalFriendshipButtonClicked( GuiGroupieListItem* ) ),       this, SLOT( slotFriendshipButtonClicked( GuiGroupieListItem* ) ) );
    connect( hostItem, SIGNAL( signalMenuButtonClicked( GuiGroupieListItem * ) ),	        this, SLOT( slotMenuButtonClicked( GuiGroupieListItem * ) ) );
    connect( hostItem, SIGNAL( signalJoinButtonClicked( GuiGroupieListItem * ) ),		    this, SLOT( slotJoinButtonClicked( GuiGroupieListItem * ) ) );
    connect( hostItem, SIGNAL( signalConnectButtonClicked( GuiGroupieListItem* ) ),          this, SLOT( slotConnectButtonClicked( GuiGroupieListItem* ) ) );

    hostItem->updateWidgetFromInfo();

    return hostItem;
}

//============================================================================
GuiGroupieListSession* GuiGroupieListWidget::widgetToSession( GuiGroupieListItem * item )
{
    return item->getHostSession();
}

//============================================================================
GuiGroupieListSession * GuiGroupieListWidget::findSession( VxGUID& lclSessionId )
{
    int iCnt = count();
    for( int iRow = 0; iRow < iCnt; iRow++ )
    {
        GuiGroupieListItem* listItem =  (GuiGroupieListItem*)item( iRow );
        if( listItem )
        {
            GuiGroupieListSession * groupieSession = listItem->getHostSession();
            if( groupieSession && groupieSession->getSessionId() == lclSessionId )
            {
                return groupieSession;
            }
        }
    }

    return nullptr;
}

//============================================================================
GuiGroupieListSession* GuiGroupieListWidget::findSession( GroupieId& hostedId )
{
    int iCnt = count();
    for( int iRow = 0; iRow < iCnt; iRow++ )
    {
        GuiGroupieListItem* listItem = ( GuiGroupieListItem* )item( iRow );
        if( listItem )
        {
            GuiGroupieListSession* groupieSession = listItem->getHostSession();
            if( groupieSession && groupieSession->getGroupieId() == hostedId )
            {
                return groupieSession;
            }
        }
    }

    return nullptr;
}

//============================================================================
GuiGroupieListItem* GuiGroupieListWidget::findListItemWidgetByHostId( GroupieId& hostedId )
{
    int iCnt = count();
    for( int iRow = 0; iRow < iCnt; iRow++ )
    {
        GuiGroupieListItem*  hostItem = (GuiGroupieListItem*)item( iRow );
        if( hostItem )
        {
            GuiGroupieListSession * groupieSession = hostItem->getHostSession();
            if( groupieSession && groupieSession->getGroupieId() == hostedId )
            {
                return hostItem;
            }
        }
    }

    return nullptr;
}

//============================================================================
GuiGroupieListItem* GuiGroupieListWidget::findListItemWidgetByOnlineId( VxGUID& onlineId )
{
    int iCnt = count();
    for( int iRow = 0; iRow < iCnt; iRow++ )
    {
        GuiGroupieListItem*  hostItem = (GuiGroupieListItem*)item( iRow );
        if( hostItem )
        {
            GuiGroupieListSession * groupieSession = hostItem->getHostSession();
            if( groupieSession && groupieSession->getUserIdent()->getMyOnlineId() == onlineId )
            {
                return hostItem;
            }
        }
    }

    return nullptr;
}

//============================================================================
void GuiGroupieListWidget::slotItemClicked( QListWidgetItem * item )
{
	if( 300 < m_ClickEventTimer.elapsedMs()  ) // avoid duplicate clicks
	{
		slotGuiGroupieListItemClicked( (GuiGroupieListItem *)item );
	}
}

//============================================================================
void GuiGroupieListWidget::slotGuiGroupieListItemClicked( QListWidgetItem* hostItem )
{
	if( 300 > m_ClickEventTimer.elapsedMs()  ) // avoid duplicate clicks
	{
		return;
	}

	m_ClickEventTimer.startTimer();
    GuiGroupieListItem* hostWidget = dynamic_cast<GuiGroupieListItem*>(hostItem);
    if( hostWidget )
    {
        onGuiGroupieListItemClicked(hostWidget);
    }
}

//============================================================================
void GuiGroupieListWidget::slotIconButtonClicked( GuiGroupieListItem* hostItem )
{
    if( 300 > m_ClickEventTimer.elapsedMs() ) // avoid duplicate clicks
    {
        return;
    }

    m_ClickEventTimer.startTimer();
    onIconButtonClicked(hostItem);
}

//============================================================================
void GuiGroupieListWidget::slotFriendshipButtonClicked( GuiGroupieListItem* hostItem )
{
    if( 300 > m_ClickEventTimer.elapsedMs() ) // avoid duplicate clicks
    {
        return;
    }

    m_ClickEventTimer.startTimer();
    onFriendshipButtonClicked( hostItem );
}

//============================================================================
void GuiGroupieListWidget::slotMenuButtonClicked( GuiGroupieListItem* hostItem )
{
	if( 300 > m_ClickEventTimer.elapsedMs()  ) // avoid duplicate clicks
	{
		return;
	}

	m_ClickEventTimer.startTimer();
    onMenuButtonClicked( hostItem );
}

//============================================================================
void GuiGroupieListWidget::slotJoinButtonClicked( GuiGroupieListItem* hostItem )
{
    if( 300 > m_ClickEventTimer.elapsedMs()  ) // avoid duplicate clicks
    {
        return;
    }

    m_ClickEventTimer.startTimer();
    onJoinButtonClicked( hostItem );
}

//============================================================================
void GuiGroupieListWidget::slotConnectButtonClicked( GuiGroupieListItem* hostItem )
{
    if( 300 > m_ClickEventTimer.elapsedMs() ) // avoid duplicate clicks
    {
        return;
    }

    m_ClickEventTimer.startTimer();
    onConnectButtonClicked( hostItem );
}

//============================================================================
void GuiGroupieListWidget::updateGroupieList( GroupieId& hostedId, GuiGroupie* guiGroupie, VxGUID& sessionId )
{
    GuiGroupieListSession* groupieSession = findSession( hostedId );
    if( groupieSession )
    {
        groupieSession->setSessionId( sessionId );
    }
    else
    {
        GuiGroupieListSession* groupieSession = new GuiGroupieListSession( hostedId, guiGroupie, this );
        groupieSession->setSessionId( sessionId );

        addOrUpdateHostSession( groupieSession );
    }
}

//============================================================================
GuiGroupieListItem* GuiGroupieListWidget::addOrUpdateHostSession( GuiGroupieListSession* groupieSession )
{
    GuiGroupieListItem* hostItem = findListItemWidgetByHostId( groupieSession->getGroupieId() );
    if( hostItem )
    {
        GuiGroupieListSession* hostOldSession = hostItem->getHostSession();
        if( hostOldSession != groupieSession )
        {
            hostItem->setHostSession( groupieSession );
            if( !hostOldSession->parent() )
            {
                hostOldSession->deleteLater();
            }
        }

        hostItem->updateWidgetFromInfo();
    }
    else
    {
        hostItem = sessionToWidget( groupieSession );
        if( hostItem )
        {
            if( 0 == count() )
            {
                LogMsg( LOG_INFO, "add groupie %s", groupieSession->getUserIdent() ? groupieSession->getUserIdent()->getOnlineName().c_str() : groupieSession->getGroupieTitle().c_str() );
                addItem( hostItem );
            }
            else
            {
                LogMsg( LOG_INFO, "insert groupie %s", groupieSession->getUserIdent() ? groupieSession->getUserIdent()->getOnlineName().c_str() : groupieSession->getGroupieTitle().c_str() );
                insertItem( 0, (QListWidgetItem*)hostItem );
            }

            setItemWidget( (QListWidgetItem*)hostItem, (QWidget*)hostItem );
            if( groupieSession->getUserIdent() )
            {
                VxGUID thumbId = groupieSession->getUserIdent()->getHostThumbId( groupieSession->getHostType(), true );
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
        hostItem->setJoinedState( m_Engine.fromGuiQueryJoinState( groupieSession->getHostType(), groupieSession->getUserIdent()->getNetIdent() ) );
    }

    return hostItem;
}

//============================================================================
void GuiGroupieListWidget::clearHostList( void )
{
    for(int i = 0; i < count(); ++i)
    {
        QListWidgetItem* hostItem = item(i);
        delete ((GuiGroupieListItem *)hostItem);
    }

    clear();
}

//============================================================================
void GuiGroupieListWidget::onGuiGroupieListItemClicked( GuiGroupieListItem* hostItem )
{
    LogMsg( LOG_DEBUG, "onGuiGroupieListItemClicked" );
    onJoinButtonClicked( hostItem );
}

//============================================================================
void GuiGroupieListWidget::onIconButtonClicked( GuiGroupieListItem* hostItem )
{
    LogMsg( LOG_DEBUG, "onIconButtonClicked" );
    GuiGroupieListSession* groupieSession = hostItem->getHostSession();
    if( groupieSession )
    {
        emit signalIconButtonClicked( groupieSession, hostItem );
    }
}

//============================================================================
void GuiGroupieListWidget::onFriendshipButtonClicked( GuiGroupieListItem* hostItem )
{
    LogMsg( LOG_DEBUG, "onFriendshipButtonClicked" );
    if( hostItem )
    {
        GuiGroupieListSession* groupieSession = hostItem->getHostSession();
        if( groupieSession )
        {
            if( groupieSession && groupieSession->getUserIdent() )
            {
                launchChangeFriendship( groupieSession->getUserIdent() );
            }
        }
    }
}

//============================================================================
void GuiGroupieListWidget::onMenuButtonClicked( GuiGroupieListItem* hostItem )
{
    LogMsg( LOG_DEBUG, "GuiGroupieListWidget::onMenuButtonClicked" );
    if( hostItem )
    {
        GuiGroupieListSession* groupieSession = hostItem->getHostSession();
        if( groupieSession )
        {
            // emit signalMenuButtonClicked( groupieSession, hostItem );
            AppletPopupMenu* popupMenu = dynamic_cast<AppletPopupMenu*>(m_MyApp.launchApplet( eAppletPopupMenu, dynamic_cast<QWidget*>(this->parent()) ));
            if( popupMenu )
            {
                popupMenu->showGroupieListSessionMenu( groupieSession );
            }
        }
    }
}

//============================================================================
void GuiGroupieListWidget::onJoinButtonClicked( GuiGroupieListItem* hostItem )
{
    LogMsg( LOG_DEBUG, "onJoinButtonClicked" );
    GuiGroupieListSession* groupieSession = hostItem->getHostSession();
    if( groupieSession )
    {
        emit signalJoinButtonClicked( groupieSession, hostItem );
    }
}

//============================================================================
void GuiGroupieListWidget::onConnectButtonClicked( GuiGroupieListItem* hostItem )
{
    LogMsg( LOG_DEBUG, "onConnectButtonClicked" );
    GuiGroupieListSession* groupieSession = hostItem->getHostSession();
    if( groupieSession )
    {
        emit signalConnectButtonClicked( groupieSession, hostItem );
    }
}
