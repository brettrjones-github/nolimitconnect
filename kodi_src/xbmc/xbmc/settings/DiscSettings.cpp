/*
 *  Copyright (C) 2017-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include <string>

#include "DiscSettings.h"
#include "Settings.h"
#include "settings/lib/Setting.h"
#include "libbluray/DllLibbluray.h"
#include "ServiceBroker.h"
#include "dialogs/GUIDialogKaiToast.h"
#include "guilib/LocalizeStrings.h"
#include "utils/log.h"
#include "messaging/helpers/DialogOKHelper.h" 
#include "utils/Variant.h"

#include <libbluray/src/libbluray/bluray-version.h>

using namespace KODI::MESSAGING;

CDiscSettings::CDiscSettings(void)
{

  m_dll = new DllLibbluray;
  if (!m_dll->Load())
  {
    delete m_dll;
    m_dll = NULL;
  }
}


CDiscSettings::~CDiscSettings()
{
  delete m_dll;
}

CDiscSettings& CDiscSettings::GetInstance()
{
  static CDiscSettings sDiscSettings;
  return sDiscSettings;
}


void CDiscSettings::OnSettingChanged(std::shared_ptr<const CSetting> setting)
{
#if (BLURAY_VERSION >= BLURAY_VERSION_CODE(1,0,1))
  if (setting == NULL)
    return;

  const std::string &settingId = setting->GetId();

  if (settingId == CSettings::SETTING_DISC_PLAYBACK)
  {
    int mode = std::static_pointer_cast<const CSettingInt>(setting)->GetValue();
    if (mode == BD_PLAYBACK_DISC_MENU)
    {
      BLURAY* bd = m_dll->bd_init();
      const BLURAY_DISC_INFO* info = m_dll->bd_get_disc_info(bd);
      if (!info->bdj_handled)
      {
        if (!info->libjvm_detected)
        {
          CLog::Log(LOGDEBUG, "DiscSettings - Could not load the java vm.");
          HELPERS::ShowOKDialogText(CVariant{ 29803 }, CVariant{ 29804 });
        }
        CLog::Log(LOGDEBUG, "DiscSettings - Could not load the libbluray.jar.");
      }
      m_dll->bd_close(bd);
    }
  }
#endif
}
