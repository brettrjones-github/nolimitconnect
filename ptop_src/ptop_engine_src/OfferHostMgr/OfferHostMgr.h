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

#include "OfferHostInfoDb.h"

#include <ptop_src/ptop_engine_src/OfferBase/OfferBaseMgr.h>

#include <CoreLib/VxThread.h>
#include <CoreLib/VxSemaphore.h>
#include <CoreLib/VxMutex.h>

class PktFileListReply;

class OfferHostInfo;
class OfferHostHistoryMgr;
class OfferHostCallbackInterface;

class OfferHostMgr : public OfferBaseMgr
{
    const int THUMB_DB_VERSION = 1;
public:
	OfferHostMgr( P2PEngine& engine, const char * dbName, const char * stateDbName );
	virtual ~OfferHostMgr() = default;

    void                        addOfferHostMgrClient( OfferHostCallbackInterface * client, bool enable );

    void                        fromGuiUserLoggedOn( void ) override;
    bool				        fromGuiOfferHostCreated( OfferHostInfo& thumbInfo );
    bool				        fromGuiOfferHostUpdated( OfferHostInfo& thumbInfo );

    virtual void				announceOfferAdded( OfferBaseInfo * assetInfo ) override;
    virtual void				announceOfferUpdated( OfferBaseInfo * assetInfo ) override;
    virtual void				announceOfferRemoved( OfferBaseInfo * assetInfo ) override;
    virtual void				announceOfferXferState( VxGUID& assetOfferId, EOfferSendState assetSendState, int param ) override;

protected:
    virtual OfferBaseInfo *     createOfferInfo( const char * fileName, uint64_t fileLen, uint16_t fileType ) override;
    virtual OfferBaseInfo *     createOfferInfo( OfferBaseInfo& assetInfo ) override;

    OfferHostInfoDb&            m_OfferHostInfoDb;
    std::vector<OfferBaseInfo*>&	m_OfferHostInfoList;
    VxMutex						m_OfferHostInfoMutex;
    bool                        m_OfferHostListInitialized{ false };

    std::vector<OfferHostCallbackInterface *> m_OfferHostClients;
    VxMutex						m_OfferHostClientMutex;
};

