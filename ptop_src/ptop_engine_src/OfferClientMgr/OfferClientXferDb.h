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

#include <config_appcorelibs.h>

#include <CoreLib/DbBase.h>
#include <CoreLib/VxGUID.h>

class OfferClientInfo;

class OfferClientXferDb : public DbBase
{
public:
	OfferClientXferDb();
	virtual ~OfferClientXferDb();

	void						lockOfferClientXferDb( void )			{ m_OfferClientXferDbMutex.lock(); }
	void						unlockOfferClientXferDb( void )			{ m_OfferClientXferDbMutex.unlock(); }

	virtual RCODE				onCreateTables( int iDbVersion );
	virtual RCODE				onDeleteTables( int iOldVersion );

	void 						addOfferClient( VxGUID& assetOfferId );
	void						removeOfferClient( VxGUID& assetOfferId );

	void						getAllOfferClients( std::vector<VxGUID>& assetList );
	void						purgeAllOfferClientXfer( void ); 

protected:
	VxMutex						m_OfferClientXferDbMutex;
};

