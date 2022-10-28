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

#include "OfferMgr.h"
#include "OfferBaseInfo.h"

#include "OfferCallbackInterface.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>
#include <GuiInterface/IToGui.h>

#include <PktLib/PktAnnounce.h>
#include <PktLib/PktsFileList.h>

#include <CoreLib/VxFileUtil.h>
#include <CoreLib/VxFileIsTypeFunctions.h>
#include <CoreLib/VxGlobals.h>
#include <CoreLib/VxTime.h>

#include <time.h>

//============================================================================
OfferMgr::OfferMgr( P2PEngine& engine, const char* dbName, const char* stateDbName )
: OfferBaseMgr( engine, dbName, stateDbName, eOfferMgrClient )
{
}

//============================================================================
void OfferMgr::fromGuiUserLoggedOn( void )
{
    OfferBaseMgr::fromGuiUserLoggedOn();
}

//============================================================================
void OfferMgr::announceOfferAdded( OfferBaseInfo* offerInfo )
{
    OfferBaseMgr::announceOfferAdded( offerInfo );

    if( offerInfo )
    {
        LogMsg( LOG_INFO, "OfferHostMgr::announceOfferHostAdded start" );

        lockClientList();
        for( auto client : m_OfferClients )
        {
            client->callbackOfferAdded( offerInfo );
        }

        unlockClientList();
        LogMsg( LOG_INFO, "OfferHostMgr::announceOfferHostAdded done" );
    }
    else
    {
        LogMsg( LOG_ERROR, "OfferHostMgr::announceOfferAdded dynamic_cast failed" );
    }
}

//============================================================================
void OfferMgr::announceOfferUpdated( OfferBaseInfo* offerInfo )
{
    OfferBaseMgr::announceOfferUpdated( offerInfo );

    if( offerInfo )
    {
        lockClientList();
        for( auto client : m_OfferClients )
        {
            client->callbackOfferUpdated( offerInfo );
        }

        unlockClientList();
    }
    else
    {
        LogMsg( LOG_ERROR, "OfferHostMgr::announceOfferRemoved dynamic_cast failed" );
    }
}

//============================================================================
void OfferMgr::announceOfferRemoved( OfferBaseInfo* offerInfo )
{
    OfferBaseMgr::announceOfferRemoved( offerInfo );

    if( offerInfo )
    {
        lockClientList();
        for( auto client : m_OfferClients )
        {
            client->callbackOfferRemoved( offerInfo );
        }

        unlockClientList();
    }
    else
    {
        LogMsg( LOG_ERROR, "OfferHostMgr::announceOfferRemoved dynamic_cast failed" );
    }
}

//============================================================================
void OfferMgr::announceOfferXferState( VxGUID& assetOfferId, EOfferSendState assetSendState, int param )
{
    OfferBaseMgr::announceOfferXferState( assetOfferId, assetSendState, param );

    LogMsg( LOG_INFO, "OfferHostMgr::announceOfferXferState state %d start\n", assetSendState );
    lockClientList();
    for( auto client : m_OfferClients )
    {
        client->callbackOfferSendState( assetOfferId, assetSendState, param );
    }

    unlockClientList();
    LogMsg( LOG_INFO, "OfferHostMgr::announceOfferXferState state %d done\n", assetSendState );
}

//============================================================================
bool OfferMgr::fromGuiOfferCreated( OfferBaseInfo& offerInfo )
{
    return addOffer( offerInfo );
}

//============================================================================
bool OfferMgr::fromGuiOfferUpdated( OfferBaseInfo& offerInfo )
{
    return updateOffer( offerInfo );
}