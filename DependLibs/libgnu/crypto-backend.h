/*
 * Copyright (C) 2011-2012 Free Software Foundation, Inc.
 *
 * Author: Nikos Mavrogiannopoulos
 *
 * This file is part of GnuTLS.
 *
 * The GnuTLS is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 *
 */

#ifndef GNUTLS_CRYPTO_BACKEND_H
#define GNUTLS_CRYPTO_BACKEND_H

#include <libgnu/abstract_gnu.h>

GOTV_BEGIN_CDECLARES
int gnutls_crypto_bigint_register( int priority,
								   gnutls_crypto_bigint_st * s );
GOTV_END_CDECLARES

#include <libgnu/crypto.h>

#define gnutls_crypto_single_cipher_st gnutls_crypto_cipher_st
#define gnutls_crypto_single_mac_st gnutls_crypto_mac_st
#define gnutls_crypto_single_digest_st gnutls_crypto_digest_st


typedef struct {
	gnutls_cipher_init_func init;
	gnutls_cipher_setkey_func setkey;
	gnutls_cipher_setiv_func setiv;
	gnutls_cipher_encrypt_func encrypt;
	gnutls_cipher_decrypt_func decrypt;
	gnutls_cipher_aead_encrypt_func aead_encrypt;
	gnutls_cipher_aead_decrypt_func aead_decrypt;
	gnutls_cipher_deinit_func deinit;
	gnutls_cipher_auth_func auth;
	gnutls_cipher_tag_func tag;

	/* Not needed for registered on run-time. Only included
	 * should define it. */
	int (*exists) (gnutls_cipher_algorithm_t);	/* true/false */
} gnutls_crypto_cipher_st;

typedef struct {
	gnutls_mac_init_func init;
	gnutls_mac_setkey_func setkey;
	gnutls_mac_setnonce_func setnonce;
	gnutls_mac_hash_func hash;
	gnutls_mac_output_func output;
	gnutls_mac_deinit_func deinit;
	gnutls_mac_fast_func fast;

	/* Not needed for registered on run-time. Only included
	 * should define it. */
	int (*exists) (gnutls_mac_algorithm_t);
} gnutls_crypto_mac_st;

typedef struct {
	gnutls_digest_init_func init;
	gnutls_digest_hash_func hash;
	gnutls_digest_output_func output;
	gnutls_digest_deinit_func deinit;
	gnutls_digest_fast_func fast;

	/* Not needed for registered on run-time. Only included
	 * should define it. */
	int (*exists) (gnutls_digest_algorithm_t);
} gnutls_crypto_digest_st;

typedef struct gnutls_crypto_rnd {
	int (*init) (void **ctx);
	int (*check) (void **ctx);
	int (*rnd) (void *ctx, int level, void *data, size_t datasize);
	void (*rnd_refresh) (void *ctx);
	void (*deinit) (void *ctx);
	int (*self_test) (void);
} gnutls_crypto_rnd_st;


#define MAX_PUBLIC_PARAMS_SIZE 4	/* ok for RSA and DSA */

/* parameters should not be larger than this limit */
#define DSA_PUBLIC_PARAMS 4
#define DH_PUBLIC_PARAMS 4
#define RSA_PUBLIC_PARAMS 2
#define ECC_PUBLIC_PARAMS 2



/* parameters should not be larger than this limit */
#define DSA_PRIVATE_PARAMS 5
#define DH_PRIVATE_PARAMS 5
#define RSA_PRIVATE_PARAMS 8
#define ECC_PRIVATE_PARAMS 3

#if MAX_PRIV_PARAMS_SIZE - RSA_PRIVATE_PARAMS < 0
#error INCREASE MAX_PRIV_PARAMS
#endif

#if MAX_PRIV_PARAMS_SIZE - ECC_PRIVATE_PARAMS < 0
#error INCREASE MAX_PRIV_PARAMS
#endif

#if MAX_PRIV_PARAMS_SIZE - DSA_PRIVATE_PARAMS < 0
#error INCREASE MAX_PRIV_PARAMS
#endif


/* params are:
 * RSA:
 *  [0] is modulus
 *  [1] is public exponent
 *  [2] is private exponent (private key only)
 *  [3] is prime1 (p) (private key only)
 *  [4] is prime2 (q) (private key only)
 *  [5] is coefficient (u == inverse of p mod q) (private key only)
 *  [6] e1 == d mod (p-1)
 *  [7] e2 == d mod (q-1)
 *
 *  note that for libgcrypt that does not use the inverse of q mod p,
 *  we need to perform conversions using fixup_params().
 *
 * DSA:
 *  [0] is p
 *  [1] is q
 *  [2] is g
 *  [3] is y (public key)
 *  [4] is x (private key only)
 *
 * DH: as DSA
 *
 * ECC:
 *  [0] is prime
 *  [1] is order
 *  [2] is A
 *  [3] is B
 *  [4] is Gx
 *  [5] is Gy
 *  [6] is x
 *  [7] is y
 *  [8] is k (private key)
 */

#define ECC_X 0
#define ECC_Y 1
#define ECC_K 2

#define DSA_P 0
#define DSA_Q 1
#define DSA_G 2
#define DSA_Y 3
#define DSA_X 4

#define DH_P 0
#define DH_Q 1
#define DH_G 2
#define DH_Y 3
#define DH_X 4

#define RSA_MODULUS 0
#define RSA_PUB 1
#define RSA_PRIV 2
#define RSA_PRIME1 3
#define RSA_PRIME2 4
#define RSA_COEF 5
#define RSA_E1 6
#define RSA_E2 7

GOTV_BEGIN_CDECLARES

/**
 * gnutls_direction_t:
 * @GNUTLS_IMPORT: Import direction.
 * @GNUTLS_EXPORT: Export direction.
 *
 * Enumeration of different directions.
 */
typedef enum {
	GNUTLS_IMPORT = 0,
	GNUTLS_EXPORT = 1
} gnutls_direction_t;

/* Public key algorithms */
typedef struct gnutls_crypto_pk {
	/* The params structure should contain the private or public key
	 * parameters, depending on the operation */
	int (*encrypt) (gnutls_pk_algorithm_t, gnutls_datum_t * ciphertext,
			const gnutls_datum_t * plaintext,
			const gnutls_pk_params_st * pub);
	int (*decrypt) (gnutls_pk_algorithm_t, gnutls_datum_t * plaintext,
			const gnutls_datum_t * ciphertext,
			const gnutls_pk_params_st * priv);

	int (*sign) (gnutls_pk_algorithm_t, gnutls_datum_t * signature,
		     const gnutls_datum_t * data,
		     const gnutls_pk_params_st * priv);
	int (*verify) (gnutls_pk_algorithm_t, const gnutls_datum_t * data,
		       const gnutls_datum_t * sig,
		       const gnutls_pk_params_st * pub);
	/* sanity checks the public key parameters */
	int (*verify_priv_params) (gnutls_pk_algorithm_t,
			      const gnutls_pk_params_st * priv);
	int (*verify_pub_params) (gnutls_pk_algorithm_t,
			      const gnutls_pk_params_st * pub);
	int (*generate_keys) (gnutls_pk_algorithm_t, unsigned int nbits,
			 gnutls_pk_params_st *);
	int (*generate_params) (gnutls_pk_algorithm_t, unsigned int nbits,
			 gnutls_pk_params_st *);
	/* this function should convert params to ones suitable
	 * for the above functions
	 */
	int (*pk_fixup_private_params) (gnutls_pk_algorithm_t,
					gnutls_direction_t,
					gnutls_pk_params_st *);
	int (*derive) (gnutls_pk_algorithm_t, gnutls_datum_t * out,
		       const gnutls_pk_params_st * priv,
		       const gnutls_pk_params_st * pub);

	int (*curve_exists) (gnutls_ecc_curve_t);	/* true/false */
} gnutls_crypto_pk_st;

/* priority: infinity for backend algorithms, 90 for kernel
   algorithms, lowest wins
 */
int gnutls_crypto_single_cipher_register(gnutls_cipher_algorithm_t
					 algorithm, int priority,
					 const gnutls_crypto_single_cipher_st *s,
					 int free_s);
int gnutls_crypto_single_mac_register(gnutls_mac_algorithm_t algorithm,
				      int priority,
				      const gnutls_crypto_single_mac_st *
				      s, int free_s);
int gnutls_crypto_single_digest_register(gnutls_digest_algorithm_t
					 algorithm, int priority,
					 const
					 gnutls_crypto_single_digest_st *
					 s, int free_s);

int gnutls_crypto_rnd_register(int priority,
			       const gnutls_crypto_rnd_st * s);
int gnutls_crypto_pk_register(int priority, const gnutls_crypto_pk_st * s);

GOTV_END_CDECLARES

#endif
