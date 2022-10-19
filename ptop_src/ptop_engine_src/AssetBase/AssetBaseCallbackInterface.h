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

#include <CoreLib/AssetDefs.h>
#include <CoreLib/VxGUID.h>
#include <CoreLib/VxSha1Hash.h>

class AssetBaseInfo;

class AssetBaseCallbackInterface
{
public:
	virtual void				callbackFileWasShredded( std::string& fileName ){};

	virtual void				callbackHashIdGenerated( std::string& fileName, VxSha1Hash& hashId ){};
	virtual void				callbackAssetSendState( VxGUID& assetUniqueId, EAssetSendState assetSendState, int param ){};

	virtual void				callbackAssetFileTypesChanged( uint16_t fileTypes ){};
	virtual void				callbackAssetPktFileListUpdated( void ){};

    virtual void				callbackAssetAdded( AssetBaseInfo* assetInfo ){};
    virtual void				callbackAssetUpdated( AssetBaseInfo* assetInfo ){};
    virtual void				callbackAssetRemoved( AssetBaseInfo* assetInfo ){};
    virtual void				callbackAssetHistory( void * userData, AssetBaseInfo* assetInfo ){};
};

