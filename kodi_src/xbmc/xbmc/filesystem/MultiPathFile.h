/*
 *  Copyright (C) 2005-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include "filesystem/OverrideFile.h"

namespace XFILE
{
  class CMultiPathFile : public COverrideFile
  {
  public:
    CMultiPathFile(void);
    ~CMultiPathFile(void) override;
    bool Open(const GoTvUrl& url) override;
    bool Exists(const GoTvUrl& url) override;
    int Stat(const GoTvUrl& url, struct __stat64* buffer) override;

  protected:
    std::string TranslatePath(const GoTvUrl &url) override;
  };
}
