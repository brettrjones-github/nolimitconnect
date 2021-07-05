/*
 *  Copyright (C) 2005-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include "filesystem/IDirectory.h"
#include "SMBFile.h"
#include "MediaSource.h"

namespace XFILE
{
class CSMBDirectory : public IDirectory
{
public:
  CSMBDirectory(void);
  ~CSMBDirectory(void) override;
  bool GetDirectory(const GoTvUrl& url, CFileItemList &items) override;
  DIR_CACHE_TYPE GetCacheType(const GoTvUrl& url) const override { return DIR_CACHE_ONCE; };
  bool Create(const GoTvUrl& url) override;
  bool Exists(const GoTvUrl& url) override;
  bool Remove(const GoTvUrl& url) override;

  int Open(const GoTvUrl& url);

private:
  int OpenDir(const GoTvUrl& url, std::string& strAuth);
};
}
