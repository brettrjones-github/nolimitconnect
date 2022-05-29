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

#include <QWidget> // must be declared first or linux Qt 6.2.4 will error in qmetatype.h 2167:23: array subscript value ‘53’ is outside the bounds

#include "MultiSessionState.h"
#include "AppletMultiMessenger.h"
#include "AppCommon.h"

#include "VxPushButton.h"
#include "GuiHelpers.h"
#include "GuiParams.h"

#include <QFrame>

//========================================================================
MultiSessionState::MultiSessionState( AppCommon& myApp, AppletMultiMessenger& activityToFriendMultiSession, EMSessionType sessionType )
: QWidget( &activityToFriendMultiSession )
, m_MyApp( myApp )
, m_IFromGui( myApp.getEngine().getFromGuiInterface() )
, m_Activity( activityToFriendMultiSession )
, m_eMSessionType( sessionType )
, m_SessionState( eMSessionStateUnknown )
, m_HisIdent( 0 )
, m_OffersLayout( 0 )
, m_ResponseLayout( 0 )
, m_HangupLayout( 0 )
, m_SessionOfferButton( 0 )
, m_AcceptSessionButton( 0 )
, m_AcceptSessionText( 0 )
, m_RejectSessionButton( 0 )
, m_RejectSessionText( 0 )
, m_HangupButton( 0 )
, m_VidCamView( 0 )
, m_IsInChatSession( false )
{
}

//========================================================================
void MultiSessionState::setGuiWidgets(	GuiUser *		    hisIdent,
										QFrame * 			offersLayout,
										QFrame * 			responseLayout,
										QFrame * 			hangupLayout,
										VxPushButton *		sessionOfferButton,
										VxPushButton *		acceptSessionButton,
										QLabel *			acceptSessionText,
										VxPushButton *		rejectSessionButton,
										QLabel *			rejectSessionText,
										VxPushButton *		hangupButton,
										VidWidget * 		vidCamView,
										TodGameLogic *		todGameLogic )
{
	m_HisIdent				= hisIdent;
	m_HangupLayout 			= hangupLayout;
	m_OffersLayout 			= offersLayout;
	m_ResponseLayout 		= responseLayout;
	m_SessionOfferButton 	= sessionOfferButton;
	m_AcceptSessionButton 	= acceptSessionButton;	
	m_AcceptSessionText		= acceptSessionText;
	m_RejectSessionButton 	= rejectSessionButton;
	m_RejectSessionText		= rejectSessionText;
	m_HangupButton 			= hangupButton;		
	m_VidCamView			= vidCamView;
	m_TodGameLogic			= todGameLogic;

	switch( m_eMSessionType )
	{
	case eMSessionTypePhone:
		m_SessionOfferButton->setIcons( eMyIconVoicePhoneNormal );
		break;
	case eMSessionTypeVidChat:
		m_SessionOfferButton->setIcons( eMyIconVideoPhoneNormal );
		break;
	case eMSessionTypeTruthOrDare:
		m_SessionOfferButton->setIcons( eMyIconTruthOrDareNormal );
		break;
    case eMaxMSessionType:
    default:
        break;
	}

	connect( m_SessionOfferButton,		SIGNAL(clicked()), this, SLOT(onSessionOfferButtonClicked()) );
	connect( m_HangupButton,			SIGNAL(clicked()), this, SLOT(onHangupButtonClicked()) );
}

//========================================================================
void MultiSessionState::checkForSendAccess()
{
	if( canSend() )
	{
		if( ( eMSessionStateDisabled == m_SessionState ) 
			|| ( eMSessionStateUnknown == m_SessionState ) )
		{
			setSessionState( eMSessionStateIdle );
		}
	}
	else
	{
		setSessionState( eMSessionStateDisabled );
	}	
}

//========================================================================
void MultiSessionState::onSessionOfferButtonClicked()
{
	if( false == canSend() )
	{
		std::string cantSendReason = describeCantSendReason();
		m_MyApp.toGuiStatusMessage( cantSendReason.c_str() );
		return;
	}

	if( eMSessionStateDisabled == m_SessionState )
	{
		return;
	}

	if( eMSessionStateIdle == m_SessionState )
	{
		if( false == m_IFromGui.fromGuiMultiSessionAction( eMSessionActionOffer, m_HisIdent->getMyOnlineId(), (int)m_eMSessionType, m_LclSessionId ) )
		{
			LogMsg( LOG_INFO, "MultiSessionState::onSessionOfferButtonClicked user offline\n" ); 
			return;
		}

		setSessionState( eMSessionStateWaitingOfferResponse );	
	}
	else if( eMSessionStateWaitingOfferResponse == m_SessionState )
	{
		// is being canceled
		if( false == m_IFromGui.fromGuiMultiSessionAction( eMSessionActionReject, m_HisIdent->getMyOnlineId(), (int)m_eMSessionType, m_LclSessionId ) )
		{
			LogMsg( LOG_INFO, "MultiSessionState::onSessionOfferButtonClicked user offline\n" ); 
			return;
		}

		setSessionState( eMSessionStateIdle );	
	}
	else if( eMSessionStateWaitingClickSessionButton == m_SessionState )
	{
		// user has been offered and icon is blinking now he clicked so show accept or reject buttons
		setSessionState( eMSessionStateWaitingUserAcceptReject );	
	}
	else
	{
		LogMsg( LOG_INFO, "MultiSessionState::onSessionOfferButtonClicked state %d\n", m_SessionState );
	}
}

//========================================================================
bool MultiSessionState::canSend( void )
{
	if( m_IsInChatSession
		&& m_HisIdent->isOnline() )
	{
		EPluginAccess accessState = ePluginAccessLocked; 
		switch( m_eMSessionType )
		{
		case eMSessionTypePhone:
			accessState = m_HisIdent->getMyAccessPermissionFromHim( ePluginTypeVoicePhone );
			break;
		case eMSessionTypeVidChat:
			accessState = m_HisIdent->getMyAccessPermissionFromHim( ePluginTypeVideoPhone );
			break;
		case eMSessionTypeTruthOrDare:
			accessState = m_HisIdent->getMyAccessPermissionFromHim( ePluginTypeTruthOrDare );
			break;
        default:
            break;
		}

		return ( ePluginAccessOk == accessState );
	}

	return false;
}

//========================================================================
std::string MultiSessionState::describeCantSendReason( void )
{
	std::string reasonStr = "";
	int accessState = ePluginAccessLocked; 
	switch( m_eMSessionType )
	{
	case eMSessionTypePhone:
		reasonStr = GuiParams::describePlugin( ePluginTypeVoicePhone, false );
		accessState = m_HisIdent->getMyAccessPermissionFromHim( ePluginTypeVoicePhone );
		break;
	case eMSessionTypeVidChat:
		reasonStr = GuiParams::describePlugin( ePluginTypeVideoPhone, false );
		accessState = m_HisIdent->getMyAccessPermissionFromHim( ePluginTypeVideoPhone );
		break;
	case eMSessionTypeTruthOrDare:
		reasonStr = GuiParams::describePlugin( ePluginTypeTruthOrDare, false );
		accessState = m_HisIdent->getMyAccessPermissionFromHim( ePluginTypeTruthOrDare );
		break;

    case eMaxMSessionType:
    default:
        break;
	}

	reasonStr += " is disabled because ";
	if( ePluginAccessOk != accessState )
	{
		reasonStr += "Insufficient Permission";
	}
	else if( false == m_HisIdent->isOnline() )
	{
		reasonStr += m_HisIdent->getOnlineName();	
		reasonStr += "Is Offline";							
	}
	else if( false == m_IsInChatSession )
	{
		reasonStr += m_HisIdent->getOnlineName();		
		reasonStr += "Is Not In Chat Session With You";				
	}
	else
	{
		reasonStr += "Unknown Reason";		
	}

	return reasonStr;
}

//========================================================================
void MultiSessionState::onAcceptSessionButtonClicked()
{
	disconnect( m_AcceptSessionButton,		SIGNAL(clicked()), this, SLOT(onAcceptSessionButtonClicked()) );
	disconnect( m_RejectSessionButton,		SIGNAL(clicked()), this, SLOT(onRejectSessionButtonClicked()) );

	setSessionState( eMSessionStateInSession );
	m_IFromGui.fromGuiMultiSessionAction( eMSessionActionAccept, m_HisIdent->getMyOnlineId(), (int)m_eMSessionType, m_LclSessionId );
}

//========================================================================
void MultiSessionState::onRejectSessionButtonClicked()
{
	disconnect( m_AcceptSessionButton,		SIGNAL(clicked()), this, SLOT(onAcceptSessionButtonClicked()) );
	disconnect( m_RejectSessionButton,		SIGNAL(clicked()), this, SLOT(onRejectSessionButtonClicked()) );

	setSessionState( eMSessionStateIdle );
	m_IFromGui.fromGuiMultiSessionAction( eMSessionActionReject, m_HisIdent->getMyOnlineId(), (int)m_eMSessionType, m_LclSessionId );
}

//========================================================================
void MultiSessionState::onHangupButtonClicked()
{
	if( eMSessionStateIdle != m_SessionState )
	{
		setSessionState( eMSessionStateIdle );
		m_IFromGui.fromGuiMultiSessionAction( eMSessionActionHangup, m_HisIdent->getMyOnlineId(), (int)m_eMSessionType, m_LclSessionId );
	}
}

//========================================================================
void MultiSessionState::onInSession( bool isInSession )
{
	m_IsInChatSession = isInSession;
	if( ( false == m_IsInChatSession )
		&& ( eMSessionStateUnknown != m_SessionState )
		&& ( eMSessionStateIdle != m_SessionState )
		&& ( eMSessionStateDisabled != m_SessionState ) )
	{
		// force stop
		m_MyApp.getEngine().fromGuiMultiSessionAction( eMSessionActionHangup, m_HisIdent->getMyOnlineId(), (int)m_eMSessionType, m_LclSessionId );			
		setSessionState( eMSessionStateIdle );
	}

	checkForSendAccess();
}

//========================================================================
void MultiSessionState::setSessionState( EMSessionState sessionState )
{	
	if( eMSessionStateDisabled == sessionState )
	{
		if( eMSessionStateDisabled != m_SessionState )
		{
			m_SessionState = sessionState;
			resetGui();
			setButtonImage( m_SessionOfferButton, eImageTypeDisabled );
			//m_SessionOfferButton->setEnabled( false );
		}

		return;
	}

	if( sessionState == m_SessionState )
	{
		return;
	}

	if( ( eMSessionStateInSession == m_SessionState )
		&& ( eMSessionStateInSession != sessionState ) )
	{
		if( eMSessionTypeTruthOrDare == m_eMSessionType )
		{
			m_TodGameLogic->endGame();
		}

		resetGui();
	}

	switch( sessionState )
	{
	case eMSessionStateIdle:
		resetGui();
		m_SessionState = sessionState;
		setButtonImage( m_SessionOfferButton, eImageTypeNormal );
		m_SessionOfferButton->setEnabled( true );
		break;

	case eMSessionStateWaitingOfferResponse:
		//m_SessionOfferButton->setEnabled( false );
		setButtonImage( m_SessionOfferButton, eImageTypeCancel );
		m_SessionOfferButton->setEnabled( true );
		m_SessionState = sessionState;
		switch( m_eMSessionType )
		{
		case eMSessionTypePhone:
			setStatusText( tr("Waiting Phone Answer" ) );
			break;
		case eMSessionTypeVidChat:
			setStatusText( tr("Waiting Video Chat Offer Response" ) );
			break;
		case eMSessionTypeTruthOrDare:
			setStatusText( tr("Waiting Truth Or Dare Offer Response" ) );
			break;

        case eMaxMSessionType:
        default:
            break;		}

		break;

	case eMSessionStateWaitingClickSessionButton :
		m_SessionState = sessionState;
		m_SessionOfferButton->setEnabled( true );
		m_SessionOfferButton->setNotifyOnlineEnabled( true );
		switch( m_eMSessionType )
		{
		case eMSessionTypePhone:
			setStatusText( tr("Please Answer Phone" ) );
			break;
		case eMSessionTypeVidChat:
			setStatusText( tr("Video Chat Offer Available" ) );
			break;
		case eMSessionTypeTruthOrDare:
			setStatusText( tr("Truth Or Dare Offer Available" ) );
			break;
        default:
            break;
		}

		break;

	case eMSessionStateWaitingUserAcceptReject:
		m_SessionState = sessionState;
		m_SessionOfferButton->setNotifyOnlineEnabled( false );
		m_SessionOfferButton->setEnabled( true );
		switch( m_eMSessionType )
		{
		case eMSessionTypePhone:
			m_AcceptSessionText->setText( tr("Answer Phone" ) );
			m_RejectSessionText->setText( tr("Reject Phone Call" ) );
			break;
		case eMSessionTypeVidChat:
			m_AcceptSessionText->setText( tr("Accept Video Chat" ) );
			m_RejectSessionText->setText( tr("Reject Video Chat" ) );
			break;
		case eMSessionTypeTruthOrDare:
			m_AcceptSessionText->setText( tr("Play Truth Or Dare" ) );
			m_RejectSessionText->setText( tr("Reject Truth Or Dare" ) );
			break;

        case eMaxMSessionType:
        default:
            break;
		}

		setButtonImage( m_AcceptSessionButton, eImageTypeNormal );
		setButtonImage( m_RejectSessionButton, eImageTypeCancel );
		connect( m_AcceptSessionButton,		SIGNAL(clicked()), this, SLOT(onAcceptSessionButtonClicked()) );
		connect( m_RejectSessionButton,		SIGNAL(clicked()), this, SLOT(onRejectSessionButtonClicked()) );
		m_VidCamView->setVisible( false );
		m_TodGameLogic->setVisible( false );
		m_OffersLayout->setVisible( false );
		m_ResponseLayout->setVisible( true );
		m_HangupLayout->setVisible( false );
		break;

	case eMSessionStateInSession:
		setButtonImage( m_HangupButton, eImageTypeCancel );
		m_OffersLayout->setVisible( false );
		m_ResponseLayout->setVisible( false );
		m_HangupLayout->setVisible( true );
		switch( m_eMSessionType )
		{
		case eMSessionTypePhone:
			setStatusText( tr("Phone Connected" ) );
			m_VidCamView->setVisible( false );
			m_TodGameLogic->setVisible( false );
			break;

		case eMSessionTypeVidChat:
			setStatusText( tr("In Video Chat Session" ) );
			m_VidCamView->setVisible( true );
			m_TodGameLogic->setVisible( false );
			break;

		case eMSessionTypeTruthOrDare:
			setStatusText( tr("In Truth Or Dare Session" ) );
			m_VidCamView->setVisible( false );
			m_TodGameLogic->setVisible( true );
			m_TodGameLogic->beginGame( eMSessionStateWaitingOfferResponse == m_SessionState  );
			break;

        case eMaxMSessionType:
        default:
            break;
		}

		m_SessionState = sessionState;
		break;

	case eMSessionStateOfferRejected:
		resetGui();
		m_SessionState = eMSessionStateIdle;
		setButtonImage( m_SessionOfferButton, eImageTypeNormal );
		m_SessionOfferButton->setNotifyOnlineEnabled( false );
		m_SessionOfferButton->setEnabled( true );
		switch( m_eMSessionType )
		{
		case eMSessionTypePhone:
			setStatusText( tr("Phone Offer Rejected" ) );
			break;
		case eMSessionTypeVidChat:
			setStatusText( tr("Video Chat Offer Rejected" ) );
			break;
		case eMSessionTypeTruthOrDare:
			setStatusText( tr("Truth Or Dare Offer Rejected" ) );
			break;

        case eMaxMSessionType:
        default:
            break;
		}

		break;

	default:
		break;
	}
}

//========================================================================
void MultiSessionState::handleMultiSessionAction( EMSessionAction mSessionAction )
{
	switch( mSessionAction )
	{
	case eMSessionActionChatSessionReq:
		m_IFromGui.fromGuiMultiSessionAction( eMSessionActionChatSessionAccept, m_HisIdent->getMyOnlineId(), 0, m_LclSessionId );
		break;

	case eMSessionActionChatSessionAccept:
	case eMSessionActionChatSessionReject:
		break;

	case eMSessionActionOffer:
		setSessionState( eMSessionStateWaitingClickSessionButton );
		break;

	case eMSessionActionAccept:
		setSessionState( eMSessionStateInSession );
		break;

	case eMSessionActionReject:
		setSessionState( eMSessionStateOfferRejected );
		break;

	case eMSessionActionHangup:
		setSessionState( eMSessionStateIdle );
		switch( m_eMSessionType )
		{
		case eMSessionTypePhone:
			setStatusText( QObject::tr("User Hung Up Phone") );
			break;
		case eMSessionTypeVidChat:
			setStatusText( QObject::tr("User Ended Video Chat") );
			break;
		case eMSessionTypeTruthOrDare:
			setStatusText( QObject::tr("User Ended Truth Or Dare") );
			break;

        case eMaxMSessionType:
        default:
            break;
		}

		break;

	default:
		break;
	}
}

//========================================================================
void MultiSessionState::setButtonImage( VxPushButton * button, EImageType imageType )
{
	switch( m_eMSessionType )
	{
	case eMSessionTypePhone:
		switch( imageType )
		{
		case eImageTypeNormal: 			
			button->setIcons( eMyIconVoicePhoneNormal, eMyIconVoicePhoneDisabled );
			break;
		case eImageTypeDisabled: 			
			button->setIcons( eMyIconVoicePhoneDisabled );
			break;
		case eImageTypeCancel: 			
			button->setIcons( eMyIconVoicePhoneCancel );
			break;
		case eImageTypeRed: 			
			button->setIcons( eMyIconVoicePhoneNormal );
			break;
		case eImageTypeYellow: 			
			button->setIcons( eMyIconVoicePhoneNormal );
			break;
		}

		break;

	case eMSessionTypeVidChat:
		switch( imageType )
		{
		case eImageTypeNormal: 			
			button->setIcons( eMyIconVideoPhoneNormal, eMyIconVideoPhoneDisabled );
			break;
		case eImageTypeDisabled: 			
			button->setIcons( eMyIconVideoPhoneDisabled );
			break;
		case eImageTypeCancel: 			
			button->setIcons( eMyIconVideoPhoneCancel );
			break;
//		case eImageTypeRed:
//			button->setIcons( eMyIconVideoPhoneRed );
//			break;
//		case eImageTypeYellow:
//			button->setIcons( eMyIconVideoPhoneYellow );
//			break;
        default:
            break;
		}

		break;

	case eMSessionTypeTruthOrDare:
		switch( imageType )
		{
		case eImageTypeNormal: 			
			button->setIcons( eMyIconTruthOrDareNormal, eMyIconTruthOrDareDisabled );
			break;
		case eImageTypeDisabled: 			
			button->setIcons( eMyIconTruthOrDareDisabled );
			break;
		case eImageTypeCancel: 			
			button->setIcons( eMyIconTruthOrDareCancel );
			break;
		//case eImageTypeRed: 			
		//	button->setIcons( eMyIconTruthOrDareRed );
		//	break;
		//case eImageTypeYellow: 			
		//	button->setIcons( eMyIconTruthOrDareYellow );
		//	break;
        default:
            break;
        }

		break;

	default:
		button->setIcons( eMyIconAnonymous ); // unknown
		break;
	}   		
}

//========================================================================
void MultiSessionState::setStatusText( QString statusText )
{
	m_MyApp.toGuiStatusMessage( statusText.toUtf8().constData() );
}

//========================================================================
void MultiSessionState::resetGui()
{
	setStatusText( "" );
	m_HangupLayout->setVisible( false );
	m_OffersLayout->setVisible( true );
	m_ResponseLayout->setVisible( false );
	m_SessionOfferButton->setNotifyOnlineEnabled( false );
	m_VidCamView->setVisible( false );
	m_TodGameLogic->setVisible( false );
	if( canSend() )
	{
		setButtonImage( m_SessionOfferButton, eImageTypeNormal );
		m_SessionOfferButton->setEnabled( true );
	}
	else
	{
		setButtonImage( m_SessionOfferButton, eImageTypeDisabled );
		//m_SessionOfferButton->setEnabled( false );
	}
}
