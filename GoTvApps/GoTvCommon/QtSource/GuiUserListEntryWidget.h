#pragma once
//============================================================================
// Copyright (C) 2021 Brett R. Jones 
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

#include "config_gotvapps.h"
#include "ui_GuiUserListEntryWidget.h"

#include <QListWidgetItem>
#include <QWidget>

class GuiHostSession;

class GuiUserListEntryWidget : public QWidget, public QListWidgetItem
{
	Q_OBJECT;
public:
	GuiUserListEntryWidget( QWidget *parent=0 );
	virtual ~GuiUserListEntryWidget();

    AppCommon&					getMyApp( void ) { return m_MyApp; }
    MyIcons&					getMyIcons( void );

    void                        setHostSession( GuiHostSession* hostSession );
    GuiHostSession*             getHostSession( void );

    void						updateWidgetFromInfo( void );

signals:
    void						signalHostListItemClicked( QListWidgetItem * poItemWidget );
	void						signalIconButtonClicked( GuiUserListEntryWidget* listEntryWidget );
	void						signalMenuButtonClicked( GuiUserListEntryWidget* listEntryWidget );
    void						signalJoinButtonClicked( GuiUserListEntryWidget* listEntryWidget );

public slots:
	void						slotIconButtonClicked( void );
	void						slotMenuButtonPressed( void );
	void						slotMenuButtonReleased( void );
    void						slotJoinButtonPressed( void );
 
protected:
    virtual void				mousePressEvent( QMouseEvent * event ) override;
    virtual void				resizeEvent( QResizeEvent* resizeEvent ) override;

	//=== vars ===//
    Ui::GuiUserListEntryWidgetUi	ui;
    AppCommon&					m_MyApp;
};




