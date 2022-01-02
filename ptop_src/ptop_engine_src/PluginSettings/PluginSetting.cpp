//============================================================================
// Copyright (C) 2019 Brett R. Jones
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

#include "PluginSetting.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>

#include <PktLib/PktBlobEntry.h>
#include <CoreLib/BinaryBlob.h>
#include <CoreLib/StringListBinary.h>
#include <CoreLib/VxDebug.h>

#include <string.h>

#define PLUGIN_SETTINGS_STRING_COUNT 7


//============================================================================
void PluginSettingHdr::setUpdateTimestampToNow( void )
{
    setLastUpdateTimestamp( GetTimeStampMs() );
}

//============================================================================
bool PluginSetting::addToBlob( PktBlobEntry& blob )
{
    uint8_t startMagicNum = 97;
    bool result = blob.setValue( startMagicNum );
    result &= blob.setValue( m_BlobStorageVersion );
    result &= blob.setValue( m_SecondaryPermissionLevel );
    result &= blob.setValue( m_PluginType );
    result &= blob.setValue( m_SecondaryPluginType );
    result &= blob.setValue( m_UpdateTimestamp );
    result &= blob.setValue( m_PluginThumb );
    result &= blob.setValue( m_SecondaryPluginThumb );
    result &= blob.setValue( m_SecondaryIdentGuid );

    result &= blob.setValue( m_AgeType );
    result &= blob.setValue( m_Gender );
    result &= blob.setValue( m_Language );
    result &= blob.setValue( m_ContentRating );
    result &= blob.setValue( m_ContentCatagory );
    result &= blob.setValue( m_ContentSubCatagory );
    result &= blob.setValue( m_MaxConnectionsPerUser );
    result &= blob.setValue( m_MaxStoreAndForwardPerUser );
    result &= blob.setValue( m_AnnounceToHost );
    result &= blob.setValue( m_ThumbnailIsCircular );
    result &= blob.setValue( m_ResBw1 );
    result &= blob.setValue( m_Reserve1Setting );
    result &= blob.setValue( m_Reserve2Setting );

    result &= blob.setValue( m_PluginTitle );
    result &= blob.setValue( m_PluginDesc );
    result &= blob.setValue( m_PluginUrl );
    result &= blob.setValue( m_GreetingMsg );
    result &= blob.setValue( m_RejectMsg );
    result &= blob.setValue( m_KeyWords );
    result &= blob.setValue( m_SecondaryUrl );
    result &= blob.setValue( m_Res1 );
    uint8_t stopMagicNum = 96;
    result &= blob.setValue( stopMagicNum );

    if( !result )
    {
        LogMsg( LOG_ERROR, "PluginSetting::addToBlob failed" );
    }

    return result;
}

//============================================================================
bool PluginSetting::extractFromBlob( PktBlobEntry& blob )
{
    uint8_t startMagicNum;
    bool result = blob.getValue( startMagicNum );
    if( !result || startMagicNum != 97 )
    {
        LogMsg( LOG_ERROR, "PluginSetting::extractFromBlob startMagicNum not valid" );
        return false;
    }

    uint16_t blobStorageVersion;
    if( !blob.getValue( blobStorageVersion ) || blobStorageVersion != m_BlobStorageVersion )
    {
        LogMsg( LOG_ERROR, "PluginSetting::extractFromBlob invalid storage version" );
        return false;
    }

    result &= blob.getValue( m_SecondaryPermissionLevel );
    result &= blob.getValue( m_PluginType );
    result &= blob.getValue( m_SecondaryPluginType );
    result &= blob.getValue( m_UpdateTimestamp );
    result &= blob.getValue( m_PluginThumb );
    result &= blob.getValue( m_SecondaryPluginThumb );
    result &= blob.getValue( m_SecondaryIdentGuid );

    result &= blob.getValue( m_AgeType );
    result &= blob.getValue( m_Gender );
    result &= blob.getValue( m_Language );
    result &= blob.getValue( m_ContentRating );
    result &= blob.getValue( m_ContentCatagory );
    result &= blob.getValue( m_ContentSubCatagory );
    result &= blob.getValue( m_MaxConnectionsPerUser );
    result &= blob.getValue( m_MaxStoreAndForwardPerUser );
    result &= blob.getValue( m_AnnounceToHost );
    result &= blob.getValue( m_ThumbnailIsCircular );
    result &= blob.getValue( m_ResBw1 );
    result &= blob.getValue( m_Reserve1Setting );
    result &= blob.getValue( m_Reserve2Setting );

    result &= blob.getValue( m_PluginTitle );
    result &= blob.getValue( m_PluginDesc );
    result &= blob.getValue( m_PluginUrl );
    result &= blob.getValue( m_GreetingMsg );
    result &= blob.getValue( m_RejectMsg );
    result &= blob.getValue( m_KeyWords );
    result &= blob.getValue( m_SecondaryUrl );
    result &= blob.getValue( m_Res1 );

    uint8_t stopMagicNum;
    result &= blob.getValue( stopMagicNum );
    if( !result || stopMagicNum != 96 )
    {
        LogMsg( LOG_ERROR, "PluginSetting::extractFromBlob stopMagicNum not valid" );
        return false;
    }

    if( !result )
    {
        LogMsg( LOG_ERROR, "PluginSetting::extractFromBlob failed" );
    }

    return result;
}

//============================================================================
bool PluginSetting::toBinary( BinaryBlob& binaryBlob, bool networkOrder )
{
    binaryBlob.createStorage( BLOB_PLUGIN_SETTING_MAX_STORAGE_LEN );
    binaryBlob.resetWrite();
    binaryBlob.setUseNetworkOrder( networkOrder );

    bool result = binaryBlob.setValue( m_BlobStorageVersion );
    result &= binaryBlob.setValue( m_SecondaryPermissionLevel );
    result &= binaryBlob.setValue( m_PluginType );
    result &= binaryBlob.setValue( m_SecondaryPluginType );
    result &= binaryBlob.setValue( m_UpdateTimestamp );
    result &= binaryBlob.setValue( m_PluginThumb );
    result &= binaryBlob.setValue( m_SecondaryPluginThumb );
    result &= binaryBlob.setValue( m_SecondaryIdentGuid );

    result &= binaryBlob.setValue( m_AgeType );
    result &= binaryBlob.setValue( m_Gender );
    result &= binaryBlob.setValue( m_Language );
    result &= binaryBlob.setValue( m_ContentRating );
    result &= binaryBlob.setValue( m_ContentCatagory );
    result &= binaryBlob.setValue( m_ContentSubCatagory );
    result &= binaryBlob.setValue( m_MaxConnectionsPerUser );
    result &= binaryBlob.setValue( m_MaxStoreAndForwardPerUser );
    result &= binaryBlob.setValue( m_AnnounceToHost );
    result &= binaryBlob.setValue( m_ThumbnailIsCircular );
    result &= binaryBlob.setValue( m_ResBw1 );
    result &= binaryBlob.setValue( m_Reserve1Setting );
    result &= binaryBlob.setValue( m_Reserve2Setting );

    result &= binaryBlob.setValue( m_PluginTitle );
    result &= binaryBlob.setValue( m_PluginDesc );
    result &= binaryBlob.setValue( m_PluginUrl );
    result &= binaryBlob.setValue( m_GreetingMsg );
    result &= binaryBlob.setValue( m_RejectMsg );
    result &= binaryBlob.setValue( m_KeyWords );
    result &= binaryBlob.setValue( m_SecondaryUrl );
    result &= binaryBlob.setValue( m_Res1 );

    if( !result )
    {
        LogMsg( LOG_ERROR, "PluginSetting::toBinary failed" );
    }

    return result;
}

//============================================================================
bool PluginSetting::fromBinary( BinaryBlob& binaryBlob, bool networkOrder )
{
    binaryBlob.resetRead();
    // not sure if this is needed binaryBlob.setUseNetworkOrder( networkOrder );
    uint16_t blobStorageVersion;
    if( !binaryBlob.getValue( blobStorageVersion ) || blobStorageVersion != m_BlobStorageVersion )
    {
        LogMsg( LOG_ERROR, "PluginSetting::fromBinary invalid storage version" );
        return false;
    }

    bool result = binaryBlob.getValue( m_SecondaryPermissionLevel );
    result &= binaryBlob.getValue( m_PluginType );
    result &= binaryBlob.getValue( m_SecondaryPluginType );
    result &= binaryBlob.getValue( m_UpdateTimestamp );
    result &= binaryBlob.getValue( m_PluginThumb );
    result &= binaryBlob.getValue( m_SecondaryPluginThumb );
    result &= binaryBlob.getValue( m_SecondaryIdentGuid );

    result &= binaryBlob.getValue( m_AgeType );
    result &= binaryBlob.getValue( m_Gender );
    result &= binaryBlob.getValue( m_Language );
    result &= binaryBlob.getValue( m_ContentRating );
    result &= binaryBlob.getValue( m_ContentCatagory );
    result &= binaryBlob.getValue( m_ContentSubCatagory );
    result &= binaryBlob.getValue( m_MaxConnectionsPerUser );
    result &= binaryBlob.getValue( m_MaxStoreAndForwardPerUser );
    result &= binaryBlob.getValue( m_AnnounceToHost );
    result &= binaryBlob.getValue( m_ThumbnailIsCircular );
    result &= binaryBlob.getValue( m_ResBw1 );
    result &= binaryBlob.getValue( m_Reserve1Setting );
    result &= binaryBlob.getValue( m_Reserve2Setting );

    result &= binaryBlob.getValue( m_PluginTitle );
    result &= binaryBlob.getValue( m_PluginDesc );
    result &= binaryBlob.getValue( m_PluginUrl );
    result &= binaryBlob.getValue( m_GreetingMsg );
    result &= binaryBlob.getValue( m_RejectMsg );
    result &= binaryBlob.getValue( m_KeyWords );
    result &= binaryBlob.getValue( m_SecondaryUrl );
    result &= binaryBlob.getValue( m_Res1 );

    if( !result )
    {
        LogMsg( LOG_ERROR, "PluginSetting::fromBinary failed" );
    }

    return result;
}

//============================================================================
bool PluginSetting::fillSearchStrings( std::vector<std::string>& searchStrings )
{
    searchStrings.push_back( m_PluginTitle );
    searchStrings.push_back( m_PluginDesc );
    searchStrings.push_back( m_GreetingMsg );
    searchStrings.push_back( m_RejectMsg );
    searchStrings.push_back( m_KeyWords );
    searchStrings.push_back( m_SecondaryUrl );
    searchStrings.push_back( m_Res1 );
    return true;
}

//============================================================================
//! set to default values for case use has not set anything but has enabled the plugin
bool PluginSetting::setDefaultValues( P2PEngine& engine, EPluginType pluginType )
{
    setUpdateTimestampToNow();
    setPluginType( pluginType );
    PktAnnounce pktAnn;
    engine.copyMyPktAnnounce(pktAnn);
    std::string onlineName = pktAnn.getOnlineName();
    std::string onlineDesc = pktAnn.getOnlineDescription();
    std::string myUrl = pktAnn.getMyOnlineUrl();
    m_PluginUrl = myUrl;
    m_PluginTitle = DescribePluginType( pluginType );
    m_PluginTitle += " - ";
    m_PluginTitle += onlineName;
    m_PluginDesc = DescribePluginType( pluginType );
    m_PluginDesc += " - ";
    m_PluginDesc += onlineDesc;
    return true;
}

//============================================================================
//! set to default values for case use has not set anything but has enabled the plugin
bool PluginSetting::getHostTitleAndDescription( std::string& hostTitle, std::string& hostDesc, int64_t& lastModifiedTime )
{
    hostTitle = getTitle();
    hostDesc = getDescription();
    bool result = !hostTitle.empty() && !hostDesc.empty();
    if( !result )
    {
        LogMsg( LOG_ERROR, "getHostTitleAndDescription %s INVALID title or description ", DescribePluginType( getPluginType() ) );
    }

    lastModifiedTime = getLastUpdateTimestamp();
    if( lastModifiedTime <= 0 )
    {
        result = false;
        LogMsg( LOG_ERROR, "getHostTitleAndDescription %s INVALID modified time ", DescribePluginType( getPluginType() ) );
    }

    return result;
}