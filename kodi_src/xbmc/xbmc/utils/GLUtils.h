/*
 *  Copyright (C) 2005-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

// GL Error checking macro
// this function is useful for tracking down GL errors, which otherwise
// just result in undefined behavior and can be difficult to track down.
//
// Just call it 'VerifyGLState()' after a sequence of GL calls
//
// if GL_DEBUGGING and HAS_GL are defined, the function checks
// for GL errors and prints the current state of the various matrices;
// if not it's just an empty inline stub, and thus won't affect performance
// and will be optimized out.

#include "config_kodi.h"
#include "system_gl.h"
#if defined(HAS_GL) || defined(HAS_GLES) || defined(HAVE_QT_GUI) 

namespace KODI
{
namespace UTILS
{
namespace GL
{

void GlErrorCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const char* message, const void* userParam);

}
}
}

void _VerifyGLState(const char* szfile, const char* szfunction, int lineno);
#if defined(DEBUG) || defined(GL_DEBUGGING) && (defined(HAS_GL) || defined(HAS_GLES))
//# ifndef  VerifyGLState()
#  define VerifyGLState() _VerifyGLState(__FILE__, __FUNCTION__, __LINE__)
//# endif //  VerifyGLState()
#else
//# ifndef  VerifyGLState()
#  define VerifyGLState()
//# endif //  VerifyGLState()
#endif // VerifyGLState



void LogGraphicsInfo();

int glFormatElementByteCount( GLenum format );
#endif // defined(HAS_GL) || defined(HAS_GLES) || defined(HAS_QT_GUI) 
