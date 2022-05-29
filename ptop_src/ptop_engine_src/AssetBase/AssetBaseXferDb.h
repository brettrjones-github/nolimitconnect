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

class AssetBaseInfo;

class AssetBaseXferDb : public DbBase
{
public:
	AssetBaseXferDb( const char * stateDbName );
	virtual ~AssetBaseXferDb() = default;

	void						lockAssetBaseXferDb( void )			{ m_AssetBaseXferDbMutex.lock(); }
	void						unlockAssetBaseXferDb( void )			{ m_AssetBaseXferDbMutex.unlock(); }

	virtual RCODE				onCreateTables( int iDbVersion );
	virtual RCODE				onDeleteTables( int iOldVersion );

	void 						addAsset( VxGUID& assetUniqueId );
	void						removeAsset( VxGUID& assetUniqueId );

	void						getAllAssets( std::vector<VxGUID>& assetList );
	void						purgeAllAssetBaseXfer( void ); 

protected:
	VxMutex						m_AssetBaseXferDbMutex;
};

