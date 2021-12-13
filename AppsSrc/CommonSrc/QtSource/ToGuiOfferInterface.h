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

#include "OfferSessionState.h"

class GuiOfferSession;
class OfferSessionState;

class ToGuiOfferInterface
{
public:
	// called on gui thread
	virtual void				doGuiUpdatePluginOffer( OfferSessionState * offerState ) = 0; 
	virtual void				doGuiOfferRemoved( OfferSessionState * offerState ) = 0; 
	virtual void				doGuiAllOffersRemoved( void ) = 0; 
};

class FromGuiOfferInterface
{
public:
	virtual OfferSessionState *	getTopGuiOfferSession( void ) = 0; 
	virtual void				wantToGuiOfferCallbacks( ToGuiOfferInterface * clientInterface, bool wantCallbacks ) = 0; 
};
