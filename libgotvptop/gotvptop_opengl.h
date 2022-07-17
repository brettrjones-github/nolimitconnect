/*****************************************************************************
 * gotvptop_opengl.h: GOTV GL API
 *****************************************************************************
 * Copyright (C) 2009 Laurent Aimar
 * Copyright (C) 2011 Rémi Denis-Courmont
 *
 * Authors: Laurent Aimar <fenrir _AT_ videolan _DOT_ org>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

#ifndef GOTV_GL_H
#define GOTV_GL_H 1

/**
 * \file
 * This file defines GL structures and functions.
 */

struct vout_window_t;
struct vout_window_cfg_t;

/**
 * A GOTV GL context (and its underlying surface)
 */
typedef struct gotvptop_gl_t gotvptop_gl_t;

struct gotvptop_gl_t
{
    struct gotvptop_common_members obj;

    struct vout_window_t *surface;
    module_t *module;
    void *sys;

    int  (*makeCurrent)(gotvptop_gl_t *);
    void (*releaseCurrent)(gotvptop_gl_t *);
    void (*resize)(gotvptop_gl_t *, unsigned, unsigned);
    void (*swap)(gotvptop_gl_t *);
    void*(*getProcAddress)(gotvptop_gl_t *, const char *);

    enum {
        GOTV_GL_EXT_DEFAULT,
        GOTV_GL_EXT_EGL,
        GOTV_GL_EXT_WGL,
    } ext;

    union {
        /* if ext == GOTV_GL_EXT_EGL */
        struct {
            /* call eglQueryString() with current display */
            const char *(*queryString)(gotvptop_gl_t *, int32_t name);
            /* call eglCreateImageKHR() with current display and context, can
             * be NULL */
            void *(*createImageKHR)(gotvptop_gl_t *, unsigned target, void *buffer,
                                    const int32_t *attrib_list);
            /* call eglDestroyImageKHR() with current display, can be NULL */
            bool (*destroyImageKHR)(gotvptop_gl_t *, void *image);
        } egl;
        /* if ext == GOTV_GL_EXT_WGL */
        struct
        {
            const char *(*getExtensionsString)(gotvptop_gl_t *);
        } wgl;
    };
};

enum {
    GOTV_OPENGL,
    GOTV_OPENGL_ES2,
};

NLC_API gotvptop_gl_t *gotvptop_gl_Create(struct vout_window_t *, unsigned, const char *) GOTV_USED;
NLC_API void gotvptop_gl_Release(gotvptop_gl_t *);
NLC_API void gotvptop_gl_Hold(gotvptop_gl_t *);

static inline int gotvptop_gl_MakeCurrent(gotvptop_gl_t *gl)
{
    return gl->makeCurrent(gl);
}

static inline void gotvptop_gl_ReleaseCurrent(gotvptop_gl_t *gl)
{
    gl->releaseCurrent(gl);
}

static inline void gotvptop_gl_Resize(gotvptop_gl_t *gl, unsigned w, unsigned h)
{
    if (gl->resize != NULL)
        gl->resize(gl, w, h);
}

static inline void gotvptop_gl_Swap(gotvptop_gl_t *gl)
{
    gl->swap(gl);
}

static inline void *gotvptop_gl_GetProcAddress(gotvptop_gl_t *gl, const char *name)
{
    return (gl->getProcAddress != NULL) ? gl->getProcAddress(gl, name) : NULL;
}

NLC_API gotvptop_gl_t *gotvptop_gl_surface_Create(gotvptop_object_t *,
                                        const struct vout_window_cfg_t *,
                                        struct vout_window_t **) GOTV_USED;
NLC_API bool gotvptop_gl_surface_CheckSize(gotvptop_gl_t *, unsigned *w, unsigned *h);
NLC_API void gotvptop_gl_surface_Destroy(gotvptop_gl_t *);

#endif /* GOTV_GL_H */
