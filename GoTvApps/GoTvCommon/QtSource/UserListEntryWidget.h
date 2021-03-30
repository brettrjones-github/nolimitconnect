#pragma once
//============================================================================
// Copyright (C) 2021 Brett R. Jones 
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
#include "ui_UserListEntryWidget.h"

#include <QListWidgetItem>
#include <QWidget>

class GuiHostSession;

class UserListEntryWidget : public QWidget, public QListWidgetItem
{
	Q_OBJECT;
public:
	UserListEntryWidget( QWidget *parent=0 );
	virtual ~UserListEntryWidget();

    AppCommon&					getMyApp( void ) { return m_MyApp; }
    MyIcons&					getMyIcons( void );

    void                        setHostSession( GuiHostSession* hostSession );
    GuiHostSession*             getHostSession( void );

    void						updateWidgetFromInfo( void );

signals:
    void						signalUserListItemClicked( QListWidgetItem * poItemWidget );
	void						signalAvatarButtonClicked( UserListEntryWidget* listEntryWidget );
    void						signalFriendshipButtonClicked( UserListEntryWidget* listEntryWidget );
	void						signalMenuButtonClicked( UserListEntryWidget* listEntryWidget );

public slots:
	void						slotAvatarButtonClicked( void );
    void						slotFriendshipButtonClicked( void );
	void						slotMenuButtonPressed( void );
	void						slotMenuButtonReleased( void );

protected:
    virtual void				mousePressEvent( QMouseEvent * event ) override;
    virtual void				resizeEvent( QResizeEvent* resizeEvent ) override;

	//=== vars ===//
    Ui::UserListEntryWidgetUi	ui;
    AppCommon&					m_MyApp;
};




