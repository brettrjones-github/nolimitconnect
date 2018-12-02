/*
 *  Copyright (C) 2005-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include <stdlib.h>
#include "DarwinStorageProvider.h"
#include "utils/RegExp.h"
#include "Util.h"
#include "guilib/LocalizeStrings.h"

#include <sys/mount.h>
#if defined(TARGET_DARWIN_OSX)
#include <DiskArbitration/DiskArbitration.h>
#include <IOKit/storage/IOCDMedia.h>
#include <IOKit/storage/IODVDMedia.h>
#include "platform/darwin/DarwinUtils.h"
#endif
#include "platform/darwin/osx/CocoaInterface.h"

bool CDarwinStorageProvider::m_event = false;

IStorageProvider* IStorageProvider::CreateInstance()
{
  return new CDarwinStorageProvider();
}

CDarwinStorageProvider::CDarwinStorageProvider()
{
  PumpDriveChangeEvents(NULL);
}

void CDarwinStorageProvider::GetLocalDrives(VECSOURCES &localDrives)
{
  CMediaSource share;

  // User home folder
  #ifdef TARGET_DARWIN_IOS
    share.strPath = "special://envhome/";
  #else
    share.strPath = getenv("HOME");
  #endif
  share.strName = g_localizeStrings.Get(21440);
  share.m_ignore = true;
  localDrives.push_back(share);

#if defined(TARGET_DARWIN_IOS)
  // iOS Inbox folder
  share.strPath = "special://envhome/Documents/Inbox";
  share.strName = "Inbox";
  share.m_ignore = true;
  localDrives.push_back(share);
#endif

#if defined(TARGET_DARWIN_OSX)
  // User desktop folder
  share.strPath = getenv("HOME");
  share.strPath += "/Desktop";
  share.strName = "Desktop";
  share.m_ignore = true;
  localDrives.push_back(share);

  // Volumes (all mounts are present here)
  share.strPath = "/Volumes";
  share.strName = "Volumes";
  share.m_ignore = true;
  localDrives.push_back(share);

  // This will pick up all local non-removable disks including the Root Disk.
  DASessionRef session = DASessionCreate(kCFAllocatorDefault);
  if (session)
  {
    unsigned i, count = 0;
    struct statfs *buf = NULL;
    std::string mountpoint, devicepath;

    count = getmntinfo(&buf, 0);
    for (i=0; i<count; i++)
    {
      mountpoint = buf[i].f_mntonname;
      devicepath = buf[i].f_mntfromname;

      DADiskRef disk = DADiskCreateFromBSDName(kCFAllocatorDefault, session, devicepath.c_str());
      if (disk)
      {
        CFDictionaryRef details = DADiskCopyDescription(disk);
        if (details)
        {
          if (kCFBooleanFalse == CFDictionaryGetValue(details, kDADiskDescriptionMediaRemovableKey))
          {
            CMediaSource share;

            share.strPath = mountpoint;
            Cocoa_GetVolumeNameFromMountPoint(mountpoint.c_str(), share.strName);
            share.m_ignore = true;
            localDrives.push_back(share);
          }
          CFRelease(details);
        }
        CFRelease(disk);
      }
    }

    CFRelease(session);
  }
#endif
}

void CDarwinStorageProvider::GetRemovableDrives(VECSOURCES &removableDrives)
{
#if defined(TARGET_DARWIN_OSX)

  DASessionRef session = DASessionCreate(kCFAllocatorDefault);
  if (session)
  {
    unsigned i, count = 0;
    struct statfs *buf = NULL;
    std::string mountpoint, devicepath;

    count = getmntinfo(&buf, 0);
    for (i=0; i<count; i++)
    {
      mountpoint = buf[i].f_mntonname;
      devicepath = buf[i].f_mntfromname;

      DADiskRef disk = DADiskCreateFromBSDName(kCFAllocatorDefault, session, devicepath.c_str());
      if (disk)
      {
        CFDictionaryRef details = DADiskCopyDescription(disk);
        if (details)
        {
          if (kCFBooleanTrue == CFDictionaryGetValue(details, kDADiskDescriptionMediaRemovableKey))
          {
            CMediaSource share;

            share.strPath = mountpoint;
            Cocoa_GetVolumeNameFromMountPoint(mountpoint.c_str(), share.strName);
            share.m_ignore = true;
            // detect if its a cd or dvd
            // needs to be ejectable
            if (kCFBooleanTrue == CFDictionaryGetValue(details, kDADiskDescriptionMediaEjectableKey))
            {
              CFStringRef mediaKind = (CFStringRef)CFDictionaryGetValue(details, kDADiskDescriptionMediaKindKey);
              // and either cd or dvd kind of media in it
              if (mediaKind != NULL &&
                  (CFStringCompare(mediaKind, CFSTR(kIOCDMediaClass), 0) == kCFCompareEqualTo ||
                  CFStringCompare(mediaKind, CFSTR(kIODVDMediaClass), 0) == kCFCompareEqualTo))
                share.m_iDriveType = CMediaSource::SOURCE_TYPE_DVD;
            }
            removableDrives.push_back(share);
          }
          CFRelease(details);
        }
        CFRelease(disk);
      }
    }

    CFRelease(session);
  }
#endif
}

std::vector<std::string> CDarwinStorageProvider::GetDiskUsage()
{
  std::vector<std::string> result;
  char line[1024];

#ifdef TARGET_DARWIN_IOS
  FILE* pipe = popen("df -ht hfs,apfs", "r");
#else
  FILE* pipe = popen("df -HT ufs,cd9660,hfs,apfs,udf", "r");
#endif

  if (pipe)
  {
    while (fgets(line, sizeof(line) - 1, pipe))
    {
      result.push_back(line);
    }
    pclose(pipe);
  }

  return result;
}

bool CDarwinStorageProvider::Eject(const std::string& mountpath)
{
  return false;
}

bool CDarwinStorageProvider::PumpDriveChangeEvents(IStorageEventsCallback *callback)
{
  bool event = m_event;
  m_event = false;
  return event;
}

void CDarwinStorageProvider::SetEvent(void)
{
  CDarwinStorageProvider::m_event = true;
}
