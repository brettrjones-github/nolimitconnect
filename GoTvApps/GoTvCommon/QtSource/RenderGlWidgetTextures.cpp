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

#include <app_precompiled_hdr.h>
#include "RenderGlWidget.h"
#include "AppCommon.h"
#include "AppGlobals.h"
#include "AppSettings.h"
#include "MyIcons.h"
#include "GuiHelpers.h"
#include "MyIcons.h"

#include "GuiInterface/GoTvRenderFrame.h"

#include "guilib/TextureQt.h"
#include "guilib/GUITextureQt.h"

#include "ServiceBroker.h"
#include "rendering/RenderSystem.h"
#include "windowing/WinSystem.h"

 //=== textures ===//

//============================================================================
void RenderGlWidget::setActiveGlTexture( unsigned int activeTextureNum )
{
    switch( activeTextureNum )
    {
    case 0:
        getGlFunctions()->glActiveTexture( GL_TEXTURE0 );
        break;
    case 1:
        getGlFunctions()->glActiveTexture( GL_TEXTURE1 );
        break;
    case 2:
        getGlFunctions()->glActiveTexture( GL_TEXTURE2 );
        break;
    case 3:
        getGlFunctions()->glActiveTexture( GL_TEXTURE3 );
        break;
    case 4:
        getGlFunctions()->glActiveTexture( GL_TEXTURE4 );
        break;
    case 5:
        getGlFunctions()->glActiveTexture( GL_TEXTURE5 );
        break;
    case 6:
        getGlFunctions()->glActiveTexture( GL_TEXTURE6 );
        break;
    case 7:
        getGlFunctions()->glActiveTexture( GL_TEXTURE7 );
        break;
    case 8:
        getGlFunctions()->glActiveTexture( GL_TEXTURE8 );
        break;
    case 9:
        getGlFunctions()->glActiveTexture( GL_TEXTURE9 );
        break;
    case 10:
        getGlFunctions()->glActiveTexture( GL_TEXTURE10 );
        break;
    case 11:
        getGlFunctions()->glActiveTexture( GL_TEXTURE11 );
        break;
    case 12:
        getGlFunctions()->glActiveTexture( GL_TEXTURE12 );
        break;
    case 13:
        getGlFunctions()->glActiveTexture( GL_TEXTURE13 );
        break;
    case 14:
        getGlFunctions()->glActiveTexture( GL_TEXTURE14 );
        break;
    case 15:
        getGlFunctions()->glActiveTexture( GL_TEXTURE15 );
        break;
    case 16:
        getGlFunctions()->glActiveTexture( GL_TEXTURE16 );
        break;
    case 17:
        getGlFunctions()->glActiveTexture( GL_TEXTURE17 );
        break;
    case 18:
        getGlFunctions()->glActiveTexture( GL_TEXTURE18 );
        break;
    case 19:
        getGlFunctions()->glActiveTexture( GL_TEXTURE19 );
        break;
    case 20:
        getGlFunctions()->glActiveTexture( GL_TEXTURE20 );
        break;
    case 21:
        getGlFunctions()->glActiveTexture( GL_TEXTURE21 );
        break;
    case 22:
        getGlFunctions()->glActiveTexture( GL_TEXTURE22 );
        break;
    case 23:
        getGlFunctions()->glActiveTexture( GL_TEXTURE23 );
        break;
    case 24:
        getGlFunctions()->glActiveTexture( GL_TEXTURE24 );
        break;
    case 25:
        getGlFunctions()->glActiveTexture( GL_TEXTURE25 );
        break;
    case 26:
        getGlFunctions()->glActiveTexture( GL_TEXTURE26 );
        break;
    case 27:
        getGlFunctions()->glActiveTexture( GL_TEXTURE27 );
        break;
    case 28:
        getGlFunctions()->glActiveTexture( GL_TEXTURE28 );
        break;
    case 29:
        getGlFunctions()->glActiveTexture( GL_TEXTURE29 );
        break;
    case 30:
        getGlFunctions()->glActiveTexture( GL_TEXTURE30 );
        break;
    case 31:
        getGlFunctions()->glActiveTexture( GL_TEXTURE31 );
        break;
    default:
        LogMsg( LOG_ERROR, "RenderGlWidget::setActiveGlTextur invalid param %d", activeTextureNum );
        break;
    }

    VerifyGLStateQt();
}
 
 //============================================================================
void RenderGlWidget::createTextureObject( CQtTexture * texture )
{
    getGlFunctions()->glGenTextures( 1, ( GLuint* )&texture->m_texture );

    getGlFunctions()->glActiveTexture( GL_TEXTURE0 );
    //getGlFunctions()->glBindTexture( GL_TEXTURE_2D, texture->m_texture );
}

//============================================================================
void RenderGlWidget::destroyTextureObject( CQtTexture * texture )
{
}

//============================================================================
bool RenderGlWidget::loadToGPU( CQtTexture * texture )
{
    if( !texture || !texture->m_pixels )
    {
        // nothing to load - probably same image (no change)
        return false;
    }

    if( texture->m_texture == 0 )
    {
        // Have OpenGL generate a texture object handle for us
        // this happens only one time - the first time the texture is loaded
        createTextureObject( texture );
    }

    // Bind the texture object
    getGlFunctions()->glBindTexture( GL_TEXTURE_2D, texture->m_texture );

    GLenum filter = ( texture->m_scalingMethod == TEXTURE_SCALING::NEAREST ? GL_NEAREST : GL_LINEAR );

    // Set the texture's stretching properties
    if( texture->IsMipmapped() )
    {
        GLenum mipmapFilter = ( texture->m_scalingMethod == TEXTURE_SCALING::NEAREST ? GL_LINEAR_MIPMAP_NEAREST : GL_LINEAR_MIPMAP_LINEAR );
        getGlFunctions()->glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mipmapFilter );

//#ifndef HAS_GLES
//        // Lower LOD bias equals more sharpness, but less smooth animation
//        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -0.5f );
//        if( !texture->m_isOglVersion3orNewer )
//        {
//            glTexParameteri( GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE );
//        }
//#endif
    }
    else
    {
         getGlFunctions()->glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter );
    }

     getGlFunctions()->glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter );
     getGlFunctions()->glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
     getGlFunctions()->glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

    unsigned int maxSize = getMaxTextureSize();
    if( texture->m_textureHeight > maxSize )
    {
        LogMsg( LOG_ERROR, "GL: Image height %d too big to fit into single texture unit, truncating to %u", texture->m_textureHeight, maxSize );
        texture->m_textureHeight = maxSize;
    }
    if( texture->m_textureWidth > maxSize )
    {
        LogMsg( LOG_ERROR, "GL: Image width %d too big to fit into single texture unit, truncating to %u", texture->m_textureWidth, maxSize );
//#ifndef HAS_GLES
//         getGlFunctions()->glPixelStorei( GL_UNPACK_ROW_LENGTH, texture->m_textureWidth );
//#endif
        texture->m_textureWidth = maxSize;
    }
//
//#ifndef HAS_GLES
//    GLenum format = GL_BGRA;
//    GLint numcomponents = GL_RGBA;
//
//    switch( texture->m_format )
//    {
//    case XB_FMT_DXT1:
//        format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
//        break;
//    case XB_FMT_DXT3:
//        format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
//        break;
//    case XB_FMT_DXT5:
//    case XB_FMT_DXT5_YCoCg:
//        format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
//        break;
//    case XB_FMT_RGB8:
//        format = GL_RGB;
//        numcomponents = GL_RGB;
//        break;
//    case XB_FMT_A8R8G8B8:
//    default:
//        break;
//    }
//
//    if( ( texture->m_format & XB_FMT_DXT_MASK ) == 0 )
//    {
//         getGlFunctions()->glTexImage2D( GL_TEXTURE_2D, 0, numcomponents,
//                                texture->m_textureWidth, texture->m_textureHeight, 0,
//                                format, GL_UNSIGNED_BYTE, texture->m_pixels );
//    }
//    else
//    {
//         getGlFunctions()->glCompressedTexImage2D( GL_TEXTURE_2D, 0, format,
//                                texture->m_textureWidth, texture->m_textureHeight, 0,
//                                texture->GetPitch() * texture->GetRows(), texture->m_pixels );
//    }
//
//    if( texture->IsMipmapped() && texture->m_isOglVersion3orNewer )
//    {
//         getGlFunctions()->glGenerateMipmap( GL_TEXTURE_2D );
//    }
//
//     getGlFunctions()->glPixelStorei( GL_UNPACK_ROW_LENGTH, 0 );
//
//#else	// GLES version
//
    // All incoming textures are BGRA, which GLES does not necessarily support.
    // Some (most?) hardware supports BGRA textures via an extension.
    // If not, we convert to RGBA first to avoid having to swizzle in shaders.
    // Explicitly define GL_BGRA_EXT here in the case that it's not defined by
    // system headers, and trust the extension list instead.
#ifndef GL_BGRA_EXT
#define GL_BGRA_EXT 0x80E1
#endif
    VerifyGLStateQt();

    GLint internalformat;
    GLenum pixelformat;

    switch( texture->m_format )
    {
    default:
    case XB_FMT_RGBA8:
        internalformat = pixelformat = GL_RGBA;
        break;
    case XB_FMT_RGB8:
        internalformat = pixelformat = GL_RGB;
        break;
    case XB_FMT_A8R8G8B8:
        if( CServiceBroker::GetRenderSystem()->IsExtSupported( "GL_EXT_texture_format_BGRA8888" ) ||
            CServiceBroker::GetRenderSystem()->IsExtSupported( "GL_IMG_texture_format_BGRA8888" ) )
        {
            internalformat = pixelformat = GL_BGRA_EXT;
        }
        else if( CServiceBroker::GetRenderSystem()->IsExtSupported( "GL_APPLE_texture_format_BGRA8888" ) )
        {
            // Apple's implementation does not conform to spec. Instead, they require
            // differing format/internalformat, more like GL.
            internalformat = GL_RGBA;
            pixelformat = GL_BGRA_EXT;
        }
        else
        {
            texture->SwapBlueRed( texture->m_pixels,  texture->m_textureHeight, texture->GetPitch() );
            internalformat = pixelformat = GL_RGBA;
        }
        break;
    }

     getGlFunctions()->glTexImage2D( GL_TEXTURE_2D, 0, internalformat, texture->m_textureWidth, texture->m_textureHeight, 0,
                  pixelformat, GL_UNSIGNED_BYTE,  texture->m_pixels );

    if( texture->IsMipmapped() )
    {
         getGlFunctions()->glGenerateMipmap( GL_TEXTURE_2D );
    }

//#endif
    VerifyGLStateQt();

    if( !texture->m_bCacheMemory )
    {
        _aligned_free( texture->m_pixels );
        texture->m_pixels = NULL;
    }

    texture->m_loadedToGPU = true;

    return true;
}

//============================================================================
void RenderGlWidget::bindToUnit( CQtTexture * texture, unsigned int unit )
{
      getGlFunctions()->glActiveTexture( GL_TEXTURE0 + unit );
      getGlFunctions()->glBindTexture( GL_TEXTURE_2D, texture->m_texture );
}

//============================================================================
void RenderGlWidget::beginGuiTexture( CGUITextureQt * guiTexture, GoTvColor color )
{
    VerifyGLStateQt();

    CBaseTexture* texture = guiTexture->m_texture.m_textures[ guiTexture->m_currentFrame ];
    texture->LoadToGPU();
    if( guiTexture->m_diffuse.size() )
        guiTexture->m_diffuse.m_textures[ 0 ]->LoadToGPU();

    texture->BindToUnit( 0 );

    // Setup Colors
    guiTexture->m_col[ 0 ] = ( uint8_t )GET_R( color );
    guiTexture->m_col[ 1 ] = ( uint8_t )GET_G( color );
    guiTexture->m_col[ 2 ] = ( uint8_t )GET_B( color );
    guiTexture->m_col[ 3 ] = ( uint8_t )GET_A( color );

    if( CServiceBroker::GetWinSystem()->UseLimitedColor() )
    {
        guiTexture->m_col[ 0 ] = ( 235 - 16 ) * guiTexture->m_col[ 0 ] / 255 + 16;
        guiTexture->m_col[ 1 ] = ( 235 - 16 ) * guiTexture->m_col[ 1 ] / 255 + 16;
        guiTexture->m_col[ 2 ] = ( 235 - 16 ) * guiTexture->m_col[ 2 ] / 255 + 16;
    }

    bool hasAlpha = guiTexture->m_texture.m_textures[ guiTexture->m_currentFrame ]->HasAlpha() || guiTexture->m_col[ 3 ] < 255;
    VerifyGLStateQt();

    if( guiTexture->m_diffuse.size() )
    {
        if( guiTexture->m_col[ 0 ] == 255 && guiTexture->m_col[ 1 ] == 255 && guiTexture->m_col[ 2 ] == 255 && guiTexture->m_col[ 3 ] == 255 )
        {
            enableShader( SM_MULTI );
        }
        else
        {
            enableShader( SM_MULTI_BLENDCOLOR );
        }

        hasAlpha |= guiTexture->m_diffuse.m_textures[ 0 ]->HasAlpha();

        guiTexture->m_diffuse.m_textures[ 0 ]->BindToUnit( 1 );
    }
    else
    {
        if( guiTexture->m_col[ 0 ] == 255 && guiTexture->m_col[ 1 ] == 255 && guiTexture->m_col[ 2 ] == 255 && guiTexture->m_col[ 3 ] == 255 )
        {
            enableShader( SM_TEXTURE_NOBLEND );
        }
        else
        {
            enableShader( SM_TEXTURE );
        }
    }

    guiTexture->m_packedVertices.clear();

    if( hasAlpha )
    {
        getGlFunctions()->glBlendFuncSeparate( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA, GL_ONE );
        getGlFunctions()->glEnable( GL_BLEND );
    }
    else
    {
        getGlFunctions()->glDisable( GL_BLEND );
    }

    VerifyGLStateQt();

}

//============================================================================
void RenderGlWidget::drawGuiTexture( CGUITextureQt * guiTexture, float * x, float * y, float * z, const GoTvRect& textureRect, const GoTvRect& diffuse, int orientation )
{
    VerifyGLStateQt();

    PackedVertex vertices[ 4 ];

    // Setup texture coordinates
    //TopLeft
    vertices[ 0 ].u1 = textureRect.x1;
    vertices[ 0 ].v1 = textureRect.y1;
    //TopRight
    if( orientation & 4 )
    {
        vertices[ 1 ].u1 = textureRect.x1;
        vertices[ 1 ].v1 = textureRect.y2;
    }
    else
    {
        vertices[ 1 ].u1 = textureRect.x2;
        vertices[ 1 ].v1 = textureRect.y1;
    }
    //BottomRight
    vertices[ 2 ].u1 = textureRect.x2;
    vertices[ 2 ].v1 = textureRect.y2;
    //BottomLeft
    if( orientation & 4 )
    {
        vertices[ 3 ].u1 = textureRect.x2;
        vertices[ 3 ].v1 = textureRect.y1;
    }
    else
    {
        vertices[ 3 ].u1 = textureRect.x1;
        vertices[ 3 ].v1 = textureRect.y2;
    }

    if( guiTexture->m_diffuse.size() )
    {
        //TopLeft
        vertices[ 0 ].u2 = diffuse.x1;
        vertices[ 0 ].v2 = diffuse.y1;
        //TopRight
        if( guiTexture->m_info.orientation & 4 )
        {
            vertices[ 1 ].u2 = diffuse.x1;
            vertices[ 1 ].v2 = diffuse.y2;
        }
        else
        {
            vertices[ 1 ].u2 = diffuse.x2;
            vertices[ 1 ].v2 = diffuse.y1;
        }
        //BottomRight
        vertices[ 2 ].u2 = diffuse.x2;
        vertices[ 2 ].v2 = diffuse.y2;
        //BottomLeft
        if( guiTexture->m_info.orientation & 4 )
        {
            vertices[ 3 ].u2 = diffuse.x2;
            vertices[ 3 ].v2 = diffuse.y1;
        }
        else
        {
            vertices[ 3 ].u2 = diffuse.x1;
            vertices[ 3 ].v2 = diffuse.y2;
        }
    }

    for( int i = 0; i < 4; i++ )
    {
        vertices[ i ].x = x[ i ];
        vertices[ i ].y = y[ i ];
        vertices[ i ].z = z[ i ];
        guiTexture->m_packedVertices.push_back( vertices[ i ] );
    }

    if( ( guiTexture->m_packedVertices.size() / 4 ) > ( guiTexture->m_idx.size() / 6 ) )
    {
        uint16_t i = (uint16_t)guiTexture->m_packedVertices.size() - 4;
        guiTexture->m_idx.push_back( i + 0 );
        guiTexture->m_idx.push_back( i + 1 );
        guiTexture->m_idx.push_back( i + 2 );
        guiTexture->m_idx.push_back( i + 2 );
        guiTexture->m_idx.push_back( i + 3 );
        guiTexture->m_idx.push_back( i + 0 );
    }

    VerifyGLStateQt();
}


//============================================================================
void RenderGlWidget::endGuiTexture( CGUITextureQt * guiTexture )
{
    VerifyGLStateQt();

    if( guiTexture->m_packedVertices.size() )
    {
        GLint posLoc = shaderGetPos();
        GLint tex0Loc = shaderGetCoord0();
        GLint tex1Loc = shaderGetCoord1();
        GLint uniColLoc = shaderGetUniCol();


        if( uniColLoc >= 0 )
        {
            getGlFunctions()->glUniform4f( uniColLoc, ( guiTexture->m_col[ 0 ] / 255.0f ), ( guiTexture->m_col[ 1 ] / 255.0f ), ( guiTexture->m_col[ 2 ] / 255.0f ), ( guiTexture->m_col[ 3 ] / 255.0f ) );
        }

        if( guiTexture->m_diffuse.size() )
        {
            getGlFunctions()->glVertexAttribPointer( tex1Loc, 2, GL_FLOAT, 0, sizeof( PackedVertex ), ( char* )&guiTexture->m_packedVertices[ 0 ] + offsetof( PackedVertex, u2 ) );
            getGlFunctions()->glEnableVertexAttribArray( tex1Loc );
        }
        getGlFunctions()->glVertexAttribPointer( posLoc, 3, GL_FLOAT, 0, sizeof( PackedVertex ), ( char* )&guiTexture->m_packedVertices[ 0 ] + offsetof( PackedVertex, x ) );
        getGlFunctions()->glEnableVertexAttribArray( posLoc );
        getGlFunctions()->glVertexAttribPointer( tex0Loc, 2, GL_FLOAT, 0, sizeof( PackedVertex ), ( char* )&guiTexture->m_packedVertices[ 0 ] + offsetof( PackedVertex, u1 ) );
        getGlFunctions()->glEnableVertexAttribArray( tex0Loc );

        getGlFunctions()->glDrawElements( GL_TRIANGLES, guiTexture->m_packedVertices.size() * 6 / 4, GL_UNSIGNED_SHORT, guiTexture->m_idx.data() );

        if( guiTexture->m_diffuse.size() )
            getGlFunctions()->glDisableVertexAttribArray( tex1Loc );

        getGlFunctions()->glDisableVertexAttribArray( posLoc );
        getGlFunctions()->glDisableVertexAttribArray( tex0Loc );
    }

    if( guiTexture->m_diffuse.size() )
        getGlFunctions()->glActiveTexture( GL_TEXTURE0 );
   getGlFunctions()-> glEnable( GL_BLEND );
   VerifyGLStateQt();

}

//============================================================================
void RenderGlWidget::drawQuad( const GoTvRect &rect, GoTvColor color, CBaseTexture * texture, const GoTvRect * texCoords )
{
    VerifyGLStateQt();

    if( texture )
    {
        texture->LoadToGPU();
        texture->BindToUnit( 0 );
    }

    getGlFunctions()->glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    getGlFunctions()->glEnable( GL_BLEND );          // Turn Blending On

    VerifyGLStateQt();

    uint8_t col[ 4 ];
    GLfloat ver[ 4 ][ 3 ];
    GLfloat tex[ 4 ][ 2 ];
    uint8_t idx[ 4 ] = { 0, 1, 3, 2 };        //determines order of triangle strip

    if( texture )
        enableShader( SM_TEXTURE );
    else
        enableShader( SM_DEFAULT );

    GLint posLoc = shaderGetPos();
    GLint tex0Loc = shaderGetCoord0();
    GLint uniColLoc = shaderGetUniCol();

    getGlFunctions()->glVertexAttribPointer( posLoc, 3, GL_FLOAT, 0, 0, ver );
    if( texture )
        getGlFunctions()->glVertexAttribPointer( tex0Loc, 2, GL_FLOAT, 0, 0, tex );

   getGlFunctions()-> glEnableVertexAttribArray( posLoc );
    if( texture )
        getGlFunctions()->glEnableVertexAttribArray( tex0Loc );

    // Setup Colors
    col[ 0 ] = ( uint8_t )GET_R( color );
    col[ 1 ] = ( uint8_t )GET_G( color );
    col[ 2 ] = ( uint8_t )GET_B( color );
    col[ 3 ] = ( uint8_t )GET_A( color );

    getGlFunctions()->glUniform4f( uniColLoc, col[ 0 ] / 255.0f, col[ 1 ] / 255.0f, col[ 2 ] / 255.0f, col[ 3 ] / 255.0f );

    ver[ 0 ][ 0 ] = ver[ 3 ][ 0 ] = rect.x1;
    ver[ 0 ][ 1 ] = ver[ 1 ][ 1 ] = rect.y1;
    ver[ 1 ][ 0 ] = ver[ 2 ][ 0 ] = rect.x2;
    ver[ 2 ][ 1 ] = ver[ 3 ][ 1 ] = rect.y2;
    ver[ 0 ][ 2 ] = ver[ 1 ][ 2 ] = ver[ 2 ][ 2 ] = ver[ 3 ][ 2 ] = 0;

    if( texture )
    {
        // Setup texture coordinates
        GoTvRect coords = texCoords ? *texCoords : GoTvRect( 0.0f, 0.0f, 1.0f, 1.0f );
        tex[ 0 ][ 0 ] = tex[ 3 ][ 0 ] = coords.x1;
        tex[ 0 ][ 1 ] = tex[ 1 ][ 1 ] = coords.y1;
        tex[ 1 ][ 0 ] = tex[ 2 ][ 0 ] = coords.x2;
        tex[ 2 ][ 1 ] = tex[ 3 ][ 1 ] = coords.y2;
    }

    getGlFunctions()->glDrawElements( GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, idx );

    getGlFunctions()->glDisableVertexAttribArray( posLoc );
    if( texture )
    {
        getGlFunctions()->glDisableVertexAttribArray( tex0Loc );
    }

    VerifyGLStateQt();
}

//============================================================================
void RenderGlWidget::initTextures()
{
    if( !m_TexturesInited )
    {
        for( unsigned i = 0; i < MAX_RENDER_PLANES; ++i ) {
            getGlFunctions()->glBindTexture( m_TexDescriptor.m_Target, m_TextureIds[ i ] );
            getGlFunctions()->glTexImage2D( m_TexDescriptor.m_Target,
                                 0,
                                 m_TexDescriptor.m_InternalFormat,
                                 m_Frame->m_VisiblePitch[ i ],
                                 m_Frame->m_VisibleLines[ i ],
                                 0,
                                 m_TexDescriptor.m_Format,
                                 m_TexDescriptor.m_Type,
                                 0 );
            // Scale appropriately so we can change to target geometry without
            // much hassle.
            getGlFunctions()->glTexParameterf( m_TexDescriptor.m_Target, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
            getGlFunctions()->glTexParameterf( m_TexDescriptor.m_Target, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
            getGlFunctions()->glTexParameterf( m_TexDescriptor.m_Target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
            getGlFunctions()->glTexParameterf( m_TexDescriptor.m_Target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
            m_TexturesInited = true;
        }
    }

    for( unsigned i = 0; i < MAX_RENDER_PLANES; ++i )
    {
        getGlFunctions()->glBindTexture( m_TexDescriptor.m_Target, m_TextureIds[ i ] );

#if !defined(GL_UNPACK_ROW_LENGTH) && !defined(GL_UNPACK_ROW_LENGTH_EXT)
#define ALIGN(x, y) (((x) + ((y) - 1)) & ~((y) - 1))
        int dst_pitch = ALIGN( m_Frame->m_VisiblePitch[ i ], 4 );
        char *new_plane = ( char * )malloc( dst_pitch * m_Frame->m_VisibleLines[ i ] );
        const char *source = &m_Frame->m_PlaneData[ i ][ 0 ];
        char *destination = new_plane;
        for( int y = 0; y < m_Frame->m_VisibleLines[ i ]; y++ ) {
            memcpy( destination, source, m_Frame->m_VisiblePitch[ i ] );
            source += m_Frame->pitch[ i ];
            destination += dst_pitch;
        }

        getGlFunctions()->glTexSubImage2D( m_TexDescriptor.m_Target, 0,
                                0, 0,
                                m_Frame->m_VisiblePitch[ i ],
                                m_Frame->m_VisibleLines[ i ],
                                m_TexDescriptor.m_Format,
                                m_TexDescriptor.m_Type,
                                new_plane );
        free( new_plane );
#else
# if defined(GL_UNPACK_ROW_LENGTH)
        getGlFunctions()->glPixelStorei( GL_UNPACK_ROW_LENGTH, m_Frame->m_Pitch[ i ] );
# else
        getGlFunctions()->glPixelStorei( GL_UNPACK_ROW_LENGTH_EXT, m_Frame->m_Pitch[ i ] );
# endif // defined(GL_UNPACK_ROW_LENGTH)

        getGlFunctions()->glTexSubImage2D( m_TexDescriptor.m_Target, 0,
                                0, 0,
                                m_Frame->m_VisiblePitch[ i ],
                                m_Frame->m_VisibleLines[ i ],
                                m_TexDescriptor.m_Format,
                                m_TexDescriptor.m_Type,
                                &m_Frame->m_PlaneData[ i ][ 0 ] );
# if defined(GL_UNPACK_ROW_LENGTH)
        getGlFunctions()->glPixelStorei( GL_UNPACK_ROW_LENGTH, 0 ); // reset to default
# else
        getGlFunctions()->glPixelStorei( GL_UNPACK_ROW_LENGTH_EXT, 0 ); // reset to default
# endif // defined(GL_UNPACK_ROW_LENGTH)
#endif
    }
}
