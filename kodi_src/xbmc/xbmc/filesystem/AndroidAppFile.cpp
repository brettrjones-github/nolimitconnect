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

#include <jni.h>
#include <sys/stat.h>

#include <android/bitmap.h>
#if defined(HAVE_QT_GUI)
# include <platform/qt/qtandroid/jni/Bitmap.h>
# include <platform/qt/qtandroid/jni/Drawable.h>
# include <platform/qt/qtandroid/jni/BitmapDrawable.h>
# include <platform/qt/qtandroid/jni/Build.h>
# include <platform/qt/qtandroid/jni/Context.h>
# include <platform/qt/qtandroid/jni/DisplayMetrics.h>
# include <platform/qt/qtandroid/jni/PackageManager.h>
# include <platform/qt/qtandroid/jni/Resources.h>
#else
# include <android/jni/Bitmap.h>
# include <android/jni/Drawable.h>
# include <android/jni/BitmapDrawable.h>
# include <android/jni/Build.h>
# include <android/jni/Context.h>
# include <android/jni/DisplayMetrics.h>
# include <android/jni/PackageManager.h>
# include <android/jni/Resources.h>
#endif // defined(HAVE_QT_GUI)

#include "AndroidAppFile.h"
#if defined(TARGET_OS_ANDROID) && !defined(HAVE_QT_GUI)
# include "platform/android/activity/XBMCApp.h"
#elif defined(TARGET_OS_ANDROID) && defined(HAVE_QT_GUI)
# include "platform/qt/KodiQtApp.h"
#endif // defined(TARGET_OS_ANDROID) && !defined(HAVE_QT_GUI)

#include "NlcCoreUtil.h"
#include "NlcUrl.h"
#include "utils/log.h"
#include "utils/URIUtils.h"
using namespace XFILE;

CFileAndroidApp::CFileAndroidApp(void)
{
  m_iconWidth = 0;
  m_iconHeight = 0;
}

CFileAndroidApp::~CFileAndroidApp(void)
{
  Close();
}

bool CFileAndroidApp::Open(const NlcUrl& url)
{
  m_url = url;
  m_packageName =  URIUtils::GetFileName(url.Get());
  m_packageName = m_packageName.substr(0, m_packageName.size() - 4);

  std::vector<androidPackage> applications = CXBMCApp::GetApplications();
  for(std::vector<androidPackage>::iterator i = applications.begin(); i != applications.end(); ++i)
  {
    if ((*i).packageName == m_packageName)
    {
      m_packageLabel = i->packageLabel;
      m_icon         = i->icon;
      return true;
    }
  }

  return false;
}

bool CFileAndroidApp::Exists(const NlcUrl& url)
{
  std::string appname =  URIUtils::GetFileName(url.Get());
  appname = appname.substr(0, appname.size() - 4);

  std::vector<androidPackage> applications = CXBMCApp::GetApplications();
  for(std::vector<androidPackage>::iterator i = applications.begin(); i != applications.end(); ++i)
  {
    if ((*i).packageName == appname)
      return true;
  }

  return false;
}

unsigned int CFileAndroidApp::ReadIcon(unsigned char** lpBuf, unsigned int* width, unsigned int* height)
{
  JNIEnv* env = xbmc_jnienv();
  void *bitmapBuf = NULL;
  int densities[] = { CJNIDisplayMetrics::DENSITY_XXXHIGH, CJNIDisplayMetrics::DENSITY_XXHIGH, CJNIDisplayMetrics::DENSITY_XHIGH, -1 };

  CJNIBitmap bmp;
  jclass cBmpDrw = env->FindClass("android/graphics/drawable/BitmapDrawable");

  if (CJNIBuild::SDK_INT >= 15 && m_icon)
  {
    CJNIResources res = CJNIContext::GetPackageManager().getResourcesForApplication(m_packageName);
    if (res)
    {
      for (int i=0; densities[i] != -1 && !bmp; ++i)
      {
        int density = densities[i];
        CJNIDrawable drw = res.getDrawableForDensity(m_icon, density);
        if (xbmc_jnienv()->ExceptionCheck())
          xbmc_jnienv()->ExceptionClear();
        else if (!drw);
        else
        {
          if (env->IsInstanceOf(drw.get_raw(), cBmpDrw))
          {
            CJNIBitmapDrawable resbmp = drw;
            if (resbmp)
              bmp = resbmp.getBitmap();
          }
        }
      }
    }
  }

  if (!bmp)
  {
    CJNIDrawable drw = CJNIContext::GetPackageManager().getApplicationIcon(m_packageName);
    if (xbmc_jnienv()->ExceptionCheck())
      xbmc_jnienv()->ExceptionClear();
    else if (!drw);
    else
    {
      if (env->IsInstanceOf(drw.get_raw(), cBmpDrw))
      {
        CJNIBitmapDrawable resbmp = drw;
        if (resbmp)
          bmp = resbmp.getBitmap();
      }
    }
  }
  if (!bmp)
    return 0;

  AndroidBitmapInfo info;
  AndroidBitmap_getInfo(env, bmp.get_raw(), &info);
  if (!info.width || !info.height)
    return 0;

  *width = info.width;
  *height = info.height;

  int imgsize = *width * *height * 4;
  *lpBuf = new unsigned char[imgsize];

  AndroidBitmap_lockPixels(env, bmp.get_raw(), &bitmapBuf);
  if (bitmapBuf)
  {
    memcpy(*lpBuf, bitmapBuf, imgsize);
    AndroidBitmap_unlockPixels(env, bmp.get_raw());
    return imgsize;
  }
  return 0;
}

void CFileAndroidApp::Close()
{
}

int CFileAndroidApp::GetChunkSize()
{
  return 0;
}
int CFileAndroidApp::Stat(const NlcUrl& url, struct __stat64* buffer)
{
  return 0;
}
int CFileAndroidApp::IoControl(EIoControl request, void* param)
{
  if(request == IOCTRL_SEEK_POSSIBLE)
    return 0;
  return 1;
}
#endif

