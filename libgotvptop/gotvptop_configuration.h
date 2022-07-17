/*****************************************************************************
 * gotvptop_configuration.h : configuration management module
 * This file describes the programming interface for the configuration module.
 * It includes functions allowing to declare, get or set configuration options.
 *****************************************************************************
 * Copyright (C) 1999-2006 GOTV authors and VideoLAN
 * $Id: 209168615be2a7bc5db889282b32389093ab262e $
 *
 * Authors: Gildas Bazin <gbazin@videolan.org>
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

#ifndef GOTV_CONFIGURATION_H
#define GOTV_CONFIGURATION_H 1

/**
 * \file
 * This file describes the programming interface for the configuration module.
 * It includes functions allowing to declare, get or set configuration options.
 */

#include <sys/types.h>  /* for ssize_t */

# ifdef __cplusplus
extern "C" {
# endif

struct config_category_t
{
    int         i_id;
    const char *psz_name;
    const char *psz_help;
};

typedef union
{
    char       *psz;
    int64_t     i;
    float       f;
} module_value_t;

typedef int (*gotvptop_string_list_cb)(gotvptop_object_t *, const char *,
                                  char ***, char ***);
typedef int (*gotvptop_integer_list_cb)(gotvptop_object_t *, const char *,
                                   int64_t **, char ***);

struct module_config_t
{
    uint8_t     i_type;                        /* Configuration type */
    char        i_short;               /* Optional short option name */
    unsigned    b_advanced:1;                     /* Advanced option */
    unsigned    b_internal:1;          /* Hidden from prefs and help */
    unsigned    b_unsaveable:1;       /* Not stored in configuration */
    unsigned    b_safe:1;       /* Safe in web plugins and playlists */
    unsigned    b_removed:1;                           /* Deprecated */

    const char *psz_type;                           /* Configuration subtype */
    const char *psz_name;                                     /* Option name */
    const char *psz_text;       /* Short comment on the configuration option */
    const char *psz_longtext;    /* Long comment on the configuration option */

    module_value_t value;                                    /* Option value */
    module_value_t orig;
    module_value_t min;
    module_value_t max;

    /* Values list */
    uint16_t list_count;                                /* Options list size */
    union
    {
        const char **psz;          /* List of possible values for the option */
        const int  *i;
        gotvptop_string_list_cb psz_cb;
        gotvptop_integer_list_cb i_cb;
    } list;
    const char **list_text;                /* Friendly names for list values */
    const char *list_cb_name;
    void *owner;
};

/*****************************************************************************
 * Prototypes - these methods are used to get, set or manipulate configuration
 * data.
 *****************************************************************************/
NLC_API int config_GetType(const char *) GOTV_USED;
NLC_API int64_t libgotvptop_config_GetInt(gotvptop_object_t *, const char *) GOTV_USED;
NLC_API void libgotvptop_config_PutInt(gotvptop_object_t *, const char *, int64_t);
NLC_API float libgotvptop_config_GetFloat(gotvptop_object_t *, const char *) GOTV_USED;
NLC_API void libgotvptop_config_PutFloat(gotvptop_object_t *, const char *, float);
NLC_API char * libgotvptop_config_GetPsz(gotvptop_object_t *, const char *) GOTV_USED GOTV_MALLOC;
NLC_API void libgotvptop_config_PutPsz(gotvptop_object_t *, const char *, const char *);
NLC_API ssize_t config_GetIntChoices(gotvptop_object_t *, const char *,
                                     int64_t **, char ***) GOTV_USED;
NLC_API ssize_t config_GetPszChoices(gotvptop_object_t *, const char *,
                                     char ***, char ***) GOTV_USED;

NLC_API int libgotvptop_config_SaveConfigFile( gotvptop_object_t * );
#define config_SaveConfigFile(a) libgotvptop_config_SaveConfigFile(GOTV_OBJECT(a))

NLC_API void libgotvptop_config_ResetAll( gotvptop_object_t * );
#define config_ResetAll(a) libgotvptop_config_ResetAll(GOTV_OBJECT(a))

NLC_API module_config_t * config_FindConfig(const char *) GOTV_USED;
NLC_API char * config_GetDataDir(void) GOTV_USED GOTV_MALLOC;
NLC_API char *config_GetLibDir(void) GOTV_USED;

typedef enum gotvptop_userdir
{
    GOTV_HOME_DIR, /* User's home */
    GOTV_CONFIG_DIR, /* GOTV-specific configuration directory */
    GOTV_DATA_DIR, /* GOTV-specific data directory */
    GOTV_CACHE_DIR, /* GOTV-specific user cached data directory */
    /* Generic directories (same as XDG) */
    GOTV_DESKTOP_DIR=0x80,
    GOTV_DOWNLOAD_DIR,
    GOTV_TEMPLATES_DIR,
    GOTV_PUBLICSHARE_DIR,
    GOTV_DOCUMENTS_DIR,
    GOTV_MUSIC_DIR,
    GOTV_PICTURES_DIR,
    GOTV_VIDEOS_DIR,
} gotvptop_userdir_t;

NLC_API char * config_GetUserDir( gotvptop_userdir_t ) GOTV_USED GOTV_MALLOC;

NLC_API void libgotvptop_config_AddIntf( gotvptop_object_t *, const char * );
NLC_API void libgotvptop_config_RemoveIntf( gotvptop_object_t *, const char * );
NLC_API bool libgotvptop_config_ExistIntf( gotvptop_object_t *, const char * ) GOTV_USED;

#define config_GetInt(a,b) libgotvptop_config_GetInt(GOTV_OBJECT(a),b)
#define config_PutInt(a,b,c) libgotvptop_config_PutInt(GOTV_OBJECT(a),b,c)
#define config_GetFloat(a,b) libgotvptop_config_GetFloat(GOTV_OBJECT(a),b)
#define config_PutFloat(a,b,c) libgotvptop_config_PutFloat(GOTV_OBJECT(a),b,c)
#define config_GetPsz(a,b) libgotvptop_config_GetPsz(GOTV_OBJECT(a),b)
#define config_PutPsz(a,b,c) libgotvptop_config_PutPsz(GOTV_OBJECT(a),b,c)

#define config_AddIntf(a,b) libgotvptop_config_AddIntf(GOTV_OBJECT(a),b)
#define config_RemoveIntf(a,b) libgotvptop_config_RemoveIntf(GOTV_OBJECT(a),b)
#define config_ExistIntf(a,b) libgotvptop_config_ExistIntf(GOTV_OBJECT(a),b)

/****************************************************************************
 * config_chain_t:
 ****************************************************************************/
struct config_chain_t
{
    config_chain_t *p_next;     /**< Pointer on the next config_chain_t element */

    char        *psz_name;      /**< Option name */
    char        *psz_value;     /**< Option value */
};

/**
 * This function will
 * - create all options in the array ppsz_options (var_Create).
 * - parse the given linked list of config_chain_t and set the value (var_Set).
 *
 * The option names will be created by adding the psz_prefix prefix.
 */
NLC_API void libgotvptop_config_ChainParse( gotvptop_object_t *, const char *psz_prefix, const char *const *ppsz_options, config_chain_t * );
#define config_ChainParse( a, b, c, d ) libgotvptop_config_ChainParse( GOTV_OBJECT(a), b, c, d )

/**
 * This function will parse a configuration string (psz_opts) and
 * - set all options for this module in a chained list (*pp_cfg)
 * - returns a pointer on the next module if any.
 *
 * The string format is
 *   module{option=*,option=*}
 *
 * The options values are unescaped using config_StringUnescape.
 */
NLC_API const char *config_ChainParseOptions( config_chain_t **pp_cfg, const char *ppsz_opts );

/**
 * This function will parse a configuration string (psz_string) and
 * - set the module name (*ppsz_name)
 * - set all options for this module in a chained list (*pp_cfg)
 * - returns a pointer on the next module if any.
 *
 * The string format is
 *   module{option=*,option=*}[:modulenext{option=*,...}]
 *
 * The options values are unescaped using config_StringUnescape.
 */
NLC_API char *config_ChainCreate( char **ppsz_name, config_chain_t **pp_cfg, const char *psz_string ) GOTV_USED GOTV_MALLOC;

/**
 * This function will release a linked list of config_chain_t
 * (Including the head)
 */
NLC_API void config_ChainDestroy( config_chain_t * );

/**
 * This function will duplicate a linked list of config_chain_t
 */
NLC_API config_chain_t * config_ChainDuplicate( const config_chain_t * ) GOTV_USED GOTV_MALLOC;

/**
 * This function will unescape a string in place and will return a pointer on
 * the given string.
 * No memory is allocated by it (unlike config_StringEscape).
 * If NULL is given as parameter nothing will be done (NULL will be returned).
 *
 * The following sequences will be unescaped (only one time):
 * \\ \' and \"
 */
NLC_API char * config_StringUnescape( char *psz_string );

/**
 * This function will escape a string that can be unescaped by
 * config_StringUnescape.
 * The returned value is allocated by it. You have to free it once you
 * do not need it anymore (unlike config_StringUnescape).
 * If NULL is given as parameter nothing will be done (NULL will be returned).
 *
 * The escaped characters are ' " and \
 */
NLC_API char * config_StringEscape( const char *psz_string ) GOTV_USED GOTV_MALLOC;

# ifdef __cplusplus
}
# endif

#endif /* _GOTV_CONFIGURATION_H */
