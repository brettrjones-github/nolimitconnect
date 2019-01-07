/*****************************************************************************
 * gotvptop_aout_volume.h: audio volume module
 *****************************************************************************
 * Copyright (C) 2002-2009 GOTV authors and VideoLAN
 * $Id: ee8125e7232f79c1508840764b932738cc03d80b $
 *
 * Authors: Christophe Massiot <massiot@via.ecp.fr>
 *          Laurent Aimar <fenrir _AT_ videolan _DOT_ org>
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

#ifndef GOTV_AOUT_MIXER_H
#define GOTV_AOUT_MIXER_H 1

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup audio_volume Audio output volume
 * \ingroup audio_output
 * @{
 * \file
 * This file defines functions, structures and macros for audio output mixer object
 */

typedef struct audio_volume audio_volume_t;

/**
 * Audio volume
 */
struct audio_volume
{
    struct gotvptop_common_members obj;

    gotvptop_fourcc_t format; /**< Audio samples format */
    void (*amplify)(audio_volume_t *, block_t *, float); /**< Amplifier */
};

/** @} */

#ifdef __cplusplus
}
#endif

#endif
