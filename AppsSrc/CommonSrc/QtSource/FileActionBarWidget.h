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

#include "ui_FileActionBarWidget.h"
#include <QListWidgetItem>
#include <QFrame>

class FileItemInfo;
class AppCommon;

class FileActionBarWidget : public QFrame
{
	Q_OBJECT

public:
	FileActionBarWidget(QWidget *parent=0);
	virtual ~FileActionBarWidget() = default;

	void						setIsInLibrary( bool isInLibrary );
	void						setIsShared( bool isShared );

signals:
	void						signalPlayButtonClicked( void );
	void						signalLibraryButtonClicked( void );
	void						signalFileShareButtonClicked( void );
	void						signalShredButtonClicked( void );

protected slots:
	void						slotPlayButtonClicked( void );
	void						slotLibraryButtonClicked( void );
	void						slotFileShareButtonClicked( void );
	void						slotShredButtonClicked( void );

protected:
	Ui::FileActionBarWidgetClass	ui;
    AppCommon&					m_MyApp;
};

