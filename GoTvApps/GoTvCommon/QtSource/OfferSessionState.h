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


#include "GuiOfferSession.h"
#include <PktLib/VxCommon.h>

#include <QString>
#include <QObject>

class ActivityBase;
class OfferMgr;
class OfferSessionCallbackInterface;
class AppCommon;
class VxNetIdent;
class GuiOfferSession;

class OfferSessionState : public QObject
{
public:	
    OfferSessionState( AppCommon& myApp, GuiOfferSession * offerSession );
	virtual ~OfferSessionState();

	void						setPluginType( EPluginType ePluginType ) 			{ m_OfferSession->setPluginType( ePluginType ); }
	EPluginType 				getPluginType()										{ return m_OfferSession->getPluginType(); }
    void						setIsRemoteInitiated( bool bIsRemoteInitiated ) 	{ m_OfferSession->setIsRemoteInitiated( bIsRemoteInitiated ); }
    bool						getIsRemoteInitiated( void )						{ return m_OfferSession->getIsRemoteInitiated(); }
	void						setHisIdent( GuiUser * netIdent ) 				    { m_OfferSession->setHisIdent( netIdent ); }
    GuiUser *				    getHisIdent()										{ return m_OfferSession->getHisIdent(); }
	void 						setOfferSessionId( VxGUID& sessionId )				{ m_OfferSession->setOfferSessionId( sessionId ); }
	VxGUID						getOfferSessionId()									{ return m_OfferSession->getOfferSessionId(); }

	int							getMissedCallsCnt( void )							{ return m_OfferSession->getMissedCallsCnt(); }
	bool						isAvailableAndActiveOffer( void );

	void 						setOfferState( EOfferState eOfferState )			{ m_EOfferState = eOfferState; }
	EOfferState					getOfferState()										{ return m_EOfferState; }

	GuiOfferSession *			getGuiOfferSession()								{ return m_OfferSession; }

	std::string 				describePlugin( void );
	std::string 				describeOffer( void );
	std::string					getOnlineName( void );

	void						onContactOffline( void ); // user went offline

private:
	//=== vars ===//
	AppCommon& 				    m_MyApp;
	GuiOfferSession *			m_OfferSession;
    EOfferState					m_EOfferState{ eOfferStateNone };
};





