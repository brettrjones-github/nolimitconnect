/* aes256-set-decrypt-key.c

   Copyright (C) 2013, Niels Möller

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

#include <assert.h>

#include "nettle_aes-internal.h"
#include "nettle_macros.h"

void
nettle_aes256_invert_key (struct aes256_ctx *dst, const struct aes256_ctx *src)
{
  _nettle_aes_invert (_AES256_ROUNDS, dst->keys, src->keys); 
}

void
nettle_aes256_set_decrypt_key(struct aes256_ctx *ctx, const uint8_t *key)
{
  nettle_aes256_set_encrypt_key (ctx, key);
  nettle_aes256_invert_key (ctx, ctx);
}
