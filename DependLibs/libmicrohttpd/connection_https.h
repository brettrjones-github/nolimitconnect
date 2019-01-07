/*
     This file is part of libmicrohttpd
     Copyright (C) 2008 Daniel Pittman and Christian Grothoff

     This library is free software; you can redistribute it and/or
     modify it under the terms of the GNU Lesser General Public
     License as published by the Free Software Foundation; either
     version 2.1 of the License, or (at your option) any later version.

     This library is distributed in the hope that it will be useful,
     but WITHOUT ANY WARRANTY; without even the implied warranty of
     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
     Lesser General Public License for more details.

     You should have received a copy of the GNU Lesser General Public
     License along with this library; if not, write to the Free Software
     Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

/**
 * @file connection_https.h
 * @brief  Methods for managing connections
 * @author Christian Grothoff
 */

#ifndef CONNECTION_HTTPS_H
#define CONNECTION_HTTPS_H

#include "internal_microhttpd.h"

#if HTTPS_SUPPORT
/**
 * Set connection callback function to be used through out
 * the processing of this secure connection.
 *
 * @param connection which callbacks should be modified
 */
void 
MHD_set_https_callbacks (struct MHD_Connection *connection);
#endif

#endif
