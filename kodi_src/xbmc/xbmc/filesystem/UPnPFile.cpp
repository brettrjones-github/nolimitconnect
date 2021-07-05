/*
 *  Copyright (C) 2011-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "UPnPFile.h"
#include "UPnPDirectory.h"
#include "FileFactory.h"
#include "FileItem.h"
#include "GoTvUrl.h"

using namespace XFILE;

CUPnPFile::CUPnPFile() = default;

CUPnPFile::~CUPnPFile() = default;

bool CUPnPFile::Open(const GoTvUrl& url)
{
  CFileItem item_new;
  if (CUPnPDirectory::GetResource(url, item_new))
  {
    //CLog::Log(LOGDEBUG,"FileUPnP - file redirect to %s.", item_new.GetPath().c_str());
    IFile *pNewImp = CFileFactory::CreateLoader(item_new.GetPath());    
    GoTvUrl *pNewUrl = new GoTvUrl(item_new.GetPath());    
    if (pNewImp)
    {
      throw new CRedirectException(pNewImp, pNewUrl);
    }
    delete pNewUrl;    
  }
  return false;
}

int CUPnPFile::Stat(const GoTvUrl& url, struct __stat64* buffer)
{
  CFileItem item_new;
  if (CUPnPDirectory::GetResource(url, item_new))
  {
    //CLog::Log(LOGDEBUG,"FileUPnP - file redirect to %s.", item_new.GetPath().c_str());
    IFile *pNewImp = CFileFactory::CreateLoader(item_new.GetPath());
    GoTvUrl *pNewUrl = new GoTvUrl(item_new.GetPath());
    if (pNewImp)
    {
      throw new CRedirectException(pNewImp, pNewUrl);
    }
    delete pNewUrl;
  }
  return -1;
}

bool CUPnPFile::Exists(const GoTvUrl& url)
{
  CFileItem item_new;
  if (CUPnPDirectory::GetResource(url, item_new))
  {
    //CLog::Log(LOGDEBUG,"FileUPnP - file redirect to %s.", item_new.GetPath().c_str());
    IFile *pNewImp = CFileFactory::CreateLoader(item_new.GetPath());
    GoTvUrl *pNewUrl = new GoTvUrl(item_new.GetPath());
    if (pNewImp)
    {
      throw new CRedirectException(pNewImp, pNewUrl);
    }
    delete pNewUrl;
  }
  return false;
}
