//============================================================================
// Copyright (C) 2013 Brett R. Jones
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

#include "FileItemInfo.h"

#include "AppCommon.h"
#include "GuiHelpers.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>
#include <ptop_src/ptop_engine_src/AssetMgr/AssetMgr.h>
#include <ptop_src/ptop_engine_src/AssetMgr/AssetInfo.h>

#include <PktLib/VxSearchDefs.h>
#include <PktLib/VxCommon.h>
#include <CoreLib/VxParse.h>
#include <CoreLib/VxFileIsTypeFunctions.h>
#include <CoreLib/VxFileUtil.h>

#include <string>

//============================================================================
FileItemInfo::FileItemInfo( FileInfo& fileInfo )
: m_FileInfo( fileInfo )
{
}

//============================================================================
QString FileItemInfo::describeFileType()
{
    return GuiParams::describeFileType( getFileType() );
}

//============================================================================
QString FileItemInfo::describeFileLength()
{
    return GuiParams::describeFileLength( getFileLength() );
}

//============================================================================
bool FileItemInfo::toggleIsShared( void )
{
    m_FileInfo.setIsSharedFile( !m_FileInfo.getIsSharedFile() );
    return m_FileInfo.getIsSharedFile();
}

//============================================================================
bool FileItemInfo::toggleIsInLibrary( void )
{
    m_FileInfo.setIsInLibrary( !m_FileInfo.getIsInLibrary() );
    return m_FileInfo.getIsInLibrary();
}

//============================================================================
void FileItemInfo::updateWidget( void )
{
	if( 0 != m_Widget )
	{
		m_Widget->update();
	}
}

//============================================================================
bool FileItemInfo::shouldOpenFile( void )
{
	return VxShouldOpenFile( getFileType() );
}

//============================================================================
bool FileItemInfo::toAsssetInfo( AppCommon& myApp, AssetBaseInfo& assetInfoOut, bool* retIsNewAsset )
{
    bool result{ false };
    if( retIsNewAsset )
    {
        *retIsNewAsset = false;
    }

    std::string assetFileName = getFullFileName().toUtf8().constData();
    VxGUID assetId = getAssetId();

    if( VXFILE_TYPE_DIRECTORY == getFileType() )
    {
        // convert to asset but do not add to asset manager and return false
    }
    else
    {
        // determine asset id for this file.. may be different than the one given       
        int64_t fileLen = VxFileUtil::fileExists( assetFileName.c_str() );
        if( fileLen )
        {
            AssetBaseInfo* assetInfo = myApp.getEngine().getAssetMgr().findAsset( assetFileName );
            if( assetInfo && assetInfo->getAssetUniqueId().isVxGUIDValid() )
            {
                assetId = assetInfo->getAssetUniqueId();
                setAssetId( assetId );

                if( fileLen != assetInfo->getAssetLength() )
                {
                    assetInfo->updateAssetLength( fileLen );
                }

                assetInfoOut = *assetInfo;
                result = true;
                myApp.getPlayerMgr().playMedia( *assetInfo );
            }
            else
            {
                if( !assetId.isVxGUIDValid() )
                {
                    assetId.initializeWithNewVxGUID();
                }

                AssetInfo newAsset( GuiParams::fileTypeToAssetType( getFileType() ), assetFileName.c_str(), fileLen, assetId );
                assetInfoOut = newAsset;
                if( retIsNewAsset )
                {
                    *retIsNewAsset = true;
                }

                result = true;
            }
        }
    }

    return result;
}