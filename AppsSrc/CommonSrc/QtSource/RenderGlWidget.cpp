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

#include "RenderGlWidget.h"
#include "AppCommon.h"
#include "AppGlobals.h"
#include "AppSettings.h"
#include "MyIcons.h"
#include "GuiHelpers.h"
#include "MyIcons.h"
#include "RenderGlOffScreenSurface.h"

#include "GuiInterface/GoTvRenderFrame.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>
#include <CoreLib/VxTimeUtil.h>

#include <QDebug>
#include <QTimer>
#include <QOpenGLFramebufferObjectFormat>
#include <QKeyEvent>
#include <QtGui/qopenglfunctions.h>
#include <QPainter>

#include <time.h>

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


const int RESIZE_WINDOW_COMPLETED_TIMEOUT = 500;

//============================================================================
RenderGlWidget::RenderGlWidget(QWidget *parent)
: RenderGlLogic( *this, parent)
, m_MyApp( GetAppInstance() )
, m_QtToKodi( m_MyApp )
, m_ScreenSize( 1, 1 )
, m_ResizingWindowSize( 1, 1 )
, m_ResizingTimer( new QTimer( this ) )
{
    memset( m_viewPort, 0, sizeof( m_viewPort ) );
    memset( m_TextureIds, 0, sizeof( m_TextureIds ) );
    memset( m_TexSize, 0, sizeof( m_TexSize ) );

	connect( m_ResizingTimer, SIGNAL( timeout() ), this, SLOT( slotResizeWindowTimeout() ) );
    connect( this, SIGNAL( signalFrameRendered() ), this, SLOT( slotOnFrameRendered() ) );
    initRenderGlContext();
}

//============================================================================
RenderGlWidget::~RenderGlWidget()
{
    m_QtToKodi.fromGuiCloseEvent();
    aboutToDestroy();
}

//============================================================================
void RenderGlWidget::takeSnapshot()
{
    if( m_WidgetGlInitialized && getIsRenderInitialized() )
    {
        lockRenderer();
        QImage frameImage = getLastRenderedImage();
        if( !frameImage.isNull() )
        {
#ifdef TARGET_OS_WINDOWS
            frameImage.save( QString( "F:\\NoLimitConnect_Image.png" ) );
#else
            frameImage.save( QString( "~/NoLimitConnect_Image.png" ) );
#endif
        }

         unlockRenderer();
    }
}

//============================================================================
void RenderGlWidget::slotOnFrameRendered()
{
   update();
}

//============================================================================
void RenderGlWidget::paintEvent( QPaintEvent * ev )
{
    QWidget::paintEvent( ev );

    if( getIsRenderInitialized() )
    {
        QImage frameImage = getLastRenderedImage();
        if( !frameImage.isNull() )
        {
            QPainter painter;
            painter.begin( this );
            painter.setRenderHint( QPainter::Antialiasing );
            painter.drawImage( 0, 0, frameImage );
            painter.end();
        }
    }
}


//============================================================================
void RenderGlWidget::handleGlResize( int width, int height )
{
    LogMsg( LOG_DEBUG, "handleGlResize x(%d) y(%d)", width, height );

    QSize screenSize = QSize( width, height );
    if( screenSize != m_ScreenSize )
    {
        m_ScreenSize = QSize( width, height );
        m_ResizingWindowSize = m_ScreenSize;

        setSurfaceSize( m_ScreenSize );
        if( !m_IsResizing )
        {
            m_IsResizing = true;
            onResizeBegin( m_ResizingWindowSize );
        }

        onResizeEvent( m_ResizingWindowSize );
        m_ResizingTimer->stop();
        m_ResizingTimer->setSingleShot( true );
        m_ResizingTimer->start( RESIZE_WINDOW_COMPLETED_TIMEOUT );
    }
}

//============================================================================
void RenderGlWidget::showEvent( QShowEvent * ev )
{
    QWidget::showEvent( ev );
    setRenderWindowVisible( true );
}

//============================================================================
void RenderGlWidget::hideEvent( QHideEvent * ev )
{
    setRenderWindowVisible( false );
    QWidget::hideEvent( ev );
}

//============================================================================
void RenderGlWidget::closeEvent( QCloseEvent * ev )
{
    m_QtToKodi.fromGuiCloseEvent();
    setRenderKodiThreadShouldRun(false);
    aboutToDestroy();
    QWidget::closeEvent( ev );
}

//============================================================================
void RenderGlWidget::resizeEvent( QResizeEvent * ev )
{
    QWidget::resizeEvent( ev );

    handleGlResize( ev->size().width(), ev->size().width() );
}

//============================================================================
void RenderGlWidget::onResizeBegin( QSize& newSize )
{
	m_QtToKodi.fromGuiResizeBegin( newSize.width(), newSize.height() );
}

//============================================================================
void RenderGlWidget::onResizeEvent( QSize& newSize )
{
	m_QtToKodi.fromGuiResizeEvent( newSize.width(), newSize.height() );
}

//============================================================================
void RenderGlWidget::onResizeEnd( QSize& newSize )
{
	m_QtToKodi.fromGuiResizeEnd( newSize.width(), newSize.height() );
}

//============================================================================
void RenderGlWidget::onModuleState( EAppModule moduleNum, EModuleState moduleState )
{
	if( ( moduleNum == eAppModuleKodi ) && ( moduleState == eModuleStateInitialized ) )
	{
		// send a resize message so kodi will resize to fit window
		m_QtToKodi.fromGuiResizeEnd( m_ScreenSize.width(), m_ScreenSize.height() );
	}
}

//============================================================================
void RenderGlWidget::keyPressEvent( QKeyEvent * ev )
{
    if( ev->isAutoRepeat() )
        return;

    if( ! m_QtToKodi.fromGuiKeyPressEvent( ev->key() ) )
    {
        QWidget::keyPressEvent( ev );
    }
}

//============================================================================
void RenderGlWidget::keyReleaseEvent( QKeyEvent * ev )
{
    if( ev->isAutoRepeat() )
        return;

    if( !m_QtToKodi.fromGuiKeyReleaseEvent( ev->key() ) )
    {
        QWidget::keyReleaseEvent( ev );
    }
}

//============================================================================
void RenderGlWidget::mousePressEvent( QMouseEvent * ev )
{
    takeSnapshot();
    if( !m_QtToKodi.fromGuiMousePressEvent( ev->x(), ev->y(), ev->button() ) )
    {
        QWidget::mousePressEvent( ev );
    }
}

//============================================================================
void RenderGlWidget::mouseReleaseEvent( QMouseEvent * ev )
{
    if( !m_QtToKodi.fromGuiMouseReleaseEvent( ev->x(), ev->y(), ev->button() ) )
    {
        QWidget::mouseReleaseEvent( ev );
    }
}

//============================================================================
void RenderGlWidget::mouseMoveEvent( QMouseEvent * ev )
{
    if( !m_QtToKodi.fromGuiMouseMoveEvent( ev->x(), ev->y() ) )
    {
        QWidget::mouseMoveEvent( ev );
    }
}

//============================================================================
void RenderGlWidget::slotResizeWindowTimeout()
{
	if( m_IsResizing )
	{
		m_IsResizing = false;
		onResizeEnd( m_ResizingWindowSize );
	}
}
