/*
 *  Copyright (C) 2005-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include <map>
#include <string>
#include <stdint.h>
#include "threads/CriticalSection.h"

class GoTvUrl;

/*!
 \ingroup filesystem
 \brief Password Manager class for saving authentication details
 
 Handles access to previously saved passwords for paths, translating normal URLs
 into authenticated URLs if the user has details about the username and password
 for a path previously saved. Should be accessed via CPasswordManager::GetInstance()
 */
class CPasswordManager
{
public:
 /*!
   \brief The only way through which the global instance of the CPasswordManager should be accessed.
   \return the global instance.
   */
  static CPasswordManager &GetInstance();

  /*!
   \brief Authenticate a URL by looking the URL up in the temporary and permanent caches
   First looks up based on host and share name.  If that fails, it will try a match purely
   on the host name (eg different shares on the same host with the same credentials)
   \param url a GoTvUrl to authenticate
   \return true if we have details in the cache, false otherwise.
   \sa GoTvUrl
   */
  bool AuthenticateURL(GoTvUrl &url);

  /*!
   \brief Prompt for a username and password for the particular URL.
   
   This routine pops up a dialog, requesting the user enter a username and password
   to access the given URL.  The user may optionally save these details.  If saved
   we write the details into the users profile.  If not saved, the details are temporarily
   stored so that further access no longer requires prompting for authentication.
   
   \param url the URL to authenticate.
   \return true if the user entered details, false if the user cancelled the dialog.
   \sa GoTvUrl, SaveAuthenticatedURL
   */
  bool PromptToAuthenticateURL(GoTvUrl &url);

  /*!
   \brief Save an authenticated URL.

   This routine stores an authenticated URL in the temporary cache, and optionally
   saves these details into the users profile.

   \param url the URL to authenticate.
   \param saveToProfile whether to save in the users profile, defaults to true.
   \sa GoTvUrl, PromptToAuthenticateURL
   */
  void SaveAuthenticatedURL(const GoTvUrl &url, bool saveToProfile = true);

  /*!
   \brief Is an URL is supported (by the manager)

   This routine checks that an URL is supported by the manager

   \param url the URL to check.
   \return true if the URL is supported
   \sa CURL, IsURLSupported
   */
  bool IsURLSupported(const GoTvUrl &url);

  /*!
   \brief Clear any previously cached passwords
   */
  void Clear();

private:
  // private construction, and no assignments; use the provided singleton methods
  CPasswordManager();
  CPasswordManager(const CPasswordManager&) = delete;
  CPasswordManager& operator=(CPasswordManager const&) = delete;
  ~CPasswordManager() = default;

  void Load();
  void Save() const;
  std::string GetLookupPath(const GoTvUrl &url) const;
  std::string GetServerLookup(const std::string &path) const;

  std::map<std::string, std::string>  m_temporaryCache;
  std::map<std::string, std::string>  m_permanentCache;
  bool m_loaded;

  CCriticalSection m_critSection;
};
