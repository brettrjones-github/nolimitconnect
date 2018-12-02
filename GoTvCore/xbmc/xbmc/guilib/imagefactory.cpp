/*
 *  Copyright (C) 2012-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "imagefactory.h"
#include "guilib/FFmpegImage.h"
#include "addons/ImageDecoder.h"
#include "addons/binary-addons/BinaryAddonBase.h"
#include "utils/Mime.h"
#include "utils/StringUtils.h"
#include "ServiceBroker.h"

#include <algorithm>

CCriticalSection ImageFactory::m_createSec;

using namespace ADDON;

IImage* ImageFactory::CreateLoader(const std::string& strFileName)
{
  GoTvUrl url(strFileName);
  return CreateLoader(url);
}

IImage* ImageFactory::CreateLoader(const GoTvUrl& url)
{
  if(!url.GetFileType().empty())
    return CreateLoaderFromMimeType("image/"+url.GetFileType());

  return CreateLoaderFromMimeType(CMime::GetMimeType(url));
}

IImage* ImageFactory::CreateLoaderFromMimeType(const std::string& strMimeType)
{
  BinaryAddonBaseList addonInfos;

  CServiceBroker::GetBinaryAddonManager().GetAddonInfos(addonInfos, true, ADDON_IMAGEDECODER);
  for (auto addonInfo : addonInfos)
  {
    std::vector<std::string> mime = StringUtils::Split(addonInfo->Type(ADDON_IMAGEDECODER)->GetValue("@mimetype").asString(), "|");
    if (std::find(mime.begin(), mime.end(), strMimeType) != mime.end())
    {
      CSingleLock lock(m_createSec);
      CImageDecoder* result = new CImageDecoder(addonInfo);
      result->Create(strMimeType);
      return result;
    }
  }

  return new CFFmpegImage(strMimeType);
}
