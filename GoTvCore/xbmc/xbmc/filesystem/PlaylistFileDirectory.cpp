/*
 *  Copyright (C) 2005-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "PlaylistFileDirectory.h"
#include "playlists/PlayListFactory.h"
#include "File.h"
#include <GoTvCore/xbmc/xbmc/GoTvUrl.h>
#include "playlists/PlayList.h"

using namespace PLAYLIST;

namespace XFILE
{
  CPlaylistFileDirectory::CPlaylistFileDirectory() = default;

  CPlaylistFileDirectory::~CPlaylistFileDirectory() = default;

  bool CPlaylistFileDirectory::GetDirectory(const GoTvUrl& url, CFileItemList& items)
  {
    const std::string pathToUrl = url.Get();
    std::unique_ptr<CPlayList> pPlayList (CPlayListFactory::Create(pathToUrl));
    if ( NULL != pPlayList.get())
    {
      // load it
      if (!pPlayList->Load(pathToUrl))
        return false; //hmmm unable to load playlist?

      CPlayList playlist = *pPlayList;
      // convert playlist items to songs
      for (int i = 0; i < (int)playlist.size(); ++i)
      {
        CFileItemPtr item = playlist[i];
        item->m_iprogramCount = i;  // hack for playlist order
        items.Add(item);
      }
    }
    return true;
  }

  bool CPlaylistFileDirectory::ContainsFiles(const GoTvUrl& url)
  {
    const std::string pathToUrl = url.Get();
    std::unique_ptr<CPlayList> pPlayList (CPlayListFactory::Create(pathToUrl));
    if ( NULL != pPlayList.get())
    {
      // load it
      if (!pPlayList->Load(pathToUrl))
        return false; //hmmm unable to load playlist?

      return (pPlayList->size() > 1);
    }
    return false;
  }

  bool CPlaylistFileDirectory::Remove(const GoTvUrl& url)
  {
    return XFILE::CFile::Delete(url);
  }
}

