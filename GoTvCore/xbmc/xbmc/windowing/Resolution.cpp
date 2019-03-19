/*
 *  Copyright (C) 2005-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "windowing/Resolution.h"
#include "guilib/gui3d.h"
#include "windowing/GraphicContext.h"
#include "utils/Variant.h"
#include "utils/log.h"
#include "utils/MathUtils.h"
#include "settings/AdvancedSettings.h"
#include "settings/DisplaySettings.h"
#include "settings/Settings.h"
#include "settings/SettingsComponent.h"
#include "ServiceBroker.h"

#include <cstdlib>

RESOLUTION_INFO::RESOLUTION_INFO(int width, int height, float aspect, const std::string &mode) :
  strMode(mode)
{
  iWidth = width;
  iHeight = height;
  iBlanking = 0;
  iScreenWidth = width;
  iScreenHeight = height;
  fPixelRatio = aspect ? ((float)width)/height / aspect : 1.0f;
  bFullScreen = true;
  fRefreshRate = 0;
  dwFlags = iSubtitles = 0;
}

RESOLUTION_INFO::RESOLUTION_INFO(const RESOLUTION_INFO& res) :
  Overscan(res.Overscan),
  strMode(res.strMode),
  strOutput(res.strOutput),
  strId(res.strId)
{
  bFullScreen = res.bFullScreen;
  iWidth = res.iWidth; iHeight = res.iHeight;
  iScreenWidth = res.iScreenWidth; iScreenHeight = res.iScreenHeight;
  iSubtitles = res.iSubtitles; dwFlags = res.dwFlags;
  fPixelRatio = res.fPixelRatio; fRefreshRate = res.fRefreshRate;
  iBlanking = res.iBlanking;
}

float RESOLUTION_INFO::DisplayRatio() const
{
  return iWidth * fPixelRatio / iHeight;
}

RESOLUTION CResolutionUtils::ChooseBestResolution(float fps, int width, int height, bool is3D)
{
  RESOLUTION res = CServiceBroker::GetWinSystem()->GetGfxContext().GetVideoResolution();
  float weight;

  if (!FindResolutionFromOverride(fps, width, is3D, res, weight, false)) //find a refreshrate from overrides
  {
    if (!FindResolutionFromOverride(fps, width, is3D, res, weight, true)) //if that fails find it from a fallback
    {
      FindResolutionFromWhitelist(fps, width, height, is3D, res); //find a refreshrate from whitelist
    }
  }

  CLog::Log(LOGNOTICE, "Display resolution ADJUST : %s (%d) (weight: %.3f)",
            CServiceBroker::GetWinSystem()->GetGfxContext().GetResInfo(res).strMode.c_str(), res, weight);
  return res;
}

void CResolutionUtils::FindResolutionFromWhitelist(float fps, int width, int height, bool is3D, RESOLUTION &resolution)
{
  RESOLUTION_INFO curr = CServiceBroker::GetWinSystem()->GetGfxContext().GetResInfo(resolution);
  CLog::Log(LOGNOTICE, "Whitelist search for: width: %d, height: %d, fps: %0.3f, 3D: %s",
    width, height, fps, is3D ? "true" : "false");

  std::vector<CVariant> indexList = CServiceBroker::GetSettingsComponent()->GetSettings()->GetList(CSettings::SETTING_VIDEOSCREEN_WHITELIST);
  if (indexList.empty())
  {
    CLog::Log(LOGDEBUG, "Whitelist is empty using default one");
    std::vector<RESOLUTION> candidates;
    RESOLUTION_INFO info;
    std::string resString;
    CServiceBroker::GetWinSystem()->GetGfxContext().GetAllowedResolutions(candidates);
    for (const auto& c : candidates)
    {
      info = CServiceBroker::GetWinSystem()->GetGfxContext().GetResInfo(c);
      if (info.iScreenHeight >= curr.iScreenHeight && info.iScreenWidth >= curr.iScreenWidth &&
          (info.dwFlags & D3DPRESENTFLAG_MODEMASK) == (curr.dwFlags & D3DPRESENTFLAG_MODEMASK))
      {
        // do not add half refreshrates (25, 29.97 by default) as kodi cannot cope with
        // them on playback start. Especially interlaced content is not properly detected
        // and this causes ugly double switching.
        // This won't allow 25p / 30p playback on native refreshrate by default
        if ((info.fRefreshRate > 30) || (MathUtils::FloatEquals(info.fRefreshRate, 24.0f, 0.1f)))
      {
        resString = CDisplaySettings::GetInstance().GetStringFromRes(c);
        indexList.push_back(resString);
        }
      }
    }
  }

  CLog::Log(LOGDEBUG, "Trying to find exact refresh rate");

  for (const auto &mode : indexList)
  {
    auto i = CDisplaySettings::GetInstance().GetResFromString(mode.asString());
    const RESOLUTION_INFO info = CServiceBroker::GetWinSystem()->GetGfxContext().GetResInfo(i);

    // allow resolutions that are exact and have the correct refresh rate
    if (((height == info.iScreenHeight && width <= info.iScreenWidth) ||
         (width == info.iScreenWidth && height <= info.iScreenHeight)) &&
        (info.dwFlags & D3DPRESENTFLAG_MODEMASK) == (curr.dwFlags & D3DPRESENTFLAG_MODEMASK) &&
        MathUtils::FloatEquals(info.fRefreshRate, fps, 0.01f))
    {
      CLog::Log(LOGDEBUG, "Matched exact whitelisted Resolution %s (%d)", info.strMode.c_str(), i);
      resolution = i;
      return;
    }
  }

  CLog::Log(LOGDEBUG, "No exact whitelisted resolution matched, trying double refresh rate");

  for (const auto &mode : indexList)
  {
    auto i = CDisplaySettings::GetInstance().GetResFromString(mode.asString());
    const RESOLUTION_INFO info = CServiceBroker::GetWinSystem()->GetGfxContext().GetResInfo(i);

    // allow resolutions that are exact and have double the refresh rate
    if (((height == info.iScreenHeight && width <= info.iScreenWidth) ||
         (width == info.iScreenWidth && height <= info.iScreenHeight)) &&
        (info.dwFlags & D3DPRESENTFLAG_MODEMASK) == (curr.dwFlags & D3DPRESENTFLAG_MODEMASK) &&
        MathUtils::FloatEquals(info.fRefreshRate, fps * 2, 0.01f))
    {
      CLog::Log(LOGDEBUG, "Matched fuzzy whitelisted Resolution %s (%d)", info.strMode.c_str(), i);
      resolution = i;
      return;
    }
  }

  CLog::Log(LOGDEBUG, "No double refresh rate whitelisted resolution matched, trying current resolution");

  for (const auto &mode : indexList)
  {
    auto i = CDisplaySettings::GetInstance().GetResFromString(mode.asString());
    const RESOLUTION_INFO info = CServiceBroker::GetWinSystem()->GetGfxContext().GetResInfo(i);

    const RESOLUTION_INFO desktop_info = CServiceBroker::GetWinSystem()->GetGfxContext().GetResInfo(CDisplaySettings::GetInstance().GetCurrentResolution());

    // allow resolutions that are desktop resolution but have the correct refresh rate
    if (info.iScreenWidth == desktop_info.iWidth &&
        (info.dwFlags & D3DPRESENTFLAG_MODEMASK) == (desktop_info.dwFlags & D3DPRESENTFLAG_MODEMASK) &&
        MathUtils::FloatEquals(info.fRefreshRate, fps, 0.01f))
    {
      CLog::Log(LOGDEBUG, "Matched fuzzy whitelisted Resolution %s (%d)", info.strMode.c_str(), i);
      resolution = i;
      return;
    }
  }

  CLog::Log(LOGDEBUG, "No larger whitelisted resolution matched, trying current resolution with double refreshrate");

  for (const auto &mode : indexList)
  {
    auto i = CDisplaySettings::GetInstance().GetResFromString(mode.asString());
    const RESOLUTION_INFO info = CServiceBroker::GetWinSystem()->GetGfxContext().GetResInfo(i);

    const RESOLUTION_INFO desktop_info = CServiceBroker::GetWinSystem()->GetGfxContext().GetResInfo(CDisplaySettings::GetInstance().GetCurrentResolution());

    // allow resolutions that are desktop resolution but have double the refresh rate
    if (info.iScreenWidth == desktop_info.iWidth &&
        (info.dwFlags & D3DPRESENTFLAG_MODEMASK) == (desktop_info.dwFlags & D3DPRESENTFLAG_MODEMASK) &&
        MathUtils::FloatEquals(info.fRefreshRate, fps * 2, 0.01f))
    {
      CLog::Log(LOGDEBUG, "Matched fuzzy whitelisted Resolution %s (%d)", info.strMode.c_str(), i);
      resolution = i;
      return;
    }
  }

  CLog::Log(LOGDEBUG, "No whitelisted resolution matched");
}

bool CResolutionUtils::FindResolutionFromOverride(float fps, int width, bool is3D, RESOLUTION &resolution, float& weight, bool fallback)
{
  RESOLUTION_INFO curr = CServiceBroker::GetWinSystem()->GetGfxContext().GetResInfo(resolution);

  //try to find a refreshrate from the override
  for (int i = 0; i < (int)CServiceBroker::GetSettingsComponent()->GetAdvancedSettings()->m_videoAdjustRefreshOverrides.size(); i++)
  {
    RefreshOverride& override = CServiceBroker::GetSettingsComponent()->GetAdvancedSettings()->m_videoAdjustRefreshOverrides[i];

    if (override.fallback != fallback)
      continue;

    //if we're checking for overrides, check if the fps matches
    if (!fallback && (fps < override.fpsmin || fps > override.fpsmax))
      continue;

    for (size_t j = (int)RES_DESKTOP; j < CDisplaySettings::GetInstance().ResolutionInfoSize(); j++)
    {
      RESOLUTION_INFO info = CServiceBroker::GetWinSystem()->GetGfxContext().GetResInfo((RESOLUTION)j);

      if (info.iScreenWidth  == curr.iScreenWidth &&
          info.iScreenHeight == curr.iScreenHeight &&
          (info.dwFlags & D3DPRESENTFLAG_MODEMASK) == (curr.dwFlags & D3DPRESENTFLAG_MODEMASK))
      {
        if (info.fRefreshRate <= override.refreshmax &&
            info.fRefreshRate >= override.refreshmin)
        {
          resolution = (RESOLUTION)j;

          if (fallback)
          {
            CLog::Log(LOGDEBUG, "Found Resolution %s (%d) from fallback (refreshmin:%.3f refreshmax:%.3f)",
                      info.strMode.c_str(), resolution,
                      override.refreshmin, override.refreshmax);
          }
          else
          {
            CLog::Log(LOGDEBUG, "Found Resolution %s (%d) from override of fps %.3f (fpsmin:%.3f fpsmax:%.3f refreshmin:%.3f refreshmax:%.3f)",
                      info.strMode.c_str(), resolution, fps,
                      override.fpsmin, override.fpsmax, override.refreshmin, override.refreshmax);
          }

          weight = RefreshWeight(info.fRefreshRate, fps);

          return true; //fps and refresh match with this override, use this resolution
        }
      }
    }
  }

  return false; //no override found
}

//distance of refresh to the closest multiple of fps (multiple is 1 or higher), as a multiplier of fps
float CResolutionUtils::RefreshWeight(float refresh, float fps)
{
  float div   = refresh / fps;
  int   round = MathUtils::round_int(div);

  float weight = 0.0f;

  if (round < 1)
    weight = (fps - refresh) / fps;
  else
    weight = (float)fabs(div / round - 1.0);

  // punish higher refreshrates and prefer better matching
  // e.g. 30 fps content at 60 hz is better than
  // 30 fps at 120 hz - as we sometimes don't know if
  // the content is interlaced at the start, only
  // punish when refreshrate > 60 hz to not have to switch
  // twice for 30i content
  if (refresh > 60 && round > 1)
    weight += round / 10000.0;

  return weight;
}

bool CResolutionUtils::HasWhitelist()
{
  std::vector<CVariant> indexList = CServiceBroker::GetSettingsComponent()->GetSettings()->GetList(CSettings::SETTING_VIDEOSCREEN_WHITELIST);
  return !indexList.empty();
}
