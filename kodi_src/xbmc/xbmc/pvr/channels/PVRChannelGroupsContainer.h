/*
 *  Copyright (C) 2012-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include "threads/CriticalSection.h"

#include "pvr/channels/PVRChannelGroups.h"

class NlcUrl;

namespace PVR
{
  class CPVRChannelGroupsContainer
  {
  public:
    /*!
     * @brief Create a new container for all channel groups
     */
    CPVRChannelGroupsContainer(void);

    /*!
     * @brief Destroy this container.
     */
    virtual ~CPVRChannelGroupsContainer(void);

    /*!
     * @brief Load all channel groups and all channels in those channel groups.
     * @return True if all groups were loaded, false otherwise.
     */
    bool Load(void);

    /*!
     * @brief Checks whether groups were already loaded.
     * @return True if groups were successfully loaded, false otherwise.
     */
    bool Loaded(void) const;

    /*!
     * @brief Unload and destruct all channel groups and all channels in them.
     */
    void Unload(void);

    /*!
     * @brief Update the contents of all the groups in this container.
     * @param bChannelsOnly Set to true to only update channels, not the groups themselves.
     * @return True if the update was successful, false otherwise.
     */
    bool Update(bool bChannelsOnly = false);

    /*!
     * @brief Get the TV channel groups.
     * @return The TV channel groups.
     */
    CPVRChannelGroups *GetTV(void) const { return Get(false); }

    /*!
     * @brief Get the radio channel groups.
     * @return The radio channel groups.
     */
    CPVRChannelGroups *GetRadio(void) const { return Get(true); }

    /*!
     * @brief Get the radio or TV channel groups.
     * @param bRadio If true, get the radio channel groups. Get the TV channel groups otherwise.
     * @return The requested groups.
     */
    CPVRChannelGroups *Get(bool bRadio) const;

    /*!
     * @brief Get the group containing all TV channels.
     * @return The group containing all TV channels.
     */
    CPVRChannelGroupPtr GetGroupAllTV(void)  const{ return GetGroupAll(false); }

    /*!
     * @brief Get the group containing all radio channels.
     * @return The group containing all radio channels.
     */
    CPVRChannelGroupPtr GetGroupAllRadio(void)  const{ return GetGroupAll(true); }

    /*!
     * @brief Get the group containing all TV or radio channels.
     * @param bRadio If true, get the group containing all radio channels. Get the group containing all TV channels otherwise.
     * @return The requested group.
     */
    CPVRChannelGroupPtr GetGroupAll(bool bRadio) const;

    /*!
     * @brief Get a group given it's ID.
     * @param iGroupId The ID of the group.
     * @return The requested group or NULL if it wasn't found.
     */
    CPVRChannelGroupPtr GetByIdFromAll(int iGroupId) const;

    /*!
     * @brief Get a channel given it's database ID.
     * @param iChannelId The ID of the channel.
     * @return The channel or NULL if it wasn't found.
     */
    CPVRChannelPtr GetChannelById(int iChannelId) const;

    /*!
     * @brief Get a channel given it's EPG ID.
     * @param iEpgId The EPG ID of the channel.
     * @return The channel or NULL if it wasn't found.
     */
    CPVRChannelPtr GetChannelByEpgId(int iEpgId) const;

    /*!
     * @brief Get the groups list for a directory.
     * @param strBase The directory path.
     * @param results The file list to store the results in.
     * @param bRadio Get radio channels or tv channels.
     * @return True if the list was filled successfully.
     */
    bool GetGroupsDirectory(CFileItemList *results, bool bRadio) const;

    /*!
     * @brief Get a channel given it's path.
     * @param strPath The path.
     * @return The channel or NULL if it wasn't found.
     */
    CFileItemPtr GetByPath(const std::string &strPath) const;

    /*!
     * @brief Get the directory for a path.
     * @param strPath The path.
     * @param results The file list to store the results in.
     * @return True if the directory was found, false if not.
     */
    bool GetDirectory(const std::string& strPath, CFileItemList &results) const;

    /*!
     * @brief Get the group that is currently selected in the UI.
     * @param bRadio True to get the selected radio group, false to get the selected TV group.
     * @return The selected group.
     */
    CPVRChannelGroupPtr GetSelectedGroup(bool bRadio) const;

    /*!
     * @brief Get a channel given it's channel ID from all containers.
     * @param iUniqueChannelId The unique channel id on the client.
     * @param iClientID The ID of the client.
     * @return The channel or NULL if it wasn't found.
     */
    CPVRChannelPtr GetByUniqueID(int iUniqueChannelId, int iClientID) const;

    /*!
     * @brief Try to find missing channel icons automatically
     */
    void SearchMissingChannelIcons(void) const;

    /*!
     * @brief The channel that was played last that has a valid client or NULL if there was none.
     * @return The requested channel.
     */
    CFileItemPtr GetLastPlayedChannel(void) const;

    /*!
     * @brief The group that was played last and optionally contains the given channel.
     * @param iChannelID The channel ID
     * @return The last watched group.
     */
    CPVRChannelGroupPtr GetLastPlayedGroup(int iChannelID = -1) const;

    /*!
     * @brief Create EPG tags for channels in all internal channel groups.
     * @return True if EPG tags were created successfully.
     */
    bool CreateChannelEpgs(void);

    /*!
     * @brief Return the group which was previous played.
     * @return The group which was previous played.
     */
    CPVRChannelGroupPtr GetPreviousPlayedGroup(void);

    /*!
     * @brief Set the last played group.
     * @param The last played group
     */
    void SetLastPlayedGroup(const CPVRChannelGroupPtr &group);

  protected:
    CPVRChannelGroups *m_groupsRadio; /*!< all radio channel groups */
    CPVRChannelGroups *m_groupsTV;    /*!< all TV channel groups */
    CCriticalSection   m_critSection;
    bool               m_bUpdateChannelsOnly = false;
    bool               m_bIsUpdating = false;
    CPVRChannelGroupPtr m_lastPlayedGroups[2]; /*!< used to store the last played groups */

  private :
    CPVRChannelGroupsContainer& operator=(const CPVRChannelGroupsContainer&) = delete;
    CPVRChannelGroupsContainer(const CPVRChannelGroupsContainer&) = delete;

    bool FilterDirectory(const NlcUrl &url, CFileItemList &results) const;

    bool m_bLoaded = false;
  };
}
