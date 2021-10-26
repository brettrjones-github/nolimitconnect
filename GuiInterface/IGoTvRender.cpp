
#include <CommonHdr.h>

#include "IGoTv.h"
#include <CoreLib/VxDebug.h>
#include <CoreLib/VxThread.h>
#include <kodi_src/xbmc/xbmc/utils/StringUtils.h>
#include <libavutil/log.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavfilter/avfilter.h>


#include "filesystem/Directory.h"
#include "filesystem/SpecialProtocol.h"
#include "utils/log.h"
#include "Application.h"
#include "ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h"
#include "GoTvApps/GoTvCommon/QtSource/AppCommon.h"

#include <CoreLib/VxGlobals.h>
#include <NetLib/VxPeerMgr.h>



//============================================================================
//=== to gui media/render ===//
//============================================================================

//============================================================================
void IGoTv::verifyGlState( const char * msg )
{
    getAppCommon().verifyGlState( msg );
}

//============================================================================
//=== textures ===//
//============================================================================

//============================================================================
void IGoTv::setActiveGlTexture( unsigned int activeTextureNum /* 0 == GL_TEXTURE0 , 1 == GL_TEXTURE1 etc*/ )
{
    getAppCommon().setActiveGlTexture( activeTextureNum );
}

//============================================================================
void IGoTv::createTextureObject( CQtTexture * texture )
{
    getAppCommon().createTextureObject( texture );
}

//============================================================================
void IGoTv::destroyTextureObject( CQtTexture * texture )
{
    getAppCommon().destroyTextureObject( texture );
}

//============================================================================
bool IGoTv::loadToGPU( CQtTexture * texture )
{
   return getAppCommon().loadToGPU( texture );
}

//============================================================================
void IGoTv::bindToUnit( CQtTexture * texture, unsigned int unit )
{
    getAppCommon().bindToUnit( texture, unit );
}

//============================================================================
void IGoTv::beginGuiTexture( CGUITextureQt * guiTexture, GoTvColor color )
{
    getAppCommon().beginGuiTexture( guiTexture, color );
}

//============================================================================
void IGoTv::drawGuiTexture( CGUITextureQt * guiTexture, float * x, float * y, float * z, const GoTvRect& textureRect, const GoTvRect& diffuse, int orientation )
{
    getAppCommon().drawGuiTexture( guiTexture, x, y, z, textureRect, diffuse, orientation );
}

//============================================================================
void IGoTv::endGuiTexture( CGUITextureQt * guiTexture  )
{
    getAppCommon().endGuiTexture( guiTexture );
}

//============================================================================
void IGoTv::drawQuad( const GoTvRect& rect, GoTvColor color, CBaseTexture * texture, const GoTvRect * texCoords )
{
    getAppCommon().drawQuad( rect, color, texture, texCoords );
}

//============================================================================
bool IGoTv::firstBegin( CGUIFontTTFQt * font )
{
    return getAppCommon().firstBegin( font );
}

//============================================================================
void IGoTv::lastEnd( CGUIFontTTFQt * font )
{
    getAppCommon().lastEnd( font );
}

#if ENABLE_KODI
//============================================================================
CVertexBuffer IGoTv::createVertexBuffer( CGUIFontTTFQt * font, const std::vector<SVertex>& vertices )
{
    return getAppCommon().createVertexBuffer( font, vertices );
}
#endif // ENABLE_KODI

//============================================================================
void IGoTv::destroyVertexBuffer( CGUIFontTTFQt * font, CVertexBuffer & vertBuffer )
{
    getAppCommon().destroyVertexBuffer( font, vertBuffer );
}

//============================================================================
void IGoTv::deleteHardwareTexture( CGUIFontTTFQt * font )
{
    getAppCommon().deleteHardwareTexture( font );
}

//============================================================================
void IGoTv::createStaticVertexBuffers( CGUIFontTTFQt * font )
{
    getAppCommon().createStaticVertexBuffers( font );
}

//============================================================================
void IGoTv::destroyStaticVertexBuffers( CGUIFontTTFQt * font )
{
    getAppCommon().destroyStaticVertexBuffers( font );
}


//============================================================================
//=== shader ===//
//============================================================================

//============================================================================
int IGoTv::shaderGetPos()
{
    return  getAppCommon().shaderGetPos();
}

//============================================================================
int IGoTv::shaderGetCol()
{
    return  getAppCommon().shaderGetCol();
}

//============================================================================
int IGoTv::shaderGetModel()
{
    return  getAppCommon().shaderGetModel();
}

//============================================================================
int IGoTv::shaderGetCoord0()
{
    return  getAppCommon().shaderGetCoord0();
}

//============================================================================
int IGoTv::shaderGetCoord1()
{
    return  getAppCommon().shaderGetCoord1();
}

//============================================================================
int IGoTv::shaderGetUniCol()
{
    return  getAppCommon().shaderGetUniCol();
}

//============================================================================
//=== remder ===//
//============================================================================

//============================================================================
void IGoTv::captureScreen( CScreenshotSurface * screenCaptrue, GoTvRect& captureArea )
{
    getAppCommon().captureScreen( screenCaptrue, captureArea );
}

//============================================================================
void IGoTv::toGuiRenderVideoFrame( int textureIdx, CRenderBuffer* videoBuffer )
{
    getAppCommon().toGuiRenderVideoFrame( textureIdx, videoBuffer );
}

//============================================================================
bool IGoTv::initRenderSystem()
{
    return getAppCommon().initRenderSystem( );
}

//============================================================================
bool IGoTv::destroyRenderSystem()
{
    return getAppCommon().destroyRenderSystem();
}

//============================================================================
bool IGoTv::resetRenderSystem( int width, int height )
{
    return getAppCommon().resetRenderSystem( width, height );
}

//============================================================================
int IGoTv::getMaxTextureSize( )
{
    return getAppCommon().getMaxTextureSize( );
}

//============================================================================
bool IGoTv::beginRender()
{
    return getAppCommon().beginRender();
}

//============================================================================
bool IGoTv::endRender()
{
    return getAppCommon().endRender();
}

//============================================================================
void IGoTv::presentRender( bool rendered, bool videoLayer )
{
    getAppCommon().presentRender( rendered, videoLayer );
}

//============================================================================
bool IGoTv::clearBuffers( GoTvColor color )
{
    return getAppCommon().clearBuffers( color );
}

//============================================================================
bool IGoTv::isExtSupported( const char* extension ) 
{
    return getAppCommon().isExtSupported( extension );
}

//============================================================================
void IGoTv::setVSync( bool vsync )
{
    getAppCommon().setVSync( vsync );
}

//============================================================================
void IGoTv::setViewPort( const GoTvRect& viewPort )
{
    getAppCommon().setViewPort( viewPort );
}

//============================================================================
void IGoTv::getViewPort( GoTvRect& viewPort )
{
    getAppCommon().getViewPort( viewPort );
}

//============================================================================
bool IGoTv::scissorsCanEffectClipping()
{
    return getAppCommon().scissorsCanEffectClipping();
}

//============================================================================
GoTvRect IGoTv::clipRectToScissorRect( const GoTvRect &rect )
{
    return getAppCommon().clipRectToScissorRect( rect );
}

//============================================================================
void IGoTv::setScissors( const GoTvRect& rect )
{
    getAppCommon().setScissors( rect );
}

//============================================================================
void IGoTv::resetScissors()
{
    getAppCommon().resetScissors();
}

//============================================================================
void IGoTv::captureStateBlock()
{
    getAppCommon().captureStateBlock();
}

//============================================================================
void IGoTv::applyStateBlock()
{
    getAppCommon().applyStateBlock();
}

//============================================================================
void IGoTv::setCameraPosition( const GoTvPoint& camera, int screenWidth, int screenHeight, float stereoFactor )
{
    getAppCommon().setCameraPosition( camera, screenWidth, screenHeight, stereoFactor );
}

//============================================================================
void IGoTv::applyHardwareTransform( const TransformMatrix &matrix )
{
    getAppCommon().applyHardwareTransform( matrix );
}

//============================================================================
void IGoTv::restoreHardwareTransform()
{
    getAppCommon().restoreHardwareTransform();
}

//============================================================================
bool IGoTv::testRender()
{
    return getAppCommon().testRender();
}

//============================================================================
void IGoTv::project( float &x, float &y, float &z )
{
    getAppCommon().project( x, y, z );
}

//============================================================================
void IGoTv::initialiseShaders()
{
    getAppCommon().initialiseShaders();
}

//============================================================================
void IGoTv::releaseShaders()
{
    getAppCommon().releaseShaders();
}

//============================================================================
bool IGoTv::enableShader( ESHADERMETHOD method )
{
    return getAppCommon().enableShader( method );
}

//============================================================================
bool IGoTv::isShaderValid( ESHADERMETHOD method )
{
    return getAppCommon().isShaderValid( method );
}

//============================================================================
void IGoTv::disableGUIShader()
{
    getAppCommon().disableGUIShader();
}

//============================================================================
void IGoTv::disableShader( ESHADERMETHOD method )
{
    getAppCommon().disableShader( method );
}

// yuv shader
//============================================================================
void IGoTv::shaderSetField( ESHADERMETHOD shader, int field )
{
    getAppCommon().shaderSetField( shader, field );
}

//============================================================================
void IGoTv::shaderSetWidth( ESHADERMETHOD shader, int w )
{
    getAppCommon().shaderSetWidth( shader, w );
}

//============================================================================
void IGoTv::shaderSetHeight( ESHADERMETHOD shader, int h )
{
    getAppCommon().shaderSetHeight( shader, h );
}

//============================================================================
void IGoTv::shaderSetBlack( ESHADERMETHOD shader, float black )
{
    getAppCommon().shaderSetBlack( shader, black );
}

//============================================================================
void IGoTv::shaderSetContrast( ESHADERMETHOD shader, float contrast )
{
    getAppCommon().shaderSetContrast( shader, contrast );
}

//============================================================================
void IGoTv::shaderSetConvertFullColorRange( ESHADERMETHOD shader, bool convertFullRange )
{
    getAppCommon().shaderSetConvertFullColorRange( shader, convertFullRange );
}

//============================================================================
int IGoTv::shaderGetVertexLoc( ESHADERMETHOD shader )
{
    return getAppCommon().shaderGetVertexLoc( shader );
}

//============================================================================
int IGoTv::shaderGetYcoordLoc( ESHADERMETHOD shader )
{
    return getAppCommon().shaderGetYcoordLoc( shader );
}

//============================================================================
int IGoTv::shaderGetUcoordLoc( ESHADERMETHOD shader )
{
    return getAppCommon().shaderGetUcoordLoc( shader );
}

//============================================================================
int IGoTv::shaderGetVcoordLoc( ESHADERMETHOD shader )
{
    return getAppCommon().shaderGetVcoordLoc( shader );
}

//============================================================================
void IGoTv::shaderSetMatrices( ESHADERMETHOD shader, const float *p, const float *m )
{
    getAppCommon().shaderSetMatrices( shader, p, m );
}

//============================================================================
void IGoTv::shaderSetAlpha( ESHADERMETHOD shader, float alpha )
{
    getAppCommon().shaderSetAlpha( shader, alpha );
}

//============================================================================
void IGoTv::shaderSetFlags( ESHADERMETHOD shader, unsigned int flags )
{
    getAppCommon().shaderSetFlags( shader, flags );
}

//============================================================================
void IGoTv::shaderSetFormat( ESHADERMETHOD shader, EShaderFormat format )
{
    getAppCommon().shaderSetFormat( shader, format );
}

//============================================================================
void IGoTv::shaderSourceTexture( ESHADERMETHOD shader, int ytex )
{
    getAppCommon().shaderSourceTexture( shader, ytex );
}

//============================================================================
void IGoTv::shaderSetStepX( ESHADERMETHOD shader, float stepX )
{
    getAppCommon().shaderSetStepX( shader, stepX );
}

//============================================================================
void IGoTv::shaderSetStepY( ESHADERMETHOD shader, float stepY )
{
    getAppCommon().shaderSetStepY( shader, stepY );
}

//============================================================================
// filter shader
bool IGoTv::shaderGetTextureFilter( ESHADERMETHOD shader, int& filter )
{
    return getAppCommon().shaderGetTextureFilter( shader, filter );
}

//============================================================================
int IGoTv::shaderGetcoordLoc( ESHADERMETHOD shader )
{
    return getAppCommon().shaderGetcoordLoc( shader );
}

//============================================================================
int IGoTv::shaderVertexAttribPointer( ESHADERMETHOD shader, unsigned int index, int size, int type, bool normalized, int stride, const void * pointer )
{
    return getAppCommon().shaderVertexAttribPointer( shader, index, size, type, normalized, stride, pointer );
}

//============================================================================
void IGoTv::shaderEnableVertexAttribArray( ESHADERMETHOD shader, int arrayId )
{
    getAppCommon().shaderEnableVertexAttribArray( shader, arrayId );
}

//============================================================================
void IGoTv::shaderDisableVertexAttribArray( ESHADERMETHOD shader, int arrayId )
{
    getAppCommon().shaderDisableVertexAttribArray( shader, arrayId );
}


// frame buffers
//============================================================================
void IGoTv::frameBufferGen( int bufCount, unsigned int* fboId )
{
    getAppCommon().frameBufferGen( bufCount, fboId );
}

//============================================================================
void IGoTv::frameBufferDelete( int bufCount, unsigned int* fboId )
{
    getAppCommon().frameBufferDelete( bufCount, fboId );
}

//============================================================================
void IGoTv::frameBufferTexture2D( int target, unsigned int texureId )
{
    getAppCommon().frameBufferTexture2D( target, texureId );
}

//============================================================================
void IGoTv::frameBufferBind( unsigned int fboId )
{
    getAppCommon().frameBufferBind( fboId );
}

//============================================================================
bool IGoTv::frameBufferStatus()
{
    return getAppCommon().frameBufferStatus();
}

//============================================================================
void IGoTv::glFuncDrawElements( GLenum mode, GLsizei count, GLenum type, const GLvoid *indices )
{
    getAppCommon().glFuncDrawElements( mode, count, type, indices );
}

//============================================================================
void IGoTv::glFuncDisable( GLenum cap )
{
    getAppCommon().glFuncDisable( cap );
}

//============================================================================
void IGoTv::glFuncBindTexture( GLenum target, GLuint texture )
{
    getAppCommon().glFuncBindTexture( target, texture );
}

//============================================================================
void IGoTv::glFuncViewport( GLint x, GLint y, GLsizei width, GLsizei height )
{
    getAppCommon().glFuncViewport( x, y, width, height );
}

//============================================================================
void IGoTv::glFuncScissor( GLint x, GLint y, GLsizei width, GLsizei height )
{
    getAppCommon().glFuncScissor( x, y, width, height );
}

//============================================================================
void IGoTv::glFuncGenTextures( GLsizei n, GLuint * textures )
{
    getAppCommon().glFuncGenTextures( n, textures );
}

//============================================================================
void IGoTv::glFuncDeleteTextures( GLsizei n, const GLuint *textures )
{
    getAppCommon().glFuncDeleteTextures( n, textures );
}

//============================================================================
void IGoTv::glFuncTexImage2D( GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels )
{
    getAppCommon().glFuncTexImage2D( target, level, internalformat, width, height, border, format, type, pixels );
}

//============================================================================
void IGoTv::glFuncTexParameteri( GLenum target, GLenum pname, GLint param )
{
    getAppCommon().glFuncTexParameteri( target, pname, param );
}

//============================================================================
void IGoTv::glFuncReadPixels( GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels )
{
    getAppCommon().glFuncReadPixels( x, y, width, height, format, type, pixels );
}

//============================================================================
void IGoTv::glFuncPixelStorei( GLenum pname, GLint param )
{
    getAppCommon().glFuncPixelStorei( pname, param );
}

//============================================================================
void IGoTv::glFuncFinish()
{
    getAppCommon().glFuncFinish();
}

//============================================================================
void IGoTv::glFuncEnable( GLenum cap )
{
    getAppCommon().glFuncEnable( cap );
}

//============================================================================
void IGoTv:: glFuncTexSubImage2D( GLenum target, GLint level,
                                      GLint xoffset, GLint yoffset,
                                      GLsizei width, GLsizei height,
                                      GLenum format, GLenum type,
                                      const GLvoid *pixels )
{
    getAppCommon().glFuncTexSubImage2D( target, level,
                                        xoffset, yoffset,
                                        width, height,
                                        format, type,
                                        pixels );
}

//============================================================================
void IGoTv::glFuncBlendFunc( GLenum sfactor, GLenum dfactor )
{
    getAppCommon().glFuncBlendFunc( sfactor, dfactor );
}

//============================================================================
void IGoTv::glFuncVertexAttribPointer( GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer )
{
    getAppCommon().glFuncVertexAttribPointer( index, size, type, normalized, stride, pointer );
}

//============================================================================
void IGoTv::glFuncDisableVertexAttribArray( GLuint index )
{
    getAppCommon().glFuncDisableVertexAttribArray( index );
}

//============================================================================
void IGoTv::glFuncEnableVertexAttribArray( GLuint index )
{
    getAppCommon().glFuncEnableVertexAttribArray( index );
}

//============================================================================
void IGoTv::glFuncDrawArrays( GLenum mode, GLint first, GLsizei count )
{
    getAppCommon().glFuncDrawArrays( mode, first, count );
}
