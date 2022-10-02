
#include <CommonHdr.h>

#include "INlc.h"
#include <CoreLib/VxDebug.h>
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
#include "AppsSrc/CommonSrc/QtSource/AppCommon.h"

#include <CoreLib/VxGlobals.h>
#include <NetLib/VxPeerMgr.h>



//============================================================================
//=== to gui media/render ===//
//============================================================================

//============================================================================
void INlc::verifyGlState( const char* msg )
{
    getAppCommon().verifyGlState( msg );
}

//============================================================================
//=== textures ===//
//============================================================================

//============================================================================
void INlc::setActiveGlTexture( unsigned int activeTextureNum /* 0 == GL_TEXTURE0 , 1 == GL_TEXTURE1 etc*/ )
{
    getAppCommon().setActiveGlTexture( activeTextureNum );
}

//============================================================================
void INlc::createTextureObject( CQtTexture * texture )
{
    getAppCommon().createTextureObject( texture );
}

//============================================================================
void INlc::destroyTextureObject( CQtTexture * texture )
{
    getAppCommon().destroyTextureObject( texture );
}

//============================================================================
bool INlc::loadToGPU( CQtTexture * texture )
{
   return getAppCommon().loadToGPU( texture );
}

//============================================================================
void INlc::bindToUnit( CQtTexture * texture, unsigned int unit )
{
    getAppCommon().bindToUnit( texture, unit );
}

//============================================================================
void INlc::beginGuiTexture( CGUITextureQt * guiTexture, NlcColor color )
{
    getAppCommon().beginGuiTexture( guiTexture, color );
}

//============================================================================
void INlc::drawGuiTexture( CGUITextureQt * guiTexture, float * x, float * y, float * z, const NlcRect& textureRect, const NlcRect& diffuse, int orientation )
{
    getAppCommon().drawGuiTexture( guiTexture, x, y, z, textureRect, diffuse, orientation );
}

//============================================================================
void INlc::endGuiTexture( CGUITextureQt * guiTexture  )
{
    getAppCommon().endGuiTexture( guiTexture );
}

//============================================================================
void INlc::drawQuad( const NlcRect& rect, NlcColor color, CBaseTexture * texture, const NlcRect * texCoords )
{
    getAppCommon().drawQuad( rect, color, texture, texCoords );
}

//============================================================================
bool INlc::firstBegin( CGUIFontTTFQt * font )
{
    return getAppCommon().firstBegin( font );
}

//============================================================================
void INlc::lastEnd( CGUIFontTTFQt * font )
{
    getAppCommon().lastEnd( font );
}

#if ENABLE_KODI
//============================================================================
CVertexBuffer INlc::createVertexBuffer( CGUIFontTTFQt * font, const std::vector<SVertex>& vertices )
{
    return getAppCommon().createVertexBuffer( font, vertices );
}
#endif // ENABLE_KODI

//============================================================================
void INlc::destroyVertexBuffer( CGUIFontTTFQt * font, CVertexBuffer & vertBuffer )
{
    getAppCommon().destroyVertexBuffer( font, vertBuffer );
}

//============================================================================
void INlc::deleteHardwareTexture( CGUIFontTTFQt * font )
{
    getAppCommon().deleteHardwareTexture( font );
}

//============================================================================
void INlc::createStaticVertexBuffers( CGUIFontTTFQt * font )
{
    getAppCommon().createStaticVertexBuffers( font );
}

//============================================================================
void INlc::destroyStaticVertexBuffers( CGUIFontTTFQt * font )
{
    getAppCommon().destroyStaticVertexBuffers( font );
}


//============================================================================
//=== shader ===//
//============================================================================

//============================================================================
int INlc::shaderGetPos()
{
    return  getAppCommon().shaderGetPos();
}

//============================================================================
int INlc::shaderGetCol()
{
    return  getAppCommon().shaderGetCol();
}

//============================================================================
int INlc::shaderGetModel()
{
    return  getAppCommon().shaderGetModel();
}

//============================================================================
int INlc::shaderGetCoord0()
{
    return  getAppCommon().shaderGetCoord0();
}

//============================================================================
int INlc::shaderGetCoord1()
{
    return  getAppCommon().shaderGetCoord1();
}

//============================================================================
int INlc::shaderGetUniCol()
{
    return  getAppCommon().shaderGetUniCol();
}

//============================================================================
//=== remder ===//
//============================================================================

//============================================================================
void INlc::captureScreen( CScreenshotSurface * screenCaptrue, NlcRect& captureArea )
{
    getAppCommon().captureScreen( screenCaptrue, captureArea );
}

//============================================================================
void INlc::toGuiRenderVideoFrame( int textureIdx, CRenderBuffer* videoBuffer )
{
    getAppCommon().toGuiRenderVideoFrame( textureIdx, videoBuffer );
}

//============================================================================
bool INlc::initRenderSystem()
{
    return getAppCommon().initRenderSystem( );
}

//============================================================================
bool INlc::destroyRenderSystem()
{
    return getAppCommon().destroyRenderSystem();
}

//============================================================================
bool INlc::resetRenderSystem( int width, int height )
{
    return getAppCommon().resetRenderSystem( width, height );
}

//============================================================================
int INlc::getMaxTextureSize( )
{
    return getAppCommon().getMaxTextureSize( );
}

//============================================================================
bool INlc::beginRender()
{
    return getAppCommon().beginRender();
}

//============================================================================
bool INlc::endRender()
{
    return getAppCommon().endRender();
}

//============================================================================
void INlc::presentRender( bool rendered, bool videoLayer )
{
    getAppCommon().presentRender( rendered, videoLayer );
}

//============================================================================
bool INlc::clearBuffers( NlcColor color )
{
    return getAppCommon().clearBuffers( color );
}

//============================================================================
bool INlc::isExtSupported( const char* extension ) 
{
    return getAppCommon().isExtSupported( extension );
}

//============================================================================
void INlc::setVSync( bool vsync )
{
    getAppCommon().setVSync( vsync );
}

//============================================================================
void INlc::setViewPort( const NlcRect& viewPort )
{
    getAppCommon().setViewPort( viewPort );
}

//============================================================================
void INlc::getViewPort( NlcRect& viewPort )
{
    getAppCommon().getViewPort( viewPort );
}

//============================================================================
bool INlc::scissorsCanEffectClipping()
{
    return getAppCommon().scissorsCanEffectClipping();
}

//============================================================================
NlcRect INlc::clipRectToScissorRect( const NlcRect &rect )
{
    return getAppCommon().clipRectToScissorRect( rect );
}

//============================================================================
void INlc::setScissors( const NlcRect& rect )
{
    getAppCommon().setScissors( rect );
}

//============================================================================
void INlc::resetScissors()
{
    getAppCommon().resetScissors();
}

//============================================================================
void INlc::captureStateBlock()
{
    getAppCommon().captureStateBlock();
}

//============================================================================
void INlc::applyStateBlock()
{
    getAppCommon().applyStateBlock();
}

//============================================================================
void INlc::setCameraPosition( const NlcPoint& camera, int screenWidth, int screenHeight, float stereoFactor )
{
    getAppCommon().setCameraPosition( camera, screenWidth, screenHeight, stereoFactor );
}

//============================================================================
void INlc::applyHardwareTransform( const TransformMatrix &matrix )
{
    getAppCommon().applyHardwareTransform( matrix );
}

//============================================================================
void INlc::restoreHardwareTransform()
{
    getAppCommon().restoreHardwareTransform();
}

//============================================================================
bool INlc::testRender()
{
    return getAppCommon().testRender();
}

//============================================================================
void INlc::project( float &x, float &y, float &z )
{
    getAppCommon().project( x, y, z );
}

//============================================================================
void INlc::initialiseShaders()
{
    getAppCommon().initialiseShaders();
}

//============================================================================
void INlc::releaseShaders()
{
    getAppCommon().releaseShaders();
}

//============================================================================
bool INlc::enableShader( ESHADERMETHOD method )
{
    return getAppCommon().enableShader( method );
}

//============================================================================
bool INlc::isShaderValid( ESHADERMETHOD method )
{
    return getAppCommon().isShaderValid( method );
}

//============================================================================
void INlc::disableGUIShader()
{
    getAppCommon().disableGUIShader();
}

//============================================================================
void INlc::disableShader( ESHADERMETHOD method )
{
    getAppCommon().disableShader( method );
}

// yuv shader
//============================================================================
void INlc::shaderSetField( ESHADERMETHOD shader, int field )
{
    getAppCommon().shaderSetField( shader, field );
}

//============================================================================
void INlc::shaderSetWidth( ESHADERMETHOD shader, int w )
{
    getAppCommon().shaderSetWidth( shader, w );
}

//============================================================================
void INlc::shaderSetHeight( ESHADERMETHOD shader, int h )
{
    getAppCommon().shaderSetHeight( shader, h );
}

//============================================================================
void INlc::shaderSetBlack( ESHADERMETHOD shader, float black )
{
    getAppCommon().shaderSetBlack( shader, black );
}

//============================================================================
void INlc::shaderSetContrast( ESHADERMETHOD shader, float contrast )
{
    getAppCommon().shaderSetContrast( shader, contrast );
}

//============================================================================
void INlc::shaderSetConvertFullColorRange( ESHADERMETHOD shader, bool convertFullRange )
{
    getAppCommon().shaderSetConvertFullColorRange( shader, convertFullRange );
}

//============================================================================
int INlc::shaderGetVertexLoc( ESHADERMETHOD shader )
{
    return getAppCommon().shaderGetVertexLoc( shader );
}

//============================================================================
int INlc::shaderGetYcoordLoc( ESHADERMETHOD shader )
{
    return getAppCommon().shaderGetYcoordLoc( shader );
}

//============================================================================
int INlc::shaderGetUcoordLoc( ESHADERMETHOD shader )
{
    return getAppCommon().shaderGetUcoordLoc( shader );
}

//============================================================================
int INlc::shaderGetVcoordLoc( ESHADERMETHOD shader )
{
    return getAppCommon().shaderGetVcoordLoc( shader );
}

//============================================================================
void INlc::shaderSetMatrices( ESHADERMETHOD shader, const float *p, const float *m )
{
    getAppCommon().shaderSetMatrices( shader, p, m );
}

//============================================================================
void INlc::shaderSetAlpha( ESHADERMETHOD shader, float alpha )
{
    getAppCommon().shaderSetAlpha( shader, alpha );
}

//============================================================================
void INlc::shaderSetFlags( ESHADERMETHOD shader, unsigned int flags )
{
    getAppCommon().shaderSetFlags( shader, flags );
}

//============================================================================
void INlc::shaderSetFormat( ESHADERMETHOD shader, EShaderFormat format )
{
    getAppCommon().shaderSetFormat( shader, format );
}

//============================================================================
void INlc::shaderSourceTexture( ESHADERMETHOD shader, int ytex )
{
    getAppCommon().shaderSourceTexture( shader, ytex );
}

//============================================================================
void INlc::shaderSetStepX( ESHADERMETHOD shader, float stepX )
{
    getAppCommon().shaderSetStepX( shader, stepX );
}

//============================================================================
void INlc::shaderSetStepY( ESHADERMETHOD shader, float stepY )
{
    getAppCommon().shaderSetStepY( shader, stepY );
}

//============================================================================
// filter shader
bool INlc::shaderGetTextureFilter( ESHADERMETHOD shader, int& filter )
{
    return getAppCommon().shaderGetTextureFilter( shader, filter );
}

//============================================================================
int INlc::shaderGetcoordLoc( ESHADERMETHOD shader )
{
    return getAppCommon().shaderGetcoordLoc( shader );
}

//============================================================================
int INlc::shaderVertexAttribPointer( ESHADERMETHOD shader, unsigned int index, int size, int type, bool normalized, int stride, const void * pointer )
{
    return getAppCommon().shaderVertexAttribPointer( shader, index, size, type, normalized, stride, pointer );
}

//============================================================================
void INlc::shaderEnableVertexAttribArray( ESHADERMETHOD shader, int arrayId )
{
    getAppCommon().shaderEnableVertexAttribArray( shader, arrayId );
}

//============================================================================
void INlc::shaderDisableVertexAttribArray( ESHADERMETHOD shader, int arrayId )
{
    getAppCommon().shaderDisableVertexAttribArray( shader, arrayId );
}


// frame buffers
//============================================================================
void INlc::frameBufferGen( int bufCount, unsigned int* fboId )
{
    getAppCommon().frameBufferGen( bufCount, fboId );
}

//============================================================================
void INlc::frameBufferDelete( int bufCount, unsigned int* fboId )
{
    getAppCommon().frameBufferDelete( bufCount, fboId );
}

//============================================================================
void INlc::frameBufferTexture2D( int target, unsigned int texureId )
{
    getAppCommon().frameBufferTexture2D( target, texureId );
}

//============================================================================
void INlc::frameBufferBind( unsigned int fboId )
{
    getAppCommon().frameBufferBind( fboId );
}

//============================================================================
bool INlc::frameBufferStatus()
{
    return getAppCommon().frameBufferStatus();
}

//============================================================================
void INlc::glFuncDrawElements( GLenum mode, GLsizei count, GLenum type, const GLvoid *indices )
{
    getAppCommon().glFuncDrawElements( mode, count, type, indices );
}

//============================================================================
void INlc::glFuncDisable( GLenum cap )
{
    getAppCommon().glFuncDisable( cap );
}

//============================================================================
void INlc::glFuncBindTexture( GLenum target, GLuint texture )
{
    getAppCommon().glFuncBindTexture( target, texture );
}

//============================================================================
void INlc::glFuncViewport( GLint x, GLint y, GLsizei width, GLsizei height )
{
    getAppCommon().glFuncViewport( x, y, width, height );
}

//============================================================================
void INlc::glFuncScissor( GLint x, GLint y, GLsizei width, GLsizei height )
{
    getAppCommon().glFuncScissor( x, y, width, height );
}

//============================================================================
void INlc::glFuncGenTextures( GLsizei n, GLuint * textures )
{
    getAppCommon().glFuncGenTextures( n, textures );
}

//============================================================================
void INlc::glFuncDeleteTextures( GLsizei n, const GLuint *textures )
{
    getAppCommon().glFuncDeleteTextures( n, textures );
}

//============================================================================
void INlc::glFuncTexImage2D( GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels )
{
    getAppCommon().glFuncTexImage2D( target, level, internalformat, width, height, border, format, type, pixels );
}

//============================================================================
void INlc::glFuncTexParameteri( GLenum target, GLenum pname, GLint param )
{
    getAppCommon().glFuncTexParameteri( target, pname, param );
}

//============================================================================
void INlc::glFuncReadPixels( GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels )
{
    getAppCommon().glFuncReadPixels( x, y, width, height, format, type, pixels );
}

//============================================================================
void INlc::glFuncPixelStorei( GLenum pname, GLint param )
{
    getAppCommon().glFuncPixelStorei( pname, param );
}

//============================================================================
void INlc::glFuncFinish()
{
    getAppCommon().glFuncFinish();
}

//============================================================================
void INlc::glFuncEnable( GLenum cap )
{
    getAppCommon().glFuncEnable( cap );
}

//============================================================================
void INlc:: glFuncTexSubImage2D( GLenum target, GLint level,
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
void INlc::glFuncBlendFunc( GLenum sfactor, GLenum dfactor )
{
    getAppCommon().glFuncBlendFunc( sfactor, dfactor );
}

//============================================================================
void INlc::glFuncVertexAttribPointer( GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer )
{
    getAppCommon().glFuncVertexAttribPointer( index, size, type, normalized, stride, pointer );
}

//============================================================================
void INlc::glFuncDisableVertexAttribArray( GLuint index )
{
    getAppCommon().glFuncDisableVertexAttribArray( index );
}

//============================================================================
void INlc::glFuncEnableVertexAttribArray( GLuint index )
{
    getAppCommon().glFuncEnableVertexAttribArray( index );
}

//============================================================================
void INlc::glFuncDrawArrays( GLenum mode, GLint first, GLsizei count )
{
    getAppCommon().glFuncDrawArrays( mode, first, count );
}
