//============================================================================
// Copyright (C) 2015 Brett R. Jones
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

#include "ThumbMgr.h"
#include "ThumbInfo.h"
#include "ThumbInfoDb.h"
#include "ThumbCallbackInterface.h"

#include <GoTvCore/GoTvP2P/P2PEngine/P2PEngine.h>
#include <GoTvInterface/IToGui.h>

#include <PktLib/PktAnnounce.h>
#include <PktLib/PktsFileList.h>

#include <CoreLib/VxFileUtil.h>
#include <CoreLib/VxFileIsTypeFunctions.h>
#include <CoreLib/VxGlobals.h>
#include <CoreLib/VxTime.h>

#include <time.h>

//============================================================================
ThumbMgr::ThumbMgr( P2PEngine& engine, const char * dbName )
: AssetBaseMgr( engine, dbName )
{
}

//============================================================================
void ThumbMgr::announceAssetAdded( AssetBaseInfo * assetInfo )
{
    AssetBaseMgr::announceAssetAdded( assetInfo );
    ThumbInfo * thumbInfo = dynamic_cast<ThumbInfo *>( assetInfo );
    if( thumbInfo )
    {
	    LogMsg( LOG_INFO, "ThumbMgr::announceThumbAdded start\n" );
	
	    lockClientList();
	    std::vector<ThumbCallbackInterface *>::iterator iter;
	    for( iter = m_ThumbClients.begin();	iter != m_ThumbClients.end(); ++iter )
	    {
		    ThumbCallbackInterface * client = *iter;
		    client->callbackThumbAdded( thumbInfo );
	    }

	    unlockClientList();
	    LogMsg( LOG_INFO, "ThumbMgr::announceThumbAdded done\n" );
    }
    else
    {
        LogMsg( LOG_ERROR, "ThumbMgr::announceAssetAdded dynamic_cast failed" );
    }
}

//============================================================================
void ThumbMgr::announceAssetRemoved( AssetBaseInfo * assetInfo )
{
    AssetBaseMgr::announceAssetRemoved( assetInfo );
    ThumbInfo * thumbInfo = dynamic_cast<ThumbInfo *>( assetInfo );
    if( thumbInfo )
    {
	    lockClientList();
	    std::vector<ThumbCallbackInterface *>::iterator iter;
	    for( iter = m_ThumbClients.begin();	iter != m_ThumbClients.end(); ++iter )
	    {
		    ThumbCallbackInterface * client = *iter;
		    client->callbackThumbRemoved( thumbInfo );
	    }

	    unlockClientList();
    }
    else
    {
        LogMsg( LOG_ERROR, "ThumbMgr::announceAssetRemoved dynamic_cast failed" );
    }
}

//============================================================================
void ThumbMgr::announceAssetXferState( VxGUID& assetUniqueId, EAssetSendState assetSendState, int param )
{
    AssetBaseMgr::announceAssetXferState( assetUniqueId, assetSendState, param );

	LogMsg( LOG_INFO, "ThumbMgr::announceAssetXferState state %d start\n", assetSendState );
	lockClientList();
	std::vector<ThumbCallbackInterface *>::iterator iter;
	for( iter = m_ThumbClients.begin();	iter != m_ThumbClients.end(); ++iter )
	{
		ThumbCallbackInterface * client = *iter;
		client->callbackAssetSendState( assetUniqueId, assetSendState, param );
	}

	unlockClientList();
	LogMsg( LOG_INFO, "ThumbMgr::announceAssetXferState state %d done\n", assetSendState );
}
