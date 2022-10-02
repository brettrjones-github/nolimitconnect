
//#define SHADERS_INCLUDE
#if defined(SHADERS_INCLUDE)

static const char* gles_shader_vert =
"attribute vec4 m_attrpos;\n"
"attribute vec4 m_attrcol;\n"
"attribute vec4 m_attrcord0;\n"
"attribute vec4 m_attrcord1;\n"
"uniform mat4 m_proj;\n"
"uniform mat4 m_model;\n"
"uniform mat4 m_coord0Matrix;\n"
"varying vec4 m_cord0;\n"
"varying vec4 m_cord1;\n"
"varying lowp vec4 m_colour;\n"
"\n"
"void main()\n"
"{\n"
"    mat4 mvp = m_proj * m_model;\n"
"    gl_Position = mvp * m_attrpos;\n"
"    m_colour = m_attrcol;\n"
"    m_cord0 = m_coord0Matrix * m_attrcord0;\n"
"    m_cord1 = m_attrcord1;\n"
"}\n";

// SM_DEFAULT
#if defined(TARGET_OS_WINDOWS)
static const char* gles_shader_default_frag =
"precision mediump float;\n"
#else
static const char* gles_shader_default_frag =
#endif
"uniform lowp vec4 m_unicol;\n"
"void main()\n"
"{\n"
" vec4 rgb;\n"
"\n"
" rgb = m_unicol;\n"
"\n"
"#if defined(KODI_LIMITED_RANGE)\n"
" rgb.rgb *= ( 235.0 - 16.0 ) / 255.0;\n"
" rgb.rgb += 16.0 / 255.0;\n"
"#endif\n"
"\n"
" gl_FragColor = rgb;\n"
"}\n";


// SM_TEXTURE shader
#if defined(TARGET_OS_WINDOWS)
static const char* gles_shader_texture_frag =
"precision mediump   float;\n"
#else
static const char* gles_shader_texture_frag =
#endif
"uniform sampler2D m_samp0;\n"
"uniform lowp vec4 m_unicol;\n"
"varying vec4 m_cord0;\n"
"\n"
"void main()\n"
"{\n"
"    vec4 rgb;\n"
"\n"
"    rgb = texture2D( m_samp0, m_cord0.xy ).rgba * m_unicol;\n"
"\n"
"#if defined(KODI_LIMITED_RANGE)\n"
"    rgb.rgb *= ( 235.0 - 16.0 ) / 255.0;\n"
"    rgb.rgb += 16.0 / 255.0;\n"
"#endif\n"
"\n"
"    gl_FragColor = rgb;\n"
"}\n";

// SM_MULTI shader;
#if defined(TARGET_OS_WINDOWS)
static const char* gles_shader_multi_frag =
"precision mediump   float;\n"
#else
static const char* gles_shader_multi_frag =
#endif
"uniform sampler2D m_samp0;\n"
"uniform sampler2D m_samp1;\n"
"varying vec4 m_cord0;\n"
"varying vec4 m_cord1;\n"
"\n"
"void main()\n"
"{\n"
"    vec4 rgb;\n"
"\n"
"    rgb = texture2D( m_samp0, m_cord0.xy ) * texture2D( m_samp1, m_cord1.xy );\n"
"\n"
"#if defined(KODI_LIMITED_RANGE)\n"
"    rgb.rgb *= ( 235.0 - 16.0 ) / 255.0;\n"
"    rgb.rgb += 16.0 / 255.0;\n"
"#endif\n"
"\n"
"    gl_FragColor = rgb;\n"
"}\n";

// SM_FONTS
#if defined(TARGET_OS_WINDOWS)
static const char* gles_shader_fonts_frag =
"precision mediump   float;\n"
#else
static const char* gles_shader_fonts_frag =
#endif
"uniform   sampler2D m_samp0;\n"
"varying   vec4      m_cord0;\n"
"varying   lowp vec4 m_colour;\n"
"\n"
"void main()\n"
"{\n"
" vec4 rgb;\n"
"\n"
" rgb.rgb = m_colour.rgb;\n"
" rgb.a = m_colour.a * texture2D( m_samp0, m_cord0.xy ).a;\n"
"\n"
"#if defined(KODI_LIMITED_RANGE)\n"
" rgb.rgb *= ( 235.0 - 16.0 ) / 255.0;\n"
" rgb.rgb += 16.0 / 255.0;\n"
"#endif\n"
"\n"
" gl_FragColor = rgb;\n"
"}\n";

// SM_TEXTURE_NOBLEND
#if defined(TARGET_OS_WINDOWS)
static const char* gles_shader_texture_noblend_frag =
"precision mediump   float;\n"
#else
static const char* gles_shader_texture_noblend_frag =
#endif
"uniform sampler2D m_samp0;\n"
"varying vec4 m_cord0;\n"
"\n"
"void main()\n"
"{\n"
"    vec4 rgb;\n"
"\n"
"    rgb = texture2D( m_samp0, m_cord0.xy );\n"
"\n"
"#if defined(KODI_LIMITED_RANGE)\n"
"    rgb.rgb *= ( 235.0 - 16.0 ) / 255.0;\n"
"    rgb.rgb += 16.0 / 255.0;\n"
"#endif\n"
"\n"
"    gl_FragColor = rgb;\n"
"}\n";

// SM_MULTI_BLENDCOLOR shader
#if defined(TARGET_OS_WINDOWS)
static const char* gles_shader_multi_blendcolor_frag =
"precision mediump   float;\n"
#else
static const char* gles_shader_multi_blendcolor_frag =
#endif
"uniform sampler2D m_samp0;\n"
"uniform sampler2D m_samp1;\n"
"varying vec4 m_cord0;\n"
"varying vec4 m_cord1;\n"
"uniform lowp vec4 m_unicol;\n"
"\n"
"void main()\n"
"{\n"
"    vec4 rgb;\n"
"\n"
"    rgb = m_unicol * texture2D( m_samp0, m_cord0.xy ) * texture2D( m_samp1, m_cord1.xy );\n"
"\n"
"#if defined(KODI_LIMITED_RANGE)\n"
"    rgb.rgb *= ( 235.0 - 16.0 ) / 255.0;\n"
"    rgb.rgb += 16.0 / 255.0;\n"
"#endif\n"
"\n"
"    gl_FragColor = rgb;\n"
"}\n";

// SM_TEXTURE_RGBA
#if defined(TARGET_OS_WINDOWS)
static const char* gles_shader_rgba_frag =
"precision mediump   float;\n"
#else
static const char* gles_shader_rgba_frag =
#endif
"uniform sampler2D m_samp0;\n"
"uniform sampler2D m_samp1;\n"
"varying vec4      m_cord0;\n"
"varying vec4      m_cord1;\n"
"varying lowp vec4 m_colour;\n"
"uniform int       m_method;\n"
"uniform float     m_brightness;\n"
"uniform float     m_contrast;\n"
"\n"
"void main()\n"
"{\n"
"    vec4 rgb;\n"
"\n"
"    rgb = texture2D( m_samp0, m_cord0.xy );\n"
"    rgb *= m_contrast;\n"
"    rgb += m_brightness;\n"
"\n"
"    gl_FragColor = rgb;\n"
"}\n";

// SM_TEXTURE_RGBA_OES
#if defined(TARGET_OS_WINDOWS)
static const char* gles_shader_rgba_oes_frag =
"#extension GL_OES_EGL_image_external : require\n"
"\n"
"precision mediump   float;\n"
#else
static const char* gles_shader_rgba_oes_frag =
"#extension GL_OES_EGL_image_external : require\n"
"\n"
#endif
"uniform samplerExternalOES m_samp0;\n"
"varying vec4      m_cord0;\n"
"\n"
"uniform float     m_brightness;\n"
"uniform float     m_contrast;\n"
"\n"
"void main ()\n"
"{\n"
"    vec4 rgb;\n"
"\n"
"    rgb = texture2D( m_samp0, m_cord0.xy );\n"
"    rgb *= m_contrast;\n"
"    rgb += m_brightness;\n"
"\n"
"    gl_FragColor = rgb;\n"
"}\n";

// SM_TEXTURE_RGBA_BLENDCOLOR
#if defined(TARGET_OS_WINDOWS)
static const char* gles_shader_rgba_blendcolor_frag =
"precision mediump   float;\n"
#else
static const char* gles_shader_rgba_blendcolor_frag =
#endif
"uniform sampler2D m_samp0;\n"
"varying vec4 m_cord0;\n"
"varying vec4 m_cord1;\n"
"varying lowp vec4 m_colour;\n"
"uniform int m_method;\n"
"\n"
"void main()\n"
"{\n"
"    vec4 rgb;\n"
"\n"
"    rgb = texture2D( m_samp0, m_cord0.xy ).rgba * m_colour;\n"
"\n"
"#if defined(KODI_LIMITED_RANGE)\n"
"    rgb.rgb *= ( 235.0 - 16.0 ) / 255.0;\n"
"#endif\n"
"\n"
"    gl_FragColor = rgb;\n"
"}\n";

// SM_TEXTURE_RGBA_BOB
#if defined(TARGET_OS_WINDOWS)
static const char* gles_shader_rgba_bob_frag =
"precision highp   float;\n"
#else
static const char* gles_shader_rgba_bob_frag =
#endif
"uniform sampler2D m_samp0;\n"
"uniform sampler2D m_samp1;\n"
"varying vec4      m_cord0;\n"
"varying vec4      m_cord1;\n"
"varying lowp vec4 m_colour;\n"
"uniform int       m_method;\n"
"uniform int       m_field;\n"
"uniform float     m_step;\n"
"\n"
"uniform float     m_brightness;\n"
"uniform float     m_contrast;\n"
"\n"
"void main()\n"
"{\n"
"    vec2 source;\n"
"    source = m_cord0.xy;\n"
"\n"
"    float temp1 = mod( source.y, 2.0*m_step );\n"
"    float temp2 = source.y - temp1;\n"
"    source.y = temp2 + m_step / 2.0 - float( m_field )*m_step;\n"
"\n"
"    // Blend missing line\n"
"    vec2 below;\n"
"    float bstep = step( m_step, temp1 );\n"
"    below.x = source.x;\n"
"    below.y = source.y + ( 2.0*m_step*bstep );\n"
"\n"
"    vec4 color = mix( texture2D( m_samp0, source ), texture2D( m_samp0, below ), 0.5 );\n"
"    color = color * m_contrast;\n"
"    color = color + m_brightness;\n"
"\n"
"    gl_FragColor = color;\n"
"}\n";

// SM_TEXTURE_RGBA_BOB_OES
#if defined(TARGET_OS_WINDOWS)
static const char* gles_shader_rgba_bob_oes_frag =
"#extension GL_OES_EGL_image_external : require\n"
"precision highp   float;\n"
#else
static const char* gles_shader_rgba_bob_oes_frag =
"#extension GL_OES_EGL_image_external : require\n"
#endif
"uniform samplerExternalOES m_samp0;\n"
"uniform samplerExternalOES m_samp1;\n"
"varying vec4 m_cord0;\n"
"varying vec4 m_cord1;\n"
"varying lowp vec4 m_colour;\n"
"uniform int m_method;\n"
"uniform int m_field;\n"
"uniform float m_step;\n"
"uniform float m_brightness;\n"
"uniform float m_contrast;\n"
"\n"
"void main()\n"
"{\n"
"    vec2 source;\n"
"    source = m_cord0.xy;\n"
"\n"
"    float temp1 = mod( source.y, 2.0 * m_step );\n"
"    float temp2 = source.y - temp1;\n"
"    source.y = temp2 + m_step / 2.0 - float( m_field ) * m_step;\n"
"\n"
"    // Blend missing line\n"
"    vec2 below;\n"
"    float bstep = step( m_step, temp1 );\n"
"    below.x = source.x;\n"
"    below.y = source.y + ( 2.0*m_step * bstep );\n"
"\n"
"    vec4 color = mix( texture2D( m_samp0, source ), texture2D( m_samp0, below ), 0.5 );\n"
"    color *= m_contrast;\n"
"    color += m_brightness;\n"
"\n"
"    gl_FragColor = color;\n"
"}\n"
"\n";
//============================================================================
// video format
//============================================================================
// SM_VIDEO_YV12_BASIC // SM_VIDEO_NV12_BASIC // SM_VIDEO_YUY2_BASIC // SM_VIDEO_UYVY_BASIC // SM_VIDEO_NV12_RGB_BASIC
static const char* gles_yuv2rgb_vert =
"uniform mat4 m_proj;\n"
"uniform mat4 m_model;\n"
"attribute vec4 m_attrpos;\n"
"attribute vec2 m_attrcordY;\n"
"attribute vec2 m_attrcordU;\n"
"attribute vec2 m_attrcordV;\n"
"varying vec2 m_cordY;\n"
"varying vec2 m_cordU;\n"
"varying vec2 m_cordV;\n"
"void main()\n"
"{\n"
"    mat4 mvp = m_proj * m_model;\n"
"    gl_Position = mvp * m_attrpos;\n"
"    m_cordY = m_attrcordY;\n"
"    m_cordU = m_attrcordU;\n"
"    m_cordV = m_attrcordV;\n"
"}\n";

// most SM_VIDEO_XXXX_BASIC fragments
#if defined(TARGET_OS_WINDOWS)
static const char* gles_yuv2rgb_basic_frag =
"precision mediump float;\n"
#else
static const char* gles_yuv2rgb_basic_frag =
#endif
"uniform sampler2D m_sampY;\n"
"uniform sampler2D m_sampU;\n"
"uniform sampler2D m_sampV;\n"
"varying vec2 m_cordY;\n"
"varying vec2 m_cordU;\n"
"varying vec2 m_cordV;\n"
"uniform vec2 m_step;\n"
"uniform mat4 m_yuvmat;\n"
"uniform mat3 m_primMat;\n"
"uniform float m_gammaDstInv;\n"
"uniform float m_gammaSrc;\n"
"uniform float m_toneP1;\n"
"uniform vec3 m_coefsDst;\n"
"uniform float m_alpha;\n"
"\n"
"void main()\n"
"{\n"
"    vec4 rgb;\n"
"    vec4 yuv;\n"
"\n"
"#if defined(XBMC_YV12) || defined(XBMC_NV12)\n"
"\n"
"    yuv = vec4( texture2D( m_sampY, m_cordY ).r,\n"
"        texture2D( m_sampU, m_cordU ).g,\n"
"        texture2D( m_sampV, m_cordV ).a,\n"
"        1.0 );\n"
"\n"
"#elif defined(XBMC_NV12_RRG)\n"
"\n"
"    yuv = vec4( texture2D( m_sampY, m_cordY ).r,\n"
"        texture2D( m_sampU, m_cordU ).r,\n"
"        texture2D( m_sampV, m_cordV ).g,\n"
"        1.0 );\n"
"\n"
"#endif\n"
"\n"
"    rgb = m_yuvmat * yuv;\n"
"    rgb.a = m_alpha;\n"
"\n"
"#if defined(XBMC_COL_CONVERSION)\n"
"    rgb.rgb = pow( max( vec3( 0 ), rgb.rgb ), vec3( m_gammaSrc ) );\n"
"    rgb.rgb = max( vec3( 0 ), m_primMat * rgb.rgb );\n"
"    rgb.rgb = pow( rgb.rgb, vec3( m_gammaDstInv ) );\n"
"\n"
"#if defined(XBMC_TONE_MAPPING)\n"
"    float luma = dot( rgb.rgb, m_coefsDst );\n"
"    rgb.rgb *= tonemap( luma ) / luma;\n"
"#endif\n"
"\n"
"#endif\n"
"\n"
"    gl_FragColor = rgb;\n"
"}\n";


// SM_VIDEO_YV12_BOB
#if defined(TARGET_OS_WINDOWS)
static const char* gles_yuv2rgb_bob_frag =
"precision highp float;\n"
#else
static const char* gles_yuv2rgb_bob_frag =
#endif
"uniform sampler2D m_sampY;\n"
"uniform sampler2D m_sampU;\n"
"uniform sampler2D m_sampV;\n"
"varying vec2 m_cordY;\n"
"varying vec2 m_cordU;\n"
"varying vec2 m_cordV;\n"
"uniform float m_alpha;\n"
"uniform mat4 m_yuvmat;\n"
"uniform float m_stepX;\n"
"uniform float m_stepY;\n"
"uniform int m_field;\n"
"uniform mat3 m_primMat;\n"
"uniform float m_gammaDstInv;\n"
"uniform float m_gammaSrc;\n"
"uniform float m_toneP1;\n"
"uniform vec3 m_coefsDst;\n"
"\n"
"void main()\n"
"{\n"
"    vec4 rgb;\n"
"\n"
"    vec2 offsetY;\n"
"    vec2 offsetU;\n"
"    vec2 offsetV;\n"
"    float temp1 = mod( m_cordY.y, 2.0 * m_stepY );\n"
"\n"
"    offsetY = m_cordY;\n"
"    offsetU = m_cordU;\n"
"    offsetV = m_cordV;\n"
"\n"
"    offsetY.y -= ( temp1 - m_stepY / 2.0 + float( m_field ) * m_stepY );\n"
"    offsetU.y -= ( temp1 - m_stepY / 2.0 + float( m_field ) * m_stepY ) / 2.0;\n"
"    offsetV.y -= ( temp1 - m_stepY / 2.0 + float( m_field ) * m_stepY ) / 2.0;\n"
"\n"
"    float bstep = step( m_stepY, temp1 );\n"
"\n"
"    // Blend missing line\n"
"    vec2 belowY, belowU, belowV;\n"
"\n"
"    belowY.x = offsetY.x;\n"
"    belowY.y = offsetY.y + ( 2.0 * m_stepY * bstep );\n"
"    belowU.x = offsetU.x;\n"
"    belowU.y = offsetU.y + ( m_stepY * bstep );\n"
"    belowV.x = offsetV.x;\n"
"    belowV.y = offsetV.y + ( m_stepY * bstep );\n"
"\n"
"    vec4 rgbAbove;\n"
"    vec4 rgbBelow;\n"
"    vec4 yuvAbove;\n"
"    vec4 yuvBelow;\n"
"\n"
"    yuvAbove = vec4( texture2D( m_sampY, offsetY ).r, texture2D( m_sampU, offsetU ).g, texture2D( m_sampV, offsetV ).a, 1.0 );\n"
"    rgbAbove = m_yuvmat * yuvAbove;\n"
"    rgbAbove.a = m_alpha;\n"
"\n"
"    yuvBelow = vec4( texture2D( m_sampY, belowY ).r, texture2D( m_sampU, belowU ).g, texture2D( m_sampV, belowV ).a, 1.0 );\n"
"    rgbBelow = m_yuvmat * yuvBelow;\n"
"    rgbBelow.a = m_alpha;\n"
"\n"
"    rgb = mix( rgb, rgbBelow, 0.5 );\n"
"\n"
"#if defined(XBMC_COL_CONVERSION)\n"
"    rgb.rgb = pow( max( vec3( 0 ), rgb.rgb ), vec3( m_gammaSrc ) );\n"
"    rgb.rgb = max( vec3( 0 ), m_primMat * rgb.rgb );\n"
"    rgb.rgb = pow( rgb.rgb, vec3( m_gammaDstInv ) );\n"
"\n"
"#if defined(XBMC_TONE_MAPPING)\n"
"    float luma = dot( rgb.rgb, m_coefsDst );\n"
"    rgb.rgb *= tonemap( luma ) / luma;\n"
"#endif\n"
"\n"
"#endif\n"
"\n"
"    gl_FragColor = rgb;\n"
"}\n";

//============================================================================
// video filter
//============================================================================
// SM_VID_FILTER_DEFAULT
static const char* gles_videofilter_vert =
"uniform mat4 m_proj;\n"
"uniform mat4 m_model;\n"
"attribute vec4 m_attrpos;\n"
"attribute vec2 m_attrcord;\n"
"varying vec2 cord;\n"
"void main()\n"
"{\n"
"    mat4 mvp = m_proj * m_model;\n"
"    gl_Position = mvp * m_attrpos;\n"
"    cord = m_attrcord.xy;\n"
"}\n";

// SM_VID_FILTER_DEFAULT
#if defined(TARGET_OS_WINDOWS)
static const char* gles_videofilter_frag =
"precision mediump float;\n"
#else
static const char* gles_videofilter_frag =
#endif
"uniform sampler2D img;\n"
"varying vec2 cord;\n"
"void main()\n"
"{\n"
"    gl_FragColor = texture2D( img, cord );\n"
"}\n";

// SM_VID_FILTER_CONVOLUTION_4X4_RGBA
#if defined(TARGET_OS_WINDOWS)
static const char* gles_convolution_4x4_rgba_frag =
"precision highp float;\n"
#else
static const char* gles_convolution_4x4_rgba_frag =
#endif
"#define HAS_FLOAT_TEXTURE 0\n"
"uniform sampler2D img;\n"
"uniform vec2      stepxy;\n"
"varying vec2      cord;\n"
"uniform float     m_alpha;\n"
"uniform sampler2D kernelTex;\n"
"vec4 weight( float pos )\n"
"{\n"
"#if (HAS_FLOAT_TEXTURE)\n"
"    return texture2D( kernelTex, vec2( pos - 0.5 ) );\n"
"#else\n"
"    return texture2D( kernelTex, vec2( pos - 0.5 ) ) * 2.0 - 1.0;\n"
"#endif\n"
"}\n"
"vec3 pixel( float xpos, float ypos )\n"
"{\n"
"    return texture2D( img, vec2( xpos, ypos ) ).rgb;\n"
"}\n"
"vec3 line( float ypos, vec4 xpos, vec4 linetaps )\n"
"{\n"
"    return\n"
"        pixel( xpos.r, ypos ) * linetaps.r +\n"
"        pixel( xpos.g, ypos ) * linetaps.g +\n"
"        pixel( xpos.b, ypos ) * linetaps.b +\n"
"        pixel( xpos.a, ypos ) * linetaps.a;\n"
"}\n"
"void main()\n"
"{\n"
"    vec4 rgb;\n"
"    vec2 pos = cord + stepxy * 0.5;\n"
"    vec2 f = fract( pos / stepxy );\n"
"    vec4 linetaps = weight( 1.0 - f.x );\n"
"    vec4 columntaps = weight( 1.0 - f.y );\n"
"    //make sure all taps added together is exactly 1.0, otherwise some (very small) distortion can occur\n"
"    linetaps /= linetaps.r + linetaps.g + linetaps.b + linetaps.a;\n"
"    columntaps /= columntaps.r + columntaps.g + columntaps.b + columntaps.a;\n"
"    vec2 xystart = ( -1.5 - f ) * stepxy + pos;\n"
"    vec4 xpos = vec4( xystart.x, xystart.x + stepxy.x, xystart.x + stepxy.x * 2.0, xystart.x + stepxy.x * 3.0 );\n"
"    rgb.rgb =\n"
"        line( xystart.y, xpos, linetaps ) * columntaps.r +\n"
"        line( xystart.y + stepxy.y, xpos, linetaps ) * columntaps.g +\n"
"        line( xystart.y + stepxy.y * 2.0, xpos, linetaps ) * columntaps.b +\n"
"        line( xystart.y + stepxy.y * 3.0, xpos, linetaps ) * columntaps.a;\n"
"    rgb.a = m_alpha;\n"
"    gl_FragColor = rgb;\n"
"}\n";

// SM_VID_FILTER_CONVOLUTION_4X4_FLOAT
#if defined(TARGET_OS_WINDOWS)
static const char* gles_convolution_4x4_float_frag =
"precision highp float;\n"
#else
static const char* gles_convolution_4x4_float_frag =
#endif
"#define HAS_FLOAT_TEXTURE 1\n"
"uniform sampler2D img;\n"
"uniform vec2      stepxy;\n"
"varying vec2      cord;\n"
"uniform float     m_alpha;\n"
"uniform sampler2D kernelTex;\n"
"vec4 weight( float pos )\n"
"{\n"
"#if (HAS_FLOAT_TEXTURE)\n"
"    return texture2D( kernelTex, vec2( pos - 0.5 ) );\n"
"#else\n"
"    return texture2D( kernelTex, vec2( pos - 0.5 ) ) * 2.0 - 1.0;\n"
"#endif\n"
"}\n"
"vec3 pixel( float xpos, float ypos )\n"
"{\n"
"    return texture2D( img, vec2( xpos, ypos ) ).rgb;\n"
"}\n"
"vec3 line( float ypos, vec4 xpos, vec4 linetaps )\n"
"{\n"
"    return\n"
"        pixel( xpos.r, ypos ) * linetaps.r +\n"
"        pixel( xpos.g, ypos ) * linetaps.g +\n"
"        pixel( xpos.b, ypos ) * linetaps.b +\n"
"        pixel( xpos.a, ypos ) * linetaps.a;\n"
"}\n"
"void main()\n"
"{\n"
"    vec4 rgb;\n"
"    vec2 pos = cord + stepxy * 0.5;\n"
"    vec2 f = fract( pos / stepxy );\n"
"    vec4 linetaps = weight( 1.0 - f.x );\n"
"    vec4 columntaps = weight( 1.0 - f.y );\n"
"    //make sure all taps added together is exactly 1.0, otherwise some (very small) distortion can occur\n"
"    linetaps /= linetaps.r + linetaps.g + linetaps.b + linetaps.a;\n"
"    columntaps /= columntaps.r + columntaps.g + columntaps.b + columntaps.a;\n"
"    vec2 xystart = ( -1.5 - f ) * stepxy + pos;\n"
"    vec4 xpos = vec4( xystart.x, xystart.x + stepxy.x, xystart.x + stepxy.x * 2.0, xystart.x + stepxy.x * 3.0 );\n"
"    rgb.rgb =\n"
"        line( xystart.y, xpos, linetaps ) * columntaps.r +\n"
"        line( xystart.y + stepxy.y, xpos, linetaps ) * columntaps.g +\n"
"        line( xystart.y + stepxy.y * 2.0, xpos, linetaps ) * columntaps.b +\n"
"        line( xystart.y + stepxy.y * 3.0, xpos, linetaps ) * columntaps.a;\n"
"    rgb.a = m_alpha;\n"
"    gl_FragColor = rgb;\n"
"}\n";

// SM_VID_FILTER_CONVOLUTION_6X6_RGBA
#if defined(TARGET_OS_WINDOWS)
static const char* gles_convolution_6x6_rgba_frag =
"precision highp float;\n"
#else
static const char* gles_convolution_6x6_rgba_frag =
#endif
"#define HAS_FLOAT_TEXTURE 0\n"
"uniform sampler2D img;\n"
"uniform vec2      stepxy;\n"
"varying vec2      cord;\n"
"uniform float     m_alpha;\n"
"uniform sampler2D kernelTex;\n"
"vec3 weight( float pos )\n"
"{\n"
"#if (HAS_FLOAT_TEXTURE)\n"
"    return texture2D( kernelTex, vec2( pos - 0.5 ) ).rgb;\n"
"#else\n"
"    return texture2D( kernelTex, vec2( pos - 0.5 ) ).rgb * 2.0 - 1.0;\n"
"#endif\n"
"}\n"
"vec3 pixel( float xpos, float ypos )\n"
"{\n"
"    return texture2D( img, vec2( xpos, ypos ) ).rgb;\n"
"}\n"
"vec3 line( float ypos, vec3 xpos1, vec3 xpos2, vec3 linetaps1, vec3 linetaps2 )\n"
"{\n"
"    return\n"
"        pixel( xpos1.r, ypos ) * linetaps1.r +\n"
"        pixel( xpos1.g, ypos ) * linetaps2.r +\n"
"        pixel( xpos1.b, ypos ) * linetaps1.g +\n"
"        pixel( xpos2.r, ypos ) * linetaps2.g +\n"
"        pixel( xpos2.g, ypos ) * linetaps1.b +\n"
"        pixel( xpos2.b, ypos ) * linetaps2.b;\n"
"}\n"
"void main()\n"
"{\n"
"    vec4 rgb;\n"
"    vec2 pos = cord + stepxy * 0.5;\n"
"    vec2 f = fract( pos / stepxy );\n"
"    vec3 linetaps1 = weight( ( 1.0 - f.x ) / 2.0 );\n"
"    vec3 linetaps2 = weight( ( 1.0 - f.x ) / 2.0 + 0.5 );\n"
"    vec3 columntaps1 = weight( ( 1.0 - f.y ) / 2.0 );\n"
"    vec3 columntaps2 = weight( ( 1.0 - f.y ) / 2.0 + 0.5 );\n"
"    //make sure all taps added together is exactly 1.0, otherwise some (very small) distortion can occur\n"
"    float sum = linetaps1.r + linetaps1.g + linetaps1.b + linetaps2.r + linetaps2.g + linetaps2.b;\n"
"    linetaps1 /= sum;\n"
"    linetaps2 /= sum;\n"
"    sum = columntaps1.r + columntaps1.g + columntaps1.b + columntaps2.r + columntaps2.g + columntaps2.b;\n"
"    columntaps1 /= sum;\n"
"    columntaps2 /= sum;\n"
"    vec2 xystart = ( -2.5 - f ) * stepxy + pos;\n"
"    vec3 xpos1 = vec3( xystart.x, xystart.x + stepxy.x, xystart.x + stepxy.x * 2.0 );\n"
"    vec3 xpos2 = vec3( xystart.x + stepxy.x * 3.0, xystart.x + stepxy.x * 4.0, xystart.x + stepxy.x * 5.0 );\n"
"    rgb.rgb =\n"
"        line( xystart.y, xpos1, xpos2, linetaps1, linetaps2 ) * columntaps1.r +\n"
"        line( xystart.y + stepxy.y, xpos1, xpos2, linetaps1, linetaps2 ) * columntaps2.r +\n"
"        line( xystart.y + stepxy.y * 2.0, xpos1, xpos2, linetaps1, linetaps2 ) * columntaps1.g +\n"
"        line( xystart.y + stepxy.y * 3.0, xpos1, xpos2, linetaps1, linetaps2 ) * columntaps2.g +\n"
"        line( xystart.y + stepxy.y * 4.0, xpos1, xpos2, linetaps1, linetaps2 ) * columntaps1.b +\n"
"        line( xystart.y + stepxy.y * 5.0, xpos1, xpos2, linetaps1, linetaps2 ) * columntaps2.b;\n"
"    rgb.a = m_alpha;\n"
"    gl_FragColor = rgb;\n"
"}\n";

// SM_VID_FILTER_CONVOLUTION_6X6_FLOAT
#if defined(TARGET_OS_WINDOWS)
static const char* gles_convolution_6x6_float_frag =
"precision highp float;\n"
#else
static const char* gles_convolution_6x6_float_frag =
#endif
"#define HAS_FLOAT_TEXTURE 1\n"
"uniform sampler2D img;\n"
"uniform vec2      stepxy;\n"
"varying vec2      cord;\n"
"uniform float     m_alpha;\n"
"uniform sampler2D kernelTex;\n"
"vec3 weight( float pos )\n"
"{\n"
"#if (HAS_FLOAT_TEXTURE)\n"
"    return texture2D( kernelTex, vec2( pos - 0.5 ) ).rgb;\n"
"#else\n"
"    return texture2D( kernelTex, vec2( pos - 0.5 ) ).rgb * 2.0 - 1.0;\n"
"#endif\n"
"}\n"
"vec3 pixel( float xpos, float ypos )\n"
"{\n"
"    return texture2D( img, vec2( xpos, ypos ) ).rgb;\n"
"}\n"
"vec3 line( float ypos, vec3 xpos1, vec3 xpos2, vec3 linetaps1, vec3 linetaps2 )\n"
"{\n"
"    return\n"
"        pixel( xpos1.r, ypos ) * linetaps1.r +\n"
"        pixel( xpos1.g, ypos ) * linetaps2.r +\n"
"        pixel( xpos1.b, ypos ) * linetaps1.g +\n"
"        pixel( xpos2.r, ypos ) * linetaps2.g +\n"
"        pixel( xpos2.g, ypos ) * linetaps1.b +\n"
"        pixel( xpos2.b, ypos ) * linetaps2.b;\n"
"}\n"
"void main()\n"
"{\n"
"    vec4 rgb;\n"
"    vec2 pos = cord + stepxy * 0.5;\n"
"    vec2 f = fract( pos / stepxy );\n"
"    vec3 linetaps1 = weight( ( 1.0 - f.x ) / 2.0 );\n"
"    vec3 linetaps2 = weight( ( 1.0 - f.x ) / 2.0 + 0.5 );\n"
"    vec3 columntaps1 = weight( ( 1.0 - f.y ) / 2.0 );\n"
"    vec3 columntaps2 = weight( ( 1.0 - f.y ) / 2.0 + 0.5 );\n"
"    //make sure all taps added together is exactly 1.0, otherwise some (very small) distortion can occur\n"
"    float sum = linetaps1.r + linetaps1.g + linetaps1.b + linetaps2.r + linetaps2.g + linetaps2.b;\n"
"    linetaps1 /= sum;\n"
"    linetaps2 /= sum;\n"
"    sum = columntaps1.r + columntaps1.g + columntaps1.b + columntaps2.r + columntaps2.g + columntaps2.b;\n"
"    columntaps1 /= sum;\n"
"    columntaps2 /= sum;\n"
"    vec2 xystart = ( -2.5 - f ) * stepxy + pos;\n"
"    vec3 xpos1 = vec3( xystart.x, xystart.x + stepxy.x, xystart.x + stepxy.x * 2.0 );\n"
"    vec3 xpos2 = vec3( xystart.x + stepxy.x * 3.0, xystart.x + stepxy.x * 4.0, xystart.x + stepxy.x * 5.0 );\n"
"    rgb.rgb =\n"
"        line( xystart.y, xpos1, xpos2, linetaps1, linetaps2 ) * columntaps1.r +\n"
"        line( xystart.y + stepxy.y, xpos1, xpos2, linetaps1, linetaps2 ) * columntaps2.r +\n"
"        line( xystart.y + stepxy.y * 2.0, xpos1, xpos2, linetaps1, linetaps2 ) * columntaps1.g +\n"
"        line( xystart.y + stepxy.y * 3.0, xpos1, xpos2, linetaps1, linetaps2 ) * columntaps2.g +\n"
"        line( xystart.y + stepxy.y * 4.0, xpos1, xpos2, linetaps1, linetaps2 ) * columntaps1.b +\n"
"        line( xystart.y + stepxy.y * 5.0, xpos1, xpos2, linetaps1, linetaps2 ) * columntaps2.b;\n"
"    rgb.a = m_alpha;\n"
"    gl_FragColor = rgb;\n"
"}\n";

//============================================================================
static const char* gles_tonemap_frag =
"float tonemap(float val)\n"
"{\n"
"    return val * ( 1.0 + val / ( m_toneP1 * m_toneP1 ) ) / ( 1.0 + val );\n"
"}\n";


//============================================================================
void RenderGlShaders::getShaderSourceCode( int shaderIdx, EShaderType& shaderType, std::string& shaderName, std::string& vertexShaderCode, std::string& fragmentShaderCode )
{
    vertexShaderCode.clear();
    fragmentShaderCode.clear();

    shaderName = "SM_UNKNOWN";
    shaderType = eShaderGui;
    std::string vertDefines = "";
    std::string fragDefines = "";

    switch( shaderIdx )
    {
    case SM_DEFAULT:
        vertexShaderCode = gles_shader_vert;
        fragmentShaderCode = gles_shader_default_frag;
        shaderName = "SM_DEFAULT";
        break;

    case SM_TEXTURE:
        vertexShaderCode = gles_shader_vert;
        fragmentShaderCode = gles_shader_texture_frag;
        shaderName = "SM_TEXTURE";
        break;

    case SM_MULTI:
        vertexShaderCode = gles_shader_vert;
        fragmentShaderCode = gles_shader_multi_frag;
        shaderName = "SM_MULTI";
        break;

    case SM_FONTS:
        vertexShaderCode = gles_shader_vert;
        fragmentShaderCode = gles_shader_fonts_frag;
        shaderName = "SM_FONTS";
        break;

    case SM_TEXTURE_NOBLEND:
        vertexShaderCode = gles_shader_vert;
        fragmentShaderCode = gles_shader_texture_noblend_frag;
        shaderName = "SM_TEXTURE_NOBLEND";
        break;

    case SM_MULTI_BLENDCOLOR:
        vertexShaderCode = gles_shader_vert;
        fragmentShaderCode = gles_shader_multi_blendcolor_frag;
        shaderName = "SM_MULTI_BLENDCOLOR";
        break;

    case SM_TEXTURE_RGBA:
        vertexShaderCode = gles_shader_vert;
        fragmentShaderCode = gles_shader_rgba_frag;
        shaderName = "SM_TEXTURE_RGBA";
        break;

    case SM_TEXTURE_RGBA_OES:
        vertexShaderCode = gles_shader_vert;
        fragmentShaderCode = gles_shader_rgba_oes_frag;
        shaderName = "SM_TEXTURE_RGBA_OES";
        break;

    case SM_TEXTURE_RGBA_BLENDCOLOR:
        vertexShaderCode = gles_shader_vert;
        fragmentShaderCode = gles_shader_rgba_blendcolor_frag;
        shaderName = "SM_TEXTURE_RGBA_BLENDCOLOR";
        break;

    case SM_TEXTURE_RGBA_BOB:
        vertexShaderCode = gles_shader_vert;
        fragmentShaderCode = gles_shader_rgba_bob_frag;
        shaderName = "SM_TEXTURE_RGBA_BOB";
        break;

    case SM_TEXTURE_RGBA_BOB_OES:
        vertexShaderCode = gles_shader_vert;
        fragmentShaderCode = gles_shader_rgba_bob_oes_frag;
        shaderName = "SM_TEXTURE_RGBA_BOB_OES";
        break;

    //=== video format ===//
    case SM_VIDEO_YV12_BASIC:
        vertexShaderCode = gles_yuv2rgb_vert;
        fragmentShaderCode = gles_yuv2rgb_basic_frag;
        shaderType = eShaderVideoFormat;
        shaderName = "SM_VIDEO_YV12_BASIC";
        vertDefines = "#define XBMC_YV12\n";
        fragDefines = "#define XBMC_YV12\n";
        break;
    case SM_VIDEO_NV12_BASIC:
        vertexShaderCode = gles_yuv2rgb_vert;
        fragmentShaderCode = gles_yuv2rgb_basic_frag;
        shaderType = eShaderVideoFormat;
        shaderName = "SM_VIDEO_NV12_BASIC";
        vertDefines = "#define XBMC_NV12\n";
        fragDefines = "#define XBMC_NV12\n";
        break;

    case SM_VIDEO_YUY2_BASIC:
        vertexShaderCode = gles_yuv2rgb_vert;
        fragmentShaderCode = gles_yuv2rgb_basic_frag;
        shaderType = eShaderVideoFormat;
        shaderName = "SM_VIDEO_YUY2_BASIC";
        shaderDefines = "#define XBMC_YUY2\n";
        fragDefines = "#define XBMC_YUY2\n";
        break;


    case SM_VIDEO_UYVY_BASIC:
        vertexShaderCode = gles_yuv2rgb_vert;
        fragmentShaderCode = gles_yuv2rgb_basic_frag;
        shaderType = eShaderVideoFormat;
        shaderName = "SM_VIDEO_UYVY_BASIC";
        vertDefines = "#define XBMC_UYVY\n";
        fragDefines = "#define XBMC_UYVY\n";
        break;
    case SM_VIDEO_NV12_RGB_BASIC:
        vertexShaderCode = gles_yuv2rgb_vert;
        fragmentShaderCode = gles_yuv2rgb_basic_frag;
        shaderType = eShaderVideoFormat;
        shaderName = "SM_VIDEO_NV12_RGB_BASIC";
        vertDefines = "#define XBMC_UYVY\n";
        fragDefines = "#define XBMC_UYVY\n";
        break;

#ifdef ENABLE_GLES_SHADERS
    case SM_VIDEO_YV12_BOB:
        vertexShaderCode = gles_yuv2rgb_yv12_vert;
        fragmentShaderCode = gles_yuv2rgb_bob_frag;
        shaderType = eShaderVideoFormat;
        shaderName = "SM_VIDEO_YV12_BOB";
        break;
    case SM_VIDEO_NV12_BOB:
        vertexShaderCode = gles_yuv2rgb_nv12_vert;
        fragmentShaderCode = gles_yuv2rgb_bob_frag;
        shaderType = eShaderVideoFormat;
        shaderName = "SM_VIDEO_NV12_BOB";
        break;
    case SM_VIDEO_YUY2_BOB:
        vertexShaderCode = gles_yuv2rgb_yuy2_vert;
        fragmentShaderCode = gles_yuv2rgb_bob_frag;
        shaderType = eShaderVideoFormat;
        shaderName = "SM_VIDEO_YUY2_BOB";
        break;
    case SM_VIDEO_UYVY_BOB:
        vertexShaderCode = gles_yuv2rgb_uyvy_vert;
        fragmentShaderCode = gles_yuv2rgb_bob_frag;
        shaderType = eShaderVideoFormat;
        shaderName = "SM_VIDEO_UYVY_BOB";
        break;
    case SM_VIDEO_NV12_RGB_BOB:
        vertexShaderCode = gles_yuv2rgb_nv12_rgb_vert;
        fragmentShaderCode = gles_yuv2rgb_bob_frag;
        shaderType = eShaderVideoFormat;
        shaderName = "SM_VIDEO_NV12_RGB_BOB";
        break;
#endif // ENABLE_GLES_SHADERS

    //=== video filter ===//
    case SM_VID_FILTER_DEFAULT:
        vertexShaderCode = gles_videofilter_vert;
        fragmentShaderCode = gles_videofilter_frag;
        shaderType = eShaderVideoFilter;
        shaderName = "SM_VID_FILTER_DEFAULT";
        break;
    case SM_VID_FILTER_CONVOLUTION_4X4_RGBA:
        vertexShaderCode = gles_videofilter_vert;
        fragmentShaderCode = gles_convolution_4x4_rgba_frag;
        shaderType = eShaderVideoFilter;
        shaderName = "SM_VID_FILTER_CONVOLUTION_4X4_RGBA";
        break;
    case SM_VID_FILTER_CONVOLUTION_4X4_FLOAT:
        vertexShaderCode = gles_videofilter_vert;
        fragmentShaderCode = gles_convolution_4x4_float_frag;
        shaderType = eShaderVideoFilter;
        shaderName = "SM_VID_FILTER_CONVOLUTION_4X4_FLOAT";
        break;
    case SM_VID_FILTER_CONVOLUTION_6X6_RGBA:
        vertexShaderCode = gles_videofilter_vert;
        fragmentShaderCode = gles_convolution_6x6_rgba_frag;
        shaderType = eShaderVideoFilter;
        shaderName = "SM_VID_FILTER_CONVOLUTION_6X6_RGBA";
        break;
    case SM_VID_FILTER_CONVOLUTION_6X6_FLOAT:
        vertexShaderCode = gles_videofilter_vert;
        fragmentShaderCode = gles_convolution_6x6_float_frag;
        shaderType = eShaderVideoFilter;
        shaderName = "SM_VID_FILTER_CONVOLUTION_6X6_FLOAT";
        break;

    case SM_SHADER_NONE:
        m_Shaders[ shaderIdx ] = nullptr;
        return;

    default:
        LogMsg( LOG_ERROR, "Unknown shader type %d",  shaderIdx );
    }

    if( vertexShaderCode.empty() || fragmentShaderCode.empty() )
    {
#ifdef DEBUG_KODI_SHADERS
        LogMsg( LOG_INFO, "ERROR empty Compiling shader %s", shaderName.c_str() );
#endif // DEBUG_KODI_SHADERS
        //VerifyGLStateQt();
        vertexShaderCode = gles_shader_vert;
        fragmentShaderCode = gles_shader_default_frag;
    }
    else
    {
#ifdef TARGET_OS_WINDOWS
        vertexShaderCode = "#version 150\n" + vertDefines + vertexShaderCode;
        fragmentShaderCode = fragDefines + fragmentShaderCode;
#else
        // version must be first line of code on linux
        vertexShaderCode = "#version 120\n" + vertDefines + vertexShaderCode;
        fragmentShaderCode = "#version 120\n" + fragDefines + fragmentShaderCode;
#endif // TARGET_OS_WINDOWS
    }
}


#endif // defined(SHADERS_INCLUDE)
