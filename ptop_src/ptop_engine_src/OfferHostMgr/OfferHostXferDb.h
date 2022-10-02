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
// http://www.nolimitconnect.org
//============================================================================

#include <config_appcorelibs.h>

#include <CoreLib/DbBase.h>
#include <CoreLib/VxGUID.h>

class OfferHostInfo;

class OfferHostXferDb : public DbBase
{
public:
	OfferHostXferDb( const char* stateDbName );
	virtual ~OfferHostXferDb() = default;

	void						lockOfferHostXferDb( void )			    { m_OfferHostXferDbMutex.lock(); }
	void						unlockOfferHostXferDb( void )			{ m_OfferHostXferDbMutex.unlock(); }

	virtual RCODE				onCreateTables( int iDbVersion );
	virtual RCODE				onDeleteTables( int iOldVersion );

	void 						addOfferHost( VxGUID& assetOfferId );
	void						removeOfferHost( VxGUID& assetOfferId );

	void						getAllOfferHosts( std::vector<VxGUID>& assetList );
	void						purgeAllOfferHostXfer( void ); 

protected:
	VxMutex						m_OfferHostXferDbMutex;
};

