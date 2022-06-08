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

#include <QWidget> // must be declared first or linux Qt 6.2.4 will error in qmetatype.h 2167:23: array subscript value ‘53’ is outside the bounds

#include <CoreLib/VxDefs.h>
#include <CoreLib/AssetDefs.h>
#include <CoreLib/VxGUID.h>
#include <PktLib/VxCommon.h>

#include <GuiInterface/IDefs.h>

#include <QString>

class VxGUID;
class GuiUser;
class GuiOfferSession;
class AssetBaseInfo;
class BlobInfo;
class ThumbInfo;

class ToGuiActivityInterface
{
public:
	virtual void				toToGuiRxedPluginOffer( GuiOfferSession * offer ){}; 
	virtual void				toToGuiRxedOfferReply( GuiOfferSession * offer ){}; 

    virtual void				toGuiPluginMsg( EPluginType pluginType, VxGUID& onlineId, EPluginMsgType msgType, QString& paramValue ){};

	virtual void				toGuiPluginSessionEnded( GuiOfferSession * offer ){}; 

    virtual void				toGuiIndentListUpdate( EUserViewType listType, VxGUID& onlineId, uint64_t timestamp ) {};
    virtual void				toGuiIndentListRemove( EUserViewType listType, VxGUID& onlineId ) {};

    virtual void				toGuiContactAdded( VxNetIdent * netIdent ){}; 
    virtual void				toGuiContactRemoved( VxGUID& onlineId ) {}; 

	virtual void				toGuiContactOnline( VxNetIdent * netIdent ){}; 
	virtual void				toGuiContactOffline( GuiUser * netIdent ){}; 
    virtual void				toGuiContactNameChange( GuiUser * netIdent ){}; 
    virtual void				toGuiContactDescChange( GuiUser * netIdent ){}; 
    virtual void				toGuiContactMyFriendshipChange( GuiUser * netIdent ){}; 
    virtual void				toGuiContactHisFriendshipChange( GuiUser * netIdent ){}; 
    virtual void				toGuiPluginPermissionChange( GuiUser * netIdent ){}; 
    virtual void				toGuiContactSearchFlagsChange( GuiUser * netIdent ){}; 
    virtual void				toGuiContactLastSessionTimeChange( GuiUser * netIdent ){}; 
    virtual void				toGuiUpdateMyIdent(GuiUser * netIdent ){}; 


    virtual void				toGuiClientPlayVideoFrame( VxGUID& onlineId, uint8_t * pu8Jpg, uint32_t u32JpgDataLen, int motion0To100000 ) {};
    virtual int				    toGuiClientPlayVideoFrame( VxGUID& onlineId, uint8_t * picBuf, uint32_t picBufLen, int picWidth, int picHeight ) { return 0;};
	virtual void				toGuiInstMsg( GuiUser * friendIdent, EPluginType ePluginType, QString instMsg ){}; 

	virtual void				toGuiScanResultSuccess( EScanType eScanType, GuiUser * netIdent ){};
	virtual void				toGuiSearchResultError( EScanType eScanType, GuiUser * netIdent, int errCode ){};
	virtual void				toGuiSearchResultProfilePic( GuiUser * netIdent, uint8_t * pu8JpgData, uint32_t u32JpgDataLen ){};
	virtual void				toGuiSearchResultFileSearch(	GuiUser *	    netIdent, 		
																VxGUID&			lclSessionId, 
																uint8_t			u8FileType, 
																uint64_t		u64FileLen, 
																const char *	pFileName,
                                                                VxGUID          assetId ){};
	virtual void				toGuiClientScanSearchComplete( EScanType eScanType ){};

    virtual void				toGuiAssetAdded( AssetBaseInfo& assetInfo ) {};
    virtual void				toGuiAssetUpdated( AssetBaseInfo& assetInfo ) {};
    virtual void				toGuiAssetRemoved( AssetBaseInfo& assetInfo ) {};

    virtual void				toGuiAssetXferState( VxGUID& assetUniqueId, EAssetSendState assetSendState, int param ) {};

    virtual void				toGuiAssetSessionHistory( AssetBaseInfo& assetInfo ) {};
	virtual void				toGuiClientAssetAction( EAssetAction assetAction, VxGUID& assetId, int pos0to100000 ){};
	virtual void				toGuiMultiSessionAction( EMSessionAction mSessionAction, VxGUID& onlineId, int pos0to100000 ){};

	virtual void				toGuiSetGameValueVar( EPluginType ePluginType, VxGUID& onlineId, int32_t varId, int32_t varValue ){};
	virtual void				toGuiSetGameActionVar( EPluginType ePluginType, VxGUID& onlineId, int32_t actionId, int32_t varValue ){};

    virtual void				toGuiClientBlobAction( EAssetAction assetAction, VxGUID& assetId, int pos0to100000 ){};
    virtual void				toGuiBlobAdded( BlobInfo& blobInfo ){};
    virtual void				toGuiBlobSessionHistory( BlobInfo& blobInfo ){};

    virtual void				toGuiThumbAdded( ThumbInfo * assetInfo ){};
    virtual void				toGuiThumbUpdated( ThumbInfo * assetInfo ){};
    virtual void				toGuiThumbRemoved( VxGUID& thumbId ){};
};

