/*****************************************************************************
 * gotvptop_messages.h: messages interface
 * This library provides basic functions for threads to interact with user
 * interface, such as message output.
 *****************************************************************************
 * Copyright (C) 1999, 2000, 2001, 2002 GOTV authors and VideoLAN
 * $Id: 63f9476cd5e6d6d4e274b4d4a7b947ec59d2843e $
 *
 * Authors: Vincent Seguin <seguin@via.ecp.fr>
 *          Samuel Hocevar <sam@zoy.org>
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

#ifndef GOTV_MESSAGES_H_
#define GOTV_MESSAGES_H_

#include <stdarg.h>

/**
 * \defgroup messages Logging
 * \brief Message logs
 *
 * Functions for modules to emit log messages.
 *
 * @{
 * \file
 * Logging functions
 */

/** Message types */
enum gotvptop_log_type
{
    GOTV_MSG_INFO=0, /**< Important information */
    GOTV_MSG_ERR,    /**< Error */
    GOTV_MSG_WARN,   /**< Warning */
    GOTV_MSG_DBG,    /**< Debug */
};

/**
 * Log message
 */
typedef struct gotvptop_log_t
{
    uintptr_t   i_object_id; /**< Emitter (temporarily) unique object ID or 0 */
    const char *psz_object_type; /**< Emitter object type name */
    const char *psz_module; /**< Emitter module (source code) */
    const char *psz_header; /**< Additional header (used by VLM media) */
    const char *file; /**< Source code file name or NULL */
    int line; /**< Source code file line number or -1 */
    const char *func; /**< Source code calling function name or NULL */
    unsigned long tid; /**< Emitter thread ID */
} gotvptop_log_t;

NLC_API void gotvptop_Log(gotvptop_object_t *obj, int prio, const char *module,
                     const char *file, unsigned line, const char *func,
                     const char *format, ...) GOTV_FORMAT(7, 8);
NLC_API void gotvptop_vaLog(gotvptop_object_t *obj, int prio, const char *module,
                       const char *file, unsigned line, const char *func,
                       const char *format, va_list ap);
#define msg_GenericVa(o, p, fmt, ap) \
    gotvptop_vaLog(GOTV_OBJECT(o), p, gotvptop_module_name, __FILE__, __LINE__, \
              __func__, fmt, ap)

#define msg_Generic(o, p, ...) \
    gotvptop_Log(GOTV_OBJECT(o), p, gotvptop_module_name, __FILE__, __LINE__, \
            __func__, __VA_ARGS__)
#define msg_Info(p_this, ...) \
    msg_Generic(p_this, GOTV_MSG_INFO, __VA_ARGS__)
#define msg_Err(p_this, ...) \
    msg_Generic(p_this, GOTV_MSG_ERR, __VA_ARGS__)
#define msg_Warn(p_this, ...) \
    msg_Generic(p_this, GOTV_MSG_WARN, __VA_ARGS__)
#define msg_Dbg(p_this, ...) \
    msg_Generic(p_this, GOTV_MSG_DBG, __VA_ARGS__)

extern const char gotvptop_module_name[];

NLC_API const char *gotvptop_strerror(int);
NLC_API const char *gotvptop_strerror_c(int);

/**
 * Message logging callback signature.
 * \param data data pointer as provided to gotvptop_msg_SetCallback().
 * \param type message type (GOTV_MSG_* values from enum gotvptop_log_type)
 * \param item meta information
 * \param fmt format string
 * \param args format string arguments
 */
typedef void (*gotvptop_log_cb) (void *data, int type, const gotvptop_log_t *item,
                            const char *fmt, va_list args);

/**
 * @}
 */
#endif
