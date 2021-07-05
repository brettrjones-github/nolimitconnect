/*
 *      Copyright (c) 2002 Frodo
 *      Portions Copyright (c) by the authors of ffmpeg and xvid
 *  Copyright (C) 2002-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

// IoSupport.cpp: implementation of the CIoSupport class.
//
//////////////////////////////////////////////////////////////////////

#include "IoSupport.h"
#include "utils/log.h"
#ifdef TARGET_WINDOWS
#include "my_ntddcdrm.h"
#include "platform/win32/CharsetConverter.h"
#endif
#if defined(TARGET_LINUX)
#include <linux/limits.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/cdrom.h>
#endif
#if defined(TARGET_DARWIN)
#include <sys/param.h>
#include <mach-o/dyld.h>
#if defined(TARGET_DARWIN_OSX)
#include <IOKit/IOKitLib.h>
#include <IOKit/IOBSD.h>
#include <IOKit/storage/IOCDTypes.h>
#include <IOKit/storage/IODVDTypes.h>
#include <IOKit/storage/IOMedia.h>
#include <IOKit/storage/IOCDMedia.h>
#include <IOKit/storage/IODVDMedia.h>
#include <IOKit/storage/IOCDMediaBSDClient.h>
#include <IOKit/storage/IODVDMediaBSDClient.h>
#include <IOKit/storage/IOStorageDeviceCharacteristics.h>
#endif
#endif
#ifdef TARGET_FREEBSD
#include <sys/syslimits.h>
#endif
#include "cdioSupport.h"
#include "MediaManager.h"
#ifdef TARGET_POSIX
#include "XHandle.h"
#include "XFileUtils.h"
#endif

#ifdef TARGET_ANDROID
#include <unistd.h>
#endif

#ifdef HAS_DVD_DRIVE
using namespace MEDIA_DETECT;
#endif

void* CIoSupport::m_rawXferBuffer;

HANDLE CIoSupport::OpenCDROM()
{
  HANDLE hDevice = 0;

#ifdef HAS_DVD_DRIVE
#if defined(TARGET_POSIX)
  int fd = open(CLibcdio::GetInstance()->GetDeviceFileName(), O_RDONLY | O_NONBLOCK);
  hDevice = new CXHandle(CXHandle::HND_FILE);
  hDevice->fd = fd;
  hDevice->m_bCDROM = true;
#elif defined(TARGET_WINDOWS)
  auto filename = KODI::PLATFORM::WINDOWS::ToW(g_mediaManager.TranslateDevicePath("", true));
  hDevice = CreateFileW(filename.c_str(), GENERIC_READ, FILE_SHARE_READ,
                       nullptr, OPEN_EXISTING,
                       FILE_FLAG_RANDOM_ACCESS, nullptr );
#else

  hDevice = CreateFile("\\\\.\\Cdrom0", GENERIC_READ, FILE_SHARE_READ,
                       NULL, OPEN_EXISTING,
                       FILE_FLAG_RANDOM_ACCESS, NULL );

#endif
#endif
  return hDevice;
}

void CIoSupport::AllocReadBuffer()
{
#ifndef TARGET_POSIX
  m_rawXferBuffer = GlobalAlloc(GPTR, RAW_SECTOR_SIZE);
#endif
}

void CIoSupport::FreeReadBuffer()
{
#ifndef TARGET_POSIX
  GlobalFree(m_rawXferBuffer);
#endif
}

int CIoSupport::ReadSector(HANDLE hDevice, DWORD dwSector, char* lpczBuffer)

{
  DWORD dwRead;
  DWORD dwSectorSize = 2048;

#if defined(TARGET_DARWIN) && defined(HAS_DVD_DRIVE)
  dk_cd_read_t cd_read;
  memset( &cd_read, 0, sizeof(cd_read) );

  cd_read.sectorArea  = kCDSectorAreaUser;
  cd_read.buffer      = lpczBuffer;

  cd_read.sectorType  = kCDSectorTypeMode1;
  cd_read.offset      = dwSector * kCDSectorSizeMode1;

  cd_read.bufferLength = 2048;

  if( ioctl(hDevice->fd, DKIOCCDREAD, &cd_read ) == -1 )
  {
    return -1;
  }
  return 2048;
#elif defined(TARGET_POSIX)
  if (hDevice->m_bCDROM)
  {
    int fd = hDevice->fd;

    // seek to requested sector
    off_t offset = (off_t)dwSector * (off_t)MODE1_DATA_SIZE;

    if (lseek(fd, offset, SEEK_SET) < 0)
    {
      CLog::Log(LOGERROR, "CD: ReadSector Request to read sector %d\n", (int)dwSector);
      CLog::Log(LOGERROR, "CD: ReadSector error: %s\n", strerror(errno));
      return (-1);
    }

    // read data block of this sector
    while (read(fd, lpczBuffer, MODE1_DATA_SIZE) < 0)
    {
      // read was interrupted - try again
      if (errno == EINTR)
        continue;

      // error reading sector
      CLog::Log(LOGERROR, "CD: ReadSector Request to read sector %d\n", (int)dwSector);
      CLog::Log(LOGERROR, "CD: ReadSector error: %s\n", strerror(errno));
      return (-1);
    }

    return MODE1_DATA_SIZE;
  }
#endif
  LARGE_INTEGER Displacement;
  Displacement.QuadPart = static_cast<long long>(dwSector) * dwSectorSize;

  for (int i = 0; i < 5; i++)
  {
    if (SetFilePointer(hDevice, Displacement.u.LowPart, &Displacement.u.HighPart, FILE_BEGIN) != (DWORD)-1)
    {
      if (ReadFile(hDevice, m_rawXferBuffer, dwSectorSize, &dwRead, NULL))
      {
        memcpy(lpczBuffer, m_rawXferBuffer, dwSectorSize);
        return dwRead;
      }
    }
  }

  CLog::Log(LOGERROR, "%s: CD Read error", __FUNCTION__);
  return -1;
}


int CIoSupport::ReadSectorMode2(HANDLE hDevice, DWORD dwSector, char* lpczBuffer)
{
#ifdef HAS_DVD_DRIVE
#if defined(TARGET_DARWIN)
  dk_cd_read_t cd_read;

  memset( &cd_read, 0, sizeof(cd_read) );

  cd_read.sectorArea = kCDSectorAreaUser;
  cd_read.buffer = lpczBuffer;

  cd_read.offset       = dwSector * kCDSectorSizeMode2Form2;
  cd_read.sectorType   = kCDSectorTypeMode2Form2;
  cd_read.bufferLength = kCDSectorSizeMode2Form2;

  if( ioctl( hDevice->fd, DKIOCCDREAD, &cd_read ) == -1 )
  {
    return -1;
  }
  return MODE2_DATA_SIZE;
#elif defined(TARGET_FREEBSD)
  // NYI
#elif defined(TARGET_POSIX)
  if (hDevice->m_bCDROM)
  {
    int fd = hDevice->fd;
    int lba = (dwSector + CD_MSF_OFFSET) ;
    int m,s,f;
    union
    {
      struct cdrom_msf msf;
      char buffer[2356];
    } arg;

    // convert sector offset to minute, second, frame format
    // since that is what the 'ioctl' requires as input
    f = lba % CD_FRAMES;
    lba /= CD_FRAMES;
    s = lba % CD_SECS;
    lba /= CD_SECS;
    m = lba;

    arg.msf.cdmsf_min0 = m;
    arg.msf.cdmsf_sec0 = s;
    arg.msf.cdmsf_frame0 = f;

    int ret = ioctl(fd, CDROMREADMODE2, &arg);
    if (ret==0)
    {
      memcpy(lpczBuffer, arg.buffer, MODE2_DATA_SIZE); // don't think offset is needed here
      return MODE2_DATA_SIZE;
    }
    CLog::Log(LOGERROR, "CD: ReadSectorMode2 Request to read sector %d\n", (int)dwSector);
    CLog::Log(LOGERROR, "CD: ReadSectorMode2 error: %s\n", strerror(errno));
    CLog::Log(LOGERROR, "CD: ReadSectorMode2 minute %d, second %d, frame %d\n", m, s, f);
    return -1;
  }
#elif defined(TARGET_WINDOWS_STORE)
  CLog::Log(LOGDEBUG, "%s is not implemented", __FUNCTION__);
#else
  DWORD dwBytesReturned;
  RAW_READ_INFO rawRead = {0};

  // Oddly enough, DiskOffset uses the Red Book sector size
  rawRead.DiskOffset.QuadPart = 2048 * dwSector;
  rawRead.SectorCount = 1;
  rawRead.TrackMode = XAForm2;


  for (int i = 0; i < 5; i++)
  {
    if ( DeviceIoControl( hDevice,
                          IOCTL_CDROM_RAW_READ,
                          &rawRead,
                          sizeof(RAW_READ_INFO),
                          m_rawXferBuffer,
                          RAW_SECTOR_SIZE,
                          &dwBytesReturned,
                          NULL ) != 0 )
    {
      memcpy(lpczBuffer, (char*)m_rawXferBuffer+MODE2_DATA_START, MODE2_DATA_SIZE);
      return MODE2_DATA_SIZE;
    }
    else
    {
      int iErr = GetLastError();
    }
  }
#endif
#endif
  return -1;
}

int CIoSupport::ReadSectorCDDA(HANDLE hDevice, DWORD dwSector, char* lpczBuffer)
{
  return -1;
}

void CIoSupport::CloseCDROM(HANDLE hDevice)
{
  CloseHandle(hDevice);
}

