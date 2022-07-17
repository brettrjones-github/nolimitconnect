/*
 *      Copyright (C) 2010 Team Boxee
 *      http://www.boxee.tv
 *
 *  Copyright (C) 2010-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include "IFile.h"
#include "udf25.h"

namespace XFILE
{

class CUDFFile : public IFile
{
public:
  CUDFFile();
  ~CUDFFile() override;
  int64_t GetPosition() override;
  int64_t GetLength() override;
  bool Open(const NlcUrl& url) override;
  bool Exists(const NlcUrl& url) override;
  int Stat(const NlcUrl& url, struct __stat64* buffer) override;
  ssize_t Read(void* lpBuf, size_t uiBufSize) override;
  int64_t Seek(int64_t iFilePosition, int iWhence = SEEK_SET) override;
  void Close() override;
protected:
  bool m_bOpened = false;
  HANDLE m_hFile;
  udf25 m_udfIsoReaderLocal;
};
}

