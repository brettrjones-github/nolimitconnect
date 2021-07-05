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
#include "config_kodi.h"
#if defined(HAVE_QT_GUI)
#if defined(TARGET_OS_WINDOWS)
# include <libglew/include/GL/glew.h>
#endif

#include "GUITextureQt.h"
#include "Texture.h"
#include "ServiceBroker.h"
#include "utils/log.h"
#include "utils/GLUtils.h"
#include "utils/MathUtils.h"
#include "rendering/qt/RenderSystemQt.h"
#include "windowing/GraphicContext.h"
#include "windowing/WinSystem.h"

#include <cstddef>

#include "GuiInterface/IGoTv.h"


CGUITextureQt::CGUITextureQt( float posX, float posY, float width, float height, const CTextureInfo &texture )
    : CGUITextureBase( posX, posY, width, height, texture )
    , m_IGoTv( IGoTv::getIGoTv() )
{
    m_renderSystem = dynamic_cast< CRenderSystemQt* >( CServiceBroker::GetRenderSystem() );
}

void CGUITextureQt::Begin( UTILS::Color color )
{
    m_IGoTv.beginGuiTexture( this, (GoTvColor) color );
}

void CGUITextureQt::End()
{
    m_IGoTv.endGuiTexture( this );

    m_renderSystem->DisableGUIShader();
}

void CGUITextureQt::Draw( float *x, float *y, float *z, const CRect &texture, const CRect &diffuse, int orientation )
{
    m_IGoTv.drawGuiTexture( this, x, y, z, (const GoTvRect &)texture, (const GoTvRect &)diffuse, orientation );
}

void CGUITextureQt::DrawQuad( const CRect &rect, UTILS::Color color, CBaseTexture *texture, const CRect *texCoords )
{
    IGoTv::getIGoTv().drawQuad( ( const GoTvRect & )rect, ( GoTvColor )color, texture, ( const GoTvRect * )texCoords );

    CRenderSystemQt *renderSystem = dynamic_cast< CRenderSystemQt* >( CServiceBroker::GetRenderSystem() );
    renderSystem->DisableGUIShader();
}

#endif // defined(HAS_GLES)
