/*****************************************************************************
 * gotvptop_access.h: Access descriptor, queries and methods
 *****************************************************************************
 * Copyright (C) 1999-2006 GOTV authors and VideoLAN
 * $Id: da06554814885d03823aefbf58e858f6941b3400 $
 *
 * Authors: Laurent Aimar <fenrir@via.ecp.fr>
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

#ifndef GOTV_ACCESS_H
#define GOTV_ACCESS_H 1

#include <gotvptop_stream.h>

/**
 * \defgroup access Access
 * \ingroup stream
 * Raw input byte streams
 * @{
 * \file
 * Input byte stream modules interface
 */

/**
 * Special redirection error code.
 *
 * In case of redirection, the access open function should clean up (as in
 * normal failure case), store the heap-allocated redirection URL in
 * stream_t.psz_url, and return this value.
 */
#define GOTV_ACCESS_REDIRECT GOTV_ETIMEOUT

/**
 * Opens a new read-only byte stream.
 *
 * This function might block.
 * The initial offset is of course always zero.
 *
 * \param obj parent GOTV object
 * \param mrl media resource location to read
 * \return a new access object on success, NULL on failure
 */
NLC_API stream_t *gotvptop_access_NewMRL(gotvptop_object_t *obj, const char *mrl);

/**
 * \defgroup access_helper Access Helpers
 * @{
 */

/**
 * Default pf_control callback for directory accesses.
 */
NLC_API int access_vaDirectoryControlHelper( stream_t *p_access, int i_query, va_list args );

#define ACCESS_SET_CALLBACKS( read, block, control, seek ) \
    do { \
        p_access->pf_read = (read); \
        p_access->pf_block = (block); \
        p_access->pf_control = (control); \
        p_access->pf_seek = (seek); \
    } while(0)

/**
 * @} @}
 */

#endif
