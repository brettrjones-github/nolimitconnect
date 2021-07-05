/*
 *  Copyright (C) 2005-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

//#include<libglew/include/gl/glew.h>
#include "config_kodi.h"
#ifdef HAVE_QT_GUI

#include "WinSystemQt.h"

#include <string.h>
#include <float.h>

#include "WinEventsQt.h"
#include "OSScreenSaverQt.h"
#include "ServiceBroker.h"
#include "windowing/GraphicContext.h"
#include "windowing/Resolution.h"
#include "settings/Settings.h"
#include "settings/DisplaySettings.h"
#include "guilib/DispResource.h"
#include "utils/log.h"
#include "threads/SingleLock.h"

#include "cores/RetroPlayer/process/qt/RPProcessInfoQt.h"
#include "cores/VideoPlayer/DVDCodecs/Video/DVDVideoCodecFFmpeg.h"
#include "cores/VideoPlayer/DVDCodecs/Audio/DVDAudioCodecFFmpeg.h"
#include "cores/VideoPlayer/VideoRenderers/RendererQt.h"
#include "cores/AudioEngine/Sinks/AESinkQt.h"
#include "cores/AudioEngine/AESinkFactory.h"

//#include "platform/qt/drm/MediaDrmCryptoSession.h"
//#include <qtjni/MediaCodecList.h>

using namespace KODI;

//============================================================================
CWinSystemQt::CWinSystemQt()
{
    m_displayWidth = 0;
    m_displayHeight = 0;

    //  m_stereo_mode = RENDER_STEREO_MODE_OFF;
    m_delayDispReset = false;

    m_qt = nullptr;

    m_winEvents.reset( new CWinEventsQt() );
    AE::CAESinkFactory::ClearSinks();
    CAESinkQt::Register(); // register sound sink
}

//============================================================================
CWinSystemQt::~CWinSystemQt()
{
    //  if(m_nativeWindow)
    //  {
    //    m_nativeWindow = nullptr;
    //  }
}

//============================================================================
bool CWinSystemQt::InitWindowSystem()
{
    m_qt = new CQtUtils();

    //  CDVDVideoCodecQtMediaCodec::Register();
    //  CDVDAudioCodecQtMediaCodec::Register();

    CRendererQt::Register();
    RETRO::CRPProcessInfoQt::Register();
    //RETRO::CRPProcessInfoQt::RegisterRendererFactory(new RETRO::CRendererFactoryQt);
    //  CRendererMediaCodec::Register();
    //  CRendererMediaCodecSurface::Register();
    //  ADDON::Interface_Qt::Register();
    //  DRM::CMediaDrmCryptoSession::Register();
    return CWinSystemBase::InitWindowSystem();
}

//============================================================================
bool CWinSystemQt::DestroyWindowSystem()
{
    delete m_qt;
    m_qt = nullptr;

    return true;
}

//============================================================================
void CWinSystemQt::GetConnectedOutputs( std::vector<std::string>* outputs )
{
    if( outputs )
    {
        outputs->push_back( MONITOR_QT_NAME );
    }
}

//============================================================================
bool CWinSystemQt::CreateNewWindow( const std::string& name, bool fullScreen, RESOLUTION_INFO& res )
{
    RESOLUTION_INFO current_resolution;
    current_resolution.iWidth = current_resolution.iHeight = 0;
    RENDER_STEREO_MODE stereo_mode = CServiceBroker::GetWinSystem()->GetGfxContext().GetStereoMode();

    m_nWidth = res.iWidth;
    m_nHeight = res.iHeight;
    m_displayWidth = res.iScreenWidth;
    m_displayHeight = res.iScreenHeight;
    m_fRefreshRate = res.fRefreshRate;

    if( ( m_bWindowCreated && m_qt->GetNativeResolution( &current_resolution ) ) &&
        current_resolution.iWidth == res.iWidth && current_resolution.iHeight == res.iHeight &&
        current_resolution.iScreenWidth == res.iScreenWidth && current_resolution.iScreenHeight == res.iScreenHeight &&
        m_bFullScreen == fullScreen && current_resolution.fRefreshRate == res.fRefreshRate &&
        ( current_resolution.dwFlags & D3DPRESENTFLAG_MODEMASK ) == ( res.dwFlags & D3DPRESENTFLAG_MODEMASK ) ) //&&
    //    m_stereo_mode == stereo_mode)
    {
        CLog::Log( LOGDEBUG, "CWinSystemEGL::CreateNewWindow: No need to create a new window" );
        return true;
    }


    {
        CSingleLock lock( m_resourceSection );
        for( std::vector<IDispResource *>::iterator i = m_resources.begin(); i != m_resources.end(); ++i )
        {
            ( *i )->OnLostDisplay();
        }
    }

    //  m_stereo_mode = stereo_mode;
    m_bFullScreen = fullScreen;

    //  m_nativeWindow = CXBMCApp::GetNativeWindow(2000);

    m_qt->SetNativeResolution( res );

    {
        CSingleLock lock( m_resourceSection );
        // tell any shared resources
        for( std::vector<IDispResource *>::iterator i = m_resources.begin(); i != m_resources.end(); ++i )
        {
            ( *i )->OnResetDisplay();
        }
    }

    return true;
}

bool CWinSystemQt::DestroyWindow()
{
    return true;
}

void CWinSystemQt::UpdateResolutions()
{
    CWinSystemBase::UpdateResolutions();

    RESOLUTION_INFO resDesktop, curDisplay;
    std::vector<RESOLUTION_INFO> resolutions;

    if( !m_qt->ProbeResolutions( resolutions ) || resolutions.empty() )
    {
        CLog::Log( LOGWARNING, "%s: ProbeResolutions failed.", __FUNCTION__ );
    }

    /* ProbeResolutions includes already all resolutions.
     * Only get desktop resolution so we can replace xbmc's desktop res
     */
    if( m_qt->GetNativeResolution( &curDisplay ) )
    {
        resDesktop = curDisplay;
    }

    RESOLUTION ResDesktop = RES_INVALID;
    RESOLUTION res_index = RES_DESKTOP;

    for( size_t i = 0; i < resolutions.size(); i++ )
    {
        // if this is a new setting,
        // create a new empty setting to fill in.
        if( ( int )CDisplaySettings::GetInstance().ResolutionInfoSize() <= res_index )
        {
            RESOLUTION_INFO res;
            CDisplaySettings::GetInstance().AddResolutionInfo( res );
        }

        CServiceBroker::GetWinSystem()->GetGfxContext().ResetOverscan( resolutions[ i ] );
        CDisplaySettings::GetInstance().GetResolutionInfo( res_index ) = resolutions[ i ];

        CLog::Log( LOGNOTICE, "Found resolution %d x %d for with %d x %d%s @ %f Hz\n",
                   resolutions[ i ].iWidth,
                   resolutions[ i ].iHeight,
                   //      resolutions[i].iScreen,
                   resolutions[ i ].iScreenWidth,
                   resolutions[ i ].iScreenHeight,
                   resolutions[ i ].dwFlags & D3DPRESENTFLAG_INTERLACED ? "i" : "",
                   resolutions[ i ].fRefreshRate );

        if( resDesktop.iWidth == resolutions[ i ].iWidth &&
            resDesktop.iHeight == resolutions[ i ].iHeight &&
            resDesktop.iScreenWidth == resolutions[ i ].iScreenWidth &&
            resDesktop.iScreenHeight == resolutions[ i ].iScreenHeight &&
            ( resDesktop.dwFlags & D3DPRESENTFLAG_MODEMASK ) == ( resolutions[ i ].dwFlags & D3DPRESENTFLAG_MODEMASK ) &&
            fabs( resDesktop.fRefreshRate - resolutions[ i ].fRefreshRate ) < FLT_EPSILON )
        {
            ResDesktop = res_index;
        }

        res_index = ( RESOLUTION )( ( int )res_index + 1 );
    }

    // set RES_DESKTOP
    if( ResDesktop != RES_INVALID )
    {
        CLog::Log( LOGNOTICE, "Found (%dx%d%s@%f) at %d, setting to RES_DESKTOP at %d",
                   resDesktop.iWidth, resDesktop.iHeight,
                   resDesktop.dwFlags & D3DPRESENTFLAG_INTERLACED ? "i" : "",
                   resDesktop.fRefreshRate,
                   ( int )ResDesktop, ( int )RES_DESKTOP );

        CDisplaySettings::GetInstance().GetResolutionInfo( RES_DESKTOP ) = CDisplaySettings::GetInstance().GetResolutionInfo( ResDesktop );
    }

    //unsigned int num_codecs = CJNIMediaCodecList::getCodecCount();
    //for( int i = 0; i < num_codecs; i++ )
    //{
    //    CJNIMediaCodecInfo codec_info = CJNIMediaCodecList::getCodecInfoAt( i );
    //    if( codec_info.isEncoder() )
    //        continue;

    //    std::string codecname = codec_info.getName();
    //    CLog::Log( LOGNOTICE, "Mediacodec: %s", codecname.c_str() );
    //}
}

bool CWinSystemQt::Hide()
{
    return false;
}

bool CWinSystemQt::Show( bool raise )
{
    return false;
}

void CWinSystemQt::Register( IDispResource *resource )
{
    CSingleLock lock( m_resourceSection );
    m_resources.push_back( resource );
}

void CWinSystemQt::Unregister( IDispResource *resource )
{
    CSingleLock lock( m_resourceSection );
    std::vector<IDispResource*>::iterator i = find( m_resources.begin(), m_resources.end(), resource );
    if( i != m_resources.end() )
        m_resources.erase( i );
}

void CWinSystemQt::MessagePush( XBMC_Event *newEvent )
{
    dynamic_cast< CWinEventsQt& >( *m_winEvents ).MessagePush( newEvent );
}

bool CWinSystemQt::MessagePump()
{
    return m_winEvents->MessagePump();
}

std::unique_ptr<WINDOWING::IOSScreenSaver> CWinSystemQt::GetOSScreenSaverImpl()
{
    std::unique_ptr<KODI::WINDOWING::IOSScreenSaver> ret( new COSScreenSaverQt() );
    return ret;
}

#endif // HAVE_QT_GUI
