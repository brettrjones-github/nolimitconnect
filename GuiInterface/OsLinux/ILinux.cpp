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


#include "ILinux.h"
#ifdef TARGET_OS_LINUX
#include "GuiInterface/INlc.h" 

#if ENABLE_KODI
#include "CompileInfo.h"
#include "threads/Thread.h"
#include "platform/xbmc.h"
#include "settings/AdvancedSettings.h"
#include "utils/CPUInfo.h"
#include "platform/Environment.h"
#include "utils/CharsetConverter.h" // Required to initialize converters before usage

#include "filesystem/Directory.h"
#include "filesystem/SpecialProtocol.h"
#include "utils/log.h"
#include "utils/URIUtils.h"
#include "NlcCoreUtil.h"

using namespace XFILE;

#endif // ENABLE_KODI

#include <CoreLib/VxDebug.h>

//============================================================================
ILinux::ILinux( INlc& gotv )
    : OsInterface( gotv )
{
}

//=== stages of create ===//
//============================================================================
bool ILinux::doPreStartup()
{
    LogModule(eLogStartup, LOG_VERBOSE, "ILinux::doPreStartup");
    // this fixes crash if OPENSSL_CONF is set to existed openssl.cfg
    // need to set it as soon as possible  
#if ENABLE_KODI
    CEnvironment::unsetenv( "OPENSSL_CONF" );
#endif// ENABLE_KODI
    //g_advancedSettings.Initialize();
    return true;
}

bool ILinux::doStartup()
{
    LogModule(eLogStartup, LOG_VERBOSE, "ILinux::doStartup");
    return true;
}

//=== stages of run ===//
//============================================================================

//=== stages of destroy ===//
//============================================================================
void ILinux::doPreShutdown( )
{
    LogModule(eLogStartup, LOG_VERBOSE, "ILinux::doPreShutdown");
}

//============================================================================
void ILinux::doShutdown( )
{
    LogModule(eLogStartup, LOG_VERBOSE, "ILinux::doShutdown");
    // clear previously set timer resolution
    // timeEndPeriod( 1 );
}

//=== utilities ===//
//============================================================================

#endif // TARGET_OS_LINUX
