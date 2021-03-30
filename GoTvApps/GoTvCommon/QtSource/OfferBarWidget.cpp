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
// http://www.nolimitconnect.com
//============================================================================

#include <app_precompiled_hdr.h>
#include "OfferBarWidget.h"
#include "OffersMgr.h"

#include "MyIcons.h"
#include "AppCommon.h"

#include <CoreLib/VxGlobals.h>

#include <QCloseEvent>

//============================================================================
OfferBarWidget::OfferBarWidget( QWidget * parent )
: QFrame( parent )
, m_MyApp( GetAppInstance() )
, m_OffersMgr( m_MyApp.getOffersMgr() )
, m_HisIdent( 0 )
, m_ePluginType( ePluginTypeInvalid )
{
	ui.setupUi( this );
	ui.m_RejectButton->setIcon( eMyIconCancelNormal );

	connect( ui.m_FriendButton,				SIGNAL(clicked()),		this, SLOT(slotAcceptOfferButtonClicked()) );
	connect( ui.m_AcceptButton,				SIGNAL(clicked()),		this, SLOT(slotAcceptOfferButtonClicked()) );
	connect( ui.m_RejectButton,				SIGNAL(clicked()),		this, SLOT(slotRejectOfferButtonClicked()) );
	m_OffersMgr.wantToGuiOfferCallbacks( this, true );
}

//============================================================================
OfferBarWidget::~OfferBarWidget()
{
    if( false == VxIsAppShuttingDown() )
    {
        m_OffersMgr.wantToGuiOfferCallbacks( this, false );
    }
}

//============================================================================
MyIcons&  OfferBarWidget::getMyIcons( void )
{
	return m_MyApp.getMyIcons();
}

//============================================================================
void OfferBarWidget::showEvent( QShowEvent * ev )
{
	QWidget::showEvent( ev );
	initializeOfferBar();
}

//============================================================================
void OfferBarWidget::hideEvent( QHideEvent * ev )
{
	QWidget::hideEvent( ev );
}

//============================================================================
void OfferBarWidget::closeEvent( QCloseEvent * ev )
{
	m_OffersMgr.wantToGuiOfferCallbacks( this, false );
	ev->accept();
}

//============================================================================
void OfferBarWidget::initializeOfferBar( void )
{
	OfferSessionState * offerState = m_OffersMgr.getTopGuiOfferSession();
	fillOfferBar( offerState );
}

//============================================================================
void OfferBarWidget::fillOfferBar( OfferSessionState * offerState )
{
	if( 0 != offerState )
	{
		this->setVisible( true );
		m_OfferSessionId	= offerState->getOfferSessionId();
		m_HisIdent			= offerState->getHisIdent();
		m_ePluginType		= offerState->getPluginType();
		updateOfferBar( offerState );
	}
	else
	{
		this->setVisible( false );
		m_OfferSessionId.clearVxGUID();
		m_HisIdent = nullptr;
		m_ePluginType = ePluginTypeInvalid;
	}
}

//============================================================================
void OfferBarWidget::updateOfferBar( OfferSessionState * offerState )
{
	ui.m_OfferCntLabel->setText( QString("%1").arg( m_OffersMgr.getTotalOffersCount() ) );
	if( offerState->getOfferSessionId() == m_OfferSessionId )
	{
		switch( offerState->getOfferState() )
		{
		case eOfferStateRxedOffer:
			setIsOfferAvailable( true );
			break;

		default:
			setIsOfferAvailable( false );
			break;
		}

		setOfferOnlineState( offerState->getHisIdent()->getIsOnline() );

		GuiOfferSession * offerSession = offerState->getGuiOfferSession();
		QString strMsg		= offerSession->getOfferMsg().c_str();
		QString strFileName = offerSession->getFileName().c_str();
		QString strName;
		if( offerSession->getIsRemoteInitiated() )
		{
			strName = "From ";
		}
		else
		{
			strName = "To ";
		}

		strName += offerSession->getHisIdent()->getOnlineName();
		ui.m_FriendButton->setIcon( getMyIcons().getPluginIcon( offerSession->getPluginType() ) );

		QString strFrienshipDescription = offerSession->getHisIdent()->describeMyFriendshipToHim();
		ui.m_FriendTypeLabel->setText( strFrienshipDescription );
		ui.m_FriendNameLabel->setText( strName );

		if( strFileName.length() > 0 )
		{
			QString strPrefix = "File: ";
			ui.m_CallMsgTextLabel->setText( strPrefix + strFileName );	
		}
		else
		{
			ui.m_CallMsgTextLabel->setText( strMsg );
		}

		ui.m_MissedCallCntLabel->setText( QString("%1").arg( offerSession->getMissedCallsCnt() ) );
		bool setCntVisible = ( 0 != offerSession->getMissedCallsCnt() );
		ui.m_MissedCallCntLabel->setVisible( setCntVisible );
		ui.m_MissedCallsLabel->setVisible( setCntVisible );
	}
}
//============================================================================
void OfferBarWidget::setOfferOnlineState( bool isOnline )
{
	if( isOnline )
	{
		this->setStyleSheet("border-image: 0; background-color: rgb(255,255,232); color: rgb(0, 0, 0); font: 14px;\n");
	}
	else
	{
		ui.m_AcceptButton->setIcon( eMyIconAcceptDisabled );
		ui.m_AcceptButton->setEnabled( false );
		this->setStyleSheet("border-image: 0; background-color: rgb(192, 192, 192); color: rgb(0, 0, 0); font: 14px;\n");
	}
}

//============================================================================
void OfferBarWidget::setIsOfferAvailable( bool available )
{
	ui.m_AcceptButton->setEnabled( available );
	ui.m_AcceptButton->setIcon( available ? eMyIconAcceptNormal : eMyIconAcceptDisabled );
}

//============================================================================
bool OfferBarWidget::getIsOfferAvailable( void )
{
	return ui.m_AcceptButton->isEnabled();
}

//============================================================================
void OfferBarWidget::slotAcceptOfferButtonClicked( void )
{
	m_OffersMgr.acceptOfferButtonClicked( m_ePluginType, m_OfferSessionId, m_HisIdent );
}

//============================================================================
void OfferBarWidget::slotRejectOfferButtonClicked( void )
{
	m_OffersMgr.rejectOfferButtonClicked( m_ePluginType, m_OfferSessionId, m_HisIdent );	
}

//============================================================================
void OfferBarWidget::doGuiUpdatePluginOffer( OfferSessionState * offerState )
{
	if( ( 0 == parentWidget() ) || parentWidget()->isVisible() )
	{
		fillOfferBar( offerState );
	}
}

//============================================================================
void OfferBarWidget::doGuiOfferRemoved( OfferSessionState * offerState )
{
	if( this->isVisible() && ( offerState->getOfferSessionId() == m_OfferSessionId ) )
	{
		initializeOfferBar();
	}
}

//============================================================================
void OfferBarWidget::doGuiAllOffersRemoved( void )
{
	this->setVisible( false );
}
