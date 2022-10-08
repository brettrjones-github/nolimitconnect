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
// http://www.nolimitconnect.org
//============================================================================


#include "OfferSessionState.h"
#include "AppCommon.h"
#include "GuiOfferSession.h"
#include "GuiHelpers.h"
#include "GuiParams.h"

//============================================================================
OfferSessionState::OfferSessionState( AppCommon& myApp, GuiOfferSession* offerSession )
: m_MyApp( myApp )
, m_OfferSession( offerSession )
{
}

//============================================================================
OfferSessionState::~OfferSessionState()
{
	if( 0 != m_OfferSession )
	{
		m_OfferSession->deleteLater();
	}
}


//============================================================================
std::string OfferSessionState::describePlugin()
{
	return GuiParams::describePlugin( getPluginType(), getIsRemoteInitiated() );	
}

//============================================================================
std::string OfferSessionState::describeOffer()
{
	std::string offferDesc = getHisIdent()->getOnlineName();
	offferDesc +=  " Offers ";
	offferDesc += GuiParams::describePlugin( getPluginType(), getIsRemoteInitiated() );
	return offferDesc;	
}

//============================================================================
std::string OfferSessionState::getOnlineName()
{
	return getHisIdent()->getOnlineName();	
}

//============================================================================
bool OfferSessionState::isAvailableAndActiveOffer( void )
{
	if( false == getHisIdent()->isOnline() )
	{
		return false;
	}

	bool avail = false;
	switch( m_EOfferState )
	{
	case eOfferStateSending:
	case eOfferStateSent:
    case eOfferStateRxedByUser:
	case eOfferStateInSession:
		avail = true;
		break;
	default:
		break;
	}

	return avail;
}






