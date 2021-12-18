#pragma once
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

#include "config_apps.h"

#include "ui_FileItemWidget.h"
#include <QListWidgetItem>

class FileItemInfo;

class FileItemWidget : public QWidget, public QListWidgetItem
{
	Q_OBJECT

public:
	FileItemWidget(QWidget *parent=0);
	virtual ~FileItemWidget();

	void						setXferStatus(QString strStatus);


signals:
	void						signalFileItemClicked( QListWidgetItem * poItemWidget );
	void						signalFileIconButtonClicked( QListWidgetItem * poItemWidget );

public slots:
	void						slotFileIconButtonClicked( void );
	//void						slotThisItemClicked( void );

public:
	Ui::FileItemWidget			ui;
};
