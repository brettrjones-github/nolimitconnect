/*
 *  Copyright (C) 2014-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include "filesystem/File.h"
#include "filesystem/IFile.h"

namespace XFILE
{
class COverrideFile : public IFile
{
public:
  explicit COverrideFile(bool writable);
  ~COverrideFile() override;

  bool Open(const NlcUrl& url) override;
  bool Exists(const NlcUrl& url) override;
  int Stat(const NlcUrl& url, struct __stat64* buffer) override;
  int Stat(struct __stat64* buffer) override;
  bool OpenForWrite(const NlcUrl& url, bool bOverWrite = false) override;
  bool Delete(const NlcUrl& url) override;
  bool Rename(const NlcUrl& url, const NlcUrl& urlnew) override;

  ssize_t Read(void* lpBuf, size_t uiBufSize) override;
  ssize_t Write(const void* lpBuf, size_t uiBufSize) override;
  int64_t Seek(int64_t iFilePosition, int iWhence = SEEK_SET) override;
  void Close() override;
  int64_t GetPosition() override;
  int64_t GetLength() override;

protected:
  virtual std::string TranslatePath(const NlcUrl &url) = 0;

  CFile m_file;
  bool m_writable;
};
}
