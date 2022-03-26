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

#include "ToGuiActivityInterface.h"

#include <ptop_src/ptop_engine_src/AssetMgr/AssetInfo.h>

#include <QWidget>
#include <QListWidgetItem>

class AppCommon;
class P2PEngine;
class QProgressBar;
class MyIcons;

class AssetBaseWidget : public QWidget, public QListWidgetItem, public ToGuiActivityInterface
{
	Q_OBJECT

public:
	AssetBaseWidget( AppCommon& appCommon, QWidget *parent=0 );
	virtual ~AssetBaseWidget();

	AppCommon&					getMyApp( void )								{ return m_MyApp; }
	MyIcons&					getMyIcons( void );
	P2PEngine&					getEngine( void )								{ return m_Engine; }

    virtual void				setAssetInfo( AssetBaseInfo& assetInfo )		{ m_AssetInfo = assetInfo; }
    virtual AssetBaseInfo&		getAssetInfo( void )                            { return m_AssetInfo; }
	virtual void				setXferBar( QProgressBar * xferProgressBar );
	virtual QProgressBar *		getXferBar( void )								{ return m_XferProgressBar; }
	virtual void				updateFromAssetInfo( void );

	virtual void				toGuiClientAssetAction( EAssetAction assetAction, VxGUID& assetId, int pos0to100000 );

	virtual void				showSendFail( bool show, bool permissionErr = false ) = 0;
	virtual void				showResendButton( bool show ) = 0;
	virtual void				showShredder( bool show ) = 0;
	virtual void				showXferProgress( bool show ) = 0;
	virtual void				setXferProgress( int xferProgress ) = 0;

	virtual void				onActivityStop( void ) {};

signals:
	void						signalShreddingAsset( AssetBaseWidget * assetWidget );

protected slots:
	virtual void				slotShredAsset( void );
	virtual void				slotResendAsset( void );

protected:
	void						showEvent( QShowEvent * ev );
	void						hideEvent( QHideEvent * ev );
	void						updateProgressBarVisibility( void );

	void						doShowProgress( bool showProgress );
	void						doSetProgress(  int xferProgress );

	AppCommon&					m_MyApp;
	P2PEngine&					m_Engine;
    AssetBaseInfo				m_AssetInfo;
	int							m_AssetSendProgress{ 0 };
	bool						m_CallbacksRequested{ false };
	QProgressBar *				m_XferProgressBar{ nullptr };
	bool						m_ProgressBarShouldBeVisible{ false };
	bool						m_ProgressBarIsVisible{ false };
};
