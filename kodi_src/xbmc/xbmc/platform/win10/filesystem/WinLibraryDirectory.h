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
    bool GetDirectory(const NlcUrl& url, CFileItemList &items) override;
    DIR_CACHE_TYPE GetCacheType(const NlcUrl& url) const override { return DIR_CACHE_ONCE; };
    bool Create(const NlcUrl& url) override;
    bool Exists(const NlcUrl& url) override;
    bool Remove(const NlcUrl& url) override;

    static bool GetStoragePath(std::string library, std::string& path);
    static bool IsValid(const NlcUrl& url);

  private:
    friend CWinLibraryFile;
    static winrt::Windows::Storage::StorageFolder GetRootFolder(const NlcUrl& url);
    static winrt::Windows::Storage::StorageFolder GetFolder(const NlcUrl& url);
    static int StatDirectory(const NlcUrl& url, struct __stat64* statData);
  };
}
