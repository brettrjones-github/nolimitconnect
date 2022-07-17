/*
 *      Copyright (C) 2010 Team Boxee
 *      http://www.boxee.tv
 *
 *  Copyright (C) 2010-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include "IFileDirectory.h"

namespace XFILE
{
class CUDFDirectory :
      public IFileDirectory
{
public:
  CUDFDirectory(void);
  ~CUDFDirectory(void) override;
  bool GetDirectory(const NlcUrl& url, CFileItemList &items) override;
  bool Exists(const NlcUrl& url) override;
  bool ContainsFiles(const NlcUrl& url) override { return true; }
};
}

