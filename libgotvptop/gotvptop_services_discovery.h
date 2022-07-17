/*****************************************************************************
 * gotvptop_services_discovery.h : Services Discover functions
 *****************************************************************************
 * Copyright (C) 1999-2004 GOTV authors and VideoLAN
 * $Id: 1011d6b630e6f4357e5ab47b464cd111444e3207 $
 *
 * Authors: Pierre d'Herbemont <pdherbemont # videolan.org>
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

#ifndef GOTV_SERVICES_DISCOVERY_H_
#define GOTV_SERVICES_DISCOVERY_H_

#include <gotvptop_input.h>
#include <gotvptop_probe.h>

/**
 * \file
 * This file lists functions and structures for service discovery (SD) in gotvptop
 */

# ifdef __cplusplus
extern "C" {
# endif

/**
 * @{
 */

struct services_discovery_owner_t
{
    void *sys; /**< Private data for the owner callbacks */
    void (*item_added)(struct services_discovery_t *sd, input_item_t *parent,
                       input_item_t *item, const char *category);
    void (*item_removed)(struct services_discovery_t *sd, input_item_t *item);
};

/**
 * Main service discovery structure to build a SD module
 */
struct services_discovery_t
{
    struct gotvptop_common_members obj;
    module_t *          p_module;             /**< Loaded module */

    char *psz_name;                           /**< Main name of the SD */
    config_chain_t *p_cfg;                    /**< Configuration for the SD */

    const char *description; /**< Human-readable name */

    /** Control function
     * \see services_discovery_command_e
     */
    int ( *pf_control ) ( services_discovery_t *, int, va_list );

    services_discovery_sys_t *p_sys;          /**< Custom private data */

    struct services_discovery_owner_t owner; /**< Owner callbacks */
};

/**
 * Service discovery categories
 * \see gotvptop_sd_probe_Add
 */
enum services_discovery_category_e
{
    SD_CAT_DEVICES = 1,           /**< Devices, like portable music players */
    SD_CAT_LAN,                   /**< LAN/WAN services, like Upnp or SAP */
    SD_CAT_INTERNET,              /**< Internet or Website channels services */
    SD_CAT_MYCOMPUTER             /**< Computer services, like Discs or Apps */
};

/**
 * Service discovery control commands
 */
enum services_discovery_command_e
{
    SD_CMD_SEARCH = 1,          /**< arg1 = query */
    SD_CMD_DESCRIPTOR           /**< arg1 = services_discovery_descriptor_t* */
};

/**
 * Service discovery capabilities
 */
enum services_discovery_capability_e
{
    SD_CAP_SEARCH = 1           /**< One can search in the SD */
};

/**
 * Service discovery descriptor
 * \see services_discovery_command_e
 */
typedef struct
{
    char *psz_short_desc;       /**< The short description, human-readable */
    char *psz_icon_url;         /**< URL to the icon that represents it */
    char *psz_url;              /**< URL for the service */
    int   i_capabilities;       /**< \see services_discovery_capability_e */
} services_discovery_descriptor_t;


/***********************************************************************
 * Service Discovery
 ***********************************************************************/

/**
 * Ask for a research in the SD
 * @param p_sd: the Service Discovery
 * @param i_control: the command to issue
 * @param args: the argument list
 * @return GOTV_SUCCESS in case of success, the error code overwise
 */
static inline int gotvptop_sd_control( services_discovery_t *p_sd, int i_control, va_list args )
{
    if( p_sd->pf_control )
        return p_sd->pf_control( p_sd, i_control, args );
    else
        return GOTV_EGENERIC;
}

/* Get the services discovery modules names to use in Create(), in a null
 * terminated string array. Array and string must be freed after use. */
NLC_API char ** libgotvptop_sd_GetNames( gotvptop_object_t *, char ***, int ** ) GOTV_USED;
#define gotvptop_sd_GetNames(obj, pln, pcat ) \
        libgotvptop_sd_GetNames(GOTV_OBJECT(obj), pln, pcat)

/**
 * Creates a services discoverer.
 */
NLC_API services_discovery_t *gotvptop_sd_Create(gotvptop_object_t *parent,
    const char *chain, const struct services_discovery_owner_t *owner)
GOTV_USED;

NLC_API void gotvptop_sd_Destroy( services_discovery_t * );

/**
 * Added top-level service callback.
 *
 * This is a convenience wrapper for services_discovery_AddSubItem().
 * It covers the most comomn case wherby the added item is a top-level service,
 * i.e. it has no parent node.
 */
static inline void services_discovery_AddItem(services_discovery_t *sd,
                                              input_item_t *item)
{
    sd->owner.item_added(sd, NULL, item, NULL);
}

/**
 * Added service callback.
 *
 * A services discovery module invokes this function when it "discovers" a new
 * service, i.e. a new input item.
 *
 * @note This callback does not take ownership of the input item; it might
 * however (and most probably will) add one of more references to the item.
 *
 * The caller is responsible for releasing its own reference(s) eventually.
 * Keeping a reference is necessary to call services_discovery_RemoveItem() or
 * to alter the item later. However, if the caller will never remove nor alter
 * the item, it can drop its reference(s) immediately.
 *
 * @param sd services discoverer / services discovery module instance
 * @param item input item to add
 */
static inline void services_discovery_AddSubItem(services_discovery_t *sd,
                                                 input_item_t *parent,
                                                 input_item_t *item)
{
    sd->owner.item_added(sd, parent, item, NULL);
}

/**
 * Added service backward compatibility callback.
 *
 * @param category Optional name of a group that the item belongs in
 *                 (for backward compatibility with legacy modules)
 */
GOTV_DEPRECATED
static inline void services_discovery_AddItemCat(services_discovery_t *sd,
                                                 input_item_t *item,
                                                 const char *category)
{
    sd->owner.item_added(sd, NULL, item, category);
}

/**
 * Removed service callback.
 *
 * A services discovery module invokes this function when it senses that a
 * service is no longer available.
 */
static inline void services_discovery_RemoveItem(services_discovery_t *sd,
                                                 input_item_t *item)
{
    sd->owner.item_removed(sd, item);
}

/* SD probing */

NLC_API int gotvptop_sd_probe_Add(gotvptop_probe_t *, const char *, const char *, int category);

#define GOTV_SD_PROBE_SUBMODULE \
    add_submodule() \
        set_capability( "services probe", 100 ) \
        set_callbacks( gotvptop_sd_probe_Open, NULL )

#define GOTV_SD_PROBE_HELPER(name, longname, cat) \
static int gotvptop_sd_probe_Open (gotvptop_object_t *obj) \
{ \
    return gotvptop_sd_probe_Add ((struct gotvptop_probe_t *)obj, name, \
                             longname, cat); \
}

/** @} */
# ifdef __cplusplus
}
# endif

#endif
