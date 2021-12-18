#ifndef RENDERGLLOGIC_H
#define RENDERGLLOGIC_H
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
#include "config_apps.h"
#include <QWidget>
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
#include <QOpenGLWidget>
#endif // QT_VERSION < QT_VERSION_CHECK(6,0,0)
#include <QOpenGLFunctions>
#include <QOpenGLExtraFunctions>

#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QVector3D>
#include <QMatrix4x4>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QElapsedTimer>

#include "RenderGlShaders.h"
#include "RenderLogoShaders.h"
//#include "LogoRenderer.h"

// uncomment to render qt logo as test instead of running kodi
//#define RENDER_LOGO_INSTEAD_OF_KODI

class RenderGlWidget;
class RenderKodiThread;
class RenderGlOffScreenSurface;


class RenderGlLogic : public QWidget
{
    Q_OBJECT
public:
    explicit RenderGlLogic( RenderGlWidget& renderWidget, QWidget * parent );
    virtual ~RenderGlLogic() = default;

#ifdef DEBUG
    void VerifyGLStateQtDbg( const char* szfile, const char* szfunction, int lineno );
#  define VerifyGLStateQt() VerifyGLStateQtDbg(__FILE__, __FUNCTION__, __LINE__)
#else
    void VerifyGLStateQt();
#endif

    virtual void                verifyGlState( const char * msg = nullptr ); // show gl error if any

    void                        setLastRenderedImage( QImage& image ){ m_renderMutex.lock();  m_LastRenderedImage = image;  m_renderMutex.unlock(); }
    QImage                      getLastRenderedImage()
    {
        m_renderMutex.lock();
        QImage retImage = m_LastRenderedImage;
        m_renderMutex.unlock();
        return retImage;
    }

    bool                        isRenderReady()  { return m_RenderRunning && m_RenderSystemReady && m_Glf;  }
    bool                        getIsRenderInitialized() { return m_RenderSystemReady; }

    QOpenGLFunctions *          getGlFunctions() { return m_Glf; }

    // start kodi first time widget is visible
    void						setRenderWindowVisible( bool isVisible ) { m_RenderWindowVisible = isVisible; if( isVisible ) startRenderKodiThread(); }
 

    void                        setRenderThreadId( unsigned int threadId ) { m_RenderThreadId = threadId; }
    unsigned int                getRenderThreadId() { return m_RenderThreadId; }

    void						setRenderKodiThreadShouldRun( bool shouldRun );

    bool						isRenderKodiThreadStarted();

    void						startRenderKodiThread();
    void						stopRenderKodiThread();

    //! called from render thread
    void						initRenderGlContext();

    //! called from render thread when ready for opengl rendering
    void						glWidgetInitialized( QOpenGLContext * threadGlContext, RenderGlOffScreenSurface * offScreenSurface );

    void                        setSurfaceSize( QSize surfaceSize );

    virtual bool                initRenderGlSystem();
    virtual bool                destroyRenderGlSystem();

    virtual bool                beginRenderGl();
    virtual bool                endRenderGl();
    virtual void                presentRenderGl( bool rendered, bool videoLayer );

    void                        lockRenderer() { m_renderMutex.lock(); }
    void                        unlockRenderer() { m_renderMutex.unlock(); }

    void                        aboutToDestroy();

    RenderKodiThread*           m_RenderKodiThread = nullptr;

signals:
    void                        signalFrameRendered();

protected:

    RenderGlWidget&             m_RenderWidget;
    RenderGlShaders             m_RenderGlShaders;

    QOpenGLContext *            m_ThreadGlContext = nullptr;
    QOpenGLFunctions *          m_Glf = nullptr;
    QOpenGLExtraFunctions *     m_GlfExtra = nullptr;

    RenderGlOffScreenSurface *  m_RenderThreadSurface = nullptr;

    QMutex                      m_renderMutex;

    QImage                      m_LastRenderedImage;

    bool                        m_RenderSystemReady = false;
    bool                        m_RenderRunning = false;

    bool                        m_exiting = false;
    bool                        m_RenderWindowVisible = false;

    unsigned int                m_RenderThreadId = 0;
};

#endif // RENDERGLLOGIC_H
