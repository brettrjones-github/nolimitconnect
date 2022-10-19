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
// http://www.nolimitconnect.org
//============================================================================

#include <config_appcorelibs.h>

#include <CoreLib/DbBase.h>
#include <CoreLib/VxGUID.h>
#include <CoreLib/AssetDefs.h>

class AssetBaseInfo;
class AssetBaseMgr;
class VxGUID;
class VxSha1Hash;

class AssetBaseInfoDb : public DbBase
{
public:
	AssetBaseInfoDb( AssetBaseMgr& mgr, const char*dbName );
	virtual ~AssetBaseInfoDb() = default;

	void						lockAssetInfoDb( void )					{ m_AssetBaseInfoDbMutex.lock(); }
	void						unlockAssetInfoDb( void )				{ m_AssetBaseInfoDbMutex.unlock(); }

    bool						addAsset(	VxGUID&			assetId, 
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
											EAssetSendState sendState = eAssetSendStateNone);

	bool 						addAsset( AssetBaseInfo* assetInfo );

	void						removeAsset( const char* assetName );
	void						removeAsset( VxGUID& assetId );
	void						removeAsset( AssetBaseInfo* assetInfo );

	void						getAllAssets( std::vector<AssetBaseInfo*>& AssetBaseAssetList );
	void						purgeAllAssets( void ); 
	void						updateAssetSendState( VxGUID& assetId, EAssetSendState sendState );

protected:
    virtual AssetBaseInfo*     createAssetInfo( EAssetType assetType, const char* assetName, uint64_t assetLen ) = 0;
    virtual AssetBaseInfo*     createAssetInfo( AssetBaseInfo& assetInfo ) = 0;

	virtual RCODE				onCreateTables( int iDbVersion );
	virtual RCODE				onDeleteTables( int iOldVersion );
	void						insertAssetInTimeOrder( AssetBaseInfo*assetInfo, std::vector<AssetBaseInfo*>& assetList );

	AssetBaseMgr&				m_AssetMgr;
	VxMutex						m_AssetBaseInfoDbMutex;
};

