/*
 *  Copyright (C) 2015-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "guilib/XBTFReader.h"

class GoTvUrl;
class CXBTFFile;

namespace XFILE
{
class CXbtManager
{
public:
  ~CXbtManager();

  static CXbtManager& GetInstance();

  bool HasFiles(const GoTvUrl& path) const;
  bool GetFiles(const GoTvUrl& path, std::vector<CXBTFFile>& files) const;

  bool GetReader(const GoTvUrl& path, CXBTFReaderPtr& reader) const;

  void Release(const GoTvUrl& path);

private:
  CXbtManager();
  CXbtManager(const CXbtManager&) = delete;
  CXbtManager& operator=(const CXbtManager&) = delete;

  struct XBTFReader
  {
    CXBTFReaderPtr reader;
    time_t lastModification;
  };
  using XBTFReaders = std::map<std::string, XBTFReader>;

  XBTFReaders::iterator GetReader(const GoTvUrl& path) const;
  XBTFReaders::iterator GetReader(const std::string& path) const;
  void RemoveReader(XBTFReaders::iterator readerIterator) const;
  XBTFReaders::const_iterator ProcessFile(const GoTvUrl& path) const;

  static std::string NormalizePath(const GoTvUrl& path);

  mutable XBTFReaders m_readers;
};
}
