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

#include <GuiInterface/IDefs.h>
#include <NetLib/VxXferDefs.h>
#include <CoreLib/VxMutex.h>
#include <map>

class VxGUID;
class WebPageCallbackInterface;
class P2PEngine;

class WebPageMgr 
{
public:
	WebPageMgr( P2PEngine& engine );
	virtual ~WebPageMgr() = default;

    void                        fromGuiUserLoggedOn( void );

    void                        wantWebPageMgrCallbacks( WebPageCallbackInterface * client, bool enable );


    virtual void				announceWebDownloadStarted( EWebPageType webPageType, VxGUID& onlineId, std::string& fileName, int fileNum );
    virtual void				announceWebDownloadProgress( EWebPageType webPageType, VxGUID& onlineId, int fileNum, int progress );
    virtual void				announceWebDownloadComplete( EWebPageType webPageType, VxGUID& onlineId, std::string& fileName );
    virtual void				announceWebDownloadFailed( EWebPageType webPageType, VxGUID& onlineId, std::string& fileName, enum EXferError xferErr );


protected:
    void						lockClientList( void )						{ m_WebPageClientMutex.lock(); }
    void						unlockClientList( void )					{ m_WebPageClientMutex.unlock(); }

    P2PEngine&					m_Engine;
    VxMutex						m_ResourceMutex;
    bool						m_Initialized{ false };

    std::vector<WebPageCallbackInterface *> m_WebPageClients;
    VxMutex						m_WebPageClientMutex;
};

