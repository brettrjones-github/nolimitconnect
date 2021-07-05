/*
 *  Copyright (C) 2005-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include "config_kodi.h"

// always define GL_GLEXT_PROTOTYPES before include gl headers
#if !defined(GL_GLEXT_PROTOTYPES)
  #define GL_GLEXT_PROTOTYPES
#endif

#if (defined(TARGET_OS_WINDOWS) && ! HAS_DX) || defined(HAVE_QT_GUI)
# if defined(TARGET_OS_APPLE)
#  include <OpenGLES/ES2/gl.h>
# elif defined(TARGET_OS_ANDROID)
#  include <GLES2/gl2.h>
#  include <GLES2/gl2ext.h>
#  include <GLES3/gl3.h>
# elif defined(TARGET_OS_LINUX)
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/glext.h>
# endif // defined(TARGET_OS_ANDROID)

# ifdef HAVE_QT_GUI

// assume gles only 
// GLES2.0 cant do CLAMP, but can do CLAMP_TO_EDGE.
# ifndef GL_CLAMP
#  define GL_CLAMP GL_CLAMP_TO_EDGE
# endif // GL_CLAMP

# ifndef GL_CLAMP_TO_EDGE
#  define GL_CLAMP_TO_EDGE GL_CLAMP
# endif // GL_CLAMP

# endif // HAVE_QT_GUI

#endif


#if defined(HAS_GL)  || defined(HAVE_QT_GUI)
  #if defined(TARGET_FREEBSD)
    #include <GL/gl.h>
    #include <GL/glu.h>
  #elif defined(TARGET_DARWIN)
    #include <OpenGL/gl3.h>
    #include <OpenGL/glu.h>
    #include <OpenGL/gl3ext.h>
  #elif defined(TARGET_OS_WINDOWS)
    //#include <libglew/include/GL/glew.h>
    #include <GL/gl.h>
    #include <GL/glu.h>
  #endif
#elif HAS_GLES >= 2
  #if defined(TARGET_DARWIN)
    #include <OpenGLES/ES2/gl.h>
    #include <OpenGLES/ES2/glext.h>
  #else
    #include <GLES2/gl2.h>
    #include <GLES2/gl2ext.h>
  #endif
  #if HAS_GLES == 3
    #include <GLES3/gl3.h>
  #endif
#endif

// Useful pixel colour manipulation macros
#define GET_A(color)            ((color >> 24) & 0xFF)
#define GET_R(color)            ((color >> 16) & 0xFF)
#define GET_G(color)            ((color >>  8) & 0xFF)
#define GET_B(color)            ((color >>  0) & 0xFF)
