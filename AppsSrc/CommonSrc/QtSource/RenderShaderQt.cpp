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

#include <QWidget> // must be declared first or Qt 6.2.4 will error in qmetatype.h 2167:23: array subscript value ‘53’ is outside the bounds

#include "RenderShaderQt.h"
#include "RenderGlWidget.h"

#if ENABLE_KODI
#include "ServiceBroker.h"
#include "utils/log.h"
#include "rendering/RenderSystem.h"
#include "rendering/MatrixGL.h"
#include "windowing/GraphicContext.h"
#include "cores/VideoPlayer/VideoRenderers/VideoShaders/YUVMatrix.h"
#include "cores/VideoPlayer/VideoRenderers/VideoShaders/ConvolutionKernels.h"
#endif // ENABLE_KODI

#include <CoreLib/VxDebug.h>

 //============================================================================
static void CalculateYUVMatrixGLES( GLfloat      res[ 4 ][ 4 ]
                                    , unsigned int flags
                                    , EShaderFormat format
                                    , float        black
                                    , float         contrast
                                    , bool          limited )
{
#if ENABLE_KODI
    TransformMatrix matrix;
    CalculateYUVMatrix( matrix, flags, format, black, contrast, limited );

    for( int row = 0; row < 3; row++ )
        for( int col = 0; col < 4; col++ )
            res[ col ][ row ] = matrix.m[ row ][ col ];

    res[ 0 ][ 3 ] = 0.0f;
    res[ 1 ][ 3 ] = 0.0f;
    res[ 2 ][ 3 ] = 0.0f;
    res[ 3 ][ 3 ] = 1.0f;
#endif // ENABLE_KODI
}

//============================================================================
RenderShaderQt::RenderShaderQt( ESHADERMETHOD shaderMethod, EShaderType shaderType, QString shaderName, RenderGlLogic& renderLogic )
    : QOpenGLShaderProgram()
    , m_ShaderMethod( shaderMethod )
    , m_ScalingMethod( VS_SCALINGMETHOD_LINEAR )
    , m_ShaderType( shaderType )
    , m_ShaderName( shaderName )
    , m_RenderLogic( renderLogic )
{
 //#ifdef GL_RGBA16F_EXT
#if 0 //  crashes because render system not setup yet
    if( CServiceBroker::GetRenderSystem()->IsExtSupported( "GL_EXT_color_buffer_float" ) )
    {
        m_floattex = true;
        m_internalformat = GL_RGBA16F_EXT;
    }
    else
    {
        m_floattex = false;
        m_internalformat = GL_RGBA;
    }
#else
    m_floattex = false;
    m_internalformat = GL_RGBA;
#endif // GL_RGBA16F_EXT
}

//============================================================================
void RenderGlShaders::compileShader( int shaderIdx )
{
    EShaderType shaderType = eShaderGui;
    ESHADERMETHOD shaderMethod = static_cast<ESHADERMETHOD>( shaderIdx );

    std::string shaderName = "SM_UNKNOWN";
    std::string vertexShaderCode;
    std::string fragmentShaderCode;

    getShaderSourceCode( shaderIdx, shaderType, shaderName, vertexShaderCode, fragmentShaderCode );

    if( !vertexShaderCode.empty() && !fragmentShaderCode.empty() )
    {
        //LogMsg( LOG_INFO, "Compiling shader %s", shaderName.toUtf8().constData() );
        m_RenderLogic.VerifyGLStateQt();

        RenderShaderQt * shader = new RenderShaderQt( shaderMethod, shaderType, shaderName.c_str(), m_RenderLogic );

        if( shader->compileAndLink( vertexShaderCode.c_str(), fragmentShaderCode.c_str() ) )
        {
            // success
//            if( shaderIdx == 10 ) 
//            {
//               LogMsg( LOG_INFO, "Success compile shader %s", shaderName.c_str() );
//            }

            m_Shaders[ shaderIdx ] = shader;
        }
        else
        {
            LogMsg( LOG_ERROR, "Failed to compile shader %d %s", shaderIdx, shaderName.c_str() );
        }
    }
}

//============================================================================
bool RenderShaderQt::compileAndLink( const char* vertexShaderCode, const char*fragmentShaderCode )
{
    bool result = true;
    m_proj = nullptr;
    m_model = nullptr;
    m_clipPossible = false;
    m_RenderLogic.VerifyGLStateQt();

    if( vertexShaderCode )
    {
        if( !addShaderFromSourceCode( QOpenGLShader::Vertex, vertexShaderCode ) )
        {
            LogMsg( LOG_ERROR, "could not add vertex shader %s", describeShader( m_ShaderMethod ) );
            result = false;
        }
    }
    
    if( result && !addShaderFromSourceCode( QOpenGLShader::Fragment, fragmentShaderCode ) )
    {
        LogMsg( LOG_ERROR, "could not add fragment shader %s", describeShader( m_ShaderMethod ) );
        result = false;
    }
    
    if( result && !link() )
    {
        LogMsg( LOG_ERROR, "could not link shader %s", describeShader( m_ShaderMethod ) );
    }

    m_RenderLogic.VerifyGLStateQt();
    
    if( result )
    {
        // success
        onCompiledAndLinked( vertexShaderCode, fragmentShaderCode );
    }

    if( result && !validateProgram() )
    {
        LogMsg( LOG_ERROR, "could not validate shader %s", describeShader( m_ShaderMethod ) );
    }

    m_RenderLogic.VerifyGLStateQt();

    return result;
}

//============================================================================
void RenderShaderQt::onCompiledAndLinked( const char* vertexShaderCode, const char*fragmentShaderCode )
{
    // This is called after CompileAndLink()

    QOpenGLFunctions * glf = m_RenderLogic.getGlFunctions();
    if( !glf )
    {
        LogMsg( LOG_ERROR, "onCompiledAndLinked could not get gl functions" );
        return;
    }

    onCompiledAndLinkedCommon( glf, vertexShaderCode, fragmentShaderCode );
    switch( m_ShaderType )
    {
    case eShaderVideoFilter:
        onCompiledAndLinkedVideoFilter( glf, vertexShaderCode, fragmentShaderCode );
        break;

    case eShaderVideoFormat:
        onCompiledAndLinkedVideoFormat( glf, vertexShaderCode, fragmentShaderCode );
        break;

    case eShaderGui:
    default:
        onCompiledAndLinkedGui( glf, vertexShaderCode, fragmentShaderCode );
        break;
    }

    if( !m_shaderValidated )
    {
        validateProgram();
    }
}

//============================================================================
bool RenderShaderQt::validateProgram()
{
    verifyProgramId();

    QOpenGLFunctions * glf = m_RenderLogic.getGlFunctions();
    if( glf && programId() )
    {
        glf->glUseProgram( programId() );
        //if( onEnabled() )
        {
            if( !m_shaderValidated )
            {
                // validate the program
                GLint params[ 4 ];
                glf->glValidateProgram( programId() );
                glf->glGetProgramiv( programId(), GL_VALIDATE_STATUS, params );
                if( params[ 0 ] != GL_TRUE )
                {
                    #define LOG_SIZE 1024

                    GLchar log[ LOG_SIZE ];
                    LogMsg( LOG_ERROR, "GL: Error validating shader" );
                    glf->glGetProgramInfoLog( programId(), LOG_SIZE, NULL, log );
                    LogMsg( LOG_ERROR, "%s", log );
                }

                //verifyShaderValues();
                m_shaderValidated = true;
            }

            //VerifyGLStateQt();
            return true;
        }
        //else
        //{
        //    LogMsg( LOG_ERROR, "GL: Error glUseProgram %d", programId() );
        //    glf->glUseProgram( 0 );
        //    return false;
        //}
    }

    return false;
}

//============================================================================
void RenderShaderQt::onCompiledAndLinkedCommon( QOpenGLFunctions * glf, const char* vertexShaderCode, const char*fragmentShaderCode )
{
    if( ( m_ShaderMethod != SM_VID_FILTER_CONVOLUTION_4X4_RGBA )
        && ( m_ShaderMethod != SM_VID_FILTER_CONVOLUTION_4X4_FLOAT )
        && ( m_ShaderMethod != SM_VID_FILTER_CONVOLUTION_6X6_RGBA )
        && ( m_ShaderMethod != SM_VID_FILTER_CONVOLUTION_6X6_FLOAT )
        )
    {
        //=== all other shaders have these vars ===//
        m_hProj = glf->glGetUniformLocation( programId(), "m_proj" );
        verifyValidValue( m_hProj, "m_hProj" );

        m_hModel = glf->glGetUniformLocation( programId(), "m_model" );
        verifyValidValue( m_hModel, "m_hModel" );
    }
}

//============================================================================
void RenderShaderQt::onCompiledAndLinkedGui( QOpenGLFunctions * glf, const char* vertexShaderCode, const char*fragmentShaderCode )
{

    //=== Variables passed directly to the Vertex shader ===//
//#ifdef ENABLE_GLES_SHADERS
    m_hCoord0Matrix = glf->glGetUniformLocation( programId(), "m_coord0Matrix" );
//    verifyValidValue( m_hCoord0Matrix, "m_hCoord0Matrix" ); 
//#endif // ENABLE_GLES_SHADERS

    m_hPos = glf->glGetAttribLocation( programId(), "m_attrpos" );
    verifyValidValue( m_hPos, "m_hPos" );

    if( ( SM_MULTI != m_ShaderMethod )
        )
    {
        m_hCol = glf->glGetAttribLocation( programId(), "m_attrcol" );
        //verifyValidValue( m_hCol, "m_hCol" );
    }

    m_hCord0 = glf->glGetAttribLocation( programId(), "m_attrcord0" );
    //verifyValidValue( m_hCord0, "m_hCord0" );

    m_hCord1 = glf->glGetAttribLocation( programId(), "m_attrcord1" );
    //verifyValidValue( m_hCord1, "m_hCord1" );

    // Variables passed directly to the Fragment shader
    if( ( SM_MULTI != m_ShaderMethod )
        && ( SM_FONTS != m_ShaderMethod )
        && ( SM_TEXTURE_NOBLEND != m_ShaderMethod )
        && ( SM_MULTI_BLENDCOLOR != m_ShaderMethod )
#ifdef ENABLE_GLES_SHADERS
        && ( SM_TEXTURE_RGBA != m_ShaderMethod ) 
        && ( SM_TEXTURE_RGBA_OES != m_ShaderMethod )
        && ( SM_TEXTURE_RGBA_BLENDCOLOR != m_ShaderMethod )
        && ( SM_TEXTURE_RGBA_BOB != m_ShaderMethod )    
        && ( SM_TEXTURE_RGBA_BOB_OES != m_ShaderMethod )
#endif // ENABLE_GLES_SHADERS
        )
    {
        m_hUniCol = glf->glGetUniformLocation( programId(), "m_unicol" );
        verifyValidValue( m_hUniCol, "m_hUniCol" );
    }

    if( ( SM_DEFAULT != m_ShaderMethod )
        && ( SM_VIDEO_YUY2_BASIC != m_ShaderMethod ) 
        )
    {
        m_hTex0 = glf->glGetUniformLocation( programId(), "m_samp0" );
        verifyValidValue( m_hTex0, "m_hTex0" );
    }

    if( ( SM_DEFAULT != m_ShaderMethod )
        && ( SM_TEXTURE != m_ShaderMethod )
        && ( SM_FONTS != m_ShaderMethod )
        && ( SM_TEXTURE_NOBLEND != m_ShaderMethod )
#ifdef ENABLE_GLES_SHADERS
        && ( SM_TEXTURE_RGBA != m_ShaderMethod )
        && ( SM_TEXTURE_RGBA_OES != m_ShaderMethod )
        && ( SM_TEXTURE_RGBA_BLENDCOLOR != m_ShaderMethod )
        && ( SM_TEXTURE_RGBA_BOB != m_ShaderMethod )
        && ( SM_TEXTURE_RGBA_BOB_OES != m_ShaderMethod )
#endif // ENABLE_GLES_SHADERS

        )
    {
        m_hTex1 = glf->glGetUniformLocation( programId(), "m_samp1" );
        verifyValidValue( m_hTex1, "m_hTex1" );
    }

    if( ( SM_DEFAULT != m_ShaderMethod )
        && ( SM_TEXTURE != m_ShaderMethod )
        && ( SM_MULTI != m_ShaderMethod )
        && ( SM_FONTS != m_ShaderMethod )
        && ( SM_TEXTURE_NOBLEND != m_ShaderMethod )
        && ( SM_MULTI_BLENDCOLOR != m_ShaderMethod )
#ifdef ENABLE_GLES_SHADERS
        && ( SM_TEXTURE_RGBA != m_ShaderMethod )
        && ( SM_TEXTURE_RGBA_OES != m_ShaderMethod )
        && ( SM_TEXTURE_RGBA_BLENDCOLOR != m_ShaderMethod )
#endif // ENABLE_GLES_SHADERS

        )
    {
        m_hField = glf->glGetUniformLocation( programId(), "m_field" );
        verifyValidValue( m_hField, "m_hField" );
        m_hStep = glf->glGetUniformLocation( programId(), "m_step" );
        verifyValidValue( m_hStep, "m_hStep" );
    }

    if( ( SM_DEFAULT != m_ShaderMethod )
        && ( SM_TEXTURE != m_ShaderMethod )
        && ( SM_MULTI != m_ShaderMethod )
        && ( SM_FONTS != m_ShaderMethod )
        && ( SM_TEXTURE_NOBLEND != m_ShaderMethod )
        && ( SM_MULTI_BLENDCOLOR != m_ShaderMethod ) 
#ifdef ENABLE_GLES_SHADERS
        && ( SM_TEXTURE_RGBA_BLENDCOLOR != m_ShaderMethod )
#endif // ENABLE_GLES_SHADERS

        )
    {
        m_hContrast = glf->glGetUniformLocation( programId(), "m_contrast" );
        verifyValidValue( m_hContrast, "m_hContrast" );
        m_hBrightness = glf->glGetUniformLocation( programId(), "m_brightness" );
        verifyValidValue( m_hBrightness, "m_hBrightness" );
    }

    if( ( SM_DEFAULT != m_ShaderMethod )
        && ( SM_TEXTURE != m_ShaderMethod )
        && ( SM_MULTI != m_ShaderMethod )
        && ( SM_FONTS != m_ShaderMethod )
        && ( SM_TEXTURE_NOBLEND != m_ShaderMethod )
        && ( SM_MULTI_BLENDCOLOR != m_ShaderMethod )
#ifdef ENABLE_GLES_SHADERS
        && ( SM_TEXTURE_RGBA != m_ShaderMethod )
        && ( SM_TEXTURE_RGBA_OES != m_ShaderMethod )
        && ( SM_TEXTURE_RGBA_BLENDCOLOR != m_ShaderMethod )
        && ( SM_TEXTURE_RGBA_BOB != m_ShaderMethod )
        && ( SM_TEXTURE_RGBA_BOB_OES != m_ShaderMethod )
#endif // ENABLE_GLES_SHADERS

        )
    {
        m_hMethod = glf->glGetUniformLocation( programId(), "m_method" );
        verifyValidValue( m_hMethod, "m_method" );
    }

    if( ( SM_DEFAULT != m_ShaderMethod )
        && ( SM_TEXTURE != m_ShaderMethod )
        && ( SM_MULTI != m_ShaderMethod )
        && ( SM_FONTS != m_ShaderMethod )
        && ( SM_TEXTURE_NOBLEND != m_ShaderMethod )
        && ( SM_MULTI_BLENDCOLOR != m_ShaderMethod )
#ifdef ENABLE_GLES_SHADERS
        && ( SM_TEXTURE_RGBA != m_ShaderMethod )
        && ( SM_TEXTURE_RGBA_OES != m_ShaderMethod )
        && ( SM_TEXTURE_RGBA_BLENDCOLOR != m_ShaderMethod )
        && ( SM_TEXTURE_RGBA_BOB != m_ShaderMethod )
        && ( SM_TEXTURE_RGBA_BOB_OES != m_ShaderMethod )
#endif // ENABLE_GLES_SHADERS


        )
    {
        m_hColor = glf->glGetUniformLocation( programId(), "m_colour" );
        verifyValidValue( m_hColor, "m_colour" );
    }

    //=== Setup program for use ===//

    // It's okay to do this only one time. Textures units never change.
    glf->glUseProgram( programId() );
    glf->glUniform1i( m_hTex0, 0 );

    if( m_hTex1 >= 0 )
    {
        glf->glUniform1i( m_hTex1, 1 );
    }

    if( m_hUniCol >= 0 )
    {
        glf->glUniform4f( m_hUniCol, 1.0, 1.0, 1.0, 1.0 );
    }

    static const float identity[ 16 ] = {
      1.0f, 0.0f, 0.0f, 0.0f,
      0.0f, 1.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 1.0f, 0.0f,
      0.0f, 0.0f, 0.0f, 1.0f
    };

    glf->glUniformMatrix4fv( m_hCoord0Matrix, 1, GL_FALSE, identity );

    glf->glUseProgram( 0 );
}

//============================================================================
void RenderShaderQt::onCompiledAndLinkedVideoFormat( QOpenGLFunctions * glf, const char* vertexShaderCode, const char*fragmentShaderCode )
{
    // Variables passed directly to the Vertex shader
    m_hVertex = glf->glGetAttribLocation( programId(), "m_attrpos" );
    verifyValidValue( m_hVertex, "m_hVertex" );

    m_hYcoord = glf->glGetAttribLocation( programId(), "m_attrcordY" );
    verifyValidValue( m_hYcoord, "m_hYcoord" );
    m_hUcoord = glf->glGetAttribLocation( programId(), "m_attrcordU" );
    //verifyValidValue( m_hUcoord, "m_hUcoord" );
    m_hVcoord = glf->glGetAttribLocation( programId(), "m_attrcordV" );
    //verifyValidValue( m_hVcoord, "m_hVcoord" );

    // Variables passed directly to the Fragment shader
    m_hAlpha = glf->glGetUniformLocation( programId(), "m_alpha" );
    verifyValidValue( m_hAlpha, "m_hAlpha" );

    m_hYTex = glf->glGetUniformLocation( programId(), "m_sampY" );
    verifyValidValue( m_hYTex, "m_hYTex" );
    m_hUTex = glf->glGetUniformLocation( programId(), "m_sampU" );
    //verifyValidValue( m_hUTex, "m_hUTex" );
    m_hVTex = glf->glGetUniformLocation( programId(), "m_sampV" );
    //verifyValidValue( m_hVTex, "m_hVTex" );

    m_hMatrix = glf->glGetUniformLocation( programId(), "m_yuvmat" );
    verifyValidValue( m_hMatrix, "m_hMatrix" );

    if( ( SM_VIDEO_YV12_BASIC != m_ShaderMethod )
        && ( SM_VIDEO_NV12_BASIC != m_ShaderMethod )
        )
    {
        m_hStep = glf->glGetUniformLocation( programId(), "m_step" );
        verifyValidValue( m_hStep, "m_hStep" );
    }

#ifdef ENABLE_GLES_SHADERS

    //VerifyGLStateQt();
    if( ( SM_VIDEO_YV12_BOB == m_ShaderMethod )
        || ( SM_VIDEO_YV12_BOB == m_ShaderMethod )
        || ( SM_VIDEO_NV12_BOB == m_ShaderMethod )
        || ( SM_VIDEO_YUY2_BOB == m_ShaderMethod )
        || ( SM_VIDEO_UYVY_BOB == m_ShaderMethod )
        || ( SM_VIDEO_NV12_RGB_BOB == m_ShaderMethod )
        )
    {
        m_hStepX = glf->glGetUniformLocation( programId(), "m_stepX" );
        verifyValidValue( m_hStepX, "m_hStepX" );
        m_hStepY = glf->glGetUniformLocation( programId(), "m_stepY" );
        verifyValidValue( m_hStepY, "m_hStepY" );
        m_hField = glf->glGetUniformLocation( programId(), "m_field" );
        verifyValidValue( m_hField, "m_hField" );

        m_hPrimMat = glf->glGetUniformLocation( programId(), "m_primMat" );
        verifyValidValue( m_hPrimMat, "m_hPrimMat" );
        m_hGammaDstInv = glf->glGetUniformLocation( programId(), "m_gammaDstInv" );
        verifyValidValue( m_hGammaDstInv, "m_hGammaDstInv" );
        m_hGammaSrc = glf->glGetUniformLocation( programId(), "m_gammaSrc" );
        verifyValidValue( m_hGammaSrc, "m_hGammaSrc" );
        m_hToneP1 = glf->glGetUniformLocation( programId(), "m_toneP1" );
        verifyValidValue( m_hToneP1, "m_hToneP1" );
        m_hCoefsDst = glf->glGetUniformLocation( programId(), "m_coefsDst" );
        verifyValidValue( m_hCoefsDst, "m_hCoefsDst" );
    }
#endif // ENABLE_GLES_SHADERS

}

//============================================================================
void RenderShaderQt::onCompiledAndLinkedVideoFilter( QOpenGLFunctions * glf, const char* vertexShaderCode, const char*fragmentShaderCode )
{
#if ENABLE_KODI
    m_hVertex = glf->glGetAttribLocation( programId(), "m_attrpos" );
    verifyValidValue( m_hVertex, "m_hVertex" );
    m_hcoord = glf->glGetAttribLocation( programId(), "m_attrcord" );
    verifyValidValue( m_hcoord, "m_hcoord" );
    m_hAlpha = glf->glGetUniformLocation( programId(), "m_alpha" );
    //verifyValidValue( m_hAlpha, "m_hAlpha" );

    switch( m_ShaderMethod )
    {
    case SM_VID_FILTER_CONVOLUTION_4X4_RGBA:
    case SM_VID_FILTER_CONVOLUTION_4X4_FLOAT:
    case SM_VID_FILTER_CONVOLUTION_6X6_RGBA:
    case SM_VID_FILTER_CONVOLUTION_6X6_FLOAT:
    {
        m_RenderLogic.VerifyGLStateQt();

        // obtain shader attribute handles on successful compilation
        m_hSourceTex    = glf->glGetUniformLocation( programId(), "img" );
        verifyValidValue( m_hSourceTex, "m_hSourceTex" );
        m_hStepXY       = glf->glGetUniformLocation( programId(), "stepxy" );
        verifyValidValue( m_hStepXY, "m_hStepXY" );
        m_hKernTex      = glf->glGetUniformLocation( programId(), "kernelTex" );
        verifyValidValue( m_hKernTex, "m_hKernTex" );

        m_RenderLogic.VerifyGLStateQt();
        CConvolutionKernel kernel( m_ScalingMethod, 256 );
        m_RenderLogic.VerifyGLStateQt();

        if( m_kernelTex1 )
        {
            glf->glDeleteTextures( 1, &m_kernelTex1 );
            m_kernelTex1 = 0;
        }

        m_RenderLogic.VerifyGLStateQt();

        glf->glGenTextures( 1, &m_kernelTex1 );
        verifyValidValue( m_kernelTex1, "m_kernelTex1" );
        m_RenderLogic.VerifyGLStateQt();

        if( ( m_kernelTex1 <= 0 ) )
        {
            CLog::Log( LOGERROR, "GL: ConvolutionFilterShader: Error creating kernel texture" );
            return;
        }

        m_RenderLogic.VerifyGLStateQt();

        //make a kernel texture on GL_TEXTURE2 and set clamping and interpolation
        glf-> glActiveTexture( GL_TEXTURE2 );
        glf->glBindTexture( GL_TEXTURE_2D, m_kernelTex1 );
        glf->glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        glf->glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glf->glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
        glf->glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

        //if float textures are supported, we can load the kernel as a float texture
        //if not we load it as 8 bit unsigned which gets converted back to float in the shader
        GLenum  format;
        GLvoid* data;
        if( m_floattex )
        {
            format = GL_FLOAT;
            data = ( GLvoid* )kernel.GetFloatPixels();
        }
        else
        {
            format = GL_UNSIGNED_BYTE;
            data = ( GLvoid* )kernel.GetUint8Pixels();
        }
        m_RenderLogic.VerifyGLStateQt();

        //upload as 2D texture with height of 1
        glf->glTexImage2D( GL_TEXTURE_2D, 0, m_internalformat, kernel.GetSize(), 1, 0, GL_RGBA, format, data );

        glf->glActiveTexture( GL_TEXTURE0 );

        m_RenderLogic.VerifyGLStateQt();
        break;
    }

    default:
        break;
    }
#endif // ENABLE_KODI
}

//============================================================================
bool RenderShaderQt::onEnabled()
{
    // This is called after glUseProgram()

    if( !m_RenderLogic.getGlFunctions() )
    {
        LogMsg( LOG_ERROR, "onEnabled has no render widget" );
        return false;
    }

    QOpenGLFunctions * glf = m_RenderLogic.getGlFunctions();

    switch( m_ShaderType )
    {
    case eShaderVideoFilter:
        return onShaderVideoFilterEnabled( glf );

    case eShaderVideoFormat:
        return onShaderVideoFormatEnabled( glf );

    case eShaderGui:
    default:
        return onShaderGuiEnabled( glf );
    }
}
    
//============================================================================
bool RenderShaderQt::onShaderGuiEnabled( QOpenGLFunctions * glf )
{
#if ENABLE_KODI
    const GLfloat *projMatrix = glMatrixProject.Get();
    const GLfloat *modelMatrix = glMatrixModview.Get();
    verifyValidValue( m_hProj, "m_hProj" );
    glf->glUniformMatrix4fv( m_hProj, 1, GL_FALSE, projMatrix );
    verifyValidValue( m_hModel, "m_hModel" );
    glf->glUniformMatrix4fv( m_hModel, 1, GL_FALSE, modelMatrix );

    const TransformMatrix &guiMatrix = CServiceBroker::GetWinSystem()->GetGfxContext().GetGUIMatrix();
    CRect viewPort; // absolute positions of corners
    CServiceBroker::GetRenderSystem()->GetViewPort( viewPort );

    /* glScissor operates in window coordinates. In order that we can use it to
     * perform clipping, we must ensure that there is an independent linear
     * transformation from the coordinate system used by CGraphicContext::ClipRect
     * to window coordinates, separately for X and Y (in other words, no
     * rotation or shear is introduced at any stage). To do, this, we need to
     * check that zeros are present in the following locations:
     *
     * GUI matrix:
     * / * 0 * * \
     * | 0 * * * |
     * \ 0 0 * * /
     *       ^ TransformMatrix::TransformX/Y/ZCoord are only ever called with
     *         input z = 0, so this column doesn't matter
     * Model-view matrix:
     * / * 0 0 * \
     * | 0 * 0 * |
     * | 0 0 * * |
     * \ * * * * /  <- eye w has no influence on window x/y (last column below
     *                                                       is either 0 or ignored)
     * Projection matrix:
     * / * 0 0 0 \
     * | 0 * 0 0 |
     * | * * * * |  <- normalised device coordinate z has no influence on window x/y
     * \ 0 0 * 0 /
     *
     * Some of these zeros are not strictly required to ensure this, but they tend
     * to be zeroed in the common case, so by checking for zeros here, we simplify
     * the calculation of the window x/y coordinates further down the line.
     *
     * (Minor detail: we don't quite deal in window coordinates as defined by
     * OpenGL, because CRenderSystemGLES::SetScissors flips the Y axis. But all
     * that's needed to handle that is an effective negation at the stage where
     * Y is in normalized device coordinates.)
     */
    m_clipPossible = guiMatrix.m[ 0 ][ 1 ] == 0 &&
        guiMatrix.m[ 1 ][ 0 ] == 0 &&
        guiMatrix.m[ 2 ][ 0 ] == 0 &&
        guiMatrix.m[ 2 ][ 1 ] == 0 &&
        modelMatrix[ 0 + 1 * 4 ] == 0 &&
        modelMatrix[ 0 + 2 * 4 ] == 0 &&
        modelMatrix[ 1 + 0 * 4 ] == 0 &&
        modelMatrix[ 1 + 2 * 4 ] == 0 &&
        modelMatrix[ 2 + 0 * 4 ] == 0 &&
        modelMatrix[ 2 + 1 * 4 ] == 0 &&
        projMatrix[ 0 + 1 * 4 ] == 0 &&
        projMatrix[ 0 + 2 * 4 ] == 0 &&
        projMatrix[ 0 + 3 * 4 ] == 0 &&
        projMatrix[ 1 + 0 * 4 ] == 0 &&
        projMatrix[ 1 + 2 * 4 ] == 0 &&
        projMatrix[ 1 + 3 * 4 ] == 0 &&
        projMatrix[ 3 + 0 * 4 ] == 0 &&
        projMatrix[ 3 + 1 * 4 ] == 0 &&
        projMatrix[ 3 + 3 * 4 ] == 0;

    m_clipXFactor = 0.0;
    m_clipXOffset = 0.0;
    m_clipYFactor = 0.0;
    m_clipYOffset = 0.0;

    if( m_clipPossible )
    {
        m_clipXFactor = guiMatrix.m[ 0 ][ 0 ] * modelMatrix[ 0 + 0 * 4 ] * projMatrix[ 0 + 0 * 4 ];
        m_clipXOffset = ( guiMatrix.m[ 0 ][ 3 ] * modelMatrix[ 0 + 0 * 4 ] + modelMatrix[ 0 + 3 * 4 ] ) * projMatrix[ 0 + 0 * 4 ];
        m_clipYFactor = guiMatrix.m[ 1 ][ 1 ] * modelMatrix[ 1 + 1 * 4 ] * projMatrix[ 1 + 1 * 4 ];
        m_clipYOffset = ( guiMatrix.m[ 1 ][ 3 ] * modelMatrix[ 1 + 1 * 4 ] + modelMatrix[ 1 + 3 * 4 ] ) * projMatrix[ 1 + 1 * 4 ];
        float clipW = ( guiMatrix.m[ 2 ][ 3 ] * modelMatrix[ 2 + 2 * 4 ] + modelMatrix[ 2 + 3 * 4 ] ) * projMatrix[ 3 + 2 * 4 ];
        float xMult = ( viewPort.x2 - viewPort.x1 ) / ( 2 * clipW );
        float yMult = ( viewPort.y1 - viewPort.y2 ) / ( 2 * clipW ); // correct for inverted window coordinate scheme
        m_clipXFactor = m_clipXFactor * xMult;
        m_clipXOffset = m_clipXOffset * xMult + ( viewPort.x2 + viewPort.x1 ) / 2;
        m_clipYFactor = m_clipYFactor * yMult;
        m_clipYOffset = m_clipYOffset * yMult + ( viewPort.y2 + viewPort.y1 ) / 2;
    }

    if( m_hBrightness >= 0 )
    {
        glf->glUniform1f( m_hBrightness, 0.0f );
    }

    if( m_hContrast >= 0 )
    {
        glf->glUniform1f( m_hContrast, 1.0f );
    }

#endif // ENABLE_KODI
    return true;
}

//============================================================================
bool RenderShaderQt::onShaderVideoFormatEnabled( QOpenGLFunctions * glf )
{
    //LogMsg( LOG_ERROR, "onShaderVideoFormatEnabled %s", describeShader( m_ShaderMethod ) );
    /*BaseYUV2RGBGLSLShader
    m_hYTex = glGetUniformLocation( ProgramHandle(), "m_sampY" );
    m_hUTex = glGetUniformLocation( ProgramHandle(), "m_sampU" );
    m_hVTex = glGetUniformLocation( ProgramHandle(), "m_sampV" );
    m_hYuvMat = glGetUniformLocation( ProgramHandle(), "m_yuvmat" );
    m_hStretch = glGetUniformLocation( ProgramHandle(), "m_stretch" );
    m_hStep = glGetUniformLocation( ProgramHandle(), "m_step" );
    m_hVertex = glGetAttribLocation( ProgramHandle(), "m_attrpos" );
    m_hYcoord = glGetAttribLocation( ProgramHandle(), "m_attrcordY" );
    m_hUcoord = glGetAttribLocation( ProgramHandle(), "m_attrcordU" );
    m_hVcoord = glGetAttribLocation( ProgramHandle(), "m_attrcordV" );
    m_hProj = glGetUniformLocation( ProgramHandle(), "m_proj" );
    m_hModel = glGetUniformLocation( ProgramHandle(), "m_model" );
    m_hAlpha = glGetUniformLocation( ProgramHandle(), "m_alpha" );
    m_hPrimMat = glGetUniformLocation( ProgramHandle(), "m_primMat" );
    m_hGammaSrc = glGetUniformLocation( ProgramHandle(), "m_gammaSrc" );
    m_hGammaDstInv = glGetUniformLocation( ProgramHandle(), "m_gammaDstInv" );
    m_hCoefsDst = glGetUniformLocation( ProgramHandle(), "m_coefsDst" );
    m_hToneP1 = glGetUniformLocation( ProgramHandle(), "m_toneP1" );
    */

    // set shader attributes once enabled
    m_RenderLogic.VerifyGLStateQt();
    verifyValidValue( m_hYTex, "m_hYTex" );
    glf->glUniform1i( m_hYTex, 0 );
    verifyValidValue( m_hUTex, "m_hUTex" );
    glf->glUniform1i( m_hUTex, 1 );
    verifyValidValue( m_hVTex, "m_hVTex" );
    glf->glUniform1i( m_hVTex, 2 );
    if( m_hStep >= 0 )
    {
        verifyValidValue( m_hStep, "m_hStep" );
        glf->glUniform2f( m_hStep, 1.0 / m_width, 1.0 / m_height );
    }

    GLfloat matrix[ 4 ][ 4 ];
    // keep video levels
    CalculateYUVMatrixGLES( matrix, m_flags, m_format, m_black, m_contrast, !m_convertFullRange );

    verifyValidValue( m_hMatrix, "m_hMatrix" );
    glf->glUniformMatrix4fv( m_hMatrix, 1, GL_FALSE, ( GLfloat* )matrix );
    verifyValidValue( m_hProj, "m_hProj" );
    glf->glUniformMatrix4fv( m_hProj, 1, GL_FALSE, m_proj );
    verifyValidValue( m_hModel, "m_hModel" );
    glf->glUniformMatrix4fv( m_hModel, 1, GL_FALSE, m_model );
    verifyValidValue( m_hAlpha, "m_hAlpha" );
    glf->glUniform1f( m_hAlpha, m_alpha );
    m_RenderLogic.VerifyGLStateQt();

#ifdef ENABLE_GLES_SHADERS

    switch( m_ShaderMethod )
    {
    case SM_VIDEO_YV12_BOB:
    case SM_VIDEO_NV12_BOB:
    case SM_VIDEO_YUY2_BOB:
    case SM_VIDEO_UYVY_BOB:
    case SM_VIDEO_NV12_RGB_BOB:
        verifyValidValue( m_hField, "m_hField" );
        glf->glUniform1i( m_hField, m_field );
        verifyValidValue( m_hStepX, "m_hStepX" );
        glf->glUniform1f( m_hStepX, 1.0f / ( float )m_width );
        verifyValidValue( m_hStepY, "m_hStepY" );
        glf->glUniform1f( m_hStepY, 1.0f / ( float )m_height );
        m_RenderLogic.VerifyGLStateQt();
        break;

    default:
        break;
    }
#endif // ENABLE_GLES_SHADERS

    return true;
}

//============================================================================
bool RenderShaderQt::onShaderVideoFilterEnabled( QOpenGLFunctions * glf )
{
    LogMsg( LOG_ERROR, "onShaderVideoFormatEnabled %s", describeShader( m_ShaderMethod ) );

    m_RenderLogic.VerifyGLStateQt();
    verifyValidValue( m_hProj, "m_hProj" );
    glf->glUniformMatrix4fv( m_hProj, 1, GL_FALSE, m_proj );
    verifyValidValue( m_hModel, "m_hModel" );
    glf->glUniformMatrix4fv( m_hModel, 1, GL_FALSE, m_model );
    verifyValidValue( m_hAlpha, "m_hAlpha" );
    glf->glUniform1f( m_hAlpha, m_alpha );

    switch( m_ShaderMethod )
    {
    case SM_VID_FILTER_CONVOLUTION_4X4_RGBA:
    case SM_VID_FILTER_CONVOLUTION_4X4_FLOAT:
    case SM_VID_FILTER_CONVOLUTION_6X6_RGBA:
    case SM_VID_FILTER_CONVOLUTION_6X6_FLOAT:
    {
        // set shader attributes once enabled
        glf->glActiveTexture( GL_TEXTURE2 );
        glf->glBindTexture( GL_TEXTURE_2D, m_kernelTex1 );

        glf->glActiveTexture( GL_TEXTURE0 );
        verifyValidValue( m_hSourceTex, "m_hSourceTex" );
        glf->glUniform1i( m_hSourceTex, m_sourceTexUnit );
        verifyValidValue( m_hKernTex, "m_hKernTex" );
        glf->glUniform1i( m_hKernTex, 2 );
        verifyValidValue( m_hStepXY, "m_hStepXY" );
        glf->glUniform2f( m_hStepXY, m_stepX, m_stepY );
        m_RenderLogic.VerifyGLStateQt();
        break;
    }

    default:
        break;
    }

    return true;
}

//============================================================================
bool RenderShaderQt::verifyValidValue( GLint handle, const char* msg )
{
    if( handle < 0 )
    {
        LogMsg( LOG_DEBUG, "ERROR %s verifyValidValue %s is %d", describeShader( m_ShaderMethod ), msg, handle );
        return false;
    }

    return true;
}

//============================================================================
const char* RenderShaderQt::describeShader( ESHADERMETHOD shaderType )
{
    switch( shaderType )
    {
    case SM_DEFAULT:
        return "SM_DEFAULT";
    case SM_TEXTURE:
        return "SM_TEXTURE";
    case SM_MULTI:
        return "SM_MULTI";
    case SM_FONTS:
        return "SM_FONTS";
    case SM_TEXTURE_NOBLEND:
        return "SM_TEXTURE_NOBLEND";
    case SM_MULTI_BLENDCOLOR:
        return "SM_MULTI_BLENDCOLOR";
#ifdef ENABLE_GLES_SHADERS

    case SM_TEXTURE_RGBA:
        return "SM_TEXTURE_RGBA";
    case SM_TEXTURE_RGBA_OES:
        return "SM_TEXTURE_RGBA_OES";
    case SM_TEXTURE_RGBA_BLENDCOLOR:
        return "SM_TEXTURE_RGBA_BLENDCOLOR";
    case SM_TEXTURE_RGBA_BOB:
        return "SM_TEXTURE_RGBA_BOB";
    case SM_TEXTURE_RGBA_BOB_OES:
        return "SM_TEXTURE_RGBA_BOB_OES";
#endif // ENABLE_GLES_SHADERS

    case SM_VIDEO_YV12_BASIC:
        return "SM_VIDEO_YV12_BASIC";
    case SM_VIDEO_NV12_BASIC:
        return "SM_VIDEO_NV12_BASIC";

    case SM_VIDEO_YUY2_BASIC:
        return "SM_VIDEO_YUY2_BASIC";
    case SM_VIDEO_UYVY_BASIC:
        return "SM_VIDEO_UYVY_BASIC";
    case SM_VIDEO_NV12_RGB_BASIC:
        return "SM_VIDEO_NV12_RGB_BASIC";

#ifdef ENABLE_GLES_SHADERS
    case SM_VIDEO_YV12_BOB:
        return "SM_VIDEO_YV12_BOB";
    case SM_VIDEO_NV12_BOB:
        return "SM_VIDEO_NV12_BOB";
    case SM_VIDEO_YUY2_BOB:
        return "SM_VIDEO_YUY2_BOB";
    case SM_VIDEO_UYVY_BOB:
        return "SM_VIDEO_UYVY_BOB";
    case SM_VIDEO_NV12_RGB_BOB:
        return "SM_VIDEO_NV12_RGB_BOB";
#endif // ENABLE_GLES_SHADERS

    case SM_VID_FILTER_DEFAULT:
        return "SM_VID_FILTER_DEFAULT";
    case SM_VID_FILTER_CONVOLUTION_4X4_RGBA:
        return "SM_VID_FILTER_CONVOLUTION_4X4_RGBA";
    case SM_VID_FILTER_CONVOLUTION_4X4_FLOAT:
        return "SM_VID_FILTER_CONVOLUTION_4X4_FLOAT";
    case SM_VID_FILTER_CONVOLUTION_6X6_RGBA:
        return "SM_VID_FILTER_CONVOLUTION_6X6_RGBA";
    case SM_VID_FILTER_CONVOLUTION_6X6_FLOAT:
        return "SM_VID_FILTER_CONVOLUTION_6X6_FLOAT";
    case SM_SHADER_NONE:
        return "SM_SHADER_NONE";
    default:
        return "SM_UNKNOWN";
    }
}

//============================================================================
void RenderShaderQt::onDisabled()
{
}

//============================================================================
void RenderShaderQt::onFree()
{
    if( m_kernelTex1 )
    {
        glDeleteTextures( 1, &m_kernelTex1 );
    }

    m_kernelTex1 = 0;

    removeAllShaders();
    deleteLater();
}

//============================================================================
bool RenderShaderQt::validateShader()
{
    m_RenderLogic.VerifyGLStateQt();

    QOpenGLFunctions * glf = m_RenderLogic.getGlFunctions();

    if( glf && m_shaderValidated )
    {
        glf->glUseProgram( programId() );
        if( onEnabled() )
        {
            if( !m_shaderValidated )
            {
                // validate the program
                GLint params[ 4 ];
                glf->glValidateProgram( programId() );
                glf->glGetProgramiv( programId(), GL_VALIDATE_STATUS, params );
                if( params[ 0 ] != GL_TRUE )
                {
#define LOG_SIZE 1024

                    GLchar log[ LOG_SIZE ];
                    LogMsg( LOG_ERROR, "GL: Error validating shader" );
                    glf->glGetProgramInfoLog( programId(), LOG_SIZE, NULL, log );
                    LogMsg( LOG_ERROR, "%s", log );
                }

                m_RenderLogic.VerifyGLStateQt();
                m_shaderValidated = true;
            }

            //VerifyGLStateQt();
            return true;
        }
        else
        {
            LogMsg( LOG_ERROR, "GL: Error glUseProgram %d", programId() );
            glf->glUseProgram( 0 );
        }
    }

    return false;
}

//============================================================================
bool RenderShaderQt::enableShader()
{
    verifyProgramId();
    QOpenGLFunctions * glf = m_RenderLogic.getGlFunctions();
    if( glf && m_shaderValidated )
    {
        glf->glUseProgram( programId() );
        if( onEnabled() )
        {
            if( !m_shaderValidated )
            {
                // validate the program
                GLint params[ 4 ];
                glf->glValidateProgram( programId() );
                glf->glGetProgramiv( programId(), GL_VALIDATE_STATUS, params );
                if( params[ 0 ] != GL_TRUE )
                {
#define LOG_SIZE 1024

                    GLchar log[ LOG_SIZE ];
                    LogMsg( LOG_ERROR, "GL: Error validating shader" );
                    glf->glGetProgramInfoLog( programId(), LOG_SIZE, NULL, log );
                    LogMsg( LOG_ERROR, "%s", log );
                }

                //verifyShaderValues();
                m_shaderValidated = true;
            }

            //VerifyGLStateQt();
            return true;
        }
        else
        {
            LogMsg( LOG_ERROR, "GL: Error glUseProgram %d", programId() );
            glf->glUseProgram( 0 );
            return false;
        }
    }
    else
    {
        LogMsg( LOG_ERROR, "GL: Error Shader %s Program %d not validated", programId() );
    }


    return true;
}

//============================================================================
bool RenderShaderQt::disableShader()
{
    switch( m_ShaderMethod )
    {
    case SM_VID_FILTER_CONVOLUTION_4X4_RGBA:
    case SM_VID_FILTER_CONVOLUTION_4X4_FLOAT:
    case SM_VID_FILTER_CONVOLUTION_6X6_RGBA:
    case SM_VID_FILTER_CONVOLUTION_6X6_FLOAT:
        return true; 
    default:
        break;
    }

    QOpenGLFunctions * glf = m_RenderLogic.getGlFunctions();
    glf->glUseProgram( 0 );
    onDisabled();
    return true;
}

// yuv shader
//============================================================================
void RenderShaderQt::shaderSetField( int field )
{
    m_field = field;
    //VerifyGLStateQt();
}

//============================================================================
void RenderShaderQt::shaderSetWidth( int w )
{
    m_width = w;
    //VerifyGLStateQt();
}

//============================================================================
void RenderShaderQt::shaderSetHeight( int h )
{
    m_height = h;

   // VerifyGLStateQt();
}

//============================================================================
void RenderShaderQt::shaderSetBlack( float black )
{
    m_black = black;

    //VerifyGLStateQt();
}

//============================================================================
void RenderShaderQt::shaderSetContrast( float contrast )
{
    m_contrast = contrast;
    //VerifyGLStateQt();
}

//============================================================================
void RenderShaderQt::shaderSetConvertFullColorRange( bool convertFullRange )
{
    m_RenderLogic.VerifyGLStateQt();
    m_convertFullRange = convertFullRange;
    m_RenderLogic.VerifyGLStateQt();
}

//============================================================================
int RenderShaderQt::shaderGetVertexLoc(  )
{
    verifyValidValue( m_hVertex, "m_hVertex" );
    return m_hVertex;
}

//============================================================================
int RenderShaderQt::shaderGetYcoordLoc(  )
{
    verifyValidValue( m_hYcoord, "m_hYcoord" );
    return m_hYcoord;
}

//============================================================================
int RenderShaderQt::shaderGetUcoordLoc(  )
{
    verifyValidValue( m_hUcoord, "m_hUcoord" );
    return m_hUcoord;
}

//============================================================================
int RenderShaderQt::shaderGetVcoordLoc(  )
{
    verifyValidValue( m_hVcoord, "m_hVcoord" );
    return m_hVcoord;
}

//============================================================================
void RenderShaderQt::shaderSetMatrices( const float *p, const float *m )
{
    m_proj = p; 
    m_model = m;
}

//============================================================================
void RenderShaderQt::shaderSetAlpha( float alpha )
{
    m_alpha = alpha;
}

//============================================================================
void RenderShaderQt::shaderSetFlags( unsigned int flags )
{
    m_flags = flags;
}

//============================================================================
void RenderShaderQt::shaderSetFormat(  EShaderFormat format )
{
    m_format = format;
}

//============================================================================
void RenderShaderQt::shaderSourceTexture( int ytex )
{
    m_sourceTexUnit = ytex;
}

//============================================================================
void RenderShaderQt::shaderSetStepX( float stepX )
{
    m_stepX = stepX;
}

//============================================================================
void RenderShaderQt::shaderSetStepY( float stepY )
{
    m_stepY = stepY;
}

//============================================================================
// filter shader
bool RenderShaderQt::shaderGetTextureFilter(  int& filter )
{
    return false;
}

//============================================================================
int RenderShaderQt::shaderGetcoordLoc(  )
{
    verifyValidValue( m_hcoord, "m_hcoord" );
    return m_hcoord;
}

//============================================================================
int RenderShaderQt::shaderVertexAttribPointer( unsigned int index, int size, int type, bool normalized, int stride, const void * pointer )
{
    QOpenGLFunctions * glf = m_RenderLogic.getGlFunctions();
    glf->glVertexAttribPointer( index, size, type, normalized, stride, pointer );
    //VerifyGLStateQt();
    return 0; // return value not needed .. should be void
}

//============================================================================
void RenderShaderQt::shaderEnableVertexAttribArray( int arrayId )
{
    QOpenGLFunctions * glf = m_RenderLogic.getGlFunctions();
    glf->glEnableVertexAttribArray( arrayId );
    //VerifyGLStateQt();
}

//============================================================================
void RenderShaderQt::shaderDisableVertexAttribArray( int arrayId )
{
    QOpenGLFunctions * glf = m_RenderLogic.getGlFunctions();
    glf->glDisableVertexAttribArray( arrayId );
    //VerifyGLStateQt();
}

//============================================================================
bool RenderShaderQt::verifyProgramId()
{
    if( programId() <= 0 )
    {
        LogMsg( LOG_DEBUG, "ERROR %s verifyProgramId id is %d", describeShader( m_ShaderMethod ), programId() );
        return false;
    }

    return true;
}

