/*
 *  Copyright (C) 2005-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "SpecialProtocolFile.h"
#include <GoTvCore/xbmc/xbmc/GoTvUrl.h>
#include "filesystem/SpecialProtocol.h"

using namespace XFILE;

CSpecialProtocolFile::CSpecialProtocolFile(void)
  : COverrideFile(true)
{ }

CSpecialProtocolFile::~CSpecialProtocolFile(void) = default;

std::string CSpecialProtocolFile::TranslatePath(const GoTvUrl& url)
{
  return CSpecialProtocol::TranslatePath(url);
}
