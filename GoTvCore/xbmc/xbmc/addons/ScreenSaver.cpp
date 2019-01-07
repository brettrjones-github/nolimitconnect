/*
 *  Copyright (C) 2005-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "ScreenSaver.h"
#include "filesystem/SpecialProtocol.h"
#include "windowing/GraphicContext.h"
#include "windowing/WinSystem.h"
#include "utils/log.h"
#ifdef TARGET_WINDOWS
#include "rendering/dx/DeviceResources.h"
#endif

namespace ADDON
{

CScreenSaver::CScreenSaver(BinaryAddonBasePtr addonBase)
 : IAddonInstanceHandler(ADDON_INSTANCE_SCREENSAVER, addonBase)
{
  m_name = Name();
  m_presets = CSpecialProtocol::TranslatePath(Path());
  m_profile = CSpecialProtocol::TranslatePath(Profile());

  m_struct = {{0}};
#if defined(TARGET_WINDOWS) && !defined(HAVE_QT_GUI)
  m_struct.props.device = DX::DeviceResources::Get()->GetD3DContext();
#else
  m_struct.props.device = nullptr;
#endif
  m_struct.props.x = 0;
  m_struct.props.y = 0;
  m_struct.props.width = CServiceBroker::GetWinSystem()->GetGfxContext().GetWidth();
  m_struct.props.height = CServiceBroker::GetWinSystem()->GetGfxContext().GetHeight();
  m_struct.props.pixelRatio = CServiceBroker::GetWinSystem()->GetGfxContext().GetResInfo().fPixelRatio;
  m_struct.props.name = m_name.c_str();
  m_struct.props.presets = m_presets.c_str();
  m_struct.props.profile = m_profile.c_str();

  m_struct.toKodi.kodiInstance = this;

  /* Open the class "kodi::addon::CInstanceScreensaver" on add-on side */
  if (CreateInstance(&m_struct) != ADDON_STATUS_OK)
    CLog::Log(LOGFATAL, "Screensaver: failed to create instance for '%s' and not usable!", ID().c_str());
}

CScreenSaver::~CScreenSaver()
{
  /* Destroy the class "kodi::addon::CInstanceScreensaver" on add-on side */
  DestroyInstance();
}

bool CScreenSaver::Start()
{
  if (m_struct.toAddon.Start)
    return m_struct.toAddon.Start(&m_struct);
  return false;
}

void CScreenSaver::Stop()
{
  if (m_struct.toAddon.Stop)
    m_struct.toAddon.Stop(&m_struct);
}

void CScreenSaver::Render()
{
  if (m_struct.toAddon.Render)
    m_struct.toAddon.Render(&m_struct);
}

} /* namespace ADDON */
