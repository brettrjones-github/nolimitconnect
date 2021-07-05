#pragma once
//============================================================================
// Copyright (C) 2009 Brett R. Jones 
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

#include <GuiInterface/IFromGui.h>
#include <CoreLib/MediaCallbackInterface.h>
#include <CoreLib/VxMutex.h>

#include <PktLib/PktAnnounce.h>

#include <PktLib/PktAnnounce.h>

#include <ptop_src/ptop_engine_src/MediaProcessor/MediaProcessor.h>

class P2PEngine : public IFromGui, public MediaCallbackInterface
{
public:
    P2PEngine();
    virtual ~P2PEngine();
    P2PEngine( const P2PEngine& ) = delete; // don't allow copy constructor


    void						startupEngine( void );
    void						shutdownEngine( void );

    bool						isAppPaused( void )								{ return m_AppIsPaused; }

    virtual void				fromGuiVideoData( uint32_t u32FourCc, uint8_t * pu8VidDataIn, int iWidth, int iHeight, uint32_t u32VidDataLen, int iRotation ) override;
    //========================================================================
    // media processor callbacks
    //========================================================================
    virtual void				callbackVideoJpgBig( void * userData, VxGUID& vidFeedId, uint8_t * jpgData, uint32_t jpgDataLen ) override;
    virtual void				callbackVideoJpgSmall( void * userData, VxGUID& vidFeedId, uint8_t * jpgData, uint32_t jpgDataLen, int motion0to100000 ) override;

    void                        fromGuiWantMediaInput( EMediaInputType mediaType, MediaCallbackInterface * callback, void * userData, bool wantInput );
    void                        fromGuiWantMediaInput( VxGUID& onlineId, EMediaInputType mediaType, bool wantInput );

    PktAnnounce&				getMyPktAnnounce( void )						{ return m_PktAnn; }
    VxGUID&						getMyOnlineId( void )							{ return m_PktAnn.getMyOnlineId(); }

protected:
    bool						m_AppStartupCalled{ false };
    bool						m_AppIsPaused{ false };
    bool						m_IsUserSpecificDirSet{ false };
    bool                        m_EngineInitialized{ false };

    PktAnnounce					m_PktAnn;
    VxMutex						m_AnnouncePktMutex;
    MediaProcessor				m_MediaProcessor;
};

P2PEngine& GetP2PEngineInstance();