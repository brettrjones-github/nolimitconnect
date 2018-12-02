/*
 *  Copyright (C) 2005-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include <string>
#include <vector>

#include "addons/Addon.h"
#include "dbwrappers/Database.h"
#include "FileItem.h"
#include "AddonBuilder.h"

class CAddonDatabase : public CDatabase
{
public:
  CAddonDatabase();
  ~CAddonDatabase() override;
  bool Open() override;

  /*! @deprecated: use CAddonMgr::FindInstallableById */
  bool GetAddon(const std::string& addonID, ADDON::AddonPtr& addon);

  /*! \brief Get an addon with a specific version and repository. */
  bool GetAddon(const std::string& addonID, const ADDON::AddonVersion& version, const std::string& repoId, ADDON::AddonPtr& addon);

  /*! Get the addon IDs that has been set to disabled */
  bool GetDisabled(std::set<std::string>& addons);

  /*! @deprecated: use FindByAddonId */
  bool GetAvailableVersions(const std::string& addonId,
      std::vector<std::pair<ADDON::AddonVersion, std::string>>& versionsInfo);

  /*! @deprecated use CAddonMgr::FindInstallableById */
  std::pair<ADDON::AddonVersion, std::string> GetAddonVersion(const std::string &id);

  /*! Returns all addons in the repositories with id `addonId`. */
  bool FindByAddonId(const std::string& addonId, ADDON::VECADDONS& addons);

  bool UpdateRepositoryContent(const std::string& repositoryId, const ADDON::AddonVersion& version,
      const std::string& checksum, const std::vector<ADDON::AddonPtr>& addons);

  int GetRepoChecksum(const std::string& id, std::string& checksum);

  /*!
   \brief Get addons in repository `id`
   \param id id of the repository
   \returns true on success, false on error or if repository have never been synced.
   */
  bool GetRepositoryContent(const std::string& id, ADDON::VECADDONS& addons);

  /*! Get addons across all repositories */
  bool GetRepositoryContent(ADDON::VECADDONS& addons);

  /*!
   \brief Set repo last checked date, and create the repo if needed
   \param id id of the repository
   \returns id of the repository, or -1 on error.
   */
  int SetLastChecked(const std::string& id, const ADDON::AddonVersion& version, const std::string& timestamp);

  /*!
   \brief Retrieve the time a repository was last checked and the version it was for
   \param id id of the repo
   \return last time the repo was checked, or invalid CDateTime if never checked
   */
  std::pair<CDateTime, ADDON::AddonVersion> LastChecked(const std::string& id);

  bool Search(const std::string& search, ADDON::VECADDONS& items);

  /*! \brief Disable an addon.
   Sets a flag that this addon has been disabled.  If disabled, it is usually still available on disk.
   \param addonID id of the addon to disable
   \param disable whether to enable or disable.  Defaults to true (disable)
   \return true on success, false on failure
   \sa IsAddonDisabled, HasDisabledAddons */
  bool DisableAddon(const std::string &addonID, bool disable = true);

  /*! \brief Mark an addon as broken
   Sets a flag that this addon has been marked as broken in the repository.
   \param addonID id of the addon to mark as broken
   \param reason why it is broken - if non empty we take it as broken.  Defaults to empty
   \return true on success, false on failure
   \sa IsAddonBroken */
  bool BreakAddon(const std::string &addonID, const std::string& reason="");

  /*! \brief Check whether an addon has been marked as broken via BreakAddon.
   \param addonID id of the addon to check
   \sa BreakAddon */
  bool IsAddonBroken(const std::string &addonID);

  bool BlacklistAddon(const std::string& addonID);
  bool RemoveAddonFromBlacklist(const std::string& addonID);
  bool GetBlacklisted(std::set<std::string>& addons);

  /*! \brief Store an addon's package filename and that file's hash for future verification
      \param  addonID         id of the addon we're adding a package for
      \param  packageFileName filename of the package
      \param  hash            MD5 checksum of the package
      \return Whether or not the info successfully made it into the DB.
      \sa GetPackageHash, RemovePackage
  */
  bool AddPackage(const std::string& addonID,
                  const std::string& packageFileName,
                  const std::string& hash);
  /*! \brief Query the MD5 checksum of the given given addon's given package
      \param  addonID         id of the addon we're who's package we're querying
      \param  packageFileName filename of the package
      \param  hash            return the MD5 checksum of the package
      \return Whether or not we found a hash for the given addon's given package
      \sa AddPackage, RemovePackage
  */
  bool GetPackageHash(const std::string& addonID,
                      const std::string& packageFileName,
                      std::string&       hash);
  /*! \brief Remove a package's info from the database
      \param  packageFileName filename of the package
      \return Whether or not we succeeded in removing the package
      \sa AddPackage, GetPackageHash
  */
  bool RemovePackage(const std::string& packageFileName);

  /*! Clear internal fields that shouldn't be kept around indefinitely */
  void OnPostUnInstall(const std::string& addonId);

  void SyncInstalled(const std::set<std::string>& ids,
                     const std::set<std::string>& system,
                     const std::set<std::string>& optional);

  void GetInstalled(std::vector<ADDON::CAddonBuilder>& addons);

  bool SetLastUpdated(const std::string& addonId, const CDateTime& dateTime);
  bool SetOrigin(const std::string& addonId, const std::string& origin);
  bool SetLastUsed(const std::string& addonId, const CDateTime& dateTime);


protected:
  void CreateTables() override;
  void CreateAnalytics() override;
  void UpdateTables(int version) override;
  int GetMinSchemaVersion() const override;
  int GetSchemaVersion() const override;
  const char *GetBaseDBName() const override { return "Addons"; }

  bool GetAddon(int id, ADDON::AddonPtr& addon);
  void DeleteRepository(const std::string& id);
};

