#pragma once
/*
 *      Copyright (C) 2005-2013 Team XBMC
 *      http://kodi.tv
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with XBMC; see the file COPYING.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 */

#include "IFile.h"
#include "FileItem.h"
#include "threads/CriticalSection.h"

#include <libssh/libssh.h>
#ifdef TARGET_WINDOWS
#define ssize_t SSIZE_T
#include <libssh/sftp.h>
#undef ssize_t
#else  // !TARGET_WINDOWS
#include <libssh/sftp.h>
#endif // !TARGET_WINDOWS
#include <string>
#include <map>
#include <memory>

class GoTvUrl;

#if LIBSSH_VERSION_INT < SSH_VERSION_INT(0,3,2)
#define ssh_session SSH_SESSION
#endif

#if LIBSSH_VERSION_INT < SSH_VERSION_INT(0,4,0)
#define sftp_file SFTP_FILE*
#define sftp_session SFTP_SESSION*
#define sftp_attributes SFTP_ATTRIBUTES*
#define sftp_dir SFTP_DIR*
#define ssh_session ssh_session*
#endif

//five secs timeout for SFTP
#define SFTP_TIMEOUT 5

class CSFTPSession
{
public:
  CSFTPSession(const std::string &host, unsigned int port, const std::string &username, const std::string &password);
  virtual ~CSFTPSession();

  sftp_file CreateFileHandle(const std::string &file);
  void CloseFileHandle(sftp_file handle);
  bool GetDirectory(const std::string &base, const std::string &folder, CFileItemList &items);
  bool DirectoryExists(const char *path);
  bool FileExists(const char *path);
  int Stat(const char *path, struct __stat64* buffer);
  int Seek(sftp_file handle, uint64_t position);
  int Read(sftp_file handle, void *buffer, size_t length);
  int64_t GetPosition(sftp_file handle);
  bool IsIdle();
private:
  bool VerifyKnownHost(ssh_session session);
  bool Connect(const std::string &host, unsigned int port, const std::string &username, const std::string &password);
  void Disconnect();
  bool GetItemPermissions(const char *path, uint32_t &permissions);
  CCriticalSection m_critSect;

  bool m_connected;
  ssh_session  m_session;
  sftp_session m_sftp_session;
  int m_LastActive;
};

typedef std::shared_ptr<CSFTPSession> CSFTPSessionPtr;

class CSFTPSessionManager
{
public:
  static CSFTPSessionPtr CreateSession(const GoTvUrl &url);
  static CSFTPSessionPtr CreateSession(const std::string &host, unsigned int port, const std::string &username, const std::string &password);
  static void ClearOutIdleSessions();
  static void DisconnectAllSessions();
private:
  static CCriticalSection m_critSect;
  static std::map<std::string, CSFTPSessionPtr> sessions;
};

namespace XFILE
{
  class CSFTPFile : public IFile
  {
  public:
    CSFTPFile();
    ~CSFTPFile() override;
    void Close() override;
    int64_t Seek(int64_t iFilePosition, int iWhence = SEEK_SET) override;
    ssize_t Read(void* lpBuf, size_t uiBufSize) override;
    bool Open(const GoTvUrl& url) override;
    bool Exists(const GoTvUrl& url) override;
    int Stat(const GoTvUrl& url, struct __stat64* buffer) override;
    int Stat(struct __stat64* buffer) override;
    int64_t GetLength() override;
    int64_t GetPosition() override;
    int GetChunkSize() override {return 1;};
    int IoControl(EIoControl request, void* param) override;
  private:
    std::string m_file;
    CSFTPSessionPtr m_session;
    sftp_file m_sftp_handle;
  };
}
