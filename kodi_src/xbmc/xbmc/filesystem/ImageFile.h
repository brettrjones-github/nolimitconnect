/*
 *  Copyright (C) 2012-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include "File.h"
#include "IFile.h"

namespace XFILE
{
  class CImageFile: public IFile
  {
  public:
    CImageFile();
    ~CImageFile() override;
    bool Open(const NlcUrl& url) override;
    bool Exists(const NlcUrl& url) override;
    int Stat(const NlcUrl& url, struct __stat64* buffer) override;

    ssize_t Read(void* lpBuf, size_t uiBufSize) override;
    int64_t Seek(int64_t iFilePosition, int iWhence = SEEK_SET) override;
    void Close() override;
    int64_t GetPosition() override;
    int64_t GetLength() override;

  protected:
    CFile m_file;
  };
}
