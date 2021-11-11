//============================================================================
// Copyright (C) 2013 Brett R. Jones 
// Issued to MIT style license by Brett R. Jones in 2017
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
#include "FriendListEntryWidget.h"
#include "FriendListWidget.h"

#include "MyIcons.h"
#include "AppletPopupMenu.h"
#include "AppGlobals.h"
#include "AppCommon.h"

#include <CoreLib/VxDebug.h>
 
//============================================================================
FriendListWidget::FriendListWidget( QWidget * parent )
: QListWidget( parent )
, m_MyApp( GetAppInstance() )
, m_Engine( m_MyApp.getEngine() )
, m_eFriendViewType( eFriendViewEverybody )
, m_SelectedFriend( NULL )
, m_ViewingOnlineId()
, m_IsCurrentlyViewing( false )
{
	QListWidget::setSortingEnabled( true );
	sortItems( Qt::DescendingOrder );
	connect( this, SIGNAL(signalRefreshFriendList(EFriendViewType)), 
             this, SLOT(slotRefreshFriendList(EFriendViewType)), Qt::QueuedConnection );

    connect( this, SIGNAL(itemClicked(QListWidgetItem *)),
                          this, SLOT(slotItemClicked(QListWidgetItem *))) ;

    connect( this, SIGNAL(itemDoubleClicked(QListWidgetItem *)),
                          this, SLOT(slotItemClicked(QListWidgetItem *))) ;

    connect( this, SIGNAL(signalUpdateFriend( GuiUser *,bool)),
                          this, SLOT(slotUpdateFriend( GuiUser *,bool)), Qt::QueuedConnection );

    connect( &m_MyApp, SIGNAL(signalRefreshFriend(VxGUID)),
                          this, SLOT(slotRefreshFriend(VxGUID)), Qt::QueuedConnection )
            ;
    connect( &m_MyApp, SIGNAL(signalAssetViewMsgAction(EAssetAction,VxGUID,int)),
                          this, SLOT(slotAssetViewMsgAction(EAssetAction,VxGUID,int)), Qt::QueuedConnection );
}

//============================================================================
MyIcons&  FriendListWidget::getMyIcons( void )
{
	return m_MyApp.getMyIcons();
}

//============================================================================
void FriendListWidget::slotAssetViewMsgAction( EAssetAction eAssetAction, VxGUID onlineIdIn, int pos0to100000 )
{
	VxGUID onlineId;
	uint64_t hiPart = onlineIdIn.getVxGUIDHiPart();
	uint64_t loPart = onlineIdIn.getVxGUIDLoPart();
	onlineId.setVxGUID( hiPart, loPart );

	if( eAssetActionRxViewingMsg == eAssetAction )
	{
		m_IsCurrentlyViewing = pos0to100000 ? true : false;
		if( m_IsCurrentlyViewing )
		{
			m_ViewingOnlineId = onlineId;
			setFriendHasUnviewedTextMessages( m_ViewingOnlineId, false );
		}

		return;
	}

	if( eAssetActionRxNotifyNewMsg == eAssetAction )
	{
		if( m_IsCurrentlyViewing 
			&& ( onlineId == m_ViewingOnlineId ) )
		{
			// user is viewing messages from this contact
			setFriendHasUnviewedTextMessages( onlineId, false );
			return;
		}

		setFriendHasUnviewedTextMessages( onlineId, true );
	}
}

//============================================================================
void FriendListWidget::setFriendHasUnviewedTextMessages( VxGUID& onlineId, bool hasTextMsgs )
{
	FriendListEntryWidget * poWidget;
	int iIdx = 0;
	while( iIdx < this->count() )
	{
		poWidget = (FriendListEntryWidget *)this->item(iIdx);
		if( poWidget )
		{
			GuiUser * poFriend = poWidget->getUser();
			if( poFriend
				&& ( poFriend->getMyOnlineId() == onlineId ) )
			{
				// found the widget
				poFriend->setHasTextOffers( hasTextMsgs );
				updateListEntryWidget( poWidget, poFriend );
				return;
			}
		}

		iIdx++;
	}
}

//============================================================================
void FriendListWidget::setFriendViewType( EFriendViewType eWhichFriendsToShow )
{
	m_eFriendViewType = eWhichFriendsToShow;
}

//============================================================================
EFriendViewType FriendListWidget::getFriendViewType( void )
{
	return m_eFriendViewType;
}

//============================================================================
void FriendListWidget::refreshFriendList( EFriendViewType eWhichFriendsToShow )
{
	this->clear();
	emit signalRefreshFriendList( eWhichFriendsToShow );
}

//============================================================================
void FriendListWidget::slotRefreshFriendList( EFriendViewType eWhichFriendsToShow )
{
	this->clear();
	this->setFriendViewType( eWhichFriendsToShow );
	m_Engine.fromGuiSendContactList( eWhichFriendsToShow, 500 );
}

//============================================================================
void FriendListWidget::updateFriend( GuiUser * netIdent, bool sessionTimeChange ) 
{
	emit signalUpdateFriend( netIdent, sessionTimeChange );
}

//============================================================================
void FriendListWidget::slotItemClicked(QListWidgetItem * item )
{
	if( 300 < m_ClickEventTimer.elapsedMs()  ) // avoid duplicate clicks
	{
		slotFriendListItemClicked( (FriendListEntryWidget *) item );
	}
}

//============================================================================
void FriendListWidget::slotFriendListItemClicked( FriendListEntryWidget* item )
{
	if( 300 > m_ClickEventTimer.elapsedMs()  ) // avoid duplicate clicks
	{
		return;
	}

	m_ClickEventTimer.startTimer();
	m_SelectedFriend = widgetToFriend( item );
	if( m_SelectedFriend )
	{
		if( m_SelectedFriend->isMyAccessAllowedFromHim( ePluginTypeMessenger ) )
		{
			QWidget * parent2 = 0;
			QWidget * parent1 = (QWidget *)this->parent();
			if( parent1 )
			{
				parent2 = (QWidget *)parent1->parent();
			}

			m_MyApp.offerToFriendPluginSession( m_SelectedFriend, ePluginTypeMessenger, parent2 ? parent2 : parent1 );
			emit signalFriendClicked( m_SelectedFriend );
		}
		else
		{
			QString warnTitle = QObject::tr("Insufficient Permission Level");
			QString warmPermission = warnTitle + QObject::tr(" To Access Plugin ") + DescribePluginType( ePluginTypeMessenger );
			QMessageBox::warning(this, QObject::tr("Insufficient Permission Level "), warmPermission );
		}
	}
}

//============================================================================
void FriendListWidget::slotFriendMenuButtonClicked( FriendListEntryWidget* item )
{
	if( 300 > m_ClickEventTimer.elapsedMs()  ) // avoid duplicate clicks
	{
		return;
	}

	m_ClickEventTimer.startTimer();
	m_SelectedFriend = widgetToFriend( item );
	if( m_SelectedFriend )
	{
		AppletPopupMenu* popupMenu = dynamic_cast<AppletPopupMenu*>(m_MyApp.launchApplet(eAppletPopupMenu, dynamic_cast<QWidget*>(this->parent())));
		// emit signalFriendClicked( m_SelectedFriend );
        if( popupMenu )
        {
            popupMenu->showFriendMenu( m_SelectedFriend );
        }
	}
}

//============================================================================
void FriendListWidget::slotUpdateFriend( GuiUser * netIdent, bool sessionTimeChange ) 
{
	LogMsg( LOG_INFO, "FriendListWidget::slotUpdateFriend  %d\n", this->count() );
	FriendListEntryWidget * poWidget = findListEntryWidget( netIdent );
	if( poWidget )
	{
        LogMsg( LOG_INFO, "FriendListWidget::onFriendUpdated %s online ? %d widget exists\n", netIdent->getOnlineName().c_str(), netIdent->isOnline() );
		updateListEntryWidget( poWidget, netIdent );
	}
	else
	{
		FriendListEntryWidget * poFriendItem = friendToWidget( netIdent ); 
		int64_t hisSessionTime = netIdent->getLastSessionTimeMs();
		if( 0 == this->count() )
		{
			insertItem( this->count(), poFriendItem );
            LogMsg( LOG_INFO, "FriendListWidget::onFriendUpdated %s First Item online ? %d inserted %d time %d\n", netIdent->getOnlineName().c_str(), netIdent->isOnline(), this->count() - 1, hisSessionTime );
			//addItem( poFriendItem );
		}
		else
		{
			bool wasInserted = false;
			int rowIdx = 0;
			FriendListEntryWidget * listEntryWidget;
			while( rowIdx < this->count() )
			{
				listEntryWidget = (FriendListEntryWidget *)this->item( rowIdx );
				if( listEntryWidget )
				{
                    GuiUser * listEntryFriend = listEntryWidget->getUser();
					if( listEntryFriend )
					{
						int64_t listSessionTime = listEntryFriend->getLastSessionTimeMs();
						if( listSessionTime < hisSessionTime )
						{
                            LogMsg( LOG_INFO, "FriendListWidget::onFriendUpdated %s Insert Item online ? %d inserted %d time %d\n", netIdent->getOnlineName().c_str(), netIdent->isOnline(), rowIdx, hisSessionTime  );
							insertItem( rowIdx, poFriendItem );
							wasInserted = true;
							break;
						}
					}
				}

				rowIdx++;
			}

			if( false == wasInserted )
			{
				//addItem( poFriendItem );
                LogMsg( LOG_INFO, "FriendListWidget::onFriendUpdated %s Add Item online ? %d inserted %d time %d\n", netIdent->getOnlineName().c_str(), netIdent->isOnline(), this->count(), hisSessionTime );
				insertItem( this->count(), poFriendItem );
			}
		}

		//poFriendItem->setSizeHint( poFriendItem->getSubWidget()->size() );

		//setItemWidget( poFriendItem, poFriendItem->getSubWidget() );

		updateListEntryWidget( poFriendItem, netIdent );
	}
}

//============================================================================
void FriendListWidget::slotRefreshFriend( VxGUID friendId )
{
	int iIdx = 0;
	FriendListEntryWidget * poWidget;
	while( iIdx < this->count() )
	{
		poWidget = (FriendListEntryWidget *)this->item(iIdx);
		if( poWidget )
		{
            GuiUser * poFriend = poWidget->getUser();
			if( poFriend
				&& ( poFriend->getMyOnlineId().getVxGUIDHiPart() == friendId.getVxGUIDHiPart() )
				&& ( poFriend->getMyOnlineId().getVxGUIDLoPart() == friendId.getVxGUIDLoPart() ) )
			{
				// found the widget
				updateListEntryWidget( poWidget, poFriend );
				return;
			}
		}

		iIdx++;
	}
}

//============================================================================
//! called when friend in list is removed
void FriendListWidget::removeFriend( GuiUser * netIdent )
{
	int iIdx = 0;
	FriendListEntryWidget * poWidget;
	while( iIdx < this->count() )
	{
		poWidget = (FriendListEntryWidget *)this->item(iIdx);
		if( poWidget )
		{
            GuiUser * poFriend = poWidget->getUser();
			if( poFriend && ( poFriend->getMyOnlineId() == netIdent->getMyOnlineId() ) )
			{
                LogMsg( LOG_INFO, "AppCommon::onFriendRemoved %s removing widget idx %d\n", netIdent->getOnlineName().c_str(), iIdx );
				takeItem( iIdx );
				return;
			}
		}

		iIdx++;
	}
}

//============================================================================
//!	fill friend into new QListWidgetItem *
FriendListEntryWidget * FriendListWidget::friendToWidget( GuiUser * poFriend )
{
	FriendListEntryWidget * item = new FriendListEntryWidget( this );
    item->setData( Qt::UserRole + 2, QVariant((quint64)(poFriend)) );
    connect( item, SIGNAL(listButtonClicked(FriendListEntryWidget*)), this, SLOT(slotFriendListItemClicked(FriendListEntryWidget*)));

    connect( item, SIGNAL(signalMenuButtonClicked(FriendListEntryWidget*)), this, SLOT(slotFriendMenuButtonClicked(FriendListEntryWidget*)));

	return item;
}

//============================================================================
void FriendListWidget::updateListEntryWidget( FriendListEntryWidget * item, GuiUser * netIdent )
{
	item->updateIdentity( netIdent );

    // display in seconds
	QVariant dispValue( (uint)( netIdent->getLastSessionTimeMs() / 1000 ) );
	item->setData( Qt::DisplayRole, dispValue );

}

//============================================================================
//!	get friend from QListWidgetItem data
GuiUser * FriendListWidget::widgetToFriend( FriendListEntryWidget * item )
{
	return item->getUser();
}

//============================================================================
FriendListEntryWidget * FriendListWidget::findListEntryWidget( GuiUser * netIdent )
{
	int iIdx = 0;
	FriendListEntryWidget * poWidget;
	while( iIdx < this->count() )
	{
		poWidget = (FriendListEntryWidget *)this->item(iIdx);
		if( poWidget )
		{
            GuiUser * poFriend = poWidget->getUser();
			if( poFriend && ( poFriend->getMyOnlineId() == netIdent->getMyOnlineId() ) )
			{
				return poWidget;
			}
		}

		iIdx++;
	}

	return NULL;
}
