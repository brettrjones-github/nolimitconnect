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
// http://www.nolimitconnect.org
//============================================================================

#include "GuiFileXferSession.h"
#include "ui_FileXferWidget.h"

#include <QListWidgetItem>

class AppCommon;
class MyIcons;

class FileXferWidget : public QWidget, public QListWidgetItem
{
	Q_OBJECT

public:
	FileXferWidget(QWidget *parent=0);
	virtual ~FileXferWidget();

	AppCommon&					getMyApp( void ) { return m_MyApp; }
	MyIcons&					getMyIcons( void );

    void						setFileItemInfo( GuiFileXferSession * fileItemInfo );
    GuiFileXferSession *		getFileItemInfo( void );

	void						setXferState( EXferState eXferState, int param1, int param2 );

	void						setIsShared( bool isShared );
	bool						getIsShared( void );
	void						setIsInLibrary( bool isInLibrary );
	bool						getIsInLibrary( void );

	void						setFileIconButtonEnabled( bool enable );
	void						setFileProgressBarValue( int val );

	void						updateWidgetFromInfo( void );

signals:
	void						signalFileXferItemClicked( QListWidgetItem * poItemWidget );
	void						signalFileIconButtonClicked( QListWidgetItem * poItemWidget );
	void						signalCancelButtonClicked( QListWidgetItem * poItemWidget );

	void						signalPlayButtonClicked( QListWidgetItem * poItemWidget );
	void						signalLibraryButtonClicked( QListWidgetItem * poItemWidget );
	void						signalFileShareButtonClicked( QListWidgetItem * poItemWidget );
	void						signalShredButtonClicked( QListWidgetItem * poItemWidget );

protected slots:
	void						slotFileIconButtonClicked( void );
	void						slotCancelButtonClicked( void );
	void						slotPlayButtonClicked( void );
	void						slotLibraryButtonClicked( void );
	void						slotFileShareButtonClicked( void );
	void						slotShredButtonClicked( void );

protected:
    virtual void				mousePressEvent( QMouseEvent * event ) override;
    virtual void				resizeEvent( QResizeEvent* resizeEvent ) override;

	Ui::FileXferWidgetClass		ui;
	AppCommon&					m_MyApp;
};
