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
#include "ui_CallListItem.h"

#include <QListWidgetItem>
#include <QWidget>

class GuiOfferSession;

class CallListItem : public QWidget, public QListWidgetItem
{
	Q_OBJECT;
public:
	CallListItem( QWidget *parent=0 );
	virtual ~CallListItem();

    AppCommon&					getMyApp( void ) { return m_MyApp; }
    MyIcons&					getMyIcons( void );

    void                        setCallSession( GuiOfferSession* hostSession );
    GuiOfferSession*            getCallSession( void );

    void						updateWidgetFromInfo( void );
    QSize                       calculateSizeHint( void );

signals:
    void						signalCallListItemClicked( CallListItem * poItemWidget );
	void						signalAvatarButtonClicked( CallListItem* listEntryWidget );
    void						signalFriendshipButtonClicked( CallListItem* listEntryWidget );
	void						signalMenuButtonClicked( CallListItem* listEntryWidget );

public slots:
	void						slotAvatarButtonClicked( void );
    void						slotFriendshipButtonClicked( void );
	void						slotMenuButtonClicked( void );

protected:
    virtual void				mousePressEvent( QMouseEvent * event ) override;
    virtual void				resizeEvent( QResizeEvent* resizeEvent ) override;

	//=== vars ===//
    Ui::CallListItemUi	        ui;
    AppCommon&					m_MyApp;
};



