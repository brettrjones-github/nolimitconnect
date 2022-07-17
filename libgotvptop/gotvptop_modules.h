/*****************************************************************************
 * gotvptop_modules.h : Module descriptor and load functions
 *****************************************************************************
 * Copyright (C) 2001-2011 GOTV authors and VideoLAN
 * $Id: c2d3c26d20c3c45529bc01afb40377560cdf7306 $
 *
 * Authors: Samuel Hocevar <sam@zoy.org>
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

#ifndef GOTV_MODULES_H
#define GOTV_MODULES_H 1

/**
 * \file
 * This file defines functions for modules in gotvptop
 */

typedef int (*gotvptop_activate_t)(void *func, va_list args);
typedef void (*gotvptop_deactivate_t)(void *func, va_list args);

/*****************************************************************************
 * Exported functions.
 *****************************************************************************/

NLC_API module_t * libgotvptop_gotvptop_module_load( gotvptop_object_t *obj, const char *cap, const char *name, bool strict, gotvptop_activate_t probe, ... ) GOTV_USED;
#define gotvptop_module_load(o,c,n,s,...) \
        libgotvptop_gotvptop_module_load(GOTV_OBJECT(o),c,n,s,__VA_ARGS__)
NLC_API void libgotvptop_gotvptop_module_unload( gotvptop_object_t *obj, module_t *, gotvptop_deactivate_t deinit, ... );
#define gotvptop_module_unload(o,m,d,...) \
        libgotvptop_gotvptop_module_unload(GOTV_OBJECT(o),m,d,__VA_ARGS__)

NLC_API module_t * libgotvptop_module_need( gotvptop_object_t *, const char *, const char *, bool ) GOTV_USED;
#define gotvptop_module_need(a,b,c,d) libgotvptop_module_need(GOTV_OBJECT(a),b,c,d)
NLC_API void libgotvptop_module_unneed( gotvptop_object_t *, module_t * );
#define module_unneed(a,b) libgotvptop_module_unneed(GOTV_OBJECT(a),b)
NLC_API bool module_exists(const char *) GOTV_USED;
NLC_API module_t * module_find(const char *) GOTV_USED;

int libgotvptop_module_start(gotvptop_object_t *, const module_t *);
#define module_start(o, m) libgotvptop_module_start(GOTV_OBJECT(o),m)
void libgotvptop_module_stop(gotvptop_object_t *, const module_t *);
#define module_stop(o, m) libgotvptop_module_stop(GOTV_OBJECT(o),m)

NLC_API module_config_t * module_config_get( const module_t *, unsigned * ) GOTV_USED;
NLC_API void module_config_free( module_config_t * );

NLC_API void module_list_free(module_t **);
NLC_API module_t ** module_list_get(size_t *n) GOTV_USED;

NLC_API bool module_provides( const module_t *m, const char *cap );
NLC_API const char * module_get_object( const module_t *m ) GOTV_USED;
NLC_API const char * module_get_name( const module_t *m, bool long_name ) GOTV_USED;
#define module_GetLongName( m ) module_get_name( m, true )
NLC_API const char * module_get_help( const module_t *m ) GOTV_USED;
NLC_API const char * module_get_capability( const module_t *m ) GOTV_USED;
NLC_API int module_get_score( const module_t *m ) GOTV_USED;
NLC_API const char * module_gettext( const module_t *, const char * ) GOTV_USED;

GOTV_USED static inline module_t *module_get_main (void)
{
    return module_find ("core");
}
#define module_get_main(a) module_get_main()

GOTV_USED static inline bool module_is_main( const module_t * p_module )
{
    return !strcmp( module_get_object( p_module ), "core" );
}

#endif /* GOTV_MODULES_H */
