/*
 *  Copyright (C) 2012-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "PVRChannelGroupsContainer.h"

#include "GoTvUrl.h"
#include "guilib/LocalizeStrings.h"
#include "utils/StringUtils.h"
#include "utils/URIUtils.h"
#include "utils/log.h"

#include "pvr/PVRManager.h"

using namespace PVR;

CPVRChannelGroupsContainer::CPVRChannelGroupsContainer(void) :
    m_groupsRadio(new CPVRChannelGroups(true)),
    m_groupsTV(new CPVRChannelGroups(false))
{
}

CPVRChannelGroupsContainer::~CPVRChannelGroupsContainer(void)
{
  delete m_groupsRadio;
  delete m_groupsTV;
}

bool CPVRChannelGroupsContainer::Update(bool bChannelsOnly /* = false */)
{
  CSingleLock lock(m_critSection);
  if (m_bIsUpdating)
    return false;
  m_bIsUpdating = true;
  m_bUpdateChannelsOnly = bChannelsOnly;
  lock.Leave();

  CLog::LogFC(LOGDEBUG, LOGPVR, "Updating %s", bChannelsOnly ? "channels" : "channel groups");
  bool bReturn = m_groupsTV->Update(bChannelsOnly) && m_groupsRadio->Update(bChannelsOnly);

  lock.Enter();
  m_bIsUpdating = false;
  lock.Leave();

  return bReturn;
}

bool CPVRChannelGroupsContainer::Load(void)
{
  Unload();
  m_bLoaded = m_groupsTV->Load() && m_groupsRadio->Load();
  return m_bLoaded;
}

bool CPVRChannelGroupsContainer::Loaded(void) const
{
  return m_bLoaded;
}

void CPVRChannelGroupsContainer::Unload(void)
{
  m_groupsRadio->Clear();
  m_groupsTV->Clear();
  m_bLoaded = false;
}

CPVRChannelGroups *CPVRChannelGroupsContainer::Get(bool bRadio) const
{
  return bRadio ? m_groupsRadio : m_groupsTV;
}

CPVRChannelGroupPtr CPVRChannelGroupsContainer::GetGroupAll(bool bRadio) const
{
  return Get(bRadio)->GetGroupAll();
}

CPVRChannelGroupPtr CPVRChannelGroupsContainer::GetByIdFromAll(int iGroupId) const
{
  CPVRChannelGroupPtr group = m_groupsTV->GetById(iGroupId);
  if (!group)
    group = m_groupsRadio->GetById(iGroupId);

  return group;
}

CPVRChannelPtr CPVRChannelGroupsContainer::GetChannelById(int iChannelId) const
{
  CPVRChannelPtr channel = m_groupsTV->GetGroupAll()->GetByChannelID(iChannelId);
  if (!channel)
    channel = m_groupsRadio->GetGroupAll()->GetByChannelID(iChannelId);

  return channel;
}

CPVRChannelPtr CPVRChannelGroupsContainer::GetChannelByEpgId(int iEpgId) const
{
  CPVRChannelPtr channel = m_groupsTV->GetGroupAll()->GetByChannelEpgID(iEpgId);
  if (!channel)
    channel = m_groupsRadio->GetGroupAll()->GetByChannelEpgID(iEpgId);

  return channel;
}

bool CPVRChannelGroupsContainer::GetGroupsDirectory(CFileItemList *results, bool bRadio) const
{
  const CPVRChannelGroups *channelGroups = Get(bRadio);
  if (channelGroups)
  {
    channelGroups->GetGroupList(results);
    return true;
  }
  return false;
}

CFileItemPtr CPVRChannelGroupsContainer::GetByPath(const std::string &strPath) const
{
  for (unsigned int bRadio = 0; bRadio <= 1; ++bRadio)
  {
    const CPVRChannelGroups *groups = Get(bRadio == 1);
    CFileItemPtr retVal = groups->GetByPath(strPath);
    if (retVal && retVal->HasPVRChannelInfoTag())
      return retVal;
  }

  CFileItemPtr retVal(new CFileItem);
  return retVal;
}

bool CPVRChannelGroupsContainer::GetDirectory(const std::string& strPath, CFileItemList &results) const
{
  std::string strBase(strPath);
  URIUtils::RemoveSlashAtEnd(strBase);

  /* get the filename from curl */
  GoTvUrl url(strPath);
  std::string fileName = url.GetFileName();
  URIUtils::RemoveSlashAtEnd(fileName);

  if (fileName == "channels")
  {
    CFileItemPtr item;

    /* all tv channels */
    item.reset(new CFileItem(strBase + "/tv/", true));
    item->SetLabel(g_localizeStrings.Get(19020));
    item->SetLabelPreformatted(true);
    results.Add(item);

    /* all radio channels */
    item.reset(new CFileItem(strBase + "/radio/", true));
    item->SetLabel(g_localizeStrings.Get(19021));
    item->SetLabelPreformatted(true);
    results.Add(item);

    return true;
  }
  else if (fileName == "channels/tv")
  {
    return GetGroupsDirectory(&results, false);
  }
  else if (fileName == "channels/radio")
  {
    return GetGroupsDirectory(&results, true);
  }
  else if (StringUtils::StartsWith(fileName, "channels/tv/"))
  {
    std::string strGroupName(fileName.substr(12));
    URIUtils::RemoveSlashAtEnd(strGroupName);

    CPVRChannelGroupPtr group;
    bool bShowHiddenChannels = StringUtils::EndsWithNoCase(fileName, ".hidden");
    if (strGroupName == "*" || bShowHiddenChannels) // all channels
      group = GetGroupAllTV();
    else
      group = GetTV()->GetByName(strGroupName);

    if (group)
    {
      group->GetMembers(results, bShowHiddenChannels ? CPVRChannelGroup::Include::ONLY_HIDDEN : CPVRChannelGroup::Include::ONLY_VISIBLE);
    }
    else
    {
      CLog::LogF(LOGERROR, "Unable to obtain members of channel group '%s'", strGroupName.c_str());
      return false;
    }

    FilterDirectory(url, results);
    return true;
  }
  else if (StringUtils::StartsWith(fileName, "channels/radio/"))
  {
    std::string strGroupName(fileName.substr(15));
    URIUtils::RemoveSlashAtEnd(strGroupName);

    CPVRChannelGroupPtr group;
    bool bShowHiddenChannels = StringUtils::EndsWithNoCase(fileName, ".hidden");
    if (strGroupName == "*" || bShowHiddenChannels) // all channels
      group = GetGroupAllRadio();
    else
      group = GetRadio()->GetByName(strGroupName);

    if (group)
    {
      group->GetMembers(results, bShowHiddenChannels ? CPVRChannelGroup::Include::ONLY_HIDDEN : CPVRChannelGroup::Include::ONLY_VISIBLE);
    }
    else
    {
      CLog::LogF(LOGERROR, "Unable to obtain members of channel group '%s'", strGroupName.c_str());
      return false;
    }

    FilterDirectory(url, results);
    return true;
  }

  return false;
}

bool CPVRChannelGroupsContainer::FilterDirectory(const GoTvUrl &url, CFileItemList &results) const
{
  if (!results.IsEmpty())
  {
    if (url.HasOption("view"))
    {
      const std::string view(url.GetOption("view"));
      if (view == "lastplayed")
      {
        // remove channels never played so far
        for (int i = 0; i < results.Size(); ++i)
        {
          const CPVRChannelPtr channel(results.Get(i)->GetPVRChannelInfoTag());
          time_t lastWatched = channel->LastWatched();
          if (!lastWatched)
          {
            results.Remove(i);
            --i;
          }
        }
      }
      else
      {
        CLog::LogF(LOGERROR, "Unsupported value '%s' for channel list URL parameter 'view'", view.c_str());
        return false;
      }
    }
  }
  return true;
}

CPVRChannelGroupPtr CPVRChannelGroupsContainer::GetSelectedGroup(bool bRadio) const
{
  return Get(bRadio)->GetSelectedGroup();
}

CPVRChannelPtr CPVRChannelGroupsContainer::GetByUniqueID(int iUniqueChannelId, int iClientID) const
{
  CPVRChannelPtr channel;
  CPVRChannelGroupPtr channelgroup = GetGroupAllTV();
  if (channelgroup)
    channel = channelgroup->GetByUniqueID(iUniqueChannelId, iClientID);

  if (!channelgroup || !channel)
    channelgroup = GetGroupAllRadio();
  if (channelgroup)
    channel = channelgroup->GetByUniqueID(iUniqueChannelId, iClientID);

  return channel;
}

void CPVRChannelGroupsContainer::SearchMissingChannelIcons(void) const
{
  CLog::Log(LOGINFO, "Starting PVR channel icon search");

  CPVRChannelGroupPtr channelgrouptv  = GetGroupAllTV();
  CPVRChannelGroupPtr channelgroupradio = GetGroupAllRadio();

  if (channelgrouptv)
    channelgrouptv->SearchAndSetChannelIcons(true);
  if (channelgroupradio)
    channelgroupradio->SearchAndSetChannelIcons(true);
}

CFileItemPtr CPVRChannelGroupsContainer::GetLastPlayedChannel(void) const
{
  CFileItemPtr channelTV = m_groupsTV->GetGroupAll()->GetLastPlayedChannel();
  CFileItemPtr channelRadio = m_groupsRadio->GetGroupAll()->GetLastPlayedChannel();

  if (!channelTV ||
      (channelRadio && channelRadio->GetPVRChannelInfoTag()->LastWatched() > channelTV->GetPVRChannelInfoTag()->LastWatched()))
     return channelRadio;

  return channelTV;
}

CPVRChannelGroupPtr CPVRChannelGroupsContainer::GetLastPlayedGroup(int iChannelID /* = -1 */) const
{
  CPVRChannelGroupPtr groupTV = m_groupsTV->GetLastPlayedGroup(iChannelID);
  CPVRChannelGroupPtr groupRadio = m_groupsRadio->GetLastPlayedGroup(iChannelID);

  if (!groupTV || (groupRadio && groupTV->LastWatched() < groupRadio->LastWatched()))
    return groupRadio;

  return groupTV;
}

bool CPVRChannelGroupsContainer::CreateChannelEpgs(void)
{
  return m_groupsTV->CreateChannelEpgs() && m_groupsRadio->CreateChannelEpgs();
}

CPVRChannelGroupPtr CPVRChannelGroupsContainer::GetPreviousPlayedGroup(void)
{
  CSingleLock lock(m_critSection);
  return m_lastPlayedGroups[0];
}

void CPVRChannelGroupsContainer::SetLastPlayedGroup(const CPVRChannelGroupPtr &group)
{
  CSingleLock lock(m_critSection);
  m_lastPlayedGroups[0] = m_lastPlayedGroups[1];
  m_lastPlayedGroups[1] = group;
}
