/*
 *  Copyright (C) 2012-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "PVRChannel.h"

#include "ServiceBroker.h"
#include "addons/PVRClient.h"
#include "filesystem/File.h"
#include "guilib/LocalizeStrings.h"
#include "threads/SingleLock.h"
#include "utils/StringUtils.h"
#include "utils/Variant.h"
#include "utils/log.h"

#include "pvr/PVRDatabase.h"
#include "pvr/PVRManager.h"
#include "pvr/channels/PVRChannelGroupInternal.h"
#include "pvr/epg/EpgContainer.h"
#include "pvr/timers/PVRTimers.h"

using namespace PVR;

bool CPVRChannel::operator==(const CPVRChannel &right) const
{
  return (m_bIsRadio  == right.m_bIsRadio &&
          m_iUniqueId == right.m_iUniqueId &&
          m_iClientId == right.m_iClientId);
}

bool CPVRChannel::operator!=(const CPVRChannel &right) const
{
  return !(*this == right);
}

CPVRChannel::CPVRChannel(bool bRadio /* = false */)
{
  m_iChannelId              = -1;
  m_bIsRadio                = bRadio;
  m_bIsHidden               = false;
  m_bIsUserSetIcon          = false;
  m_bIsUserSetName          = false;
  m_bIsLocked               = false;
  m_iLastWatched            = 0;
  m_bChanged                = false;

  m_iEpgId                  = -1;
  m_bEPGCreated             = false;
  m_bEPGEnabled             = true;
  m_strEPGScraper           = "client";

  m_iUniqueId               = -1;
  m_iClientId               = -1;
  m_iClientEncryptionSystem = -1;
  UpdateEncryptionName();
}

CPVRChannel::CPVRChannel(const PVR_CHANNEL &channel, unsigned int iClientId)
: m_clientChannelNumber(channel.iChannelNumber, channel.iSubChannelNumber)
{
  m_iChannelId              = -1;
  m_bIsRadio                = channel.bIsRadio;
  m_bIsHidden               = channel.bIsHidden;
  m_bIsUserSetIcon          = false;
  m_bIsUserSetName          = false;
  m_bIsLocked               = false;
  m_strIconPath             = channel.strIconPath;
  m_strChannelName          = channel.strChannelName;
  m_iUniqueId               = channel.iUniqueId;
  m_strClientChannelName    = channel.strChannelName;
  m_strInputFormat          = channel.strInputFormat;
  m_iClientEncryptionSystem = channel.iEncryptionSystem;
  m_iClientId               = iClientId;
  m_iLastWatched            = 0;
  m_bEPGEnabled             = !channel.bIsHidden;
  m_strEPGScraper           = "client";
  m_iEpgId                  = -1;
  m_bEPGCreated             = false;
  m_bChanged                = false;

  if (m_strChannelName.empty())
    m_strChannelName = StringUtils::Format("%s %d", g_localizeStrings.Get(19029).c_str(), m_iUniqueId);

  UpdateEncryptionName();
}

void CPVRChannel::Serialize(CVariant& value) const
{
  value["channelid"] = m_iChannelId;
  value["channeltype"] = m_bIsRadio ? "radio" : "tv";
  value["hidden"] = m_bIsHidden;
  value["locked"] = m_bIsLocked;
  value["icon"] = m_strIconPath;
  value["channel"]  = m_strChannelName;
  value["uniqueid"]  = m_iUniqueId;
  CDateTime lastPlayed(m_iLastWatched);
  value["lastplayed"] = lastPlayed.IsValid() ? lastPlayed.GetAsDBDate() : "";
  value["channelnumber"] = m_channelNumber.GetChannelNumber();
  value["subchannelnumber"] = m_channelNumber.GetSubChannelNumber();

  CPVREpgInfoTagPtr epg(GetEPGNow());
  if (epg)
  {
    // add the properties of the current EPG item to the main object
    epg->Serialize(value);
    // and add an extra sub-object with only the current EPG details
    epg->Serialize(value["broadcastnow"]);
  }

  epg = GetEPGNext();
  if (epg)
    epg->Serialize(value["broadcastnext"]);

  value["isrecording"] = IsRecording();
}

/********** XBMC related channel methods **********/

bool CPVRChannel::Delete(void)
{
  bool bReturn = false;
  const CPVRDatabasePtr database(CServiceBroker::GetPVRManager().GetTVDatabase());
  if (!database)
    return bReturn;

  /* delete the EPG table */
  CPVREpgPtr epg = GetEPG();
  if (epg)
  {
    CPVRChannelPtr empty;
    epg->SetChannel(empty);
    CServiceBroker::GetPVRManager().EpgContainer().DeleteEpg(epg, true);
    CSingleLock lock(m_critSection);
    m_bEPGCreated = false;
  }

  bReturn = database->Delete(*this);
  return bReturn;
}

CPVREpgPtr CPVRChannel::GetEPG(void) const
{
  int iEpgId(-1);
  {
    CSingleLock lock(m_critSection);
    if (!m_bIsHidden && m_bEPGEnabled && m_iEpgId > 0)
      iEpgId = m_iEpgId;
  }

  return iEpgId > 0 ? CServiceBroker::GetPVRManager().EpgContainer().GetById(iEpgId) : CPVREpgPtr();
}

bool CPVRChannel::CreateEPG(bool bForce)
{
  CSingleLock lock(m_critSection);
  if (!m_bEPGCreated || bForce)
  {
    CPVREpgPtr epg = CServiceBroker::GetPVRManager().EpgContainer().CreateChannelEpg(shared_from_this());
    if (epg)
    {
      m_bEPGCreated = true;
      if (epg->EpgID() != m_iEpgId)
      {
        m_iEpgId = epg->EpgID();
        m_bChanged = true;
      }
      return true;
    }
  }
  return false;
}

bool CPVRChannel::UpdateFromClient(const CPVRChannelPtr &channel)
{
  SetClientID(channel->ClientID());

  CSingleLock lock(m_critSection);

  if (m_clientChannelNumber     != channel->m_clientChannelNumber ||
      m_strInputFormat          != channel->InputFormat() ||
      m_iClientEncryptionSystem != channel->EncryptionSystem() ||
      m_strClientChannelName    != channel->ClientChannelName())
  {
    m_clientChannelNumber     = channel->m_clientChannelNumber;
    m_strInputFormat          = channel->InputFormat();
    m_iClientEncryptionSystem = channel->EncryptionSystem();
    m_strClientChannelName    = channel->ClientChannelName();

    UpdateEncryptionName();
    SetChanged();
  }

  // only update the channel name and icon if the user hasn't changed them manually
  if (m_strChannelName.empty() || !IsUserSetName())
    SetChannelName(channel->ClientChannelName());
  if (m_strIconPath.empty() || !IsUserSetIcon())
    SetIconPath(channel->IconPath());

  return m_bChanged;
}

bool CPVRChannel::Persist()
{
  {
    // not changed
    CSingleLock lock(m_critSection);
    if (!m_bChanged && m_iChannelId > 0)
      return true;
  }

  const CPVRDatabasePtr database(CServiceBroker::GetPVRManager().GetTVDatabase());
  if (database)
  {
    bool bReturn = database->Persist(*this, true);
    CSingleLock lock(m_critSection);
    m_bChanged = !bReturn;
    return bReturn;
  }

  return false;
}

bool CPVRChannel::SetChannelID(int iChannelId)
{
  CSingleLock lock(m_critSection);
  if (m_iChannelId != iChannelId)
  {
    /* update the id */
    m_iChannelId = iChannelId;
    SetChanged();
    m_bChanged = true;

    return true;
  }

  return false;
}

const CPVRChannelNumber& CPVRChannel::ChannelNumber() const
{
  CSingleLock lock(m_critSection);
  return m_channelNumber;
}

bool CPVRChannel::SetHidden(bool bIsHidden)
{
  CSingleLock lock(m_critSection);

  if (m_bIsHidden != bIsHidden)
  {
    /* update the hidden flag */
    m_bIsHidden = bIsHidden;
	m_bEPGEnabled = !bIsHidden;
    SetChanged();
    m_bChanged = true;

    return true;
  }

  return false;
}

bool CPVRChannel::SetLocked(bool bIsLocked)
{
  CSingleLock lock(m_critSection);

  if (m_bIsLocked != bIsLocked)
  {
    /* update the locked flag */
    m_bIsLocked = bIsLocked;
    SetChanged();
    m_bChanged = true;

    return true;
  }

  return false;
}

bool CPVRChannel::IsRecording(void) const
{
  return CServiceBroker::GetPVRManager().Timers()->IsRecordingOnChannel(*this);
}

CPVRRecordingPtr CPVRChannel::GetRecording(void) const
{
  CPVREpgInfoTagPtr epgTag = GetEPGNow();
  return (epgTag && epgTag->HasRecording()) ?
      epgTag->Recording() :
      CPVRRecordingPtr();
}

bool CPVRChannel::HasRecording(void) const
{
  CPVREpgInfoTagPtr epgTag = GetEPGNow();
  return epgTag && epgTag->HasRecording();
}

bool CPVRChannel::SetIconPath(const std::string &strIconPath, bool bIsUserSetIcon /* = false */)
{
  CSingleLock lock(m_critSection);

  if (m_strIconPath != strIconPath)
  {
    /* update the path */
    m_strIconPath = StringUtils::Format("%s", strIconPath.c_str());
    SetChanged();
    m_bChanged = true;
    m_bIsUserSetIcon = bIsUserSetIcon && !m_strIconPath.empty();

    return true;
  }

  return false;
}

bool CPVRChannel::SetChannelName(const std::string &strChannelName, bool bIsUserSetName /*= false*/)
{
  std::string strName(strChannelName);

  if (strName.empty())
    strName = StringUtils::Format(g_localizeStrings.Get(19085).c_str(), m_clientChannelNumber.FormattedChannelNumber().c_str());

  CSingleLock lock(m_critSection);
  if (m_strChannelName != strName)
  {
    m_strChannelName = strName;
    m_bIsUserSetName = bIsUserSetName;

    /* if the user changes the name manually to an empty string we reset the
       flag and use the name from the client instead */
    if (bIsUserSetName && strChannelName.empty())
    {
      m_bIsUserSetName = false;
      m_strChannelName = ClientChannelName();
    }

    SetChanged();
    m_bChanged = true;

    return true;
  }

  return false;
}

bool CPVRChannel::SetLastWatched(time_t iLastWatched)
{
  {
    CSingleLock lock(m_critSection);

    if (m_iLastWatched != iLastWatched)
      m_iLastWatched = iLastWatched;
  }

  const CPVRDatabasePtr database(CServiceBroker::GetPVRManager().GetTVDatabase());
  if (database)
    return database->UpdateLastWatched(*this);

  return false;
}

bool CPVRChannel::IsEmpty() const
{
  CSingleLock lock(m_critSection);
  return (m_strFileNameAndPath.empty());
}

/********** Client related channel methods **********/

bool CPVRChannel::SetClientID(int iClientId)
{
  CSingleLock lock(m_critSection);

  if (m_iClientId != iClientId)
  {
    /* update the client ID */
    m_iClientId = iClientId;
    SetChanged();
    m_bChanged = true;

    return true;
  }

  return false;
}

void CPVRChannel::UpdatePath(CPVRChannelGroupInternal* group)
{
  if (!group)
    return;

  const CPVRClientPtr client = CServiceBroker::GetPVRManager().GetClient(m_iClientId);
  if (client)
  {
    CSingleLock lock(m_critSection);
    std::string strFileNameAndPath = StringUtils::Format("%s%s_%d.pvr",
                                                         group->GetPath(),
                                                         client->ID().c_str(),
                                                         m_iUniqueId);
    if (m_strFileNameAndPath != strFileNameAndPath)
    {
      m_strFileNameAndPath = strFileNameAndPath;
      SetChanged();
    }
  }
}

std::string CPVRChannel::GetEncryptionName(int iCaid)
{
  // http://www.dvb.org/index.php?id=174
  // http://en.wikipedia.org/wiki/Conditional_access_system
  std::string strName(g_localizeStrings.Get(13205)); /* Unknown */

  if (     iCaid == 0x0000)
    strName = g_localizeStrings.Get(19013); /* Free To Air */
  else if (iCaid >= 0x0001 &&
           iCaid <= 0x009F)
    strName = g_localizeStrings.Get(19014); /* Fixed */
  else if (iCaid >= 0x00A0 &&
           iCaid<= 0x00A1)
    strName = g_localizeStrings.Get(338); /* Analog */
  else if (iCaid >= 0x00A2 &&
           iCaid <= 0x00FF)
    strName = g_localizeStrings.Get(19014); /* Fixed */
  else if (iCaid >= 0x0100 &&
           iCaid <= 0x01FF)
    strName = "SECA Mediaguard";
  else if (iCaid == 0x0464)
    strName = "EuroDec";
  else if (iCaid >= 0x0500 &&
           iCaid <= 0x05FF)
    strName = "Viaccess";
  else if (iCaid >= 0x0600 &&
           iCaid <= 0x06FF)
    strName = "Irdeto";
  else if (iCaid >= 0x0900 &&
           iCaid <= 0x09FF)
    strName = "NDS Videoguard";
  else if (iCaid >= 0x0B00 &&
           iCaid <= 0x0BFF)
    strName = "Conax";
  else if (iCaid >= 0x0D00 &&
           iCaid <= 0x0DFF)
    strName = "CryptoWorks";
  else if (iCaid >= 0x0E00 &&
           iCaid <= 0x0EFF)
    strName = "PowerVu";
  else if (iCaid == 0x1000)
    strName = "RAS";
  else if (iCaid >= 0x1200 &&
           iCaid <= 0x12FF)
    strName = "NagraVision";
  else if (iCaid >= 0x1700 &&
           iCaid <= 0x17FF)
    strName = "BetaCrypt";
  else if (iCaid >= 0x1800 &&
           iCaid <= 0x18FF)
    strName = "NagraVision";
  else if (iCaid == 0x22F0)
    strName = "Codicrypt";
  else if (iCaid == 0x2600)
    strName = "BISS";
  else if (iCaid == 0x4347)
    strName = "CryptOn";
  else if (iCaid == 0x4800)
    strName = "Accessgate";
  else if (iCaid == 0x4900)
    strName = "China Crypt";
  else if (iCaid == 0x4A10)
    strName = "EasyCas";
  else if (iCaid == 0x4A20)
    strName = "AlphaCrypt";
  else if (iCaid == 0x4A70)
    strName = "DreamCrypt";
  else if (iCaid == 0x4A60)
    strName = "SkyCrypt";
  else if (iCaid == 0x4A61)
    strName = "Neotioncrypt";
  else if (iCaid == 0x4A62)
    strName = "SkyCrypt";
  else if (iCaid == 0x4A63)
    strName = "Neotion SHL";
  else if (iCaid >= 0x4A64 &&
           iCaid <= 0x4A6F)
    strName = "SkyCrypt";
  else if (iCaid == 0x4A80)
    strName = "ThalesCrypt";
  else if (iCaid == 0x4AA1)
    strName = "KeyFly";
  else if (iCaid == 0x4ABF)
    strName = "DG-Crypt";
  else if (iCaid >= 0x4AD0 &&
           iCaid <= 0x4AD1)
    strName = "X-Crypt";
  else if (iCaid == 0x4AD4)
    strName = "OmniCrypt";
  else if (iCaid == 0x4AE0)
    strName = "RossCrypt";
  else if (iCaid == 0x5500)
    strName = "Z-Crypt";
  else if (iCaid == 0x5501)
    strName = "Griffin";
  else if (iCaid == 0x5601)
    strName = "Verimatrix";

  if (iCaid >= 0)
    strName += StringUtils::Format(" (%04X)", iCaid);

  return strName;
}

void CPVRChannel::UpdateEncryptionName(void)
{
  CSingleLock lock(m_critSection);
  m_strClientEncryptionName = GetEncryptionName(m_iClientEncryptionSystem);
}

/********** EPG methods **********/

int CPVRChannel::GetEPG(CFileItemList &results) const
{
  CPVREpgPtr epg = GetEPG();
  if (!epg)
  {
    CLog::LogFC(LOGDEBUG, LOGPVR, "Cannot get EPG for channel '%s'",
                m_strChannelName.c_str());
    return -1;
  }

  return epg->Get(results);
}

bool CPVRChannel::ClearEPG() const
{
  CPVREpgPtr epg = GetEPG();
  if (epg)
    epg->Clear();

  return true;
}

CPVREpgInfoTagPtr CPVRChannel::GetEPGNow() const
{
  CPVREpgPtr epg = GetEPG();
  if (epg)
    return epg->GetTagNow();

  CPVREpgInfoTagPtr empty;
  return empty;
}

CPVREpgInfoTagPtr CPVRChannel::GetEPGNext() const
{
  const CPVREpgPtr epg = GetEPG();
  if (epg)
    return epg->GetTagNext();

  return CPVREpgInfoTagPtr();
}

CPVREpgInfoTagPtr CPVRChannel::GetEPGPrevious() const
{
  const CPVREpgPtr epg = GetEPG();
  if (epg)
    return epg->GetTagPrevious();

  return CPVREpgInfoTagPtr();
}

bool CPVRChannel::SetEPGEnabled(bool bEPGEnabled)
{
  CSingleLock lock(m_critSection);

  if (m_bEPGEnabled != bEPGEnabled)
  {
    /* update the EPG flag */
    m_bEPGEnabled = bEPGEnabled;
    SetChanged();
    m_bChanged = true;

    /* clear the previous EPG entries if needed */
    if (!m_bEPGEnabled && m_bEPGCreated)
      ClearEPG();

    return true;
  }

  return false;
}

bool CPVRChannel::SetEPGScraper(const std::string &strScraper)
{
  CSingleLock lock(m_critSection);

  if (m_strEPGScraper != strScraper)
  {
    bool bCleanEPG = !m_strEPGScraper.empty() || strScraper.empty();

    /* update the scraper name */
    m_strEPGScraper = StringUtils::Format("%s", strScraper.c_str());
    SetChanged();
    m_bChanged = true;

    /* clear the previous EPG entries if needed */
    if (bCleanEPG && m_bEPGEnabled && m_bEPGCreated)
      ClearEPG();

    return true;
  }

  return false;
}

void CPVRChannel::SetChannelNumber(const CPVRChannelNumber& channelNumber)
{
  CSingleLock lock(m_critSection);
  m_channelNumber = channelNumber;
}

void CPVRChannel::ToSortable(SortItem& sortable, Field field) const
{
  CSingleLock lock(m_critSection);
  if (field == FieldChannelName)
    sortable[FieldChannelName] = m_strChannelName;
  else if (field == FieldChannelNumber)
    sortable[FieldChannelNumber] = m_channelNumber.FormattedChannelNumber();
  else if (field == FieldLastPlayed)
  {
    const CDateTime lastWatched(m_iLastWatched);
    sortable[FieldLastPlayed] = lastWatched.IsValid() ? lastWatched.GetAsDBDateTime() : StringUtils::Empty;
  }
}

int CPVRChannel::ChannelID(void) const
{
  CSingleLock lock(m_critSection);
  return m_iChannelId;
}

bool CPVRChannel::IsNew(void) const
{
  CSingleLock lock(m_critSection);
  return m_iChannelId <= 0;
}

bool CPVRChannel::IsHidden(void) const
{
  CSingleLock lock(m_critSection);
  return m_bIsHidden;
}

bool CPVRChannel::IsLocked(void) const
{
  CSingleLock lock(m_critSection);
  return m_bIsLocked;
}

std::string CPVRChannel::IconPath(void) const
{
  CSingleLock lock(m_critSection);
  std::string strReturn(m_strIconPath);
  return strReturn;
}

bool CPVRChannel::IsUserSetIcon(void) const
{
  CSingleLock lock(m_critSection);
  return m_bIsUserSetIcon;
}

bool CPVRChannel::IsIconExists() const
{
  return XFILE::CFile::Exists(IconPath());
}

bool CPVRChannel::IsUserSetName() const
{
  CSingleLock lock(m_critSection);
  return m_bIsUserSetName;
}

std::string CPVRChannel::ChannelName(void) const
{
  CSingleLock lock(m_critSection);
  return m_strChannelName;
}

time_t CPVRChannel::LastWatched(void) const
{
  CSingleLock lock(m_critSection);
  return m_iLastWatched;
}

bool CPVRChannel::IsChanged() const
{
  CSingleLock lock(m_critSection);
  return m_bChanged;
}

void CPVRChannel::Persisted()
{
  CSingleLock lock(m_critSection);
  m_bChanged = false;
}

int CPVRChannel::UniqueID(void) const
{
  return m_iUniqueId;
}

int CPVRChannel::ClientID(void) const
{
  CSingleLock lock(m_critSection);
  return m_iClientId;
}

const CPVRChannelNumber& CPVRChannel::ClientChannelNumber() const
{
  CSingleLock lock(m_critSection);
  return m_clientChannelNumber;
}

std::string CPVRChannel::ClientChannelName(void) const
{
  CSingleLock lock(m_critSection);
  std::string strReturn(m_strClientChannelName);
  return strReturn;
}

std::string CPVRChannel::InputFormat(void) const
{
  CSingleLock lock(m_critSection);
  std::string strReturn(m_strInputFormat);
  return strReturn;
}

std::string CPVRChannel::Path(void) const
{
  CSingleLock lock(m_critSection);
  std::string strReturn(m_strFileNameAndPath);
  return strReturn;
}

bool CPVRChannel::IsEncrypted(void) const
{
  CSingleLock lock(m_critSection);
  return m_iClientEncryptionSystem > 0;
}

int CPVRChannel::EncryptionSystem(void) const
{
  CSingleLock lock(m_critSection);
  return m_iClientEncryptionSystem;
}

std::string CPVRChannel::EncryptionName(void) const
{
  CSingleLock lock(m_critSection);
  std::string strReturn(m_strClientEncryptionName);
  return strReturn;
}

int CPVRChannel::EpgID(void) const
{
  CSingleLock lock(m_critSection);
  return m_iEpgId;
}

void CPVRChannel::SetEpgID(int iEpgId)
{
  CSingleLock lock(m_critSection);

  if (m_iEpgId != iEpgId)
  {
    m_iEpgId = iEpgId;
    SetChanged();
    m_bChanged = true;
  }
}

bool CPVRChannel::EPGEnabled(void) const
{
  CSingleLock lock(m_critSection);
  return m_bEPGEnabled;
}

std::string CPVRChannel::EPGScraper(void) const
{
  CSingleLock lock(m_critSection);
  std::string strReturn(m_strEPGScraper);
  return strReturn;
}

bool CPVRChannel::CanRecord(void) const
{
  const CPVRClientPtr client = CServiceBroker::GetPVRManager().GetClient(m_iClientId);
  return client && client->GetClientCapabilities().SupportsRecordings();
}
