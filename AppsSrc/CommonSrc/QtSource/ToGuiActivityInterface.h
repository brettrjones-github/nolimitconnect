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

#include "config_apps.h"
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
	virtual void				doToGuiRxedPluginOffer( void * callbackData, GuiOfferSession * offer ){}; 
	virtual void				doToGuiRxedOfferReply( void * callbackData, GuiOfferSession * offer ){}; 
	virtual void				toGuiPluginSessionEnded(void * callbackData, GuiOfferSession * offer ){}; 

    virtual void				toGuiIndentListUpdate( void* callbackData, EUserViewType listType, VxGUID& onlineId, uint64_t timestamp ) {};
    virtual void				toGuiIndentListRemove( void* callbackData, EUserViewType listType, VxGUID& onlineId ) {};

    virtual void				toGuiContactAdded( void * callbackData, VxNetIdent * netIdent ){}; 
    virtual void				toGuiContactRemoved( void * callbackData, VxGUID& onlineId ) {}; 

	virtual void				toGuiContactOnline( void * callbackData, VxNetIdent * netIdent, EHostType hostType, bool newContact ){}; 
	virtual void				toGuiContactOffline( void * callbackData, GuiUser * netIdent ){}; 
    virtual void				toGuiContactNameChange( void * callbackData, GuiUser * netIdent ){}; 
    virtual void				toGuiContactDescChange( void * callbackData, GuiUser * netIdent ){}; 
    virtual void				toGuiContactMyFriendshipChange( void * callbackData, GuiUser * netIdent ){}; 
    virtual void				toGuiContactHisFriendshipChange( void * callbackData, GuiUser * netIdent ){}; 
    virtual void				toGuiPluginPermissionChange( void * callbackData, GuiUser * netIdent ){}; 
    virtual void				toGuiContactSearchFlagsChange( void * callbackData, GuiUser * netIdent ){}; 
    virtual void				toGuiContactLastSessionTimeChange( void * callbackData, GuiUser * netIdent ){}; 
    virtual void				toGuiUpdateMyIdent( void * callbackData, GuiUser * netIdent ){}; 


    virtual void				toGuiClientPlayVideoFrame( void * userData, VxGUID& onlineId, uint8_t * pu8Jpg, uint32_t u32JpgDataLen, int motion0To100000 ) {};
    virtual int				    toGuiClientPlayVideoFrame( void * userData, VxGUID& onlineId, uint8_t * picBuf, uint32_t picBufLen, int picWidth, int picHeight ) { return 0;};
	virtual void				toGuiInstMsg( void * callbackData, GuiUser * friendIdent, EPluginType ePluginType, QString instMsg ){}; 

	virtual void				toGuiScanResultSuccess( void * callbackData, EScanType eScanType, GuiUser * netIdent ){};
	virtual void				toGuiSearchResultError( void * callbackData, EScanType eScanType, GuiUser * netIdent, int errCode ){};
	virtual void				toGuiSearchResultProfilePic( void * callbackData, GuiUser * netIdent, uint8_t * pu8JpgData, uint32_t u32JpgDataLen ){};
	virtual void				toGuiSearchResultFileSearch(	void *			callbackData, 
																GuiUser *	    netIdent, 		
																VxGUID&			lclSessionId, 
																uint8_t			u8FileType, 
																uint64_t		u64FileLen, 
																const char *	pFileName,
                                                                VxGUID          assetId ){};
	virtual void				toGuiClientScanSearchComplete( void * callbackData, EScanType eScanType ){};

	virtual void				toGuiClientAssetAction( void * callbackData, EAssetAction assetAction, VxGUID& assetId, int pos0to100000 ){};
    virtual void				toGuiAssetSessionHistory( void * userData, AssetBaseInfo * assetInfo ){};
    virtual void				toGuiAssetAdded( void * userData, AssetBaseInfo * assetInfo ){};

	virtual void				toGuiMultiSessionAction( void * callbackData, EMSessionAction mSessionAction, VxGUID& onlineId, int pos0to100000 ){};
	virtual void				toGuiSetGameValueVar( void * callbackData, EPluginType ePluginType, VxGUID& onlineId, int32_t varId, int32_t varValue ){};
	virtual void				toGuiSetGameActionVar( void * callbackData, EPluginType ePluginType, VxGUID& onlineId, int32_t actionId, int32_t varValue ){};

    virtual void				toGuiClientBlobAction( void * callbackData, EAssetAction assetAction, VxGUID& assetId, int pos0to100000 ){};
    virtual void				toGuiBlobAdded( void * userData, BlobInfo * assetInfo ){};
    virtual void				toGuiBlobSessionHistory( void * userData, BlobInfo * assetInfo ){};

    virtual void				toGuiThumbAdded( void * userData, ThumbInfo * assetInfo ){};
    virtual void				toGuiThumbUpdated( void * userData, ThumbInfo * assetInfo ){};
    virtual void				toGuiThumbRemoved( void * userData, VxGUID& thumbId ){};
};

