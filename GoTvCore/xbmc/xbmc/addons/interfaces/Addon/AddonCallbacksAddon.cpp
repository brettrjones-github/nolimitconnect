/*
 *  Copyright (C) 2012-2018 Team Kodi
 *  Copyright (C) 2015-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */
#include <vector>
#include "Application.h"
#include "addons/Addon.h"
#include "addons/settings/AddonSettings.h"
#include "AddonCallbacksAddon.h"
#include "utils/log.h"
#include "LangInfoKodi.h"
#include "dialogs/GUIDialogKaiToast.h"
#include "filesystem/File.h"
#include "filesystem/Directory.h"
#include "guilib/LocalizeStrings.h"
#include "FileItem.h"
#include "network/Network.h"
#include "ServiceBroker.h"
#include "utils/CharsetConverter.h"
#include "utils/StringUtils.h"
#include "utils/XMLUtils.h"
#include <GoTvCore/xbmc/xbmc/GoTvUrl.h>
#include "addons/kodi-addon-dev-kit/include/kodi/kodi_vfs_types.h"
#include "filesystem/SpecialProtocol.h"

using namespace ADDON;
using namespace XFILE;

namespace KodiAPI
{
namespace AddOn
{

CAddonCallbacksAddon::CAddonCallbacksAddon(CAddon* addon)
  : m_addon(addon),
    m_callbacks(new CB_AddOnLib)
{
  /* write XBMC addon-on specific add-on function addresses to the callback table */
  m_callbacks->Log                = AddOnLog;
  m_callbacks->QueueNotification  = QueueNotification;
  m_callbacks->WakeOnLan          = WakeOnLan;
  m_callbacks->GetSetting         = GetAddonSetting;
  m_callbacks->TranslateSpecialProtocol = TranslateSpecialProtocol;
  m_callbacks->UnknownToUTF8      = UnknownToUTF8;
  m_callbacks->GetLocalizedString = GetLocalizedString;
  m_callbacks->GetDVDMenuLanguage = GetDVDMenuLanguage;
  m_callbacks->FreeString         = FreeString;
  m_callbacks->FreeStringArray    = FreeStringArray;

  m_callbacks->OpenFile           = OpenFile;
  m_callbacks->OpenFileForWrite   = OpenFileForWrite;
  m_callbacks->ReadFile           = ReadFile;
  m_callbacks->ReadFileString     = ReadFileString;
  m_callbacks->WriteFile          = WriteFile;
  m_callbacks->FlushFile          = FlushFile;
  m_callbacks->SeekFile           = SeekFile;
  m_callbacks->TruncateFile       = TruncateFile;
  m_callbacks->GetFilePosition    = GetFilePosition;
  m_callbacks->GetFileLength      = GetFileLength;
  m_callbacks->GetFileDownloadSpeed = GetFileDownloadSpeed;
  m_callbacks->CloseFile          = CloseFile;
  m_callbacks->GetFileChunkSize   = GetFileChunkSize;
  m_callbacks->FileExists         = FileExists;
  m_callbacks->StatFile           = StatFile;
  m_callbacks->GetFilePropertyValue = GetFilePropertyValue;
  m_callbacks->GetFilePropertyValues = GetFilePropertyValues;
  m_callbacks->DeleteFile         = DeleteFile;

  m_callbacks->CanOpenDirectory   = CanOpenDirectory;
  m_callbacks->CreateDirectory    = CreateDirectory;
  m_callbacks->DirectoryExists    = DirectoryExists;
  m_callbacks->RemoveDirectory    = RemoveDirectory;
  m_callbacks->GetDirectory       = GetDirectory;
  m_callbacks->FreeDirectory      = FreeDirectory;

  m_callbacks->CURLCreate         = CURLCreate;
  m_callbacks->CURLAddOption      = CURLAddOption;
  m_callbacks->CURLOpen           = CURLOpen;
}

CAddonCallbacksAddon::~CAddonCallbacksAddon()
{
  /* delete the callback table */
  delete m_callbacks;
}

void CAddonCallbacksAddon::AddOnLog(void *addonData, const addon_log_t addonLogLevel, const char *strMessage)
{
  CAddonInterfaces* addon = (CAddonInterfaces*) addonData;
  if (addon == NULL || strMessage == NULL)
  {
    CLog::Log(LOGERROR, "CAddonCallbacksAddon - %s - called with a null pointer", __FUNCTION__);
    return;
  }

  CAddonCallbacksAddon* addonHelper = static_cast<CAddonCallbacksAddon*>(addon->AddOnLib_GetHelper());

  try
  {
    int xbmcLogLevel = LOGNONE;
    switch (addonLogLevel)
    {
      case LOG_ERROR:
        xbmcLogLevel = LOGERROR;
        break;
      case LOG_INFO:
        xbmcLogLevel = LOGINFO;
        break;
      case LOG_NOTICE:
        xbmcLogLevel = LOGNOTICE;
        break;
      case LOG_DEBUG:
      default:
        xbmcLogLevel = LOGDEBUG;
        break;
    }

    std::string strXbmcMessage = StringUtils::Format("AddOnLog: %s: %s", addonHelper->m_addon->Name().c_str(), strMessage);
    CLog::Log(xbmcLogLevel, "%s", strXbmcMessage.c_str());
  }
  catch (std::exception &e)
  {
    CLog::Log(LOGERROR, "CAddonCallbacksAddon - %s - exception '%s' caught in call in add-on '%s'. please contact the developer of this addon: %s",
        __FUNCTION__, e.what(), addonHelper->m_addon->Name().c_str(), addonHelper->m_addon->Author().c_str());
  }
}

void CAddonCallbacksAddon::QueueNotification(void *addonData, const queue_msg_t type, const char *strMessage)
{
  CAddonInterfaces* addon = (CAddonInterfaces*) addonData;
  if (addon == NULL || strMessage == NULL)
  {
    CLog::Log(LOGERROR, "CAddonCallbacksAddon - %s - called with a null pointer", __FUNCTION__);
    return;
  }

  CAddonCallbacksAddon* addonHelper = static_cast<CAddonCallbacksAddon*>(addon->AddOnLib_GetHelper());

  try
  {
    switch (type)
    {
      case QUEUE_WARNING:
        CGUIDialogKaiToast::QueueNotification(CGUIDialogKaiToast::Warning, addonHelper->m_addon->Name(), strMessage, 3000, true);
        CLog::Log(LOGDEBUG, "CAddonCallbacksAddon - %s - %s - Warning Message: '%s'", __FUNCTION__, addonHelper->m_addon->Name().c_str(), strMessage);
        break;

      case QUEUE_ERROR:
        CGUIDialogKaiToast::QueueNotification(CGUIDialogKaiToast::Error, addonHelper->m_addon->Name(), strMessage, 3000, true);
        CLog::Log(LOGDEBUG, "CAddonCallbacksAddon - %s - %s - Error Message : '%s'", __FUNCTION__, addonHelper->m_addon->Name().c_str(), strMessage);
        break;

      case QUEUE_INFO:
      default:
        CGUIDialogKaiToast::QueueNotification(CGUIDialogKaiToast::Info, addonHelper->m_addon->Name(), strMessage, 3000, false);
        CLog::Log(LOGDEBUG, "CAddonCallbacksAddon - %s - %s - Info Message : '%s'", __FUNCTION__, addonHelper->m_addon->Name().c_str(), strMessage);
        break;
    }
  }
  catch (std::exception &e)
  {
    CLog::Log(LOGERROR, "CAddonCallbacksAddon - %s - exception '%s' caught in call in add-on '%s'. please contact the developer of this addon: %s",
        __FUNCTION__, e.what(), addonHelper->m_addon->Name().c_str(), addonHelper->m_addon->Author().c_str());
  }
}

bool CAddonCallbacksAddon::WakeOnLan(const char *mac)
{
  return CServiceBroker::GetNetwork().WakeOnLan(mac);
}

bool CAddonCallbacksAddon::GetAddonSetting(void *addonData, const char *strSettingName, void *settingValue)
{
  CAddonInterfaces* addon = (CAddonInterfaces*) addonData;
  if (addon == NULL || strSettingName == NULL || settingValue == NULL)
  {
    CLog::Log(LOGERROR, "CAddonCallbacksAddon - %s - called with a null pointer", __FUNCTION__);
    return false;
  }

  CAddonCallbacksAddon* addonHelper = static_cast<CAddonCallbacksAddon*>(addon->AddOnLib_GetHelper());

  try
  {
    CLog::Log(LOGDEBUG, "CAddonCallbacksAddon - %s - add-on '%s' requests setting '%s'", __FUNCTION__, addonHelper->m_addon->Name().c_str(), strSettingName);

    if (strcasecmp(strSettingName, "__addonpath__") == 0)
    {
      strcpy((char*) settingValue, addonHelper->m_addon->Path().c_str());
      return true;
    }
    else if (strcasecmp(strSettingName, "__addonname__") == 0)
    {
      strcpy((char*)settingValue, addonHelper->m_addon->Name().c_str());
      return true;
    }

    if (!addonHelper->m_addon->ReloadSettings() || addonHelper->m_addon->GetSettings() == nullptr)
    {
      CLog::Log(LOGERROR, "CAddonCallbacksAddon - %s - couldn't get settings for add-on '%s'", __FUNCTION__, addonHelper->m_addon->Name().c_str());
      return false;
    }

    auto setting = addonHelper->m_addon->GetSettings()->GetSetting(strSettingName);
    if (setting == nullptr)
    {
      CLog::Log(LOGERROR, "CAddonCallbacksAddon - %s - can't find setting '%s' in '%s'", __FUNCTION__, strSettingName, addonHelper->m_addon->Name().c_str());
      return false;
    }

    switch (setting->GetType())
    {
      case SettingType::Boolean:
        *static_cast<bool*>(settingValue) = std::static_pointer_cast<CSettingBool>(setting)->GetValue();
        return true;

      case SettingType::Integer:
        *static_cast<int*>(settingValue) = std::static_pointer_cast<CSettingInt>(setting)->GetValue();
        return true;

      case SettingType::Number:
        *static_cast<float*>(settingValue) = static_cast<float>(std::static_pointer_cast<CSettingNumber>(setting)->GetValue());
        return true;

      case SettingType::String:
        strcpy((char*)settingValue, std::static_pointer_cast<CSettingString>(setting)->GetValue().c_str());
        return true;

      default:
        CLog::Log(LOGERROR, "CAddonCallbacksAddon - %s - setting '%s' in '%s' has unsupported type", __FUNCTION__, strSettingName, addonHelper->m_addon->Name().c_str());
        return false;
    }
  }
  catch (std::exception &e)
  {
    CLog::Log(LOGERROR, "CAddonCallbacksAddon - %s - exception '%s' caught in call in add-on '%s'. please contact the developer of this addon: %s",
        __FUNCTION__, e.what(), addonHelper->m_addon->Name().c_str(), addonHelper->m_addon->Author().c_str());
  }

  return false;
}

char* CAddonCallbacksAddon::TranslateSpecialProtocol(const char *strSource)
{
  try
  {
    if (strSource)
      return strdup(CSpecialProtocol::TranslatePath(strSource).c_str());
    else
      return NULL;
  }
  catch (std::exception &e)
  {
    CLog::Log(LOGERROR, "CAddonCallbacksAddon - %s - exception '%s' caught", __FUNCTION__, e.what());
    return NULL;
  }
}

char* CAddonCallbacksAddon::UnknownToUTF8(const char *strSource)
{
  std::string string;
  if (strSource != NULL)
    g_charsetConverter.unknownToUTF8(strSource, string);
  else
    string = "";
  char* buffer = strdup(string.c_str());
  return buffer;
}

char* CAddonCallbacksAddon::GetLocalizedString(const void* addonData, long dwCode)
{
  CAddonInterfaces* addon = (CAddonInterfaces*) addonData;
  if (!addon || g_application.m_bStop)
    return NULL;

  CAddonCallbacksAddon* addonHelper = static_cast<CAddonCallbacksAddon*>(addon->AddOnLib_GetHelper());

  std::string string;
  if ((dwCode >= 30000 && dwCode <= 30999) || (dwCode >= 32000 && dwCode <= 32999))
    string = g_localizeStrings.GetAddonString(addonHelper->m_addon->ID(), dwCode).c_str();
  else
    string = g_localizeStrings.Get(dwCode).c_str();

  char* buffer = strdup(string.c_str());
  return buffer;
}

char* CAddonCallbacksAddon::GetDVDMenuLanguage(const void* addonData)
{
  CAddonInterfaces* helper = (CAddonInterfaces*) addonData;
  if (!helper)
    return NULL;

  std::string string = g_langInfo.GetDVDMenuLanguage();

  char* buffer = strdup(string.c_str());
  return buffer;
}

void CAddonCallbacksAddon::FreeString(const void* addonData, char* str)
{
  free(str);
}

void CAddonCallbacksAddon::FreeStringArray(const void* addonData, char** arr, int numElements)
{
  for (int i = 0; i < numElements; ++i)
  {
    free(arr[i]);
  }
  free(arr);
}

void* CAddonCallbacksAddon::OpenFile(const void* addonData, const char* strFileName, unsigned int flags)
{
  CAddonInterfaces* helper = (CAddonInterfaces*) addonData;
  if (!helper)
    return NULL;

  CFile* file = new CFile;
  if (file->Open(strFileName, flags))
    return ((void*)file);

  delete file;
  return NULL;
}

void* CAddonCallbacksAddon::OpenFileForWrite(const void* addonData, const char* strFileName, bool bOverwrite)
{
  CAddonInterfaces* helper = (CAddonInterfaces*) addonData;
  if (!helper)
    return NULL;

  CFile* file = new CFile;
  if (file->OpenForWrite(strFileName, bOverwrite))
    return ((void*)file);

  delete file;
  return NULL;
}

ssize_t CAddonCallbacksAddon::ReadFile(const void* addonData, void* file, void* lpBuf, size_t uiBufSize)
{
  CAddonInterfaces* helper = (CAddonInterfaces*) addonData;
  if (!helper)
    return 0;

  CFile* cfile = (CFile*)file;
  if (!cfile)
    return 0;

  return cfile->Read(lpBuf, uiBufSize);
}

bool CAddonCallbacksAddon::ReadFileString(const void* addonData, void* file, char *szLine, int iLineLength)
{
  CAddonInterfaces* helper = (CAddonInterfaces*) addonData;
  if (!helper)
    return false;

  CFile* cfile = (CFile*)file;
  if (!cfile)
    return false;

  return cfile->ReadString(szLine, iLineLength);
}

ssize_t CAddonCallbacksAddon::WriteFile(const void* addonData, void* file, const void* lpBuf, size_t uiBufSize)
{
  CAddonInterfaces* helper = (CAddonInterfaces*) addonData;
  if (!helper)
    return -1;

  CFile* cfile = (CFile*)file;
  if (!cfile)
    return -1;

  return cfile->Write(lpBuf, uiBufSize);
}

void CAddonCallbacksAddon::FlushFile(const void* addonData, void* file)
{
  CAddonInterfaces* helper = (CAddonInterfaces*) addonData;
  if (!helper)
    return;

  CFile* cfile = (CFile*)file;
  if (!cfile)
    return;

  cfile->Flush();
}

int64_t CAddonCallbacksAddon::SeekFile(const void* addonData, void* file, int64_t iFilePosition, int iWhence)
{
  CAddonInterfaces* helper = (CAddonInterfaces*) addonData;
  if (!helper)
    return 0;

  CFile* cfile = (CFile*)file;
  if (!cfile)
    return 0;

  return cfile->Seek(iFilePosition, iWhence);
}

int CAddonCallbacksAddon::TruncateFile(const void* addonData, void* file, int64_t iSize)
{
  CAddonInterfaces* helper = (CAddonInterfaces*) addonData;
  if (!helper)
    return 0;

  CFile* cfile = (CFile*)file;
  if (!cfile)
    return 0;

  return cfile->Truncate(iSize);
}

int64_t CAddonCallbacksAddon::GetFilePosition(const void* addonData, void* file)
{
  CAddonInterfaces* helper = (CAddonInterfaces*) addonData;
  if (!helper)
    return 0;

  CFile* cfile = (CFile*)file;
  if (!cfile)
    return 0;

  return cfile->GetPosition();
}

int64_t CAddonCallbacksAddon::GetFileLength(const void* addonData, void* file)
{
  CAddonInterfaces* helper = (CAddonInterfaces*) addonData;
  if (!helper)
    return 0;

  CFile* cfile = (CFile*)file;
  if (!cfile)
    return 0;

  return cfile->GetLength();
}

double CAddonCallbacksAddon::GetFileDownloadSpeed(const void* addonData, void* file)
{
  CAddonInterfaces* helper = (CAddonInterfaces*)addonData;
  if (!helper)
    return 0.0f;

  CFile* cfile = (CFile*)file;
  if (!cfile)
    return 0.0f;

  return cfile->GetDownloadSpeed();
}

void CAddonCallbacksAddon::CloseFile(const void* addonData, void* file)
{
  CAddonInterfaces* helper = (CAddonInterfaces*) addonData;
  if (!helper)
    return;

  CFile* cfile = (CFile*)file;
  if (cfile)
  {
    cfile->Close();
    delete cfile;
  }
}

int CAddonCallbacksAddon::GetFileChunkSize(const void* addonData, void* file)
{
  CAddonInterfaces* helper = (CAddonInterfaces*) addonData;
  if (!helper)
    return 0;

  CFile* cfile = (CFile*)file;
  if (!cfile)
    return 0;

  return cfile->GetChunkSize();
}

bool CAddonCallbacksAddon::FileExists(const void* addonData, const char *strFileName, bool bUseCache)
{
  CAddonInterfaces* helper = (CAddonInterfaces*) addonData;
  if (!helper)
    return false;

  return CFile::Exists(strFileName, bUseCache);
}

int CAddonCallbacksAddon::StatFile(const void* addonData, const char *strFileName, struct ::__stat64* buffer)
{
  CAddonInterfaces* helper = (CAddonInterfaces*) addonData;
  if (!helper)
    return -1;

  return CFile::Stat(strFileName, buffer);
}

char *CAddonCallbacksAddon::GetFilePropertyValue(const void* addonData, void* file, XFILE::FileProperty type, const char *name)
{
  CAddonInterfaces* helper = (CAddonInterfaces*)addonData;
  if (!helper)
    return nullptr;

  CFile* cfile = (CFile*)file;
  if (cfile)
  {
    std::vector<std::string> values = cfile->GetPropertyValues(type, name);
    if (values.empty()) {
      return nullptr;
    }
    return strdup(values[0].c_str());
  }
  return nullptr;
}

char **CAddonCallbacksAddon::GetFilePropertyValues(const void* addonData, void* file, XFILE::FileProperty type, const char *name, int *numValues)
{
  CAddonInterfaces* helper = (CAddonInterfaces*)addonData;
  if (!helper)
    return nullptr;

  CFile* cfile = static_cast<CFile*>(file);
  if (!cfile)
  {
    return nullptr;
  }
  std::vector<std::string> values = cfile->GetPropertyValues(type, name);
  *numValues = values.size();
  char **ret = static_cast<char**>(malloc(sizeof(char*)*values.size()));
  for (int i = 0; i < *numValues; ++i)
  {
    ret[i] = strdup(values[i].c_str());
  }
  return ret;
}

bool CAddonCallbacksAddon::DeleteFile(const void* addonData, const char *strFileName)
{
  CAddonInterfaces* helper = (CAddonInterfaces*) addonData;
  if (!helper)
    return false;

  return CFile::Delete(strFileName);
}

bool CAddonCallbacksAddon::CanOpenDirectory(const void* addonData, const char* strURL)
{
  CAddonInterfaces* helper = (CAddonInterfaces*) addonData;
  if (!helper)
    return false;

  CFileItemList items;
  return CDirectory::GetDirectory(strURL, items, "", DIR_FLAG_DEFAULTS);
}

bool CAddonCallbacksAddon::CreateDirectory(const void* addonData, const char *strPath)
{
  CAddonInterfaces* helper = (CAddonInterfaces*) addonData;
  if (!helper)
    return false;

  return CDirectory::Create(strPath);
}

bool CAddonCallbacksAddon::DirectoryExists(const void* addonData, const char *strPath)
{
  CAddonInterfaces* helper = (CAddonInterfaces*) addonData;
  if (!helper)
    return false;

  return CDirectory::Exists(strPath);
}

bool CAddonCallbacksAddon::RemoveDirectory(const void* addonData, const char *strPath)
{
  CAddonInterfaces* helper = (CAddonInterfaces*) addonData;
  if (!helper)
    return false;

  // Empty directory
  CFileItemList fileItems;
  CDirectory::GetDirectory(strPath, fileItems, "", DIR_FLAG_DEFAULTS);
  for (int i = 0; i < fileItems.Size(); ++i)
    CFile::Delete(fileItems.Get(i)->GetPath());

  return CDirectory::Remove(strPath);
}

static void CFileItemListToVFSDirEntries(VFSDirEntry* entries,
                                         unsigned int num_entries,
                                         const CFileItemList& items)
{
  if (!entries)
    return;

  int toCopy = std::min(num_entries, (unsigned int)items.Size());

  for (int i=0;i<toCopy;++i)
  {
    entries[i].label = strdup(items[i]->GetLabel().c_str());
    entries[i].path = strdup(items[i]->GetPath().c_str());
    entries[i].size = items[i]->m_dwSize;
    entries[i].folder = items[i]->m_bIsFolder;
  }
}

bool CAddonCallbacksAddon::GetDirectory(const void* addonData, const char *strPath, const char* mask, VFSDirEntry** items, unsigned int* num_items)
{
  CAddonInterfaces* helper = (CAddonInterfaces*) addonData;
  if (!helper)
    return false;

  CFileItemList fileItems;
  if (!CDirectory::GetDirectory(strPath, fileItems, mask, DIR_FLAG_NO_FILE_DIRS))
    return false;

  if (fileItems.Size() > 0)
  {
    *num_items = static_cast<unsigned int>(fileItems.Size());
    *items = new VFSDirEntry[fileItems.Size()];
  }
  else
  {
    *num_items = 0;
    *items = nullptr;
  }

  CFileItemListToVFSDirEntries(*items, *num_items, fileItems);
  return true;
}

void CAddonCallbacksAddon::FreeDirectory(const void* addonData, VFSDirEntry* items, unsigned int num_items)
{
  CAddonInterfaces* helper = (CAddonInterfaces*) addonData;
  if (!helper)
    return;

  for (unsigned int i = 0; i < num_items; ++i)
  {
    free(items[i].label);
    free(items[i].path);
  }
  delete[] items;
}

void* CAddonCallbacksAddon::CURLCreate(const void* addonData, const char* strURL)
{
  CAddonInterfaces* helper = (CAddonInterfaces*)addonData;
  if (!helper)
    return nullptr;

  CFile* file = new CFile;
  if (file->CURLCreate(strURL))
    return ((void*)file);

  delete file;

  return nullptr;
}

bool CAddonCallbacksAddon::CURLAddOption(const void* addonData, void* file, XFILE::CURLOPTIONTYPE type, const char* name, const char * value)
{
  CAddonInterfaces* helper = (CAddonInterfaces*)addonData;
  if (!helper)
    return false;

  CFile* cfile = (CFile*)file;
  if (!cfile)
    return false;

  return cfile->CURLAddOption(type, name, value);
}

bool CAddonCallbacksAddon::CURLOpen(const void* addonData, void* file, unsigned int flags)
{
  CAddonInterfaces* helper = (CAddonInterfaces*)addonData;
  if (!helper)
    return false;

  CFile* cfile = (CFile*)file;
  if (!cfile)
    return false;

  return cfile->CURLOpen(flags);
}

} /* namespace AddOn */
} /* namespace KodiAPI */
