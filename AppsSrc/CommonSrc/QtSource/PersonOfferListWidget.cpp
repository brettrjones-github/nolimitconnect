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
// http://www.nolimitconnect.org
//============================================================================


#include "FriendListEntryWidget.h"
#include "PersonOfferListWidget.h"

#include "MyIcons.h"
#include "AppletPopupMenu.h"
#include "AppGlobals.h"
#include "AppCommon.h"

#include <CoreLib/VxDebug.h>
 
//============================================================================
PersonOfferListWidget::PersonOfferListWidget( QWidget* parent )
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

    connect( this, SIGNAL(itemClicked(QListWidgetItem*)),
                          this, SLOT(slotItemClicked(QListWidgetItem*))) ;

    connect( this, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
                          this, SLOT(slotItemClicked(QListWidgetItem*))) ;

    connect( this, SIGNAL(signalUpdateFriend( GuiUser*,bool)),
                          this, SLOT(slotUpdateFriend( GuiUser*,bool)), Qt::QueuedConnection );

    connect( &m_MyApp, SIGNAL(signalRefreshFriend(VxGUID)),
                          this, SLOT(slotRefreshFriend(VxGUID)), Qt::QueuedConnection )
            ;
    connect( &m_MyApp, SIGNAL(signalAssetViewMsgAction(EAssetAction,VxGUID,int)),
                          this, SLOT(slotAssetViewMsgAction(EAssetAction,VxGUID,int)), Qt::QueuedConnection );
}

//============================================================================
MyIcons&  PersonOfferListWidget::getMyIcons( void )
{
	return m_MyApp.getMyIcons();
}

//============================================================================
void PersonOfferListWidget::slotAssetViewMsgAction( EAssetAction eAssetAction, VxGUID onlineIdIn, int pos0to100000 )
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
void PersonOfferListWidget::setFriendHasUnviewedTextMessages( VxGUID& onlineId, bool hasTextMsgs )
{
	FriendListEntryWidget * poWidget;
	int iIdx = 0;
	while( iIdx < this->count() )
	{
		poWidget = (FriendListEntryWidget *)this->item(iIdx);
		if( poWidget )
		{
			GuiUser* poFriend = poWidget->getUser();
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
void PersonOfferListWidget::setFriendViewType( EFriendViewType eWhichFriendsToShow )
{
	m_eFriendViewType = eWhichFriendsToShow;
}

//============================================================================
EFriendViewType PersonOfferListWidget::getFriendViewType( void )
{
	return m_eFriendViewType;
}

//============================================================================
void PersonOfferListWidget::refreshFriendList( EFriendViewType eWhichFriendsToShow )
{
	this->clear();
	emit signalRefreshFriendList( eWhichFriendsToShow );
}

//============================================================================
void PersonOfferListWidget::slotRefreshFriendList( EFriendViewType eWhichFriendsToShow )
{
	this->clear();
	this->setFriendViewType( eWhichFriendsToShow );
	m_Engine.fromGuiSendContactList( eWhichFriendsToShow, 500 );
}

//============================================================================
void PersonOfferListWidget::updateFriend( GuiUser* guiUser, bool sessionTimeChange ) 
{
	emit signalUpdateFriend( guiUser, sessionTimeChange );
}

//============================================================================
void PersonOfferListWidget::slotItemClicked(QListWidgetItem* item )
{
	if( 300 < m_ClickEventTimer.elapsedMs()  ) // avoid duplicate clicks
	{
		slotFriendListItemClicked( (FriendListEntryWidget *) item );
	}
}

//============================================================================
void PersonOfferListWidget::slotFriendListItemClicked( FriendListEntryWidget* item )
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
			QWidget* parent2 = 0;
			QWidget* parent1 = (QWidget*)this->parent();
			if( parent1 )
			{
				parent2 = (QWidget*)parent1->parent();
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
void PersonOfferListWidget::slotFriendMenuButtonClicked( FriendListEntryWidget* item )
{
	if( 300 > m_ClickEventTimer.elapsedMs()  ) // avoid duplicate clicks
	{
		return;
	}

	m_ClickEventTimer.startTimer();
	m_SelectedFriend = widgetToFriend( item );
	if( m_SelectedFriend )
	{
		emit signalFriendClicked( m_SelectedFriend );

		AppletPopupMenu* popupMenu = dynamic_cast<AppletPopupMenu*>(m_MyApp.launchApplet( eAppletPopupMenu, dynamic_cast<QWidget*>(this->parent()) ));
		if( popupMenu )
		{
			popupMenu->showFriendMenu( m_SelectedFriend );
		}
	}
}

//============================================================================
void PersonOfferListWidget::slotUpdateFriend( GuiUser* guiUser, bool sessionTimeChange ) 
{
	LogMsg( LOG_INFO, "PersonOfferListWidget::slotUpdateFriend  %d", this->count() );
	FriendListEntryWidget * poWidget = findListEntryWidget( guiUser );
	if( poWidget )
	{
        LogMsg( LOG_INFO, "PersonOfferListWidget::onFriendUpdated %s online ? %d widget exists", guiUser->getOnlineName().c_str(), guiUser->isOnline() );
		updateListEntryWidget( poWidget, guiUser );
	}
	else
	{
		FriendListEntryWidget * poFriendItem = friendToWidget( guiUser ); 
		int64_t hisSessionTime = guiUser->getLastSessionTimeMs();
		if( 0 == this->count() )
		{
			insertItem( this->count(), poFriendItem );
            LogMsg( LOG_INFO, "PersonOfferListWidget::onFriendUpdated %s First Item online ? %d inserted %d time %d", guiUser->getOnlineName().c_str(), guiUser->isOnline(), this->count() - 1, hisSessionTime );
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
                    GuiUser* listEntryFriend =listEntryWidget->getUser();
					if( listEntryFriend )
					{
						int64_t listSessionTime = listEntryFriend->getLastSessionTimeMs();
						if( listSessionTime < hisSessionTime )
						{
                            LogMsg( LOG_INFO, "PersonOfferListWidget::onFriendUpdated %s Insert Item online ? %d inserted %d time %d\n", guiUser->getOnlineName().c_str(), guiUser->isOnline(), rowIdx, hisSessionTime  );
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
                LogMsg( LOG_INFO, "PersonOfferListWidget::onFriendUpdated %s Add Item online ? %d inserted %d time %d\n", guiUser->getOnlineName().c_str(), guiUser->isOnline(), this->count(), hisSessionTime );
				insertItem( this->count(), poFriendItem );
			}
		}

		//poFriendItem->setSizeHint( poFriendItem->getSubWidget()->size() );

		//setItemWidget( poFriendItem, poFriendItem->getSubWidget() );

		updateListEntryWidget( poFriendItem, guiUser );
	}
}

//============================================================================
void PersonOfferListWidget::slotRefreshFriend( VxGUID friendId )
{
	int iIdx = 0;
	FriendListEntryWidget * poWidget;
	while( iIdx < this->count() )
	{
		poWidget = (FriendListEntryWidget *)this->item(iIdx);
		if( poWidget )
		{
            GuiUser* poFriend = poWidget->getUser();
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
void PersonOfferListWidget::removeFriend( GuiUser* guiUser )
{
	int iIdx = 0;
	FriendListEntryWidget * poWidget;
	while( iIdx < this->count() )
	{
		poWidget = (FriendListEntryWidget *)this->item(iIdx);
		if( poWidget )
		{
            GuiUser* poFriend = poWidget->getUser();
			if( poFriend && ( poFriend->getMyOnlineId() == guiUser->getMyOnlineId() ) )
			{
                LogMsg( LOG_INFO, "AppCommon::onFriendRemoved %s removing widget idx %d\n", guiUser->getOnlineName().c_str(), iIdx );
				takeItem( iIdx );
				return;
			}
		}

		iIdx++;
	}
}

//============================================================================
void PersonOfferListWidget::updateListEntryBackgroundColor( GuiUser* guiUser, FriendListEntryWidget * poWidget )
{
	if(  guiUser->isOnline() )
	{
		poWidget->getIdentMenuButton()->setIcons( eMyIconMenuNormal );
	}
	else
	{
		poWidget->getIdentMenuButton()->setIcons( eMyIconMenuDisabled );
	}
}

//============================================================================
//!	fill friend into new QListWidgetItem*
FriendListEntryWidget * PersonOfferListWidget::friendToWidget( GuiUser* poFriend )
{
	FriendListEntryWidget * item = new FriendListEntryWidget( this );
    item->setData( Qt::UserRole + 2, QVariant((quint64)(poFriend)) );
    connect( item, SIGNAL(listButtonClicked(FriendListEntryWidget*)), this, SLOT(slotFriendListItemClicked(FriendListEntryWidget*)));

    connect( item, SIGNAL(signalMenuButtonClicked(FriendListEntryWidget*)), this, SLOT(slotFriendMenuButtonClicked(FriendListEntryWidget*)));

	return item;
}

//============================================================================
void PersonOfferListWidget::updateListEntryWidget( FriendListEntryWidget * item, GuiUser* guiUser )
{
	item->updateIdentity( guiUser );
    // display in seconds
	QVariant dispValue( (uint)( guiUser->getLastSessionTimeMs() / 1000 ) );
	item->setData( Qt::DisplayRole, dispValue );
}

//============================================================================
//!	get friend from QListWidgetItem data
GuiUser* PersonOfferListWidget::widgetToFriend( FriendListEntryWidget * item )
{
	return item->getUser();
}

//============================================================================
FriendListEntryWidget * PersonOfferListWidget::findListEntryWidget( GuiUser* guiUser )
{
	int iIdx = 0;
	FriendListEntryWidget * poWidget;
	while( iIdx < this->count() )
	{
		poWidget = (FriendListEntryWidget *)this->item(iIdx);
		if( poWidget )
		{
            GuiUser* poFriend = poWidget->getUser();
			if( poFriend && ( poFriend->getMyOnlineId() == guiUser->getMyOnlineId() ) )
			{
				return poWidget;
			}
		}

		iIdx++;
	}

	return NULL;
}
