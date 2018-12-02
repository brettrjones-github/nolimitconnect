/*
 *  Copyright (C) 2005-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "CDDADirectory.h"
#include <storage/cdioSupport.h>
#include "music/MusicDatabase.h"
#include "FileItem.h"
#include "File.h"
#include "storage/MediaManager.h"
#include "utils/StringUtils.h"

using namespace XFILE;
using namespace MEDIA_DETECT;

CCDDADirectory::CCDDADirectory(void) = default;

CCDDADirectory::~CCDDADirectory(void) = default;


bool CCDDADirectory::GetDirectory(const GoTvUrl& url, CFileItemList &items)
{
#ifndef HAS_DVD_DRIVE
    return false;
#else
  // Reads the tracks from an audio cd
  std::string strPath = url.Get();

  if (!g_mediaManager.IsDiscInDrive(strPath))
    return false;

  // Get information for the inserted disc
  CCdInfo* pCdInfo = g_mediaManager.GetCdInfo(strPath);
  if (pCdInfo == NULL)
    return false;

  //  Preload CDDB info
  CMusicDatabase musicdatabase;
  musicdatabase.LookupCDDBInfo();

  // If the disc has no tracks, we are finished here.
  int nTracks = pCdInfo->GetTrackCount();
  if (nTracks <= 0)
    return false;

  // Generate fileitems
  for (int i = 1;i <= nTracks;++i)
  {
    // Skip Datatracks for display,
    // but needed to query cddb
    if (!pCdInfo->IsAudio(i))
      continue;

    // Format standard cdda item label
    std::string strLabel = StringUtils::Format("Track %2.2i", i);

    CFileItemPtr pItem(new CFileItem(strLabel));
    pItem->m_bIsFolder = false;
    std::string path = StringUtils::Format("cdda://local/%2.2i.cdda", i);
    pItem->SetPath(path);

    struct __stat64 s64;
    if (CFile::Stat(pItem->GetPath(), &s64) == 0)
      pItem->m_dwSize = s64.st_size;

    items.Add(pItem);
  }
  return true;
#endif // HAS_DVD_DRIVE

}
