/*
 *  Copyright (C) 2014-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include <string>

#include "addons/Resource.h"

class NlcUrl;

namespace ADDON
{

//! \brief A collection of images. The collection can have a type.
class CImageResource : public CResource
{
public:
  static std::unique_ptr<CImageResource> FromExtension(CAddonInfo addonInfo, const cp_extension_t* ext);

  explicit CImageResource(CAddonInfo addonInfo) : CResource(std::move(addonInfo)) {};
  CImageResource(CAddonInfo addonInfo, std::string type);

  void OnPreUnInstall() override;

  bool IsAllowed(const std::string &file) const override;
  std::string GetFullPath(const std::string &filePath) const override;

  //! \brief Returns type of image collection
  const std::string& GetType() const { return m_type; }

private:
  bool HasXbt(NlcUrl& xbtUrl) const;

  std::string m_type; //!< Type of images
};

} /* namespace ADDON */
