//============================================================================
// Copyright (C) 2018 Brett R. Jones
//
// You may use, copy, modify, merge, publish, distribute, sub-license, and/or sell this software
// provided this Copyright is not modified or removed and is included all copies or substantial portions of the Software
//
// This code is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// bjones.engineer@gmail.com
// http://www.nolimitconnect.org
//============================================================================

#ifdef TARGET_OS_WINDOWS
#include "WinSock2.h"
#endif // TARGET_OS_WINDOWS

#include <QWidget> // must be declared first or linux Qt 6.2.4 will error in qmetatype.h 2167:23: array subscript value ‘53’ is outside the bounds
#include <GuiInterface/NlcRenderFrame.h>

#include "RenderGlShaders.h"
#include "RenderShaderQt.h"
#include "RenderGlWidget.h"

#include <CoreLib/VxDebug.h>

#include <QDebug>
#include <QTimer>
#include <QFile>

# if defined(TARGET_OS_APPLE)
#  include <OpenGLES/ES2/gl.h>
# elif defined(TARGET_OS_ANDROID)
#  include <GLES2/gl2.h>
#  include <GLES2/gl2ext.h>
#  include <GLES3/gl3.h>
# elif defined(TARGET_OS_LINUX)
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/glext.h>
# elif defined(TARGET_OS_WINDOWS)
#  include <GL/gl.h>
#  include <GL/glu.h>
# endif // defined(TARGET_OS_ANDROID)

#define SHADERS_INCLUDE // NOTE: must modify this file for changes in RenderShaderDefsGl.cpp to take effect 
#include "RenderShaderDefsGl.cpp" 

//============================================================================
RenderGlShaders::RenderGlShaders( RenderGlLogic& renderGlLogic )
: m_RenderLogic( renderGlLogic )
{ 
}

//============================================================================
RenderGlShaders::~RenderGlShaders()
{
    destroyShaders();
}

//============================================================================
void RenderGlShaders::initShaders( QOpenGLFunctions * glf )
{
    m_Glf = glf;
    if( m_ShadersInited || !m_Glf )
    {
        return;
    }

    memset( m_Shaders, 0, sizeof( m_Shaders ) );
    for( int i = 0; i < SM_MAX; i++ )
    {
        compileShader( i );
    }

    m_ShadersInited = true;
}

//============================================================================
void RenderGlShaders::destroyShaders()
{
    if( m_ShadersInited )
    {
        if( QOpenGLContext::currentContext() )
        {
            for( int i = 0; i < SM_MAX; i++ )
            {
                if( m_Shaders[ i ] )
                {
                    m_Shaders[ i ]->onFree();
                    m_Shaders[ i ] = nullptr;
                }
            }
        }

        m_ShadersInited = false;
    }
}

//============================================================================
bool RenderGlShaders::enableShader( ESHADERMETHOD method )
{
#ifdef DEBUG_SHADER_ENABLE_DISABLE
    if( method != SM_FONTS )
    {
        LogMsg( LOG_ERROR, "enableShader - %s", describeShaderMethod( method ) );
    }
#endif // DEBUG_SHADER_ENABLE_DISABLE

    bool enabled = false;
    m_RenderLogic.VerifyGLStateQt();
    if( ( method < SM_MAX) && m_Shaders[ method ] )
    {
        enabled = m_Shaders[ method ]->enableShader();
        if( enabled && ( eShaderGui == m_Shaders[ method ]->getShaderType() ) )
        {
            m_CurShaderMethodType = method;
        }
    }
    else
    {
        LogMsg( LOG_ERROR, "Invalid GUI Shader selected - %d", method );
    }

    m_RenderLogic.VerifyGLStateQt();
    return enabled;
}

//============================================================================
bool RenderGlShaders::isShaderValid( ESHADERMETHOD method )
{
    bool shaderValid = false;
    if( ( method < SM_MAX ) && m_Shaders[ method ] )
    {
        shaderValid = m_Shaders[ method ]->isShaderValid();
    }
    else
    {
        LogMsg( LOG_ERROR, "Invalid GUI isShaderValid selected - %d", method );
    }

    m_RenderLogic.VerifyGLStateQt();
    return shaderValid;
}

//============================================================================
void RenderGlShaders::disableShader( ESHADERMETHOD method )
{
#ifdef DEBUG_SHADER_ENABLE_DISABLE
    if( method != SM_FONTS )
    {
        LogMsg( LOG_ERROR, "disableShader - %s", describeShaderMethod( method ) );
    }
#endif // DEBUG_SHADER_ENABLE_DISABLE

    if( m_Shaders[ method ] )
    {
        m_Shaders[ method ]->disableShader();
        if( eShaderGui == m_Shaders[ method ]->getShaderType() )
        {
            m_CurShaderMethodType = SM_DEFAULT;
        }
    }

    m_RenderLogic.VerifyGLStateQt();
}

//============================================================================
void RenderGlShaders::disableGUIShader()
{
#ifdef DEBUG_SHADER_ENABLE_DISABLE
    if( m_CurShaderMethodType != SM_FONTS )
    {
        LogMsg( LOG_ERROR, "disableGuiShader - %s", describeShaderMethod( m_CurShaderMethodType ) );
    }
#endif // DEBUG_SHADER_ENABLE_DISABLE


    if( ( m_CurShaderMethodType < SM_MAX ) && m_Shaders[ m_CurShaderMethodType ] )
    {
        m_Shaders[ m_CurShaderMethodType ]->disableShader();
        m_CurShaderMethodType = SM_DEFAULT;
    }

    m_RenderLogic.VerifyGLStateQt();
}

//============================================================================
int RenderGlShaders::shaderGetPos( )
{
    if( m_Shaders[ m_CurShaderMethodType ] )
        return m_Shaders[ m_CurShaderMethodType ]->GetPosLoc();

    return -1;
}

//============================================================================
int RenderGlShaders::shaderGetCol()
{
    if( m_Shaders[ m_CurShaderMethodType ] )
        return m_Shaders[ m_CurShaderMethodType ]->GetColLoc();

    return -1;
}

//============================================================================
int RenderGlShaders::shaderGetModel()
{
    if( m_Shaders[ m_CurShaderMethodType ] )
        return m_Shaders[ m_CurShaderMethodType ]->GetModelLoc();

    return -1;
}

//============================================================================
int RenderGlShaders::shaderGetCoord0()
{
    if( m_Shaders[ m_CurShaderMethodType ] )
        return m_Shaders[ m_CurShaderMethodType ]->GetCord0Loc();

    return -1;
}

//============================================================================
int RenderGlShaders::shaderGetCoord1()
{
    if( m_Shaders[ m_CurShaderMethodType ] )
        return m_Shaders[ m_CurShaderMethodType ]->GetCord1Loc();

    return -1;
}

//============================================================================
int RenderGlShaders::shaderGetUniCol()
{
    if( m_Shaders[ m_CurShaderMethodType ] )
        return m_Shaders[ m_CurShaderMethodType ]->GetUniColLoc();

    return -1;
}

// yuv shader
//============================================================================
void RenderGlShaders::shaderSetField( ESHADERMETHOD shader, int field )
{
    m_RenderLogic.VerifyGLStateQt();
    if( m_Shaders[ shader ] )
    {
        m_Shaders[ shader ]->shaderSetField( field );
    }

    m_RenderLogic.VerifyGLStateQt();
}

//============================================================================
void RenderGlShaders::shaderSetWidth( ESHADERMETHOD shader, int w )
{
    m_RenderLogic.VerifyGLStateQt();
    if( m_Shaders[ shader ] )
    {
        m_Shaders[ shader ]->shaderSetWidth( w );
    }

    m_RenderLogic.VerifyGLStateQt();
}

//============================================================================
void RenderGlShaders::shaderSetHeight( ESHADERMETHOD shader, int h )
{
    m_RenderLogic.VerifyGLStateQt();
    if( m_Shaders[ shader ] )
    {
        m_Shaders[ shader ]->shaderSetHeight( h );
    }

    m_RenderLogic.VerifyGLStateQt();
}

//============================================================================
void RenderGlShaders::shaderSetBlack( ESHADERMETHOD shader, float black )
{
    m_RenderLogic.VerifyGLStateQt();
    if( m_Shaders[ shader ] )
    {
        m_Shaders[ shader ]->shaderSetBlack( black );
    }

    m_RenderLogic.VerifyGLStateQt();
}

//============================================================================
void RenderGlShaders::shaderSetContrast( ESHADERMETHOD shader, float contrast )
{
    m_RenderLogic.VerifyGLStateQt();
    if( m_Shaders[ shader ] )
    {
        m_Shaders[ shader ]->shaderSetContrast( contrast );
    }

    m_RenderLogic.VerifyGLStateQt();
}

//============================================================================
void RenderGlShaders::shaderSetConvertFullColorRange( ESHADERMETHOD shader, bool convertFullRange )
{
    m_RenderLogic.VerifyGLStateQt();
    if( m_Shaders[ shader ] )
    {
        m_Shaders[ shader ]->shaderSetConvertFullColorRange( convertFullRange );
    }

    m_RenderLogic.VerifyGLStateQt();
}

//============================================================================
int RenderGlShaders::shaderGetVertexLoc( ESHADERMETHOD shader )
{
    m_RenderLogic.VerifyGLStateQt();
    int result = false;
    if( m_Shaders[ shader ] )
    {
        result = m_Shaders[ shader ]->shaderGetVertexLoc();
    }

    m_RenderLogic.VerifyGLStateQt();
    return result;
}

//============================================================================
int RenderGlShaders::shaderGetYcoordLoc( ESHADERMETHOD shader )
{
    m_RenderLogic.VerifyGLStateQt();
    int result = false;
    if( m_Shaders[ shader ] )
    {
        result = m_Shaders[ shader ]->shaderGetYcoordLoc();
    }

    m_RenderLogic.VerifyGLStateQt();
    return result;
}

//============================================================================
int RenderGlShaders::shaderGetUcoordLoc( ESHADERMETHOD shader )
{
    m_RenderLogic.VerifyGLStateQt();
    int result = false;
    if( m_Shaders[ shader ] )
    {
        result = m_Shaders[ shader ]->shaderGetUcoordLoc();
    }

    m_RenderLogic.VerifyGLStateQt();
    return result;
}

//============================================================================
int RenderGlShaders::shaderGetVcoordLoc( ESHADERMETHOD shader )
{
    m_RenderLogic.VerifyGLStateQt();
    int result = false;
    if( m_Shaders[ shader ] )
    {
        result = m_Shaders[ shader ]->shaderGetVcoordLoc();
    }

    m_RenderLogic.VerifyGLStateQt();
    return result;
}

//============================================================================
void RenderGlShaders::shaderSetMatrices( ESHADERMETHOD shader, const float *p, const float *m )
{
    if( m_Shaders[ shader ] )
    {
        m_Shaders[ shader ]->shaderSetMatrices( p, m );
    }
}

//============================================================================
void RenderGlShaders::shaderSetAlpha( ESHADERMETHOD shader, float alpha )
{
    if( m_Shaders[ shader ] )
    {
        m_Shaders[ shader ]->shaderSetAlpha( alpha );
    }
}

//============================================================================
void RenderGlShaders::shaderSetFlags( ESHADERMETHOD shader, unsigned int flags )
{
    if( m_Shaders[ shader ] )
    {
        m_Shaders[ shader ]->shaderSetFlags( flags );
    }
}

//============================================================================
void RenderGlShaders::shaderSetFormat( ESHADERMETHOD shader, EShaderFormat format )
{
    if( m_Shaders[ shader ] )
    {
        m_Shaders[ shader ]->shaderSetFormat( format );
    }
}

//============================================================================
void RenderGlShaders::shaderSourceTexture( ESHADERMETHOD shader, int ytex )
{
    if( m_Shaders[ shader ] )
    {
        m_Shaders[ shader ]->shaderSourceTexture( ytex );
    }
}

//============================================================================
void RenderGlShaders::shaderSetStepX( ESHADERMETHOD shader, float stepX )
{
    if( m_Shaders[ shader ] )
    {
        m_Shaders[ shader ]->shaderSetStepX( stepX );
    }
}

//============================================================================
void RenderGlShaders::shaderSetStepY( ESHADERMETHOD shader, float stepY )
{
    if( m_Shaders[ shader ] )
    {
        m_Shaders[ shader ]->shaderSetStepY( stepY );
    }
}

//============================================================================
// filter shader
bool RenderGlShaders::shaderGetTextureFilter( ESHADERMETHOD shader, int& filter )
{
    bool result = false;
    if( m_Shaders[ shader ] )
    {
        result = m_Shaders[ shader ]->shaderGetTextureFilter( filter );
    }

    return result;
}

//============================================================================
int RenderGlShaders::shaderGetcoordLoc( ESHADERMETHOD shader )
{
    m_RenderLogic.VerifyGLStateQt();
    int result = false;
    if( m_Shaders[ shader ] )
    {
        result = m_Shaders[ shader ]->shaderGetVcoordLoc();
    }

    m_RenderLogic.VerifyGLStateQt();
    return result;
}

//============================================================================
int RenderGlShaders::shaderVertexAttribPointer( ESHADERMETHOD shader, unsigned int index, int size, int type, bool normalized, int stride, const void * pointer )
{
    m_RenderLogic.VerifyGLStateQt();
    int result = false;
    if( m_Shaders[ shader ] )
    {
        result = m_Shaders[ shader ]->shaderVertexAttribPointer( index, size, type, normalized, stride, pointer );
    }

    m_RenderLogic.VerifyGLStateQt();
    return result;
}

//============================================================================
void RenderGlShaders::shaderEnableVertexAttribArray( ESHADERMETHOD shader, int arrayId )
{
    m_RenderLogic.VerifyGLStateQt();
    if( m_Shaders[ shader ] )
    {
        m_Shaders[ shader ]->shaderEnableVertexAttribArray( arrayId );
    }

    m_RenderLogic.VerifyGLStateQt();
}

//============================================================================
void RenderGlShaders::shaderDisableVertexAttribArray( ESHADERMETHOD shader, int arrayId )
{
    m_RenderLogic.VerifyGLStateQt();
    if( m_Shaders[ shader ] )
    {
       m_Shaders[ shader ]->shaderDisableVertexAttribArray( arrayId );
    }

    m_RenderLogic.VerifyGLStateQt();
}

//============================================================================
// shader method to string for debugging
const char* RenderGlShaders::describeShaderMethod( ESHADERMETHOD method )
{
    return RenderShaderQt::describeShader( method );
}
