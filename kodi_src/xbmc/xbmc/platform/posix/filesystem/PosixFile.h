/*
 *  Copyright (C) 2014-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include "filesystem/IFile.h"

namespace XFILE
{
  
  class CPosixFile : public IFile
  {
  public:
    ~CPosixFile() override;
    
    bool Open(const GoTvUrl& url) override;
    bool OpenForWrite(const GoTvUrl& url, bool bOverWrite = false) override;
    void Close() override;
    
    ssize_t Read(void* lpBuf, size_t uiBufSize) override;
    ssize_t Write(const void* lpBuf, size_t uiBufSize) override;
    int64_t Seek(int64_t iFilePosition, int iWhence = SEEK_SET) override;
    int Truncate(int64_t size) override;
    int64_t GetPosition() override;
    int64_t GetLength() override;
    void Flush() override;
    int IoControl(EIoControl request, void* param) override;
    
    bool Delete(const GoTvUrl& url) override;
    bool Rename(const GoTvUrl& url, const GoTvUrl& urlnew) override;
    bool Exists(const GoTvUrl& url) override;
    int Stat(const GoTvUrl& url, struct __stat64* buffer) override;
    int Stat(struct __stat64* buffer) override;

  protected:
    int     m_fd = -1;
    int64_t m_filePos = -1;
    int64_t m_lastDropPos = -1;
    bool    m_allowWrite = false;
  };
  
}
