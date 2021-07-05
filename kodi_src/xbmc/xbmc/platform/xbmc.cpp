/*
 *  Copyright (C) 2005-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "Application.h"

#ifdef TARGET_RASPBERRY_PI
#include "platform/linux/RBP.h"
#endif

#ifdef TARGET_WINDOWS_DESKTOP
#include "platform/win32/IMMNotificationClient.h"
#include <mmdeviceapi.h>
#include <wrl/client.h>
#endif

#if defined(TARGET_OS_ANDROID) && !defined(HAVE_QT_GUI)
# include "platform/android/activity/XBMCApp.h"
#elif defined(TARGET_OS_ANDROID) && defined(HAVE_QT_GUI)
# include "platform/qt/KodiQtApp.h"
#endif

#include "platform/MessagePrinter.h"
#include "utils/log.h"
#include "commons/Exception.h"

static int g_KodiIsRunning = false;

extern "C" int XBMC_Run( bool renderGUI, const CAppParamParser &params )
{
    int status = -1;

	if ( g_KodiIsRunning )
	{
		CMessagePrinter::DisplayError("Kodi is already running");
        
		return status;
	}

    g_KodiIsRunning = true;

    if( !g_application.Create( params ) )
    {
        CMessagePrinter::DisplayError( "ERROR: Unable to create Kodi applet" );
        return status;
    }

#ifdef TARGET_RASPBERRY_PI
    if( !g_RBP.Initialize() )
        return false;
    g_RBP.LogFirmwareVersion();
#elif defined(TARGET_ANDROID)
    //CXBMCApp::get()->Initialize();
#endif

    if( renderGUI && !g_application.CreateGUI() )
    {
        CMessagePrinter::DisplayError( "ERROR: Unable to create GUI. Exiting" );
        g_application.Stop(EXITCODE_QUIT);
        g_application.Cleanup();
        return status;
    }
    if( !g_application.Initialize() )
    {
        CMessagePrinter::DisplayError( "ERROR: Unable to Initialize. Exiting" );
        return status;
    }

#ifdef TARGET_WINDOWS_DESKTOP
    Microsoft::WRL::ComPtr<IMMDeviceEnumerator> pEnumerator = nullptr;
    CMMNotificationClient cMMNC;
    HRESULT hr = CoCreateInstance( __uuidof(MMDeviceEnumerator), nullptr, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), // CLSID_MMDeviceEnumerator, nullptr, CLSCTX_ALL, IID_IMMDeviceEnumerator,
                                   reinterpret_cast< void** >( pEnumerator.GetAddressOf() ) );
    if( SUCCEEDED( hr ) )
    {
        pEnumerator->RegisterEndpointNotificationCallback( &cMMNC );
        pEnumerator = nullptr;
    }
#endif

    status = g_application.Run( params );

#ifdef TARGET_WINDOWS_DESKTOP
    // the end
    hr = CoCreateInstance( __uuidof(MMDeviceEnumerator), nullptr, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), // CLSID_MMDeviceEnumerator, nullptr, CLSCTX_ALL, IID_IMMDeviceEnumerator,
                           reinterpret_cast< void** >( pEnumerator.GetAddressOf() ) );
    if( SUCCEEDED( hr ) )
    {
        pEnumerator->UnregisterEndpointNotificationCallback( &cMMNC );
        pEnumerator = nullptr;
    }
#endif

#ifdef TARGET_RASPBERRY_PI
    g_RBP.Deinitialize();
#elif defined(TARGET_ANDROID)
    CXBMCApp::get()->Deinitialize();
#endif

	g_KodiIsRunning = false;

    return status;
}
