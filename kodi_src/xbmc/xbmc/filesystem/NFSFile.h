/*
 *  Copyright (C) 2011-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

// FileNFS.h: interface for the CNFSFile class.

#include "IFile.h"
#include "GoTvUrl.h"
#include "threads/CriticalSection.h"
#include <list>
#include <map>
#include "DllLibNfs.h" // for define NFSSTAT

#ifdef TARGET_WINDOWS
# ifndef S_IRGRP
#define S_IRGRP 0
#define S_IROTH 0
#define S_IWUSR _S_IWRITE
#define S_IRUSR _S_IREAD
#define	S_IFLNK 0120000

#define S_ISBLK(m) (0)
#define S_ISSOCK(m) (0)
#define S_ISLNK(m) ((m & S_IFLNK) != 0)
#define S_ISCHR(m) ((m & _S_IFCHR) != 0)
#define S_ISDIR(m) ((m & _S_IFDIR) != 0)
#define S_ISFIFO(m) ((m & _S_IFIFO) != 0)
#define S_ISREG(m) ((m & _S_IFREG) != 0)
# endif // S_IRGRP
#endif

class DllLibNfs;

class CNfsConnection : public CCriticalSection
{     
public:
  struct keepAliveStruct
  {
    std::string exportPath;
    uint64_t refreshCounter;
  };
  typedef std::map<struct nfsfh  *, struct keepAliveStruct> tFileKeepAliveMap;  

  struct contextTimeout
  {
    struct nfs_context *pContext;
    uint64_t lastAccessedTime;
  };

  typedef std::map<std::string, struct contextTimeout> tOpenContextMap;    
  
  CNfsConnection();
  ~CNfsConnection();
  bool Connect(const GoTvUrl &url, std::string &relativePath);
  struct nfs_context *GetNfsContext() {return m_pNfsContext;}
  uint64_t GetMaxReadChunkSize() {return m_readChunkSize;}
  uint64_t GetMaxWriteChunkSize() {return m_writeChunkSize;} 
  DllLibNfs *GetImpl() {return m_pLibNfs;}
  std::list<std::string> GetExportList(const GoTvUrl &url);
  //this functions splits the url into the exportpath (feed to mount) and the rest of the path
  //relative to the mounted export
  bool splitUrlIntoExportAndPath(const GoTvUrl& url, std::string &exportPath, std::string &relativePath, std::list<std::string> &exportList);
  bool splitUrlIntoExportAndPath(const GoTvUrl& url, std::string &exportPath, std::string &relativePath);
  
  //special stat which uses its own context
  //needed for getting intervolume symlinks to work
  int stat(const GoTvUrl &url, NFSSTAT *statbuff);

  void AddActiveConnection();
  void AddIdleConnection();
  void CheckIfIdle();
  void Deinit();
  bool HandleDyLoad();//loads the lib if needed
  //adds the filehandle to the keep alive list or resets
  //the timeout for this filehandle if already in list
  void resetKeepAlive(std::string _exportPath, struct nfsfh  *_pFileHandle);
  //removes file handle from keep alive list
  void removeFromKeepAliveList(struct nfsfh  *_pFileHandle);  
  
  const std::string& GetConnectedIp() const {return m_resolvedHostName;}
  const std::string& GetConnectedExport() const {return m_exportPath;}
  const std::string GetContextMapId() const {return m_hostName + m_exportPath;}

private:
  struct nfs_context *m_pNfsContext;//current nfs context
  std::string m_exportPath;//current connected export path
  std::string m_hostName;//current connected host
  std::string m_resolvedHostName;//current connected host - as ip
  uint64_t m_readChunkSize;//current read chunksize of connected server
  uint64_t m_writeChunkSize;//current write chunksize of connected server
  int m_OpenConnections;//number of open connections
  unsigned int m_IdleTimeout;//timeout for idle connection close and dyunload
  tFileKeepAliveMap m_KeepAliveTimeouts;//mapping filehandles to its idle timeout
  tOpenContextMap m_openContextMap;//unique map for tracking all open contexts
  uint64_t m_lastAccessedTime;//last access time for m_pNfsContext
  DllLibNfs *m_pLibNfs;//the lib
  std::list<std::string> m_exportList;//list of exported paths of current connected servers
  CCriticalSection keepAliveLock;
  CCriticalSection openContextLock;
 
  void clearMembers();
  struct nfs_context *getContextFromMap(const std::string &exportname, bool forceCacheHit = false);
  int getContextForExport(const std::string &exportname);//get context for given export and add to open contexts map - sets m_pNfsContext (my return a already mounted cached context)
  void destroyOpenContexts();
  void destroyContext(const std::string &exportName);
  void resolveHost(const GoTvUrl &url);//resolve hostname by dnslookup
  void keepAlive(std::string _exportPath, struct nfsfh  *_pFileHandle);
};

extern CNfsConnection gNfsConnection;

namespace XFILE
{
  class CNFSFile : public IFile
  {
  public:
    CNFSFile();
    ~CNFSFile() override;
    void Close() override;
    int64_t Seek(int64_t iFilePosition, int iWhence = SEEK_SET) override;
    int64_t Read(void* lpBuf, size_t uiBufSize) override;
    bool Open(const GoTvUrl& url) override;
    bool Exists(const GoTvUrl& url) override;
    int Stat(const GoTvUrl& url, struct __stat64* buffer) override;
    int Stat(struct __stat64* buffer) override;
    int64_t GetLength() override;
    int64_t GetPosition() override;
    int64_t Write(const void* lpBuf, size_t uiBufSize) override;
    int Truncate(int64_t iSize) override;

    //implement iocontrol for seek_possible for preventing the stat in File class for
    //getting this info ...
    int IoControl(EIoControl request, void* param) override{ return request == IOCTRL_SEEK_POSSIBLE ? 1 : -1; };    
    int GetChunkSize() override {return static_cast<int>(gNfsConnection.GetMaxReadChunkSize());}
    
    bool OpenForWrite(const GoTvUrl& url, bool bOverWrite = false) override;
    bool Delete(const GoTvUrl& url) override;
    bool Rename(const GoTvUrl& url, const GoTvUrl& urlnew) override;    
  protected:
    GoTvUrl m_url;
    bool IsValidFile(const std::string& strFileName);
    int64_t m_fileSize;
    struct nfsfh *m_pFileHandle;
    struct nfs_context *m_pNfsContext;//current nfs context
    std::string m_exportPath;
  };
}

