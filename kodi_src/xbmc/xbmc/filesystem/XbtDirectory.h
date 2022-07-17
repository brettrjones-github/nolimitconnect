/*
 *  Copyright (C) 2015-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include <map>
#include <string>

#include "IFileDirectory.h"

class CXBTFFile;

namespace XFILE
{
class CXbtDirectory : public IFileDirectory
{
public:
  CXbtDirectory();
  ~CXbtDirectory() override;

  // specialization of IDirectory
  DIR_CACHE_TYPE GetCacheType(const NlcUrl& url) const override { return DIR_CACHE_ALWAYS; };
  bool GetDirectory(const NlcUrl& url, CFileItemList& items) override;

  // specialization of IFileDirectory
  bool ContainsFiles(const NlcUrl& url) override;
};
}
