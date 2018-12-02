/*
 *  Copyright (C) 2012-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include <map>
#include <string>
#include <vector>

#include "FileItem.h"
#include "threads/CriticalSection.h"
#include "utils/Observer.h"

#include "pvr/PVRTypes.h"
#include "pvr/channels/PVRChannel.h"
#include "pvr/epg/EpgInfoTag.h"
#include "pvr/epg/EpgSearchFilter.h"

/** EPG container for CPVREpgInfoTag instances */
namespace PVR
{
  class CPVREpg : public Observable
  {
    friend class CPVREpgDatabase;

  public:
    /*!
     * @brief Create a new EPG instance.
     * @param iEpgID The ID of this table or <= 0 to create a new ID.
     * @param strName The name of this table.
     * @param strScraperName The name of the scraper to use.
     * @param bLoadedFromDb True if this table was loaded from the database, false otherwise.
     */
    CPVREpg(int iEpgID, const std::string &strName, const std::string &strScraperName, bool bLoadedFromDb);

    /*!
     * @brief Create a new EPG instance for a channel.
     * @param channel The channel to create the EPG for.
     */
    CPVREpg(const CPVRChannelPtr &channel);

    /*!
     * @brief Destroy this EPG instance.
     */
    ~CPVREpg(void) override;

    /*!
     * @brief Load all entries for this table from the database.
     * @return True if any entries were loaded, false otherwise.
     */
    bool Load(void);

    /*!
     * @brief The channel this EPG belongs to.
     * @return The channel this EPG belongs to
     */
    CPVRChannelPtr Channel(void) const;

    /*!
     * @brief The id of the channel this EPG belongs to.
     * @return The channel id or -1 if no channel
     */
    int ChannelID(void) const;

    /*!
     * @brief Channel the channel tag linked to this EPG table.
     * @param channel The new channel tag.
     */
    void SetChannel(const CPVRChannelPtr &channel);

    /*!
     * @brief Get the name of the scraper to use for this table.
     * @return The name of the scraper to use for this table.
     */
    const std::string &ScraperName(void) const;

    /*!
     * @brief Returns if there is a manual update pending for this EPG
     * @return True if there is a manual update pending, false otherwise
     */
    bool UpdatePending(void) const;

    /*!
     * @brief Clear the current tags and schedule manual update
     */
    void ForceUpdate(void);

    /*!
     * @brief Get the name of this table.
     * @return The name of this table.
     */
    const std::string &Name(void) const;

    /*!
     * @brief Get the database ID of this table.
     * @return The database ID of this table.
     */
    int EpgID(void) const;

    /*!
     * @brief Check whether this EPG contains valid entries.
     * @return True if it has valid entries, false if not.
     */
    bool HasValidEntries(void) const;

    /*!
     * @brief Remove all entries from this EPG that finished before the given time
     *        and that have no timers set.
     * @param time Delete entries with an end time before this time in UTC.
     */
    void Cleanup(const CDateTime &time);

    /*!
     * @brief Remove all entries from this EPG that finished before the given time
     *        and that have no timers set.
     */
    void Cleanup(void);

    /*!
     * @brief Remove all entries from this EPG.
     */
    void Clear(void);

    /*!
     * @brief Get the event that is occurring now
     * @return The current event or NULL if it wasn't found.
     */
    CPVREpgInfoTagPtr GetTagNow(bool bUpdateIfNeeded = true) const;

    /*!
     * @brief Get the event that will occur next
     * @return The next event or NULL if it wasn't found.
     */
    CPVREpgInfoTagPtr GetTagNext() const;

    /*!
     * @brief Get the event that occured previously
     * @return The previous event or NULL if it wasn't found.
     */
    CPVREpgInfoTagPtr GetTagPrevious() const;

    /*!
     * @brief Get the event that occurs between the given begin and end time.
     * @param beginTime Minimum start time in UTC of the event.
     * @param endTime Maximum end time in UTC of the event.
     * @param bUpdateFromClient if true, try to fetch the event from the client if not found locally.
     * @return The found tag or NULL if it wasn't found.
     */
    CPVREpgInfoTagPtr GetTagBetween(const CDateTime &beginTime, const CDateTime &endTime, bool bUpdateFromClient = false);

    /*!
     * @brief Get all events occurring between the given begin and end time.
     * @param beginTime Minimum start time in UTC of the event.
     * @param endTime Maximum end time in UTC of the event.
     * @return The tags found or an empty vector if none was found.
     */
    std::vector<CPVREpgInfoTagPtr> GetTagsBetween(const CDateTime &beginTime, const CDateTime &endTime) const;

    /*!
     * @brief Get the event matching the given unique broadcast id
     * @param iUniqueBroadcastId The uid to look up
     * @return The matching event or NULL if it wasn't found.
     */
    CPVREpgInfoTagPtr GetTagByBroadcastId(unsigned int iUniqueBroadcastId) const;

    /*!
     * @brief Update an entry in this EPG.
     * @param data The tag to update.
     * @param iClientId The id of the pvr client this event belongs to.
     * @param bUpdateDatabase If set to true, this event will be persisted in the database.
     * @return True if it was updated successfully, false otherwise.
     */
    bool UpdateEntry(const EPG_TAG *data, int iClientId, bool bUpdateDatabase);

    /*!
     * @brief Update an entry in this EPG.
     * @param tag The tag to update.
     * @param bUpdateDatabase If set to true, this event will be persisted in the database.
     * @return True if it was updated successfully, false otherwise.
     */
    bool UpdateEntry(const CPVREpgInfoTagPtr &tag, bool bUpdateDatabase);

    /*!
     * @brief Update an entry in this EPG.
     * @param tag The tag to update.
     * @param newState the new state of the event.
     * @param bUpdateDatabase If set to true, this event will be persisted in the database.
     * @return True if it was updated successfully, false otherwise.
     */
    bool UpdateEntry(const CPVREpgInfoTagPtr &tag, EPG_EVENT_STATE newState, bool bUpdateDatabase);

    /*!
     * @brief Update the EPG from 'start' till 'end'.
     * @param start The start time.
     * @param end The end time.
     * @param iUpdateTime Update the table after the given amount of time has passed.
     * @param bForceUpdate Force update from client even if it's not the time to
     * @return True if the update was successful, false otherwise.
     */
    bool Update(const time_t start, const time_t end, int iUpdateTime, bool bForceUpdate = false);

    /*!
     * @brief Get all EPG entries.
     * @param results The file list to store the results in.
     * @return The amount of entries that were added.
     */
    int Get(CFileItemList &results) const;

    /*!
     * @brief Get all EPG entries that and apply a filter.
     * @param results The file list to store the results in.
     * @param filter The filter to apply.
     * @return The amount of entries that were added.
     */
    int Get(CFileItemList &results, const CPVREpgSearchFilter &filter) const;

    /*!
     * @brief Persist this table in the database.
     * @return True if the table was persisted, false otherwise.
     */
    bool Persist(void);

    /*!
     * @brief Get the start time of the first entry in this table.
     * @return The first date in UTC.
     */
    CDateTime GetFirstDate(void) const;

    /*!
     * @brief Get the end time of the last entry in this table.
     * @return The last date in UTC.
     */
    CDateTime GetLastDate(void) const;

    /*!
     * @brief Get the time the EPG data were last updated.
     * @return The last time this table was scanned.
     */
    CDateTime GetLastScanTime(void);

    /*!
     * @brief Notify observers when the currently active tag changed.
     * @return True if the playing tag has changed, false otherwise.
     */
    bool CheckPlayingEvent(void);

    /*!
     * @brief Convert a genre id and subid to a human readable name.
     * @param iID The genre ID.
     * @param iSubID The genre sub ID.
     * @return A human readable name.
     */
    static const std::string& ConvertGenreIdToString(int iID, int iSubID);

    /*!
     * @brief Check whether this EPG has unsaved data.
     * @return True if this EPG contains unsaved data, false otherwise.
     */
    bool NeedsSave(void) const;

    /*!
     * @brief Check whether this EPG is valid.
     * @return True if this EPG is valid and can be updated, false otherwise.
     */
    bool IsValid(void) const;

  private:
    CPVREpg(void) = delete;
    CPVREpg(const CPVREpg&) = delete;
    CPVREpg& operator =(const CPVREpg&) = delete;

    /*!
     * @brief Update the EPG from a scraper set in the channel tag.
     * @todo not implemented yet for non-pvr EPGs
     * @param start Get entries with a start date after this time.
     * @param end Get entries with an end date before this time.
     * @param bForceUpdate Force update from client even if it's not the time to
     * @return True if the update was successful, false otherwise.
     */
    bool UpdateFromScraper(time_t start, time_t end, bool bForceUpdate);

    /*!
     * @brief Fix overlapping events from the tables.
     * @param bUpdateDb If set to yes, any changes to tags during fixing will be persisted to database
     * @return True if anything changed, false otherwise.
     */
    bool FixOverlappingEvents(bool bUpdateDb = false);

    /*!
     * @brief Add an infotag to this container.
     * @param tag The tag to add.
     */
    void AddEntry(const CPVREpgInfoTag &tag);

    /*!
     * @brief Load all EPG entries from clients into a temporary table and update this table with the contents of that temporary table.
     * @param start Only get entries after this start time. Use 0 to get all entries before "end".
     * @param end Only get entries before this end time. Use 0 to get all entries after "begin". If both "begin" and "end" are 0, all entries will be updated.
     * @param bForceUpdate Force update from client even if it's not the time to
     * @return True if the update was successful, false otherwise.
     */
    bool LoadFromClients(time_t start, time_t end, bool bForceUpdate);

    /*!
     * @brief Update the contents of this table with the contents provided in "epg"
     * @param epg The updated contents.
     * @param bStoreInDb True to store the updated contents in the db, false otherwise.
     * @return True if the update was successful, false otherwise.
     */
    bool UpdateEntries(const CPVREpg &epg, bool bStoreInDb = true);

    std::map<CDateTime, CPVREpgInfoTagPtr> m_tags;
    std::map<int, CPVREpgInfoTagPtr>       m_changedTags;
    std::map<int, CPVREpgInfoTagPtr>       m_deletedTags;
    bool                                m_bChanged = false;        /*!< true if anything changed that needs to be persisted, false otherwise */
    bool                                m_bTagsChanged = false;    /*!< true when any tags are changed and not persisted, false otherwise */
    bool                                m_bLoaded = false;         /*!< true when the initial entries have been loaded */
    bool                                m_bUpdatePending = false;  /*!< true if manual update is pending */
    int                                 m_iEpgID = 0;          /*!< the database ID of this table */
    std::string                         m_strName;         /*!< the name of this table */
    std::string                         m_strScraperName;  /*!< the name of the scraper to use */
    mutable CDateTime                   m_nowActiveStart;  /*!< the start time of the tag that is currently active */

    CDateTime                           m_lastScanTime;    /*!< the last time the EPG has been updated */

    CPVRChannelPtr                      m_pvrChannel;      /*!< the channel this EPG belongs to */

    mutable CCriticalSection            m_critSection;     /*!< critical section for changes in this table */
    bool                                m_bUpdateLastScanTime = false;
  };
}
