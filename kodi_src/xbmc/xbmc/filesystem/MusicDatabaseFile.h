/*
 *  Copyright (C) 2005-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include "IFile.h"
#include "File.h"

namespace XFILE
{
class CMusicDatabaseFile : public IFile
{
public:
  CMusicDatabaseFile(void);
  ~CMusicDatabaseFile(void) override;
  bool Open(const NlcUrl& url) override;
  bool Exists(const NlcUrl& url) override;
  int Stat(const NlcUrl& url, struct __stat64* buffer) override;

  ssize_t Read(void* lpBuf, size_t uiBufSize) override;
  int64_t Seek(int64_t iFilePosition, int iWhence = SEEK_SET) override;
  void Close() override;
  int64_t GetPosition() override;
  int64_t GetLength() override;

  static std::string TranslateUrl(const NlcUrl& url);
protected:
  CFile m_file;
};
}
