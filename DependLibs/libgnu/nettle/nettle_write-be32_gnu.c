/* write-be32.c

   Copyright (C) 2001 Niels Möller

   This file is part of GNU Nettle.

   GNU Nettle is free software: you can redistribute it and/or
   modify it under the terms of either:

     * the GNU Lesser General Public License as published by the Free
       Software Foundation; either version 3 of the License, or (at your
       option) any later version.

   or

     * the GNU General Public License as published by the Free
       Software Foundation; either version 2 of the License, or (at your
       option) any later version.

   or both in parallel, as here.

   GNU Nettle is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received copies of the GNU General Public License and
   the GNU Lesser General Public License along with this program.  If
   not, see http://www.gnu.org/licenses/.
*/

#include "config_nettle.h"

#include <stdlib.h>

#include "nettle-write.h"

#include "nettle_macros.h"

void
_nettle_write_be32(size_t length, uint8_t *dst,
		   uint32_t *src)
{
  size_t i;
  size_t words;
  unsigned leftover;
  
  words = length / 4;
  leftover = length % 4;

  for (i = 0; i < words; i++, dst += 4)
    WRITE_UINT32(dst, src[i]);

  if (leftover)
    {
      uint32_t word;
      unsigned j = leftover;
      
      word = src[i];
      
      switch (leftover)
	{
	default:
	  abort();
	case 3:
	  dst[--j] = (word >> 8) & 0xff;
	  /* Fall through */
	case 2:
	  dst[--j] = (word >> 16) & 0xff;
	  /* Fall through */
	case 1:
	  dst[--j] = (word >> 24) & 0xff;
	}
    }
}
