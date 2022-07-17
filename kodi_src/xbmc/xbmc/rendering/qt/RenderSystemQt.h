/*
 *  Copyright (C) 2005-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once
#include "config_kodi.h"
#ifdef HAVE_QT_GUI

#include "system_gl.h"
#include "rendering/RenderSystem.h"
#include "utils/Color.h"
#include "GuiInterface/INlcRender.h"

#include <array>

class INlc;

class CRenderSystemQt : public CRenderSystemBase
{
public:
    CRenderSystemQt();
    virtual ~CRenderSystemQt() = default;

    bool                        InitRenderSystem() override;
    bool                        DestroyRenderSystem() override;
    bool                        ResetRenderSystem( int width, int height ) override;

    bool                        BeginRender() override;
    bool                        EndRender() override;
    void                        PresentRender( bool rendered, bool videoLayer ) override;
    bool                        ClearBuffers( UTILS::Color color ) override;
    bool                        IsExtSupported( const char* extension ) const override;

    void                        SetVSync( bool vsync );
    void                        ResetVSync() { m_bVsyncInit = false; }

    void                        SetViewPort( const CRect& viewPort ) override;
    void                        GetViewPort( CRect& viewPort ) override;

    bool                        ScissorsCanEffectClipping() override;
    CRect                       ClipRectToScissorRect( const CRect &rect ) override;
    void                        SetScissors( const CRect& rect ) override;
    void                        ResetScissors() override;

    void                        CaptureStateBlock() override;
    void                        ApplyStateBlock() override;

    void                        SetCameraPosition( const CPoint &camera, int screenWidth, int screenHeight, float stereoFactor = 0.0f ) override;

    bool                        SupportsStereo( RENDER_STEREO_MODE mode ) const override { return false; }

    bool                        TestRender();

    void                        Project( float &x, float &y, float &z ) override;

    std::string                 GetShaderPath( const std::string &filename ) override { return "GLES/2.0/"; }

    void                        InitialiseShaders();
    void                        ReleaseShaders();
    void                        EnableGUIShader( ESHADERMETHOD method );
    bool                        isShaderValid( ESHADERMETHOD method );
    //void                        disableShader( ESHADERMETHOD method ) override;
    void                        DisableGUIShader();

    int                         GUIShaderGetPos();
    int                         GUIShaderGetCol();
    int                         GUIShaderGetModel();
    int                         GUIShaderGetCoord0();
    int                         GUIShaderGetCoord1();
    int                         GUIShaderGetUniCol();


protected:
    virtual void                SetVSyncImpl( bool enable ) = 0;
    virtual void                PresentRenderImpl( bool rendered ) = 0;
    void                        CalculateMaxTexturesize();

    INlc&                      m_INlc;
    int                         m_iVSyncMode;
    int                         m_iVSyncErrors;
    bool                        m_bVsyncInit;
    int                         m_width;
    int                         m_height;

    std::string                 m_RenderExtensions;
};

#endif // HAVE_QT_GUI
