#include "RenderGlLogic.h"
#include "RenderKodiThread.h"
#include "RenderGlOffScreenSurface.h"
#include "RenderGlWidget.h"

#include <QOpenGLContext>
#include <QOpenGLFunctions>

#include "GoTvDebugConfig.h"
#include <CoreLib/VxDebug.h>
#include <CoreLib/VxThread.h>

#include <GL/glu.h>

//============================================================================
RenderGlLogic::RenderGlLogic( RenderGlWidget& renderWidget, QWidget * parent )
: QWidget( parent )
, m_RenderKodiThread( new RenderKodiThread( *this ) )
, m_RenderWidget( renderWidget )
, m_RenderGlShaders( *this )
{
}

//============================================================================
void RenderGlLogic::aboutToDestroy()
{
    setRenderKodiThreadShouldRun( false );
    if( m_RenderKodiThread  )
    {
        m_RenderKodiThread->quit(); // some platforms may not have windows to close so ensure quit()
        m_RenderKodiThread->wait();
        delete m_RenderKodiThread;
        m_RenderKodiThread = nullptr;
    }
}

//============================================================================
void RenderGlLogic::setRenderKodiThreadShouldRun( bool shouldRun )
{
    if( m_RenderKodiThread )
    {
        m_RenderKodiThread->setRenderThreadShouldRun( shouldRun );
    }
}

//============================================================================
bool RenderGlLogic::isRenderKodiThreadStarted()
{
    return m_RenderKodiThread && m_RenderKodiThread->isRenderThreadStarted();
}

//============================================================================
void RenderGlLogic::startRenderKodiThread()
{
    if( m_RenderKodiThread && !m_RenderKodiThread->isRenderThreadStarted() )
    {
        m_RenderKodiThread->startRenderThread();
    }
}

//============================================================================
void RenderGlLogic::stopRenderKodiThread()
{
    if( m_RenderKodiThread && m_RenderKodiThread->isRenderThreadStarted() )
    {
        m_RenderKodiThread->stopRenderThread();
    }
}

//============================================================================
//! must be called from render thread
void RenderGlLogic::initRenderGlContext()
{
    m_ThreadGlContext = new QOpenGLContext( this );
    QSurfaceFormat surfaceFormat = m_ThreadGlContext->format();
    surfaceFormat.setColorSpace( QSurfaceFormat::ColorSpace::sRGBColorSpace );
    surfaceFormat.setRenderableType( QSurfaceFormat::RenderableType::OpenGL ); // QSurfaceFormat::RenderableType::OpenGLES );
    surfaceFormat.setSwapBehavior( QSurfaceFormat::SwapBehavior::DoubleBuffer );
    surfaceFormat.setAlphaBufferSize( 8 );
    surfaceFormat.setRedBufferSize( 8 );
    surfaceFormat.setGreenBufferSize( 8 );
    surfaceFormat.setBlueBufferSize( 8 );
    surfaceFormat.setDepthBufferSize( 24 );
    surfaceFormat.setStencilBufferSize( 8 );
    surfaceFormat.setSwapInterval( 1 );
    surfaceFormat.setSamples( 0 );

    m_ThreadGlContext->setFormat( surfaceFormat );
    m_ThreadGlContext->create();
    //m_ThreadGlContext->blockSignals( true );

    m_RenderThreadSurface = new RenderGlOffScreenSurface( this, &m_RenderWidget, m_ThreadGlContext ); // BRJ geometry not set at this point nullptr, geometry().size() );
    m_RenderThreadSurface->setFormat( surfaceFormat );
    m_RenderThreadSurface->create();

    m_ThreadGlContext->makeCurrent( m_RenderThreadSurface );

    m_Glf = m_ThreadGlContext->functions();
    Q_ASSERT( m_Glf );
    m_Glf->initializeOpenGLFunctions();

    m_GlfExtra = m_ThreadGlContext->currentContext()->extraFunctions();
    Q_ASSERT( m_GlfExtra );
    if( m_GlfExtra )
    {
        m_GlfExtra->initializeOpenGLFunctions();
        m_Glf = m_GlfExtra;
    }

    m_RenderThreadSurface->setRenderFunctions( m_Glf, m_GlfExtra );

    glWidgetInitialized( m_ThreadGlContext, m_RenderThreadSurface );
    m_ThreadGlContext->doneCurrent();
}

//============================================================================
//! called from gui thread when ready for opengl rendering
void RenderGlLogic::glWidgetInitialized( QOpenGLContext * threadGlContext, RenderGlOffScreenSurface * renderSurface )
{
    m_ThreadGlContext = threadGlContext;
    m_RenderThreadSurface = renderSurface;

    // shader initialize must be done in gui thread
    m_RenderGlShaders.initShaders( m_GlfExtra );
    m_RenderSystemReady = true;
}

//============================================================================
void RenderGlLogic::setSurfaceSize( QSize surfaceSize )
{
    if( m_RenderThreadSurface  
        && surfaceSize.width() > 0
        && surfaceSize.height() > 0 )
    {
        m_RenderThreadSurface->setSurfaceSize( surfaceSize );
    }
}

//============================================================================
bool RenderGlLogic::initRenderGlSystem()
{
    LogMsg( LOG_DEBUG, "initRenderGlSystem thread %d", VxGetCurrentThreadId() );
    if( m_RenderThreadSurface )
    {
        m_RenderThreadSurface->initRenderGlSystem();
    }

    VerifyGLStateQt();
    m_RenderRunning = true;
    return true;
}

//============================================================================
bool RenderGlLogic::destroyRenderGlSystem()
{
    VerifyGLStateQt();
    m_RenderRunning = false;
    if( m_RenderThreadSurface )
    {
        m_RenderThreadSurface->destroyRenderGlSystem();
    }

    return true;
}

//============================================================================
bool RenderGlLogic::beginRenderGl()
{
    VerifyGLStateQt();
    if( m_RenderThreadSurface )
    {
        m_RenderThreadSurface->beginRenderGl();
    }

    VerifyGLStateQt();
    return true;
}

//============================================================================
bool RenderGlLogic::endRenderGl()
{
    VerifyGLStateQt();
    //render();

    if( m_RenderThreadSurface )
    {
        m_RenderThreadSurface->endRenderGl();
    }

    VerifyGLStateQt();
    return true;
}

//============================================================================
void RenderGlLogic::presentRenderGl( bool rendered, bool videoLayer )
{
    VerifyGLStateQt();
    if( m_RenderThreadSurface )
    {
        m_RenderThreadSurface->presentRenderGl( rendered, videoLayer );
        emit signalFrameRendered();
    }

    VerifyGLStateQt();
}


//============================================================================
#ifdef DEBUG
void  RenderGlLogic::VerifyGLStateQtDbg( const char* szfile, const char* szfunction, int lineno )
{
    GLenum err = glGetError();
    if( err == GL_NO_ERROR )
        return;
    LogMsg( LOG_ERROR, "GL ERROR: %d thread %d %s\n", err, VxGetCurrentThreadId(), gluErrorString( err ) );
    if( szfile && szfunction )
    {
        LogMsg( LOG_ERROR, "In file:%s function:%s line:%d", szfile, szfunction, lineno );
    }

}
#else
void RenderGlBaseWidget::VerifyGLStateQt()
{
    GLenum err = glGetError();
    if( err == GL_NO_ERROR )
        return;
    LogMsg( LOG_ERROR, "GL ERROR: %s\n", gluErrorString( err ) );
}
#endif

//============================================================================
void RenderGlLogic::verifyGlState( const char * msg ) // show gl error if any
{
#ifdef DEBUG_RENDER_THREADS
    if( msg && getRenderThreadId() && ( getRenderThreadId() != VxGetCurrentThreadId() ) )
    {
        LogMsg( LOG_ERROR, "ERROR %s render thread 0x%X != current thread %d ", msg, getRenderThreadId(), VxGetCurrentThreadId() );
    }
#endif // DEBUG_RENDER_THREADS

#ifdef DEBUG_LOG_RENDER_CALLS
    if( msg && getRenderThreadId() && ( getRenderThreadId() != VxGetCurrentThreadId() ) )
    {
        LogMsg( LOG_ERROR, "ERROR %s render thread 0x%X != thread %d ", msg, getRenderThreadId(), VxGetCurrentThreadId() );
    }
    else if( msg )
    {
        LogMsg( LOG_ERROR, "gl func %s render thread %d ", msg, getRenderThreadId() );
    }

#endif // DEBUG_RENDER_THREADS

    VerifyGLStateQt();
}
