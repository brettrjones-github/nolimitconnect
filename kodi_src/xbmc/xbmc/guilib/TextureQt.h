/*
 *      Copyright (C) 2005-2013 Team XBMC
 *      http://kodi.tv
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with XBMC; see the file COPYING.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 */

#pragma once

#include "Texture.h"

class INlc;

/************************************************************************/
/*    CQtTexture                                                       */
/************************************************************************/
class CQtTexture : public CBaseTexture
{
public:
    CQtTexture( unsigned int width = 0, unsigned int height = 0, unsigned int format = XB_FMT_A8R8G8B8 );
    ~CQtTexture() override;

    void                        CreateTextureObject() override;
    void                        DestroyTextureObject() override;
    void                        LoadToGPU() override;
    void                        BindToUnit( unsigned int unit ) override;

    int                         m_texture = 0;
    bool                        m_isOglVersion3orNewer = false;

protected:
    INlc&                      m_INlc;
};

