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

#include "AppletPeerSessionFileOffer.h"

#include "GuiOfferSession.h"

//============================================================================
AppletPeerSessionFileOffer::AppletPeerSessionFileOffer(	AppCommon& app, QWidget* parent )
: AppletPeerBase( OBJNAME_ACTIVITY_SESSION_FILE_OFFER, app, parent )
, m_ePluginType(ePluginTypePersonFileXfer)
{
    setPluginType( ePluginTypePersonFileXfer );
    setAppletType( eAppletPeerSessionFileOffer );
    ui.setupUi( getContentItemsFrame() );
    setTitleBarText( DescribeApplet( m_EAppletType ) );

    /*
	ui.setupUi(this);
	ui.m_TitleBarWidget->setTitleBarText( QObject::tr("Offer File"));


    connectBarWidgets();

	setupStyledDlg(	
		poOffer->getHisIdent(), 		
		ui.FriendIdentWidget,
		m_ePluginType,
		ui.PermissionButton, 
		ui.PermissionLabel );
	ui.progressBar->setValue( poOffer->getProgress() );
    */

	connect( ui.AcceptButton, SIGNAL(clicked()), this, SLOT(onAcceptButClick()) );
	connect( ui.CancelButton, SIGNAL(clicked()), this, SLOT(onCancelButClick()) );
}

//============================================================================
//! Implement the OnClickListener callback    
void AppletPeerSessionFileOffer::onAcceptButClick()
{
	accept();
}

//============================================================================
//! Implement the OnClickListener callback    
void AppletPeerSessionFileOffer::onCancelButClick()
{
	//setOfferResponse( eOfferResponseCancelSession );

	reject();
}
