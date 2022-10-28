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


#include <ptop_src/ptop_engine_src/OfferBase/OfferBaseMgr.h>

#include <CoreLib/VxThread.h>
#include <CoreLib/VxSemaphore.h>
#include <CoreLib/VxMutex.h>

 
class OfferCallbackInterface;

class OfferMgr : public OfferBaseMgr
{
public:
	OfferMgr( P2PEngine& engine, const char* dbName, const char* stateDbName );
	virtual ~OfferMgr() = default;

    void                        fromGuiUserLoggedOn( void ) override;

    bool				        fromGuiOfferCreated( OfferBaseInfo& offerInfo );
    bool				        fromGuiOfferUpdated( OfferBaseInfo& offerInfo );

    virtual void				announceOfferAdded( OfferBaseInfo* offerInfo ) override;
    virtual void				announceOfferUpdated( OfferBaseInfo* offerInfo ) override;
    virtual void				announceOfferRemoved( OfferBaseInfo* offerInfo ) override;
    virtual void				announceOfferXferState( VxGUID& assetOfferId, EOfferSendState assetSendState, int param ) override;

protected:

};

