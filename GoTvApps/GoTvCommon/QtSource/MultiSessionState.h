#pragma once
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
// http://www.gotvptop.net
//============================================================================

#include "config_gotvapps.h"
#include <GoTvInterface/IDefs.h>
#include <CoreLib/VxGUID.h>

enum EMSessionState
{
	eMSessionStateUnknown 					= 0,
	eMSessionStateDisabled 					= 1,
	eMSessionStateIdle 						= 2,
	eMSessionStateWaitingOfferResponse 		= 3,
	eMSessionStateWaitingClickSessionButton = 4,
	eMSessionStateWaitingUserAcceptReject 	= 5,
	eMSessionStateInSession 				= 6,
	eMSessionStateOfferRejected				= 7,

	eMaxMSessionState
};

enum EImageType
{
	eImageTypeNormal 			= 0,
	eImageTypeDisabled 			= 1,
	eImageTypeCancel 			= 2,
    eImageTypeRed 				= 3,
    eImageTypeYellow 			= 4,

//	eMaxImageType
};

#include <QWidget>

class AppCommon;
class ActivityToFriendMultiSession;
class QFrame;
class VxPushButton;
class QLabel;
class VidWidget;
class TodGameLogic;
class IFromGui;
class VxNetIdent;

class MultiSessionState : public QWidget
{
	Q_OBJECT

public:
	MultiSessionState( AppCommon& myApp, ActivityToFriendMultiSession& activityToFriendMultiSession, EMSessionType sessionType );

	void						setGuiWidgets(	VxNetIdent *		hisIdent,
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
												TodGameLogic *		todGameLogic );

	void 						checkForSendAccess( void );
	void						handleMultiSessionAction( EMSessionAction mSessionAction );
	void						onInSession( bool bResponseOk );

protected slots:
	void						onSessionOfferButtonClicked( void );
	void						onAcceptSessionButtonClicked( void );
	void						onRejectSessionButtonClicked( void );
	void						onHangupButtonClicked( void );

protected:
	void						setSessionState( EMSessionState sessionState );
	void						setButtonImage( VxPushButton * button, EImageType imageType );
	void						setStatusText( QString statusText );
	void						resetGui( void );

	bool						canSend( void );
	std::string					describeCantSendReason( void );

	AppCommon& 				m_MyApp;
	IFromGui&					m_IFromGui;
	ActivityToFriendMultiSession&  	m_Activity;
	EMSessionType				m_eMSessionType;
	EMSessionState				m_SessionState;

	VxNetIdent *				m_HisIdent;

	QFrame * 					m_OffersLayout;
	QFrame * 					m_ResponseLayout;
	QFrame * 					m_HangupLayout;

	VxPushButton *				m_SessionOfferButton;

	VxPushButton *				m_AcceptSessionButton;
	QLabel *					m_AcceptSessionText;
	VxPushButton *				m_RejectSessionButton;
	QLabel *					m_RejectSessionText;
	VxPushButton *				m_HangupButton;

	VidWidget * 				m_VidCamView;
	TodGameLogic *				m_TodGameLogic;
	VxGUID						m_LclSessionId;
	bool						m_IsInChatSession;
};
