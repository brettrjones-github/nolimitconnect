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


#include "RenderGlWidget.h"
#include "RenderGlWidget.h"
#include "AppGlobals.h"
#include "AppSettings.h"
#include "MyIcons.h"
#include "GuiHelpers.h"
#include "RenderKodiThread.h"
#include "RenderGlOffScreenSurface.h"

#include "GuiInterface/GoTvRenderFrame.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>
#include <CoreLib/VxTimeUtil.h>

#include "rendering/MatrixGL.h"

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
# endif // defined(TARGET_OS_ANDROID)



//============================================================================
//=== to gui media/render ===//
//============================================================================

//============================================================================
// called from kodi thread when initializing kodi
// if never gets called then kodi failed to initialize and is a fatal error
bool RenderGlWidget::initRenderSystem()
{
#ifdef DEBUG_RENDER_THREADS
    setRenderThreadId( VxGetCurrentThreadId() );
#endif // DEBUG_RENDER_THREADS

    RenderGlLogic::initRenderGlSystem();

    return m_RenderRunning;
}

//============================================================================
bool RenderGlWidget::destroyRenderSystem()
{
    RenderGlLogic::destroyRenderGlSystem();

    return true;
}


//============================================================================
void RenderGlWidget::captureScreen( CScreenshotSurface * screenCaptrue, GoTvRect& captureArea )
{

}

//============================================================================
void RenderGlWidget::toGuiRenderVideoFrame( int textureIdx, CRenderBuffer* videoBuffer )
{
}

//============================================================================
bool RenderGlWidget::resetRenderSystem( int width, int height )
{
    m_SrcWidth = width;
    m_SrcHeight = height;

    GoTvRect rect( 0, 0, width, height );
    setViewPort( rect );

    //getGlFunctions()->glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
    //getGlFunctions()->glClearColor( 0.5f, 0.1f, 0.2f, 1.0f );
    getGlFunctions()->glClearColor( 0.1f, 0.0f, 0.2f, 1.0f );

    getGlFunctions()->glEnable( GL_SCISSOR_TEST );

    glMatrixProject.Clear();
    glMatrixProject->LoadIdentity();
#if defined(TARGET_OS_ANDROID)
    // android needs flipped veritical
    glMatrixProject->Ortho( 0.0f, width - 1, 0.0f, height - 1, -1.0f, 1.0f );
#else
    glMatrixProject->Ortho( 0.0f, width - 1, height - 1, 0.0f, -1.0f, 1.0f );
#endif // defined(TARGET_OS_ANDROID)
    glMatrixProject.Load();

    glMatrixModview.Clear();
    glMatrixModview->LoadIdentity();
    glMatrixModview.Load();

    glMatrixTexture.Clear();
    glMatrixTexture->LoadIdentity();
    glMatrixTexture.Load();

    getGlFunctions()->glBlendFunc( GL_SRC_ALPHA, GL_ONE );
    getGlFunctions()->glEnable( GL_BLEND );          // Turn Blending On
    getGlFunctions()->glDisable( GL_DEPTH_TEST );

    return true;
}

//============================================================================
bool RenderGlWidget::clearBuffers( GoTvColor color )
{
	if( !isRenderReady() )
	{
		return false;
	}

    VerifyGLStateQt();
    //float r = GET_R( color ) / 255.0f;
    //float g = GET_G( color ) / 255.0f;
    //float b = GET_B( color ) / 255.0f;
    //float a = GET_A( color ) / 255.0f;

    //getGlFunctions()->glClearColor( r, g, b, a );
    //glClearColor( 0.1f, 0.1f, 0.2f, 1.0f );

    GLbitfield flags = GL_COLOR_BUFFER_BIT;
    getGlFunctions()->glClear( flags );
    VerifyGLStateQt();

    return true;
}

//============================================================================
bool RenderGlWidget::isExtSupported( const char* extension )
{
    if( strcmp( extension, "GL_EXT_framebuffer_object" ) == 0 )
    {
        // GLES has FBO as a core element, not an extension!
        return true;
    }
    else if( strcmp( extension, "GL_TEXTURE_NPOT" ) == 0 )
    {
        // GLES supports non-power-of-two textures as standard.
        return true;
        /* Note: The wrap mode can only be GL_CLAMP_TO_EDGE and the minification filter can only be
         * GL_NEAREST or GL_LINEAR (in other words, not mipmapped). The extension GL_OES_texture_npot
         * relaxes these restrictions and allows wrap modes of GL_REPEAT and GL_MIRRORED_REPEAT and
         * also	allows npot textures to be mipmapped with the full set of minification filters
         */
    }

    return false;
}

//============================================================================
void RenderGlWidget::setVSync( bool vsync )
{
}

//============================================================================
void RenderGlWidget::setViewPort( const GoTvRect& viewPort )
{
    getGlFunctions()->glScissor( ( GLint )viewPort.x1, ( GLint )( m_SrcHeight - viewPort.y1 - viewPort.Height() ), ( GLsizei )viewPort.Width(), ( GLsizei )viewPort.Height() );
    getGlFunctions()->glViewport( ( GLint )viewPort.x1, ( GLint )( m_SrcHeight - viewPort.y1 - viewPort.Height() ), ( GLsizei )viewPort.Width(), ( GLsizei )viewPort.Height() );
    m_viewPort[ 0 ] = viewPort.x1;
    m_viewPort[ 1 ] = m_SrcHeight - viewPort.y1 - viewPort.Height();
    m_viewPort[ 2 ] = viewPort.Width();
    m_viewPort[ 3 ] = viewPort.Height();
/*
    getGlFunctions()->glScissor( (GLint)viewPort.x1, (GLint)( viewPort.y1 ), (GLsizei)viewPort.Width(), (GLsizei)viewPort.Height() );
    getGlFunctions()->glViewport( (GLint)viewPort.x1, (GLint)( viewPort.y1 ), (GLsizei)viewPort.Width(), (GLsizei)viewPort.Height() );
    glScissor( (GLint)viewPort.x1, (GLint)( viewPort.y1 ), (GLsizei)viewPort.Width(), (GLsizei)viewPort.Height() );
    glViewport( (GLint)viewPort.x1, (GLint)( viewPort.y1 ), (GLsizei)viewPort.Width(), (GLsizei)viewPort.Height() );

    m_viewPort[ 0 ] = viewPort.x1;
    m_viewPort[ 1 ] = viewPort.y1;
    m_viewPort[ 2 ] = viewPort.Width();
    m_viewPort[ 3 ] = viewPort.Height();
    */
}

//============================================================================
void RenderGlWidget::getViewPort( GoTvRect& viewPort )
{
    viewPort.x1 = m_viewPort[ 0 ];
    viewPort.y1 = m_viewPort[ 1 ];
    viewPort.x2 = m_viewPort[ 2 ];
    viewPort.y2 = m_viewPort[ 3 ];
}

//============================================================================
bool RenderGlWidget::scissorsCanEffectClipping()
{
    return true;
}

//============================================================================
GoTvRect RenderGlWidget::clipRectToScissorRect( const GoTvRect &rect )
{
    return rect;
}

//============================================================================
void RenderGlWidget::setScissors( const GoTvRect& rect )
{
}

//============================================================================
void RenderGlWidget::resetScissors()
{
}

//============================================================================
void RenderGlWidget::captureStateBlock()
{
    glMatrixProject.Push();
    glMatrixModview.Push();
    glMatrixTexture.Push();

    getGlFunctions()->glDisable( GL_SCISSOR_TEST ); // fixes FBO corruption on Macs
    getGlFunctions()->glActiveTexture( GL_TEXTURE0 );
}

//============================================================================
void RenderGlWidget::applyStateBlock()
{
    glMatrixProject.PopLoad();
    glMatrixModview.PopLoad();
    glMatrixTexture.PopLoad();
    getGlFunctions()->glActiveTexture( GL_TEXTURE0 );
    getGlFunctions()->glEnable( GL_BLEND );
    getGlFunctions()->glEnable( GL_SCISSOR_TEST );
    getGlFunctions()->glClear( GL_DEPTH_BUFFER_BIT );
}

//============================================================================
void RenderGlWidget::setCameraPosition( const GoTvPoint& camera, int screenWidth, int screenHeight, float stereoFactor )
{
	if( !isRenderReady() )
	{
		return;
	}

    GoTvPoint offset = camera - GoTvPoint( screenWidth*0.5f, screenHeight*0.5f );

    float w = ( float )m_viewPort[ 2 ] * 0.5f;
    float h = ( float )m_viewPort[ 3 ] * 0.5f;

    glMatrixModview->LoadIdentity();
    glMatrixModview->Translatef( -( w + offset.x - stereoFactor ), +( h + offset.y ), 0 );
    glMatrixModview->LookAt( 0.0, 0.0, -2.0*h, 0.0, 0.0, 0.0, 0.0, -1.0, 0.0 );
    glMatrixModview.Load();

    glMatrixProject->LoadIdentity();
    glMatrixProject->Frustum( ( -w - offset.x )*0.5f, ( w - offset.x )*0.5f, ( -h + offset.y )*0.5f, ( h + offset.y )*0.5f, h, 100 * h );
    glMatrixProject.Load();
}

//============================================================================
void RenderGlWidget::applyHardwareTransform( const TransformMatrix& finalMatrix )
{
    if( !isRenderReady() )
        return;

    glMatrixModview.Push();
    glMatrixModview->MultMatrixf( finalMatrix );
    glMatrixModview.Load();
}

//============================================================================
void RenderGlWidget::restoreHardwareTransform()
{
    if( !isRenderReady() )
        return;

    glMatrixModview.PopLoad();
}

//============================================================================
bool RenderGlWidget::testRender()
{
    static float theta = 0.0;

    //RESOLUTION_INFO resInfo = CDisplaySettings::GetInstance().GetCurrentResolutionInfo();
    //glViewport(0, 0, resInfo.iWidth, resInfo.iHeight);

    glMatrixModview.Push();
    glMatrixModview->Rotatef( theta, 0.0f, 0.0f, 1.0f );

    enableShader( SM_DEFAULT );

    GLfloat col[ 4 ] = { 1.0f, 0.0f, 0.0f, 1.0f };
    GLfloat ver[ 3 ][ 2 ];
    GLint   posLoc = shaderGetPos();
    GLint   colLoc = shaderGetCol();

    getGlFunctions()->glVertexAttribPointer( posLoc, 2, GL_FLOAT, 0, 0, ver );
    getGlFunctions()->glVertexAttribPointer( colLoc, 4, GL_FLOAT, 0, 0, col );

    getGlFunctions()->glEnableVertexAttribArray( posLoc );
    getGlFunctions()->glEnableVertexAttribArray( colLoc );

    // Setup vertex position values
    ver[ 0 ][ 0 ] = 0.0f;
    ver[ 0 ][ 1 ] = 1.0f;
    ver[ 1 ][ 0 ] = 0.87f;
    ver[ 1 ][ 1 ] = -0.5f;
    ver[ 2 ][ 0 ] = -0.87f;
    ver[ 2 ][ 1 ] = -0.5f;

    getGlFunctions()->glDrawArrays( GL_TRIANGLES, 0, 3 );

    getGlFunctions()->glDisableVertexAttribArray( posLoc );
    getGlFunctions()->glDisableVertexAttribArray( colLoc );

    disableGUIShader();

    glMatrixModview.Pop();

    theta += 1.0f;

    return true;
}

//============================================================================
void RenderGlWidget::project( float &x, float &y, float &z )
{
    /*
    GLfloat coordX, coordY, coordZ;
    if( CMatrixGL::Project( x, y, z, glMatrixModview.Get(), glMatrixProject.Get(), m_viewPort, &coordX, &coordY, &coordZ ) )
    {
        x = coordX;
        y = ( float )( m_viewPort[ 1 ] + m_viewPort[ 3 ] - coordY );
        z = 0;
    }*/
}

// frame buffers
//============================================================================
void RenderGlWidget::frameBufferGen( int bufCount, unsigned int* fboId )
{
    getGlFunctions()->glGenFramebuffers( bufCount, fboId );
}

//============================================================================
void RenderGlWidget::frameBufferDelete( int bufCount, unsigned int* fboId )
{
    getGlFunctions()->glDeleteFramebuffers( bufCount, fboId );
}

//============================================================================
void RenderGlWidget::frameBufferTexture2D( int target, unsigned int texureId )
{
    getGlFunctions()->glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, target, texureId, 0 );
}

//============================================================================
void RenderGlWidget::frameBufferBind( unsigned int fboId )
{
    getGlFunctions()->glBindFramebuffer( GL_FRAMEBUFFER, fboId );
}

//============================================================================
bool RenderGlWidget::frameBufferStatus()
{
    GLenum status = getGlFunctions()->glCheckFramebufferStatus( GL_FRAMEBUFFER );
    return ( status == GL_FRAMEBUFFER_COMPLETE );
}

//============================================================================
void RenderGlWidget::glFuncDrawElements( GLenum mode, GLsizei count, GLenum type, const GLvoid *indices )
{
    getGlFunctions()->glDrawElements( mode, count, type, indices );
}

//============================================================================
void RenderGlWidget::glFuncDisable( GLenum cap )
{
    getGlFunctions()->glDisable( cap );
}

//============================================================================
void RenderGlWidget::glFuncBindTexture( GLenum target, GLuint texture )
{
    getGlFunctions()->glBindTexture( target, texture );
}

//============================================================================
void RenderGlWidget::glFuncViewport( GLint x, GLint y, GLsizei width, GLsizei height )
{
    getGlFunctions()->glViewport( x, y, width, height );
}

//============================================================================
void RenderGlWidget::glFuncScissor( GLint x, GLint y, GLsizei width, GLsizei height )
{
    getGlFunctions()->glScissor( x, y, width, height );
}

//============================================================================
void RenderGlWidget::glFuncGenTextures( GLsizei n, GLuint * textures )
{
    getGlFunctions()->glGenTextures( n, textures );
}

//============================================================================
void RenderGlWidget::glFuncDeleteTextures( GLsizei n, const GLuint *textures )
{
    getGlFunctions()->glDeleteTextures( n, textures );
}

//============================================================================
void RenderGlWidget::glFuncTexImage2D( GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels )
{
    getGlFunctions()->glTexImage2D( target, level, internalformat, width, height, border, format, type, pixels );
}

//============================================================================
void RenderGlWidget::glFuncTexParameteri( GLenum target, GLenum pname, GLint param )
{
    getGlFunctions()->glTexParameteri( target, pname, param );
}

//============================================================================
void RenderGlWidget::glFuncReadPixels( GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels )
{
    getGlFunctions()->glReadPixels( x, y, width, height, format, type, pixels );
}

//============================================================================
void RenderGlWidget::glFuncPixelStorei( GLenum pname, GLint param )
{
    getGlFunctions()->glPixelStorei( pname, param );
}

//============================================================================
void RenderGlWidget::glFuncFinish()
{
    getGlFunctions()->glFinish();
}

//============================================================================
void RenderGlWidget::glFuncEnable( GLenum cap )
{
     getGlFunctions()->glEnable( cap );
}

//============================================================================
void RenderGlWidget:: glFuncTexSubImage2D( GLenum target, GLint level,
                                      GLint xoffset, GLint yoffset,
                                      GLsizei width, GLsizei height,
                                      GLenum format, GLenum type,
                                      const GLvoid *pixels )
{
     getGlFunctions()->glTexSubImage2D( target, level,
                                        xoffset, yoffset,
                                        width, height,
                                        format, type,
                                        pixels );
}

//============================================================================
void RenderGlWidget::glFuncBlendFunc( GLenum sfactor, GLenum dfactor )
{
     getGlFunctions()->glBlendFunc( sfactor, dfactor );
}

//============================================================================
void RenderGlWidget::glFuncVertexAttribPointer( GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer )
{
     getGlFunctions()->glVertexAttribPointer( index, size, type, normalized, stride, pointer );
}

//============================================================================
void RenderGlWidget::glFuncDisableVertexAttribArray( GLuint index )
{
     getGlFunctions()->glDisableVertexAttribArray( index );
}

//============================================================================
void RenderGlWidget::glFuncEnableVertexAttribArray( GLuint index )
{
     getGlFunctions()->glEnableVertexAttribArray( index );
}

//============================================================================
void RenderGlWidget::glFuncDrawArrays( GLenum mode, GLint first, GLsizei count )
{
    getGlFunctions()->glDrawArrays( mode, first, count );
}
