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
// brett.jones@engineer.com
// http://www.gotvptop.net
//============================================================================

#include "ToGuiOfferInterface.h"

#include "ui_OfferBarWidget.h"
#include <QListWidgetItem>

class Friend;
class GuiOfferSession;
class QLabel;
class AppCommon;
class MyIcons;

class OfferBarWidget : public QFrame, public ToGuiOfferInterface
{
	Q_OBJECT

public:
	OfferBarWidget( QWidget * parent = 0 );
	virtual ~OfferBarWidget();

	AppCommon&					getMyApp( void ) { return m_MyApp; }
	MyIcons&					getMyIcons( void );

protected slots:
	void						slotAcceptOfferButtonClicked( void );
	void						slotRejectOfferButtonClicked( void );

protected:
	virtual void				showEvent( QShowEvent * ev );
	virtual void				hideEvent( QHideEvent * ev );
	virtual void				closeEvent( QCloseEvent * ev );

	virtual void				doGuiUpdatePluginOffer( OfferSessionState * offerState ); 
	virtual void				doGuiOfferRemoved( OfferSessionState * offerState ); 
	virtual void				doGuiAllOffersRemoved( void ); 


	void						initializeOfferBar( void );
	void						fillOfferBar( OfferSessionState * offerState );
	void						updateOfferBar( OfferSessionState * offerState );
	void						setOfferOnlineState( bool isOnline );
	void						setIsOfferAvailable( bool available );
	bool						getIsOfferAvailable( void );


	Ui::OfferBarWidgetClass		ui;
	AppCommon&				m_MyApp;
	OffersMgr&					m_OffersMgr;
	VxGUID						m_OfferSessionId;
	VxNetIdent *				m_HisIdent;
	EPluginType					m_ePluginType;
};
