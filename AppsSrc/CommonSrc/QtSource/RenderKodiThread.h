#pragma once
#include "config_gotvapps.h"

#include <QThread>
#include <QOpenGLContext>
#include <QOffscreenSurface>

class RenderGlOffScreenSurface;
class QOpenGLFramebufferObject;
class RenderGlWidget;
class RenderGlLogic;
class AppCommon;

class RenderKodiThread : public QThread
{
    Q_OBJECT
public:
    explicit RenderKodiThread( RenderGlLogic& renderLogic );
    virtual ~RenderKodiThread() override = default;

    void						setRenderThreadShouldRun(bool shouldRun) { m_ShouldRun = shouldRun; }

    bool                        isRenderThreadStarted() { return m_IsThreadStarted; }
    void                        startRenderThread();
    void                        stopRenderThread();

protected:
    virtual void                run() override;

    RenderGlLogic&				m_RenderLogic;
    bool						m_ShouldRun = true;
    bool                        m_IsThreadStarted = false;
    bool                        m_IsKodiRunning = false;
};
