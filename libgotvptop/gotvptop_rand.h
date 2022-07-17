/*****************************************************************************
 * gotvptop_rand.h: RNG
 *****************************************************************************
 * Copyright © 2007 Rémi Denis-Courmont
 * $Id: 3ae95ac04c55f46d116481eb89255b013f6d1c32 $
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

#ifndef GOTV_RAND_H
# define GOTV_RAND_H

/**
 * \file
 * This file defined random number generator function in gotvptop
 */

NLC_API void gotvptop_rand_bytes(void *buf, size_t len);

/* Interlocked (but not reproducible) functions for the POSIX PRNG */
NLC_API double gotvptop_drand48(void) GOTV_USED;
NLC_API long gotvptop_lrand48(void) GOTV_USED;
NLC_API long gotvptop_mrand48(void) GOTV_USED;

#endif
