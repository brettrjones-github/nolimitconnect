#pragma once
//============================================================================
// Copyright (C) 2021 Brett R. Jones
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

#include <config_gotvcore.h>

#include <CoreLib/DbBase.h>
#include <CoreLib/VxGUID.h>

class OfferBaseInfo;

class OfferBaseXferDb : public DbBase
{
public:
	OfferBaseXferDb();
	virtual ~OfferBaseXferDb();

	void						lockOfferBaseXferDb( void )			{ m_OfferBaseXferDbMutex.lock(); }
	void						unlockOfferBaseXferDb( void )			{ m_OfferBaseXferDbMutex.unlock(); }

	virtual RCODE				onCreateTables( int iDbVersion );
	virtual RCODE				onDeleteTables( int iOldVersion );

	void 						addOffer( VxGUID& assetOfferId );
	void						removeOffer( VxGUID& assetOfferId );

	void						getAllOffers( std::vector<VxGUID>& assetList );
	void						purgeAllOfferBaseXfer( void ); 

protected:
	VxMutex						m_OfferBaseXferDbMutex;
};

