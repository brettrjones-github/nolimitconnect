/*
 *  Copyright (C) 2005-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "CodecFactory.h"
#include <GoTvCore/xbmc/xbmc/GoTvUrl.h>
#include "VideoPlayerCodec.h"
#include "utils/StringUtils.h"
#include "addons/AudioDecoder.h"
#include "addons/binary-addons/BinaryAddonBase.h"
#include "ServiceBroker.h"

using namespace ADDON;

ICodec* CodecFactory::CreateCodec(const std::string &strFileType)
{
  std::string fileType = strFileType;
  StringUtils::ToLower(fileType);

  BinaryAddonBaseList addonInfos;
  CServiceBroker::GetBinaryAddonManager().GetAddonInfos(addonInfos, true, ADDON_AUDIODECODER);
  for (const auto& addonInfo : addonInfos)
  {
    if (CAudioDecoder::GetExtensions(addonInfo).find("."+fileType) != std::string::npos)
    {
      CAudioDecoder* result = new CAudioDecoder(addonInfo);
      if (!result->CreateDecoder())
      {
        delete result;
        return nullptr;
      }
      return result;
    }
  }

  VideoPlayerCodec *dvdcodec = new VideoPlayerCodec();
  return dvdcodec;
}

ICodec* CodecFactory::CreateCodecDemux(const CFileItem& file, unsigned int filecache)
{
  GoTvUrl urlFile(file.GetDynPath());
  std::string content = file.GetMimeType();
  StringUtils::ToLower(content);
  if (!content.empty())
  {
    BinaryAddonBaseList addonInfos;
    CServiceBroker::GetBinaryAddonManager().GetAddonInfos(addonInfos, true, ADDON_AUDIODECODER);
    for (const auto& addonInfo : addonInfos)
    {
      if (CAudioDecoder::GetMimetypes(addonInfo).find(content) != std::string::npos)
      {
        CAudioDecoder* result = new CAudioDecoder(addonInfo);
        if (!result->CreateDecoder())
        {
          delete result;
          return nullptr;
        }
        return result;
      }
    }
  }

  if( content == "audio/mpeg"       ||
      content == "audio/mpeg3"      ||
      content == "audio/mp3"        ||
      content == "audio/aac"        ||
      content == "audio/aacp"       ||
      content == "audio/x-ms-wma"   ||
      content == "audio/x-ape"      ||
      content == "audio/ape"        ||
      content == "application/ogg"  ||
      content == "audio/ogg"        ||
      content == "audio/x-xbmc-pcm" ||
      content == "audio/flac"       || 
      content == "audio/x-flac"     || 
      content == "application/x-flac"
      )
  {
    VideoPlayerCodec *dvdcodec = new VideoPlayerCodec();
    dvdcodec->SetContentType(content);
    return dvdcodec;
  }
  else if (urlFile.IsProtocol("shout"))
  {
    VideoPlayerCodec *dvdcodec = new VideoPlayerCodec();
    dvdcodec->SetContentType("audio/mp3");
    return dvdcodec; // if we got this far with internet radio - content-type was wrong. gamble on mp3.
  }
  else if (urlFile.IsFileType("wav") ||
      content == "audio/wav" ||
      content == "audio/x-wav")
  {
    VideoPlayerCodec *dvdcodec = new VideoPlayerCodec();
    dvdcodec->SetContentType("audio/x-spdif-compressed");
    if (dvdcodec->Init(file, filecache))
    {
      return dvdcodec;
    }

    dvdcodec = new VideoPlayerCodec();
    dvdcodec->SetContentType(content);
    return dvdcodec;
  }
  else
    return CreateCodec(urlFile.GetFileType());
}

