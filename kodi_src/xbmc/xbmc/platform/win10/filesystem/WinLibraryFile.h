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

    virtual bool Open(const NlcUrl& url);
    virtual bool OpenForWrite(const NlcUrl& url, bool bOverWrite = false);
    virtual void Close();

    virtual ssize_t Read(void* lpBuf, size_t uiBufSize);
    virtual ssize_t Write(const void* lpBuf, size_t uiBufSize);
    virtual int64_t Seek(int64_t iFilePosition, int iWhence = SEEK_SET);
    virtual int Truncate(int64_t toSize);
    virtual int64_t GetPosition();
    virtual int64_t GetLength();
    virtual void Flush();

    virtual bool Delete(const NlcUrl& url);
    virtual bool Rename(const NlcUrl& urlCurrentName, const NlcUrl& urlNewName);
    virtual bool SetHidden(const NlcUrl& url, bool hidden);
    virtual bool Exists(const NlcUrl& url);
    virtual int Stat(const NlcUrl& url, struct __stat64* statData);
    virtual int Stat(struct __stat64* statData);

    static IFile* Get(const NlcUrl& url);
    static bool IsValid(const NlcUrl& url);

    static bool IsInAccessList(const NlcUrl& url);

  private:
    bool OpenIntenal(const NlcUrl& url, winrt::Windows::Storage::FileAccessMode mode);
    winrt::Windows::Storage::StorageFile GetFile(const NlcUrl& url);
    static bool IsInList(const NlcUrl& url, const winrt::Windows::Storage::AccessCache::IStorageItemAccessList& list);
    static winrt::hstring GetTokenFromList(const NlcUrl& url, const winrt::Windows::Storage::AccessCache::IStorageItemAccessList& list);
    static int Stat(const winrt::Windows::Storage::StorageFile& file, struct __stat64* statData);

    bool m_allowWrite = false;
    winrt::Windows::Storage::StorageFile m_sFile = nullptr;
    winrt::Windows::Storage::Streams::IRandomAccessStream m_fileStream = nullptr;
  };
}
