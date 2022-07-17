/*****************************************************************************
 * gotvptop_keystore.h:
 *****************************************************************************
 * Copyright (C) 2015-2016 GOTV authors and VideoLAN
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

#ifndef GOTV_KEYSTORE_H
# define GOTV_KEYSTORE_H

#include <gotvptop_common.h>

typedef struct gotvptop_keystore gotvptop_keystore;
typedef struct gotvptop_keystore_entry gotvptop_keystore_entry;
typedef struct gotvptop_credential gotvptop_credential;

/* Called from src/libgotvptop.c */
int
libgotvptop_InternalKeystoreInit(libgotvptop_int_t *p_libgotvptop);

/* Called from src/libgotvptop.c */
void
libgotvptop_InternalKeystoreClean(libgotvptop_int_t *p_libgotvptop);

/**
 * @defgroup keystore Keystore and credential API
 * @{
 * @file
 * This file declares gotvptop keystore API
 * @defgroup keystore_public Keystore public API
 * @{
 */

/**
 * List of keys that can be stored via the keystore API
 */
enum gotvptop_keystore_key {
    KEY_PROTOCOL,
    KEY_USER,
    KEY_SERVER,
    KEY_PATH,
    KEY_PORT,
    KEY_REALM,
    KEY_AUTHTYPE,
    KEY_MAX,
};
#define GOTV_KEYSTORE_VALUES_INIT(ppsz_values) memset(ppsz_values, 0, sizeof(const char *) * KEY_MAX)

/**
 * Keystore entry returned by gotvptop_keystore_find()
 */
struct gotvptop_keystore_entry
{
    /** Set of key/values. Values can be NULL */
    char *              ppsz_values[KEY_MAX];
    /** Secret password */
    uint8_t *           p_secret;
    /** Length of the secret */
    size_t              i_secret_len;
};

/**
 * Create a keystore object
 *
 * A keystore object is persistent across runtime. It is saved on local
 * filesystem via a gotvptop keystore module (KWallet, SecretService, Apple Keychain
 * Service ...).
 *
 * @note to be released with gotvptop_keystore_release()
 *
 * @param p_parent the parent object used to create the keystore object
 *
 * @return a pointer to the keystore object, or NULL in case of error
 */
NLC_API gotvptop_keystore *
gotvptop_keystore_create(gotvptop_object_t *p_parent);
#define gotvptop_keystore_create(x) gotvptop_keystore_create(GOTV_OBJECT(x))

/**
 * Release a keystore object
 */
NLC_API void
gotvptop_keystore_release(gotvptop_keystore *p_keystore);


/**
 * Store a secret associated with a set of key/values
 *
 * @param ppsz_values set of key/values, see gotvptop_keystore_key.
 *        ppsz_values[KEY_PROTOCOL] and  ppsz_values[KEY_SERVER] must be valid
 *        strings
 * @param p_secret binary secret or string password
 * @param i_secret_len length of p_secret. If it's less than 0, then p_secret
 * is assumed to be a '\0' terminated string
 * @param psz_label user friendly label
 *
 * @return GOTV_SUCCESS on success, or GOTV_EGENERIC on error
 */
NLC_API int
gotvptop_keystore_store(gotvptop_keystore *p_keystore,
                   const char *const ppsz_values[KEY_MAX],
                   const uint8_t* p_secret, ssize_t i_secret_len,
                   const char *psz_label);

/**
 * Find all entries that match a set of key/values
 *
 * @param ppsz_values set of key/values, see gotvptop_keystore_key, any values can
 * be NULL
 * @param pp_entries list of found entries. To be released with
 * gotvptop_keystore_release_entries()
 *
 * @return the number of entries
 */
NLC_API unsigned int
gotvptop_keystore_find(gotvptop_keystore *p_keystore,
                  const char *const ppsz_values[KEY_MAX],
                  gotvptop_keystore_entry **pp_entries) GOTV_USED;

/**
 * Remove all entries that match a set of key/values
 *
 * @note only entries added by GOTV can be removed
 *
 * @param ppsz_values set of key/values, see gotvptop_keystore_key, any values can
 * be NULL
 *
 * @return the number of entries
 */
NLC_API unsigned int
gotvptop_keystore_remove(gotvptop_keystore *p_keystore,
                    const char *const ppsz_values[KEY_MAX]);

/**
 * Release the list of entries returned by gotvptop_keystore_find()
 */
NLC_API void
gotvptop_keystore_release_entries(gotvptop_keystore_entry *p_entries, unsigned int i_count);

/**
 * @}
 * @defgroup credential Credential API
 * @{
 */

/**
 * @note init with gotvptop_credential_init()
 */
struct gotvptop_credential
{
    /** url to store or to search */
    const gotvptop_url_t *p_url;
    /** http realm or smb domain to search, can be overridden after a call to
     * gotvptop_credential_get() */
    const char *psz_realm;
    /** http authtype to search, can be overridden after a call to
     * gotvptop_credential_get() */
    const char *psz_authtype;
    /** valid only if gotvptop_credential_get() returned true */
    const char *psz_username;
    /** valid only if gotvptop_credential_get() returned true */
    const char *psz_password;

    /* internal */
    enum {
        GET_FROM_URL,
        GET_FROM_OPTION,
        GET_FROM_MEMORY_KEYSTORE,
        GET_FROM_KEYSTORE,
        GET_FROM_DIALOG,
    } i_get_order;

    gotvptop_keystore *p_keystore;
    gotvptop_keystore_entry *p_entries;
    unsigned int i_entries_count;

    char *psz_split_domain;
    char *psz_var_username;
    char *psz_var_password;

    char *psz_dialog_username;
    char *psz_dialog_password;
    bool b_from_keystore;
    bool b_store;
};

/**
 * Init a credential struct
 *
 * @note to be cleaned with gotvptop_credential_clean()
 *
 * @param psz_url url to store or to search
 */
NLC_API void
gotvptop_credential_init(gotvptop_credential *p_credential, const gotvptop_url_t *p_url);

/**
 * Clean a credential struct
 */
NLC_API void
gotvptop_credential_clean(gotvptop_credential *p_credential);

/**
 * Get a username/password couple
 *
 * This will search for a credential using url, GOTV options, the gotvptop_keystore
 * or by asking the user via dialog_Login(). This function can be called
 * indefinitely, it will first return the user/password from the url (if any),
 * then from GOTV options (if any), then from the keystore (if any), and finally
 * from the dialog (if any). This function will return true as long as the user
 * fill the dialog texts and will return false when the user cancel it.
 *
 * @param p_parent the parent object (for var, keystore and dialog)
 * @param psz_option_username GOTV option name for the username
 * @param psz_option_password GOTV option name for the password
 * @param psz_dialog_title dialog title, if NULL, this function won't use the
 * keystore or the dialog
 * @param psz_dialog_fmt dialog text using format
 *
 * @return true if gotvptop_credential.psz_username and gotvptop_credential.psz_password
 * are valid, otherwise this function should not be called again.
 */

NLC_API bool
gotvptop_credential_get(gotvptop_credential *p_credential, gotvptop_object_t *p_parent,
                   const char *psz_option_username,
                   const char *psz_option_password,
                   const char *psz_dialog_title,
                   const char *psz_dialog_fmt, ...) GOTV_FORMAT(6, 7);
#define gotvptop_credential_get(a, b, c, d, e, f, ...) \
    gotvptop_credential_get(a, GOTV_OBJECT(b), c, d, e, f, ##__VA_ARGS__)

/**
 * Store the last dialog credential returned by gotvptop_credential_get()
 *
 * This function will store the credential in the memory keystore if it's
 * valid, or will store in the permanent one if it comes from the dialog and if
 * the user asked for it.
 *
 * @return true if the credential was stored or comes from the keystore, false
 * otherwise
 */
NLC_API bool
gotvptop_credential_store(gotvptop_credential *p_credential, gotvptop_object_t *p_parent);
#define gotvptop_credential_store(a, b) \
    gotvptop_credential_store(a, GOTV_OBJECT(b))

/**
 * @}
 * @defgroup keystore_implementation Implemented by keystore modules
 * @{
 */

#define GOTV_KEYSTORE_NAME "libGOTV"

static inline int
gotvptop_keystore_entry_set_secret(gotvptop_keystore_entry *p_entry,
                              const uint8_t *p_secret, size_t i_secret_len)
{
    p_entry->p_secret = (uint8_t*) malloc(i_secret_len);
    if (!p_entry->p_secret)
        return GOTV_EGENERIC;
    memcpy(p_entry->p_secret, p_secret, i_secret_len);
    p_entry->i_secret_len = i_secret_len;
    return GOTV_SUCCESS;
}

static inline void
gotvptop_keystore_release_entry(gotvptop_keystore_entry *p_entry)
{
    for (unsigned int j = 0; j < KEY_MAX; ++j)
    {
        free(p_entry->ppsz_values[j]);
        p_entry->ppsz_values[j] = NULL;
    }
    free(p_entry->p_secret);
    p_entry->p_secret = NULL;
}

typedef struct gotvptop_keystore_sys gotvptop_keystore_sys;
struct gotvptop_keystore
{
    struct gotvptop_common_members obj;
    module_t            *p_module;
    gotvptop_keystore_sys    *p_sys;

    /** See gotvptop_keystore_store() */
    int                 (*pf_store)(gotvptop_keystore *p_keystore,
                                    const char *const ppsz_values[KEY_MAX],
                                    const uint8_t *p_secret,
                                    size_t i_secret_len, const char *psz_label);
    /**  See gotvptop_keystore_find() */
    unsigned int        (*pf_find)(gotvptop_keystore *p_keystore,
                                   const char *const ppsz_values[KEY_MAX],
                                   gotvptop_keystore_entry **pp_entries);

    /** See gotvptop_keystore_remove() */
    unsigned int        (*pf_remove)(gotvptop_keystore *p_keystore,
                                     const char *const ppsz_values[KEY_MAX]);
};

/** @} @} */

#endif
