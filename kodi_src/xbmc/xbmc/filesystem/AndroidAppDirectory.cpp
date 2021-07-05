/*
 *      Copyright (C) 2012-2013 Team XBMC
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

#include "config_kodi.h"

#if defined(TARGET_OS_ANDROID)

#if defined(TARGET_OS_ANDROID) && !defined(HAVE_QT_GUI)
# include "filesystem/AndroidAppDirectory.h"
# include "platform/android/activity/XBMCApp.h"
#elif defined(TARGET_ANDROID) && defined(HAVE_QT_GUI)
# include "filesystem/AndroidAppDirectory.h"
# include "platform/qt/KodiQtApp.h"
#endif // defined(TARGET_ANDROID) && !defined(HAVE_QT_GUI)

#include "FileItem.h"
#include "File.h"
#include "utils/URIUtils.h"
#include <vector>
#include "utils/log.h"
#include "utils/StringUtils.h"
#include "GoTvUrl.h"
#include "CompileInfo.h"

using namespace XFILE;

CAndroidAppDirectory::CAndroidAppDirectory(void)
{
}

CAndroidAppDirectory::~CAndroidAppDirectory(void)
{
}

bool CAndroidAppDirectory::GetDirectory(const GoTvUrl& url, CFileItemList &items)
{
  std::string dirname = url.GetFileName();
  URIUtils::RemoveSlashAtEnd(dirname);
  CLog::Log(LOGDEBUG, "CAndroidAppDirectory::GetDirectory: %s",dirname.c_str()); 
  std::string appName = CCompileInfo::GetAppName();
  StringUtils::ToLower(appName);
  std::string className = CCompileInfo::GetPackage();

  if (dirname == "apps")
  {
    std::vector<androidPackage> applications = CXBMCApp::GetApplications();
    if (applications.empty())
    {
      CLog::Log(LOGERROR, "CAndroidAppDirectory::GetDirectory Application lookup listing failed");
      return false;
    }
    for(std::vector<androidPackage>::iterator i = applications.begin(); i != applications.end(); ++i)
    {
      if ((*i).packageName == className.c_str())
        continue;
      CFileItemPtr pItem(new CFileItem((*i).packageName));
      pItem->m_bIsFolder = false;
      std::string path = StringUtils::Format("androidapp://%s/%s/%s", url.GetHostName().c_str(), dirname.c_str(), (*i).packageName.c_str());
      pItem->SetPath(path);
      pItem->SetLabel((*i).packageLabel);
      pItem->SetArt("thumb", path+".png");
      pItem->m_dwSize = -1;  // No size
      items.Add(pItem);
    }
    return true;
  }

  CLog::Log(LOGERROR, "CAndroidAppDirectory::GetDirectory Failed to open %s", url.Get().c_str());
  return false;
}

#endif // defined(TARGET_OS_ANDROID)
