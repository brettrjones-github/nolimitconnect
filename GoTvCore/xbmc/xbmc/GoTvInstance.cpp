
#include "GoTvInstance.h"
//#include "system.h"
#include "FileItem.h"
#include "messaging/ApplicationMessenger.h"
#include "PlayListPlayer.h"
//#include "XBApplicationEx.h"
#include <utils/log.h>
#include <threads/SystemClock.h>
#ifdef HAS_PERFORMANCE_SAMPLE
# include <utils/PerformanceSample.h>
#else
# define MEASURE_FUNCTION
#endif

#include <commons/Exception.h>
#ifdef TARGET_POSIX
# include <linux/XTimeUtils.h>
#endif

#include <GoTvCore/GoTvP2P/P2PEngine/P2PEngine.h>
#include <GuiInterface/IToGui.h>

// Put this here for easy enable and disable
#ifndef _DEBUG
# define XBMC_TRACK_EXCEPTIONS
#endif

//============================================================================
GoTvInstance::GoTvInstance( P2PEngine& engine )
: IGoTv( engine )
, m_IsCreated( false )
, m_bStop( false )
, m_AppFocused( true )
, m_ExitCode( EXITCODE_QUIT )
, m_renderGUI( false )
{
}

//============================================================================
bool GoTvInstance::runGoTv( void )
{
    return Run();
}

//============================================================================
int GoTvInstance::toGuiPlayVideoFrame( VxGUID& onlineId, uint8_t * picBuf, uint32_t picBufLen, int picWidth, int picHeight )
{
    return m_Engine.getToGuiInterface().toGuiPlayVideoFrame( onlineId, picBuf, picBufLen, picWidth, picHeight );
}

//============================================================================
void GoTvInstance::destroyGoTv( void )
{
    if( m_IsCreated )
    {
        Destroy();
    }
}

//============================================================================
bool GoTvInstance::checkCreateGoTv( void )
{
    if( !m_IsCreated )
    {
        m_IsCreated = true;
        if( createGoTv() )
        {
            m_IsInitialized = true;
        }
        else
        {
            CLog::Log( LOGERROR, "XBAppEx: Call to Initialize() failed!" );
            return false;
        }   
    }

    return true;
}

//============================================================================
// Destroy the app 
void GoTvInstance::Destroy()
{
    m_IsCreated = false;
    m_IsInitialized = false;
    CLog::Log( LOGNOTICE, "destroy" );
    // Perform app-specific cleanup
    Cleanup();
}

/* Function that runs the application */
/*
INT CXBApplicationEx::Run(CFileItemList &playlist)
{

  if (playlist.Size() > 0)
  {
    g_playlistPlayer.Add(0, playlist);
    g_playlistPlayer.SetCurrentPlaylist(0);
    KODI::MESSAGING::CApplicationMessenger::GetInstance().PostMsg(TMSG_PLAYLISTPLAYER_PLAY, -1);
  }
    return Run();
}
*/

//============================================================================
bool GoTvInstance::Run()
{
    if( !m_IsInitialized )
    {
        return true;
    }

static unsigned int             lastFrameTime = 0;
static unsigned int             frameTime = 0;
static const unsigned int       noRenderFrameTime = 15;  // Simulates ~66fps

    bool result = false;

    // Run xbmc
    //while( !m_bStop )
    if( !m_bStop )
    {
#ifdef HAS_PERFORMANCE_SAMPLE
        CPerformanceSample sampleLoop( "XBApplicationEx-loop" );
#endif
        //-----------------------------------------
        // Animate and render a frame
        //-----------------------------------------
#ifdef XBMC_TRACK_EXCEPTIONS
        try
        {
#endif
            result = true;
            lastFrameTime = XbmcThreads::SystemClockMillis();
            Process();
            //reset exception count
#ifdef XBMC_TRACK_EXCEPTIONS

        }
        catch( const XbmcCommons::UncheckedException &e )
        {
            e.LogThrowMessage( "CApplication::Process()" );
            throw;
        }
        catch( ... )
        {
            CLog::Log( LOGERROR, "exception in CApplication::Process()" );
            throw;
        }
#endif
        // Frame move the scene
#ifdef XBMC_TRACK_EXCEPTIONS
        try
        {
#endif
            if( !m_bStop )
            {
                FrameMove( true, m_renderGUI );
            }

            //reset exception count
#ifdef XBMC_TRACK_EXCEPTIONS
        }
        catch( const XbmcCommons::UncheckedException &e )
        {
            e.LogThrowMessage( "CApplication::FrameMove()" );
            throw;
        }
        catch( ... )
        {
            CLog::Log( LOGERROR, "exception in CApplication::FrameMove()" );
            throw;
        }
#endif

        // Render the scene
#ifdef XBMC_TRACK_EXCEPTIONS
        try
        {
#endif
            if( m_renderGUI && !m_bStop )
            {
                Render();
            }
            /*
            else if( !m_renderGUI )
            {
                frameTime = XbmcThreads::SystemClockMillis() - lastFrameTime;
                if( frameTime < noRenderFrameTime )
                    Sleep( noRenderFrameTime - frameTime );
            }
            */
#ifdef XBMC_TRACK_EXCEPTIONS
        }
        catch( const XbmcCommons::UncheckedException &e )
        {
            e.LogThrowMessage( "CApplication::Render()" );
            throw;
        }
        catch( ... )
        {
            CLog::Log( LOGERROR, "exception in CApplication::Render()" );
            throw;
        }
#endif
    } // while (!m_bStop)
    //Destroy();

    //CLog::Log( LOGNOTICE, "application stopped..." );
    //return m_ExitCode;
    return result && !m_bStop;
}


//============================================================================
void GoTvInstance::FrameMove( bool processEvents, bool processGUI )
{

}

//============================================================================
void GoTvInstance::Render( void )
{
}

//============================================================================
void GoTvInstance::Process( void )
{
}

