#pragma once

#include "IGoTv.h"
#include "guilib/IWindowManagerCallback.h"
#include <CoreLib/VxGUID.h>

// Do not change the numbering, external scripts depend on them
enum {
    EXITCODE_QUIT = 0,
    EXITCODE_POWERDOWN = 64,
    EXITCODE_RESTARTAPP = 65,
    EXITCODE_REBOOT = 66,
};

class GoTvInstance : public IGoTv, public IWindowManagerCallback 
{
public:
    GoTvInstance( P2PEngine& engine );

    //=== IGoTv interface ===//
    virtual bool 				playGoTvMedia( AssetInfo * assetInfo ) = 0;
    virtual VxGUID&             getAssetUniqueId( void )        { return m_AssetUniqueId; }
    virtual int                 toGuiPlayVideoFrame( VxGUID& onlineId, uint8_t * picBuf, uint32_t picBufLen, int picWidth, int picHeight );
    //virtual int                 toGuiAudioFrame( uint8_t * audioBuf, uint32_t audioBufLen, int bytesPerSample, int sampleCount, int audioFrequency, int audioChannels );


    // called in on idle
    virtual bool 				runGoTv( void );
    virtual void 				destroyGoTv( void );

    // IWindowManagerCallback
    virtual void                FrameMove( bool processEvents, bool processGUI = true ) = 0;
    virtual void                Render() = 0;
    virtual void                Process() = 0;

    // Application
    virtual bool                checkCreateGoTv( void );
    virtual bool                Run( void );
    virtual bool                Cleanup() { return false; }
    virtual void 				Destroy();

    virtual void                SetRenderGUI( bool renderGUI ) {};

protected:
    virtual bool                createGoTv() = 0;


    bool                        m_IsCreated;
    bool                        m_IsInitialized;
    bool                        m_bStop;
    int                         m_ExitCode;
    bool                        m_AppFocused;
    bool 						m_renderGUI;
    VxGUID                      m_AssetUniqueId;
};

