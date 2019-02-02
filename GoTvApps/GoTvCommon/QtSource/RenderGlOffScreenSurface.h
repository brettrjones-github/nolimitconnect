#pragma once
#include "config_gotvapps.h"

#include <QtCore/QObject>
#include <QtGui/QScreen>
#include <QtGui/QOffscreenSurface>
#include <QtGui/QPaintEvent>
#include <QtGui/QResizeEvent>
#include <QtGui/QOpenGLPaintDevice>
#include <QtGui/QOpenGLFunctions>
#include <QtGui/QOpenGLFunctions_3_0>
#include <QtGui/QOpenGLFramebufferObject>
#include <QtGui/QSurfaceFormat>
#include <QtWidgets/QWidget>
#include <QOpenGLShaderProgram>

#include <QOpenGLFunctions_3_0>

#include <atomic>
#include <mutex>

class RenderGlBaseWidget;

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
    explicit RenderGlOffScreenSurface( RenderGlBaseWidget * glWidget,
                                       QOpenGLContext* renderContext, 
                                       QScreen* targetScreen = nullptr, 
                                       const QSize& size = QSize( 1, 1 ) );

    /// @brief Destructor.
    virtual ~RenderGlOffScreenSurface();

    /// @brief Check if the window is initialized and can be used for rendering.
    /// @return Returns true if context, surface and FBO have been set up to start rendering.
    bool                        isValid() const;

    /// @brief get context that was setup by main gui thread
    QOpenGLContext*             getRenderContext( )                                     { return m_RenderContext; }

    /// @brief must be called from render thread
    void                        setRenderFunctions( QOpenGLFunctions * glFunctions );

    /// @brief begin render from thread
    bool                        beginRender( );

    /// @brief end render from thread
    void                        presentRender( bool rendered, bool videoLayer );

    /// @brief end render from thread
    bool                        endRender( );


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

    /// @brief Return the QPaintDevice for paint into it.
    QOpenGLPaintDevice*         getPaintDevice() const;

    /// @brief Return the OpenGL off-screen frame buffer object identifier.
    /// @return The OpenGL off-screen frame buffer object identifier or 0 if no FBO has been created
    /// yet.
    void                        bindFramebufferObject();

    /// @brief Return the current contents of the FBO.
    /// @return FBO content as 32bit QImage. You might need to swap RGBA to BGRA or vice-versa.
    QImage                      grabFramebuffer();

    /// @brief Makes the OpenGL context current for rendering.
    /// @note Make sure to bindFramebufferObject() if you want to render to this widgets FBO.
    void                        makeCurrent();

    /// @brief Release the OpenGL context.
    void                        doneCurrent();

    /// @brief Copy content of framebuffer to back buffer and swap buffers if the surface is
    /// double-buffered.
    /// If the surface is not double-buffered, the frame buffer content is blitted to the front
    /// buffer.
    /// If the window is not exposed, only the OpenGL pipeline is glFlush()ed so the framebuffer can
    /// be read back.
    void                        swapBuffers();

    /// @brief Use bufferSize() instead size() for get a size of a surface buffer. We can't override size() as it is not virtual.
    QSize                       bufferSize() const;

    /// @brief Resize surface buffer to newSize.
    void                        resize( const QSize& newSize );

    /// @brief Resize surface buffer to size with width w and height h.
    /// @param w Width.
    /// @param h Height.
    void                        resize( int w, int h );

    /// @brief  change surface size in thread if required
    void                        checkForSizeChange();

    /// @brief get last frame capture
    QImage&                     getLastRenderedImage()      { return m_FrameImage; }

public slots:
    /// @brief Lazy update routine like QWidget::update().
    void                        update();

    /// @brief Immediately render the widget contents to framebuffer.
    void                        render( );

    void                        slotGlResized( int w, int h );

signals:
    /// @brief Emitted when swapBuffers() was called and bufferswapping is done.
    void                        frameSwapped();

    /// @brief Emitted after a resizeEvent().
    void                        resized();

protected:
    virtual void                exposeEvent( QExposeEvent* e );
    virtual void                resizeEvent( QResizeEvent* e );
    virtual bool                event( QEvent* e ) override;

    //    virtual int metric(QPaintDevice::PaintDeviceMetric metric) const override;

        /// @brief Called exactly once when the window is first exposed OR render() is called when the
        /// widget is invisible.
        /// @note After this the off-screen surface and FBO are available.
    virtual void                initializeGL();

    /// @brief Called whenever the window size changes.
    /// @param width New window width.
    /// @param height New window height.
    virtual void                resizeGL( int width, int height );

    /// @brief Called whenever the window needs to repaint itself. Override to draw OpenGL content.
    /// When this function is called, the context is already current and the correct framebuffer is
    /// bound.
    virtual void                paintGL( );

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

    /// @brief Internal method that checks state and makes the context current, NOT using a mutex.
    void                        makeCurrentInternal();

    /// @brief Internal method to grab content of a specific framebuffer.
    QImage                      grabFramebufferInternal( QOpenGLFramebufferObject* fbo );

    /// @brief (Re-)allocate FBO and paint device if needed due to size changes etc.
    void                        recreateFBOAndPaintDevice();

    /// @brief BRJ test texture render
    void                        testTexureRender( bool startRender );

     RenderGlBaseWidget *       m_GlWidget;

    /// @brief Mutex making sure not grabbing while drawing etc.
    std::mutex                  m_mutex;

    /// @brief False before the window was first exposed OR render() was called.
    std::atomic_bool            m_initialized;

    /// @brief False before the overridden initializeGL() was first called.
    bool                        m_initializedGL = false;

    /// @brief True when currently a window update is pending.
    std::atomic_bool            m_updatePending;

    /// @brief OpenGL render context.
    QOpenGLContext*             m_RenderContext = nullptr;

    /// @brief The OpenGL 2.1 / ES 2.0 function object that can be used the issue OpenGL commands.
    QOpenGLFunctions*           m_functions = nullptr;

#if !defined(QT_OPENGL_ES_2)
    /// @brief The OpenGL 3.0 function object that can be used the issue OpenGL commands.
    QOpenGLFunctions_3_0*       m_functions_3_0 = nullptr;
#endif // !defined(QT_OPENGL_ES_2)

    /// @brief OpenGL paint device for painting with a QPainter.
    QOpenGLPaintDevice*         m_paintDevice = nullptr;

    /// @brief Background FBO for off-screen rendering when the window is not exposed.
    QOpenGLFramebufferObject*   m_fbo = nullptr;

    /// @brief Background FBO resolving a multi sampling frame buffer in m_fbo to a frame buffer
    /// that can be grabbed to a QImage.
    QOpenGLFramebufferObject*   m_resolvedFbo = nullptr;

    /// @brief Shader used for blitting m_fbo to screen if glBlitFrameBuffer is not available.
    QOpenGLShaderProgram*       m_blitShader = nullptr;

    /// @brief current size of surface to render on
    QSize                       m_SurfaceSize;

    /// @brief new size of surface if window size changed
    QSize                       m_NextSurfaceSize;

    /// @brief storage of image for GL window render
   QImage                      m_FrameImage;

   
   GLuint                      m_TestTexure1;
};
