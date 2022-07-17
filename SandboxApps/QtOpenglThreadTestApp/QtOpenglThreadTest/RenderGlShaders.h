#ifndef RENDERGLSHADERS_H
#define RENDERGLSHADERS_H

#include "RenderShaderQt.h"
//#include "EventsQtToNlc.h"
//#include "GuiInterface/INlcRender.h"
//#include "GuiInterface/INlcEvents.h"

#include <QtGui/qopenglshaderprogram.h>
#include <QtGui/qopenglfunctions.h>

typedef uint32_t NlcColor;

template <typename T> class NlcRectGen;
typedef NlcRectGen<float>  NlcRect;

class CQtTexture;
class CGUITextureQt;
class CBaseTexture;
class CGUIFontTTFQt;
class CScreenshotSurface;

class INlcRender
{
public:
    //=== textures ===//
//    virtual void                setActiveGlTexture( unsigned int activeTextureNum /* 0 == GL_TEXTURE0 , 1 == GL_TEXTURE1 etc*/ )  = 0;

//    virtual void                createTextureObject( CQtTexture * texture ) = 0;
//    virtual void                destroyTextureObject( CQtTexture * texture ) = 0;
//    virtual bool                loadToGPU( CQtTexture * texture ) = 0;
//    virtual void                bindToUnit(  CQtTexture * texture, unsigned int unit ) = 0;

//    virtual void                beginGuiTexture( CGUITextureQt * guiTexture, NlcColor color ) = 0;
//    virtual void                drawGuiTexture( CGUITextureQt * textrue, float * x, float * y, float * z, const NlcRect& texture, const NlcRect& diffuse, int orientation ) = 0;
//    virtual void                endGuiTexture( CGUITextureQt * guiTexture ) = 0;

//    virtual void                drawQuad( const NlcRect &rect, NlcColor color, CBaseTexture * texture, const NlcRect * texCoords ) = 0;

//    virtual bool                firstBegin( CGUIFontTTFQt * font ) = 0;
//    virtual void                lastEnd( CGUIFontTTFQt * font ) = 0;
//    virtual CVertexBuffer       createVertexBuffer( CGUIFontTTFQt * font, const std::vector<SVertex>& vertices) = 0;
//    virtual void                destroyVertexBuffer( CGUIFontTTFQt * font, CVertexBuffer& vertBuffer ) = 0;

//    virtual void                deleteHardwareTexture( CGUIFontTTFQt * font ) = 0;
//    virtual void                createStaticVertexBuffers( CGUIFontTTFQt * font ) = 0;
//    virtual void                destroyStaticVertexBuffers( CGUIFontTTFQt * font ) = 0;

    //=== remder ===//
//    virtual void                captureScreen( CScreenshotSurface * screenCaptrue, NlcRect& captureArea ) = 0;

//    virtual bool                initRenderSystem() = 0;
//    virtual bool                destroyRenderSystem() = 0;
//    virtual bool                resetRenderSystem( int width, int height ) = 0;

//    virtual int                 getMaxTextureSize() = 0;

//    virtual bool                beginRender() = 0;
//    virtual bool                endRender() = 0;
//    virtual void                presentRender( bool rendered, bool videoLayer ) = 0;
//    virtual bool                clearBuffers( NlcColor color ) = 0;
//    virtual bool                isExtSupported( const char* extension ) = 0;

//    virtual void                setVSync( bool vsync ) {}
//    virtual void                resetVSync() {}

//    virtual void                setViewPort( const NlcRect& viewPort ) = 0;
//    virtual void                getViewPort( NlcRect& viewPort ) = 0;

//    virtual bool                scissorsCanEffectClipping() = 0;
//    virtual NlcRect            clipRectToScissorRect( const NlcRect &rect ) = 0;
//    virtual void                setScissors( const NlcRect& rect ) = 0;
//    virtual void                resetScissors() = 0;

//    virtual void                captureStateBlock() = 0;
//    virtual void                applyStateBlock() = 0;

//    virtual void                setCameraPosition( const NlcPoint &camera, int screenWidth, int screenHeight, float stereoFactor = 0.0f ) = 0;

//    virtual void                applyHardwareTransform( const TransformMatrix &matrix ) = 0;
//    virtual void                restoreHardwareTransform() = 0;
//    virtual bool                supportsStereo( RENDER_STEREO_MODE mode ) const { return false; }

//    virtual bool                testRender() = 0;

//    virtual void                project( float &x, float &y, float &z ) = 0;



    virtual std::string         getShaderPath( const std::string &/*filename*/ ) { return ""; }

    virtual void                initialiseShaders() = 0;
    virtual void                releaseShaders() = 0;
    virtual bool                enableShader( ESHADERMETHOD method ) = 0;
    virtual bool                isShaderValid( ESHADERMETHOD method ) = 0;
    virtual void                disableShader( ESHADERMETHOD method ) = 0;
    virtual void                disableGUIShader() = 0;

    virtual int                 shaderGetPos( ) = 0;
    virtual int                 shaderGetCol( )   = 0;
    virtual int                 shaderGetModel( )   = 0;
    virtual int                 shaderGetCoord0( ) = 0;
    virtual int                 shaderGetCoord1( ) = 0;
    virtual int                 shaderGetUniCol( ) = 0;

    // yuv shader
    virtual void                shaderSetField( ESHADERMETHOD shader, int field ) = 0;
    virtual void                shaderSetWidth( ESHADERMETHOD shader, int w ) = 0;
    virtual void                shaderSetHeight( ESHADERMETHOD shader, int h ) = 0;

    virtual void                shaderSetBlack( ESHADERMETHOD shader, float black ) = 0;
    virtual void                shaderSetContrast( ESHADERMETHOD shader, float contrast ) = 0;
    virtual void                shaderSetConvertFullColorRange( ESHADERMETHOD shader, bool convertFullRange ) = 0;

    virtual int                 shaderGetVertexLoc( ESHADERMETHOD shader ) = 0;
    virtual int                 shaderGetYcoordLoc( ESHADERMETHOD shader ) = 0;
    virtual int                 shaderGetUcoordLoc( ESHADERMETHOD shader ) = 0;
    virtual int                 shaderGetVcoordLoc( ESHADERMETHOD shader ) = 0;

    virtual void                shaderSetMatrices( ESHADERMETHOD shader, const float *p, const float *m ) = 0;
    virtual void                shaderSetAlpha( ESHADERMETHOD shader, float alpha ) = 0;

    virtual void                shaderSetFlags( ESHADERMETHOD shader, unsigned int flags ) = 0;
    virtual void                shaderSetFormat( ESHADERMETHOD shader, EShaderFormat format ) = 0;
    virtual void                shaderSourceTexture( ESHADERMETHOD shader, int ytex ) = 0;
    virtual void                shaderSetStepX( ESHADERMETHOD shader, float stepX ) = 0;
    virtual void                shaderSetStepY( ESHADERMETHOD shader, float stepY ) = 0;


    // filter shader
    virtual bool                shaderGetTextureFilter( ESHADERMETHOD shader, int& filter ) = 0;
    virtual int                 shaderGetcoordLoc( ESHADERMETHOD shader ) = 0;

    // renderqt
    virtual int                 shaderVertexAttribPointer( ESHADERMETHOD shader, unsigned int index, int size, int type, bool normalized, int stride, const void * pointer ) = 0;
    virtual void                shaderEnableVertexAttribArray( ESHADERMETHOD shader, int arrayId ) = 0;
    virtual void                shaderDisableVertexAttribArray( ESHADERMETHOD shader, int arrayId ) = 0;

    // frame buffers
//    virtual void                frameBufferGen( int bufCount, unsigned int* fboId ) = 0;
//    virtual void                frameBufferDelete( int bufCount, unsigned int* fboId ) = 0;
//    virtual void                frameBufferTexture2D( int target, unsigned int texureId ) = 0;
//    virtual void                frameBufferBind( unsigned int fboId ) = 0;
//    virtual bool                frameBufferStatus( ) = 0; // return true when frame buffer completed


private:

};

class RenderGlWidget;
class RenderGlLogic;


class RenderGlShaders : public INlcRender
{
public:

#ifdef DEBUG
    void VerifyGLStateQtDbg( const char* szfile, const char* szfunction, int lineno );
#  define VerifyGLStateQt() VerifyGLStateQtDbg(__FILE__, __FUNCTION__, __LINE__)
#else
    void VerifyGLStateQt();
#endif

    RenderGlShaders( RenderGlLogic& renderGlLogic );
    virtual ~RenderGlShaders();

    // must be called from gui thread (required for linux)
    void initShaders( QOpenGLFunctions * glf );

    // cleanup shaders
    void destroyShaders();

    //! ignore from kodi
    void                        initialiseShaders() override;

    //! ignore from kodi
    void                        releaseShaders() override;

    bool                        enableShader( ESHADERMETHOD method ) override;
    bool                        isShaderValid( ESHADERMETHOD method ) override;
    void                        disableShader( ESHADERMETHOD method ) override;
    void                        disableGUIShader() override;

    int                         shaderGetPos()  override;
    int                         shaderGetCol()  override;
    int                         shaderGetModel()  override;
    int                         shaderGetCoord0()  override;
    int                         shaderGetCoord1()  override;
    int                         shaderGetUniCol()  override;

    // yuv shader
    virtual void                shaderSetField( ESHADERMETHOD shader, int field )   override;
    virtual void                shaderSetWidth( ESHADERMETHOD shader, int w )   override;
    virtual void                shaderSetHeight( ESHADERMETHOD shader, int h )  override;

    virtual void                shaderSetBlack( ESHADERMETHOD shader, float black ) override;
    virtual void                shaderSetContrast( ESHADERMETHOD shader, float contrast ) override;
    virtual void                shaderSetConvertFullColorRange( ESHADERMETHOD shader, bool convertFullRange ) override;

    virtual int                 shaderGetVertexLoc( ESHADERMETHOD shader ) override;
    virtual int                 shaderGetYcoordLoc( ESHADERMETHOD shader ) override;
    virtual int                 shaderGetUcoordLoc( ESHADERMETHOD shader ) override;
    virtual int                 shaderGetVcoordLoc( ESHADERMETHOD shader ) override;

    virtual void                shaderSetMatrices( ESHADERMETHOD shader, const float *p, const float *m ) override;
    virtual void                shaderSetAlpha( ESHADERMETHOD shader, float alpha ) override;

    virtual void                shaderSetFlags( ESHADERMETHOD shader, unsigned int flags ) override;
    virtual void                shaderSetFormat( ESHADERMETHOD shader, EShaderFormat format ) override;
    virtual void                shaderSourceTexture( ESHADERMETHOD shader, int ytex ) override;
    virtual void                shaderSetStepX( ESHADERMETHOD shader, float stepX ) override;
    virtual void                shaderSetStepY( ESHADERMETHOD shader, float stepY )  override;

    // filter shader
    virtual bool                shaderGetTextureFilter( ESHADERMETHOD shader, int& filter ) override;
    virtual int                 shaderGetcoordLoc( ESHADERMETHOD shader ) override;

    // renderqt
    virtual int                 shaderVertexAttribPointer( ESHADERMETHOD shader, unsigned int index, int size, int type, bool normalized, int stride, const void * pointer ) override;
    virtual void                shaderEnableVertexAttribArray( ESHADERMETHOD shader, int arrayId ) override;
    virtual void                shaderDisableVertexAttribArray( ESHADERMETHOD shader, int arrayId ) override;

protected:
    void                        getShaderSourceCode( int shaderIdx, EShaderType& shaderType, std::string& shaderName, std::string& vertexShaderCode, std::string& fragmentShaderCode );
    void                        compileShader( int shaderIdx );

    RenderGlLogic&              m_RenderGlWidget;
    // shaders
    QOpenGLFunctions *          m_Glf = nullptr;
    RenderShaderQt *            m_Shaders[ SM_MAX ];
    bool                        m_ShadersInited = false;
    ESHADERMETHOD               m_CurShaderMethodType = SM_DEFAULT;
};

#endif // RENDERGLSHADERS_H
