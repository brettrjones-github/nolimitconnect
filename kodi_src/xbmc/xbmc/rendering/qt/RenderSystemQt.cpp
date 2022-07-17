/*
 *  Copyright (C) 2005-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */
#include "config_kodi.h"
#ifdef HAVE_QT_GUI

#include "RenderSystemQt.h"

#include "guilib/DirtyRegion.h"


#include "windowing/GraphicContext.h"
#include "settings/AdvancedSettings.h"
#include "utils/log.h"
#include "utils/TimeUtils.h"
#include "utils/SystemInfo.h"
#include "utils/MathUtils.h"
#ifdef TARGET_POSIX
#include "XTimeUtils.h"
#endif

#include "GuiInterface/INlc.h"

//============================================================================
CRenderSystemQt::CRenderSystemQt()
    : CRenderSystemBase()
    , m_INlc( INlc::getINlc() )
{
}

//============================================================================
bool CRenderSystemQt::InitRenderSystem()
{
    m_maxTextureSize = 1024;
    m_bVSync = false;
    m_iVSyncMode = 0;
    m_bVsyncInit = false;
    // Get the version number
    m_RenderVersionMajor = 1;
    m_RenderVersionMinor = 0;

    // Get our driver vendor and renderer
    m_RenderVendor = "Qt";
    m_RenderExtensions = " ";

    //LogGraphicsInfo();

    m_INlc.initRenderSystem();
    m_bRenderCreated = true;

    return true;
}

//============================================================================
bool CRenderSystemQt::ResetRenderSystem( int width, int height )
{
    int maxTextureSize = m_INlc.getMaxTextureSize( );
    if( maxTextureSize )
    {
        m_maxTextureSize = maxTextureSize;
    }

    return m_INlc.resetRenderSystem(  width, height );
}

//============================================================================
bool CRenderSystemQt::DestroyRenderSystem()
{
    ResetScissors();
    CDirtyRegionList dirtyRegions;
    CDirtyRegion dirtyWindow( CServiceBroker::GetWinSystem()->GetGfxContext().GetViewWindow() );
    dirtyRegions.push_back( dirtyWindow );

    ClearBuffers( 0 );
    PresentRenderImpl( true );

    m_bRenderCreated = false;
    m_INlc.destroyRenderSystem( );

    return true;
}

//============================================================================
bool CRenderSystemQt::BeginRender()
{
    if( !m_bRenderCreated )
        return false;

    bool useLimited = CServiceBroker::GetWinSystem()->UseLimitedColor();


    m_limitedColorRange = useLimited;
    m_INlc.beginRender( );
    return true;
}

//============================================================================
bool CRenderSystemQt::EndRender()
{
    if( !m_bRenderCreated )
        return false;
    m_INlc.endRender( );
    return true;
}

//============================================================================
bool CRenderSystemQt::ClearBuffers( UTILS::Color color )
{
    return m_INlc.clearBuffers( (NlcColor)color );
}

//============================================================================
bool CRenderSystemQt::IsExtSupported( const char* extension ) const
{
    return m_INlc.isExtSupported( extension );
}

//============================================================================
void CRenderSystemQt::PresentRender( bool rendered, bool videoLayer )
{
    SetVSync( true );

    if( !m_bRenderCreated )
        return;

    PresentRenderImpl( rendered );

     m_INlc.presentRender(  rendered, videoLayer );
    // if video is rendered to a separate layer, we should not block this thread
    if( !rendered && !videoLayer )
        Sleep( 40 );
}

//============================================================================
void CRenderSystemQt::SetVSync( bool enable )
{
    if( m_bVSync == enable && m_bVsyncInit == true )
        return;

    if( !m_bRenderCreated )
        return;

    if( enable )
        CLog::Log( LOGINFO, "Qt: Enabling VSYNC" );
    else
        CLog::Log( LOGINFO, "Qt: Disabling VSYNC" );

    m_iVSyncMode = 0;
    m_iVSyncErrors = 0;
    m_bVSync = enable;
    m_bVsyncInit = true;

    m_INlc.verifyGlState( "SetVSync Begin" );
    SetVSyncImpl( enable );
    m_INlc.verifyGlState( "SetVSync End" );

    if( !enable )
        return;

    if( !m_iVSyncMode )
        CLog::Log( LOGERROR, "Qt: Vertical Blank Syncing unsupported" );
    else
        CLog::Log( LOGINFO, "Qt: Selected vsync mode %d", m_iVSyncMode );
}

//============================================================================
void CRenderSystemQt::CaptureStateBlock()
{
    if( !m_bRenderCreated )
        return;
    m_INlc.captureStateBlock( );
}

//============================================================================
void CRenderSystemQt::ApplyStateBlock()
{
    if( !m_bRenderCreated )
        return;
    m_INlc.applyStateBlock( );
}

//============================================================================
void CRenderSystemQt::SetCameraPosition( const CPoint &camera, int screenWidth, int screenHeight, float stereoFactor )
{
    if( !m_bRenderCreated )
        return;

    m_INlc.setCameraPosition( (const NlcPoint &)camera, screenWidth, screenHeight, stereoFactor );
}

//============================================================================
void CRenderSystemQt::Project( float &x, float &y, float &z )
{
    m_INlc.project( x, y, z );
}

//============================================================================
bool CRenderSystemQt::TestRender()
{
    return  m_INlc.testRender( );
}

//============================================================================
void CRenderSystemQt::InitialiseShaders()
{
    m_INlc.initialiseShaders( );
}

//============================================================================
void CRenderSystemQt::ReleaseShaders()
{
    m_INlc.releaseShaders();
}

//============================================================================
void CRenderSystemQt::EnableGUIShader( ESHADERMETHOD method )
{
    m_INlc.enableShader( method );
}

//============================================================================
void CRenderSystemQt::DisableGUIShader()
{
    return  m_INlc.disableGUIShader();
}

//============================================================================
int CRenderSystemQt::GUIShaderGetPos()
{
    return  m_INlc.shaderGetPos();
}

//============================================================================
int CRenderSystemQt::GUIShaderGetCol()
{
    return  m_INlc.shaderGetCol();
}

//============================================================================
int CRenderSystemQt::GUIShaderGetModel()
{
    return  m_INlc.shaderGetModel();
}

//============================================================================
int CRenderSystemQt::GUIShaderGetCoord0()
{
    return  m_INlc.shaderGetCoord0();
}

//============================================================================
int CRenderSystemQt::GUIShaderGetCoord1()
{
    return  m_INlc.shaderGetCoord1();
}

//============================================================================
int CRenderSystemQt::GUIShaderGetUniCol()
{
    return  m_INlc.shaderGetUniCol();
}

//============================================================================
void CRenderSystemQt::CalculateMaxTexturesize()
{
    // GLES cannot do PROXY textures to determine maximum size,
    CLog::Log( LOGINFO, "Qt: Maximum texture width: %u", m_maxTextureSize );
}

//============================================================================
void CRenderSystemQt::GetViewPort( CRect& viewPort )
{
    if( !m_bRenderCreated )
        return;

    return  m_INlc.getViewPort( (NlcRect&) viewPort );
}

//============================================================================
void CRenderSystemQt::SetViewPort( const CRect& viewPort )
{
    if( !m_bRenderCreated )
        return;

    m_INlc.setViewPort( ( NlcRect& )viewPort );
}

//============================================================================
bool CRenderSystemQt::ScissorsCanEffectClipping()
{

    return false;
}

//============================================================================
CRect CRenderSystemQt::ClipRectToScissorRect( const CRect &rect )
{
    return rect;
}

//============================================================================
void CRenderSystemQt::SetScissors( const CRect &rect )
{
    if( !m_bRenderCreated )
        return;
}

//============================================================================
void CRenderSystemQt::ResetScissors()
{
    SetScissors( CRect( 0, 0, ( float )m_width, ( float )m_height ) );
}

#endif // HAVE_QT_GUI
