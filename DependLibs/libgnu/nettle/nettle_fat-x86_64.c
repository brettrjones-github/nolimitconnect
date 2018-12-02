/* fat-x86_64.c

   Copyright (C) 2015 Niels Möller

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

#define _GNU_SOURCE

#include "config_nettle.h"
#ifndef TARGET_OS_ANDROID
#ifndef BUILD_NETTLE_GENERIC

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "nettle-types.h"

#include "nettle_aes-internal.h"
#include "nettle_memxor.h"
#include "nettle_fat-setup.h"

void _nettle_cpuid (uint32_t input, uint32_t regs[4]);

struct x86_features
{
  enum x86_vendor { X86_OTHER, X86_INTEL, X86_AMD } vendor;
  int have_aesni;
};

#define SKIP(s, slen, literal, llen)				\
  (((slen) >= (llen) && memcmp ((s), (literal), llen) == 0)	\
   ? ((slen) -= (llen), (s) += (llen), 1) : 0)
#define MATCH(s, slen, literal, llen)				\
  ((slen) == (llen) && memcmp ((s), (literal), llen) == 0)

static void
get_x86_features (struct x86_features *features)
{
  const char *s;
  features->vendor = X86_OTHER;
  features->have_aesni = 0;

  s = secure_getenv (ENV_OVERRIDE);
  if (s)
    for (;;)
      {
	const char *sep = strchr (s, ',');
	size_t length = sep ? (size_t) (sep - s) : strlen(s);

	if (SKIP (s, length, "vendor:", 7))
	  {
	    if (MATCH(s, length, "intel", 5))
	      features->vendor = X86_INTEL;
	    else if (MATCH(s, length, "amd", 3))
	      features->vendor = X86_AMD;
	    
	  }
	else if (MATCH (s, length, "aesni", 5))
	  features->have_aesni = 1;
	if (!sep)
	  break;
	s = sep + 1;	
      }
  else
    {
      uint32_t cpuid_data[4];
      _nettle_cpuid (0, cpuid_data);
      if (memcmp (cpuid_data + 1, "Genu" "ntel" "ineI", 12) == 0)
	features->vendor = X86_INTEL;
      else if (memcmp (cpuid_data + 1, "Auth" "cAMD" "enti", 12) == 0)
	features->vendor = X86_AMD;

      _nettle_cpuid (1, cpuid_data);
      if (cpuid_data[2] & 0x02000000)
	features->have_aesni = 1;      
    }
}

//DECLARE_FAT_FUNC(_nettle_aes_encrypt, aes_crypt_internal_func)
aes_crypt_internal_func _nettle_aes_encrypt;
static aes_crypt_internal_func _nettle_aes_encrypt_init;
static aes_crypt_internal_func * _nettle_aes_encrypt_vec = _nettle_aes_encrypt_init;

//DECLARE_FAT_FUNC_VAR(nettle_aes_encrypt, aes_crypt_internal_func, x86_64)
aes_crypt_internal_func _nettle_aes_encrypt_x86_64;
//DECLARE_FAT_FUNC_VAR(nettle_aes_encrypt, aes_crypt_internal_func, aesni)
aes_crypt_internal_func _nettle_aes_encrypt_aesni;

//DECLARE_FAT_FUNC(_nettle_aes_decrypt, aes_crypt_internal_func)
aes_crypt_internal_func _nettle_aes_decrypt;
static aes_crypt_internal_func _nettle_aes_decrypt_init;
static aes_crypt_internal_func * _nettle_aes_decrypt_vec = _nettle_aes_decrypt_init;

//DECLARE_FAT_FUNC_VAR(nettle_aes_decrypt, aes_crypt_internal_func, x86_64)
aes_crypt_internal_func _nettle_aes_decrypt_x86_64;
//DECLARE_FAT_FUNC_VAR(nettle_aes_decrypt, aes_crypt_internal_func, aesni)
aes_crypt_internal_func _nettle_aes_decrypt_aesni;

//DECLARE_FAT_FUNC(nettle_memxor, memxor_func)
memxor_func nettle_memxor;
static memxor_func nettle_memxor_init;
static memxor_func * nettle_memxor_vec = nettle_memxor_init;

//DECLARE_FAT_FUNC_VAR(_nettle_memxor, memxor_func, x86_64)
memxor_func _nettle_memxor_x86_64;
//DECLARE_FAT_FUNC_VAR(_nettle_memxor, memxor_func, sse2)
memxor_func _nettle_memxor_sse2;


/* This function should usually be called only once, at startup. But
   it is idempotent, and on x86, pointer updates are atomic, so
   there's no danger if it is called simultaneously from multiple
   threads. */
static void CONSTRUCTOR
nettle_fat_init (void)
{
  struct x86_features features;
  int verbose;

  /* FIXME: Replace all getenv calls by getenv_secure? */
  verbose = getenv (ENV_VERBOSE) != NULL;
  if (verbose)
    fprintf (stderr, "libnettle: fat library initialization.\n");

  get_x86_features (&features);
  if (verbose)
    {
      const char * const vendor_names[3] =
	{ "other", "intel", "amd" };
      fprintf (stderr, "libnettle: cpu features: vendor:%s%s\n",
	       vendor_names[features.vendor],
	       features.have_aesni ? ",aesni" : "");
    }
  if (features.have_aesni)
    {
      if (verbose)
	fprintf (stderr, "libnettle: using aes instructions.\n");
      _nettle_aes_encrypt_vec = _nettle_aes_encrypt_aesni;
      _nettle_aes_decrypt_vec = _nettle_aes_decrypt_aesni;
    }
  else
    {
      if (verbose)
	fprintf (stderr, "libnettle: not using aes instructions.\n");
      _nettle_aes_encrypt_vec = _nettle_aes_encrypt_x86_64;
      _nettle_aes_decrypt_vec = _nettle_aes_decrypt_x86_64;
    }

  if (features.vendor == X86_INTEL)
    {
      if (verbose)
	fprintf (stderr, "libnettle: intel SSE2 will be used for _nettle_memxor.\n");
      nettle_memxor_vec = _nettle_memxor_sse2;
    }
  else
    {
      if (verbose)
	fprintf (stderr, "libnettle: intel SSE2 will not be used for _nettle_memxor.\n");
      nettle_memxor_vec = _nettle_memxor_x86_64;
    }
}

//#define DEFINE_FAT_FUNC(name, rtype, prototype, args)		\
//  rtype name prototype						\
//  {								\
//    return name##_vec args;					\
//  }								\
//  static rtype name##_init prototype {				\
//    if (getenv (ENV_VERBOSE))					\
//      fprintf (stderr, "libnettle: "#name"_init\n");		\
//    if (name##_vec == name##_init)				\
//      nettle_fat_init();						\
//    assert (name##_vec != name##_init);				\
//    return name##_vec args;					\
//  }

//DEFINE_FAT_FUNC( _nettle_aes_encrypt, void,
//( unsigned rounds, const uint32_t *keys,
//  const struct aes_table *T,
//  size_t length, uint8_t *dst,
//  const uint8_t *src ),
//  ( rounds, keys, T, length, dst, src ) )

void _nettle_aes_encrypt( unsigned rounds, const uint32_t *keys, const struct aes_table *T, size_t length, uint8_t *dst, const uint8_t *src )
{
	_nettle_aes_encrypt_vec( rounds, keys, T, length, dst, src );
}

void _nettle_aes_encrypt_init( unsigned rounds, const uint32_t *keys, const struct aes_table *T, size_t length, uint8_t *dst, const uint8_t *src )
{
	if( _nettle_aes_encrypt_vec == _nettle_aes_encrypt_init )
	{
		nettle_fat_init();
	}

	//vx_assert( _nettle_aes_encrypt_vec != _nettle_aes_encrypt_init  );
	_nettle_aes_encrypt_vec( rounds, keys, T, length, dst, src );
}

	//DEFINE_FAT_FUNC( _nettle_aes_decrypt, void,
	//( unsigned rounds, const uint32_t *keys,
	//  const struct aes_table *T,
	//  size_t length, uint8_t *dst,
	//  const uint8_t *src ),
	//  ( rounds, keys, T, length, dst, src ) )
void _nettle_aes_decrypt( unsigned rounds, const uint32_t *keys, const struct aes_table *T, size_t length, uint8_t *dst, const uint8_t *src )
{
	_nettle_aes_decrypt_vec( rounds, keys, T, length, dst, src );
}

void _nettle_aes_decrypt_init( unsigned rounds, const uint32_t *keys, const struct aes_table *T, size_t length, uint8_t *dst, const uint8_t *src )
{
	if( _nettle_aes_decrypt_vec == _nettle_aes_decrypt_init )
	{
		nettle_fat_init();
	}

	//vx_assert( _nettle_aes_decrypt_vec != _nettle_aes_decrypt_init  );
	_nettle_aes_decrypt_vec( rounds, keys, T, length, dst, src );
}

	//DEFINE_FAT_FUNC( nettle_memxor, void *,
	//( void *dst, const void *src, size_t n ),
	//				 ( dst, src, n ) )

void * nettle_memxor( void *dst, const void *src, size_t n )
{
	return nettle_memxor_vec( dst, src, n );
}

void * nettle_memxor_init( void *dst, const void *src, size_t n )
{
	if( nettle_memxor_vec == nettle_memxor_init )
	{
		nettle_fat_init();
	}

	//vx_assert( _nettle_aes_decrypt_vec != _nettle_aes_decrypt_init  );
	return nettle_memxor_vec( dst, src, n );
}

#endif //#ifndef BUILD_NETTLE_GENERIC
#endif //#ifndef TARGET_OS_ANDROID

