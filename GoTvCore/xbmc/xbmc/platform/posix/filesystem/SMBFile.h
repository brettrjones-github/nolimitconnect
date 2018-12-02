/*
 *  Copyright (C) 2005-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

// SMBFile.h: interface for the CSMBFile class.

//

//////////////////////////////////////////////////////////////////////


#include "filesystem/IFile.h"
#include "GoTvUrl.h"
#include "threads/CriticalSection.h"

#define NT_STATUS_CONNECTION_REFUSED long(0xC0000000 | 0x0236)
#define NT_STATUS_INVALID_HANDLE long(0xC0000000 | 0x0008)
#define NT_STATUS_ACCESS_DENIED long(0xC0000000 | 0x0022)
#define NT_STATUS_OBJECT_NAME_NOT_FOUND long(0xC0000000 | 0x0034)
#define NT_STATUS_INVALID_COMPUTER_NAME long(0xC0000000 | 0x0122)

struct _SMBCCTX;
typedef _SMBCCTX SMBCCTX;

class CSMB : public CCriticalSection
{
public:
  CSMB();
  ~CSMB();
  void Init();
  void Deinit();
  void CheckIfIdle();
  void SetActivityTime();
  void AddActiveConnection();
  void AddIdleConnection();
  std::string URLEncode(const std::string &value);
  std::string URLEncode(const GoTvUrl& url);

  DWORD ConvertUnixToNT(int error);
private:
  SMBCCTX *m_context;
#ifdef TARGET_POSIX
  int m_OpenConnections;
  unsigned int m_IdleTimeout;
  static bool IsFirstInit;
#endif
};

extern CSMB smb;

namespace XFILE
{
class CSMBFile : public IFile
{
public:
  CSMBFile();
  int OpenFile(const GoTvUrl& url, std::string& strAuth);
  ~CSMBFile() override;
  void Close() override;
  int64_t Seek(int64_t iFilePosition, int iWhence = SEEK_SET) override;
  ssize_t Read(void* lpBuf, size_t uiBufSize) override;
  bool Open(const GoTvUrl& url) override;
  bool Exists(const GoTvUrl& url) override;
  int Stat(const GoTvUrl& url, struct __stat64* buffer) override;
  int Stat(struct __stat64* buffer) override;
  int Truncate(int64_t size) override;
  int64_t GetLength() override;
  int64_t GetPosition() override;
  ssize_t Write(const void* lpBuf, size_t uiBufSize) override;

  bool OpenForWrite(const GoTvUrl& url, bool bOverWrite = false) override;
  bool Delete(const GoTvUrl& url) override;
  bool Rename(const GoTvUrl& url, const GoTvUrl& urlnew) override;
  int GetChunkSize() override { return 2048*1024; }
  int IoControl(EIoControl request, void* param) override;

protected:
  CURL m_url;
  bool IsValidFile(const std::string& strFileName);
  std::string GetAuthenticatedPath(const GoTvUrl& url);
  int64_t m_fileSize;
  int m_fd;
  bool m_allowRetry;
};
}
