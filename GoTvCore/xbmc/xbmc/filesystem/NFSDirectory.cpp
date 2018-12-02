/*
 *  Copyright (C) 2011-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "DllLibNfs.h"

#ifdef TARGET_WINDOWS
#include <sys\stat.h>
#endif

#ifdef TARGET_POSIX
#include "platform/linux/XTimeUtils.h"
#endif

#include "NFSDirectory.h"
#include "FileItem.h"
#include "utils/log.h"
#include "utils/URIUtils.h"
#include "utils/StringUtils.h"
#include "threads/SingleLock.h"
using namespace XFILE;
#include <limits.h>
#include <libnfs/libnfs-raw-nfs.h>

CNFSDirectory::CNFSDirectory(void)
{
  gNfsConnection.AddActiveConnection();
}

CNFSDirectory::~CNFSDirectory(void)
{
  gNfsConnection.AddIdleConnection();
}

bool CNFSDirectory::GetDirectoryFromExportList(const std::string& strPath, CFileItemList &items)
{
  GoTvUrl url(strPath);
  std::string nonConstStrPath(strPath);
  std::list<std::string> exportList=gNfsConnection.GetExportList(url);
  std::list<std::string>::iterator it;
  
  for(it=exportList.begin();it!=exportList.end();++it)
  {
      std::string currentExport(*it);     
      URIUtils::RemoveSlashAtEnd(nonConstStrPath);
           
      CFileItemPtr pItem(new CFileItem(currentExport));
      std::string path(nonConstStrPath + currentExport);
      URIUtils::AddSlashAtEnd(path);
      pItem->SetPath(path);
      pItem->m_dateTime=0;

      pItem->m_bIsFolder = true;
      items.Add(pItem);
  }
  
  return exportList.empty()? false : true;
}

bool CNFSDirectory::GetServerList(CFileItemList &items)
{
  struct nfs_server_list *srvrs;
  struct nfs_server_list *srv;
  bool ret = false;

  if(!gNfsConnection.HandleDyLoad())
  {
    return false;
  }

  srvrs = gNfsConnection.GetImpl()->nfs_find_local_servers();	

  for (srv=srvrs; srv; srv = srv->next) 
  {
      std::string currentExport(srv->addr);

      CFileItemPtr pItem(new CFileItem(currentExport));
      std::string path("nfs://" + currentExport);
      URIUtils::AddSlashAtEnd(path);
      pItem->m_dateTime=0;

      pItem->SetPath(path);
      pItem->m_bIsFolder = true;
      items.Add(pItem);
      ret = true; //added at least one entry
  }
  gNfsConnection.GetImpl()->free_nfs_srvr_list(srvrs);

  return ret;
}

bool CNFSDirectory::ResolveSymlink( const std::string &dirName, struct nfsdirent *dirent, GoTvUrl &resolvedUrl)
{
  CSingleLock lock(gNfsConnection); 
  int ret = 0;  
  bool retVal = true;
  std::string fullpath = dirName;
  char resolvedLink[MAX_PATH];
  
  URIUtils::AddSlashAtEnd(fullpath);
  fullpath.append(dirent->name);
  
  resolvedUrl.Reset();
  resolvedUrl.SetPort(2049);
  resolvedUrl.SetProtocol("nfs");
  resolvedUrl.SetHostName(gNfsConnection.GetConnectedIp()); 
  
  ret = gNfsConnection.GetImpl()->nfs_readlink(gNfsConnection.GetNfsContext(), fullpath.c_str(), resolvedLink, MAX_PATH);    
  
  if(ret == 0)
  {
    NFSSTAT tmpBuffer = {0};
    fullpath = dirName;
    URIUtils::AddSlashAtEnd(fullpath);
    fullpath.append(resolvedLink);
  
    //special case - if link target is absolute it could be even another export
    //intervolume symlinks baby ...
    if(resolvedLink[0] == '/')
    {    
      //use the special stat function for using an extra context
      //because we are inside of a dir traversal
      //and just can't change the global nfs context here
      //without destroying something...
      fullpath = resolvedLink;
      resolvedUrl.SetFileName(fullpath);            
      ret = gNfsConnection.stat(resolvedUrl, &tmpBuffer);
    }
    else
    {
      ret = gNfsConnection.GetImpl()->nfs_stat(gNfsConnection.GetNfsContext(), fullpath.c_str(), &tmpBuffer);
      resolvedUrl.SetFileName(gNfsConnection.GetConnectedExport() + fullpath);      
    }

    if (ret != 0) 
    {
      CLog::Log(LOGERROR, "NFS: Failed to stat(%s) on link resolve %s\n", fullpath.c_str(), gNfsConnection.GetImpl()->nfs_get_error(gNfsConnection.GetNfsContext()));
      retVal = false;
    }
    else
    {  
      dirent->inode = tmpBuffer.st_ino;
      dirent->mode = tmpBuffer.st_mode;
      dirent->size = tmpBuffer.st_size;
      dirent->atime.tv_sec = static_cast<long>(tmpBuffer.st_atime);
      dirent->mtime.tv_sec = static_cast<long>(tmpBuffer.st_mtime);
      dirent->ctime.tv_sec = static_cast<long>(tmpBuffer.st_ctime);
      
      //map stat mode to nf3type
      if(S_ISBLK(tmpBuffer.st_mode)){ dirent->type = NF3BLK; }
      else if(S_ISCHR(tmpBuffer.st_mode)){ dirent->type = NF3CHR; }
      else if(S_ISDIR(tmpBuffer.st_mode)){ dirent->type = NF3DIR; }
      else if(S_ISFIFO(tmpBuffer.st_mode)){ dirent->type = NF3FIFO; }
      else if(S_ISREG(tmpBuffer.st_mode)){ dirent->type = NF3REG; }      
      else if(S_ISLNK(tmpBuffer.st_mode)){ dirent->type = NF3LNK; }      
      else if(S_ISSOCK(tmpBuffer.st_mode)){ dirent->type = NF3SOCK; }            
    }
  }
  else
  {
    CLog::Log(LOGERROR, "Failed to readlink(%s) %s\n", fullpath.c_str(), gNfsConnection.GetImpl()->nfs_get_error(gNfsConnection.GetNfsContext()));
    retVal = false;
  }
  return retVal;
}

bool CNFSDirectory::GetDirectory(const GoTvUrl& url, CFileItemList &items)
{
  // We accept nfs://server/path[/file]]]]
  int ret = 0;
  FILETIME fileTime, localTime;    
  CSingleLock lock(gNfsConnection); 
  std::string strDirName="";
  std::string myStrPath(url.Get());
  URIUtils::AddSlashAtEnd(myStrPath); //be sure the dir ends with a slash
   
  if(!gNfsConnection.Connect(url,strDirName))
  {
    //connect has failed - so try to get the exported filesystems if no path is given to the url
    if(url.GetShareName().empty())
    {
      if(url.GetHostName().empty())
      {
        return GetServerList(items);
      }
      else 
      {
        return GetDirectoryFromExportList(myStrPath, items); 
      }
    }
    else
    {
      return false;
    }    
  }
      
  struct nfsdir *nfsdir = NULL;
  struct nfsdirent *nfsdirent = NULL;

  ret = gNfsConnection.GetImpl()->nfs_opendir(gNfsConnection.GetNfsContext(), strDirName.c_str(), &nfsdir);

  if(ret != 0)
  {
    CLog::Log(LOGERROR, "Failed to open(%s) %s\n", strDirName.c_str(), gNfsConnection.GetImpl()->nfs_get_error(gNfsConnection.GetNfsContext()));
    return false;
  }
  lock.Leave();
  
  while((nfsdirent = gNfsConnection.GetImpl()->nfs_readdir(gNfsConnection.GetNfsContext(), nfsdir)) != NULL) 
  {
    struct nfsdirent tmpDirent = *nfsdirent;
    std::string strName = tmpDirent.name;
    std::string path(myStrPath + strName);
    int64_t iSize = 0;
    bool bIsDir = false;
    int64_t lTimeDate = 0;

    //reslove symlinks
    if(tmpDirent.type == NF3LNK)
    {
      GoTvUrl linkUrl;
      //resolve symlink changes tmpDirent and strName
      if(!ResolveSymlink(strDirName,&tmpDirent,linkUrl))
      { 
        continue;
      }
      
      path = linkUrl.Get();
    }
    
    iSize = tmpDirent.size;
    bIsDir = tmpDirent.type == NF3DIR;
    lTimeDate = tmpDirent.mtime.tv_sec;

    if (!StringUtils::EqualsNoCase(strName,".") && !StringUtils::EqualsNoCase(strName,"..")
        && !StringUtils::EqualsNoCase(strName,"lost+found"))
    {
      if(lTimeDate == 0) // if modification date is missing, use create date
      {
        lTimeDate = tmpDirent.ctime.tv_sec;
      }

      long long ll = lTimeDate & 0xffffffff;
      ll *= 10000000ll;
      ll += 116444736000000000ll;
      fileTime.dwLowDateTime = (DWORD) (ll & 0xffffffff);
      fileTime.dwHighDateTime = (DWORD)(ll >> 32);
      FileTimeToLocalFileTime(&fileTime, &localTime);

      CFileItemPtr pItem(new CFileItem(tmpDirent.name));
      pItem->m_dateTime=localTime;   
      pItem->m_dwSize = iSize;        
      
      if (bIsDir)
      {
        URIUtils::AddSlashAtEnd(path);
        pItem->m_bIsFolder = true;
      }
      else
      {
        pItem->m_bIsFolder = false;
      }

      if (strName[0] == '.')
      {
        pItem->SetProperty("file:hidden", true);
      }
      pItem->SetPath(path);
      items.Add(pItem);
    }
  }

  lock.Enter();
  gNfsConnection.GetImpl()->nfs_closedir(gNfsConnection.GetNfsContext(), nfsdir);//close the dir
  lock.Leave();
  return true;
}

bool CNFSDirectory::Create(const GoTvUrl& url2)
{
  int ret = 0;
  bool success=true;
  
  CSingleLock lock(gNfsConnection);
  std::string folderName(url2.Get());
  URIUtils::RemoveSlashAtEnd(folderName);//mkdir fails if a slash is at the end!!! 
  GoTvUrl url(folderName); 
  folderName = "";
  
  if(!gNfsConnection.Connect(url,folderName))
    return false;
  
  ret = gNfsConnection.GetImpl()->nfs_mkdir(gNfsConnection.GetNfsContext(), folderName.c_str());

  success = (ret == 0 || -EEXIST == ret);
  if(!success)
    CLog::Log(LOGERROR, "NFS: Failed to create(%s) %s\n", folderName.c_str(), gNfsConnection.GetImpl()->nfs_get_error(gNfsConnection.GetNfsContext()));
  return success;
}

bool CNFSDirectory::Remove(const GoTvUrl& url2)
{
  int ret = 0;

  CSingleLock lock(gNfsConnection);
  std::string folderName(url2.Get());
  URIUtils::RemoveSlashAtEnd(folderName);//rmdir fails if a slash is at the end!!!   
  GoTvUrl url(folderName);
  folderName = "";
  
  if(!gNfsConnection.Connect(url,folderName))
    return false;
  
  ret = gNfsConnection.GetImpl()->nfs_rmdir(gNfsConnection.GetNfsContext(), folderName.c_str());

  if(ret != 0 && errno != ENOENT)
  {
    CLog::Log(LOGERROR, "%s - Error( %s )", __FUNCTION__, gNfsConnection.GetImpl()->nfs_get_error(gNfsConnection.GetNfsContext()));
    return false;
  }
  return true;
}

bool CNFSDirectory::Exists(const GoTvUrl& url2)
{
  int ret = 0;

  CSingleLock lock(gNfsConnection); 
  std::string folderName(url2.Get());
  URIUtils::RemoveSlashAtEnd(folderName);//remove slash at end or URIUtils::GetFileName won't return what we want...
  GoTvUrl url(folderName);
  folderName = "";
  
  if(!gNfsConnection.Connect(url,folderName))
    return false;
  
  NFSSTAT info;
  ret = gNfsConnection.GetImpl()->nfs_stat(gNfsConnection.GetNfsContext(), folderName.c_str(), &info);
  
  if (ret != 0)
  {
    return false;
  }
  return S_ISDIR(info.st_mode) ? true : false;
}
