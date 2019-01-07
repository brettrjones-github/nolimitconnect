/*
 *  Copyright (C) 2005-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include "addons/kodi-addon-dev-kit/include/kodi/addon-instance/Visualization.h"
#include "addons/binary-addons/AddonInstanceHandler.h"

namespace ADDON
{

class CVisualization : public IAddonInstanceHandler
{
public:
  CVisualization(ADDON::BinaryAddonBasePtr addonBase, float x, float y, float w, float h);
  ~CVisualization() override;

  bool Start(int channels, int samplesPerSec, int bitsPerSample, const std::string& songName);
  void Stop();
  void AudioData(const float* audioData, int audioDataLength, float *freqData, int freqDataLength);
  bool IsDirty();
  void Render();
  void GetInfo(VIS_INFO *info);
  bool OnAction(VIS_ACTION action, const void *param);
  bool HasPresets();
  bool GetPresetList(std::vector<std::string>& vecpresets);
  int GetActivePreset();
  std::string GetActivePresetName();
  bool IsLocked();
 
private:
  std::string m_name; /*!< To add-on sended name */
  std::string m_presetsPath; /*!< To add-on sended preset path */
  std::string m_profilePath; /*!< To add-on sended profile path */
  std::vector<std::string> m_presets; /*!< cached preset list */

  AddonInstance_Visualization m_struct; /*!< Interface table who contains function addresses and fixed values */

  // Static function to transfer data from add-on to kodi
  static void transfer_preset(void* kodiInstance, const char* preset);
};

} /* namespace ADDON */
