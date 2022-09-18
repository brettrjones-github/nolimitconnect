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
// http://www.nolimitconnect.org
//============================================================================

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

	void						initializeHistory( void );

public slots:
	void						slotShreddingAsset( AssetBaseWidget * assetWidget );
	void						slotStartupTimeout( void );

protected:
    void						showEvent( QShowEvent * ev ) override;
    void						hideEvent( QHideEvent * ev ) override;

    virtual void				toGuiAssetSessionHistory( AssetBaseInfo& assetInfo ) override;
    virtual void				toGuiAssetAdded( AssetBaseInfo& assetInfo ) override;
	virtual void				toGuiClientAssetAction( EAssetAction assetAction, VxGUID& assetId, int pos0to100000 ) override;


    AssetBaseWidget *			createAssetWidget( AssetBaseInfo * assetInfo );
    int							determinInsertIndex( AssetBaseInfo * assetInfo );
	void						clearHistoryList( void );

	AppCommon&					m_MyApp;
	P2PEngine&					m_Engine;
	GuiUser*					m_MyIdent{ nullptr };
    GuiUser*				    m_HisIdent{ nullptr };
	bool						m_CallbacksRequested{ false };
	QTimer*						m_StartupTimer{ nullptr };
	bool						m_QueryHistoryCalled{ false };
};
