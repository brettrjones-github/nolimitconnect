/*****************************************************************************
 * gotvptop_renderer_discovery.h : Renderer Discovery functions
 *****************************************************************************
 * Copyright (C) 2016 GOTV authors and VideoLAN
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

#ifndef GOTV_RENDERER_DISCOVERY_H
#define GOTV_RENDERER_DISCOVERY_H 1

#include <gotvptop_input.h>
#include <gotvptop_probe.h>
#include <gotvptop_url.h>

/**
 * @defgroup gotvptop_renderer GOTV renderer discovery
 * @{
 *
 * @file
 * This file declares GOTV renderer discvoery structures and functions
 *
 * @defgroup gotvptop_renderer_item GOTV renderer items returned by the discovery
 * @{
 */

#define GOTV_RENDERER_CAN_AUDIO 0x0001
#define GOTV_RENDERER_CAN_VIDEO 0x0002

/**
 * Create a new renderer item
 *
 * @param psz_type type of the item
 * @param psz_name name of the item
 * @param psz_uri uri of the renderer item, must contains a valid protocol and
 * a valid host
 * @param psz_extra_sout extra sout options
 * @param psz_demux_filter demux filter to use with the renderer
 * @param psz_icon_uri icon uri of the renderer item
 * @param i_flags flags for the item
 * @return a renderer item or NULL in case of error
 */
NLC_API gotvptop_renderer_item_t *
gotvptop_renderer_item_new(const char *psz_type, const char *psz_name,
                      const char *psz_uri, const char *psz_extra_sout,
                      const char *psz_demux_filter, const char *psz_icon_uri,
                      int i_flags) GOTV_USED;

/**
 * Hold a renderer item, i.e. creates a new reference
 */
NLC_API gotvptop_renderer_item_t *
gotvptop_renderer_item_hold(gotvptop_renderer_item_t *p_item);

/**
 * Releases a renderer item, i.e. decrements its reference counter
 */
NLC_API void
gotvptop_renderer_item_release(gotvptop_renderer_item_t *p_item);

/**
 * Get the human readable name of a renderer item
 */
NLC_API const char *
gotvptop_renderer_item_name(const gotvptop_renderer_item_t *p_item);

/**
 * Get the type (not translated) of a renderer item. For now, the type can only
 * be "chromecast" ("upnp", "airplay" may come later).
 */
NLC_API const char *
gotvptop_renderer_item_type(const gotvptop_renderer_item_t *p_item);

/**
 * Get the demux filter to use with a renderer item
 */
NLC_API const char *
gotvptop_renderer_item_demux_filter(const gotvptop_renderer_item_t *p_item);

/**
 * Get the sout command of a renderer item
 */
NLC_API const char *
gotvptop_renderer_item_sout(const gotvptop_renderer_item_t *p_item);

/**
 * Get the icon uri of a renderer item
 */
NLC_API const char *
gotvptop_renderer_item_icon_uri(const gotvptop_renderer_item_t *p_item);

/**
 * Get the flags of a renderer item
 */
NLC_API int
gotvptop_renderer_item_flags(const gotvptop_renderer_item_t *p_item);

/**
 * @}
 * @defgroup gotvptop_renderer_discovery GOTV renderer discovery interface
 * @{
 */

typedef struct gotvptop_renderer_discovery_sys gotvptop_renderer_discovery_sys;
struct gotvptop_renderer_discovery_owner;

/**
 * Return a list of renderer discovery modules
 *
 * @param pppsz_names a pointer to a list of module name, NULL terminated
 * @param pppsz_longnames a pointer to a list of module longname, NULL
 * terminated
 *
 * @return GOTV_SUCCESS on success, or GOTV_EGENERIC on error
 */
NLC_API int
libgotvptop_rd_get_names(gotvptop_object_t *p_obj, char ***pppsz_names,
                 char ***pppsz_longnames) GOTV_USED;
#define gotvptop_rd_get_names(a, b, c) \
        libgotvptop_rd_get_names(GOTV_OBJECT(a), b, c)

/**
 * Create a new renderer discovery module
 *
 * @param psz_name name of the module to load, see gotvptop_rd_get_names() to get
 * the list of names
 *
 * @return a valid gotvptop_renderer_discovery, need to be released with
 * gotvptop_rd_release()
 */
NLC_API gotvptop_renderer_discovery_t *
gotvptop_rd_new(gotvptop_object_t *p_obj, const char *psz_name,
           const struct gotvptop_renderer_discovery_owner *owner) GOTV_USED;

NLC_API void gotvptop_rd_release(gotvptop_renderer_discovery_t *p_rd);

/**
 * @}
 * @defgroup gotvptop_renderer_discovery_module GOTV renderer module
 * @{
 */

struct gotvptop_renderer_discovery_owner
{
    void *sys;
    void (*item_added)(struct gotvptop_renderer_discovery_t *,
                       struct gotvptop_renderer_item_t *);
    void (*item_removed)(struct gotvptop_renderer_discovery_t *,
                         struct gotvptop_renderer_item_t *);
};

struct gotvptop_renderer_discovery_t
{
    struct gotvptop_common_members obj;
    module_t *          p_module;

    struct gotvptop_renderer_discovery_owner owner;

    char *              psz_name;
    config_chain_t *    p_cfg;

    gotvptop_renderer_discovery_sys *p_sys;
};

/**
 * Add a new renderer item
 *
 * This will send the gotvptop_RendererDiscoveryItemAdded event
 */
static inline void gotvptop_rd_add_item(gotvptop_renderer_discovery_t * p_rd,
                                   gotvptop_renderer_item_t * p_item)
{
    p_rd->owner.item_added(p_rd, p_item);
}

/**
 * Add a new renderer item
 *
 * This will send the gotvptop_RendererDiscoveryItemRemoved event
 */
static inline void gotvptop_rd_remove_item(gotvptop_renderer_discovery_t * p_rd,
                                      gotvptop_renderer_item_t * p_item)
{
    p_rd->owner.item_removed(p_rd, p_item);
}

/**
 * Renderer Discovery proble helpers
 */
NLC_API int
gotvptop_rd_probe_add(gotvptop_probe_t *p_probe, const char *psz_name,
                 const char *psz_longname);

#define GOTV_RD_PROBE_HELPER(name, longname) \
static int gotvptop_rd_probe_open(gotvptop_object_t *obj) \
{ \
    return gotvptop_rd_probe_add((struct gotvptop_probe_t *)obj, name, longname); \
}

#define GOTV_RD_PROBE_SUBMODULE \
    add_submodule() \
        set_capability("renderer probe", 100) \
        set_callbacks(gotvptop_rd_probe_open, NULL)

/** @} @} */

#endif
