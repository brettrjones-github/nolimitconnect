/*
 *  Copyright (C) 2014-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include <sys/stat.h>

#include "OverrideFile.h"
#include <GoTvCore/xbmc/xbmc/GoTvUrl.h>

using namespace XFILE;


COverrideFile::COverrideFile(bool writable)
  : m_writable(writable)
{ }


COverrideFile::~COverrideFile()
{
  Close();
}

bool COverrideFile::Open(const GoTvUrl& url)
{
  std::string strFileName = TranslatePath(url);

  return m_file.Open(strFileName);
}

bool COverrideFile::OpenForWrite(const GoTvUrl& url, bool bOverWrite /* = false */)
{
  if (!m_writable)
    return false;

  std::string strFileName = TranslatePath(url);

  return m_file.OpenForWrite(strFileName, bOverWrite);
}

bool COverrideFile::Delete(const GoTvUrl& url)
{
  if (!m_writable)
    return false;

  std::string strFileName = TranslatePath(url);

  return m_file.Delete(strFileName);
}

bool COverrideFile::Exists(const GoTvUrl& url)
{
  std::string strFileName = TranslatePath(url);

  return m_file.Exists(strFileName);
}

int COverrideFile::Stat(const GoTvUrl& url, struct __stat64* buffer)
{
  std::string strFileName = TranslatePath(url);

  return m_file.Stat(strFileName, buffer);
}

bool COverrideFile::Rename(const GoTvUrl& url, const GoTvUrl& urlnew)
{
  if (!m_writable)
    return false;

  std::string strFileName = TranslatePath(url);
  std::string strFileName2 = TranslatePath(urlnew);

  return m_file.Rename(strFileName, strFileName2);
}

int COverrideFile::Stat(struct __stat64* buffer)
{
  return m_file.Stat(buffer);
}

ssize_t COverrideFile::Read(void* lpBuf, size_t uiBufSize)
{
  return m_file.Read(lpBuf, uiBufSize);
}

ssize_t COverrideFile::Write(const void* lpBuf, size_t uiBufSize)
{
  if (!m_writable)
    return -1;

  return m_file.Write(lpBuf, uiBufSize);
}

int64_t COverrideFile::Seek(int64_t iFilePosition, int iWhence /*=SEEK_SET*/)
{
  return m_file.Seek(iFilePosition, iWhence);
}

void COverrideFile::Close()
{
  m_file.Close();
}

int64_t COverrideFile::GetPosition()
{
  return m_file.GetPosition();
}

int64_t COverrideFile::GetLength()
{
  return m_file.GetLength();
}
