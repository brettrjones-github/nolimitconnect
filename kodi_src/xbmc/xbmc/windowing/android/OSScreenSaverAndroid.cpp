/*
 *      Copyright (C) 2018 Chris Browet
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "OSScreenSaverAndroid.h"

#if defined(TARGET_OS_ANDROID) && !defined(HAVE_QT_GUI)
# include "platform/android/activity/XBMCApp.h"
#elif defined(TARGET_OS_ANDROID) && defined(HAVE_QT_GUI)
# include "platform/qt/KodiQtApp.h"
#endif // defined(TARGET_OS_ANDROID) && !defined(HAVE_QT_GUI)


void COSScreenSaverAndroid::Inhibit()
{
  CXBMCApp::get()->EnableWakeLock(true);
}

void COSScreenSaverAndroid::Uninhibit()
{
  CXBMCApp::get()->EnableWakeLock(false);
}
