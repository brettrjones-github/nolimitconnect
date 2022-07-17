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
    
    bool Open(const NlcUrl& url) override;
    bool OpenForWrite(const NlcUrl& url, bool bOverWrite = false) override;
    void Close() override;
    
    ssize_t Read(void* lpBuf, size_t uiBufSize) override;
    ssize_t Write(const void* lpBuf, size_t uiBufSize) override;
    int64_t Seek(int64_t iFilePosition, int iWhence = SEEK_SET) override;
    int Truncate(int64_t size) override;
    int64_t GetPosition() override;
    int64_t GetLength() override;
    void Flush() override;
    int IoControl(EIoControl request, void* param) override;
    
    bool Delete(const NlcUrl& url) override;
    bool Rename(const NlcUrl& url, const NlcUrl& urlnew) override;
    bool Exists(const NlcUrl& url) override;
    int Stat(const NlcUrl& url, struct __stat64* buffer) override;
    int Stat(struct __stat64* buffer) override;

  protected:
    int     m_fd = -1;
    int64_t m_filePos = -1;
    int64_t m_lastDropPos = -1;
    bool    m_allowWrite = false;
  };
  
}
