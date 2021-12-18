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

#include "AppletPeerReplyFileOffer.h"

#include "GuiOfferSession.h"

//============================================================================
AppletPeerReplyFileOffer::AppletPeerReplyFileOffer(	AppCommon& app, 
												    QWidget * parent )
: AppletPeerBase( OBJNAME_ACTIVITY_REPLY_FILE_OFFER, app, parent )
{
    setPluginType( ePluginTypeFileXfer );
    setAppletType( eAppletPeerReplyOfferFile );
    ui.setupUi( getContentItemsFrame() );
    setTitleBarText( DescribeApplet( m_EAppletType ) );
   
    //setGuiOfferSession( poOffer );
    
    connectBarWidgets();
    /*
	setupStyledDlg( poOffer->getHisIdent(), 
		            ui.FriendIdentWidget,
		            m_ePluginType,
		            ui.PermissionButton, 
		            ui.PermissionLabel );
                    */

	QString strRxFile = QObject::tr("Receive File");
	ui.m_PermissionLabel->setText( strRxFile );

    /*
	ui.m_FileNameEdit->setText( poOffer->getFileName().c_str() );
	ui.m_OfferMsgEdit->setPlainText( poOffer->getOfferMsg().c_str() );

	connect( ui.m_AcceptButton, SIGNAL(clicked()), this, SLOT(onReceiveFileButClick()) );
	connect( ui.m_CancelButton, SIGNAL(clicked()), this, SLOT(onCancelButClick()) );
    */
}

//============================================================================
//! Implement the OnClickListener callback    
void AppletPeerReplyFileOffer::onReceiveFileButClick()
{
	//setOfferResponse( eOfferResponseAccept );
	m_OfferSessionLogic.sendOfferReply( eOfferResponseAccept );
	accept();
}

//============================================================================
//! Implement the OnClickListener callback    
void AppletPeerReplyFileOffer::onCancelButClick()
{
	m_OfferSessionLogic.sendOfferReply( eOfferResponseReject );
	reject();
}

