/*
 *  Copyright (C) 2011-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include "filesystem/IFile.h"
#include <winrt/Windows.Storage.AccessCache.h>

namespace XFILE
{
  class CWinLibraryFile : public IFile
  {
  public:
    CWinLibraryFile();
    virtual ~CWinLibraryFile(void);

    virtual bool Open(const GoTvUrl& url);
    virtual bool OpenForWrite(const GoTvUrl& url, bool bOverWrite = false);
    virtual void Close();

    virtual ssize_t Read(void* lpBuf, size_t uiBufSize);
    virtual ssize_t Write(const void* lpBuf, size_t uiBufSize);
    virtual int64_t Seek(int64_t iFilePosition, int iWhence = SEEK_SET);
    virtual int Truncate(int64_t toSize);
    virtual int64_t GetPosition();
    virtual int64_t GetLength();
    virtual void Flush();

    virtual bool Delete(const GoTvUrl& url);
    virtual bool Rename(const GoTvUrl& urlCurrentName, const GoTvUrl& urlNewName);
    virtual bool SetHidden(const GoTvUrl& url, bool hidden);
    virtual bool Exists(const GoTvUrl& url);
    virtual int Stat(const GoTvUrl& url, struct __stat64* statData);
    virtual int Stat(struct __stat64* statData);

    static IFile* Get(const GoTvUrl& url);
    static bool IsValid(const GoTvUrl& url);

    static bool IsInAccessList(const GoTvUrl& url);

  private:
    bool OpenIntenal(const GoTvUrl& url, winrt::Windows::Storage::FileAccessMode mode);
    winrt::Windows::Storage::StorageFile GetFile(const GoTvUrl& url);
    static bool IsInList(const GoTvUrl& url, const winrt::Windows::Storage::AccessCache::IStorageItemAccessList& list);
    static winrt::hstring GetTokenFromList(const GoTvUrl& url, const winrt::Windows::Storage::AccessCache::IStorageItemAccessList& list);
    static int Stat(const winrt::Windows::Storage::StorageFile& file, struct __stat64* statData);

    bool m_allowWrite = false;
    winrt::Windows::Storage::StorageFile m_sFile = nullptr;
    winrt::Windows::Storage::Streams::IRandomAccessStream m_fileStream = nullptr;
  };
}
