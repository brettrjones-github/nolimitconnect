
#include <app_precompiled_hdr.h>
#include "RenderLogoShaders.h"

//============================================================================
// must be called from gui thread (required for linux)
bool RenderLogoShaders::initLogoShaders( QOpenGLFunctions * glf )
{
    m_Glf = glf;

    m_Glf->glClearColor(0.1f, 0.1f, 0.2f, 1.0f);


//#define TEST_KODI_SHADER
#ifndef TEST_KODI_SHADER

    const char *vsrc1 =
        "attribute highp vec4 vertex;\n"
        "attribute mediump vec3 normal;\n"
        "uniform mediump mat4 matrix;\n"
        "varying mediump vec4 color;\n"
        "void main(void)\n"
        "{\n"
        "    vec3 toLight = normalize(vec3(0.0, 0.3, 1.0));\n"
        "    float angle = max(dot(normal, toLight), 0.0);\n"
        "    vec3 col = vec3(0.40, 1.0, 0.0);\n"
        "    color = vec4(col * 0.2 + col * 0.8 * angle, 1.0);\n"
        "    color = clamp(color, 0.0, 1.0);\n"
        "    gl_Position = matrix * vertex;\n"
        "}\n";

    const char *fsrc1 =
        "varying mediump vec4 color;\n"
        "void main(void)\n"
        "{\n"
        "    gl_FragColor = color;\n"
        "}\n";
#else
    static const char * gles_shader_vert =
    "#version 120\n"
    "attribute vec4 m_attrpos;\n"
    "attribute vec4 m_attrcol;\n"
    "attribute vec4 m_attrcord0;\n"
    "attribute vec4 m_attrcord1;\n"
    "varying vec4   m_cord0;\n"
    "varying vec4   m_cord1;\n"
    "varying lowp vec4 m_colour;\n"
    "uniform mat4   m_proj;\n"
    "uniform mat4   m_model;\n"
    "uniform mat4   m_coord0Matrix;\n"
    "void main()\n"
    "{\n"
    "    mat4 mvp = m_proj * m_model;\n"
    "    gl_Position = mvp * m_attrpos;\n"
    "    m_colour = m_attrcol;\n"
    "    m_cord0 = m_coord0Matrix * m_attrcord0;\n"
    "    m_cord1 = m_attrcord1;\n"
    "}\n";

    //BRJ NOTE: had to remove "#version 100\n"  from code or failed with error about keyword float
    //"#version 120\n\n"
    //"precision mediump float;\n"
    static const char * gles_shader_default_frag =
    "#version 120\n"
    "uniform vec4 m_unicol;\n"
    "void main()\n"
    "{\n"
    " gl_FragColor = m_unicol;\n"
    "}\n";

    const char *vsrc1 = gles_shader_vert;
    const char *fsrc1 = gles_shader_default_frag;
 #endif // TEST_KODI_SHADER

//QOpenGLShader::compile(Fragment): 0:1(10): error: GLSL 3.10 is not supported. Supported versions are: 1.10, 1.20, 1.30, 1.40, 1.00 ES, and 3.00 ES

    program1.addCacheableShaderFromSourceCode(QOpenGLShader::Vertex, vsrc1);
    program1.addCacheableShaderFromSourceCode(QOpenGLShader::Fragment, fsrc1);
    program1.link();

    vertexAttr1 = program1.attributeLocation("vertex");
    normalAttr1 = program1.attributeLocation("normal");
    matrixUniform1 = program1.uniformLocation("matrix");

    return true;
}
