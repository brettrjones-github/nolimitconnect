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


#include "IApple.h"
#ifdef TARGET_OS_APPLE

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


#include "filesystem/Directory.h"
#include "filesystem/SpecialProtocol.h"
#include "utils/log.h"
#include "utils/URIUtils.h"
#include "GoTvCoreUtil.h"
#include "platform/win32/WIN32Util.h"



using namespace XFILE;



IApple::IApple( IGoTv& gotv )
    : OsInterface( gotv )
{
}

//=== stages of create ===//
//============================================================================
bool IApple::doPreStartup()
{
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
    m_AppRunningMutex = CreateMutexW( nullptr, FALSE, ToW( appName + " Media Center" ).c_str() );
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
    g_advancedSettings.Initialize();
    return true;
}

bool IApple::doStartup()
{
    return true;
}

//=== stages of run ===//
//============================================================================

//bool  IWin32::doRun()

//=== stages of destroy ===//
//============================================================================
void IApple::doPreShutdown( )
{


}

void IApple::doShutdown( )
{

    // clear previously set timer resolution
    timeEndPeriod( 1 );

    WSACleanup();
    CoUninitialize();
    ReleaseMutex( m_AppRunningMutex );
}

//=== utilities ===//
//============================================================================

bool IApple::initDirectories()
{
#if defined(TARGET_DARWIN)
    std::string userName;
    if( getenv( "USER" ) )
        userName = getenv( "USER" );
    else
        userName = "root";

    std::string userHome;
    if( getenv( "HOME" ) )
        userHome = getenv( "HOME" );
    else
        userHome = "/root";

    std::string binaddonAltDir;
    if( getenv( CCompileInfo::GetBinAddonEnvName() ) )
        binaddonAltDir = getenv( CCompileInfo::GetBinAddonEnvName() );

    std::string appPath = CUtil::GetHomePath();
    setenv( CCompileInfo::GetHomeEnvName(), appPath.c_str(), 0 );

#if defined(TARGET_DARWIN_IOS)
    std::string fontconfigPath;
    fontconfigPath = appPath + "/system/players/VideoPlayer/etc/fonts/fonts.conf";
    setenv( "FONTCONFIG_FILE", fontconfigPath.c_str(), 0 );
#endif

    // setup path to our internal dylibs so loader can find them
    std::string frameworksPath = CUtil::GetFrameworksPath();
    CSpecialProtocol::SetXBMCFrameworksPath( frameworksPath );

    // OSX always runs with m_bPlatformDirectories == true
    if( m_bPlatformDirectories )
    {
        // map our special drives
        CSpecialProtocol::SetXBMCBinPath( appPath );
        CSpecialProtocol::SetXBMCAltBinAddonPath( binaddonAltDir );
        CSpecialProtocol::SetXBMCPath( appPath );
#if defined(TARGET_DARWIN_IOS)
        std::string appName = CCompileInfo::GetAppName();
        CSpecialProtocol::SetHomePath( userHome + "/" + CDarwinUtils::GetAppRootFolder() + "/" + appName );
        CSpecialProtocol::SetMasterProfilePath( userHome + "/" + CDarwinUtils::GetAppRootFolder() + "/" + appName + "/userdata" );
#else
        std::string appName = CCompileInfo::GetAppName();
        CSpecialProtocol::SetHomePath( userHome + "/Library/Application Support/" + appName );
        CSpecialProtocol::SetMasterProfilePath( userHome + "/Library/Application Support/" + appName + "/userdata" );
#endif

        std::string dotLowerAppName = "." + appName;
        StringUtils::ToLower( dotLowerAppName );
        // location for temp files
#if defined(TARGET_DARWIN_IOS)
        std::string strTempPath = URIUtils::AddFileToFolder( userHome, std::string( CDarwinUtils::GetAppRootFolder() ) + "/" + appName + "/temp" );
#else
        std::string strTempPath = URIUtils::AddFileToFolder( userHome, dotLowerAppName + "/" );
        CDirectory::Create( strTempPath );
        strTempPath = URIUtils::AddFileToFolder( userHome, dotLowerAppName + "/temp" );
#endif
        CSpecialProtocol::SetTempPath( strTempPath );

        // xbmc.log file location
#if defined(TARGET_DARWIN_IOS)
        strTempPath = userHome + "/" + std::string( CDarwinUtils::GetAppRootFolder() );
#else
        strTempPath = userHome + "/Library/Logs";
#endif
        CSpecialProtocol::SetLogPath( strTempPath );
        CreateUserDirs();
    }
    else
    {
        URIUtils::AddSlashAtEnd( appPath );

        CSpecialProtocol::SetXBMCBinPath( appPath );
        CSpecialProtocol::SetXBMCAltBinAddonPath( binaddonAltDir );
        CSpecialProtocol::SetXBMCPath( appPath );
        CSpecialProtocol::SetHomePath( URIUtils::AddFileToFolder( appPath, "portable_data" ) );
        CSpecialProtocol::SetMasterProfilePath( URIUtils::AddFileToFolder( appPath, "portable_data/userdata" ) );

        // For P2P
        CSpecialProtocol::SetAppDataPath( URIUtils::AddFileToFolder( appPath, "portable_data/appdata" );
        CSpecialProtocol::SetAppAssetsPath( URIUtils::AddFileToFolder( appPath, "portable_data/appassets" );
        CSpecialProtocol::SetAccountsPath( URIUtils::AddFileToFolder( appPath, "portable_data/accounts" );
        CSpecialProtocol::SetUserXferPath( URIUtils::AddFileToFolder( appPath, "portable_data/userxfer" );
        CSpecialProtocol::SetUserGroupPath( URIUtils::AddFileToFolder( appPath, "portable_data/usergroup" );

        std::string strTempPath = URIUtils::AddFileToFolder( appPath, "portable_data/temp" );
        CSpecialProtocol::SetTempPath( strTempPath );
        CSpecialProtocol::SetLogPath( strTempPath );
    }
    CSpecialProtocol::SetXBMCBinAddonPath( appPath + "/addons" );
    return true;
#else
    return false;
#endif
}


#endif // TARGET_OS_APPLE
