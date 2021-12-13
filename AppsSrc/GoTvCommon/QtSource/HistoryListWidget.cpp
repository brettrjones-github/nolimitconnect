//============================================================================
// Copyright (C) 2015 Brett R. Jones
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
#include "HistoryListWidget.h"
#include "AppCommon.h"
#include "DialogConfirmRemoveMessage.h"

#include "AssetTextWidget.h"
#include "AssetFaceWidget.h"
#include "AssetPhotoWidget.h"
#include "AssetVoiceWidget.h"
#include "AssetVideoWidget.h"

#include <ptop_src/ptop_engine_src/AssetMgr/AssetInfo.h>
#include <CoreLib/VxGlobals.h>

//============================================================================
HistoryListWidget::HistoryListWidget(QWidget *parent)
: QListWidget( parent )
, m_MyApp( GetAppInstance() )
, m_Engine( GetAppInstance().getEngine() )
, m_MyIdent( 0 )
, m_HisIdent( 0 )
, m_CallbacksRequested( false )
{
	connect( this, SIGNAL(signalToGuiClientAssetAction(EAssetAction, VxGUID, int)), this, SLOT(slotToGuiClientAssetAction(EAssetAction, VxGUID, int)), Qt::QueuedConnection );
	connect( this, SIGNAL( signalToGuiSessionHistory(AssetBaseInfo *)), this, SLOT(slotToGuiSessionHistory( AssetBaseInfo*)), Qt::QueuedConnection );
	connect( this, SIGNAL(signalToGuiAssetAdded( AssetBaseInfo*)), this, SLOT(slotToGuiAssetAdded( AssetBaseInfo*)), Qt::QueuedConnection );
}

//============================================================================
HistoryListWidget::~HistoryListWidget()
{
	if( m_CallbacksRequested && ( false == VxIsAppShuttingDown() ) )
	{
		m_MyApp.wantToGuiActivityCallbacks( this, this, false );
		m_CallbacksRequested = false;
	}
}

//============================================================================
void HistoryListWidget::showEvent(QShowEvent * showEvent)
{
	QListWidget::showEvent(showEvent);
	if( ( false == m_CallbacksRequested )
		&& ( false == VxIsAppShuttingDown() ) )
	{
		m_CallbacksRequested = true;
		m_MyApp.wantToGuiActivityCallbacks( this, this, true );
	}
}

//============================================================================
void HistoryListWidget::hideEvent( QHideEvent * ev )
{
	if( m_CallbacksRequested )
	{
		m_CallbacksRequested = false;
		m_MyApp.wantToGuiActivityCallbacks( this, this, false );
	}

	QListWidget::hideEvent( ev );
}

//============================================================================
void HistoryListWidget::onActivityStop( void )
{
	for(int i = 0; i < this->count(); ++i)
	{
		QListWidgetItem* item = this->item(i);
		((AssetBaseWidget *)item)->onActivityStop();
	}
}

//============================================================================
void HistoryListWidget::setIdents( GuiUser * myIdent, GuiUser * hisIdent )
{ 
	m_MyIdent = myIdent; 
	m_HisIdent = hisIdent; 
	m_MyApp.wantToGuiActivityCallbacks( this, this, true );
	m_Engine.fromGuiQuerySessionHistory( hisIdent->getMyOnlineId() );
}

//============================================================================
void HistoryListWidget::toGuiClientAssetAction( void * userData, EAssetAction assetAction, VxGUID& assetId, int pos0to100000 )
{
	Q_UNUSED( userData );
	VxGUID assetIdPro( assetId );
	emit signalToGuiClientAssetAction( assetAction, assetIdPro, pos0to100000 );
}

//============================================================================
void HistoryListWidget::toGuiAssetSessionHistory( void * userData, AssetBaseInfo * assetInfo )
{
	Q_UNUSED( userData );
	emit signalToGuiSessionHistory( assetInfo );
}

//============================================================================
void HistoryListWidget::toGuiAssetAdded( void * userData, AssetBaseInfo * assetInfo )
{
	Q_UNUSED( userData );
	emit signalToGuiAssetAdded( assetInfo );
}

//============================================================================
void HistoryListWidget::slotToGuiClientAssetAction( EAssetAction assetAction, VxGUID assetId, int pos0to100000 )
{
}

//============================================================================
void HistoryListWidget::slotShreddingAsset( AssetBaseWidget * assetWidget )
{
	for(int i = 0; i < this->count(); ++i)
	{
		QListWidgetItem* item = this->item(i);
		if( ((AssetBaseWidget *)item) == assetWidget )
		{
			// found the widget being shredded;
            AssetBaseInfo& assetInfo = assetWidget->getAssetInfo();
			DialogConfirmRemoveMessage dlg( assetInfo, this );
			dlg.exec();
			EAssetAction removeAssetAction = dlg.getAssetActionResult();
			if( eAssetActionShreadFile == removeAssetAction )
			{
				m_MyApp.playSound( eSndDefPaperShredder );
				m_Engine.fromGuiAssetAction( eAssetActionShreadFile, assetInfo, 0 );
				//removeItemWidget( item );
				takeItem( i );
			}
			else if( eAssetActionRemoveFromAssetMgr  == removeAssetAction )
			{
				m_MyApp.playSound( eSndDefNeckSnap );
				m_Engine.fromGuiAssetAction( eAssetActionRemoveFromAssetMgr, assetInfo, 0 );
				//removeItemWidget( item );
				takeItem( i );
			}
			else
			{
				m_MyApp.playSound( eSndDefCancel );
			}

			break;
		}
	}
	
	update();
	repaint();
}

//============================================================================
void HistoryListWidget::slotToGuiSessionHistory( AssetBaseInfo * assetInfo )
{
	slotToGuiAssetAdded( assetInfo );
}

//============================================================================
void HistoryListWidget::slotToGuiAssetAdded( AssetBaseInfo * assetInfo )
{
	if( m_HisIdent 
		&& ( m_HisIdent->getMyOnlineId() == assetInfo->getHistoryId() ) )
	{
		// this asset belongs in our history
		// create appropriate widget type for asset type
		AssetBaseWidget * assetWidget = createAssetWidget( assetInfo );
		if( assetWidget )
		{
			connect( assetWidget, SIGNAL(signalShreddingAsset(AssetBaseWidget *)), this, SLOT(slotShreddingAsset(AssetBaseWidget *)) );
			int insertAtIndex = determinInsertIndex( assetInfo );
			if( 0 <= insertAtIndex )
			{
				this->insertItem( insertAtIndex, (QListWidgetItem *)assetWidget );
			}
			else
			{
				this->addItem( (QListWidgetItem *)assetWidget );
			}

			this->setItemWidget( (QListWidgetItem *)assetWidget, (QWidget *)assetWidget );
		}
	}
}

//============================================================================
int  HistoryListWidget::determinInsertIndex( AssetBaseInfo * assetInfo )
{
	int insertIdx = -1;
	for( int i = 0; i < this->count(); i++ )
	{
		AssetBaseWidget * assetWidget = (AssetBaseWidget *)this->item(i);
		if( assetInfo->getCreationTime() <= assetWidget->getAssetInfo().getCreationTime() )
		{
			insertIdx = i;
			break;
		}
	}

	return insertIdx;
}

//============================================================================
AssetBaseWidget * HistoryListWidget::createAssetWidget( AssetBaseInfo * assetInfo )
{
	AssetBaseWidget * assetWidget = 0;
	switch( assetInfo->getAssetType() )
	{
	case eAssetTypePhoto:
		assetWidget = new AssetPhotoWidget( this );
		break;
	case eAssetTypeAudio:
		assetWidget = new AssetVoiceWidget( this );
		break;
	case eAssetTypeVideo:
		assetWidget = new AssetVideoWidget( this );
		break;
	case eAssetTypeChatText:
		assetWidget = new AssetTextWidget( this );
		break;
	case eAssetTypeChatFace:
		assetWidget = new AssetFaceWidget( this );
		break;

	default:
		break;
	}

	if( assetWidget )
	{
		assetWidget->setAssetInfo( *assetInfo );
	}

	return assetWidget;
}


//============================================================================
void HistoryListWidget::clearHistoryList( void )
{
	for(int i = 0; i < this->count(); ++i)
	{
		QListWidgetItem* item = this->item(i);
		delete ((AssetBaseWidget *)item);
	}

	this->clear();
}
