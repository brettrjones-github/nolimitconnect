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
// bjones.engineer@gmail.com
// http://www.nolimitconnect.com
//============================================================================

#include "ToGuiOfferInterface.h"
#include "OfferSessionState.h"

#include <CoreLib/VxMutex.h>
#include <CoreLib/VxTimer.h>

#include <QWidget>

class AppCommon;
class VxGUID;
class GuiOfferSession;
class OfferSessionState;
class GuiUser;
class OfferWidget;
class QListWidgetItem;
class QTimer;

class OffersMgr : public QWidget, public FromGuiOfferInterface
{
	Q_OBJECT
public:
	OffersMgr( AppCommon& myApp );

	void						lockOffersMgr( void )						{ m_OffersMgrMutex.lock(); }
	void						unlockOffersMgr( void )						{ m_OffersMgrMutex.unlock(); }
	int							getNewOffersCount( void )					{ return m_iNewRmtOffersCount; }
	size_t						getTotalOffersCount( void )					{ return m_aoOffersList.size(); }

// from gui interface
	virtual OfferSessionState *	getTopGuiOfferSession( void ); // returns null if no session offers
	virtual void				wantToGuiOfferCallbacks( ToGuiOfferInterface * clientInterface, bool wantCallbacks ); 

// from engine
	void						toGuiRxedPluginOffer( GuiOfferSession * offerSession );
	void						toGuiRxedOfferReply( GuiOfferSession * offerSession );
	void						toGuiPluginSessionEnded( GuiOfferSession * offerSession );

	void						onIsInSession( EPluginType ePluginType, VxGUID offerSessionId, GuiUser * hisIdent, bool isInSession );
	void						onSessionExit( EPluginType ePluginType, VxGUID offerSessionId, GuiUser * hisIdent );
	void						startedSessionInReply( EPluginType ePluginType, VxGUID offerSessionId, GuiUser * hisIdent );

	void						acceptOfferButtonClicked( EPluginType ePluginType, VxGUID offerSessionId, GuiUser * hisIdent );
	void						rejectOfferButtonClicked( EPluginType ePluginType, VxGUID offerSessionId, GuiUser * hisIdent );


	// called if starting new session to know if responding to existing offer
	GuiOfferSession *			findActiveAndAvailableOffer( GuiUser * netIdent, EPluginType ePluginType );
	void						sentOffer( EPluginType ePluginType, VxGUID offerSessionId, GuiUser * hisIdent );
	void						sentOfferReply( EPluginType ePluginType, VxGUID offerSessionId, GuiUser * hisIdent, EOfferResponse eOfferResponse );
	void						removePluginSessionOffer( EPluginType ePluginType, GuiUser * netIdent );
	void						removePluginSessionOffer( VxGUID& offerSessionId );

signals:
	void						signalToGuiRxedPluginOffer( GuiOfferSession * offerSession );
	void						signalToGuiRxedOfferReply( GuiOfferSession * offerSession );
	void						signalToGuiPluginSessionEnded( GuiOfferSession * offerSession );

protected slots:
	void						slotOncePerSecond( void );

	void						slotToGuiRxedPluginOffer( GuiOfferSession * offerSession );
	void						slotToGuiRxedOfferReply( GuiOfferSession * offerSession );
	void						slotToGuiPluginSessionEnded( GuiOfferSession * offerSession );
	void						slotToGuiContactOffline( GuiUser * netIdent ); // connected to app signal

protected:
	GuiOfferSession *			findOffer( void * pvUserData );
	GuiOfferSession *			findOffer( GuiUser * netIdent, EPluginType ePluginType );

	void						recievedOffer( EPluginType ePluginType, VxGUID offerSessionId, GuiUser * hisIdent );
	void						recievedOfferReply( EPluginType ePluginType, VxGUID offerSessionId, GuiUser * hisIdent, EOfferResponse eOfferResponse );
	void						recievedSessionEnd( EPluginType ePluginType, VxGUID offerSessionId, GuiUser * hisIdent, EOfferResponse eOfferResponse );

	bool						isPhonePlugin( EPluginType ePluginType );
	bool						isOfferBasedPlugin( EPluginType ePluginType );
	//!	fill offer into new QListWidgetItem *
	QListWidgetItem *			offerToWidget( GuiOfferSession * poOffer );
	//!	get offer from QListWidgetItem data
	GuiOfferSession *			widgetToOffer( OfferWidget * item );

	bool						handleOfferResponse(  OfferWidget * item, GuiOfferSession * poOffer );
	void						handleMissedCall( GuiOfferSession * offerSession );

	bool						isMatch( OfferWidget * poOfferWidget, EPluginType ePluginType, GuiUser * netIdent );

	void						setOnlineState( GuiOfferSession * offerSession, bool isOnline );
	void						enableNotifyIconFlash( bool enable );
	void						updateNotifyStatus( void );
	void						playNotifySound( bool playSnd );

	void						changeOfferState( OfferSessionState * sessionState, EOfferState newOfferState );
	void						forceToNotInSession( OfferSessionState * sessionState );
	void						startRingTimerIfNotInSession( void );
	void						stopRingTimer( void );
	void						updateSndsAndMessages( OfferSessionState * sessionState );
	OfferSessionState *			findOrAddOfferSession( GuiOfferSession * offerSession );
    OfferSessionState *			findOfferSession( VxGUID sessionId );
	void						checkAndUpdateIfEmptyOfferList( void );


	//=== vars ===//
	static const int 			RING_COUNT 				= 4; 
	static const int 			RING_ELAPSE_SEC 		= 4; 

	AppCommon& 					m_MyApp;
	VxMutex 					m_OffersMgrMutex; 
	std::vector<OfferSessionState *> m_aoOffersList;	
	std::vector<ToGuiOfferInterface *> m_OfferCallbackList;	
	bool                        m_UserIsInSession;
	int							m_RingTimerCycleCnt;
	int							m_RingTimerSecondCnt;
	VxTimer						m_UpdateTimer;
	double						m_LastOfferUpdateTime;

	int							m_iNewRmtOffersCount;
	//QTimer *					m_OncePerSecTimer;
};
