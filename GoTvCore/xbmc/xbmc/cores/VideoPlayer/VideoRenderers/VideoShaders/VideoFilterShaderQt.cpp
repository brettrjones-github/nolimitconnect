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

#include <string>
#include <math.h>

#include "VideoFilterShaderQt.h"
#include "ServiceBroker.h"
#include "utils/log.h"
#include "utils/GLUtils.h"
#include "ConvolutionKernels.h"
#include "rendering/qt/RenderSystemQt.h"
#include "GuiInterface/IGoTv.h"

using namespace Shaders;

//////////////////////////////////////////////////////////////////////
// BaseVideoFilterShaderQt - base class for video filter shaders
//////////////////////////////////////////////////////////////////////

BaseVideoFilterShaderQt::BaseVideoFilterShaderQt( IGoTv& gotv )
: m_IGoTv( gotv )
{
    m_ShaderMethod = SM_VID_FILTER_DEFAULT;
#ifdef GL_RGBA16F_EXT
    if( CServiceBroker::GetRenderSystem()->IsExtSupported( "GL_EXT_color_buffer_float" ) )
    {
        m_floattex = true;
    }
    else
    {
        m_floattex = false;
    }
#else
    m_floattex = false;
#endif // GL_RGBA16F_EXT


    /*
    m_width = 1;
    m_height = 1;
    m_hStepXY = 0;
    m_stepX = 0;
    m_stepY = 0;
    m_sourceTexUnit = 0;
    m_hSourceTex = 0;

    m_hVertex = -1;
    m_hcoord = -1;
    m_hProj = -1;
    m_hModel = -1;
    m_hAlpha = -1;

    m_proj = nullptr;
    m_model = nullptr;
    m_alpha = -1;
    */
    //VertexShader()->LoadSource( "gles_videofilter.vert" );

    //PixelShader()->LoadSource( "gles_videofilter.frag" );
}

//============================================================================
void BaseVideoFilterShaderQt::SetSourceTexture( GLint ytex ) //         { m_sourceTexUnit = ytex; }
{
    m_IGoTv.shaderSourceTexture( getShaderMethod(), ytex );
}

//============================================================================
void BaseVideoFilterShaderQt::SetWidth( int w )//                      { m_width = w; m_stepX = w > 0 ? 1.0f / w : 0; }
{
    m_IGoTv.shaderSetWidth( getShaderMethod(), w );
    float stepX = w > 0 ? 1.0f / w : 0;
    m_IGoTv.shaderSetStepX( getShaderMethod(), stepX );
}

//============================================================================
void BaseVideoFilterShaderQt::SetHeight( int h )//                     { m_height = h; m_stepY = h > 0 ? 1.0f / h : 0; }
{
    m_IGoTv.shaderSetHeight( getShaderMethod(), h );
    float stepY = h > 0 ? 1.0f / h : 0;
    m_IGoTv.shaderSetStepY( getShaderMethod(), stepY );
}

//============================================================================
GLint BaseVideoFilterShaderQt::GetVertexLoc()//                        { return m_hVertex; }
{
    return m_IGoTv.shaderGetVertexLoc( getShaderMethod() );
}

//============================================================================
GLint BaseVideoFilterShaderQt::GetcoordLoc()//                         { return m_hcoord; }
{
    return m_IGoTv.shaderGetcoordLoc( getShaderMethod() );
}

//============================================================================
void BaseVideoFilterShaderQt::SetMatrices( const GLfloat *p, const GLfloat *m )// { m_proj = p; m_model = m; }
{
    m_IGoTv.shaderSetMatrices( getShaderMethod(), p, m );
}

//============================================================================
void BaseVideoFilterShaderQt::SetAlpha( GLfloat alpha )//              { m_alpha = alpha; }
{
    m_IGoTv.shaderSetAlpha( getShaderMethod(), alpha );
}

void BaseVideoFilterShaderQt::OnCompiledAndLinked()
{
    //m_hVertex = glGetAttribLocation( ProgramHandle(), "m_attrpos" );
    //m_hcoord = glGetAttribLocation( ProgramHandle(), "m_attrcord" );
    //m_hAlpha = glGetUniformLocation( ProgramHandle(), "m_alpha" );
    //m_hProj = glGetUniformLocation( ProgramHandle(), "m_proj" );
    //m_hModel = glGetUniformLocation( ProgramHandle(), "m_model" );
}

bool BaseVideoFilterShaderQt::OnEnabled()
{
    //glUniformMatrix4fv( m_hProj, 1, GL_FALSE, m_proj );
    //glUniformMatrix4fv( m_hModel, 1, GL_FALSE, m_model );
    //glUniform1f( m_hAlpha, m_alpha );
    return true;
}

ConvolutionFilterShaderQt::ConvolutionFilterShaderQt( IGoTv& gotv, ESCALINGMETHOD method )
: BaseVideoFilterShaderQt( gotv )
{
    m_method = method;
    //m_kernelTex1 = 0;
    //m_hKernTex = -1;

    //std::string shadername;
    //std::string defines;

    if( m_method == VS_SCALINGMETHOD_CUBIC ||
        m_method == VS_SCALINGMETHOD_LANCZOS2 ||
        m_method == VS_SCALINGMETHOD_SPLINE36_FAST ||
        m_method == VS_SCALINGMETHOD_LANCZOS3_FAST )
    {
        //shadername = "gles_convolution-4x4.frag";
        m_ShaderMethod = m_floattex ? SM_VID_FILTER_CONVOLUTION_4X4_FLOAT : SM_VID_FILTER_CONVOLUTION_4X4_RGBA;

    }
    else if( m_method == VS_SCALINGMETHOD_SPLINE36 ||
             m_method == VS_SCALINGMETHOD_LANCZOS3 )
    {
        //shadername = "gles_convolution-6x6.frag";
        m_ShaderMethod = m_floattex ? SM_VID_FILTER_CONVOLUTION_6X6_FLOAT : SM_VID_FILTER_CONVOLUTION_6X6_RGBA;
    }

    //if( m_floattex )
    //{
    //    m_internalformat = GL_RGBA16F_EXT;
    //    defines = "#define HAS_FLOAT_TEXTURE 1\n";
    //}
    //else
    //{
    //    m_internalformat = GL_RGBA;
    //    defines = "#define HAS_FLOAT_TEXTURE 0\n";
    //}

    //CLog::Log( LOGDEBUG, "GL: ConvolutionFilterShaderQt: using %s defines:\n%s", shadername.c_str(), defines.c_str() );
    //PixelShader()->LoadSource( shadername, defines );
}

ConvolutionFilterShaderQt::~ConvolutionFilterShaderQt()
{
    Free();
}

void ConvolutionFilterShaderQt::OnCompiledAndLinked()
{
//    BaseVideoFilterShaderQt::OnCompiledAndLinked();
//
//    // obtain shader attribute handles on successful compilation
//    m_hSourceTex = glGetUniformLocation( ProgramHandle(), "img" );
//    m_hStepXY = glGetUniformLocation( ProgramHandle(), "stepxy" );
//    m_hKernTex = glGetUniformLocation( ProgramHandle(), "kernelTex" );
//
//    CConvolutionKernel kernel( m_method, 256 );
//
//    if( m_kernelTex1 )
//    {
//        glDeleteTextures( 1, &m_kernelTex1 );
//        m_kernelTex1 = 0;
//    }
//
//    glGenTextures( 1, &m_kernelTex1 );
//
//    if( ( m_kernelTex1 <= 0 ) )
//    {
//        CLog::Log( LOGERROR, "GL: ConvolutionFilterShaderQt: Error creating kernel texture" );
//        return;
//    }
//
//    //make a kernel texture on GL_TEXTURE2 and set clamping and interpolation
//    glActiveTexture( GL_TEXTURE2 );
//    glBindTexture( GL_TEXTURE_2D, m_kernelTex1 );
//    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
//    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
//    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
//    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
//
//    //if float textures are supported, we can load the kernel as a float texture
//    //if not we load it as 8 bit unsigned which gets converted back to float in the shader
//    GLenum  format;
//    GLvoid* data;
//    if( m_floattex )
//    {
//        format = GL_FLOAT;
//        data = ( GLvoid* )kernel.GetFloatPixels();
//    }
//    else
//    {
//        format = GL_UNSIGNED_BYTE;
//        data = ( GLvoid* )kernel.GetUint8Pixels();
//    }
//
//    //upload as 2D texture with height of 1
//    glTexImage2D( GL_TEXTURE_2D, 0, m_internalformat, kernel.GetSize(), 1, 0, GL_RGBA, format, data );
//
//    glActiveTexture( GL_TEXTURE0 );
//
//    VerifyGLState();
}

bool ConvolutionFilterShaderQt::OnEnabled()
{
//    BaseVideoFilterShaderQt::OnEnabled();
//
//    // set shader attributes once enabled
//    glActiveTexture( GL_TEXTURE2 );
//    glBindTexture( GL_TEXTURE_2D, m_kernelTex1 );
//
//    glActiveTexture( GL_TEXTURE0 );
//    glUniform1i( m_hSourceTex, m_sourceTexUnit );
//    glUniform1i( m_hKernTex, 2 );
//    glUniform2f( m_hStepXY, m_stepX, m_stepY );
//    VerifyGLState();

    return true;
}

void ConvolutionFilterShaderQt::OnDisabled()
{
}

void ConvolutionFilterShaderQt::Free()
{
    //if( m_kernelTex1 )
    //    glDeleteTextures( 1, &m_kernelTex1 );
    //m_kernelTex1 = 0;
    BaseVideoFilterShaderQt::Free();
}

void DefaultFilterShaderQt::OnCompiledAndLinked()
{
    BaseVideoFilterShaderQt::OnCompiledAndLinked();

    //m_hSourceTex = glGetUniformLocation( ProgramHandle(), "img" );
}

bool DefaultFilterShaderQt::OnEnabled()
{
    BaseVideoFilterShaderQt::OnEnabled();

    //glUniform1i( m_hSourceTex, m_sourceTexUnit );
    //VerifyGLState();
    return true;
}
