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
// http://www.nolimitconnect.com
//============================================================================

#include <GuiInterface/IDefs.h>
#include <NetLib/VxXferDefs.h>

#include <string>

class VxGUID;

class WebPageCallbackInterface
{
public:
    virtual void				callbackWebDownloadStarted( EWebPageType webPageType, VxGUID& onlineId, std::string& fileName, int fileNum ) {};
    virtual void				callbackWebDownloadProgress( EWebPageType webPageType, VxGUID& onlineId, int fileNum, int progress ) {};
    virtual void				callbackWebDownloadComplete( EWebPageType webPageType, VxGUID& onlineId, std::string& fileName ) {};
    virtual void				callbackWebDownloadFailed( EWebPageType webPageType, VxGUID& onlineId, std::string& fileName, enum EXferError xferErr ) {};
};

