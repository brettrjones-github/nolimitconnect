

#include <GuiInterface/IGoTv.h>
#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>

#include <CoreLib/VxDebug.h>
#include <CoreLib/VxGlobals.h>

#include "RenderKodiThread.h"
#include "RenderGlOffScreenSurface.h"
#include "RenderGlWidget.h"

//============================================================================
RenderKodiThread::RenderKodiThread( RenderGlLogic& renderLogic )
: QThread()
, m_RenderLogic( renderLogic )
{
    //connect( this, SIGNAL( finished() ), this, SLOT( deleteLater() ) );
}

//============================================================================
void RenderKodiThread::run()
{
    static bool isKodiRunning = false;
    if( !isKodiRunning )
    {
        LogMsg( LOG_ERROR, "RenderKodiThread %d\n", VxGetCurrentThreadId() );
        isKodiRunning = true;
#ifdef RENDER_LOGO_INSTEAD_OF_KODI
        qDebug() << "hello from worker thread " << VxGetCurrentThreadId();
        m_RenderLogic.initRenderGlSystem();
        while( m_ShouldRun )
        {
            if( !m_RenderLogic.beginRenderGl() )
            {
                qDebug() << "thread beginRender failed";
                break;
            }

            if( !m_RenderLogic.endRenderGl() )
            {
                qDebug() << "thread endRender failed";
                break;
            }

            m_RenderLogic.presentRenderGl( true, true );
            msleep( 30 );
        }

        m_RenderLogic.destroyRenderGlSystem();
        qDebug() << "worker thread done " << VxGetCurrentThreadId();
#else
        IGoTv& iGoTv = IGoTv::getIGoTv();
        // will not return from doRun until kodi is shutdown
        iGoTv.doRun( eAppModuleKodi );
        // BRJ FIXME
        // BRJ for no reason I can see g_RunKodi->deleteLater() will let memory allocated by Qt to be overwritten by normal malloc calls in kodi
        //    g_RunKodi->deleteLater();
        //g_RunKodi = nullptr;
#endif // RENDER_LOGO_INSTEAD_OF_KODI
        isKodiRunning = false;
    }
    else
    {
        LogMsg( LOG_ERROR, "Tried to run kodi twice\n" );
    }
}

//============================================================================
void RenderKodiThread::startRenderThread()
{
    if( !isRenderThreadStarted() )
    {
        m_IsThreadStarted = true;
        start();
    }
}

//============================================================================
void RenderKodiThread::stopRenderThread()
{
    if( isRenderThreadStarted() )
    {
        m_IsThreadStarted = false;
        //stop();
    }
}
