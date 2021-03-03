#pragma once
//============================================================================
// Copyright (C) 2009 Brett R. Jones 
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
#include "ui_ListEntryWidget.h"

#include <QListWidgetItem>
#include <QObject>

class FriendListEntryWidget : public QObject, public QListWidgetItem
{
	Q_OBJECT;
public:
	FriendListEntryWidget( QListWidget * parent = 0, int type = Type );
	virtual ~FriendListEntryWidget() override = default;

	QPushButton *				getSubWidget( void )		{ return SubWidget; }
	QPushButton *				getMenuButton( void )		{ return m_MenuButton; }

signals:
	void						listButtonClicked( FriendListEntryWidget* listEntryWidget );
	void						signalMenuButtonClicked( FriendListEntryWidget* listEntryWidget );

public slots:
	void						iconButtonPressed( void );
	void						iconButtonReleased( void );
	void						listButtonPressed( void );
	void						listButtonReleased( void );
	void						slotMenuButtonPressed( void );
	void						slotMenuButtonReleased( void );

public:
	//=== vars ===//
    Ui::ListWidgetEntryClass	ui;
	QPushButton *				SubWidget;
	QPushButton *				m_MenuButton;
};




