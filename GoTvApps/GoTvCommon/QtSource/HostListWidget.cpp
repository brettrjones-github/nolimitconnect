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
#include "HostListEntryWidget.h"
#include "HostListWidget.h"
#include "GuiHostSession.h"

#include "MyIcons.h"
#include "PopupMenu.h"
#include "AppGlobals.h"
#include "AppCommon.h"
#include "GuiParams.h"

#include <CoreLib/VxDebug.h>
 
//============================================================================
HostListWidget::HostListWidget( QWidget * parent )
: QListWidget( parent )
, m_MyApp( GetAppInstance() )
, m_Engine( m_MyApp.getEngine() )
{
	QListWidget::setSortingEnabled( true );
	sortItems( Qt::DescendingOrder );

    connect( this, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(slotItemClicked(QListWidgetItem *))) ;

    connect( this, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(slotItemClicked(QListWidgetItem *))) ;
}

//============================================================================
HostListEntryWidget* HostListWidget::sessionToWidget( GuiHostSession* hostSession )
{
    HostListEntryWidget* hostItem = new HostListEntryWidget(this);
    hostItem->setSizeHint( QSize( ( int )( GuiParams::getGuiScale() * 200 ),
        ( int )( 62 * GuiParams::getGuiScale() ) ) );

    hostItem->setHostSession( hostSession );

    /*
    connect( item, SIGNAL(signalFileXferItemClicked(QListWidgetItem *)),	this, SLOT(slotFileXferItemClicked(QListWidgetItem *)) );
    connect( item, SIGNAL(signalFileIconButtonClicked(QListWidgetItem *)),	this, SLOT(slotFileIconButtonClicked(QListWidgetItem *)) );
    connect( item, SIGNAL(signalCancelButtonClicked(QListWidgetItem *)),	this, SLOT(slotCancelButtonClicked(QListWidgetItem *)) );
    connect( item, SIGNAL(signalPlayButtonClicked(QListWidgetItem *)),		this, SLOT(slotPlayButtonClicked(QListWidgetItem *)) );
    connect( item, SIGNAL(signalLibraryButtonClicked(QListWidgetItem *)),	this, SLOT(slotLibraryButtonClicked(QListWidgetItem *)) );
    connect( item, SIGNAL(signalFileShareButtonClicked(QListWidgetItem *)),	this, SLOT(slotFileShareButtonClicked(QListWidgetItem *)) );
    connect( item, SIGNAL(signalShredButtonClicked(QListWidgetItem *)),		this, SLOT(slotShredButtonClicked(QListWidgetItem *)) );
    */

    hostItem->updateWidgetFromInfo();

    return hostItem;
}

//============================================================================
GuiHostSession* HostListWidget::widgetToSession( HostListEntryWidget * item )
{
    return item->getHostSession();
}

//============================================================================
MyIcons&  HostListWidget::getMyIcons( void )
{
	return m_MyApp.getMyIcons();
}

//============================================================================
GuiHostSession * HostListWidget::findSession( VxGUID& lclSessionId )
{
    int iCnt = count();
    for( int iRow = 0; iRow < iCnt; iRow++ )
    {
        HostListEntryWidget* listItem =  (HostListEntryWidget*)item( iRow );
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
HostListEntryWidget* HostListWidget::findListEntryWidgetBySessionId( VxGUID& sessionId )
{
    int iCnt = count();
    for( int iRow = 0; iRow < iCnt; iRow++ )
    {
        HostListEntryWidget*  hostItem = (HostListEntryWidget*)item( iRow );
        if( hostItem )
        {
            GuiHostSession * hostSession = hostItem->getHostSession();
            if( hostSession && ( hostSession->getSessionId() == sessionId ) )
            {
                return hostItem;
            }
        }
    }

    return nullptr;
}

//============================================================================
HostListEntryWidget* HostListWidget::findListEntryWidgetByOnlineId( VxGUID& onlineId )
{
    int iCnt = count();
    for( int iRow = 0; iRow < iCnt; iRow++ )
    {
        HostListEntryWidget*  hostItem = (HostListEntryWidget*)item( iRow );
        if( hostItem )
        {
            GuiHostSession * hostSession = hostItem->getHostSession();
            if( hostSession && ( hostSession->getHostIdent().getMyOnlineId() == onlineId ) )
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
		slotHostListItemClicked( (HostListEntryWidget *)item );
	}
}

//============================================================================
void HostListWidget::slotHostListItemClicked( HostListEntryWidget* hostItem )
{
	if( 300 > m_ClickEventTimer.elapsedMs()  ) // avoid duplicate clicks
	{
		return;
	}

	m_ClickEventTimer.startTimer();
    GuiHostSession* hostSession  = hostItem->getHostSession();
	if( hostSession )
	{
	}
}

//============================================================================
void HostListWidget::slotHostMenuButtonClicked( HostListEntryWidget* item )
{
	if( 300 > m_ClickEventTimer.elapsedMs()  ) // avoid duplicate clicks
	{
		return;
	}

	m_ClickEventTimer.startTimer();
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
void HostListWidget::addHostAndSettingsToList( EHostType hostType, VxGUID& sessionId, VxNetIdent& hostIdent, PluginSetting& pluginSetting )
{
    GuiHostSession* hostSession = new GuiHostSession( hostType, sessionId, hostIdent, pluginSetting, this );

    addOrUpdateHostSession( hostSession );
}

//============================================================================
HostListEntryWidget* HostListWidget::addOrUpdateHostSession( GuiHostSession* hostSession )
{
    HostListEntryWidget* hostItem = findListEntryWidgetBySessionId( hostSession->getSessionId() );
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
        if( 0 == count() )
        {
            LogMsg( LOG_INFO, "add host %s\n", hostSession->getHostIdent().getOnlineName() );
            addItem( hostItem );
        }
        else
        {
            LogMsg( LOG_INFO, "insert host %s\n", hostSession->getHostIdent().getOnlineName() );
            insertItem( 0, (QListWidgetItem *)hostItem );
        }

        setItemWidget( (QListWidgetItem *)hostItem, (QWidget *)hostItem );
    }

    return hostItem;
}

//============================================================================
void HostListWidget::clearHostList( void )
{
    for(int i = 0; i < count(); ++i)
    {
        QListWidgetItem* hostItem = item(i);
        delete ((HostListEntryWidget *)hostItem);
    }

    clear();
}
