/*
 *  Copyright (C) 2012-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include "filesystem/IFileDirectory.h"

namespace XFILE
{
  class CAPKDirectory : public IFileDirectory
  {
    public:
    CAPKDirectory() {}
    virtual ~CAPKDirectory() {}
    virtual bool GetDirectory(const NlcUrl& url, CFileItemList &items);
    virtual bool ContainsFiles(const NlcUrl& url);
    virtual DIR_CACHE_TYPE GetCacheType(const NlcUrl& url) const;
    virtual bool Exists(const NlcUrl& url);
  };
}
