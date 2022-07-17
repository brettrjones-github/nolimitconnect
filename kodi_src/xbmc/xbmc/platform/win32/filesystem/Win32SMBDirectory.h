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
    virtual bool GetDirectory(const NlcUrl& url, CFileItemList& items);
    virtual bool Create(const NlcUrl& url);
    virtual bool Exists(const NlcUrl& url);
    virtual bool Remove(const NlcUrl& url);
  protected:
    bool RealCreate(const NlcUrl& url, bool tryToConnect);
    bool RealExists(const NlcUrl& url, bool tryToConnect);
    static bool GetNetworkResources(const NlcUrl& basePath, CFileItemList& items);
    bool ConnectAndAuthenticate(NlcUrl& url, bool allowPromptForCredential = false);
  };
}
