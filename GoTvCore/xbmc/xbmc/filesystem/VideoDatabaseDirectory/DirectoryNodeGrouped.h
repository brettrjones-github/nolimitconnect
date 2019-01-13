/*
 *  Copyright (C) 2005-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include "DirectoryNodeVideo.h"

namespace XFILE
{
  namespace VIDEODATABASEDIRECTORY
  {
    class CDirectoryNodeGrouped : public CDirectoryNode
    {
    public:
      CDirectoryNodeGrouped(NODE_TYPE type, const std::string& strName, CDirectoryNode* pParent);
    protected:
      NODE_TYPE GetChildType() const override;
      bool GetContent(CFileItemList& items) const override;
      std::string GetLocalizedName() const override;

    private:
      std::string GetContentType() const;
      std::string GetContentType(const CQueryParams &params) const;
    };
  }
}


