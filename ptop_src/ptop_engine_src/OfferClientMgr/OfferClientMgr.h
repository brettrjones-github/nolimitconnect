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

#include "OfferClientInfoDb.h"

#include <ptop_src/ptop_engine_src/OfferBase/OfferBaseMgr.h>

#include <CoreLib/VxThread.h>
#include <CoreLib/VxSemaphore.h>
#include <CoreLib/VxMutex.h>

class PktFileListReply;

class OfferClientInfo;
class OfferClientHistoryMgr;
class OfferClientCallbackInterface;

class OfferClientMgr : public OfferBaseMgr
{
    const int THUMB_DB_VERSION = 1;
public:
	OfferClientMgr( P2PEngine& engine, const char* dbName, const char* statDbName );
	virtual ~OfferClientMgr() = default;

    void                        addOfferClientMgrClient( OfferClientCallbackInterface * client, bool enable );

    void                        fromGuiUserLoggedOn( void ) override;
    bool				        fromGuiOfferClientCreated( OfferClientInfo& thumbInfo );
    bool				        fromGuiOfferClientUpdated( OfferClientInfo& thumbInfo );

    virtual void				announceOfferAdded( OfferBaseInfo * assetInfo ) override;
    virtual void				announceOfferUpdated( OfferBaseInfo * assetInfo ) override;
    virtual void				announceOfferRemoved( OfferBaseInfo * assetInfo ) override;
    virtual void				announceOfferXferState( VxGUID& assetOfferId, EOfferSendState assetSendState, int param ) override;

protected:
    virtual OfferBaseInfo *     createOfferInfo( const char* fileName, uint64_t fileLen, uint16_t fileType ) override;
    virtual OfferBaseInfo *     createOfferInfo( OfferBaseInfo& assetInfo ) override;

    OfferClientInfoDb&          m_OfferClientInfoDb;
    std::vector<OfferBaseInfo*>&	m_OfferClientInfoList;
    VxMutex						m_OfferClientInfoMutex;
    bool                        m_OfferClientListInitialized{ false };

    std::vector<OfferClientCallbackInterface *> m_OfferClientClients;
    VxMutex						m_OfferClientClientMutex;
};

