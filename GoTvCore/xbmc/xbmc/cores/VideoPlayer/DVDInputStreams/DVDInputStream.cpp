/*
 *  Copyright (C) 2005-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "DVDInputStream.h"
#include "GoTvUrl.h"

CDVDInputStream::CDVDInputStream(DVDStreamType streamType, const CFileItem& fileitem)
{
  m_streamType = streamType;
  m_contentLookup = true;
  m_realtime = fileitem.GetProperty("isrealtimestream").asBoolean(false);
  m_item = fileitem;
}

CDVDInputStream::~CDVDInputStream() = default;

bool CDVDInputStream::Open()
{
  m_content = m_item.GetMimeType();
  m_contentLookup = m_item.ContentLookup();
  return true;
}

void CDVDInputStream::Close()
{

}

std::string CDVDInputStream::GetFileName()
{
  GoTvUrl url(m_item.GetDynPath());

  url.SetProtocolOptions("");
  return url.Get();
}

GoTvUrl CDVDInputStream::GetURL()
{
  return m_item.GetDynURL();
}
