/*
 *  Copyright (C) 2012-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "PVRTimerType.h"

#include "ServiceBroker.h"
#include "addons/PVRClient.h"
#include "guilib/LocalizeStrings.h"
#include "utils/StringUtils.h"
#include "utils/log.h"

#include "pvr/PVRManager.h"
#include "pvr/addons/PVRClients.h"

using namespace PVR;

const std::vector<CPVRTimerTypePtr> CPVRTimerType::GetAllTypes()
{
  std::vector<CPVRTimerTypePtr> allTypes;
  CServiceBroker::GetPVRManager().Clients()->GetTimerTypes(allTypes);
  return allTypes;
}

const CPVRTimerTypePtr CPVRTimerType::GetFirstAvailableType()
{
  std::vector<CPVRTimerTypePtr> allTypes(GetAllTypes());
  return allTypes.empty() ? CPVRTimerTypePtr() : *(allTypes.begin());
}

CPVRTimerTypePtr CPVRTimerType::CreateFromIds(unsigned int iTypeId, int iClientId)
{
  const CPVRClientPtr client = CServiceBroker::GetPVRManager().GetClient(iClientId);
  if (client)
  {
    std::vector<CPVRTimerTypePtr> types;
    if (client->GetTimerTypes(types) == PVR_ERROR_NO_ERROR)
    {
      for (const auto &type : types)
      {
        if (type->GetTypeId() == iTypeId)
          return type;
      }
    }
  }

  CLog::LogF(LOGERROR, "Unable to resolve numeric timer type (%d, %d)", iTypeId, iClientId);
  return CPVRTimerTypePtr();
}

CPVRTimerTypePtr CPVRTimerType::CreateFromAttributes(
  unsigned int iMustHaveAttr, unsigned int iMustNotHaveAttr, int iClientId)
{
  const CPVRClientPtr client = CServiceBroker::GetPVRManager().GetClient(iClientId);
  if (client)
  {
    std::vector<CPVRTimerTypePtr> types;
    if (client->GetTimerTypes(types) == PVR_ERROR_NO_ERROR)
    {
      for (const auto &type : types)
      {
        if (((type->m_iAttributes & iMustHaveAttr)    == iMustHaveAttr) &&
            ((type->m_iAttributes & iMustNotHaveAttr) == 0))
          return type;
      }
    }
  }

  CLog::LogF(LOGERROR, "Unable to resolve timer type (0x%x, 0x%x, %d)", iMustHaveAttr, iMustNotHaveAttr, iClientId);
  return CPVRTimerTypePtr();
}

CPVRTimerType::CPVRTimerType() :
  m_iTypeId(PVR_TIMER_TYPE_NONE),
  m_iAttributes(PVR_TIMER_TYPE_ATTRIBUTE_NONE)
{
}

CPVRTimerType::CPVRTimerType(const PVR_TIMER_TYPE &type, int iClientId) :
  m_iClientId(iClientId),
  m_iTypeId(type.iId),
  m_iAttributes(type.iAttributes),
  m_strDescription(type.strDescription)
{
  InitAttributeValues(type);
}

CPVRTimerType::~CPVRTimerType() = default;

bool CPVRTimerType::operator ==(const CPVRTimerType& right) const
{
  return (m_iClientId                  == right.m_iClientId   &&
          m_iTypeId                    == right.m_iTypeId  &&
          m_iAttributes                == right.m_iAttributes &&
          m_strDescription             == right.m_strDescription &&
          m_priorityValues             == right.m_priorityValues &&
          m_iPriorityDefault           == right.m_iPriorityDefault &&
          m_lifetimeValues             == right.m_lifetimeValues &&
          m_iLifetimeDefault           == right.m_iLifetimeDefault &&
          m_maxRecordingsValues        == right.m_maxRecordingsValues &&
          m_iMaxRecordingsDefault      == right.m_iMaxRecordingsDefault &&
          m_preventDupEpisodesValues   == right.m_preventDupEpisodesValues &&
          m_iPreventDupEpisodesDefault == right.m_iPreventDupEpisodesDefault &&
          m_recordingGroupValues       == right.m_recordingGroupValues &&
          m_iRecordingGroupDefault     == right.m_iRecordingGroupDefault);
}

bool CPVRTimerType::operator !=(const CPVRTimerType& right) const
{
  return !(*this == right);
}

void CPVRTimerType::InitAttributeValues(const PVR_TIMER_TYPE &type)
{
  InitPriorityValues(type);
  InitLifetimeValues(type);
  InitMaxRecordingsValues(type);
  InitPreventDuplicateEpisodesValues(type);
  InitRecordingGroupValues(type);
}

void CPVRTimerType::InitPriorityValues(const PVR_TIMER_TYPE &type)
{
  if (type.iPrioritiesSize > 0)
  {
    for (unsigned int i = 0; i < type.iPrioritiesSize; ++i)
    {
      std::string strDescr(type.priorities[i].strDescription);
      if (strDescr.empty())
      {
        // No description given by addon. Create one from value.
        strDescr = StringUtils::Format("%d", type.priorities[i].iValue);
      }
      m_priorityValues.push_back(std::make_pair(strDescr, type.priorities[i].iValue));
    }

    m_iPriorityDefault = type.iPrioritiesDefault;
  }
  else if (SupportsPriority())
  {
    // No values given by addon, but priority supported. Use default values 1..100
    for (int i = 1; i < 101; ++i)
      m_priorityValues.push_back(std::make_pair(StringUtils::Format("%d", i), i));

    m_iPriorityDefault = DEFAULT_RECORDING_PRIORITY;
  }
  else
  {
    // No priority supported.
    m_iPriorityDefault = DEFAULT_RECORDING_PRIORITY;
  }
}

void CPVRTimerType::GetPriorityValues(std::vector< std::pair<std::string, int> > &list) const
{
  for (const auto &prio : m_priorityValues)
    list.push_back(prio);
}

void CPVRTimerType::InitLifetimeValues(const PVR_TIMER_TYPE &type)
{
  if (type.iLifetimesSize > 0)
  {
    for (unsigned int i = 0; i < type.iLifetimesSize; ++i)
    {
      int iValue = type.lifetimes[i].iValue;
      std::string strDescr(type.lifetimes[i].strDescription);
      if (strDescr.empty())
      {
        // No description given by addon. Create one from value.
        strDescr = StringUtils::Format("%d", iValue);
      }
      m_lifetimeValues.push_back(std::make_pair(strDescr, iValue));
    }

    m_iLifetimeDefault = type.iLifetimesDefault;
  }
  else if (SupportsLifetime())
  {
    // No values given by addon, but lifetime supported. Use default values 1..365
    for (int i = 1; i < 366; ++i)
    {
      m_lifetimeValues.push_back(std::make_pair(StringUtils::Format(g_localizeStrings.Get(17999).c_str(), i), i)); // "%s days"
    }
    m_iLifetimeDefault = DEFAULT_RECORDING_LIFETIME;
  }
  else
  {
    // No lifetime supported.
    m_iLifetimeDefault = DEFAULT_RECORDING_LIFETIME;
  }
}

void CPVRTimerType::GetLifetimeValues(std::vector< std::pair<std::string, int> > &list) const
{
  for (const auto &lifetime : m_lifetimeValues)
    list.push_back(lifetime);
}

void CPVRTimerType::InitMaxRecordingsValues(const PVR_TIMER_TYPE &type)
{
  if (type.iMaxRecordingsSize > 0)
  {
    for (unsigned int i = 0; i < type.iMaxRecordingsSize; ++i)
    {
      std::string strDescr(type.maxRecordings[i].strDescription);
      if (strDescr.empty())
      {
        // No description given by addon. Create one from value.
        strDescr = StringUtils::Format("%d", type.maxRecordings[i].iValue);
      }
      m_maxRecordingsValues.push_back(std::make_pair(strDescr, type.maxRecordings[i].iValue));
    }

    m_iMaxRecordingsDefault = type.iMaxRecordingsDefault;
  }
}

void CPVRTimerType::GetMaxRecordingsValues(std::vector< std::pair<std::string, int> > &list) const
{
  for (const auto &maxRecordings : m_maxRecordingsValues)
    list.push_back(maxRecordings);
}

void CPVRTimerType::InitPreventDuplicateEpisodesValues(const PVR_TIMER_TYPE &type)
{
  if (type.iPreventDuplicateEpisodesSize > 0)
  {
    for (unsigned int i = 0; i < type.iPreventDuplicateEpisodesSize; ++i)
    {
      std::string strDescr(type.preventDuplicateEpisodes[i].strDescription);
      if (strDescr.empty())
      {
        // No description given by addon. Create one from value.
        strDescr = StringUtils::Format("%d", type.preventDuplicateEpisodes[i].iValue);
      }
      m_preventDupEpisodesValues.push_back(std::make_pair(strDescr, type.preventDuplicateEpisodes[i].iValue));
    }

    m_iPreventDupEpisodesDefault = type.iPreventDuplicateEpisodesDefault;
  }
  else if (SupportsRecordOnlyNewEpisodes())
  {
    // No values given by addon, but prevent duplicate episodes supported. Use default values 0..1
    m_preventDupEpisodesValues.push_back(std::make_pair(g_localizeStrings.Get(815), 0)); // "Record all episodes"
    m_preventDupEpisodesValues.push_back(std::make_pair(g_localizeStrings.Get(816), 1)); // "Record only new episodes"
    m_iPreventDupEpisodesDefault = DEFAULT_RECORDING_DUPLICATEHANDLING;
  }
  else
  {
    // No prevent duplicate episodes supported.
    m_iPreventDupEpisodesDefault = DEFAULT_RECORDING_DUPLICATEHANDLING;
  }
}

void CPVRTimerType::GetPreventDuplicateEpisodesValues(std::vector< std::pair<std::string, int> > &list) const
{
  for (const auto &preventDupEpisodes : m_preventDupEpisodesValues)
    list.push_back(preventDupEpisodes);
}

void CPVRTimerType::InitRecordingGroupValues(const PVR_TIMER_TYPE &type)
{
  if (type.iRecordingGroupSize > 0)
  {
    for (unsigned int i = 0; i < type.iRecordingGroupSize; ++i)
    {
      std::string strDescr(type.recordingGroup[i].strDescription);
      if (strDescr.empty())
      {
        // No description given by addon. Create one from value.
        strDescr = StringUtils::Format("%s %d",
                                       g_localizeStrings.Get(811).c_str(), // Recording group
                                       type.recordingGroup[i].iValue);
      }
      m_recordingGroupValues.push_back(std::make_pair(strDescr, type.recordingGroup[i].iValue));
    }

    m_iRecordingGroupDefault = type.iRecordingGroupDefault;
  }
}

void CPVRTimerType::GetRecordingGroupValues(std::vector< std::pair<std::string, int> > &list) const
{
  for (const auto &recordingGroup : m_recordingGroupValues)
    list.push_back(recordingGroup);
}
