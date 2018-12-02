/*
 *  Copyright (C) 2005-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include "CurlFile.h"

namespace XFILE
{
  class CDAVFile : public CCurlFile
  {
  public:
    CDAVFile(void);
    ~CDAVFile(void) override;

    virtual bool Execute(const GoTvUrl& url);

    bool Delete(const GoTvUrl& url) override;
    bool Rename(const GoTvUrl& url, const GoTvUrl& urlnew) override;

    virtual int GetLastResponseCode() { return m_lastResponseCode; }

  private:
    int m_lastResponseCode = 0;
  };
}
