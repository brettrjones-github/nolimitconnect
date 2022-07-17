/*
 *  Copyright (C) 2005-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "config_kodi.h"
//#if defined(TARGET_OS_WINDOWS)
//# include <libglew/include/GL/glew.h>
//#endif // defined(TARGET_OS_WINDOWS)

#include "FrameBufferObject.h"
#include "ServiceBroker.h"
#include "rendering/RenderSystem.h"
#include "utils/GLUtils.h"
#include "utils/log.h"
#ifdef HAVE_QT_GUI
# include "GuiInterface/INlc.h"
#endif // HAVE_QT_GUI

 //////////////////////////////////////////////////////////////////////
 // CFrameBufferObject
 //////////////////////////////////////////////////////////////////////

CFrameBufferObject::CFrameBufferObject()
{
    m_fbo = 0;
    m_valid = false;
    m_supported = false;
    m_bound = false;
    m_texid = 0;
}

bool CFrameBufferObject::IsSupported()
{
    if( CServiceBroker::GetRenderSystem()->IsExtSupported( "GL_EXT_framebuffer_object" ) )
        m_supported = true;
    else
        m_supported = false;
    return m_supported;
}

bool CFrameBufferObject::Initialize()
{
    if( !IsSupported() )
        return false;

    Cleanup();
#ifdef HAVE_QT_GUI
    INlc::getINlc().frameBufferGen( 1, &m_fbo );
#else
    glGenFramebuffers( 1, &m_fbo );
#endif // HAVE_QT_GUI

    VerifyGLState();

    if( !m_fbo )
        return false;

    m_valid = true;
    return true;
}

void CFrameBufferObject::Cleanup()
{
    if( !IsValid() )
        return;

    if( m_fbo )
    {
#ifdef HAVE_QT_GUI
        INlc::getINlc().frameBufferDelete( 1, &m_fbo );
#else
        glDeleteFramebuffers( 1, &m_fbo );
#endif // HAVE_QT_GUI
    }

    if( m_texid )
        glDeleteTextures( 1, &m_texid );

    m_texid = 0;
    m_fbo = 0;
    m_valid = false;
    m_bound = false;
}

bool CFrameBufferObject::CreateAndBindToTexture( GLenum target, int width, int height, GLenum format, GLenum type,
                                                 GLenum filter, GLenum clampmode )
{
    if( !IsValid() )
        return false;

    INlc& gotv = INlc::getINlc();

    if( m_texid )
        gotv.glFuncDeleteTextures( 1, &m_texid );

    m_bound = false;

    gotv.glFuncGenTextures( 1, &m_texid );
    gotv.glFuncBindTexture( target, m_texid );
    gotv.glFuncTexImage2D( target, 0, format, width, height, 0, GL_RGBA, type, NULL );
    gotv.glFuncTexParameteri( target, GL_TEXTURE_WRAP_S, clampmode );
    gotv.glFuncTexParameteri( target, GL_TEXTURE_WRAP_T, clampmode );
    gotv.glFuncTexParameteri( target, GL_TEXTURE_MAG_FILTER, filter );
    gotv.glFuncTexParameteri( target, GL_TEXTURE_MIN_FILTER, filter );
    VerifyGLState();

#ifdef HAVE_QT_GUI
    m_bound = false;

    gotv.glFuncGenTextures( 1, &m_texid );
    gotv.glFuncBindTexture( target, m_texid );
    gotv.glFuncTexImage2D( target, 0, format, width, height, 0, GL_RGBA, type, NULL );
    gotv.glFuncTexParameteri( target, GL_TEXTURE_WRAP_S, clampmode );
    gotv.glFuncTexParameteri( target, GL_TEXTURE_WRAP_T, clampmode );
    gotv.glFuncTexParameteri( target, GL_TEXTURE_MAG_FILTER, filter );
    gotv.glFuncTexParameteri( target, GL_TEXTURE_MIN_FILTER, filter );

    gotv.frameBufferBind( m_fbo );
    gotv.glFuncBindTexture( target, m_texid );
    gotv.frameBufferTexture2D( target, m_texid );
    VerifyGLState();
    bool status = gotv.frameBufferStatus( );
    gotv.frameBufferBind( 0 );
    if( !status )
    {
        VerifyGLState();
        return false;
    }
#else
    m_bound = false;

    glGenTextures( 1, &m_texid );
    glBindTexture( target, m_texid );
    glTexImage2D( target, 0, format, width, height, 0, GL_RGBA, type, NULL );
    glTexParameteri( target, GL_TEXTURE_WRAP_S, clampmode );
    glTexParameteri( target, GL_TEXTURE_WRAP_T, clampmode );
    glTexParameteri( target, GL_TEXTURE_MAG_FILTER, filter );
    glTexParameteri( target, GL_TEXTURE_MIN_FILTER, filter );

    glBindFramebuffer( GL_FRAMEBUFFER, m_fbo );
    glBindTexture( target, m_texid );
    glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, target, m_texid, 0 );
    VerifyGLState();
    GLenum status = glCheckFramebufferStatus( GL_FRAMEBUFFER );
    glBindFramebuffer( GL_FRAMEBUFFER, 0 );
    if( status != GL_FRAMEBUFFER_COMPLETE )
    {
        VerifyGLState();
        return false;
    }
#endif // HAVE_QT_GUI

    m_bound = true;
    return true;
}

void CFrameBufferObject::SetFiltering( GLenum target, GLenum mode )
{
    INlc& gotv = INlc::getINlc();
    gotv.glFuncBindTexture( target, m_texid );
    gotv.glFuncTexParameteri( target, GL_TEXTURE_MAG_FILTER, mode );
    gotv.glFuncTexParameteri( target, GL_TEXTURE_MIN_FILTER, mode );
}

// Begin rendering to FBO
bool CFrameBufferObject::BeginRender()
{
    if( IsValid() && IsBound() )
    {
#ifdef HAVE_QT_GUI
        INlc::getINlc().frameBufferBind( m_fbo );
#else
       glBindFramebuffer( GL_FRAMEBUFFER, m_fbo );
#endif // HAVE_QT_GUI
        return true;
    }
    return false;
}

// Finish rendering to FBO
void CFrameBufferObject::EndRender() const
{
    if( IsValid() )
    {
#ifdef HAVE_QT_GUI
        INlc::getINlc().frameBufferBind( 0 );
#else
        glBindFramebuffer( GL_FRAMEBUFFER, 0 );
#endif // HAVE_QT_GUI
    }
}
