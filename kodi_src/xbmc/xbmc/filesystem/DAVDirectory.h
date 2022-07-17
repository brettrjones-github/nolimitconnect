/*
 *  Copyright (C) 2005-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include "IDirectory.h"
#include "utils/XBMCTinyXML.h"
#include "FileItem.h"

namespace XFILE
{
  class CDAVDirectory : public IDirectory
  {
    public:
      CDAVDirectory(void);
      ~CDAVDirectory(void) override;
      bool GetDirectory(const NlcUrl& url, CFileItemList &items) override;
      bool Create(const NlcUrl& url) override;
      bool Exists(const NlcUrl& url) override;
      bool Remove(const NlcUrl& url) override;
      DIR_CACHE_TYPE GetCacheType(const NlcUrl& url) const override { return DIR_CACHE_ONCE; };
    private:
      void ParseResponse(const TiXmlElement *pElement, CFileItem &item);
  };
}
