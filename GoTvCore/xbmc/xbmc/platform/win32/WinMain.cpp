/*
 *  Copyright (C) 2005-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "config_kodi.h"
#ifdef TARGET_OS_WINDOWS
#ifndef HAVE_QT_GUI


#include "AppParamParser.h"
#include "CompileInfo.h"
#include "threads/Thread.h"
#include "threads/platform/win/Win32Exception.h"
#include "platform/win32/CharsetConverter.h"
#include "platform/xbmc.h"
#include "settings/AdvancedSettings.h"
#include "utils/CPUInfo.h"
#include "platform/Environment.h"
#include "utils/CharsetConverter.h" // Required to initialize converters before usage

#include "AppParamParser.h" 
#include "GuiInterface/IGoTv.h" 



#include <dbghelp.h>
#include <mmsystem.h>
#include <Objbase.h>
#include <shellapi.h>
#include <WinSock2.h>


// Minidump creation function
LONG WINAPI CreateMiniDump(EXCEPTION_POINTERS* pEp)
{
  win32_exception::write_stacktrace(pEp);
  win32_exception::write_minidump(pEp);
  return pEp->ExceptionRecord->ExceptionCode;
}

//-----------------------------------------------------------------------------
// Name: WinMain()
// Desc: The application's entry point
//-----------------------------------------------------------------------------
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR commandLine, INT )
{
    using KODI::PLATFORM::WINDOWS::ToW;
  // this fixes crash if OPENSSL_CONF is set to existed openssl.cfg
  // need to set it as soon as possible
  CEnvironment::unsetenv("OPENSSL_CONF");

  // Initializes CreateMiniDump to handle exceptions.
  char ver[100];
  if (strlen(CCompileInfo::GetSuffix()) > 0)
    sprintf_s(ver, "%d.%d-%s Git:%s", CCompileInfo::GetMajor(),
    CCompileInfo::GetMinor(), CCompileInfo::GetSuffix(), CCompileInfo::GetSCMID());
  else
    sprintf_s(ver, "%d.%d Git:%s", CCompileInfo::GetMajor(),
    CCompileInfo::GetMinor(), CCompileInfo::GetSCMID());

  if (win32_exception::ShouldHook())
  {
    win32_exception::set_version(std::string(ver));
    SetUnhandledExceptionFilter(CreateMiniDump);
  }

  // check if Kodi is already running
  std::string appName = CCompileInfo::GetAppName();
  HANDLE appRunningMutex = CreateMutex(nullptr, FALSE, ToW(appName + " GoTvPtoP").c_str());
  if (GetLastError() == ERROR_ALREADY_EXISTS)
  {
    auto appNameW = ToW(appName);
    HWND hwnd = FindWindow(appNameW.c_str(), appNameW.c_str());
    if (hwnd != nullptr)
    {
      // switch to the running instance
      ShowWindow(hwnd, SW_RESTORE);
      SetForegroundWindow(hwnd);
    }
    ReleaseMutex(appRunningMutex);
    return 0;
  }

  if ((g_cpuInfo.GetCPUFeatures() & CPU_FEATURE_SSE2) == 0)
  {
    MessageBox(NULL, L"No SSE2 support detected", ToW(appName + ": Fatal Error").c_str(), MB_OK | MB_ICONERROR);
    ReleaseMutex(appRunningMutex);
    return 0;
  }

  //Initialize COM
  CoInitializeEx(nullptr, COINIT_MULTITHREADED);

    GetIGoTv().doPreStartup();

    // NOTE GetIGoTv().doStartup() will be called by XBMC_Run

    int argc;
    LPWSTR* argvW = CommandLineToArgvW( GetCommandLineW(), &argc );

    char** argv = new char*[ argc ];

    for( int i = 0; i < argc; ++i )
    {
        int size = WideCharToMultiByte( CP_UTF8, 0, argvW[ i ], -1, nullptr, 0, nullptr, nullptr );
        if( size > 0 )
        {
            argv[ i ] = new char[ size ];
            int result = WideCharToMultiByte( CP_UTF8, 0, argvW[ i ], -1, argv[ i ], size, nullptr, nullptr );
        }
    }

  // Initialise Winsock
  WSADATA wd;
  WSAStartup(MAKEWORD(2, 2), &wd);

  // use 1 ms timer precision - like SDL initialization used to do
  timeBeginPeriod(1);

#ifndef _DEBUG
  // we don't want to see the "no disc in drive" windows message box
  SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOOPENFILEERRORBOX);
#endif

  int status;
    {
        static CAppParamParser appParamParser;
        appParamParser.Parse( argv, argc );
        GetIGoTv().initRun( appParamParser );
        // Create and run the app
        GetIGoTv().doRun( );
    }

    for( int i = 0; i < argc; ++i )
        delete[] argv[ i ];
    delete[] argv;

    GetIGoTv().doPreShutdown();
    GetIGoTv().doShutdown();

  ReleaseMutex(appRunningMutex);
    return GetOsInterface().getRunResultCode();
}

#endif //ifndef HAVE_QT_GUI
#endif // TARGET_OS_WINDOWS
