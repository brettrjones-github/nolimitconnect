#pragma once

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

#include "GUITexture.h"

#include <vector>
#include "utils/Color.h"

#if !defined(HAS_GLES) || defined(HAVE_QT_GUI) // gles defines PackedVertex
struct PackedVertex
{
    float x, y, z;
    float u1, v1;
    float u2, v2;
};
#endif // !defined(HAS_GLES)

typedef std::vector<PackedVertex> PackedVertices;

class CRenderSystemQt;
class IGoTv;

class CGUITextureQt : public CGUITextureBase
{
public:
    CGUITextureQt( float posX, float posY, float width, float height, const CTextureInfo& texture );
    static void                 DrawQuad( const CRect &coords, UTILS::Color color, CBaseTexture *texture = NULL, const CRect *texCoords = NULL );


    uint8_t                     m_col[ 4 ];
    PackedVertices              m_packedVertices;
    std::vector<uint16_t>       m_idx;
    CRenderSystemQt *           m_renderSystem;

protected:
    void                        Begin( UTILS::Color color );
    void                        Draw( float *x, float *y, float *z, const CRect &texture, const CRect &diffuse, int orientation );
    void                        End();

    IGoTv&                      m_IGoTv;
};

