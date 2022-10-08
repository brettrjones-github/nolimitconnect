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

#include <QWidget> // must be declared first or Qt 6.2.4 will error in qmetatype.h 2167:23: array subscript value ‘53’ is outside the bounds

#include "GuiOfferSession.h"
#include "OfferSessionState.h"
#include "GuiHelpers.h"

//============================================================================
GuiOfferSession::GuiOfferSession(QWidget*parent)
: GuiUserSessionBase(parent)
, m_ePluginType(ePluginTypeInvalid)
, m_HisIdent(0)
, m_pvUserData(0)
, m_bRmtInitiated(false)
, m_bHasBeenViewed( false )
, m_iProgress(0)
, m_bRequiresReply( false )
, m_bHasNewResponse( false )
, m_eOfferResponse( eOfferResponseNotSet )
, m_MissedCalls( 0 )
{
}

//============================================================================
GuiOfferSession::GuiOfferSession(const GuiOfferSession& rhs)
: GuiUserSessionBase(rhs)
{
}

//============================================================================
GuiOfferSession& GuiOfferSession::operator=(const GuiOfferSession& rhs)
{
	if( this != &rhs )
	{
		m_ePluginType				= rhs.m_ePluginType;
		m_HisIdent					= rhs.m_HisIdent;
		m_pvUserData				= rhs.m_pvUserData;
		m_bRmtInitiated				= rhs.m_bRmtInitiated;
		m_bHasBeenViewed			= rhs.m_bHasBeenViewed;
		m_iProgress					= rhs.m_iProgress;
		m_bRequiresReply			= rhs.m_bRequiresReply;
		m_strOfferMsg				= rhs.m_strOfferMsg;
		m_strFileName				= rhs.m_strFileName;
		m_LclSessionId				= rhs.m_LclSessionId;
		m_RmtSessionId				= rhs.m_RmtSessionId;
		m_OfferSessionId			= rhs.m_OfferSessionId;
		m_FileHashId				= rhs.m_FileHashId;
		m_bHasNewResponse			= rhs.m_bHasNewResponse;
		m_eOfferResponse			= rhs.m_eOfferResponse;

		m_aoQuedMessages.clear();
		std::vector<std::string>::iterator iter;
		std::vector<std::string>& rhsMsgs = ((GuiOfferSession&)rhs).getQuedMessages();
		for( iter = rhsMsgs.begin(); iter != rhsMsgs.end(); ++iter )
		{
			m_aoQuedMessages.push_back( (*iter).c_str() );
		}

		m_MissedCalls				= rhs.m_MissedCalls;
	}

	return *this;
}

//======================================================================== 
VxGUID&   GuiOfferSession::assuredValidLclSessionId() 
{
	if( false == m_LclSessionId.isVxGUIDValid() )
	{
		m_LclSessionId.initializeWithNewVxGUID();
	}

	if( GuiHelpers::isPluginSingleSession( m_ePluginType ) )
	{
		m_OfferSessionId = m_HisIdent->getMyOnlineId();
	}
	else
	{
		m_OfferSessionId = m_LclSessionId;
	}

	return m_LclSessionId;
}

//============================================================================
OfferSessionState * GuiOfferSession::createNewSessionState( AppCommon& myApp, GuiOfferSession* offerSession )
{
	return new OfferSessionState( myApp, offerSession );
}

//============================================================================
void GuiOfferSession::addMsg( const char* pMsg )
{
	std::string msg = pMsg;
	m_aoQuedMessages.push_back( msg );
}

//============================================================================
void GuiOfferSession::setFileName( const char* fileName )
{
	if( fileName )
	{
		m_strFileName = fileName; 
	}
	else
	{
		m_strFileName = "";
	}
}

