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
#include "config_apps.h"

#include "GuiInterface/IGoTvRender.h"
#include "cores/VideoPlayer/VideoRenderers/VideoShaders/ShaderFormats.h"
#include "cores/VideoSettings.h"

#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>


enum EShaderType
{
    eShaderGui,
    eShaderVideoFormat,
    eShaderVideoFilter,

    eMaxShaderType
};

class RenderGlWidget;
class RenderGlLogic;


class RenderShaderQt : public QOpenGLShaderProgram
{
public:
    RenderShaderQt() = delete;
    RenderShaderQt( ESHADERMETHOD shaderMethod, EShaderType shaderType, QString shaderName, RenderGlLogic& renderLogic );

    bool                        compileAndLink( const char * vertexShaderCode, const char * fragmentShaderCode );

    bool                        isShaderValid()     { return m_shaderValidated; }
    EShaderType                 getShaderType()     { return m_ShaderType; }

    bool                        enableShader();
    bool                        disableShader();

    bool                        onEnabled();
    bool                        onShaderGuiEnabled( QOpenGLFunctions * glf );
    bool                        onShaderVideoFormatEnabled( QOpenGLFunctions * glf );
    bool                        onShaderVideoFilterEnabled( QOpenGLFunctions * glf );

    void                        onDisabled();
    void                        onFree();

    void                        setShaderFlags(  unsigned int flags )                   { m_flags = flags; }
    void                        setShaderScalingMethod( ESCALINGMETHOD scaling )        { m_ScalingMethod = scaling; }

    GLint                       GetPosLoc()         { verifyValidValue( m_hPos, "m_hPos" );  return m_hPos; }
    GLint                       GetColLoc()         { verifyValidValue( m_hCol, "m_hCol" );  return m_hCol; }
    GLint                       GetCord0Loc()       { verifyValidValue( m_hCord0, "m_hCord0" ); return m_hCord0; }
    GLint                       GetCord1Loc()       { return m_hCord1; } // SM_TEXTURE_NOBLEND calls even though invalid
    GLint                       GetUniColLoc()      { return m_hUniCol; } // called when not valid so do not validate
    GLint                       GetCoord0MatrixLoc() { verifyValidValue( m_hCoord0Matrix, "m_hCoord0Matrix" ); return m_hCoord0Matrix; }
    GLint                       GetFieldLoc()       { verifyValidValue( m_hField, "m_hField" ); return m_hField; }
    GLint                       GetStepLoc()        { verifyValidValue( m_hStep, "m_hStep" ); return m_hStep; }
    GLint                       GetContrastLoc()    { verifyValidValue( m_hContrast, "m_hContrast" ); return m_hContrast; }
    GLint                       GetBrightnessLoc()  { verifyValidValue( m_hBrightness, "m_hBrightness" ); return m_hBrightness; }
    GLint                       GetModelLoc()       { verifyValidValue( m_hModel, "m_hModel" ); return m_hModel; }

    bool                        HardwareClipIsPossible() { return m_clipPossible; }
    GLfloat                     GetClipXFactor()    { return m_clipXFactor; }
    GLfloat                     GetClipXOffset()    { return m_clipXOffset; }
    GLfloat                     GetClipYFactor()    { return m_clipYFactor; }
    GLfloat                     GetClipYOffset()    { return m_clipYOffset; }

    // yuv shader
    virtual void                shaderSetField( int field );
    virtual void                shaderSetWidth( int w );
    virtual void                shaderSetHeight( int h );

    virtual void                shaderSetBlack( float black );
    virtual void                shaderSetContrast( float contrast );
    virtual void                shaderSetConvertFullColorRange( bool convertFullRange );

    virtual int                 shaderGetVertexLoc(  );
    virtual int                 shaderGetYcoordLoc(  );
    virtual int                 shaderGetUcoordLoc(  );
    virtual int                 shaderGetVcoordLoc(  );

    virtual void                shaderSetMatrices( const float *p, const float *m );
    virtual void                shaderSetAlpha( float alpha );

    virtual void                shaderSetFlags( unsigned int flags );
    virtual void                shaderSetFormat( EShaderFormat format );
    virtual void                shaderSourceTexture( int ytex );
    virtual void                shaderSetStepX( float stepX );
    virtual void                shaderSetStepY( float stepY );

    // filter shader
    virtual bool                shaderGetTextureFilter( int& filter );
    virtual int                 shaderGetcoordLoc(  );

    // renderqt
    virtual int                 shaderVertexAttribPointer( unsigned int index, int size, int type, bool normalized, int stride, const void * pointer );
    virtual void                shaderEnableVertexAttribArray( int arrayId ) ;
    virtual void                shaderDisableVertexAttribArray( int arrayId );

    static const char *         describeShader( ESHADERMETHOD shaderMethod );

protected:
    void                        onCompiledAndLinked( const char * vertexShaderCode, const char *fragmentShaderCode );
    void                        onCompiledAndLinkedCommon( QOpenGLFunctions * glf, const char * vertexShaderCode, const char *fragmentShaderCode );
    void                        onCompiledAndLinkedGui( QOpenGLFunctions * glf, const char * vertexShaderCode, const char *fragmentShaderCode );
    void                        onCompiledAndLinkedVideoFormat( QOpenGLFunctions * glf, const char * vertexShaderCode, const char *fragmentShaderCode );
    void                        onCompiledAndLinkedVideoFilter( QOpenGLFunctions * glf, const char * vertexShaderCode, const char *fragmentShaderCode );
    bool                        verifyProgramId();
    bool                        verifyValidValue( GLint handle, const char * msg );
    bool                        validateProgram();
    bool                        validateShader();

    //=== common to all shaders ===//
    ESHADERMETHOD               m_ShaderMethod;
    ESCALINGMETHOD              m_ScalingMethod;
    EShaderType                 m_ShaderType;
    QString                     m_ShaderName;
    RenderGlLogic&              m_RenderLogic;
    bool                        m_shaderValidated = false;

    const GLfloat *             m_proj = nullptr;
    const GLfloat *             m_model = nullptr;
    GLint                       m_hProj = -1;
    GLint                       m_hModel = -1;

    //=== gui shader ===//
    GLint                       m_hTex0 = -1;
    GLint                       m_hTex1 = -1;
    GLint                       m_hUniCol = -1;
    GLint                       m_hPos = -1;
    GLint                       m_hCol = -1;
    GLint                       m_hCord0 = -1;
    GLint                       m_hCord1 = -1;
    GLint                       m_hCoord0Matrix = -1;

    GLint                       m_hField = -1;

    GLint                       m_hMethod = -1;
    GLint                       m_hColor = -1;

    GLint                       m_hContrast = -1;
    GLint                       m_hBrightness = -1;

    bool                        m_clipPossible = false;
    GLfloat                     m_clipXFactor = 0.0f;
    GLfloat                     m_clipXOffset = 0.0f;
    GLfloat                     m_clipYFactor = 0.0f;
    GLfloat                     m_clipYOffset = 0.0f;

    //=== gui and format common ===//
    GLint                       m_hStep = -1;

    //=== video format shader ===//
    unsigned int                m_flags = 0;
    EShaderFormat               m_format = SHADER_YV12;
    int                         m_field = 0;

    float                       m_black = 0.0f;
    float                       m_contrast = 0.0f;

    // shader attribute handles
    GLint                       m_hYTex = -1;
    GLint                       m_hUTex = -1;
    GLint                       m_hVTex = -1;
    GLint                       m_hMatrix = -1;

    GLint                       m_hYcoord = -1;
    GLint                       m_hUcoord = -1;
    GLint                       m_hVcoord = -1;

    bool                        m_convertFullRange = false;

    //=== format and filter common ===//
    GLint                       m_hVertex = -1;
    GLint                       m_hAlpha = -1;

    int                         m_width = 1;
    int                         m_height = 1;
    GLfloat                     m_alpha  = 1;

    //=== video filter shader ===//
    float                       m_stepX = -1;
    float                       m_stepY = -1;
    GLint                       m_sourceTexUnit = -1;

    GLint                       m_hPrimMat = -1;
    GLint                       m_hGammaDstInv = -1;
    GLint                       m_hGammaSrc = -1;
    GLint                       m_hToneP1 = -1;
    GLint                       m_hCoefsDst = -1;

    // shader attribute handles
    GLint                       m_hSourceTex = -1;
    GLint                       m_hStepXY = -1;

    GLint                       m_hStepX = -1;
    GLint                       m_hStepY = -1;

    GLint                       m_hcoord = -1;

    // shader handles to kernel textures
    GLint                       m_hKernTex = -1;

    GLuint                      m_kernelTex1 = 0;

    bool                        m_floattex = false; // if float textures are supported
    GLint                       m_internalformat = 0;

};

