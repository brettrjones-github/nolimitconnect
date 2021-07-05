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

#include <app_precompiled_hdr.h>
#include "GuiOfferSession.h"
#include "AppletPeerVideoPhone.h"
#include "AppGlobals.h"
#include "MyIcons.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>

#include <CoreLib/VxGlobals.h>

//============================================================================
AppletPeerVideoPhone::AppletPeerVideoPhone(	AppCommon& app, QWidget* parent )
: AppletPeerBase( OBJNAME_ACTIVITY_TO_FRIEND_VIDEO_PHONE, app, parent )
{
    setPluginType( ePluginTypeVideoPhone );
    setAppletType( eAppletPeerVideoPhone );
    ui.setupUi( getContentItemsFrame() );
    setTitleBarText( DescribeApplet( m_EAppletType ) );

	setupActivityVideoPhone();
	// m_OfferSessionLogic.sendOfferOrResponse();
}

//============================================================================
void AppletPeerVideoPhone::setupActivityVideoPhone()
{
    /*
	setupBaseWidgets( ui.m_TitleBarWidget, ui.m_FriendIdentWidget, ui.m_PermissionButton, ui.m_PermissionLabel );
	ui.m_TitleBarWidget->setTitleBarText( tr("Video Chat") );
	ui.m_TitleBarWidget->enableAudioControls( true );
    connectBarWidgets();
	ui.m_InstMsgWidget->setInstMsgWidgets( m_ePluginType, m_HisIdent );

	ui.m_CamVidWidget->setVideoFeedId( m_HisIdent->getMyOnlineId() );
	ui.m_CamVidWidget->setRecordFilePath( VxGetDownloadsDirectory().c_str() );
	ui.m_CamVidWidget->setRecordFriendName( m_HisIdent->getOnlineName() );
	setVidCamWidget( ui.m_CamVidWidget );
    */
}

//============================================================================
void AppletPeerVideoPhone::doToGuiRxedPluginOffer( void * callbackData, GuiOfferSession * offer )
{
	m_OfferSessionLogic.doToGuiRxedPluginOffer( offer );
}

//============================================================================
void AppletPeerVideoPhone::doToGuiRxedOfferReply( void * callbackData, GuiOfferSession * offerSession )
{
	m_OfferSessionLogic.doToGuiRxedOfferReply( offerSession );
}

//============================================================================
//! called by base class with in session state
void AppletPeerVideoPhone::onInSession( bool isInSession )
{
	//if( isInSession )
	//{
	//	setStatusText( tr( "In Video Chat Session" ) );
	//}
	//else
	//{
	//	setStatusText( tr( "Video Phone Chat Ended" ) );
	//}
}

//============================================================================
void AppletPeerVideoPhone::toGuiInstMsg( void * callbackData, GuiUser * friendIdent, EPluginType ePluginType, QString instMsg )
{
	Q_UNUSED( callbackData );
	if( ( ePluginType == m_ePluginType )
		&& m_HisIdent 
		&& ( m_HisIdent->getMyOnlineId() == friendIdent->getMyOnlineId() ) )
	{
		ui.m_InstMsgWidget->toGuiInstMsg( instMsg );
	}
}; 

