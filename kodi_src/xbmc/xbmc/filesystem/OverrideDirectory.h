/*
 *  Copyright (C) 2014-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include "filesystem/IDirectory.h"

namespace XFILE
{
class COverrideDirectory : public IDirectory
{
public:
  COverrideDirectory();
  ~COverrideDirectory() override;

  bool Create(const GoTvUrl& url) override;
  bool Exists(const GoTvUrl& url) override;
  bool Remove(const GoTvUrl& url) override;

protected:
  virtual std::string TranslatePath(const GoTvUrl &url) = 0;
};
}
