/*****************************************************************************
 * gotvptop_objects.h: gotvptop_object_t definition and manipulation methods
 *****************************************************************************
 * Copyright (C) 2002-2008 GOTV authors and VideoLAN
 * $Id: 11b7844b08cebfef1a558221102a4dcab14d022f $
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
#pragma once

#include "gotvptop_config.h"
#include "gotvptop_common.h"

/**
 * \defgroup gotvptop_object GOTV objects
 * @{
 * \file
 * Common GOTV object defintions
 */

/**
 * GOTV object common members
 *
 * Common public properties for all GOTV objects.
 * Object also have private properties maintained by the core, see
 * \ref gotvptop_object_internals_t
 */
struct gotvptop_common_members
{
    /** Object type name
     *
     * A constant string identifying the type of the object (for logging)
     */
    const char *object_type;

    /** Log messages header
     *
     * Human-readable header for log messages. This is not thread-safe and
     * only used by VLM and Lua interfaces.
     */
    char *header;

    int  flags;

    /** Module probe flag
     *
     * A boolean during module probing when the probe is "forced".
     * See \ref module_need().
     */
    bool force;

    /** LibGOTV instance
     *
     * Root GOTV object of the objects tree that this object belongs in.
     */
    libgotvptop_int_t *libgotvptop;

    /** Parent object
     *
     * The parent GOTV object in the objects tree. For the root (the LibGOTV
     * instance) object, this is NULL.
     */
    gotvptop_object_t *parent;
};

/**
 * Type-safe gotvptop_object_t cast
 *
 * This macro attempts to cast a pointer to a compound type to a
 * \ref gotvptop_object_t pointer in a type-safe manner.
 * It checks if the compound type actually starts with an embedded
 * \ref gotvptop_object_t structure.
 */
#if !defined(__cplusplus)
# define GOTV_OBJECT(x) \
    _Generic((x)->obj, \
        struct gotvptop_common_members: (gotvptop_object_t *)(&(x)->obj), \
        const struct gotvptop_common_members: (const gotvptop_object_t *)(&(x)->obj) \
    )
#else
# define GOTV_OBJECT( x ) ((gotvptop_object_t *)&(x)->obj)
#endif

/* Object flags */
#define OBJECT_FLAGS_QUIET       0x0002
#define OBJECT_FLAGS_NOINTERACT  0x0004

/*****************************************************************************
 * The gotvptop_object_t type. Yes, it's that simple :-)
 *****************************************************************************/
/** The main gotvptop_object_t structure */
struct gotvptop_object_t
{
    struct gotvptop_common_members obj;
};

/* The root object */
struct libgotvptop_int_t
{
    struct gotvptop_common_members obj;
};

/*****************************************************************************
 * Prototypes
 *****************************************************************************/
NLC_API void *libgotvptop_object_create( gotvptop_object_t *, size_t ) GOTV_MALLOC GOTV_USED;
NLC_API gotvptop_object_t * libgotvptop_object_find_name( gotvptop_object_t *, const char * ) GOTV_USED GOTV_DEPRECATED;
NLC_API void * libgotvptop_object_hold( gotvptop_object_t * );
NLC_API void libgotvptop_object_release( gotvptop_object_t * );
NLC_API gotvptop_list_t * libgotvptop_list_children( gotvptop_object_t * ) GOTV_USED;
NLC_API void gotvptop_list_release( gotvptop_list_t * );
NLC_API char *libgotvptop_object_get_name( const gotvptop_object_t * ) GOTV_USED;
#define gotvptop_object_get_name(o) libgotvptop_object_get_name(GOTV_OBJECT(o))

#define gotvptop_object_create(a,b) libgotvptop_object_create( GOTV_OBJECT(a), b )

#define gotvptop_object_find_name(a,b) \
    libgotvptop_object_find_name( GOTV_OBJECT(a),b)

#define gotvptop_object_hold(a) \
    libgotvptop_object_hold( GOTV_OBJECT(a) )

#define gotvptop_object_release(a) \
    libgotvptop_object_release( GOTV_OBJECT(a) )

#define gotvptop_list_children(a) \
    libgotvptop_list_children( GOTV_OBJECT(a) )

NLC_API GOTV_MALLOC void *gotvptop_obj_malloc(gotvptop_object_t *, size_t);
NLC_API GOTV_MALLOC void *gotvptop_obj_calloc(gotvptop_object_t *, size_t, size_t);
NLC_API void gotvptop_obj_free(gotvptop_object_t *, void *);

/** @} */
