
#include <app_precompiled_hdr.h>

#include "RenderGlWidget.h"

//============================================================================
bool RenderGlWidget::enableShader( ESHADERMETHOD method )
{
    return m_RenderGlShaders.enableShader( method );
}

//============================================================================
bool RenderGlWidget::isShaderValid( ESHADERMETHOD method )
{
    return m_RenderGlShaders.isShaderValid( method );
}

//============================================================================
void RenderGlWidget::disableShader( ESHADERMETHOD method )
{
    m_RenderGlShaders.disableShader( method );
}

//============================================================================
void RenderGlWidget::disableGUIShader()
{
    m_RenderGlShaders.disableGUIShader();
}

//============================================================================
int RenderGlWidget::shaderGetPos( )
{
    return m_RenderGlShaders.shaderGetPos();
}

//============================================================================
int RenderGlWidget::shaderGetCol()
{
    return m_RenderGlShaders.shaderGetCol();
}

//============================================================================
int RenderGlWidget::shaderGetModel()
{
    return m_RenderGlShaders.shaderGetModel();
}

//============================================================================
int RenderGlWidget::shaderGetCoord0()
{
    return m_RenderGlShaders.shaderGetCoord0();
}

//============================================================================
int RenderGlWidget::shaderGetCoord1()
{
    return m_RenderGlShaders.shaderGetCoord1();
}

//============================================================================
int RenderGlWidget::shaderGetUniCol()
{
    return m_RenderGlShaders.shaderGetUniCol();
}

// yuv shader
//============================================================================
void RenderGlWidget::shaderSetField( ESHADERMETHOD shader, int field )
{
    m_RenderGlShaders.shaderSetField( shader, field );
}

//============================================================================
void RenderGlWidget::shaderSetWidth( ESHADERMETHOD shader, int w )
{
    m_RenderGlShaders.shaderSetWidth( shader, w );
}

//============================================================================
void RenderGlWidget::shaderSetHeight( ESHADERMETHOD shader, int h )
{
    m_RenderGlShaders.shaderSetHeight( shader, h );
}

//============================================================================
void RenderGlWidget::shaderSetBlack( ESHADERMETHOD shader, float black )
{
    m_RenderGlShaders.shaderSetBlack( shader, black );
}

//============================================================================
void RenderGlWidget::shaderSetContrast( ESHADERMETHOD shader, float contrast )
{
    m_RenderGlShaders.shaderSetContrast( shader, contrast );
}

//============================================================================
void RenderGlWidget::shaderSetConvertFullColorRange( ESHADERMETHOD shader, bool convertFullRange )
{
    m_RenderGlShaders.shaderSetConvertFullColorRange( shader, convertFullRange );
}

//============================================================================
int RenderGlWidget::shaderGetVertexLoc( ESHADERMETHOD shader )
{
    return m_RenderGlShaders.shaderGetVertexLoc( shader );
}

//============================================================================
int RenderGlWidget::shaderGetYcoordLoc( ESHADERMETHOD shader )
{
    return m_RenderGlShaders.shaderGetYcoordLoc( shader );
}

//============================================================================
int RenderGlWidget::shaderGetUcoordLoc( ESHADERMETHOD shader )
{
    return m_RenderGlShaders.shaderGetUcoordLoc( shader );
}

//============================================================================
int RenderGlWidget::shaderGetVcoordLoc( ESHADERMETHOD shader )
{
    return m_RenderGlShaders.shaderGetVcoordLoc( shader );
}

//============================================================================
void RenderGlWidget::shaderSetMatrices( ESHADERMETHOD shader, const float *p, const float *m )
{
    m_RenderGlShaders.shaderSetMatrices( shader, p, m );
}

//============================================================================
void RenderGlWidget::shaderSetAlpha( ESHADERMETHOD shader, float alpha )
{
    m_RenderGlShaders.shaderSetAlpha( shader, alpha );
}

//============================================================================
void RenderGlWidget::shaderSetFlags( ESHADERMETHOD shader, unsigned int flags )
{
    m_RenderGlShaders.shaderSetFlags( shader, flags );
}

//============================================================================
void RenderGlWidget::shaderSetFormat( ESHADERMETHOD shader, EShaderFormat format )
{
    m_RenderGlShaders.shaderSetFormat( shader, format );
}

//============================================================================
void RenderGlWidget::shaderSourceTexture( ESHADERMETHOD shader, int ytex )
{
    m_RenderGlShaders.shaderSourceTexture( shader, ytex );
}

//============================================================================
void RenderGlWidget::shaderSetStepX( ESHADERMETHOD shader, float stepX )
{
    m_RenderGlShaders.shaderSetStepX( shader, stepX );
}

//============================================================================
void RenderGlWidget::shaderSetStepY( ESHADERMETHOD shader, float stepY )
{
    m_RenderGlShaders.shaderSetStepY( shader, stepY );
}

//============================================================================
// filter shader
bool RenderGlWidget::shaderGetTextureFilter( ESHADERMETHOD shader, int& filter )
{
    return m_RenderGlShaders.shaderGetTextureFilter( shader, filter );
}

//============================================================================
int RenderGlWidget::shaderGetcoordLoc( ESHADERMETHOD shader )
{
    return m_RenderGlShaders.shaderGetVcoordLoc( shader );
}

//============================================================================
int RenderGlWidget::shaderVertexAttribPointer( ESHADERMETHOD shader, unsigned int index, int size, int type, bool normalized, int stride, const void * pointer )
{
    return m_RenderGlShaders.shaderVertexAttribPointer( shader, index, size, type, normalized, stride, pointer );
}

//============================================================================
void RenderGlWidget::shaderEnableVertexAttribArray( ESHADERMETHOD shader, int arrayId )
{
    m_RenderGlShaders.shaderEnableVertexAttribArray( shader, arrayId );
}

//============================================================================
void RenderGlWidget::shaderDisableVertexAttribArray( ESHADERMETHOD shader, int arrayId )
{
    m_RenderGlShaders.shaderDisableVertexAttribArray( shader, arrayId );
}

//============================================================================
//! ignore from kodi
void RenderGlWidget::initialiseShaders()
{
}

//============================================================================
//! ignore from kodi
void RenderGlWidget::releaseShaders()
{
}
