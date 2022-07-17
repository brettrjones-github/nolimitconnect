/*
 *      Copyright (c) 2007 d4rk
 *      Copyright (C) 2007-2013 Team XBMC
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

#if defined(TARGET_OS_WINDOWS)
# include <libglew/include/GL/glew.h>
#endif // defined(TARGET_OS_WINDOWS)

#include "../RenderFlags.h"
#include "YUV2RGBShaderQt.h"
#include "YUVMatrix.h"
#include "settings/AdvancedSettings.h"
#include "utils/TransformMatrix.h"
#include "utils/log.h"
#include "utils/GLUtils.h"

#include "GuiInterface/INlc.h"

#include <string>
#include <sstream>

using namespace Shaders;

//============================================================================
static void CalculateYUVMatrixGLES( GLfloat      res[ 4 ][ 4 ]
                                    , unsigned int flags
                                    , EShaderFormat format
                                    , float        black
                                    , float         contrast
                                    , bool          limited )
{
    TransformMatrix matrix;
    CalculateYUVMatrix( matrix, flags, format, black, contrast, limited );

    for( int row = 0; row < 3; row++ )
        for( int col = 0; col < 4; col++ )
            res[ col ][ row ] = matrix.m[ row ][ col ];

    res[ 0 ][ 3 ] = 0.0f;
    res[ 1 ][ 3 ] = 0.0f;
    res[ 2 ][ 3 ] = 0.0f;
    res[ 3 ][ 3 ] = 1.0f;
}

//////////////////////////////////////////////////////////////////////
// BaseYUV2RGBShaderQt - base class for GLSL YUV2RGB shaders
//////////////////////////////////////////////////////////////////////
//============================================================================
void BaseYUV2RGBShader::SetField( int field )
{
    m_INlc.shaderSetField( getShaderMethod(), field );
}

//============================================================================
void BaseYUV2RGBShader::SetWidth( int width )
{
    m_INlc.shaderSetWidth( getShaderMethod(), width );
}

//============================================================================
void BaseYUV2RGBShader::SetHeight( int width )
{
    m_INlc.shaderSetHeight( getShaderMethod(), width );
}

//============================================================================
void BaseYUV2RGBShader::SetBlack( float black )
{
    m_INlc.shaderSetBlack( getShaderMethod(), black );
}

//============================================================================
void BaseYUV2RGBShader::SetContrast( float contrast )
{
    m_INlc.shaderSetContrast( getShaderMethod(), contrast );
}

//============================================================================
int BaseYUV2RGBShader::GetVertexLoc( )
{
    return m_INlc.shaderGetVertexLoc( getShaderMethod() );
}

//============================================================================
int BaseYUV2RGBShader::GetYcoordLoc()
{
    return m_INlc.shaderGetYcoordLoc( getShaderMethod() );
}

//============================================================================
int BaseYUV2RGBShader::GetUcoordLoc()
{
    return m_INlc.shaderGetUcoordLoc( getShaderMethod() );
}

//============================================================================
int BaseYUV2RGBShader::GetVcoordLoc()
{
    return m_INlc.shaderGetVcoordLoc( getShaderMethod() );
}

//============================================================================
void BaseYUV2RGBShader::SetMatrices( const GLfloat *p, const GLfloat *m ) 
{
    m_INlc.shaderSetMatrices( getShaderMethod(), p, m );
}

//============================================================================
void BaseYUV2RGBShader::SetAlpha( GLfloat alpha )
{
    m_INlc.shaderSetAlpha( getShaderMethod(), alpha );
}

//============================================================================
void BaseYUV2RGBShader::SetConvertFullColorRange( bool convertFullRange )
{
    m_INlc.shaderSetConvertFullColorRange( getShaderMethod(), convertFullRange );
}

//============================================================================
//============================================================================
BaseYUV2RGBShaderQt::BaseYUV2RGBShaderQt(  INlc& gotv, unsigned flags, EShaderFormat format )
: BaseYUV2RGBShader( gotv )
{
    m_ShaderMethod = SM_SHADER_NONE;
    m_flags = flags;
    m_format = format;
    m_Valid = true;

    if( m_format == SHADER_YV12 ||
        m_format == SHADER_YV12_9 ||
        m_format == SHADER_YV12_10 ||
        m_format == SHADER_YV12_12 ||
        m_format == SHADER_YV12_14 ||
        m_format == SHADER_YV12_16 )
        m_ShaderMethod = SM_VIDEO_YV12_BASIC;
    else if( m_format == SHADER_NV12 )
        m_ShaderMethod = SM_VIDEO_NV12_BASIC;
#ifdef ENABLE_GLES_SHADERS
    else if( m_format == SHADER_YUY2 )
        m_ShaderMethod = SM_VIDEO_YUY2_BASIC;
#endif // ENABLE_GLES_SHADERS

    else if( m_format == SHADER_UYVY )
        m_ShaderMethod = SM_VIDEO_UYVY_BASIC;
    else if( m_format == SHADER_NV12_RRG )
        m_ShaderMethod = SM_VIDEO_NV12_RGB_BASIC;
    else if( m_format == SHADER_YV12 )
        m_ShaderMethod = SM_VIDEO_YV12_BASIC;
    else
    {
         CLog::Log( LOGERROR, "GLES: BaseYUV2RGBShaderQt - unsupported format %d", m_format );
         m_Valid = false;
    }

     m_INlc.shaderSetFormat( getShaderMethod(), format );
}

//============================================================================
BaseYUV2RGBShaderQt::~BaseYUV2RGBShaderQt()
{
    Free();
}

//============================================================================
void BaseYUV2RGBShaderQt::OnCompiledAndLinked()
{
}

//============================================================================
bool BaseYUV2RGBShaderQt::OnEnabled()
{
     return true;
}

//============================================================================
void BaseYUV2RGBShaderQt::OnDisabled()
{
}

//============================================================================
void BaseYUV2RGBShaderQt::Free()
{
}

//////////////////////////////////////////////////////////////////////
// YUV2RGBProgressiveShader - YUV2RGB with no deinterlacing
// Use for weave deinterlacing / progressive
//////////////////////////////////////////////////////////////////////
//============================================================================
YUV2RGBProgressiveShaderQt::YUV2RGBProgressiveShaderQt( INlc& gotv, unsigned flags, EShaderFormat format )
    : BaseYUV2RGBShaderQt( gotv, flags, format )
{
    //PixelShader()->LoadSource( "gles_yuv2rgb_basic.frag", m_defines );
    if( m_Valid )
    {
        m_INlc.shaderSetFlags( getShaderMethod(), flags );
    }
}

//////////////////////////////////////////////////////////////////////
// YUV2RGBBobShader - YUV2RGB with Bob deinterlacing
//////////////////////////////////////////////////////////////////////
//============================================================================
YUV2RGBBobShaderQt::YUV2RGBBobShaderQt( INlc& gotv, unsigned flags, EShaderFormat format )
    : BaseYUV2RGBShaderQt( gotv, flags, format )
{
    //m_hStepX = -1;
    //m_hStepY = -1;
    //m_hField = -1;

    //PixelShader()->LoadSource( "gles_yuv2rgb_bob.frag", m_defines );
    switch( m_ShaderMethod )
    {
#ifdef ENABLE_GLES_SHADERS
    case SM_VIDEO_YV12_BASIC:
        m_ShaderMethod = SM_VIDEO_YV12_BOB;
        break;
    case SM_VIDEO_NV12_BASIC:
        m_ShaderMethod = SM_VIDEO_NV12_BOB;
        break;
    case SM_VIDEO_YUY2_BASIC:
        m_ShaderMethod = SM_VIDEO_YUY2_BOB;
        break;
    case SM_VIDEO_UYVY_BASIC:
        m_ShaderMethod = SM_VIDEO_UYVY_BOB;
        break;
    case SM_VIDEO_NV12_RGB_BASIC:
        m_ShaderMethod = SM_VIDEO_NV12_RGB_BOB;
        break;
#endif // ENABLE_GLES_SHADERS

    default:
        m_Valid = false;
        break;
    }

    if( m_Valid )
    {
        m_INlc.shaderSetFlags( getShaderMethod(), flags );
        m_INlc.shaderSetFormat( getShaderMethod(), m_format );
    }
}

//============================================================================
void YUV2RGBBobShaderQt::OnCompiledAndLinked()
{
}

//============================================================================
bool YUV2RGBBobShaderQt::OnEnabled()
{
    if( !BaseYUV2RGBShaderQt::OnEnabled() )
        return false;
    return true;
}
