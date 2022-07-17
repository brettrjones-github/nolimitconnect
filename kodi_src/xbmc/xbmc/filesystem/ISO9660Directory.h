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
class CISO9660Directory :
      public IDirectory
{
public:
  CISO9660Directory(void);
  ~CISO9660Directory(void) override;
  bool GetDirectory(const NlcUrl& url, CFileItemList &items) override;
  bool Exists(const NlcUrl& url) override;
};
}
