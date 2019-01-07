/*
 *  Copyright (C) 2016-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include "platform/Platform.h"

class CPlatformDarwin : public CPlatform
{
  public:
    /**\brief C'tor */
    CPlatformDarwin();

    /**\brief D'tor */
    virtual ~CPlatformDarwin();

    void Init() override;
};
