/*
 *      Copyright (C) 2005-2013 Team XBMC
 *      http://kodi.tv
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with XBMC; see the file COPYING.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 */

#pragma once
#include "config_kodi.h"
#ifdef HAVE_QT_GUI

#include "QtUtils.h"

 //#include "rendering/gles/RenderSystemGLES.h"
#include "threads/CriticalSection.h"
#include "windowing/WinSystem.h"
#include "threads/SystemClock.h"
//#include "EGL/egl.h"

class IDispResource;

class CWinSystemQt : public CWinSystemBase
{
public:
    const char * MONITOR_QT_NAME = "AppletKodi"; 

    CWinSystemQt();
    virtual ~CWinSystemQt();

    bool                        InitWindowSystem() override;
    bool                        DestroyWindowSystem() override;

    bool                        CreateNewWindow( const std::string& name,
                                                 bool fullScreen,
                                                 RESOLUTION_INFO& res ) override;

    bool                        DestroyWindow() override;
    void                        UpdateResolutions() override;

    bool                        HasCursor() override { return false; };

    bool                        Hide() override;
    bool                        Show( bool raise = true ) override;
    void                        Register( IDispResource *resource ) override;
    void                        Unregister( IDispResource *resource ) override;

    void                        GetConnectedOutputs( std::vector<std::string> *outputs );

    void                        MessagePush( XBMC_Event *newEvent );

    // winevents override
    bool                        MessagePump() override;

protected:
    std::unique_ptr<KODI::WINDOWING::IOSScreenSaver> GetOSScreenSaverImpl() override;

    CQtUtils *                  m_qt;

    //  EGLDisplay m_nativeDisplay;
    //  EGLNativeWindowType m_nativeWindow;

    int                         m_displayWidth;
    int                         m_displayHeight;

    //  RENDER_STEREO_MODE      m_stereo_mode;

    bool                        m_delayDispReset;
    XbmcThreads::EndTime        m_dispResetTimer;

    CCriticalSection            m_resourceSection;
    std::vector<IDispResource*> m_resources;
};

#endif // HAVE_QT_GUI
