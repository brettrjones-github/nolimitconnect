/*
 *  Copyright (C) 2005-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "IPowerSyscall.h"

class CFileItem;
class CSetting;
class CSettings;

// This class will wrap and handle PowerSyscalls.
// It will handle and decide if syscalls are needed.
class CPowerManager : public IPowerEventsCallback
{
public:
  CPowerManager();
  ~CPowerManager() override;

  void Initialize();
  void SetDefaults();

  bool Powerdown();
  bool Suspend();
  bool Hibernate();
  bool Reboot();

  bool CanPowerdown();
  bool CanSuspend();
  bool CanHibernate();
  bool CanReboot();

  int  BatteryLevel();

  void ProcessEvents();

  static void SettingOptionsShutdownStatesFiller(std::shared_ptr<const CSetting> setting, std::vector< std::pair<std::string, int> > &list, int &current, void *data);

private:
  void OnSleep() override;
  void OnWake() override;
  void OnLowBattery() override;
  void RestorePlayerState();
  void StorePlayerState();

  // Construction parameters
  std::shared_ptr<CSettings> m_settings;

  std::unique_ptr<IPowerSyscall> m_instance;
  std::unique_ptr<CFileItem> m_lastPlayedFileItem;
  std::string m_lastUsedPlayer;
};
