/*
 *  Copyright (C) 2005-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "SpecialProtocolDirectory.h"
#include "SpecialProtocol.h"
#include "Directory.h"
#include "utils/URIUtils.h"
#include "FileItem.h"
#include "NlcUrl.h"

using namespace XFILE;

CSpecialProtocolDirectory::CSpecialProtocolDirectory(void) = default;

CSpecialProtocolDirectory::~CSpecialProtocolDirectory(void) = default;

bool CSpecialProtocolDirectory::GetDirectory(const NlcUrl& url, CFileItemList &items)
{
  const std::string pathToUrl(url.Get());
  std::string translatedPath = CSpecialProtocol::TranslatePath(url);
  if (CDirectory::GetDirectory(translatedPath, items, m_strFileMask, m_flags | DIR_FLAG_GET_HIDDEN))
  { // replace our paths as necessary
    items.SetURL(url);
    for (int i = 0; i < items.Size(); i++)
    {
      CFileItemPtr item = items[i];
      if (URIUtils::PathHasParent(item->GetPath(), translatedPath))
        item->SetPath(URIUtils::AddFileToFolder(pathToUrl, item->GetPath().substr(translatedPath.size())));
    }
    return true;
  }
  return false;
}

std::string CSpecialProtocolDirectory::TranslatePath(const NlcUrl &url)
{
  return CSpecialProtocol::TranslatePath(url);
}
