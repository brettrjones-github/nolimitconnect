#pragma once
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

#include "config_apps.h"
#include "ToGuiActivityInterface.h"

#include <QListWidget>

class AppCommon;
class P2PEngine;
class VxNetIdent;
class AssetBaseWidget;
class AssetBaseInfo;

class HistoryListWidget : public QListWidget, public ToGuiActivityInterface
{
	Q_OBJECT

public:
	HistoryListWidget(QWidget *parent=0);
	virtual ~HistoryListWidget();

	void						setIdents( GuiUser * myIdent, GuiUser * hisIdent );
	void						onActivityStop( void );

signals:
	void						signalToGuiClientAssetAction(EAssetAction, VxGUID, int);
    void						signalToGuiAssetAdded( AssetBaseInfo * assetInfo );
    void						signalToGuiSessionHistory( AssetBaseInfo * assetInfo );

public slots:
	void						slotToGuiClientAssetAction( EAssetAction assetAction, VxGUID assetId, int pos0to100000 );
    void						slotToGuiAssetAdded( AssetBaseInfo * assetInfo );
    void						slotToGuiSessionHistory( AssetBaseInfo * assetInfo );

	void						slotShreddingAsset( AssetBaseWidget * assetWidget );


protected:
	void						showEvent( QShowEvent * ev );
	void						hideEvent( QHideEvent * ev );

	virtual void				toGuiClientAssetAction( void * userData, EAssetAction assetAction, VxGUID& assetId, int pos0to100000 );
    virtual void				toGuiAssetSessionHistory( void * userData, AssetBaseInfo * assetInfo );
    virtual void				toGuiAssetAdded( void * userData, AssetBaseInfo * assetInfo );

    AssetBaseWidget *			createAssetWidget( AssetBaseInfo * assetInfo );
    int							determinInsertIndex( AssetBaseInfo * assetInfo );
	void						clearHistoryList( void );

	AppCommon&					m_MyApp;
	P2PEngine&					m_Engine;
    GuiUser *				    m_MyIdent;
    GuiUser *				    m_HisIdent;
	bool						m_CallbacksRequested;
};
