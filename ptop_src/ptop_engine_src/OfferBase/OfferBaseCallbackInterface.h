#pragma once
//============================================================================
// Copyright (C) 2021 Brett R. Jones
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

#include <GuiInterface/IDefs.h>
#include <CoreLib/VxGUID.h>
#include <CoreLib/VxSha1Hash.h>

class OfferBaseInfo;

class OfferBaseCallbackInterface
{
public:
	virtual void				callbackFileWasShredded( std::string& fileName ){};

	virtual void				callbackHashIdGenerated( std::string& fileName, VxSha1Hash& hashId ){};
	virtual void				callbackOfferSendState( VxGUID& assetOfferId, EOfferSendState assetSendState, int param ){};
    virtual void				callbackOfferAction( VxGUID& assetOfferId, EOfferAction offerAction, int param ){};

	virtual void				callbackOfferFileTypesChanged( uint16_t fileTypes ){};
	virtual void				callbackOfferPktFileListUpdated( void ){};

    virtual void				callbackOfferAdded( OfferBaseInfo * assetInfo ){};
    virtual void				callbackOfferUpdated( OfferBaseInfo * assetInfo ){};
    virtual void				callbackOfferRemoved( OfferBaseInfo * assetInfo ){};
	virtual void				callbackOfferHistory( void * userData, OfferBaseInfo * assetInfo ){};
};

