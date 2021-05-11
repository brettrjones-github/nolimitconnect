//============================================================================
// Copyright (C) 2021 Brett R. Jones
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

#include "AssetDefs.h"
#include "VxFileIsTypeFunctions.h"
#include "VxDebug.h"

//============================================================================
EAssetType VxFileNameToAssetType( std::string fileName )
{
    if( fileName.empty() )
    {
        LogMsg( LOG_ERROR, "VxFileNameToAssetType empty file name" );
        return eAssetTypeUnknown;

    }
    else if( !VxIsRecognizedFile( fileName ) )
    {
        LogMsg( LOG_WARN, "VxFileNameToAssetType unrecognized file" );
        return eAssetTypeUnknown;
    }
    else if( VxIsPhotoFile( fileName ) )
    {
        return eAssetTypePhoto;
    }
    else if( VxIsAudioFile( fileName ) )
    {
        return eAssetTypeAudio;
    }
    else if( VxIsVideoFile( fileName ) )
    {
        return eAssetTypeVideo;
    }
    else if( VxIsDocumentFile( fileName ) )
    {
        return eAssetTypeDocument;
    }
    else if( VxIsArcOrCDImageFile( fileName ) )
    {
        return eAssetTypeArchives;
    }
    else if( VxIsExecutableFile( fileName ) )
    {
        LogMsg( LOG_WARN, "VxFileNameToAssetType is exe type" );
        return eAssetTypeExe;
    }
    else if( VxIsThumbnailFile( fileName ) )
    {
        return eAssetTypeThumbnail;
    }
    else
    {
        LogMsg( LOG_WARN, "VxFileNameToAssetType unknown file type" );
        return eAssetTypeUnknown;
    }
}

//============================================================================
EAssetType VxFileTypeToAssetType( uint8_t fileType )
{
    if( !fileType )
    {
        LogMsg( LOG_WARN, "VxFileTypeToAssetType no file type is set" );
        return eAssetTypeUnknown;
    }

    if( eAssetTypePhoto & fileType )
    {
        return eAssetTypePhoto;
    }
    else if( eAssetTypeAudio & fileType )
    {
        return eAssetTypeAudio;
    }
    else if( eAssetTypeVideo & fileType )
    {
        return eAssetTypeVideo;
    }
    else if( eAssetTypeDocument & fileType )
    {
        return eAssetTypeDocument;
    }
    else if( eAssetTypeArchives & fileType )
    {
        return eAssetTypeArchives;
    }
    else if( eAssetTypeExe & fileType )
    {
        LogMsg( LOG_WARN, "VxFileTypeToAssetType is exe type" );
        return eAssetTypeExe;
    }
    else if( eAssetTypeOtherFiles & fileType )
    {
        LogMsg( LOG_WARN, "VxFileTypeToAssetType is other files type" );
        return eAssetTypeOtherFiles;
    }
    else if( eAssetTypeDirectory & fileType )
    {
        LogMsg( LOG_WARN, "VxFileTypeToAssetType is directory type" );
        return eAssetTypeDirectory;
    }
    else
    {
        return eAssetTypeUnknown;
    }
}

//============================================================================
const char * DescribeAssetType( EAssetType assetType )
{
    if( assetType < 0 || eAssetTypeCamRecord < assetType )
    {
        return "DescribeAssetType BAD PARAM";
    }

    if( !assetType )
    {
        LogMsg( LOG_WARN, "VxFileTypeToAssetType no assetType is set" );
        return "Asset Type Unknown";
    }

    switch( assetType )
    {
    case eAssetTypePhoto:               return "Asset Type Photo";
    case eAssetTypeAudio:               return "Asset Type Audio";
    case eAssetTypeVideo:               return "Asset Type Video";
    case eAssetTypeDocument:            return "Asset Type Document";
    case eAssetTypeArchives:            return "Asset Type Archive";
    case eAssetTypeExe:                 return "Asset Type Executable";
    case eAssetTypeOtherFiles:          return "Asset Type OtherFiles";
    case eAssetTypeDirectory:           return "Asset Type Directory";
    case eAssetTypeThumbnail:           return "Asset Type Thumbnail";
    case eAssetTypeChatText:            return "Asset Type Chat Text";
    case eAssetTypeChatFace:            return "Asset Type Chat Face";
    case eAssetTypeCamRecord:           return "Asset Type Cam Record";

    default:
        LogMsg( LOG_ERROR, "Asset Type More than one flag 0x%X", assetType );
        vx_assert( false );
        return "Asset Type Invalid";
    }
}

//============================================================================
bool VxIsValidAssetType( EAssetType assetType )
{
    switch( assetType )
    {
    case eAssetTypePhoto:
    case eAssetTypeAudio:
    case eAssetTypeVideo:
    case eAssetTypeDocument:
    case eAssetTypeArchives:
    case eAssetTypeExe:
    case eAssetTypeOtherFiles:
    case eAssetTypeDirectory:
    case eAssetTypeThumbnail:
    case eAssetTypeChatText:
    case eAssetTypeChatFace:
    case eAssetTypeCamRecord:
        return true;

    default:
        LogMsg( LOG_ERROR, "VxIsValidAssetType false 0x%X", assetType );
        vx_assert( false );
        return false;
    }
}
