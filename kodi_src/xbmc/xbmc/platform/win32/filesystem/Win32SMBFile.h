/*
 *  Copyright (C) 2014-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
*
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
*/

#pragma once

#include "Win32File.h"

namespace XFILE
{
  class CWin32SMBFile : public CWin32File
  {
  public:
    CWin32SMBFile();
    virtual ~CWin32SMBFile();
    virtual bool Open(const NlcUrl& url);
    virtual bool OpenForWrite(const NlcUrl& url, bool bOverWrite = false);

    virtual bool Delete(const NlcUrl& url);
    virtual bool Rename(const NlcUrl& urlCurrentName, const NlcUrl& urlNewName);
    virtual bool SetHidden(const NlcUrl& url, bool hidden);
    virtual bool Exists(const NlcUrl& url);
    virtual int Stat(const NlcUrl& url, struct __stat64* statData);
  private:
    static bool ConnectAndAuthenticate(const NlcUrl& url);
  };

}
