/*
 *  Copyright (C) 2005-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
*
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
*/
#include "LanguageResource.h"
#include "LangInfoKodi.h"
#include "ServiceBroker.h"
#include "addons/AddonManager.h"
#include "guilib/GUIWindowManager.h"
#include "settings/Settings.h"
#include "settings/SettingsComponent.h"
#include "utils/StringUtils.h"
#include "utils/Variant.h"
#include "messaging/helpers/DialogHelper.h"
#include "Skin.h"

using namespace KODI::MESSAGING;

using KODI::MESSAGING::HELPERS::DialogResponse;

#define LANGUAGE_ADDON_PREFIX   "resource.language."

namespace ADDON
{

std::unique_ptr<CLanguageResource> CLanguageResource::FromExtension(CAddonInfo addonInfo, const cp_extension_t* ext)
{
  // parse <extension> attributes
  CLocale locale = CLocale::FromString(CServiceBroker::GetAddonMgr().GetExtValue(ext->configuration, "@locale"));

  // parse <charsets>
  std::string charsetGui;
  bool forceUnicodeFont(false);
  std::string charsetSubtitle;
  cp_cfg_element_t *charsetsElement = CServiceBroker::GetAddonMgr().GetExtElement(ext->configuration, "charsets");
  if (charsetsElement != NULL)
  {
    charsetGui = CServiceBroker::GetAddonMgr().GetExtValue(charsetsElement, "gui");
    forceUnicodeFont = CServiceBroker::GetAddonMgr().GetExtValue(charsetsElement, "gui@unicodefont") == "true";
    charsetSubtitle = CServiceBroker::GetAddonMgr().GetExtValue(charsetsElement, "subtitle");
  }

  // parse <dvd>
  std::string dvdLanguageMenu;
  std::string dvdLanguageAudio;
  std::string dvdLanguageSubtitle;
  cp_cfg_element_t *dvdElement = CServiceBroker::GetAddonMgr().GetExtElement(ext->configuration, "dvd");
  if (dvdElement != NULL)
  {
    dvdLanguageMenu = CServiceBroker::GetAddonMgr().GetExtValue(dvdElement, "menu");
    dvdLanguageAudio = CServiceBroker::GetAddonMgr().GetExtValue(dvdElement, "audio");
    dvdLanguageSubtitle = CServiceBroker::GetAddonMgr().GetExtValue(dvdElement, "subtitle");
  }
  // fall back to the language of the addon if a DVD language is not defined
  if (dvdLanguageMenu.empty())
    dvdLanguageMenu = locale.GetLanguageCode();
  if (dvdLanguageAudio.empty())
    dvdLanguageAudio = locale.GetLanguageCode();
  if (dvdLanguageSubtitle.empty())
    dvdLanguageSubtitle = locale.GetLanguageCode();

  // parse <sorttokens>
  std::set<std::string> sortTokens;
  cp_cfg_element_t *sorttokensElement = CServiceBroker::GetAddonMgr().GetExtElement(ext->configuration, "sorttokens");
  if (sorttokensElement != NULL)
  {
    for (size_t i = 0; i < sorttokensElement->num_children; ++i)
    {
      cp_cfg_element_t &tokenElement = sorttokensElement->children[i];
      if (tokenElement.name != NULL && strcmp(tokenElement.name, "token") == 0 &&
          tokenElement.value != NULL)
      {
        std::string token(tokenElement.value);
        std::string separators = CServiceBroker::GetAddonMgr().GetExtValue(&tokenElement, "@separators");
        if (separators.empty())
          separators = " ._";

        for (std::string::const_iterator separator = separators.begin(); separator != separators.end(); ++separator)
          sortTokens.insert(token + *separator);
      }
    }
  }
  return std::unique_ptr<CLanguageResource>(new CLanguageResource(
      std::move(addonInfo),
      locale,
      charsetGui,
      forceUnicodeFont,
      charsetSubtitle,
      dvdLanguageMenu,
      dvdLanguageAudio,
      dvdLanguageSubtitle,
      sortTokens));
}

CLanguageResource::CLanguageResource(
    CAddonInfo addonInfo,
    const CLocale& locale,
    const std::string& charsetGui,
    bool forceUnicodeFont,
    const std::string& charsetSubtitle,
    const std::string& dvdLanguageMenu,
    const std::string& dvdLanguageAudio,
    const std::string& dvdLanguageSubtitle,
    const std::set<std::string>& sortTokens)
  : CResource(std::move(addonInfo)),
    m_locale(locale),
    m_charsetGui(charsetGui),
    m_forceUnicodeFont(forceUnicodeFont),
    m_charsetSubtitle(charsetSubtitle),
    m_dvdLanguageMenu(dvdLanguageMenu),
    m_dvdLanguageAudio(dvdLanguageAudio),
    m_dvdLanguageSubtitle(dvdLanguageSubtitle),
    m_sortTokens(sortTokens)
{ }

bool CLanguageResource::IsInUse() const
{
  return StringUtils::EqualsNoCase(CServiceBroker::GetSettingsComponent()->GetSettings()->GetString(CSettings::SETTING_LOCALE_LANGUAGE), ID());
}

void CLanguageResource::OnPostInstall(bool update, bool modal)
{
  if (!g_SkinInfo)
    return;

  if (IsInUse() ||
     (!update && !modal && 
       (HELPERS::ShowYesNoDialogText(CVariant{Name()}, CVariant{24132}) == DialogResponse::YES)))
  {
    if (IsInUse())
      g_langInfo.SetLanguage(ID());
    else
      CServiceBroker::GetSettingsComponent()->GetSettings()->SetString(CSettings::SETTING_LOCALE_LANGUAGE, ID());
  }
}

bool CLanguageResource::IsAllowed(const std::string &file) const
{
  return file.empty() ||
         StringUtils::EqualsNoCase(file.c_str(), "langinfo.xml") ||
         StringUtils::EqualsNoCase(file.c_str(), "strings.po") ||
         StringUtils::EqualsNoCase(file.c_str(), "strings.xml");
}

std::string CLanguageResource::GetAddonId(const std::string& locale)
{
  if (locale.empty())
    return "";

  std::string addonId = locale;
  if (!StringUtils::StartsWith(addonId, LANGUAGE_ADDON_PREFIX))
    addonId = LANGUAGE_ADDON_PREFIX + locale;

  StringUtils::ToLower(addonId);
  return addonId;
}

bool CLanguageResource::FindLegacyLanguage(const std::string &locale, std::string &legacyLanguage)
{
  if (locale.empty())
    return false;

  std::string addonId = GetAddonId(locale);

  AddonPtr addon;
  if (!CServiceBroker::GetAddonMgr().GetAddon(addonId, addon, ADDON_RESOURCE_LANGUAGE, true))
    return false;

  legacyLanguage = addon->Name();
  return true;
}

}
