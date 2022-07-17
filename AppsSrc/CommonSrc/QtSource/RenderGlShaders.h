#ifndef RENDERGLSHADERS_H
#define RENDERGLSHADERS_H
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

#include "RenderShaderQt.h"
#include "EventsQtToNlc.h"
#include "GuiInterface/INlcRender.h"
#include "GuiInterface/INlcEvents.h"

#include <QtGui/qopenglfunctions.h>
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
#include <QtGui/qopenglshaderprogram.h>
#endif // QT_VERSION < QT_VERSION_CHECK(6,0,0)

class RenderGlWidget;

class RenderGlShaders
{
public:

    RenderGlShaders( RenderGlLogic& renderGlLogic );
    virtual ~RenderGlShaders();

    // must be called from gui thread (required for linux)
    void initShaders( QOpenGLFunctions * glf );

    // cleanup shaders
    void destroyShaders();

    // shader method to string for debugging
    const char *                describeShaderMethod( ESHADERMETHOD method );

    //virtual void                initialiseShaders();
    //virtual void                releaseShaders();

    bool                        enableShader( ESHADERMETHOD method );
    bool                        isShaderValid( ESHADERMETHOD method );
    void                        disableShader( ESHADERMETHOD method );
    void                        disableGUIShader();

    int                         shaderGetPos();
    int                         shaderGetCol();
    int                         shaderGetModel();
    int                         shaderGetCoord0();
    int                         shaderGetCoord1();
    int                         shaderGetUniCol();

    // yuv shader
    virtual void                shaderSetField( ESHADERMETHOD shader, int field );
    virtual void                shaderSetWidth( ESHADERMETHOD shader, int w );
    virtual void                shaderSetHeight( ESHADERMETHOD shader, int h );

    virtual void                shaderSetBlack( ESHADERMETHOD shader, float black );
    virtual void                shaderSetContrast( ESHADERMETHOD shader, float contrast );
    virtual void                shaderSetConvertFullColorRange( ESHADERMETHOD shader, bool convertFullRange );

    virtual int                 shaderGetVertexLoc( ESHADERMETHOD shader );
    virtual int                 shaderGetYcoordLoc( ESHADERMETHOD shader );
    virtual int                 shaderGetUcoordLoc( ESHADERMETHOD shader );
    virtual int                 shaderGetVcoordLoc( ESHADERMETHOD shader );

    virtual void                shaderSetMatrices( ESHADERMETHOD shader, const float *p, const float *m );
    virtual void                shaderSetAlpha( ESHADERMETHOD shader, float alpha );

    virtual void                shaderSetFlags( ESHADERMETHOD shader, unsigned int flags );
    virtual void                shaderSetFormat( ESHADERMETHOD shader, EShaderFormat format );
    virtual void                shaderSourceTexture( ESHADERMETHOD shader, int ytex );
    virtual void                shaderSetStepX( ESHADERMETHOD shader, float stepX );
    virtual void                shaderSetStepY( ESHADERMETHOD shader, float stepY ) ;

    // filter shader
    virtual bool                shaderGetTextureFilter( ESHADERMETHOD shader, int& filter );
    virtual int                 shaderGetcoordLoc( ESHADERMETHOD shader );

    // renderqt
    virtual int                 shaderVertexAttribPointer( ESHADERMETHOD shader, unsigned int index, int size, int type, bool normalized, int stride, const void * pointer );
    virtual void                shaderEnableVertexAttribArray( ESHADERMETHOD shader, int arrayId );
    virtual void                shaderDisableVertexAttribArray( ESHADERMETHOD shader, int arrayId );

protected:
    void                        getShaderSourceCode( int shaderIdx, EShaderType& shaderType, std::string& shaderName, std::string& vertexShaderCode, std::string& fragmentShaderCode );
    void                        compileShader( int shaderIdx );

    RenderGlLogic&              m_RenderLogic;
    // shaders
    QOpenGLFunctions *          m_Glf = nullptr;
    RenderShaderQt *            m_Shaders[ SM_MAX ];
    bool                        m_ShadersInited = false;
    ESHADERMETHOD               m_CurShaderMethodType = SM_DEFAULT;
};

#endif // RENDERGLSHADERS_H
