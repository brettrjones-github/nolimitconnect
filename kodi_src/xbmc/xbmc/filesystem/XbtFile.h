/*
 *  Copyright (C) 2015-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include <vector>

#include "IFile.h"
#include "NlcUrl.h"
#include "guilib/XBTF.h"
#include "guilib/XBTFReader.h"

namespace XFILE
{
class CXbtFile : public IFile
{
public:
  CXbtFile();
  ~CXbtFile() override;

  bool Open(const NlcUrl& url) override;
  void Close() override;
  bool Exists(const NlcUrl& url) override;

  int64_t GetPosition() override;
  int64_t GetLength() override;

  int Stat(struct __stat64* buffer) override;
  int Stat(const NlcUrl& url, struct __stat64* buffer) override;

  ssize_t Read(void* lpBuf, size_t uiBufSize) override;
  int64_t Seek(int64_t iFilePosition, int iWhence = SEEK_SET) override;

  uint32_t GetImageWidth() const;
  uint32_t GetImageHeight() const;
  uint32_t GetImageFormat() const;
  bool HasImageAlpha() const;

private:
  bool GetFirstFrame(CXBTFFrame& frame) const;

  static bool GetReader(const NlcUrl& url, CXBTFReaderPtr& reader);
  static bool GetReaderAndFile(const NlcUrl& url, CXBTFReaderPtr& reader, CXBTFFile& file);
  static bool GetFile(const NlcUrl& url, CXBTFFile& file);

  NlcUrl m_url;
  bool m_open = false;
  CXBTFReaderPtr m_xbtfReader;
  CXBTFFile m_xbtfFile;

  std::vector<uint64_t> m_frameStartPositions;
  size_t m_frameIndex = 0;
  uint64_t m_positionWithinFrame = 0;
  int64_t m_positionTotal = 0;

  std::vector<uint8_t*> m_unpackedFrames;
};
}
