/*
 *      Copyright (C) 2014 Team XBMC
 *      http://xbmc.org
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

#include "config_kodi.h"

#include "CompileInfo.h"
#include <CoreLib/VxAppInfo.h>

#include <cstddef>
#include <string>
#include <algorithm>

#ifndef DATE
#ifdef __DATE__
#define DATE __DATE__
#else
#define DATE "xx/xx/xx"
#endif
#endif

#ifndef TIME
#ifdef __TIME__
#define TIME __TIME__
#else
#define TIME "xx:xx:xx"
#endif
#endif

 /* XXX Only unix build process has been tested */
#ifndef GITVERSION
#define GITVERSION ""
#endif
#ifndef GITTAG
#define GITTAG ""
#endif
#ifndef GITBRANCH
#define GITBRANCH ""
#endif

// NOTE: in order to be able to use Kodi updates the build has to be the Kodi info

int CCompileInfo::GetMajor()
{
  return 18;
}

int CCompileInfo::GetMinor()
{
  return 1;
}

const char* CCompileInfo::GetPackage()
{
  //return "net.gotvptop";
    return "org.xbmc.kodi";
}

const char* CCompileInfo::GetClass()
{
  static std::string s_classname;
  
  if (s_classname.empty())
  {
    s_classname = CCompileInfo::GetPackage();
    std::replace(s_classname.begin(), s_classname.end(), '.', '/');
  }
  return s_classname.c_str();
}

const char* CCompileInfo::GetAppName()
{
//#if BUILD_NLC_APP
//    return "NlcPtoP";
//#elif BUILD_KODIQT_APP
//    return "KodiQt";
//#elif BUILD_KODIP2P_APP
//    return "KodiP2P";
//#else
//    echo you must define your application information in NlcAppConfig.h
//#endif
    return "Kodi";
}

const char* CCompileInfo::GetAppNameLowerCase()
{
//#if BUILD_NLC_APP
//    return "gotvptop";
//#elif BUILD_KODIQT_APP
//    return "kodiqt";
//#elif BUILD_KODIP2P_APP
//    return "kodiptop";
//#else
//    echo you must define your application information in NlcAppConfig.h
//#endif
    return "gotvptop";
}

const char* CCompileInfo::GetSuffix()
{
  return "";
}

const char* CCompileInfo::GetSCMID()
{
  return "20190310-nogitfound";
}

const char* CCompileInfo::GetCopyrightYears()
{
    return "2005-2018";
}

std::string  CCompileInfo::GetBuildDate()
{
    const std::string bdate = "20190318";
    if( !bdate.empty() )
    {
        std::string datestamp = bdate.substr( 0, 4 ) + "-" + bdate.substr( 4, 2 ) + "-" + bdate.substr( 6, 2 );
        return datestamp;
    }

    return "1970-01-01";
}

const char* CCompileInfo::GetHomeEnvName()
{
#if BUILD_NLC_APP
    return "GOTVPTOP_HOME";
#elif BUILD_KODIQT_APP
    return "KODIQT_HOME";
#elif BUILD_KODIP2P_APP
    return "KODIP2P_HOME";
#else
    echo you must define your application information in NlcAppConfig.h
#endif

}


const char* CCompileInfo::GetBinHomeEnvName()
{
#if BUILD_NLC_APP
    return "GOTVPTOP_BIN_HOME";
#elif BUILD_KODIQT_APP
    return "KODIQT_BIN_HOME";
#elif BUILD_KODIP2P_APP
    return "KODIP2P_BIN_HOME";
#else
    echo you must define your application information in NlcAppConfig.h
#endif

}

const char* CCompileInfo::GetBinAddonEnvName()
{
#if BUILD_NLC_APP
    return "GOTVPTOP_BINADDON_PATH";
#elif BUILD_KODIQT_APP
    return "KODIQT_BINADDON_PATH";
#elif BUILD_KODIP2P_APP
    return "KODIP2P_BINADDON_PATH";
#else
    echo you must define your application information in NlcAppConfig.h
#endif

}

const char* CCompileInfo::GetTempEnvName()
{
#if BUILD_NLC_APP
    return "GOTVPTOP_TEMP";
#elif BUILD_KODIQT_APP
    return "KODIQT_TEMP";
#elif BUILD_KODIP2P_APP
    return "KODIP2P_TEMP";
#else
    echo you must define your application information in NlcAppConfig.h
#endif

}

const char* CCompileInfo::GetUserProfileEnvName()
{
#if BUILD_NLC_APP
    return "KODI_PROFILE_USERDATA";
#elif BUILD_KODIQT_APP
    return "KODIQT_PROFILE_USERDATA";
#elif BUILD_KODIP2P_APP
    return "KODIP2P_PROFILE_USERDATA";
#else
    echo you must define your application information in NlcAppConfig.h
#endif

}
