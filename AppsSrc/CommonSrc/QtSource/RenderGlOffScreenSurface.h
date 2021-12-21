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
// http://www.nolimitconnect.com
//============================================================================
#include "config_apps.h"

#include <QtCore/QObject>
#include <QtGui/QScreen>
#include <QtGui/QOffscreenSurface>
#include <QtGui/QPaintEvent>
#include <QtGui/QResizeEvent>
#include <QtGui/QOpenGLFunctions>
#include <QtGui/QOpenGLExtraFunctions>
#include <QtGui/QSurfaceFormat>
#include <QtWidgets/QWidget>
#include <QOpenGLShaderProgram>

#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
#include <QOpenGLFramebufferObject>
#include <QOpenGLPaintDevice>
#else
#include <QtGui/QOpenGLPaintDevice>
#include <QtGui/QOpenGLFramebufferObject>
#endif // QT_VERSION < QT_VERSION_CHECK(6,0,0)


#include <atomic>
#include <mutex>

class RenderGlWidget;
class RenderKodiThread;
class RenderGlLogic;

class RenderGlOffScreenSurface : public QOffscreenSurface
{
    Q_OBJECT

public:
    /// @brief Constructor. Creates a render window.
    /// @param targetScreen Target screen.
    /// @param size Initial size of a surface buffer.
    /// this is because before the FBO and off-screen surface haven't been created.
    /// By default this uses the QWindow::requestedFormat() for OpenGL context and off-screen
    /// surface.
    explicit RenderGlOffScreenSurface(  RenderGlLogic * renderLogic,
                                        RenderGlWidget * glWidget,
                                        QOpenGLContext * threadRenderContext,
										QScreen* targetScreen = nullptr, 
										const QSize& size = QSize( 1, 1 ) );

    /// @brief Destructor.
    virtual ~RenderGlOffScreenSurface();

    /// @brief Check if the window is initialized and can be used for rendering.
    /// @return Returns true if context, surface and FBO have been set up to start rendering.
    bool                        isValid() const;

    /// @brief set target surface size.. will recreate FrameBufferObject and paint object in next render pass
    void                        setSurfaceSize( QSize surfaceSize );

    /// @brief Use getSurfaceSize() instead size() for get a size of a surface buffer. We can't override size() as it is not virtual.
    QSize                       getSurfaceSize();

    /// @brief same as getSurfaceSize but no mutex lock
    QSize                       bufferSize() const;

    /// @brief get context for render thread
    QOpenGLContext*             getThreadRenderContext()        { return m_RenderThreadContext; }

    /// @brief render initialize from thread
    virtual void				initRenderGlSystem();

	/// @brief render is shutting down
    virtual void				destroyRenderGlSystem();

    /// @brief begin render from thread
    bool                        beginRenderGl( );

    /// @brief end render from thread
    bool                        endRenderGl();

    /// @brief end render from thread
    void                        presentRenderGl( bool rendered, bool videoLayer );

    /// @brief Makes the OpenGL context current for rendering.
    /// @note Make sure to bindFramebufferObject() if you want to render to this widgets FBO.
    void                        makeCurrent();

    /// @brief Release the OpenGL context.
    void                        doneCurrent();

    /// @brief must be called from render thread
    void                        setRenderFunctions( QOpenGLFunctions * glFunctions, QOpenGLExtraFunctions* glExtraFunctions );

protected:

    /// @brief Return the OpenGL function object that can be used the issue OpenGL commands.
    /// @return The functions for the context or nullptr if it the context hasn't been created yet.
    QOpenGLFunctions*           functions() const;

    /// @brief Return the OpenGL off-screen frame buffer object identifier.
    /// @return The OpenGL off-screen frame buffer object identifier or 0 if no FBO has been created
    /// yet.
    /// @note This changes on every resize!
    GLuint                      framebufferObjectHandle() const;

    /// @brief Return the OpenGL off-screen frame buffer object.
    /// @return The OpenGL off-screen frame buffer object or nullptr if no FBO has been created yet.
    /// @note This changes on every resize!
    const QOpenGLFramebufferObject* getFramebufferObject() const;

    /// @brief Return the OpenGL off-screen frame buffer object identifier.
    /// @return The OpenGL off-screen frame buffer object identifier or 0 if no FBO has been created
    /// yet.
    void                        bindFramebufferObject();

    /// @brief Return the current contents of the FBO.
    /// @return FBO content as 32bit QImage. You might need to swap RGBA to BGRA or vice-versa.
    QImage                      grabFramebuffer();

    /// @brief Copy content of framebuffer to back buffer and swap buffers if the surface is
    /// double-buffered.
    /// If the surface is not double-buffered, the frame buffer content is blitted to the front
    /// buffer.
    /// If the window is not exposed, only the OpenGL pipeline is glFlush()ed so the framebuffer can
    /// be read back.
    void                        swapBuffers();

    /// @brief  change surface size in thread if required
    void                        checkForSizeChange();

	/// @brief return true if has been initialized from kodi thread
	bool						isReadyForRender();

    /// @brief get gl function
    QOpenGLFunctions *          getGlFunctions() { return m_Glf; }

    /// @brief get gl function
    QOpenGLExtraFunctions *     getGlExtraFunctions() { return m_GlfExtra; }

public slots:
    /// @brief Lazy update routine like QWidget::update().
    void                        update();

signals:
    /// @brief Emitted when swapBuffers() was called and bufferswapping is done.
    void                        frameSwapped();

protected:
	virtual void                exposeEvent( QExposeEvent* e );
    virtual bool                event( QEvent* e ) override;

    //    virtual int metric(QPaintDevice::PaintDeviceMetric metric) const override;

    //      /// @brief Called whenever the window needs to repaint itself. Override to draw QPainter
    // content.
    //      /// @brief This is called AFTER paintGL()! Only needed when painting using a QPainter.
    //      virtual void paintEvent(QPainter & painter) = 0;

private:
    Q_DISABLE_COPY( RenderGlOffScreenSurface )

        
    /// @brief Initialize the window.
    void                        initializeInternal();

    /// @brief Internal method that does the actual swap work, NOT using a mutex.
    void                        swapBuffersInternal();

    /// @brief Internal method to grab content of a specific framebuffer.
    QImage                      grabFramebufferInternal( QOpenGLFramebufferObject* fbo );

    /// @brief (Re-)allocate FBO and paint device if needed due to size changes etc.
    void                        recreateFBOAndPaintDevice();

    /// @brief test texture render (for testing only)
    void                        testTexureRender( bool startRender );

	//=== vars ===//
	/// @brief render thread
    RenderGlLogic*				m_RenderGlLogic{ nullptr };

    RenderGlWidget *            m_GlWidget{ nullptr };

    /// @brief OpenGL render context.
    QOpenGLContext*             m_RenderThreadContext{ nullptr };

    /// @brief Mutex making sure not grabbing while drawing etc.
    std::mutex                  m_mutex;

    /// @brief False before the window was first exposed OR render() was called.
    std::atomic_bool            m_initialized;

    /// @brief False before the overridden initializeGL() was first called.
    bool                        m_initializedGL{ false };

	/// @brief false before thread called initRenderSystem
	bool                        m_RenderSystemInitialized = false;

    /// @brief True when currently a window update is pending.
    std::atomic_bool            m_updatePending;

    /// @brief The OpenGL 2.1 / ES 2.0 function object that can be used the issue OpenGL commands.
    QOpenGLFunctions*           m_Glf{ nullptr };
    QOpenGLExtraFunctions*      m_GlfExtra{ nullptr };

    /// @brief Background FBO for off-screen rendering when the window is not exposed.
    QOpenGLFramebufferObject*   m_fbo{ nullptr };

    /// @brief Background FBO resolving a multi sampling frame buffer in m_fbo to a frame buffer
    /// that can be grabbed to a QImage.
    QOpenGLFramebufferObject*   m_resolvedFbo{ nullptr };

    /// @brief Shader used for blitting m_fbo to screen if glBlitFrameBuffer is not available.
    QOpenGLShaderProgram*       m_blitShader{ nullptr };

    /// @brief current size of surface to render on
    QSize                       m_SurfaceSize;

    /// @brief new size of surface if window size changed
    QSize                       m_NextSurfaceSize;

   
    GLuint                      m_TestTexure1;
};
