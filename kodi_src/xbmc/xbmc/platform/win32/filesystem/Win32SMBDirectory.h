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
  class CWin32SMBFile; // forward declaration

  class CWin32SMBDirectory : public IDirectory
  {
    friend class CWin32SMBFile;
  public:
    CWin32SMBDirectory(void);
    virtual ~CWin32SMBDirectory(void);
    virtual bool GetDirectory(const GoTvUrl& url, CFileItemList& items);
    virtual bool Create(const GoTvUrl& url);
    virtual bool Exists(const GoTvUrl& url);
    virtual bool Remove(const GoTvUrl& url);
  protected:
    bool RealCreate(const GoTvUrl& url, bool tryToConnect);
    bool RealExists(const GoTvUrl& url, bool tryToConnect);
    static bool GetNetworkResources(const GoTvUrl& basePath, CFileItemList& items);
    bool ConnectAndAuthenticate(GoTvUrl& url, bool allowPromptForCredential = false);
  };
}
