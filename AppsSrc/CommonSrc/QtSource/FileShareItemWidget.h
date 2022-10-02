#pragma once
//============================================================================
// Copyright (C) 2010 Brett R. Jones 
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

#include "ui_FileShareItemWidget.h"
#include <QListWidgetItem>

class FileItemInfo;
class AppCommon;
class MyIcons;

class FileShareItemWidget : public QWidget, public QListWidgetItem
{
	Q_OBJECT

public:
	FileShareItemWidget(QWidget*parent=0 );
	virtual ~FileShareItemWidget();

	AppCommon&					getMyApp( void ) { return m_MyApp; }
	MyIcons&					getMyIcons( void );

	void						setIsShared( bool isShared );
	bool						getIsShared( void );
	void						setIsInLibrary( bool isInLibrary );
	bool						getIsInLibrary( void );

	void						setFileItemInfo( FileItemInfo* fileItemInfo );
	FileItemInfo*				getFileItemInfo( void );

	void						updateWidgetFromInfo( void );

signals:
	void						signalFileShareItemClicked( QListWidgetItem * poItemWidget );
	void						signalFileIconClicked( QListWidgetItem * poItemWidget );

	void						signalPlayButtonClicked( QListWidgetItem * poItemWidget );
	void						signalLibraryButtonClicked( QListWidgetItem * poItemWidget );
	void						signalFileShareButtonClicked( QListWidgetItem * poItemWidget );
	void						signalShredButtonClicked( QListWidgetItem * poItemWidget );

protected slots:
	void						slotFileIconButtonClicked( void );
	void						slotPlayButtonClicked( void );
	void						slotLibraryButtonClicked( void );
	void						slotFileShareButtonClicked( void );
	void						slotShredButtonClicked( void );

protected:
	virtual void				mousePressEvent(QMouseEvent * event);
	virtual void				resizeEvent(QResizeEvent* resizeEvent);

	Ui::FileShareItemWidget		ui;
	AppCommon&					m_MyApp;
};
