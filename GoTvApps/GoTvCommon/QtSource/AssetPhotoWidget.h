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
// http://www.gotvptop.net
//============================================================================

#include "AssetBaseWidget.h"
#include "ui_AssetPhotoWidget.h"

class AssetPhotoWidget : public AssetBaseWidget
{
	Q_OBJECT

public:
	AssetPhotoWidget( QWidget * parent = 0 );
	AssetPhotoWidget( AppCommon& appCommon, QWidget * parent = 0 );

	virtual void				setAssetInfo( AssetInfo& assetInfo );
	virtual void				showSendFail( bool show, bool permissionErr = false );
	virtual void				showResendButton( bool show );
	virtual void				showShredder( bool show );
	virtual void				showXferProgress( bool show );
	virtual void				setXferProgress( int sendProgress );

protected slots:
	void						slotAssetWasClicked( void );

protected:
	void						initAssetPhotoWidget( void );
	//=== vars ===//

	Ui::AssetPhotoWidget	ui;
};
