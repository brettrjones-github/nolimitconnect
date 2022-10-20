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
#include <GuiInterface/IDefs.h>

class OfferBaseInfo;
class OfferBaseMgr;
class VxGUID;
class VxSha1Hash;

class OfferBaseInfoDb : public DbBase
{
public:
	OfferBaseInfoDb( OfferBaseMgr& mgr, const char*dbName );
	virtual ~OfferBaseInfoDb() = default;

	void						lockOfferInfoDb( void )					{ m_OfferBaseInfoDbMutex.lock(); }
	void						unlockOfferInfoDb( void )				{ m_OfferBaseInfoDbMutex.unlock(); }

	void						addOffer(	VxGUID&			assetId, 
											VxGUID&			creatorId, 
											VxGUID&			historyId, 
                                            VxGUID&			thumbId, 
											const char*	assetName, 
											int64_t			assetLen, 
											uint32_t		assetType, 							
											VxSha1Hash&		hashId, 
											uint32_t		locationFlags, 
                                            uint32_t		attributedFlags, 
                                            int             isTemp,
                                            int64_t			createdTimestamp = 0,
                                            int64_t			modifiedTimestamp = 0,     
                                            int64_t			accessedTimestamp = 0,          
											const char*	assetTag = "", 
											EOfferSendState sendState = eOfferSendStateNone);

	void 						addOffer( OfferBaseInfo* assetInfo );

	void						removeOffer( const char* assetName );
	void						removeOffer( VxGUID& assetId );
	void						removeOffer( OfferBaseInfo* assetInfo );

	void						getAllOffers( std::vector<OfferBaseInfo*>& OfferBaseOfferList );
	void						purgeAllOffers( void ); 
	void						updateOfferSendState( VxGUID& assetId, EOfferSendState sendState );

protected:
    virtual OfferBaseInfo*     createOfferInfo( std::string assetName, uint64_t assetLen, uint16_t assetType ) = 0;
    virtual OfferBaseInfo*     createOfferInfo( OfferBaseInfo& assetInfo ) = 0;

	virtual RCODE				onCreateTables( int iDbVersion );
	virtual RCODE				onDeleteTables( int iOldVersion );
	void						insertOfferInTimeOrder( OfferBaseInfo*assetInfo, std::vector<OfferBaseInfo*>& assetList );

	OfferBaseMgr&				m_OfferMgr;
	VxMutex						m_OfferBaseInfoDbMutex;
};

