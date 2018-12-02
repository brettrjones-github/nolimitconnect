/*
 *  Copyright (C) 2012-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "GUIEPGGridContainerModel.h"

#include <cmath>

#include "FileItem.h"
#include "ServiceBroker.h"
#include "utils/Variant.h"
#include "utils/log.h"

#include "pvr/PVRManager.h"
#include "pvr/channels/PVRChannel.h"
#include "pvr/epg/EpgInfoTag.h"

class CGUIListItem;
typedef std::shared_ptr<CGUIListItem> CGUIListItemPtr;

using namespace PVR;

static const unsigned int GRID_START_PADDING = 30; // minutes

void CGUIEPGGridContainerModel::SetInvalid()
{
  for (const auto &programme : m_programmeItems)
    programme->SetInvalid();
  for (const auto &channel : m_channelItems)
    channel->SetInvalid();
  for (const auto &ruler : m_rulerItems)
    ruler->SetInvalid();
}

void CGUIEPGGridContainerModel::Initialize(const std::unique_ptr<CFileItemList> &items, const CDateTime &gridStart, const CDateTime &gridEnd, int iRulerUnit, int iBlocksPerPage, float fBlockSize)
{
  if (!m_channelItems.empty())
  {
    CLog::LogF(LOGERROR, "Already initialized!");
    return;
  }

  ////////////////////////////////////////////////////////////////////////
  // Create programme & channel items
  m_programmeItems.reserve(items->Size());
  CFileItemPtr fileItem;
  int iLastChannelID = -1;
  ItemsPtr itemsPointer;
  itemsPointer.start = 0;
  CPVRChannelPtr channel;
  int j = 0;
  for (int i = 0; i < items->Size(); ++i)
  {
    fileItem = items->Get(i);
    if (!fileItem->HasEPGInfoTag() || !fileItem->GetEPGInfoTag()->HasChannel())
      continue;

    m_programmeItems.emplace_back(fileItem);

    channel = fileItem->GetEPGInfoTag()->Channel();
    if (!channel)
      continue;

    int iCurrentChannelID = channel->ChannelID();
    if (iCurrentChannelID != iLastChannelID)
    {
      if (j > 0)
      {
        itemsPointer.stop = j - 1;
        m_epgItemsPtr.emplace_back(itemsPointer);
        itemsPointer.start = j;
      }
      iLastChannelID = iCurrentChannelID;
      m_channelItems.emplace_back(CFileItemPtr(new CFileItem(channel)));
    }
    ++j;
  }
  if (!m_programmeItems.empty())
  {
    itemsPointer.stop = m_programmeItems.size() - 1;
    m_epgItemsPtr.emplace_back(itemsPointer);
  }

  /* check for invalid start and end time */
  if (gridStart >= gridEnd)
  {
    // default to start "now minus GRID_START_PADDING minutes" and end "start plus one page".
    m_gridStart = CDateTime::GetUTCDateTime() - CDateTimeSpan(0, 0, GetGridStartPadding(), 0);
    m_gridEnd = m_gridStart + CDateTimeSpan(0, 0, iBlocksPerPage * MINSPERBLOCK, 0);
  }
  else if (gridStart > (CDateTime::GetUTCDateTime() - CDateTimeSpan(0, 0, GetGridStartPadding(), 0)))
  {
    // adjust to start "now minus GRID_START_PADDING minutes".
    m_gridStart = CDateTime::GetUTCDateTime() - CDateTimeSpan(0, 0, GetGridStartPadding(), 0);
    m_gridEnd = gridEnd;
  }
  else
  {
    m_gridStart = gridStart;
    m_gridEnd = gridEnd;
  }

  // roundup
  m_gridStart = CDateTime(m_gridStart.GetYear(), m_gridStart.GetMonth(), m_gridStart.GetDay(), m_gridStart.GetHour(), m_gridStart.GetMinute() >= 30 ? 30 : 0, 0);
  m_gridEnd = CDateTime(m_gridEnd.GetYear(), m_gridEnd.GetMonth(), m_gridEnd.GetDay(), m_gridEnd.GetHour(), m_gridEnd.GetMinute() >= 30 ? 30 : 0, 0);

  ////////////////////////////////////////////////////////////////////////
  // Create ruler items
  CDateTime ruler;
  ruler.SetFromUTCDateTime(m_gridStart);
  CDateTime rulerEnd;
  rulerEnd.SetFromUTCDateTime(m_gridEnd);
  CFileItemPtr rulerItem(new CFileItem(ruler.GetAsLocalizedDate(true)));
  rulerItem->SetProperty("DateLabel", true);
  m_rulerItems.emplace_back(rulerItem);

  const CDateTimeSpan unit(0, 0, iRulerUnit * MINSPERBLOCK, 0);
  for (; ruler < rulerEnd; ruler += unit)
  {
    rulerItem.reset(new CFileItem(ruler.GetAsLocalizedTime("", false)));
    rulerItem->SetLabel2(ruler.GetAsLocalizedDate(true));
    m_rulerItems.emplace_back(rulerItem);
  }

  FreeItemsMemory();

  ////////////////////////////////////////////////////////////////////////
  // Create epg grid
  const CDateTimeSpan blockDuration(0, 0, MINSPERBLOCK, 0);
  const CDateTimeSpan gridDuration(m_gridEnd - m_gridStart);
  m_blocks = (gridDuration.GetDays() * 24 * 60 + gridDuration.GetHours() * 60 + gridDuration.GetMinutes()) / MINSPERBLOCK;
  if (m_blocks >= MAXBLOCKS)
    m_blocks = MAXBLOCKS;
  else if (m_blocks < iBlocksPerPage)
    m_blocks = iBlocksPerPage;

  m_gridIndex.reserve(m_channelItems.size());
  const std::vector<GridItem> blocks(m_blocks);

  for (size_t channel = 0; channel < m_channelItems.size(); ++channel)
  {
    m_gridIndex.emplace_back(blocks);

    CDateTime gridCursor(m_gridStart); //reset cursor for new channel
    unsigned long progIdx = m_epgItemsPtr[channel].start;
    unsigned long lastIdx = m_epgItemsPtr[channel].stop;
    int iEpgId            = m_programmeItems[progIdx]->GetEPGInfoTag()->EpgID();
    int itemSize          = 1; // size of the programme in blocks
    int savedBlock        = 0;
    CFileItemPtr item;
    CPVREpgInfoTagPtr tag;

    for (int block = 0; block < m_blocks; ++block)
    {
      while (progIdx <= lastIdx)
      {
        item = m_programmeItems[progIdx];
        tag = item->GetEPGInfoTag();

        // Note: Start block of an event is start-time-based calculated block + 1,
        //       unless start times matches exactly the begin of a block.

        if (tag->EpgID() != iEpgId || gridCursor < tag->StartAsUTC() || m_gridEnd <= tag->StartAsUTC())
          break;

        if (gridCursor < tag->EndAsUTC())
        {
          m_gridIndex[channel][block].item = item;
          m_gridIndex[channel][block].progIndex = progIdx;
          break;
        }

        progIdx++;
      }

      gridCursor += blockDuration;

      if (block == 0)
        continue;

      const CFileItemPtr prevItem(m_gridIndex[channel][block - 1].item);
      const CFileItemPtr currItem(m_gridIndex[channel][block].item);

      if (block == m_blocks - 1 || prevItem != currItem)
      {
        // special handling for last block.
        int blockDelta = -1;
        int sizeDelta = 0;
        if (block == m_blocks - 1 && prevItem == currItem)
        {
          itemSize++;
          blockDelta = 0;
          sizeDelta = 1;
        }

        if (prevItem)
        {
          m_gridIndex[channel][savedBlock].item->SetProperty("GenreType", prevItem->GetEPGInfoTag()->GenreType());
        }
        else
        {
          const CPVREpgInfoTagPtr gapTag(new CPVREpgInfoTag(m_channelItems[channel]->GetPVRChannelInfoTag()));
          const CFileItemPtr gapItem(new CFileItem(gapTag));
          for (int i = block + blockDelta; i >= block - itemSize + sizeDelta; --i)
          {
            m_gridIndex[channel][i].item = gapItem;
          }
        }

        float fItemWidth = itemSize * fBlockSize;
        m_gridIndex[channel][savedBlock].originWidth = fItemWidth;
        m_gridIndex[channel][savedBlock].width = fItemWidth;

        itemSize = 1;
        savedBlock = block;

        // special handling for last block.
        if (block == m_blocks - 1 && prevItem != currItem)
        {
          if (currItem)
          {
            m_gridIndex[channel][savedBlock].item->SetProperty("GenreType", currItem->GetEPGInfoTag()->GenreType());
          }
          else
          {
            const CPVREpgInfoTagPtr gapTag(new CPVREpgInfoTag(m_channelItems[channel]->GetPVRChannelInfoTag()));
            const CFileItemPtr gapItem(new CFileItem(gapTag));
            m_gridIndex[channel][block].item = gapItem;
          }

          m_gridIndex[channel][savedBlock].originWidth = fBlockSize; // size always 1 block here
          m_gridIndex[channel][savedBlock].width = fBlockSize;
        }
      }
      else
      {
        itemSize++;
      }
    }
  }
}

void CGUIEPGGridContainerModel::FindChannelAndBlockIndex(int channelUid, unsigned int broadcastUid, int eventOffset, int &newChannelIndex, int &newBlockIndex) const
{
  const CDateTimeSpan blockDuration(0, 0, MINSPERBLOCK, 0);

  newChannelIndex = INVALID_INDEX;
  newBlockIndex = INVALID_INDEX;

  // find the channel
  int iCurrentChannel = 0;
  for (const auto& channel : m_channelItems)
  {
    if (channel->GetPVRChannelInfoTag()->UniqueID() == channelUid)
    {
      newChannelIndex = iCurrentChannel;
      break;
    }
    iCurrentChannel++;
  }

  if (newChannelIndex != INVALID_INDEX)
  {
    // find the block
    CDateTime gridCursor(m_gridStart); //reset cursor for new channel
    unsigned long progIdx = m_epgItemsPtr[newChannelIndex].start;
    unsigned long lastIdx = m_epgItemsPtr[newChannelIndex].stop;
    int iEpgId = m_programmeItems[progIdx]->GetEPGInfoTag()->EpgID();
    CPVREpgInfoTagPtr tag;
    for (int block = 0; block < m_blocks; ++block)
    {
      while (progIdx <= lastIdx)
      {
        tag = m_programmeItems[progIdx]->GetEPGInfoTag();

        if (tag->EpgID() != iEpgId || gridCursor < tag->StartAsUTC() || m_gridEnd <= tag->StartAsUTC())
          break; // next block

        if (gridCursor < tag->EndAsUTC())
        {
          if (broadcastUid > 0 && tag->UniqueBroadcastID() == broadcastUid)
          {
            newBlockIndex = block + eventOffset;
            return; // done.
          }
          break; // next block
        }
        progIdx++;
      }
      gridCursor += blockDuration;
    }
  }
}

unsigned int CGUIEPGGridContainerModel::GetGridStartPadding() const
{
  unsigned int iPastMinutes = CServiceBroker::GetPVRManager().EpgContainer().GetPastDaysToDisplay() * 24 * 60;

  if (iPastMinutes < GRID_START_PADDING)
    return iPastMinutes;

  return GRID_START_PADDING; // minutes
}

void CGUIEPGGridContainerModel::FreeChannelMemory(int keepStart, int keepEnd)
{
  if (keepStart < keepEnd)
  {
    // remove before keepStart and after keepEnd
    for (int i = 0; i < keepStart && i < ChannelItemsSize(); ++i)
      m_channelItems[i]->FreeMemory();
    for (int i = keepEnd + 1; i < ChannelItemsSize(); ++i)
      m_channelItems[i]->FreeMemory();
  }
  else
  {
    // wrapping
    for (int i = keepEnd + 1; i < keepStart && i < ChannelItemsSize(); ++i)
      m_channelItems[i]->FreeMemory();
  }
}

void CGUIEPGGridContainerModel::FreeProgrammeMemory(int channel, int keepStart, int keepEnd)
{
  if (keepStart < keepEnd)
  {
    // remove before keepStart and after keepEnd
    if (keepStart > 0 && keepStart < m_blocks)
    {
      // if item exist and block is not part of visible item
      CGUIListItemPtr last(m_gridIndex[channel][keepStart].item);
      for (int i = keepStart - 1; i > 0; --i)
      {
        if (m_gridIndex[channel][i].item && m_gridIndex[channel][i].item != last)
        {
          m_gridIndex[channel][i].item->FreeMemory();
          // FreeMemory() is smart enough to not cause any problems when called multiple times on same item
          // but we can make use of condition needed to not call FreeMemory() on item that is partially visible
          // to avoid calling FreeMemory() multiple times on item that occupy few blocks in a row
          last = m_gridIndex[channel][i].item;
        }
      }
    }

    if (keepEnd > 0 && keepEnd < m_blocks)
    {
      CGUIListItemPtr last(m_gridIndex[channel][keepEnd].item);
      for (int i = keepEnd + 1; i < m_blocks; ++i)
      {
        // if item exist and block is not part of visible item
        if (m_gridIndex[channel][i].item && m_gridIndex[channel][i].item != last)
        {
          m_gridIndex[channel][i].item->FreeMemory();
          // FreeMemory() is smart enough to not cause any problems when called multiple times on same item
          // but we can make use of condition needed to not call FreeMemory() on item that is partially visible
          // to avoid calling FreeMemory() multiple times on item that occupy few blocks in a row
          last = m_gridIndex[channel][i].item;
        }
      }
    }
  }
}

void CGUIEPGGridContainerModel::FreeRulerMemory(int keepStart, int keepEnd)
{
  if (keepStart < keepEnd)
  {
    // remove before keepStart and after keepEnd
    for (int i = 1; i < keepStart && i < RulerItemsSize(); ++i)
      m_rulerItems[i]->FreeMemory();
    for (int i = keepEnd + 1; i < RulerItemsSize(); ++i)
      m_rulerItems[i]->FreeMemory();
  }
  else
  {
    // wrapping
    for (int i = keepEnd + 1; i < keepStart && i < RulerItemsSize(); ++i)
    {
      if (i == 0)
        continue;

      m_rulerItems[i]->FreeMemory();
    }
  }
}

void CGUIEPGGridContainerModel::FreeItemsMemory()
{
  for (const auto &programme : m_programmeItems)
    programme->FreeMemory();
  for (const auto &channel : m_channelItems)
    channel->FreeMemory();
  for (const auto &ruler : m_rulerItems)
    ruler->FreeMemory();
}

unsigned int CGUIEPGGridContainerModel::GetPageNowOffset() const
{
  return GetGridStartPadding() / MINSPERBLOCK; // this is the 'now' block relative to page start
}

CDateTime CGUIEPGGridContainerModel::GetStartTimeForBlock(int block) const
{
  if (block < 0)
    block = 0;
  else if (block >= m_blocks)
    block = m_blocks - 1;

  return m_gridStart + CDateTimeSpan(0, 0 , block * MINSPERBLOCK, 0);
}

int CGUIEPGGridContainerModel::GetBlock(const CDateTime &datetime) const
{
  int diff;

  if (m_gridStart == datetime)
    return 0; // block is at grid start
  else if (m_gridStart > datetime)
    diff = -1 * (m_gridStart - datetime).GetSecondsTotal(); // block is before grid start
  else
    diff = (datetime - m_gridStart).GetSecondsTotal(); // block is after grid start

  return diff / 60 / MINSPERBLOCK;
}

int CGUIEPGGridContainerModel::GetNowBlock() const
{
  return GetBlock(CDateTime::GetUTCDateTime()) - GetPageNowOffset();
}

int CGUIEPGGridContainerModel::GetFirstEventBlock(const CPVREpgInfoTagPtr &event) const
{
  const CDateTime eventStart = event->StartAsUTC();
  int diff;

  if (m_gridStart == eventStart)
    return 0; // block is at grid start
  else if (m_gridStart > eventStart)
    diff = -1 * (m_gridStart - eventStart).GetSecondsTotal();
  else
    diff = (eventStart - m_gridStart).GetSecondsTotal();

  // First block of a tag is always the block calculated using event's start time, rounded up.
  // Refer to CGUIEPGGridContainerModel::Refresh, where the model is created, for details!
  float fBlockIndex = diff / 60.0f / MINSPERBLOCK;
  return std::ceil(fBlockIndex);
}

int CGUIEPGGridContainerModel::GetLastEventBlock(const CPVREpgInfoTagPtr &event) const
{
  // Last block of a tag is always the block calculated using event's end time, not rounded up.
  // Refer to CGUIEPGGridContainerModel::Refresh, where the model is created, for details!
  return GetBlock(event->EndAsUTC());
}
