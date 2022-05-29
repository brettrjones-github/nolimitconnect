//============================================================================
// Copyright (C) 2015 Brett R. Jones
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

#include "AppSettings.h"
#include "AppletMultiMessenger.h"
#include "ActivityMessageBox.h"
#include "GuiOfferSession.h"
#include "GuiParams.h"
#include "AppGlobals.h"
#include "MyIcons.h"
#include "AppCommon.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>
#include <ptop_src/ptop_engine_src/P2PEngine/EngineSettings.h>

#include <CoreLib/VxGlobals.h>

//============================================================================
AppletMultiMessenger::AppletMultiMessenger(	AppCommon& app, QWidget* parent )
: AppletPeerBase( OBJNAME_APPLET_MULTI_MESSENGER, app, parent )
, m_TodGameLogic( app, app.getEngine(), ePluginTypeMessenger, this )
{
    setPluginType( ePluginTypeMessenger );
    setAppletType( eAppletMultiMessenger );
    ui.setupUi( getContentItemsFrame() );
    setTitleBarText( DescribeApplet( m_EAppletType ) );
    setBackButtonVisibility( false );

	ui.m_UserListWidget->setAppletType( eAppletMultiMessenger );

    ui.m_HostViewFrame->setFixedHeight( GuiParams::getButtonSize( eButtonSizeSmall ).height() + 4 );
	ui.m_FriendsView->setFixedSize( eButtonSizeSmall );
    ui.m_FriendsView->setIcon( eMyIconEyeFriends );
	ui.m_GroupHosts->setFixedSize( eButtonSizeSmall );
    ui.m_GroupHosts->setIcon( eMyIconEyeGroup );
	ui.m_ChatRoomHosts->setFixedSize(eButtonSizeSmall);
    ui.m_ChatRoomHosts->setIcon( eMyIconEyeChatRoom );
	ui.m_RandomConnectHosts->setFixedSize( eButtonSizeSmall );
    ui.m_RandomConnectHosts->setIcon( eMyIconEyeRandomConnect );
	ui.m_EverybodyView->setFixedSize( eButtonSizeSmall );
    ui.m_EverybodyView->setIcon( eMyIconEyeAll );
	if( !m_MyApp.getAppSettings().getFeatureEnable( eAppFeatureChatRoom ) )
	{
		ui.m_ChatRoomHosts->setVisible( false );
	}

	if( !m_MyApp.getAppSettings().getFeatureEnable( eAppFeatureRandomConnect ) )
	{
		ui.m_RandomConnectHosts->setVisible( false );
	}

	ui.m_EyeHosts->setFixedSize( eButtonSizeSmall );
    ui.m_EyeHosts->setIcon( eMyIconEyeShow );
	ui.m_EyeSession->setFixedSize( eButtonSizeSmall );
    ui.m_EyeSession->setIcon( eMyIconEyeShow );

	ui.m_VideoChatButton->setFixedSize( eButtonSizeSmall );
    ui.m_VideoChatButton->setIcon( eMyIconVideoPhoneNormal );
	ui.m_VoipButton->setFixedSize( eButtonSizeSmall );
    ui.m_VoipButton->setIcon( eMyIconVoicePhoneNormal );
	ui.m_TrueOrDareButton->setFixedSize( eButtonSizeSmall );
    ui.m_TrueOrDareButton->setIcon( eMyIconTruthOrDareNormal );
	ui.m_OfferFileButton->setFixedSize( eButtonSizeSmall );
    ui.m_OfferFileButton->setIcon( eMyIconFileServer );
	ui.m_OptionsButton->setFixedSize( eButtonSizeSmall );
    ui.m_OptionsButton->setIcon( eMyIconSettingsGear );

    m_OffersFrame			= ui.m_OffersFrame;
    m_ResponseFrame			= ui.m_ResponseFrame;
    m_HangupSessionFrame	= ui.m_HangupSessionFrame;
    m_VidChatWidget			= ui.m_VidWidget;

    m_ResponseFrame->setVisible( false );
    m_HangupSessionFrame->setVisible( false );
    m_VidChatWidget->setVisible( false );
    m_VidChatWidget->setRecordFilePath( VxGetDownloadsDirectory().c_str() );
    ui.m_SessionWidget->setIsPersonalRecorder( false );

    ui.m_SessionWidget->setEntryMode( eAssetTypeUnknown );
    ui.m_OptionsButton->setVisible( false );
    ui.m_OfferFileButton->setVisible( false );
    m_TodGameLogic.setVisible( false );

    connect( ui.m_EyeHosts,		        SIGNAL(clicked()),						this,	SLOT(slotEyeHostButtonClicked()) );
    connect( ui.m_EyeSession,           SIGNAL(clicked()),						this,	SLOT(slotEyeSessionButtonClicked()) );

    connect( ui.m_FriendsView,		    SIGNAL(clicked()),						this,	SLOT(slotFriendsButtonClicked()) );
    connect( ui.m_GroupHosts,	        SIGNAL(clicked()),						this,	SLOT(slotGroupHostButtonClicked()) );
    connect( ui.m_ChatRoomHosts,        SIGNAL(clicked()),						this,	SLOT(slotChatRoomHostButtonClicked()) );
    connect( ui.m_RandomConnectHosts,	SIGNAL(clicked()),	                    this,	SLOT(slotRandomConnectHostButtonClicked()) );

    connect( ui.m_OfferFileButton,	    SIGNAL(clicked()),						this,	SLOT(slotOfferFileButtonClicked()) );

    connect( ui.m_VideoChatButton,	    SIGNAL(clicked()),						this,	SLOT(slotUserInputButtonClicked()) );
    connect( ui.m_VoipButton,		    SIGNAL(clicked()),						this,	SLOT(slotUserInputButtonClicked()) );
    connect( ui.m_TrueOrDareButton,     SIGNAL(clicked()),						this,	SLOT(slotUserInputButtonClicked()) );
    connect( ui.m_SessionWidget,	    SIGNAL(signalUserInputButtonClicked()),	this,	SLOT(slotUserInputButtonClicked()) );

	connect( &m_MyApp.getUserMgr(), SIGNAL( signalUserOnlineStatus(GuiUser*) ), this, SLOT( slotUserOnlineStatus(GuiUser*) ) );


    /*
    //setupMultiSessionActivity( netIdent );
    //m_OfferSessionLogic.sendOfferOrResponse();
    //m_OfferOrResponseIsSent = true;
	m_IsInitialized = true;
    */
    m_MyApp.activityStateChange( this, true ); 
    m_MyApp.onMessengerReady( true );
}

//============================================================================
AppletMultiMessenger::~AppletMultiMessenger()
{
	m_MyApp.onMessengerReady( false );
    m_MyApp.activityStateChange( this, false );
}

//============================================================================
void AppletMultiMessenger::setupMultiSessionActivity( GuiUser * hisIdent )
{
	m_HisIdent = hisIdent;

	m_VidChatWidget->setVideoFeedId( m_HisIdent->getMyOnlineId() );
	m_TodGameWidget->getVidWidget()->setVideoFeedId( m_HisIdent->getMyOnlineId() );

	m_VidChatWidget->setRecordFriendName( m_HisIdent->getOnlineName().c_str() );

	ui.m_SessionWidget->setIdents( m_UserMgr.getMyIdent(), hisIdent );

	m_TodGameLogic.setGuiWidgets( m_HisIdent, m_TodGameWidget );

	MultiSessionState * sessionState = new MultiSessionState( m_MyApp, *this, eMSessionTypePhone );
	sessionState->setGuiWidgets(	hisIdent,
									m_OffersFrame,
									m_ResponseFrame,
									m_HangupSessionFrame,
									ui.m_VoipButton,
									ui.m_AcceptOfferButton,
									ui.m_AcceptLabel,
									ui.m_RejectOfferButton,
									ui.m_RejectLabel,
									ui.m_HangupSessionButton,
									m_VidChatWidget,
									&m_TodGameLogic );
	m_MSessionsList.push_back( sessionState );

	sessionState = new MultiSessionState( m_MyApp, *this, eMSessionTypeVidChat );
	sessionState->setGuiWidgets(	hisIdent,
									m_OffersFrame,
									m_ResponseFrame,
									m_HangupSessionFrame,
									ui.m_VideoChatButton,
									ui.m_AcceptOfferButton,
									ui.m_AcceptLabel,
									ui.m_RejectOfferButton,
									ui.m_RejectLabel,
									ui.m_HangupSessionButton,
									m_VidChatWidget,
									&m_TodGameLogic );
	m_MSessionsList.push_back( sessionState );

	sessionState = new MultiSessionState( m_MyApp, *this, eMSessionTypeTruthOrDare );
	sessionState->setGuiWidgets(	hisIdent,
									m_OffersFrame,
									m_ResponseFrame,
									m_HangupSessionFrame,
									ui.m_TrueOrDareButton,
									ui.m_AcceptOfferButton,
									ui.m_AcceptLabel,
									ui.m_RejectOfferButton,
									ui.m_RejectLabel,
									ui.m_HangupSessionButton,
									m_VidChatWidget,
									&m_TodGameLogic );
	m_MSessionsList.push_back( sessionState );
	ui.m_SessionWidget->setCanSend( false );

	if( false == checkForSendAccess( false ) )
	{
		showReasonAccessNotAllowed();
	}

	m_MyApp.toGuiAssetAction( eAssetActionRxViewingMsg, m_HisIdent->getMyOnlineId(), 1 );
}

//============================================================================
void AppletMultiMessenger::onActivityFinish( void )
{
	ui.m_SessionWidget->onActivityStop();
	m_OfferSessionLogic.onStop();
}

//============================================================================
void AppletMultiMessenger::toToGuiRxedPluginOffer( GuiOfferSession * offer )
{
	m_OfferSessionLogic.toToGuiRxedPluginOffer( offer );
}

//============================================================================
void AppletMultiMessenger::toToGuiRxedOfferReply( GuiOfferSession * offerSession )
{
	m_OfferSessionLogic.toToGuiRxedOfferReply( offerSession );
}

//============================================================================
void AppletMultiMessenger::showReasonAccessNotAllowed( void )
{
	EPluginAccess accessState = m_HisIdent->getMyAccessPermissionFromHim( m_ePluginType );
	QString accessDesc = DescribePluginType( m_ePluginType );
	accessDesc += QObject::tr( " with " );
	accessDesc += m_HisIdent->getOnlineName().c_str();
	accessDesc +=  " ";

	if( ePluginAccessOk != accessState )
	{
		accessDesc +=  DescribePluginAccess( accessState );
		setStatusMsg( accessDesc );
		return;
	}

	if( false == m_HisIdent->isOnline() )
	{
		accessDesc +=   QObject::tr( " requires user be online " );
		setStatusMsg( accessDesc );
		return;
	}
}

//============================================================================
void AppletMultiMessenger::slotUserOnlineStatus( GuiUser* user )
{
    if( !user || !user->isValid() )
    {
        LogMsg( LOG_ERROR, "AppletMultiMessenger slotUserOnlineStatus invalid param" );
        return;
    }

    if( m_UserMgr.isUserInSession( user->getMyOnlineId() ) || user->isFriend() )
	{
		QString statMsg = user->getOnlineName().c_str();
		statMsg += user->isOnline() ? QObject::tr( " is online" ) : QObject::tr( " went offline" );
		setStatusMsg( statMsg );

		checkForSendAccess( false );
	}
}

//============================================================================
void AppletMultiMessenger::slotUserInputButtonClicked( void )
{
	checkForSendAccess( true );
}

//============================================================================
bool AppletMultiMessenger::checkForSendAccess( bool sendOfferIfPossible )
{
	bool canSend = false;
    if( !m_HisIdent )
    {
        LogMsg( LOG_DEBUG, "AppletMultiMessenger::checkForSendAccess null m_HisIdent" );
        return false;
    }

	if( m_HisIdent->isOnline() 
		&& m_HisIdent->isMyAccessAllowedFromHim( m_ePluginType ) )
	{
		canSend = true;
	}

	if( canSend != m_CanSend )
	{
		m_CanSend = canSend;
		ui.m_SessionWidget->setCanSend( m_CanSend );
	}

	if( m_CanSend && sendOfferIfPossible && ( false == m_OfferOrResponseIsSent ) )
	{
		QString offMsg = QObject::tr( "Requested Text Chat Session with " );
		offMsg += m_HisIdent->getOnlineName().c_str();
		m_MyApp.toGuiStatusMessage( offMsg.toUtf8().constData() );
		//m_Engine.fromGuiMultiSessionAction( eMSessionActionChatSessionReq, m_HisIdent->getMyOnlineId(), 0, m_OfferSessionLogic.getOfferSessionId() );
		if( m_OfferSessionLogic.sendOfferOrResponse() )
		{
			m_OfferOrResponseIsSent = true;
		}
	}

	QVector<MultiSessionState*>::iterator iter;
	for( iter = m_MSessionsList.begin();  iter != m_MSessionsList.end(); ++iter )
	{
		(*iter)->checkForSendAccess();
	}

	return canSend;
}


//============================================================================
void AppletMultiMessenger::showEvent( QShowEvent * ev )
{
	AppletPeerBase::showEvent( ev );
	m_MyApp.wantToGuiActivityCallbacks( this, true );
    if( m_HisIdent )
    {
        m_MyApp.toGuiAssetAction( eAssetActionRxViewingMsg, m_HisIdent->getMyOnlineId(), 1 );
    }
}

//============================================================================
void AppletMultiMessenger::hideEvent( QHideEvent * ev )
{
    if( m_HisIdent )
    {
        m_MyApp.toGuiAssetAction( eAssetActionRxViewingMsg, m_HisIdent->getMyOnlineId(), 0 );
    }

	m_MyApp.wantToGuiActivityCallbacks( this, false );
    if( ePluginTypeInvalid != m_ePluginType )
    {
        m_MyApp.setPluginVisible( m_ePluginType, false );
    }

    AppletPeerBase::hideEvent( ev );
}

//============================================================================
MultiSessionState *	 AppletMultiMessenger::getMSessionState( EMSessionType sessionType )
{
	if( (sessionType >= 0 ) 
		&& ( sessionType < m_MSessionsList.size() ) )
	{
		return m_MSessionsList[ sessionType ];
	}

	return 0;
}

//============================================================================
void AppletMultiMessenger::onSessionStateChange( ESessionState eSessionState )
{

}

//============================================================================
// called from session logic
void AppletMultiMessenger::onInSession( bool isInSession )
{
	QVector<MultiSessionState*>::iterator iter;
	for( iter = m_MSessionsList.begin();  iter != m_MSessionsList.end(); ++iter )
	{
		(*iter)->onInSession( isInSession );
	}
}

//============================================================================
void AppletMultiMessenger::toGuiMultiSessionAction(  EMSessionAction mSessionAction, VxGUID& onlineId, int pos0to100000 )
{
	if( m_HisIdent && m_HisIdent->getMyOnlineId() == onlineId )
	{

		if( ( onlineId.getVxGUIDHiPart() == m_HisIdent->getMyOnlineId().getVxGUIDHiPart() ) &&
			( onlineId.getVxGUIDLoPart() == m_HisIdent->getMyOnlineId().getVxGUIDLoPart() ) )
		{
			if( eMSessionActionChatSessionAccept == mSessionAction )
			{
				onInSession( true );
			}
			else if( eMSessionActionChatSessionReject == mSessionAction )
			{
				onInSession( false );
			}

			MultiSessionState* sessionState = getMSessionState( ( EMSessionType )pos0to100000 );
			if( sessionState )
			{
				sessionState->handleMultiSessionAction( mSessionAction );
			}
		}
	}
}

//============================================================================
void AppletMultiMessenger::toGuiClientPlayVideoFrame(	
														VxGUID&			onlineId, 
														uint8_t *		pu8Jpg, 
														uint32_t		u32JpgDataLen,
														int				motion0To100000 )
{
	if( m_IsInitialized )
	{
		ui.m_SessionWidget->playVideoFrame( onlineId, pu8Jpg, u32JpgDataLen, motion0To100000 );
		m_TodGameWidget->ui.TruthOrDareWidget->playVideoFrame( onlineId, pu8Jpg, u32JpgDataLen, motion0To100000 );
		if( m_VidChatWidget && m_VidChatWidget->isVisible() )
		{
			m_VidChatWidget->playVideoFrame( onlineId, pu8Jpg, u32JpgDataLen, motion0To100000 );
		}
	}
}

//============================================================================
void AppletMultiMessenger::toGuiPluginSessionEnded( GuiOfferSession * offer )
{
	//handleSessionEnded( offerResponse, this );
}; 

//============================================================================
void AppletMultiMessenger::setStatusMsg( QString strStatus )
{
	m_MyApp.toGuiStatusMessage( strStatus.toUtf8().constData() );
}

//============================================================================
void AppletMultiMessenger::toGuiSetGameValueVar(	
													EPluginType ePluginType, 
													VxGUID&		onlineId, 
													int32_t		s32VarId, 
													int32_t		s32VarValue )
{
	if( ( ePluginType == m_ePluginType )
		&& ( onlineId == m_HisIdent->getMyOnlineId() ) )
	{
		// emit signalToGuiSetGameValueVar( s32VarId, s32VarValue );
	}
}

//============================================================================
void AppletMultiMessenger::toGuiSetGameActionVar(	
													EPluginType     ePluginType, 
													VxGUID&		    onlineId, 
													int32_t			s32VarId, 
													int32_t			s32VarValue )
{
	if( ( ePluginType == m_ePluginType )
		&& ( onlineId == m_HisIdent->getMyOnlineId() ) )
	{
		// emit signalToGuiSetGameActionVar( s32VarId, s32VarValue );
	}
}

//============================================================================
void AppletMultiMessenger::slotEyeHostButtonClicked( void )
{
    if( ui.m_UserListWidget->isVisible() )
    {
        ui.m_UserListWidget->setVisible( false );
        ui.m_EyeHosts->setIcon( eMyIconEyeHide );
    }
    else
    {
        ui.m_UserListWidget->setVisible( true );
        ui.m_EyeHosts->setIcon( eMyIconEyeShow );
    }
}

//============================================================================
void AppletMultiMessenger::slotEyeSessionButtonClicked( void )
{
    if( ui.m_SessionFrame->isVisible() )
    {
        ui.m_SessionFrame->setVisible( false );
        ui.m_EyeHosts->setIcon( eMyIconEyeHide );
    }
    else
    {
        ui.m_SessionFrame->setVisible( true );
        ui.m_EyeHosts->setIcon( eMyIconEyeShow );
    }
}

//============================================================================
void AppletMultiMessenger::slotFriendsButtonClicked( void )
{

}

//============================================================================
void AppletMultiMessenger::slotGroupHostButtonClicked( void )
{

}

//============================================================================
void AppletMultiMessenger::slotChatRoomHostButtonClicked( void )
{

}

//============================================================================
void AppletMultiMessenger::slotRandomConnectHostButtonClicked( void )
{

}

//============================================================================
void AppletMultiMessenger::slotOfferFileButtonClicked( void )
{

}

