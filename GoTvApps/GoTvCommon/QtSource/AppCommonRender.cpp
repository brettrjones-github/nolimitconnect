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
// http://www.nolimitconnect.com
//============================================================================
#include <app_precompiled_hdr.h>
#include "AppCommon.h"	
#include "AppGlobals.h"
#include "VxAppDisplay.h"
#include "MyIcons.h"
#include "AppletPopupMenu.h"
#include "ToGuiActivityInterface.h"
#include "MySndMgr.h"
#include "MyIcons.h"
#include "VxTilePositioner.h"

#include "HomeWindow.h"

#include "GuiOfferSession.h"

#include "AccountMgr.h"
#include "AppSettings.h"
#include "AppletMgr.h"

#include "FileListReplySession.h"
#include "RenderGlWidget.h"

#include <PktLib/VxCommon.h>

#include <CoreLib/VxFileUtil.h>
#include <CoreLib/VxParse.h>
#include <CoreLib/VxGlobals.h>
#include <CoreLib/IsBigEndianCpu.h>
#include <CoreLib/VxGUID.h>

#include <NetLib/VxPeerMgr.h>

#include <QApplication>
#include <QMainWindow>
#include <QCloseEvent>
#include <QDesktopServices>
#include <QDebug>
#include <QSettings>


//============================================================================
RenderGlWidget * AppCommon::getRenderConsumer( void )
{
    return m_AppletMgr.getRenderConsumer();
}

//============================================================================
void AppCommon::toGuiModuleState( EAppModule moduleNum, EModuleState moduleState )
{
    if( eModuleStateInitError == moduleState )
    {
        LogMsg( LOG_ERROR, "ERROR App Module %s failed to initialize", DescribeAppModule( moduleNum ) );
    }

	if( moduleNum == eAppModuleKodi )
	{
		RenderGlWidget * glWidget = getRenderConsumer();
		if( glWidget )
		{
			glWidget->onModuleState( moduleNum, moduleState );
            glWidget->verifyGlState( "toGuiModuleState" );
		}
	}
}


//============================================================================
//=== to gui media/render ===//
//============================================================================
//============================================================================
void AppCommon::verifyGlState( const char * msg )  // show gl error if any
{
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        glWidget->verifyGlState( msg );
    }
}

//============================================================================
//=== textures ===//
//============================================================================

//============================================================================
void AppCommon::setActiveGlTexture( unsigned int activeTextureNum )
{
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        glWidget->setActiveGlTexture( activeTextureNum );
        glWidget->verifyGlState( "setActiveGlTexture" );
    }
}

//============================================================================
void AppCommon::createTextureObject( CQtTexture * texture )
{
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        glWidget->createTextureObject( texture );
        glWidget->verifyGlState( "createTextureObject" );
    }
}

//============================================================================
void AppCommon::destroyTextureObject( CQtTexture * texture )
{
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        glWidget->destroyTextureObject( texture );
        glWidget->verifyGlState( "destroyTextureObject" );
    }
}

//============================================================================
bool AppCommon::loadToGPU( CQtTexture * texture )
{
    bool result = false;
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        result = glWidget->loadToGPU( texture );
        if( !result )
        {
            //  this is normal.. once texture is loaded it does not need to load to GPU again
            //glWidget->verifyGlState( "ERROR loadToGPU" );
        }
        else
        {
            glWidget->verifyGlState( "loadToGPU" );
        }
    }

    return result;
}

//============================================================================
void AppCommon::bindToUnit( CQtTexture * texture, unsigned int unit )
{
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        glWidget->bindToUnit( texture, unit );
        glWidget->verifyGlState( "bindToUnit" );
    }
}

//============================================================================
void AppCommon::beginGuiTexture( CGUITextureQt * guiTexture, GoTvColor color )
{
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        glWidget->beginGuiTexture( guiTexture, color );
        glWidget->verifyGlState();
    }
}

//============================================================================
void AppCommon::drawGuiTexture( CGUITextureQt * guiTexture, float * x, float * y, float * z, const GoTvRect& textureRect, const GoTvRect& diffuse, int orientation )
{
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        glWidget->drawGuiTexture( guiTexture, x, y, z, textureRect, diffuse, orientation );
        glWidget->verifyGlState( "drawGuiTexture" );
    }
}

//============================================================================
void AppCommon::endGuiTexture( CGUITextureQt * guiTexture )
{
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        glWidget->endGuiTexture( guiTexture );
        glWidget->verifyGlState( "endGuiTexture" );
    }
}

//============================================================================
void AppCommon::drawQuad( const GoTvRect& rect, GoTvColor color, CBaseTexture * texture, const GoTvRect * texCoords )
{
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        glWidget->drawQuad( rect, color, texture, texCoords );
        glWidget->verifyGlState( "drawQuad" );
    }
}

//============================================================================
bool AppCommon::firstBegin( CGUIFontTTFQt * font )
{
    bool result = false;
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        result = glWidget->firstBegin( font );
        glWidget->verifyGlState( "firstBegin" );
    }

    return result;
}

//============================================================================
void AppCommon::lastEnd( CGUIFontTTFQt * font )
{
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        glWidget->lastEnd( font );
        glWidget->verifyGlState( "lastEnd" );
    }
}

//============================================================================
CVertexBuffer AppCommon::createVertexBuffer( CGUIFontTTFQt * font, const std::vector<SVertex>& vertices )
{
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        glWidget->verifyGlState( "createVertexBuffer" );
        return glWidget->createVertexBuffer( font, vertices );
    }

    CVertexBuffer vertBuf;
    return vertBuf;
}

//============================================================================
void AppCommon::destroyVertexBuffer( CGUIFontTTFQt * font, CVertexBuffer & vertBuffer )
{
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        glWidget->destroyVertexBuffer( font, vertBuffer );
        glWidget->verifyGlState( "destroyVertexBuffer" );
    }
}


//============================================================================
void AppCommon::deleteHardwareTexture( CGUIFontTTFQt * font )
{
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        glWidget->deleteHardwareTexture( font );
        glWidget->verifyGlState( "deleteHardwareTexture" );
    }
}

//============================================================================
void AppCommon::createStaticVertexBuffers( CGUIFontTTFQt * font )
{
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        glWidget->createStaticVertexBuffers( font );
        glWidget->verifyGlState( "createStaticVertexBuffers" );
    }
}

//============================================================================
void AppCommon::destroyStaticVertexBuffers( CGUIFontTTFQt * font )
{
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        glWidget->destroyStaticVertexBuffers( font );
        glWidget->verifyGlState( "destroyStaticVertexBuffers" );
    }
}

//============================================================================
//=== remder ===//
//============================================================================

//============================================================================
void AppCommon::captureScreen( CScreenshotSurface * screenCaptrue, GoTvRect& captureArea )
{
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        glWidget->captureScreen( screenCaptrue, captureArea );
        glWidget->verifyGlState( "captureScreen" );
    }
}

//============================================================================
void AppCommon::toGuiRenderVideoFrame( int textureIdx, CRenderBuffer* videoBuffer )
{
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        glWidget->toGuiRenderVideoFrame( textureIdx, videoBuffer );
        glWidget->verifyGlState( "toGuiRenderVideoFrame" );
    }
}

//============================================================================
bool AppCommon::initRenderSystem()
{
    bool result = false;
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        result = glWidget->initRenderSystem( );
        if( result )
        {
            glWidget->verifyGlState( "initRenderSystem" );
        }
        else
        {
            glWidget->verifyGlState( "ERROR initRenderSystem" );
        }
    }

    return result;
}

//============================================================================
bool AppCommon::destroyRenderSystem()
{
    bool result = false;
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        glWidget->verifyGlState( "destroyRenderSystem" );
        result = glWidget->destroyRenderSystem();
    }

    return result;
}

//============================================================================
bool AppCommon::resetRenderSystem( int width, int height )
{
    bool result = false;
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        result = glWidget->resetRenderSystem( width, height );
        if( result )
        {
            glWidget->verifyGlState( "resetRenderSystem" );
        }
        else
        {
            glWidget->verifyGlState( "ERROR resetRenderSystem" );
        }
    }

    return result;
}

//============================================================================
int AppCommon::getMaxTextureSize()
{ 
    int result = 0;
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        result = glWidget->getMaxTextureSize( );
        if( result )
        {
            glWidget->verifyGlState( "getMaxTextureSize" );
        }
        else
        {
            glWidget->verifyGlState( "ERROR getMaxTextureSize" );
        }
    }

    return result;
}

//============================================================================
bool AppCommon::beginRender()
{
    bool result = false;
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        result = glWidget->beginRender();
        if( result )
        {
            //glWidget->verifyGlState( "beginRender" );
        }
        else
        {
            glWidget->verifyGlState( "ERROR beginRender" );
        }
    }

    return result;
}

//============================================================================
bool AppCommon::endRender()
{
    bool result = false;
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        result = glWidget->endRender();
        if( result )
        {
            //glWidget->verifyGlState( "endRenderBegin" );
        }
        else
        {
            glWidget->verifyGlState( "ERROR endRenderBegin" );
        }
    }

    return result;
}

//============================================================================
void AppCommon::presentRender( bool rendered, bool videoLayer )
{
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        glWidget->presentRender( rendered, videoLayer );
        //glWidget->verifyGlState( "presentRender" );
    }
}

//============================================================================
bool AppCommon::clearBuffers( GoTvColor color )
{
    bool result = false;
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        glWidget->verifyGlState( "clearBuffers" );
        result = glWidget->clearBuffers( color );
        if( result )
        {
            glWidget->verifyGlState( "clearBuffers" );
        }
        else
        {
            glWidget->verifyGlState( "ERROR clearBuffers" );
        }
    }

    return result;
}

//============================================================================
bool AppCommon::isExtSupported( const char* extension )
{
    bool result = false;
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        result = glWidget->isExtSupported( extension );
        glWidget->verifyGlState( "isExtSupported" );
    }

    return result;
}

//============================================================================
void AppCommon::setVSync( bool vsync )
{
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        glWidget->setVSync( vsync );
        glWidget->verifyGlState( "setVSync" );
    }
}

//============================================================================
void AppCommon::setViewPort( const GoTvRect& viewPort )
{
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        glWidget->setViewPort( viewPort );
        glWidget->verifyGlState( "setViewPort" );
    }
}

//============================================================================
void AppCommon::getViewPort( GoTvRect& viewPort )
{
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        glWidget->getViewPort( viewPort );
        glWidget->verifyGlState( "getViewPort" );
    }
}

//============================================================================
bool AppCommon::scissorsCanEffectClipping()
{
    bool result = false;
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        result = glWidget->scissorsCanEffectClipping();
        glWidget->verifyGlState( "scissorsCanEffectClipping" );
    }

    return result;
}

//============================================================================
GoTvRect AppCommon::clipRectToScissorRect( const GoTvRect &rect )
{
    GoTvRect result;
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        result = glWidget->clipRectToScissorRect( rect );
        glWidget->verifyGlState( "clipRectToScissorRect" );
    }

    return result;
}

//============================================================================
void AppCommon::setScissors( const GoTvRect& rect )
{
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        glWidget->setScissors( rect );
        glWidget->verifyGlState( "setScissors" );
    }
}

//============================================================================
void AppCommon::resetScissors()
{
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        glWidget->resetScissors();
        glWidget->verifyGlState( "resetScissors" );
    }
}

//============================================================================
void AppCommon::captureStateBlock()
{
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        glWidget->captureStateBlock();
        glWidget->verifyGlState( "captureStateBlock" );
    }
}

//============================================================================
void AppCommon::applyStateBlock()
{
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        glWidget->applyStateBlock();
        glWidget->verifyGlState( "applyStateBlock" );
    }
}

//============================================================================
void AppCommon::setCameraPosition( const GoTvPoint& camera, int screenWidth, int screenHeight, float stereoFactor )
{
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
#ifdef DEBUG_ENABLE_BROKEN_GL_CALLS
        glWidget->setCameraPosition( camera, screenWidth, screenHeight, stereoFactor );
        glWidget->verifyGlState( "setCameraPosition" );
#endif // DEBUG_ENABLE_BROKEN_GL_CALLS

    }
}

//============================================================================
void AppCommon::applyHardwareTransform( const TransformMatrix &matrix )
{
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        glWidget->applyHardwareTransform( matrix );
        glWidget->verifyGlState( "applyHardwareTransform" );
    }
}

//============================================================================
void AppCommon::restoreHardwareTransform()
{
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        glWidget->restoreHardwareTransform();
        glWidget->verifyGlState( "restoreHardwareTransform" );
    }
}

//============================================================================
bool AppCommon::testRender()
{
    bool result = false;
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        result = glWidget->testRender();
        glWidget->verifyGlState( "testRender" );
    }

    return result;
}

//============================================================================
void AppCommon::project( float &x, float &y, float &z )
{
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
#ifdef DEBUG_ENABLE_BROKEN_GL_CALLS
        glWidget->project( x, y, z );
        glWidget->verifyGlState( "project" );
#endif // DEBUG_ENABLE_BROKEN_GL_CALLS
    }
}

//============================================================================
//=== shaders ===//
//============================================================================

//============================================================================
void AppCommon::initialiseShaders()
{
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        glWidget->initialiseShaders();
        glWidget->verifyGlState( "initialiseShaders" );
    }
}

//============================================================================
void AppCommon::releaseShaders()
{
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        glWidget->releaseShaders();
        glWidget->verifyGlState( "releaseShaders" );
    }
}

//============================================================================
bool AppCommon::enableShader( ESHADERMETHOD method )
{
    bool enabled = false;
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        enabled = glWidget->enableShader( method );
        glWidget->verifyGlState( "enableShader" );
    }

    return enabled;
}

//============================================================================
bool AppCommon::isShaderValid( ESHADERMETHOD method )
{
    bool result = false;
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        result = glWidget->isShaderValid( method );
        if( !result )
        {
            glWidget->verifyGlState( "isShaderValid false" );
        }
        else
        {
            glWidget->verifyGlState( "isShaderValid true" );
        }
    }

    return result;
}

//============================================================================
void AppCommon::disableGUIShader()
{
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        glWidget->disableGUIShader();
        glWidget->verifyGlState( "disableGUIShader" );
    }
}

//============================================================================
void AppCommon::disableShader( ESHADERMETHOD method )
{
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        glWidget->disableShader( method );
        glWidget->verifyGlState( "disableShader" );
    }
}

//============================================================================
int AppCommon::shaderGetPos()
{
    int result = false;
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        result = glWidget->shaderGetPos();
        glWidget->verifyGlState( "shaderGetPos" );
    }

    return result;
}

//============================================================================
int AppCommon::shaderGetCol()
{
    int result = false;
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        result = glWidget->shaderGetCol();
        glWidget->verifyGlState( "shaderGetCol" );
    }

    return result;
}

//============================================================================
int AppCommon::shaderGetModel()
{
    int result = false;
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        result = glWidget->shaderGetModel();
        glWidget->verifyGlState( "shaderGetModel" );
    }

    return result;
}

//============================================================================
int AppCommon::shaderGetCoord0()
{
    int result = 0;
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        result = glWidget->shaderGetCoord0();
        glWidget->verifyGlState( "shaderGetCoord0" );
    }

    return result;
}

//============================================================================
int AppCommon::shaderGetCoord1()
{
    int result = false;
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        result = glWidget->shaderGetCoord1();
        glWidget->verifyGlState( "shaderGetCoord1" );
    }

    return result;
}

//============================================================================
int AppCommon::shaderGetUniCol()
{
    int result = false;
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        result = glWidget->shaderGetUniCol();
        glWidget->verifyGlState( "shaderGetUniCol" );
    }

    return result;
}

// yuv shader
//============================================================================
void AppCommon::shaderSetField( ESHADERMETHOD shader, int field )
{
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        glWidget->shaderSetField( shader, field );
        glWidget->verifyGlState( "shaderSetField" );
    }
}

//============================================================================
void AppCommon::shaderSetWidth( ESHADERMETHOD shader, int w )
{
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        glWidget->shaderSetWidth( shader, w );
        glWidget->verifyGlState( "shaderSetWidth" );
    }
}

//============================================================================
void AppCommon::shaderSetHeight( ESHADERMETHOD shader, int h )
{
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        glWidget->shaderSetHeight( shader, h );
        glWidget->verifyGlState( "shaderSetHeight" );
    }
}

//============================================================================
void AppCommon::shaderSetBlack( ESHADERMETHOD shader, float black )
{
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        glWidget->shaderSetBlack( shader, black );
        glWidget->verifyGlState( "shaderSetBlack" );
    }
}

//============================================================================
void AppCommon::shaderSetContrast( ESHADERMETHOD shader, float contrast )
{
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        glWidget->shaderSetContrast( shader, contrast );
        glWidget->verifyGlState( "shaderSetContrast" );
    }
}

//============================================================================
void AppCommon::shaderSetConvertFullColorRange( ESHADERMETHOD shader, bool convertFullRange )
{
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        glWidget->shaderSetConvertFullColorRange( shader, convertFullRange );
        glWidget->verifyGlState( "shaderSetConvertFullColorRange" );
    }
}

//============================================================================
int AppCommon::shaderGetVertexLoc( ESHADERMETHOD shader )
{
    int result = false;
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        result = glWidget->shaderGetVertexLoc( shader );
        glWidget->verifyGlState( "shaderGetVertexLoc" );
    }

    return result;
}

//============================================================================
int AppCommon::shaderGetYcoordLoc( ESHADERMETHOD shader )
{
    int result = false;
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        result = glWidget->shaderGetYcoordLoc( shader );
        glWidget->verifyGlState( "shaderGetYcoordLoc" );
    }

    return result;
}

//============================================================================
int AppCommon::shaderGetUcoordLoc( ESHADERMETHOD shader )
{
    int result = false;
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        result = glWidget->shaderGetUcoordLoc( shader );
        glWidget->verifyGlState( "shaderGetUcoordLoc" );
    }

    return result;
}

//============================================================================
int AppCommon::shaderGetVcoordLoc( ESHADERMETHOD shader )
{
    int result = false;
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        result = glWidget->shaderGetVcoordLoc( shader );
        glWidget->verifyGlState( "shaderGetVcoordLoc" );
    }

    return result;
}

//============================================================================
void AppCommon::shaderSetMatrices( ESHADERMETHOD shader, const float *p, const float *m )
{
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        glWidget->shaderSetMatrices( shader, p, m );
        glWidget->verifyGlState( "shaderSetMatrices" );
    }
}

//============================================================================
void AppCommon::shaderSetAlpha( ESHADERMETHOD shader, float alpha )
{
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        glWidget->shaderSetAlpha( shader, alpha );
        glWidget->verifyGlState( "shaderSetAlpha" );
    }
}

//============================================================================
void AppCommon::shaderSetFlags( ESHADERMETHOD shader, unsigned int flags )
{
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        glWidget->shaderSetFlags( shader, flags );
        glWidget->verifyGlState( "shaderSetFlags" );
    }
}

//============================================================================
void AppCommon::shaderSetFormat( ESHADERMETHOD shader, EShaderFormat format )
{
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        glWidget->shaderSetFormat( shader, format );
        glWidget->verifyGlState( "shaderSetFormat" );
    }
}

//============================================================================
void AppCommon::shaderSourceTexture( ESHADERMETHOD shader, int ytex )
{
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        glWidget->shaderSourceTexture( shader, ytex );
        glWidget->verifyGlState( "shaderSourceTexture" );
    }
}

//============================================================================
void AppCommon::shaderSetStepX( ESHADERMETHOD shader, float stepX )
{
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        glWidget->shaderSetStepX( shader, stepX );
        glWidget->verifyGlState( "shaderSetStepX" );
    }
}

//============================================================================
void AppCommon::shaderSetStepY( ESHADERMETHOD shader, float stepY )
{
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        glWidget->shaderSetStepY( shader, stepY );
        glWidget->verifyGlState( "shaderSetStepY" );
    }
}

//============================================================================
// filter shader
bool AppCommon::shaderGetTextureFilter( ESHADERMETHOD shader, int& filter )
{
    bool result = false;
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        result = glWidget->shaderGetTextureFilter( shader, filter );
        if( result )
        {
            glWidget->verifyGlState( "shaderGetTextureFilter" );
        }
        else
        {
            glWidget->verifyGlState( "ERROR shaderGetTextureFilter" );
        }
    }

    return result;
}

//============================================================================
int AppCommon::shaderGetcoordLoc( ESHADERMETHOD shader )
{
    int result = false;
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        result = glWidget->shaderGetcoordLoc( shader );
        glWidget->verifyGlState( "shaderGetcoordLoc" );
    }

    return result;
}

//============================================================================
int AppCommon::shaderVertexAttribPointer( ESHADERMETHOD shader, unsigned int index, int size, int type, bool normalized, int stride, const void * pointer )
{
    int result = false;
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        result = glWidget->shaderVertexAttribPointer( shader, index, size, type, normalized, stride, pointer );
        glWidget->verifyGlState( "shaderVertexAttribPointer" );
    }

    return result;
}

//============================================================================
void AppCommon::shaderEnableVertexAttribArray( ESHADERMETHOD shader, int arrayId )
{
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        glWidget->shaderEnableVertexAttribArray( shader, arrayId );
        glWidget->verifyGlState( "shaderEnableVertexAttribArray" );
    }
}

//============================================================================
void AppCommon::shaderDisableVertexAttribArray( ESHADERMETHOD shader, int arrayId )
{
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        glWidget->shaderDisableVertexAttribArray( shader, arrayId );
        glWidget->verifyGlState( "shaderDisableVertexAttribArray" );
    }
}

// frame buffers
//============================================================================
void AppCommon::frameBufferGen( int bufCount, unsigned int* fboId )
{
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        glWidget->frameBufferGen( bufCount, fboId );
        glWidget->verifyGlState( "frameBufferGen" );
    }
}

//============================================================================
void AppCommon::frameBufferDelete( int bufCount, unsigned int* fboId )
{
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        glWidget->frameBufferDelete( bufCount, fboId );
        glWidget->verifyGlState( "frameBufferDelete" );
    }
}

//============================================================================
void AppCommon::frameBufferTexture2D( int target, unsigned int texureId )
{
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        glWidget->frameBufferTexture2D( target, texureId );
        glWidget->verifyGlState( "frameBufferTexture2D" );
    }
}

//============================================================================
void AppCommon::frameBufferBind( unsigned int fboId )
{
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        glWidget->frameBufferBind( fboId );
        glWidget->verifyGlState( "frameBufferBind" );
    }
}

//============================================================================
bool AppCommon::frameBufferStatus()
{
    bool result = false;
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        result = glWidget->frameBufferStatus();
        if( result )
        {
            glWidget->verifyGlState( "frameBufferStatus" );
        }
        else
        {
            glWidget->verifyGlState( "ERROR frameBufferStatus" );
        }
    }

    return result;
}

//============================================================================
void AppCommon::glFuncDrawElements( GLenum mode, GLsizei count, GLenum type, const GLvoid *indices )
{
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        glWidget->glFuncDrawElements( mode, count, type, indices );
        glWidget->verifyGlState( "glFuncDrawElements" );
    }
}

//============================================================================
void AppCommon::glFuncDisable( GLenum cap )
{
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        glWidget->glFuncDisable( cap );
        glWidget->verifyGlState( "glFuncDisable" );
    }
}

//============================================================================
void AppCommon::glFuncBindTexture( GLenum target, GLuint texture )
{
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        glWidget->glFuncBindTexture( target, texture );
        glWidget->verifyGlState( "glFuncBindTexture" );
    }
}

//============================================================================
void AppCommon::glFuncViewport( GLint x, GLint y, GLsizei width, GLsizei height )
{
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        glWidget->glFuncViewport( x, y, width, height );
        glWidget->verifyGlState( "glFuncViewport" );
    }
}

//============================================================================
void AppCommon::glFuncScissor( GLint x, GLint y, GLsizei width, GLsizei height )
{
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        glWidget->glFuncScissor( x, y, width, height );
        glWidget->verifyGlState( "glFuncScissor" );
    }
}

//============================================================================
void AppCommon::glFuncGenTextures( GLsizei n, GLuint * textures )
{
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        glWidget->glFuncGenTextures( n, textures );
        glWidget->verifyGlState( "glFuncGenTextures" );
    }
}

//============================================================================
void AppCommon::glFuncDeleteTextures( GLsizei n, const GLuint *textures )
{
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        glWidget->glFuncDeleteTextures( n, textures );
        glWidget->verifyGlState( "glFuncDeleteTextures" );
    }
}

//============================================================================
void AppCommon::glFuncTexImage2D( GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels )
{
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        glWidget->glFuncTexImage2D( target, level, internalformat, width, height, border, format, type, pixels );
        glWidget->verifyGlState( "glFuncTexImage2D" );
    }
}

//============================================================================
void AppCommon::glFuncTexParameteri( GLenum target, GLenum pname, GLint param )
{
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        glWidget->glFuncTexParameteri( target, pname, param );
        glWidget->verifyGlState( "glFuncTexParameteri" );
    }
}

//============================================================================
void AppCommon::glFuncReadPixels( GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels )
{
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        glWidget->glFuncReadPixels( x, y, width, height, format, type, pixels );
        glWidget->verifyGlState( "glFuncReadPixels" );
    }
}

//============================================================================
void AppCommon::glFuncPixelStorei( GLenum pname, GLint param )
{
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        glWidget->glFuncPixelStorei( pname, param );
        glWidget->verifyGlState( "glFuncPixelStorei" );
    }
}

//============================================================================
void AppCommon::glFuncFinish()
{
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        glWidget->glFuncFinish();
        glWidget->verifyGlState( "glFuncFinish" );
    }
}

//============================================================================
void AppCommon::glFuncEnable( GLenum cap )
{
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        glWidget->glFuncEnable( cap );
        glWidget->verifyGlState( "glFuncEnable" );
    }
}

//============================================================================
void AppCommon:: glFuncTexSubImage2D( GLenum target, GLint level,
                                      GLint xoffset, GLint yoffset,
                                      GLsizei width, GLsizei height,
                                      GLenum format, GLenum type,
                                      const GLvoid *pixels )
{
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        glWidget-> glFuncTexSubImage2D( target, level,
                                        xoffset, yoffset,
                                        width, height,
                                        format, type,
                                        pixels );
        glWidget->verifyGlState( "glFuncTexSubImage2D" );
    }
}

//============================================================================
void AppCommon::glFuncBlendFunc( GLenum sfactor, GLenum dfactor )
{
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        glWidget->glFuncBlendFunc( sfactor, dfactor );
        glWidget->verifyGlState( "glFuncBlendFunc" );
    }
}

//============================================================================
void AppCommon::glFuncVertexAttribPointer( GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer )
{
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        glWidget->glFuncVertexAttribPointer( index, size, type, normalized, stride, pointer );
        glWidget->verifyGlState( "glFuncVertexAttribPointer" );
    }
}

//============================================================================
void AppCommon::glFuncDisableVertexAttribArray( GLuint index )
{
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        glWidget->glFuncDisableVertexAttribArray( index );
        glWidget->verifyGlState( "glFuncDisableVertexAttribArray" );
    }
}

//============================================================================
void AppCommon::glFuncEnableVertexAttribArray( GLuint index )
{
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        glWidget->glFuncEnableVertexAttribArray( index );
        glWidget->verifyGlState( "glFuncEnableVertexAttribArray" );
    }
}

//============================================================================
void AppCommon::glFuncDrawArrays( GLenum mode, GLint first, GLsizei count )
{
    RenderGlWidget * glWidget = getRenderConsumer();
    if( glWidget )
    {
        glWidget->glFuncDrawArrays( mode, first, count );
        glWidget->verifyGlState( "glFuncDrawArrays" );
    }
}
