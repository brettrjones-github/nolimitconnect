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

#include "GuiThumb.h"
#include "GuiThumbList.h"

#include <ptop_src/ptop_engine_src/ThumbMgr/ThumbCallbackInterface.h>

#include <CoreLib/VxMutex.h>

#include <QObject>

class AppCommon;
class GuiUser;
class AppletBase;

class GuiThumbMgr : public QObject, public ThumbCallbackInterface
{
    Q_OBJECT
public:
    GuiThumbMgr() = delete;
    GuiThumbMgr( AppCommon& app );
    GuiThumbMgr( const GuiThumbMgr& rhs ) = delete;
	virtual ~GuiThumbMgr() = default;

    virtual void                onAppCommonCreated( void );
    virtual void                onMessengerReady( bool ready )              { }
    virtual bool                isMessengerReady( void );
    virtual void                onSystemReady( bool ready ) { }

    VxGUID                      getMyOnlineId( void )                       { return m_MyOnlineId; }  

    void                        generateEmoticonsIfNeeded( AppletBase* applet );
    bool                        getEmoticonImage( int emoticonNum, QSize imageSize, QPixmap& retImage );

    bool                        requestAvatarImage( GuiUser* user, EHostType requestedThumbType, QImage& retAvatarImage, bool requestFromUserIfValid = false );
    bool                        requestAvatarImage( GuiUser* user, EPluginType requestedThumbType, QImage& retAvatarImage, bool requestFromUserIfValid = false );

    virtual void				callbackThumbAdded( ThumbInfo * thumbInfo ) override; 
    virtual void				callbackThumbUpdated( ThumbInfo * thumbInfo ) override; 
    virtual void				callbackThumbRemoved( VxGUID& thumbId ) override; 

    void                        onThumbAdded( GuiThumb* thumb );
    void                        onThumbRemoved( VxGUID& onlineId );
    void                        onThumbUpdated( GuiThumb* thumb );

    void                        lockThumbMgr( void )             { m_ThumbListMutex.lock(); }
    void                        unlockThumbMgr( void )           { m_ThumbListMutex.unlock(); }

    GuiThumb*                   getThumb( VxGUID& thumbId );
    bool                        getAvatarImage( VxGUID& thumbId, QImage& image ); // get rounded image for avatar
    bool                        getThumbImage( VxGUID& thumbId, QImage& image );
    GuiThumbList&               getThumbList( void )             { return m_ThumbList; }

signals:
    void				        signalThumbAdded( GuiThumb* thumb ); 
    void                        signalThumbUpdated( GuiThumb* thumb );
    void				        signalThumbRemoved( VxGUID thumbId ); 

    void                        signalInternalThumbAdded( ThumbInfo thumbInfo );
    void                        signalInternalThumbUpdated( ThumbInfo thumbInfo );
    void                        signalInternalThumbRemoved( VxGUID thumbId );

private slots:
    void                        slotInternalThumbAdded( ThumbInfo thumbInfo );
    void                        slotInternalThumbUpdated( ThumbInfo thumbInfo );
    void                        slotInternalThumbRemoved( VxGUID onlineId );

protected:
    GuiThumb*                   updateThumb( ThumbInfo& thumbInfo  );
    void                        removeThumb( VxGUID& thumbId );
    GuiThumb*                   findThumb( VxGUID& thumbId );
    
    AppCommon&                  m_MyApp;
    VxMutex                     m_ThumbListMutex;
    GuiThumbList                m_ThumbList;
    GuiThumb*                   m_MyIdent{ nullptr };
    VxGUID                      m_MyOnlineId;
};
