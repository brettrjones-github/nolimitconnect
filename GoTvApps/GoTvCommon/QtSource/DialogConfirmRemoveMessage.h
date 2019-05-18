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
// http://www.gotvptop.net
//============================================================================


#include <CoreLib/AssetDefs.h>
#include <QDialog>
#include "ui_DialogConfirmRemoveMessage.h"

class AssetInfo;
class AppCommon;
class MyIcons;

class DialogConfirmRemoveMessage : public QDialog
{
	Q_OBJECT

public:
	DialogConfirmRemoveMessage( AssetInfo& assetInfo, QWidget * parent = NULL );
	virtual ~DialogConfirmRemoveMessage();

	AppCommon&					getMyApp( void )					{ return m_MyApp; }
	MyIcons&					getMyIcons( void );

	EAssetAction				getAssetActionResult( void )		{ return m_AssetAction; }

protected slots:
	void						slotRemoveAssetButtonClicked( void );
	void						slotShredFileButtonClicked( void );


protected:
	Ui::ConfirmRemoveMessageClass	ui;
	AppCommon&					m_MyApp;
	AssetInfo&					m_AssetInfo;
	EAssetAction				m_AssetAction;

private:
	DialogConfirmRemoveMessage( QWidget * parent = NULL );// dont allow without asset info construct
};
