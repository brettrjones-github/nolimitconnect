/*
 *  Copyright (C) 2011-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include "IDirectory.h"
#include "NFSFile.h"

struct nfsdirent;

namespace XFILE
{
  class CNFSDirectory : public IDirectory
  {
    public:
      CNFSDirectory(void);
      ~CNFSDirectory(void) override;
      bool GetDirectory(const NlcUrl& url, CFileItemList &items) override;
      DIR_CACHE_TYPE GetCacheType(const NlcUrl& url) const override { return DIR_CACHE_ONCE; };
      bool Create(const NlcUrl& url) override;
      bool Exists(const NlcUrl& url) override;
      bool Remove(const NlcUrl& url) override;
    private:
      bool GetServerList(CFileItemList &items);
      bool GetDirectoryFromExportList(const std::string& strPath, CFileItemList &items);
      bool ResolveSymlink( const std::string &dirName, struct nfsdirent *dirent, NlcUrl &resolvedUrl);
  };
}

