/*****************************************************************************
 * gotvptop_mime.h: Mime type recognition
 *****************************************************************************
 * Copyright (C) 2012 GOTV authors and VideoLAN
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

#ifndef GOTV_MIME_H
#define GOTV_MIME_H 1

/**
 * \file
 * Mime type recognition helpers.
 */

NLC_API const char * gotvptop_mime_Ext2Mime( const char *psz_url );

#endif /* _GOTV_MIME_H */
