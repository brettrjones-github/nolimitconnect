/*
 *  Copyright (C) 2012-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "PVROperations.h"

#include "ServiceBroker.h"

#include "pvr/PVRGUIActions.h"
#include "pvr/PVRManager.h"
#include "pvr/channels/PVRChannelGroupsContainer.h"
#include "pvr/channels/PVRChannel.h"
#include "pvr/epg/Epg.h"
#include "pvr/epg/EpgContainer.h"
#include "pvr/recordings/PVRRecordings.h"
#include "pvr/timers/PVRTimers.h"
#include "utils/Variant.h"

using namespace JSONRPC;
using namespace PVR;
using namespace KODI::MESSAGING;

JSONRPC_STATUS CPVROperations::GetProperties(const std::string &method, ITransportLayer *transport, IClient *client, const CVariant &parameterObject, CVariant &result)
{
  if (!CServiceBroker::GetPVRManager().IsStarted())
    return FailedToExecute;
  
  CVariant properties = CVariant(CVariant::VariantTypeObject);
  for (unsigned int index = 0; index < parameterObject["properties"].size(); index++)
  {
    std::string propertyName = parameterObject["properties"][index].asString();
    CVariant property;
    JSONRPC_STATUS ret;
    if ((ret = GetPropertyValue(propertyName, property)) != OK)
      return ret;

    properties[propertyName] = property;
  }

  result = properties;

  return OK;
}

JSONRPC_STATUS CPVROperations::GetChannelGroups(const std::string &method, ITransportLayer *transport, IClient *client, const CVariant &parameterObject, CVariant &result)
{
  if (!CServiceBroker::GetPVRManager().IsStarted())
    return FailedToExecute;
  
  CPVRChannelGroupsContainerPtr channelGroupContainer = CServiceBroker::GetPVRManager().ChannelGroups();
  if (!channelGroupContainer)
    return FailedToExecute;

  CPVRChannelGroups *channelGroups = channelGroupContainer->Get(parameterObject["channeltype"].asString().compare("radio") == 0);
  if (channelGroups == NULL)
    return FailedToExecute;

  int start, end;

  std::vector<CPVRChannelGroupPtr> groupList = channelGroups->GetMembers(true);
  HandleLimits(parameterObject, result, groupList.size(), start, end);
  for (int index = start; index < end; index++)
    FillChannelGroupDetails(groupList.at(index), parameterObject, result["channelgroups"], true);

  return OK;
}

JSONRPC_STATUS CPVROperations::GetChannelGroupDetails(const std::string &method, ITransportLayer *transport, IClient *client, const CVariant &parameterObject, CVariant &result)
{
  if (!CServiceBroker::GetPVRManager().IsStarted())
    return FailedToExecute;

  CPVRChannelGroupsContainerPtr channelGroupContainer = CServiceBroker::GetPVRManager().ChannelGroups();
  if (!channelGroupContainer)
    return FailedToExecute;
  
  CPVRChannelGroupPtr channelGroup;
  CVariant id = parameterObject["channelgroupid"];
  if (id.isInteger())
    channelGroup = channelGroupContainer->GetByIdFromAll((int)id.asInteger());
  else if (id.isString())
    channelGroup = channelGroupContainer->GetGroupAll(id.asString() == "allradio");

  if (channelGroup == NULL)
    return InvalidParams;
  
  FillChannelGroupDetails(channelGroup, parameterObject, result["channelgroupdetails"], false);
  
  return OK;
}

JSONRPC_STATUS CPVROperations::GetChannels(const std::string &method, ITransportLayer *transport, IClient *client, const CVariant &parameterObject, CVariant &result)
{
  if (!CServiceBroker::GetPVRManager().IsStarted())
    return FailedToExecute;
  
  CPVRChannelGroupsContainerPtr channelGroupContainer = CServiceBroker::GetPVRManager().ChannelGroups();
  if (!channelGroupContainer)
    return FailedToExecute;
  
  CPVRChannelGroupPtr channelGroup;
  CVariant id = parameterObject["channelgroupid"];
  if (id.isInteger())
    channelGroup = channelGroupContainer->GetByIdFromAll((int)id.asInteger());
  else if (id.isString())
    channelGroup = channelGroupContainer->GetGroupAll(id.asString() == "allradio");
  
  if (channelGroup == NULL)
    return InvalidParams;
  
  CFileItemList channels;
  if (channelGroup->GetMembers(channels) < 0)
    return InvalidParams;
  
  HandleFileItemList("channelid", false, "channels", channels, parameterObject, result, true);
    
  return OK;
}

JSONRPC_STATUS CPVROperations::GetChannelDetails(const std::string &method, ITransportLayer *transport, IClient *client, const CVariant &parameterObject, CVariant &result)
{
  if (!CServiceBroker::GetPVRManager().IsStarted())
    return FailedToExecute;
  
  CPVRChannelGroupsContainerPtr channelGroupContainer = CServiceBroker::GetPVRManager().ChannelGroups();
  if (!channelGroupContainer)
    return FailedToExecute;
  
  CPVRChannelPtr channel = channelGroupContainer->GetChannelById((int)parameterObject["channelid"].asInteger());
  if (channel == NULL)
    return InvalidParams;

  HandleFileItem("channelid", false, "channeldetails", CFileItemPtr(new CFileItem(channel)), parameterObject, parameterObject["properties"], result, false);
    
  return OK;
}

JSONRPC_STATUS CPVROperations::GetBroadcasts(const std::string &method, ITransportLayer *transport, IClient *client, const CVariant &parameterObject, CVariant &result)
{
  if (!CServiceBroker::GetPVRManager().IsStarted())
    return FailedToExecute;

  CPVRChannelGroupsContainerPtr channelGroupContainer = CServiceBroker::GetPVRManager().ChannelGroups();
  if (!channelGroupContainer)
    return FailedToExecute;

  CPVRChannelPtr channel = channelGroupContainer->GetChannelById((int)parameterObject["channelid"].asInteger());
  if (channel == NULL)
    return InvalidParams;

  CPVREpgPtr channelEpg = channel->GetEPG();
  if (!channelEpg)
    return InternalError;

  CFileItemList programFull;
  channelEpg->Get(programFull);

  HandleFileItemList("broadcastid", false, "broadcasts", programFull, parameterObject, result, programFull.Size(), true);

  return OK;
}

JSONRPC_STATUS CPVROperations::GetBroadcastDetails(const std::string &method, ITransportLayer *transport, IClient *client, const CVariant &parameterObject, CVariant &result)
{
  if (!CServiceBroker::GetPVRManager().IsStarted())
    return FailedToExecute;

  const CPVREpgInfoTagPtr epgTag = CServiceBroker::GetPVRManager().EpgContainer().GetTagById(CPVRChannelPtr(), parameterObject["broadcastid"].asUnsignedInteger());

  if (!epgTag)
    return InvalidParams;

  HandleFileItem("broadcastid", false, "broadcastdetails", CFileItemPtr(new CFileItem(epgTag)), parameterObject, parameterObject["properties"], result, false);

  return OK;
}


JSONRPC_STATUS CPVROperations::Record(const std::string &method, ITransportLayer *transport, IClient *client, const CVariant &parameterObject, CVariant &result)
{
  if (!CServiceBroker::GetPVRManager().IsStarted())
    return FailedToExecute;

  CPVRChannelPtr pChannel;
  CVariant channel = parameterObject["channel"];
  if (channel.isString() && channel.asString() == "current")
  {
    pChannel = CServiceBroker::GetPVRManager().GetPlayingChannel();
    if (!pChannel)
      return InternalError;
  }
  else if (channel.isInteger())
  {
    CPVRChannelGroupsContainerPtr channelGroupContainer = CServiceBroker::GetPVRManager().ChannelGroups();
    if (!channelGroupContainer)
      return FailedToExecute;

    pChannel = channelGroupContainer->GetChannelById((int)channel.asInteger());
  }
  else
    return InvalidParams;

  if (pChannel == NULL)
    return InvalidParams;
  else if (!pChannel->CanRecord())
    return FailedToExecute;

  CVariant record = parameterObject["record"];
  bool toggle = true;
  if (record.isBoolean() && record.asBoolean() == pChannel->IsRecording())
    toggle = false;

  if (toggle)
  {
    if (!CServiceBroker::GetPVRManager().GUIActions()->SetRecordingOnChannel(pChannel, pChannel->IsRecording()))
      return FailedToExecute;
  }

  return ACK;
}

JSONRPC_STATUS CPVROperations::Scan(const std::string &method, ITransportLayer *transport, IClient *client, const CVariant &parameterObject, CVariant &result)
{
  if (!CServiceBroker::GetPVRManager().IsStarted())
    return FailedToExecute;

  CServiceBroker::GetPVRManager().GUIActions()->StartChannelScan();
  return ACK;
}

JSONRPC_STATUS CPVROperations::GetPropertyValue(const std::string &property, CVariant &result)
{
  bool started = CServiceBroker::GetPVRManager().IsStarted();

  if (property == "available")
    result = started;
  else if (property == "recording")
  {
    if (started)
      result = CServiceBroker::GetPVRManager().IsRecording();
    else
      result = false;
  }
  else if (property == "scanning")
  {
    if (started)
      result = CServiceBroker::GetPVRManager().GUIActions()->IsRunningChannelScan();
    else
      result = false;
  }
  else
    return InvalidParams;

  return OK;
}

void CPVROperations::FillChannelGroupDetails(const CPVRChannelGroupPtr &channelGroup, const CVariant &parameterObject, CVariant &result, bool append /* = false */)
{
  if (channelGroup == NULL)
    return;

  CVariant object(CVariant::VariantTypeObject);
  object["channelgroupid"] = channelGroup->GroupID();
  object["channeltype"] = channelGroup->IsRadio() ? "radio" : "tv";
  object["label"] = channelGroup->GroupName();

  if (append)
    result.append(object);
  else
  {
    CFileItemList channels;
    channelGroup->GetMembers(channels);
    object["channels"] = CVariant(CVariant::VariantTypeArray);
    HandleFileItemList("channelid", false, "channels", channels, parameterObject["channels"], object, false);

    result = object;
  }
}

JSONRPC_STATUS CPVROperations::GetTimers(const std::string &method, ITransportLayer *transport, IClient *client, const CVariant &parameterObject, CVariant &result)
{
  if (!CServiceBroker::GetPVRManager().IsStarted())
    return FailedToExecute;

  CPVRTimersPtr timers = CServiceBroker::GetPVRManager().Timers();
  if (!timers)
    return FailedToExecute;

  CFileItemList timerList;
  timers->GetAll(timerList);

  HandleFileItemList("timerid", false, "timers", timerList, parameterObject, result, true);

  return OK;
}

JSONRPC_STATUS CPVROperations::GetTimerDetails(const std::string &method, ITransportLayer *transport, IClient *client, const CVariant &parameterObject, CVariant &result)
{
  if (!CServiceBroker::GetPVRManager().IsStarted())
    return FailedToExecute;

  CPVRTimersPtr timers = CServiceBroker::GetPVRManager().Timers();
  if (!timers)
    return FailedToExecute;

  CPVRTimerInfoTagPtr timer = timers->GetById((int)parameterObject["timerid"].asInteger());
  if (!timer)
    return InvalidParams;

  HandleFileItem("timerid", false, "timerdetails", CFileItemPtr(new CFileItem(timer)), parameterObject, parameterObject["properties"], result, false);

  return OK;
}

JSONRPC_STATUS CPVROperations::AddTimer(const std::string &method, ITransportLayer *transport, IClient *client, const CVariant &parameterObject, CVariant &result)
{
  if (!CServiceBroker::GetPVRManager().IsStarted())
    return FailedToExecute;

  const CPVREpgInfoTagPtr epgTag = CServiceBroker::GetPVRManager().EpgContainer().GetTagById(CPVRChannelPtr(), parameterObject["broadcastid"].asUnsignedInteger());

  if (!epgTag)
    return InvalidParams;

  if (epgTag->HasTimer())
    return InvalidParams;

  CPVRTimerInfoTagPtr newTimer = CPVRTimerInfoTag::CreateFromEpg(epgTag, parameterObject["timerrule"].asBoolean(false));
  if (newTimer)
  {
    if (CServiceBroker::GetPVRManager().GUIActions()->AddTimer(newTimer))
      return ACK;
  }
  return FailedToExecute;
}


JSONRPC_STATUS CPVROperations::DeleteTimer(const std::string &method, ITransportLayer *transport, IClient *client, const CVariant &parameterObject, CVariant &result)
{
  if (!CServiceBroker::GetPVRManager().IsStarted())
    return FailedToExecute;

  CPVRTimersPtr timers = CServiceBroker::GetPVRManager().Timers();
  if (!timers)
    return FailedToExecute;

  CPVRTimerInfoTagPtr timer = timers->GetById(parameterObject["timerid"].asInteger());
  if (!timer)
    return InvalidParams;

  if (timers->DeleteTimer(timer, timer->IsRecording(), false) == TimerOperationResult::OK)
    return ACK;

  return FailedToExecute;
}

JSONRPC_STATUS CPVROperations::ToggleTimer(const std::string &method, ITransportLayer *transport, IClient *client, const CVariant &parameterObject, CVariant &result)
{
  if (!CServiceBroker::GetPVRManager().IsStarted())
    return FailedToExecute;

  const CPVREpgInfoTagPtr epgTag = CServiceBroker::GetPVRManager().EpgContainer().GetTagById(CPVRChannelPtr(), parameterObject["broadcastid"].asUnsignedInteger());

  if (!epgTag)
    return InvalidParams;

  bool timerrule = parameterObject["timerrule"].asBoolean(false);
  bool sentOkay = false;
  CPVRTimerInfoTagPtr timer(epgTag->Timer());
  if (timer)
  {
    if (timerrule)
      timer = CServiceBroker::GetPVRManager().Timers()->GetTimerRule(timer);

    if (timer)
      sentOkay = (CServiceBroker::GetPVRManager().Timers()->DeleteTimer(timer, timer->IsRecording(), false) == TimerOperationResult::OK);
  }
  else
  {
    timer = CPVRTimerInfoTag::CreateFromEpg(epgTag, timerrule);
    if (!timer)
      return InvalidParams;

    sentOkay = CServiceBroker::GetPVRManager().GUIActions()->AddTimer(timer);
  }

  if (sentOkay)
    return ACK;

  return FailedToExecute;
}

JSONRPC_STATUS CPVROperations::GetRecordings(const std::string &method, ITransportLayer *transport, IClient *client, const CVariant &parameterObject, CVariant &result)
{
  if (!CServiceBroker::GetPVRManager().IsStarted())
    return FailedToExecute;

  CPVRRecordingsPtr recordings = CServiceBroker::GetPVRManager().Recordings();
  if (!recordings)
    return FailedToExecute;

  CFileItemList recordingsList;
  recordings->GetAll(recordingsList);

  HandleFileItemList("recordingid", true, "recordings", recordingsList, parameterObject, result, true);

  return OK;
}

JSONRPC_STATUS CPVROperations::GetRecordingDetails(const std::string &method, ITransportLayer *transport, IClient *client, const CVariant &parameterObject, CVariant &result)
{
  if (!CServiceBroker::GetPVRManager().IsStarted())
    return FailedToExecute;

  CPVRRecordingsPtr recordings = CServiceBroker::GetPVRManager().Recordings();
  if (!recordings)
    return FailedToExecute;

  CFileItemPtr recording = recordings->GetById((int)parameterObject["recordingid"].asInteger());
  if (!recording)
    return InvalidParams;

  HandleFileItem("recordingid", true, "recordingdetails", recording, parameterObject, parameterObject["properties"], result, false);

  return OK;
}
