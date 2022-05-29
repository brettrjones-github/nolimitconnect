#pragma once
//============================================================================
// Copyright (C) 2016 Brett R. Jones
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


#include <CoreLib/AssetDefs.h>
#include <QDialog>
#include "ui_DialogConfirmRemoveMessage.h"

class AssetBaseInfo;
class AppCommon;
class MyIcons;

class DialogConfirmRemoveMessage : public QDialog
{
	Q_OBJECT

public:
    DialogConfirmRemoveMessage( AssetBaseInfo& assetInfo, QWidget * parent = NULL );
	virtual ~DialogConfirmRemoveMessage() = default;

	AppCommon&					getMyApp( void )					{ return m_MyApp; }
	MyIcons&					getMyIcons( void );

	EAssetAction				getAssetActionResult( void )		{ return m_AssetAction; }

protected slots:
	void						slotRemoveAssetButtonClicked( void );
	void						slotShredFileButtonClicked( void );


protected:
	Ui::ConfirmRemoveMessageClass	ui;
	AppCommon&					m_MyApp;
    AssetBaseInfo&				m_AssetInfo;
	EAssetAction				m_AssetAction;

private:
	DialogConfirmRemoveMessage( QWidget * parent = NULL );// dont allow without asset info construct
};
