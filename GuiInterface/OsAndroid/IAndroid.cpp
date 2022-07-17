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


#include "IAndroid.h"
#ifdef TARGET_OS_ANDROID

#include "GuiInterface/INlc.h" 
#include <CoreLib/VxDebug.h>

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


IAndroid::IAndroid( INlc& gotv )
    : OsInterface( gotv )
{
}

//=== stages of create ===//
//============================================================================
bool IAndroid::doPreStartup()
{
    LogModule(eLogStartup, LOG_VERBOSE, "IAndroid::doPreStartup");
    // Initialize before CAppParamParser so it can set the log level
    //g_advancedSettings.Initialize();
    return true;
}

bool IAndroid::doStartup()
{
    LogModule(eLogStartup, LOG_VERBOSE, "IAndroid::doStartup");
    return true;
}

//=== stages of run ===//
//============================================================================

//bool  IAndroid::doRun()

//=== stages of destroy ===//
//============================================================================
void IAndroid::doPreShutdown( )
{
    LogModule(eLogStartup, LOG_VERBOSE, "IAndroid::doPreShutdown");
}

void IAndroid::doShutdown( )
{
    LogModule(eLogStartup, LOG_VERBOSE, "IAndroid::doShutdown");
}

#endif // TARGET_OS_ANDROID
