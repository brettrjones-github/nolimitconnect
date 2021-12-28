#pragma once

#ifdef TARGET_OS_WINDOWS
#include "WinSock2.h"
#endif // TARGET_OS_WINDOWS

#include <QtGui/qopenglfunctions.h>
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
#include <QtGui/qopenglshaderprogram.h>
#endif // QT_VERSION < QT_VERSION_CHECK(6,0,0)

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

