/*
 *      Copyright (C) 2007-2015 Team XBMC
 *      http://kodi.tv
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with XBMC; see the file COPYING.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 */
#pragma once
#include "system_gl.h"

#include "utils/TransformMatrix.h"
#include "ShaderFormats.h"
#include "GuiInterface/IGoTvRender.h"


void CalculateYUVMatrixGLES( TransformMatrix&  matrix
                             , unsigned int    flags
                             , EShaderFormat   format
                             , float           black
                             , float           contrast
                             , bool            limited );

#include "guilib/Shader.h"

class IGoTv;

namespace Shaders {

    class BaseYUV2RGBShader
    {
    public:
        BaseYUV2RGBShader( IGoTv& gotv ) : m_IGoTv( gotv ) {};
        virtual ~BaseYUV2RGBShader() = default;

        virtual void            setShaderMethod( ESHADERMETHOD shaderMethod )   { m_ShaderMethod = shaderMethod; };
        virtual ESHADERMETHOD   getShaderMethod( )                              { return m_ShaderMethod; };

        virtual void            SetField( int field );
        virtual void            SetWidth( int width );
        virtual void            SetHeight( int width );

        virtual void            SetBlack( float black );
        virtual void            SetContrast( float contrast );

        virtual GLint           GetVertexLoc();
        virtual GLint           GetYcoordLoc();
        virtual GLint           GetUcoordLoc();
        virtual GLint           GetVcoordLoc();

        virtual void            SetMatrices( const GLfloat *p, const GLfloat *m );
        virtual void            SetAlpha( GLfloat alpha );

        virtual void            SetConvertFullColorRange( bool convertFullRange );

        IGoTv&                  m_IGoTv;
        ESHADERMETHOD           m_ShaderMethod;
    };


    class BaseYUV2RGBShaderQt
        : public BaseYUV2RGBShader
    {
    public:
        BaseYUV2RGBShaderQt(  IGoTv& gotv, unsigned flags, EShaderFormat format );
        ~BaseYUV2RGBShaderQt();
        void                    SetField( int field )   { BaseYUV2RGBShader::SetField( field ); }
        void                    SetWidth( int w )       { BaseYUV2RGBShader::SetWidth( w );  }
        void                    SetHeight( int h )      { BaseYUV2RGBShader::SetHeight( h );  }

        void                    SetBlack( float black ) override       { BaseYUV2RGBShader::SetBlack( black ); }
        void                    SetContrast( float contrast ) override { BaseYUV2RGBShader::SetContrast( contrast ); }
        void                    SetConvertFullColorRange( bool convertFullRange ) override { BaseYUV2RGBShader::SetConvertFullColorRange( convertFullRange ); }

        GLint                   GetVertexLoc() override { return BaseYUV2RGBShader::GetVertexLoc(); }
        GLint                   GetYcoordLoc() override { return BaseYUV2RGBShader::GetYcoordLoc(); }
        GLint                   GetUcoordLoc() override { return BaseYUV2RGBShader::GetUcoordLoc(); }
        GLint                   GetVcoordLoc() override { return BaseYUV2RGBShader::GetVcoordLoc(); }

        void                    SetMatrices( const GLfloat *p, const GLfloat *m ) override { BaseYUV2RGBShader::SetMatrices( p, m ); }
        void                    SetAlpha( GLfloat alpha ) override { BaseYUV2RGBShader::SetAlpha( alpha ); }

    protected:
        void                    OnCompiledAndLinked();
        bool                    OnEnabled();
        void                    OnDisabled();
        void                    Free();


        unsigned                m_flags;
        EShaderFormat           m_format;
        bool                    m_Valid;
                /*
        int                     m_width;
        int                     m_height;
        int                     m_field;

        float                   m_black;
        float                   m_contrast;

        std::string             m_defines;

        // shader attribute handles
        GLint                   m_hYTex;
        GLint                   m_hUTex;
        GLint                   m_hVTex;
        GLint                   m_hMatrix;
        GLint                   m_hStep;

        GLint                   m_hVertex;
        GLint                   m_hYcoord;
        GLint                   m_hUcoord;
        GLint                   m_hVcoord;
        GLint                   m_hProj;
        GLint                   m_hModel;
        GLint                   m_hAlpha;

        const GLfloat *         m_proj;
        const GLfloat *         m_model;
        GLfloat                 m_alpha;

        bool                    m_convertFullRange;
        */
    };

    class YUV2RGBProgressiveShaderQt : public BaseYUV2RGBShaderQt
    {
    public:
        YUV2RGBProgressiveShaderQt(  IGoTv& gotv, unsigned flags = 0,
                                  EShaderFormat format = SHADER_NONE );
    };

    class YUV2RGBBobShaderQt : public BaseYUV2RGBShaderQt
    {
    public:
        YUV2RGBBobShaderQt( IGoTv& gotv, unsigned flags = 0, EShaderFormat format = SHADER_NONE );
        void                    OnCompiledAndLinked();
        bool                    OnEnabled();
        /*
        GLint                   m_hStepX;
        GLint                   m_hStepY;
        GLint                   m_hField;
        */
    };

} // end namespace
