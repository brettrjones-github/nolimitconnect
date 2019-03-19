/*
 *  Copyright (C) 2012-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "PVRClients.h"

#include <utility>

#include "ServiceBroker.h"
#include "addons/BinaryAddonCache.h"
#include "guilib/LocalizeStrings.h"
#include "messaging/ApplicationMessenger.h"
#include "utils/log.h"

#include "pvr/PVRJobs.h"
#include "pvr/PVRManager.h"
#include "pvr/channels/PVRChannelGroupInternal.h"

using namespace ADDON;
using namespace PVR;
using namespace KODI::MESSAGING;

namespace
{
  int ClientIdFromAddonId(const std::string &strID)
  {
    std::hash<std::string> hasher;
    int iClientId = static_cast<int>(hasher(strID));
    if (iClientId < 0)
      iClientId = -iClientId;
    return iClientId;
  }

} // unnamed namespace

CPVRClients::CPVRClients(void)
{
  CServiceBroker::GetAddonMgr().RegisterAddonMgrCallback(ADDON_PVRDLL, this);
  CServiceBroker::GetAddonMgr().Events().Subscribe(this, &CPVRClients::OnAddonEvent);
}

CPVRClients::~CPVRClients(void)
{
  CServiceBroker::GetAddonMgr().Events().Unsubscribe(this);
  CServiceBroker::GetAddonMgr().UnregisterAddonMgrCallback(ADDON_PVRDLL);

  for (const auto &client : m_clientMap)
  {
    client.second->Destroy();
  }
}

void CPVRClients::Start(void)
{
  UpdateAddons();
}

void CPVRClients::Stop()
{
  CSingleLock lock(m_critSection);
  for (const auto &client : m_clientMap)
  {
    client.second->Stop();
  }
}

void CPVRClients::Continue()
{
  CSingleLock lock(m_critSection);
  for (const auto &client : m_clientMap)
  {
    client.second->Continue();
  }
}

void CPVRClients::UpdateAddons(const std::string &changedAddonId /*= ""*/)
{
  VECADDONS addons;
  CServiceBroker::GetAddonMgr().GetInstalledAddons(addons, ADDON_PVRDLL);

  if (addons.empty())
    return;

  bool bFoundChangedAddon = changedAddonId.empty();
  std::vector<std::pair<AddonPtr, bool>> addonsWithStatus;
  for (const auto &addon : addons)
  {
    bool bEnabled = !CServiceBroker::GetAddonMgr().IsAddonDisabled(addon->ID());
    addonsWithStatus.emplace_back(std::make_pair(addon, bEnabled));

    if (!bFoundChangedAddon && addon->ID() == changedAddonId)
      bFoundChangedAddon = true;
  }

  if (!bFoundChangedAddon)
    return; // changed addon is not a known pvr client addon, so nothing to update

  addons.clear();

  std::vector<std::pair<CPVRClientPtr, int>> addonsToCreate;
  std::vector<AddonPtr> addonsToReCreate;
  std::vector<AddonPtr> addonsToDestroy;

  {
    CSingleLock lock(m_critSection);
    for (const auto &addonWithStatus : addonsWithStatus)
    {
      AddonPtr addon = addonWithStatus.first;
      bool bEnabled = addonWithStatus.second;

      if (bEnabled && (!IsKnownClient(addon) || !IsCreatedClient(addon)))
      {
        int iClientId = ClientIdFromAddonId(addon->ID());

        CPVRClientPtr client;
        if (IsKnownClient(addon))
        {
          GetClient(iClientId, client);
        }
        else
        {
          client = std::dynamic_pointer_cast<CPVRClient>(addon);
          if (!client)
          {
            CLog::LogF(LOGERROR, "Severe error, incorrect add-on type");
            continue;
          }
        }
        addonsToCreate.emplace_back(std::make_pair(client, iClientId));
      }
      else if (IsCreatedClient(addon))
      {
        if (bEnabled)
          addonsToReCreate.emplace_back(addon);
        else
          addonsToDestroy.emplace_back(addon);
      }
    }
  }

  if (!addonsToCreate.empty() || !addonsToReCreate.empty() || !addonsToDestroy.empty())
  {
    CServiceBroker::GetPVRManager().Stop();

    for (const auto& addon : addonsToCreate)
    {
      ADDON_STATUS status = addon.first->Create(addon.second);

      if (status != ADDON_STATUS_OK)
      {
        CLog::LogF(LOGERROR, "Failed to create add-on %s, status = %d", addon.first->Name().c_str(), status);
        if (status == ADDON_STATUS_PERMANENT_FAILURE)
        {
          CServiceBroker::GetAddonMgr().DisableAddon(addon.first->ID());
          CJobManager::GetInstance().AddJob(new CPVREventlogJob(true, true, addon.first->Name(), g_localizeStrings.Get(24070), addon.first->Icon()), nullptr);
        }
      }
    }

    for (const auto& addon : addonsToReCreate)
    {
      // recreate client
      StopClient(addon, true);
    }

    for (const auto& addon : addonsToDestroy)
    {
      // destroy client
      StopClient(addon, false);
    }

    if (!addonsToCreate.empty())
    {
      // update created clients map
      CSingleLock lock(m_critSection);
      for (const auto& addon : addonsToCreate)
      {
        if (m_clientMap.find(addon.second) == m_clientMap.end())
        {
          m_clientMap.insert(std::make_pair(addon.second, addon.first));
        }
      }
    }

    CServiceBroker::GetPVRManager().Start();
  }
}

bool CPVRClients::RequestRestart(AddonPtr addon, bool bDataChanged)
{
  return StopClient(addon, true);
}

bool CPVRClients::StopClient(const AddonPtr &addon, bool bRestart)
{
  // stop playback if needed
  if (CServiceBroker::GetPVRManager().IsPlaying())
    CApplicationMessenger::GetInstance().SendMsg(TMSG_MEDIA_STOP);

  CSingleLock lock(m_critSection);

  int iId = GetClientId(addon->ID());
  CPVRClientPtr mappedClient;
  if (GetClient(iId, mappedClient))
  {
    if (bRestart)
    {
      mappedClient->ReCreate();
    }
    else
    {
      const auto it = m_clientMap.find(iId);
      if (it != m_clientMap.end())
        m_clientMap.erase(it);

      mappedClient->Destroy();
    }
    return true;
  }

  return false;
}

void CPVRClients::OnAddonEvent(const AddonEvent& event)
{
  if (typeid(event) == typeid(AddonEvents::Enabled) ||  // also called on install,
      typeid(event) == typeid(AddonEvents::Disabled) || // not called on uninstall
      typeid(event) == typeid(AddonEvents::UnInstalled) ||
      typeid(event) == typeid(AddonEvents::ReInstalled))
  {
    // update addons
    CJobManager::GetInstance().AddJob(new CPVRUpdateAddonsJob(event.id), nullptr);
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// client access
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool CPVRClients::GetClient(const std::string &strId, AddonPtr &addon) const
{
  CSingleLock lock(m_critSection);
  for (const auto &client : m_clientMap)
  {
    if (client.second->ID() == strId)
    {
      addon = client.second;
      return true;
    }
  }
  return false;
}

bool CPVRClients::GetClient(int iClientId, CPVRClientPtr &addon) const
{
  bool bReturn = false;
  if (iClientId <= PVR_INVALID_CLIENT_ID)
    return bReturn;

  CSingleLock lock(m_critSection);
  const auto &itr = m_clientMap.find(iClientId);
  if (itr != m_clientMap.end())
  {
    addon = itr->second;
    bReturn = true;
  }

  return bReturn;
}

int CPVRClients::GetClientId(const std::string& strId) const
{
  CSingleLock lock(m_critSection);
  for (const auto &entry : m_clientMap)
  {
    if (entry.second->ID() == strId)
    {
      return entry.first;
    }
  }

  return -1;
}

int CPVRClients::CreatedClientAmount(void) const
{
  int iReturn = 0;

  CSingleLock lock(m_critSection);
  for (const auto &client : m_clientMap)
  {
    if (client.second->ReadyToUse())
      ++iReturn;
  }

  return iReturn;
}

bool CPVRClients::HasCreatedClients(void) const
{
  CSingleLock lock(m_critSection);
  for (const auto &client : m_clientMap)
  {
    if (client.second->ReadyToUse() && !client.second->IgnoreClient())
      return true;
  }

  return false;
}

bool CPVRClients::IsKnownClient(const AddonPtr &client) const
{
  // valid client IDs start at 1
  return GetClientId(client->ID()) > 0;
}

bool CPVRClients::IsCreatedClient(int iClientId) const
{
  CPVRClientPtr client;
  return GetCreatedClient(iClientId, client);
}

bool CPVRClients::IsCreatedClient(const AddonPtr &addon)
{
  CSingleLock lock(m_critSection);
  for (const auto &client : m_clientMap)
  {
    if (client.second->ID() == addon->ID())
      return client.second->ReadyToUse();
  }
  return false;
}

bool CPVRClients::GetCreatedClient(int iClientId, CPVRClientPtr &addon) const
{
  if (GetClient(iClientId, addon))
    return addon->ReadyToUse();

  return false;
}

int CPVRClients::GetCreatedClients(CPVRClientMap &clients) const
{
  int iReturn = 0;

  CSingleLock lock(m_critSection);
  for (const auto &client : m_clientMap)
  {
    if (client.second->ReadyToUse() && !client.second->IgnoreClient())
    {
      clients.insert(std::make_pair(client.second->GetID(), client.second));
      ++iReturn;
    }
  }

  return iReturn;
}

PVR_ERROR CPVRClients::GetCreatedClients(CPVRClientMap &clientsReady, std::vector<int> &clientsNotReady) const
{
  clientsNotReady.clear();

  VECADDONS addons;
  CBinaryAddonCache &addonCache = CServiceBroker::GetBinaryAddonCache();
  addonCache.GetAddons(addons, ADDON::ADDON_PVRDLL);

  for (const auto &addon : addons)
  {
    int iClientId = ClientIdFromAddonId(addon->ID());
    CPVRClientPtr client;
    GetClient(iClientId, client);

    if (client && client->ReadyToUse() && !client->IgnoreClient())
    {
      clientsReady.insert(std::make_pair(iClientId, client));
    }
    else
    {
      clientsNotReady.emplace_back(iClientId);
    }
  }

  return clientsNotReady.empty() ? PVR_ERROR_NO_ERROR : PVR_ERROR_SERVER_ERROR;
}

int CPVRClients::GetFirstCreatedClientID(void)
{
  CSingleLock lock(m_critSection);
  for (const auto &client : m_clientMap)
  {
    if (client.second->ReadyToUse())
      return client.second->GetID();
  }

  return -1;
}

int CPVRClients::EnabledClientAmount(void) const
{
  int iReturn = 0;

  CPVRClientMap clientMap;
  {
    CSingleLock lock(m_critSection);
    clientMap = m_clientMap;
  }

  for (const auto &client : clientMap)
  {
    if (!CServiceBroker::GetAddonMgr().IsAddonDisabled(client.second->ID()))
      ++iReturn;
  }

  return iReturn;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// client API calls
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::vector<SBackend> CPVRClients::GetBackendProperties() const
{
  std::vector<SBackend> backendProperties;

  ForCreatedClients(__FUNCTION__, [&backendProperties](const CPVRClientPtr &client) {
    SBackend properties;

    if (client->GetDriveSpace(properties.diskTotal, properties.diskUsed) == PVR_ERROR_NO_ERROR)
    {
      properties.diskTotal *= 1024;
      properties.diskUsed *= 1024;
    }

    int iAmount = 0;
    if (client->GetChannelsAmount(iAmount) == PVR_ERROR_NO_ERROR)
      properties.numChannels = iAmount;
    if (client->GetTimersAmount(iAmount) == PVR_ERROR_NO_ERROR)
      properties.numTimers = iAmount;
    if (client->GetRecordingsAmount(false, iAmount) == PVR_ERROR_NO_ERROR)
      properties.numRecordings = iAmount;
    if (client->GetRecordingsAmount(true, iAmount) == PVR_ERROR_NO_ERROR)
      properties.numDeletedRecordings = iAmount;
    properties.name = client->GetBackendName();
    properties.version = client->GetBackendVersion();
    properties.host = client->GetConnectionString();

    backendProperties.emplace_back(properties);
    return PVR_ERROR_NO_ERROR;
  });

  return backendProperties;
}

bool CPVRClients::SupportsTimers() const
{
  bool bReturn = false;
  ForCreatedClients(__FUNCTION__, [&bReturn](const CPVRClientPtr &client) {
    if (!bReturn)
      bReturn = client->GetClientCapabilities().SupportsTimers();
    return PVR_ERROR_NO_ERROR;
  });
  return bReturn;
}

bool CPVRClients::GetTimers(CPVRTimersContainer *timers, std::vector<int> &failedClients)
{
  return ForCreatedClients(__FUNCTION__, [timers](const CPVRClientPtr &client) {
    return client->GetTimers(timers);
  }, failedClients) == PVR_ERROR_NO_ERROR;
}

PVR_ERROR CPVRClients::GetTimerTypes(CPVRTimerTypes& results) const
{
  return ForCreatedClients(__FUNCTION__, [&results](const CPVRClientPtr &client) {
    CPVRTimerTypes types;
    PVR_ERROR ret = client->GetTimerTypes(types);
    if (ret == PVR_ERROR_NO_ERROR)
      results.insert(results.end(), types.begin(), types.end());
    return ret;
  });
}

PVR_ERROR CPVRClients::GetRecordings(CPVRRecordings *recordings, bool deleted)
{
  return ForCreatedClients(__FUNCTION__, [recordings, deleted](const CPVRClientPtr &client) {
    return client->GetRecordings(recordings, deleted);
  });
}

PVR_ERROR CPVRClients::DeleteAllRecordingsFromTrash()
{
  return ForCreatedClients(__FUNCTION__, [](const CPVRClientPtr &client) {
    return client->DeleteAllRecordingsFromTrash();
  });
}

PVR_ERROR CPVRClients::SetEPGTimeFrame(int iDays)
{
  return ForCreatedClients(__FUNCTION__, [iDays](const CPVRClientPtr &client) {
    return client->SetEPGTimeFrame(iDays);
  });
}

PVR_ERROR CPVRClients::GetChannels(CPVRChannelGroupInternal *group, std::vector<int> &failedClients)
{
  return ForCreatedClients(__FUNCTION__, [group](const CPVRClientPtr &client) {
    return client->GetChannels(*group, group->IsRadio());
  }, failedClients);
}

PVR_ERROR CPVRClients::GetChannelGroups(CPVRChannelGroups *groups, std::vector<int> &failedClients)
{
  return ForCreatedClients(__FUNCTION__, [groups](const CPVRClientPtr &client) {
    return client->GetChannelGroups(groups);
  }, failedClients);
}

PVR_ERROR CPVRClients::GetChannelGroupMembers(CPVRChannelGroup *group, std::vector<int> &failedClients)
{
  return ForCreatedClients(__FUNCTION__, [group](const CPVRClientPtr &client) {
    return client->GetChannelGroupMembers(group);
  }, failedClients);
}

std::vector<CPVRClientPtr> CPVRClients::GetClientsSupportingChannelScan(void) const
{
  std::vector<CPVRClientPtr> possibleScanClients;
  ForCreatedClients(__FUNCTION__, [&possibleScanClients](const CPVRClientPtr &client) {
    if (client->GetClientCapabilities().SupportsChannelScan())
      possibleScanClients.emplace_back(client);
    return PVR_ERROR_NO_ERROR;
  });
  return possibleScanClients;
}

std::vector<CPVRClientPtr> CPVRClients::GetClientsSupportingChannelSettings(bool bRadio) const
{
  std::vector<CPVRClientPtr> possibleSettingsClients;
  ForCreatedClients(__FUNCTION__, [bRadio, &possibleSettingsClients](const CPVRClientPtr &client) {
    const CPVRClientCapabilities& caps = client->GetClientCapabilities();
    if (caps.SupportsChannelSettings() &&
        ((bRadio && caps.SupportsRadio()) || (!bRadio && caps.SupportsTV())))
      possibleSettingsClients.emplace_back(client);
    return PVR_ERROR_NO_ERROR;
  });
  return possibleSettingsClients;
}

void CPVRClients::OnSystemSleep()
{
  ForCreatedClients(__FUNCTION__, [](const CPVRClientPtr &client) {
    client->OnSystemSleep();
    return PVR_ERROR_NO_ERROR;
  });
}

void CPVRClients::OnSystemWake()
{
  ForCreatedClients(__FUNCTION__, [](const CPVRClientPtr &client) {
    client->OnSystemWake();
    return PVR_ERROR_NO_ERROR;
  });
}

void CPVRClients::OnPowerSavingActivated()
{
  ForCreatedClients(__FUNCTION__, [](const CPVRClientPtr &client) {
    client->OnPowerSavingActivated();
    return PVR_ERROR_NO_ERROR;
  });
}

void CPVRClients::OnPowerSavingDeactivated()
{
  ForCreatedClients(__FUNCTION__, [](const CPVRClientPtr &client) {
    client->OnPowerSavingDeactivated();
    return PVR_ERROR_NO_ERROR;
  });
}

void CPVRClients::ConnectionStateChange(
  CPVRClient *client, std::string &strConnectionString, PVR_CONNECTION_STATE newState, std::string &strMessage)
{
  if (!client)
    return;

  int iMsg = -1;
  bool bError = true;
  bool bNotify = true;

  switch (newState)
  {
    case PVR_CONNECTION_STATE_SERVER_UNREACHABLE:
      iMsg = 35505; // Server is unreachable
      if (client->GetPreviousConnectionState() == PVR_CONNECTION_STATE_UNKNOWN ||
          client->GetPreviousConnectionState() == PVR_CONNECTION_STATE_CONNECTING)
      {
        // Make our users happy. There were so many complaints about this notification because their TV backend
        // was not up quick enough after Kodi start. So, ignore the very first 'server not reachable' notification.
        bNotify = false;
      }
      break;
    case PVR_CONNECTION_STATE_SERVER_MISMATCH:
      iMsg = 35506; // Server does not respond properly
      break;
    case PVR_CONNECTION_STATE_VERSION_MISMATCH:
      iMsg = 35507; // Server version is not compatible
      break;
    case PVR_CONNECTION_STATE_ACCESS_DENIED:
      iMsg = 35508; // Access denied
      break;
    case PVR_CONNECTION_STATE_CONNECTED:
      bError = false;
      iMsg = 36034; // Connection established
      if (client->GetPreviousConnectionState() == PVR_CONNECTION_STATE_UNKNOWN ||
          client->GetPreviousConnectionState() == PVR_CONNECTION_STATE_CONNECTING)
        bNotify = false;
      break;
    case PVR_CONNECTION_STATE_DISCONNECTED:
      iMsg = 36030; // Connection lost
      break;
    case PVR_CONNECTION_STATE_CONNECTING:
      bError = false;
      iMsg = 35509; // Connecting
      bNotify = false;
      break;
    default:
      CLog::LogF(LOGERROR, "Unknown connection state");
      return;
  }

  // Use addon-supplied message, if present
  std::string strMsg;
  if (!strMessage.empty())
    strMsg = strMessage;
  else
    strMsg = g_localizeStrings.Get(iMsg);

  // Notify user.
  CJobManager::GetInstance().AddJob(new CPVREventlogJob(bNotify, bError, client->Name(), strMsg, client->Icon()), nullptr);

  if (newState == PVR_CONNECTION_STATE_CONNECTED)
  {
    // update properties on connect
    if (!client->GetAddonProperties())
      CLog::LogF(LOGERROR, "Error reading PVR client properties");

    CServiceBroker::GetPVRManager().Start();
  }
}

PVR_ERROR CPVRClients::ForCreatedClients(const char* strFunctionName, PVRClientFunction function) const
{
  std::vector<int> failedClients;
  return ForCreatedClients(strFunctionName, function, failedClients);
}

PVR_ERROR CPVRClients::ForCreatedClients(const char* strFunctionName, PVRClientFunction function, std::vector<int> &failedClients) const
{
  PVR_ERROR lastError = PVR_ERROR_NO_ERROR;

  CPVRClientMap clients;
  GetCreatedClients(clients, failedClients);

  for (const auto &clientEntry : clients)
  {
    PVR_ERROR currentError = function(clientEntry.second);

    if (currentError != PVR_ERROR_NO_ERROR && currentError != PVR_ERROR_NOT_IMPLEMENTED)
    {
      CLog::LogFunction(LOGERROR, strFunctionName,
                        "PVR client '%s' returned an error: %s",
                        clientEntry.second->GetFriendlyName().c_str(), CPVRClient::ToString(currentError));
      lastError = currentError;
      failedClients.emplace_back(clientEntry.first);
    }
  }
  return lastError;
}
