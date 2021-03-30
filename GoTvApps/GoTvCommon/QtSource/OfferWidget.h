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

#include "config_gotvapps.h"
#include "ui_OfferWidget.h"
#include <QListWidgetItem>

class GuiUser;
class GuiOfferSession;
class AppCommon;
class MyIcons;

class OfferWidget : public QWidget
{
	Q_OBJECT

public:
	OfferWidget( QWidget * parent = 0 );
	virtual ~OfferWidget();

	AppCommon&					getMyApp( void ) { return m_MyApp; }
	MyIcons&					getMyIcons( void );

	Ui::OfferWidget				ui;

	QPushButton *				getSubWidget( void )				{ return SubWidget; }

	void						setOnlineState( bool isOnline );
	bool						isOnline( void )					{ return m_IsOnline; }
	void						setAvailable( bool available );
	bool						isOfferAvailable( void );
	void						setMissedCalls( int missedCallCnt );

signals:
	void						listButtonClicked( OfferWidget* listEntryWidget );
	void						acceptButtonClicked( OfferWidget* listEntryWidget );
	void						rejectButtonClicked( OfferWidget* listEntryWidget );

protected slots:
	void						iconButtonPressed( void );
	void						iconButtonReleased( void );
	void						listButtonPressed( void );
	void						listButtonReleased( void );
	void						slotAcceptButtonPressed( void );
	void						slotRejectButtonReleased( void );

protected:
	AppCommon&					m_MyApp;
	bool						m_IsOnline;
	QPushButton *				SubWidget;

};
