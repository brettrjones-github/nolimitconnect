/*****************************************************************************
 * gotvptop_dialog.h: user interaction dialogs
 *****************************************************************************
 * Copyright (C) 2009 Rémi Denis-Courmont
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

#ifndef GOTV_DIALOG_H_
#define GOTV_DIALOG_H_
# include <stdarg.h>

typedef struct gotvptop_dialog_provider gotvptop_dialog_provider;
typedef struct gotvptop_dialog_id gotvptop_dialog_id;
typedef struct extension_dialog_t extension_dialog_t;

/* Called from src/libgotvptop.c */
int
libgotvptop_InternalDialogInit(libgotvptop_int_t *p_libgotvptop);

/* Called from src/libgotvptop.c */
void
libgotvptop_InternalDialogClean(libgotvptop_int_t *p_libgotvptop);

/**
 * @defgroup gotvptop_dialog GOTV dialog
 * @ingroup interface
 * @{
 * @file
 * This file declares GOTV dialog functions
 * @defgroup gotvptop_dialog_api GOTV dialog functions
 * In order to interact with the user
 * @{
 */

/**
 * Dialog question type, see gotvptop_dialog_wait_question()
 */
typedef enum gotvptop_dialog_question_type
{
    GOTV_DIALOG_QUESTION_NORMAL,
    GOTV_DIALOG_QUESTION_WARNING,
    GOTV_DIALOG_QUESTION_CRITICAL,
} gotvptop_dialog_question_type;

/**
 * Sends an error message
 *
 * This function returns immediately
 *
 * @param p_obj the GOTV object emitting the error
 * @param psz_title title of the error dialog
 * @param psz_fmt format string for the error message
 * @return GOTV_SUCCESS on success, or a GOTV error code on error
 */
NLC_API int
libgotvptop_dialog_display_error(gotvptop_object_t *p_obj, const char *psz_title,
                         const char *psz_fmt, ...) GOTV_FORMAT(3,4);
#define gotvptop_dialog_display_error(a, b, c, ...) \
    libgotvptop_dialog_display_error(GOTV_OBJECT(a), b, c, ##__VA_ARGS__)

/**
 * Sends an error message
 *
 * Equivalent to gotvptop_dialog_display_error() expect that it's called with a
 * va_list.
 */
NLC_API int
gotvptop_dialog_display_error_va(gotvptop_object_t *p_obj, const char *psz_title,
                            const char *psz_fmt, va_list ap);

/**
 * Requests an user name and a password
 *
 * This function waits until the user dismisses the dialog or responds. It's
 * interruptible via gotvptop_interrupt. In that case, gotvptop_dialog_cbs.pf_cancel()
 * will be invoked. If p_store is not NULL, the user will be asked to store the
 * password or not.
 *
 * @param p_obj the GOTV object emitting the dialog
 * @param ppsz_username a pointer to the user name provided by the user, it
 * must be freed with free() on success
 * @param ppsz_password a pointer to the password provided by the user, it must
 * be freed with free() on success
 * @param p_store a pointer to the store answer provided by the user (optional)
 * @param psz_default_username default user name proposed
 * @param psz_title title of the login dialog
 * @param psz_fmt format string for the login message
 * @return < 0 on error, 0 if the user cancelled it, and 1 if ppsz_username and
 * ppsz_password are valid.
 */
NLC_API int
libgotvptop_dialog_wait_login(gotvptop_object_t *p_obj, char **ppsz_username,
                      char **ppsz_password, bool *p_store,
                      const char *psz_default_username,
                      const char *psz_title, const char *psz_fmt, ...)
                      GOTV_FORMAT(7,8);
#define gotvptop_dialog_wait_login(a, b, c, d, e, f, g, ...) \
    libgotvptop_dialog_wait_login(GOTV_OBJECT(a), b, c, d, e, f, g, ##__VA_ARGS__)

/**
 * Requests an user name and a password
 *
 * Equivalent to gotvptop_dialog_wait_login() expect that it's called with a
 * va_list.
 */
NLC_API int
gotvptop_dialog_wait_login_va(gotvptop_object_t *p_obj, char **ppsz_username,
                         char **ppsz_password, bool *p_store,
                         const char *psz_default_username,
                         const char *psz_title, const char *psz_fmt, va_list ap);

/**
 * Asks a total (Yes/No/Cancel) question
 *
 * This function waits until the user dismisses the dialog or responds. It's
 * interruptible via gotvptop_interrupt. In that case, gotvptop_dialog_cbs.pf_cancel()
 * will be invoked. The psz_cancel is mandatory since this dialog is always
 * cancellable by the user.
 *
 * @param p_obj the GOTV object emitting the dialog
 * @param i_type question type (severity of the question)
 * @param psz_cancel text of the cancel button
 * @param psz_action1 first choice/button text (optional)
 * @param psz_action2 second choice/button text (optional)
 * @param psz_title title of the question dialog
 * @param psz_fmt format string for the question message
 * @return < 0 on error, 0 if the user cancelled it, 1 on action1, 2 on action2
 */
NLC_API int
libgotvptop_dialog_wait_question(gotvptop_object_t *p_obj,
                         gotvptop_dialog_question_type i_type,
                         const char *psz_cancel, const char *psz_action1,
                         const char *psz_action2, const char *psz_title,
                         const char *psz_fmt, ...) GOTV_FORMAT(7,8);
#define gotvptop_dialog_wait_question(a, b, c, d, e, f, g, ...) \
    libgotvptop_dialog_wait_question(GOTV_OBJECT(a), b, c, d, e, f, g, ##__VA_ARGS__)

/**
 * Asks a total (Yes/No/Cancel) question
 *
 * Equivalent to gotvptop_dialog_wait_question() expect that it's called with a
 * va_list.
 */
NLC_API int
gotvptop_dialog_wait_question_va(gotvptop_object_t *p_obj,
                            gotvptop_dialog_question_type i_type,
                            const char *psz_cancel, const char *psz_action1,
                            const char *psz_action2, const char *psz_title,
                            const char *psz_fmt, va_list ap);

/**
 * Display a progress dialog
 *
 * This function returns immediately
 *
 * @param p_obj the GOTV object emitting the dialog
 * @param b_indeterminate true if the progress dialog is indeterminate
 * @param f_position initial position of the progress bar (between 0.0 and 1.0)
 * @param psz_cancel text of the cancel button, if NULL the dialog is not
 * cancellable (optional)
 * @param psz_title title of the progress dialog
 * @param psz_fmt format string for the progress message
 * @return a valid gotvptop_dialog_id on success, must be released with
 * gotvptop_dialog_id_release()
 */
NLC_API gotvptop_dialog_id *
libgotvptop_dialog_display_progress(gotvptop_object_t *p_obj, bool b_indeterminate,
                            float f_position, const char *psz_cancel,
                            const char *psz_title, const char *psz_fmt, ...)
                            GOTV_FORMAT(6,7);
#define gotvptop_dialog_display_progress(a, b, c, d, e, f, ...) \
    libgotvptop_dialog_display_progress(GOTV_OBJECT(a), b, c, d, e, f, ##__VA_ARGS__)

/**
 * Display a progress dialog
 *
 * Equivalent to gotvptop_dialog_display_progress() expect that it's called with a
 * va_list.
 */
NLC_API gotvptop_dialog_id *
gotvptop_dialog_display_progress_va(gotvptop_object_t *p_obj, bool b_indeterminate,
                               float f_position, const char *psz_cancel,
                               const char *psz_title, const char *psz_fmt,
                               va_list ap);

/**
 * Update the position of the progress dialog
 *
 * @param p_obj the GOTV object emitting the dialog
 * @param p_id id of the dialog to update
 * @param f_position position of the progress bar (between 0.0 and 1.0)
 * @return GOTV_SUCCESS on success, or a GOTV error code on error
 */
NLC_API int
libgotvptop_dialog_update_progress(gotvptop_object_t *p_obj, gotvptop_dialog_id *p_id,
                           float f_position);
#define gotvptop_dialog_update_progress(a, b, c) \
    libgotvptop_dialog_update_progress(GOTV_OBJECT(a), b, c)

/**
 * Update the position and the message of the progress dialog
 *
 * @param p_obj the GOTV object emitting the dialog
 * @param p_id id of the dialog to update
 * @param f_position position of the progress bar (between 0.0 and 1.0)
 * @param psz_fmt format string for the progress message
 * @return GOTV_SUCCESS on success, or a GOTV error code on error
 */
NLC_API int
libgotvptop_dialog_update_progress_text(gotvptop_object_t *p_obj, gotvptop_dialog_id *p_id,
                                float f_position, const char *psz_fmt, ...)
                                GOTV_FORMAT(4, 5);
#define gotvptop_dialog_update_progress_text(a, b, c, d, ...) \
    libgotvptop_dialog_update_progress_text(GOTV_OBJECT(a), b, c, d, ##__VA_ARGS__)

/**
 * Update the position and the message of the progress dialog
 *
 * Equivalent to gotvptop_dialog_update_progress_text() expect that it's called
 * with a va_list.
 */
NLC_API int
gotvptop_dialog_update_progress_text_va(gotvptop_object_t *p_obj, gotvptop_dialog_id *p_id,
                                   float f_position, const char *psz_fmt,
                                   va_list ap);

/**
 * Release the dialog id returned by gotvptop_dialog_display_progress()
 *
 * It causes the gotvptop_dialog_cbs.pf_cancel() callback to be invoked.
 *
 * @param p_obj the GOTV object emitting the dialog
 * @param p_id id of the dialog to release
 */
NLC_API void
libgotvptop_dialog_release(gotvptop_object_t *p_obj, gotvptop_dialog_id *p_id);
#define gotvptop_dialog_release(a, b) \
    libgotvptop_dialog_release(GOTV_OBJECT(a), b)

/**
 * Return true if the dialog id is cancelled
 *
 * @param p_obj the GOTV object emitting the dialog
 * @param p_id id of the dialog
 */
NLC_API bool
libgotvptop_dialog_is_cancelled(gotvptop_object_t *p_obj, gotvptop_dialog_id *p_id);
#define gotvptop_dialog_is_cancelled(a, b) \
    libgotvptop_dialog_is_cancelled(GOTV_OBJECT(a), b)

/**
 * @}
 * @defgroup gotvptop_dialog_impl GOTV dialog callbacks
 * Need to be implemented by GUI modules or libgotvptop
 * @{
 */

/**
 * Dialog callbacks to be implemented
 */
typedef struct gotvptop_dialog_cbs
{
    /**
     * Called when an error message needs to be displayed
     *
     * @param p_data opaque pointer for the callback
     * @param psz_title title of the dialog
     * @param psz_text text of the dialog
     */
    void (*pf_display_error)(void *p_data, const char *psz_title,
                             const char *psz_text);

    /**
     * Called when a login dialog needs to be displayed
     *
     * You can interact with this dialog by calling gotvptop_dialog_id_post_login()
     * to post an answer or gotvptop_dialog_id_dismiss() to cancel this dialog.
     *
     * @note to receive this callback, gotvptop_dialog_cbs.pf_cancel should not be
     * NULL.
     *
     * @param p_data opaque pointer for the callback
     * @param p_id id used to interact with the dialog
     * @param psz_title title of the dialog
     * @param psz_text text of the dialog
     * @param psz_default_username user name that should be set on the user form
     * @param b_ask_store if true, ask the user if he wants to save the
     * credentials
     */
    void (*pf_display_login)(void *p_data, gotvptop_dialog_id *p_id,
                             const char *psz_title, const char *psz_text,
                             const char *psz_default_username,
                             bool b_ask_store);

    /**
     * Called when a question dialog needs to be displayed
     *
     * You can interact with this dialog by calling gotvptop_dialog_id_post_action()
     * to post an answer or gotvptop_dialog_id_dismiss() to cancel this dialog.
     *
     * @note to receive this callback, gotvptop_dialog_cbs.pf_cancel should not be
     * NULL.
     *
     * @param p_data opaque pointer for the callback
     * @param p_id id used to interact with the dialog
     * @param psz_title title of the dialog
     * @param psz_text text of the dialog
     * @param i_type question type (or severity) of the dialog
     * @param psz_cancel text of the cancel button
     * @param psz_action1 text of the first button, if NULL, don't display this
     * button
     * @param psz_action2 text of the second button, if NULL, don't display
     * this button
     */
    void (*pf_display_question)(void *p_data, gotvptop_dialog_id *p_id,
                                const char *psz_title, const char *psz_text,
                                gotvptop_dialog_question_type i_type,
                                const char *psz_cancel, const char *psz_action1,
                                const char *psz_action2);

    /**
     * Called when a progress dialog needs to be displayed
     *
     * If cancellable (psz_cancel != NULL), you can cancel this dialog by
     * calling gotvptop_dialog_id_dismiss()
     *
     * @note to receive this callback, gotvptop_dialog_cbs.pf_cancel and
     * gotvptop_dialog_cbs.pf_update_progress should not be NULL.
     *
     * @param p_data opaque pointer for the callback
     * @param p_id id used to interact with the dialog
     * @param psz_title title of the dialog
     * @param psz_text text of the dialog
     * @param b_indeterminate true if the progress dialog is indeterminate
     * @param f_position initial position of the progress bar (between 0.0 and
     * 1.0)
     * @param psz_cancel text of the cancel button, if NULL the dialog is not
     * cancellable
     */
    void (*pf_display_progress)(void *p_data, gotvptop_dialog_id *p_id,
                                const char *psz_title, const char *psz_text,
                                bool b_indeterminate, float f_position,
                                const char *psz_cancel);

    /**
     * Called when a displayed dialog needs to be cancelled
     *
     * The implementation must call gotvptop_dialog_id_dismiss() to really release
     * the dialog.
     *
     * @param p_data opaque pointer for the callback
     * @param p_id id of the dialog
     */
    void (*pf_cancel)(void *p_data, gotvptop_dialog_id *p_id);

    /**
     * Called when a progress dialog needs to be updated
     *
     * @param p_data opaque pointer for the callback
     * @param p_id id of the dialog
     * @param f_position osition of the progress bar (between 0.0 and 1.0)
     * @param psz_text new text of the progress dialog
     */
    void (*pf_update_progress)(void *p_data, gotvptop_dialog_id *p_id,
                               float f_position, const char *psz_text);
} gotvptop_dialog_cbs;

/**
 * Register callbacks to handle GOTV dialogs
 *
 * @param p_cbs a pointer to callbacks, or NULL to unregister callbacks.
 * @param p_data opaque pointer for the callback
 */
NLC_API void
libgotvptop_dialog_provider_set_callbacks(gotvptop_object_t *p_obj,
                                  const gotvptop_dialog_cbs *p_cbs, void *p_data);
#define gotvptop_dialog_provider_set_callbacks(a, b, c) \
    libgotvptop_dialog_provider_set_callbacks(GOTV_OBJECT(a), b, c)

/**
 * Associate an opaque pointer with the dialog id
 */
NLC_API void
gotvptop_dialog_id_set_context(gotvptop_dialog_id *p_id, void *p_context);

/**
 * Return the opaque pointer associated with the dialog id
 */
NLC_API void *
gotvptop_dialog_id_get_context(gotvptop_dialog_id *p_id);

/**
 * Post a login answer
 *
 * After this call, p_id won't be valid anymore
 *
 * @see gotvptop_dialog_cbs.pf_display_login
 *
 * @param p_id id of the dialog
 * @param psz_username valid and non empty string
 * @param psz_password valid string (can be empty)
 * @param b_store if true, store the credentials
 * @return GOTV_SUCCESS on success, or a GOTV error code on error
 */
NLC_API int
gotvptop_dialog_id_post_login(gotvptop_dialog_id *p_id, const char *psz_username,
                         const char *psz_password, bool b_store);

/**
 * Post a question answer
 *
 * After this call, p_id won't be valid anymore
 *
 * @see gotvptop_dialog_cbs.pf_display_question
 *
 * @param p_id id of the dialog
 * @param i_action 1 for action1, 2 for action2
 * @return GOTV_SUCCESS on success, or a GOTV error code on error
 */
NLC_API int
gotvptop_dialog_id_post_action(gotvptop_dialog_id *p_id, int i_action);

/**
 * Dismiss a dialog
 *
 * After this call, p_id won't be valid anymore
 *
 * @see gotvptop_dialog_cbs.pf_cancel
 *
 * @param p_id id of the dialog
 * @return GOTV_SUCCESS on success, or a GOTV error code on error
 */
NLC_API int
gotvptop_dialog_id_dismiss(gotvptop_dialog_id *p_id);

/**
 * @}
 * @defgroup gotvptop_dialog_ext GOTV extension dialog functions
 * @{
 */

NLC_API int
libgotvptop_ext_dialog_update(gotvptop_object_t *p_obj, extension_dialog_t *dialog);
#define gotvptop_ext_dialog_update(a, b) \
    libgotvptop_ext_dialog_update(GOTV_OBJECT(a), b)

/**
 * Dialog extension callback to be implemented
 */
typedef void (*gotvptop_dialog_ext_update_cb)(extension_dialog_t *p_ext_dialog,
                                         void *p_data);

/**
 * Register a callback for GOTV extension dialog
 *
 * @param pf_update a pointer to the update callback, or NULL to unregister
 * callback
 * @param p_data opaque pointer for the callback
 */
NLC_API void
libgotvptop_dialog_provider_set_ext_callback(gotvptop_object_t *p_obj,
                                     gotvptop_dialog_ext_update_cb pf_update,
                                     void *p_data);
#define gotvptop_dialog_provider_set_ext_callback(a, b, c) \
    libgotvptop_dialog_provider_set_ext_callback(GOTV_OBJECT(a), b, c)

/** @} @} */

#endif
