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

  bool Open(const GoTvUrl& url) override;
  bool Exists(const GoTvUrl& url) override;
  int Stat(const GoTvUrl& url, struct __stat64* buffer) override;
  int Stat(struct __stat64* buffer) override;
  bool OpenForWrite(const GoTvUrl& url, bool bOverWrite = false) override;
  bool Delete(const GoTvUrl& url) override;
  bool Rename(const GoTvUrl& url, const GoTvUrl& urlnew) override;

  ssize_t Read(void* lpBuf, size_t uiBufSize) override;
  ssize_t Write(const void* lpBuf, size_t uiBufSize) override;
  int64_t Seek(int64_t iFilePosition, int iWhence = SEEK_SET) override;
  void Close() override;
  int64_t GetPosition() override;
  int64_t GetLength() override;

protected:
  virtual std::string TranslatePath(const GoTvUrl &url) = 0;

  CFile m_file;
  bool m_writable;
};
}
