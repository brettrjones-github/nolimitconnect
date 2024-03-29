/* camellia192-meta.c

   Copyright (C) 2010, 2013, 2014 Niels Möller

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

#include "nettle-meta.h"

#include "nettle_camellia.h"

const struct nettle_cipher nettle_camellia192 =
  { "camellia192", sizeof(struct camellia256_ctx),
    CAMELLIA_BLOCK_SIZE, CAMELLIA192_KEY_SIZE,
    (nettle_set_key_func *) camellia192_set_encrypt_key,
    (nettle_set_key_func *) camellia192_set_decrypt_key,
    (nettle_cipher_func *) camellia256_crypt,
    (nettle_cipher_func *) camellia256_crypt
  };
