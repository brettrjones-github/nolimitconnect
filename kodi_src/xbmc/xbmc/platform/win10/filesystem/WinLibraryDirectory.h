/*
 *  Copyright (C) 2011-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include "filesystem/IDirectory.h"

namespace XFILE
{
  class CWinLibraryFile;

  class CWinLibraryDirectory : public IDirectory
  {
  public:
    CWinLibraryDirectory();
    virtual ~CWinLibraryDirectory(void);
    bool GetDirectory(const GoTvUrl& url, CFileItemList &items) override;
    DIR_CACHE_TYPE GetCacheType(const GoTvUrl& url) const override { return DIR_CACHE_ONCE; };
    bool Create(const GoTvUrl& url) override;
    bool Exists(const GoTvUrl& url) override;
    bool Remove(const GoTvUrl& url) override;

    static bool GetStoragePath(std::string library, std::string& path);
    static bool IsValid(const GoTvUrl& url);

  private:
    friend CWinLibraryFile;
    static winrt::Windows::Storage::StorageFolder GetRootFolder(const GoTvUrl& url);
    static winrt::Windows::Storage::StorageFolder GetFolder(const GoTvUrl& url);
    static int StatDirectory(const GoTvUrl& url, struct __stat64* statData);
  };
}
