/*
 *  Copyright (C) 2005-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#if defined (TARGET_DARWIN)
// defined in PlatformDefs.h but I don't want to include that here
typedef unsigned char BYTE;

#include "utils/log.h"
#include "utils/SystemInfo.h"
#include "Application.h"
#include "powermanagement/PowerManager.h"
#include "ServiceBroker.h"
#include "CocoaPowerSyscall.h"

#if defined(TARGET_DARWIN_OSX)
  #include <IOKit/pwr_mgt/IOPMLib.h>
  #include <IOKit/ps/IOPowerSources.h>
  #include <IOKit/ps/IOPSKeys.h>
  #include <ApplicationServices/ApplicationServices.h>
#endif

#include "platform/darwin/DarwinUtils.h"

#include "platform/darwin/osx/CocoaInterface.h"

IPowerSyscall* CCocoaPowerSyscall::CreateInstance()
{
  return new CCocoaPowerSyscall();
}

void CCocoaPowerSyscall::Register()
{
  IPowerSyscall::RegisterPowerSyscall(CCocoaPowerSyscall::CreateInstance);
}

#if defined(TARGET_DARWIN_OSX)
OSStatus SendAppleEventToSystemProcess(AEEventID eventToSendID)
{
  AEAddressDesc targetDesc;
  static const  ProcessSerialNumber kPSNOfSystemProcess = {0, kSystemProcess };
  AppleEvent    eventReply  = {typeNull, NULL};
  AppleEvent    eventToSend = {typeNull, NULL};

  OSStatus status = AECreateDesc(typeProcessSerialNumber,
    &kPSNOfSystemProcess, sizeof(kPSNOfSystemProcess), &targetDesc);

  if (status != noErr)
    return status;

  status = AECreateAppleEvent(kCoreEventClass, eventToSendID,
    &targetDesc, kAutoGenerateReturnID, kAnyTransactionID, &eventToSend);
  AEDisposeDesc(&targetDesc);

  if (status != noErr)
    return status;

  status = AESendMessage(&eventToSend, &eventReply, kAENormalPriority, kAEDefaultTimeout);
  AEDisposeDesc(&eventToSend);

  if (status != noErr)
    return status;

  AEDisposeDesc(&eventReply);

  return status;
}
#endif

CCocoaPowerSyscall::CCocoaPowerSyscall()
{
  m_OnResume = false;
  m_OnSuspend = false;
  // assume on AC power at startup
  m_OnBattery = false;
  m_HasBattery = -1;
  m_BatteryPercent = 100;
  m_SentBatteryMessage = false;
#if !defined(TARGET_DARWIN_IOS)
  m_power_source = NULL;
#endif
  CreateOSPowerCallBacks();
}

CCocoaPowerSyscall::~CCocoaPowerSyscall()
{
  if (HasBattery())
    DeleteOSPowerCallBacks();
}

bool CCocoaPowerSyscall::Powerdown(void)
{
  bool result;
#if defined(TARGET_DARWIN_IOS)
  result = false;
#else
  CLog::Log(LOGDEBUG, "CCocoaPowerSyscall::Powerdown");
  //sending shutdown event to system
  OSErr error = SendAppleEventToSystemProcess(kAEShutDown);
  if (error == noErr)
    CLog::Log(LOGINFO, "Computer is going to shutdown!");
  else
    CLog::Log(LOGINFO, "Computer wouldn't shutdown!");
  result = (error == noErr);
#endif
  return result;
}

bool CCocoaPowerSyscall::Suspend(void)
{
#if defined(TARGET_DARWIN_IOS)
  return false;
#else
  CLog::Log(LOGDEBUG, "CCocoaPowerSyscall::Suspend");
  m_OnSuspend = true;

  //sending sleep event to system
  OSErr error = SendAppleEventToSystemProcess(kAESleep);
  if (error == noErr)
    CLog::Log(LOGINFO, "Computer is going to sleep!");
  else
    CLog::Log(LOGINFO, "Computer wouldn't sleep!");
  return (error == noErr);
#endif
}

bool CCocoaPowerSyscall::Hibernate(void)
{
  CLog::Log(LOGDEBUG, "CCocoaPowerSyscall::Hibernate");
#if defined(TARGET_DARWIN_IOS)
  return false;
#else
  // just in case hibernate is ever called
  return Suspend();
#endif
}

bool CCocoaPowerSyscall::Reboot(void)
{
  bool result;
  CLog::Log(LOGDEBUG, "CCocoaPowerSyscall::Reboot");
#if defined(TARGET_DARWIN_IOS)
  result = false;
#else
  OSErr error = SendAppleEventToSystemProcess(kAERestart);
  if (error == noErr)
    CLog::Log(LOGINFO, "Computer is going to restart!");
  else
    CLog::Log(LOGINFO, "Computer wouldn't restart!");
  result = (error == noErr);
#endif
  return result;
}

bool CCocoaPowerSyscall::CanPowerdown(void)
{
#if defined(TARGET_DARWIN_IOS)
  return false;
#else
  // All Apple products can power down
  return true;
#endif
}

bool CCocoaPowerSyscall::CanSuspend(void)
{
  bool result;
#if defined(TARGET_DARWIN_IOS)
  result = false;
#else
  result =IOPMSleepEnabled();
#endif
  return(result);
}

bool CCocoaPowerSyscall::CanHibernate(void)
{
  // Darwin does "sleep" which automatically handles hibernate
  // so always return false so the GUI does not show hibernate
  return false;
}

bool CCocoaPowerSyscall::CanReboot(void)
{
#if defined(TARGET_DARWIN_IOS)
  return false;
#else
  // All Apple products except iOS can reboot
  return true;
#endif
}

bool CCocoaPowerSyscall::HasBattery(void)
{
  bool result;
#if defined(TARGET_DARWIN_IOS)
  result = false;
#else
  result = true;

  if (m_HasBattery == -1)
  {
    CCocoaAutoPool autopool;
    CFArrayRef battery_info = NULL;

    if (IOPMCopyBatteryInfo(kIOMasterPortDefault, &battery_info) != kIOReturnSuccess)
      result = false;
    else
      CFRelease(battery_info);
    // cache if we have a battery or not
    m_HasBattery = result;
  }
  else
  {
    result = m_HasBattery;
  }
#endif
  return result;
}

int CCocoaPowerSyscall::BatteryLevel(void)
{
  return CDarwinUtils::BatteryLevel();
}

bool CCocoaPowerSyscall::PumpPowerEvents(IPowerEventsCallback *callback)
{
  if (m_OnSuspend)
  {
    callback->OnSleep();
    m_OnSuspend = false;
  }
  else if (m_OnResume)
  {
    callback->OnWake();
    m_OnResume = false;
  }

  if (m_HasBattery && m_OnBattery && !m_SentBatteryMessage)
  {
    if (m_BatteryPercent < 10)
    {
      callback->OnLowBattery();
      m_SentBatteryMessage = true;
    }
  }

  return true;
}

void CCocoaPowerSyscall::CreateOSPowerCallBacks(void)
{
#if !defined(TARGET_DARWIN_IOS)
  CCocoaAutoPool autopool;
  // we want sleep/wake notifications, register to receive system power notifications
  m_root_port = IORegisterForSystemPower(this, &m_notify_port, OSPowerCallBack, &m_notifier_object);
  if (m_root_port)
  {
    // add the notification port to the application runloop
    CFRunLoopAddSource(CFRunLoopGetCurrent(),
      IONotificationPortGetRunLoopSource(m_notify_port), kCFRunLoopDefaultMode);
  }
  else
  {
    CLog::Log(LOGERROR, "%s - IORegisterForSystemPower failed", __FUNCTION__);
  }

  // if we have a battery, we want power source change notifications (on AC, on Battery, etc)
  if (m_HasBattery)
  {
    m_power_source = IOPSNotificationCreateRunLoopSource(OSPowerSourceCallBack, this);
    if (m_power_source)
      CFRunLoopAddSource(CFRunLoopGetCurrent(), m_power_source, kCFRunLoopDefaultMode);
    else
      CLog::Log(LOGERROR, "%s - IOPSNotificationCreateRunLoopSource failed", __FUNCTION__);
  }
#endif
}

void CCocoaPowerSyscall::DeleteOSPowerCallBacks(void)
{
#if !defined(TARGET_DARWIN_IOS)
  CCocoaAutoPool autopool;
  // we no longer want sleep/wake notifications
  // remove the sleep notification port from the application runloop
  CFRunLoopRemoveSource( CFRunLoopGetCurrent(),
    IONotificationPortGetRunLoopSource(m_notify_port), kCFRunLoopDefaultMode );
  // deregister for system sleep notifications
  IODeregisterForSystemPower(&m_notifier_object);
  // IORegisterForSystemPower implicitly opens the Root Power Domain IOService
  // so we close it here
  IOServiceClose(m_root_port);
  // destroy the notification port allocated by IORegisterForSystemPower
  IONotificationPortDestroy(m_notify_port);

  // we no longer want power source change notifications
  if (m_HasBattery)
  {
    if (m_power_source)
    {
      CFRunLoopRemoveSource( CFRunLoopGetCurrent(), m_power_source, kCFRunLoopDefaultMode );
      CFRelease(m_power_source);
    }
  }
#endif
}

void CCocoaPowerSyscall::OSPowerCallBack(void *refcon, io_service_t service, natural_t msg_type, void *msg_arg)
{
#if !defined(TARGET_DARWIN_IOS)
  CCocoaAutoPool autopool;
  CCocoaPowerSyscall  *ctx;

  ctx = (CCocoaPowerSyscall*)refcon;

  switch (msg_type)
  {
    case kIOMessageCanSystemSleep:
      // System has been idle for sleeptime and will sleep soon.
      // we can either allow or cancel this notification.
      // if we don't respond, OS will sleep in 30 second.
      ctx->m_OnSuspend = true;
      IOAllowPowerChange(ctx->m_root_port, (long)msg_arg);
      //CLog::Log(LOGDEBUG, "%s - kIOMessageCanSystemSleep", __FUNCTION__);
    break;
    case kIOMessageSystemWillSleep:
      // System demanded sleep from:
      //   1) selecting sleep from the Apple menu.
      //   2) closing the lid of a laptop.
      //   3) running out of battery power.
      ctx->m_OnSuspend = true;
      // force processing of this power event. This callback runs
      // in main thread so we can do this.
      CServiceBroker::GetPowerManager().ProcessEvents();
      IOAllowPowerChange(ctx->m_root_port, (long)msg_arg);
      //CLog::Log(LOGDEBUG, "%s - kIOMessageSystemWillSleep", __FUNCTION__);
      // let XBMC know system will sleep
      //! @todo implement
    break;
    case kIOMessageSystemHasPoweredOn:
      // System has awakened from sleep.
      // let XBMC know system has woke
      //! @todo implement
      ctx->m_OnResume = true;
      //CLog::Log(LOGDEBUG, "%s - kIOMessageSystemHasPoweredOn", __FUNCTION__);
    break;
	}
#endif
}

#if !defined(TARGET_DARWIN_IOS)
static bool stringsAreEqual(CFStringRef a, CFStringRef b)
{
	if (a == nil || b == nil)
		return 0;
	return (CFStringCompare (a, b, 0) == kCFCompareEqualTo);
}
#endif

void CCocoaPowerSyscall::OSPowerSourceCallBack(void *refcon)
{
#if !defined(TARGET_DARWIN_IOS)
  // Called whenever any power source is added, removed, or changes.
  // When on battery, we get called periodically as battery level changes.
  CCocoaAutoPool autopool;
  CCocoaPowerSyscall  *ctx = (CCocoaPowerSyscall*)refcon;

  CFTypeRef power_sources_info = IOPSCopyPowerSourcesInfo();
  CFArrayRef power_sources_list = IOPSCopyPowerSourcesList(power_sources_info);

  for (int i = 0; i < CFArrayGetCount(power_sources_list); i++)
  {
		CFTypeRef power_source;
		CFDictionaryRef description;

		power_source = CFArrayGetValueAtIndex(power_sources_list, i);
		description  = IOPSGetPowerSourceDescription(power_sources_info, power_source);

    // skip power sources that are not present (i.e. an absent second battery in a 2-battery machine)
    if ((CFBooleanRef)CFDictionaryGetValue(description, CFSTR(kIOPSIsPresentKey)) == kCFBooleanFalse)
      continue;

    if (stringsAreEqual((CFStringRef)CFDictionaryGetValue(description, CFSTR (kIOPSTransportTypeKey)), CFSTR (kIOPSInternalType)))
    {
      CFStringRef currentState = (CFStringRef)CFDictionaryGetValue(description, CFSTR (kIOPSPowerSourceStateKey));

      if (stringsAreEqual (currentState, CFSTR (kIOPSACPowerValue)))
      {
        ctx->m_OnBattery = false;
        ctx->m_BatteryPercent = 100;
        ctx->m_SentBatteryMessage = false;
      }
      else if (stringsAreEqual (currentState, CFSTR (kIOPSBatteryPowerValue)))
      {
        CFNumberRef cf_number_ref;
        int32_t curCapacity, maxCapacity;

        cf_number_ref = (CFNumberRef)CFDictionaryGetValue(description, CFSTR(kIOPSCurrentCapacityKey));
        CFNumberGetValue(cf_number_ref, kCFNumberSInt32Type, &curCapacity);

        cf_number_ref = (CFNumberRef)CFDictionaryGetValue(description, CFSTR(kIOPSMaxCapacityKey));
        CFNumberGetValue(cf_number_ref, kCFNumberSInt32Type, &maxCapacity);

        ctx->m_OnBattery = true;
        ctx->m_BatteryPercent = (int)((double)curCapacity/(double)maxCapacity * 100);
      }
		}
  }

  CFRelease(power_sources_list);
  CFRelease(power_sources_info);
#endif
}

#endif
