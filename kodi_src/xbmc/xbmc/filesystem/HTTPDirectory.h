/*
 *  Copyright (C) 2005-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include "IDirectory.h"

namespace XFILE
{
  class CHTTPDirectory : public IDirectory
  {
    public:
      CHTTPDirectory(void);
      ~CHTTPDirectory(void) override;
      bool GetDirectory(const GoTvUrl& url, CFileItemList &items) override;
      bool Exists(const GoTvUrl& url) override;
      DIR_CACHE_TYPE GetCacheType(const GoTvUrl& url) const override { return DIR_CACHE_ONCE; };
    private:
  };
}
