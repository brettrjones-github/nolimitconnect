#pragma once
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

#include "IDefs.h"
#include "INlcGeometry.h"
#include "utils/TransformMatrix.h"
#include "rendering/RenderSystemTypes.h"
#include "guilib/GUIFont.h"
#include "guilib/GUIFontTTF.h"
#include "cores/VideoPlayer/VideoRenderers/VideoShaders/ShaderFormats.h"
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

//#define  ENABLE_GLES_SHADERS // uncomment to enable all shaders

enum ESHADERMETHOD
{
    SM_DEFAULT,
    SM_TEXTURE,
    SM_MULTI,
    SM_FONTS,
    SM_TEXTURE_NOBLEND,
    SM_MULTI_BLENDCOLOR,

    SM_VIDEO_YV12_BASIC,
    SM_VIDEO_NV12_BASIC,
    SM_VIDEO_YUY2_BASIC,

    SM_VIDEO_UYVY_BASIC,
    SM_VIDEO_NV12_RGB_BASIC,

    SM_VID_FILTER_DEFAULT,
    SM_VID_FILTER_CONVOLUTION_4X4_RGBA,
    SM_VID_FILTER_CONVOLUTION_4X4_FLOAT,
    SM_VID_FILTER_CONVOLUTION_6X6_RGBA,
    SM_VID_FILTER_CONVOLUTION_6X6_FLOAT,

#ifdef ENABLE_GLES_SHADERS
    SM_TEXTURE_RGBA,
    SM_TEXTURE_RGBA_OES,
    SM_TEXTURE_RGBA_BLENDCOLOR,
    SM_TEXTURE_RGBA_BOB,
    SM_TEXTURE_RGBA_BOB_OES,

    SM_VIDEO_YV12_BOB,
    SM_VIDEO_NV12_BOB,
    SM_VIDEO_YUY2_BOB,
    SM_VIDEO_UYVY_BOB,
    SM_VIDEO_NV12_RGB_BOB,
#endif // ENABLE_GLES_SHADERS


    SM_SHADER_NONE,

    SM_MAX
};

typedef uint32_t NlcColor;

template <typename T> class NlcRectGen;
typedef NlcRectGen<float>  NlcRect;

class CQtTexture;
class CGUITextureQt;
class CBaseTexture;
class CGUIFontTTFQt;
class CScreenshotSurface;

class INlcRender
{
public:
    virtual void                verifyGlState( const char* msg = nullptr ) = 0; // show gl error if any

    //=== textures ===//
    virtual void                setActiveGlTexture( unsigned int activeTextureNum /* 0 == GL_TEXTURE0 , 1 == GL_TEXTURE1 etc*/ )  = 0;

    virtual void                createTextureObject( CQtTexture * texture ) = 0;
    virtual void                destroyTextureObject( CQtTexture * texture ) = 0;
    virtual bool                loadToGPU( CQtTexture * texture ) = 0;
    virtual void                bindToUnit(  CQtTexture * texture, unsigned int unit ) = 0;

    virtual void                beginGuiTexture( CGUITextureQt * guiTexture, NlcColor color ) = 0;
    virtual void                drawGuiTexture( CGUITextureQt * textrue, float * x, float * y, float * z, const NlcRect& texture, const NlcRect& diffuse, int orientation ) = 0;
    virtual void                endGuiTexture( CGUITextureQt * guiTexture ) = 0;

    virtual void                drawQuad( const NlcRect &rect, NlcColor color, CBaseTexture * texture, const NlcRect * texCoords ) = 0;

    virtual bool                firstBegin( CGUIFontTTFQt * font ) = 0;
    virtual void                lastEnd( CGUIFontTTFQt * font ) = 0;
#if ENABLE_KODI
    virtual CVertexBuffer       createVertexBuffer( CGUIFontTTFQt * font, const std::vector<SVertex>& vertices) = 0;
#endif // ENABLE_KODI
    virtual void                destroyVertexBuffer( CGUIFontTTFQt * font, CVertexBuffer& vertBuffer ) = 0;

    virtual void                deleteHardwareTexture( CGUIFontTTFQt * font ) = 0;
    virtual void                createStaticVertexBuffers( CGUIFontTTFQt * font ) = 0;
    virtual void                destroyStaticVertexBuffers( CGUIFontTTFQt * font ) = 0;

    //=== remder ===//
    virtual void                captureScreen( CScreenshotSurface * screenCaptrue, NlcRect& captureArea ) = 0;

    virtual bool                initRenderSystem() = 0;
    virtual bool                destroyRenderSystem() = 0;
    virtual bool                resetRenderSystem( int width, int height ) = 0;

    virtual int                 getMaxTextureSize() = 0;

    virtual bool                beginRender() = 0;
    virtual bool                endRender() = 0;
    virtual void                presentRender( bool rendered, bool videoLayer ) = 0;
    virtual bool                clearBuffers( NlcColor color ) = 0;
    virtual bool                isExtSupported( const char* extension ) = 0;

    virtual void                setVSync( bool vsync ) {}
    virtual void                resetVSync() {}

    virtual void                setViewPort( const NlcRect& viewPort ) = 0;
    virtual void                getViewPort( NlcRect& viewPort ) = 0;

    virtual bool                scissorsCanEffectClipping() = 0;
    virtual NlcRect            clipRectToScissorRect( const NlcRect &rect ) = 0;
    virtual void                setScissors( const NlcRect& rect ) = 0;
    virtual void                resetScissors() = 0;

    virtual void                captureStateBlock() = 0;
    virtual void                applyStateBlock() = 0;

    virtual void                setCameraPosition( const NlcPoint &camera, int screenWidth, int screenHeight, float stereoFactor = 0.0f ) = 0;

    virtual void                applyHardwareTransform( const TransformMatrix &matrix ) = 0;
    virtual void                restoreHardwareTransform() = 0;
    virtual bool                supportsStereo( RENDER_STEREO_MODE mode ) const { return false; }

    virtual bool                testRender() = 0;

    virtual void                project( float &x, float &y, float &z ) = 0;



    virtual std::string         getShaderPath( const std::string &filename ) { return ""; }

    virtual void                initialiseShaders() = 0;
    virtual void                releaseShaders() = 0;
    virtual bool                enableShader( ESHADERMETHOD method ) = 0;
    virtual bool                isShaderValid( ESHADERMETHOD method ) = 0;
    virtual void                disableShader( ESHADERMETHOD method ) = 0;
    virtual void                disableGUIShader() = 0;

    virtual int                 shaderGetPos( ) = 0;
    virtual int                 shaderGetCol( )   = 0;
    virtual int                 shaderGetModel( )   = 0;
    virtual int                 shaderGetCoord0( ) = 0;
    virtual int                 shaderGetCoord1( ) = 0;
    virtual int                 shaderGetUniCol( ) = 0;

    // yuv shader
    virtual void                shaderSetField( ESHADERMETHOD shader, int field ) = 0;
    virtual void                shaderSetWidth( ESHADERMETHOD shader, int w ) = 0;
    virtual void                shaderSetHeight( ESHADERMETHOD shader, int h ) = 0;

    virtual void                shaderSetBlack( ESHADERMETHOD shader, float black ) = 0;
    virtual void                shaderSetContrast( ESHADERMETHOD shader, float contrast ) = 0;
    virtual void                shaderSetConvertFullColorRange( ESHADERMETHOD shader, bool convertFullRange ) = 0;

    virtual int                 shaderGetVertexLoc( ESHADERMETHOD shader ) = 0;
    virtual int                 shaderGetYcoordLoc( ESHADERMETHOD shader ) = 0;
    virtual int                 shaderGetUcoordLoc( ESHADERMETHOD shader ) = 0;
    virtual int                 shaderGetVcoordLoc( ESHADERMETHOD shader ) = 0;

    virtual void                shaderSetMatrices( ESHADERMETHOD shader, const float *p, const float *m ) = 0;
    virtual void                shaderSetAlpha( ESHADERMETHOD shader, float alpha ) = 0;

    virtual void                shaderSetFlags( ESHADERMETHOD shader, unsigned int flags ) = 0;
    virtual void                shaderSetFormat( ESHADERMETHOD shader, EShaderFormat format ) = 0;
    virtual void                shaderSourceTexture( ESHADERMETHOD shader, int ytex ) = 0;
    virtual void                shaderSetStepX( ESHADERMETHOD shader, float stepX ) = 0;
    virtual void                shaderSetStepY( ESHADERMETHOD shader, float stepY ) = 0;


    // filter shader
    virtual bool                shaderGetTextureFilter( ESHADERMETHOD shader, int& filter ) = 0;
    virtual int                 shaderGetcoordLoc( ESHADERMETHOD shader ) = 0;
 
    // renderqt
    virtual int                 shaderVertexAttribPointer( ESHADERMETHOD shader, unsigned int index, int size, int type, bool normalized, int stride, const void * pointer ) = 0;
    virtual void                shaderEnableVertexAttribArray( ESHADERMETHOD shader, int arrayId ) = 0;
    virtual void                shaderDisableVertexAttribArray( ESHADERMETHOD shader, int arrayId ) = 0;

    // frame buffers
    virtual void                frameBufferGen( int bufCount, unsigned int* fboId ) = 0;
    virtual void                frameBufferDelete( int bufCount, unsigned int* fboId ) = 0;
    virtual void                frameBufferTexture2D( int target, unsigned int texureId ) = 0;
    virtual void                frameBufferBind( unsigned int fboId ) = 0;
    virtual bool                frameBufferStatus() = 0; // return true when frame buffer completed

    // gl functions
    virtual void                glFuncDrawElements( GLenum mode, GLsizei count, GLenum type, const GLvoid *indices ) = 0;
    virtual void                glFuncDisable( GLenum cap ) = 0;
    virtual void                glFuncBindTexture( GLenum target, GLuint texture ) = 0;
    virtual void                glFuncViewport( GLint x, GLint y, GLsizei width, GLsizei height ) = 0;
    virtual void                glFuncScissor( GLint x, GLint y, GLsizei width, GLsizei height ) = 0;

    virtual void                glFuncGenTextures( GLsizei n, GLuint * textures ) = 0;
    virtual void                glFuncDeleteTextures( GLsizei n, const GLuint *textures ) = 0;
    virtual void                glFuncTexImage2D( GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels ) = 0;
    virtual void                glFuncTexParameteri( GLenum target, GLenum pname, GLint param ) = 0;
    virtual void                glFuncReadPixels( GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels ) = 0;
    virtual void                glFuncPixelStorei( GLenum pname, GLint param ) = 0;
    virtual void                glFuncFinish() = 0;

    virtual void                glFuncEnable( GLenum cap ) = 0;
    virtual void                glFuncTexSubImage2D( GLenum target, GLint level,
                                                       GLint xoffset, GLint yoffset,
                                                       GLsizei width, GLsizei height,
                                                       GLenum format, GLenum type,
                                                       const GLvoid *pixels ) = 0;
    virtual void                glFuncBlendFunc( GLenum sfactor, GLenum dfactor ) = 0;
    virtual void                glFuncVertexAttribPointer( GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer ) = 0;
    virtual void                glFuncDisableVertexAttribArray( GLuint index ) = 0;
    virtual void                glFuncEnableVertexAttribArray( GLuint index ) = 0;
    virtual void                glFuncDrawArrays( GLenum mode, GLint first, GLsizei count ) = 0;
};
