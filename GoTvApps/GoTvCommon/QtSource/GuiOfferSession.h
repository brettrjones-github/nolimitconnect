#pragma once
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
// http://www.gotvptop.net
//============================================================================

#include "config_gotvapps.h"

#include "VxGuidQt.h"

#include <CoreLib/VxSha1Hash.h>
#include <PktLib/VxCommon.h>

#include <QObject>

class P2PEngine;
class OfferWidget;
class OfferSessionState;
class AppCommon;

// offer from remote friend
class GuiOfferSession : public QObject
{
	Q_OBJECT

public:
	GuiOfferSession();
	GuiOfferSession(const GuiOfferSession& rhs);
	GuiOfferSession& operator=(const GuiOfferSession& rhs); 

	VxGUID&						assuredValidLclSessionId( void );
	OfferSessionState *			createNewSessionState( AppCommon& myApp, GuiOfferSession * offerSession );

	void						setPluginType( EPluginType type )		{ m_ePluginType = type; }
	EPluginType					getPluginType( void )					{ return m_ePluginType; }

	void						setIsRemoteInitiated( bool bIsRemoteInitiated ) { m_bRmtInitiated = bIsRemoteInitiated; }
	bool						getIsRemoteInitiated( void )			{ return m_bRmtInitiated; }

	void						setHisIdent( VxNetIdent * netIdent )	{ m_HisIdent = netIdent; }
	VxNetIdent *				getHisIdent( void )						{ return m_HisIdent; }
	void						setOfferMsg( const char * msg )			{ m_strOfferMsg = msg; }
	std::string&				getOfferMsg( void )						{ return m_strOfferMsg; }
	std::vector<std::string>&	getQuedMessages( void )					{ return m_aoQuedMessages; }

	void						setLclSessionId( VxGuidQt lclSessionId)	{ m_LclSessionId = lclSessionId;}
	VxGuidQt					getLclSessionId( void )					{ return m_LclSessionId; }
	void						setRmtSessionId( VxGuidQt rmtSessionId ){ m_RmtSessionId = rmtSessionId;}
	VxGuidQt					getRmtSessionId( void )					{ return m_RmtSessionId; }
	void						setOfferSessionId( VxGuidQt offerSessionId ){ m_OfferSessionId = offerSessionId;}
	VxGuidQt					getOfferSessionId( void )				{ return m_OfferSessionId; }

	void						setProgress( int iProgress )			{ m_iProgress = iProgress; }
	int							getProgress( void )						{ return m_iProgress; }

	void						setHasBeenViewed( bool bHasBeenViewed ) { m_bHasBeenViewed = bHasBeenViewed; }
	bool						getHasBeenViewed( void )				{ return m_bHasBeenViewed; }
	void						setRequiresReply( bool bRequiresReply ) { m_bRequiresReply = bRequiresReply; }
	bool						getRequiresReply( void )				{ return m_bRequiresReply; }

	bool						hasNewResponse( void )					{ return m_bRequiresReply; }
	void						setHasNewResponse( bool bNewResponse )	{ m_bRequiresReply = bNewResponse; }

	void						setOfferResponse( EOfferResponse eOfferResponse ) { m_bHasNewResponse = true; m_eOfferResponse = eOfferResponse; }
	EOfferResponse				getOfferResponse( void )				{ return m_eOfferResponse; }

	bool						isOfferAccepted( void )					{ return (m_eOfferResponse == eOfferResponseAccept); }
	bool						isOfferRejected( void )					{ return (m_eOfferResponse == eOfferResponseReject)||(m_eOfferResponse == eOfferResponseEndSession); }
	void						addMsg( const char * pMsg );

	void						setIsMissedCall( void )					{ m_MissedCalls++; }
	void						setMissedCallsCnt( int missedCnt )		{ m_MissedCalls = missedCnt; }
	int							getMissedCallsCnt( void )				{ return m_MissedCalls; }

	void						setFileName( const char * fileName );
	std::string&				getFileName( void )						{ return m_strFileName; }
	void						setFileHashId( VxSha1Hash& hashId )		{ m_FileHashId = hashId; }
	void						setFileHashId( uint8_t * id )				{ m_FileHashId.setHashData( id ); }
	VxSha1Hash&					getFileHashId( void )					{ return m_FileHashId; }

	void						setUserData( int userData )				{ m_pvUserData = userData; }
	int							getUserData( void )						{ return m_pvUserData; }

protected:
	//=== vars ===//
	EPluginType					m_ePluginType;
	VxNetIdent *				m_HisIdent;
	int							m_pvUserData;
	bool						m_bRmtInitiated;
	bool						m_bHasBeenViewed;
	int							m_iProgress;
	bool						m_bRequiresReply;
	std::string					m_strOfferMsg;
	std::string					m_strFileName;
	VxGuidQt					m_LclSessionId;
	VxGuidQt					m_RmtSessionId;
	VxGuidQt					m_OfferSessionId;
	VxSha1Hash					m_FileHashId;
	bool						m_bHasNewResponse;
	EOfferResponse				m_eOfferResponse;
	std::vector<std::string>	m_aoQuedMessages;
	int							m_MissedCalls;
};
