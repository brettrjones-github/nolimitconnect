#ifndef RENDER_LOGO_SHADERS_H
#define RENDER_LOGO_SHADERS_H

#include "config_gotvapps.h"

#include <QtGui/qopenglshaderprogram.h>
#include <QtGui/qopenglfunctions.h>

#include "RenderGlShaders.h"

class RenderGlWidget;
class RenderGlLogic;
class RenderLogoShaders : protected QOpenGLFunctions
{
public:
    RenderLogoShaders( RenderGlLogic& renderLogic )
        : m_RenderLogic( renderLogic ) {}

    // must be called from gui thread (required for linux)
    bool initLogoShaders( QOpenGLFunctions * glf );

    QOpenGLShaderProgram&       getShader()     { return program1; }
    int&                        getVertexAttr() { return vertexAttr1; }
    int&                        getNormalAttr() { return normalAttr1; }
    int&                        getMatrixAttr() { return matrixUniform1; }

protected:
    RenderGlLogic&              m_RenderLogic;
    QOpenGLFunctions *          m_Glf = nullptr;
    QOpenGLShaderProgram        program1;
    int                         vertexAttr1;
    int                         normalAttr1;
    int                         matrixUniform1;
};

#endif // RENDER_LOGO_SHADERS_H
