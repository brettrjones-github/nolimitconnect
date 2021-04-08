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

#include "ToGuiThumbUpdateInterface.h"
#include "GuiThumb.h"

#include <CoreLib/VxMutex.h>

#include <QObject>

class AppCommon;

class GuiThumbMgr : public QObject, public ToGuiThumbUpdateInterface
{
    Q_OBJECT
public:
    GuiThumbMgr() = delete;
    GuiThumbMgr( AppCommon& app );
    GuiThumbMgr( const GuiThumbMgr& rhs ) = delete;
	virtual ~GuiThumbMgr() = default;
    void                        onAppCommonCreated( void );

    void                        setMessengerReady( bool ready )             { m_MessengerReady = ready; }
    bool                        isMessengerReady( void )                    { return m_MessengerReady; }
    VxGUID                      getMyOnlineId( void )                       { return m_MyOnlineId; }  

    virtual void				toGuiThumbAdded( ThumbInfo * thumb ) override; 
    virtual void				toGuiThumbUpdated( ThumbInfo * thumb ) override; 
    virtual void				toGuiThumbRemoved( VxGUID& thumbId ) override; 

    bool                        isThumbInSession( VxGUID& onlineId );
    void                        setThumbOffline( VxGUID& onlineId );

    void                        onThumbAdded( GuiThumb* thumb );
    void                        onThumbRemoved( VxGUID& onlineId );
    void                        onThumbUpdated( GuiThumb* thumb );

    void                        lockThumbMgr( void )             { m_ThumbListMutex.lock(); }
    void                        unlockThumbMgr( void )           { m_ThumbListMutex.unlock(); }
    GuiThumb*                   getThumb( VxGUID& onlineId );
    std::map<VxGUID, GuiThumb*>& getThumbList( void )             { return m_ThumbList; }

signals:
    void				        signalThumbAdded( GuiThumb* thumb ); 
    void                        signalThumbUpdated( GuiThumb* thumb );
    void				        signalThumbRemoved( VxGUID thumbId ); 

    void                        signalInternalThumbAdded( ThumbInfo * thumbInfo );
    void                        signalInternalThumbUpdated( ThumbInfo* thumbInfo );
    void                        signalInternalThumbRemoved( VxGUID thumbId );

private slots:
    void                        slotInternalThumbAdded( ThumbInfo * thumbInfo );
    void                        slotInternalThumbUpdated( ThumbInfo* thumbInfo );
    void                        slotInternalThumbRemoved( VxGUID onlineId );

protected:
    GuiThumb*                   updateThumb( ThumbInfo * thumbInfo  );
    void                        removeThumb( VxGUID& thumbId );
    GuiThumb*                   findThumb( VxGUID& thumbId );
    
    AppCommon&                  m_MyApp;
    VxMutex                     m_ThumbListMutex;
    // map of online id to GuiThumb
    std::map<VxGUID, GuiThumb*> m_ThumbList;
    bool                        m_MessengerReady{ false };
    GuiThumb*                   m_MyIdent{ nullptr };
    VxGUID                      m_MyOnlineId;
};
