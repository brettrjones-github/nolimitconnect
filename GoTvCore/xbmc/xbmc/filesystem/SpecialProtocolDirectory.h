/*
 *  Copyright (C) 2005-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include "filesystem/OverrideDirectory.h"

namespace XFILE
{
  class CSpecialProtocolDirectory : public COverrideDirectory
  {
  public:
    CSpecialProtocolDirectory(void);
    ~CSpecialProtocolDirectory(void) override;
    bool GetDirectory(const GoTvUrl& url, CFileItemList &items) override;

  protected:
    std::string TranslatePath(const GoTvUrl &url) override;
  };
}
