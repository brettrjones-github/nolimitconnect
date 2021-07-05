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
    virtual bool Open(const GoTvUrl& url);
    virtual bool OpenForWrite(const GoTvUrl& url, bool bOverWrite = false);

    virtual bool Delete(const GoTvUrl& url);
    virtual bool Rename(const GoTvUrl& urlCurrentName, const GoTvUrl& urlNewName);
    virtual bool SetHidden(const GoTvUrl& url, bool hidden);
    virtual bool Exists(const GoTvUrl& url);
    virtual int Stat(const GoTvUrl& url, struct __stat64* statData);
  private:
    static bool ConnectAndAuthenticate(const GoTvUrl& url);
  };

}
