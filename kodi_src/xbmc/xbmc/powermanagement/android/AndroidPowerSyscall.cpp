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
#if defined (TARGET_OS_ANDROID)

#include "AndroidPowerSyscall.h"
#if defined(TARGET_OS_ANDROID) && !defined(HAVE_QT_GUI)
# include "platform/android/activity/XBMCApp.h"
#elif defined(TARGET_OS_ANDROID) && defined(HAVE_QT_GUI)
# include "platform/qt/KodiQtApp.h"
#endif // defined(TARGET_OS_ANDROID) && !defined(HAVE_QT_GUI)


IPowerSyscall* CAndroidPowerSyscall::CreateInstance()
{
  return new CAndroidPowerSyscall();
}

void CAndroidPowerSyscall::Register()
{
  IPowerSyscall::RegisterPowerSyscall(CAndroidPowerSyscall::CreateInstance);
}

CAndroidPowerSyscall::CAndroidPowerSyscall()
{ }

CAndroidPowerSyscall::~CAndroidPowerSyscall()
{ }

int CAndroidPowerSyscall::BatteryLevel(void)
{
  return CXBMCApp::GetBatteryLevel();
}

bool CAndroidPowerSyscall::PumpPowerEvents(IPowerEventsCallback *callback)
{    
  return true;
}

#endif
