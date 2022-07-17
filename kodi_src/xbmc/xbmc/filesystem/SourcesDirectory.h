/*
 *  Copyright (C) 2005-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include <vector>

#include "IDirectory.h"

class CMediaSource;
typedef std::vector<CMediaSource> VECSOURCES;

namespace XFILE
{
  class CSourcesDirectory : public IDirectory
  {
  public:
    CSourcesDirectory(void);
    ~CSourcesDirectory(void) override;
    bool GetDirectory(const NlcUrl& url, CFileItemList &items) override;
    bool GetDirectory(const VECSOURCES &sources, CFileItemList &items);
    bool Exists(const NlcUrl& url) override;
    bool AllowAll() const override { return true; }
  };
}
