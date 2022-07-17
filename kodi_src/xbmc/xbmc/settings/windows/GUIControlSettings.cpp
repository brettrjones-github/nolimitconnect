/*
 *  Copyright (C) 2005-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "GUIControlSettings.h"

#include <set>
#include <utility>

#include "ServiceBroker.h"
#include "addons/AddonManager.h"
#include "addons/GUIWindowAddonBrowser.h"
#include "addons/settings/SettingUrlEncodedString.h"
#include "dialogs/GUIDialogFileBrowser.h"
#include "dialogs/GUIDialogNumeric.h"
#include "dialogs/GUIDialogSelect.h"
#include "dialogs/GUIDialogSlider.h"
#include "FileItem.h"
#include "guilib/GUIComponent.h"
#include "guilib/GUIEditControl.h"
#include "guilib/GUIImage.h"
#include "guilib/GUILabelControl.h"
#include "guilib/GUIRadioButtonControl.h"
#include "guilib/GUISettingsSliderControl.h"
#include "guilib/GUISpinControlEx.h"
#include "guilib/GUIWindowManager.h"
#include "guilib/LocalizeStrings.h"
#include "settings/lib/Setting.h"
#include "settings/MediaSourceSettings.h"
#include "settings/SettingAddon.h"
#include "settings/SettingControl.h"
#include "settings/SettingDateTime.h"
#include "settings/SettingPath.h"
#include "settings/SettingUtils.h"
#include "storage/MediaManager.h"
#include "NlcCoreUtil.h"
#include "utils/StringUtils.h"
#include "utils/Variant.h"

using namespace ADDON;

static std::string Localize(std::uint32_t code, ILocalizer* localizer)
{
  if (localizer == nullptr)
    return "";

  return localizer->Localize(code);
}

template<typename TValueType>
static CFileItemPtr GetFileItem(const std::string& label, const TValueType& value, const std::set<TValueType>& selectedValues)
{
  CFileItemPtr item(new CFileItem(label));
  item->SetProperty("value", value);

  if (selectedValues.find(value) != selectedValues.end())
    item->Select(true);

  return item;
}

static bool GetIntegerOptions(SettingConstPtr setting, IntegerSettingOptions& options, std::set<int>& selectedOptions, ILocalizer* localizer)
{
  std::shared_ptr<const CSettingInt> pSettingInt = NULL;
  if (setting->GetType() == SettingType::Integer)
  {
    pSettingInt = std::static_pointer_cast<const CSettingInt>(setting);
    selectedOptions.insert(pSettingInt->GetValue());
  }
  else if (setting->GetType() == SettingType::List)
  {
    std::shared_ptr<const CSettingList> settingList = std::static_pointer_cast<const CSettingList>(setting);
    if (settingList->GetElementType() != SettingType::Integer)
      return false;

    pSettingInt = std::static_pointer_cast<const CSettingInt>(settingList->GetDefinition());
    std::vector<CVariant> list = CSettingUtils::GetList(settingList);
    for (const auto& itValue : list)
    {
      if (!itValue.isInteger())
        return false;
      selectedOptions.insert((int)itValue.asInteger());
    }
  }
  else
    return false;

  switch (pSettingInt->GetOptionsType())
  {
    case SettingOptionsType::StaticTranslatable:
    {
      const TranslatableIntegerSettingOptions& settingOptions = pSettingInt->GetTranslatableOptions();
      for (const auto& option : settingOptions)
        options.push_back(std::make_pair(Localize(option.first, localizer), option.second));
      break;
    }

    case SettingOptionsType::Static:
    {
      const IntegerSettingOptions& settingOptions = pSettingInt->GetOptions();
      options.insert(options.end(), settingOptions.begin(), settingOptions.end());
      break;
    }

    case SettingOptionsType::Dynamic:
    {
      IntegerSettingOptions settingOptions = std::const_pointer_cast<CSettingInt>(pSettingInt)->UpdateDynamicOptions();
      options.insert(options.end(), settingOptions.begin(), settingOptions.end());
      break;
    }

    case SettingOptionsType::Unknown:
    default:
    {
      std::shared_ptr<const CSettingControlFormattedRange> control = std::static_pointer_cast<const CSettingControlFormattedRange>(pSettingInt->GetControl());
      for (int i = pSettingInt->GetMinimum(); i <= pSettingInt->GetMaximum(); i += pSettingInt->GetStep())
      {
        std::string strLabel;
        if (i == pSettingInt->GetMinimum() && control->GetMinimumLabel() > -1)
          strLabel = Localize(control->GetMinimumLabel(), localizer);
        else if (control->GetFormatLabel() > -1)
          strLabel = StringUtils::Format(Localize(control->GetFormatLabel(), localizer).c_str(), i);
        else
          strLabel = StringUtils::Format(control->GetFormatString().c_str(), i);

        options.push_back(std::make_pair(strLabel, i));
      }

      break;
    }
  }

  return true;
}

static bool GetStringOptions(SettingConstPtr setting, StringSettingOptions& options, std::set<std::string>& selectedOptions, ILocalizer* localizer)
{
  std::shared_ptr<const CSettingString> pSettingString = NULL;
  if (setting->GetType() == SettingType::String)
  {
    pSettingString = std::static_pointer_cast<const CSettingString>(setting);
    selectedOptions.insert(pSettingString->GetValue());
  }
  else if (setting->GetType() == SettingType::List)
  {
    std::shared_ptr<const CSettingList>settingList = std::static_pointer_cast<const CSettingList>(setting);
    if (settingList->GetElementType() != SettingType::String)
      return false;

    pSettingString = std::static_pointer_cast<const CSettingString>(settingList->GetDefinition());
    std::vector<CVariant> list = CSettingUtils::GetList(settingList);
    for (const auto& itValue : list)
    {
      if (!itValue.isString())
        return false;
      selectedOptions.insert(itValue.asString());
    }
  }
  else
    return false;

  switch (pSettingString->GetOptionsType())
  {
    case SettingOptionsType::StaticTranslatable:
    {
      const TranslatableStringSettingOptions& settingOptions = pSettingString->GetTranslatableOptions();
      for (const auto& option : settingOptions)
        options.push_back(std::make_pair(Localize(option.first, localizer), option.second));
      break;
    }

    case SettingOptionsType::Static:
    {
      const StringSettingOptions& settingOptions = pSettingString->GetOptions();
      options.insert(options.end(), settingOptions.begin(), settingOptions.end());
      break;
    }

    case SettingOptionsType::Dynamic:
    {
      StringSettingOptions settingOptions = std::const_pointer_cast<CSettingString>(pSettingString)->UpdateDynamicOptions();
      options.insert(options.end(), settingOptions.begin(), settingOptions.end());
      break;
    }

    case SettingOptionsType::Unknown:
    default:
      return false;
  }

  return true;
}

CGUIControlBaseSetting::CGUIControlBaseSetting(int id, std::shared_ptr<CSetting> pSetting, ILocalizer* localizer)
  : m_id(id),
    m_pSetting(pSetting),
    m_localizer(localizer),
    m_delayed(false),
    m_valid(true)
{ }

bool CGUIControlBaseSetting::IsEnabled() const
{
  return m_pSetting != NULL && m_pSetting->IsEnabled();
}

void CGUIControlBaseSetting::Update(bool updateDisplayOnly /* = false */)
{
  if (updateDisplayOnly)
    return;

  CGUIControl *control = GetControl();
  if (control == NULL)
    return;

  control->SetEnabled(IsEnabled());
  if (m_pSetting)
    control->SetVisible(m_pSetting->IsVisible());
  SetValid(true);
}

std::string CGUIControlBaseSetting::Localize(std::uint32_t code) const
{
  return ::Localize(code, m_localizer);
}

CGUIControlRadioButtonSetting::CGUIControlRadioButtonSetting(CGUIRadioButtonControl *pRadioButton, int id, std::shared_ptr<CSetting> pSetting, ILocalizer* localizer)
  : CGUIControlBaseSetting(id, pSetting, localizer)
{
  m_pRadioButton = pRadioButton;
  if (m_pRadioButton == NULL)
    return;

  m_pRadioButton->SetID(id);
  Update();
}

CGUIControlRadioButtonSetting::~CGUIControlRadioButtonSetting() = default;

bool CGUIControlRadioButtonSetting::OnClick()
{
  SetValid(std::static_pointer_cast<CSettingBool>(m_pSetting)->SetValue(!std::static_pointer_cast<CSettingBool>(m_pSetting)->GetValue()));
  return IsValid();
}

void CGUIControlRadioButtonSetting::Update(bool updateDisplayOnly /* = false */)
{
  if (m_pRadioButton == NULL)
    return;

  CGUIControlBaseSetting::Update();

  m_pRadioButton->SetSelected(std::static_pointer_cast<CSettingBool>(m_pSetting)->GetValue());
}

CGUIControlSpinExSetting::CGUIControlSpinExSetting(CGUISpinControlEx *pSpin, int id, std::shared_ptr<CSetting> pSetting, ILocalizer* localizer)
  : CGUIControlBaseSetting(id, pSetting, localizer)
{
  m_pSpin = pSpin;
  if (m_pSpin == NULL)
    return;

  m_pSpin->SetID(id);
  
  FillControl();
}

CGUIControlSpinExSetting::~CGUIControlSpinExSetting() = default;

bool CGUIControlSpinExSetting::OnClick()
{
  if (m_pSpin == NULL)
    return false;

  switch (m_pSetting->GetType())
  {
    case SettingType::Integer:
      SetValid(std::static_pointer_cast<CSettingInt>(m_pSetting)->SetValue(m_pSpin->GetValue()));
      break;

    case SettingType::Number:
    {
      auto pSettingNumber = std::static_pointer_cast<CSettingNumber>(m_pSetting);
      const auto& controlFormat = m_pSetting->GetControl()->GetFormat();
      if (controlFormat == "number")
        SetValid(pSettingNumber->SetValue(m_pSpin->GetFloatValue()));
      else
        SetValid(pSettingNumber->SetValue(pSettingNumber->GetMinimum() + pSettingNumber->GetStep() * m_pSpin->GetValue()));

      break;
    }
    
    case SettingType::String:
      SetValid(std::static_pointer_cast<CSettingString>(m_pSetting)->SetValue(m_pSpin->GetStringValue()));
      break;
    
    default:
      return false;
  }
  
  return IsValid();
}

void CGUIControlSpinExSetting::Update(bool updateDisplayOnly /* = false */)
{
  if (updateDisplayOnly || m_pSpin == NULL)
    return;

  CGUIControlBaseSetting::Update();

  FillControl();

  // disable the spinner if it has less than two items
  if (!m_pSpin->IsDisabled() && (m_pSpin->GetMaximum() - m_pSpin->GetMinimum()) == 0)
    m_pSpin->SetEnabled(false);
}

void CGUIControlSpinExSetting::FillControl()
{
  if (m_pSpin == NULL)
    return;

  m_pSpin->Clear();

  const std::string &controlFormat = m_pSetting->GetControl()->GetFormat();
  if (controlFormat == "number")
  {
    std::shared_ptr<CSettingNumber> pSettingNumber = std::static_pointer_cast<CSettingNumber>(m_pSetting);
    m_pSpin->SetType(SPIN_CONTROL_TYPE_FLOAT);
    m_pSpin->SetFloatRange((float)pSettingNumber->GetMinimum(), (float)pSettingNumber->GetMaximum());
    m_pSpin->SetFloatInterval((float)pSettingNumber->GetStep());

    m_pSpin->SetFloatValue((float)pSettingNumber->GetValue());
  }
  else if (controlFormat == "integer")
  {
    m_pSpin->SetType(SPIN_CONTROL_TYPE_TEXT);
    FillIntegerSettingControl();
  }
  else if (controlFormat == "string")
  {
    m_pSpin->SetType(SPIN_CONTROL_TYPE_TEXT);

    if (m_pSetting->GetType() == SettingType::Integer)
      FillIntegerSettingControl();
    else if (m_pSetting->GetType() == SettingType::Number)
    {
      std::shared_ptr<CSettingNumber> pSettingNumber = std::static_pointer_cast<CSettingNumber>(m_pSetting);
      std::shared_ptr<const CSettingControlFormattedRange> control = std::static_pointer_cast<const CSettingControlFormattedRange>(m_pSetting->GetControl());
      int index = 0;
      int currentIndex = 0;
      for (double value = pSettingNumber->GetMinimum(); value <= pSettingNumber->GetMaximum(); value += pSettingNumber->GetStep(), index++)
      {
        if (value == pSettingNumber->GetValue())
          currentIndex = index;

        std::string strLabel;
        if (value == pSettingNumber->GetMinimum() && control->GetMinimumLabel() > -1)
          strLabel = Localize(control->GetMinimumLabel());
        else if (control->GetFormatLabel() > -1)
          strLabel = StringUtils::Format(Localize(control->GetFormatLabel()).c_str(), value);
        else
          strLabel = StringUtils::Format(control->GetFormatString().c_str(), value);

        m_pSpin->AddLabel(strLabel, index);
      }

      m_pSpin->SetValue(currentIndex);
    }
    else if (m_pSetting->GetType() == SettingType::String)
    {
      StringSettingOptions options;
      std::set<std::string> selectedValues;
      // get the string options
      if (!GetStringOptions(m_pSetting, options, selectedValues, m_localizer) || selectedValues.size() != 1)
        return;

      // add them to the spinner
      for (const auto& option : options)
        m_pSpin->AddLabel(option.first, option.second);

      // and set the current value
      m_pSpin->SetStringValue(*selectedValues.begin());
    }
  }
}

void CGUIControlSpinExSetting::FillIntegerSettingControl()
{
  IntegerSettingOptions options;
  std::set<int> selectedValues;
  // get the integer options
  if (!GetIntegerOptions(m_pSetting, options, selectedValues, m_localizer) || selectedValues.size() != 1)
    return;

  // add them to the spinner
  for (const auto& option : options)
    m_pSpin->AddLabel(option.first, option.second);

  // and set the current value
  m_pSpin->SetValue(*selectedValues.begin());
}

CGUIControlListSetting::CGUIControlListSetting(CGUIButtonControl *pButton, int id, std::shared_ptr<CSetting> pSetting, ILocalizer* localizer)
  : CGUIControlBaseSetting(id, pSetting, localizer)
{
  m_pButton = pButton;
  if (m_pButton == NULL)
    return;

  m_pButton->SetID(id);
  Update();
}

CGUIControlListSetting::~CGUIControlListSetting() = default;

bool CGUIControlListSetting::OnClick()
{
  if (m_pButton == NULL)
    return false;

  CGUIDialogSelect *dialog = CServiceBroker::GetGUI()->GetWindowManager().GetWindow<CGUIDialogSelect>(WINDOW_DIALOG_SELECT);
  if (dialog == NULL)
    return false;

  CFileItemList options;
  std::shared_ptr<const CSettingControlList> control = std::static_pointer_cast<const CSettingControlList>(m_pSetting->GetControl());
  if (!GetItems(m_pSetting, options) ||
      options.Size() <= 0 ||
     (!control->CanMultiSelect() && options.Size() <= 1))
    return false;
  
  dialog->Reset();
  dialog->SetHeading(CVariant{Localize(m_pSetting->GetLabel())});
  dialog->SetItems(options);
  dialog->SetMultiSelection(control->CanMultiSelect());
  dialog->Open();

  if (!dialog->IsConfirmed())
    return false;

  std::vector<CVariant> values;
  for (int i : dialog->GetSelectedItems())
  {
    const CFileItemPtr item = options.Get(i);
    if (item == NULL || !item->HasProperty("value"))
      return false;

    values.push_back(item->GetProperty("value"));
  }

  bool ret = false;
  switch (m_pSetting->GetType())
  {
    case SettingType::Integer:
      if (values.size() > 1)
        return false;
      ret = std::static_pointer_cast<CSettingInt>(m_pSetting)->SetValue((int)values.at(0).asInteger());
      break;

    case SettingType::String:
      if (values.size() > 1)
        return false;
      ret = std::static_pointer_cast<CSettingString>(m_pSetting)->SetValue(values.at(0).asString());
      break;

    case SettingType::List:
      ret = CSettingUtils::SetList(std::static_pointer_cast<CSettingList>(m_pSetting), values);
      break;
    
    default:
      return false;
  }

  if (ret)
    Update();
  else
    SetValid(false);

  return IsValid();
}

void CGUIControlListSetting::Update(bool updateDisplayOnly /* = false */)
{
  if (updateDisplayOnly || m_pButton == NULL)
    return;

  CGUIControlBaseSetting::Update();
  
  CFileItemList options;
  std::shared_ptr<const CSettingControlList> control = std::static_pointer_cast<const CSettingControlList>(m_pSetting->GetControl());
  bool optionsValid = GetItems(m_pSetting, options);
  std::string label2;
  if (optionsValid && !control->HideValue())
  {
    SettingControlListValueFormatter formatter = control->GetFormatter();
    if (formatter)
      label2 = formatter(m_pSetting);

    if (label2.empty())
    {
      std::vector<std::string> labels;
      for (int index = 0; index < options.Size(); index++)
      {
        const CFileItemPtr pItem = options.Get(index);
        if (pItem->IsSelected())
          labels.push_back(pItem->GetLabel());
      }

      label2 = StringUtils::Join(labels, ", ");
    }
  }

  m_pButton->SetLabel2(label2);

  // disable the control if
  // * there are no items to be chosen
  // * only one value can be chosen and there are less than two items available
  if (!m_pButton->IsDisabled() &&
     (options.Size() <= 0 ||
     (!control->CanMultiSelect() && options.Size() <= 1)))
    m_pButton->SetEnabled(false);
}

bool CGUIControlListSetting::GetItems(SettingConstPtr setting, CFileItemList &items) const
{
  std::shared_ptr<const CSettingControlList> control = std::static_pointer_cast<const CSettingControlList>(setting->GetControl());
  const std::string &controlFormat = control->GetFormat();

  if (controlFormat == "integer")
    return GetIntegerItems(setting, items);
  else if (controlFormat == "string")
  {
    if (setting->GetType() == SettingType::Integer ||
       (setting->GetType() == SettingType::List && std::static_pointer_cast<const CSettingList>(setting)->GetElementType() == SettingType::Integer))
      return GetIntegerItems(setting, items);
    else if (setting->GetType() == SettingType::String ||
            (setting->GetType() == SettingType::List && std::static_pointer_cast<const CSettingList>(setting)->GetElementType() == SettingType::String))
      return GetStringItems(setting, items);
  }
  else
    return false;

  return true;
}

bool CGUIControlListSetting::GetIntegerItems(SettingConstPtr setting, CFileItemList &items) const
{
  IntegerSettingOptions options;
  std::set<int> selectedValues;
  // get the integer options
  if (!GetIntegerOptions(setting, options, selectedValues, m_localizer))
    return false;

  // turn them into CFileItems and add them to the item list
  for (const auto& option : options)
    items.Add(GetFileItem(option.first, option.second, selectedValues));

  return true;
}

bool CGUIControlListSetting::GetStringItems(SettingConstPtr setting, CFileItemList &items) const
{
  StringSettingOptions options;
  std::set<std::string> selectedValues;
  // get the string options
  if (!GetStringOptions(setting, options, selectedValues, m_localizer))
    return false;

  // turn them into CFileItems and add them to the item list
  for (const auto& option : options)
    items.Add(GetFileItem(option.first, option.second, selectedValues));

  return true;
}

CGUIControlButtonSetting::CGUIControlButtonSetting(CGUIButtonControl *pButton, int id, std::shared_ptr<CSetting> pSetting, ILocalizer* localizer)
  : CGUIControlBaseSetting(id, pSetting, localizer)
{
  m_pButton = pButton;
  if (m_pButton == NULL)
    return;

  m_pButton->SetID(id);
  Update();
}

CGUIControlButtonSetting::~CGUIControlButtonSetting() = default;

bool CGUIControlButtonSetting::OnClick()
{
  if (m_pButton == NULL)
    return false;
  
  std::shared_ptr<const ISettingControl> control = m_pSetting->GetControl();
  const std::string &controlType = control->GetType();
  const std::string &controlFormat = control->GetFormat();
  if (controlType == "button")
  {
    std::shared_ptr<const CSettingControlButton> buttonControl = std::static_pointer_cast<const CSettingControlButton>(control);
    if (controlFormat == "addon")
    {
      // prompt for the addon
      std::shared_ptr<CSettingAddon> setting;
      std::vector<std::string> addonIDs;
      if (m_pSetting->GetType() == SettingType::List)
      {
        std::shared_ptr<CSettingList> settingList = std::static_pointer_cast<CSettingList>(m_pSetting);
        setting = std::static_pointer_cast<CSettingAddon>(settingList->GetDefinition());
        for (const SettingPtr addon : settingList->GetValue())
          addonIDs.push_back(std::static_pointer_cast<CSettingAddon>(addon)->GetValue());
      }
      else
      {
        setting = std::static_pointer_cast<CSettingAddon>(m_pSetting);
        addonIDs.push_back(setting->GetValue());
      }

      if (CGUIWindowAddonBrowser::SelectAddonID(setting->GetAddonType(), addonIDs, setting->AllowEmpty(),
                                                buttonControl->ShowAddonDetails(), m_pSetting->GetType() == SettingType::List,
                                                buttonControl->ShowInstalledAddons(), buttonControl->ShowInstallableAddons(),
                                                buttonControl->ShowMoreAddons()) != 1)
        return false;

      if (m_pSetting->GetType() == SettingType::List)
        std::static_pointer_cast<CSettingList>(m_pSetting)->FromString(addonIDs);
      else
        SetValid(setting->SetValue(addonIDs[0]));
    }
    else if (controlFormat == "path" || controlFormat == "file" || controlFormat == "image")
      SetValid(GetPath(std::static_pointer_cast<CSettingPath>(m_pSetting), m_localizer));
    else if (controlFormat == "date")
    {
      std::shared_ptr<CSettingDate> settingDate = std::static_pointer_cast<CSettingDate>(m_pSetting);

      SYSTEMTIME systemdate;
      settingDate->GetDate().GetAsSystemTime(systemdate);
      if (CGUIDialogNumeric::ShowAndGetDate(systemdate, Localize(buttonControl->GetHeading())))
        SetValid(settingDate->SetDate(CDateTime(systemdate)));
    }
    else if (controlFormat == "time")
    {
      std::shared_ptr<CSettingTime> settingTime = std::static_pointer_cast<CSettingTime>(m_pSetting);

      SYSTEMTIME systemtime;
      settingTime->GetTime().GetAsSystemTime(systemtime);

      if (CGUIDialogNumeric::ShowAndGetTime(systemtime, Localize(buttonControl->GetHeading())))
        SetValid(settingTime->SetTime(CDateTime(systemtime)));
    }
    else if (controlFormat == "action")
    {
      // simply call the OnSettingAction callback and whoever knows what to
      // do can do so (based on the setting's identification)
      m_pSetting->OnSettingAction(m_pSetting);
      SetValid(true);
    }
  }
  else if (controlType == "slider")
  {
    float value, min, step, max;
    if (m_pSetting->GetType() == SettingType::Integer)
    {
      std::shared_ptr<CSettingInt> settingInt = std::static_pointer_cast<CSettingInt>(m_pSetting);
      value = (float)settingInt->GetValue();
      min = (float)settingInt->GetMinimum();
      step = (float)settingInt->GetStep();
      max = (float)settingInt->GetMaximum();
    }
    else if (m_pSetting->GetType() == SettingType::Number)
    {
      std::shared_ptr<CSettingNumber> settingNumber = std::static_pointer_cast<CSettingNumber>(m_pSetting);
      value = (float)settingNumber->GetValue();
      min = (float)settingNumber->GetMinimum();
      step = (float)settingNumber->GetStep();
      max = (float)settingNumber->GetMaximum();
    }
    else
      return false;

    std::shared_ptr<const CSettingControlSlider> sliderControl = std::static_pointer_cast<const CSettingControlSlider>(control);
    CGUIDialogSlider::ShowAndGetInput(Localize(sliderControl->GetHeading()), value, min, step, max, this, NULL);
    SetValid(true);
  }

  // update the displayed value
  Update();

  return IsValid();
}

void CGUIControlButtonSetting::Update(bool updateDisplayOnly /* = false */)
{
  if (updateDisplayOnly || m_pButton == NULL)
    return;

  CGUIControlBaseSetting::Update();
  
  std::string strText;
  std::shared_ptr<const ISettingControl> control = m_pSetting->GetControl();
  const std::string &controlType = control->GetType();
  const std::string &controlFormat = control->GetFormat();

  if (controlType == "button")
  {
    if (!std::static_pointer_cast<const CSettingControlButton>(control)->HideValue())
    {
      switch (m_pSetting->GetType())
      {
      case SettingType::String:
      {
        std::string strValue = std::static_pointer_cast<CSettingString>(m_pSetting)->GetValue();
        if (controlFormat == "addon")
        {
          ADDON::AddonPtr addon;
          if (CServiceBroker::GetAddonMgr().GetAddon(strValue, addon))
            strText = addon->Name();
          if (strText.empty())
            strText = g_localizeStrings.Get(231); // None
        }
        else if (controlFormat == "path" || controlFormat == "file" || controlFormat == "image")
        {
          std::string shortPath;
          if (CUtil::MakeShortenPath(strValue, shortPath, 30))
            strText = shortPath;
        }
        else if (controlFormat == "infolabel")
        {
          strText = strValue;
          if (strText.empty())
            strText = g_localizeStrings.Get(231); // None
        }
        else
          strText = strValue;

        break;
      }

      case SettingType::Action:
      {
        // CSettingAction. 
        // Note: This can be removed once all settings use a proper control & format combination.
        // CSettingAction is strictly speaking not designed to have a label2, it does not even have a value.
        strText = m_pButton->GetLabel2();

        break;
      }

      default:
        break;
      }
    }
  }
  else if (controlType == "slider")
  {
    switch (m_pSetting->GetType())
    {
      case SettingType::Integer:
      {
        std::shared_ptr<const CSettingInt> settingInt = std::static_pointer_cast<CSettingInt>(m_pSetting);
        strText = CGUIControlSliderSetting::GetText(std::static_pointer_cast<const CSettingControlSlider>(m_pSetting->GetControl()),
          settingInt->GetValue(), settingInt->GetMinimum(), settingInt->GetStep(), settingInt->GetMaximum(), m_localizer);
        break;
      }

      case SettingType::Number:
      {
        std::shared_ptr<const CSettingNumber> settingNumber = std::static_pointer_cast<CSettingNumber>(m_pSetting);
        strText = CGUIControlSliderSetting::GetText(std::static_pointer_cast<const CSettingControlSlider>(m_pSetting->GetControl()),
          settingNumber->GetValue(), settingNumber->GetMinimum(), settingNumber->GetStep(), settingNumber->GetMaximum(), m_localizer);
        break;
      }
    
      default:
        break;
    }
  }

  m_pButton->SetLabel2(strText);
}

bool CGUIControlButtonSetting::GetPath(std::shared_ptr<CSettingPath> pathSetting, ILocalizer* localizer)
{
  if (pathSetting == NULL)
    return false;

  std::string path = pathSetting->GetValue();

  VECSOURCES shares;
  bool localSharesOnly = false;
  const std::vector<std::string>& sources = pathSetting->GetSources();
  for (const auto& source : sources)
  {
    if (StringUtils::EqualsNoCase(source, "local"))
      localSharesOnly = true;
    else
    {
      VECSOURCES *sources = CMediaSourceSettings::GetInstance().GetSources(source);
      if (sources != NULL)
        shares.insert(shares.end(), sources->begin(), sources->end());
    }
  }

  g_mediaManager.GetLocalDrives(shares);
  if (!localSharesOnly)
    g_mediaManager.GetNetworkLocations(shares);

  bool result = false;
  std::shared_ptr<const CSettingControlButton> control = std::static_pointer_cast<const CSettingControlButton>(pathSetting->GetControl());
  const auto heading = ::Localize(control->GetHeading(), localizer);
  if (control->GetFormat() == "file")
    result = CGUIDialogFileBrowser::ShowAndGetFile(shares, pathSetting->GetMasking(), heading, path, control->UseImageThumbs(), control->UseFileDirectories());
  else if (control->GetFormat() == "image")
    result = CGUIDialogFileBrowser::ShowAndGetImage(shares, heading, path);
  else
    result = CGUIDialogFileBrowser::ShowAndGetDirectory(shares, heading, path, pathSetting->Writable());

  if (!result)
    return false;

  return pathSetting->SetValue(path);
}

void CGUIControlButtonSetting::OnSliderChange(void *data, CGUISliderControl *slider)
{
  if (slider == NULL)
    return;

  std::string strText;
  switch (m_pSetting->GetType())
  {
    case SettingType::Integer:
    {
      std::shared_ptr<CSettingInt> settingInt = std::static_pointer_cast<CSettingInt>(m_pSetting);
      if (settingInt->SetValue(slider->GetIntValue()))
        strText = CGUIControlSliderSetting::GetText(std::static_pointer_cast<const CSettingControlSlider>(m_pSetting->GetControl()),
          settingInt->GetValue(), settingInt->GetMinimum(), settingInt->GetStep(), settingInt->GetMaximum(), m_localizer);
      break;
    }

    case SettingType::Number:
    {
      std::shared_ptr<CSettingNumber> settingNumber = std::static_pointer_cast<CSettingNumber>(m_pSetting);
      if (settingNumber->SetValue(static_cast<double>(slider->GetFloatValue())))
        strText = CGUIControlSliderSetting::GetText(std::static_pointer_cast<const CSettingControlSlider>(m_pSetting->GetControl()),
          settingNumber->GetValue(), settingNumber->GetMinimum(), settingNumber->GetStep(), settingNumber->GetMaximum(), m_localizer);
      break;
    }
    
    default:
      break;
  }

  if (!strText.empty())
    slider->SetTextValue(strText);
}

CGUIControlEditSetting::CGUIControlEditSetting(CGUIEditControl *pEdit, int id, std::shared_ptr<CSetting> pSetting, ILocalizer* localizer)
  : CGUIControlBaseSetting(id, pSetting, localizer)
{
  std::shared_ptr<const CSettingControlEdit> control = std::static_pointer_cast<const CSettingControlEdit>(pSetting->GetControl());
  m_pEdit = pEdit;
  if (m_pEdit == NULL)
    return;

  m_pEdit->SetID(id);
  int heading = m_pSetting->GetLabel();
  if (control->GetHeading() > 0)
    heading = control->GetHeading();
  if (heading < 0)
    heading = 0;

  CGUIEditControl::INPUT_TYPE inputType = CGUIEditControl::INPUT_TYPE_TEXT;
  const std::string &controlFormat = control->GetFormat();
  if (controlFormat == "string")
  {
    if (control->IsHidden())
      inputType = CGUIEditControl::INPUT_TYPE_PASSWORD;
  }
  else if (controlFormat == "integer" || controlFormat == "number")
  {
    if (control->VerifyNewValue())
      inputType = CGUIEditControl::INPUT_TYPE_PASSWORD_NUMBER_VERIFY_NEW;
    else
      inputType = CGUIEditControl::INPUT_TYPE_NUMBER;
  }
  else if (controlFormat == "ip")
    inputType = CGUIEditControl::INPUT_TYPE_IPADDRESS;
  else if (controlFormat == "md5")
    inputType = CGUIEditControl::INPUT_TYPE_PASSWORD_MD5;

  m_pEdit->SetInputType(inputType, heading);

  Update();

  // this will automatically trigger validation so it must be executed after
  // having set the value of the control based on the value of the setting
  m_pEdit->SetInputValidation(InputValidation, this);
}

CGUIControlEditSetting::~CGUIControlEditSetting() = default;

bool CGUIControlEditSetting::OnClick()
{
  if (m_pEdit == NULL)
    return false;

  // update our string
  if (m_pSetting->GetControl()->GetFormat() == "urlencoded")
  {
    std::shared_ptr<CSettingUrlEncodedString> urlEncodedSetting = std::static_pointer_cast<CSettingUrlEncodedString>(m_pSetting);
    SetValid(urlEncodedSetting->SetDecodedValue(m_pEdit->GetLabel2()));
  }
  else
    SetValid(m_pSetting->FromString(m_pEdit->GetLabel2()));

  return IsValid();
}

void CGUIControlEditSetting::Update(bool updateDisplayOnly /* = false */)
{
  if (updateDisplayOnly || m_pEdit == NULL)
    return;

  CGUIControlBaseSetting::Update();

  std::shared_ptr<const CSettingControlEdit> control = std::static_pointer_cast<const CSettingControlEdit>(m_pSetting->GetControl());

  if (control->GetFormat() == "urlencoded")
  {
    std::shared_ptr<CSettingUrlEncodedString> urlEncodedSetting = std::static_pointer_cast<CSettingUrlEncodedString>(m_pSetting);
    m_pEdit->SetLabel2(urlEncodedSetting->GetDecodedValue());
  }
  else
    m_pEdit->SetLabel2(m_pSetting->ToString());
}

bool CGUIControlEditSetting::InputValidation(const std::string &input, void *data)
{
  if (data == NULL)
    return true;

  CGUIControlEditSetting *editControl = reinterpret_cast<CGUIControlEditSetting*>(data);
  if (editControl == NULL || editControl->GetSetting() == NULL)
    return true;

  editControl->SetValid(editControl->GetSetting()->CheckValidity(input));
  return editControl->IsValid();
}

CGUIControlSliderSetting::CGUIControlSliderSetting(CGUISettingsSliderControl *pSlider, int id, std::shared_ptr<CSetting> pSetting, ILocalizer* localizer)
  : CGUIControlBaseSetting(id, pSetting, localizer)
{
  m_pSlider = pSlider;
  if (m_pSlider == NULL)
    return;

  m_pSlider->SetID(id);
  
  switch (m_pSetting->GetType())
  {
    case SettingType::Integer:
    {
      std::shared_ptr<CSettingInt> settingInt = std::static_pointer_cast<CSettingInt>(m_pSetting);
      if (m_pSetting->GetControl()->GetFormat() == "percentage")
        m_pSlider->SetType(SLIDER_CONTROL_TYPE_PERCENTAGE);
      else
      {
        m_pSlider->SetType(SLIDER_CONTROL_TYPE_INT);
        m_pSlider->SetRange(settingInt->GetMinimum(), settingInt->GetMaximum());
      }
      m_pSlider->SetIntInterval(settingInt->GetStep());
      break;
    }

    case SettingType::Number:
    {
      std::shared_ptr<CSettingNumber> settingNumber = std::static_pointer_cast<CSettingNumber>(m_pSetting);
      m_pSlider->SetType(SLIDER_CONTROL_TYPE_FLOAT);
      m_pSlider->SetFloatRange((float)settingNumber->GetMinimum(), (float)settingNumber->GetMaximum());
      m_pSlider->SetFloatInterval((float)settingNumber->GetStep());
      break;
    }

    default:
      break;
  }

  Update();
}

CGUIControlSliderSetting::~CGUIControlSliderSetting() = default;

bool CGUIControlSliderSetting::OnClick()
{
  if (m_pSlider == NULL)
    return false;

  switch (m_pSetting->GetType())
  {
    case SettingType::Integer:
      SetValid(std::static_pointer_cast<CSettingInt>(m_pSetting)->SetValue(m_pSlider->GetIntValue()));
      break;

    case SettingType::Number:
      SetValid(std::static_pointer_cast<CSettingNumber>(m_pSetting)->SetValue(m_pSlider->GetFloatValue()));
      break;
    
    default:
      return false;
  }
  
  return IsValid();
}

void CGUIControlSliderSetting::Update(bool updateDisplayOnly /* = false */)
{
  if (m_pSlider == NULL)
    return;

  CGUIControlBaseSetting::Update();

  std::string strText;
  switch (m_pSetting->GetType())
  {
    case SettingType::Integer:
    {
      std::shared_ptr<const CSettingInt> settingInt = std::static_pointer_cast<CSettingInt>(m_pSetting);
      int value;
      if (updateDisplayOnly)
        value = m_pSlider->GetIntValue();
      else
      {
        value = std::static_pointer_cast<CSettingInt>(m_pSetting)->GetValue();
        m_pSlider->SetIntValue(value);
      }

      strText = CGUIControlSliderSetting::GetText(std::static_pointer_cast<const CSettingControlSlider>(m_pSetting->GetControl()),
        value, settingInt->GetMinimum(), settingInt->GetStep(), settingInt->GetMaximum(), m_localizer);
      break;
    }

    case SettingType::Number:
    {
      std::shared_ptr<const CSettingNumber> settingNumber = std::static_pointer_cast<CSettingNumber>(m_pSetting);
      double value;
      if (updateDisplayOnly)
        value = m_pSlider->GetFloatValue();
      else
      {
        value = std::static_pointer_cast<CSettingNumber>(m_pSetting)->GetValue();
        m_pSlider->SetFloatValue((float)value);
      }

      strText = CGUIControlSliderSetting::GetText(std::static_pointer_cast<const CSettingControlSlider>(m_pSetting->GetControl()),
        value, settingNumber->GetMinimum(), settingNumber->GetStep(), settingNumber->GetMaximum(), m_localizer);
      break;
    }
    
    default:
      break;
  }

  if (!strText.empty())
    m_pSlider->SetTextValue(strText);
}

std::string CGUIControlSliderSetting::GetText(std::shared_ptr<const CSettingControlSlider> control, const CVariant &value, const CVariant &minimum, const CVariant &step, const CVariant &maximum, ILocalizer* localizer)
{
  if (control == NULL ||
      !(value.isInteger() || value.isDouble()))
    return "";

  SettingControlSliderFormatter formatter = control->GetFormatter();
  if (formatter != NULL)
    return formatter(control, value, minimum, step, maximum);

  std::string formatString = control->GetFormatString();
  if (control->GetFormatLabel() > -1)
    formatString = ::Localize(control->GetFormatLabel(), localizer);

  if (value.isDouble())
    return StringUtils::Format(formatString.c_str(), value.asDouble());

  return StringUtils::Format(formatString.c_str(), static_cast<int>(value.asInteger()));
}

CGUIControlRangeSetting::CGUIControlRangeSetting(CGUISettingsSliderControl *pSlider, int id, std::shared_ptr<CSetting> pSetting, ILocalizer* localizer)
  : CGUIControlBaseSetting(id, pSetting, localizer)
{
  m_pSlider = pSlider;
  if (m_pSlider == NULL)
    return;

  m_pSlider->SetID(id);
  m_pSlider->SetRangeSelection(true);
  
  if (m_pSetting->GetType() == SettingType::List)
  {
    std::shared_ptr<CSettingList> settingList = std::static_pointer_cast<CSettingList>(m_pSetting);
    SettingConstPtr listDefintion = settingList->GetDefinition();
    switch (listDefintion->GetType())
    {
      case SettingType::Integer:
      {
        std::shared_ptr<const CSettingInt> listDefintionInt = std::static_pointer_cast<const CSettingInt>(listDefintion);
        if (m_pSetting->GetControl()->GetFormat() == "percentage")
          m_pSlider->SetType(SLIDER_CONTROL_TYPE_PERCENTAGE);
        else
        {
          m_pSlider->SetType(SLIDER_CONTROL_TYPE_INT);
          m_pSlider->SetRange(listDefintionInt->GetMinimum(), listDefintionInt->GetMaximum());
        }
        m_pSlider->SetIntInterval(listDefintionInt->GetStep());
        break;
      }

      case SettingType::Number:
      {
        std::shared_ptr<const CSettingNumber> listDefinitionNumber = std::static_pointer_cast<const CSettingNumber>(listDefintion);
        m_pSlider->SetType(SLIDER_CONTROL_TYPE_FLOAT);
        m_pSlider->SetFloatRange((float)listDefinitionNumber->GetMinimum(), (float)listDefinitionNumber->GetMaximum());
        m_pSlider->SetFloatInterval((float)listDefinitionNumber->GetStep());
        break;
      }

      default:
        break;
    }
  }

  Update();
}

CGUIControlRangeSetting::~CGUIControlRangeSetting() = default;

bool CGUIControlRangeSetting::OnClick()
{
  if (m_pSlider == NULL ||
      m_pSetting->GetType() != SettingType::List)
    return false;

  std::shared_ptr<CSettingList> settingList = std::static_pointer_cast<CSettingList>(m_pSetting);
  const SettingList &settingListValues = settingList->GetValue();
  if (settingListValues.size() != 2)
    return false;

  std::vector<CVariant> values;
  SettingConstPtr listDefintion = settingList->GetDefinition();
  switch (listDefintion->GetType())
  {
    case SettingType::Integer:
      values.push_back(m_pSlider->GetIntValue(CGUISliderControl::RangeSelectorLower));
      values.push_back(m_pSlider->GetIntValue(CGUISliderControl::RangeSelectorUpper));
      break;

    case SettingType::Number:
      values.push_back(m_pSlider->GetFloatValue(CGUISliderControl::RangeSelectorLower));
      values.push_back(m_pSlider->GetFloatValue(CGUISliderControl::RangeSelectorUpper));
      break;
    
    default:
      return false;
  }
  
  if (values.size() != 2)
    return false;

  SetValid(CSettingUtils::SetList(settingList, values));
  return IsValid();
}

void CGUIControlRangeSetting::Update(bool updateDisplayOnly /* = false */)
{
  if (m_pSlider == NULL ||
      m_pSetting->GetType() != SettingType::List)
    return;

  CGUIControlBaseSetting::Update();

  std::shared_ptr<CSettingList> settingList = std::static_pointer_cast<CSettingList>(m_pSetting);
  const SettingList &settingListValues = settingList->GetValue();
  if (settingListValues.size() != 2)
    return;

  SettingConstPtr listDefintion = settingList->GetDefinition();
  std::shared_ptr<const CSettingControlRange> controlRange = std::static_pointer_cast<const CSettingControlRange>(m_pSetting->GetControl());
  const std::string &controlFormat = controlRange->GetFormat();

  std::string strText;
  std::string strTextLower, strTextUpper;
  std::string formatString = Localize(controlRange->GetFormatLabel() > -1 ? controlRange->GetFormatLabel() : 21469);
  std::string valueFormat = controlRange->GetValueFormat();
  if (controlRange->GetValueFormatLabel() > -1)
    valueFormat = Localize(controlRange->GetValueFormatLabel());

  switch (listDefintion->GetType())
  {
    case SettingType::Integer:
    {
      int valueLower, valueUpper;
      if (updateDisplayOnly)
      {
        valueLower = m_pSlider->GetIntValue(CGUISliderControl::RangeSelectorLower);
        valueUpper = m_pSlider->GetIntValue(CGUISliderControl::RangeSelectorUpper);
      }
      else
      {
        valueLower = std::static_pointer_cast<CSettingInt>(settingListValues[0])->GetValue();
        valueUpper = std::static_pointer_cast<CSettingInt>(settingListValues[1])->GetValue();
        m_pSlider->SetIntValue(valueLower, CGUISliderControl::RangeSelectorLower);
        m_pSlider->SetIntValue(valueUpper, CGUISliderControl::RangeSelectorUpper);
      }

      if (controlFormat == "date" || controlFormat == "time")
      {
        CDateTime dateLower((time_t)valueLower);
        CDateTime dateUpper((time_t)valueUpper);

        if (controlFormat == "date")
        {
          if (valueFormat.empty())
          {
            strTextLower = dateLower.GetAsLocalizedDate();
            strTextUpper = dateUpper.GetAsLocalizedDate();
          }
          else
          {
            strTextLower = dateLower.GetAsLocalizedDate(valueFormat);
            strTextUpper = dateUpper.GetAsLocalizedDate(valueFormat);
          }
        }
        else
        {
          if (valueFormat.empty())
            valueFormat = "mm:ss";

          strTextLower = dateLower.GetAsLocalizedTime(valueFormat);
          strTextUpper = dateUpper.GetAsLocalizedTime(valueFormat);
        }
      }
      else
      {
        strTextLower = StringUtils::Format(valueFormat.c_str(), valueLower);
        strTextUpper = StringUtils::Format(valueFormat.c_str(), valueUpper);
      }

      if (valueLower != valueUpper)
        strText = StringUtils::Format(formatString.c_str(), strTextLower.c_str(), strTextUpper.c_str());
      else
        strText = strTextLower;
      break;
    }

    case SettingType::Number:
    {
      double valueLower, valueUpper;
      if (updateDisplayOnly)
      {
        valueLower = static_cast<double>(m_pSlider->GetFloatValue(CGUISliderControl::RangeSelectorLower));
        valueUpper = static_cast<double>(m_pSlider->GetFloatValue(CGUISliderControl::RangeSelectorUpper));
      }
      else
      {
        valueLower = std::static_pointer_cast<CSettingNumber>(settingListValues[0])->GetValue();
        valueUpper = std::static_pointer_cast<CSettingNumber>(settingListValues[1])->GetValue();
        m_pSlider->SetFloatValue((float)valueLower, CGUISliderControl::RangeSelectorLower);
        m_pSlider->SetFloatValue((float)valueUpper, CGUISliderControl::RangeSelectorUpper);
      }

      strTextLower = StringUtils::Format(valueFormat.c_str(), valueLower);
      if (valueLower != valueUpper)
      {
        strTextUpper = StringUtils::Format(valueFormat.c_str(), valueUpper);
        strText = StringUtils::Format(formatString.c_str(), strTextLower.c_str(), strTextUpper.c_str());
      }
      else
        strText = strTextLower;
      break;
    }
    
    default:
      strText.clear();
      break;
  }

  if (!strText.empty())
    m_pSlider->SetTextValue(strText);
}

CGUIControlSeparatorSetting::CGUIControlSeparatorSetting(CGUIImage *pImage, int id, ILocalizer* localizer)
    : CGUIControlBaseSetting(id, NULL, localizer)
{
  m_pImage = pImage;
  if (m_pImage == NULL)
    return;

  m_pImage->SetID(id);
}

CGUIControlSeparatorSetting::~CGUIControlSeparatorSetting() = default;

CGUIControlGroupTitleSetting::CGUIControlGroupTitleSetting(CGUILabelControl *pLabel, int id, ILocalizer* localizer)
  : CGUIControlBaseSetting(id, NULL, localizer)
{
  m_pLabel = pLabel;
  if (m_pLabel == NULL)
    return;

  m_pLabel->SetID(id);
}

CGUIControlGroupTitleSetting::~CGUIControlGroupTitleSetting() = default;

CGUIControlLabelSetting::CGUIControlLabelSetting(CGUIButtonControl *pButton, int id, std::shared_ptr<CSetting> pSetting, ILocalizer* localizer)
  : CGUIControlBaseSetting(id, pSetting, localizer)
{
  m_pButton = pButton;
  if (m_pButton == NULL)
    return;

  m_pButton->SetID(id);
  Update();
}
