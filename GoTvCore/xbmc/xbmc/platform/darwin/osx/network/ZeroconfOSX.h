/*
 *  Copyright (C) 2005-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include <memory>
#include <vector>

#include "network/Zeroconf.h"
#include "threads/CriticalSection.h"

#include <CoreFoundation/CoreFoundation.h>
#if defined(TARGET_DARWIN_OSX)
  #include <CoreServices/CoreServices.h>
#else
  #include <CFNetwork/CFNetServices.h>
#endif

class CZeroconfOSX : public CZeroconf
{
public:
  CZeroconfOSX();
  ~CZeroconfOSX();
protected:
  //implement base CZeroConf interface
  bool doPublishService(const std::string& fcr_identifier,
                        const std::string& fcr_type,
                        const std::string& fcr_name,
                        unsigned int f_port,
                        const std::vector<std::pair<std::string, std::string> >& txt);

  bool doForceReAnnounceService(const std::string& fcr_identifier);

  bool doRemoveService(const std::string& fcr_ident);

  virtual void doStop();

private:
  static void registerCallback(CFNetServiceRef theService, CFStreamError* error, void* info);
  void cancelRegistration(CFNetServiceRef theService);

  //CF runloop ref; we're using main-threads runloop
  CFRunLoopRef m_runloop;

  //lock + data (accessed from runloop(main thread) + the rest)
  CCriticalSection m_data_guard;
  typedef std::map<std::string, CFNetServiceRef> tServiceMap;
  tServiceMap m_services;
};
