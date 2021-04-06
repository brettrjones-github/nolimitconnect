#pragma once
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

#include <config_gotvcore.h>

#include <GoTvCore/GoTvP2P/AssetBase/AssetBaseMgr.h>

#include <CoreLib/VxThread.h>
#include <CoreLib/VxSemaphore.h>
#include <CoreLib/VxMutex.h>

class PktFileListReply;

class ThumbInfo;
class ThumbInfoDb;
class ThumbHistoryMgr;
class ThumbCallbackInterface;

class ThumbMgr : public AssetBaseMgr
{
public:
	ThumbMgr( P2PEngine& engine, const char * dbName );
	virtual ~ThumbMgr() = default;

    virtual void				announceAssetAdded( AssetBaseInfo * assetInfo ) override;
    virtual void				announceAssetRemoved( AssetBaseInfo * assetInfo ) override;
    virtual void				announceAssetXferState( VxGUID& assetUniqueId, EAssetSendState assetSendState, int param ) override;

protected:
    std::vector<AssetBaseInfo*>	m_ThumbInfoList;
    VxMutex						m_ThumbInfoMutex;

    std::vector<ThumbCallbackInterface *> m_ThumbClients;
    VxMutex						m_ThumbClientMutex;
};

