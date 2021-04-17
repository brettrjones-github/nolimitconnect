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

// some of this code is copied from Kodi
#include <app_precompiled_hdr.h>
#include "IWin32.h"
#ifdef TARGET_OS_WINDOWS

#include "GuiInterface/IGoTv.h" 

#include "CompileInfo.h"
#include "threads/Thread.h"
#include "threads/platform/win/Win32Exception.h"
#include "platform/win32/CharsetConverter.h"
#include "platform/xbmc.h"
#include "settings/AdvancedSettings.h"
#include "utils/CPUInfo.h"
#include "platform/Environment.h"
#include "utils/CharsetConverter.h" // Required to initialize converters before usage

#include <dbghelp.h>
#include <mmsystem.h>
#include <Objbase.h>
#include <shellapi.h>
#include <WinSock2.h>

#include "filesystem/Directory.h"
#include "filesystem/SpecialProtocol.h"
#include "utils/log.h"
#include "utils/URIUtils.h"
#include "GoTvCoreUtil.h"
#include "platform/win32/WIN32Util.h"

#include "ServiceBroker.h"
#include "settings/SettingsComponent.h"

#include <CoreLib/VxDebug.h>

using namespace XFILE;

//============================================================================
// Minidump creation function
LONG WINAPI CreateMiniDump( EXCEPTION_POINTERS* pEp )
{
    win32_exception::write_stacktrace( pEp );
    win32_exception::write_minidump( pEp );
    return pEp->ExceptionRecord->ExceptionCode;
}

//============================================================================
IWin32::IWin32( IGoTv& gotv )
    : OsInterface( gotv )
    , m_AppRunningMutex( 0 )
{
}

//=== stages of create ===//
//============================================================================
bool IWin32::doPreStartup()
{
    LogModule( eLogStartup, LOG_VERBOSE, "IWin32::doPreStartup" );
    using KODI::PLATFORM::WINDOWS::ToW;
    // this fixes crash if OPENSSL_CONF is set to existed openssl.cfg  
    // need to set it as soon as possible  
    CEnvironment::unsetenv( "OPENSSL_CONF" );

    // Initializes CreateMiniDump to handle exceptions.
    char ver[ 100 ];
    if( strlen( CCompileInfo::GetSuffix() ) > 0 )
        sprintf_s( ver, "%d.%d-%s Git:%s", CCompileInfo::GetMajor(),
                   CCompileInfo::GetMinor(), CCompileInfo::GetSuffix(), CCompileInfo::GetSCMID() );
    else
        sprintf_s( ver, "%d.%d Git:%s", CCompileInfo::GetMajor(),
                   CCompileInfo::GetMinor(), CCompileInfo::GetSCMID() );

    if( win32_exception::ShouldHook() )
    {
        win32_exception::set_version( std::string( ver ) );
        SetUnhandledExceptionFilter( CreateMiniDump );
    }

    // check if application is already running
    std::string appName = CCompileInfo::GetAppName();
    m_AppRunningMutex = CreateMutexW( nullptr, FALSE, ToW( appName + " GoTvPtoP" ).c_str() );
    if( GetLastError() == ERROR_ALREADY_EXISTS )
    {
        auto appNameW = ToW( appName );
        HWND hwnd = FindWindowW( appNameW.c_str(), appNameW.c_str() );
        if( hwnd != nullptr )
        {
            // switch to the running instance
            ShowWindow( hwnd, SW_RESTORE );
            SetForegroundWindow( hwnd );
        }
        ReleaseMutex( m_AppRunningMutex );
        m_AppRunningMutex = 0;
        return 0;
    }

    if( ( g_cpuInfo.GetCPUFeatures() & CPU_FEATURE_SSE2 ) == 0 )
    {
        MessageBoxW( NULL, L"No SSE2 support detected", ToW( appName + ": Fatal Error" ).c_str(), MB_OK | MB_ICONERROR );
        ReleaseMutex( m_AppRunningMutex );
        m_AppRunningMutex = 0;
       return 0;
    }

    //Initialize COM
    CoInitializeEx( nullptr, COINIT_MULTITHREADED );


    // Initialise Winsock
    WSADATA wd;
    WSAStartup( MAKEWORD( 2, 2 ), &wd );

    // use 1 ms timer precision - like SDL initialization used to do
    timeBeginPeriod( 1 );

#ifndef _DEBUG
    // we don't want to see the "no disc in drive" windows message box
    SetErrorMode( SEM_FAILCRITICALERRORS | SEM_NOOPENFILEERRORBOX );
#endif
    // Initialize before CAppParamParser so it can set the log level
    //BRJ CServiceBroker::GetSettingsComponent()->GetAdvancedSettings()->Initialize();
    return true;
}

bool IWin32::doStartup()
{
    return true;
}

//=== stages of run ===//
//============================================================================

//bool  IWin32::doRun()

//=== stages of destroy ===//
//============================================================================
void IWin32::doPreShutdown( )
{
    LogModule( eLogStartup, LOG_VERBOSE, "IWin32::doPreShutdown" );
}

//============================================================================
void IWin32::doShutdown( )
{
    LogModule( eLogStartup, LOG_VERBOSE, "IWin32::doShutdown" );

    // clear previously set timer resolution
    timeEndPeriod( 1 );

    WSACleanup();
    CoUninitialize();
    ReleaseMutex( m_AppRunningMutex );
}


#endif // TARGET_OS_WINDOWS
