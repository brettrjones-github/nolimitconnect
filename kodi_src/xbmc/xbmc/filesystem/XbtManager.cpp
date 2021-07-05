/*
 *  Copyright (C) 2015-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "XbtManager.h"

#include <utility>

#include "guilib/XBTF.h"
#include "guilib/XBTFReader.h"
#include "GoTvUrl.h"

namespace XFILE
{

CXbtManager::CXbtManager() = default;

CXbtManager::~CXbtManager() = default;

CXbtManager& CXbtManager::GetInstance()
{
  static CXbtManager xbtManager;
  return xbtManager;
}

bool CXbtManager::HasFiles(const GoTvUrl& path) const
{
  return ProcessFile(path) != m_readers.end();
}

bool CXbtManager::GetFiles(const GoTvUrl& path, std::vector<CXBTFFile>& files) const
{
  const auto& reader = ProcessFile(path);
  if (reader == m_readers.end())
    return false;

  files = reader->second.reader->GetFiles();
  return true;
}

bool CXbtManager::GetReader(const GoTvUrl& path, CXBTFReaderPtr& reader) const
{
  const auto& it = ProcessFile(path);
  if (it == m_readers.end())
    return false;

  reader = it->second.reader;
  return true;
}

void CXbtManager::Release(const GoTvUrl& path)
{
  const auto& it = GetReader(path);
  if (it == m_readers.end())
    return;

  RemoveReader(it);
}

CXbtManager::XBTFReaders::iterator CXbtManager::GetReader(const GoTvUrl& path) const
{
  return GetReader(NormalizePath(path));
}

CXbtManager::XBTFReaders::iterator CXbtManager::GetReader(const std::string& path) const
{
  if (path.empty())
    return m_readers.end();

  return m_readers.find(path);
}

void CXbtManager::RemoveReader(XBTFReaders::iterator readerIterator) const
{
  if (readerIterator == m_readers.end())
    return;

  // close the reader
  readerIterator->second.reader->Close();

  // and remove it from the map
  m_readers.erase(readerIterator);
}

CXbtManager::XBTFReaders::const_iterator CXbtManager::ProcessFile(const GoTvUrl& path) const
{
  std::string filePath = NormalizePath(path);

  // check if the file is known
  auto it = GetReader(filePath);
  if (it != m_readers.end())
  {
    // check if the XBT file has been modified
    if (it->second.reader->GetLastModificationTimestamp() <= it->second.lastModification)
      return it;

    // the XBT file has been modified so close and remove it from the cache
    // it will be re-opened by the following logic
    RemoveReader(it);
  }

  // try to read the file
  CXBTFReaderPtr reader(new CXBTFReader());
  if (!reader->Open(filePath))
    return m_readers.end();

  XBTFReader xbtfReader = {
    reader,
    reader->GetLastModificationTimestamp()
  };
  std::pair<XBTFReaders::iterator, bool> result = m_readers.insert(std::make_pair(filePath, xbtfReader));
  return result.first;
}

std::string CXbtManager::NormalizePath(const GoTvUrl& path)
{
  if (path.IsProtocol("xbt"))
    return path.GetHostName();

  return path.Get();
}

}
