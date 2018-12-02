/* pbkdf2-hmac-sha1.c

   PKCS #5 PBKDF2 used with HMAC-SHA1, see RFC 2898.

   Copyright (C) 2012 Simon Josefsson

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

#include "nettle_pbkdf2.h"

#include "nettle_hmac.h"

void
nettle_pbkdf2_hmac_sha1 (size_t key_length, const uint8_t *key,
		  unsigned iterations,
		  size_t salt_length, const uint8_t *salt,
		  size_t length, uint8_t *dst)
{
  struct nettle_hmac_sha1_ctx sha1ctx;

  nettle_hmac_sha1_set_key (&sha1ctx, key_length, key);
  PBKDF2 (&sha1ctx, nettle_hmac_sha1_update, nettle_hmac_sha1_digest,
	  SHA1_DIGEST_SIZE, iterations, salt_length, salt, length, dst);
}
